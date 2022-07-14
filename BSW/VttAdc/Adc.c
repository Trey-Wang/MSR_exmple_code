/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Adc.c
 *    Component:  DrvAdc_VttCanoe01Asr
 *       Module:  MICROSAR Adc
 *    Generator:  -
 *
 *  Description:  MICROSAR CAL Module Adc implements a driver for generating a digital code from an analogue voltage
 *                signal.
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
 
#define ADC_SOURCE
  
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Adc.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Adc header file */
#if (  (ADC_SW_MAJOR_VERSION != (2U)) \
    || (ADC_SW_MINOR_VERSION != (5U)) \
    || (ADC_AR_RELEASE_MAJOR_VERSION != (4U)) \
    || (ADC_AR_RELEASE_MINOR_VERSION != (0U)) \
    || (ADC_AR_RELEASE_PATCH_VERSION != (3U)) )
  #error "Vendor specific version numbers of Adc.c and Adc.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ADC_CFG_MAJOR_VERSION != (3U)) \
    || (ADC_CFG_MINOR_VERSION != (3U)) )
  #error "Version numbers of Adc.c and Adc_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* This identifier is the invalid group ID */
#define ADC_INVALID_GRP_ID              (0xFF)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define ADC_GROUP_CFG(group)            (Adc_ConfigPtr_pt->Group_at[group])
#define ADC_GROUP_DATA_PTR(group)       (ADC_GROUP_CFG(group).GroupData_pt)

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
/* Adc_ConfigPtr_pt keeps the reference to the post-build configuration set, which is passed to function Adc_Init() */
STATIC P2CONST(Adc_ConfigType, AUTOMATIC, ADC_APPL_CONST) Adc_ConfigPtr_pt = NULL_PTR;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
STATIC FUNC(void, ADC_CODE) Adc_StartUnit (Adc_GroupType group);
STATIC FUNC(void, ADC_CODE) Adc_ProcessBuffer (Adc_GroupType group);
STATIC FUNC(void, ADC_CODE) Adc_StateTransitionAfterBufferRead (Adc_GroupType group);



/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Adc_StartUnit
 *********************************************************************************************************************/
/*! \brief      Adc_StartUnit
 *
 *              This function configures the hardware to start the Adc conversions.
 *  \param[in]  Group                   Numeric Id of the Group within the runtime
 *  \context    The function can be called on task level.
 *  \note
 *********************************************************************************************************************/
STATIC FUNC(void, ADC_CODE) Adc_StartUnit (Adc_GroupType Group)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Set group data variables */
  /* Reset countdown variable to the number of stream samples, that fit in application result buffer */
  ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t = ADC_GROUP_CFG(Group).StreamingNumSamples_t;
  /* Reset sample counter. No valid samples shall be in the buffer when group starts. */
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = 0u;
  /* Reset the stored values for reads in idle state */
  ADC_GROUP_DATA_PTR(Group)->SampleCountdownInIdle_t = 0;
  ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t = 0;
  /* Toggle group state to ADC_BUSY */
  ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_BUSY;

  if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    if (ADC_CONV_MODE_ONESHOT == ADC_GROUP_CFG(Group).GroupConversionMode_u8)
    {
      VttAdc_StartOneShotImmediate (Group);
    }
    else /* ADC_CONV_MODE_CONTINUOUS == ADC_GROUP_CFG(Group).GroupConversionMode_u8 */
    {
      VttAdc_StartContinuous (Group);
    }
  }
  else /* ADC_TRIGG_SRC_HW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8 */
  {
    VttAdc_StartOneShotHwTriggered (Group, ADC_GROUP_CFG(Group).HwTrigEvent_u8);
  }

  return;
} /* Adc_StartUnit() */


/**********************************************************************************************************************
 * Adc_ProcessBuffer
 *********************************************************************************************************************/
/*! \brief      Adc_ProcessBuffer
 *
 *              This method copies results from hardware fifo buffer to application result buffer.
 *              This function is called in ISR context
 *
 *  \param[in]  group   group id where it can be referenced to the relevant group configuration and group data variables
 *
 *  \context    This function can be called on task and interrupt level.
 *********************************************************************************************************************/
STATIC FUNC(void, ADC_CODE) Adc_ProcessBuffer (Adc_GroupType group)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  ChCounter_u8;
  uint16 ChOffset_u16;
  uint16 CurrSample_u16;
  Adc_ValueGroupRefType ChannelValue = NULL_PTR;
  
  /* ----- Implementation ----------------------------------------------- */
  /* Index of the sample that is copied with this function. */
  CurrSample_u16 = ADC_GROUP_CFG(group).StreamingNumSamples_t -
                                        ADC_GROUP_DATA_PTR(group)->SampleCountdown_t;

  /* read back the data of the latest adc conversion */
  VttAdc_GetAdcValue (group, &ChannelValue);

  /* Loop all channels of the current sample */
  for (ChCounter_u8 = 0u; ChCounter_u8 < ADC_GROUP_CFG(group).SeqLength_u8; ChCounter_u8++)
  {
    /* This is the offset of the first entry of the current channel relative to the first entry of channel 0 */
    ChOffset_u16 = ADC_GROUP_CFG(group).StreamingNumSamples_t * ChCounter_u8;

    /* Store one conversion result in the application result buffer. */
    ADC_GROUP_DATA_PTR(group)->ResultBuffer_pt[ChOffset_u16 + CurrSample_u16] = ChannelValue[ChCounter_u8];
  }
  return;
} /* Adc_ProcessBuffer() */


/**********************************************************************************************************************
 * Adc_StateTransitionAfterBufferRead
 *********************************************************************************************************************/
/*! \brief      Adc_StateTransitionAfterBufferRead
 *
 *              This method is responsible for a transition of the group status after a buffer read access. This access
 *              can be caused by Adc_ReadGroup or Adc_GetStreamLastPointer.
 *
 *  \param[in]  CurrentGroupConfigPtr           Pointer to the relevant group configuration and group data variables
 *
 *  \context    This function can be called on task and interrupt level.
 *********************************************************************************************************************/
