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
 *              File: Os_Hal_Context_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_HAL_CONTEXT_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Hal_Context_Lcfg.h"
#include "Os_Hal_Context.h"

/* Os kernel module dependencies */
#include "OsInt.h"
#include "Os_Core.h"
#include "Os_Hook_Lcfg.h"
#include "Os_Hook.h"
#include "Os_Ioc_Lcfg.h"
#include "Os_Isr_Lcfg.h"
#include "Os_Lcfg.h"
#include "Os_Stack_Lcfg.h"
#include "Os_Task_Lcfg.h"
#include "Os_Task.h"

/* Os hal dependencies */
#include "Os_Hal_Core.h"


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

#define OS_START_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! HAL dynamic hook context data: Os_CoreInitHook_OsCore */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_Os_CoreInitHook_OsCore_Dyn;

/*! HAL dynamic hook context data: ErrorHook_OsCore */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_ErrorHook_OsCore_Dyn;

/*! HAL dynamic ISR2 level context data: Level1 */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn;

/*! HAL dynamic task context data: IO_Task */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_IO_Task_Dyn;

/*! HAL dynamic task context data: IdleTask_OsCore */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_IdleTask_OsCore_Dyn;

/*! HAL dynamic task context data: Init_Task */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_Init_Task_Dyn;

/*! HAL dynamic task context data: My_Task */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_My_Task_Dyn;

/*! HAL dynamic task context data: SchM_Task */
VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_SchM_Task_Dyn;

/*! HAL exception context data: OsCore */
VAR(Os_ExceptionContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_OsCore_ExceptionContext;

#define OS_STOP_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA
 *********************************************************************************************************************/

#define OS_START_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! HAL hook context configuration data: Os_CoreInitHook_OsCore */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_Os_CoreInitHook_OsCore =
{

  /* Type =              */ Hook,
  /* Name =              */ "Os_CoreInitHook_OsCore",
  /* CANoeEmuTaskId =    */ CANoeTaskId_Os_CoreInitHook_OsCore,
  /* CANoeTaskFunction = */ Os_Hal_CANoeTaskWrapper,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) Os_HookWrapperOs_CoreInitHook,
  /* ReturnAddress =     */ Os_TrapHookReturn,
  /* IntLockLevel =      */ 1,
  /* Stack =             */ &OsCfg_Stack_OsCore_Init
};

/*! HAL hook context configuration data: ErrorHook_OsCore */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_ErrorHook_OsCore =
{

  /* Type =              */ Hook,
  /* Name =              */ "ErrorHook_OsCore",
  /* CANoeEmuTaskId =    */ CANoeTaskId_ErrorHook_OsCore,
  /* CANoeTaskFunction = */ Os_Hal_CANoeTaskWrapper,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) Os_HookWrapperStatusHook,
  /* ReturnAddress =     */ Os_TrapHookReturn,
  /* IntLockLevel =      */ 1,
  /* Stack =             */ &OsCfg_Stack_OsCore_Error
};

/*! HAL ISR2 context configuration data: CanIsrErr_0 */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrErr_0 =
{
  /* Type =              */ Isr,
  /* Name =              */ "CanIsrErr_0",
  /* CANoeEmuTaskId =    */ CANoeTaskId_CanIsrErr_0,
  /* CANoeTaskFunction = */ &Os_Hal_CanoeIrqTask_CanIsrErr_0,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Isr_CanIsrErr_0,
  /* ReturnAddress =     */ &Os_IsrEpilogue,
  /* IntLockLevel =      */ 2,
  /* Stack =             */ &OsCfg_Stack_OsCore_Isr_Core
};

/*! HAL ISR2 context configuration data: CanIsrRx_0 */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrRx_0 =
{
  /* Type =              */ Isr,
  /* Name =              */ "CanIsrRx_0",
  /* CANoeEmuTaskId =    */ CANoeTaskId_CanIsrRx_0,
  /* CANoeTaskFunction = */ &Os_Hal_CanoeIrqTask_CanIsrRx_0,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Isr_CanIsrRx_0,
  /* ReturnAddress =     */ &Os_IsrEpilogue,
  /* IntLockLevel =      */ 2,
  /* Stack =             */ &OsCfg_Stack_OsCore_Isr_Core
};

/*! HAL ISR2 context configuration data: CanIsrTx_0 */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrTx_0 =
{
  /* Type =              */ Isr,
  /* Name =              */ "CanIsrTx_0",
  /* CANoeEmuTaskId =    */ CANoeTaskId_CanIsrTx_0,
  /* CANoeTaskFunction = */ &Os_Hal_CanoeIrqTask_CanIsrTx_0,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Isr_CanIsrTx_0,
  /* ReturnAddress =     */ &Os_IsrEpilogue,
  /* IntLockLevel =      */ 2,
  /* Stack =             */ &OsCfg_Stack_OsCore_Isr_Core
};

