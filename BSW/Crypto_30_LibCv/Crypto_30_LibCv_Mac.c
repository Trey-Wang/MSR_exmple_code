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
/*      \file  Crypto_30_LibCv_Mac.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for MAC Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_MAC_SOURCE
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
 *  LOCAL_CONSTANT MACROS
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_MODULO_8(x)           ((x) & (8u - 1u))
#define CRYPTO_30_LIBCV_SIP_HASH_KEY_LENGTH   (16u)
#define CRYPTO_30_LIBCV_SHA2_256_KEY_LENGTH   (256u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( (CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON) )
/**********************************************************************************************************************
 *  Crypto_30_LibCv_CompareMac()
 *********************************************************************************************************************/
/*! \brief          Compares two MACs
 *  \details        This function compares two MAC values from MSB to LSB.
 *  \param[in]      mac1                  Holds a pointer to the first mac
 *  \param[in]      mac2                  Holds a pointer to the second mac
 *  \param[in]      lengthInBit           Number of bits which shall be compared from MSB to LSB.
 *  \param[in,out]  verResult             Holds a pointer to the variable which will contain the result of the verification.
 *  \return         CRYPTO_E_VER_NOT_OK   Macs are not similar
 *  \return         CRYPTO_E_VER_OK       Macs are similar
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Crypto_VerifyResultType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CompareMac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac1,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac2,
  uint32 lengthInBit);
#endif

#if ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_LibCv_InitializeCmacAes()
*********************************************************************************************************************/
/*! \brief         Handels the initialization of the CmacAesGenerate job.
*  \details        This function initializes the provided workspace for the lower layer crypto library. Based for the 
*                  job operationmode start.
*  \param[in,out]  workspace          Pointer to the esl workspace.
*  \param[in]      cryptoKeyId        Holds the identifier of the Crypto Key Id.
*  \return         E_OK               Init was successful.
*  \return         E_NOT_OK           Init failed.
*  \pre            workspace has to be valid.
*  \context        TASK
*  \reentrant      TRUE
*  \synchronous    TRUE
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitializeCmacAes(
  P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace, 
  uint32 cryptoKeyId);
#endif

#if (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCmacAesGenerate()
 *********************************************************************************************************************/
/*! \brief         Dispatches the CmacAesGenerate job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCmacAesGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCmacAesVerify()
 *********************************************************************************************************************/
/*! \brief         Dispatches the CmacAesVerify job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCmacAesVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_SIPHASHGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSipHashGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SipHashGenerate job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSipHashGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSipHashVerify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SipHashVerify job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSipHashVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSha256Generate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the HmacSha256Generate job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSha256Generate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

#if (CRYPTO_30_LIBCV_CMACAES_ROUNDKEY_ACCELERATION == STD_ON)
# define CRYPTO_30_LIBCV_CMACAES_LOCAL_BUFFER_SIZE (256u)
#else
# define CRYPTO_30_LIBCV_CMACAES_LOCAL_BUFFER_SIZE (16u)
#endif
#define CRYPTO_30_LIBCV_CMACAES_MAC_SIZE (16u)
#define CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE (8u)

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_CompareMac()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Crypto_VerifyResultType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CompareMac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac1,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac2,
  uint32 lengthInBit)
{
  Crypto_VerifyResultType result = CRYPTO_E_VER_OK;
  uint8 verifyBitMask;
  uint32 numberBitsInLastByte;
  uint8 numberOfFullBytesInMac = (uint8)(lengthInBit >> 3u);
  uint8 i;

  /* #05 Compare full bytes of both MACs.*/
  for (i = 0u; i < (numberOfFullBytesInMac); i++)
  {
    if (mac1[i] != mac2[i])
    {
      /* Verification does not match */
      result = CRYPTO_E_VER_NOT_OK;
    }
  }

  /* #10 Compare remaining bits. */
  /* Compute number of bits to verify in the last byte */
  numberBitsInLastByte = CRYPTO_30_LIBCV_MODULO_8(lengthInBit);  /* Calculate modulo */
  if (numberBitsInLastByte != 0u)
  {
    /* Generate bitmask which contains all bits to be verified
    * This depends on the byteorder */
# if (CPU_BIT_ORDER == LSB_FIRST)   /* little endian bit ordering */ /* COV_CRYPTO_30_LIBCV_BIT_ORDER */
    verifyBitMask = (uint8)~((1u << (8u - numberBitsInLastByte)) - 1u);
# else
    verifyBitMask = ((1u << (numberBitsInLastByte)) - 1u);
# endif

    /* Verify the last masked byte */
    if ((mac1[numberOfFullBytesInMac] & verifyBitMask)
      != (mac2[numberOfFullBytesInMac] & verifyBitMask))
    {
      result = CRYPTO_E_VER_NOT_OK;
    }
  }

  return result;
}
#endif