STATIC FUNC(void, ADC_CODE) Adc_StateTransitionAfterBufferRead (Adc_GroupType group)
{
  /* ----- Implementation ----------------------------------------------- */
  if (ADC_COMPLETED == ADC_GROUP_DATA_PTR(group)->Status_t)
  {
  /* Conversion has not been completely finished, when the group was read. State transition to ADC_BUSY (continue
   * converting the remaining samples).
   */
    ADC_GROUP_DATA_PTR(group)->Status_t = ADC_BUSY;
  }
  else if (ADC_STREAM_COMPLETED == ADC_GROUP_DATA_PTR(group)->Status_t)
  {
    /* Conversion of all samples has finished before the group was read.
     * State transition to ADC_IDLE if the group stops implicitly or
     *                  to ADC_BUSY if the group has to be stopped explicitly.
     */
    if (ADC_GROUP_STOP_EXPLICIT == ADC_GROUP_CFG(group).GroupStopMode_u8)
    {
      /* Group has to be stopped explicitly, so conversion is going on after this read. */
      ADC_GROUP_DATA_PTR(group)->Status_t = ADC_BUSY;
    }
    else
    {
      /* Group stops implicitly when ADC_STREAM_COMPLETED is reached and the group is read. After a read, this group's
       * status is ADC_IDLE
       */
      ADC_GROUP_DATA_PTR(group)->Status_t = ADC_IDLE;

      *(ADC_GROUP_DATA_PTR(group)->GroupAssignment_pt) = ADC_INVALID_GRP_ID;
    }
  }
  else
  {
    /* Conversion is either still ongoing or group is not started, so do nothing */
  }
  return;
} /* Adc_StateTransitionAfterBufferRead() */



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Adc_InitMemory
 *********************************************************************************************************************/
/*! \brief      This service initializes the global variables in case the startup code does not work
 *  \param[in]  void
 *  \return     void
 *  \pre        Module must not be initialized
 *  \context    Function is called during startup
 *  \note
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Reset configuration pointer. All other variables are reset in Adc_Init() */
  Adc_ConfigPtr_pt = NULL_PTR;
  
  return;
} /* End of Adc_InitMemory */


/**********************************************************************************************************************
 *  Adc_Init
 *********************************************************************************************************************/
/*! \brief      This service initializes the driver and the Adc hardware
 *  \param[in]  ConfigPtr                      Input parameter
 *  \return     void
 *  \pre        Input parameter must not be NULL.
 *  \context    Function is called during startup
 *  \note
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_Init_Vtt (P2CONST(Adc_ConfigType, AUTOMATIC, ADC_APPL_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 GroupCounter_u8;
  uint8 HwUnitCounter_u8;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is already initialized */ 
  if (NULL_PTR != Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_INIT, ADC_E_ALREADY_INITIALIZED);
    return;
  }
  else
  {
    /* Adc has not been initialized before: continue */
  }

  /* check ConfigPtr not being the NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    Adc_ReportDet (ADC_SID_INIT, ADC_E_PARAM_CONFIG);
    return;
  }
  else
  {
    /* ConfigPtr seems to be valid: continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Store parameter 'ConfigPtr' in Adc_ConfigPtr_pt for use in other API functions */
  Adc_ConfigPtr_pt = ConfigPtr;

  /* Initialize all ActiveGroups vars in use. */
  for (HwUnitCounter_u8 = 0; HwUnitCounter_u8 < Adc_ConfigPtr_pt->NumberOfHwUnits_t; HwUnitCounter_u8++)
  {
    Adc_ConfigPtr_pt->ActiveGroupRtx_at[HwUnitCounter_u8] = ADC_INVALID_GRP_ID;
  }

  /* Initialize all groups in use */
  for (GroupCounter_u8 = 0; GroupCounter_u8 < Adc_ConfigPtr_pt->NumberOfGroups_t; GroupCounter_u8++)
  {
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->ResultBuffer_pt    = NULL_PTR;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->SampleCountdown_t  = 0u;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->ValidSampleCount_t = 0u;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->SampleCountdownInIdle_t = 0;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->ValidSamplesInIdle_t = 0;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->Status_t           = ADC_IDLE;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->Notification_u8    = ADC_NOTIFICATION_OFF;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->GroupAssignment_pt = 
                                &(Adc_ConfigPtr_pt->ActiveGroupRtx_at[ADC_GROUP_CFG(GroupCounter_u8).HwUnitOfTheGroup]);
    /* Hardware trigger cannot be disabled at this time. */
  }

  return;
} /* End of Adc_Init */


/**********************************************************************************************************************
 * Adc_SetupResultBuffer
 *********************************************************************************************************************/
