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
 *          File:  TSC_CtSaDisplay.h
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
Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeDisplayBusState(boolean *data);
Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeLightOnOffCounter(uint32 *data);
Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeOdometerValue(uint32 *data);
Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeOdometerWriteRequestPending(boolean *data);

/** Sender receiver - explicit write services */
Std_ReturnType TSC_CtSaDisplay_Rte_Write_PpLocalIgnitionState_DeIgnitionState(IdtDisplayIgnitionState data);

/** Client server interfaces */
Std_ReturnType TSC_CtSaDisplay_Rte_Call_PpDisplayStateIoHwAb_WriteChannel(IdtDioValueType value);
Std_ReturnType TSC_CtSaDisplay_Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(IdtDioValueType *value);
Std_ReturnType TSC_CtSaDisplay_Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(IdtDioValueType *value);




