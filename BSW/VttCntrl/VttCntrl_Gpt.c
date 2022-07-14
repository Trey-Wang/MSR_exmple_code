/**********************************************************************************************************************
*  COPYRIGHT
*  -------------------------------------------------------------------------------------------------------------------
*  \verbatim
*  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
*
*                This software is copyright protected and proprietary to Vector Informatik GmbH.
*                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
*                All other rights remain with Vector Informatik GmbH.
*  \endverbatim
*  -------------------------------------------------------------------------------------------------------------------
*  FILE DESCRIPTION
*  -----------------------------------------------------------------------------------------------------------------*/
/**      \file    VTTCntrl_Gpt.h
*        \brief   Implements the public API of the virtual GPT controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual GPT controller that enables to setup general purpose timers in continuous or one shot mode.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the module's header file.
*
*  FILE VERSION
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the VERSION CHECK below.
*********************************************************************************************************************/

#define VTTGPT_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Gpt.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the GPT controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Gpt header file. */
#if (  (VTTGPT_SW_MAJOR_VERSION != (4u)) \
    || (VTTGPT_SW_MINOR_VERSION != (0u)) \
    || (VTTGPT_SW_PATCH_VERSION != (2u)) )
# error "Vendor specific version numbers of VttCntrl_Gpt.c and VttCntrl_Gpt.h are inconsistent"
#endif

/* Check the version of the configuration header file. */
#if (  (VTTGPT_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTGPT_CFG_MINOR_VERSION != (0x01u)) \
    || (VTTGPT_CFG_PATCH_VERSION != (0x02u)))
  #error "Version numbers of VttCntrl_Gpt.c and VttCntrl_Gpt_Cfg.h are inconsistent!"
#endif

/* Check the version of VttCntrl_Base header file.  */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
#error "Vendor specific version numbers of VttCntrl_Gpt.c and VttCntrl_Base.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

#define VttGpt_TimerHandle(index)   (VttGpt_Cntrl[index].handle)
#define VttGpt_TimerName(index)     (VttGpt_Cntrl[index].name)
#define VttGpt_IRQ(index)           (VttGpt_Cntrl[index].irq)

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
STATIC uint8 VttGpt_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Internal GPT controller state of each channel. */
extern VttGpt_CntrlType VttGpt_Cntrl[VTTGPT_CHANNELS];

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttGpt_OnTimeout(uint8 index, const char *name);

STATIC void VttGpt_AssignIRQ(void);
STATIC sint32 VttGpt_GetIRQbyTimerIndex(uint8 index);

STATIC void VttGpt_InitTimers(void);
STATIC void VttGpt_DeinitTimers(void);
STATIC void VttGpt_Deinit(void);

STATIC void VttGpt_OnLoad(void);
STATIC void VttGpt_OnStartMeasurement(void);
STATIC void VttGpt_OnReset(void);
STATIC void VttGpt_OnSwitchOff(void);
STATIC void VttGpt_OnStopMeasurement(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttGpt_OnTimeout
**********************************************************************************************************************/
/*! \brief      Time-out handler that gets called when a GPT channel time-out occurs.
*
*  \param[in]   index   CANoe user timer index associated with the GPT channel.
*  \param[in]   name    Name of the CANoe user timer that implements the GPT channel.
*
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttGpt_OnTimeout(uint8 index, const char *name)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttGpt_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    VttCntrl_Base_SetCheckedInterrupt(VttGpt_GetIRQbyTimerIndex(index));
  }
  else if (VttGpt_EcuState == CANOEAPI_ECUSTATE_SLEEPING)
  {
    VttCntrl_Base_SetCheckedInterrupt(VttGpt_GetIRQbyTimerIndex(index));
    CANoeAPI_WakeupProcessor();
  }

} /* VttGpt_OnTimeout() */

/***********************************************************************************************************************
*  VttGpt_Deinit
**********************************************************************************************************************/
/*! \brief     De-initializes the internal GPT controller state of each channel.
*  \context    Called by VttGpt_OnLoad and VttGpt_OnStopMeasurement.
**********************************************************************************************************************/
STATIC void VttGpt_Deinit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    VttGpt_TimerHandle(i) = CANOEAPI_USERTIMER_INVALID;
  }

} /* VttGpt_Deinit() */

