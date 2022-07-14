/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file  Ea_Layer1_Invalidate.c
 *        \brief  This sub-component is responsible for invalidating data
 *
 *      \details  EaInvalidate sub-component is responsible for invalidating data in EEPROM which belongs to passed
 *                BlockNumber. Internally, sub-component InstanceFinder is used to determine correct destination of
 *                invalidation job. Additionally InvalidateInstance sub-component is used to actually perform the
 *                invalidation of the most recent instance in EEPROM.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer1_Invalidate.h"
#include "Ea_Layer2_InstanceFinder.h"
#include "Ea_Layer2_InvalidateInstance.h"

#include "Ea_InstanceHandler.h"
#include "Ea_TaskManager.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EA_INVALIDATE_LAYER EA_LAYER_ONE_INDEX

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

typedef enum
{
  EA_INV_STATE_UNINIT = 0,
  EA_INV_STATE_IDLE,
  EA_INV_STATE_WAIT_FOR_INSTANCEFINDER,
  EA_INV_STATE_WAIT_FOR_INVALIDATION
}EEaInvStateType;

typedef struct
{
  MemIf_JobResultType EaInv_JobResult;
  MemIf_StatusType EaInv_Status;
  EEaInvStateType EaInv_StateMachine;
}EaInv_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaInv_ComponentParameterType, EA_APPL_DATA) EaInv_ComponentParameter =
    {MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_INV_STATE_UNINIT};

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaIh_InstanceType, EA_APPL_DATA) EaInv_Instance;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  EaInv_FinishJob()
**********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. EaInvalidate sub-component de-registers from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PUBLIC_CODE) EaInv_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
  EaInv_ProcessStateMachine()
**********************************************************************************************************************/
/*!
 * \brief      Processes state machine of EaInvalidate sub-component
 * \details    Processes state machine of EaInvalidate sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInv_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaInv_FinishJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PUBLIC_CODE) EaInv_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 EaInvalidate sub-component de-registers itself from TaskManager */
  EaTm_RemoveTask(EaInv_Execute, EaInv_Cancel, EA_INVALIDATE_LAYER); /* SBSW_EA_15 */

  EaInv_ComponentParameter.EaInv_JobResult = JobResult;
  EaInv_ComponentParameter.EaInv_Status = MEMIF_IDLE;
  EaInv_ComponentParameter.EaInv_StateMachine = EA_INV_STATE_IDLE;
}

/**********************************************************************************************************************
  EaInv_ProcessStateMachine()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInv_ProcessStateMachine(void)
{
  switch (EaInv_ComponentParameter.EaInv_StateMachine)
  {
    /*** -------------- IDLE -------------------------------------------------- ***/
    case EA_INV_STATE_IDLE:

      /* #10 InstanceFinder sub-component will be initiated */
      if (EaIf_StartJob(&EaInv_Instance) == E_OK) /* SBSW_EA_101 */
      {
        EaInv_ComponentParameter.EaInv_StateMachine = EA_INV_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else
      {
        EaInv_FinishJob(MEMIF_JOB_FAILED);
      }
      break;
    /*** -------------- WAIT FOR INSTANCE FINDER ------------------------------ ***/
    case EA_INV_STATE_WAIT_FOR_INSTANCEFINDER:
      if (EaIf_GetStatus() == MEMIF_IDLE)
      {
        /* #20 InstanceFinder finished its job */
        if (EaIf_GetJobResult() == MEMIF_JOB_OK)
        {
          /* #30 InvalidateInstance sub-component will be initiated */
          if (EaInvInst_StartJob(EaInv_Instance) == E_OK)
          {
            EaInv_ComponentParameter.EaInv_StateMachine = EA_INV_STATE_WAIT_FOR_INVALIDATION;
          }
          else
          {
            EaInv_FinishJob(MEMIF_JOB_FAILED);
          }
        }
        else
        {
          EaInv_FinishJob(EaIf_GetJobResult());
        }
      }
      break;
    /*** -------------- INVALIDATING DATA ------------------------------------- ***/
    case EA_INV_STATE_WAIT_FOR_INVALIDATION:
      if (EaInvInst_GetStatus() == MEMIF_IDLE)
      {
        /* #40 InvalidateInstance finished its job, thus layer1 EaInvalidate sub-component is finished */
        EaInv_FinishJob(EaInvInst_GetJobResult());
      }
      break;

    default:
      EaInv_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaInv_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaInv_Init(void)
{
  EaInv_ComponentParameter.EaInv_Status = MEMIF_IDLE;
  EaInv_ComponentParameter.EaInv_JobResult = MEMIF_JOB_OK;

  EaIh_InitInstance(&EaInv_Instance); /* SBSW_EA_101 */

  EaInv_ComponentParameter.EaInv_StateMachine = EA_INV_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaInv_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaInv_GetStatus(void)
{
  return EaInv_ComponentParameter.EaInv_Status;
}

/**********************************************************************************************************************
 *  EaInv_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaInv_GetJobResult(void)
{
  return EaInv_ComponentParameter.EaInv_JobResult;
}

/**********************************************************************************************************************
 *  EaInv_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaInv_StartJob(void)
{
  Std_ReturnType retVal;

  /* #10 Invalidate job is only accepted if state machine is idle */
  if (EaInv_ComponentParameter.EaInv_StateMachine == EA_INV_STATE_IDLE)
  {
    /* #20 EaInvalidate sub-component registers its execution and cancel service to TaskManager */
    retVal = EaTm_AddTask(EaInv_Execute, EaInv_Cancel, EA_INVALIDATE_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaInv_ComponentParameter.EaInv_Status = MEMIF_BUSY;
      EaInv_ComponentParameter.EaInv_JobResult = MEMIF_JOB_PENDING;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaInv_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaInv_Execute(void)
{
  /* #10 State machine is only processed if module status is busy */
  if (EaInv_ComponentParameter.EaInv_Status == MEMIF_BUSY)
  {
    EaInv_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaInv_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaInv_Cancel(void)
{
  EaInv_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_101
      \DESCRIPTION Function is called with reference to local parameter EaInv_Instance
      \COUNTERMEASURE \N EaInv_Instance is a local parameter, thus references to this parameter are valid.
SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Invalidate.c
 *********************************************************************************************************************/