/*! \brief      Result Buffer Setup Service
 *
 *              This method is the Adc group result buffer setup service. It is responsible for setting up the result
 *              buffer pointer of the group 'Group'.
 *
 *  \param[in]  Group             Numeric Id of requested Adc channel group.
 *  \param[in]  DataBufferPtr     Adc result buffer pointer is initialized with the value of the
 *                                data buffer pointer.
 *
 *  \return     E_OK              Result buffer pointer initialized correctly.
 *  \return     E_NOT_OK          Operation failed or development error occured.
 *
 *  \pre        The module has to be initialized.
 *
 *  \context    This function can be called on task level.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ADC_CODE) Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupRefType DataBufferPtr)
{  
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_SETUP_RESULT_BUFFER, ADC_E_UNINIT);
    return E_NOT_OK;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }


  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_SETUP_RESULT_BUFFER, ADC_E_PARAM_GROUP);
    return E_NOT_OK;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check 'DataBufferPtr' not being the NULL_PTR */
  if (NULL_PTR == DataBufferPtr)
  {
    Adc_ReportDet (ADC_SID_SETUP_RESULT_BUFFER, ADC_E_PARAM_RESULT_BUFFER);
    return E_NOT_OK;
  }
  else
  {
    /* Parameter 'DataBufferPtr' does not reference NULL_PTR: continue */
  }

  /* check the group status */
  if (ADC_IDLE != ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    Adc_ReportDet (ADC_SID_SETUP_RESULT_BUFFER, ADC_E_BUSY);
    return E_NOT_OK;
  }
  else
  {
    /* group is in state ADC_IDLE: continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  ADC_GROUP_DATA_PTR(Group)->ResultBuffer_pt = DataBufferPtr;

  return E_OK;
} /* End of Adc_SetupResultBuffer() */


#if (STD_ON == ADC_DE_INIT_API)
/**********************************************************************************************************************
 *  Adc_DeInit
 *********************************************************************************************************************/
/*! \brief      This method is the Adc_DeInit() service. It deinitializes the Adc peripheral and sets it to a status
 *              comparable to its power on status.
 *  \param[in]  void
 *  \return     void
 *  \pre        Adc_Init() must be called.
 *  \context    Function could be called from task level
 *  \note
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 GroupCounter_u8;
  uint8 HwUnitCounter_u8;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */ 
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_DEINIT, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc has been initialized before: continue */
  }

  /* Check if any of the groups is still converting. If so, raise error to DET and return. Therefore check group
   * assignment of all activated hardware units.
   */
  for (HwUnitCounter_u8 = 0; HwUnitCounter_u8 < Adc_ConfigPtr_pt->NumberOfHwUnits_t; HwUnitCounter_u8++)
  {
    if (ADC_INVALID_GRP_ID != Adc_ConfigPtr_pt->ActiveGroupRtx_at[HwUnitCounter_u8])
    {
      Adc_ReportDet (ADC_SID_DEINIT, ADC_E_BUSY);
      return;
    }
    else
    {
      /* This unit is not busy: continue with loop */
    }
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Deinitialize all registers of all HW units in use. */
  for (HwUnitCounter_u8 = 0; HwUnitCounter_u8 < Adc_ConfigPtr_pt->NumberOfHwUnits_t; HwUnitCounter_u8++)
  {
    Adc_ConfigPtr_pt->ActiveGroupRtx_at[HwUnitCounter_u8] = ADC_INVALID_GRP_ID;
  }

  /* Deinitialize all groups in use */
  for (GroupCounter_u8 = 0; GroupCounter_u8 < Adc_ConfigPtr_pt->NumberOfGroups_t; GroupCounter_u8++)
  {
    /* Reset all runtime variables. */
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->ResultBuffer_pt    = NULL_PTR;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->SampleCountdown_t  = 0u;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->ValidSampleCount_t = 0u;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->SampleCountdownInIdle_t = 0;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->ValidSamplesInIdle_t    = 0;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->Status_t           = ADC_IDLE;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->Notification_u8    = ADC_NOTIFICATION_OFF;
    ADC_GROUP_DATA_PTR(GroupCounter_u8)->GroupAssignment_pt = NULL_PTR;
  }

  /* Reset the global Adc_ConfigPtr_pt, which is used for ADC_E_UNINIT check. */
  Adc_ConfigPtr_pt = NULL_PTR;
  return;
} /* End of Adc_DeInit() */
#endif /* (ADC_DE_INIT_API == STD_ON) */


#if (STD_ON == ADC_ENABLE_START_STOP_GROUP_API)
/**********************************************************************************************************************
 * Adc_StartGroupConversion
 *********************************************************************************************************************/
/*! \brief      Start group conversion
 *
 *              This method starts a software triggered conversion of all channels of the requested Adc channel group.
 *
 *  \param[in]  Group             Numeric Id of requested Adc channel group.
 *
 *  \pre        The module has to be initialized.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_StartGroupConversion(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_START_GROUP_CONVERSION, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }


  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_START_GROUP_CONVERSION, ADC_E_PARAM_GROUP);
    return;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if the trigger source is ADC_TRIGG_SRC_HW */
  if (ADC_TRIGG_SRC_HW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    Adc_ReportDet (ADC_SID_START_GROUP_CONVERSION, ADC_E_WRONG_TRIGG_SRC);
    return;
  }
  else
  {
    /* 'Group' seems to be a Sw triggered group: continue */
  }

  /* check if ResultBuffer_pt has not been initialized before */
  if (NULL_PTR == ADC_GROUP_DATA_PTR(Group)->ResultBuffer_pt)
  {
    Adc_ReportDet (ADC_SID_START_GROUP_CONVERSION, ADC_E_BUFFER_UNINIT);
    return;
  }
  else
  {
    /* Result buffer seems to be initialized: continue */
  }


  /* Check if a group was started on the requested hardware group. */
  if (ADC_INVALID_GRP_ID == *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt))
  {
    /* Currently there is no group converting on the requested HW unit. */
  }
  else
  {
    /* The group that was started before has not finished until now. */
    Adc_ReportDet (ADC_SID_START_GROUP_CONVERSION, ADC_E_BUSY);
    return;
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */
  
  /* ----- Implementation ----------------------------------------------- */
  /* Fill in group assignment table, to see which group was started last on the HW conversion group. */
  *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt) = Group;

  /* Start the hardware conversion unit. */
  Adc_StartUnit(Group);

  return;
} /* End of Adc_StartGroupConversion() */


/**********************************************************************************************************************
 * Adc_StopGroupConversion
 *********************************************************************************************************************/
