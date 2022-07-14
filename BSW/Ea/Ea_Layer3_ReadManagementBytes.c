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
 *        \file  Ea_Layer3_ReadManagementBytes.c
 *        \brief  ReadManagementBytes is responsible for reading both header and trailer information of passed instance.
 *
 *      \details  ReadManagementBytes is responsible for reading both header and trailer information of passed instance.
 *                It distinguishes between two ways of reading. Either the passed instance is very small and both header
 *                and trailer can be read at once, or EEP is accessed two times for reading header and trailer separately.
 *                Read values are stored in passed instance.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer3_ReadManagementBytes.h"

#include "Ea_TaskManager.h"
#include "Ea_EepCoordinator.h"

#include "Ea_PartitionHandler.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define EA_READ_MANAGEMENT_BYTES_LAYER EA_LAYER_THREE_INDEX

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

typedef enum
{
  EA_RDMGMT_STATE_UNINIT = 0,
  EA_RDMGMT_STATE_IDLE,
  EA_RDMGMT_STATE_WAIT_FOR_HEADER,
  EA_RDMGMT_STATE_WAIT_FOR_TRAILER,
  EA_RDMGMT_STATE_WAIT_FOR_ENTIRE
} EEaRdMgmtStateType;

typedef struct
{
  MemIf_JobResultType EaRdMgmt_JobResult;
  MemIf_StatusType EaRdMgmt_Status;
  EEaRdMgmtStateType EaRdMgmt_StateMachine;
} EaRdMgmt_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaRdMgmt_ComponentParameterType, EA_APPL_DATA) EaRdMgmt_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_RDMGMT_STATE_UNINIT };

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL P2VAR(EaIh_InstanceType, AUTOMATIC, EA_APPL_DATA) EaRdMgmt_Instance;

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaRdMgmt_FinishJob()
 *********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. ReadManagementBytes sub-component is de-registered from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
 EaRdMgmt_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes idle state of ReadManagementBytes sub-component
 * \details    Processes idle state of ReadManagementBytes sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessIdleState(void);

/**********************************************************************************************************************
 EaRdMgmt_ProcessWaitForHeaderState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes WaitForHeader state of ReadManagementBytes sub-component
 * \details    Processes WaitForHeader state of ReadManagementBytes sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessWaitForHeaderState(void);

/**********************************************************************************************************************
 EaRdMgmt_ProcessWaitForTrailerState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes WaitForTrailer state of ReadManagementBytes sub-component
 * \details    Processes WaitForTrailer state of ReadManagementBytes sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessWaitForTrailerState(void);

/**********************************************************************************************************************
 EaRdMgmt_ProcessWaitForEntireState()
 **********************************************************************************************************************/
/*!
 * \brief      Processes WaitForEntire state of ReadManagementBytes sub-component
 * \details    Processes WaitForEntire state of ReadManagementBytes sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessWaitForEntireState(void);

/**********************************************************************************************************************
 EaRdMgmt_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * \brief      Processes state machine of ReadManagementBytes sub-component
 * \details    Processes state machine of ReadManagementBytes sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaRdMgmt_FinishJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 ReadManagementByte sub-component is de-registered from TaskManager */
  EaTm_RemoveTask(EaRdMgmt_Execute, EaRdMgmt_Cancel, EA_READ_MANAGEMENT_BYTES_LAYER); /* SBSW_EA_15 */

  /* #20 Reset internal parameters */
  EaRdMgmt_ComponentParameter.EaRdMgmt_JobResult = JobResult;
  EaRdMgmt_ComponentParameter.EaRdMgmt_Status = MEMIF_IDLE;
  EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine = EA_RDMGMT_STATE_IDLE;
}

/**********************************************************************************************************************
 EaRdMgmt_ProcessIdleState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessIdleState(void)
{
  /* #10 It is checked if both header and trailer are read at once or separately */
  if (EaIh_GetInstanceLength(EaRdMgmt_Instance) > EaPh_GetPartitionReadAlignment()) /* SBSW_EA_51 */
  {
    /* #20 Header and Trailer have to be read separately */
    if (EaIh_ReadHeader(EaRdMgmt_Instance) == E_OK) /* SBSW_EA_51 */
    {
      EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine = EA_RDMGMT_STATE_WAIT_FOR_HEADER;
    }
    else
    {
      EaRdMgmt_FinishJob(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* #30 Instance is small enough that one read access to EEPROM will get both header and trailer value */
    if (EaIh_ReadHeaderAndTrailer(EaRdMgmt_Instance) == E_OK) /* SBSW_EA_51 */
    {
      EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine = EA_RDMGMT_STATE_WAIT_FOR_ENTIRE;
    }
    else
    {
      EaRdMgmt_FinishJob(MEMIF_JOB_FAILED);
    }
  }
}

/**********************************************************************************************************************
 EaRdMgmt_ProcessWaitForHeaderState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessWaitForHeaderState(void)
{
  if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
  {
    /* #10 Header Byte was read to buffer. If configured, Single Bit Flip Correction has to be performed to the */
    /* #11 header byte of the instance */
    EaIh_StoreHeaderValueFromBuffer(EaRdMgmt_Instance); /* SBSW_EA_51 */

#   if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
    EaIh_CorrectSingleBitFlips(&(EaRdMgmt_Instance)->HeaderValue); /* SBSW_EA_51 */
#   endif

    /* #20 Initiate reading trailer of instance */
    if (EaIh_ReadTrailer(EaRdMgmt_Instance) == E_OK) /* SBSW_EA_51 */
    {
      EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine = EA_RDMGMT_STATE_WAIT_FOR_TRAILER;
    }
    else
    {
      EaRdMgmt_FinishJob(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    EaRdMgmt_FinishJob(Ea_Eep_GetJobResult());
  }
}

/**********************************************************************************************************************
 EaRdMgmt_ProcessWaitForTrailerState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessWaitForTrailerState(void)
{
  if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
  {
    /* #10 Trailer Byte was read to buffer. If configured, Single Bit Flip Correction has to be performed to the */
    /* #11 trailer byte of the instance */
    EaIh_StoreTrailerValueFromBuffer(EaRdMgmt_Instance); /* SBSW_EA_51 */

#   if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
    EaIh_CorrectSingleBitFlips(&(EaRdMgmt_Instance)->TrailerValue); /* SBSW_EA_51 */
#   endif

    EaRdMgmt_FinishJob(MEMIF_JOB_OK);
  }
  else
  {
    EaRdMgmt_FinishJob(Ea_Eep_GetJobResult());
  }
}

