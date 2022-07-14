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
/**        \file  VTTCntrl_Pwm.c
 *      \brief  Implements the public API of the virtual PWM controller integrated in the vVIRTUALtarget.
 *
 *      \details  Virtual PWM controller that produces pulse-width-modulated signals with variable duty cycle.
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

# define VTTPWM_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Pwm.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the PWM controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Pwm header file */
#if (  (VTTPWM_SW_MAJOR_VERSION != (3u)) \
    || (VTTPWM_SW_MINOR_VERSION != (0u)) \
    || (VTTPWM_SW_PATCH_VERSION != (6u)) )
# error "Vendor specific version numbers of VttCntrl_Pwm.c and VttCntrl_Pwm.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VTTPWM_CFG_MAJOR_VERSION != (0x13u)) \
    || (VTTPWM_CFG_MINOR_VERSION != (0x00u)) \
    || (VTTPWM_CFG_PATCH_VERSION != (0x01u)))
#error "Version numbers of VttCntrl_Pwm.c and VttCntrl_Pwm_Cfg.h are inconsistent!"
#endif

/* Check the version of VttCntrl_Base header file */
#if (  (VTTCNTRL_SW_MAJOR_VERSION != (9u)) \
    || (VTTCNTRL_SW_MINOR_VERSION != (0u)) \
    || (VTTCNTRL_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_Pwm.c and VttCntrl_Base.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

#define MAX_FREQUENCY_SCALING ((VttPwm_Frequency_Type)4000u)

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

/*! \brief Internal ECU state. */
STATIC uint8 VttPwm_EcuState = CANOEAPI_ECUSTATE_INITIAL;
/*! \brief Internal PWM controller state of each channel. */
extern VttPwm_CntrlType VttPwm_Cntrl[VTTPWM_CHANNELS];

/*! \brief Internal structures for handling the CANoeEmu system variables that implement the PWM channels. */
extern CANoeAPI_SysVar_StructDefinition Pwm_Interfaces[];
extern CANoeAPI_SysVar_Struct Pwm_Channels[];

STATIC CANoeAPI_SysVar_StructMember_Scale Pwm_DutyCycle_Scalings[] =
{
  { 0, 0, (100.0 / ((double)(VTTPWM_DUTYCYCLE_MAXVAL))), 0, "%" },
};

STATIC CANoeAPI_SysVar_StructMember_Scale Pwm_Frequency_Scalings[] =
{
  { 0, 0, (1.0 / ((double)MAX_FREQUENCY_SCALING)), 0, "Hz" },
};

STATIC CANoeAPI_SysVar_ValueText Pwm_Polarity_Values[] =
{
  { 0, "PWM_LOW" },
  { 1, "PWM_HIGH" },
};

STATIC CANoeAPI_SysVar_ValueText Pwm_State_Values[] =
{
  { 1, "IDLE" },
  { 0, "RUNNING" },
};

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

CANoeAPI_SysVar_CompuMethod Pwm_State_CompuMethod =
{
  0,
  0,
  Pwm_State_Values,
  sizeof(Pwm_State_Values) / sizeof(Pwm_State_Values[0]),
};

CANoeAPI_SysVar_CompuMethod Pwm_DutyCycle_CompuMethod =
{
  Pwm_DutyCycle_Scalings,
  sizeof(Pwm_DutyCycle_Scalings) / sizeof(Pwm_DutyCycle_Scalings[0]),
  0,
  0,
};

CANoeAPI_SysVar_CompuMethod Pwm_Frequency_CompuMethod =
{
  Pwm_Frequency_Scalings,
  sizeof(Pwm_Frequency_Scalings) / sizeof(Pwm_Frequency_Scalings[0]),
  0,
  0,
};

CANoeAPI_SysVar_CompuMethod Pwm_Polarity_CompuMethod =
{
  0,
  0,
  Pwm_Polarity_Values,
  sizeof(Pwm_Polarity_Values) / sizeof(Pwm_Polarity_Values[0]),
};

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttPwm_TimerVarHandler(uint8 index, const char* name);
STATIC Pwm_PeriodType VttPwm_GetTimeInPeriod(Pwm_ChannelType channel);
LOCAL_INLINE uint32 VttPwm_DutyCycleInTime(Pwm_ChannelType channel);
LOCAL_INLINE long long VttPwm_GetPeriodNumber(Pwm_ChannelType channel);
STATIC void VttPwm_UpdateTimer(Pwm_ChannelType channel);
STATIC void VttPwm_UpdateDutyCycleImmediate(Pwm_ChannelType channel);
STATIC Pwm_OutputStateType VttPwm_GetState(Pwm_ChannelType channel);
STATIC void VttPwm_HandleChannelEdge(Pwm_ChannelType channel, Pwm_EdgeNotificationType edge);

STATIC void VttPwm_OnLoad(void);
STATIC void VttPwm_OnStartMeasurement(void);
STATIC void VttPwm_OnReset(void);
STATIC void VttPwm_MapChannel(void);
STATIC void VttPwm_AssignIRQ(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttPwm_TimerVarHandler
**********************************************************************************************************************/
/*! \brief     Sets physical value, duration and interrupt of a PWM channel
*  \param[in]  index           CANoeEmu user timer index associated with a PWM channel.
*  \param[in]  name            Name of CANoe user timer.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttPwm_TimerVarHandler(uint8 index, const char* name)
{
  /* ----- Local Variables ---------------------------------------------- */
  Pwm_OutputStateType chPhysState;
  long long periodNumber;
  long long now = CANoeAPI_UserTimer_CurrentTime() / 1000;
  Pwm_ChannelType channel = 0;
  int i;

  /* ----- Implementation ----------------------------------------------- */
  if( CANOEAPI_ECUSTATE_PROCESSING != VttPwm_EcuState && CANOEAPI_ECUSTATE_SLEEPING != VttPwm_EcuState )
  {
    return;
  }
  
  CANoeAPI_AtomicBegin();
  /* Search for PWM channel with corresponding Timer-ID */
  for( i = 0; i < VTTPWM_CHANNELS; ++i )
  {
    if( VttPwm_Cntrl[i].ChTimerHandle == index )
    {
      channel = (Pwm_ChannelType)i;
    }
  }
  
  /* Get the physical state of the pin to see which part of the period we are in. */
  periodNumber = VttPwm_GetPeriodNumber(channel);

  if((VttPwm_Cntrl[channel].ChNewPeriodNumber < periodNumber) && (VTTPWM_NEWDUTYCYCLE_NO_UPDATE > VttPwm_Cntrl[channel].ChNewDutyCycle))
  {
    VttPwm_Cntrl[channel].ChDutyCycle = VttPwm_Cntrl[channel].ChNewDutyCycle;
    VttPwm_Cntrl[channel].ChNewDutyCycle = VTTPWM_NEWDUTYCYCLE_NO_UPDATE;
    VttPwm_Cntrl[channel].ChOutputEnabled = VTTPWM_TRUE;

    if (VTTPWM_NEWPERIOD_NO_UPDATE > VttPwm_Cntrl[channel].ChNewPeriod) 
    {
      VttPwm_Cntrl[channel].ChPeriod = VttPwm_Cntrl[channel].ChNewPeriod;
      VttPwm_Cntrl[channel].ChNewPeriod = VTTPWM_NEWPERIOD_NO_UPDATE;
    }

    VttPwm_UpdateDutyCycleImmediate(channel);
    /* Note: This will also update the timer. */
  }
  else
  {
    /* See if the timer must be set again */
    VttPwm_UpdateTimer(channel);
  }

  /* Trigger the interrupt. */
  /* See if there is an edge to handle */
  if ((VttPwm_Cntrl[channel].ChPeriod > 0) && (VttPwm_Cntrl[channel].ChDutyCycle > 0) && (VttPwm_Cntrl[channel].ChDutyCycle < VTTPWM_DUTYCYCLE_MAXVAL))
  {
    chPhysState = VttPwm_GetState(channel);

    /* Determine type of edge depending on polarity.
     * chPhysState does not consider polarity, i.e., HIGH is duty-part, LOW is idle part of the period.
     * Resulting Logic: If polarity and returned physical state are equal, the edge is rising. */
    if (VttPwm_Cntrl[channel].ChPolarity == chPhysState)
    {
      VttPwm_HandleChannelEdge(channel, PWM_RISING_EDGE);
    }
    else
    {
      VttPwm_HandleChannelEdge(channel, PWM_FALLING_EDGE);
    }
  }

  CANoeAPI_AtomicEnd();
  
} /* VttPwm_TimerVarHandler() */

/***********************************************************************************************************************
*  VttPwm_GetTimeInPeriod
**********************************************************************************************************************/
/*! \brief     Determines the current time since the start of the last PWM period.
*
*  \param[in]  channel           The PWM channel to determine the period for.
*
*  \return     Returns the time in microseconds [us] since the start of the current period, or 0 if the period is 0, too.
*  \context    Called by VttPwm_GetState and VttPwm_UpdateTimer.
**********************************************************************************************************************/
STATIC Pwm_PeriodType VttPwm_GetTimeInPeriod(Pwm_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  long long currentTime = CANoeAPI_UserTimer_CurrentTime() / 1000;
  
  /* ----- Implementation ----------------------------------------------- */ 
  CANoeAPI_AtomicBegin(); 

  currentTime -= VttPwm_Cntrl[channel].ChStartTime;
  if (VttPwm_Cntrl[channel].ChPeriod == 0)
  {
    currentTime = 0;
  }
  else
  {
    currentTime %= VttPwm_Cntrl[channel].ChPeriod;
  }
    
  CANoeAPI_AtomicEnd();

  return (Pwm_PeriodType)currentTime;
  
} /* VttPwm_GetTimeInPeriod() */

/***********************************************************************************************************************
*  VttPwm_DutyCycleInTime
**********************************************************************************************************************/
/*! \brief     Determines the duration of the duty cycle in time units.
*
*  \param[in]  channel           The PWM channel to determine the period for.
*
*  \return     Returns the duration of the duty cycle in microseconds [us].
*  \context    Called by VttPwm_GetState and VttPwm_UpdateTimer.
**********************************************************************************************************************/
LOCAL_INLINE uint32 VttPwm_DutyCycleInTime(Pwm_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint64 value;
  
  /* ----- Implementation ----------------------------------------------- */ 
  CANoeAPI_AtomicBegin();
  value = (((uint64)VttPwm_Cntrl[channel].ChPeriod * VttPwm_Cntrl[channel].ChDutyCycle) >> 15);
  CANoeAPI_AtomicEnd();
  
  return (uint32)value;
  
} /* VttPwm_DutyCycleInTime() */

/***********************************************************************************************************************
*  VttPwm_GetPeriodNumber
**********************************************************************************************************************/
/*! \brief     Get the current period number.
*
*  \param[in]  channel           The channel to determine the period number for.
*
*  \return     Returns the period number, or 0 if the period is 0, too.
*  \context    Called by VttPwm_TimerVarHandler and VttPwm_UpdateDutycycleAndPeriod.
**********************************************************************************************************************/
LOCAL_INLINE long long VttPwm_GetPeriodNumber(Pwm_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  long long currentTime = CANoeAPI_UserTimer_CurrentTime() / 1000;
  long long periodNumber;
  
  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  
  currentTime -= VttPwm_Cntrl[channel].ChStartTime;  
  if (VttPwm_Cntrl[channel].ChPeriod == 0)
  {
    periodNumber = 0;
  }
  else
  {
    periodNumber = currentTime / VttPwm_Cntrl[channel].ChPeriod;
  }
  
  CANoeAPI_AtomicEnd();

  return periodNumber;
  
} /* VttPwm_GetPeriodNumber() */

/***********************************************************************************************************************
*  VttPwm_GetState
**********************************************************************************************************************/
/*! \brief     Determines the current physical state of the given channel
*  \param[in]  channel         PWM Channel
*  \return     no return 
*  \context    Function is called by VttVntrl_Pwm internally.
**********************************************************************************************************************/
STATIC Pwm_OutputStateType VttPwm_GetState(Pwm_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* Point in time where we are in the current duty cycle */
  long long currentTime = VttPwm_GetTimeInPeriod(channel);
  
  /* Duration of the duty cycle */  
  uint32 dutycycleTime = VttPwm_DutyCycleInTime(channel); 

  /* ----- Implementation ----------------------------------------------- */
  if (VttPwm_Cntrl[channel].ChDutyCycle <= 0)
  {
    /* 0% duty cycle */
    return PWM_LOW;
  }
  else if (VttPwm_Cntrl[channel].ChDutyCycle >= VTTPWM_DUTYCYCLE_MAXVAL)
  {
    /* 100% duty cycle */
    return PWM_HIGH;
  }
  else if (currentTime < dutycycleTime)
  {
    /* Channel is in state High - first part of duty cycle */
    return PWM_HIGH;
  }
  else
  {
    /* Channel is in state Low - second part of duty cycle */
    return PWM_LOW;
  }
  
} /* VttPwm_GetState() */

/***********************************************************************************************************************
*  VttPwm_UpdateDutyCycleImmediate
**********************************************************************************************************************/
/*! \brief     Updates duty cycle of the given channel
*  \param[in]  channel         PWM Channel
*  \param[out] channel         Physical state
*  \param[out] channel         Duration of the physical state
*  \return     no return 
*  \context    Function is called by VttCntrl_Pwm internally.
*  \note       -
**********************************************************************************************************************/
STATIC void VttPwm_UpdateDutyCycleImmediate(Pwm_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  VttPwm_Frequency_Type frequency;
  boolean idle;
  boolean polarity;
  VttPwm_Value_Type value;
  
  CANoeAPI_SysVar_StructMemberLocation locations[] =
  {
    {0, sizeof(frequency)},
    {0, sizeof(polarity)},
    {0, sizeof(idle)},
  };
  
  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();

  idle = (VttPwm_Cntrl[channel].ChOutputEnabled == VTTPWM_FALSE);
  polarity = (VttPwm_Cntrl[channel].ChPolarity == PWM_HIGH);

  if (VttPwm_Cntrl[channel].ChPeriod == 0) 
  {
    frequency = 0;
  }
  else
  {
    frequency = (MAX_FREQUENCY_SCALING * (VttPwm_Frequency_Type)1000000u) / VttPwm_Cntrl[channel].ChPeriod;
  }

  /* Send updates to CANoeEmu. */
  locations[0].address = (void*)&frequency;
  locations[1].address = (void*)&polarity;
  locations[2].address = (void*)&idle;
  
  /* Apply value function. */
  value = (VttPwm_Cntrl[channel].ChValueFunction)(VttPwm_Cntrl[channel].ChDutyCycle, VttPwm_Cntrl[channel].ChPeriod, 
    VttPwm_Cntrl[channel].ChOutputEnabled, VttPwm_Cntrl[channel].ChPolarity);

  CANoeAPI_SysVar_Struct_SetUint16(VttPwm_Cntrl[channel].ChSysVarHandle, 0, value);
  CANoeAPI_SysVar_Struct_SetUint16(VttPwm_Cntrl[channel].ChSysVarHandle, 1, VttPwm_Cntrl[channel].ChDutyCycle);
  CANoeAPI_SysVar_Struct_SetStruct_From_Locations(VttPwm_Cntrl[channel].ChSysVarHandle, 2, 
    locations, sizeof( locations) / sizeof( locations[0]));

  /* Note down start of new PWM cycle. */
  VttPwm_Cntrl[channel].ChStartTime = CANoeAPI_UserTimer_CurrentTime() / 1000;

  /* Update the PWM duty cycle timer. */
  VttPwm_UpdateTimer(channel);
  
  CANoeAPI_AtomicEnd();
  
} /* VttPwm_UpdateDutyCycleImmediate() */

/***********************************************************************************************************************
*  VttPwm_UpdateTimer
**********************************************************************************************************************/
/*! \brief     Sets the user timer for the given channel if notifications are enabled and the duty cycle is in (0, period).
*  \param[in]  channel         PWM Channel
*  \return     no return 
*  \context    Function is called by VttCntrl_Pwm internally.
*  \note       -
**********************************************************************************************************************/
STATIC void VttPwm_UpdateTimer(Pwm_ChannelType channel)
{ 
  /* ----- Local Variables ---------------------------------------------- */
  long long currentTime = 0;
  uint32 dutycycleTime = 0;
  uint32 microSecondsToNextTimerEvent = 0;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  
  if (0 == VttPwm_Cntrl[channel].ChPeriod)
  {
    /* PWM is disabled, turn off the timer. */
    CANoeAPI_UserTimer_Cancel(VttPwm_Cntrl[channel].ChTimerHandle);
  }
  else
  {
    /* Compute where we are in the duty cycle */
    currentTime = VttPwm_GetTimeInPeriod(channel);
    dutycycleTime = VttPwm_DutyCycleInTime(channel);

    if (VTTPWM_TRUE == VttPwm_Cntrl[channel].ChIntEnabled)
    {
      /* Notifications are enabled. Set timer to expire at next edge */
      /* Potential for optimization: Have the timer expire only on edges where the interrupt has to be triggered.
         Consider that there may be a pending update. In that case, the respective edge may not be skipped! */
      
      if (currentTime < dutycycleTime)
      {
        /* Channel is in state High - first part of duty cycle */
        /* Next edge is falling. */
        microSecondsToNextTimerEvent = dutycycleTime - ((uint32)currentTime);
      }
      else
      {
        /* Channel is in state Low - second part of duty cycle */
        /* Next edge is rising */
        microSecondsToNextTimerEvent = VttPwm_Cntrl[channel].ChPeriod - ((uint32)currentTime);
      }
      
      CANoeAPI_UserTimer_Cancel(VttPwm_Cntrl[channel].ChTimerHandle);
      CANoeAPI_UserTimer_SetRelative(VttPwm_Cntrl[channel].ChTimerHandle, microSecondsToNextTimerEvent);
    }
    else if (VTTPWM_NEWDUTYCYCLE_NO_UPDATE > VttPwm_Cntrl[channel].ChNewDutyCycle)
    {
      /* Notifications are disabled but there is a pending update. 
       * Set the timer to the end of the current period. */
      microSecondsToNextTimerEvent = VttPwm_Cntrl[channel].ChPeriod - ((uint32)currentTime);
      CANoeAPI_UserTimer_Cancel(VttPwm_Cntrl[channel].ChTimerHandle);
      CANoeAPI_UserTimer_SetRelative(VttPwm_Cntrl[channel].ChTimerHandle, microSecondsToNextTimerEvent);
    }
    else
    {
      /* Timer is to be disabled */ 
      CANoeAPI_UserTimer_Cancel(VttPwm_Cntrl[channel].ChTimerHandle);
    }
  }
  
  CANoeAPI_AtomicEnd();
  
} /* VttPwm_UpdateTimer() */


/***********************************************************************************************************************
*  VttPwm_HandleChannelEdge
**********************************************************************************************************************/
/*! \brief     Triggers interrupt/wakes up ECU depending on the PWM channels settings
*  \param[in]  channel         PWM Channel
*  \param[in]  edge            Current edge (Rising, Falling)
*  \return     no return 
*  \context    Function is called by VttVntrl_Pwm internally.
*  \note       -
**********************************************************************************************************************/
STATIC void VttPwm_HandleChannelEdge(Pwm_ChannelType channel, Pwm_EdgeNotificationType edge)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */  
  CANoeAPI_AtomicBegin();
  
  if (VTTPWM_TRUE == VttPwm_Cntrl[channel].ChIntEnabled)
  {
    if((VttPwm_Cntrl[channel].ChIntTriggerEdge == edge) || (PWM_BOTH_EDGES == VttPwm_Cntrl[channel].ChIntTriggerEdge))
    {
      VttCntrl_Base_SetCheckedInterrupt(VttPwm_Cntrl[channel].ChIsrNr);

      if( CANOEAPI_ECUSTATE_SLEEPING == VttPwm_EcuState )
      {
        CANoeAPI_WakeupProcessor();
      }
    }
  }
  
  CANoeAPI_AtomicEnd();
  
} /* VttPwm_HandleChannelEdge() */

