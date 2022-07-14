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
 *        \file  Ea_Layer2_InstanceFinder.c
 *        \brief  InstanceFinder is responsible for finding the most recent data
 *
 *      \details  InstanceFinder is responsible for determination of the address of recently written data (here: instance)
 *                in EEPROM according to specific BlockNumber (UserJobParameter). InstanceFinder is responsible for
 *                evaluating the determined instance's status and stores it to the instance object which is passed by caller.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer2_InstanceFinder.h"
#include "Ea_Layer3_ReadManagementBytes.h"

#include "Ea_TaskManager.h"

#include "Ea_DatasetHandler.h"
#include "Ea_BlockHandler.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EA_INSTANCE_FINDER_LAYER EA_LAYER_TWO_INDEX

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
  EA_IF_STATE_UNINIT = 0,
  EA_IF_STATE_IDLE,
  EA_IF_STATE_WAIT_FOR_READ,
  EA_IF_STATE_EXAMINE_INSTANCE
} EEaIfStateType;

typedef struct
{
  MemIf_JobResultType EaIf_JobResult;
  MemIf_StatusType EaIf_Status;
  EEaIfStateType EaIf_StateMachine;
} EaIf_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

EA_LOCAL P2VAR(EaIh_InstanceType, AUTOMATIC, EA_APPL_DATA) EaIf_Instance;

EA_LOCAL VAR(EaIh_InstanceType, EA_APPL_DATA) EaIf_InstancePrev;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaIf_ComponentParameterType, EA_APPL_DATA) EaIf_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_IF_STATE_UNINIT };

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
 *  EaIf_IsSearchFinished()
 *********************************************************************************************************************/
/*!
 * \brief      Compares both instance objects and determines if search is finished
 * \details    Compares management values of passed instance objects (previous instance and newly read instance).
 *             Based on status and cycle counter of both instances, it is determined if search for most recent
 *             instance is finished. In case search is finished, Instance object will be updated and then returned
 *             to caller of InstanceFinder sub-component. In case search is not yet finished, PreviousInstance object
 *             will be updated with values of instance object.
 * \param[in]  Instance
 * \param[in]  PreviousInstance
 * \return     TRUE: Search is finished
 * \return     FALSE: Search is not yet finished
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIf_IsSearchFinished(EaIh_InstanceVarPointerType Instance,
        EaIh_InstanceVarPointerType PreviousInstance);

/**********************************************************************************************************************
 EaIf_FinishJob()
 **********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. InstanceFinder sub-component is de-registered from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
 EaIf_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes idle state of InstanceFinder sub-component
 * \details    Processes idle state of InstanceFinder sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_ProcessIdleState(void);

/**********************************************************************************************************************
 EaIf_ProcessExamineInstanceState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes Examine Instance state of InstanceFinder sub-component
 * \details    Processes Examine Instance state of InstanceFinder sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_ProcessExamineInstanceState(void);

/**********************************************************************************************************************
 EaIf_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * \brief      Processes state machine of InstanceFinder sub-component
 * \details    Processes state machine of InstanceFinder sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaIf_IsSearchFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIf_IsSearchFinished(EaIh_InstanceVarPointerType Instance,
        EaIh_InstanceVarPointerType PreviousInstance)
{
  boolean retVal = TRUE;

  switch (EaIh_GetInstanceStatus(Instance)) /* SBSW_EA_46 */
  {
    case INSTANCE_STATUS_ERASED:
      /* #10 Search finished. Instance is erased, thus previousInstance will be returned */
      *Instance = *PreviousInstance; /* SBSW_EA_41 */
      break;

    case INSTANCE_STATUS_INCONSISTENT:
      /* #20 Search finished. If inconsistent instance contains one correct (valid or invalid) management byte, */
      /* #21 it has to be compared to previous instance */
      if (EaIh_IsValidOrInvalidManagement(EaIh_GetHeaderValue(Instance), EaIh_GetTrailerValue(Instance)) == TRUE) /* SBSW_EA_46 */ /* SBSW_EA_46 */
      {
        /* #22 Header or trailer of instance is either valid or invalid: instance has to be compared to previous one */
        if (EaIh_CompareTo(PreviousInstance, Instance) == FALSE) /* SBSW_EA_46 */
        {
          /* #23 Previous instance is more up to date, thus it is returned */
          *Instance = *PreviousInstance; /* SBSW_EA_41 */
        }
        /* #24 No else necessary. In case CompareTo == TRUE, instance is returned, since it's more up to date */
      }
      else
      {
        /* #26 Neither header nor trailer is correct (valid or invalid): previousInstance is returned */
        *Instance = *PreviousInstance; /* SBSW_EA_41 */
      }
      break;

    default :/* #40 INSTANCE_STATUS_OK and INSTANCE_STATUS_INVALID */
      /* #42 CycleCounter of both instances have to be compared to each other */
      if (EaIh_CompareTo(PreviousInstance, Instance) == TRUE) /* SBSW_EA_46 */
      {
        /* #44 Both instances have same age. Search has to be continued */
        *PreviousInstance = *Instance; /* SBSW_EA_42 */
        retVal = FALSE;
      }
      else
      {
        /* #46 Search finished. Previous instance is more up to date: previousInstance will be returned */
        *Instance = *PreviousInstance; /* SBSW_EA_41 */
      }
      break;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  EaIf_FinishJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 InstanceFinder sub-component de-registers itself from TaskManager */
  EaTm_RemoveTask(EaIf_Execute, EaIf_Cancel, EA_INSTANCE_FINDER_LAYER); /* SBSW_EA_15 */

  EaIf_ComponentParameter.EaIf_JobResult = JobResult;
  EaIf_ComponentParameter.EaIf_Status = MEMIF_IDLE;
  EaIf_ComponentParameter.EaIf_StateMachine = EA_IF_STATE_IDLE;
}

