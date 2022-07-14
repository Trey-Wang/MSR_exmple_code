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
 *              File: Os_Core_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_CORE_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Core_Cfg.h"
#include "Os_Core_Lcfg.h"
#include "Os_Core.h"

/* Os kernel module dependencies */
#include "Os_Application_Lcfg.h"
#include "Os_Barrier_Lcfg.h"
#include "Os_Common.h"
#include "Os_Hook.h"
#include "Os_Ioc_Lcfg.h"
#include "Os_MemoryProtection_Lcfg.h"
#include "Os_Resource_Lcfg.h"
#include "Os_Scheduler_Lcfg.h"
#include "Os_Spinlock_Lcfg.h"
#include "Os_Stack_Lcfg.h"
#include "Os_Task_Lcfg.h"
#include "Os_TimingProtection_Lcfg.h"
#include "Os_Trace_Lcfg.h"
#include "Os.h"

/* Os hal dependencies */
#include "Os_Hal_Core_Cfg.h"
#include "Os_Hal_Core_Lcfg.h"
#include "Os_Hal_Kernel_Lcfg.h"


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

#define OS_START_SEC_CORESTATUS_CORE0_VAR_NOCACHE_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Dynamic core status: OsCore */
OS_LOCAL VAR(Os_CoreAsrStatusType, OS_VAR_NOINIT) OsCfg_Core_OsCore_Status_Dyn;

#define OS_STOP_SEC_CORESTATUS_CORE0_VAR_NOCACHE_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Dynamic core boot barrier: OsCore */
OS_LOCAL VAR(Os_CoreBootBarrierType, OS_VAR_NOINIT) OsCfg_Core_OsCore_BootBarrier_Dyn;

#define OS_STOP_SEC_VAR_NOCACHE_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define OS_START_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Dynamic core data: OsCore */
VAR(Os_CoreAsrType, OS_VAR_NOINIT) OsCfg_Core_OsCore_Dyn;

#define OS_STOP_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA
 *********************************************************************************************************************/

#define OS_START_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for core hooks: OsCore */
OS_LOCAL CONST(Os_HookConfigRefType, OS_CONST) OsCfg_Core_OsCore_HookRefs[OS_CFG_NUM_CORE_OSCORE_HOOKS + 1] =
{
  OS_HOOK_CASTCONFIG_STATUSHOOK_2_HOOK(OsCfg_Hook_Os_CoreInitHook_OsCore),
  OS_HOOK_CASTCONFIG_STATUSHOOK_2_HOOK(OsCfg_Hook_ErrorHook_OsCore),
  NULL_PTR
};

/*! Object reference table for core applications: OsCore */
OS_LOCAL CONST(Os_AppConfigRefType, OS_CONST) OsCfg_Core_OsCore_AppRefs[OS_CFG_NUM_CORE_OSCORE_APPS + 1] =
{
  NULL_PTR
};

/*! Object reference table for core IOC communications: OsCore */
OS_LOCAL CONST(Os_IocConfigRefType, OS_CONST) OsCfg_Core_OsCore_IocRefs[OS_CFG_NUM_CORE_OSCORE_IOCS + 1] =
{
  NULL_PTR
};

/*! Object reference table for core barriers: OsCore */
OS_LOCAL CONST(Os_BarrierBaseConfigRefType, OS_CONST) OsCfg_Core_OsCore_BarrierRefs[OS_CFG_NUM_BARRIERS + 1] =
{
  NULL_PTR
};

/*! Object reference table for core resources: OsCore */
OS_LOCAL CONST(Os_ResourceConfigRefType, OS_CONST) OsCfg_Core_OsCore_ResourceRefs[OS_CFG_NUM_CORE_OSCORE_RESOURCES + 1] =
{
  OS_RESOURCE_CASTCONFIG_RESOURCE_2_RESOURCE(OsCfg_Resource_OsResource),
  NULL_PTR
};

/*! Object reference table for core stacks: OsCore */
OS_LOCAL CONST(Os_StackConfigRefType, OS_CONST) OsCfg_Core_OsCore_StackRefs[OS_CFG_NUM_CORE_OSCORE_STACKS + 1] =
{
  (Os_StackConfigRefType) &OsCfg_Stack_IO_Task,
  (Os_StackConfigRefType) &OsCfg_Stack_My_Task,
  (Os_StackConfigRefType) &OsCfg_Stack_OsCore_Error,
  (Os_StackConfigRefType) &OsCfg_Stack_OsCore_Init,
  (Os_StackConfigRefType) &OsCfg_Stack_OsCore_Isr_Core,
  (Os_StackConfigRefType) &OsCfg_Stack_OsCore_Kernel,
  (Os_StackConfigRefType) &OsCfg_Stack_OsCore_Task_Prio1,
  (Os_StackConfigRefType) &OsCfg_Stack_OsCore_Task_Prio4,
  (Os_StackConfigRefType) &OsCfg_Stack_SchM_Task,
  NULL_PTR
};

