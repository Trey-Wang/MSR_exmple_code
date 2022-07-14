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
 *              File: Os_Counter_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_COUNTER_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Counter_Cfg.h"
#include "Os_Counter_Lcfg.h"
#include "Os_Counter.h"

/* Os kernel module dependencies */
#include "Os_Application_Lcfg.h"
#include "Os_Cfg.h"
#include "Os_Common.h"
#include "Os_Core_Lcfg.h"
#include "Os_PriorityQueue.h"
#include "Os_Timer.h"

/* Os hal dependencies */
#include "Os_Hal_Cfg.h"
#include "Os_Hal_Timer_Lcfg.h"


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

/*! Dynamic counter data: SystemTimer */
OS_LOCAL VAR(Os_TimerSwType, OS_VAR_NOINIT) OsCfg_Counter_SystemTimer_Dyn;
OS_LOCAL VAR(Os_PriorityQueueType, OS_VAR_NOINIT) OsCfg_Counter_SystemTimer_JobQueue_Dyn;
OS_LOCAL VAR(Os_PriorityQueueNodeType, OS_VAR_NOINIT)
  OsCfg_Counter_SystemTimer_JobQueueNodes_Dyn[OS_CFG_NUM_COUNTER_SYSTEMTIMER_JOBS + 1];

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

/*! Counter configuration data: SystemTimer */
CONST(Os_TimerPitConfigType, OS_CONST) OsCfg_Counter_SystemTimer =
{
  /* .SwCounter = */
  {
  /* .Counter = */
  {
    /* .Characteristics       = */
    {
      /* .MaxAllowedValue      = */ OSMAXALLOWEDVALUE_SystemTimer,
      /* .MaxCountingValue     = */ OS_TIMERPIT_GETMAXCOUNTINGVALUE(OSMAXALLOWEDVALUE_SystemTimer),
      /* .MaxDifferentialValue = */ OS_TIMERPIT_GETMAXDIFFERENTIALVALUE(OSMAXALLOWEDVALUE_SystemTimer),
      /* .MinCycle             = */ OSMINCYCLE_SystemTimer,
      /* .TicksPerBase         = */ OSTICKSPERBASE_SystemTimer
    },
    /* .JobQueue              = */
    {
      /* .Queue     = */ OsCfg_Counter_SystemTimer_JobQueueNodes_Dyn,
      /* .Dyn       = */ &OsCfg_Counter_SystemTimer_JobQueue_Dyn,
      /* .QueueSize = */ OS_CFG_NUM_COUNTER_SYSTEMTIMER_JOBS
    },
    /* .DriverType            = */ OS_TIMERTYPE_PERIODIC_TICK,
    /* .Core                  = */ &OsCfg_Core_OsCore,
    /* .OwnerApplication      = */ &OsCfg_App_SystemApplication_OsCore,
    /* .AccessingApplications = */ OS_APPID2MASK(SystemApplication_OsCore)
  },
  /* .Dyn     = */ &OsCfg_Counter_SystemTimer_Dyn
},
  /* .HwConfig  = */ &OsCfg_Hal_TimerPit_SystemTimer
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Object reference table for counters. */
CONSTP2CONST(Os_CounterConfigType, OS_CONST, OS_CONST) OsCfg_CounterRefs[OS_COUNTERID_COUNT + 1] =
{
  OS_COUNTER_CASTCONFIG_TIMERPIT_2_COUNTER(OsCfg_Counter_SystemTimer),
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
 *  END OF FILE: Os_Counter_Lcfg.c
 *********************************************************************************************************************/