/*! \brief      Stop group conversion
 *
 *              This method stops a software triggered group conversion.
 *
 *  \param[in]  Group           Numeric Id of requested Adc channel group.
 *
 *  \pre        The module has to be initialized. A group conversion shall be ongoing.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_StopGroupConversion(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_STOP_GROUP_CONVERSION, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_STOP_GROUP_CONVERSION, ADC_E_PARAM_GROUP);
    return;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if the trigger source is ADC_TRIGG_SRC_HW */
  if (ADC_TRIGG_SRC_HW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    Adc_ReportDet (ADC_SID_STOP_GROUP_CONVERSION, ADC_E_WRONG_TRIGG_SRC);
    return;
  }
  else
  {
    /* 'Group' seems to be a Sw triggered group: continue */
  }

  /* Check, if the group is active. Only an active, non-idling group can be stopped. */
  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    /* Current Group is in state ADC_IDLE: cannot be stopped. */
    Adc_ReportDet (ADC_SID_STOP_GROUP_CONVERSION, ADC_E_IDLE);
    return;
  }
  else
  {
    /* Group is not in state ADC_IDLE: continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Get the index of the software group that was started last on this hardware group. Affect HW only when the current
   * group 'Group' was started last on this HW group and was not stopped in the meantime.
   */
  if (Group == *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt))
  {
    /* Stop conversion. This is equal to stopping a real hardware unit. */
    VttAdc_StopConversion (Group);

    /* Reset GroupAssignment_t to default state, because HW is not in use anymore */
    *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt) = ADC_INVALID_GRP_ID;
  }
  else
  {
    /* Don't modify HW, because group 'Group' is not on the HW anymore. */
  }

  /* Disable group notification */
  ADC_GROUP_DATA_PTR(Group)->Notification_u8 = ADC_NOTIFICATION_OFF;

  /* Set group state to ADC_IDLE */
  ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_IDLE;

  /* Reset and store ValidSampleCount and SampleCountdown */
  ADC_GROUP_DATA_PTR(Group)->SampleCountdownInIdle_t = ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t;
  ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t = ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t;

  ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t = ADC_GROUP_CFG(Group).StreamingNumSamples_t;
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = 0u;

  return;
} /* End of Adc_StopGroupConversion() */
#endif /* (STD_ON == ADC_ENABLE_START_STOP_GROUP_API) */


#if (ADC_READ_GROUP_API == STD_ON)
/**********************************************************************************************************************
 * Adc_ReadGroup
 *********************************************************************************************************************/
/*! \brief      Read group
 *
 *              Reads the group conversion result of the last completed conversion round of the requested group and
 *              stores the channel values starting at the DataBufferPtr address. The group channel values are stored in
 *              ascending channel number order (in contrast to the storage layout of the result buffer if streaming
 *              access is configured).
 *
 *  \param[in]  Group               Numeric ID of requested ADC channel group.
 *  \param[in]  DataBufferPtr       ADC results of all channels of the selected group are stored in the data buffer
 *                                  addressed with the pointer.
 *
 *  \param[out] versioninfo         Data structure where the version info shall be written to
 *
 *  \return     E_OK                Results are available and written to the data buffer.
 *  \return     E_NOT_OK            No results are available or development error occured
 *
 *  \pre        The module has to be initialized.
 *
 *  \context    This function can be called on task level.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ADC_CODE) Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupRefType DataBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ChCounter_u8;
  uint16 ResultPosition_u16;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_READ_GROUP, ADC_E_UNINIT);
    return E_NOT_OK;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_READ_GROUP, ADC_E_PARAM_GROUP);
    return E_NOT_OK;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check 'DataBufferPtr' not being the NULL_PTR */
  if (NULL_PTR == DataBufferPtr)
  {
    /* DataBufferPtr references NULL_PTR. */
    Adc_ReportDet (ADC_SID_READ_GROUP, ADC_E_PARAM_READ_BUFFER);
    return E_NOT_OK;
  }
  else
  {
    /* DataBufferPointer seems to be valid: continue */
  }

  /* Check, if the group is active. Only an active, non-idling group can be read. */
  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    if (ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t == 0)
    {
      /* Current Group is in state ADC_IDLE and no valid data were stored before: cannot be read. */
      Adc_ReportDet (ADC_SID_READ_GROUP, ADC_E_IDLE);
      return E_NOT_OK;
    }
    else
    {
      /* Group is in idle state, but valid unread data are in buffer to be read */
    }
  }
  else
  {
    /* Group is not in state ADC_IDLE: continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  if (ADC_BUSY == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    /* No results are available. Group cannot be read. */
    return E_NOT_OK;
  }
  else
  {
    /* State equals ADC_COMPLETED or ADC_STREAM_COMPLETED, so results are available: start reading group */
  }

  /* No need for masking out other information, there is only the pure conversion result in result buffer without
  * status bits or other information that needs to be masked out.
  */

  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    /* State of the group is in idle. bit a conversion was done before and stopped */
    ResultPosition_u16 = ADC_GROUP_CFG(Group).StreamingNumSamples_t-ADC_GROUP_DATA_PTR(Group)->SampleCountdownInIdle_t;
  }
  else
  {
    /* Calculate position of the NEXT entry in buffer */
    ResultPosition_u16 =  ADC_GROUP_CFG(Group).StreamingNumSamples_t - ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t;
  }

  if (0u == ResultPosition_u16)
  {
    /* The NEXT sample will be written at position [0], so the LATEST sample is at the last position in the buffer,
     * which is [StreamingNumSamples - 1]
     */
    ResultPosition_u16 = ADC_GROUP_CFG(Group).StreamingNumSamples_t - 1;
  }
  else
  {
    /* The next result will be written at position [n], so the latest result in buffer is at [n-1] */
    ResultPosition_u16--;
  }

  for (ChCounter_u8 = 0; ChCounter_u8 < ADC_GROUP_CFG(Group).SeqLength_u8; ChCounter_u8++)
  {
    /* Explanation of the formula below:
     *
     * Write the channel i (i = 0..n-1) of the latest sample in the application result buffer to
     * position[i] of the application read buffer.
     *
     * ChCounter_u8 * ADC_GROUP_CFG(Group).StreamingNumSamples_t :    provides the offset of the n-th channel
     *                                                                base address to the result buffer base address.
     *
     * ResultPosition_u16 :                                           provides the position of the latest converted
     *                                                                sample in buffer
     */

    /* Write to application read buffer */
    DataBufferPtr[ChCounter_u8] = 
        ADC_GROUP_DATA_PTR(Group)->ResultBuffer_pt[ChCounter_u8 * ADC_GROUP_CFG(Group).StreamingNumSamples_t 
                                                                                                  + ResultPosition_u16];
  }

  /* Reset ValidSampleCount after read process */
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = 0u;
  ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t = 0u;

  /* State transition after reading */
  Adc_StateTransitionAfterBufferRead (Group);

  return  E_OK;
}
#endif /* (ADC_READ_GROUP_API == STD_ON) */


