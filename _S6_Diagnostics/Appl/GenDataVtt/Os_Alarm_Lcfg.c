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
 *              File: Os_Alarm_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_ALARM_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Alarm_Lcfg.h"
#include "Os_Alarm.h"

/* Os kernel module dependencies */
#include "Os_Application_Lcfg.h"
#include "Os_Cfg.h"
#include "Os_Common.h"
#include "Os_Counter_Lcfg.h"
#include "Os_Counter.h"
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

/*! Dynamic alarm data: Rte_Al_TE2_SchM_Task_0_10ms */
OS_LOCAL VAR(Os_AlarmType, OS_VAR_NOINIT) OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_10ms_Dyn;

/*! Dynamic alarm data: Rte_Al_TE2_SchM_Task_0_20ms */
OS_LOCAL VAR(Os_AlarmType, OS_VAR_NOINIT) OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_20ms_Dyn;

/*! Dynamic alarm data: Rte_Al_TE2_SchM_Task_0_5ms */
OS_LOCAL VAR(Os_AlarmType, OS_VAR_NOINIT) OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_5ms_Dyn;

/*! Dynamic alarm data: Rte_Al_TE_CpApMySwc_RCtApMySwcCode */
OS_LOCAL VAR(Os_AlarmType, OS_VAR_NOINIT) OsCfg_Alarm_Rte_Al_TE_CpApMySwc_RCtApMySwcCode_Dyn;

/*! Dynamic alarm data: Rte_Al_TE_IO_Task_0_200ms */
OS_LOCAL VAR(Os_AlarmType, OS_VAR_NOINIT) OsCfg_Alarm_Rte_Al_TE_IO_Task_0_200ms_Dyn;

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

/*! Alarm configuration data: Rte_Al_TE2_SchM_Task_0_10ms */
CONST(Os_AlarmSetEventConfigType, OS_CONST) OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_10ms =
{
  /* .Alarm = */
  {
    /* .Job                   = */
    {
      /* .Dyn      = */ OS_ALARM_CASTDYN_ALARM_2_JOB(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_10ms_Dyn),
      /* .Counter  = */ OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
      /* .Callback = */ Os_AlarmActionSetEvent
    },
    /* .Autostart             = */
    {
      /* .AlarmTime        = */ 0UL, /* 0.0 sec */
      /* .Cycle            = */ 0UL, /* 0.0 sec */
      /* .ApplicationModes = */ OS_APPMODE_NONE,
      /* .AlarmMode        = */ OS_ALARMMODE_ABSOLUTE
    },
    /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore
  },
  /* .Task  = */ &OsCfg_Task_SchM_Task,
  /* .Mask  = */ Rte_Ev_Cyclic2_SchM_Task_0_10ms
};

/*! Alarm configuration data: Rte_Al_TE2_SchM_Task_0_20ms */
CONST(Os_AlarmSetEventConfigType, OS_CONST) OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_20ms =
{
  /* .Alarm = */
  {
    /* .Job                   = */
    {
      /* .Dyn      = */ OS_ALARM_CASTDYN_ALARM_2_JOB(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_20ms_Dyn),
      /* .Counter  = */ OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
      /* .Callback = */ Os_AlarmActionSetEvent
    },
    /* .Autostart             = */
    {
      /* .AlarmTime        = */ 0UL, /* 0.0 sec */
      /* .Cycle            = */ 0UL, /* 0.0 sec */
      /* .ApplicationModes = */ OS_APPMODE_NONE,
      /* .AlarmMode        = */ OS_ALARMMODE_ABSOLUTE
    },
    /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore
  },
  /* .Task  = */ &OsCfg_Task_SchM_Task,
  /* .Mask  = */ Rte_Ev_Cyclic2_SchM_Task_0_20ms
};