#if ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_LibCv_InitializeCmacAes()
*********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitializeCmacAes(P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace, uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;
  /* #15 Initialize workspace header. */
  retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_CMACAES, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    uint8 cmacAesKey[CRYPTO_30_LIBCV_CMACAES_LOCAL_BUFFER_SIZE];
    uint32 cmacAesKeyLength;

    /* #20 Load key element from storage and initialize algorithm with keys. Dependent on roundkey feature, either the roundkey is loaded as key or the original key. */
# if (CRYPTO_30_LIBCV_CMACAES_ROUNDKEY_ACCELERATION == STD_ON)
    uint8 roundkeyExists;
    Crypto_30_LibCv_KeyElementsIterType elementId = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY);

    roundkeyExists = (uint8)(elementId < Crypto_30_LibCv_GetSizeOfKeyElements());

    /* Check if AES Roundkey is available */
    if ((roundkeyExists == TRUE) && ((Crypto_30_LibCv_GetKeyElementState(elementId) & CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_MASK) == CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_MASK))
    {
      Std_ReturnType localRetVal;
      cmacAesKeyLength = CRYPTO_30_LIBCV_CMACAES_LOCAL_BUFFER_SIZE;
      /* Check if key element is accessible */
      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
      localRetVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY, cmacAesKey, &cmacAesKeyLength);  /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
      if (E_OK == localRetVal)
      {
        retValCv = esl_initExpandedCMACAES128(workspace, cmacAesKey, (eslt_Length)cmacAesKeyLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
      }
      else
      {
        retValCv = ESL_ERC_ERROR;
        retVal = CRYPTO_E_KEY_NOT_VALID;
      }

    }
    else
# endif
    {
      cmacAesKeyLength = 16u;
      /* Get key */
      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
      if (E_OK == Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_MAC_KEY, cmacAesKey, &cmacAesKeyLength)) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
# if (CRYPTO_30_LIBCV_CMACAES_ROUNDKEY_ACCELERATION == STD_OFF)
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
# endif
        /* Store key in workspace */
        retValCv = esl_initCMACAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
          (eslt_Length)cmacAesKeyLength,
          (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))cmacAesKey);

