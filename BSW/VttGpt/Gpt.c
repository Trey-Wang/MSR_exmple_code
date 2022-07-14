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
 *         File:  Gpt.c
 *      Project:  DrvGpt_VttCanoe01Asr
 *       Module:  MICROSAR Gpt
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Gpt implements a general purpose timer driver.
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

#define GPT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Gpt.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Gpt header file */
#if (  (GPT_SW_MAJOR_VERSION != (2U)) \
    || (GPT_SW_MINOR_VERSION != (5U)) \
    || (GPT_AR_RELEASE_MAJOR_VERSION != (4U)) \
    || (GPT_AR_RELEASE_MINOR_VERSION != (0U)) \
    || (GPT_AR_RELEASE_PATCH_VERSION != (3U)) )
  #error "Version numbers of Gpt.c and Gpt.h are inconsistent!"
#endif

#if (  (GPT_CFG_MAJOR_VERSION != (4U)) \
    || (GPT_CFG_MINOR_VERSION != (3U)) )
  #error "Version numbers of Gpt.c and Gpt_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define GPT_INVALID_TIMER_VALUE                 (0x00u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define GPT_CHL(x)  (Gpt_Config_pt->Channels[x])

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
  #define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* Reference to the current global runtime configuration */
P2CONST(Gpt_ConfigType, AUTOMATIC, GPT_APPL_CONST) Gpt_Config_pt = NULL_PTR;

/* GPT operation mode (sleep or normal) */
STATIC VAR(Gpt_ModeType, GPT_VAR) Gpt_ModuleMode = GPT_MODE_NORMAL;
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

/***********************************************************************************************************************
 *  Gpt_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_InitMemory(void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* Initialize all module-global variables */
  Gpt_Config_pt = NULL_PTR;
  Gpt_ModuleMode = GPT_MODE_NORMAL;
  
  return;
} /* End of Gpt_InitMemory() */


/***********************************************************************************************************************
 *  Gpt_Init
 *********************************************************************************************************************/
/*! \brief      This function initializes the GPT and all of it's channels. All channels are reset, notifications and
 *              wakeup capability are disabled. A CANoe timer is created for each channel.
 *  \param[in]  ConfigPtr               Pointer to a configuration set
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \context    Function is called during startup
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_Init_Vtt (P2CONST(Gpt_ConfigType, AUTOMATIC, GPT_APPL_CONST)ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Channel Index variable */
  uint32 Index = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)  
  /* ConfigPtr must not reference NULL_PTR. In case it is, report an error to the DET and return immediately */
  if (NULL_PTR != ConfigPtr)
  {
    /* ConfigPtr is not referencing NULL_PTR. Continue */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_INIT, GPT_E_PARAM_CONFIG);
    return;
  }


  /* ConfigPtr must not reference NULL_PTR. In case it is, report an error to the DET and return immediately */
  if (NULL_PTR == Gpt_Config_pt)
  {
    /* ConfigPtr is not referencing NULL_PTR. Continue */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_INIT, GPT_E_ALREADY_INITIALIZED);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Store configuration pointer in module-global variable. This global variable indicates other functions, that GPT is
   * already initialized.
   */
  Gpt_Config_pt = ConfigPtr;

  for (Index = 0; Index < (Gpt_Config_pt->ChannelQuantity_u8); Index++)
  {
    /* Disable all notifications */
    #if (STD_ON == GPT_ENABLE_DISABLE_NOTIFICATION_API)
      GPT_CHL(Index).NotificationEnabled_u8 = FALSE;
    #endif

    /* Stop and reset all timers */
    GPT_CHL(Index).TimerActive_u8 = FALSE;
    GPT_CHL(Index).TimerValue_u32 = 0;

    /* Disable Wakeup */
    #if (STD_ON == GPT_WAKEUP_FUNCTIONALITY_API)
      GPT_CHL(Index).WakeupEnabled_u8 = FALSE;
    #endif
  }

  /* Set operation mode to normal mode */
  Gpt_ModuleMode = GPT_MODE_NORMAL;

  return;
} /* End of Gpt_Init() */


#if (STD_ON == GPT_DEINIT_API)
/***********************************************************************************************************************
 *  Gpt_DeInit
 *********************************************************************************************************************/