/***********************************************************************************************************************
*  VttPwm_OnLoad
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual PWM controller.
*
*         Tasks:
*            - Assigns all PWM channels' controller a IRQ number.
*            - Initializes the CANoeEmu system variables that implements the PWM channels.
*
*  \context    Called by VttPwm_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttPwm_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  int i;

  /* ----- Implementation ----------------------------------------------- */
  for(i = 0; i < VTTPWM_CHANNELS; i++)
  {
    /* Define sysvar interface for each channel. */
    CANoeAPI_SysVar_DefineStruct("Pwm::Types", Pwm_Interfaces[i].name, Pwm_Interfaces[i].members, 
      Pwm_Interfaces[i].membercount);
      
    /* Define sysvar for each channel. */
    VttPwm_Cntrl[i].ChSysVarHandle = CANoeAPI_SysVar_DeclareStruct(Pwm_Channels[i].nameSpace, Pwm_Channels[i].name,
      Pwm_Channels[i].definition, Pwm_Channels[i].initValues, Pwm_Channels[i].count);

    VttPwm_Cntrl[i].ChIsrNr = IRQ_PwmIsr_0 + i;
  }

} /* VttPwm_OnLoad() */

/***********************************************************************************************************************
*  VttPwm_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     OnStartMeasurement callout of virtual PWM controller.
*
*         Tasks: 
*            - Initializes the CANoeEmu user timers that are used to implement the PWM duty cycle.
*
*  \context    Called by VttPwm_OnStateChange handler immediately before simulation is started.
**********************************************************************************************************************/
STATIC void VttPwm_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  int i;
  
  /* ----- Implementation ----------------------------------------------- */
  for(i = 0; i < VTTPWM_CHANNELS; i++)
  {
    VttPwm_Cntrl[i].ChTimerHandle = CANoeAPI_UserTimer_Create(VttPwm_Cntrl[i].ChName);
    CANoeAPI_UserTimer_SetCallbackFunction(VttPwm_Cntrl[i].ChTimerHandle, VttPwm_TimerVarHandler);
  }

} /* VttPwm_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttPwm_OnReset
**********************************************************************************************************************/
/*! \brief     OnReset callout of virtual PWM controller.
*
*         Tasks: 
*            - Stops all CANoeEmu user timers (needed to realize the PWM duty cycles).
*            - Stops the PWM signal generators of the channels.
*
*  \context    Called by VttPwm_OnStateChange handler right before ECU is going to reset itself.
**********************************************************************************************************************/
STATIC void VttPwm_OnReset(void)
{
   /* ----- Local Variables ---------------------------------------------- */
   int i;
   
  /* ----- Implementation ----------------------------------------------- */
  for( i=0; i < VTTPWM_CHANNELS; i++ )
  {
    CANoeAPI_UserTimer_Cancel(VttPwm_Cntrl[i].ChTimerHandle);
    VttPwm_StopPwmGenerator((Pwm_ChannelType)i, PWM_LOW);
  }
  
} /* VttPwm_OnReset() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttPwm_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual PWM controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttPwm_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
   VttPwm_EcuState = newState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    {
      VttPwm_OnLoad();
    }
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    {
      VttPwm_OnStartMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttPwm_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }

}  /* VttPwm_OnStateChange() */

