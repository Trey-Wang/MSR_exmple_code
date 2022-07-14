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
 *              File: Os_Scheduler_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_SCHEDULER_LCFG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Scheduler_Cfg.h"
#include "Os_Scheduler_Lcfg.h"
#include "Os_Scheduler.h"

/* Os kernel module dependencies */
#include "Os_BitArray.h"
#include "Os_Common.h"
#include "Os_Deque.h"

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

/*! Dynamic scheduler data: OsCore */
OS_LOCAL VAR(Os_SchedulerType, OS_VAR_NOINIT) OsCfg_Scheduler_OsCore_Dyn;
OS_LOCAL VAR(Os_BitArrayType, OS_VAR_NOINIT) OsCfg_Scheduler_OsCore_BitArray_Dyn;
OS_LOCAL OS_BITARRAY_DECLARE(OsCfg_Scheduler_OsCore_BitField_Dyn, OS_CFG_NUM_TASKQUEUES, OS_VAR_NOINIT);

/*! Dynamic task queues data: OsCore */
OS_LOCAL VAR(Os_DequeType, OS_VAR_NOINIT) OsCfg_TaskQueue0_OsCore_Dyn;
OS_LOCAL VAR(Os_FifoPtrNodeType, OS_VAR_NOINIT)
  OsCfg_TaskQueueNodes0_OsCore_Dyn[OS_CFG_NUM_TASKQUEUE0_OSCORE_SLOTS];
OS_LOCAL VAR(Os_DequeType, OS_VAR_NOINIT) OsCfg_TaskQueue1_OsCore_Dyn;
OS_LOCAL VAR(Os_FifoPtrNodeType, OS_VAR_NOINIT)
  OsCfg_TaskQueueNodes1_OsCore_Dyn[OS_CFG_NUM_TASKQUEUE1_OSCORE_SLOTS];
OS_LOCAL VAR(Os_DequeType, OS_VAR_NOINIT) OsCfg_TaskQueue2_OsCore_Dyn;
OS_LOCAL VAR(Os_FifoPtrNodeType, OS_VAR_NOINIT)
  OsCfg_TaskQueueNodes2_OsCore_Dyn[OS_CFG_NUM_TASKQUEUE2_OSCORE_SLOTS];
OS_LOCAL VAR(Os_DequeType, OS_VAR_NOINIT) OsCfg_TaskQueue3_OsCore_Dyn;
OS_LOCAL VAR(Os_FifoPtrNodeType, OS_VAR_NOINIT)
  OsCfg_TaskQueueNodes3_OsCore_Dyn[OS_CFG_NUM_TASKQUEUE3_OSCORE_SLOTS];
OS_LOCAL VAR(Os_DequeType, OS_VAR_NOINIT) OsCfg_TaskQueue4_OsCore_Dyn;
OS_LOCAL VAR(Os_FifoPtrNodeType, OS_VAR_NOINIT)
  OsCfg_TaskQueueNodes4_OsCore_Dyn[OS_CFG_NUM_TASKQUEUE4_OSCORE_SLOTS];

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

