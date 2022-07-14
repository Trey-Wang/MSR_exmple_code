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
 *         File:  Icu.c
 *      Project:  DrvIcu_CanoeemuCanoe01Asr
 *       Module:  MICROSAR Icu
 *    Generator:  -
 *
 *  Description:  The ICU driver is a module using the input capture unit (ICU) for demodulation of a PWM signal,
 *                counting pulses, measuring of frequency and duty cycle, generating simple interrupts and wakeup
 *                interrupts. The ICU driver provides services for
 *                -    Signal edge notification
 *                -    Controlling wakeup interrupts
 *                -    Periodic signal time measurement
 *                -    Edge timestamping, usable for the acquisition of non-periodic signals
 *                -    Edge counting
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
 
#define ICU_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "Icu.h"


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Icu header file */
#if (  (ICU_SW_MAJOR_VERSION != (2U) ) \
    || (ICU_SW_MINOR_VERSION != (5U))  \
    || (ICU_AR_RELEASE_MAJOR_VERSION != (4U)) \
    || (ICU_AR_RELEASE_MINOR_VERSION != (0U)) \
    || (ICU_AR_RELEASE_PATCH_VERSION != (3U)) )
  #error "Vendor specific version numbers of Icu.c and Icu.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ICU_CFG_MAJOR_VERSION != (0x03U) ) \
    || (ICU_CFG_MINOR_VERSION != (0x03U)) )
  #error "Version numbers of Icu.c and Icu_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Defines are used for splitting up the channel type and position */
#define ICU_SPL_CHL               (0x3F)
#define ICU_MM                    (0xC0)

#define ILLEGAL_CHANNEL           (0x3F)


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define ICU_SPLIT_CHANNEL(channelId, part) (channelId & part)

#define ICU_ED_CHL_CFG(Channel)   ((Icu_ConfigSet_pt->Icu_EdChls_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)]))
#define ICU_EC_CHL_CFG(Channel)   ((Icu_ConfigSet_pt->Icu_EcChls_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)]))
#define ICU_TS_CHL_CFG(Channel)   ((Icu_ConfigSet_pt->Icu_TsChls_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)]))
#define ICU_SM_CHL_CFG(Channel)   ((Icu_ConfigSet_pt->Icu_SmChls_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)]))

#define ICU_ED_HW_CHL(Channel)    (ICU_ED_CHL_CFG(Channel).Icu_ChannelId_u8)
#define ICU_EC_HW_CHL(Channel)    (ICU_EC_CHL_CFG(Channel).Icu_ChannelId_u8)
#define ICU_TS_HW_CHL(Channel)    (ICU_TS_CHL_CFG(Channel).Icu_ChannelId_u8)
#define ICU_SM_HW_CHL(Channel)    (ICU_SM_CHL_CFG(Channel).Icu_ChannelId_u8)

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
#if (ICU_DEV_ERROR_DETECT == STD_ON)
/* Variable to store the information about the initiation status of
* this driver */
STATIC VAR(boolean, ICU_VAR)              Icu_ModuleInitialized_b = FALSE;
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

STATIC VAR(Icu_ModeType, ICU_VAR)         Icu_OperatingMode = ICU_MODE_NORMAL;

/* Stores the pointer to the current configuration-set */
STATIC P2CONST(Icu_ConfigType, AUTOMATIC, ICU_APPL_CONST) Icu_ConfigSet_pt = NULL_PTR;

/* 32Bit variables that will be initialized by the startup code*/
#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
/* New Sm value measured  
* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
STATIC VAR(uint64, ICU_VAR) Icu_NewSmValueAvailable = 0x0000000000000000;
STATIC VAR(uint64, ICU_VAR) Icu_SmFirstMeasurement = 0xFFFFFFFFFFFFFFFF;
STATIC VAR(uint64, ICU_VAR) Icu_SmRunning = 0x0000000000000000;
/* Run states of signal measurement channels
* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
STATIC VAR(uint64, ICU_VAR) Icu_SmChlRunStates_u8 = 0x0000000000000000;
#endif

#if (ICU_EDGE_DETECT_API == STD_ON)
/* Represents, if notification of edge detection is enabled
* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
STATIC VAR(uint64, ICU_VAR) Icu_EdChlNotifEnabled_u8 = 0x0000000000000000;
#endif

#if ( (ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON) || (ICU_REPORT_WAKEUP_SOURCE == STD_ON) )
/* Variables to store information about the wakeup-capability of a channel */
/* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
STATIC VAR(uint64, ICU_VAR) Icu_WakeupActive = 0x0000000000000000;
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if (STD_ON == ICU_EDGE_DETECT_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForEd (Icu_ChannelType Channel);
#endif
#if (STD_ON == ICU_EDGE_COUNT_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForEc (Icu_ChannelType Channel);
#endif
#if (STD_ON == ICU_TIMESTAMP_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForTs (Icu_ChannelType Channel);
#endif
#if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForSm (Icu_ChannelType Channel);
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  Icu_ActionForEd()
***********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is Edge Detection
 *  \param[in]  Channel       ChannelId
 *  \return     void
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (STD_ON == ICU_EDGE_DETECT_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForEd (Icu_ChannelType Channel)
{
# if (ICU_REPORT_WAKEUP_SOURCE == STD_ON)
  EcuM_WakeupSourceType TempWakeupSource;
  
  if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE)
  {
    if (Icu_OperatingMode == ICU_MODE_SLEEP)
    {
      TempWakeupSource = (((uint64)1) << ICU_ED_CHL_CFG(Channel).Icu_WakeupSource_u32);
      
      EcuM_CheckWakeup(TempWakeupSource);
    }
# else
  if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE)
  {
# endif

    ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_ACTIVE;

    if ( (Icu_EdChlNotifEnabled_u8 >> ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)) & 0x1 )
    {
      if (ICU_ED_CHL_CFG(Channel).Icu_Notify_pt != NULL_PTR)
      {
        /* Call the User Notification Function */
        ICU_ED_CHL_CFG(Channel).Icu_Notify_pt();
      }
    }

  }
  return;
}
#endif /* (STD_ON == ICU_EDGE_DETECT_API) */


/**********************************************************************************************************************
 *  Icu_ActionForEc()
 *********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is Edge Counting
 *  \param[in]  Channel       ChannelId
 *  \return     void
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (STD_ON == ICU_EDGE_COUNT_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForEc (Icu_ChannelType Channel)
{
  if (ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 == TRUE)
  {
    ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16++;
  }
  return;
}
#endif


/**********************************************************************************************************************
 *  Icu_ActionForTs()
 *********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is Timestamp mode
 *  \param[in]  Channel       ChannelId
 *  \return     void
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (STD_ON == ICU_TIMESTAMP_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForTs (Icu_ChannelType Channel)
{
  Icu_IndexType OldBufferIndex = 0; /* Is Used to store the index before increasing the buffer. saves a if case */
  Icu_ChannelType ChannelId = ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL);
  Icu_TsArrayType *TsData = &Icu_ConfigSet_pt->Icu_TsData_at[ChannelId];
  
  /* Check if Timestamping is activated for this channel */
  if ( TsData->RunState == TRUE)
  {
   TsData->BufferPtr[TsData->BufferIndex] = (Icu_ValueType) VttIcu_GetTimerValue(ICU_TS_HW_CHL(Channel));

    /* Store the Index and Increase it's value */
    OldBufferIndex = TsData->BufferIndex;
    TsData->BufferIndex++;


    /*   If the buffer index reaches the end of the buffer and the bufer is a circualar buffer
    *    then reset the index to start from the beginning. Otherwise stop the Timestamping
    */
    if ( TsData->BufferIndex == TsData->BufferSize )
    {
      if (ICU_TS_CHL_CFG(Channel).Icu_BufferType_t == ICU_CIRCULAR_BUFFER)
      {
        TsData->BufferIndex = 0;
      }
      else
      {
        Icu_StopTimestamp(Channel);
      }
    }

    /* Check if notifications are enabled */
    if ( TRUE == TsData->NotifyInfo )
    {
      /* Increase the Notification Counter */
      TsData->NotifyCnt++;
      /* If the notification counter reaches the notification intervall, call the notification function and reset
      * the counter
      */
      if (TsData->NotifyCnt == TsData->NotifyInterval)
      {
        if (ICU_TS_CHL_CFG(Channel).Icu_Notify_pt != NULL_PTR) 
        {
          ICU_TS_CHL_CFG(Channel).Icu_Notify_pt();
        }
        TsData->NotifyCnt = 0;
      }
    }
  }
  return;
}
#endif


