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
 *        \file  Ea_Layer2_WriteInstance.c
 *        \brief  WriteInstance sub-component is responsible for writing an instance and veryfing the written data.
 *
 *      \details  WriteInstance sub-component is responsible for writing an instance and veryfing the written data,
 *                if configured. WriteInstance will check the instance's status before writing in order to determine
 *                the correct destination for the write job. Verification of written data can be enabled/disabled by
 *                pre-compile switch.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer2_WriteInstance.h"

#include "Ea_TaskManager.h"
#include "Ea_EepCoordinator.h"

#include "Ea_DatasetHandler.h"
#include "Ea_BlockHandler.h"
#include "Ea_PartitionHandler.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define EA_WRITE_INSTANCE_LAYER EA_LAYER_TWO_INDEX

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

typedef enum
{
  EA_WRINST_STATE_UNINIT = 0,
  EA_WRINST_STATE_IDLE,
  EA_WRINST_STATE_WRITE_ENTIRE,
  EA_WRINST_STATE_VERIFY_ENTIRE,
  EA_WRINST_STATE_WRITE_HEADER,
  EA_WRINST_STATE_VERIFY_HEADER,
  EA_WRINST_STATE_WRITE_MIDDLE_PART,
  EA_WRINST_STATE_VERIFY_MIDDLE_PART,
  EA_WRINST_STATE_WRITE_TRAILER,
  EA_WRINST_STATE_VERIFY_TRAILER
} EEaWrInstStateType;

typedef struct
{
  MemIf_JobResultType EaWrInst_JobResult;
  MemIf_StatusType EaWrInst_Status;
  EEaWrInstStateType EaWrInst_StateMachine;
} EaWrInst_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaWrInst_ComponentParameterType, EA_APPL_DATA) EaWrInst_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_WRINST_STATE_UNINIT };

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaIh_CompareParameterType, EA_APPL_DATA) EaWrInst_CompareParameter;

EA_LOCAL VAR(EaIh_InstanceType, EA_APPL_DATA) EaWrInst_Instance;

EA_LOCAL VAR(Ea_UserJobParameterType, EA_APPL_DATA) EaWrInst_UserJobParameter;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaWrInst_FinishJob()
 *********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. WriteInstance sub-component is de-registered from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
 *  EaWrInst_PrepareInstanceForWriting()
 *********************************************************************************************************************/
/*!
 * \brief      Prepares instance for writing
 * \details    Checks status of instance and calculates next instance if necessary. If instance status is either OK or
 *             INVALID it is necessary to get the next instance, because current instance must not be overwritten.
 * \param[in]  Instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_PrepareInstanceForWriting(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaWrInst_InitiateVerifyState()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates verification of recently written data
 * \details    Initiates verification of recently written data and switches to passed state
 * \param[in]  nextState
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \trace DSGN-Ea22975
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_InitiateVerifyState(EEaWrInstStateType nextState);

/**********************************************************************************************************************
 *  EaWrInst_InitiateNextWriteState()
 *********************************************************************************************************************/
/*!
 * \brief      Determines and initiates next write state
 * \details    Depending on instance length, either trailer part is written in the following or
 *             the middle part of the instance.
 *             This service checks length of instance and then initiates the appropriate following write state.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_InitiateNextWriteState(void);

/**********************************************************************************************************************
 *  EaWrInst_InitiateTrailerWriteState()
 *********************************************************************************************************************/
/*!
 * \brief      Initiates trailer write state
 * \details    To avoid duplicate code this service is used to initiate trailer write state. There are several states
 *             which use this service.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_InitiateTrailerWriteState(void);

/**********************************************************************************************************************
 EaWrInst_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes idle state of WriteInstance state machine
 * \details    Processes idle state of WriteInstance state machine
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateIdle(void);

/**********************************************************************************************************************
 EaWrInst_ProcessStateWriteHeader()
 **********************************************************************************************************************/
