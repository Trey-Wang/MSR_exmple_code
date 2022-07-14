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
/**        \file  VTTCntrl_Wdg.c
 *        \brief  Implements the public API of the virtual CAN controller integrated in the vVIRTUALtarget.
 *
 *      \details  Virtual Watchdog controller that enables to monitor the liveness of the MCU.
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

#define VTTWDG_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Wdg.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the Watchdog controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Wdg header file. */
#if (  (VTTWDG_SW_MAJOR_VERSION != (4u)) \
    || (VTTWDG_SW_MINOR_VERSION != (0u)) \
    || (VTTWDG_SW_PATCH_VERSION != (1u)) )
  #error "Vendor specific version numbers of VttCntrl_Wdg.c and VttCntrl_Wdg.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Current hardware period in microseconds [us] of the watchdog. */
STATIC uint32 HWPeriod = 0;
/*! \brief Current trigger window in microseconds [us] of the watchdog. */
STATIC uint32 TriggerWindow = 0;
/*! \brief Timestamp in microseconds [us] when the last time the trigger window was set of the watchdog. */
STATIC uint32 LastTimeTriggerWindowSet = 0;
/*! \brief Timestamp in microseconds [us] when the last time the hardware period was set of the watchdog. */
STATIC uint32 LastTimeHWPeriodSet = 0;
/*! \brief CANoe user timer index identified with watchdog. */
STATIC uint8 ResetTimerIndex = CANOEAPI_USERTIMER_INVALID;

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttWdg_ComputeResetTimeout(boolean HWPeriodUpdated);
STATIC void VttWdg_OnTimeout(uint8 index, const char* name);

STATIC void VttWdg_InvokeReset(void);
STATIC void VttWdg_Deinit(void);

STATIC void VttWdg_OnLoad(void);
STATIC void VttWdg_OnStartMeasurement(void);
STATIC void VttWdg_OnReset(void);
STATIC void VttWdg_OnStopMeasurement(void);
STATIC void VttWdg_OnSwitchOff(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttWdg_ComputeResetTimeout
**********************************************************************************************************************/
/*! \brief     Computes the point in time the next reset is executed if trigger window is not refreshed in beforehand.
*
*  \param[in]  HWPeriodUpdated        TRUE iff HWPeriod was updated,
*                      FALSE iff TriggerWindow was updated.
*
*  \context     Called by VttWdg_Start and VttWdg_SetTriggerWindow.
**********************************************************************************************************************/
STATIC void VttWdg_ComputeResetTimeout(boolean HWPeriodUpdated)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 RemainingHWPeriod = 0;    /* microseconds */
  uint32 RemainingTriggerWindow = 0;  /* microseconds */
  uint32 ResetTimerTimeout = 0;    /* microseconds */

  uint32 CurrentTime = (uint32)(CANoeAPI_UserTimer_CurrentTime() / ((sint64)1000)); /* microseconds */

  /* ----- Implementation ----------------------------------------------- */

  /* Remember update time. */
  if (HWPeriodUpdated)
  {
    LastTimeHWPeriodSet = CurrentTime;
  }
  else
  {
    LastTimeTriggerWindowSet = CurrentTime;
  }

  /* Check if the trigger window has closed already. */
  if ((CurrentTime - LastTimeTriggerWindowSet) >= TriggerWindow)
  {
    /* Guard against an underflow in case the trigger windows has closed already */
    RemainingTriggerWindow = 0;
  }
  else
  {
    /* Compute the time until the trigger window closes. */
    RemainingTriggerWindow = TriggerWindow - (CurrentTime - LastTimeTriggerWindowSet);
  }

  if (HWPeriod == 0)
  {
    /* VTTWdg is not started. Reset at the end of the trigger window. */
    /* Also guards against later divisions by 0. */
    ResetTimerTimeout = RemainingTriggerWindow;
  }
  else
  {
    /* Compute the time until the current trigger period ends */
    RemainingHWPeriod = HWPeriod - ((CurrentTime - LastTimeHWPeriodSet) % HWPeriod);

    if (RemainingTriggerWindow == 0 || (RemainingTriggerWindow < RemainingHWPeriod))
    {
      /* Case 1: VTTWdg is running, but no trigger window has been set.
      *       Reset at the end of the current trigger period. */

      /* Case 2: VTTWdg is running and trigger windows has been set,
      *         but trigger window closes before the end of the trigger period
      *         (also guards against underflows in the else-branch). */
      
            if ((HWPeriodUpdated == FALSE) && (RemainingTriggerWindow == 0))
            {
               /* Immediate reset */
               VttWdg_InvokeReset();
            }
            else
            {
          ResetTimerTimeout = RemainingHWPeriod;
            }
    }
    else
    {
      /* VTTWdg is fully operational. Compute relative time until next reset may occur.
      * Note: Modulo operation is required to ensure correct operation when
      *     RemainingTriggerWindow is not a multiple of the HWPeriod.
      *
      *       If HWPeriod was updated (via VttWdg_Start), RemainingHWPeriod == HWPeriod:
      *      ResetTimerTimeout = RemainingTriggerWindow + (HWPeriod - ((RemainingTriggerWindow - HWPeriod) % HWPeriod))
      *
      *     If TriggerWindow was updated (via VttWdg_SetTriggerWindow), RemainingTriggerWindow == TriggerWindow:
      *      ResetTimerTimeout = TriggerWindow + (HWPeriod - ((TriggerWindow - RemainingHWPeriod) % HWPeriod))
      */
      ResetTimerTimeout = RemainingTriggerWindow +
        (HWPeriod - ((RemainingTriggerWindow - RemainingHWPeriod) % HWPeriod));
    }
  }

  CANoeAPI_UserTimer_SetRelative(ResetTimerIndex, ResetTimerTimeout);

}    /* VttWdg_ComputeResetTimeout() */

