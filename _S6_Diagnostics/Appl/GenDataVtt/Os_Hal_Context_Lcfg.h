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
 *              File: Os_Hal_Context_Lcfg.h
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#if !defined (OS_HAL_CONTEXT_LCFG_H)                                                 /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_HAL_CONTEXT_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_Context_Types.h"

/* Os kernel module dependencies */
# include "Os_Core_Cfg.h"

/* Os hal dependencies */
# include "Os_Hal_Os_Types.h"


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

# define OS_START_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! HAL dynamic hook context data: Os_CoreInitHook_OsCore */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_Os_CoreInitHook_OsCore_Dyn;

/*! HAL dynamic hook context data: ErrorHook_OsCore */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_ErrorHook_OsCore_Dyn;

/*! HAL dynamic ISR2 level context data: Level1 */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn;

/*! HAL dynamic task context data: IO_Task */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_IO_Task_Dyn;

/*! HAL dynamic task context data: IdleTask_OsCore */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_IdleTask_OsCore_Dyn;

/*! HAL dynamic task context data: Init_Task */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_Init_Task_Dyn;

/*! HAL dynamic task context data: My_Task */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_My_Task_Dyn;

/*! HAL dynamic task context data: SchM_Task */
extern VAR(Os_Hal_ContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_SchM_Task_Dyn;

/*! HAL exception context data: OsCore */
extern VAR(Os_ExceptionContextType, OS_VAR_NOINIT) OsCfg_Hal_Context_OsCore_ExceptionContext;

# define OS_STOP_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_CORE0_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! HAL hook context configuration data: Os_CoreInitHook_OsCore */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_Os_CoreInitHook_OsCore;

/*! HAL hook context configuration data: ErrorHook_OsCore */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_ErrorHook_OsCore;

/*! HAL ISR2 context configuration data: CanIsrErr_0 */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrErr_0;

/*! HAL ISR2 context configuration data: CanIsrRx_0 */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrRx_0;

/*! HAL ISR2 context configuration data: CanIsrTx_0 */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrTx_0;

/*! HAL ISR2 context configuration data: CanIsrWakeup_0 */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CanIsrWakeup_0;

/*! HAL ISR2 context configuration data: CounterIsr_SystemTimer */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_CounterIsr_SystemTimer;

/*! HAL task context configuration data: IO_Task */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_IO_Task;

/*! HAL task context configuration data: IdleTask_OsCore */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_IdleTask_OsCore;

/*! HAL task context configuration data: Init_Task */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_Init_Task;

/*! HAL task context configuration data: My_Task */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_My_Task;

/*! HAL task context configuration data: SchM_Task */
extern CONST(Os_Hal_ContextConfigType, OS_CONST) OsCfg_Hal_Context_SchM_Task;

/*! HAL kernel stack configuration data: OsCore_Kernel */
extern CONST(Os_Hal_ContextStackConfigType, OS_CONST) OsCfg_Hal_Stack_OsCore_Kernel;

# define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */



# define OS_START_SEC_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for HAL exception context. */
extern CONSTP2VAR(Os_ExceptionContextType, AUTOMATIC, OS_CONST)
  OsCfg_Hal_Context_ExceptionContextRef[OS_CFG_COREPHYSICALID_COUNT + 1];

# define OS_STOP_SEC_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


#endif /* OS_HAL_CONTEXT_LCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context_Lcfg.h
 *********************************************************************************************************************/
