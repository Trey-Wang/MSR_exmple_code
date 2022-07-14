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
/*      \file  Crypto_30_LibCv.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials       Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Markus Schneider              vismss         Vector Informatik GmbH
 *  Tobias Finke                  vistof         Vector Informatik GmbH
 *  Anant Gupta                   visgut         Vector Informatik GmbH
 *  Stefan Rombach                visros         Vector Informatik GmbH
 *  Mathias Waldenburger          vismaw         Vector Informatik GmbH
 *  Matthias Weniger              vismwe         Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-10-31  vismss                Initial creation for ASR 4.3
 *  01.00.01  2016-12-06  vismss                Minor adaptions to the specification
 *  01.00.02  2016-12-07  vismss                Fixed rng and key storage init
 *  01.00.03  2016-12-14  vismss  ESCAN00093302 Fixed AES enc/dec and EdDSA
 *  01.00.04  2017-01-24  vismss  ESCAN00093690 Support of simplified SHE Key Update Protocol
 *                                ESCAN00093350 Fixed workspace casts
 *                                ESCAN00093365 DET encapsulation
 *                                              Adaptions to ASR spec and coding style patterns
 *  01.01.00              visros                Fixed DET ServiceIds
 *                                              Added DET for process job
 *                        visgut                Fixed processing of asynchronous jobs in mainfunction.
 *                                              Include SchM_Crypto_30_LibCv.h
 *                                              Support multiple driver objects and locking of workspaces.
 *                                ESCAN00094360 Compiler warning: GetVersionInfo API missing return statement at end of non-void function
 *                        vistof                Moved Crypto_30_LibCv_QueueStateType from Csm_types.h to Crypto
 *                                              Added Cancel API
 *                        vismss                Added SipHash
 *                                              Adapted EDDSA
 *  01.01.01              vismaw  ESCAN00094769 Calling of services always returns false
 *  01.02.00  2017-05-05  vismaw  FEATC-1229    Release of component
 *                                ESCAN00093105 Release of component
 *                                ESCAN00095032 AEAD Encrypt does not check if output buffer for authentication tag has a wrong size
 *                                ESCAN00095030 Crypto_30_LibCv_KeyElementIdsGet always returns E_NOT_OK
 *                                ESCAN00095028 Wrong output length returned for AEAD Encrypt
 *                                ESCAN00095033 KeyElementSet with SHE Key update mechanism does not set the new key
 *  02.00.00  2017-08-06  vismwe  ESCAN00095629 Crypto_30_LibCv_KeyElementIdsGet returns E_NOT_OK instead of CRYPTO_E_SMALL_BUFFER
 *                                ESCAN00096063 Crypto_30_LibCv_KeyElementSet calculates wrong values for M4 proof if a she update key flags is set.
 *                                ESCAN00096032 Crypto_30_LibCv_KeyDerive returns suspect values, if the calculation failed.
 *                                ESCAN00095987 Crypto_30_LibCv_KeyElementSet returns always E_NOT_OK for she key update without the key element "proof"
 *                                ESCAN00095985 Compiler error: undeclared identifier for virtual key usage
 *                                ESCAN00095928 Crypto_30_LibCv_KeyExchangeCalcSecret does not reject calls with invalid length of public key
 *                                ESCAN00096031 Crypto_30_LibCv_KeyCopy copies key elements with different lengths without enabled partial access.
 *                                ESCAN00096176 Crypto_30_LibCv_KeyElementGet returns CRYPTO_E_KEY_NOT_VALID instead of CRYPTO_E_KEY_NOT_AVAILABLE
 *  02.01.00  2017-08-15  vistof  ESCAN00096308 MAC verification only done for complete bytes.
 *                        vismaw  STORYC-934    Optimize AES runtime through keeping the SubKey and Roundkeys of former used Keys
 *                        vismwe  ESCAN00096452 Error is returned when IV key element is missing during AES-ECB.
 *                                STORYC-281    Safe BSW Release
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_H)
# define CRYPTO_30_LIBCV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Csm_Types.h"
# include "Crypto_30_LibCv_Cfg.h"
# include "Crypto_30_LibCv_KeyManagement.h"
# include "SchM_Crypto_30_LibCv.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CRYPTO_30_LIBCV_VENDOR_ID                      (30u)
# define CRYPTO_30_LIBCV_MODULE_ID                      (114u)
# define CRYPTO_30_LIBCV_INSTANCE_ID                    (0u)

/* AUTOSAR Software specification version information */
# define CRYPTO_30_LIBCV_AR_RELEASE_MAJOR_VERSION       (0x04u)
# define CRYPTO_30_LIBCV_AR_RELEASE_MINOR_VERSION       (0x03u)
# define CRYPTO_30_LIBCV_AR_RELEASE_REVISION_VERSION    (0x00u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CRYPTO_30_LIBCV_MAJOR_VERSION                  (2u)
# define CRYPTO_30_LIBCV_MINOR_VERSION                  (1u)
# define CRYPTO_30_LIBCV_PATCH_VERSION                  (0u)

