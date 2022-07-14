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
 *        \file  Ea_Layer2_InvalidateInstance.c
 *        \brief  InvalidateInstance sub-component is responsible for invalidating an instance.
 *
 *      \details  InvalidateInstance sub-component is responsible for invalidating an instance.
 *                InvalidateInstance will check the instance's status before invalidation in order to determine
 *                the correct destination for the invalidation job.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer2_InvalidateInstance.h"

#include "Ea_TaskManager.h"
#include "Ea_EepCoordinator.h"

#include "Ea_DatasetHandler.h"
#include "Ea_PartitionHandler.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define EA_INVALIDATE_INSTANCE_LAYER EA_LAYER_TWO_INDEX

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef enum
{
  EA_INVINST_STATE_UNINIT = 0,
  EA_INVINST_STATE_IDLE,
  EA_INVINST_STATE_WAIT_FOR_HEADER_INVALIDATION,
  EA_INVINST_STATE_WAIT_FOR_TRAILER_INVALIDATION,
  EA_INVINST_STATE_WAIT_FOR_ENTIRE_INVALIDATION
} EEaInvInstStateType;

typedef struct
{
  MemIf_JobResultType EaInvInst_JobResult;
  MemIf_StatusType EaInvInst_Status;
  EEaInvInstStateType EaInvInst_StateMachine;
} EaInvInst_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

EA_LOCAL VAR(EaIh_InstanceType, EA_APPL_DATA) EaInvInst_Instance;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaInvInst_ComponentParameterType, EA_APPL_DATA) EaInvInst_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_INVINST_STATE_UNINIT };

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaInvInst_FinishJob()
 *********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. InvalidateInstance sub-component is de-registered from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
 *  EaInvInst_PrepareInstanceForInvalidation()
 *********************************************************************************************************************/