/*! Task queues configuration data: OsCore */
OS_LOCAL CONST(Os_DequeConfigType, OS_CONST) OsCfg_Scheduler_OsCore_TaskQueues[OS_CFG_NUM_TASKQUEUES];
OS_LOCAL CONST(Os_DequeConfigType, OS_CONST) OsCfg_Scheduler_OsCore_TaskQueues[OS_CFG_NUM_TASKQUEUES] =
{
  /* [0] = */
  {
    /* .FifoRead  = */
    {
      /* .Dyn   = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue0_OsCore_Dyn),
      /* .Write = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue0_OsCore_Dyn),
      /* .Data  = */ OsCfg_TaskQueueNodes0_OsCore_Dyn,
      /* .Size  = */ OS_CFG_NUM_TASKQUEUE0_OSCORE_SLOTS
    },
    /* .FifoWrite = */
    {
      /* .Dyn  = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue0_OsCore_Dyn),
      /* .Read = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue0_OsCore_Dyn),
      /* .Data = */ OsCfg_TaskQueueNodes0_OsCore_Dyn,
      /* .Size = */ OS_CFG_NUM_TASKQUEUE0_OSCORE_SLOTS
    }
  },
  /* [1] = */
  {
    /* .FifoRead  = */
    {
      /* .Dyn   = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue1_OsCore_Dyn),
      /* .Write = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue1_OsCore_Dyn),
      /* .Data  = */ OsCfg_TaskQueueNodes1_OsCore_Dyn,
      /* .Size  = */ OS_CFG_NUM_TASKQUEUE1_OSCORE_SLOTS
    },
    /* .FifoWrite = */
    {
      /* .Dyn  = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue1_OsCore_Dyn),
      /* .Read = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue1_OsCore_Dyn),
      /* .Data = */ OsCfg_TaskQueueNodes1_OsCore_Dyn,
      /* .Size = */ OS_CFG_NUM_TASKQUEUE1_OSCORE_SLOTS
    }
  },
  /* [2] = */
  {
    /* .FifoRead  = */
    {
      /* .Dyn   = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue2_OsCore_Dyn),
      /* .Write = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue2_OsCore_Dyn),
      /* .Data  = */ OsCfg_TaskQueueNodes2_OsCore_Dyn,
      /* .Size  = */ OS_CFG_NUM_TASKQUEUE2_OSCORE_SLOTS
    },
    /* .FifoWrite = */
    {
      /* .Dyn  = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue2_OsCore_Dyn),
      /* .Read = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue2_OsCore_Dyn),
      /* .Data = */ OsCfg_TaskQueueNodes2_OsCore_Dyn,
      /* .Size = */ OS_CFG_NUM_TASKQUEUE2_OSCORE_SLOTS
    }
  },
  /* [3] = */
  {
    /* .FifoRead  = */
    {
      /* .Dyn   = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue3_OsCore_Dyn),
      /* .Write = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue3_OsCore_Dyn),
      /* .Data  = */ OsCfg_TaskQueueNodes3_OsCore_Dyn,
      /* .Size  = */ OS_CFG_NUM_TASKQUEUE3_OSCORE_SLOTS
    },
    /* .FifoWrite = */
    {
      /* .Dyn  = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue3_OsCore_Dyn),
      /* .Read = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue3_OsCore_Dyn),
      /* .Data = */ OsCfg_TaskQueueNodes3_OsCore_Dyn,
      /* .Size = */ OS_CFG_NUM_TASKQUEUE3_OSCORE_SLOTS
    }
  },
  /* [4] = */
  {
    /* .FifoRead  = */
    {
      /* .Dyn   = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue4_OsCore_Dyn),
      /* .Write = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue4_OsCore_Dyn),
      /* .Data  = */ OsCfg_TaskQueueNodes4_OsCore_Dyn,
      /* .Size  = */ OS_CFG_NUM_TASKQUEUE4_OSCORE_SLOTS
    },
    /* .FifoWrite = */
    {
      /* .Dyn  = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRWRITE(OsCfg_TaskQueue4_OsCore_Dyn),
      /* .Read = */ OS_DEQUE_CASTDYN_DEQUE_2_FIFOPTRREAD(OsCfg_TaskQueue4_OsCore_Dyn),
      /* .Data = */ OsCfg_TaskQueueNodes4_OsCore_Dyn,
      /* .Size = */ OS_CFG_NUM_TASKQUEUE4_OSCORE_SLOTS
    }
  }
};

/*! Scheduler configuration data: OsCore */
CONST(Os_SchedulerConfigType, OS_CONST) OsCfg_Scheduler_OsCore =
{
  /* .BitArray           = */
  {
    /* .Dyn       = */ &OsCfg_Scheduler_OsCore_BitArray_Dyn,
    /* .Data      = */ OsCfg_Scheduler_OsCore_BitField_Dyn,
    /* .Size      = */ OS_BITARRAY_SIZE(OS_CFG_NUM_TASKQUEUES),
    /* .BitLength = */ OS_BITARRAY_LENGTH(OS_CFG_NUM_TASKQUEUES),
  },
  /* .Dyn                = */ &OsCfg_Scheduler_OsCore_Dyn,
  /* .TaskQueues         = */ OsCfg_Scheduler_OsCore_TaskQueues,
  /* .NumberOfPriorities = */ OS_CFG_NUM_TASKQUEUES
};

#define OS_STOP_SEC_CORE0_CONST_UNSPECIFIED
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
 *  END OF FILE: Os_Scheduler_Lcfg.c
 *********************************************************************************************************************/