/* ----- API service IDs ----- */
# define CRYPTO_30_LIBCV_SID_INIT                       (0x00u) /*!< Service ID: Crypto_30_LibCv_Init() */
# define CRYPTO_30_LIBCV_SID_GET_VERSION_INFO           (0x01u) /*!< Service ID: Crypto_30_LibCv_GetVersionInfo() */
# define CRYPTO_30_LIBCV_SID_PROCESS_JOB                (0x03u) /*!< Service ID: Crypto_30_LibCv_ProcessJob() */
# define CRYPTO_30_LIBCV_SID_CANCEL_JOB                 (0x0Eu) /*!< Service ID: Crypto_30_LibCv_CancelJob() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_SET            (0x04u) /*!< Service ID: Crypto_30_LibCv_KeyElementSet() */
# define CRYPTO_30_LIBCV_SID_KEY_VALID_SET              (0x05u) /*!< Service ID: Crypto_30_LibCv_KeyValidSet() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_GET            (0x06u) /*!< Service ID: Crypto_30_LibCv_KeyElementGet() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_COPY           (0x0Fu) /*!< Service ID: Crypto_30_LibCv_KeyElementCopy() */
# define CRYPTO_30_LIBCV_SID_KEY_COPY                   (0x10u) /*!< Service ID: Crypto_30_LibCv_KeyCopy() */
# define CRYPTO_30_LIBCV_SID_KEY_ELEMENT_IDS_GET        (0x11u) /*!< Service ID: Crypto_30_LibCv_KeyElementIdsGet() */
# define CRYPTO_30_LIBCV_SID_RANDOM_SEED                (0x0Du) /*!< Service ID: Crypto_30_LibCv_RandomSeed() */
# define CRYPTO_30_LIBCV_SID_KEY_GENERATE               (0x07u) /*!< Service ID: Crypto_30_LibCv_KeyGenerate() */
# define CRYPTO_30_LIBCV_SID_KEY_DERIVE                 (0x08u) /*!< Service ID: Crypto_30_LibCv_KeyDerive() */
# define CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_PUB_VAL  (0x09u) /*!< Service ID: Crypto_30_LibCv_KeyExchangeCalcPubVal() */
# define CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_SECRET   (0x0Au) /*!< Service ID: Crypto_30_LibCv_KeyExchangeCalcSecret() */
# define CRYPTO_30_LIBCV_SID_CERTIFICATE_PARSE          (0x0Bu) /*!< Service ID: Crypto_30_LibCv_CertificateParse() */
# define CRYPTO_30_LIBCV_SID_CERTIFICATE_VERIFY         (0x12u) /*!< Service ID: Crypto_30_LibCv_CertificateVerify() */
# define CRYPTO_30_LIBCV_SID_MAIN_FUNCTION              (0x0Cu) /*!< Service ID: Crypto_30_LibCv_MainFunction() */

/* Additional Key Element defines */
# define CRYPTO_KE_CUSTOM_MAC_AES_ROUNDKEY              (0x81u)
# define CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL           (0x82u)

/* ----- Modes ----- */
/* State: Uninitialized */
# define CRYPTO_30_LIBCV_UNINIT                         (0x00u)
/* State: Initialized */
# define CRYPTO_30_LIBCV_INITIALIZED                    (0x01u)

/* Development Error Types [SWS_Crypto_00040] */
# ifndef CRYPTO_E_NO_ERROR /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_NO_ERROR 255u
# endif
# ifndef CRYPTO_E_UNINIT /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_UNINIT 0u
# endif
# ifndef CRYPTO_E_INIT_FAILED /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_INIT_FAILED 1u
# endif
# ifndef CRYPTO_E_PARAM_POINTER /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_PARAM_POINTER 2u
# endif
# ifndef CRYPTO_E_PARAM_HANDLE /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_PARAM_HANDLE 4u
# endif
# ifndef CRYPTO_E_PARAM_VALUE /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
#  define CRYPTO_E_PARAM_VALUE 5u
# endif

