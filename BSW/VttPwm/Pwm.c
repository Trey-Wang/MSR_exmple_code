/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Pwm.c
 *    Component:  DrvPwm_VttCanoe01Asr
 *       Module:  DrvPwm
 *    Generator:  -
 *
 *   Description: Module for Pulse Width Modulation
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

#define PWM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Pwm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Pwm header file */
#if (  (PWM_SW_MAJOR_VERSION != (2U)) \
    || (PWM_SW_MINOR_VERSION != (5U)) \
    || (PWM_AR_RELEASE_MAJOR_VERSION != (4U)) \
    || (PWM_AR_RELEASE_MINOR_VERSION != (0U)) \
    || (PWM_AR_RELEASE_PATCH_VERSION != (3U)))
  #error "Version numbers of Pwm.c and Pwm.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (PWM_CFG_MAJOR_VERSION != (3U)) \
    || (PWM_CFG_MINOR_VERSION != (3U)) )
  #error "Version numbers of Pwm.c and Pwm_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define PWM_CHL_CFG(Channel)    ((Pwm_ConfigSet_pt->Pwm_ChannelCfg_pt[Channel]))


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/* reference to the current global runtime configuration */
STATIC P2CONST(Pwm_ConfigType, AUTOMATIC, PWM_APPL_CONST) Pwm_ConfigSet_pt = NULL_PTR;

/* Not initialised variables: 8 bit */
STATIC VAR(uint8, PWM_VAR) Pwm_ChlNotifEnabled_u8[PWM_CHL_COUNT];

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Pwm_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_InitMemory (void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* Initialize all module-global variables */
  Pwm_ConfigSet_pt = NULL_PTR;
  return;
} /* End of Pwm_InitMemory() */


/***********************************************************************************************************************
 *  Pwm_Init
 *********************************************************************************************************************/
/*! \brief      This service initializes the PWM driver
 *  \param[in]  ConfigPtr               Reference to the configuration
 *  \return     void
 *  \context    Function has to be called during startup
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_Init_Vtt (P2CONST(Pwm_ConfigType, AUTOMATIC, PWM_APPL_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 counter;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)
  /* check if Config_ptr is valid */
  if (NULL_PTR == ConfigPtr)
  {
    Pwm_Errorhook (PWM_SID_INIT, PWM_E_PARAM_CONFIG);
    return;
  }
  else
  {
    /* continue processing - ConfigPtr is not NULL_PTR */
  }
  
  /* check if module is already initialized */
  if (NULL_PTR != Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_INIT, PWM_E_ALREADY_INITIALIZED);
    return;
  }
  else
  {
    /* continue processing - module is not initialized */
  }

  /* check if period of the channels are greater than zero */
  for (counter = 0; counter < ConfigPtr->Pwm_ChannelCount_u8; counter++)
  {
    if (0 == ConfigPtr->Pwm_ChannelCfg_pt[counter].Pwm_Period_t)
    {
      Pwm_Errorhook (PWM_SID_INIT, PWM_E_PERIOD_ZERO);
      return;
    }
    else
    {
      /* continue processing - period of the channels greater thab zero */
    }
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* config-pointer is ok, store it globally */
  Pwm_ConfigSet_pt = ConfigPtr;

  /* Loop through all configured channels, start the pwm generator and disable all notifications */
  for (counter = 0; counter < PWM_CHL_COUNT; counter++)
  {
    Pwm_ChlNotifEnabled_u8[counter] = FALSE;
    
    VttPwm_StartPwmGenerator (counter,                                 \
                              PWM_CHL_CFG(counter).Pwm_DutyCycle_u16,  \
                              PWM_CHL_CFG(counter).Pwm_Period_t,     \
                              PWM_CHL_CFG(counter).Pwm_Polarity_t);
  }

  return;
} /* End of Pwm_Init() */


#if (STD_ON == PWM_DE_INIT_API) 
/***********************************************************************************************************************
 *  Pwm_DeInit
 *********************************************************************************************************************/
