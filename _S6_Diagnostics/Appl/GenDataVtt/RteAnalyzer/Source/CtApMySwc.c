/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtApMySwc.c
 *        Config:  MyECU.dpa
 *     SW-C Type:  CtApMySwc
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for SW-C <CtApMySwc>
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of version logging area >>                DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/* PRQA S 0777, 0779 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779 */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0614 EOF */ /* MD_Rte_TestCode */

/* PRQA S 3109 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3112 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3197 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3198 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3199 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3201 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3203 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3205 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3206 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3218 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3229 EOF */ /* MD_Rte_TestCode */
/* PRQA S 2002 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3334 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3417 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3426 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3453 EOF */ /* MD_Rte_TestCode */

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of version logging area >>                  DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * AUTOSAR Modelling Object Descriptions
 *
 **********************************************************************************************************************
 *
 * Data Types:
 * ===========
 * BswM_ESH_Mode
 *   
 *
 * BswM_ESH_RunRequest
 *   
 *
 * BswM_MdgShutdownTimerState
 *   
 *
 * ComM_ModeType
 *   
 *
 * Dcm_NegativeResponseCodeType
 *   
 *
 * Dcm_OpStatusType
 *   
 *
 * Dem_OperationCycleStateType
 *   
 *
 * IdtDoorState
 *   
 *
 * IdtLightState
 *   
 *
 * NvM_RequestResultType
 *   
 *
 * NvM_ServiceIdType
 *   
 *
 *********************************************************************************************************************/

#include "Rte_CtApMySwc.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */
#include "TSC_CtApMySwc.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "string.h"

