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
 *          File:  TSC_CtApMySwc.c
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  Implementation of wrapper software component for Bte-based Rte test cases
 *********************************************************************************************************************/
#include "Rte_CtApMySwc.h"
#include "TSC_CtApMySwc.h"















     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */













     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */













     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */













     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */













     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */









Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(boolean data)
{
  return Rte_Write_PpDisplayState_DeOdometerWriteRequestPending(data);
}





     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */






Std_ReturnType TSC_CtApMySwc_Rte_Read_PpDoorStateFrontRight_DeDoorState(IdtDoorState *data)
{
  return Rte_Read_PpDoorStateFrontRight_DeDoorState(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Read_PpDoorStateRearRight_DeDoorState(IdtDoorState *data)
{
  return Rte_Read_PpDoorStateRearRight_DeDoorState(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Read_PpLocalIgnitionState_DeIgnitionState(IdtDisplayIgnitionState *data)
{
  return Rte_Read_PpLocalIgnitionState_DeIgnitionState(data);
}

IdtDoorState TSC_CtApMySwc_Rte_IRead_RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState(void)
{
  return Rte_IRead_RCtApMySwcCode_PpDoorStateFrontLeft_DeDoorState();
}
IdtDoorState TSC_CtApMySwc_Rte_IRead_RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState(void)
{
  return Rte_IRead_RCtApMySwcCode_PpDoorStateRearLeft_DeDoorState();
}



Std_ReturnType TSC_CtApMySwc_Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data)
{
  return Rte_Write_PpBswMRunRequestCtApMySwc_requestedMode(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeDisplayBusState(boolean data)
{
  return Rte_Write_PpDisplayState_DeDisplayBusState(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeLightOnOffCounter(uint32 data)
{
  return Rte_Write_PpDisplayState_DeLightOnOffCounter(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Write_PpDisplayState_DeOdometerValue(uint32 data)
{
  return Rte_Write_PpDisplayState_DeOdometerValue(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Write_PpLightStateFront_DeLightState(IdtLightState data)
{
  return Rte_Write_PpLightStateFront_DeLightState(data);
}

Std_ReturnType TSC_CtApMySwc_Rte_Write_PpLightStateRear_DeLightState(IdtLightState data)
{
  return Rte_Write_PpLightStateRear_DeLightState(data);
}





     /* Client Server Interfaces: */


     /* Service calls */
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(ComM_ModeType ComMode)
{
  return Rte_Call_PpComMUserRequestCtApMySwc_RequestComMode(ComMode);
}
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpNvMLightOnOffCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr)
{
  return Rte_Call_PpNvMLightOnOffCtApMySwc_WriteBlock(SrcPtr);
}
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpNvMOdometerCtApMySwc_GetErrorStatus(NvM_RequestResultType *ErrorStatus)
{
  return Rte_Call_PpNvMOdometerCtApMySwc_GetErrorStatus(ErrorStatus);
}
Std_ReturnType TSC_CtApMySwc_Rte_Call_PpNvMOdometerCtApMySwc_WriteBlock(dtRef_const_VOID SrcPtr)
{
  return Rte_Call_PpNvMOdometerCtApMySwc_WriteBlock(SrcPtr);
}


     /* Mode Interfaces */




     /* Inter-Runnable variables */
uint16 TSC_CtApMySwc_Rte_IrvRead_RCtApMySwcCode_IrvDoorStateOld(void)
{
return Rte_IrvRead_RCtApMySwcCode_IrvDoorStateOld();
}

void TSC_CtApMySwc_Rte_IrvWrite_RCtApMySwcCode_IrvDoorStateOld(uint16 data)
{
  Rte_IrvWrite_RCtApMySwcCode_IrvDoorStateOld( data);
}












     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */













     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */









Std_ReturnType TSC_CtApMySwc_Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(BswM_ESH_RunRequest data)
{
  return Rte_Write_PpBswMPostRunRequestCtApMySwc_requestedMode(data);
}





     /* Client Server Interfaces: */


     /* Service calls */
Std_ReturnType TSC_CtApMySwc_Rte_Call_OpCycle_IgnitionCycle_SetOperationCycleState(Dem_OperationCycleStateType CycleState)
{
  return Rte_Call_OpCycle_IgnitionCycle_SetOperationCycleState(CycleState);
}


     /* Mode Interfaces */




     /* Inter-Runnable variables */






Std_ReturnType TSC_CtApMySwc_Rte_Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode(BswM_MdgShutdownTimerState *data)
{
  return Rte_Read_PpMrpPShutdownTimerDisplayCtApMySwc_requestedMode(data);
}








     /* Client Server Interfaces: */


     /* Service calls */


     /* Mode Interfaces */




     /* Inter-Runnable variables */






uint32  TSC_CtApMySwc_Rte_CData_CalOdometer(void)
{
  return (uint32 ) Rte_CData_CalOdometer();
}

     /* CtApMySwc */
      /* CtApMySwc */
void TSC_CtApMySwc_Rte_Enter_ExArLightOnOffCounter(void)
{
  Rte_Enter_ExArLightOnOffCounter();
}
void TSC_CtApMySwc_Rte_Exit_ExArLightOnOffCounter(void)
{
  Rte_Exit_ExArLightOnOffCounter();
}
void TSC_CtApMySwc_Rte_Enter_ExArOdometer(void)
{
  Rte_Enter_ExArOdometer();
}
void TSC_CtApMySwc_Rte_Exit_ExArOdometer(void)
{
  Rte_Exit_ExArOdometer();
}

/** Per Instance Memories */
uint32 *TSC_CtApMySwc_Rte_Pim_PimLightOnOffCounter(void)
{
  return Rte_Pim_PimLightOnOffCounter();
}
uint32 *TSC_CtApMySwc_Rte_Pim_PimOdometer(void)
{
  return Rte_Pim_PimOdometer();
}