/***********************************************************************************************************************
*  VttPwm_StartPwmGenerator
**********************************************************************************************************************/
/*! \brief     Starts the given PWM channel.
*  \param[in]  channel         PWM Channel
*  \param[in]  dutycycle       Duty cycle: 0x0000 (0%) --> 0x8000 (100%)
*  \param[in]  period          Period of the PWM channel (in us)
*  \param[in]  polarity        PWM polarity (HIGH/LOW)
*  \return     no return 
*  \context    Function is called by PWM driver.
*  \note       -.
**********************************************************************************************************************/
void VttPwm_StartPwmGenerator(Pwm_ChannelType channel, uint16 dutycycle, Pwm_PeriodType period, Pwm_OutputStateType polarity)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }

  if(VttPwm_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {  
    CANoeAPI_AtomicBegin();
    
    VttPwm_Cntrl[channel].ChIntEnabled = VTTPWM_FALSE;
    VttPwm_Cntrl[channel].ChOutputEnabled = VTTPWM_TRUE;
    VttPwm_Cntrl[channel].ChPeriod = period;
    VttPwm_Cntrl[channel].ChPolarity = polarity;
    VttPwm_Cntrl[channel].ChIntTriggerEdge = PWM_RISING_EDGE;
    VttPwm_Cntrl[channel].ChNewDutyCycle = VTTPWM_NEWDUTYCYCLE_NO_UPDATE;
    VttPwm_Cntrl[channel].ChNewPeriod = VTTPWM_NEWPERIOD_NO_UPDATE;
    VttPwm_Cntrl[channel].ChDutyCycle = dutycycle;
    
    VttPwm_UpdateDutyCycleImmediate(channel);
   
    CANoeAPI_AtomicEnd();
  }
  
} /* VttPwm_StartPwmGenerator() */

