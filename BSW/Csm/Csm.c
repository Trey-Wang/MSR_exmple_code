/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Csm.c
 *        \brief  MICROSAR Crypto Service Manager (CSM)
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define CSM_SOURCE
/* PRQA S 6060, 0857 EOF */ /* MD_MSR_STPAR, MD_MSR_1.1_857 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm.h"
#include "Csm_Cbk.h"
#include "CryIf.h"
#include "SchM_Csm.h"

#if (CSM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

 /* Check the version of CSM header file */
#if ((CSM_SW_MAJOR_VERSION != (2u)) \
    || (CSM_SW_MINOR_VERSION != (3u)) \
    || (CSM_SW_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of Csm.c and Csm.h are inconsistent"
#endif

/* AUTOSAR version information check has to match definition in header file */
#if ((CSM_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (CSM_AR_RELEASE_MINOR_VERSION != (3u)) \
    || (CSM_AR_RELEASE_REVISION_VERSION != (0u)))
# error "AUTOSAR Specification Version numbers of Csm.c and Csm.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Service idle state */
#define CSM_SERVICE_IDLE      (0u)
/*! Service active state */
#define CSM_SERVICE_ACTIVE    (1u)

/*! Callback is idle and must not be notified to RTE. */
#define CSM_CALLBACK_IDLE     (0xFEu)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CSM_LOCAL) /* COV_CSM_ALWAYS_OFF */
# define CSM_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CSM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
CSM_LOCAL VAR(uint8, CSM_VAR_ZERO_INIT) Csm_IsInitialized = CSM_UNINIT; /* PRQA S 3408 */ /* MD_CSM_DEBUG */ /*lint -e552 */

#define CSM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CSM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CSM_JOB == STD_ON)
/**********************************************************************************************************************
 *  Csm_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Process the received job
 *  \details       This function unifies all external calls to call CryIf_ProcessJob.
 *  \param[in]     queueIdx                Holds the identifier of the queue.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \return        CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJob(uint32 queueIdx, /* PRQA S 3447 */ /* MD_CSM_DEBUG */
                                                        P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Csm_JobSort()
 *********************************************************************************************************************/
