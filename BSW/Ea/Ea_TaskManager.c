/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file  Ea_TaskManager.c
 *       \brief  Ea Sub-component TaskManager source file
 *
 *     \details  TaskManager is responsible for processing asynchronous sub-components in correct order by calling the
 *               appropriate execution services. TaskManager is also responsible for canceling all active sub-components
 *               in case Ea_Cancel was called by user.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_TaskManager.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EA_NUMBER_OF_LAYERS        (0x03u)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef P2FUNC(void, EA_PRIVATE_CODE, EaTm_FctPtrType)(void);

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/* Variable which represents job stack */
EA_LOCAL VAR(EaTm_FctPtrType, EA_PRIVATE_CODE) EaTm_ExecuteServiceStack[EA_NUMBER_OF_LAYERS];

/* Variable which represents cancel stack */
EA_LOCAL VAR(EaTm_FctPtrType, EA_PRIVATE_CODE) EaTm_CancelServiceStack[EA_NUMBER_OF_LAYERS];

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  EaTm_GetActiveTask()
**********************************************************************************************************************/
/*!
 * \brief      Returns most recent execution service from job stack
 * \details    Returns most recent execution service from job stack
 * \return     Function pointer of execution service
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (EaTm_FctPtrType, EA_PRIVATE_CODE) EaTm_GetActiveTask (void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaTm_GetActiveTask()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC (EaTm_FctPtrType, EA_PRIVATE_CODE) EaTm_GetActiveTask (void)
{
  sint8 LoopCounter;
  VAR (EaTm_FctPtrType, EA_PRIVATE_CODE) returnValue = NULL_PTR;

  /* #10 Loop steps backwards through job stack beginning with layer 3 element */
  for (LoopCounter = EA_NUMBER_OF_LAYERS - 1; LoopCounter >= 0; LoopCounter--)
  {
    /* #20 First element in job stack which contains an execution function will be returned */
    if (EaTm_ExecuteServiceStack[LoopCounter] != NULL_PTR)
    {
      returnValue = EaTm_ExecuteServiceStack[LoopCounter];
      break;
    }
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaTm_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_Init(void)
{
  uint8 LoopCounter;
  for (LoopCounter = 0; LoopCounter < EA_NUMBER_OF_LAYERS; LoopCounter++)
  {
    EaTm_ExecuteServiceStack[LoopCounter] = NULL_PTR; /* SBSW_EA_11 */
    EaTm_CancelServiceStack[LoopCounter] = NULL_PTR; /* SBSW_EA_11 */
  }
}

/**********************************************************************************************************************
  EaTm_AddTask()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaTm_AddTask (EaTm_ExecuteServicePointerType ExecuteService,
        EaTm_CancelServicePointerType CancelService, uint8 Layer)
{
  Std_ReturnType retVal;
  /* #10 Ensure that position in job stack is empty before adding services to execution and cancel stack */
  if ((EaTm_ExecuteServiceStack[Layer] == NULL_PTR) && (EaTm_CancelServiceStack[Layer] == NULL_PTR))
  {
    EaTm_ExecuteServiceStack[Layer] = ExecuteService; /* SBSW_EA_12 */
    EaTm_CancelServiceStack[Layer] = CancelService; /* SBSW_EA_12 */
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
  EaTm_RemoveTask()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_RemoveTask (EaTm_ExecuteServicePointerType ExecuteService,
        EaTm_CancelServicePointerType CancelService, uint8 Layer)
{
  /* #10 Ensure that services which should be removed match services of calling layer */
  if ((EaTm_ExecuteServiceStack[Layer] == ExecuteService) && (EaTm_CancelServiceStack[Layer] == CancelService))
  {
    EaTm_ExecuteServiceStack[Layer] = NULL_PTR; /* SBSW_EA_12 */
    EaTm_CancelServiceStack[Layer] = NULL_PTR; /* SBSW_EA_12 */
  }
}

/**********************************************************************************************************************
  EaTm_ExecuteTask()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_ExecuteTask (void)
{
  /* #10 Retrieve last execution service in job stack */
  VAR (EaTm_FctPtrType, EA_PRIVATE_CODE) currentExecuteService;
  currentExecuteService = EaTm_GetActiveTask();

  /* #20 Check for NULL_PTR which is returned in case no sub-component is registered to TaskManager */
  if (currentExecuteService != NULL_PTR)
  {
    currentExecuteService(); /* SBSW_EA_13 */
  }
}

/**********************************************************************************************************************
  EaTm_CancelTasks()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaTm_CancelTasks (void)
{
  sint8 LoopCounter;

  /* #10 Cancel services of all registered layers are called beginning with layer 3 */
  /* #20 Index accessing CancelServiceStack is zero based, thus EA_NUMBER_OF_LAYERS is decremented by one */
  for (LoopCounter = (EA_NUMBER_OF_LAYERS - 1); LoopCounter >= 0; LoopCounter--)
  {
    if (EaTm_CancelServiceStack[LoopCounter] != NULL_PTR)
    {
      EaTm_CancelServiceStack[LoopCounter](); /* SBSW_EA_13 */
    }
  }
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Ea_TaskManager.c
 *********************************************************************************************************************/

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_11
      \DESCRIPTION Array write access of parameters EaTm_ExecuteServiceStack and EaTm_CancelServiceStack in for-loop.
      \COUNTERMEASURE \N Array write access is not critical because both the upper limit of the for-loop and the size of
                         the array parameters is determined by private #define EA_NUMBER_OF_LAYERS.
                         The lower limit (0) of the for-loop is legitimate as well.
    \ID SBSW_EA_12
      \DESCRIPTION Array write access of parameters EaTm_ExecuteServiceStack and EaTm_CancelServiceStack with
                   index Layer. Every sub-component of EA module accesses the service, thus it has to be assured that
                   no out of bounds access occurs.
      \COUNTERMEASURE \T TCASE24791
                         The test ensures that each sub-component passes a valid Layer index which is not out of bounds.

    \ID SBSW_EA_13
      \DESCRIPTION Function, which is referenced by function pointer, is invoked. Function pointers are registered to TaskManager by EA's
                   sub-components.
      \COUNTERMEASURE \R It is checked whether the function pointer to be invoked is not NULL_PTR

    \ID SBSW_EA_14
      \DESCRIPTION TaskManager's function EaTm_AddTask is called with pointers to sub-component's execution service and cancel service
      \COUNTERMEASURE \N EaTm_AddTask is explicitly called with pointers to local functions

    \ID SBSW_EA_15
      \DESCRIPTION TaskManager's function EaTm_RemoveTask is called with pointers to sub-component's execution service and cancel service
      \COUNTERMEASURE \N EaTm_RemoveTask is explicitly called with pointers to local functions

SBSW_JUSTIFICATION_END */