/***********************************************************************************************************************
*  VttPwm_UpdateDutycycle
**********************************************************************************************************************/
/*! \brief    Assigns a PWM channel a new duty cycle.

*  \param[in]  channel         PWM channel.
*  \param[in]  dutycycle       Duty cycle: 0x0000 (0%) --> 0x8000 (100%)
*  \param[in]  immediate       Update immediate (TRUE) or after cycle end (FALSE).
*
*  \context    Called by PWM driver.
**********************************************************************************************************************/
void VttPwm_UpdateDutycycle(Pwm_ChannelType channel, uint16 dutycycle, uint8 immediate)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }

  /* Forward the call to VttPwm_UpdateDutycycleAndPeriod with the current Period */
  VttPwm_UpdateDutycycleAndPeriod(channel, dutycycle, VttPwm_Cntrl[channel].ChPeriod, immediate);
  
} /* VttPwm_UpdateDutycycle() */

/***********************************************************************************************************************
*  VttPwm_UpdateDutycycleAndPeriod
**********************************************************************************************************************/
/*! \brief     Assigns a PWM channel a new duty cycle and period.
*
*  \param[in]  channel         PWM channel.
*  \param[in]  dutycycle       Duty cycle: 0x0000 (0%) --> 0x8000 (100%)
*  \param[in]  period          Period of the given PWM channel microseconds [us].
*  \param[in]  immediate       Update immediate (TRUE) or after cycle end (FALSE).
*
*  \context    Called by PWM driver.
**********************************************************************************************************************/
void VttPwm_UpdateDutycycleAndPeriod(Pwm_ChannelType channel, uint16 dutycycle, Pwm_PeriodType period, uint8 immediate)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }
  
  CANoeAPI_AtomicBegin();
  
  if (0 == VttPwm_Cntrl[channel].ChPeriod)
  {
    /* PWM is currently OFF, this is an immediate update. */
    immediate = TRUE;
  }

  if(VttPwm_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    if ((VttPwm_Cntrl[channel].ChDutyCycle == dutycycle) && (VttPwm_Cntrl[channel].ChPeriod == period))
    {
      /* No update needed. Abort pending update if required. */
      VttPwm_Cntrl[channel].ChNewPeriod = VTTPWM_NEWPERIOD_NO_UPDATE;
      VttPwm_Cntrl[channel].ChNewDutyCycle = VTTPWM_NEWDUTYCYCLE_NO_UPDATE;
    }
    else if ((VttPwm_Cntrl[channel].ChNewDutyCycle == dutycycle) && (VttPwm_Cntrl[channel].ChNewPeriod == period))
    {
      /* Update is already scheduled. */
    }
    else if( TRUE == immediate )
    {
      VttPwm_Cntrl[channel].ChOutputEnabled = VTTPWM_TRUE;
      VttPwm_Cntrl[channel].ChPeriod = period;
      VttPwm_Cntrl[channel].ChDutyCycle = dutycycle;
      VttPwm_Cntrl[channel].ChNewPeriod = VTTPWM_NEWPERIOD_NO_UPDATE;
      VttPwm_Cntrl[channel].ChNewDutyCycle = VTTPWM_NEWDUTYCYCLE_NO_UPDATE;
      VttPwm_UpdateDutyCycleImmediate(channel);
    }
    else
    {
      VttPwm_Cntrl[channel].ChNewPeriod = period;
      VttPwm_Cntrl[channel].ChNewDutyCycle = dutycycle;
      VttPwm_Cntrl[channel].ChNewPeriodNumber = VttPwm_GetPeriodNumber(channel);
      VttPwm_UpdateTimer(channel); /* Force-run the timer, even if notifications are disabled */
    }
  }
  
  CANoeAPI_AtomicEnd();
  
} /* VttPwm_UpdateDutycycleAndPeriod() */

