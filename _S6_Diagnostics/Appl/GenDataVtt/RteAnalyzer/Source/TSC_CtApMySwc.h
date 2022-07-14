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
 *          File:  TSC_CtApMySwc.h
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Header of wrapper software component for Bte-based Rte test cases
 *********************************************************************************************************************/


/** Sender receiver - explicit read services */
Std_ReturnType TSC_CtApMySwc_Rte_Read_PpDoorStateFrontRight_DeDoorState(IdtDoorState *data);
Std_ReturnType TSC_CtApMySwc_Rte_Read_PpDoorStateRearRight_DeDoorState(IdtDoorState *data);
Std_ReturnType TSC_CtApMySwc_Rte_Read_PpLocalIgnitionState_DeIgnitionState(IdtDisplayIgnitionState *data);
Std_ReturnType TSC_CtApMySwc_Rte_Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode(BswM_MdgShutdownTimerState *data);

/** Sender receiver - explicit write services */
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(boolean data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(boolean data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeLightOnOffCounter(uint32 data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerValue(uint32 data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(boolean data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpLightStateFront_DeLightState(IdtLightState data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpLightStateRear_DeLightState(IdtLightState data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(boolean data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(boolean data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeLightOnOffCounter(uint32 data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpLightStateFront_DeLightState(IdtLightState data);
Std_ReturnType TSC_CtApMySwc_Rte_Write_PpLightStateRear_DeLightState(IdtLightState data);

/** Sender receiver - implicit read services */
IdtDoorState TSC_CtApMySwc_Rte_IRead_RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState(void);
IdtDoorState TSC_CtApMySwc_Rte_IRead_RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState(void);

/** Service interfaces */
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(ComM_ModeType ComMode);
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpNvMLightOnOffCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr);
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpNvMOdometerCtApMySwc_GetErrorStatus(NvM_RequestResultType *ErrorStatus);
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpNvMOdometerCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr);
Std_ReturnType TSC_CtApMySwc_Rte_Call_OpCycle_IgnitionCycle_SetOperationCycleState(Dem_OperationCycleStateType CycleState);
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(ComM_ModeType ComMode);

/** Explicit inter-runnable variables */
uint16 TSC_CtApMySwc_Rte_IrvRead_RCtApMySwcCode_IrvDoorStateOld(void);
void TSC_CtApMySwc_Rte_IrvWrite_RCtApMySwcCode_IrvDoorStateOld(uint16);

/** SW-C local Calibration Parameters */
uint32  TSC_CtApMySwc_Rte_CData_CalOdometer(void);

/** Exclusive Areas */
void TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter(void);
void TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter(void);
void TSC_CtApMySwc_Rte_Enter_ExArOdometer(void);
void TSC_CtApMySwc_Rte_Exit_ExArOdometer(void);

/** Per Instance Memories */
uint32 *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter(void);
uint32 *TSC_CtApMySwc_Rte_Pim_PimOdometer(void);