/**********************************************************************************************************************
 *  Icu_ActionForSm()
 *********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is signal measurement
 *  \param[in]  Channel       ChannelId
 *  \return     void
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
STATIC FUNC(void, ICU_CODE) Icu_ActionForSm (Icu_ChannelType Channel)
{
  Icu_ChannelType ChannelId = ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL);
  Icu_SignalMeasurementPropertyType Icu_Property;
  Icu_ValueType Icu_OldTimerValue, Icu_LatestTimerValue;
  uint8 Icu_Overflow = 0;

  Icu_Property = ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t;
  Icu_OldTimerValue = Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId]; /*   old  value */
  Icu_LatestTimerValue = (Icu_ValueType) VttIcu_GetTimerValue(ICU_SM_HW_CHL(Channel));  /* latest value */

  /* check a overflow condition */
  if ( Icu_LatestTimerValue < Icu_OldTimerValue )
  {
    Icu_Overflow = 1;
  }

  if ( Icu_Property == ICU_DUTY_CYCLE )
  {
    if ( (Icu_SmFirstMeasurement >> ChannelId) & 0x1 )
    {
      /* indicate that the first measurement was done */
      Icu_SmFirstMeasurement &= ~(((uint64)1) << ChannelId);

      /* only the very first time this following if-sequence is not executed */
      if ( (Icu_SmRunning >> ChannelId) & 0x1 )
      {
        if (Icu_Overflow == 1)
        {
          Icu_Overflow = 0;
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = 0xFFFFFFFF - Icu_OldTimerValue + Icu_LatestTimerValue;
        }
        else
        {
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = Icu_LatestTimerValue - Icu_OldTimerValue;
        }

        Icu_NewSmValueAvailable |= (((uint64)1) << ChannelId);
        Icu_ConfigSet_pt->Icu_SmChlStatus_at[ChannelId] = ICU_ACTIVE;
      }

      /* store the latest value */
      Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId] = Icu_LatestTimerValue;
    }
    else
    {
      /* set the active time of the specified dutycycle with the measured time */
      if (Icu_Overflow == 1)
      {
        Icu_Overflow = 0;
        Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = 0xFFFFFFFF - Icu_OldTimerValue + Icu_LatestTimerValue;
      }
      else
      {
        Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = Icu_LatestTimerValue - Icu_OldTimerValue;
      }

      /* Indicate that the next measurement should be treated like a first measurement, but in Running mode 
       * (not the very first) 
       */
      Icu_SmFirstMeasurement  |= (((uint64)1) << ChannelId);
      Icu_SmRunning |= (((uint64)1) << ChannelId);
    }
  }
  else /* ICU_LOW_TIME, ICU_HIGH_TIME, ICU_PERIOD_TIME */
  {
    if ( (Icu_SmFirstMeasurement >> ChannelId) & 0x1)
    {
      /* indicate that the first measurement was done */
      Icu_SmFirstMeasurement &= ~(((uint64)1) << ChannelId);

      /* store the value */
      Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId] = Icu_LatestTimerValue;
    }
    else
    {
      switch(Icu_Property)
      {
      case ICU_LOW_TIME:
      case ICU_HIGH_TIME:
        /* set the active time of the specified dutycycle array with the measured time */
        if (Icu_Overflow == 1)
        {
          Icu_Overflow = 0;
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = 0xFFFFFFFF - Icu_OldTimerValue + Icu_LatestTimerValue;
        }
        else
        {
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = Icu_LatestTimerValue - Icu_OldTimerValue;
        }
        Icu_SmFirstMeasurement  |= (((uint64)1) << ChannelId);
        break;

      case ICU_PERIOD_TIME:
        /* set the active time of the specified dutycycle array with the measured high time */
        if (Icu_Overflow == 1)
        {
          Icu_Overflow = 0;
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = 0xFFFFFFFF - Icu_OldTimerValue + Icu_LatestTimerValue;
        }
        else
        {
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = Icu_LatestTimerValue - Icu_OldTimerValue;
        }
        Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId] = Icu_LatestTimerValue;
        break;
      }

      /* indicate that a new signal measurement value is available */
      Icu_NewSmValueAvailable |= (((uint64)1) << ChannelId);
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[ChannelId] = ICU_ACTIVE;
    }
  }
}
#endif



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Icu_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_InitMemory (void)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* Initialize all module-global variables */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  Icu_ModuleInitialized_b = FALSE;
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  Icu_ConfigSet_pt = NULL_PTR;
  
  Icu_OperatingMode = ICU_MODE_NORMAL;

#if ( (ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON) || (ICU_REPORT_WAKEUP_SOURCE == STD_ON) )
  Icu_WakeupActive = 0x00000000;
#endif  

  return;
}


/**********************************************************************************************************************
 *  Icu_Init()
 ***********************************************************************************************************************/
/*! \brief      Service for driver initialization. The Initialization function initializes all relevant registers of
 *              the configured hardware with the values of the structure referenced by the parameter ConfigPtr.
 *  \param[in]  ConfigPtr               Pointer to a selected configuration structure
 *  \pre        Input parameter must not be NULL_PTR.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_Init_Vtt (P2CONST(Icu_ConfigType, AUTOMATIC, ICU_APPL_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Index;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_PARAM_CONFIG */
  if (NULL_PTR == ConfigPtr)
  {
    /* ConfigPtr is not defined */
    Icu_Errorhook (ICU_SID_INIT, ICU_E_PARAM_CONFIG);
    return;
  }
  else
  {
    /* ConfigPtr is defined */
  }

  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b)
  {
    /* ConfigPtr is not defined */
    Icu_Errorhook (ICU_SID_INIT, ICU_E_ALREADY_INITIALIZED);
    return;
  }
  else
  {
    /* ConfigPtr is defined */
  }
#endif  /* (STD_ON == ICU_DEV_ERROR_DETECT) */


  /* ----- Implementation ------------------------------------------------- */
  Icu_OperatingMode = ICU_MODE_NORMAL; /* Set driver to 'ICU_MODE_NORMAL' */

  Icu_ConfigSet_pt = ConfigPtr;

#if (STD_ON == ICU_EDGE_DETECT_API)
  /* Init Edge Detection Structure */
  for (Index=0; Index < Icu_ConfigSet_pt->Icu_EdChlCount_u8; Index++)
  {
    Icu_ConfigSet_pt->Icu_EdChls_at[Index].Icu_EdActive_u8 = FALSE;
    Icu_ConfigSet_pt->Icu_EdChls_at[Index].Icu_Ed_InputState = ICU_IDLE;
  }
  Icu_EdChlNotifEnabled_u8 = 0;
#endif /* (STD_ON == ICU_EDGE_DETECT_API) */

#if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
#if (ICU_GET_INPUT_STATE_API == STD_ON)
  for (Index=0; Index < Icu_ConfigSet_pt->Icu_SmChlCount_u8; Index++)
  {
    Icu_ConfigSet_pt->Icu_SmChlStatus_at[Index] = ICU_IDLE;
  }
#endif /* (STD_ON == ICU_GET_INPUT_STATE_API) */

  Icu_NewSmValueAvailable = 0x00000000;
  Icu_SmChlRunStates_u8 = 0x00000000;
  Icu_SmFirstMeasurement = 0xFFFFFFFF;
  Icu_SmRunning = 0x00000000;

  for (Index=0; Index < Icu_ConfigSet_pt->Icu_SmChlCount_u8; Index++)
  {
    Icu_ConfigSet_pt->Icu_SmValue_at[Index] = 0;
  }
