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
/*      \file  Crypto_30_LibCv_Signature.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for SIGNATURE Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_SIGNATURE_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY  32u

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEd25519Generate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SignatureEd25519Generate job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEd25519Generate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEd25519Verify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SignatureEd25519Verify job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEd25519Verify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEd25519Generate()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEd25519Generate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfEd25519Generate(Crypto_30_LibCv_GetEd25519GenerateIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_Ed25519, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #20 Initialize algorithm with or without pre - hashing based on secondary algorithm family. */
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
        {
          /* Init EdDsa with pre-hashing */
          retValCv = esl_initSignEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
            ESL_Curve25519,
            ESL_INSTANCE_Ed25519ph,
            (CRYPTOCV_P2CONST_PARA(eslt_Byte))NULL_PTR,
            0);
        }
        else
        {
          /* Init EdDsa without pre-hashing */
          retValCv = esl_initSignEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
            ESL_Curve25519,
            ESL_INSTANCE_Ed25519,
            (CRYPTOCV_P2CONST_PARA(eslt_Byte))NULL_PTR,
            0);
        }
      }

      break;
    }
    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
        /* #45 Process prehashing if necessary. */
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
        {
          /* Prehash Calculation */
          retValCv = esl_updateEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            job->jobPrimitiveInputOutput.inputPtr,
            (eslt_Length)job->jobPrimitiveInputOutput.inputLength);
        }
        else
        {
          /* Nothing to do if no prehashing is used */
          retValCv = ESL_ERC_NO_ERROR;
        }
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
        uint8 sigGenKey[CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY];
        uint32 sigGenKeyLength = CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY;
        Std_ReturnType localRetVal;

        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        localRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, sigGenKey, &sigGenKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        if (E_OK == localRetVal)
        {
          eslt_Length outputLength;
          outputLength = (eslt_Length)*(job->jobPrimitiveInputOutput.outputLengthPtr);

          /* #75 Generate Signature based on loaded signature key. */
          retValCv = esl_signEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            job->jobPrimitiveInputOutput.inputPtr,
            (eslt_Length)((job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512) ? (0u) : (job->jobPrimitiveInputOutput.inputLength)),
            sigGenKey,
            0,
            job->jobPrimitiveInputOutput.outputPtr,
            &outputLength);

          *(job->jobPrimitiveInputOutput.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
          retVal = CRYPTO_E_KEY_NOT_VALID;
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
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

#endif

#if (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEd25519Verify()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEd25519Verify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfEd25519Verify(Crypto_30_LibCv_GetEd25519VerifyIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
        /* #15 Initialize workspace header. */
        retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_Ed25519, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

        if (retValCv == ESL_ERC_NO_ERROR)
        {
          /* #20 Initialize algorithm with or without pre-hashing based on secondary algorithm family. */
          if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
          {
            /* Init EdDsa with pre-hashing */
            retValCv = esl_initVerifyEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
              ESL_Curve25519,
              ESL_INSTANCE_Ed25519ph,
              (CRYPTOCV_P2CONST_PARA(eslt_Byte))NULL_PTR,
              0);
          }
          else
          {
            /* Init EdDsa without pre-hashing */
            retValCv = esl_initVerifyEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
              ESL_Curve25519,
              ESL_INSTANCE_Ed25519,
              (CRYPTOCV_P2CONST_PARA(eslt_Byte))NULL_PTR,
              0);
          }
        }
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* #45 Process prehashing if necessary. */
      if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
      {
        /* Prehash Calculation */
        retValCv = esl_updateEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          job->jobPrimitiveInputOutput.inputPtr,
          (eslt_Length)job->jobPrimitiveInputOutput.inputLength);
      }
      else
      {
        /* Nothing to do if no prehashing is used */
        retValCv = ESL_ERC_NO_ERROR;
      }
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      uint8 sigVerKey[CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY];
      uint32 sigVerKeyLength = CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY;
      Std_ReturnType localRetVal;

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
      localRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, sigVerKey, &sigVerKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

      if (E_OK == localRetVal)
      {
        /* #75 Verify Signature based on loaded signature key. */
        retValCv = esl_verifyEdDSA((eslt_WorkSpace*)workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
          job->jobPrimitiveInputOutput.inputPtr,
          (eslt_Length)((job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512) ? (0u) : (job->jobPrimitiveInputOutput.inputLength)),
          sigVerKey,
          job->jobPrimitiveInputOutput.secondaryInputPtr,
          (eslt_Length)job->jobPrimitiveInputOutput.secondaryInputLength);

        if (retValCv == ESL_ERC_NO_ERROR )
        {
          *job->jobPrimitiveInputOutput.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
        else
        {
          if ((retValCv == ESL_ERC_SIGNATURE_INVALID) || (retValCv == ESL_ERC_ECC_SIGNATURE_INVALID))
          {
            retValCv = ESL_ERC_NO_ERROR;
            retVal = E_OK;
          }
          *job->jobPrimitiveInputOutput.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
      }
      else
      {
        retValCv = ESL_ERC_ERROR;
        retVal = CRYPTO_E_KEY_NOT_VALID;
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
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_SERVICE_SIGNATURE_GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */

  {

    /* ----- Implementation ------------------------------------------------- */
    switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
# if (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    /* #05 Dispatch Signature Generation based on algorithm family. */
    case CRYPTO_ALGOFAM_ED25519:
      if (Crypto_30_LibCv_IsEd25519GenerateUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchSignatureEd25519Generate(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
      break;
# endif
    default:
      errorId = CRYPTO_E_PARAM_HANDLE;
      break;
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

#if (CRYPTO_30_LIBCV_SERVICE_SIGNATURE_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */

  {
    /* ----- Implementation ------------------------------------------------- */

    switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
# if (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    /* #05 Dispatch Signature Verification based on algorithm family. */
    case CRYPTO_ALGOFAM_ED25519:
      if (Crypto_30_LibCv_IsEd25519VerifyUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchSignatureEd25519Verify(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
      break;
# endif

    default:
      errorId = CRYPTO_E_PARAM_HANDLE;
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
 *  END OF FILE: Crypto_30_LibCv_Signature.c
 *********************************************************************************************************************/
