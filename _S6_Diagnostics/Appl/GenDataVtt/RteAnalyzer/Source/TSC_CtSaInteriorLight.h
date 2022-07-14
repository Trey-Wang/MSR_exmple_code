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
 *          File:  TSC_CtSaInteriorLight.h
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
Std_ReturnType TSC_CtSaInteriorLight_Rte_Read_PpLightState_DeLightState(uint8 self, IdtLightState *data);

/** Client server interfaces */
Std_ReturnType TSC_CtSaInteriorLight_Rte_Call_PpInteriorLightDefect_ReadChannel(uint8 self, IdtDioValueType *value);
Std_ReturnType TSC_CtSaInteriorLight_Rte_Call_PpInteriorLightIoHwAb_WriteChannel(uint8 self, IdtDioValueType value);

/** Service interfaces */
Std_ReturnType TSC_CtSaInteriorLight_Rte_Call_Event_DTC_0x000002_SetEventStatus(uint8 self, Dem_EventStatusType EventStatus);