/*! Core configuration data: OsCore */
CONST(Os_CoreAsrConfigType, OS_CONST) OsCfg_Core_OsCore =
{
  /* .Core              = */
  {
    /* .Status               = */ OS_CORE_CASTSTATUS_COREASRSTATUS_2_CORESTATUS(OsCfg_Core_OsCore_Status_Dyn),
    /* .HwConfig             = */ &OsCfg_Hal_Core_OsCore,
    /* .InterruptSourceRefs  = */ OsCfg_Hal_Core_OsCore_InterruptSourceRefs,
    /* .InterruptSourceCount = */ OS_CFG_NUM_CORE_OSCORE_INTERRUPTSOURCEREFS,
    /* .Id                   = */ OS_CORE_ID_0,
    /* .IsAsrCore            = */ TRUE,
    /* .IsAutostart          = */ TRUE,
    /* .IsMasterStartAllowed = */ FALSE,
    /* .HasPrivilegedHardwareAccess = */ TRUE
  },
  /* .Dyn               = */ &OsCfg_Core_OsCore_Dyn,
  /* .Scheduler         = */ &OsCfg_Scheduler_OsCore,
  /* .IdleTask          = */ &OsCfg_Task_IdleTask_OsCore,
  /* .KernelApp         = */ &OsCfg_App_SystemApplication_OsCore,
  /* .KernelStack       = */ &OsCfg_Stack_OsCore_Kernel,
  /* .PreStartTask      = */ NULL_PTR,
  /* .StartupHookRef    = */ NULL_PTR,
  /* .ShutdownHookRef   = */ NULL_PTR,
  /* .ErrorHookRef      = */ &OsCfg_Hook_ErrorHook_OsCore,
  /* .ProtectionHookRef = */ NULL_PTR,
  /* .InitHookRef       = */ &OsCfg_Hook_Os_CoreInitHook_OsCore,
  /* .HookRefs          = */ OsCfg_Core_OsCore_HookRefs,
  /* .HookCount         = */ OS_CFG_NUM_CORE_OSCORE_HOOKS,
  /* .AppRefs           = */ OsCfg_Core_OsCore_AppRefs,
  /* .AppCount          = */ OS_CFG_NUM_CORE_OSCORE_APPS,
  /* .ResourceRefs      = */ OsCfg_Core_OsCore_ResourceRefs,
  /* .ResourceCount     = */ OS_CFG_NUM_CORE_OSCORE_RESOURCES,
  /* .HwConfig          = */ &OsCfg_Hal_CoreAsr_OsCore,
  /* .MemoryProtection  = */ NULL_PTR,
  /* .OsMpAccessRights  = */ NULL_PTR,
  /* .TimingProtection  = */ NULL_PTR,
  /* .StackRefs         = */ OsCfg_Core_OsCore_StackRefs,
  /* .StackCount        = */ OS_CFG_NUM_CORE_OSCORE_STACKS,
  /* .CoreIdx           = */ OS_COREASRCOREIDX_0,
  /* .XSignal           = */ NULL_PTR,
  /* .IocRefs           = */ OsCfg_Core_OsCore_IocRefs,
  /* .IocCount          = */ OS_CFG_NUM_CORE_OSCORE_IOCS,
  /* .Trace             = */ NULL_PTR,
  /* .Barrier           = */ &OsCfg_Barrier_OsCore,
  /* .BarrierRefs       = */ OsCfg_Core_OsCore_BarrierRefs,
  /* .BarrierCount      = */ OS_CFG_NUM_BARRIERS
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for system spinlocks. */
OS_LOCAL CONST(Os_SpinlockConfigRefType, OS_CONST) OsCfg_SystemSpinlockRefs[OS_CFG_NUM_SYSTEM_SPINLOCKS + 1] =
{
  NULL_PTR
};

/*! Object reference table for core boot barriers. */
CONSTP2VAR(Os_CoreBootBarrierType, AUTOMATIC, OS_CONST) OsCfg_CoreBootBarrierRefs[OS_COREID_COUNT + 1] =
{
  &OsCfg_Core_OsCore_BootBarrier_Dyn,
  NULL_PTR
};

/*! System configuration data. */
CONST(Os_SystemConfigType, OS_CONST) OsCfg_System =
{
  /* .VersionInfo      = */
  {
    /* .vendorID         = */ OS_VENDOR_ID,
    /* .moduleID         = */ OS_MODULE_ID,
    /* .sw_major_version = */ OS_SW_MAJOR_VERSION,
    /* .sw_minor_version = */ OS_SW_MINOR_VERSION,
    /* .sw_patch_version = */ OS_SW_PATCH_VERSION
  },
  /* .NumberOfAsrCores = */ OS_COREASRCOREIDX_COUNT,
  /* .SpinlockRefs     = */ OsCfg_SystemSpinlockRefs,
  /* .SpinlockCount    = */ OS_CFG_NUM_SYSTEM_SPINLOCKS,
  /* .HwConfig         = */ &OsCfg_Hal_System,
  /* .MemoryProtection = */ NULL_PTR,
  /* .InterruptSourceRefs  = */ OsCfg_Hal_System_InterruptSourceRefs,
  /* .InterruptSourceCount = */ OS_CFG_NUM_SYSTEM_INTERRUPTSOURCEREFS,
  /* .Core2Thread      = */ &OsCfg_Hal_Core2Thread
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
 *  END OF FILE: Os_Core_Lcfg.c
 *********************************************************************************************************************/
