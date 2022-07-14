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
/*      \file  Crypto_30_LibCv_Random.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_RANDOM_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"

#include "ESLib_types.h"
#include "actFIPS186.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_FIPS_SEED_LENGTH (20u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomFips186()
*********************************************************************************************************************/
/*! \brief         Dispatches the RandomFips186 job to the lower layer.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomFips186(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomFips186()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomFips186(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* #05 Distinguish modes. */
  switch (mode)
  {
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = E_OK;
      break;
    }
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = E_OK;
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_ErrorCode retValCv;
      P2VAR(eslt_WorkSpaceFIPS186, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfFips186(Crypto_30_LibCv_GetFips186IdxOfObjectInfo(objectId));
      actFIPS186STRUCT * randomWorkspace = (actFIPS186STRUCT *)(workspace->wsFIPS186); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST */
      uint8 fips186Key[CRYPTO_30_LIBCV_FIPS_SEED_LENGTH];
      uint32 fips186KeyLength = CRYPTO_30_LIBCV_FIPS_SEED_LENGTH;
      uint8 *randomSeedBuffer = &Crypto_30_LibCv_GetRandomSeedBuffer(Crypto_30_LibCv_GetRandomSeedBufferStartIdxOfObjectInfo(objectId));

      /* #75 Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_FIPS186, CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        uint8_least i;
        Crypto_30_LibCv_KeyElementsIterType elementIndex;

        /* #80 Restore workspace. */
        /* Restore workspace - This is done manually since workspace is not kept forever due to shared memory (union) */
        for (i = 0u; i < CRYPTO_30_LIBCV_FIPS_SEED_LENGTH; ++i)
        {
          randomWorkspace->X_KEY[i] = randomSeedBuffer[i]; /* SBSW_CRYPTO_30_LIBCV_ACT_RANDOM_WORKSPACE_ARRAY_ACCESS */
        }

        randomWorkspace->bytes_remain = 0u; /* SBSW_CRYPTO_30_LIBCV_ACT_RANDOM_WORKSPACE_MEMBER_ACCESS */
        workspace->header.status = (ESL_WST_ALGO_FIPS186 | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */

        /* Get index of keyElement SEED_STATE */
        elementIndex = Crypto_30_LibCv_Local_KeyElementSearch(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE);

        if (elementIndex < Crypto_30_LibCv_GetSizeOfKeyElements())
        {
          /* #85 Reseed if RandomSeed was called on the given key. */
          /* Check if randomSeed was called on the given key element */
          SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
          if ((Crypto_30_LibCv_GetKeyElementState(elementIndex) & CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_MASK) == CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_MASK)
          {
            /* Get new seed value */
            if (E_OK == Crypto_30_LibCv_Local_KeyElementGet(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, fips186Key, &fips186KeyLength)) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
            {
              retValCv = esl_stirFIPS186(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                (eslt_Length)fips186KeyLength,
                (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))fips186Key);
              /* mark new seed as used */
              Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
            }
            else
            {
              /* New seed value was set but key is not accessible */
              retValCv = ESL_ERC_ERROR;
              retVal = CRYPTO_E_KEY_NOT_VALID;
            }
          }
          SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
        }

        /* #90 Generate Random Numbers. */
        if (retValCv == ESL_ERC_NO_ERROR)
        {
          retValCv = esl_getBytesFIPS186(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            (eslt_Length)*job->jobPrimitiveInputOutput.outputLengthPtr,
            job->jobPrimitiveInputOutput.outputPtr);

          /* #95 Save workspace. */
          /* Save internal state of key random method to continue with the next good value. */
          for (i = 0u; i < CRYPTO_30_LIBCV_FIPS_SEED_LENGTH; ++i)
          {
            randomSeedBuffer[i] = randomWorkspace->X_KEY[i]; /* SBSW_CRYPTO_30_LIBCV_CSL01 */
          }
        }
      }

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        retVal = E_OK;
      }

      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_SERVICE_RANDOM == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandom()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandom(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */

  {
    /* ----- Implementation ------------------------------------------------- */

    /* #05 Dispatch Service according to algorithm family. */
    switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
    {
# if (CRYPTO_30_LIBCV_FIPS186 == STD_ON) /* COV_CRYPTO_30_LIBCV_VAR_OPTIONAL_DEFINE_TX */
    case CRYPTO_ALGOFAM_RNG:
      if (Crypto_30_LibCv_IsFips186UsedOfObjectInfo(objectId) == TRUE)
      {
        retVal = Crypto_30_LibCv_DispatchRandomFips186(objectId, job, mode); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
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
 *  END OF FILE: Crypto_30_LibCv_Random.c
 *********************************************************************************************************************/