/***********************************************************************************************************************
*  VttWdg_OnTimeout
**********************************************************************************************************************/
/*! \brief    Watchdog time-out handler.
*   \details  Invokes a reset of the MCU.
*
*  \param[in]  index        CANoe timer index identified with the Watchdog reset timer.
*  \param[in]  name        Name of the Watchdog reset timer. (Ignored)
*  
*  \context  Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttWdg_OnTimeout(uint8 index, const char* name)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)name;

  /* ----- Implementation ----------------------------------------------- */
  if (index == ResetTimerIndex)
  {
    VttWdg_InvokeReset();
  }
  else
  {
    /* Unknown timer, hence, nothing to do.*/
  }

}    /* VttWdg_OnTimeout() */

/***********************************************************************************************************************
*  VttWdg_InvokeReset
********************************************************************************************************************** /
/*! \brief      Invokes a MCU reset.
*
*  \context    Called by VttWdg_OnTimeout when the watchdog fires.
**********************************************************************************************************************/
STATIC void VttWdg_InvokeReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_WriteString("VTT WDG Controller: Reset!");
  CANoeAPI_ResetProcessor();

}    /* VttWdg_InvokeReset() */

/***********************************************************************************************************************
*  VttWdg_Deinit
**********************************************************************************************************************/
/*! \brief     De-initializes the internal state of the virtual Watchdog EXCEPT the reset timer index.
*
*  \context     Called by VttWdg_OnLoad, VttWdg_OnStopMeasurement and VttWdg_Stop.
**********************************************************************************************************************/
STATIC void VttWdg_Deinit(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  HWPeriod = 0;
  TriggerWindow = 0;
  LastTimeHWPeriodSet = 0;
  LastTimeTriggerWindowSet = 0;

} /* VttWdg_Deinit() */

/***********************************************************************************************************************
*  VttWdg_OnLoad()
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual Watchdog controller.
*
*         Tasks:
*            - De-initializes the internal state of the Watchdog controller.
*            - De-initializes the Watchdog reset timer index.
*
*  \context    Called by VttWdg_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttWdg_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttWdg_Deinit();
  ResetTimerIndex = CANOEAPI_USERTIMER_INVALID;

} /* VttWdg_OnLoad() */