/*! \brief      Service for PWM de-initialization.
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_DeInit (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 counter;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_DE_INIT, PWM_E_UNINIT );
    return;
  }
  else
  {
    /* continue processing - module is initialized */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Loop through all configured channels, stop the pwm generator and disable all notifications */
  for (counter = 0; counter < PWM_CHL_COUNT; counter++)
  {
    Pwm_ChlNotifEnabled_u8[counter] = FALSE;

    VttPwm_StopPwmGenerator (counter, PWM_CHL_CFG(counter).Pwm_IdleValue_t);
  }

  /* Reset the global configuration pointer */
  Pwm_ConfigSet_pt = (Pwm_ConfigType*)NULL_PTR;

  return;
} /* End of Pwm_DeInit() */
#endif /* (STD_ON == PWM_DE_INIT_API) */


#if (STD_ON == PWM_SET_DUTY_CYCLE_API)
/***********************************************************************************************************************
 *  Pwm_SetDutyCycle
 *********************************************************************************************************************/
/*! \brief      This service sets the duty cycle for a channel.
 *  \param[in]  channel                 The channel number for which the DutyCycle shall be set
 *  \param[in]  DutyCycle               Duty cycle as percentage of period (0x00 represents 0 percent, 0x8000 represents
 *                                      100 percent)
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_SetDutyCycle (Pwm_ChannelType channel, uint16 dutycycle)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)
  /* check if module is initialized */
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_SET_DUTY_CYCLE , PWM_E_UNINIT);
    return;
  }
  else
  {
    /* continue processing - module is initialized */
  }

  /* check if channel is within the specified range */
  if (channel >= PWM_CHL_COUNT)
  {
    Pwm_Errorhook (PWM_SID_SET_DUTY_CYCLE , PWM_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* continue processing - channel is in valid range */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* check if dutycycle is greater than 0x8000. If so, set it to 100 % */
  if (dutycycle > 0x8000)
  {
    VttPwm_UpdateDutycycle (channel, 0x8000, (PWM_DUTYCYCLE_UPDATED_ENDPERIOD) ? FALSE : TRUE);
  }
  else
  {
    VttPwm_UpdateDutycycle (channel, dutycycle, (PWM_DUTYCYCLE_UPDATED_ENDPERIOD) ? FALSE : TRUE);
  }

  return;
} /* End of Pwm_SetDutyCycle() */
#endif /* (STD_ON == PWM_SET_DUTY_CYCLE_API) */


#if (STD_ON == PWM_SET_PERIOD_AND_DUTY_API)
/***********************************************************************************************************************
 *  Pwm_SetPeriodAndDuty
 *********************************************************************************************************************/
/*! \brief      This service sets the period and duty cycle for a channel
 *  \param[in]  channel                 Channel that is modified
 *  \param[in]  period                  Period for the channel
 *  \param[in]  dutyCycle               Duty cycle for the channel (0x0000 represents 0 percent, 0x8000 represents 100
 *                                      percent)
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_SetPeriodAndDuty (Pwm_ChannelType channel, Pwm_PeriodType period, uint16 dutycycle)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)
  /* check if modul is initalized */
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_SET_PERIOD_AND_DUTY , PWM_E_UNINIT);
    return;
  }
  else
  {
    /* continue processing - module is initialized */
  }

  /* check if channel is within the specified range */
  if (channel >= PWM_CHL_COUNT)
  {
    Pwm_Errorhook (PWM_SID_SET_PERIOD_AND_DUTY , PWM_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* continue processing - channel is in valid range */
  }

  /* check if pwm class is fixed period */
  if (PWM_FIXED_PERIOD == PWM_CHL_CFG(channel).Pwm_Class_t)
  {
    Pwm_Errorhook (PWM_SID_SET_PERIOD_AND_DUTY, PWM_E_PERIOD_UNCHANGEABLE);
    return;
  }
  else
  {
    /* continue processing */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* check if the parameter period is zero. If so, dutycycle is not relevant and output is set to zero per-cent dutycycle [PWM150] */
  if (period == 0)
  {
    VttPwm_StopPwmGenerator (channel, (PWM_CHL_CFG(channel).Pwm_Polarity_t) ? PWM_LOW : PWM_HIGH);
  }
  /* check if dutycycle is greater than 0x8000. If so, set it to 100 % */
  else if (dutycycle > 0x8000)
  {
    VttPwm_UpdateDutycycleAndPeriod (channel, 0x8000, period, (PWM_PERIOD_UPDATED_ENDPERIOD) ? FALSE : TRUE);
  }
  else
  {
    VttPwm_UpdateDutycycleAndPeriod (channel, dutycycle, period, (PWM_PERIOD_UPDATED_ENDPERIOD) ? FALSE : TRUE);
  }

  return;
} /* End of Pwm_SetPeriodAndDuty() */
#endif /* (STD_ON == PWM_SET_PERIOD_AND_DUTY_API) */


#if (STD_ON == PWM_SET_OUTPUT_TO_IDLE_API)
/***********************************************************************************************************************
 *  Pwm_SetOutputToIdle
 *********************************************************************************************************************/
/*! \brief      This service sets the output of a channel immediately to idle
 *  \param[in]  channel                 Channel number
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_SetOutputToIdle (Pwm_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)  
  /* check if module is initialized */
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_SET_OUTPUT_TO_IDLE , PWM_E_UNINIT);
    return;
  }
  else
  {
    /* continue processing - module is initialized */
  }

  /* check if channel is within the specified range */
  if (channel >= PWM_CHL_COUNT)
  {
    Pwm_Errorhook (PWM_SID_SET_OUTPUT_TO_IDLE , PWM_E_PARAM_CHANNEL);
    return;
  }
   else
  {
    /* continue processing - channel is in valid range */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Disable the output of the specified channel */
  /* Notification are still invoked if enabled */
  VttPwm_DisableOutput (channel, PWM_CHL_CFG(channel).Pwm_IdleValue_t);

  return;
} /* End of Pwm_SetOutputToIdle() */
#endif /* (STD_ON == PWM_SET_OUTPUT_TO_IDLE_API) */