/*!
 * \brief      Processes header write state of WriteInstance state machine
 * \details    Processes header write state of WriteInstance state machine
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateWriteHeader(void);

/**********************************************************************************************************************
 EaWrInst_ProcessStateWriteMiddlePart()
 **********************************************************************************************************************/
/*!
 * \brief      Processes middle part write state of WriteInstance state machine
 * \details    Processes middle part write state of WriteInstance state machine
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateWriteMiddlePart(void);

/**********************************************************************************************************************
 EaWrInst_ProcessStateWriteTrailerAndEntire()
 **********************************************************************************************************************/
/*!
 * \brief      Processes both trailer write state and entire write state of WriteInstance state machine
 * \details    Processes both trailer write state and entire write state of WriteInstance state machine
 * \param[in]  nextState
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateWriteTrailerAndEntire(EEaWrInstStateType nextState);

/**********************************************************************************************************************
 EaWrInst_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * \brief      Processes state machine of WriteInstance sub-component
 * \details    Processes state machine of WriteInstance sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaWrInst_FinishJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 WriteInstance sub-component is de-registered from TaskManager */
  EaTm_RemoveTask(EaWrInst_Execute, EaWrInst_Cancel, EA_WRITE_INSTANCE_LAYER); /* SBSW_EA_15 */

  /* #20 Reset internal parameters */
  EaWrInst_ComponentParameter.EaWrInst_JobResult = JobResult;
  EaWrInst_ComponentParameter.EaWrInst_Status = MEMIF_IDLE;
  EaWrInst_ComponentParameter.EaWrInst_StateMachine = EA_WRINST_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaWrInst_PrepareInstanceForWriting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_PrepareInstanceForWriting(EaIh_InstanceVarPointerType Instance)
{
  if (EaIh_IsStatusOkOrInvalid(Instance) == TRUE) /* SBSW_EA_65 */
  {
    /* #10 If Instance is last instance in dataset, wrap around will occur and therefore cycleCounter has to be incremented */
    if (EaDh_IsLastInstance(Instance) == TRUE) /* SBSW_EA_65 */
    {
      EaIh_IncrementCycleCounter(Instance); /* SBSW_EA_61 */
    }
    /* #20 Calculate address of next instance and store it to instance object */
    EaDh_StoreNextInstanceAddress(Instance); /* SBSW_EA_61 */
  }

  /* #30 Status and Management Bytes of instance have to be updated, according to write job */
  EaIh_SetInstanceStatus(Instance, INSTANCE_STATUS_OK); /* SBSW_EA_61 */
  EaIh_SetManagementValuesAccordingToStatus(Instance); /* SBSW_EA_61 */
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  EaWrInst_InitiateVerifyState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_InitiateVerifyState(EEaWrInstStateType nextState)
{
  if (EaIh_VerifyData(EaWrInst_CompareParameter) == E_OK) /* SBSW_EA_62 */
  {
    EaWrInst_ComponentParameter.EaWrInst_StateMachine = nextState;
  }
  else
  {
    EaWrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

/**********************************************************************************************************************
 *  EaWrInst_InitiateNextWriteState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_InitiateNextWriteState(void)
{
  /* #10 Check whether the instance is long enough to write a middle part */
  if (EaIh_GetInstanceLength(&EaWrInst_Instance) > (2 * EaPh_GetPartitionWriteAlignment())) /* SBSW_EA_64 */
  {
    /* #20 Separate middle part has to be written due to length of instance */
    if (EaIh_WriteMiddlePart(&EaWrInst_Instance, EaWrInst_UserJobParameter, &EaWrInst_CompareParameter) == E_OK) /* SBSW_EA_63 */
    {
      EaWrInst_ComponentParameter.EaWrInst_StateMachine = EA_WRINST_STATE_WRITE_MIDDLE_PART;
    }
    else
    {
      EaWrInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* #30 It's sufficient to write only trailer part since instance is small enough */
    EaWrInst_InitiateTrailerWriteState();
  }
}

/**********************************************************************************************************************
 *  EaWrInst_InitiateTrailerWriteState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_InitiateTrailerWriteState(void)
{
  /* #10 Trailer part is written. Trailer part contains a certain number of data (depending on write alignment) and */
  /* #11 the trailer byte */
  if (EaIh_WriteTrailer(&EaWrInst_Instance, EaWrInst_UserJobParameter, &EaWrInst_CompareParameter) == E_OK) /* SBSW_EA_63 */
  {
    EaWrInst_ComponentParameter.EaWrInst_StateMachine = EA_WRINST_STATE_WRITE_TRAILER;
  }
  else
  {
    EaWrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

/**********************************************************************************************************************
 EaWrInst_ProcessStateIdle()
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
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateIdle(void)
{
  /* #5 Prepare instance to assure correct position of writing */
  EaWrInst_PrepareInstanceForWriting(&EaWrInst_Instance); /* SBSW_EA_64 */

  /* #10 If DataLength plus management size is greater than one write unit (ERASE_UNIT_SIZE), */
  /* #11 data is split into several write accesses */
  if (EaIh_GetInstanceLength(&EaWrInst_Instance) > EaPh_GetPartitionWriteAlignment()) /* SBSW_EA_64 */
  {
    /* #13 Several writings are necessary due to size of instance. First part contains header byte and */
    /* #14 a certain number of data (depending on write alignment) */
    if (EaIh_WriteHeader(&EaWrInst_Instance, EaWrInst_UserJobParameter, &EaWrInst_CompareParameter) == E_OK) /* SBSW_EA_63 */
    {
      EaWrInst_ComponentParameter.EaWrInst_StateMachine = EA_WRINST_STATE_WRITE_HEADER;
    }
    else
    {
      EaWrInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* #15 One step of writing is sufficient for entire data, since instance is small enough */
    if (EaIh_WriteEntireData(&EaWrInst_Instance, EaWrInst_UserJobParameter, &EaWrInst_CompareParameter) == E_OK) /* SBSW_EA_63 */
    {
      EaWrInst_ComponentParameter.EaWrInst_StateMachine = EA_WRINST_STATE_WRITE_ENTIRE;
    }
    else
    {
      EaWrInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
}

/**********************************************************************************************************************
 EaWrInst_ProcessStateWriteHeader()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateWriteHeader(void)
{
  if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
  {
    /* #10 EEPROM finished writing header part successfully. In case verification is enabled, written data is verified. */
    /* #11 Otherwise next part of data is written */
    if (EaBh_HasVerificationEnabled() == TRUE)
    {
      EaWrInst_InitiateVerifyState(EA_WRINST_STATE_VERIFY_HEADER);
    }
    else
    {
      EaWrInst_InitiateNextWriteState();
    }
  }
  else
  {
    EaWrInst_FinishJob(Ea_Eep_GetJobResult());
  }
}

/**********************************************************************************************************************
 EaWrInst_ProcessStateWriteMiddlePart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateWriteMiddlePart(void)
{
  if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
  {
    /* #10 EEPROM finished writing middle part successfully. In case verification is enabled, written data is verified. */
    /* #11 Otherwise trailer part of data is written */
    if (EaBh_HasVerificationEnabled() == TRUE)
    {
      EaWrInst_InitiateVerifyState(EA_WRINST_STATE_VERIFY_MIDDLE_PART);
    }
    else
    {
      EaWrInst_InitiateTrailerWriteState();
    }
  }
  else
  {
    EaWrInst_FinishJob(Ea_Eep_GetJobResult());
  }
}

/**********************************************************************************************************************
 EaWrInst_ProcessStateWriteTrailerAndEntire()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateWriteTrailerAndEntire(EEaWrInstStateType nextState)
{
  if (EaBh_HasVerificationEnabled() == TRUE)
  {
    /* #10 EEPROM finished writing last or entire part successfully. */
    /* #11 In case verification is enabled, written data is verified */
    if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
    {
      EaWrInst_InitiateVerifyState(nextState);
    }
    else
    {
      EaWrInst_FinishJob(Ea_Eep_GetJobResult());
    }
  }
  else
  {
    /* #20 Write Instance sub-component is finished */
    EaWrInst_FinishJob(Ea_Eep_GetJobResult());
  }
}