#if (STD_ON == ADC_HW_TRIGGER_API)
/**********************************************************************************************************************
 * Adc_EnableHardwareTrigger
 *********************************************************************************************************************/
/*! \brief      Enable Hardware Trigger
 *
 *              Enables the hardware trigger for the requested Adc channel group.
 *
 *  \param[in]  Group             Numeric Id of requested Adc channel group.
 *
 *  \pre        The module has to be initialized.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_EnableHardwareTrigger(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_ENABLE_HARDWARE_TRIGGER, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_ENABLE_HARDWARE_TRIGGER, ADC_E_PARAM_GROUP);
    return;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if the trigger source is ADC_TRIGG_SRC_SW */
  if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    Adc_ReportDet (ADC_SID_ENABLE_HARDWARE_TRIGGER, ADC_E_WRONG_TRIGG_SRC);
    return;
  }
  else
  {
    /* 'Group' seems to be a HW triggered group: continue */
  }

  /* check if ResultBuffer_pt has not been initialized before */
  if (NULL_PTR == ADC_GROUP_DATA_PTR(Group)->ResultBuffer_pt)
  {
    Adc_ReportDet (ADC_SID_ENABLE_HARDWARE_TRIGGER, ADC_E_BUFFER_UNINIT);
    return;
  }
  else
  {
    /* Result buffer seems to be initialized: continue */
  }

  /* Check if requested HW unit is ready for use with specified group. */
  if (ADC_INVALID_GRP_ID == *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt))
  {
    /* Currently no group is using this HW unit. */
  }
  else
  {
    /* The group that was started before has not finished until now. */
    Adc_ReportDet (ADC_SID_ENABLE_HARDWARE_TRIGGER, ADC_E_BUSY);
    return;
  }

  /* check if a hardware triggered conversion channel is configured for continuous conversion */
  if (ADC_GROUP_CFG(Group).GroupConversionMode_u8 == ADC_CONV_MODE_CONTINUOUS)
  {
    /* current group is erroneously configured for conversion mode ADC_CONV_MODE_CONTINUOUS */
    Adc_ReportDet (ADC_SID_ENABLE_HARDWARE_TRIGGER, ADC_E_WRONG_CONV_MODE);
    return;
  }
  else
  {
    /* Current group is configured with conversion mode ADC_CONV_MODE_ONESHOT */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Fill in group assignment table, to see which group was started last on the HW conversion group. */
  *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt) = Group;

  /* Start hardware conversion unit. */
  Adc_StartUnit (Group);

  return;
}


/**********************************************************************************************************************
 * Adc_DisableHardwareTrigger
 *********************************************************************************************************************/
/*! \brief      Disable Hardware Trigger
 *
 *              Disables the hardware trigger for the requested Adc channel group.
 *
 *  \param[in]  Group             Numeric Id of requested Adc channel group.
 *
 *  \pre        The module has to be initialized.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_DisableHardwareTrigger(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_DISABLE_HARDWARE_TRIGGER, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_DISABLE_HARDWARE_TRIGGER, ADC_E_PARAM_GROUP);
    return;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if the trigger source is ADC_TRIGG_SRC_SW */
  if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    Adc_ReportDet (ADC_SID_DISABLE_HARDWARE_TRIGGER, ADC_E_WRONG_TRIGG_SRC);
    return;
  }
  else
  {
    /* 'Group' seems to be a HW triggered group: continue */
  }

  /* Check, if the group is active. Only an active, non-idling group can be stopped. */
  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    /* Current Group is in state ADC_IDLE: cannot be stopped. */
    Adc_ReportDet (ADC_SID_DISABLE_HARDWARE_TRIGGER, ADC_E_IDLE);
    return;
  }
  else
  {
    /* Group is not in state ADC_IDLE: continue */
  }

  /* check if a hardware triggered conversion channel is configured for continuous conversion */
  if (ADC_GROUP_CFG(Group).GroupConversionMode_u8 == ADC_CONV_MODE_CONTINUOUS)
  {
    /* current group is erroneously configured for conversion mode ADC_CONV_MODE_CONTINUOUS */
    Adc_ReportDet (ADC_SID_DISABLE_HARDWARE_TRIGGER, ADC_E_WRONG_CONV_MODE);
    return;
  }
  else
  {
    /* Current group is configured with conversion mode ADC_CONV_MODE_ONESHOT */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Affect HW only when the current group 'Group' was started last on this HW group and is still active. */
  if (Group == *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt))
  {
    /* Stop conversion. This is equal to stopping a real hardware unit. */
    VttAdc_StopConversion (Group);

    /* Reset GroupAssignment_t to default state, because HW is not in use anymore */
    *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt) = ADC_INVALID_GRP_ID;
  }
  else
  {
    /* Don't modify HW, because group 'Group' is not on the HW anymore. */
  }

  /* Disable group notification */
  ADC_GROUP_DATA_PTR(Group)->Notification_u8 = ADC_NOTIFICATION_OFF;

  /* Set group state to ADC_IDLE */
  ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_IDLE;

  /* Reset ValidSampleCount and SampleCountdown */
  ADC_GROUP_DATA_PTR(Group)->SampleCountdownInIdle_t = ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t;
  ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t = ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t;

  ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t = ADC_GROUP_CFG(Group).StreamingNumSamples_t;
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = 0u;

  return;
} /* End of Adc_DisableHardwareTrigger() */
#endif /* (STD_ON == ADC_HW_TRIGGER_API) */


#if (STD_ON == ADC_GRP_NOTIF_CAPABILITY)
/**********************************************************************************************************************
 * Adc_EnableGroupNotification
 *********************************************************************************************************************/
