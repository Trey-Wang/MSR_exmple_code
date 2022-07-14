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
 *              File: Os_Application_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_APPLICATION_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Application_Cfg.h"
#include "Os_Application_Lcfg.h"
#include "Os_Application.h"

/* Os kernel module dependencies */
#include "Os_Alarm_Lcfg.h"
#include "Os_Alarm.h"
#include "Os_Common.h"
#include "Os_Core_Lcfg.h"
#include "Os_Counter_Lcfg.h"
#include "Os_Counter.h"
#include "Os_Hook_Lcfg.h"
#include "Os_Isr_Lcfg.h"
#include "Os_ScheduleTable_Lcfg.h"
#include "Os_ScheduleTable.h"
#include "Os_ServiceFunction_Lcfg.h"
#include "Os_Task_Lcfg.h"
#include "Os_Timer.h"

/* Os hal dependencies */


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

/*! Dynamic application data: SystemApplication_OsCore */
OS_LOCAL VAR(Os_AppType, OS_VAR_NOINIT) OsCfg_App_SystemApplication_OsCore_Dyn;

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

/*! Object reference table for application alarms: SystemApplication_OsCore */
OS_LOCAL CONST(Os_AlarmConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_AlarmRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_ALARMS + 1] =
{
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_10ms),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_20ms),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_5ms),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE_CpApMySwc_RCtApMySwcCode),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE_IO_Task_0_200ms),
  NULL_PTR
};

/*! Object reference table for application counters: SystemApplication_OsCore */
OS_LOCAL CONST(Os_CounterConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_CounterRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_COUNTERS + 1] =
{
  OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
  NULL_PTR
};

/*! Object reference table for application hooks: SystemApplication_OsCore */
OS_LOCAL CONST(Os_HookConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_HookRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_HOOKS + 1] =
{
  NULL_PTR
};

/*! Object reference table for application ISRs: SystemApplication_OsCore */
OS_LOCAL CONST(Os_IsrConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_IsrRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_ISRS + 1] =
{
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrErr_0),
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrRx_0),
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrTx_0),
  OS_ISR_CASTCONFIG_ISR_2_ISR(OsCfg_Isr_CanIsrWakeup_0),
  OS_TIMER_CASTCONFIG_TIMEREISR_2_ISR(OsCfg_Isr_CounterIsr_SystemTimer),
  NULL_PTR
};

/*! Object reference table for application category 1 (EXTERNAL) ISRs: SystemApplication_OsCore */
OS_LOCAL CONST(Os_IsrCat1ConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_Cat1IsrRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_CAT1ISRS + 1] =
{
  (Os_IsrCat1ConfigRefType) &OsCfg_Cat1Isr_VttSysVarIsr_0,
  NULL_PTR
};

/*! Object reference table for application schedule tables: SystemApplication_OsCore */
OS_LOCAL CONST(Os_SchTConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_SchTRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_SCHTS + 1] =
{
  NULL_PTR
};

/*! Object reference table for application tasks: SystemApplication_OsCore */
OS_LOCAL CONST(Os_TaskConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_TaskRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_TASKS + 1] =
{
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_IO_Task),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_IdleTask_OsCore),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_Init_Task),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_My_Task),
  OS_TASK_CASTCONFIG_TASK_2_TASK(OsCfg_Task_SchM_Task),
  NULL_PTR
};

/*! Object reference table for application trusted functions: SystemApplication_OsCore */
OS_LOCAL CONST(Os_ServiceConfigRefType, OS_CONST) OsCfg_App_SystemApplication_OsCore_ServiceRefs[OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_SERVICES + 1] =
{
  NULL_PTR
};

/*! Application configuration data: SystemApplication_OsCore */
CONST(Os_AppConfigType, OS_CONST) OsCfg_App_SystemApplication_OsCore =
{
  /* .Dyn                  = */ &OsCfg_App_SystemApplication_OsCore_Dyn,
  /* .Core                 = */ &OsCfg_Core_OsCore,
  /* .AccessRightId        = */ OS_APPID2MASK(SystemApplication_OsCore),
  /* .TaskRefs             = */ OsCfg_App_SystemApplication_OsCore_TaskRefs,
  /* .TaskCount            = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_TASKS,
  /* .IsrCat2Refs          = */ OsCfg_App_SystemApplication_OsCore_IsrRefs,
  /* .IsrCat2Count         = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_ISRS,
  /* .IsrCat1Refs          = */ OsCfg_App_SystemApplication_OsCore_Cat1IsrRefs,
  /* .IsrCat1Count         = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_CAT1ISRS,
  /* .CounterRefs          = */ OsCfg_App_SystemApplication_OsCore_CounterRefs,
  /* .CounterCount         = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_COUNTERS,
  /* .AlarmRefs            = */ OsCfg_App_SystemApplication_OsCore_AlarmRefs,
  /* .AlarmCount           = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_ALARMS,
  /* .SchTRefs             = */ OsCfg_App_SystemApplication_OsCore_SchTRefs,
  /* .SchTCount            = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_SCHTS,
  /* .ServiceFunctions     = */ OsCfg_App_SystemApplication_OsCore_ServiceRefs,
  /* .ServiceFunctionCount = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_SERVICES,
  /* .StartupHookRef       = */ NULL_PTR,
  /* .ShutdownHookRef      = */ NULL_PTR,
  /* .ErrorHookRef         = */ NULL_PTR,
  /* .HookRefs             = */ OsCfg_App_SystemApplication_OsCore_HookRefs,
  /* .HookCount            = */ OS_CFG_NUM_APP_SYSTEMAPPLICATION_OSCORE_HOOKS,
  /* .RestartTask          = */ NULL_PTR,
  /* .IsTrusted            = */ TRUE,
  /* .IsPriveleged         = */ TRUE,
  /* .HasTimingProtectionDelay = */ FALSE,
  /* .Id                   = */ SystemApplication_OsCore
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for applications. */
CONSTP2CONST(Os_AppConfigType, OS_CONST, OS_CONST) OsCfg_AppRefs[OS_APPID_COUNT + 1] =
{
  &OsCfg_App_SystemApplication_OsCore,
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
 *  END OF FILE: Os_Application_Lcfg.c
 *********************************************************************************************************************/