/*! \brief      This method deinitializes the module GPT and all of it's channels and destroys the CANoe timers.
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_DeInit (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Index;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_DEINIT, GPT_E_UNINIT);
    return;
  }

  /* Check the running states and break if a channel is still running */
  for (Index = 0u; Index < Gpt_Config_pt->ChannelQuantity_u8; Index++)
  {
    if (GPT_CHL(Index).TimerActive_u8 == TRUE)
    {
      /* Some of the channels are ON */
      Gpt_Errorhook (GPT_SID_DEINIT, GPT_E_BUSY);
      return;
    }
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Deinitialize all channels */
  for (Index = 0; Index < (Gpt_Config_pt->ChannelQuantity_u8); Index++)
  {
    /* Disable notifications */
    #if (STD_ON == GPT_ENABLE_DISABLE_NOTIFICATION_API)
      GPT_CHL(Index).NotificationEnabled_u8 = FALSE;
    #endif

    /* Disable all timers */
    GPT_CHL(Index).TimerActive_u8 = FALSE;
    GPT_CHL(Index).TimerValue_u32 = 0;

    /* Disable wakeup */
    #if (STD_ON == GPT_WAKEUP_FUNCTIONALITY_API)
      GPT_CHL(Index).WakeupEnabled_u8 = FALSE;
    #endif

    VttGpt_StopTimer (Index);
  }

  Gpt_ModuleMode = GPT_MODE_NORMAL;

  /* Clear configuration */
  Gpt_Config_pt = NULL_PTR;

  return;
} /* End of Gpt_DeInit() */
#endif /* (STD_ON == GPT_DEINIT_API) */


#if (STD_ON == GPT_TIME_ELAPSED_API)
/***********************************************************************************************************************
 *  Gpt_GetTimeElapsed
 *********************************************************************************************************************/
/*! \brief      This service returns the elapsed time of the respective channel in microseconds.
 *  \param[in]  channel                 Channel identifier
 *  \return     Gpt_ValueType           Elapsed time in microseconds
 *  \pre        Channel has to be started.
 *  \note
 *********************************************************************************************************************/
FUNC(Gpt_ValueType, GPT_CODE) Gpt_GetTimeElapsed (Gpt_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 TimeElapsed = 0u;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_GET_TIME_ELAPSED, GPT_E_UNINIT);
    return TimeElapsed;
  }

  /* Check if parameter channel is in valid range */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_GET_TIME_ELAPSED, GPT_E_PARAM_CHANNEL);
    return TimeElapsed;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */  
  if (GPT_CHL(channel).TimerActive_u8 == FALSE)
  {
    TimeElapsed = GPT_CHL(channel).TimerValue_u32;
  }
  else
  {
    /* Get remaining time*/
    TimeElapsed = VttGpt_GetRemainingTime (channel);
    /* Calculate the Elapsed time */
    TimeElapsed = GPT_CHL(channel).TimerValue_u32 - TimeElapsed;
  }

  return TimeElapsed;
} /* End of Gpt_GetTimeElapsed() */
#endif /* (STD_ON == GPT_TIME_ELAPSED_API) */


#if (STD_ON == GPT_TIME_REMAINING_API)
/***********************************************************************************************************************
 *  Gpt_GetTimeRemaining
 *********************************************************************************************************************/
/*! \brief      This service returns the remaining time of the respective channel in microseconds
 *  \param[in]  channel                 Channel identifier
 *  \return     Gpt_ValueType           Remaining time in microseconds
 *  \pre        Channels has to be started.
 *  \note
 *********************************************************************************************************************/
FUNC(Gpt_ValueType, GPT_CODE)  Gpt_GetTimeRemaining (Gpt_ChannelType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 TimeRemaining = 0u;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_GET_TIME_REMAINING, GPT_E_UNINIT);
    return TimeRemaining;
  }

  /* Check if parameter channel is in valid range */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, continue */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_GET_TIME_REMAINING, GPT_E_PARAM_CHANNEL);
    return TimeRemaining;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Check if the channel is active
   * and get the remaining time of the channel's CANoe timer and return it */
  if (GPT_CHL(channel).TimerActive_u8 == FALSE)
  {
    TimeRemaining = 0u;
  }
  else
  {
    TimeRemaining = VttGpt_GetRemainingTime (channel);
  }

  return TimeRemaining;
} /* End of Gpt_GetTimeRemaining() */
#endif /* STD_ON == GPT_TIME_REMAINING_API */