/**********************************************************************************************************************
 EaWrInst_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaWrInst_ProcessStateMachine(void)
{
  switch (EaWrInst_ComponentParameter.EaWrInst_StateMachine)
  {
    /*** ---------------- IDLE -------------------------------------------------------------------------- ***/
    case EA_WRINST_STATE_IDLE:

      EaWrInst_ProcessStateIdle();

      break;
      /*** -------------- WRITE HEADER PART ------------------------------------------------------------- ***/
    case EA_WRINST_STATE_WRITE_HEADER:
      EaWrInst_ProcessStateWriteHeader();
      break;
      /*** -------------- VERIFY HEADER PART ------------------------------------------------------------ ***/
    case EA_WRINST_STATE_VERIFY_HEADER:
      if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
      {
        EaWrInst_InitiateNextWriteState();
      }
      else
      {
        EaWrInst_FinishJob(Ea_Eep_GetJobResult());
      }
      break;
      /*** -------------- WRITE MIDDLE PART -------------------------------------------------------------- ***/
    case EA_WRINST_STATE_WRITE_MIDDLE_PART:
      EaWrInst_ProcessStateWriteMiddlePart();
      break;
      /*** -------------- VERIFY MIDDLE PART ------------------------------------------------------------- ***/
    case EA_WRINST_STATE_VERIFY_MIDDLE_PART:
      if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
      {
        EaWrInst_InitiateTrailerWriteState();
      }
      else
      {
        EaWrInst_FinishJob(Ea_Eep_GetJobResult());
      }
      break;
      /*** -------------- WRITE TRAILER PART -------------------------------------------------------------- ***/
    case EA_WRINST_STATE_WRITE_TRAILER:
      EaWrInst_ProcessStateWriteTrailerAndEntire(EA_WRINST_STATE_VERIFY_TRAILER);
      break;
      /*** -------------- VERIFY TRAILER PART ------------------------------------------------------------- ***/
    case EA_WRINST_STATE_VERIFY_TRAILER:
      EaWrInst_FinishJob(Ea_Eep_GetJobResult());
      break;
      /*** ------------------------------------------------------------------------------------------------ ***/
      /*** -------------- WRITE ENTIRE INSTANCE ----------------------------------------------------------- ***/
      /*** ------------------------------------------------------------------------------------------------ ***/
    case EA_WRINST_STATE_WRITE_ENTIRE:
      EaWrInst_ProcessStateWriteTrailerAndEntire(EA_WRINST_STATE_VERIFY_ENTIRE);
      break;
      /*** ------------------------------------------------------------------------------------------------ ***/
      /*** -------------- VERIFY ENTIRE INSTANCE ---------------------------------------------------------- ***/
      /*** ------------------------------------------------------------------------------------------------ ***/
    case EA_WRINST_STATE_VERIFY_ENTIRE:
      EaWrInst_FinishJob(Ea_Eep_GetJobResult());
      break;

    default:
      EaWrInst_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
} /* PRQA S 6050, 6030 */ /* MD_MSR_STCAL, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaWrInst_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaWrInst_Init(void)
{
  EaWrInst_ComponentParameter.EaWrInst_Status = MEMIF_IDLE;
  EaWrInst_ComponentParameter.EaWrInst_JobResult = MEMIF_JOB_OK;

  EaWrInst_CompareParameter.EaIh_CompareAddress = 0x0000U;
  EaWrInst_CompareParameter.EaIh_CompareBufferPtr = NULL_PTR;
  EaWrInst_CompareParameter.EaIh_CompareLength = 0x0000U;

  EaIh_InitInstance(&EaWrInst_Instance); /* SBSW_EA_64 */

  EaWrInst_ComponentParameter.EaWrInst_StateMachine = EA_WRINST_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaWrInst_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaWrInst_GetStatus(void)
{
  return EaWrInst_ComponentParameter.EaWrInst_Status;
}