#if (STD_ON == PWM_GET_OUTPUT_STATE_API)
/***********************************************************************************************************************
 *  Pwm_GetOutputState
 *********************************************************************************************************************/
/*! \brief      This service shall read the internal state of the PWM output signal and return it as defined in the
 *              diagram below
 *  \param[in]  channel                 Channel number
 *  \return     Pwm_OutputStateType     The current output state (PWM_HIGH/PWM_LOW)
 *  \note
 *********************************************************************************************************************/
FUNC(Pwm_OutputStateType, PWM_CODE) Pwm_GetOutputState (Pwm_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)  
  /* check if module is initialized and return PWM_LOW on error detection [PWM051c] */
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_GET_OUTPUT_STATE , PWM_E_UNINIT);
    return PWM_LOW;
  }
  else
  {
    /* continue processing - module is initialized */
  }

  /* check if the channel is within the specified range and return PWM_LOW on error detection [PWM051c] */
  if (channel >= PWM_CHL_COUNT)
  {
    Pwm_Errorhook (PWM_SID_GET_OUTPUT_STATE , PWM_E_PARAM_CHANNEL);
    return PWM_LOW;
  }
  else
  {
    /* continue processing - channel is in valid range */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* the value of the sys var is the state the channel has on the port pin
   * if the output is set to idle, the following function returns his intern representation
   * of the value before the port logic [PWM022] - this is VttPwm_Cntrl[channel].ChPhysState */
  return VttPwm_GetPwmState (channel);
} /* Pwm_OutputStateType Pwm_GetOutputState( ... ) */
#endif /* (STD_ON == PWM_GET_OUTPUT_STATE_API) */


#if (STD_ON == PWM_NOTIFICATION_SUPPORTED)
/***********************************************************************************************************************
 *  Pwm_DisableNotification
 *********************************************************************************************************************/