static void CtApMySwc_TestDefines(void);


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of include and declaration area >>          DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * Used AUTOSAR Data Types
 *
 **********************************************************************************************************************
 *
 * Primitive Types:
 * ================
 * boolean: Boolean (standard type)
 * dtRef_const_VOID: DataReference
 * uint16: Integer in interval [0...65535] (standard type)
 * uint32: Integer in interval [0...4294967295] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 *
 * Enumeration Types:
 * ==================
 * BswM_ESH_Mode: Enumeration of integer in interval [0...255] with enumerators
 *   STARTUP (0U)
 *   RUN (1U)
 *   POSTRUN (2U)
 *   WAKEUP (3U)
 *   SHUTDOWN (4U)
 * BswM_ESH_RunRequest: Enumeration of integer in interval [0...255] with enumerators
 *   RELEASED (0U)
 *   REQUESTED (1U)
 * BswM_MdgShutdownTimerState: Enumeration of integer in interval [0...255] with enumerators
 *   TIMER_EXPIRED (0U)
 *   TIMER_NOT_EXPIRED (1U)
 * ComM_ModeType: Enumeration of integer in interval [0...255] with enumerators
 *   COMM_NO_COMMUNICATION (0U)
 *   COMM_SILENT_COMMUNICATION (1U)
 *   COMM_FULL_COMMUNICATION (2U)
 * Dcm_NegativeResponseCodeType: Enumeration of integer in interval [0...255] with enumerators
 *   DCM_E_POSITIVERESPONSE (0U)
 *   DCM_E_GENERALREJECT (16U)
 *   DCM_E_SERVICENOTSUPPORTED (17U)
 *   DCM_E_SUBFUNCTIONNOTSUPPORTED (18U)
 *   DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT (19U)
 *   DCM_E_RESPONSETOOLONG (20U)
 *   DCM_E_BUSYREPEATREQUEST (33U)
 *   DCM_E_CONDITIONSNOTCORRECT (34U)
 *   DCM_E_REQUESTSEQUENCEERROR (36U)
 *   DCM_E_NORESPONSEFROMSUBNETCOMPONENT (37U)
 *   DCM_E_FAILUREPREVENTSEXECUTIONOFREQUESTEDACTION (38U)
 *   DCM_E_REQUESTOUTOFRANGE (49U)
 *   DCM_E_SECURITYACCESSDENIED (51U)
 *   DCM_E_INVALIDKEY (53U)
 *   DCM_E_EXCEEDNUMBEROFATTEMPTS (54U)
 *   DCM_E_REQUIREDTIMEDELAYNOTEXPIRED (55U)
 *   DCM_E_UPLOADDOWNLOADNOTACCEPTED (112U)
 *   DCM_E_TRANSFERDATASUSPENDED (113U)
 *   DCM_E_GENERALPROGRAMMINGFAILURE (114U)
 *   DCM_E_WRONGBLOCKSEQUENCECOUNTER (115U)
 *   DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING (120U)
 *   DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION (126U)
 *   DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION (127U)
 *   DCM_E_RPMTOOHIGH (129U)
 *   DCM_E_RPMTOOLOW (130U)
 *   DCM_E_ENGINEISRUNNING (131U)
 *   DCM_E_ENGINEISNOTRUNNING (132U)
 *   DCM_E_ENGINERUNTIMETOOLOW (133U)
 *   DCM_E_TEMPERATURETOOHIGH (134U)
 *   DCM_E_TEMPERATURETOOLOW (135U)
 *   DCM_E_VEHICLESPEEDTOOHIGH (136U)
 *   DCM_E_VEHICLESPEEDTOOLOW (137U)
 *   DCM_E_THROTTLE_PEDALTOOHIGH (138U)
 *   DCM_E_THROTTLE_PEDALTOOLOW (139U)
 *   DCM_E_TRANSMISSIONRANGENOTINNEUTRAL (140U)
 *   DCM_E_TRANSMISSIONRANGENOTINGEAR (141U)
 *   DCM_E_BRAKESWITCH_NOTCLOSED (143U)
 *   DCM_E_SHIFTERLEVERNOTINPARK (144U)
 *   DCM_E_TORQUECONVERTERCLUTCHLOCKED (145U)
 *   DCM_E_VOLTAGETOOHIGH (146U)
 *   DCM_E_VOLTAGETOOLOW (147U)
 *   DCM_E_VMSCNC_0 (240U)
 *   DCM_E_VMSCNC_1 (241U)
 *   DCM_E_VMSCNC_2 (242U)
 *   DCM_E_VMSCNC_3 (243U)
 *   DCM_E_VMSCNC_4 (244U)
 *   DCM_E_VMSCNC_5 (245U)
 *   DCM_E_VMSCNC_6 (246U)
 *   DCM_E_VMSCNC_7 (247U)
 *   DCM_E_VMSCNC_8 (248U)
 *   DCM_E_VMSCNC_9 (249U)
 *   DCM_E_VMSCNC_A (250U)
 *   DCM_E_VMSCNC_B (251U)
 *   DCM_E_VMSCNC_C (252U)
 *   DCM_E_VMSCNC_D (253U)
 *   DCM_E_VMSCNC_E (254U)
 * Dcm_OpStatusType: Enumeration of integer in interval [0...255] with enumerators
 *   DCM_INITIAL (0U)
 *   DCM_PENDING (1U)
 *   DCM_CANCEL (2U)
 *   DCM_FORCE_RCRRP_OK (3U)
 *   DCM_FORCE_RCRRP_NOT_OK (64U)
 * Dem_OperationCycleStateType: Enumeration of integer in interval [0...255] with enumerators
 *   DEM_CYCLE_STATE_START (0U)
 *   DEM_CYCLE_STATE_END (1U)
 * IdtDisplayIgnitionState: Enumeration of integer in interval [0...255] with enumerators
 *   CMIGNITION_STATEON (0U)
 *   CMIGNITION_STATEOFF (1U)
 * IdtDoorState: Enumeration of integer in interval [0...1] with enumerators
 *   CMDOORSTATE_DOORCLOSED (0U)
 *   CMDOORSTATE_DOOROPEN (1U)
 * IdtLightState: Enumeration of integer in interval [0...1] with enumerators
 *   CMLIGHTSTATE_LIGHTOFF (0U)
 *   CMLIGHTSTATE_LIGHTON (1U)
 * NvM_RequestResultType: Enumeration of integer in interval [0...255] with enumerators
 *   NVM_REQ_OK (0U)
 *   NVM_REQ_NOT_OK (1U)
 *   NVM_REQ_PENDING (2U)
 *   NVM_REQ_INTEGRITY_FAILED (3U)
 *   NVM_REQ_BLOCK_SKIPPED (4U)
 *   NVM_REQ_NV_INVALIDATED (5U)
 *   NVM_REQ_CANCELED (6U)
 *   NVM_REQ_REDUNDANCY_FAILED (7U)
 *   NVM_REQ_RESTORED_FROM_ROM (8U)
 * NvM_ServiceIdType: Enumeration of integer in interval [0...255] with enumerators
 *   NVM_READ_BLOCK (6U)
 *   NVM_WRITE_BLOCK (7U)
 *   NVM_RESTORE_BLOCK_DEFAULTS (8U)
 *   NVM_ERASE_BLOCK (9U)
 *   NVM_INVALIDATE_NV_BLOCK (11U)
 *   NVM_READ_ALL (12U)
 *
 * Array Types:
 * ============
 * DataArrayType_uint8_4: Array with 4 element(s) of type uint8
 * Dcm_Data4ByteType: Array with 4 element(s) of type uint8
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * APIs which are accessible from all runnable entities of the SW-C
 *
 **********************************************************************************************************************
 * Per-Instance Memory:
 * ====================
 *   uint32 *Rte_Pim_PimLightOnOffCounter(void)
 *   uint32 *Rte_Pim_PimOdometer(void)
 *
 * Calibration Parameters:
 * =======================
 *   SW-C local Calibration Parameters:
 *   ----------------------------------
 *   uint32 Rte_CData_CalOdometer(void)
 *
 *********************************************************************************************************************/


