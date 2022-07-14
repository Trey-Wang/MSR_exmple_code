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
/*      \file  Crypto_30_LibCv_Hash.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for HASH Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_HASH_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#if (CRYPTO_30_LIBCV_SHA1 == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchHashSha1()
*********************************************************************************************************************/
/*! \brief         Dispatches the HashSha1 job to the lower layer.
*  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
                   operation mode, it triggers different stages of the service.
*  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
*  \param[in]      mode               Operation mode in which the job currently is.
*  \param[in,out]  job                Pointer to the job which shall be dispatched.
*  \return         E_OK               Dispatching was successful.
*  \return         E_NOT_OK           Dispatching failed.
*  \pre            objectId has to be a valid driver object handle.
*  \context        TASK
*  \reentrant      TRUE
*  \synchronous    TRUE
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHashSha1(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_SHA256 == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchHashSha256()
*********************************************************************************************************************/
/*! \brief         Dispatches the HashSha256 job to the lower layer.
*  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
                   operation mode, it triggers different stages of the service.
*  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
*  \param[in]      mode               Operation mode in which the job currently is.
*  \param[in,out]  job                Pointer to the job which shall be dispatched.
*  \return         E_OK               Dispatching was successful.
*  \return         E_NOT_OK           Dispatching failed.
*  \pre            objectId has to be a valid driver object handle.
*  \context        TASK
*  \reentrant      TRUE
*  \synchronous    TRUE
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHashSha256(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_SHA512 == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchHashSha512()
*********************************************************************************************************************/
/*! \brief         Dispatches the HashSha512 job to the lower layer.
*  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
                   operation mode, it triggers different stages of the service.
*  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
*  \param[in]      mode               Operation mode in which the job currently is.
*  \param[in,out]  job                Pointer to the job which shall be dispatched.
*  \return         E_OK               Dispatching was successful.
*  \return         E_NOT_OK           Dispatching failed.
*  \pre            objectId has to be a valid driver object handle.
*  \context        TASK
*  \reentrant      TRUE
*  \synchronous    TRUE
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHashSha512(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_SHA1 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHashSha1()
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
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHashSha1(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 hash[ESL_SIZEOF_SHA1_DIGEST];
  uint8_least i;

  P2VAR(eslt_WorkSpaceSHA1, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfSHA1(Crypto_30_LibCv_GetSHA1IdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SHA1, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #20 Initialize algorithm */
        retValCv = esl_initSHA1(workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE */
      }

      break;
    }
    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* #45 Process input data. */
        retValCv = esl_updateSHA1(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);
      break;
    }
    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* #75 Write results in output buffers. */
      retValCv = esl_finalizeSHA1(workspace, hash); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */

      /* #80 Adapt output size of hash value. */
      if (*job->jobPrimitiveInputOutput.outputLengthPtr > ESL_SIZEOF_SHA1_DIGEST)
      {
        *job->jobPrimitiveInputOutput.outputLengthPtr = ESL_SIZEOF_SHA1_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      /* Copy data to output location */
      for (i = 0u; i < *job->jobPrimitiveInputOutput.outputLengthPtr; i++)
      {
        job->jobPrimitiveInputOutput.outputPtr[i] = hash[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      break;
    }
    default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (CRYPTO_30_LIBCV_SHA256 == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchHashSha256()
**********************************************************************************************************************/
/*!
*
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
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHashSha256(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 hash[ESL_SIZEOF_SHA256_DIGEST];
  uint8_least i;

  P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfSHA256(Crypto_30_LibCv_GetSHA256IdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SHA256, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #20 Initialize algorithm. */
        retValCv = esl_initSHA256(workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE */
      }
      break;
    }
    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* #45 Process input data. */
      retValCv = esl_updateSHA256(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);

      break;
    }
    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* #75 Write results in output buffers. */
      retValCv = esl_finalizeSHA256(workspace, hash); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */

      /* #80 Adapt output size of hash value. */
      if (*job->jobPrimitiveInputOutput.outputLengthPtr > ESL_SIZEOF_SHA256_DIGEST)
      {
        *job->jobPrimitiveInputOutput.outputLengthPtr = ESL_SIZEOF_SHA256_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      /* Copy data to output location */
      for (i = 0u; i < *job->jobPrimitiveInputOutput.outputLengthPtr; i++)
      {
        job->jobPrimitiveInputOutput.outputPtr[i] = hash[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      break;
    }
    default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (CRYPTO_30_LIBCV_SHA512 == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchHashSha512()
**********************************************************************************************************************/
/*!
*
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
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHashSha512(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 hash[ESL_SIZEOF_SHA512_DIGEST];
  uint8_least i;

  P2VAR(eslt_WorkSpaceSHA512, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfSHA512(Crypto_30_LibCv_GetSHA512IdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SHA512, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #20 Initialize algorithm. */
        retValCv = esl_initSHA512(workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE */
      }
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* #45 Process input data. */
      retValCv = esl_updateSHA512(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);

      break;
    }
    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* #75 Write results in output buffers. */
      retValCv = esl_finalizeSHA512(workspace, hash); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */

      /* #80 Adapt output size of hash value. */
      if (*job->jobPrimitiveInputOutput.outputLengthPtr > ESL_SIZEOF_SHA512_DIGEST)
      {
        *job->jobPrimitiveInputOutput.outputLengthPtr = ESL_SIZEOF_SHA512_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      /* Copy data to output location */
      for (i = 0u; i < *job->jobPrimitiveInputOutput.outputLengthPtr; i++)
      {
        job->jobPrimitiveInputOutput.outputPtr[i] = hash[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }

      break;
    }
    default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_SERVICE_HASH == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchHash()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHash(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */

  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */

  {
    /* ----- Implementation ------------------------------------------------- */

    /* #5 Dispatch hash generation based on algorithm family. */
    switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
# if (CRYPTO_30_LIBCV_SHA1 == STD_ON)
    case CRYPTO_ALGOFAM_SHA1:
    {

      if (Crypto_30_LibCv_IsSHA1UsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchHashSha1(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
      break;
    }
# endif
# if (CRYPTO_30_LIBCV_SHA256 == STD_ON)
    case CRYPTO_ALGOFAM_SHA2_256:
    {

      if (Crypto_30_LibCv_IsSHA256UsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchHashSha256(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }

      break;
    }
# endif
# if (CRYPTO_30_LIBCV_SHA512 == STD_ON)
    case CRYPTO_ALGOFAM_SHA2_512:
    {

      if (Crypto_30_LibCv_IsSHA512UsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchHashSha512(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }

      break;
    }
# endif
    default:
    {
      errorId = CRYPTO_E_PARAM_HANDLE;
      break;
    }
    }
  }
  /* ----- Development Error Report --------------------------------------- */

# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_LIBCV_MODULE_ID, CRYPTO_30_LIBCV_INSTANCE_ID, CRYPTO_30_LIBCV_SID_PROCESS_JOB, errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif

  return retVal;
}
#endif

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Hash.c
 *********************************************************************************************************************/