/**********************************************************************************************************************
 EaRdMgmt_ProcessWaitForEntireState()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessWaitForEntireState(void)
{
  if (Ea_Eep_GetJobResult() == MEMIF_JOB_OK)
  {
    /* #10 Header and Trailer Byte was read to buffer and has to be stored in instance object */
    EaIh_StoreHeaderAndTrailerValueFromBuffer(EaRdMgmt_Instance); /* SBSW_EA_51 */

    /* #20 If configured, Single Bit Flip Correction has to be performed to both management bytes of the instance */
#   if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
    EaIh_CorrectSingleBitFlips(&(EaRdMgmt_Instance)->HeaderValue); /* SBSW_EA_51 */
    EaIh_CorrectSingleBitFlips(&(EaRdMgmt_Instance)->TrailerValue); /* SBSW_EA_51 */
#   endif

    EaRdMgmt_FinishJob(MEMIF_JOB_OK);
  }
  else
  {
    EaRdMgmt_FinishJob(Ea_Eep_GetJobResult());
  }
}

/**********************************************************************************************************************
 EaRdMgmt_ProcessStateMachine()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaRdMgmt_ProcessStateMachine(void)
{
  switch (EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine)
  {
    /*** -------------- IDLE -------------------------------------------------- ***/
    case EA_RDMGMT_STATE_IDLE:

      EaRdMgmt_ProcessIdleState();
      break;

    /*** -------------- WAIT FOR HEADER -------------------------------------------------- ***/
    case EA_RDMGMT_STATE_WAIT_FOR_HEADER:

      EaRdMgmt_ProcessWaitForHeaderState();
      break;

    /*** -------------- WAIT FOR TRAILER -------------------------------------------------- ***/
    case EA_RDMGMT_STATE_WAIT_FOR_TRAILER:

      EaRdMgmt_ProcessWaitForTrailerState();
      break;

    /*** -------------- WAIT FOR ENTIRE -------------------------------------------------- ***/
    case EA_RDMGMT_STATE_WAIT_FOR_ENTIRE:

      EaRdMgmt_ProcessWaitForEntireState();
      break;

    default:
      EaRdMgmt_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaRdMgmt_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaRdMgmt_Init(void)
{
  EaRdMgmt_ComponentParameter.EaRdMgmt_Status = MEMIF_IDLE;
  EaRdMgmt_ComponentParameter.EaRdMgmt_JobResult = MEMIF_JOB_OK;

  EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine = EA_RDMGMT_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaRdMgmt_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaRdMgmt_GetStatus(void)
{
  return EaRdMgmt_ComponentParameter.EaRdMgmt_Status;
}

/**********************************************************************************************************************
 *  EaRdMgmt_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaRdMgmt_GetJobResult(void)
{
  return EaRdMgmt_ComponentParameter.EaRdMgmt_JobResult;
}

/**********************************************************************************************************************
 *  EaRdMgmt_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaRdMgmt_StartJob(EaIh_InstanceVarPointerType Instance)
{
  Std_ReturnType retVal;

  /* #10 Requested job is only accepted if sub-component's state machine is idle */
  if (EaRdMgmt_ComponentParameter.EaRdMgmt_StateMachine == EA_RDMGMT_STATE_IDLE)
  {
    /* #20 ReadManagementByte sub-component registers its execution and cancel service to TaskManager */
    retVal = EaTm_AddTask(EaRdMgmt_Execute, EaRdMgmt_Cancel, EA_READ_MANAGEMENT_BYTES_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaRdMgmt_ComponentParameter.EaRdMgmt_Status = MEMIF_BUSY;
      EaRdMgmt_ComponentParameter.EaRdMgmt_JobResult = MEMIF_JOB_PENDING;

      EaRdMgmt_Instance = Instance;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaRdMgmt_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaRdMgmt_Execute(void)
{
  /* #10 ReadManagementByte sub-component's state machine is only processed if status is busy */
  if (EaRdMgmt_ComponentParameter.EaRdMgmt_Status == MEMIF_BUSY)
  {
    EaRdMgmt_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaRdMgmt_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaRdMgmt_Cancel(void)
{
  EaRdMgmt_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN

   \ID SBSW_EA_51
    \DESCRIPTION Local pointer to an instance object of type EaIh_InstanceType is accessed.
    \COUNTERMEASURE \N Local pointer is initialized with reference to an instance object upon invoking Ea_Layer3_ReadManagementBytes job. Reference
                       is assigned to local pointer in EaRdMgmt_StartJob() service. Thus caller ensures that the pointer passed to this sub-component
                       is valid.

SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer3_ReadManagementBytes.c
 *********************************************************************************************************************/