/*! \brief      Enable Group Notification
 *
 *              Enables the notification mechanism for the requested Adc channel group.
 *
 *  \param[in]  Group             Numeric Id of requested Adc channel group.
 *
 *  \pre        The module has to be initialized.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_EnableGroupNotification(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_ENABLE_GROUP_NOTIFICATION, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_ENABLE_GROUP_NOTIFICATION, ADC_E_PARAM_GROUP);
    return;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if no notification is configured */
  if (NULL_PTR == ADC_GROUP_CFG(Group).Notification_p)
  {
    Adc_ReportDet (ADC_SID_ENABLE_GROUP_NOTIFICATION, ADC_E_NOTIF_CAPABILITY);
    return;
  }
  else
  {
    /* Parameter No notification function available */
  } 
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Set group notification status to on */
  ADC_GROUP_DATA_PTR(Group)->Notification_u8 = ADC_NOTIFICATION_ON;

  return;
} /* End of Adc_EnableGroupNotification() */


/**********************************************************************************************************************
 * Adc_DisableGroupNotification
 *********************************************************************************************************************/
/*! \brief      Disable Group Notification
 *
 *              Disables the notification mechanism for the requested Adc channel group.
 *
 *  \param[in]  Group             Numeric Id of requested Adc channel group.
 *
 *  \pre        The module has to be initialized.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_DisableGroupNotification(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_DISABLE_GROUP_NOTIFICATION, ADC_E_UNINIT);
    return;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_DISABLE_GROUP_NOTIFICATION, ADC_E_PARAM_GROUP);
    return;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if no notification is configured */
  if (NULL_PTR == ADC_GROUP_CFG(Group).Notification_p)
  {
    Adc_ReportDet (ADC_SID_DISABLE_GROUP_NOTIFICATION, ADC_E_NOTIF_CAPABILITY);
    return;
  }
  else
  {
    /* Parameter No notification function available */
  } 
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Change notification status */
  ADC_GROUP_DATA_PTR(Group)->Notification_u8 = ADC_NOTIFICATION_OFF;

  return;
} /* End of Adc_DisableGroupNotification() */
#endif /* (STD_ON == ADC_GRP_NOTIF_CAPABILITY) */


/**********************************************************************************************************************
 * Adc_GetGroupStatus
 *********************************************************************************************************************/
/*! \brief      Get Group Status
 *
 *              Returns the conversion status of the requested ADC channel group
 *
 *  \param[in]  Group                     Numeric Id of requested Adc channel group.
 *
 *  \return     Adc_StatusType            Conversion status for the requested group.
 *
 *  \pre        The module has to be initialized.
 *
 *  \context    This function can be called on task level.
 *********************************************************************************************************************/
FUNC(Adc_StatusType, ADC_CODE) Adc_GetGroupStatus(Adc_GroupType Group)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    Adc_ReportDet (ADC_SID_GET_GROUP_STATUS, ADC_E_UNINIT);
    return ADC_IDLE;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    Adc_ReportDet (ADC_SID_GET_GROUP_STATUS, ADC_E_PARAM_GROUP);
    return ADC_IDLE;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  return ADC_GROUP_DATA_PTR(Group)->Status_t;
} /* End of Adc_GetGroupStatus() */


/**********************************************************************************************************************
 * Adc_GetStreamLastPointer
 *********************************************************************************************************************/
/*! \brief      Get Stream Last Pointer
 *
 *              Returns the number of valid samples per channel, stored in the result buffer.
 *              Reads a pointer, pointing to a position in the group result buffer. With the pointer position, the
 *              results of all group channels of the last completed conversion round can be accessed.
 *              With the pointer and the return value, all valid group conversion results can be accessed
 *              (the user has to take the layout of the result buffer into account).
 *
 *  \param[in]  Group                     Numeric Id of requested Adc channel group.
 *  \param[out] PtrToSamplePtr            Pointer to result buffer pointer.
 *
 *  \return     Adc_StreamNumSampleType   Number of valid samples per channel.
 *
 *  \pre        The module has to be initialized.
 *
 *  \context    This function can be called on task level.
 *********************************************************************************************************************/
