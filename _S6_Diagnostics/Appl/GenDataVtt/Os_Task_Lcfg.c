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
 *              File: Os_Task_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:32
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_TASK_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Task_Lcfg.h"
#include "Os_Task.h"

/* Os kernel module dependencies */
#include "Os_AccessCheck_Lcfg.h"
#include "Os_Application_Lcfg.h"
#include "Os_Common.h"
#include "Os_Core_Lcfg.h"
#include "Os_Core.h"
#include "Os_Error.h"
#include "Os_Ioc.h"
#include "Os_Lcfg.h"
#include "Os_MemoryProtection_Lcfg.h"
#include "Os_Stack_Lcfg.h"
#include "Os_Thread.h"
#include "Os_TimingProtection_Lcfg.h"
#include "Os_Trace_Lcfg.h"

/* Os hal dependencies */
#include "Os_Hal_Context_Lcfg.h"


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

/*! Dynamic task data: IO_Task */
OS_LOCAL VAR(Os_TaskType, OS_VAR_NOINIT) OsCfg_Task_IO_Task_Dyn;

/*! Dynamic task data: IdleTask_OsCore */
OS_LOCAL VAR(Os_TaskType, OS_VAR_NOINIT) OsCfg_Task_IdleTask_OsCore_Dyn;

/*! Dynamic task data: Init_Task */
OS_LOCAL VAR(Os_TaskType, OS_VAR_NOINIT) OsCfg_Task_Init_Task_Dyn;

/*! Dynamic task data: My_Task */
OS_LOCAL VAR(Os_TaskType, OS_VAR_NOINIT) OsCfg_Task_My_Task_Dyn;

/*! Dynamic task data: SchM_Task */
OS_LOCAL VAR(Os_TaskType, OS_VAR_NOINIT) OsCfg_Task_SchM_Task_Dyn;

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

/*! Task configuration data: IO_Task */
CONST(Os_TaskConfigType, OS_CONST) OsCfg_Task_IO_Task =
{
  /* .Thread                = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_IO_Task,
    /* .Context               = */ &OsCfg_Hal_Context_IO_Task_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_IO_Task,
    /* .Dyn                   = */ OS_TASK_CASTDYN_TASK_2_THREAD(OsCfg_Task_IO_Task_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_TASK,
    /* .PreThreadHook         = */ &Os_TaskCallPreTaskHook,
    /* .InitDuringStartUp     = */ TRUE
  },
  /* .HomePriority          = */ 3UL,
  /* .TaskId                = */ IO_Task,
  /* .RunningPriority       = */ 0UL,
  /* .MaxActivations        = */ 1UL,
  /* .AutostartModes        = */ OS_APPMODE_NONE,
  /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
  /* .IsExtended            = */ TRUE,
  /* .StackSharing          = */ OS_TASKSCHEDULE_ALLOWED
};

/*! Task configuration data: IdleTask_OsCore */
CONST(Os_TaskConfigType, OS_CONST) OsCfg_Task_IdleTask_OsCore =
{
  /* .Thread                = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_IdleTask_OsCore,
    /* .Context               = */ &OsCfg_Hal_Context_IdleTask_OsCore_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Task_Prio4,
    /* .Dyn                   = */ OS_TASK_CASTDYN_TASK_2_THREAD(OsCfg_Task_IdleTask_OsCore_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_TASK,
    /* .PreThreadHook         = */ &Os_TaskCallPreTaskHook,
    /* .InitDuringStartUp     = */ TRUE
  },
  /* .HomePriority          = */ 4UL,
  /* .TaskId                = */ IdleTask_OsCore,
  /* .RunningPriority       = */ 4UL,
  /* .MaxActivations        = */ 1UL,
  /* .AutostartModes        = */ (OSDEFAULTAPPMODE | OS_APPMODE_ANY),
  /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
  /* .IsExtended            = */ FALSE,
  /* .StackSharing          = */ OS_TASKSCHEDULE_ALLOWED
};

