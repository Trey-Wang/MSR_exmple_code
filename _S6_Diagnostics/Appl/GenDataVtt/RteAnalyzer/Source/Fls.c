/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Fls.c
 *        Config:  MyECU.dpa
 *    BSW Module:  Fls
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for BSW Module <Fls>
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

#include "SchM_Fls.h"
#include "TSC_SchM_Fls.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "string.h"


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
 * ComM_InhibitionStatusType: Integer in interval [0...255]
 * ComM_UserHandleType: Integer in interval [0...255]
 * Dem_DTCGroupType: Integer in interval [0...16777215]
 * Dem_DTCStatusMaskType: Integer in interval [0...255]
 * Dem_EventIdType: Integer in interval [1...65535]
 * Dem_OperationCycleIdType: Integer in interval [0...255]
 * Dem_RatioIdType: Integer in interval [1...65535]
 * EcuM_TimeType: Integer in interval [0...4294967295]
 * IdtDioValueType: Integer in interval [0...255]
 * NetworkHandleType: Integer in interval [0...255]
 * NvM_BlockIdType: Integer in interval [1...32767]
 * boolean: Boolean (standard type)
 * boolean: Boolean (standard type)
 * dtRef_VOID: DataReference
 * dtRef_const_VOID: DataReference
 * sint8: Integer in interval [-128...127] (standard type)
 * uint16: Integer in interval [0...65535] (standard type)
 * uint32: Integer in interval [0...4294967295] (standard type)
 * uint32: Integer in interval [0...4294967295] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 *
 * Enumeration Types:
 * ==================
 * BswM_ESH_Mode: Enumeration of integer in interval [0...255] with enumerators
 * BswM_ESH_RunRequest: Enumeration of integer in interval [0...255] with enumerators
 * BswM_MdgShutdownTimerState: Enumeration of integer in interval [0...255] with enumerators
 * ComM_ModeType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_CommunicationModeType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_ConfirmationStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_ControlDtcSettingType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_DiagnosticSessionControlType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_EcuResetType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_NegativeResponseCodeType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_OpStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_ProtocolType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_RequestKindType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_SecLevelType: Enumeration of integer in interval [0...255] with enumerators
 * Dcm_SesCtrlType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_DTCFormatType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_DTCKindType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_DTCOriginType: Enumeration of integer in interval [0...65535] with enumerators
 * Dem_DTCSeverityType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_DTRControlType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_DebounceResetStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_DebouncingStateType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_EventStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_IndicatorStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_InitMonitorReasonType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_IumprDenomCondIdType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_IumprDenomCondStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_IumprReadinessGroupType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_MonitorStatusType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_OperationCycleStateType: Enumeration of integer in interval [0...255] with enumerators
 * Dem_UdsStatusByteType: Enumeration of integer in interval [0...255] with enumerators
 * EcuM_BootTargetType: Enumeration of integer in interval [0...255] with enumerators
 * EcuM_ModeType: Enumeration of integer in interval [0...255] with enumerators
 * EcuM_ShutdownCauseType: Enumeration of integer in interval [0...255] with enumerators
 * EcuM_StateType: Enumeration of integer in interval [0...255] with enumerators
 * EcuM_UserType: Enumeration of integer in interval [0...255] with enumerators
 * IdtDisplayIgnitionState: Enumeration of integer in interval [0...255] with enumerators
 * IdtDoorState: Enumeration of integer in interval [0...1] with enumerators
 * IdtLightState: Enumeration of integer in interval [0...1] with enumerators
 * NvM_RequestResultType: Enumeration of integer in interval [0...255] with enumerators
 * NvM_RequestResultType: Enumeration of integer in interval [0...255] with enumerators
 * NvM_ServiceIdType: Enumeration of integer in interval [0...255] with enumerators
 * NvM_ServiceIdType: Enumeration of integer in interval [0...255] with enumerators
 *
 * Array Types:
 * ============
 * DataArrayType_uint8_4: Array with 4 element(s) of type uint8
 * Dcm_Data2ByteType: Array with 2 element(s) of type uint8
 * Dcm_Data4ByteType: Array with 4 element(s) of type uint8
 * Dem_MaxDataValueType: Array with 4 element(s) of type uint8
 *
 *********************************************************************************************************************/


#define FLS_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Schedulable Entity Name: Fls_MainFunction
 *
 *********************************************************************************************************************/

FUNC(void, FLS_CODE) Fls_MainFunction(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: Fls_MainFunction
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define FLS_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


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