FUNC(Adc_StreamNumSampleType, ADC_CODE) Adc_GetStreamLastPointer(Adc_GroupType Group,
                                                                 Adc_ValueGroupRefRefType PtrToSamplePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 ResultPosition_u16;
  Adc_StreamNumSampleType SampleReturnValue;

  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if module is uninitialized */
  if (NULL_PTR == Adc_ConfigPtr_pt)
  {
    *PtrToSamplePtr = NULL_PTR;
    Adc_ReportDet (ADC_SID_GET_STREAM_LAST_POINTER, ADC_E_UNINIT);
    return 0;
  }
  else
  {
    /* Adc module seems to be initialized: continue */
  }

  /* check if the parameter group is within the allowed range */
  if (Group >= Adc_ConfigPtr_pt->NumberOfGroups_t)
  {
    *PtrToSamplePtr = NULL_PTR;
    Adc_ReportDet (ADC_SID_GET_STREAM_LAST_POINTER, ADC_E_PARAM_GROUP);
    return 0;
  }
  else
  {
    /* Parameter 'Group' seems to be valid: continue */
  }

  /* check if the parameter PtrToSamplePtr is the NULL_PTR */
  if (NULL_PTR == PtrToSamplePtr)
  {
    Adc_ReportDet (ADC_SID_GET_STREAM_LAST_POINTER, ADC_E_PARAM_STREAM_PTR);
    return 0;
  }
  else
  {
    /* parameter PtrToSamplePtr seems to be valid: continue */
  }

  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    if (ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t == 0)
    {
      /* Current Group is in state ADC_IDLE and no valid data were stored before: cannot be read. */
      *PtrToSamplePtr = NULL_PTR;
      Adc_ReportDet (ADC_SID_GET_STREAM_LAST_POINTER, ADC_E_IDLE);
      return 0;
    }
    else
    {
      /* Group is in idle state, but valid unread data are in buffer to be read */
    }
  }
  else
  {
    /* Continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  /* Check if results are available. If no, return. */
  if (ADC_BUSY == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    *PtrToSamplePtr = NULL_PTR;
    return 0;
  }
  else
  {
    /* Results available: continue */
  }

  /* No need for masking out other information, there is only the pure conversion result in result buffer without
   * status bits or other information that needs to be masked out.
   */
  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    /* State of the group is in idle. bit a conversion was done before and stopped */
    ResultPosition_u16 =  ADC_GROUP_CFG(Group).StreamingNumSamples_t
                                                                   - ADC_GROUP_DATA_PTR(Group)->SampleCountdownInIdle_t;
  }
  else
  {
    /* Calculate position of the NEXT entry in buffer */
    ResultPosition_u16 = ADC_GROUP_CFG(Group).StreamingNumSamples_t - ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t;
  }

  if (0u == ResultPosition_u16)
  {
    /* The NEXT result will be written at position [0], so the LATEST result is at the last position in the buffer,
     * which is [StreamingNumSamples - 1]
     */
    ResultPosition_u16 = ADC_GROUP_CFG(Group).StreamingNumSamples_t - 1;
  }
  else
  {
    /* The next result will be written at position [n], so the latest result in buffer is at [n-1] */
    ResultPosition_u16--;
  }

  /* Let PtrToSamplePtr refer to the latest element in application result buffer. */
  (*PtrToSamplePtr) = &(ADC_GROUP_DATA_PTR(Group)->ResultBuffer_pt[ResultPosition_u16]);

  if (ADC_IDLE == ADC_GROUP_DATA_PTR(Group)->Status_t)
  {
    SampleReturnValue = ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t;
  }
  else
  {
    SampleReturnValue = ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t;
  }

  /* Reset ValidSampleCount after read process */
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = 0u;
  ADC_GROUP_DATA_PTR(Group)->ValidSamplesInIdle_t = 0u;

  /* State transition after getting StreamLastPointer */
  Adc_StateTransitionAfterBufferRead (Group);

  return SampleReturnValue;
} /* End of Adc_GetStreamLastPointer() */


#if (ADC_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * Adc_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Version Information Service
 *
 *              This service writes the version information of this module into the data structure referenced by the
 *              parameter. If this parameter references NULL_PTR, a development error is reported to Det. This service
 *              is only available if the pre-compile configuration parameter ADC_VERSION_INFO_API is enabled.
 *
 *  \param[out] versioninfo         data structure where the version info shall be written to
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ADC_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (STD_ON == ADC_DEV_ERROR_DETECT)
  /* check if the parameter versioninfo is the NULL_PTR */
  if (NULL_PTR == versioninfo)
  {
    Adc_ReportDet (ADC_SID_GET_VERSION_INFO, ADC_E_PARAM_VINFO);
    return;
  }
  else
  {
    /* parameter versioninfo seems to be valid: continue */
  }
#endif /* (STD_ON == ADC_DEV_ERROR_DETECT) */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID         = (uint16) ADC_VENDOR_ID;
  versioninfo->moduleID         = (uint16) ADC_MODULE_ID;
  versioninfo->sw_major_version = (uint8)  ADC_SW_MAJOR_VERSION;
  versioninfo->sw_minor_version = (uint8)  ADC_SW_MINOR_VERSION;
  versioninfo->sw_patch_version = (uint8)  ADC_SW_PATCH_VERSION;

  return;
} /* End of Adc_GetVersionInfo() */
#endif /* End of: (ADC_VERSION_INFO_API == STD_ON) */


/**********************************************************************************************************************
 *  Adc_IsrHandler
 *********************************************************************************************************************/
/*! \brief      Interrupt Handler
 *
 *              This handler is called by the ISR. It is responsible for reading results from fifo 
 *              to application result buffer and calling the notification function, if enabled.
 *
 *  \param[in]  Adc_GroupType                   Group
 *
 *  \return     void
 *
 *  \context    Function is called on interrupt level.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_IsrHandler (Adc_HwUnitEnumType HwUnit)
{
  /* ----- Local Variables ---------------------------------------------- */
  Adc_GroupType Group;

  /* ----- Implementation ----------------------------------------------- */
  /* query the active group within the specified hardware unit 'HwUnit' */
  Group = Adc_ConfigPtr_pt->ActiveGroupRtx_at[HwUnit];

  /* check if a group is active within th HwUnit */
  if (ADC_INVALID_GRP_ID == Group)
  {
    /* No Group is currently active, so return without action */
    return;
  }
  else
  {
    /* Get the latest conversion results out of CANoe. Copy them to application result buffer. */
    Adc_ProcessBuffer (Group);

    /* Call process conversion function (single/linear/circular) which is responsible for 
     * e.g. state transitions or restarting conversions 
     */
    ADC_GROUP_CFG(Group).ProcessConversion_p (Group);

    #if (STD_ON == ADC_GRP_NOTIF_CAPABILITY)
    /* Call notification function. This is either a pointer to an empty function, in case notification is disabled, or 
     * to a non-empty function in case notification shall be sent.
     */
    if ( (NULL_PTR != ADC_GROUP_CFG(Group).Notification_p) 
          && (ADC_NOTIFICATION_ON == ADC_GROUP_DATA_PTR(Group)->Notification_u8)
          && ( (ADC_STREAM_COMPLETED == ADC_GROUP_DATA_PTR(Group)->Status_t) 
               || (ADC_COMPLETED == ADC_GROUP_DATA_PTR(Group)->Status_t) ) 
       )
    {
      ADC_GROUP_CFG(Group).Notification_p();
    }
    #endif
  }
  return;
} /* End of Adc_IsrHandler() */


/**********************************************************************************************************************
 * Adc_ProcessConversionSingle
 *********************************************************************************************************************/
