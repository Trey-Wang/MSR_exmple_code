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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CryIf.c
 *        \brief  MICROSAR Crypto Interface (CRYIF)
 *
 *      \details  The Crypto Interface module provides a unique interface to manage different Crypto HW and SW
 *                solutions.
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
#define CRYIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "CryIf_Cbk.h"
#include "Csm_Cbk.h"

#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of CryIf header file */
#if ((  CRYIF_SW_MAJOR_VERSION != (2u)) \
    || (CRYIF_SW_MINOR_VERSION != (0u)) \
    || (CRYIF_SW_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of CryIf.c and CryIf.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CRYIF_LOCAL) /* COV_CRYIF_ALWAYS_ON */
# define CRYIF_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CRYIF_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
CRYIF_LOCAL VAR(uint8, CRYIF_VAR_ZERO_INIT) CryIf_ModuleInitialized = CRYIF_UNINIT;
#endif

#define CRYIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CryIf_KeyCopyForDifferentDrivers()
 *********************************************************************************************************************/
/*! \brief       Copys two key between two drivers.
 *  \details     Gets each key element of set source key and writes it to a matching key element of the target key.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     targetCryIfKeyId        Holds the identifier of the key whose key element shall be the destination
 *                                         element.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *  \return        CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *  \return        CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *  \return        CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element sizes are not compatible.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL Std_ReturnType CryIf_KeyCopyForDifferentDrivers(uint32 cryIfKeyId, uint32 targetCryIfKeyId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CryIf_KeyCopyForDifferentDrivers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYIF_LOCAL Std_ReturnType CryIf_KeyCopyForDifferentDrivers(uint32 cryIfKeyId, uint32 targetCryIfKeyId)
{
  uint8 i, j;
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType retValSrc;
  Std_ReturnType retValDst;
  uint8 buffer[CryIf_GetMaxSizeOfKeyElementOfGeneral()];
  uint32 bufferSize = CryIf_GetMaxSizeOfKeyElementOfGeneral();
  uint32 elementIdsSrc[CryIf_GetMaxNumberOfKeyElementsOfGeneral()];
  uint32 elementIdsSrcSize = CryIf_GetMaxNumberOfKeyElementsOfGeneral();
  uint32 elementIdsDst[CryIf_GetMaxNumberOfKeyElementsOfGeneral()];
  uint32 elementIdsDstSize = CryIf_GetMaxNumberOfKeyElementsOfGeneral();

  retValSrc = CryIf_GetKeyElementIdsGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), elementIdsSrc, &elementIdsSrcSize); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
  retValDst = CryIf_GetKeyElementIdsGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId))(CryIf_GetRefOfKey(targetCryIfKeyId), elementIdsDst, &elementIdsDstSize); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */

  if ((retValSrc == E_OK) && (retValDst == E_OK))
  {
    for (i = 0; i < elementIdsSrcSize; i++)
    {
      for (j = 0; j < elementIdsDstSize; j++)
      {
        if (elementIdsSrc[i] == elementIdsDst[j])
        {
          /* Copy compatible elements */
          if (E_OK == CryIf_GetKeyElementGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), elementIdsSrc[i], buffer, &bufferSize)) /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
          {
            if (E_OK == CryIf_GetKeyElementSetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId))(CryIf_GetRefOfKey(targetCryIfKeyId), elementIdsDst[j], buffer, bufferSize)) /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
            {
              retVal = E_OK;
            }
          }
          break;
        }
      }
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CryIf_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  CryIf_ModuleInitialized = CRYIF_UNINIT;
#endif
} /* CryIf_InitMemory() */

/**********************************************************************************************************************
 *  CryIf_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_INIT)
  {
    errorId = CRYIF_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
    CryIf_ModuleInitialized = (uint8)CRYIF_INIT;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_INIT, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* CryIf_Init() */

#if (CRYIF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CryIf_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (CRYIF_VENDOR_ID); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->moduleID = (CRYIF_MODULE_ID); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->sw_major_version = (CRYIF_SW_MAJOR_VERSION); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->sw_minor_version = (CRYIF_SW_MINOR_VERSION); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->sw_patch_version = (CRYIF_SW_PATCH_VERSION); /* SBSW_CRYIF_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_GET_VERSION_INFO, errorId);
  }
# else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
} /* CryIf_GetVersionInfo() */
#endif /* (CRYIF_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  CryIf_CallbackNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_CallbackNotification(P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job,
                                                  Std_ReturnType result)
{
  Csm_CallbackNotification(job, result); /* SBSW_CRYIF_API_CALL_FORWARDING_PTR */
}