/***********************************************************************************************************************
 *  Gpt_StartTimer
 *********************************************************************************************************************/
/*! \brief      This service starts a timer channel
 *  \param[in]  channel                 Identifier of the channel that has to be started
 *  \param[in]  value                   Timeout period (in microseconds) after which a notification shall occur
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_StartTimer (Gpt_ChannelType channel, Gpt_ValueType value)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_START_TIMER, GPT_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* Check parameter 'channel' */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_START_TIMER, GPT_E_PARAM_CHANNEL);
    return;
  }
  
  /* Check parameter 'value' */
  if (GPT_INVALID_TIMER_VALUE == value)
  {
    Gpt_Errorhook (GPT_SID_START_TIMER, GPT_E_PARAM_VALUE);
    return;
  }
  else
  {
    /* value is not equal zero */
  }

  /* Check the active states */
  if (TRUE == GPT_CHL(channel).TimerActive_u8)
  {
    Gpt_Errorhook (GPT_SID_START_TIMER, GPT_E_BUSY);
    return;
  }
  else
  {
    /* Channel is not running */
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Save Timer Value to the configuration */
  GPT_CHL(channel).TimerValue_u32 = value;

  /* Start timer in CANoe */
  GPT_CHL(channel).TimerActive_u8 = TRUE;
  //Gpt_MappedTimerChannels[GPT_CHL(channel).CANoeTimerIndex_u8] = channel;
  if (GPT_CHL(channel).ChannelMode == GPT_CH_MODE_CONTINUOUS)
  {
    VttGpt_StartTimer (channel, value, VTTGPT_CH_MODE_CONTINUOUS);
  }
  else
  {
    VttGpt_StartTimer (channel, value, VTTGPT_CH_MODE_ONESHOT);
  }
  return;
} /* End of Gpt_StartTimer() */


/***********************************************************************************************************************
 *  Gpt_StopTimer
 *********************************************************************************************************************/
/*! \brief      This service stops a timer channel
 *  \param[in]  channel                 Identifier of the channel that has to be stopped
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_StopTimer (Gpt_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_STOP_TIMER, GPT_E_UNINIT);
    return;
  }

  /* Check parameter 'channel' */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_STOP_TIMER, GPT_E_PARAM_CHANNEL);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Stop timer´in CANoe */
  GPT_CHL(channel).TimerActive_u8 = FALSE;
  VttGpt_StopTimer (channel);

  /* Notification is not disabled automatically for this channel! */
  return;
} /* End of Gpt_StopTimer() */


#if (STD_ON == GPT_ENABLE_DISABLE_NOTIFICATION_API)
/***********************************************************************************************************************
 *  Gpt_EnableNotification
 *********************************************************************************************************************/
