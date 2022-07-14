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
/*      \file  Crypto_30_LibCv_Cipher.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for CIPHER Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_CIPHER_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_AES128_KEY_LENGTH (16u)
#define CRYPTO_30_LIBCV_AES128_IV_LENGTH (16u)
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CRYPTO_30_LIBCV_AES128ENCRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchCipherAes128Encrypt()
*********************************************************************************************************************/
/*! \brief         Dispatches the CipherAes128Encrypt job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAes128Encrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_AES128DECRYPT == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_LibCv_DispatchCipherAes128Decrypt()
*********************************************************************************************************************/
/*! \brief         Dispatches the CipherAes128Decrypt job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAes128Decrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_AES128ENCRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchCipherAes128Encrypt()
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
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAes128Encrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  eslt_BlockMode blockMode = ESL_BM_ECB;

  P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfAes128Encrypt(Crypto_30_LibCv_GetAes128EncryptIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      /* Init workspace variables */
      Crypto_30_LibCv_SetBufferLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8 aesEncryptKey[CRYPTO_30_LIBCV_AES128_KEY_LENGTH];
        uint8 aesEncryptIv[CRYPTO_30_LIBCV_AES128_IV_LENGTH] = {0u};
        uint32 aesEncryptKeyLength = CRYPTO_30_LIBCV_AES128_KEY_LENGTH;
        uint32 aesEncryptIvLength = CRYPTO_30_LIBCV_AES128_IV_LENGTH;
        Std_ReturnType cipherIVRetVal = E_OK, cipherKeyRetVal;
        /* #20 Load key element from storage. */

        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        cipherKeyRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_KEY, aesEncryptKey, &aesEncryptKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CBC)
        {
          blockMode = ESL_BM_CBC;
          cipherIVRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_IV, aesEncryptIv, &aesEncryptIvLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        }
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        if ((cipherKeyRetVal == E_OK) && (cipherIVRetVal == E_OK))
        {
          /* #25 Initialize algorithm with keys. */
          retValCv = esl_initEncryptAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
            (P2CONST( eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR ))aesEncryptKey,
            blockMode,
            ESL_PM_PKCS5,
            aesEncryptIv);
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
      eslt_Length outputLength;
      Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      /* Get output length in local variable to omit type mismatches */
      outputLength = (eslt_Length)*(job->jobPrimitiveInputOutput.outputLengthPtr);

      /* #45 Process input data. */
      retValCv = esl_encryptAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr,
        &outputLength,
        job->jobPrimitiveInputOutput.outputPtr);

      /* copy back written output length */
      *(job->jobPrimitiveInputOutput.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

      Crypto_30_LibCv_SetWrittenLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetBufferLength(objectId, Crypto_30_LibCv_GetBufferLength(objectId) - Crypto_30_LibCv_GetWrittenLength(objectId)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      break;
    }
    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Length outputLength;

      /* #75 Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
      if ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
      {
        Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
        Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      }

      /* Get output length in local variable to omit type mismatches */
      outputLength = (eslt_Length)(Crypto_30_LibCv_GetBufferLength(objectId));

      /* #80 Write results in output buffers. */
      retValCv = esl_finalizeEncryptAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        &outputLength,
        &job->jobPrimitiveInputOutput.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);

      /* copy back written output length and add writtenLength */
      *job->jobPrimitiveInputOutput.outputLengthPtr = outputLength + Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

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
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (CRYPTO_30_LIBCV_AES128DECRYPT == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchCipherAes128Decrypt()
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
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherAes128Decrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  eslt_BlockMode blockMode = ESL_BM_ECB;

  P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfAes128Decrypt(Crypto_30_LibCv_GetAes128DecryptIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      uint8 aesDecryptKey[CRYPTO_30_LIBCV_AES128_KEY_LENGTH];
      uint8 aesDecryptIv[CRYPTO_30_LIBCV_AES128_IV_LENGTH] = {0u};
      uint32 aesDecryptKeyLength = CRYPTO_30_LIBCV_AES128_KEY_LENGTH;
      uint32 aesDecryptIvLength = CRYPTO_30_LIBCV_AES128_IV_LENGTH;
      Std_ReturnType cipherIVRetVal = E_OK, cipherKeyRetVal;

      /* Init workspace variables */
      Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetBufferLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #20 Load key element from storage. */
        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        cipherKeyRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_KEY, aesDecryptKey, &aesDecryptKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_CBC)
        {
          blockMode = ESL_BM_CBC;
          cipherIVRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_CIPHER_IV, aesDecryptIv, &aesDecryptIvLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        }
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        if ((cipherKeyRetVal == E_OK) && (cipherIVRetVal == E_OK))
        {
            /* #25 Initialize algorithm with keys. */
            retValCv = esl_initDecryptAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
            (P2CONST( eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR ))aesDecryptKey,
            blockMode,
            ESL_PM_PKCS5,
            aesDecryptIv);
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
          retVal = CRYPTO_E_KEY_NOT_VALID;
        }
      }
      break;
    }

    /* #70 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      eslt_Length outputLength;

      /* Get output length in local variable to omit type mismatches */
      outputLength = (eslt_Length)*(job->jobPrimitiveInputOutput.outputLengthPtr);
      /* #45 Process input data. */
      retValCv = esl_decryptAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr,
        &outputLength,
        job->jobPrimitiveInputOutput.outputPtr);

      Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr) - outputLength); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetWrittenLength(objectId, outputLength); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

      /* copy back written output length */
      *(job->jobPrimitiveInputOutput.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

      break;
    }
    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Length outputLength;

      /* #75 Reinitialize written length and remaining buffer if UPDATE was not part of this job. */
      if ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) != CRYPTO_OPERATIONMODE_UPDATE)
      {
        Crypto_30_LibCv_SetWrittenLength(objectId, 0u); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
        Crypto_30_LibCv_SetBufferLength(objectId, *(job->jobPrimitiveInputOutput.outputLengthPtr)); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      }

      /* Get output length in local variable to omit type mismatches */
      outputLength = (eslt_Length)Crypto_30_LibCv_GetBufferLength(objectId);

      /* #80 Write results in output buffers. */
      retValCv = esl_finalizeDecryptAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        &outputLength,
        &job->jobPrimitiveInputOutput.outputPtr[Crypto_30_LibCv_GetWrittenLength(objectId)]);

      *job->jobPrimitiveInputOutput.outputLengthPtr = outputLength + Crypto_30_LibCv_GetWrittenLength(objectId); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

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
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */ 
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_SERVICE_ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks --------------------------------------- */
  {
    /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_AES128ENCRYPT == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    /* #05 Dispatch Encryption based on algorithm family. */
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.family == CRYPTO_ALGOFAM_AES)
    {
      errorId = CRYPTO_E_NO_ERROR;
      if (Crypto_30_LibCv_IsAes128EncryptUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchCipherAes128Encrypt(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
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

#if (CRYPTO_30_LIBCV_SERVICE_DECRYPT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCipherDecrypt()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks --------------------------------------- */
  {
    /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_AES128DECRYPT == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    /* #05 Dispatch Decryption based on algorithm family. */
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.family == CRYPTO_ALGOFAM_AES)
    {
      errorId = CRYPTO_E_NO_ERROR;
      if (Crypto_30_LibCv_IsAes128DecryptUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchCipherAes128Decrypt(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
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
 *  END OF FILE: Crypto_30_LibCv_Cipher.c
 *********************************************************************************************************************/
