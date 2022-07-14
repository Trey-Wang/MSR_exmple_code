/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Rte_CtApMySwc.h
 *        Config:  D:/usr/usage/Presentations/MSR_Exercises/MSR_Exercises_AiP4_UPDATE/MSR_Exercises_AiP4/_S6_Diagnostics/MyECU.dpa
 *     SW-C Type:  CtApMySwc
 *  Generated at:  Tue Jun 26 09:53:35 2018
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Application header file for SW-C <CtApMySwc> (Contract Phase)
 *********************************************************************************************************************/

/* double include prevention */
#ifndef _RTE_CTAPMYSWC_H
# define _RTE_CTAPMYSWC_H

# ifdef RTE_APPLICATION_HEADER_FILE
#  error Multiple application header files included.
# endif
# define RTE_APPLICATION_HEADER_FILE
# ifndef RTE_PTR2ARRAYBASETYPE_PASSING
#  define RTE_PTR2ARRAYBASETYPE_PASSING
# endif

# ifdef __cplusplus
extern "C"
{
# endif /* __cplusplus */

/* include files */

# include "Rte_CtApMySwc_Type.h"
# include "Rte_DataHandleType.h"


/**********************************************************************************************************************
 * Component Data Structures and Port Data Structures
 *********************************************************************************************************************/

struct Rte_CDS_CtApMySwc
{
  /* Data Handles section */
  P2VAR(Rte_DE_IdtDoorState, TYPEDEF, RTE_CTAPMYSWC_APPL_VAR) RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState; /* PRQA S 0850 */ /* MD_MSR_19.8 */
  P2VAR(Rte_DE_IdtDoorState, TYPEDEF, RTE_CTAPMYSWC_APPL_VAR) RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState; /* PRQA S 0850 */ /* MD_MSR_19.8 */
  /* PIM Handles section */
  P2VAR(uint32, TYPEDEF, RTE_VAR_DEFAULT_RTE_PIM_GROUP) Pim_PimLightOnOffCounter; /* PRQA S 0850 */ /* MD_MSR_19.8 */
  P2VAR(uint32, TYPEDEF, RTE_VAR_DEFAULT_RTE_PIM_GROUP) Pim_PimOdometer; /* PRQA S 0850 */ /* MD_MSR_19.8 */
  /* Vendor specific section */
};

# define RTE_START_SEC_CONST_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern CONSTP2CONST(struct Rte_CDS_CtApMySwc, RTE_CONST, RTE_CONST) Rte_Inst_CtApMySwc; /* PRQA S 0850 */ /* MD_MSR_19.8 */

# define RTE_STOP_SEC_CONST_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef P2CONST(struct Rte_CDS_CtApMySwc, TYPEDEF, RTE_CONST) Rte_Instance;


/**********************************************************************************************************************
 * Init Values for unqueued S/R communication (primitive types only)
 *********************************************************************************************************************/

# define Rte_InitValue_PpDisplayState_DeDisplayBusState (FALSE)
# define Rte_InitValue_PpDisplayState_DeLightOnOffCounter (0U)
# define Rte_InitValue_PpDisplayState_DeOdometerValue (0U)
# define Rte_InitValue_PpDisplayState_DeOdometerWriteRequestPending (FALSE)
# define Rte_InitValue_PpDoorStateFrontLeft_DeDoorState (0U)
# define Rte_InitValue_PpDoorStateFrontRight_DeDoorState (0U)
# define Rte_InitValue_PpDoorStateRearLeft_DeDoorState (0U)
# define Rte_InitValue_PpDoorStateRearRight_DeDoorState (0U)
# define Rte_InitValue_PpLightStateFront_DeLightState (0U)
# define Rte_InitValue_PpLightStateRear_DeLightState (0U)
# define Rte_InitValue_PpLocalIgnitionState_DeIgnitionState (1U)


# define RTE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * API prototypes
 *********************************************************************************************************************/
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_CtApMySwc_PpDoorStateFrontRight_DeDoorState(P2VAR(IdtDoorState, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_CtApMySwc_PpDoorStateRearRight_DeDoorState(P2VAR(IdtDoorState, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_CtApMySwc_PpLocalIgnitionState_DeIgnitionState(P2VAR(IdtDisplayIgnitionState, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_CtApMySwc_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode(P2VAR(BswM_MdgShutdownTimerState, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpBswMPostRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpBswMRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeDisplayBusState(boolean data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeLightOnOffCounter(uint32 data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeOdometerValue(uint32 data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeOdometerWriteRequestPending(boolean data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpLightStateFront_DeLightState(IdtLightState data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpLightStateRear_DeLightState(IdtLightState data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Call_CtApMySwc_OpCycle_IgnitionCycle_SetOperationCycleState(Dem_OperationCycleStateType CycleState); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Call_CtApMySwc_PpComMUserRequestCtApMySwc_RequestComMode(ComM_ModeType ComMode); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Call_CtApMySwc_PpNvMLightOnOffCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Call_CtApMySwc_PpNvMOdometerCtApMySwc_GetErrorStatus(P2VAR(NvM_RequestResultType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) ErrorStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(Std_ReturnType, RTE_CODE) Rte_Call_CtApMySwc_PpNvMOdometerCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(uint16, RTE_CODE) Rte_IrvRead_CtApMySwc_RCtApMySwcCode_IrvDoorStateOld(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(void, RTE_CODE) Rte_IrvWrite_CtApMySwc_RCtApMySwcCode_IrvDoorStateOld(uint16 data); /* PRQA S 0850 */ /* MD_MSR_19.8 */

FUNC(void, RTE_CODE) Rte_Enter_CtApMySwc_ExArLightOnOffCounter(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(void, RTE_CODE) Rte_Exit_CtApMySwc_ExArLightOnOffCounter(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

FUNC(void, RTE_CODE) Rte_Enter_CtApMySwc_ExArOdometer(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(void, RTE_CODE) Rte_Exit_CtApMySwc_ExArOdometer(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

FUNC(uint32, RTE_CODE) Rte_CData_CtApMySwc_CalOdometer(void); /* PRQA S 0850, 3447 */ /* MD_MSR_19.8, MD_Rte_3447 */

# define RTE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */



/**********************************************************************************************************************
 * Rte_IRead_<r>_<p>_<d>
 * Rte_IStatus_<r>_<p>_<d>
 * Rte_IWrite_<r>_<p>_<d>
 * Rte_IWriteRef_<r>_<p>_<d>
 * Rte_IInvalidate_<r>_<p>_<d>
 *********************************************************************************************************************/

/* PRQA S 3453 L1 */ /* MD_MSR_19.7 */
# define Rte_IRead_RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState() \
  (Rte_Inst_CtApMySwc->RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState->value)
/* PRQA L:L1 */

/* PRQA S 3453 L1 */ /* MD_MSR_19.7 */
# define Rte_IRead_RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState() \
  (Rte_Inst_CtApMySwc->RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState->value)
/* PRQA L:L1 */


/**********************************************************************************************************************
 * Rte_Read_<p>_<d> (explicit S/R communication with isQueued = false)
 *********************************************************************************************************************/
# define Rte_Read_PpDoorStateFrontRight_DeDoorState Rte_Read_CtApMySwc_PpDoorStateFrontRight_DeDoorState
# define Rte_Read_PpDoorStateRearRight_DeDoorState Rte_Read_CtApMySwc_PpDoorStateRearRight_DeDoorState
# define Rte_Read_PpLocalIgnitionState_DeIgnitionState Rte_Read_CtApMySwc_PpLocalIgnitionState_DeIgnitionState
# define Rte_Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode Rte_Read_CtApMySwc_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode


/**********************************************************************************************************************
 * Rte_Write_<p>_<d> (explicit S/R communication with isQueued = false)
 *********************************************************************************************************************/
# define Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode Rte_Write_CtApMySwc_PpBswMPostRunRequestCtApMySwc_requestedMode
# define Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode Rte_Write_CtApMySwc_PpBswMRunRequestCtApMySwc_requestedMode
# define Rte_Write_PpDisplayState_DeDisplayBusState Rte_Write_CtApMySwc_PpDisplayState_DeDisplayBusState
# define Rte_Write_PpDisplayState_DeLightOnOffCounter Rte_Write_CtApMySwc_PpDisplayState_DeLightOnOffCounter
# define Rte_Write_PpDisplayState_DeOdometerValue Rte_Write_CtApMySwc_PpDisplayState_DeOdometerValue
# define Rte_Write_PpDisplayState_DeOdometerWriteRequestPending Rte_Write_CtApMySwc_PpDisplayState_DeOdometerWriteRequestPending
# define Rte_Write_PpLightStateFront_DeLightState Rte_Write_CtApMySwc_PpLightStateFront_DeLightState
# define Rte_Write_PpLightStateRear_DeLightState Rte_Write_CtApMySwc_PpLightStateRear_DeLightState


/**********************************************************************************************************************
 * Rte_Call_<p>_<o> (C/S invocation)
 *********************************************************************************************************************/
# define Rte_Call_OpCycle_IgnitionCycle_SetOperationCycleState Rte_Call_CtApMySwc_OpCycle_IgnitionCycle_SetOperationCycleState
# define Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode Rte_Call_CtApMySwc_PpComMUserRequestCtApMySwc_RequestComMode
# define Rte_Call_PpNvMLightOnOffCtApMySwc_WriteBlock Rte_Call_CtApMySwc_PpNvMLightOnOffCtApMySwc_WriteBlock
# define Rte_Call_PpNvMOdometerCtApMySwc_GetErrorStatus Rte_Call_CtApMySwc_PpNvMOdometerCtApMySwc_GetErrorStatus
# define Rte_Call_PpNvMOdometerCtApMySwc_WriteBlock Rte_Call_CtApMySwc_PpNvMOdometerCtApMySwc_WriteBlock


/**********************************************************************************************************************
 * Exclusive Areas
 *********************************************************************************************************************/

# define Rte_Enter_ExArLightOnOffCounter Rte_Enter_CtApMySwc_ExArLightOnOffCounter /* RteAnalyzer(ExclusiveArea, OS_INTERRUPT_BLOCKING) */
# define Rte_Exit_ExArLightOnOffCounter Rte_Exit_CtApMySwc_ExArLightOnOffCounter /* RteAnalyzer(ExclusiveArea, OS_INTERRUPT_BLOCKING) */

# define Rte_Enter_ExArOdometer Rte_Enter_CtApMySwc_ExArOdometer /* RteAnalyzer(ExclusiveArea, OS_INTERRUPT_BLOCKING) */
# define Rte_Exit_ExArOdometer Rte_Exit_CtApMySwc_ExArOdometer /* RteAnalyzer(ExclusiveArea, OS_INTERRUPT_BLOCKING) */


/**********************************************************************************************************************
 * Inter-runnable variables
 *********************************************************************************************************************/

/* PRQA S 3453 L1 */ /* MD_MSR_19.7 */
# define Rte_IrvRead_RCtApMySwcCode_IrvDoorStateOld() \
  Rte_IrvRead_CtApMySwc_RCtApMySwcCode_IrvDoorStateOld()
/* PRQA L:L1 */

/* PRQA S 3453 L1 */ /* MD_MSR_19.7 */
# define Rte_IrvWrite_RCtApMySwcCode_IrvDoorStateOld(data) \
  Rte_IrvWrite_CtApMySwc_RCtApMySwcCode_IrvDoorStateOld(data)
/* PRQA L:L1 */


/**********************************************************************************************************************
 * Rte_CData (SW-C local calibration parameters)
 *********************************************************************************************************************/

# define Rte_CData_CalOdometer Rte_CData_CtApMySwc_CalOdometer

/**********************************************************************************************************************
 * Per-Instance Memory User Types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Rte_Pim (Per-Instance Memory)
 *********************************************************************************************************************/

# define Rte_Pim_PimLightOnOffCounter() (Rte_Inst_CtApMySwc->Pim_PimLightOnOffCounter) /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define Rte_Pim_PimOdometer() (Rte_Inst_CtApMySwc->Pim_PimOdometer) /* PRQA S 3453 */ /* MD_MSR_19.7 */




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


# define CtApMySwc_START_SEC_CODE
# include "CtApMySwc_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


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

# define RTE_RUNNABLE_CBReadData_OccurrenceCounter_OccurenceCounter_ReadData CBReadData_OccurrenceCounter_OccurenceCounter_ReadData
# ifdef RTE_PTR2ARRAYBASETYPE_PASSING
FUNC(Std_ReturnType, CtApMySwc_CODE) CBReadData_OccurrenceCounter_OccurenceCounter_ReadData(P2VAR(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# else
FUNC(Std_ReturnType, CtApMySwc_CODE) CBReadData_OccurrenceCounter_OccurenceCounter_ReadData(P2VAR(DataArrayType_uint8_4, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# endif

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

# define RTE_RUNNABLE_CBReadData_OdometerValue_OdometerValue_ReadData CBReadData_OdometerValue_OdometerValue_ReadData
# ifdef RTE_PTR2ARRAYBASETYPE_PASSING
FUNC(Std_ReturnType, CtApMySwc_CODE) CBReadData_OdometerValue_OdometerValue_ReadData(P2VAR(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# else
FUNC(Std_ReturnType, CtApMySwc_CODE) CBReadData_OdometerValue_OdometerValue_ReadData(P2VAR(DataArrayType_uint8_4, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# endif

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

# define RTE_RUNNABLE_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead
FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead(Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) ErrorCode); /* PRQA S 0850 */ /* MD_MSR_19.8 */

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

# define RTE_RUNNABLE_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData
# ifdef RTE_PTR2ARRAYBASETYPE_PASSING
FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData(Dcm_OpStatusType OpStatus, P2VAR(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# else
FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData(Dcm_OpStatusType OpStatus, P2VAR(Dcm_Data4ByteType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) Data); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# endif

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

# define RTE_RUNNABLE_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData
# ifdef RTE_PTR2ARRAYBASETYPE_PASSING
FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData(P2CONST(uint8, AUTOMATIC, RTE_CTAPMYSWC_APPL_DATA) Data, Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) ErrorCode); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# else
FUNC(Std_ReturnType, CtApMySwc_CODE) DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData(P2CONST(Dcm_Data4ByteType, AUTOMATIC, RTE_CTAPMYSWC_APPL_DATA) Data, Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_CTAPMYSWC_APPL_VAR) ErrorCode); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# endif

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

# define RTE_RUNNABLE_PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished
FUNC(void, CtApMySwc_CODE) PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult); /* PRQA S 0850 */ /* MD_MSR_19.8 */

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

# define RTE_RUNNABLE_RCtApMySwcCode RCtApMySwcCode
FUNC(void, CtApMySwc_CODE) RCtApMySwcCode(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

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

# define RTE_RUNNABLE_RCtApMySwcComM_ModeChange_FULL_COMM_Entry RCtApMySwcComM_ModeChange_FULL_COMM_Entry
FUNC(void, CtApMySwc_CODE) RCtApMySwcComM_ModeChange_FULL_COMM_Entry(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

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

# define RTE_RUNNABLE_RCtApMySwcComM_ModeChange_FULL_COMM_Exit RCtApMySwcComM_ModeChange_FULL_COMM_Exit
FUNC(void, CtApMySwc_CODE) RCtApMySwcComM_ModeChange_FULL_COMM_Exit(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

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

# define RTE_RUNNABLE_RCtApMySwcInit RCtApMySwcInit
FUNC(void, CtApMySwc_CODE) RCtApMySwcInit(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

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

# define RTE_RUNNABLE_RCtApMySwcPostRunCode RCtApMySwcPostRunCode
FUNC(void, CtApMySwc_CODE) RCtApMySwcPostRunCode(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

# define CtApMySwc_STOP_SEC_CODE
# include "CtApMySwc_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Application errors
 *********************************************************************************************************************/

# define RTE_E_CSDataServices_OccurrenceCounter_OccurenceCounter_E_NOT_OK (1U)

# define RTE_E_CSDataServices_OdometerValue_OdometerValue_E_NOT_OK (1U)

# define RTE_E_ComM_UserRequest_E_MODE_LIMITATION (2U)

# define RTE_E_ComM_UserRequest_E_NOT_OK (1U)

# define RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_DCM_E_PENDING (10U)

# define RTE_E_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_E_NOT_OK (1U)

# define RTE_E_NvMService_AC3_SRBS_E_NOT_OK (1U)

# define RTE_E_NvMService_AC3_SRBS_Defs_E_NOT_OK (1U)

# define RTE_E_OperationCycle_E_NOT_OK (1U)

# define RTE_E_OperationCycle_E_OK (0U)

# ifdef __cplusplus
} /* extern "C" */
# endif /* __cplusplus */

#endif /* _RTE_CTAPMYSWC_H */

/**********************************************************************************************************************
 MISRA 2004 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_3447:  MISRA rule: 8.8
     Reason:     For the purpose of monitoring during calibration or debugging it is necessary to use non-static declarations.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       No functional risk.
     Prevention: Not required.

*/