#endif /* (STD_ON == ICU_SIGNAL_MEASUREMENT_API) */

#if (STD_ON == ICU_EDGE_COUNT_API)
  /* Init Edge Counting Structure */
  for (Index=0;Index< Icu_ConfigSet_pt->Icu_EcChlCount_u8;Index++)
  {
    Icu_ConfigSet_pt->Icu_EcChls_at[Index].Icu_EcActive_u8 = FALSE;
    Icu_ConfigSet_pt->Icu_EcChls_at[Index].Icu_EdgeValue_u16 = 0x0000u;
  }
#endif /* (STD_ON == ICU_EDGE_COUNT_API) */

#if (STD_ON == ICU_TIMESTAMP_API)
  /* Init Timestamping Structure */
  for (Index=0; Index< Icu_ConfigSet_pt->Icu_TsChlCount_u8;Index++)
  {
    Icu_ConfigSet_pt->Icu_TsData_at[Index].RunState = FALSE;
    Icu_ConfigSet_pt->Icu_TsData_at[Index].NotifyInfo = FALSE;
  }
#endif /* (STD_ON == ICU_TIMESTAMP_API) */

  for (Index=0; Index<Icu_ConfigSet_pt->Icu_MaxNrOfChannels; Index++)
  {
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[Index] = ILLEGAL_CHANNEL;
  }

#if (ICU_DEV_ERROR_DETECT == STD_ON)
  Icu_ModuleInitialized_b = TRUE;
#endif  /* (STD_ON == ICU_DEV_ERROR_DETECT) */

  return ;
} /* End of Icu_Init() */



#if (STD_ON == ICU_DE_INIT_API)
/**********************************************************************************************************************
 *  Icu_DeInit()
 *********************************************************************************************************************/
/*! \brief      Service for ICU De-Initialization. After the call of this service, the state of the peripherals used
 *              by configuration will be the same as after power on reset. Values of registers which are not writeable
 *              are excluded.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DeInit (void)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 Index;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    /* The module hasn't been initialized */
    Icu_Errorhook (ICU_SID_DE_INIT, ICU_E_UNINIT);
    return;
  }
  else
  {   
    /* Module is initialized */
  }

  Icu_ModuleInitialized_b = FALSE;    /* Reset the module status */
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  for (Index=0; Index < Icu_ConfigSet_pt->Icu_MaxNrOfChannels; Index++)
  {
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[Index] = ILLEGAL_CHANNEL;
  }

  /* Clear the global configuration pointer */
  Icu_ConfigSet_pt = NULL_PTR;

  return;
}   /* End of Icu_DeInit() */
#endif /* (STD_ON == ICU_DE_INIT_API) */


#if (STD_ON == ICU_SET_MODE_API)
/**********************************************************************************************************************
 *  Icu_SetMode()
 *********************************************************************************************************************/
/*! \brief      Service for ICU mode selection. This service sets the operation mode to the given mode parameter. The
 *              component can be set to sleep mode or normal mode with this service.
 *
 *  \param[in]  Mode                    Icu-Mode to set
 *  \pre        Channel must be either ICU_MODE_NORMAL or ICU_MODE_SLEEP.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_SetMode (Icu_ModeType Mode)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 Index =0;
#if (  (STD_ON == ICU_EDGE_DETECT_API) \
    && ((ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON)) )
  uint32 tempWakeupActiv;
#endif
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  uint8 Indication = 0;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_SET_MODE, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_MODE */
  if ((ICU_MODE_NORMAL == Mode) || (ICU_MODE_SLEEP == Mode))
  {
    /* Right paremeter has been given */
  }
  else
  {
    Icu_Errorhook (ICU_SID_SET_MODE, ICU_E_PARAM_MODE);
    return;
  }

  /* Check if one of the Channels is running */
  #if (STD_ON == ICU_TIMESTAMP_API)
    for (Index=0; Index < Icu_ConfigSet_pt->Icu_TsChlCount_u8; Index++)
    {
      if (Icu_ConfigSet_pt->Icu_TsData_at[Index].RunState == TRUE)
      {
        Indication++;
      }
    }
  #endif /* (STD_ON == ICU_TIMESTAMP_API) */

  #if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
    if (Icu_SmChlRunStates_u8 > 0)
    {
      Indication++;
    }
  #endif /* (STD_ON == ICU_SIGNAL_MEASUREMENT_API) */

  #if (STD_ON == ICU_EDGE_COUNT_API)
    for (Index=0; Index < Icu_ConfigSet_pt->Icu_EcChlCount_u8; Index++)
    {
      if (ICU_EC_CHL_CFG(Index).Icu_EcActive_u8 == TRUE)
      {
        Indication++;
      }
    }
  #endif /* (STD_ON == ICU_EDGE_COUNT_API) */

  /* If one of the channels is running the indicationcounter will be > 0 so the errorhook has to be called */
  if (Indication > 0)
  {
    Icu_Errorhook (ICU_SID_SET_MODE, ICU_E_BUSY_OPERATION);
    return;
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  Icu_OperatingMode = Mode;
  if (ICU_MODE_SLEEP == Mode)
  {
  #if (STD_ON == ICU_TIMESTAMP_API)
    for (Index=0; Index < Icu_ConfigSet_pt->Icu_TsChlCount_u8; Index++)
    {
      if (Icu_ConfigSet_pt->Icu_TsData_at[Index].RunState == TRUE)
      {
        Icu_StopTimestamp (ICU_MODE_TIMESTAMP | Index);
      }
    }
  #endif /* (STD_ON == ICU_TIMESTAMP_API) */

  #if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
    for (Index=0; Index < Icu_ConfigSet_pt->Icu_SmChlCount_u8; Index++)
    {
      if ( ((Icu_SmChlRunStates_u8 >> Index) & 0x1) == TRUE )
      {
        Icu_StopSignalMeasurement (ICU_MODE_SIGNAL_MEASUREMENT| Index);
      }
    }
  #endif /* (STD_ON == ICU_SIGNAL_MEASUREMENT_API) */

  #if (STD_ON == ICU_EDGE_COUNT_API)
    for (Index=0; Index < Icu_ConfigSet_pt->Icu_EcChlCount_u8; Index++)
    {
      if (ICU_EC_CHL_CFG(Index).Icu_EcActive_u8 == TRUE )
      {
        Icu_DisableEdgeCount (ICU_MODE_EDGE_COUNTER | Index);
      }
    }
  #endif /* (STD_ON == ICU_EDGE_COUNT_API) */

  #if (STD_ON == ICU_EDGE_DETECT_API)
    for (Index=0; Index < Icu_ConfigSet_pt->Icu_EdChlCount_u8; Index++)
    {
      #if ( (ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON) )
      if (ICU_ED_CHL_CFG(Index).Icu_WakeupCapability_u8 == FALSE)
      {
        if (ICU_ED_CHL_CFG(Index).Icu_EdActive_u8 == TRUE)
        {
          Icu_DisableEdgeDetection (ICU_MODE_EDGE_DETECTION | Index);
        }
      }
      else 
      {
        tempWakeupActiv = (uint32)((((uint64)Icu_WakeupActive) >> ICU_SPLIT_CHANNEL(Index, ICU_SPL_CHL)) & 0x1);
        if (tempWakeupActiv == 0)
        {
          if (ICU_ED_CHL_CFG(Index).Icu_EdActive_u8 == TRUE)
          {
            Icu_DisableEdgeDetection (ICU_MODE_EDGE_DETECTION | Index);
          }
        }
      }
      #else
      Icu_DisableEdgeDetection (ICU_MODE_EDGE_DETECTION | Index);
      #endif
    }
  #endif /* (STD_ON == ICU_EDGE_DETECT_API) */
  }
  else
  { 
    /* ICU_MODE_NORMAL */
  }
  return;
} /* End of Icu_SetMode() */
#endif /* (STD_ON == ICU_SET_MODE_API) */