/***********************************************************************************************************************
*  VttGpt_InitTimers
**********************************************************************************************************************/
/*! \brief     Allocates and initializes the CANoe user timers that implement the configured GPT channels.
*  \context    Called by VttGpt_OnStartMeasurement.
*  \note       Array of timer indices must be stored in equal order by VttCntrl_Gpt and VttGpt, otherwise VttGpt_Start
*              and VttGpt_Stop are applied to the wrong GPT channels. Must be guaranteed by generators!
**********************************************************************************************************************/
STATIC void VttGpt_InitTimers(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    VttGpt_TimerHandle(i) = CANoeAPI_UserTimer_Create(VttGpt_TimerName(i));
    if (VttGpt_TimerHandle(i) == CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_Printf("VTT GPT Controller: Timer for channel %d could not be created!", i);
    }
    else
    {
      CANoeAPI_UserTimer_SetCallbackFunction(VttGpt_TimerHandle(i), &VttGpt_OnTimeout);
    }
  }

} /* VttGpt_InitTimers() */

/***********************************************************************************************************************
*  VttGpt_DeinitTimers
**********************************************************************************************************************/
/*! \brief     Frees the allocated CANoe user timers that implement the GPT channels.
*  \context    Called by VttGpt_OnStopMeasurement.
*  \note       Array of timer indices must be stored in equal order by VttCntrl_Gpt and VttGpt, otherwise VttGpt_Start
*              and VttGpt_Stop are applied to the wrong GPT channels. Must be guaranteed by generators!
**********************************************************************************************************************/
STATIC void VttGpt_DeinitTimers(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    if (VttGpt_TimerHandle(i) != CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_UserTimer_Destroy(VttGpt_TimerHandle(i));
      VttGpt_TimerHandle(i) = CANOEAPI_USERTIMER_INVALID;
    }
  }

} /* VttGpt_DeinitTimers() */

/***********************************************************************************************************************
*  VttGpt_GetIRQbyTimerIndex
**********************************************************************************************************************/
/*! \brief      Translates a CANoe user timer index associated with a GPT channel to the assigned IRQ number.
*  \param[in]   index    CANoe user timer index associated with the GPT channel.
*  \return      IRQ number associated with CANoe user timer index.
*  \called      Called by CANoeEmu when executing handler VttGpt_OnTimeout. 
**********************************************************************************************************************/
STATIC sint32 VttGpt_GetIRQbyTimerIndex(uint8 index)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    if (VttGpt_TimerHandle(i) == index)
    {
      return VttGpt_IRQ(i);
    }
  }

  return -1;

} /* VttGpt_GetIRQbyTimerIndex() */

/***********************************************************************************************************************
*  VttGpt_AssignIRQ
**********************************************************************************************************************/
/*! \brief    Assigns each GPT channel's controller an IRQ to trigger the corresponding time-out handler.
*   \context  Called by VttGpt_OnLoad.
**********************************************************************************************************************/
STATIC void VttGpt_AssignIRQ(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    VttGpt_IRQ(i) = IRQ_GptIsr_0 + i;
  }

} /* VttGpt_AssignIRQ() */

/***********************************************************************************************************************
*  VttGpt_OnLoad
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual GPT controller.
*
*         Tasks:
*            - De-initializes all GPT channel's internal controller state.
*            - Assign each GPT channel's controller an IRQ for channel time-out handling.
*
*  \context    Called by VttGpt_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttGpt_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttGpt_Deinit();
  VttGpt_AssignIRQ();

} /* VttGpt_OnLoad() */

/***********************************************************************************************************************
*  VttGpt_OnReset
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual GPT controller.
*
*         Tasks:
*            - Stop the CANoe user timers that implement the GPT channels.
*
*  \context    Called by VttGpt_OnStateChange handler right before ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void VttGpt_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    if (VttGpt_TimerHandle(i) != CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_UserTimer_Cancel(VttGpt_TimerHandle(i));
    }
  }

} /* VttGpt_OnReset() */