/*!
 * \brief      Prepares instance for invalidation
 * \details    Checks status of instance and calculates next instance if necessary. If instance status is either OK or
 *             INVALID it is necessary to get the next instance, because current instance must not be overwritten.
 * \param[in]  Instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_PrepareInstanceForInvalidation(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 EaInvInst_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes idle state of internal state machine
 * \details    In order to reduce complexity, processing of idle state is out-sourced to this service
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_ProcessStateIdle(void);

/**********************************************************************************************************************
 EaInvInst_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * \brief      Processes state machine of InvalidateInstance sub-component
 * \details    Processes state machine of InvalidateInstance sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaInvInst_FinishJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 De-registers InvalidateInstance sub-component from TaskManager */
  EaTm_RemoveTask(EaInvInst_Execute, EaInvInst_Cancel, EA_INVALIDATE_INSTANCE_LAYER); /* SBSW_EA_15 */

  /* #20 Reset internal parameters */
  EaInvInst_ComponentParameter.EaInvInst_JobResult = JobResult;
  EaInvInst_ComponentParameter.EaInvInst_Status = MEMIF_IDLE;
  EaInvInst_ComponentParameter.EaInvInst_StateMachine = EA_INVINST_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaInvInst_PrepareInstanceForInvalidation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_PrepareInstanceForInvalidation(EaIh_InstanceVarPointerType Instance)
{
  if (EaIh_IsStatusOkOrInvalid(Instance) == TRUE) /* SBSW_EA_73 */
  {
    /* #10 If Instance is last instance in dataset, wrap around will occur and therefore cycleCounter has to be incremented */
    if (EaDh_IsLastInstance(Instance) == TRUE) /* SBSW_EA_73 */
    {
      EaIh_IncrementCycleCounter(Instance); /* SBSW_EA_71 */
    }
    /* #20 Calculate address of next instance and store it in instance object */
    EaDh_StoreNextInstanceAddress(Instance); /* SBSW_EA_71 */
  }

  /* #30 Status and Management Bytes of instance have to be updated, according to invalidate job */
  EaIh_SetInstanceStatus(Instance, INSTANCE_STATUS_INVALID); /* SBSW_EA_71 */
  EaIh_SetManagementValuesAccordingToStatus(Instance); /* SBSW_EA_71 */
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 EaInvInst_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_ProcessStateIdle(void)
{
  /* #5 Prepare instance to assure correct position of invalidation */
  EaInvInst_PrepareInstanceForInvalidation(&EaInvInst_Instance); /* SBSW_EA_72 */

  /* #10 Check if instance is invalidated in one or two steps */
  if (EaIh_GetInstanceLength(&EaInvInst_Instance) > EaPh_GetPartitionWriteAlignment()) /* SBSW_EA_72 */
  {
    /* #11 Two steps of invalidation are needed due to size of instance */
    /* #12 Header part is invalidated first */

    if (EaIh_InvalidateHeader(&EaInvInst_Instance) == E_OK) /* SBSW_EA_72 */
    {
      EaInvInst_ComponentParameter.EaInvInst_StateMachine = EA_INVINST_STATE_WAIT_FOR_HEADER_INVALIDATION;
    }
    else
    {
      EaInvInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* #13 One step of invalidation is sufficient, since the instance is as small as EEP's write alignment */
    if (EaIh_InvalidateEntireInstance(&EaInvInst_Instance) == E_OK) /* SBSW_EA_72 */
    {
      EaInvInst_ComponentParameter.EaInvInst_StateMachine = EA_INVINST_STATE_WAIT_FOR_ENTIRE_INVALIDATION;
    }
    else
    {
      EaInvInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
}

/**********************************************************************************************************************
 EaInvInst_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaInvInst_ProcessStateMachine(void)
{
  switch (EaInvInst_ComponentParameter.EaInvInst_StateMachine)
  {
    /*** -------------- IDLE -------------------------------------------------------------------------- ***/
    case EA_INVINST_STATE_IDLE:

        /* #10 Prepare instance for invalidation and initiate invalidate job */
        EaInvInst_ProcessStateIdle();

      break;
      /*** -------------- WAIT FOR HEADER INVALIDATION -------------------------------------------------- ***/
    case EA_INVINST_STATE_WAIT_FOR_HEADER_INVALIDATION:
      if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
      {
        /* #20 Invalidate Trailer byte */
        if (EaIh_InvalidateTrailer(&EaInvInst_Instance) == E_OK) /* SBSW_EA_72 */
        {
          EaInvInst_ComponentParameter.EaInvInst_StateMachine = EA_INVINST_STATE_WAIT_FOR_TRAILER_INVALIDATION;
        }
        else
        {
          EaInvInst_FinishJob(MEMIF_JOB_FAILED);
        }
      }
      else
      {
        EaInvInst_FinishJob(Ea_Eep_GetJobResult());
      }
      break;
      /*** -------------- WAIT FOR TRAILER INVALIDATION ------------------------------------------------- ***/
    case EA_INVINST_STATE_WAIT_FOR_TRAILER_INVALIDATION:
      /* #30 Invalidation in two steps is finished */
      EaInvInst_FinishJob(Ea_Eep_GetJobResult());
      break;
      /*** -------------- WAIT FOR ENTIRE INVALIDATION -------------------------------------------------- ***/
    case EA_INVINST_STATE_WAIT_FOR_ENTIRE_INVALIDATION:
      /* #40 Invalidation in one step is finished */
      EaInvInst_FinishJob(Ea_Eep_GetJobResult());
      break;

    default:
      EaInvInst_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}/* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaInvInst_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaInvInst_Init(void)
{
  EaInvInst_ComponentParameter.EaInvInst_Status = MEMIF_IDLE;
  EaInvInst_ComponentParameter.EaInvInst_JobResult = MEMIF_JOB_OK;

  EaIh_InitInstance(&EaInvInst_Instance); /* SBSW_EA_72 */
  EaInvInst_ComponentParameter.EaInvInst_StateMachine = EA_INVINST_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaInvInst_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaInvInst_GetStatus(void)
{
  return EaInvInst_ComponentParameter.EaInvInst_Status;
}

/**********************************************************************************************************************
 *  EaInvInst_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaInvInst_GetJobResult(void)
{
  return EaInvInst_ComponentParameter.EaInvInst_JobResult;
}

/**********************************************************************************************************************
 *  EaInvInst_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaInvInst_StartJob(EaIh_InstanceType Instance)
{
  Std_ReturnType retVal;

  /* #10 Requested job is only accepted if InvalidateInstance's state machine is idle */
  if (EaInvInst_ComponentParameter.EaInvInst_StateMachine == EA_INVINST_STATE_IDLE)
  {
    /* #20 Register InvalidateInstance's execution and cancel service to TaskManager */
    retVal = EaTm_AddTask(EaInvInst_Execute, EaInvInst_Cancel, EA_INVALIDATE_INSTANCE_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaInvInst_ComponentParameter.EaInvInst_Status = MEMIF_BUSY;
      EaInvInst_ComponentParameter.EaInvInst_JobResult = MEMIF_JOB_PENDING;

      EaInvInst_Instance = Instance;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaInvInst_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaInvInst_Execute(void)
{
  /* #10 InvalidateInstance's state machine is only processed if status is busy */
  if (EaInvInst_ComponentParameter.EaInvInst_Status == MEMIF_BUSY)
  {
    EaInvInst_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaInvInst_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaInvInst_Cancel(void)
{
  EaInvInst_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_71
      \DESCRIPTION Function is called with pointer to an instance object
      \COUNTERMEASURE \N Caller ensures that pointer passed to service is valid

    \ID SBSW_EA_72
      \DESCRIPTION Function is called with reference to local parameter EaInvInst_Instance
      \COUNTERMEASURE \N EaInvInst_Instance is a local parameter, thus references to this parameter are valid.

    \ID SBSW_EA_73
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceConstPointerType or EaIh_InstanceVarPointerType
      \COUNTERMEASURE \N Instance parameter is passed to function via input parameter, thus caller ensures that parameter referenced
                         by pointer is valid.

SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer2_InvalidateInstance.c
 *********************************************************************************************************************/