#if (STD_ON == ICU_DISABLE_WAKEUP_API)
/**********************************************************************************************************************
 *  Icu_DisableWakeup()
 *********************************************************************************************************************/
/*! \brief      This service shall disable the wakeup capability of a single ICU channel. This service is only feasible
 *              for ICU channels configured statically as wakeup capable true.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel has to be configured as a wakeup-capable channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableWakeup (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_DISABLE_WAKEUP, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ((&ICU_ED_CHL_CFG(Channel)) == NULL_PTR)
  {
    Icu_Errorhook (ICU_SID_DISABLE_WAKEUP,ICU_E_PARAM_CHANNEL);
    return;
  }

  if ( (ICU_MODE_EDGE_DETECTION != ICU_SPLIT_CHANNEL(Channel,ICU_MM)) || 
       (ICU_ED_CHL_CFG(Channel).Icu_WakeupCapability_u8 == FALSE) ||
       ( (ICU_MODE_EDGE_DETECTION == ICU_SPLIT_CHANNEL(Channel,ICU_MM)) && 
         (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) ) )
  {
    Icu_Errorhook (ICU_SID_DISABLE_WAKEUP,ICU_E_PARAM_CHANNEL);
    return;
  }
#endif  /*(STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  if (ICU_ED_CHL_CFG(Channel).Icu_WakeupCapability_u8 == TRUE)
  {
    Icu_WakeupActive &= ~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL));
  }
} /* End of Icu_DisableWakeup() */
#endif /* (STD_ON == ICU_DISABLE_WAKEUP_API) */


#if (STD_ON == ICU_ENABLE_WAKEUP_API )
/**********************************************************************************************************************
 *  Icu_EnableWakeup()
 *********************************************************************************************************************/
/*! \brief      This service (re-)enables the wakeup capability of a single ICU channel. This service is only feasible
 *              for ICU channels configured as wakeup capable true.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel has to be configured as a wakeup-capable channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableWakeup (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_ENABLE_WAKEUP, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ((&ICU_ED_CHL_CFG(Channel)) == NULL_PTR)
  {
    Icu_Errorhook (ICU_SID_ENABLE_WAKEUP, ICU_E_PARAM_CHANNEL);
    return;
  }

  if ( (ICU_MODE_EDGE_DETECTION != ICU_SPLIT_CHANNEL(Channel,ICU_MM)) || 
       (ICU_ED_CHL_CFG(Channel).Icu_WakeupCapability_u8 == FALSE)     ||
       ( (ICU_MODE_EDGE_DETECTION == ICU_SPLIT_CHANNEL(Channel,ICU_MM)) && 
         (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) ) )
  {
    Icu_Errorhook (ICU_SID_ENABLE_WAKEUP,ICU_E_PARAM_CHANNEL);
    return;
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  if ( ICU_ED_CHL_CFG(Channel).Icu_WakeupCapability_u8 == TRUE )
  {
    Icu_WakeupActive |= (((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL));
  }
} /* End of Icu_EnableWakeup() */
#endif /* (STD_ON == ICU_ENABLE_WAKEUP_API) */


/**********************************************************************************************************************
 *  Icu_SetActivationCondition()
 *********************************************************************************************************************/
/*! \brief      This service sets the activation-edge according to the Activation parameter for the given channel. This
 *              service overrides the edge setting of the configuration.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \param[in]  Activation              Type of activation
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_TIMESTAMP
 *              - ICU_MODE_EDGE_COUNTER
 *  \pre        Activation must be ICU_RISING_EDGE, ICU_FALLING_EDGE, ICU_BOTH_EDGES
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_SetActivationCondition (Icu_ChannelType Channel, Icu_ActivationType Activation)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if (ICU_MODE_SIGNAL_MEASUREMENT == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {
    Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else if (ICU_MODE_EDGE_COUNTER == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {   
    if (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EcChlCount_u8) 
    {
      Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, ICU_E_PARAM_CHANNEL);
      return;
    }
    else
    {
      /* Channel identifier is valid */
    }
  }
  else if (ICU_MODE_EDGE_DETECTION == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {   
    if (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) 
    {
      Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, ICU_E_PARAM_CHANNEL);
      return;
    }
    else
    {
      /* Channel identifier is valid */
    }
  }
  else /* if ( ICU_MODE_TIMESTAMP == ICU_SPLIT_CHANNEL(Channel, ICU_MM) ) */
  {
    if (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8) 
    {
      Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, ICU_E_PARAM_CHANNEL);
      return;
    }
    else
    {
      /* Channel identifier is valid */
    }
  }

  /* check ICU_CFG_CHK_PARAM_ACTIVATION */
  if ( (ICU_RISING_EDGE !=  Activation) &&
       (ICU_FALLING_EDGE != Activation) &&
       (ICU_BOTH_EDGES != Activation) )
  {
    Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, ICU_E_PARAM_ACTIVATION);
    return;
  }
  else
  {
    /* Activation edge is valid */
  }
#endif /* (ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
#if (ICU_EDGE_DETECT_API == STD_ON)
  if (ICU_MODE_EDGE_DETECTION == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {
    ICU_ED_CHL_CFG(Channel).Icu_DefaultStartEdge_t = Activation;
    ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE;
      
    /* Update the trigger edge if the Channel is running */
    if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE)
    {
      VttIcu_StartEdgeMonitoring (ICU_ED_HW_CHL(Channel), Activation);
    }

    return;
  }
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */

#if (ICU_TIMESTAMP_API == STD_ON)
  if (ICU_MODE_TIMESTAMP == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {
    ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t = Activation;

    /* Update the trigger edge if the Channel is running */
    if (Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].RunState == TRUE)
    {
      VttIcu_StartTimeMeasurement (ICU_TS_HW_CHL(Channel), ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t, \
                                    ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t);
    }

    return;
  }
#endif /* (ICU_TIMESTAMP_API == STD_ON) */

#if (ICU_EDGE_COUNT_API == STD_ON)
  if (ICU_MODE_EDGE_COUNTER == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {
    ICU_EC_CHL_CFG(Channel).Icu_DefaultStartEdge_t = Activation;

    /* Update the trigger edge if the Channel is running */
    if (ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 == TRUE)
    {
      VttIcu_StartEdgeMonitoring (ICU_EC_HW_CHL(Channel), Activation);
    }

    return;
  }
#endif  /* ( ICU_EDGE_COUNT_API == STD_ON) */

  return;
} /* End of Icu_SetActivationCondition() */


/***********************************************************************************************************************
 *  Icu_DisableNotification()
 *********************************************************************************************************************/
/*! \brief      This service disables the notification on the given Icu channel.
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_TIMESTAMP
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableNotification (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_DISABLE_NOTIFICATION, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) != ICU_MODE_EDGE_DETECTION) &&
       (ICU_SPLIT_CHANNEL(Channel, ICU_MM) != ICU_MODE_TIMESTAMP) )
  {
    Icu_Errorhook (ICU_SID_DISABLE_NOTIFICATION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_EDGE_DETECTION) && 
            (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_DISABLE_NOTIFICATION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_TIMESTAMP) && 
            (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_DISABLE_NOTIFICATION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Correct channel identifier has been given */
  }
#endif  /*(STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
#if (ICU_EDGE_DETECT_API == STD_ON)
  if (ICU_MODE_EDGE_DETECTION == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {
    if ( (Icu_EdChlNotifEnabled_u8 >> ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)) & 0x1 )
    {
      Icu_EdChlNotifEnabled_u8 &= ~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL));
      return;
    }
  }
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */

#if (ICU_TIMESTAMP_API == STD_ON)
  if (ICU_MODE_TIMESTAMP == ICU_SPLIT_CHANNEL(Channel,ICU_MM))
  {
    Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].NotifyInfo = FALSE;
    Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].NotifyCnt = 0;
  }