/*! Alarm configuration data: Rte_Al_TE2_SchM_Task_0_5ms */
CONST(Os_AlarmSetEventConfigType, OS_CONST) OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_5ms =
{
  /* .Alarm = */
  {
    /* .Job                   = */
    {
      /* .Dyn      = */ OS_ALARM_CASTDYN_ALARM_2_JOB(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_5ms_Dyn),
      /* .Counter  = */ OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
      /* .Callback = */ Os_AlarmActionSetEvent
    },
    /* .Autostart             = */
    {
      /* .AlarmTime        = */ 0UL, /* 0.0 sec */
      /* .Cycle            = */ 0UL, /* 0.0 sec */
      /* .ApplicationModes = */ OS_APPMODE_NONE,
      /* .AlarmMode        = */ OS_ALARMMODE_ABSOLUTE
    },
    /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore
  },
  /* .Task  = */ &OsCfg_Task_SchM_Task,
  /* .Mask  = */ Rte_Ev_Cyclic2_SchM_Task_0_5ms
};

/*! Alarm configuration data: Rte_Al_TE_CpApMySwc_RCtApMySwcCode */
CONST(Os_AlarmSetEventConfigType, OS_CONST) OsCfg_Alarm_Rte_Al_TE_CpApMySwc_RCtApMySwcCode =
{
  /* .Alarm = */
  {
    /* .Job                   = */
    {
      /* .Dyn      = */ OS_ALARM_CASTDYN_ALARM_2_JOB(OsCfg_Alarm_Rte_Al_TE_CpApMySwc_RCtApMySwcCode_Dyn),
      /* .Counter  = */ OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
      /* .Callback = */ Os_AlarmActionSetEvent
    },
    /* .Autostart             = */
    {
      /* .AlarmTime        = */ 0UL, /* 0.0 sec */
      /* .Cycle            = */ 0UL, /* 0.0 sec */
      /* .ApplicationModes = */ OS_APPMODE_NONE,
      /* .AlarmMode        = */ OS_ALARMMODE_ABSOLUTE
    },
    /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore
  },
  /* .Task  = */ &OsCfg_Task_My_Task,
  /* .Mask  = */ Rte_Ev_Run_CpApMySwc_RCtApMySwcCode
};

/*! Alarm configuration data: Rte_Al_TE_IO_Task_0_200ms */
CONST(Os_AlarmSetEventConfigType, OS_CONST) OsCfg_Alarm_Rte_Al_TE_IO_Task_0_200ms =
{
  /* .Alarm = */
  {
    /* .Job                   = */
    {
      /* .Dyn      = */ OS_ALARM_CASTDYN_ALARM_2_JOB(OsCfg_Alarm_Rte_Al_TE_IO_Task_0_200ms_Dyn),
      /* .Counter  = */ OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
      /* .Callback = */ Os_AlarmActionSetEvent
    },
    /* .Autostart             = */
    {
      /* .AlarmTime        = */ 0UL, /* 0.0 sec */
      /* .Cycle            = */ 0UL, /* 0.0 sec */
      /* .ApplicationModes = */ OS_APPMODE_NONE,
      /* .AlarmMode        = */ OS_ALARMMODE_ABSOLUTE
    },
    /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore),
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore
  },
  /* .Task  = */ &OsCfg_Task_IO_Task,
  /* .Mask  = */ Rte_Ev_Cyclic_IO_Task_0_200ms
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for alarms. */
CONSTP2CONST(Os_AlarmConfigType, OS_CONST, OS_CONST) OsCfg_AlarmRefs[OS_ALARMID_COUNT + 1] =
{
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_10ms),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_20ms),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE2_SchM_Task_0_5ms),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE_CpApMySwc_RCtApMySwcCode),
  OS_ALARM_CASTCONFIG_ALARMSETEVENT_2_ALARM(OsCfg_Alarm_Rte_Al_TE_IO_Task_0_200ms),
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
 *  END OF FILE: Os_Alarm_Lcfg.c
 *********************************************************************************************************************/