/* Masks for KeyElementState */
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_MASK               (0xFFu)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_MASK        (0x04u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK    (0xFBu)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_MASK                (0x02u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INV_MASK            (0xFDu)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK               (0x01u)
# define CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK           (0xFEu)

/* Macros for virtual elements */
# define Crypto_30_LibCv_GetKeyElementVirtualOffset(Index)        (*((uint32*)Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements((Index))))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_SetKeyElementVirtualOffset(Index, Value) (*((uint32*)Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements((Index))))= (Value)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_GetKeyElementVirtualRoot(Index)          (Crypto_30_LibCv_GetIdOfKeyElements(Crypto_30_LibCv_GetKeyElementInfoVirtualIdxOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index))))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_IsKeyElementVirtual(Index)               (Crypto_30_LibCv_IsKeyElementInfoVirtualUsedOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Macros for all elements */
# define Crypto_30_LibCv_IsKeyElementPartial(Index)               (Crypto_30_LibCv_IsPartialOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_GetKeyElementLength(Index)               (Crypto_30_LibCv_GetLengthOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_HasKeyElementInitValue(Index)            (Crypto_30_LibCv_IsInitValueUsedOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_GetKeyElementInitValueLength(Index)      (Crypto_30_LibCv_GetInitValueEndIdxOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements((elementIndex))) - Crypto_30_LibCv_GetInitValueStartIdxOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_IsKeyElementValid(Index)                 ((Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageValidIdxOfKeyElements((Index))) & CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK) ==  CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_SetKeyElementState(Index, Value)         (Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageValidIdxOfKeyElements((Index)), (Value))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_LibCv_GetKeyElementState(Index)                (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageValidIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 ) */
# define Crypto_30_LibCv_ClearKeyElementStateByMask(Index, Mask)  (Crypto_30_LibCv_SetKeyElementState((Index), (uint8)(Crypto_30_LibCv_GetKeyElementState((Index)) & (Mask))))  /* PRQA S 3453 */ /* MD_MSR_19.7 ) */
# define Crypto_30_LibCv_SetKeyElementStateByMask(Index, Mask)    (Crypto_30_LibCv_SetKeyElementState((Index), (uint8)(Crypto_30_LibCv_GetKeyElementState((Index)) | (Mask))))  /* PRQA S 3453 */ /* MD_MSR_19.7 ) */

# define CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
extern VAR(uint8, CRYPTO_30_LIBCV_VAR_ZERO_INIT) Crypto_30_LibCv_ModuleInitialized;
# endif

# define CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
*  Crypto_30_LibCv_Init()
*********************************************************************************************************************/
/*! \brief      Initializes the Crypto Driver
*  \details     This function initializes the module Crypto_30_LibCv. It initializes all variables and sets the
*               module state to initialized.
*  \pre         Interrupts are disabled.
*  \pre         Module is uninitialized.
*  \context     TASK
*  \reentrant   FALSE
*  \synchronous TRUE
*  \note        Specification of module initialization
*  \trace       CREQ-131331
*********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init(void);

/**********************************************************************************************************************
*  Crypto_30_LibCv_InitMemory()
*********************************************************************************************************************/
/*! \brief      The function initializes variables, which cannot be initialized with the startup code.
*  \details     Initialize component variables at power up.
*  \pre         Module is uninitialized.
*  \context     TASK
*  \reentrant   FALSE
*  \synchronous TRUE
*********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitMemory(void);

# if (CRYPTO_30_LIBCV_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Crypto_30_LibCv_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-131329
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) versioninfo);
# endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Processes the received job
 *  \details       Performs the crypto primitive, that is configured in the job parameter.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with job and
 *                                         primitive relevant information but also pointer to result buffers.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \return        CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \trace         CREQ-131340
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ProcessJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_CancelJob()
 *********************************************************************************************************************/
/*! \brief         Cancels the received job
 *  \details       This interface removes the provided job from the queue and cancels the processing of the job if possible.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful, job has been removed.
 *  \return        E_NOT_OK                Request failed, job could not be removed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CancelJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*!
* \exclusivearea CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0
* Ensures consistency of global RAM variables.
* \protects DriverObjectState, Queue and Lock variables.
* \usedin Crypto_30_LibCv_MainFunction, Crypto_30_LibCv_CancelJob, Crypto_30_LibCv_ProcessJob
* \exclude All functions provided by Crypto_30_LibCv.
* \length SHORT in Crypto_30_LibCv_MainFunction, Crypto_30_LibCv_ProcessJob and Crypto_30_LibCv_CancelJob.
* \endexclusivearea

* \exclusivearea CRYPTO_30_LIBCV_EXCLUSIVE_AREA_1
* Ensures consistency of global RAM variables. Only necessary if 32-bit access of variables is non-atomic.
* \protects Lock variables.
* \usedin Crypto_30_LibCv_Process
* \exclude All functions provided by Crypto_30_LibCv.
* \length SHORT as only one operation is secured.
* \endexclusivearea

* \exclusivearea CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2
* Ensures consistency of reading key elements against a parallel key manipulation.
* \protects Key reading.
* \usedin Crypto_30_LibCv_Process, Crypto_30_LibCv_KeyElementGet, Crypto_30_LibCv_KeyDerive, Crypto_30_LibCv_KeyExchangeCalcSecret
* \exclude Crypto_30_LibCv_KeyElementSet, Crypto_30_LibCv_KeyCopy, Crypto_30_LibCv_KeyElementCopy, Crypto_30_LibCv_KeyDerive, Crypto_30_LibCv_KeyGenerate, Crypto_30_LibCv_KeyExchangeCalcPubVal, Crypto_30_LibCv_KeyExchangeCalcSecret
* \length LONG in all functions since key is copied and time depends on the length of the key.
* \endexclusivearea
*/

#endif /* CRYPTO_30_LIBCV_H */
/**********************************************************************************************************************
 *  END OF FILE: CSM.H
 *********************************************************************************************************************/