#endif /* (ICU_TIMESTAMP_API == STD_ON) */

  return;
} /* End of Icu_DisableNotification() */


/***********************************************************************************************************************
 *  Icu_EnableNotification()
 *********************************************************************************************************************/
/*! \brief     This service enables the notification functionality on the given Icu channel. Once the notification
 *              functionality is enabled, pre-configured notification functions are called by the component.
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_TIMESTAMP
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableNotification (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_ENABLE_NOTIFICATION, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) != ICU_MODE_EDGE_DETECTION) &&
       (ICU_SPLIT_CHANNEL(Channel, ICU_MM) != ICU_MODE_TIMESTAMP) )
  {
    Icu_Errorhook (ICU_SID_ENABLE_NOTIFICATION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_EDGE_DETECTION) && 
            (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_ENABLE_NOTIFICATION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_TIMESTAMP) && 
            (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_ENABLE_NOTIFICATION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Correct channel identifier has been given */
  }
#endif  /*(STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
#if (ICU_EDGE_DETECT_API == STD_ON)
  if (ICU_MODE_EDGE_DETECTION == ICU_SPLIT_CHANNEL(Channel, ICU_MM))
  {
    Icu_EdChlNotifEnabled_u8 |= (((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL));
    return;
  }
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */

#if (ICU_TIMESTAMP_API == STD_ON)
  if (ICU_MODE_TIMESTAMP == ICU_SPLIT_CHANNEL(Channel,ICU_MM))
  {
    Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].NotifyCnt = 0;
    Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].NotifyInfo = TRUE;
  }
#endif /* (ICU_TIMESTAMP_API == STD_ON) */

  return;
} /* End of Icu_EnableNotification() */



#if (STD_ON == ICU_GET_INPUT_STATE_API)
/***********************************************************************************************************************
 *  Icu_GetInputState()
 *********************************************************************************************************************/
/*! \brief      This service returns the status of the Icu input. If an activation edge has been detected, the service
 *              will return ICU_ACTIVE, if not, the service will return ICU_IDLE. Once the state has been read, the
 *              service resets the state to ICU_IDLE.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \return     ICU_ACTIVE, ICU_IDLE
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_SIGNAL_MEASUREMENT
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_InputStateType, ICU_CODE) Icu_GetInputState (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  Icu_InputStateType Icu_StatusTemp_tloc = ICU_IDLE;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    /* The module is not initialized */
    Icu_Errorhook (ICU_SID_GET_INPUT_STATE, ICU_E_UNINIT);
    return (ICU_IDLE);
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  #if (ICU_EDGE_DETECT_API == STD_ON)
    if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_EDGE_DETECTION) &&
         (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) < Icu_ConfigSet_pt->Icu_EdChlCount_u8) )
    {
      /* The Edge detection channel is vaild => so go ahead */
    }
    else
  #endif /* (ICU_EDGE_DETECT_API == STD_ON) */

  {
    /* Check for a Signal Measurement channel if this functionality is enabled */
    #if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
      if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_SIGNAL_MEASUREMENT) &&
            (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) < Icu_ConfigSet_pt->Icu_SmChlCount_u8) )
      {
        /* The Signal Measurement channel is vaild => so go ahead */
      }
      else
    #endif /* (ICU_SIGNAL_MEASUREMENT_API == STD_ON) */
    {
      /* Channel isn't within the expeced range */
      Icu_Errorhook (ICU_SID_GET_INPUT_STATE, ICU_E_PARAM_CHANNEL);

      /* Leave the service without any action */
      return (ICU_IDLE);
    }
  }
#endif  /*(STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
#if (ICU_EDGE_DETECT_API == STD_ON)
  if (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_EDGE_DETECTION)
  {
    if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE)
    {
      /* Read the Input State into local variable */
      Icu_StatusTemp_tloc = ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState;
      ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE;
    }
  }
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */

#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
  if (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_SIGNAL_MEASUREMENT)
  {
    if ( (Icu_SmChlRunStates_u8 >> ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)) & 0x1 )
    {
      Icu_StatusTemp_tloc = Icu_ConfigSet_pt->Icu_SmChlStatus_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)];
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)] = ICU_IDLE;
    }
  }
#endif

  /* Return the status of the current channel */
  return (Icu_StatusTemp_tloc);
} /* End Icu_GetInputState() */
#endif /* (STD_ON == ICU_GET_INPUT_STATE_API) */



#if (STD_ON == ICU_TIMESTAMP_API)
/***********************************************************************************************************************
 *  Icu_StartTimestamp()
 *********************************************************************************************************************/
/*! \brief      This service starts the capturing of timer values on the currently configured edges (rising/falling/
 *              both). The values will be written to the pre-configured buffer.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \param[in]  BufferPtr               Pointer to the buffer array, where the timestamp values shall be placed
 *  \param[in]  BufferSize              Size of the external buffer
 *  \param[in]  NotifyInterval          Notification internal (number of events until the next call of the notification)
 *  \param[out] inOut                   Output parameter reference
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be a Timestamping channel.
 *  \pre        BufferPtr must not be NULL_PTR.
 *  \pre        BufferSize must not be '0'.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StartTimestamp 
(
  Icu_ChannelType Channel,
  P2VAR(Icu_ValueType, ICU_VAR, ICU_APPL_DATA) BufferPtr,
  uint16          BufferSize,
  uint16          NotifyInterval
)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint16 counter = 0;
  Icu_TsArrayType *TsData;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_START_TIMESTAMP, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_MODE_TIMESTAMP != ICU_SPLIT_CHANNEL(Channel, ICU_MM)) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_START_TIMESTAMP, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {   /* The channel number is plausible */
  }

  /* check ICU_CFG_CHK_BUFFER_PTR */
  if (NULL_PTR == BufferPtr)
  {
    Icu_Errorhook (ICU_SID_START_TIMESTAMP, ICU_E_PARAM_BUFFER_PTR);
    return;
  }
  else
  {
    /* Buffer pointer seems to be valid */
  }

  /* check ICU_CFG_CHK_PARAM_NOTIFY_INTERVAL */
  if (NotifyInterval > 0)
  {
    /* NotifyInterval is valid => so go ahead */
  }
  else
  {
    Icu_Errorhook (ICU_SID_START_TIMESTAMP, ICU_E_PARAM_NOTIFY_INTERVAL);
    return;
  }

  /* check ICU_CFG_CHK_BUFFER_SIZE */
  if (0 == BufferSize)
  {
    Icu_Errorhook (ICU_SID_START_TIMESTAMP, ICU_E_PARAM_BUFFER_SIZE);
    return;
  }
  else
  {
    /* Buffer size has been set */
  }
#endif  /* (STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Get the TS data set */
  TsData = &Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)];

  /* check if a channel is already mapped to this Hardware */
  if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_TS_HW_CHL(Channel)])
  {
    /* Map the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_TS_HW_CHL(Channel)] = Channel;
  }
  else
  {
    return;
  }
  
  /* Store the pointer to the current application buffer */
  TsData->BufferPtr = BufferPtr;

  /* Reset the buffer index for the current channel */
  TsData->BufferIndex = 0;

  /* Reset the notificaton counter */
  TsData->NotifyCnt = 0;

  /* Disable the notification */
  TsData->NotifyInfo = FALSE;

  /* Store the informations for the current channel
  * (will be used by the interrupt service routine of this driver) */
  TsData->BufferSize = BufferSize;
  TsData->NotifyInterval = NotifyInterval;

  /* Initialize the Buffer with Zeros, needed for the first run of CaplSetTimestamp */
  for (counter = 0; counter < BufferSize; counter++)
  {
    BufferPtr[counter] = 0;
  }

  /* Set The Channel to RunState */
  TsData->RunState = TRUE;

  /* Activate the TimeMeasurement of the VttCntrl_Icu  */
  VttIcu_StartTimeMeasurement (ICU_TS_HW_CHL(Channel), ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t, \
                                                                        ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t);
  
  return;
} /* End of Icu_StartTimestamp() */