/**********************************************************************************************************************
 EaIf_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_ProcessIdleState(void)
{
  /* #10 Prepare instance object to start reading its Management Bytes. First instance starts at DatasetStartAddress. */
  EaIh_CreateInstance(EaIf_Instance, EaDh_GetDatasetStartAddress(), EaDh_GetDataLength(), 0x00); /* SBSW_EA_43 */
  EaIf_InstancePrev = *EaIf_Instance;

  /* #20 Initiate InstanceFinder job */
  if (EaRdMgmt_StartJob(EaIf_Instance) == E_OK) /* SBSW_EA_43 */
  {
    EaIf_ComponentParameter.EaIf_StateMachine = EA_IF_STATE_WAIT_FOR_READ;
  }
  else
  {
    EaIf_FinishJob(MEMIF_JOB_FAILED);
  }
}

/**********************************************************************************************************************
 EaIf_ProcessExamineInstanceState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_ProcessExamineInstanceState(void)
{
  /* #10 Set status and cycle counter of instance based on management bytes */
  EaIh_SetInstanceProperties(EaIf_Instance); /* SBSW_EA_43 */

  /* #15 Compare EaIf_Instance to previous instance and evaluate if search is finished */
  if (EaIf_IsSearchFinished(EaIf_Instance, &EaIf_InstancePrev) == TRUE) /* SBSW_EA_45 */
  {
    /* #20 Search is finished */
    EaIf_FinishJob(MEMIF_JOB_OK);
  }
  else
  {
    /* #25 At this point EaIf_InstancePrev and EaIf_Instance have same age. Parameters of next instance in dataset have */
    /* #26 to be loaded to EaIf_Instance */
    if (EaDh_IsLastInstance(EaIf_Instance) == TRUE) /* SBSW_EA_46 */
    {
      /* #30 EaIf_Instance is last instance in dataset, thus it is returned */
      EaIf_FinishJob(MEMIF_JOB_OK);
    }
    else
    {
      EaDh_StoreNextInstanceAddress(EaIf_Instance); /* SBSW_EA_43 */
      /* #35 No wrap around occurred, thus next instance has to be read */
      if (EaRdMgmt_StartJob(EaIf_Instance) == E_OK) /* SBSW_EA_43 */
      {
        EaIf_ComponentParameter.EaIf_StateMachine = EA_IF_STATE_WAIT_FOR_READ;
      }
      else
      {
        EaIf_FinishJob(MEMIF_JOB_FAILED);
      }
    }
  }
}

