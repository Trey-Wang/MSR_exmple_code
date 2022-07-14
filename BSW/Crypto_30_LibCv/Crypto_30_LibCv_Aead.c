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
/*      \file  Crypto_30_LibCv_Aead.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for AEAD Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_AEAD_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_AESGCM_KEY_LENGTH (32u)
#define CRYPTO_30_LIBCV_AESGCM_IV_LENGTH (16u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchAesGcmEncrypt()
*********************************************************************************************************************/
/*! \brief         Dispatches the AesGcmEncrypt job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAesGcmEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchAesGcmDecrypt()
*********************************************************************************************************************/
/*! \brief         Dispatches the AesGcmDecrypt job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAesGcmDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchAesGcmEncrypt()
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
 *
 *
 *
 *
 *
 *
 *
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAesGcmEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfAesGcmEncrypt(Crypto_30_LibCv_GetAesGcmEncryptIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_GCM, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8 aesGcmIv[CRYPTO_30_LIBCV_AESGCM_IV_LENGTH];
        uint8 aesGcmKey[CRYPTO_30_LIBCV_AESGCM_KEY_LENGTH];
        uint32 aesGcmIvLength = CRYPTO_30_LIBCV_AESGCM_IV_LENGTH;
        uint32 aesGcmKeyLength = CRYPTO_30_LIBCV_AESGCM_KEY_LENGTH;
        Std_ReturnType cipherKeyRetVal, cipherIVRetVal;
        /* #20 Load key element from storage. */
        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        cipherKeyRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_KEY, aesGcmKey, &aesGcmKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        cipherIVRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_IV, aesGcmIv, &aesGcmIvLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        if ((cipherKeyRetVal == E_OK) && (cipherIVRetVal == E_OK))
        {
          /* #25 Initialize algorithm with keys. */
          retValCv = esl_initEncryptGCM(workspace, aesGcmKey, (eslt_Length)aesGcmKeyLength, aesGcmIv, (eslt_Length)aesGcmIvLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
          retVal = CRYPTO_E_KEY_NOT_VALID;
        }
      }

      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* Update GCM Calculation */
      Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      /* #45 Check if output size is sufficient otherwise return error. */
      if (job->jobPrimitiveInputOutput.inputLength > (*job->jobPrimitiveInputOutput.outputLengthPtr))
      {
        retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
        retValCv = ESL_ERC_NO_ERROR;
      }

      *job->jobPrimitiveInputOutput.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

      if ((retValCv == ESL_ERC_NO_ERROR) && (job->jobPrimitiveInputOutput.secondaryInputLength != 0u))
      {
        /* #50 Process authentic data. */
        retValCv = esl_updateAuthDataEncryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          job->jobPrimitiveInputOutput.secondaryInputPtr,
          (eslt_Length)job->jobPrimitiveInputOutput.secondaryInputLength);
      }

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #55 Process plaintext data. */
        retValCv = esl_updatePlaintextEncryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          job->jobPrimitiveInputOutput.inputPtr,
          (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
          job->jobPrimitiveInputOutput.outputPtr,
          job->jobPrimitiveInputOutput.outputLengthPtr);

      }

      /* #60 Store current written length and remaining buffer length for operationmode FINISH. */
      Crypto_30_LibCv_SetWrittenLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* buffer holding Auth Tag */
      uint8 authTagTempBuffer[16] = {0u};
      uint8_least i;

      /* #75 Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
      if ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
      {
        Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
        Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      }

      /* #80 Check if output size is sufficient otherwise return error. */
      if (Crypto_30_LibCv_GetBufferLength(objectId) < 16u)
      {
        retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
        *job->jobPrimitiveInputOutput.outputLengthPtr = Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

        /* #85 Write results in output buffers. */
        retValCv = esl_finalizeEncryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
          &job->jobPrimitiveInputOutput.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)],
          job->jobPrimitiveInputOutput.outputLengthPtr,
          &authTagTempBuffer[0]);

        /* #90 Adapt output size of auth tag. */
        if (*job->jobPrimitiveInputOutput.secondaryOutputLengthPtr > 16u)
        {
          *job->jobPrimitiveInputOutput.secondaryOutputLengthPtr = 16u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
        /* #95 Copy auth tag to output buffer. */
        for (i = 0u; i < *job->jobPrimitiveInputOutput.secondaryOutputLengthPtr; i++)
        {
          job->jobPrimitiveInputOutput.secondaryOutputPtr[i] = authTagTempBuffer[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
      }

      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }
  return retVal;
} /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif

