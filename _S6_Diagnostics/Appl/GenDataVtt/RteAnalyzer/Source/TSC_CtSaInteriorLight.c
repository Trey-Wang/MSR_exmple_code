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
 *          File:  TSC_CtSaInteriorLight.c
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Implementation of wrapper software component for Bte-based Rte test cases
 *********************************************************************************************************************/
#include "Rte_CtSaInteriorLight.h"
#include "TSC_CtSaInteriorLight.h"








Std_ReturnType TSC_CtSaInteriorLight_Rte_Read_PpLightState_DeLightState(uint8 self, IdtLightState *data)
{
  return Rte_Read_PpLightState_DeLightState(self, data);
}








     /* Client Server Interfaces: */
Std_ReturnType TSC_CtSaInteriorLight_Rte_Call_PpInteriorLightDefect_ReadChannel(uint8 self, IdtDioValueType *value)
{
  return Rte_Call_PpInteriorLightDefect_ReadChannel(self, value);
}
Std_ReturnType TSC_CtSaInteriorLight_Rte_Call_PpInteriorLightIoHwAb_WriteChannel(uint8 self, IdtDioValueType value)
{
  return Rte_Call_PpInteriorLightIoHwAb_WriteChannel(self, value);
}


     /* Service calls */
Std_ReturnType TSC_CtSaInteriorLight_Rte_Call_Event_DTC_0x000002_SetEventStatus(uint8 self, Dem_EventStatusType EventStatus)
{
  return Rte_Call_Event_DTC_0x000002_SetEventStatus((P2CONST(struct Rte_CDS_CtSaInteriorLight, AUTOMATIC, RTE_CONST))0, EventStatus);
}


     /* Mode Interfaces */




     /* Inter-Runnable variables */







     /* CtSaInteriorLight */
      /* CtSaInteriorLight */