/***********************************************************************************************************************
*  VttPwm_DisableOutput
**********************************************************************************************************************/
/*! \brief     Disables output of a PWM channel, however, channel will still continue running internally.
*
*  \param[in]  channel         PWM channel.
*  \param[in]  idlestate       Value assigned to PWM channel while PWM signal generator output is dropped.
*
*  \context     Called by PWM driver.
**********************************************************************************************************************/
void VttPwm_DisableOutput(Pwm_ChannelType channel, Pwm_OutputStateType idlestate)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }

  if(VttPwm_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_AtomicBegin();
    
    VttPwm_Cntrl[channel].ChOutputEnabled = VTTPWM_FALSE;
    VttPwm_UpdateDutyCycleImmediate(channel);
    
    CANoeAPI_AtomicEnd();
  }
  
} /* VttPwm_DisableOutput() */

/***********************************************************************************************************************
*  VttPwm_StopPwmGenerator
**********************************************************************************************************************/
/*! \brief     Stops a PWM channel.
*
*  \param[in]  channel         PWM channel.
*  \param[in]  idlestate       Value assigned to PWM channel while PWM signal generator is disabled.
*
*  \context    Called by PWM driver.
**********************************************************************************************************************/
void VttPwm_StopPwmGenerator(Pwm_ChannelType channel, Pwm_OutputStateType idlestate)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }

  if(VttPwm_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_AtomicBegin();
    
    /* Cancel a possible pending update. */
    VttPwm_Cntrl[channel].ChNewDutyCycle = VTTPWM_NEWDUTYCYCLE_NO_UPDATE;
    VttPwm_Cntrl[channel].ChNewPeriod = VTTPWM_NEWPERIOD_NO_UPDATE;

    /* Stop PWM only if it is running */
    if ((VttPwm_Cntrl[channel].ChDutyCycle != 0) || (VttPwm_Cntrl[channel].ChPeriod != 0))
    {
      VttPwm_Cntrl[channel].ChIntEnabled = VTTPWM_FALSE;
      VttPwm_Cntrl[channel].ChOutputEnabled = VTTPWM_FALSE;
      VttPwm_Cntrl[channel].ChDutyCycle = 0;
      VttPwm_Cntrl[channel].ChPeriod = 0;
    
      VttPwm_UpdateDutyCycleImmediate(channel);
    }
    
    CANoeAPI_AtomicEnd();
  }
  
} /* VttPwm_StopPwmGenerator() */