/**********************************************************************************************************************
 *  EaWrInst_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaWrInst_GetJobResult(void)
{
  return EaWrInst_ComponentParameter.EaWrInst_JobResult;
}

/**********************************************************************************************************************
 *  EaWrInst_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaWrInst_StartJob(EaIh_InstanceType Instance,
        Ea_UserJobParameterType UserJobParameter)
{
  Std_ReturnType retVal;
  /* #10 Requested job is only accepted if sub-component's state machine is idle */
  if (EaWrInst_ComponentParameter.EaWrInst_StateMachine == EA_WRINST_STATE_IDLE)
  {
    /* #20 WriteInstance registers its execution and cancel service to TaskManager */
    retVal = EaTm_AddTask(EaWrInst_Execute, EaWrInst_Cancel, EA_WRITE_INSTANCE_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaWrInst_ComponentParameter.EaWrInst_Status = MEMIF_BUSY;
      EaWrInst_ComponentParameter.EaWrInst_JobResult = MEMIF_JOB_PENDING;

      EaWrInst_Instance = Instance;
      EaWrInst_UserJobParameter = UserJobParameter;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaWrInst_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaWrInst_Execute(void)
{
  /* #10 State machine is only processed if WriteInstance sub-component's status is busy */
  if (EaWrInst_ComponentParameter.EaWrInst_Status == MEMIF_BUSY)
  {
    EaWrInst_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaWrInst_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaWrInst_Cancel(void)
{
  EaWrInst_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_61
      \DESCRIPTION Function is called with pointer to an instance object
      \COUNTERMEASURE \N Caller ensures that pointer passed to service is valid

    \ID SBSW_EA_62
      \DESCRIPTION Function is called with local structure EaWrInst_CompareParameter, which contains a pointer to data buffer.
      \COUNTERMEASURE \T TCASE-24821
                         Function is always called after valid reference was assigned to pointer parameter. Test cases within TPURP07690 ensure
                         that state machine of Ea_Layer2_WriteInstance works accordingly.

    \ID SBSW_EA_63
      \DESCRIPTION Function is called with local structure EaWrInst_UserJobParameter, which contains a pointer to data buffer. Function is also called
                   with a reference to local instance EaWrInst_Instance and with a local structure EaWrInst_CompareParameter, which contains a pointer
                   to a data buffer.
      \COUNTERMEASURE \N EaWrInst_UserJobParameter is initialized with input parameter of function EaWrInst_StartJob, which is invoked upon start
                         of Ea_Layer2_WriteInstance sub-component. Thus caller ensures pointer to be valid.
                         Reference to local parameter EaWrInst_Instance is always valid.
                         EaWrInst_CompareParameter structure is initialized within called service. Therefore this service assures that content of
                         EaWrInst_CompareParameter is valid.

    \ID SBSW_EA_64
      \DESCRIPTION Function is called with reference to local parameter EaWrInst_Instance
      \COUNTERMEASURE \N EaWrInst_Instance is a local parameter, thus references to this parameter are valid.

    \ID SBSW_EA_65
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceConstPointerType or EaIh_InstanceVarPointerType
      \COUNTERMEASURE \N Instance parameter is passed to function via input parameter, thus caller ensures that parameter referenced
                         by pointer is valid.
SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer2_WriteInstance.c
 *********************************************************************************************************************/
