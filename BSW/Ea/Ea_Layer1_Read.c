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
 *        \file  Ea_Layer1_Read.c
 *        \brief  This sub-component of EA module is processing read jobs
 *
 *      \details  EaRead sub-component is responsible for processing read jobs. Upon job request it registers itself
 *                to TaskManager and after finishing the job, it de-registers itself from TaskManager. To process the
 *                internal state machine, sub-component InstanceFinder is used to find most recent data.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer1_Read.h"
#include "Ea_Layer2_InstanceFinder.h"

#include "Ea_TaskManager.h"
#include "Ea_EepCoordinator.h"

#include "Ea_PartitionHandler.h"
#include "Ea_InstanceHandler.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define EA_READ_LAYER EA_LAYER_ONE_INDEX

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

typedef enum
{
    EA_RD_STATE_UNINIT = 0,
    EA_RD_STATE_IDLE,
    EA_RD_STATE_WAIT_FOR_INSTANCEFINDER,
    EA_RD_STATE_READ_DATA
}EEaRdStateType;

typedef struct
{
    MemIf_JobResultType EaRd_JobResult ;
    MemIf_StatusType EaRd_Status;
    EEaRdStateType EaRd_StateMachine;
}EaRd_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaRd_ComponentParameterType, EA_APPL_DATA) EaRd_ComponentParameter =
    {MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_RD_STATE_UNINIT};

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaIh_InstanceType, EA_APPL_DATA) EaRd_Instance;

EA_LOCAL VAR(Ea_UserJobParameterType, EA_APPL_DATA) EaRd_UserJobParameter;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
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
  EaRd_FinishJob()
**********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. EaRead sub-component de-registers itself from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PUBLIC_CODE) EaRd_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
  EaRd_ProcessStateMachine()
**********************************************************************************************************************/
/*!
 * \brief      Processes state machine of EaRead sub-component
 * \details    Processes state machine of EaRead sub-component, only if status is busy
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRd_ProcessStateMachine(void);

/**********************************************************************************************************************
  EaRd_ProcessInstanceFinderState()
**********************************************************************************************************************/
/*!
 * \brief      Processes distinct state of EaRead sub-component
 * \details    Processes state EA_RD_STATE_WAIT_FOR_INSTANCEFINDER
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRd_ProcessInstanceFinderState(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaRd_FinishJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PUBLIC_CODE) EaRd_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 EaRead sub-component de-registers itself from TaskManager */
  EaTm_RemoveTask(EaRd_Execute, EaRd_Cancel, EA_READ_LAYER); /* SBSW_EA_15 */

  EaRd_ComponentParameter.EaRd_JobResult = JobResult;
  EaRd_ComponentParameter.EaRd_Status = MEMIF_IDLE;
  EaRd_ComponentParameter.EaRd_StateMachine = EA_RD_STATE_IDLE;
}