/*! \brief      This service enables notifications for a timer channel
 *  \param[in]  channel                 Identifier of the channel, for which notifications are enabled
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_EnableNotification (Gpt_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_ENABLE_NOTIFICATION, GPT_E_UNINIT);
    return;
  }

  /* Check parameter 'channel' */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_ENABLE_NOTIFICATION, GPT_E_PARAM_CHANNEL);
    return;
  }

  if (NULL_PTR == GPT_CHL(channel).Gpt_Notification_pt)
  {
    Gpt_Errorhook (GPT_SID_ENABLE_NOTIFICATION, GPT_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Notification is configured, proceed with function */
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Enable notifications */
  GPT_CHL(channel).NotificationEnabled_u8 = TRUE;

  return;
} /* End of Gpt_EnableNotification() */


/***********************************************************************************************************************
 *  Gpt_DisableNotification
 *********************************************************************************************************************/
/*! \brief      This service disables notifications for a timer channel
 *  \param[in]  channel                 Identifier of the channel, for which notifications are disabled
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_DisableNotification (Gpt_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_DISABLE_NOTIFICATION, GPT_E_UNINIT);
    return;
  }

  /* Check parameter 'channel' */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_DISABLE_NOTIFICATION, GPT_E_PARAM_CHANNEL);
    return;
  }

  if (NULL_PTR == GPT_CHL(channel).Gpt_Notification_pt)
  {
    Gpt_Errorhook (GPT_SID_ENABLE_NOTIFICATION, GPT_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Notification is configured, proceed with function */
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Disable notifications */
  GPT_CHL(channel).NotificationEnabled_u8 = FALSE;

  return;
} /* End of Gpt_DisableNotification() */
#endif /* (STD_ON == GPT_ENABLE_DISABLE_NOTIFICATION_API) */


#if (STD_ON == GPT_WAKEUP_FUNCTIONALITY_API)
/***********************************************************************************************************************
 *  Gpt_SetMode
 *********************************************************************************************************************/
/*! \brief      This functions sets the operation mode of the GPT module.
 *              - If the module is set to sleep mode, all non-wakeup capable channels are stopped.
 *              - If the module is set to normal mode, only the module mode is switched
 *  \param[in]  mode                    Operation mode
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_SetMode (Gpt_ModeType mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Loop counter for all channels */
  uint8 channel;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_SET_MODE, GPT_E_UNINIT);
    return;
  }

  /* Check parameter 'mode' */
  if ((GPT_MODE_SLEEP != mode) && (GPT_MODE_NORMAL != mode) )
  {
    /* Wrong mode was passed to the function */
    Gpt_Errorhook (GPT_SID_SET_MODE, GPT_E_PARAM_MODE);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  if (GPT_MODE_SLEEP == mode)
  {
    /* Loop all channels */
    for (channel = 0; channel < Gpt_Config_pt->ChannelQuantity_u8; channel++)
    {
      /* Check if this channel is wakeup enabled */
      if (TRUE == GPT_CHL(channel).WakeupEnabled_u8)
      {
        /* Do Nothing because the timer should run in sleep mode */
      }
      else
      {
        /* Stop the timer, because it is not wakeup enabled */
        GPT_CHL(channel).TimerActive_u8 = FALSE;

        VttGpt_StopTimer (channel);
      }
    }

    /* Set mode to GPT_MODE_SLEEP */
    Gpt_ModuleMode = GPT_MODE_SLEEP;
  }
  else /* Module is set to GPT_MODE_NORMAL */
  {
    /* Set mode to GPT_MODE_NORMAL */
    Gpt_ModuleMode = GPT_MODE_NORMAL;
  }

  return;
} /* End of Gpt_SetMode() */


/***********************************************************************************************************************
 *  Gpt_DisableWakeup
 *********************************************************************************************************************/
/*! \brief      This service disables wakeup functionality of a timer channel
 *  \param[in]  channel                 Identifier of the channel, for which wakeup functionality is disabled
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_DisableWakeup (Gpt_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_DISABLE_WAKEUP, GPT_E_UNINIT);
    return;
  }

  /* Check parameter 'channel' */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_DISABLE_WAKEUP, GPT_E_PARAM_CHANNEL);
    return;
  }

  /* Check if channel is wakeup capable */
  if (GPT_CFG_DISABLE_WAKEUP == GPT_CHL(channel).WakeupCapable_u8)
  {
    /* Wakeup disabled -> Development error */
    Gpt_Errorhook (GPT_SID_DISABLE_WAKEUP, GPT_E_PARAM_CHANNEL);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Disable Wakeup for this channel */
  GPT_CHL(channel).WakeupEnabled_u8 = FALSE;

  return;
} /* End of Gpt_DisableWakeup() */


/***********************************************************************************************************************
 *  Gpt_EnableWakeup
 *********************************************************************************************************************/
/*! \brief      This service enables wakeup functionality of a timer channel
 *  \param[in]  channel                 Identifier of the channel, for which wakeup functionality is enabled
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_EnableWakeup (Gpt_ChannelType channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_ENABLE_WAKEUP, GPT_E_UNINIT);
    return;
  }

  /* Check parameter 'channel' */
  if (Gpt_Config_pt->ChannelQuantity_u8 > channel)
  {
    /* Channel is in range, proceed with function */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_ENABLE_WAKEUP, GPT_E_PARAM_CHANNEL);
    return;
  }

  /* Check if channel is wakeup capable */
  if (GPT_CFG_DISABLE_WAKEUP == GPT_CHL(channel).WakeupCapable_u8)
  {
    Gpt_Errorhook (GPT_SID_ENABLE_WAKEUP, GPT_E_PARAM_CHANNEL);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  /* Enable wakeup for this channel */
  GPT_CHL(channel).WakeupEnabled_u8 = TRUE;

  return;
} /* End of Gpt_EnableWakeup() */