/**********************************************************************************************************************
 * CryIf_ProcessJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_ProcessJob(uint32 channelId,
                                                  P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'channelId' */
  else if (channelId >= CryIf_GetSizeOfChannelOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* Check if keyId is in range */
    if(job->jobPrimitiveInfo->cryIfKeyId < CryIf_GetSizeOfKey())
    {
      job->cryptoKeyId = CryIf_GetRefOfKey(job->jobPrimitiveInfo->cryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */

      retVal = CryIf_GetProcessJobOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfChannel(channelId))(CryIf_GetDriverObjectRefOfChannel(channelId), job); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
    }
    else
    {
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_PROCESS_JOB, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_ProcessJob() */

/**********************************************************************************************************************
 * CryIf_ProcessJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CancelJob(uint32 channelId,
                                                 P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'channelId' */
  else if (channelId >= CryIf_GetSizeOfChannelOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* Check if keyId is in range */
    if(job->jobPrimitiveInfo->cryIfKeyId < CryIf_GetSizeOfKey())
    {
      job->cryptoKeyId = CryIf_GetRefOfKey(job->jobPrimitiveInfo->cryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */

      retVal = CryIf_GetCancelJobOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfChannel(channelId))(CryIf_GetDriverObjectRefOfChannel(channelId), job); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
    }
    else
    {
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_CANCEL_JOB, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_CancelJob() */

/**********************************************************************************************************************
 * CryIf_KeyElementSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementSet(uint32 cryIfKeyId,
                                                     uint32 keyElementId,
                                                     P2CONST(uint8, AUTOMATIC, CRYIF_APPL_VAR) keyPtr,
                                                     uint32 keyLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (keyPtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'keyLength' */
  else if (keyLength == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKeyOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyElementSetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                     keyElementId,
                                                     keyPtr,
                                                     keyLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_SET, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyElementSet() */

/**********************************************************************************************************************
 * CryIf_KeySetValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeySetValid(uint32 cryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKeyOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyValidSetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_SET_VALID, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyValidSet() */

/**********************************************************************************************************************
 * CryIf_KeyElementGet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementGet(uint32 cryIfKeyId,
                                                     uint32 keyElementId,
                                                     P2VAR(uint8, AUTOMATIC, CRYIF_APPL_VAR) resultPtr,
                                                     P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) resultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if ((resultPtr == NULL_PTR) || (resultLengthPtr == NULL_PTR))
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'resultLengthPtr' */
  else if (*resultLengthPtr == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKeyOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyElementGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                   keyElementId,
                                                                                                   resultPtr,
                                                                                                   resultLengthPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_GET, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyElementGet() */

/**********************************************************************************************************************
 * CryIf_KeyElementCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopy(uint32 cryIfKeyId,
                                                      uint32 keyElementId,
                                                      uint32 targetCryIfKeyId,
                                                      uint32 targetKeyElementId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxSrc = CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId);
    CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxDst = CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId);
    CryIf_RefOfKeyType cryptoKeyIdSrc = CryIf_GetRefOfKey(cryIfKeyId);
    CryIf_RefOfKeyType cryptoKeyIdDst = CryIf_GetRefOfKey(targetCryIfKeyId);

    /* ----- Implementation ----------------------------------------------- */
    if (cryptoFctIdxSrc == cryptoFctIdxDst)
    {
      /* Keys are in same crypto driver */
      retVal = CryIf_GetKeyElementCopyOfCryptoFunctions(cryptoFctIdxSrc)(cryptoKeyIdSrc, keyElementId, cryptoKeyIdDst, targetKeyElementId); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
    }
    else
    {
      uint8 buffer[CryIf_GetMaxSizeOfKeyElementOfGeneral()];
      uint32 bufferSize = CryIf_GetMaxSizeOfKeyElementOfGeneral();
      /* Copy key element by getting element from source driver and set it to given key element in destination driver. */
      if (CryIf_GetKeyElementGetOfCryptoFunctions(cryptoFctIdxSrc)(cryptoKeyIdSrc, keyElementId, buffer, &bufferSize) == E_OK) /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      {
        retVal = CryIf_GetKeyElementSetOfCryptoFunctions(cryptoFctIdxDst)(cryptoKeyIdDst, targetKeyElementId, buffer, bufferSize); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_COPY, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyElementCopy() */

/**********************************************************************************************************************
 * CryIf_KeyCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyCopy(uint32 cryIfKeyId,
                                               uint32 targetCryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId) == CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId))
    {
      /* Keys are in same crypto driver */
      retVal = CryIf_GetKeyCopyOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), CryIf_GetRefOfKey(targetCryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
    }
    else
    {
      /* Keys are in different crypto drivers */
      retVal = CryIf_KeyCopyForDifferentDrivers(cryIfKeyId, targetCryIfKeyId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_COPY, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyCopy() */

/**********************************************************************************************************************
 * CryIf_RandomSeed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_RandomSeed(uint32 cryIfKeyId,
                                                  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_VAR) seedPtr,
                                                  uint32 seedLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (seedPtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check for value 'seedLength' */
  else if (seedLength == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKeyOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetRandomSeedOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                seedPtr,
                                                                                                seedLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_RANDOM_SEED, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_RandomSeed() */

/**********************************************************************************************************************
 * CryIf_KeyGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyGenerate(uint32 cryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKeyOfPCConfig())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyGenerateOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_GENERATE, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyGenerate() */

/**********************************************************************************************************************
 * CryIf_KeyDerive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyDerive(uint32 cryIfKeyId,
                                                 uint32 targetCryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyDeriveOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), CryIf_GetRefOfKey(targetCryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_DERIVE, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyDerive() */

/**********************************************************************************************************************
 * CryIf_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcPubVal(uint32 cryIfKeyId,
                                                             P2VAR(uint8, AUTOMATIC, CRYIF_APPL_VAR) publicValuePtr,
                                                             P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if ((publicValuePtr == NULL_PTR) || (publicValueLengthPtr == NULL_PTR))
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check for value 'publicValueLengthPtr' */
  else if (*publicValueLengthPtr == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyExchangeCalcPubValOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                           publicValuePtr,
                                                                                                           publicValueLengthPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_EXCHANGE_CALC_PUB_VAL, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 * CryIf_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcSecret(uint32 cryIfKeyId,
                                                             P2CONST(uint8, AUTOMATIC, CRYIF_APPL_VAR) partnerPublicValuePtr,
                                                             uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check for value 'partnerPublicValueLength' */
  else if (partnerPublicValueLength == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyExchangeCalcSecretOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                           partnerPublicValuePtr,
                                                                                                           partnerPublicValueLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_EXCHANGE_CALC_SECRET, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_KeyExchangeCalcSecret() */

/**********************************************************************************************************************
 * CryIf_CertificateParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateParse(uint32 cryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetCertificateParseOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_CERTIFICATE_PARSE, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_CertificateParse() */

/**********************************************************************************************************************
 * CryIf_CertificateVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateVerify(uint32 cryIfKeyId,
                                                         uint32 verifyCryIfKeyId,
                                                         P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_VAR) verifyPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (verifyPtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'cryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (verifyCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetCertificateVerifyOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                       CryIf_GetRefOfKey(verifyCryIfKeyId),
                                                                                                       verifyPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_CERTIFICATE_VERIFY, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* CryIf_CertificateVerify() */

#define CRYIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
*/

/* SBSW_JUSTIFICATION_BEGIN
  \ID SBSW_CRYIF_GETVERSIONINFO
    \DESCRIPTION The function CryIf_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter VersionInfoPtr is valid.

  \ID SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros. 
                    The API is called with a pointer and an id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The Id is a SNV provided by the CryIf and is converted to the corresponding ID of lower layer using ComStackLib (CSL01).
                       It is checked when it was first passed to the CryIf module.
                       The pointer is either forwarded and assumed to be valid or it is a local stack variable.

  \ID SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
                    The API is called with an Id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The Id is a SNV provided by the CryIf and is converted to the corresponding ID of lower layer using ComStackLib (CSL01).
                       It is checked when it was first passed to the CryIf module.

  \ID SBSW_CRYIF_API_CALL_FORWARDING_PTR
    \DESCRIPTION    The API call is forwarded and has as parameter one or more pointer.
    \COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

  \ID SBSW_CRYIF_WRITE_TO_JOB_POINTER
    \DESCRIPTION    The function does a write access on the job pointer.
    \COUNTERMEASURE \R The pointer is checked when it was first to the service.
SBSW_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

 \ID COV_CRYIF_ALWAYS_ON
    \ACCEPT TX
    \REASON CRYIF_LOCAL is always defined externally.

 COV_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  END OF FILE: CryIf.c
 *********************************************************************************************************************/