/***********************************************************************************************************************
 *  Icu_StopTimestamp()
 *********************************************************************************************************************/
/*! \brief      This service stops the timestamp measurement of the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be a Timestamping channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StopTimestamp (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  Icu_TsArrayType *TsData;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_STOP_TIMESTAMP, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_MODE_TIMESTAMP != ICU_SPLIT_CHANNEL(Channel, ICU_MM)) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_STOP_TIMESTAMP, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* The channel number is plausible */
  }

  /* Get the TS data set */
  TsData = &Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)];

  /* check ICU_CFG_CHK_NOT_STARTED */
  if (TsData->RunState == FALSE)
  {
    Icu_Errorhook (ICU_SID_STOP_TIMESTAMP, ICU_E_NOT_STARTED);
    return;
  }
  else
  {
    /* The channel is running */
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* Stop the Time Measurement of the channel */
  VttIcu_StopTimeMeasurement(ICU_TS_HW_CHL(Channel));

  /* Set Runstate to false to stop the timestamping */
  TsData->RunState = FALSE;

  /* Delete the mapping the ChannelId to the Interrupt Source (Hardware) */
  Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_TS_HW_CHL(Channel)] = ILLEGAL_CHANNEL;

  return;
} /* End of Icu_StopTimestamp() */



/***********************************************************************************************************************
 *  Icu_GetTimestampIndex()
 *********************************************************************************************************************/
/*! \brief      This service reads the timestamp index, which is the next to be written, on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be a Timestamping channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_IndexType, ICU_CODE) Icu_GetTimestampIndex (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  Icu_TsArrayType *TsData;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_GET_TIMESTAMP_INDEX, ICU_E_UNINIT);
    return (0u);
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL*/
  if ( (ICU_MODE_TIMESTAMP != ICU_SPLIT_CHANNEL(Channel, ICU_MM)) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8) )
  {
    /* The channel number is out of range */
    Icu_Errorhook (ICU_SID_GET_TIMESTAMP_INDEX, ICU_E_PARAM_CHANNEL);
    /* The requested service is left without any action */
    return (0u);
  }
  else
  {
    /* The channel number is plausible */
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* Get the TS data set */
  TsData = &Icu_ConfigSet_pt->Icu_TsData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)];

  /* Read the index for the current channel */
  if (TsData->RunState == FALSE)
  {
    return (0u);
  }
  else
  {
    return TsData->BufferIndex;
  }
} /* End of Icu_GetTimestampIndex() */
#endif /* (STD_ON == ICU_TIMESTAMP_API) */


#if (STD_ON == ICU_EDGE_COUNT_API)
/***********************************************************************************************************************
 *  Icu_ResetEdgeCount()
 *********************************************************************************************************************/
/*! \brief      This service resets the counted edges on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_ResetEdgeCount (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_RESET_EDGE_COUNT, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) != ICU_MODE_EDGE_COUNTER) ||
       (ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EcChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_RESET_EDGE_COUNT, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* The Channel is in a valid range */
  }
#endif  /* (STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16 = 0x0000u;

  return;
} /* End of Icu_ResetEdgeCount() */


/***********************************************************************************************************************
 *  Icu_EnableEdgeCount()
 *********************************************************************************************************************/
/*! \brief      This service enables the counting of edges on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableEdgeCount (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_ENABLE_EDGE_COUNT, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_EDGE_COUNTER) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EcChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_ENABLE_EDGE_COUNT, ICU_E_PARAM_CHANNEL);
    return;
  }
#endif  /*( STD_ON == ICU_DEV_ERROR_DETECT) */


  /* ----- Implementation ----------------------------------------------- */
  /* check if a channel is already mapped to this Hardware */
  if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_EC_HW_CHL(Channel)])
  {
    /* Map the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_EC_HW_CHL(Channel)] = Channel;
  }
  else
  {
    return;
  }

  /* Enable the Edge Count Channel */
  ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16 = 0x0000u; 
  ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 = TRUE;

  /* Activate the monitoring of the VttCntrl_Icu  */
  VttIcu_StartEdgeMonitoring (ICU_EC_HW_CHL(Channel), ICU_EC_CHL_CFG(Channel).Icu_DefaultStartEdge_t);

  return;
} /* End of Icu_EnableEdgeCount() */


/***********************************************************************************************************************
 *  Icu_DisableEdgeCount()
 *********************************************************************************************************************/
/*! \brief      This service disables the counting of edges on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableEdgeCount (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_DISABLE_EDGE_COUNT, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_EDGE_COUNTER) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EcChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_DISABLE_EDGE_COUNT, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Parameter Channel is in valid range */
  }
#endif  /*( STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Deactivate the monitoring of the VttCntrl_Icu  */
  VttIcu_StopEdgeMonitoring(ICU_EC_HW_CHL(Channel));

  /* Disable the Edge Count Channel */
  ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16 = 0x0000u;
  ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 = FALSE;

  /* Delete the mapping the ChannelId to the Interrupt Source (Hardware) */
  Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_EC_HW_CHL(Channel)] = ILLEGAL_CHANNEL;

  return;
} /* End of Icu_DisableEdgeCount() */


/***********************************************************************************************************************
 *  Icu_GetEdgeNumbers()
 *********************************************************************************************************************/
/*! \brief      This service reads the number of counted edges of the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \return     Edge number
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_EdgeNumberType, ICU_CODE) Icu_GetEdgeNumbers (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_GET_EDGE_NUMBERS, ICU_E_UNINIT);
    return (0x0000u);
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  /*  The parameter Channel shall be checked */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_EDGE_COUNTER) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EcChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_GET_EDGE_NUMBERS, ICU_E_PARAM_CHANNEL);
    return (0x0000u);
  }
  else
  {
    /* Parameter Channel is in valid range */
  }
#endif  /*( STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Return the counted edges */
  return (Icu_EdgeNumberType)(ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16);
}   /* Icu_GetEdgeNumbers() */
#endif /* (STD_ON == ICU_EDGE_COUNT_API) */


#if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
/***********************************************************************************************************************
 *  Icu_StartSignalMeasurement()
 *********************************************************************************************************************/
/*! \brief      This service starts the capturing of a periodic signal on the given channel. The capturing itself
 *              starts on the first 'default start edge', which occurrs after the call of this function.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StartSignalMeasurement (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_START_SIG_MEASUREMENT, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_SIGNAL_MEASUREMENT) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_SmChlCount_u8) ) 
  {
    Icu_Errorhook (ICU_SID_START_SIG_MEASUREMENT, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* The channel number is plausible */
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* check if a channel is already mapped to this Hardware */
  if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_SM_HW_CHL(Channel)])
  {
    /* Map the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_SM_HW_CHL(Channel)] = Channel;
  }
  else
  {
    return;
  }

  /* Set the appropriate bit, to indicate that this channel is running */
  Icu_SmChlRunStates_u8 |= ( ((uint64)1) << ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL) );

  /* no coherent data is available at the start of Signal Measurement*/
  Icu_NewSmValueAvailable &= (~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)));

  /* Set the appropriate bits, to indicate that it is the first measurement of the channel */
  Icu_SmFirstMeasurement |= ( ((uint64)1) << ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL) );
  Icu_SmRunning &= (~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)));

  /* Setting up the period and active time to 0 */
  Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].ActiveTime = 0;
  Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].PeriodTime = 0;

  /* Activiate the SignalMeasurement in the VttCntrl */
  /* Dutycycle */
  if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_DUTY_CYCLE )
  {
    if (ICU_SM_CHL_CFG(Channel).Icu_SignalStartEdge_t == ICU_FALLING_EDGE )
    {
      VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_FALLING_EDGE, ICU_BOTH_EDGES);
    }
    else
    {
      VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_RISING_EDGE, ICU_BOTH_EDGES);
    }
  }

  /* HighTime */
  if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_HIGH_TIME )
  {
    VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_RISING_EDGE, ICU_BOTH_EDGES);
  }

  /* LowTime */
  if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_LOW_TIME )
  {
    VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_FALLING_EDGE, ICU_BOTH_EDGES);
  }

  /* PeriodTime */
  if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_PERIOD_TIME)
  {
    if (ICU_SM_CHL_CFG(Channel).Icu_SignalStartEdge_t == ICU_FALLING_EDGE )
    {
      VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_FALLING_EDGE, ICU_FALLING_EDGE);
    }
    else
    {
      VttIcu_StartTimeMeasurement (ICU_SM_HW_CHL(Channel), ICU_RISING_EDGE, ICU_RISING_EDGE);
    }
  }

  /* Reset channel's state to ICU_IDLE */