/***********************************************************************************************************************
*  VttPwm_EnableInterrupt
**********************************************************************************************************************/
/*! \brief     Enable a PWM channel to cause an interrupt when a certain trigger occurs.
*
*  \param[in]  channel         PWM channel.
*  \param[in]  edge            Output edge as trigger (Rising, Falling, Both).
*
*  \context     Called by PWM driver.
**********************************************************************************************************************/
void VttPwm_EnableInterrupt(Pwm_ChannelType channel, Pwm_EdgeNotificationType edge)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }

  if(VttPwm_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_AtomicBegin();
    
    VttPwm_Cntrl[channel].ChIntEnabled = VTTPWM_TRUE;
    VttPwm_Cntrl[channel].ChIntTriggerEdge = edge;
    VttPwm_UpdateTimer(channel);
    
    CANoeAPI_AtomicEnd();
  }
  
} /* VttPwm_EnableInterrupt() */

/***********************************************************************************************************************
*  VttPwm_DisableInterrupt
**********************************************************************************************************************/
/*! \brief     Disables a PWM channel to cause trigger interrupts.
*  \param[in]  channel         PWM channel.
*  \context    Called by PWM driver.
**********************************************************************************************************************/
void VttPwm_DisableInterrupt(Pwm_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
   
  /* ----- Implementation ----------------------------------------------- */
  if( channel >= VTTPWM_CHANNELS )
  {
    return;
  }

  if(VttPwm_EcuState == CANOEAPI_ECUSTATE_PROCESSING)
  {
    CANoeAPI_AtomicBegin();
    
    VttPwm_Cntrl[channel].ChIntEnabled = VTTPWM_FALSE;
    VttPwm_UpdateTimer(channel); /* Call update timer to stop the timer if necessary. */
    
    CANoeAPI_AtomicEnd();
  }
  
} /* VttPwm_DisableInterrupt() */

