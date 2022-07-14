/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: VTTOs
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Os_Hal_Entry_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_HAL_ENTRY_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Hal_Entry_Cfg.h"
#include "Os_Hal_Entry_Lcfg.h"
#include "Os_Hal_Entry.h"

/* Os kernel module dependencies */
#include "Os_Isr_Lcfg.h"
#include "Os.h"

/* Os hal dependencies */
#include "Os_Hal_Cfg.h"
#include "Os_Hal_Core_Lcfg.h"
#include "Os_Hal_Interrupt_Cfg.h"
#include "Os_Hal_Trap.h"


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EXCEPTION VECTOR TABLE CORE 0
 *********************************************************************************************************************/

/* EMPTY FOR PLATFORM STUB exc*/


/**********************************************************************************************************************
 *  INTERRUPT VECTOR TABLE CORE 0
 *********************************************************************************************************************/


/*******************
 * Category 0 ISRs
 *******************/

/*******************
 * Category 1 ISRs
 *******************/

/* CANoe IRQ Handler: VttSysVarIsr_0 (Level: 2) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqHandler_VttSysVarIsr_0(void)
{
  Os_Hal_Debug_CanoeIrqHandler_Entry("VttSysVarIsr_0"); 
  VttSysVarIsr_0();
  CANoeEmuProcessor_EnableInterrupts();
}



/*******************
 * Category 2 ISRs
 *******************/

/* CANoe IRQ Task: CanIsrErr_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqTask_CanIsrErr_0(void)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) threadToResume;
  Os_IsrRun((Os_IsrConfigType *)&OsCfg_Isr_CanIsrErr_0);
  threadToResume = Os_CoreGetThread();
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(threadToResume->ContextConfig->CANoeEmuTaskId);
  CANoeEmuProcessor_RollbackStack();
  Os_Hal_CoreFreeze();
}

/* CANoe IRQ Handler: CanIsrErr_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqHandler_CanIsrErr_0(void)
{
  Os_Hal_Debug_CanoeIrqHandler_Entry("CanIsrErr_0"); 
  CANoeEmuProcessor_SetTaskToRun(CANoeTaskId_CanIsrErr_0);
}



/* CANoe IRQ Task: CanIsrRx_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqTask_CanIsrRx_0(void)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) threadToResume;
  Os_IsrRun((Os_IsrConfigType *)&OsCfg_Isr_CanIsrRx_0);
  threadToResume = Os_CoreGetThread();
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(threadToResume->ContextConfig->CANoeEmuTaskId);
  CANoeEmuProcessor_RollbackStack();
  Os_Hal_CoreFreeze();
}

/* CANoe IRQ Handler: CanIsrRx_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqHandler_CanIsrRx_0(void)
{
  Os_Hal_Debug_CanoeIrqHandler_Entry("CanIsrRx_0"); 
  CANoeEmuProcessor_SetTaskToRun(CANoeTaskId_CanIsrRx_0);
}



/* CANoe IRQ Task: CanIsrTx_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqTask_CanIsrTx_0(void)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) threadToResume;
  Os_IsrRun((Os_IsrConfigType *)&OsCfg_Isr_CanIsrTx_0);
  threadToResume = Os_CoreGetThread();
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(threadToResume->ContextConfig->CANoeEmuTaskId);
  CANoeEmuProcessor_RollbackStack();
  Os_Hal_CoreFreeze();
}

/* CANoe IRQ Handler: CanIsrTx_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqHandler_CanIsrTx_0(void)
{
  Os_Hal_Debug_CanoeIrqHandler_Entry("CanIsrTx_0"); 
  CANoeEmuProcessor_SetTaskToRun(CANoeTaskId_CanIsrTx_0);
}



/* CANoe IRQ Task: CanIsrWakeup_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqTask_CanIsrWakeup_0(void)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) threadToResume;
  Os_IsrRun((Os_IsrConfigType *)&OsCfg_Isr_CanIsrWakeup_0);
  threadToResume = Os_CoreGetThread();
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(threadToResume->ContextConfig->CANoeEmuTaskId);
  CANoeEmuProcessor_RollbackStack();
  Os_Hal_CoreFreeze();
}

/* CANoe IRQ Handler: CanIsrWakeup_0 (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqHandler_CanIsrWakeup_0(void)
{
  Os_Hal_Debug_CanoeIrqHandler_Entry("CanIsrWakeup_0"); 
  CANoeEmuProcessor_SetTaskToRun(CANoeTaskId_CanIsrWakeup_0);
}



/* CANoe IRQ Task: CounterIsr_SystemTimer (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqTask_CounterIsr_SystemTimer(void)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) threadToResume;
  Os_IsrRun((Os_IsrConfigType *)&OsCfg_Isr_CounterIsr_SystemTimer);
  threadToResume = Os_CoreGetThread();
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(threadToResume->ContextConfig->CANoeEmuTaskId);
  CANoeEmuProcessor_RollbackStack();
  Os_Hal_CoreFreeze();
}

/* CANoe IRQ Handler: CounterIsr_SystemTimer (Level: 1) */
FUNC(void, OS_CODE) Os_Hal_CanoeIrqHandler_CounterIsr_SystemTimer(void)
{
  Os_Hal_Debug_CanoeIrqHandler_Entry("CounterIsr_SystemTimer"); 
  CANoeEmuProcessor_SetTaskToRun(CANoeTaskId_CounterIsr_SystemTimer);
}






/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry_Lcfg.c
 *********************************************************************************************************************/