/*! Task configuration data: Init_Task */
CONST(Os_TaskConfigType, OS_CONST) OsCfg_Task_Init_Task =
{
  /* .Thread                = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_Init_Task,
    /* .Context               = */ &OsCfg_Hal_Context_Init_Task_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_OsCore_Task_Prio1,
    /* .Dyn                   = */ OS_TASK_CASTDYN_TASK_2_THREAD(OsCfg_Task_Init_Task_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_TASK,
    /* .PreThreadHook         = */ &Os_TaskCallPreTaskHook,
    /* .InitDuringStartUp     = */ TRUE
  },
  /* .HomePriority          = */ 1UL,
  /* .TaskId                = */ Init_Task,
  /* .RunningPriority       = */ 0UL,
  /* .MaxActivations        = */ 1UL,
  /* .AutostartModes        = */ OSDEFAULTAPPMODE,
  /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
  /* .IsExtended            = */ FALSE,
  /* .StackSharing          = */ OS_TASKSCHEDULE_ALLOWED
};

/*! Task configuration data: My_Task */
CONST(Os_TaskConfigType, OS_CONST) OsCfg_Task_My_Task =
{
  /* .Thread                = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_My_Task,
    /* .Context               = */ &OsCfg_Hal_Context_My_Task_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_My_Task,
    /* .Dyn                   = */ OS_TASK_CASTDYN_TASK_2_THREAD(OsCfg_Task_My_Task_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_TASK,
    /* .PreThreadHook         = */ &Os_TaskCallPreTaskHook,
    /* .InitDuringStartUp     = */ TRUE
  },
  /* .HomePriority          = */ 2UL,
  /* .TaskId                = */ My_Task,
  /* .RunningPriority       = */ 0UL,
  /* .MaxActivations        = */ 1UL,
  /* .AutostartModes        = */ OS_APPMODE_NONE,
  /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
  /* .IsExtended            = */ TRUE,
  /* .StackSharing          = */ OS_TASKSCHEDULE_ALLOWED
};

/*! Task configuration data: SchM_Task */
CONST(Os_TaskConfigType, OS_CONST) OsCfg_Task_SchM_Task =
{
  /* .Thread                = */
  {
    /* .ContextConfig         = */ &OsCfg_Hal_Context_SchM_Task,
    /* .Context               = */ &OsCfg_Hal_Context_SchM_Task_Dyn,
    /* .Stack                 = */ &OsCfg_Stack_SchM_Task,
    /* .Dyn                   = */ OS_TASK_CASTDYN_TASK_2_THREAD(OsCfg_Task_SchM_Task_Dyn),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .IntApiState           = */ &OsCfg_Core_OsCore_Dyn.IntApiState,
    /* .TimeProtConfig        = */ NULL_PTR,
    /* .MpAccessRightsInitial = */ NULL_PTR,
    /* .AccessRights          = */ &OsCfg_AccessCheck_NoAccess,
    /* .Trace                 = */ NULL_PTR,
    /* .InitialCallContext    = */ OS_CALLCONTEXT_TASK,
    /* .PreThreadHook         = */ &Os_TaskCallPreTaskHook,
    /* .InitDuringStartUp     = */ TRUE
  },
  /* .HomePriority          = */ 0UL,
  /* .TaskId                = */ SchM_Task,
  /* .RunningPriority       = */ 0UL,
  /* .MaxActivations        = */ 1UL,
  /* .AutostartModes        = */ OS_APPMODE_NONE,
  /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
  /* .IsExtended            = */ TRUE,
  /* .StackSharing          = */ OS_TASKSCHEDULE_ALLOWED
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for tasks. */
CONSTP2CONST(Os_TaskConfigType, OS_CONST, OS_CONST) OsCfg_TaskRefs[OS_TASKID_COUNT + 1] =
{
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_IO_Task),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_IdleTask_OsCore),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_Init_Task),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_My_Task),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_SchM_Task),
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
 *  END OF FILE: Os_Task_Lcfg.c
 *********************************************************************************************************************/