/***********************************************************************************************************************
*  VttWdg_OnStartMeasurement()
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual Watchdog controller.
*
*         Tasks:
*            - Allocates a user timer identified with the Watchdog reset timer.
*            - Registers the time-out callback at CANoeEmu that is triggered when the Watchdog fires.
*
*  \context    Called by VttWdg_OnStateChange handler immediately before simulation is started.
**********************************************************************************************************************/
STATIC void VttWdg_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  ResetTimerIndex = CANoeAPI_UserTimer_Create("WatchdogResetTimer");
  if (ResetTimerIndex == CANOEAPI_USERTIMER_INVALID)
  {
    CANoeAPI_WriteString("VTT WDG Controller: Reset timer could not be created!");
  }
  else
  {
    CANoeAPI_UserTimer_SetCallbackFunction(ResetTimerIndex, &VttWdg_OnTimeout);
  }  

} /* VttWdg_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttWdg_OnReset()
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual Watchdog controller.
*
*         Tasks:
*            - Stops the Watchdog controller.
*
*  \context    Called by VttWdgOnStateChange handler right before when ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void VttWdg_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttWdg_Stop();

} /* VttWdg_OnReset() */

/***********************************************************************************************************************
*  VttWdg_OnSwitchOff()
**********************************************************************************************************************/
/*! \brief     OnSwitchOff callout of virtual Watchdog controller.
*
*         Tasks:
*            - Stops the Watchdog controller.
*
*  \context    Called by VttWdgOnStateChange handler right before processor power supply is switched off.
**********************************************************************************************************************/
STATIC void VttWdg_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttWdg_Stop();

} /* VttWdg_OnSwitchOff() */

/***********************************************************************************************************************
*  VttWdg_OnStopMeasurement()
**********************************************************************************************************************/
/*! \brief     OnStopMeasurement callout of virtual Watchdog controller.
*
*         Tasks:
*            - Deallocates the Watchdog reset timer.
*            - De-initializes the internal state of the Watchdog controller.
*            - De-initializes the Watchdog reset timer index.
*
*  \context    Called by VttWdgOnStateChange handler right before simulation is stopped.
**********************************************************************************************************************/
STATIC void VttWdg_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (ResetTimerIndex != CANOEAPI_USERTIMER_INVALID)
  {
    CANoeAPI_UserTimer_Destroy(ResetTimerIndex);
  }

  VttWdg_Deinit();
  ResetTimerIndex = CANOEAPI_USERTIMER_INVALID;

} /* VttWdg_OnStopMeasurement() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttWdg_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual Watchdog controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttWdg_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;
  (void)newState;

  /* ----- Implementation ----------------------------------------------- */
  switch (action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttWdg_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttWdg_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttWdg_OnReset();
    }    
    break;
  case CANOEAPI_ECUACTION_WAKEUP:
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttWdg_OnSwitchOff();
    }
    break;
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttWdg_OnStopMeasurement();
    }
    break;
  }

} /* VttWdg_OnStateChange() */

/***********************************************************************************************************************
*  VttWdg_Start
**********************************************************************************************************************/
/*! \brief     Start the virtual Watchdog controller.
*  \param[in]  hwPeriod          Time-out period in nanoseconds [ns].
*  \context    Called by watchdog driver.
**********************************************************************************************************************/
void VttWdg_Start(uint32 hwPeriod)
{  
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  HWPeriod = hwPeriod;
  VttWdg_ComputeResetTimeout(TRUE);

} /* VttWdg_Start() */

/***********************************************************************************************************************
*  VttWdg_Stop
**********************************************************************************************************************/
/*! \brief     Stops the virtual Watchdog controller.
*  \context    Called by watchdog driver.
**********************************************************************************************************************/
void VttWdg_Stop(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (ResetTimerIndex != CANOEAPI_USERTIMER_INVALID)
  {
    CANoeAPI_UserTimer_Cancel(ResetTimerIndex);
  }

  VttWdg_Deinit();

} /* VttWdg_Stop() */

/***********************************************************************************************************************
 *  VttWdg_TriggerWindow
 **********************************************************************************************************************/
/*! \brief     Set timing window in Watchdog controller.
*  \param[in]  period            Time-out value in milliseconds [ms].
*  \context    Called by watchdog driver.
**********************************************************************************************************************/
void VttWdg_SetTriggerWindow(uint16 timeout)
{  
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TriggerWindow = ((uint32)timeout) * 1000U; /* microseconds */
  VttWdg_ComputeResetTimeout(FALSE);

} /* VttWdg_SetTriggerWindow() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Wdg.c
*********************************************************************************************************************/