/***********************************************************************************************************************
*  VttPwm_GetPwmState
**********************************************************************************************************************/
/*! \brief     Determines current physical state of a PWM channel.
*  \param[in]  channel         PWM channel.
*  \return     Current physical state of the PWM channel.
*  \context    Called by PWM driver.
**********************************************************************************************************************/
Pwm_OutputStateType VttPwm_GetPwmState(Pwm_ChannelType channel)
{  
  /* ----- Local Variables ---------------------------------------------- */
  Pwm_OutputStateType outputState;
  
  /* ----- Implementation ----------------------------------------------- */
    if( channel >= VTTPWM_CHANNELS )
  {
    return (Pwm_OutputStateType) 0;
  }

  if(VttPwm_EcuState != CANOEAPI_ECUSTATE_PROCESSING)
  {
    return (Pwm_OutputStateType) 0;
  }
  
  CANoeAPI_AtomicBegin();
  
  outputState = VttPwm_GetState(channel);
  if (PWM_LOW == VttPwm_Cntrl[channel].ChPolarity )
  {
    /* Invert polarity */
    if ( PWM_HIGH == outputState )
    {
      outputState = PWM_LOW;
    }
    else
    {
      outputState = PWM_HIGH;
    }
  }
  
  CANoeAPI_AtomicEnd();

  return outputState;
  
} /* VttPwm_GetPwmState() */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_Pwm.c
**********************************************************************************************************************/