/*! \brief     Adc_ProcessConversionSingle
 *
 *              This method is called each time results have been copied from hardware fifo buffer to application result
 *              buffer. It is responsible for setting ValidSampleCount and group status. This variant is used for
 *              single buffer access.
 *
 *  \param[in]  Group                            Numeric Id of requested Adc channel group.
 *
 *  \context    This function can be called on task and interrupt level.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_ProcessConversionSingle (Adc_GroupType Group)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Results are always in buffer. Set ValidSampleCount to max., which is 1, because of SingleAccess. */
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = 1u;
  /* Toggle group status to ADC_STREAM_COMPLETED. */
  ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_STREAM_COMPLETED;

  /* In case the group is a one-shot, software triggered group, the hardware has to stop impicitly after one
  * conversion. In this case, the group assignment table has to be reset, too.
  */
  if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    if (ADC_CONV_MODE_ONESHOT == ADC_GROUP_CFG(Group).GroupConversionMode_u8)
    {
      /* Reset GroupAssignment_t to default state, because group stops implicitly. */
      *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt) = ADC_INVALID_GRP_ID;
    }
    else
    {
      /* Software triggered continuous conversion groups are restarted automatically. */
    }
  }
  else
  {
    /* Hardware triggered one shot conversion is restarted by hardware trigger event. 
     * Listen for next Hw Trigger Edge
     */
    VttAdc_StartOneShotHwTriggered (Group, ADC_GROUP_CFG(Group).HwTrigEvent_u8);
  }

  return;
} /* End of Adc_ProcessConversionSingle() */


/**********************************************************************************************************************
 * Adc_ProcessConversionLinear
 *********************************************************************************************************************/
/*! \brief     Adc_ProcessConversionLinear
 *
 *              This method is called each time results have been copied from hardware fifo buffer to application result
 *              buffer. It is responsible for setting ValidSampleCount and group status. This variant is used for
 *              linear streaming buffer access.
 *
 *  \param[in]  Group                            Numeric Id of requested Adc channel group.
 *
 *  \context    This function can be called on task and interrupt level.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_ProcessConversionLinear (Adc_GroupType Group)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Adc_ProcessBuffer has been executed immediately before, therefore increment ValidSampleCount and decrement
   * SampleCountdown
   */
  ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t--;
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t++;

  /* Check if samples are remaining */
  if (ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t > 0u)
  {
    /* Results are remaining, conversion is ongoing. Toggle status to ADC_COMPLETED. */
    ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_COMPLETED;

    if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
    {
      /* Continuous conversion (ADC_TRIGG_SRC_SW) is ongoing. */
    }
    else
    {
      /* Hardware triggered one shot conversion is restarted by hardware trigger event. */
      VttAdc_StartOneShotHwTriggered (Group, ADC_GROUP_CFG(Group).HwTrigEvent_u8);
    }
  }
  else
  {
    /* Conversion has been finished, no samples are remaining. */
    ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_STREAM_COMPLETED;

    /* Stop the running SW triggered conversion group */
    if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
    {
      VttAdc_StopConversion (Group);
    }
    else
    {
      /* Hardware Triggered Conversion stops implizit */
    }

    /* Reset GroupAssignment_t to default state, because HW is not in use anymore */
    *(ADC_GROUP_DATA_PTR(Group)->GroupAssignment_pt) = ADC_INVALID_GRP_ID;
  }
  return;
} /* End of Adc_ProcessConversionLinear() */


/**********************************************************************************************************************
 * Adc_ProcessConversionCircular
 *********************************************************************************************************************/
/*! \brief     Adc_ProcessConversionCircular
 *
 *              This method is called each time results have been copied from hardware fifo buffer to application result
 *              buffer. It is responsible for setting ValidSampleCount and group status. This variant is used for
 *              circular streaming buffer access.
 *
 *  \param[in]  Group                              Numeric Id of requested Adc channel group.
 *
 *  \context    This function can be called on task and interrupt level.
 *********************************************************************************************************************/
FUNC(void, ADC_CODE) Adc_ProcessConversionCircular (Adc_GroupType Group)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Adc_ProcessBuffer has been executed immediately before this function call. Decrement SampleCountdown. */
  ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t--;
  /* Increment ValidSampleCount_t. If the buffer is already filled completely, the value is cropped below.*/
  ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t++;

  /* Check if end of buffer is reached and therefore the buffer has to perform a wrap around. */
  if (ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t == 0u)
  {
    /* Perform a wrap around of buffer. Start at begin of buffer. */
    ADC_GROUP_DATA_PTR(Group)->SampleCountdown_t = ADC_GROUP_CFG(Group).StreamingNumSamples_t;
  }
  else
  {
    /* no wrap around necessary */
  }

  /* Check if buffer is filled with the maximum number of VALID results. */
  if (ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t >= ADC_GROUP_CFG(Group).StreamingNumSamples_t)
  {
    /* Buffer is filled with VALID results. ValidSampleCount is max. Crop it to the maximum value. */
    ADC_GROUP_DATA_PTR(Group)->ValidSampleCount_t = ADC_GROUP_CFG(Group).StreamingNumSamples_t;
    /* Set state to ADC_STREAM_COMPLETED */
    ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_STREAM_COMPLETED;
  }
  else
  {
    /* Set state to ADC_COMPLETED, because there are samples in buffer and samples remaining */
    ADC_GROUP_DATA_PTR(Group)->Status_t = ADC_COMPLETED;
  }

  if (ADC_TRIGG_SRC_SW == ADC_GROUP_CFG(Group).GroupTriggSrc_u8)
  {
    /* Continuous conversion (ADC_TRIGG_SRC_SW) is ongoing. */
  }
  else
  {
    /* Hardware triggered one shot conversion is restarted by hardware trigger event. */
    VttAdc_StartOneShotHwTriggered (Group, ADC_GROUP_CFG(Group).HwTrigEvent_u8);
  }

  return;
} /* End of Adc_ProcessConversionCircular() */


/**********************************************************************************************************************
 *  END OF FILE: Adc.c
 *********************************************************************************************************************/
