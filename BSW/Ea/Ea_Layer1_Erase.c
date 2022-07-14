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
 *        \file  Ea_Layer1_Erase.c
 *        \brief  EaErase sub-component is responsible for erasing immediate data in EEPROM
 *
 *      \details  EaErase sub-component is responsible for erasing entire data area (Dataset) in EEPROM which belongs
 *                to passed BlockNumber. Internally, EEP driver's erase service is used to perform the job.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_Layer1_Erase.h"

#include "Ea_TaskManager.h"
#include "Ea_EepCoordinator.h"

#include "Ea_PartitionHandler.h"
#include "Ea_DatasetHandler.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EA_ERASE_LAYER EA_LAYER_ONE_INDEX

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

typedef enum
{
  EA_ER_STATE_UNINIT = 0,
  EA_ER_STATE_IDLE,
  EA_ER_STATE_WAIT_FOR_ERASE
}EEaErStateType;

typedef struct
{
  MemIf_JobResultType EaEr_JobResult;
  MemIf_StatusType EaEr_Status;
  EEaErStateType EaEr_StateMachine;
}EaEr_ComponentParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define EA_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(EaEr_ComponentParameterType, EA_APPL_DATA) EaEr_ComponentParameter =
    {MEMIF_JOB_FAILED, MEMIF_UNINIT, EA_ER_STATE_UNINIT};

#define EA_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  EaEr_FinishJob()
**********************************************************************************************************************/
/*!
 * \brief      Finishes currently processed job
 * \details    State and status are reset. EaErase sub-component de-registers from TaskManager.
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PUBLIC_CODE) EaEr_FinishJob(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
  EaEr_ProcessStateMachine()
**********************************************************************************************************************/
/*!
 * \brief      Processes state machine of EaErase sub-component
 * \details    Processes state machine of EaErase sub-component
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaEr_ProcessStateMachine(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaEr_FinishJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PUBLIC_CODE) EaEr_FinishJob(MemIf_JobResultType JobResult)
{
  /* #10 EaErase sub-component de-registers itself from TaskManager */
  EaTm_RemoveTask(EaEr_Execute, EaEr_Cancel, EA_ERASE_LAYER); /* SBSW_EA_15 */

  EaEr_ComponentParameter.EaEr_JobResult = JobResult;
  EaEr_ComponentParameter.EaEr_Status = MEMIF_IDLE;
  EaEr_ComponentParameter.EaEr_StateMachine = EA_ER_STATE_IDLE;
}

/**********************************************************************************************************************
  EaEr_ProcessStateMachine()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaEr_ProcessStateMachine(void)
{
  switch (EaEr_ComponentParameter.EaEr_StateMachine)
  {
    /*** -------------- IDLE ------------------------------------------------------------ ***/
    case EA_ER_STATE_IDLE:
      /* #10 Erasing entire dataset is requested */
      if (EaDh_EraseDataset() == E_OK)
      {
        EaEr_ComponentParameter.EaEr_StateMachine = EA_ER_STATE_WAIT_FOR_ERASE;
      }
      else
      {
        EaEr_FinishJob(MEMIF_JOB_FAILED);
      }
      break;
    /*** -------------- WAIT FOR ERASE -------------------------------------------------- ***/
    case EA_ER_STATE_WAIT_FOR_ERASE:
      /* #20 EEPROM driver finished */
      EaEr_FinishJob(Ea_Eep_GetJobResult());
      break;

    default:
      EaEr_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaEr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaEr_Init(void)
{
  EaEr_ComponentParameter.EaEr_Status = MEMIF_IDLE;
  EaEr_ComponentParameter.EaEr_JobResult = MEMIF_JOB_OK;

  EaEr_ComponentParameter.EaEr_StateMachine = EA_ER_STATE_IDLE;
}

/**********************************************************************************************************************
 *  EaEr_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EA_PUBLIC_CODE) EaEr_GetStatus(void)
{
  return EaEr_ComponentParameter.EaEr_Status;
}

/**********************************************************************************************************************
 *  EaEr_GetJobResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) EaEr_GetJobResult(void)
{
  return EaEr_ComponentParameter.EaEr_JobResult;
}

/**********************************************************************************************************************
 *  EaEr_StartJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, EA_PUBLIC_CODE) EaEr_StartJob(void)
{
  Std_ReturnType retVal;

  /* #10 Job request is only accepted if state machine is idle */
  if (EaEr_ComponentParameter.EaEr_StateMachine == EA_ER_STATE_IDLE)
  {
    /* #20 Erase sub-component registers its execution and cancel services to TaskManager */
    retVal = EaTm_AddTask(EaEr_Execute, EaEr_Cancel, EA_ERASE_LAYER); /* SBSW_EA_14 */

    if (retVal == E_OK)
    {
      EaEr_ComponentParameter.EaEr_Status = MEMIF_BUSY;
      EaEr_ComponentParameter.EaEr_JobResult = MEMIF_JOB_PENDING;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaEr_Execute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaEr_Execute(void)
{
  /* #10 Erase sub-component's execution service is only processed if status is busy */
  if (EaEr_ComponentParameter.EaEr_Status == MEMIF_BUSY)
  {
    EaEr_ProcessStateMachine();
  }
}

/**********************************************************************************************************************
 *  EaEr_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PUBLIC_CODE) EaEr_Cancel(void)
{
  EaEr_FinishJob(MEMIF_JOB_CANCELED);
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Ea_Layer1_Erase.c
 *********************************************************************************************************************/
