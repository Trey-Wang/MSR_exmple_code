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
 *        \file  Ea_Layer1_Write.c
 *        \brief  This sub-component of EA module is processing write jobs
 *
 *      \details  EaWrite sub-component is responsible for writing data from a DataBufferPtr to EEPROM. Destination in
 *                EEPROM is specified by passed BlockNumber. Internally, InstanceFinder is used to find most recent write
 *                destination and WriteVerifyInstance is used to perform the write job on the determined instance object.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer1_Write.h"
#include "Ea_Layer2_InstanceFinder.h"
#include "Ea_Layer2_WriteInstance.h"

#include "Ea_InstanceHandler.h"
#include "Ea_TaskManager.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define EA_WRITE_LAYER EA_LAYER_ONE_INDEX

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

typedef enum
{
  EA_WR_STATE_UNINIT = 0,
  EA_WR_STATE_IDLE,
  EA_WR_STATE_WAIT_FOR_INSTANCEFINDER,
  EA_WR_STATE_WAIT_FOR_WRITE
}EEaWrStateType;

typedef struct
{
  MemIf_JobResultType EaWr_JobResult;
  MemIf_StatusType EaWr_Status;
  EEaWrStateType EaWr_StateMachine;
}EaWr_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaWr_ComponentParameterType, EA_APPL_DATA) EaWr_ComponentParameter =
    {MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_WR_STATE_UNINIT};

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaIh_InstanceType, EA_APPL_DATA) EaWr_Instance;

EA_LOCAL VAR(Ea_UserJobParameterType, EA_APPL_DATA) EaWr_UserJobParameter;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaWr_ProcessStateMachine()
 *********************************************************************************************************************/
/*!
 * \brief      Processes state machine of EaWrite sub-component
 * \details    Processes state machine of EaWrite sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (void, EA_PRIVATE_CODE) EaWr_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  EaWr_FinishJob()
 *********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. EaWrite sub-component de-registers from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (void, EA_PRIVATE_CODE) EaWr_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaWr_ProcessStateMachine()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
EA_LOCAL FUNC (void, EA_PRIVATE_CODE) EaWr_ProcessStateMachine(void)
{
  switch (EaWr_ComponentParameter.EaWr_StateMachine)
  {
    /*** -------------- IDLE -------------------------------------------------- ***/
    case EA_WR_STATE_IDLE:

      /* #10 New job was requested */
      /* #15 InstanceFinder job is initiated */
      if (EaIf_StartJob(&EaWr_Instance) == E_OK) /* SBSW_EA_81 */
      {
        EaWr_ComponentParameter.EaWr_StateMachine = EA_WR_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else
      {
        EaWr_FinishJob(MEMIF_JOB_FAILED);
      }

      break;
    /*** -------------- WAIT FOR INSTANCE FINDER ------------------------------ ***/
    case EA_WR_STATE_WAIT_FOR_INSTANCEFINDER:
      if (EaIf_GetStatus() == MEMIF_IDLE)
      {
        /* #20 InstanceFinder finished its job */
        if (EaIf_GetJobResult() == MEMIF_JOB_OK)
        {
          /* #30 WriteInstance job is initiated */
          if (EaWrInst_StartJob(EaWr_Instance, EaWr_UserJobParameter) == E_OK) /* SBSW_EA_82 */
          {
            EaWr_ComponentParameter.EaWr_StateMachine = EA_WR_STATE_WAIT_FOR_WRITE;
          }
          else
          {
            EaWr_FinishJob(MEMIF_JOB_FAILED);
          }
        }
        else
        {
          EaWr_FinishJob(EaIf_GetJobResult());
        }
      }
      break;
    /*** -------------- WAIT FOR WRITING --------------------------------------- ***/
    case EA_WR_STATE_WAIT_FOR_WRITE:
      if (EaWrInst_GetStatus() == MEMIF_IDLE)
      {
        /* #40 WriteInstance sub-component finished its job, thus Layer1 EaWrite sub-component is finished */
        EaWr_FinishJob(EaWrInst_GetJobResult());
      }
      break;

    default:
      EaWr_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

/**********************************************************************************************************************
 *  EaWr_FinishJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (void, EA_PRIVATE_CODE) EaWr_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 EaWrite sub-component de-registers itself from TaskManager */
  EaTm_RemoveTask(EaWr_Execute, EaWr_Cancel, EA_WRITE_LAYER); /* SBSW_EA_15 */

  EaWr_ComponentParameter.EaWr_JobResult = JobResult;
  EaWr_ComponentParameter.EaWr_StateMachine = EA_WR_STATE_IDLE;
  EaWr_ComponentParameter.EaWr_Status = MEMIF_IDLE;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaWr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaWr_Init(void)
{
  EaWr_ComponentParameter.EaWr_Status = MEMIF_IDLE;
  EaWr_ComponentParameter.EaWr_JobResult = MEMIF_JOB_OK;

  EaIh_InitInstance(&EaWr_Instance); /* SBSW_EA_81 */

  EaWr_ComponentParameter.EaWr_StateMachine = EA_WR_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaWr_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaWr_GetStatus(void)
{
  return EaWr_ComponentParameter.EaWr_Status;
}

/**********************************************************************************************************************
 *  EaWr_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaWr_GetJobResult(void)
{
  return EaWr_ComponentParameter.EaWr_JobResult;
}

/**********************************************************************************************************************
 *  EaWr_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaWr_StartJob(Ea_UserJobParameterType Ea_UserJobParameter)
{
  Std_ReturnType retVal;

  /* #10 Requested job is only accepted if EaWrite's state machine is idle */
  if (EaWr_ComponentParameter.EaWr_StateMachine == EA_WR_STATE_IDLE)
  {
    /* #20 EaWrite sub-component registers its execution and cancel service to TaskManger */
    retVal = EaTm_AddTask(EaWr_Execute, EaWr_Cancel, EA_WRITE_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaWr_ComponentParameter.EaWr_Status = MEMIF_BUSY;
      EaWr_ComponentParameter.EaWr_JobResult = MEMIF_JOB_PENDING;

      EaWr_UserJobParameter = Ea_UserJobParameter;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}
/**********************************************************************************************************************
 *  EaWr_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaWr_Execute(void)
{
  /* #10 State machine is only processed if sub-component's status is busy */
  if (EaWr_ComponentParameter.EaWr_Status == MEMIF_BUSY)
  {
      EaWr_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaWr_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaWr_Cancel(void)
{
  EaWr_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_81
      \DESCRIPTION Function is called with reference to local parameter EaWr_Instance
      \COUNTERMEASURE \N EaWr_Instance is a local parameter, thus references to this parameter are valid.

    \ID SBSW_EA_82
      \DESCRIPTION Function is called with local structure EaWr_UserJobParameter, which contains a pointer to data buffer.
      \COUNTERMEASURE \N EaWr_UserJobParameter is initialized with input parameter of function EaWr_StartJob, which is invoked upon start
                         of Ea_Layer1_Write sub-component. Thus caller ensures pointer to be valid.
SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Write.c
 *********************************************************************************************************************/