#define CtApMySwc_START_SEC_CODE
#include "CtApMySwc_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: CBReadData_OccurrenceCounter_OccurenceCounter_ReadData
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadData> of PortPrototype <CBReadData_OccurrenceCounter_OccurenceCounter>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType CBReadData_OccurrenceCounter_OccurenceCounter_ReadData(uint8 *Data)
 *     Argument Data: uint8* is of type DataArrayType_uint8_4
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_CSDataServices_OccurrenceCounter_OccurenceCounter_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: CBReadData_OccurrenceCounter_OccurenceCounter_ReadData_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, CtApMySwc_CODE) CBReadData_OccurrenceCounter_OccurenceCounter_ReadData(P2VAR(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: CBReadData_OccurrenceCounter_OccurenceCounter_ReadData (returns application error)
 *********************************************************************************************************************/

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  CtApMySwc_TestDefines();

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: CBReadData_OdometerValue_OdometerValue_ReadData
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadData> of PortPrototype <CBReadData_OdometerValue_OdometerValue>
 *
 **********************************************************************************************************************
 *
 * Exclusive Area Access:
 * ======================
 *   void Rte_Enter_ExArOdometer(void)
 *   void Rte_Exit_ExArOdometer(void)
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType CBReadData_OdometerValue_OdometerValue_ReadData(uint8 *Data)
 *     Argument Data: uint8* is of type DataArrayType_uint8_4
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_CSDataServices_OdometerValue_OdometerValue_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: CBReadData_OdometerValue_OdometerValue_ReadData_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, CtApMySwc_CODE) CBReadData_OdometerValue_OdometerValue_ReadData(P2VAR(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: CBReadData_OdometerValue_OdometerValue_ReadData (returns application error)
 *********************************************************************************************************************/

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  TSC_CtApMySwc_Rte_Enter_ExArOdometer();
  TSC_CtApMySwc_Rte_Exit_ExArOdometer();

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ConditionCheckRead> of PortPrototype <DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value>
 *
 **********************************************************************************************************************
 *
 * Exclusive Area Access:
 * ======================
 *   void Rte_Enter_ExArLightOnOffCounter(void)
 *   void Rte_Exit_ExArLightOnOffCounter(void)
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead(Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType *ErrorCode)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_DCM_E_PENDING
 *   RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead(Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) ErrorCode) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead (returns application error)
 *********************************************************************************************************************/

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter();
  TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter();

  return RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadData> of PortPrototype <DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value>
 *
 **********************************************************************************************************************
 *
 * Exclusive Area Access:
 * ======================
 *   void Rte_Enter_ExArLightOnOffCounter(void)
 *   void Rte_Exit_ExArLightOnOffCounter(void)
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData(Dcm_OpStatusType OpStatus, uint8 *Data)
 *     Argument Data: uint8* is of type Dcm_Data4ByteType
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_DCM_E_PENDING
 *   RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData(Dcm_OpStatusType OpStatus, P2VAR(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData (returns application error)
 *********************************************************************************************************************/

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter();
  TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter();

  return RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <WriteData> of PortPrototype <DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value>
 *
 **********************************************************************************************************************
 *
 * Exclusive Area Access:
 * ======================
 *   void Rte_Enter_ExArLightOnOffCounter(void)
 *   void Rte_Exit_ExArLightOnOffCounter(void)
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData(const uint8 *Data, Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType *ErrorCode)
 *     Argument Data: uint8* is of type Dcm_Data4ByteType
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_DCM_E_PENDING
 *   RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData(P2CONST(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_DATA) Data, Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) ErrorCode) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData (returns application error)
 *********************************************************************************************************************/

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter();
  TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter();

  return RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <JobFinished> of PortPrototype <PpNvMNotifyJobFinishedOdometerCtApMySwc>
 *
 **********************************************************************************************************************
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(boolean data)
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtApMySwc_CODE) PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(Rte_InitValue_PpDisplayState_DeOdometerWriteRequestPending);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApMySwcCode
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on TimingEvent every 200ms
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpDoorStateFrontRight_DeDoorState(IdtDoorState *data)
 *   Std_ReturnType Rte_Read_PpDoorStateRearRight_DeDoorState(IdtDoorState *data)
 *   Std_ReturnType Rte_Read_PpLocalIgnitionState_DeIgnitionState(IdtDisplayIgnitionState *data)
 *
 *   Implicit S/R API:
 *   -----------------
 *   IdtDoorState Rte_IRead_RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState(void)
 *   IdtDoorState Rte_IRead_RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState(void)
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data)
 *   Std_ReturnType Rte_Write_PpDisplayState_DeDisplayBusState(boolean data)
 *   Std_ReturnType Rte_Write_PpDisplayState_DeLightOnOffCounter(uint32 data)
 *   Std_ReturnType Rte_Write_PpDisplayState_DeOdometerValue(uint32 data)
 *   Std_ReturnType Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(boolean data)
 *   Std_ReturnType Rte_Write_PpLightStateFront_DeLightState(IdtLightState data)
 *   Std_ReturnType Rte_Write_PpLightStateRear_DeLightState(IdtLightState data)
 *
 * Inter Runnable Variables:
 * =========================
 *   Explicit Read Access:
 *   ---------------------
 *   uint16 Rte_IrvRead_RCtApMySwcCode_IrvDoorStateOld(void)
 *
 *   Explicit Write Access:
 *   ----------------------
 *   void Rte_IrvWrite_RCtApMySwcCode_IrvDoorStateOld(uint16 data)
 *
 * Service Calls:
 * ==============
 *   Service Invocation:
 *   -------------------
 *   Std_ReturnType Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(ComM_ModeType ComMode)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_ComM_UserRequest_E_MODE_LIMITATION, RTE_E_ComM_UserRequest_E_NOT_OK
 *   Std_ReturnType Rte_Call_PpNvMLightOnOffCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *   Std_ReturnType Rte_Call_PpNvMOdometerCtApMySwc_GetErrorStatus(NvM_RequestResultType *ErrorStatus)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_NvMService_AC3_SRBS_Defs_E_NOT_OK
 *   Std_ReturnType Rte_Call_PpNvMOdometerCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_NvMService_AC3_SRBS_Defs_E_NOT_OK
 *
 * Exclusive Area Access:
 * ======================
 *   void Rte_Enter_ExArLightOnOffCounter(void)
 *   void Rte_Exit_ExArLightOnOffCounter(void)
 *   void Rte_Enter_ExArOdometer(void)
 *   void Rte_Exit_ExArOdometer(void)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcCode_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtApMySwc_CODE) RCtApMySwcCode(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcCode
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  IdtDoorState Read_PpDoorStateFrontRight_DeDoorState;
  IdtDoorState Read_PpDoorStateRearRight_DeDoorState;
  IdtDisplayIgnitionState Read_PpLocalIgnitionState_DeIgnitionState;

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  IdtDoorState IRead_PpDoorStateFrontLeft_DeDoorState;
  IdtDoorState IRead_PpDoorStateRearLeft_DeDoorState;

  BswM_ESH_RunRequest Write_PpBswMRunRequestCtApMySwc_requestedMode;

  uint16 RCtApMySwcCode_IrvDoorStateOld;

  NvM_RequestResultType Call_PpNvMOdometerCtApMySwc_GetErrorStatus_ErrorStatus = 0U;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  fct_status = TSC_CtApMySwc_Rte_Read_PpDoorStateFrontRight_DeDoorState(&Read_PpDoorStateFrontRight_DeDoorState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_NEVER_RECEIVED:
      fct_error = 1;
      break;
    case RTE_E_INVALID:
      fct_error = 1;
      break;
    case RTE_E_MAX_AGE_EXCEEDED:
      fct_error = 1;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Read_PpDoorStateRearRight_DeDoorState(&Read_PpDoorStateRearRight_DeDoorState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_NEVER_RECEIVED:
      fct_error = 1;
      break;
    case RTE_E_INVALID:
      fct_error = 1;
      break;
    case RTE_E_MAX_AGE_EXCEEDED:
      fct_error = 1;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Read_PpLocalIgnitionState_DeIgnitionState(&Read_PpLocalIgnitionState_DeIgnitionState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_NEVER_RECEIVED:
      fct_error = 1;
      break;
    case RTE_E_INVALID:
      fct_error = 1;
      break;
    case RTE_E_MAX_AGE_EXCEEDED:
      fct_error = 1;
      break;
  }

  IRead_PpDoorStateFrontLeft_DeDoorState = TSC_CtApMySwc_Rte_IRead_RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState();
  IRead_PpDoorStateRearLeft_DeDoorState = TSC_CtApMySwc_Rte_IRead_RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState();

  (void)memset(&Write_PpBswMRunRequestCtApMySwc_requestedMode, 0, sizeof(Write_PpBswMRunRequestCtApMySwc_requestedMode));
  fct_status = TSC_CtApMySwc_Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(Write_PpBswMRunRequestCtApMySwc_requestedMode);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(Rte_InitValue_PpDisplayState_DeDisplayBusState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeLightOnOffCounter(Rte_InitValue_PpDisplayState_DeLightOnOffCounter);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerValue(Rte_InitValue_PpDisplayState_DeOdometerValue);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(Rte_InitValue_PpDisplayState_DeOdometerWriteRequestPending);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpLightStateFront_DeLightState(Rte_InitValue_PpLightStateFront_DeLightState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpLightStateRear_DeLightState(Rte_InitValue_PpLightStateRear_DeLightState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  RCtApMySwcCode_IrvDoorStateOld = TSC_CtApMySwc_Rte_IrvRead_RCtApMySwcCode_IrvDoorStateOld();

  TSC_CtApMySwc_Rte_IrvWrite_RCtApMySwcCode_IrvDoorStateOld(0U);

  fct_status = TSC_CtApMySwc_Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(0U);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
    case RTE_E_ComM_UserRequest_E_MODE_LIMITATION:
      fct_error = 1;
      break;
    case RTE_E_ComM_UserRequest_E_NOT_OK:
      fct_error = 1;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Call_PpNvMLightOnOffCtApMySwc_WriteBlock(0);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
    case RTE_E_NvMService_AC3_SRBS_E_NOT_OK:
      fct_error = 1;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Call_PpNvMOdometerCtApMySwc_GetErrorStatus(&Call_PpNvMOdometerCtApMySwc_GetErrorStatus_ErrorStatus);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
    case RTE_E_NvMService_AC3_SRBS_Defs_E_NOT_OK:
      fct_error = 1;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Call_PpNvMOdometerCtApMySwc_WriteBlock(0);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
    case RTE_E_NvMService_AC3_SRBS_Defs_E_NOT_OK:
      fct_error = 1;
      break;
  }

  TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter();
  TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter();
  TSC_CtApMySwc_Rte_Enter_ExArOdometer();
  TSC_CtApMySwc_Rte_Exit_ExArOdometer();


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApMySwcComM_ModeChange_FULL_COMM_Entry
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on entering of Mode <COMM_FULL_COMMUNICATION> of ModeDeclarationGroupPrototype <currentMode> of PortPrototype <PpComMCurrentModeCtApMySwc>
 *
 **********************************************************************************************************************
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpDisplayState_DeDisplayBusState(boolean data)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcComM_ModeChange_FULL_COMM_Entry_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtApMySwc_CODE) RCtApMySwcComM_ModeChange_FULL_COMM_Entry(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcComM_ModeChange_FULL_COMM_Entry
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(Rte_InitValue_PpDisplayState_DeDisplayBusState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApMySwcComM_ModeChange_FULL_COMM_Exit
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on exiting of Mode <COMM_FULL_COMMUNICATION> of ModeDeclarationGroupPrototype <currentMode> of PortPrototype <PpComMCurrentModeCtApMySwc>
 *
 **********************************************************************************************************************
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpDisplayState_DeDisplayBusState(boolean data)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcComM_ModeChange_FULL_COMM_Exit_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtApMySwc_CODE) RCtApMySwcComM_ModeChange_FULL_COMM_Exit(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcComM_ModeChange_FULL_COMM_Exit
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(Rte_InitValue_PpDisplayState_DeDisplayBusState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApMySwcInit
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed once after the RTE is started
 *
 **********************************************************************************************************************
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data)
 *   Std_ReturnType Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data)
 *   Std_ReturnType Rte_Write_PpDisplayState_DeLightOnOffCounter(uint32 data)
 *
 * Service Calls:
 * ==============
 *   Service Invocation:
 *   -------------------
 *   Std_ReturnType Rte_Call_OpCycle_IgnitionCycle_SetOperationCycleState(Dem_OperationCycleStateType CycleState)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_OperationCycle_E_NOT_OK, RTE_E_OperationCycle_E_OK
 *
 * Exclusive Area Access:
 * ======================
 *   void Rte_Enter_ExArLightOnOffCounter(void)
 *   void Rte_Exit_ExArLightOnOffCounter(void)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcInit_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtApMySwc_CODE) RCtApMySwcInit(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcInit
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  BswM_ESH_RunRequest Write_PpBswMPostRunRequestCtApMySwc_requestedMode;
  BswM_ESH_RunRequest Write_PpBswMRunRequestCtApMySwc_requestedMode;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  (void)memset(&Write_PpBswMPostRunRequestCtApMySwc_requestedMode, 0, sizeof(Write_PpBswMPostRunRequestCtApMySwc_requestedMode));
  fct_status = TSC_CtApMySwc_Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(Write_PpBswMPostRunRequestCtApMySwc_requestedMode);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  (void)memset(&Write_PpBswMRunRequestCtApMySwc_requestedMode, 0, sizeof(Write_PpBswMRunRequestCtApMySwc_requestedMode));
  fct_status = TSC_CtApMySwc_Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(Write_PpBswMRunRequestCtApMySwc_requestedMode);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpDisplayState_DeLightOnOffCounter(Rte_InitValue_PpDisplayState_DeLightOnOffCounter);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Call_OpCycle_IgnitionCycle_SetOperationCycleState(0U);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
    case RTE_E_OperationCycle_E_NOT_OK:
      fct_error = 1;
      break;
  }

  TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter();
  TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter();


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApMySwcPostRunCode
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <requestedMode> of PortPrototype <PpMrpPShutdownTimerDisplayCtApMySwc>
 *   - triggered on entering of Mode <POSTRUN> of ModeDeclarationGroupPrototype <BswM_MDGP_ESH_Mode> of PortPrototype <PpBswMEshModeNotificationCtApMySwc>
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode(BswM_MdgShutdownTimerState *data)
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data)
 *   Std_ReturnType Rte_Write_PpLightStateFront_DeLightState(IdtLightState data)
 *   Std_ReturnType Rte_Write_PpLightStateRear_DeLightState(IdtLightState data)
 *
 * Service Calls:
 * ==============
 *   Service Invocation:
 *   -------------------
 *   Std_ReturnType Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(ComM_ModeType ComMode)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_ComM_UserRequest_E_MODE_LIMITATION, RTE_E_ComM_UserRequest_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcPostRunCode_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtApMySwc_CODE) RCtApMySwcPostRunCode(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApMySwcPostRunCode
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  BswM_MdgShutdownTimerState Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode;

  uint32 PimPimLightOnOffCounter;
  uint32 PimPimOdometer;

  uint32 CalOdometer_data;

  BswM_ESH_RunRequest Write_PpBswMPostRunRequestCtApMySwc_requestedMode;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  PimPimLightOnOffCounter = *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter();
  *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter() = PimPimLightOnOffCounter;
  PimPimOdometer = *TSC_CtApMySwc_Rte_Pim_PimOdometer();
  *TSC_CtApMySwc_Rte_Pim_PimOdometer() = PimPimOdometer;

  CalOdometer_data = TSC_CtApMySwc_Rte_CData_CalOdometer();

  fct_status = TSC_CtApMySwc_Rte_Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode(&Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_NEVER_RECEIVED:
      fct_error = 1;
      break;
    case RTE_E_INVALID:
      fct_error = 1;
      break;
    case RTE_E_MAX_AGE_EXCEEDED:
      fct_error = 1;
      break;
  }

  (void)memset(&Write_PpBswMPostRunRequestCtApMySwc_requestedMode, 0, sizeof(Write_PpBswMPostRunRequestCtApMySwc_requestedMode));
  fct_status = TSC_CtApMySwc_Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(Write_PpBswMPostRunRequestCtApMySwc_requestedMode);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpLightStateFront_DeLightState(Rte_InitValue_PpLightStateFront_DeLightState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Write_PpLightStateRear_DeLightState(Rte_InitValue_PpLightStateRear_DeLightState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtApMySwc_Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(0U);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
    case RTE_E_ComM_UserRequest_E_MODE_LIMITATION:
      fct_error = 1;
      break;
    case RTE_E_ComM_UserRequest_E_NOT_OK:
      fct_error = 1;
      break;
  }


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtApMySwc_STOP_SEC_CODE
#include "CtApMySwc_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

static void CtApMySwc_TestDefines(void)
{
  /* Enumeration Data Types */

  BswM_ESH_Mode Test_BswM_ESH_Mode_V_1 = STARTUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_2 = RUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_3 = POSTRUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_4 = WAKEUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_5 = SHUTDOWN;

  BswM_ESH_RunRequest Test_BswM_ESH_RunRequest_V_1 = RELEASED;
  BswM_ESH_RunRequest Test_BswM_ESH_RunRequest_V_2 = REQUESTED;

  BswM_MdgShutdownTimerState Test_BswM_MdgShutdownTimerState_V_1 = TIMER_EXPIRED;
  BswM_MdgShutdownTimerState Test_BswM_MdgShutdownTimerState_V_2 = TIMER_NOT_EXPIRED;

  ComM_ModeType Test_ComM_ModeType_V_1 = COMM_NO_COMMUNICATION;
  ComM_ModeType Test_ComM_ModeType_V_2 = COMM_SILENT_COMMUNICATION;
  ComM_ModeType Test_ComM_ModeType_V_3 = COMM_FULL_COMMUNICATION;

  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_1 = DCM_E_POSITIVERESPONSE;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_2 = DCM_E_GENERALREJECT;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_3 = DCM_E_SERVICENOTSUPPORTED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_4 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_5 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_6 = DCM_E_RESPONSETOOLONG;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_7 = DCM_E_BUSYREPEATREQUEST;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_8 = DCM_E_CONDITIONSNOTCORRECT;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_9 = DCM_E_REQUESTSEQUENCEERROR;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_10 = DCM_E_NORESPONSEFROMSUBNETCOMPONENT;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_11 = DCM_E_FAILUREPREVENTSEXECUTIONOFREQUESTEDACTION;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_12 = DCM_E_REQUESTOUTOFRANGE;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_13 = DCM_E_SECURITYACCESSDENIED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_14 = DCM_E_INVALIDKEY;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_15 = DCM_E_EXCEEDNUMBEROFATTEMPTS;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_16 = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_17 = DCM_E_UPLOADDOWNLOADNOTACCEPTED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_18 = DCM_E_TRANSFERDATASUSPENDED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_19 = DCM_E_GENERALPROGRAMMINGFAILURE;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_20 = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_21 = DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_22 = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_23 = DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_24 = DCM_E_RPMTOOHIGH;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_25 = DCM_E_RPMTOOLOW;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_26 = DCM_E_ENGINEISRUNNING;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_27 = DCM_E_ENGINEISNOTRUNNING;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_28 = DCM_E_ENGINERUNTIMETOOLOW;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_29 = DCM_E_TEMPERATURETOOHIGH;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_30 = DCM_E_TEMPERATURETOOLOW;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_31 = DCM_E_VEHICLESPEEDTOOHIGH;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_32 = DCM_E_VEHICLESPEEDTOOLOW;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_33 = DCM_E_THROTTLE_PEDALTOOHIGH;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_34 = DCM_E_THROTTLE_PEDALTOOLOW;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_35 = DCM_E_TRANSMISSIONRANGENOTINNEUTRAL;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_36 = DCM_E_TRANSMISSIONRANGENOTINGEAR;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_37 = DCM_E_BRAKESWITCH_NOTCLOSED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_38 = DCM_E_SHIFTERLEVERNOTINPARK;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_39 = DCM_E_TORQUECONVERTERCLUTCHLOCKED;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_40 = DCM_E_VOLTAGETOOHIGH;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_41 = DCM_E_VOLTAGETOOLOW;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_42 = DCM_E_VMSCNC_0;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_43 = DCM_E_VMSCNC_1;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_44 = DCM_E_VMSCNC_2;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_45 = DCM_E_VMSCNC_3;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_46 = DCM_E_VMSCNC_4;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_47 = DCM_E_VMSCNC_5;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_48 = DCM_E_VMSCNC_6;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_49 = DCM_E_VMSCNC_7;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_50 = DCM_E_VMSCNC_8;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_51 = DCM_E_VMSCNC_9;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_52 = DCM_E_VMSCNC_A;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_53 = DCM_E_VMSCNC_B;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_54 = DCM_E_VMSCNC_C;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_55 = DCM_E_VMSCNC_D;
  Dcm_NegativeResponseCodeType Test_Dcm_NegativeResponseCodeType_V_56 = DCM_E_VMSCNC_E;

  Dcm_OpStatusType Test_Dcm_OpStatusType_V_1 = DCM_INITIAL;
  Dcm_OpStatusType Test_Dcm_OpStatusType_V_2 = DCM_PENDING;
  Dcm_OpStatusType Test_Dcm_OpStatusType_V_3 = DCM_CANCEL;
  Dcm_OpStatusType Test_Dcm_OpStatusType_V_4 = DCM_FORCE_RCRRP_OK;
  Dcm_OpStatusType Test_Dcm_OpStatusType_V_5 = DCM_FORCE_RCRRP_NOT_OK;

  Dem_OperationCycleStateType Test_Dem_OperationCycleStateType_V_1 = DEM_CYCLE_STATE_START;
  Dem_OperationCycleStateType Test_Dem_OperationCycleStateType_V_2 = DEM_CYCLE_STATE_END;

  IdtDisplayIgnitionState Test_IdtDisplayIgnitionState_V_1 = CMIGNITION_STATEON;
  IdtDisplayIgnitionState Test_IdtDisplayIgnitionState_V_2 = CMIGNITION_STATEOFF;

  IdtDoorState Test_IdtDoorState_V_1 = CMDOORSTATE_DOORCLOSED;
  IdtDoorState Test_IdtDoorState_V_2 = CMDOORSTATE_DOOROPEN;

  IdtLightState Test_IdtLightState_V_1 = CMLIGHTSTATE_LIGHTOFF;
  IdtLightState Test_IdtLightState_V_2 = CMLIGHTSTATE_LIGHTON;

  NvM_RequestResultType Test_NvM_RequestResultType_V_1 = NVM_REQ_OK;
  NvM_RequestResultType Test_NvM_RequestResultType_V_2 = NVM_REQ_NOT_OK;
  NvM_RequestResultType Test_NvM_RequestResultType_V_3 = NVM_REQ_PENDING;
  NvM_RequestResultType Test_NvM_RequestResultType_V_4 = NVM_REQ_INTEGRITY_FAILED;
  NvM_RequestResultType Test_NvM_RequestResultType_V_5 = NVM_REQ_BLOCK_SKIPPED;
  NvM_RequestResultType Test_NvM_RequestResultType_V_6 = NVM_REQ_NV_INVALIDATED;
  NvM_RequestResultType Test_NvM_RequestResultType_V_7 = NVM_REQ_CANCELED;
  NvM_RequestResultType Test_NvM_RequestResultType_V_8 = NVM_REQ_REDUNDANCY_FAILED;
  NvM_RequestResultType Test_NvM_RequestResultType_V_9 = NVM_REQ_RESTORED_FROM_ROM;

  NvM_ServiceIdType Test_NvM_ServiceIdType_V_1 = NVM_READ_BLOCK;
  NvM_ServiceIdType Test_NvM_ServiceIdType_V_2 = NVM_WRITE_BLOCK;
  NvM_ServiceIdType Test_NvM_ServiceIdType_V_3 = NVM_RESTORE_BLOCK_DEFAULTS;
  NvM_ServiceIdType Test_NvM_ServiceIdType_V_4 = NVM_ERASE_BLOCK;
  NvM_ServiceIdType Test_NvM_ServiceIdType_V_5 = NVM_INVALIDATE_NV_BLOCK;
  NvM_ServiceIdType Test_NvM_ServiceIdType_V_6 = NVM_READ_ALL;

  /* Modes */

  uint8 Test_ComMMode_MV_1 = RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION;
  uint8 Test_ComMMode_MV_2 = RTE_MODE_ComMMode_COMM_NO_COMMUNICATION;
  uint8 Test_ComMMode_MV_3 = RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION;
  uint8 Test_ComMMode_TV = RTE_TRANSITION_ComMMode;

  BswM_ESH_Mode Test_ESH_Mode_MV_1 = RTE_MODE_ESH_Mode_POSTRUN;
  BswM_ESH_Mode Test_ESH_Mode_MV_2 = RTE_MODE_ESH_Mode_RUN;
  BswM_ESH_Mode Test_ESH_Mode_MV_3 = RTE_MODE_ESH_Mode_SHUTDOWN;
  BswM_ESH_Mode Test_ESH_Mode_MV_4 = RTE_MODE_ESH_Mode_STARTUP;
  BswM_ESH_Mode Test_ESH_Mode_MV_5 = RTE_MODE_ESH_Mode_WAKEUP;
  BswM_ESH_Mode Test_ESH_Mode_TV = RTE_TRANSITION_ESH_Mode;
}

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of function definition area >>              DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of removed code area >>                   DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of removed code area >>                     DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 MISRA 2004 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_TestCode:
     Reason:     This justification is used within the generated test code by the Rte Analyzer.
     Risk:       No functional risk.
     Prevention: Not required.

*/