/***********************************************************************************************************************
*  VttGpt_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual GPT controller.
*
*         Tasks:
*            - Initializes the CANoe user timers that implement the GPT channels.
*
*  \context    Called by VttGpt_OnStateChange handler right before simulation is started.
**********************************************************************************************************************/
STATIC void VttGpt_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttGpt_InitTimers();

} /* VttGpt_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttGpt_OnStopMeasurement
**********************************************************************************************************************/
/*! \brief     OnStopMeasurement callout of virtual GPT controller.
*
*         Tasks:
*            - De-initializes the CANoe user timers that implement the GPT channels.
*            - De-initializes all GPT channel's internal controller state.
*
*  \context    Called by VttGpt_OnStateChange handler right before simulation is stopped.
**********************************************************************************************************************/
STATIC void VttGpt_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttGpt_DeinitTimers();
  VttGpt_Deinit();

} /* VttGpt_OnStopMeasurement() */

/***********************************************************************************************************************
*  VttGpt_OnSwitchOff
**********************************************************************************************************************/
/*! \brief     OnSwitchOff callout of virtual GPT controller.
*
*         Tasks:
*            - Stop the CANoe user timers that implement the GPT channels.
*
*  \context    Called by VttGpt_OnStateChange handler right before the power supply of the ECU is switched off.
**********************************************************************************************************************/
STATIC void VttGpt_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for (i = 0; i < VTTGPT_CHANNELS; i++)
  {
    if (VttGpt_TimerHandle(i) != CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_UserTimer_Cancel(VttGpt_TimerHandle(i));
    }
  }

} /* VttGpt_OnSwitchOff() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttGpt_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual GPT controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttGpt_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttGpt_EcuState = newState;

  switch (action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttGpt_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttGpt_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttGpt_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;
  case CANOEAPI_ECUACTION_WAKEUP:
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    break;
  case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttGpt_OnSwitchOff();
    }
    break;
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttGpt_OnStopMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }

} /* VttGpt_OnStateChange() */

/***********************************************************************************************************************
*  VttGpt_StartTimer
**********************************************************************************************************************/
/*! \brief     Starts the general purpose timer associated with a certain channel.
*
*  \param[in]   index     Index of the GPT channel.
*               timeout   Relative time (or period) the next time-out of the GPT channel's timer shall occur in
*                         microseconds [us].
*               mode      VTTGPT_CH_MODE_ONESHOT: Relative timer.
*                         VTTGPT_CH_MODE_CONTINUOUS: Cyclic timer.
*
*  \context    Called by GPT driver.
**********************************************************************************************************************/
void VttGpt_StartTimer(uint8 index, uint32 timeout, VttGpt_ChannelModeType mode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttGpt_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if ((VttGpt_TimerHandle(index) != CANOEAPI_USERTIMER_INVALID) && (mode != VTTGPT_CH_MODE_NONE))
    {
      if (mode == VTTGPT_CH_MODE_ONESHOT)
      {
        CANoeAPI_UserTimer_SetRelative(VttGpt_TimerHandle(index), timeout);
      }
      else if (mode == VTTGPT_CH_MODE_CONTINUOUS)
      {
        CANoeAPI_UserTimer_SetCyclic(VttGpt_TimerHandle(index), timeout);
      }
    }
  }

} /* VttGpt_StartTimer() */

/***********************************************************************************************************************
*  VttGpt_StopTimer
**********************************************************************************************************************/
/*! \brief     Stops the general purpose timer associated with a certain channel.
*  \param[in]  index      Index of the GPT channel.
*  \context    Called by GPT driver.
**********************************************************************************************************************/
void VttGpt_StopTimer(uint8 index)
{  
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(VttGpt_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if (VttGpt_TimerHandle(index) != CANOEAPI_USERTIMER_INVALID)
    {
      CANoeAPI_UserTimer_Cancel(VttGpt_TimerHandle(index));
    }    
  }

} /* VttGpt_StopTimer() */

/***********************************************************************************************************************
*  VttGpt_GetRemainingTime
**********************************************************************************************************************/
/*! \brief      Returns the remaining time until the GPT channel's timer will trigger a time-out.
*  \param[in]   index      Index of the GPT channel.
*  \return      Zero, if the GPT channel index is invalid or the associated is timer is not running.
*               Remaining time in microseconds [us], otherwise.
**********************************************************************************************************************/
uint32 VttGpt_GetRemainingTime(uint8 index)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttGpt_TimerHandle(index) != CANOEAPI_USERTIMER_INVALID)
  {
    return CANoeAPI_UserTimer_GetRemaining(VttGpt_TimerHandle(index));
  }
  else
  {
    return 0;
  }

} /* VttGpt_GetRemainingTime() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Gpt.c
***********************************************************************************************************************/