/**********************************************************************************************************************
  EaRd_ProcessStateMachine()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRd_ProcessStateMachine(void)
{
  switch (EaRd_ComponentParameter.EaRd_StateMachine)
  {
    /*** -------------- IDLE -------------------------------------------------- ***/
    case EA_RD_STATE_IDLE:
      /* #10 Initiate InstanceFinder job */
      if (EaIf_StartJob(&EaRd_Instance) == E_OK) /* SBSW_EA_91 */
      {
        EaRd_ComponentParameter.EaRd_StateMachine = EA_RD_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else
      {
        EaRd_FinishJob(MEMIF_JOB_FAILED);
      }
      break;
      /*** -------------- WAIT FOR INSTANCE FINDER ----------------------------- ***/
    case EA_RD_STATE_WAIT_FOR_INSTANCEFINDER:
      /* #20 Result of InstanceFinder is awaited and processed */
      EaRd_ProcessInstanceFinderState();
      break;
      /*** -------------- READING DATA ----------------------------------------- ***/
    case EA_RD_STATE_READ_DATA:
      /* #30 EEPROM driver finished */
      EaRd_FinishJob(Ea_Eep_GetJobResult());
      break;

    default:
      EaRd_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

/**********************************************************************************************************************
  EaRd_ProcessInstanceFinderState()
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
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRd_ProcessInstanceFinderState(void)
{
  if (EaIf_GetStatus() == MEMIF_IDLE)
  {
    /* #10 InstanceFinder is finished */
    if (EaIf_GetJobResult() == MEMIF_JOB_OK)
    {
      /* #20 Check if found instance is valid for reading */
      if (EaRd_Instance.InstanceStatus == INSTANCE_STATUS_OK)
      {
        /* #30 Start reading instance data */
        if (EaIh_ReadData(&EaRd_Instance,                        /* SBSW_EA_92 */
                        EaRd_UserJobParameter.DataBufferPtr,
                        EaRd_UserJobParameter.BlockOffset,
                        EaRd_UserJobParameter.Length) == E_OK)
        {
          EaRd_ComponentParameter.EaRd_StateMachine = EA_RD_STATE_READ_DATA;
        }
        else
        {
          EaRd_FinishJob(MEMIF_JOB_FAILED);
        }
      }
      else
      {
        /* #40 Status of found instance is either INVALID or INCONSISTENT */
        EaRd_FinishJob(EaIh_SetNegativeJobResultAccordingToStatus(&EaRd_Instance)); /* SBSW_EA_91 */
      }
    }
    else
    {
      /* #50 InstanceFinder failed, thus InstanceFinder's job result is reported to upper layer */
      EaRd_FinishJob(EaIf_GetJobResult());
    }
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaRd_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaRd_Init(void)
{
  EaRd_ComponentParameter.EaRd_Status = MEMIF_IDLE;
  EaRd_ComponentParameter.EaRd_JobResult = MEMIF_JOB_OK;

  EaIh_InitInstance(&EaRd_Instance); /* SBSW_EA_91 */

  EaRd_ComponentParameter.EaRd_StateMachine = EA_RD_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaRd_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaRd_GetStatus(void)
{
  return EaRd_ComponentParameter.EaRd_Status;
}  

/**********************************************************************************************************************
 *  EaRd_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaRd_GetJobResult(void)
{
  return EaRd_ComponentParameter.EaRd_JobResult;
}

/**********************************************************************************************************************
 *  EaRd_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaRd_StartJob(Ea_UserJobParameterType Ea_UserJobParameter)
{
  Std_ReturnType retVal;

  /* #10 Job request is accepted if EaRead sub-component's state machine is idle */
  if (EaRd_ComponentParameter.EaRd_StateMachine == EA_RD_STATE_IDLE)
  {
    /* #20 EaRead sub-component registers its execution and cancel service to TaskManager */
    retVal = EaTm_AddTask(EaRd_Execute, EaRd_Cancel, EA_READ_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaRd_ComponentParameter.EaRd_Status = MEMIF_BUSY;
      EaRd_ComponentParameter.EaRd_JobResult = MEMIF_JOB_PENDING;

      EaRd_UserJobParameter = Ea_UserJobParameter;
    }

  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaRd_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaRd_Execute(void)
{
  /* #10 State machine is only processed if EaRead sub-component's status is busy */
  if (EaRd_ComponentParameter.EaRd_Status == MEMIF_BUSY)
  {
    EaRd_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaRd_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaRd_Cancel(void)
{
  EaRd_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_91
      \DESCRIPTION Function is called with reference to local parameter EaRd_Instance
      \COUNTERMEASURE \N EaRd_Instance is a local parameter, thus references to this parameter are valid.
    \ID SBSW_EA_92
      \DESCRIPTION Reference to local parameter EaRd_Instance and parameters of EaRd_UserJobParameter structure are passed to function.
      \COUNTERMEASURE \N EaRd_Instance is a local parameter, thus references to this parameter are valid. Data buffer pointer and length/offset parameters
                         of EaRd_UserJobParameter structure are both checked by development error checks upon request of read job. Input/job parameters
                         of Ea_Read API are passed to Ea_Layer1_Read sub-component via call-by-value of EaRd_StartJob service.
SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Read.c
 *********************************************************************************************************************/
