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
 *              File: Os_Isr_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_ISR_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Isr_Lcfg.h"
#include "Os_Isr.h"

/* Os kernel module dependencies */
#include "Os_AccessCheck_Lcfg.h"
#include "Os_Application_Lcfg.h"
#include "Os_Common.h"
#include "Os_Core_Lcfg.h"
#include "Os_Counter_Lcfg.h"
#include "Os_MemoryProtection_Lcfg.h"
#include "Os_Stack_Lcfg.h"
#include "Os_Thread.h"
#include "Os_Timer.h"
#include "Os_TimingProtection_Lcfg.h"
#include "Os_Trace_Lcfg.h"
#include "Os_XSignal_Lcfg.h"
#include "Os_XSignal.h"

/* Os hal dependencies */
#include "Os_Hal_Context_Lcfg.h"
#include "Os_Hal_Interrupt_Lcfg.h"


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

#define OS_START_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Dynamic ISR data: CanIsrErr_0 */
OS_LOCAL VAR(Os_IsrType, OS_VAR_NOINIT) OsCfg_Isr_CanIsrErr_0_Dyn;

/*! Dynamic ISR data: CanIsrRx_0 */
OS_LOCAL VAR(Os_IsrType, OS_VAR_NOINIT) OsCfg_Isr_CanIsrRx_0_Dyn;

/*! Dynamic ISR data: CanIsrTx_0 */
OS_LOCAL VAR(Os_IsrType, OS_VAR_NOINIT) OsCfg_Isr_CanIsrTx_0_Dyn;

/*! Dynamic ISR data: CanIsrWakeup_0 */
OS_LOCAL VAR(Os_IsrType, OS_VAR_NOINIT) OsCfg_Isr_CanIsrWakeup_0_Dyn;

/*! Dynamic ISR data: CounterIsr_SystemTimer */
OS_LOCAL VAR(Os_IsrType, OS_VAR_NOINIT) OsCfg_Isr_CounterIsr_SystemTimer_Dyn;

#define OS_STOP_SEC_CORE0_VAR_NOINIT_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA
 *********************************************************************************************************************/

#define OS_START_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! ISR configuration data: CanIsrErr_0 */
CONST(Os_IsrConfigType, OS_CONST) OsCfg_Isr_CanIsrErr_0 =
{
  /* .Thread   = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_CanIsrErr_0,
    /* .Context               = */ &OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Isr_Core,
    /* .Dyn                   = */ OS_ISR_CASTDYN_ISR_2_THREAD(OsCfg_Isr_CanIsrErr_0_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_ISR2,
    /* .PreThreadHook         = */ NULL_PTR,
    /* .InitDuringStartUp     = */ FALSE
  },
  /* .HwConfig = */ &OsCfg_Hal_IntIsr_CanIsrErr_0,
  /* .IsrId    = */ CanIsrErr_0
};
/*! ISR configuration data: CanIsrRx_0 */
CONST(Os_IsrConfigType, OS_CONST) OsCfg_Isr_CanIsrRx_0 =
{
  /* .Thread   = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_CanIsrRx_0,
    /* .Context               = */ &OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Isr_Core,
    /* .Dyn                   = */ OS_ISR_CASTDYN_ISR_2_THREAD(OsCfg_Isr_CanIsrRx_0_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_ISR2,
    /* .PreThreadHook         = */ NULL_PTR,
    /* .InitDuringStartUp     = */ FALSE
  },
  /* .HwConfig = */ &OsCfg_Hal_IntIsr_CanIsrRx_0,
  /* .IsrId    = */ CanIsrRx_0
};
/*! ISR configuration data: CanIsrTx_0 */
CONST(Os_IsrConfigType, OS_CONST) OsCfg_Isr_CanIsrTx_0 =
{
  /* .Thread   = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_CanIsrTx_0,
    /* .Context               = */ &OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Isr_Core,
    /* .Dyn                   = */ OS_ISR_CASTDYN_ISR_2_THREAD(OsCfg_Isr_CanIsrTx_0_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_ISR2,
    /* .PreThreadHook         = */ NULL_PTR,
    /* .InitDuringStartUp     = */ FALSE
  },
  /* .HwConfig = */ &OsCfg_Hal_IntIsr_CanIsrTx_0,
  /* .IsrId    = */ CanIsrTx_0
};
/*! ISR configuration data: CanIsrWakeup_0 */
CONST(Os_IsrConfigType, OS_CONST) OsCfg_Isr_CanIsrWakeup_0 =
{
  /* .Thread   = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_CanIsrWakeup_0,
    /* .Context               = */ &OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Isr_Core,
    /* .Dyn                   = */ OS_ISR_CASTDYN_ISR_2_THREAD(OsCfg_Isr_CanIsrWakeup_0_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_ISR2,
    /* .PreThreadHook         = */ NULL_PTR,
    /* .InitDuringStartUp     = */ FALSE
  },
  /* .HwConfig = */ &OsCfg_Hal_IntIsr_CanIsrWakeup_0,
  /* .IsrId    = */ CanIsrWakeup_0
};
/*! ISR configuration data: CounterIsr_SystemTimer */
CONST(Os_TimerIsrConfigType, OS_CONST) OsCfg_Isr_CounterIsr_SystemTimer =
{
  /* .Isr     = */
  {
  /* .Thread   = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_CounterIsr_SystemTimer,
    /* .Context               = */ &OsCfg_Hal_Context_OsCore_Isr_Level1_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Isr_Core,
    /* .Dyn                   = */ OS_ISR_CASTDYN_ISR_2_THREAD(OsCfg_Isr_CounterIsr_SystemTimer_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_ISR2,
    /* .PreThreadHook         = */ NULL_PTR,
    /* .InitDuringStartUp     = */ FALSE
  },
  /* .HwConfig = */ &OsCfg_Hal_IntIsr_CounterIsr_SystemTimer,
  /* .IsrId    = */ CounterIsr_SystemTimer
},
  /* .Counter = */ OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer)
};
/*! ISR configuration data: VttSysVarIsr_0 */
CONST(Os_IsrCat1ConfigType, OS_CONST) OsCfg_Cat1Isr_VttSysVarIsr_0 =
{
  /* .HwConfig = */ &OsCfg_Hal_IntIsr_VttSysVarIsr_0,
};
#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for category 2 ISRs. */
CONSTP2CONST(Os_IsrConfigType, OS_CONST, OS_CONST) OsCfg_IsrRefs[OS_ISRID_COUNT + 1] =
{
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrErr_0),
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrRx_0),
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrTx_0),
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrWakeup_0),
  OS_TIMER_CASTCONFIG_TIMEREISR_2_ISR(OsCfg_Isr_CounterIsr_SystemTimer),
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
 *  END OF FILE: Os_Isr_Lcfg.c
 *********************************************************************************************************************/
