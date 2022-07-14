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
 *          File:  TSC_CtSaDisplay.c
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Implementation of wrapper software component for Bte-based Rte test cases
 *********************************************************************************************************************/
#include "Rte_CtSaDisplay.h"
#include "TSC_CtSaDisplay.h"








Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeDisplayBusState(boolean *data)
{
  return Rte_Read_PpDisplayState_DeDisplayBusState(data);
}








     /* Client Server Interfaces: */
Std_ReturnType TSC_CtSaDisplay_Rte_Call_PpDisplayStateIoHwAb_WriteChannel(IdtDioValueType value)
{
  return Rte_Call_PpDisplayStateIoHwAb_WriteChannel(value);
}
Std_ReturnType TSC_CtSaDisplay_Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(IdtDioValueType *value)
{
  return Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(value);
}


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */






Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeLightOnOffCounter(uint32 *data)
{
  return Rte_Read_PpDisplayState_DeLightOnOffCounter(data);
}








     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */






Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeOdometerValue(uint32 *data)
{
  return Rte_Read_PpDisplayState_DeOdometerValue(data);
}








     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */






Std_ReturnType TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeOdometerWriteRequestPending(boolean *data)
{
  return Rte_Read_PpDisplayState_DeOdometerWriteRequestPending(data);
}








     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */









Std_ReturnType TSC_CtSaDisplay_Rte_Write_PpLocalIgnitionState_DeIgnitionState(IdtDisplayIgnitionState data)
{
  return Rte_Write_PpLocalIgnitionState_DeIgnitionState(data);
}





     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */







     /* CtSaDisplay */
      /* CtSaDisplay */