/*! \brief      This service shall disable the PWM signal edge notification
 *  \param[in]  channel                 Channel number
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_DisableNotification (Pwm_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)  
  /* check if module is initialized */
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_DISABLE_NOTIFICATION , PWM_E_UNINIT);
    return;
  }
  else
  {
    /* continue processing - module is initialized */
  }

  /* check if the channel is within the specified range */
  if (channel >= PWM_CHL_COUNT)
  {
    Pwm_Errorhook (PWM_SID_DISABLE_NOTIFICATION , PWM_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* continue processing - channel is in valid range */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* deregister the interrupt */
  VttPwm_DisableInterrupt (channel);

  /* reset the local variable for the ISR handler */
  Pwm_ChlNotifEnabled_u8[channel] = FALSE;

  return;
} /* End of Pwm_DisableNotification() */


/***********************************************************************************************************************
 *  Pwm_DisableNotification
 *********************************************************************************************************************/
/*! \brief      This service shall activate the PWM signal edge notification according to the notification parameter
 *  \param[in]  channel                 Channel number
 *  \param[in]  edge                    Type of edge for which a notification shall occur
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_EnableNotification (Pwm_ChannelType channel, Pwm_EdgeNotificationType edge)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)
  /* check if module is initialized */ 
  if (NULL_PTR == Pwm_ConfigSet_pt)
  {
    Pwm_Errorhook (PWM_SID_ENABLE_NOTIFICATION , PWM_E_UNINIT);
    return;
  }
  else
  {
    /* continue processing - module is initialized */
  }

  /* check if the channel is within the specified range */
  if (channel >= PWM_CHL_COUNT)
  {
    Pwm_Errorhook (PWM_SID_ENABLE_NOTIFICATION , PWM_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* continue processing - channel is in valid range */
  }

  /* check if the notification pointer is the NULL_PTR */
  if (NULL_PTR == PWM_CHL_CFG(channel).Pwm_Notification_t)
  {
    Pwm_Errorhook (PWM_SID_ENABLE_NOTIFICATION , PWM_E_PARAM_POINTER);
    return;
  }
  else
  {
    /* continue processing - notification pointer is not NULL_PTR */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* set the local variable for the ISR handler */
  Pwm_ChlNotifEnabled_u8[channel] = TRUE;

  /* register on which edge a notification should be invoked */
  VttPwm_EnableInterrupt (channel, edge);

  return;
} /* End of Pwm_EnableNotification() */
#endif /* (STD_ON == PWM_NOTIFICATION_SUPPORTED) */


#if (STD_ON == PWM_VERSION_INFO_API)
/***********************************************************************************************************************
 *  Pwm_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *  \param[out] versioninfo             Pointer to where to store the version information of this module.
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, PWM_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == PWM_DEV_ERROR_DETECT)
  /* check if versioninfo is the NULL_PTR */ 
  if (NULL_PTR == versioninfo)
  {
    Pwm_Errorhook (PWM_SID_GET_VERSION_INFO , PWM_E_PARAM_VINFO);
    return;
  }
  else
  {
    /* continue processing - versioninfo pointer is not NULL_PTR */
  }
#endif /* (STD_ON == PWM_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  versioninfo->vendorID         = (PWM_VENDOR_ID);
  versioninfo->moduleID         = (PWM_MODULE_ID);
  versioninfo->sw_major_version = (PWM_SW_MAJOR_VERSION );
  versioninfo->sw_minor_version = (PWM_SW_MINOR_VERSION );
  versioninfo->sw_patch_version = (PWM_SW_PATCH_VERSION );

  return;
} /* End of Pwm_GetVersionInfo() */
#endif /* (STD_ON == PWM_VERSION_INFO_API) */


/***********************************************************************************************************************
 *  Pwm_IsrHandler
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *  \param[in]  Irq_Source              Interrupt Source
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, PWM_CODE) Pwm_IsrHandler (Pwm_InterruptSourceType Irq_Source)
{
  /* ---------- Implementation ------------------------------------------ */
  if (Pwm_ChlNotifEnabled_u8[Irq_Source] == TRUE)
  {
    #if (STD_ON == PWM_NOTIFICATION_SUPPORTED)
    if (PWM_CHL_CFG(Irq_Source).Pwm_Notification_t != NULL_PTR)
    {
      PWM_CHL_CFG(Irq_Source).Pwm_Notification_t();
    }
    #endif /* (STD_ON == PWM_NOTIFICATION_SUPPORTED) */
  }
} /* End of Pwm_IsrHandler() */


/**********************************************************************************************************************
 *  END OF FILE: Pwm.c
 *********************************************************************************************************************/
 