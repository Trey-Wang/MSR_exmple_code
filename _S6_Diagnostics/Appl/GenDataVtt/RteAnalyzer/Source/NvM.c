/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  NvM.c
 *        Config:  MyECU.dpa
 *     SW-C Type:  NvM
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for SW-C <NvM>
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
 * NvM_BlockIdType
 *   
 *
 * NvM_RequestResultType
 *   
 *
 * NvM_ServiceIdType
 *   
 *
 *********************************************************************************************************************/

#include "Rte_NvM.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */
#include "TSC_NvM.h"
#include "SchM_NvM.h"
#include "TSC_SchM_NvM.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "string.h"

static void NvM_TestDefines(void);

typedef P2FUNC(Std_ReturnType, RTE_CODE, FncPtrType)(void); /* PRQA S 3448 */ /* MD_Rte_TestCode */


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
 * NvM_BlockIdType: Integer in interval [1...32767]
 * boolean: Boolean (standard type)
 * dtRef_VOID: DataReference
 * dtRef_const_VOID: DataReference
 * uint16: Integer in interval [0...65535] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 *
 * Enumeration Types:
 * ==================
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
 *********************************************************************************************************************/


#define NvM_START_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: EraseBlock
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <EraseBlock> of PortPrototype <PS_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <EraseBlock> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <EraseBlock> of PortPrototype <PS_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_EraseNvBlock(void)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: EraseBlock_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_EraseNvBlock(NvM_BlockIdType parg0) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_EraseNvBlock (returns application error)
 *********************************************************************************************************************/

  NvM_TestDefines();

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: GetErrorStatus
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <GetErrorStatus> of PortPrototype <PS_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <GetErrorStatus> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <GetErrorStatus> of PortPrototype <PS_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_GetErrorStatus(NvM_RequestResultType *ErrorStatus)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: GetErrorStatus_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_GetErrorStatus(NvM_BlockIdType parg0, P2VAR(NvM_RequestResultType, AUTOMATIC, RTE_NVM_APPL_VAR) ErrorStatus) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_GetErrorStatus (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: InvalidateNvBlock
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <InvalidateNvBlock> of PortPrototype <PS_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <InvalidateNvBlock> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <InvalidateNvBlock> of PortPrototype <PS_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_InvalidateNvBlock(void)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: InvalidateNvBlock_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_InvalidateNvBlock(NvM_BlockIdType parg0) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_InvalidateNvBlock (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: NvM_MainFunction
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on TimingEvent every 10ms
 *
 **********************************************************************************************************************
 *
 * Service Calls:
 * ==============
 *   Service Invocation:
 *   -------------------
 *   Std_ReturnType Rte_Call_NvM_RpNotifyJobEnd_CpApMySwcSnNvMPimOdometer_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
 *     Synchronous Service Invocation. Timeout: None
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_MainFunction_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, NvM_CODE) NvM_MainFunction(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_MainFunction
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  /**********************************************************
  * Direct Function Accesses and Take Addresses of Functions
  **********************************************************/

  {
    FncPtrType NvM_FctPtr; /* PRQA S 3408 */ /* MD_Rte_TestCode */
    NvM_FctPtr = (FncPtrType)TSC_NvM_Rte_Call_NvM_RpNotifyJobEnd_CpApMySwcSnNvMPimOdometer_JobFinished; /* PRQA S 0313 */ /* MD_Rte_TestCode */
  }
  fct_status = TSC_NvM_Rte_Call_NvM_RpNotifyJobEnd_CpApMySwcSnNvMPimOdometer_JobFinished(0U, 0U);
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
  }

  TSC_NvM_SchM_Enter_NvM_NVM_EXCLUSIVE_AREA_0();
  TSC_NvM_SchM_Exit_NvM_NVM_EXCLUSIVE_AREA_0();


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: ReadBlock
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadBlock> of PortPrototype <PS_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <ReadBlock> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <ReadBlock> of PortPrototype <PS_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_ReadBlock(dtRef_VOID DstPtr)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: ReadBlock_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_ReadBlock(NvM_BlockIdType parg0, dtRef_VOID DstPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_ReadBlock (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RestoreBlockDefaults
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <RestoreBlockDefaults> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_RestoreBlockDefaults(dtRef_VOID DstPtr)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_Defs_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RestoreBlockDefaults_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_RestoreBlockDefaults(NvM_BlockIdType parg0, dtRef_VOID DstPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_RestoreBlockDefaults (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: SetBlockProtection
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <SetBlockProtection> of PortPrototype <PAdmin_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <SetBlockProtection> of PortPrototype <PAdmin_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <SetBlockProtection> of PortPrototype <PAdmin_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_SetBlockProtection(boolean ProtectionEnabled)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMAdministration_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: SetBlockProtection_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_SetBlockProtection(NvM_BlockIdType parg0, boolean ProtectionEnabled) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_SetBlockProtection (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: SetRamBlockStatus
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <SetRamBlockStatus> of PortPrototype <PS_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <SetRamBlockStatus> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <SetRamBlockStatus> of PortPrototype <PS_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_SetRamBlockStatus(boolean RamBlockStatus)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: SetRamBlockStatus_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_SetRamBlockStatus(NvM_BlockIdType parg0, boolean RamBlockStatus) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_SetRamBlockStatus (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: WriteBlock
 *
 * This runnable can be invoked concurrently (reentrant implementation).
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <WriteBlock> of PortPrototype <PS_CpApMySwcSnNvMPimLightOnOffCounter>
 *   - triggered by server invocation for OperationPrototype <WriteBlock> of PortPrototype <PS_CpApMySwcSnNvMPimOdometer>
 *   - triggered by server invocation for OperationPrototype <WriteBlock> of PortPrototype <PS_NvMConfigBlock>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   Std_ReturnType NvM_WriteBlock(dtRef_const_VOID SrcPtr)
 *
 **********************************************************************************************************************
 *
 * Available Application Errors:
 * =============================
 *   RTE_E_NvMService_AC3_SRBS_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: WriteBlock_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(Std_ReturnType, NvM_CODE) NvM_WriteBlock(NvM_BlockIdType parg0, dtRef_const_VOID SrcPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: NvM_WriteBlock (returns application error)
 *********************************************************************************************************************/

  return RTE_E_OK;

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define NvM_STOP_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

static void NvM_TestDefines(void)
{
  /* Enumeration Data Types */

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