# if (CRYPTO_30_LIBCV_CMACAES_ROUNDKEY_ACCELERATION == STD_ON)
        /* #25 Store round key locally if roundkey feature is enabled. */
        if ((roundkeyExists == TRUE) && (retValCv == ESL_ERC_NO_ERROR))
        {
          eslt_Length aesKeyLength = CRYPTO_30_LIBCV_CMACAES_LOCAL_BUFFER_SIZE;
          cmacAesKeyLength = aesKeyLength;
          retValCv = esl_getExpandedKeyCMACAES128(workspace, cmacAesKey, &aesKeyLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */

          if (retValCv == ESL_ERC_NO_ERROR)
          {
            Crypto_30_LibCv_KeyElementSetInternal(elementId, cmacAesKey, cmacAesKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
            Crypto_30_LibCv_SetKeyElementStateByMask(elementId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
          }
        }
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
# endif
      }
      else
      {
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        retValCv = ESL_ERC_ERROR;
        retVal = CRYPTO_E_KEY_NOT_VALID;
      }

    }
  }
  
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchCmacAesGenerate()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCmacAesGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfCmacAesGenerate(Crypto_30_LibCv_GetCmacAesGenerateIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_InitializeCmacAes(workspace, job->cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* #45 Process plaintext data. */
      retValCv = esl_updateCMACAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);

      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Byte macBuffer[CRYPTO_30_LIBCV_CMACAES_MAC_SIZE];

      if (ESL_ERC_NO_ERROR == esl_finalizeCMACAES128(workspace, macBuffer)) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      {
        uint8 i;

        /* #75 Adapt output size. */
        if (*job->jobPrimitiveInputOutput.outputLengthPtr > CRYPTO_30_LIBCV_CMACAES_MAC_SIZE)
        {
          *job->jobPrimitiveInputOutput.outputLengthPtr = CRYPTO_30_LIBCV_CMACAES_MAC_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* #80 Write results in output buffers. */
        for (i = 0u; i < *job->jobPrimitiveInputOutput.outputLengthPtr; i++)
        {
          job->jobPrimitiveInputOutput.outputPtr[i] = macBuffer[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        retValCv = ESL_ERC_NO_ERROR;
      }
      break;
    }

    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
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
} /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchCmacAesVerify()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCmacAesVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfCmacAesVerify(Crypto_30_LibCv_GetCmacAesVerifyIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_InitializeCmacAes(workspace, job->cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      break;
    }

    /* #40 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
        /* #45 Process plaintext data*/
        retValCv = esl_updateCMACAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
          job->jobPrimitiveInputOutput.inputPtr);

      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {

        eslt_Byte macBuffer[CRYPTO_30_LIBCV_CMACAES_MAC_SIZE];
        uint32 macLength;

        retValCv = esl_finalizeCMACAES128(workspace, macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
        if (retValCv == ESL_ERC_NO_ERROR)
        {
          /* #75 Check if input length is not to large. */
          macLength = job->jobPrimitiveInputOutput.secondaryInputLength;
          /* size is given in bits */
          if (macLength <= (CRYPTO_30_LIBCV_CMACAES_MAC_SIZE << 3u))
          {

            /* #80 Check if verification was successful. */
            *(job->jobPrimitiveInputOutput.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
              Crypto_30_LibCv_CompareMac(job->jobPrimitiveInputOutput.secondaryInputPtr, macBuffer, macLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

          }
          else
          {
            retValCv = ESL_ERC_ERROR;
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
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */ 
#endif

#if (CRYPTO_30_LIBCV_SIPHASHGENERATE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchSipHashGenerate()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSipHashGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceSipHash, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfSipHashGenerate(Crypto_30_LibCv_GetSipHashGenerateIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SIPHASH, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8 sipHashKey[CRYPTO_30_LIBCV_SIP_HASH_KEY_LENGTH];
        uint32 sipHashKeyLength = CRYPTO_30_LIBCV_SIP_HASH_KEY_LENGTH;
        Std_ReturnType localRetVal;

        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        localRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_MAC_KEY, sipHashKey, &sipHashKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        /* #20 Load key element from storage. */
        if (E_OK == localRetVal)
        {
          /* #25 Initialize algorithm with keys. */
          retValCv = esl_initSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
            (eslt_Length)sipHashKeyLength,
            (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))sipHashKey);
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

      /* #45 Process plaintext data */
      retValCv = esl_updateSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);

      break;
    }
    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Byte macBuffer[CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE];

      retValCv = esl_finalizeSipHash(workspace, macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8 i;
        /* #75 Adapt output size. */
        if (*job->jobPrimitiveInputOutput.outputLengthPtr > CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE)
        {
          *job->jobPrimitiveInputOutput.outputLengthPtr = CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* #80 Write results in output buffers. */
        for (i = 0u; i < *job->jobPrimitiveInputOutput.outputLengthPtr; i++)
        {
          job->jobPrimitiveInputOutput.outputPtr[i] = macBuffer[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
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
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSipHashVerify()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSipHashVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  P2VAR(eslt_WorkSpaceSipHash, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfSipHashVerify(Crypto_30_LibCv_GetSipHashVerifyIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* #15 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SIPHASH, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8 sipHashKey[CRYPTO_30_LIBCV_SIP_HASH_KEY_LENGTH];
        uint32 sipHashKeyLength = CRYPTO_30_LIBCV_SIP_HASH_KEY_LENGTH;
        Std_ReturnType localRetVal;

        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        localRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_MAC_KEY, sipHashKey, &sipHashKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        /* #20 Load key element from storage. */
        if (E_OK == localRetVal)
        {
          /* #25 Initialize algorithm with keys. */
          retValCv = esl_initSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
            (eslt_Length)sipHashKeyLength,
            (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))sipHashKey);
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
      /* #45 Process plaintext data */
      retValCv = esl_updateSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);

    break;
  }

  /* #70 Handle operationmode FINISH. */
  case CRYPTO_OPERATIONMODE_FINISH:
  {
    eslt_Byte macBuffer[CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE];
    uint32 macLength;
    retValCv = esl_finalizeSipHash(workspace, macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      /* #75 Check if input length is not to large. */
      macLength = job->jobPrimitiveInputOutput.secondaryInputLength;
      if (macLength <= (CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE << 3u))
      {

        /* #80 Check if verification was successful. */
        *(job->jobPrimitiveInputOutput.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
          Crypto_30_LibCv_CompareMac(job->jobPrimitiveInputOutput.secondaryInputPtr, macBuffer, macLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
      else
      {
        retValCv = ESL_ERC_ERROR;
      }
    }
    break;
  }

  default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
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

#if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSha256Generate()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSha256Generate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 ShaKey[CRYPTO_30_LIBCV_SHA2_256_KEY_LENGTH];
  uint8 macBuffer[ESL_SIZEOF_SHA256_DIGEST];
  uint32 ShaKeyLength;
  uint8 i;

  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha256Generate(Crypto_30_LibCv_GetHmacSha256GenerateIdxOfObjectInfo(objectId));

  /* #05 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {

      /* #15 Initialize workspace header.*/
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_HMACSHA256, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        Std_ReturnType localRetVal;

        ShaKeyLength = CRYPTO_30_LIBCV_SHA2_256_KEY_LENGTH;
        SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        localRetVal = Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_MAC_KEY, ShaKey, &ShaKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

        /* #20 Load key element from storage.*/
        if (E_OK == localRetVal)
        {
          /* #25 Initialize algorithm with keys. */
          retValCv = esl_initHashMACSHA256(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
            (eslt_Length)ShaKeyLength,
            (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))ShaKey);
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
      /* #45 Process plaintext data */
      retValCv = esl_updateHashMACSHA256(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        (eslt_Length)job->jobPrimitiveInputOutput.inputLength,
        job->jobPrimitiveInputOutput.inputPtr);

      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      if (ESL_ERC_NO_ERROR == esl_finalizeHashMACSHA256(workspace, macBuffer)) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      {
        /* #75 Adapt output size. */
        if (*job->jobPrimitiveInputOutput.outputLengthPtr > ESL_SIZEOF_SHA256_DIGEST)
        {
          *job->jobPrimitiveInputOutput.outputLengthPtr = ESL_SIZEOF_SHA256_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* #80 Write results in output buffers. */
        for (i = 0u; i < *job->jobPrimitiveInputOutput.outputLengthPtr; i++)
        {
          job->jobPrimitiveInputOutput.outputPtr[i] = macBuffer[i]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* Set return value */
        retValCv = ESL_ERC_NO_ERROR;
      }
      else
      {
        *job->jobPrimitiveInputOutput.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
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
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_SERVICE_MAC_GENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchMacGenerate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchMacGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */

  {
    /* ----- Implementation ------------------------------------------------- */

    /* #05 Dispatch Mac generation based on algorithm family. */
    switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
# if (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON)
    case CRYPTO_ALGOFAM_AES:
      if (Crypto_30_LibCv_IsCmacAesGenerateUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchCmacAesGenerate(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
      break;
# endif
# if (CRYPTO_30_LIBCV_SIPHASHGENERATE == STD_ON)
    case CRYPTO_ALGOFAM_SIPHASH:

      if (Crypto_30_LibCv_IsSipHashGenerateUsedOfObjectInfo(objectId) == TRUE)
      {
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_SIPHASH_2_4)
        {
          retVal = Crypto_30_LibCv_DispatchSipHashGenerate(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
        }
        else
        {
          errorId = CRYPTO_E_PARAM_HANDLE;
        }
      }
      break;
# endif
# if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON)
    case CRYPTO_ALGOFAM_SHA2_256:

      if (Crypto_30_LibCv_IsHmacSha256GenerateUsedOfObjectInfo(objectId) == TRUE)
      {
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_HMAC)
        {
          retVal = Crypto_30_LibCv_DispatchHmacSha256Generate(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
        }
        else
        {
          errorId = CRYPTO_E_PARAM_HANDLE;
        }
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

#if (CRYPTO_30_LIBCV_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_DispatchMacVerify()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchMacVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */

  {
    /* ----- Implementation ------------------------------------------------- */

    /* #05 Dispatch Mac verification based on algorithm family. */
    switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
# if (CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON)
    case CRYPTO_ALGOFAM_AES:
      if (Crypto_30_LibCv_IsCmacAesVerifyUsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchCmacAesVerify(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
      break;
# endif
# if (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON)
    case CRYPTO_ALGOFAM_SIPHASH:
      if (Crypto_30_LibCv_IsSipHashVerifyUsedOfObjectInfo(objectId) == TRUE)
      {
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_SIPHASH_2_4)
        {
          retVal = Crypto_30_LibCv_DispatchSipHashVerify(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
        }
        else
        {
          errorId = CRYPTO_E_PARAM_HANDLE;
        }
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

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Mac.c
 *********************************************************************************************************************/