/**********************************************************************************************************************
 EaIf_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIf_ProcessStateMachine(void)
{
  switch (EaIf_ComponentParameter.EaIf_StateMachine)
  {
    /*** -------------- IDLE -------------------------------------------------- ***/
    case EA_IF_STATE_IDLE:

      /* #10 First instance of dataset will be read */
      EaIf_ProcessIdleState();

      break;
    /*** -------------- WAIT FOR READ ------------------------------------------ ***/
    case EA_IF_STATE_WAIT_FOR_READ:
      if (EaRdMgmt_GetStatus() == MEMIF_IDLE)
      {
        /* #20 Reading requested instance is finished */
        if (EaRdMgmt_GetJobResult() == MEMIF_JOB_OK)
        {
          EaIf_ComponentParameter.EaIf_StateMachine = EA_IF_STATE_EXAMINE_INSTANCE;
        }
        else
        {
          EaIf_FinishJob(EaRdMgmt_GetJobResult());
        }
      }
      break;
    /*** -------------- EXAMINE INSTANCE ---------------------------------------- ***/
    case EA_IF_STATE_EXAMINE_INSTANCE:

      /* #30 Management bytes of recently read instance will be evaluated */
      EaIf_ProcessExamineInstanceState();
      break;

    default:
      EaIf_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaIf_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaIf_Init(void)
{
  EaIf_ComponentParameter.EaIf_Status = MEMIF_IDLE;
  EaIf_ComponentParameter.EaIf_JobResult = MEMIF_JOB_OK;

  EaIh_InitInstance(&EaIf_InstancePrev); /* SBSW_EA_44 */

  EaIf_ComponentParameter.EaIf_StateMachine = EA_IF_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaIf_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaIf_GetStatus(void)
{
  return EaIf_ComponentParameter.EaIf_Status;
}

/**********************************************************************************************************************
 *  EaIf_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaIf_GetJobResult(void)
{
  return EaIf_ComponentParameter.EaIf_JobResult;
}

/**********************************************************************************************************************
 *  EaIf_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaIf_StartJob(EaIh_InstanceVarPointerType Instance)
{
  Std_ReturnType retVal;

  /* #10 Requested job is only accepted if sub-component's state machine is idle */
  if (EaIf_ComponentParameter.EaIf_StateMachine == EA_IF_STATE_IDLE)
  {
    /* #20 InstanceFinder sub-component's execution and cancel service are registered to TaskManager */
    retVal = EaTm_AddTask(EaIf_Execute, EaIf_Cancel, EA_INSTANCE_FINDER_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaIf_ComponentParameter.EaIf_Status = MEMIF_BUSY;
      EaIf_ComponentParameter.EaIf_JobResult = MEMIF_JOB_PENDING;

      EaIf_Instance = Instance;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaIf_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaIf_Execute(void)
{
  /* #10 InstanceFinder's state machine is only processed if sub-component's status is busy */
  if (EaIf_ComponentParameter.EaIf_Status == MEMIF_BUSY)
  {
    EaIf_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaIf_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaIf_Cancel(void)
{
  EaIf_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_41
      \DESCRIPTION Content of object of type EaIh_InstanceType referenced by PreviousInstance is copied to
                   object of type EaIh_InstanceType referenced by parameter Instance.
      \COUNTERMEASURE \N The compiler performs type check and therefore assures that write access to object of
                         referenced parameter Instance is valid.
                         PreviousInstance is pointing to content of local variable EaIf_InstancePrev which
                         is initialized upon sub-component initialization and is written with valid data in idle state
                         of the sub-component. Thus, PreviousInstance is always a valid pointer
                         once EaIf_IsSearchFinished function is called.
    \ID SBSW_EA_42
      \DESCRIPTION Content of object of type EaIh_InstanceType referenced by Instance is copied to
                   object of type EaIh_InstanceType referenced by parameter PreviousInstance.
      \COUNTERMEASURE \N The compiler performs type check and therefore assures that write access to object of
                         referenced parameter PreviousInstance is valid.
                         Instance which is a copy of local pointer EaIf_Instance is always a valid pointer because
                         EaIf_Instance is initialized with a valid reference of an instance object upon job start of
                         Ea_Layer2_InstanceFinder sub-component.
                         The layer 1 sub-components Invalidate, Read and Write contain a local instance object each,
                         whose address is passed to Ea_Layer2_InstanceFinder upon job start.
                         InstanceFinder's instance pointer is initialized with this reference.
    \ID SBSW_EA_43
      \DESCRIPTION Function is called with reference to an instance object EaIf_Instance
      \COUNTERMEASURE \N Local pointer to parameter EaIf_Instance is initialized upon call of EaIf_StartJob service.
                         Caller of Ea_Layer2_InstanceFinder sub-component ensures that pointer passed to the
                         sub-component is valid.
    \ID SBSW_EA_44
      \DESCRIPTION Function is called with reference to local parameter EaIf_InstancePrev
      \COUNTERMEASURE \N EaIf_InstancePrev is a local parameter, thus references to this parameter are valid.
    \ID SBSW_EA_45
      \DESCRIPTION Function is called with both a reference to the local parameter EaIf_InstancePrev and a reference
                   to an instance object EaIf_Instance
      \COUNTERMEASURE \N EaIf_InstancePrev is a local parameter, thus references to this parameter are valid.
                         Local pointer to parameter EaIf_Instance is initialized upon call of EaIf_StartJob service.
                         Caller of Ea_Layer2_InstanceFinder sub-component ensures that pointer passed to the
                         sub-component is valid.
    \ID SBSW_EA_46
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceConstPointerType or
                   EaIh_InstanceVarPointerType
      \COUNTERMEASURE \N Instance parameter is passed to function via input parameter, thus caller ensures
                         that parameter referenced by pointer is valid.
SBSW_JUSTIFICATION_END */

