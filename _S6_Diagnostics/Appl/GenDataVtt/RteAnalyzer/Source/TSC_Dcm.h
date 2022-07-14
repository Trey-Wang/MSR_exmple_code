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
 *          File:  TSC_Dcm.h
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Header of wrapper software component for Bte-based Rte test cases
 *********************************************************************************************************************/


/** Service interfaces */
Std_ReturnType TSC_Dcm_Rte_Call_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ConditionCheckRead(Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType *ErrorCode);
Std_ReturnType TSC_Dcm_Rte_Call_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_ReadData(Dcm_OpStatusType OpStatus, uint8 *Data);
Std_ReturnType TSC_Dcm_Rte_Call_DataServices_Diag_RWDI_LightOnOffCounter_LightOnOffCounter_Value_WriteData(const uint8 *Data, Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType *ErrorCode);
Std_ReturnType TSC_Dcm_Rte_Call_SecurityAccess_Level_01_CompareKey(const uint8 *Key, Dcm_OpStatusType OpStatus, Dcm_NegativeResponseCodeType *ErrorCode);
Std_ReturnType TSC_Dcm_Rte_Call_SecurityAccess_Level_01_GetSeed(Dcm_OpStatusType OpStatus, uint8 *Seed, Dcm_NegativeResponseCodeType *ErrorCode);

/** Mode switches */
Std_ReturnType TSC_Dcm_Rte_Switch_DcmControlDtcSetting_DcmControlDtcSetting(Dcm_ControlDtcSettingType mode);
Std_ReturnType TSC_Dcm_Rte_Switch_DcmDiagnosticSessionControl_DcmDiagnosticSessionControl(Dcm_DiagnosticSessionControlType mode);
Std_ReturnType TSC_Dcm_Rte_Switch_DcmEcuReset_DcmEcuReset(Dcm_EcuResetType mode);

/** Mode switch acknowledgement */
Std_ReturnType TSC_Dcm_Rte_SwitchAck_DcmEcuReset_DcmEcuReset(void);