#if (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchAesGcmDecrypt()
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
 *
 *
 *
 *
 *
 *
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAesGcmDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  P2VAR(eslt_WorkSpaceGCM, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfAesGcmDecrypt(Crypto_30_LibCv_GetAesGcmDecryptIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_GCM, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8 aesGcmIv[CRYPTO_30_LIBCV_AESGCM_IV_LENGTH];
        uint8 aesGcmKey[CRYPTO_30_LIBCV_AESGCM_KEY_LENGTH];
        uint32 aesGcmIvLength = CRYPTO_30_LIBCV_AESGCM_IV_LENGTH;
        uint32 aesGcmKeyLength = CRYPTO_30_LIBCV_AESGCM_KEY_LENGTH;
        Std_ReturnType cipherKeyRetVal, cipherIVRetVal;

        /* #25 Initialize algorithm with keys. */
        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        cipherKeyRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_KEY, aesGcmKey, &aesGcmKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        cipherIVRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_IV, aesGcmIv, &aesGcmIvLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        if ((cipherKeyRetVal == E_OK) && (cipherIVRetVal == E_OK))
        {
          /* Init GCM */
          retValCv = esl_initDecryptGCM(workspace, aesGcmKey, (eslt_Length)aesGcmKeyLength, aesGcmIv, (eslt_Length)aesGcmIvLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
          retVal = CRYPTO_E_KEY_NOT_VALID;
        }
      }
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* Update GCM Calculation */
      Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      /* #45 Check if output size is sufficient otherwise return error. */
      if (job->jobPrimitiveInputOutput.inputLength > (*job->jobPrimitiveInputOutput.outputLengthPtr))
      {
        retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
        retValCv = ESL_ERC_NO_ERROR;
      }

      *job->jobPrimitiveInputOutput.outputLengthPtr = 0; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

      /* #50 Process authentic data. */
      if ((retValCv == ESL_ERC_NO_ERROR) && (job->jobPrimitiveInputOutput.secondaryInputLength != 0u))
      {
        retValCv = esl_updateAuthDataDecryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          job->jobPrimitiveInputOutput.secondaryInputPtr,
          (eslt_Length)job->jobPrimitiveInputOutput.secondaryInputLength);
      }

      /* #55 Process plaintext data. */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        retValCv = esl_updateCiphertextDecryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          job->jobPrimitiveInputOutput.inputPtr,
          (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
          job->jobPrimitiveInputOutput.outputPtr,
          job->jobPrimitiveInputOutput.outputLengthPtr);
      }

      /* #60 Store current written length and remaining buffer length for operationmode FINISH. */
      Crypto_30_LibCv_SetWrittenLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* #75 Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
      if ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
      {
        Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
        Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      }

      /* #80 Check if output size is sufficient otherwise return error. */
      if (Crypto_30_LibCv_GetBufferLength(objectId) < 16u)
      {
        retValCv = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
      }
      else
      {
        retValCv = ESL_ERC_NO_ERROR;
      }

      *job->jobPrimitiveInputOutput.outputLengthPtr = Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

      /* #85 Write results in output buffers. */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        retValCv = esl_finalizeDecryptGCM(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          &job->jobPrimitiveInputOutput.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)],
          job->jobPrimitiveInputOutput.outputLengthPtr,
          job->jobPrimitiveInputOutput.tertiaryInputPtr);
      }

      /* #90 Check if verification was successful. */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        *job->jobPrimitiveInputOutput.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }
      else if (retValCv == ESL_ERC_GCM_TAG_VERIFICATION_FAILED)
      {
        *job->jobPrimitiveInputOutput.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        retValCv = ESL_ERC_NO_ERROR;
      }
      else
      {
        /* For all other errors do nothing */
      }

      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
} /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_SERVICE_AEADENCRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchAeadEncrypt()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAeadEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks --------------------------------------- */

  {

    /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_AESGCMENCRYPT == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    /* #05 Dispatch AEAD encryption based on algorithm family and algorithm mode. */
    if ((job->jobPrimitiveInfo->primitiveInfo->algorithm.family == CRYPTO_ALGOFAM_AES)
      && (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_GCM))
    {
      errorId = CRYPTO_E_NO_ERROR;
      if (Crypto_30_LibCv_IsAesGcmEncryptUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchAesGcmEncrypt(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
    }
# endif

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

#if (CRYPTO_30_LIBCV_SERVICE_AEADDECRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchAeadDecrypt()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAeadDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks --------------------------------------- */

  {

    /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_AESGCMDECRYPT == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    /* #5 Dispatch AEAD decryption based on algorithm family and algorithm mode. */
    if ((job->jobPrimitiveInfo->primitiveInfo->algorithm.family == CRYPTO_ALGOFAM_AES)
      && (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_GCM))
    {
      errorId = CRYPTO_E_NO_ERROR;
      if (Crypto_30_LibCv_IsAesGcmDecryptUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchAesGcmDecrypt(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
    }
# endif

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
 *  END OF FILE: Crypto_30_LibCv_Aead.c
 *********************************************************************************************************************/
