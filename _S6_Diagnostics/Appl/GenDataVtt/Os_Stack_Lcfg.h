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
 *              File: Os_Stack_Lcfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_STACK_LCFG_H)                                                       /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_STACK_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Stack_Cfg.h"
# include "Os_Stack_Types.h"

/* Os kernel module dependencies */
# include "Os_Lcfg.h"

/* Os hal dependencies */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_STACK_IO_TASK_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Task stack: IO_Task */
/* User: [IO_Task] */
extern OS_STACK_DECLARE(OsCfg_Stack_IO_Task_Dyn, OS_CFG_SIZE_IO_TASK_STACK);

# define OS_STOP_SEC_STACK_IO_TASK_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_MY_TASK_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Task stack: My_Task */
/* User: [My_Task] */
extern OS_STACK_DECLARE(OsCfg_Stack_My_Task_Dyn, OS_CFG_SIZE_MY_TASK_STACK);

# define OS_STOP_SEC_STACK_MY_TASK_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_OSCORE_ERROR_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Core error stack: OsCore_Error */
/* User: [ErrorHook_OsCore, OsCore] */
extern OS_STACK_DECLARE(OsCfg_Stack_OsCore_Error_Dyn, OS_CFG_SIZE_OSCORE_ERROR_STACK);

# define OS_STOP_SEC_STACK_OSCORE_ERROR_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_OSCORE_INIT_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Core init stack: OsCore_Init */
/* User: [OsCore, Os_CoreInitHook_OsCore] */
extern OS_STACK_DECLARE(OsCfg_Stack_OsCore_Init_Dyn, OS_CFG_SIZE_OSCORE_INIT_STACK);

# define OS_STOP_SEC_STACK_OSCORE_INIT_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_OSCORE_ISR_CORE_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ISR core global stack: OsCore_Isr_Core */
/* User: [CanIsrWakeup_0, CanIsrErr_0, CanIsrRx_0, CanIsrTx_0, CounterIsr_SystemTimer] */
extern OS_STACK_DECLARE(OsCfg_Stack_OsCore_Isr_Core_Dyn, OS_CFG_SIZE_OSCORE_ISR_CORE_STACK);

# define OS_STOP_SEC_STACK_OSCORE_ISR_CORE_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_OSCORE_KERNEL_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Core kernel stack: OsCore_Kernel */
/* User: [OsCore] */
extern OS_STACK_DECLARE(OsCfg_Stack_OsCore_Kernel_Dyn, OS_CFG_SIZE_OSCORE_KERNEL_STACK);

# define OS_STOP_SEC_STACK_OSCORE_KERNEL_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_OSCORE_TASK_PRIO1_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Shared Task stack: OsCore_Task_Prio1 */
/* User: [Init_Task] */
extern OS_STACK_DECLARE(OsCfg_Stack_OsCore_Task_Prio1_Dyn, OS_CFG_SIZE_OSCORE_TASK_PRIO1_STACK);

# define OS_STOP_SEC_STACK_OSCORE_TASK_PRIO1_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_OSCORE_TASK_PRIO4_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Shared Task stack: OsCore_Task_Prio4 */
/* User: [IdleTask_OsCore] */
extern OS_STACK_DECLARE(OsCfg_Stack_OsCore_Task_Prio4_Dyn, OS_CFG_SIZE_OSCORE_TASK_PRIO4_STACK);

# define OS_STOP_SEC_STACK_OSCORE_TASK_PRIO4_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define OS_START_SEC_STACK_SCHM_TASK_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Task stack: SchM_Task */
/* User: [SchM_Task] */
extern OS_STACK_DECLARE(OsCfg_Stack_SchM_Task_Dyn, OS_CFG_SIZE_SCHM_TASK_STACK);

# define OS_STOP_SEC_STACK_SCHM_TASK_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_Stacks.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_CORE0_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Stack configuration data: IO_Task */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_IO_Task;

/*! Stack configuration data: My_Task */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_My_Task;

/*! Stack configuration data: OsCore_Error */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_OsCore_Error;

/*! Stack configuration data: OsCore_Init */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_OsCore_Init;

/*! Stack configuration data: OsCore_Isr_Core */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_OsCore_Isr_Core;

/*! Stack configuration data: OsCore_Kernel */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_OsCore_Kernel;

/*! Stack configuration data: OsCore_Task_Prio1 */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_OsCore_Task_Prio1;

/*! Stack configuration data: OsCore_Task_Prio4 */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_OsCore_Task_Prio4;

/*! Stack configuration data: SchM_Task */
extern CONST(Os_StackConfigType, OS_CONST) OsCfg_Stack_SchM_Task;

# define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


#endif /* OS_STACK_LCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Stack_Lcfg.h
 *********************************************************************************************************************/