/*! \brief         Sort job
 *  \details       This function is sorting given jobs regarding jobPriority.
 *  \param[in]     queueIdx                Holds the identifier of the queue.
 *  \pre           -
 *  \note          Should only be called with locked interrupts
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_JobSort(uint32 queueIdx); /* PRQA S 3447 */ /* MD_CSM_DEBUG */
#endif /* (CSM_JOB == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CSM_JOB == STD_ON)
/**********************************************************************************************************************
 *  Csm_ProcessJob()
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
 *
 *
 */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJob(uint32 queueIdx,
                                                        P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 enqueuingNecessary = FALSE;
  Csm_ChannelIdOfQueueInfoType channelId = Csm_GetChannelIdOfQueueInfo(queueIdx);

  /* #10 When the job is active the unified CryIf processing function is called */
  if (job->state == CRYPTO_JOBSTATE_ACTIVE)
  {
    /* Call CryIf_ProcessJob() */
    retVal = CryIf_ProcessJob(channelId, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  else
  {
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #20 Determine if queue is empty and call CryIf processing function if so */
    if (Csm_GetQueueState(queueIdx).localQueueIndex == Csm_GetQueueStartIdxOfQueueInfo(queueIdx))
    {
      /* Call CryIf_ProcessJob() */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      retVal = CryIf_ProcessJob(channelId, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #25 When return value is either BUSY or QUEUE_FULL enqueuing is mandatory */
      if(job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
      {
        /* If crypto is busy or crypto queue full, further processing is required. */
        enqueuingNecessary = (uint8)(((retVal == CRYPTO_E_BUSY) || (retVal == CRYPTO_E_QUEUE_FULL)) ? TRUE : FALSE);
      }
    }
    /* Queue is not empty */
    /* #30 Synchronous job: the job priority is compared with the highest priority in the queue */
    else if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_SYNC)
    {
      /* #31 Sort queue if jobs where enqueued since last Csm_MainFunction call */
      if (Csm_GetQueueState(queueIdx).sortNeeded == TRUE)
      {
        /* Sort jobs by priority */
        Csm_JobSort(queueIdx);
        Csm_GetQueueState(queueIdx).sortNeeded = FALSE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
      }
      /* #32 Call CryIf processing function if job priority is higher than the highest priority in the queue,
          otherwise return busy */
      if ((job->jobInfo->jobPriority > (Csm_GetQueue(Csm_GetQueueState(queueIdx).localQueueIndex-1)->jobInfo->jobPriority)))
      {
        /* Call CryIf_ProcessJob() */
        Csm_GetQueueState(queueIdx).pauseProcessing = TRUE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        retVal = CryIf_ProcessJob(channelId, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        Csm_GetQueueState(queueIdx).pauseProcessing = FALSE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
      }
      else
      {
        retVal = CRYPTO_E_BUSY;
      }
    }
    /* #40 Asynchronous job must be queued */
    else
    {
      enqueuingNecessary = TRUE;
    }

    /* #50 Enqueuing necessary: Enqueue job if queue is not full, otherwise return that queue is full */
    if(enqueuingNecessary == TRUE)
    {
      if (Csm_GetQueueState(queueIdx).localQueueIndex < (Csm_GetQueueEndIdxOfQueueInfo(queueIdx) - Csm_GetQueueState(queueIdx).reservedIndexes))
      {
        /* Job needs to be enqueued */
        Csm_SetQueue(Csm_GetQueueState(queueIdx).localQueueIndex, job); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        Csm_GetQueueState(queueIdx).sortNeeded = TRUE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        Csm_GetQueueState(queueIdx).localQueueIndex++; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        retVal = E_OK;
      }
      else
      {
        retVal = CRYPTO_E_QUEUE_FULL;
      }
    }
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Csm_JobSort()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CSM_LOCAL FUNC(void, CSM_CODE) Csm_JobSort(uint32 queueIdx)
{
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) tempJob;
  Csm_QueueIterType startIdx = Csm_GetQueueStartIdxOfQueueInfo(queueIdx);
  Csm_QueueIterType i, j;

  for (i = (Csm_QueueIterType)(startIdx + 1u); i < Csm_GetQueueState(queueIdx).localQueueIndex; i++)
  {
    tempJob = Csm_GetQueue(i);
    for (j = i; j > startIdx; j--)
    {
      if (Csm_GetQueue(j - 1u)->jobInfo->jobPriority > (tempJob->jobInfo->jobPriority))
      {
        Csm_SetQueue(j, Csm_GetQueue(j - 1u)); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
      } 
      else
      {
        break;
      }
    }
    Csm_SetQueue(j, tempJob); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
  }
}
#endif /* (CSM_JOB == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Csm_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  Csm_IsInitialized = CSM_UNINIT;
} /* Csm_InitMemory() */

/**********************************************************************************************************************
 *  Csm_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state */
  if (Csm_IsInitialized == (uint8)CSM_INITIALIZED)
  {
    errorId = CSM_E_ALREADY_INITIALIZED;
  }
  else
# endif
  {
# if (CSM_JOB == STD_ON)
    uint32 index;
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize job input/output buffer */
    for (index = 0u; index < Csm_GetSizeOfJob(); index++)
    {
      Csm_GetJob(index).state = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.inputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.inputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryInputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryInputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.tertiaryInputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.tertiaryInputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.outputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.outputLengthPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryOutputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.secondaryOutputLengthPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.verifyPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.output64Ptr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInputOutput.mode = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobPrimitiveInfo = Csm_GetAddrJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(index)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).jobInfo = Csm_GetAddrJobInfo(Csm_GetJobInfoIdxOfJobTable(index)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      Csm_GetJob(index).cryptoKeyId = 0u; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    /* #30 Init queues */
    for (index = 0u; index < Csm_GetSizeOfQueueInfo(); index++)
    {
      Csm_GetQueueState(index).localQueueIndex = Csm_GetQueueStartIdxOfQueueInfo(index); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
      Csm_GetQueueState(index).reservedIndexes = 0u; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
      Csm_GetQueueState(index).sortNeeded = FALSE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
      Csm_GetQueueState(index).pauseProcessing = FALSE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
    }

#  if (CSM_RTECALLBACK == STD_ON)
    Csm_SetRteCallbackOccured(FALSE);
    for (index = 0u; index < Csm_GetSizeOfRteCallback(); index++)
    {
      Csm_SetRteResult(index, CSM_CALLBACK_IDLE); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
    }
#  endif

# endif /* (CSM_JOB == STD_ON) */

    /* #40 Set CSM initialization state to initialized */
    Csm_IsInitialized = CSM_INITIALIZED;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError(CSM_MODULE_ID, CSM_INSTANCE_ID, CSM_INIT_ID, errorId);
  }
#else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* Csm_Init() */

#if (CSM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Csm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (uint16)CSM_VENDOR_ID; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->moduleID = (uint8)CSM_MODULE_ID; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_major_version = (uint8)CSM_SW_MAJOR_VERSION; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_minor_version = (uint8)CSM_SW_MINOR_VERSION; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_patch_version = (uint8)CSM_SW_PATCH_VERSION; /* SBSW_CSM_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)CSM_GETVERSIONINFO_ID, errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif
} /* Csm_GetVersionInfo() */
#endif

/**********************************************************************************************************************
 *  Csm_MainFunction()
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
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_MainFunction(void)
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
  Std_ReturnType retVal;
  Csm_JobIterType queueIdx;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Loop over configured queues */
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    for (queueIdx = 0u; queueIdx < Csm_GetSizeOfQueueInfo(); queueIdx++)
    {
      /* #30 Check if queue has entries */
      if (Csm_GetQueueState(queueIdx).localQueueIndex != Csm_GetQueueStartIdxOfQueueInfo(queueIdx))
      {
        /* #40 Sort jobs if needed */
        if (Csm_GetQueueState(queueIdx).sortNeeded == TRUE)
        {
          /* Sort jobs by priority */
          Csm_JobSort(queueIdx);
          Csm_GetQueueState(queueIdx).sortNeeded = FALSE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        }
        /* #50 Process job if possible (not paused because of active synchronous request): */
        if (Csm_GetQueueState(queueIdx).pauseProcessing != TRUE)
        {
          /* #51 Remove job from queue temporarily (reserve a slot) and pass it to underlying crypto. */
          Csm_GetQueueState(queueIdx).localQueueIndex--; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
          Csm_GetQueueState(queueIdx).reservedIndexes++; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
          job = Csm_GetQueue(Csm_GetQueueState(queueIdx).localQueueIndex);

          SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          retVal = CryIf_ProcessJob(Csm_GetChannelIdOfQueueInfo(queueIdx), job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
          SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          /* #52 If job cannot be processed because of busy driver of full queue, job needs to be enqueued again. */
          if ((retVal == CRYPTO_E_BUSY) || (retVal == CRYPTO_E_QUEUE_FULL)) {
            Csm_SetQueue(Csm_GetQueueState(queueIdx).localQueueIndex, job); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
            Csm_GetQueueState(queueIdx).sortNeeded = TRUE; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
            Csm_GetQueueState(queueIdx).localQueueIndex++; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
          }
          Csm_GetQueueState(queueIdx).reservedIndexes--; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        }
      }
    }
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if (CSM_RTECALLBACK == STD_ON)
    /* Rte Notifications */
    if (Csm_IsRteCallbackOccured())
    {
      Csm_JobIterType callbackIdx;
      Csm_SetRteCallbackOccured(FALSE);
      for (callbackIdx = 0u; callbackIdx < Csm_GetSizeOfRteCallback(); callbackIdx++)
      {
        retVal = Csm_GetRteResult(callbackIdx); 
        if (retVal != CSM_CALLBACK_IDLE)
        {
          Csm_SetRteResult(callbackIdx, CSM_CALLBACK_IDLE); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
          (void)Csm_GetRteCallback(callbackIdx)(retVal);  /* SBSW_CSM_FUNCTION_POINTER */
        }
      }
    }
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MAINFUNCTION_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif
#endif /* (CSM_JOB == STD_ON) */
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */ /* Csm_MainFunction() */

/**********************************************************************************************************************
 *  Csm_CallbackNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_CallbackNotification(P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
                                              Std_ReturnType result) /* PRQA S 3673 */ /* MD_CSM_16.7 */
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (job == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  else if (job->jobInfo->jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* #30 Call user callback function */
# if (CSM_CALLBACK_START_NOTIFICATION == STD_OFF)
    if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)
            && (job->jobPrimitiveInfo->callbackUpdateNotification == TRUE))
        || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)
        || (result == CRYPTO_E_JOB_CANCELED))
# endif
    {
#  if (CSM_CALLBACKFUNC == STD_ON)
      if (Csm_IsCallbackFuncUsedOfJobTable(job->jobInfo->jobId))
      {
        Csm_GetCallbackFunc(Csm_GetCallbackFuncIdxOfJobTable(job->jobInfo->jobId))(job, result); /* SBSW_CSM_API_CALL_FORWARDING_PTR */
      }
      else
#endif
#  if (CSM_RTECALLBACK == STD_ON)
        if (Csm_IsRteCallbackUsedOfJobTable(job->jobInfo->jobId))
        {
          Csm_SetRteResult(Csm_GetRteCallbackIdxOfJobTable(job->jobInfo->jobId), result);  /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
          Csm_SetRteCallbackOccured(TRUE);
        }
        else
#  endif
        {
          errorId = CSM_E_PARAM_METHOD_INVALID;
          /* Notification was called with an invalid job */
          CSM_DUMMY_STATEMENT(result); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
        }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CALLBACKNOTIFICATION_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(job); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif
#endif /* (CSM_JOB == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/* Csm Key Management Functions */
/**********************************************************************************************************************
 *  Csm_KeyElementSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet(uint32 keyId,
                                                 uint32 keyElementId,
                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                 uint32 keyLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (keyPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyElementSet function */
    retVal = CryIf_KeyElementSet(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyPtr, keyLength); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTSET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyElementSet() */

/**********************************************************************************************************************
 *  Csm_KeySetValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValid(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeySetValid function */
    retVal = CryIf_KeySetValid(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYSETVALID_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeySetValid() */

/**********************************************************************************************************************
 *  Csm_KeyElementGet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 4 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet(uint32 keyId,
                                                 uint32 keyElementId,
                                                 P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                 P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if ((keyPtr == NULL_PTR)
         || (keyLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyElementGet function */
    retVal = CryIf_KeyElementGet(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyPtr, keyLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTGET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyElementGet() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy(uint32 keyId,
                                                  uint32 keyElementId,
                                                  uint32 targetKeyId,
                                                  uint32 targetKeyElementId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyElementCopy function */
    retVal = CryIf_KeyElementCopy(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, Csm_GetCryIfKeyIdOfKey(targetKeyId), targetKeyElementId);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTCOPY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyElementCopy() */

/**********************************************************************************************************************
 *  Csm_KeyCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy(uint32 keyId,
                                           uint32 targetKeyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyCopy function */
    retVal = CryIf_KeyCopy(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(targetKeyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYCOPY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyCopy() */

/**********************************************************************************************************************
 *  Csm_RandomSeed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeed(uint32 keyId,
                                              P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                              uint32 seedLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (seedPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_RandomSeed function */
    retVal = CryIf_RandomSeed(Csm_GetCryIfKeyIdOfKey(keyId), seedPtr, seedLength); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RANDOMSEED_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_RandomSeed() */

/**********************************************************************************************************************
 *  Csm_KeyGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyGenerate function */
    retVal = CryIf_KeyGenerate(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyGenerate() */

/**********************************************************************************************************************
 *  Csm_KeyDerive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive(uint32 keyId,
                                             uint32 targetKeyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyDerive function */
    retVal = CryIf_KeyDerive(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(targetKeyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYDERIVE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyDerive() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 3 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal(uint32 keyId,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if ((publicValuePtr == NULL_PTR)
         || (publicValueLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyExchangeCalcPubVal function */
    retVal = CryIf_KeyExchangeCalcPubVal(Csm_GetCryIfKeyIdOfKey(keyId), publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYEXCHANGECALCPUBVAL_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret(uint32 keyId,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                         uint32 partnerPublicValueLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyExchangeCalcSecret function */
    retVal = CryIf_KeyExchangeCalcSecret(Csm_GetCryIfKeyIdOfKey(keyId), partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYEXCHANGECALCSECRET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_KeyExchangeCalcSecret() */

/**********************************************************************************************************************
 *  Csm_CertificateParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_CertificateParse function */
    retVal = CryIf_CertificateParse(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CERTIFICATEPARSE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_CertificateParse() */

/**********************************************************************************************************************
 *  Csm_CertificateVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 3 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify(uint32 keyId,
                                                     uint32 verifyKeyId,
                                                     P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (verifyPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (verifyKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_CertificateVerify function */
    retVal = CryIf_CertificateVerify(Csm_GetCryIfKeyIdOfKey(keyId), verifyKeyId, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CERTIFICATEVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_CertificateVerify() */

/**********************************************************************************************************************
 *  Csm_CancelJob()
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
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJob(uint32 jobId,
                                             Crypto_OperationModeType mode)
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    boolean isJobQueued = FALSE;

    /* #30 Remove asynchronous job from job queue */
    if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
    {
      Csm_QueueIterType idx = Csm_GetQueueStartIdxOfQueueInfo(queueInfoIdx);
      uint8 queueSize;

      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      queueSize = Csm_GetQueueState(queueInfoIdx).localQueueIndex;

      for (; idx < queueSize; idx++)
      {
        if (Csm_GetQueue(idx)->jobInfo->jobId == jobId)
        {
          /* Job is queued */
          isJobQueued = TRUE;
          /* Decrease queue index */
          Csm_GetQueueState(queueInfoIdx).localQueueIndex--; /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        }

        /* Shift remaining jobs to the left */
        if ((isJobQueued == TRUE) && (idx < (queueSize - 1)))
        {
          Csm_SetQueue(idx, Csm_GetQueue(idx + 1)); /* SBSW_CSM_WRITE_TO_COMSTACK_ARRAY */
        }
      }

      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    /* #40 If job was queued, call configured callback notification function */
    if (isJobQueued == TRUE)
    {
      Csm_CallbackNotification(job, CRYPTO_E_JOB_CANCELED); /* SBSW_CSM_API_CALL_FORWARDING_PTR */
      retVal = E_OK;
    }
    else
    {
      /* #50 else cancel job in lower layer */
      retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CANCELJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return retVal;
#else
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* PRQA S 6080, 6030 */ /* MD_MSR_STMIF, MD_MSR_STCYC */ /* Csm_CancelJob() */

/* Csm Service Functions */

/**********************************************************************************************************************
 *  Csm_Hash()
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
/* PRQA S 3673 6 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_Hash(uint32 jobId,
                                        Crypto_OperationModeType mode,
                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                        uint32 dataLength,
                                        P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                        P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
#if (CSM_EXIST_HASH == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((resultPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((resultLengthPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_HASH_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_Hash() */

/**********************************************************************************************************************
 *  Csm_MacGenerate()
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
/* PRQA S 3673 6 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                               uint32 dataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr)
{
#if (CSM_EXIST_MACGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((macPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((macLengthPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = macPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = macLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MACGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(macPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(macLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_MacGenerate() */

/**********************************************************************************************************************
 *  Csm_MacVerify()
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
/* PRQA S 3673 7 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify(uint32 jobId,
                                             Crypto_OperationModeType mode,
                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                             uint32 dataLength,
                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                             uint32 macLength,
                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
#if (CSM_EXIST_MACVERIFY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((macPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((verifyPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputPtr = macPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputLength = macLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MACVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(macPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(macLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_MacVerify() */

/**********************************************************************************************************************
 *  Csm_Encrypt()
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
/* PRQA S 3673 6 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt(uint32 jobId,
                                           Crypto_OperationModeType mode,
                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                           uint32 dataLength,
                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
#if (CSM_EXIST_ENCRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((resultPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)))
         || ((resultLengthPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_ENCRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_Encrypt() */

/**********************************************************************************************************************
 *  Csm_Decrypt()
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
/* PRQA S 3673 6 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt(uint32 jobId,
                                           Crypto_OperationModeType mode,
                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                           uint32 dataLength,
                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
#if (CSM_EXIST_DECRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((resultPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)))
         || ((resultLengthPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_DECRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_Decrypt() */

/**********************************************************************************************************************
 *  Csm_AEADEncrypt()
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
/* PRQA S 3673 10 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                               uint32 plaintextLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                               uint32 associatedDataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)
{
#if (CSM_EXIST_AEADENCRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((plaintextPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((associatedDataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((ciphertextPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)))
         || ((ciphertextLengthPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)))
         || ((tagPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((tagLengthPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = plaintextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = plaintextLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputPtr = associatedDataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = ciphertextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = ciphertextLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryOutputPtr = tagPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryOutputLengthPtr = tagLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_AEADENCRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(plaintextPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(plaintextLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(associatedDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(associatedDataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(ciphertextPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(ciphertextLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(tagPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(tagLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_AEADEncrypt() */

/**********************************************************************************************************************
 *  Csm_AEADDecrypt()
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
/* PRQA S 3673 11 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                               uint32 ciphertextLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                               uint32 associatedDataLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                               uint32 tagLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                               P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
#if (CSM_EXIST_AEADDECRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((ciphertextPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((associatedDataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((tagPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((plaintextPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)))
         || ((plaintextLengthPtr == NULL_PTR) && (((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH) || ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)))
         || ((verifyPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = ciphertextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = ciphertextLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputPtr = associatedDataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.tertiaryInputPtr = tagPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.tertiaryInputLength = tagLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = plaintextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = plaintextLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_AEADDECRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(ciphertextPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(ciphertextLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(associatedDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(associatedDataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(tagPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(tagLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(plaintextPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(plaintextLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_AEADDecrypt() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerate()
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
/* PRQA S 3673 6 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate(uint32 jobId,
                                                     Crypto_OperationModeType mode,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                     uint32 dataLength,
                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
#if (CSM_EXIST_SIGNATUREGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((resultPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((resultLengthPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SIGNATUREGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_SignatureGenerate() */

/**********************************************************************************************************************
 *  Csm_SignatureVerify()
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
/* PRQA S 3673 7 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify(uint32 jobId,
                                                   Crypto_OperationModeType mode,
                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                   uint32 dataLength,
                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                   uint32 signatureLength,
                                                   P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
#if (CSM_EXIST_SIGNATUREVERIFY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if (((dataPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE))
         || ((signaturePtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
         || ((verifyPtr == NULL_PTR) && ((mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputPtr = signaturePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.secondaryInputLength = signatureLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SIGNATUREVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(signaturePtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(signatureLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_SignatureVerify() */

/**********************************************************************************************************************
 *  Csm_RandomGenerate()
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
/* PRQA S 3673 3 */ /* MD_CSM_16.7 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerate(uint32 jobId,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
#if (CSM_EXIST_RANDOMGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Csm_IsInitialized != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  /* #20 Check for null pointers */
  else if ((resultPtr == NULL_PTR)
         || (resultLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check job range */
  else if (jobId >= Csm_GetSizeOfJobOfPCConfig())
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueRefIdxOfJobTable(jobId);
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(jobId);

    job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    /* #40 Dispatch job if possible */
    retVal = Csm_ProcessJob(queueInfoIdx, job); /* SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RANDOMGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
# endif

  return retVal;
#else
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return E_NOT_OK;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* Csm_RandomGenerate() */

#define CSM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Module specific MISRA deviations
 *********************************************************************************************************************/
/* Module specific MISRA deviations:

  MD_CSM_DEBUG:
    Reason:     For testing and debugging reason the CSM_LOCAL can be overwritten to remove the static keyword.
    Risk:       None
    Prevention: Covered by code review.
  MD_CSM_16.7:
    Reason:     Pointer can only be const when method is empty and only Dummy statements are used.
    Risk:       None
    Prevention: Covered by code review.
*/

/* SBSW_JUSTIFICATION_BEGIN
  \ID SBSW_CSM_GETVERSIONINFO
    \DESCRIPTION The function Csm_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter VersionInfoPtr is valid.

  \ID SBSW_CSM_API_CALL_FORWARDING_ID_AND_PTR
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
                    The API is called with a pointer and an id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The Id is a SNV provided by the Csm and is converted to the corresponding ID of lower layer using ComStackLib (CSL01).
                       It is checked when it was first passed to the Csm module.
                       The pointer is either forwarded and assumed to be valid or it is a local stack variable.

  \ID SBSW_CSM_FUNCTION_POINTER
    \DESCRIPTION    The API call is made using a function pointer which is read using ComStackLib macros.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The pointer is forwarded and assumed to be valid.

  \ID SBSW_CSM_API_CALL_FORWARDING_PTR
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
                    The API is called with a pointer.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The pointer is forwarded and assumed to be valid.

  \ID SBSW_CSM_API_CALL_FORWARDING_ID_ONLY
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
                    The API is called with an Id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The Id is a SNV provided by the Csm and is converted to the corresponding ID of lower layer using ComStackLib (CSL01).
                       It is checked when it was first passed to the Csm module.

  \ID SBSW_CSM_WRITE_TO_JOB_POINTER
    \DESCRIPTION    The function does a write access on the job pointer.
    \COUNTERMEASURE \R The pointer is checked when it was first to the service.

  \ID SBSW_CSM_WRITE_TO_COMSTACK_ARRAY
    \DESCRIPTION    The function does a write access to an array generated by ComStackLib
    \COUNTERMEASURE \N The array is written using the passed Id and ComStackLib macros (CSL03). The index never exceeds the size of the array.

SBSW_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN
  \ID COV_CSM_NON_RTE
    \ACCEPT TX
    \ACCEPT XF
    \REASON Ensures the variable or error code define exists if the CSM is used without RTE.
  \ID COV_CSM_NOT_YET_SUPPORTED
    \ACCEPT XF
    \REASON This feature is not yet supported.
  \ID COV_CSM_ALWAYS_OFF
    \ACCEPT XF
    \REASON CSM_LOCAL is always defined externally.
  \ID COV_CSM_CALLBACKCHECK
    \ACCEPT TX
    \REASON Only a memory optimization in case there is no callback defined for any job.
COV_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  END OF FILE: CSM.C
 *********************************************************************************************************************/