/***********************************************************************************************************************
 *  Gpt_CheckWakeup
 *********************************************************************************************************************/
/*! \brief      This service checks, id the wakeup source was correct
 *  \param[in]  WakeupSource           Reported wakeup source
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_CheckWakeup (EcuM_WakeupSourceType WakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  int loop;
  EcuM_WakeupSourceType TempWakeupSource;

   /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Gpt_Config_pt )
  {
    Gpt_Errorhook (GPT_SID_CHECK_WAKEUP, GPT_E_UNINIT);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
    /*In case the expected wk source Id was found, the referenced EcuM function will be called*/
  for (loop = 0; loop < Gpt_Config_pt->ChannelQuantity_u8 ; loop++)
  {
    TempWakeupSource = 1<< Gpt_Config_pt->Channels[loop].WakeupEvent_u32;
    if ( (WakeupSource == TempWakeupSource) && (Gpt_Config_pt->Channels[loop].WakeupEnabled_u8 == TRUE) )
    {
      EcuM_SetWakeupEvent (WakeupSource);  
      break;
    } 
  }

  return;
} /* End of Gpt_CheckWakeup() */
#endif /* (STD_ON == GPT_WAKEUP_FUNCTIONALITY_API) */


#if (STD_ON == GPT_VERSION_INFO_API)
/***********************************************************************************************************************
 *  Gpt_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *  \param[out] versioninfo             Pointer to where to store the version information of this module.
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, GPT_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == GPT_DEV_ERROR_DETECT)
  if (NULL_PTR != versioninfo)
  {
    /* Pointer is not referencing NULL_PTR. Continue */
  }
  else
  {
    Gpt_Errorhook (GPT_SID_GET_VERSION_INFO, GPT_E_PARAM_VINFO);
    return;
  }
#endif /* (STD_ON == GPT_DEV_ERROR_DETECT) */

  /* ---------- Implementation ------------------------------------------ */
  versioninfo->vendorID = (GPT_VENDOR_ID);
  versioninfo->moduleID = (GPT_MODULE_ID);
  versioninfo->sw_major_version = (GPT_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (GPT_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (GPT_SW_PATCH_VERSION);

  return;
} /* End of Gpt_GetVersionInfo() */
#endif  /* (GPT_VERSION_INFO_API == STD_ON) */


/***********************************************************************************************************************
 *  Gpt_IRQFunction
 *********************************************************************************************************************/
/*! \brief      This function serves as callback function for expired CANoe timers. It sends notifications 
 *              (in GPT_MODE_NORMAL) or calls wakeup functions (in GPT_MODE_SLEEP).
 *  \param[in]  Index   TimerIndex
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \context    Function is called in interrupt context
 *  \note
 *********************************************************************************************************************/
FUNC(void, GPT_CODE) Gpt_IrqFunction (uint8 Index)
{
  /* ---------- Implementation ------------------------------------------ */
  if (Gpt_Config_pt->Channels[Index].ChannelMode == GPT_CH_MODE_ONESHOT)
  {
    Gpt_Config_pt->Channels[Index].TimerActive_u8 = FALSE;
  }

  /* Is the timer in normal mode? */
  if (GPT_MODE_NORMAL == Gpt_ModuleMode)
  {
    /* Check if the Notification is enabled */
    if (TRUE == GPT_CHL(Index).NotificationEnabled_u8)
    {
      /* Call Notification Function */
      if (NULL_PTR != GPT_CHL(Index).Gpt_Notification_pt)
      {
        GPT_CHL(Index).Gpt_Notification_pt();
      }
    }
  }
  else /* GPT_MODE_SLEEP */
  {
    /* Wakeup processing */
    #if (STD_ON == GPT_REPORT_WAKEUP_SOURCE)
      /* Wakeup handler */
      EcuM_CheckWakeup(1<<GPT_CHL(Index).WakeupEvent_u32);
    #endif /* (STD_ON == GPT_REPORT_WAKEUP_SOURCE) */
  }

  return;
} /* End of Gpt_IRQFunction() */


/**********************************************************************************************************************
 *  END OF FILE: Gpt.c
 *********************************************************************************************************************/