#if (ICU_GET_INPUT_STATE_API == STD_ON)
  Icu_ConfigSet_pt->Icu_SmChlStatus_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)] = ICU_IDLE;
#endif
  
  return;
} /* End of Icu_StartSignalMeasurement() */


/***********************************************************************************************************************
 *  Icu_StopSignalMeasurement()
 *********************************************************************************************************************/
/*! \brief      This service stops the capturing of a periodic signal on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StopSignalMeasurement (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_STOP_SIG_MEASUREMENT, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check  ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_SIGNAL_MEASUREMENT) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_SmChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_STOP_SIG_MEASUREMENT, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Channel is in valid range*/
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* deactivate the appropriate channel in VttCntrl */
  VttIcu_StopTimeMeasurement(ICU_SM_HW_CHL(Channel));

  /* Clear the appropriate bit, to indicate that this channel is stopped */
  Icu_SmChlRunStates_u8 &= (~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)));

  /* Clear the appropriate bit, to indicate that no value is available */
  Icu_NewSmValueAvailable &= (~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)));

  /* Delete the mapping the ChannelId to the Interrupt Source (Hardware) */
  Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_SM_HW_CHL(Channel)] = ILLEGAL_CHANNEL;

#if (ICU_GET_INPUT_STATE_API == STD_ON)
  /* Reset channel's state to ICU_IDLE */
  Icu_ConfigSet_pt->Icu_SmChlStatus_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)] = ICU_IDLE;
#endif

  return;
} /* End of Icu_StopSignalMeasurement() */
#endif /* (STD_ON == ICU_SIGNAL_MEASUREMENT_API) */


#if (STD_ON == ICU_EDGE_DETECT_API)
/***********************************************************************************************************************
 *  Icu_StartSignalMeasurement()
 *********************************************************************************************************************/
/*! \brief      This service starts edgte detection on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableEdgeDetection (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_ENABLE_EDGE_DETECTION, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_EDGE_DETECTION) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) )
  {
    /* The channel number is out of range */
    Icu_Errorhook (ICU_SID_ENABLE_EDGE_DETECTION, ICU_E_PARAM_CHANNEL);
    /*  The requested service is left without any action*/
    return;
  }
  else
  {
    /* The channel number is plausible */
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* check if a channel is already mapped to this Hardware */
  if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_ED_HW_CHL(Channel)])
  {
    /* Map the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_ED_HW_CHL(Channel)] = Channel;
  }
  else
  {
    return;
  }

  /* Set the channel active */
  ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 = TRUE;

  /* Set the ED channel input state to idle*/
  ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE; 

  /* Activate the monitoring of the VttCntrl_Icu  */
  VttIcu_StartEdgeMonitoring (ICU_ED_HW_CHL(Channel), ICU_ED_CHL_CFG(Channel).Icu_DefaultStartEdge_t);
  
  return;
} /* End of Icu_EnableEdgeDetection() */


/***********************************************************************************************************************
 *  Icu_DisableEdgeDetection()
 *********************************************************************************************************************/
/*! \brief      This service stops the edge detection on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableEdgeDetection (Icu_ChannelType Channel)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_DISABLE_EDGE_DETECTION, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_EDGE_DETECTION) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8) )
  {
    Icu_Errorhook (ICU_SID_DISABLE_EDGE_DETECTION, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* The channel number is plausible */
  }
#endif  /* (STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Deactivate the monitoring of the VttCntrl_Icu  */
  VttIcu_StopEdgeMonitoring(ICU_ED_HW_CHL(Channel));

  /* Set the channel inactive */
  ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 = FALSE;
  /* Set the ED channel input state to idle*/
  ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE;

  /* delete the mapping the ChannelId to the Interrupt Source (Hardware) */
  Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_ED_HW_CHL(Channel)] = ILLEGAL_CHANNEL;
  
  return;
} /* End of Icu_DisableEdgeDetection() */
#endif /* (STD_ON == ICU_EDGE_DETECT_API) */


#if (STD_ON == ICU_GET_TIME_ELAPSED_API) /* Only effective in mode 'Signal Measurement' */
/***********************************************************************************************************************
 *  Icu_GetTimeElapsed()
 *********************************************************************************************************************/
/*! \brief      This service reads the elapsed Signal Low Time for the given channel that is configured in Measurement
 *              Mode 'Signal Measurement, Signal Low Time'. The elapsed time is measured between a falling edge and the
 *              consecutive rising edge of the channel.
 *              This service reads the elapsed Signal High Time for the given channel that is configured in Measurement
 *              Mode 'Signal Measurement, Signal High Time'. The elapsed time is measured between a rising edge and the
 *              consecutive falling edge of the channel.
 *              This service reads the elapsed Period Time for the given channel that is configured in Measurement
 *              Mode 'Signal Measurement, Signal Period Time'. The elapsed time is measured between consecutive rising
 *              (or falling) edges of the channel. The period start edge is configurable.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \return     Low time/high time/period time
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_ValueType, ICU_CODE) Icu_GetTimeElapsed (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint32  Icu_TempResult_u32loc = 0;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    Icu_Errorhook (ICU_SID_GET_TIME_ELAPSED, ICU_E_UNINIT);
    return (0);
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ((&ICU_SM_CHL_CFG(Channel)) == NULL_PTR)
  {
    Icu_Errorhook (ICU_SID_GET_TIME_ELAPSED, ICU_E_PARAM_CHANNEL);
    return (0);
  }

  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_SIGNAL_MEASUREMENT) ||
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_SmChlCount_u8) ||
       ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)    == ICU_MODE_SIGNAL_MEASUREMENT) && 
         ( (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_HIGH_TIME) && 
           (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_LOW_TIME)  && 
           (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_PERIOD_TIME)) ) )
  {
    Icu_Errorhook (ICU_SID_GET_TIME_ELAPSED, ICU_E_PARAM_CHANNEL);
    return (0);
  }
  else
  {
    /* The channel number is plausible */
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */


  /* ----- Implementation ----------------------------------------------- */
  Icu_TempResult_u32loc = 0;
  if ( (Icu_NewSmValueAvailable >> ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)) & 0x1 )
  {
    /* Measurement has completed and the driver is able to provide information on the input signal  */
    /* reset the new-value-condition */
    Icu_NewSmValueAvailable &= (~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)));
    Icu_ConfigSet_pt->Icu_SmChlStatus_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)] = ICU_IDLE;

    if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_LOW_TIME)
    {
      Icu_TempResult_u32loc = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)].ActiveTime;
    }
    if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_HIGH_TIME)
    {
      Icu_TempResult_u32loc = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL)].ActiveTime;
    }
    if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_PERIOD_TIME)
    {
      Icu_TempResult_u32loc = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL)].PeriodTime;
    }
  }

  return ((Icu_ValueType)(Icu_TempResult_u32loc & 0xFFffFFff));
} /* End of Icu_GetTimeElapsed() */
#endif /* (STD_ON == ICU_GET_TIME_ELAPSED_API) */