/*! HAL ISR2 context configuration data: CanIsrWakeup_0 */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrWakeup_0 =
{
  /* Type =              */ Isr,
  /* Name =              */ "CanIsrWakeup_0",
  /* CANoeEmuTaskId =    */ CANoeTaskId_CanIsrWakeup_0,
  /* CANoeTaskFunction = */ &Os_Hal_CanoeIrqTask_CanIsrWakeup_0,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Isr_CanIsrWakeup_0,
  /* ReturnAddress =     */ &Os_IsrEpilogue,
  /* IntLockLevel =      */ 2,
  /* Stack =             */ &OsCfg_Stack_OsCore_Isr_Core
};

/*! HAL ISR2 context configuration data: CounterIsr_SystemTimer */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CounterIsr_SystemTimer =
{
  /* Type =              */ Isr,
  /* Name =              */ "CounterIsr_SystemTimer",
  /* CANoeEmuTaskId =    */ CANoeTaskId_CounterIsr_SystemTimer,
  /* CANoeTaskFunction = */ &Os_Hal_CanoeIrqTask_CounterIsr_SystemTimer,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Isr_Os_TimerPitIsr,
  /* ReturnAddress =     */ &Os_IsrEpilogue,
  /* IntLockLevel =      */ 2,
  /* Stack =             */ &OsCfg_Stack_OsCore_Isr_Core
};

/*! HAL task context configuration data: IO_Task */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_IO_Task =
{
  /* Type =              */ Task,
  /* Name =              */ "IO_Task",
  /* CANoeEmuTaskId =    */ CANoeTaskId_IO_Task,
  /* CANoeTaskFunction = */ &Os_Task_IO_Task,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Task_IO_Task,
  /* ReturnAddress =     */ &Os_TaskMissingTerminateTask,
  /* IntLockLevel =      */ 0,
  /* Stack =             */ &OsCfg_Stack_IO_Task
};

/*! HAL task context configuration data: IdleTask_OsCore */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_IdleTask_OsCore =
{
  /* Type =              */ Task,
  /* Name =              */ "IdleTask_OsCore",
  /* CANoeEmuTaskId =    */ CANoeTaskId_IdleTask_OsCore,
  /* CANoeTaskFunction = */ &Os_Task_Os_IdleTask,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Task_Os_IdleTask,
  /* ReturnAddress =     */ &Os_TaskMissingTerminateTask,
  /* IntLockLevel =      */ 0,
  /* Stack =             */ &OsCfg_Stack_OsCore_Task_Prio4
};

/*! HAL task context configuration data: Init_Task */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_Init_Task =
{
  /* Type =              */ Task,
  /* Name =              */ "Init_Task",
  /* CANoeEmuTaskId =    */ CANoeTaskId_Init_Task,
  /* CANoeTaskFunction = */ &Os_Task_Init_Task,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Task_Init_Task,
  /* ReturnAddress =     */ &Os_TaskMissingTerminateTask,
  /* IntLockLevel =      */ 0,
  /* Stack =             */ &OsCfg_Stack_OsCore_Task_Prio1
};

/*! HAL task context configuration data: My_Task */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_My_Task =
{
  /* Type =              */ Task,
  /* Name =              */ "My_Task",
  /* CANoeEmuTaskId =    */ CANoeTaskId_My_Task,
  /* CANoeTaskFunction = */ &Os_Task_My_Task,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Task_My_Task,
  /* ReturnAddress =     */ &Os_TaskMissingTerminateTask,
  /* IntLockLevel =      */ 0,
  /* Stack =             */ &OsCfg_Stack_My_Task
};

/*! HAL task context configuration data: SchM_Task */
CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_SchM_Task =
{
  /* Type =              */ Task,
  /* Name =              */ "SchM_Task",
  /* CANoeEmuTaskId =    */ CANoeTaskId_SchM_Task,
  /* CANoeTaskFunction = */ &Os_Task_SchM_Task,
  /* Entry =             */ (Os_Hal_ContextEntryCBType) &Os_Task_SchM_Task,
  /* ReturnAddress =     */ &Os_TaskMissingTerminateTask,
  /* IntLockLevel =      */ 0,
  /* Stack =             */ &OsCfg_Stack_SchM_Task
};

/*! HAL kernel stack configuration data: OsCore_Kernel */
CONST(Os_Hal_ContextStackConfigType, OS_CONST) OsCfg_Hal_Stack_OsCore_Kernel =
{
  /* .Dummy = */ (uint32)NULL_PTR,  /* OsCore_Kernel */
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */



#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for HAL exception context. */
CONSTP2VAR(Os_ExceptionContextType, AUTOMATIC, OS_CONST)
  OsCfg_Hal_Context_ExceptionContextRef[OS_CFG_COREPHYSICALID_COUNT + 1] =
{
  &OsCfg_Hal_Context_OsCore_ExceptionContext,
  NULL_PTR
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context_Lcfg.c
 *********************************************************************************************************************/