#if (STD_ON == ICU_GET_DUTY_CYCLE_VALUES_API)
/***********************************************************************************************************************
 *  Icu_GetDutyCycleValues()
 *********************************************************************************************************************/
/*! \brief      This service reads the coherent active time and period time for the given ICU Channel, if it is
 *              configured in Measurement Mode 'Signal Measurement, Duty Cycle Values'.
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \param[in]  DutyCycleValues         Pointer to a buffer where the results (high time and period time) shall be
 *                                      placed
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \pre        DutyCycleValues must not be NULL_PTR.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_GetDutyCycleValues
(
  Icu_ChannelType     Channel,
  P2VAR(Icu_DutyCycleType, ICU_VAR, ICU_APPL_DATA) DutyCycleValues
)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* check ICU_CFG_CHK_UNINIT */
  if (FALSE == Icu_ModuleInitialized_b)
  {
    DutyCycleValues->ActiveTime = 0;  /* Return '0' */
    DutyCycleValues->PeriodTime = 0;  /* Return '0' */

    Icu_Errorhook (ICU_SID_GET_DUTY_CYCLE_VALUES, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* Module is initialized */
  }

  /* check ICU_CFG_CHK_BUFFER_PTR */
  if (NULL_PTR == DutyCycleValues)
  {
    Icu_Errorhook (ICU_SID_GET_DUTY_CYCLE_VALUES, ICU_E_PARAM_BUFFER_PTR);
    return;
  }
  else
  {
    /* A valid buffer pointer has been given */
  }

  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  if ((&ICU_SM_CHL_CFG(Channel)) == NULL_PTR)
  {
    DutyCycleValues->ActiveTime = 0;  /* Return '0' */
    DutyCycleValues->PeriodTime = 0;  /* Return '0' */

    Icu_Errorhook (ICU_SID_GET_DUTY_CYCLE_VALUES, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* Channel is in valid range */
  }

  if ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM)      != ICU_MODE_SIGNAL_MEASUREMENT) || 
       (ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL) >= Icu_ConfigSet_pt->Icu_SmChlCount_u8) || 
       ( (ICU_SPLIT_CHANNEL(Channel, ICU_MM) == ICU_MODE_SIGNAL_MEASUREMENT) && 
         (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_DUTY_CYCLE) ) )
  {
    DutyCycleValues->ActiveTime = 0;  /* Return '0' */
    DutyCycleValues->PeriodTime = 0;  /* Return '0' */
    
    Icu_Errorhook (ICU_SID_GET_DUTY_CYCLE_VALUES, ICU_E_PARAM_CHANNEL);
    return;
  }
  else
  {
    /* The channel number is plausible */
  }
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  if ( (Icu_NewSmValueAvailable >> ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)) & 0x1 )
  {
    /* Measurement has completed and the driver is able to provide information on the input signal  */
    /* reset the new-value-condition */
    Icu_NewSmValueAvailable &= (~(((uint64)1) << ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)));
    Icu_ConfigSet_pt->Icu_SmChlStatus_at[ICU_SPLIT_CHANNEL(Channel, ICU_SPL_CHL)] = ICU_IDLE;

    /* Fill the dutycycle values with the new data */
    DutyCycleValues->ActiveTime = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL)].ActiveTime;
    DutyCycleValues->PeriodTime = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ICU_SPLIT_CHANNEL(Channel,ICU_SPL_CHL)].PeriodTime;
  }
  else
  {
    /* Return a default: 0 - no new value was measured */
    DutyCycleValues->ActiveTime = 0;
    DutyCycleValues->PeriodTime = 0;
  }

  return;
} /* End of Icu_GetDutyCycleValues() */
#endif /* (STD_ON == ICU_GET_DUTY_CYCLE_VALUES_API) */


#if (STD_ON == ICU_GET_VERSION_INFO_API)
/***********************************************************************************************************************
 *  Icu_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module. The version information includes:
 *              - Module Id
 *              - Vendor Id
 *              - Vendor specific version numbers
 *  \param[out] versioninfo             Pointer to where to store the version information of this module
 *  \pre        Versioninfo parameter must not be NULL_PTR.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, ICU_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* Check the versioninfo pointer for being a NULL_PTR */
  if (NULL_PTR == versioninfo)
  {
    Icu_Errorhook (ICU_SID_GET_VERSION_INFO, ICU_E_PARAM_VINFO);
    return;
  }
  else
  {
    /* versioninfo is defined */
  }
#endif /* (STD_ON == ICU_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID         = (ICU_VENDOR_ID);
  versioninfo->moduleID         = (ICU_MODULE_ID);
  versioninfo->sw_major_version = (ICU_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (ICU_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (ICU_SW_PATCH_VERSION);

  return;
}   /* Icu_GetVersionInfo() */
#endif /* (STD_ON == ICU_GET_VERSION_INFO_API) */


#if (STD_ON == ICU_REPORT_WAKEUP_SOURCE)
/***********************************************************************************************************************
 *  Icu_CheckWakeup
 *********************************************************************************************************************/
/*! \brief      This service checks, id the wakeup source was correct
 *  \param[in]  WakeupSource           Reported wakeup source
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_CheckWakeup (EcuM_WakeupSourceType WakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  int loop;
  EcuM_WakeupSourceType TempWakeupSource;

#if (STD_ON == ICU_DEV_ERROR_DETECT)
  /* Check if the module is initialized */
  if (NULL_PTR == Icu_ConfigSet_pt )
  {
    Icu_Errorhook (ICU_SID_CHECK_WAKEUP, ICU_E_UNINIT);
    return;
  }
  else
  {
    /* module is initialized */
  }
#endif /* (STD_ON == ICU_DEV_ERROR_DETECT) */

  /*In case the expected wk source Id was found, the referenced EcuM function will be called*/
  for (loop = 0; loop < Icu_ConfigSet_pt->Icu_EdChlCount_u8 ; loop++)
  {
    TempWakeupSource = (((uint64)1) << ICU_ED_CHL_CFG(loop).Icu_WakeupSource_u32);

    if ((WakeupSource == TempWakeupSource) && ((Icu_WakeupActive >> ICU_SPLIT_CHANNEL(loop,ICU_SPL_CHL))& 0x01))
    {
      EcuM_SetWakeupEvent (TempWakeupSource);  
      break;
    }
    
  }
  
  return;
} /* End of Icu_CheckWakeup() */
#endif /* (STD_ON == ICU_REPORT_WAKEUP_SOURCE) */


FUNC(void, ICU_CODE) Icu_IsrHandler (Icu_InterruptSourceType Irq_Source)
{
  Icu_ChannelType Channel = Icu_ConfigSet_pt->Icu_IsrMappingTable_at[Irq_Source];
  Icu_MeasurementModeType ModeOfChannel = (Icu_MeasurementModeType)ICU_SPLIT_CHANNEL(Channel, ICU_MM);

  if (Channel == ILLEGAL_CHANNEL)
  {
    /* no valid channel is registered in Icu_ConfigSet_pt->Icu_IsrMappingTable_at[] */
    return;
  }

  switch(ModeOfChannel)
  {
    case ICU_MODE_EDGE_DETECTION:
      #if (STD_ON == ICU_EDGE_DETECT_API)
        Icu_ActionForEd (Channel);
      #endif
      break;
    case ICU_MODE_EDGE_COUNTER:
      #if (STD_ON == ICU_EDGE_COUNT_API)
        Icu_ActionForEc (Channel);
      #endif
      break;
    case ICU_MODE_SIGNAL_MEASUREMENT:
      #if (STD_ON == ICU_SIGNAL_MEASUREMENT_API)
        Icu_ActionForSm (Channel);
      #endif
      break;
    case ICU_MODE_TIMESTAMP:
      #if (STD_ON == ICU_TIMESTAMP_API)
        Icu_ActionForTs (Channel);
      #endif
      break;
    default:
      break;
  }
}


/**********************************************************************************************************************
 *  END OF FILE: Icu.c
 *********************************************************************************************************************/
