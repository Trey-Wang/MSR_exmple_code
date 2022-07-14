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
/**        \file  CryIf.h
 *        \brief  MICROSAR Crypto Interface (CRYIF)
 *
 *      \details  The Crypto Interface module provides a unique interface to manage different Crypto HW and SW
 *                solutions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Markus Schneider              vismss        Vector Informatik GmbH
 *  Philipp Ritter                visrpp        Vector Informatik GmbH
 *  Anant Gupta                   visgut        Vector Informatik GmbH
 *  Mathias Waldenburger          vismaw        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-12-01  vismss  -             Initial creation of Crypto Interface
 *  01.00.01  2016-12-06  visrpp  ESCAN00093184 Renamed CryIf_KeyValidSet to CryIf_KeySetValid
 *  01.00.02  2016-12-16  vismss  ESCAN00093334 Adapted CryIf_CallbackNotification
 *  01.00.03  2017-01-24  vismss  ESCAN00093699 Correction of error and service id codes
 *  01.01.00  2017-02-14  visgut  FEATC-815     Support CRYIF
 *                                              Support CryIf_CancelJob
 *                                              Fix Reference of Driver Object
 *  01.02.00  2017-05-05  visrpp  FEATC-1228    FEAT-2902 Release of AR4.3 CryIf
 *  02.00.00  2017-05-12  vismaw  ESCAN00095159 Validity of cryIfKeyId is not checked
 *                                              Silent BSW
 *********************************************************************************************************************/

#if !defined (CRYIF_H)
# define CRYIF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm_Types.h"
# include "CryIf_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CRYIF_VENDOR_ID                           (30u)
# define CRYIF_MODULE_ID                           (112u)

/* AUTOSAR Software specification version information */
# define CRYIF_AR_RELEASE_MAJOR_VERSION            (4u)
# define CRYIF_AR_RELEASE_MINOR_VERSION            (3u)
# define CRYIF_AR_RELEASE_REVISION_VERSION         (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CRYIF_SW_MAJOR_VERSION                    (2u)
# define CRYIF_SW_MINOR_VERSION                    (0u)
# define CRYIF_SW_PATCH_VERSION                    (0u)

# define CRYIF_INSTANCE_ID_DET                     (0x00u)

/* ----- API service IDs ----- */
# define CRYIF_SID_INIT                            (0x00u) /*!< Service ID: CryIf_Init() */
# define CRYIF_SID_GET_VERSION_INFO                (0x01u) /*!< Service ID: CryIf_GetVersionInfo() */
# define CRYIF_SID_PROCESS_JOB                     (0x02u) /*!< Service ID: CryIf_ProcessJob() */
# define CRYIF_SID_CANCEL_JOB                      (0x03u) /*!< Service ID: CryIf_CancelJob() */
# define CRYIF_SID_KEY_ELEMENT_SET                 (0x04u) /*!< Service ID: CryIf_KeyElementSet() */
# define CRYIF_SID_KEY_SET_VALID                   (0x05u) /*!< Service ID: CryIf_KeySetValid() */
# define CRYIF_SID_KEY_ELEMENT_GET                 (0x06u) /*!< Service ID: CryIf_KeyElementGet() */
# define CRYIF_SID_KEY_ELEMENT_COPY                (0x0fu) /*!< Service ID: CryIf_KeyElementCopy() */
# define CRYIF_SID_KEY_COPY                        (0x10u) /*!< Service ID: CryIf_KeyCopy() */
# define CRYIF_SID_RANDOM_SEED                     (0x07u) /*!< Service ID: CryIf_RandomSeed() */
# define CRYIF_SID_KEY_GENERATE                    (0x08u) /*!< Service ID: CryIf_KeyGenerate() */
# define CRYIF_SID_KEY_DERIVE                      (0x09u) /*!< Service ID: CryIf_KeyDerive() */
# define CRYIF_SID_KEY_EXCHANGE_CALC_PUB_VAL       (0x0au) /*!< Service ID: CryIf_KeyExchangeCalcPubVal() */
# define CRYIF_SID_KEY_EXCHANGE_CALC_SECRET        (0x0bu) /*!< Service ID: CryIf_KeyExchangeCalcSecret() */
# define CRYIF_SID_CERTIFICATE_PARSE               (0x0cu) /*!< Service ID: CryIf_CertificateParse() */
# define CRYIF_SID_CERTIFICATE_VERIFY              (0x11u) /*!< Service ID: CryIf_CertificateVerify() */

/* ----- Error codes ----- */
# define CRYIF_E_NO_ERROR                          (0xFFu) /*!< used to check if no error occurred - use a value unequal to any error code */
# define CRYIF_E_UNINIT                            (0x00u) /*!< Error code: API service used without module initialization */
# define CRYIF_E_INIT_FAILED                       (0x01u) /*!< Error code: Initialisation of CRYIF module failed */
# define CRYIF_E_PARAM_POINTER                     (0x02u) /*!< Error code: API request called with invalid parameter (null pointer) */
# define CRYIF_E_PARAM_HANDLE                      (0x03u) /*!< Error code: API request called with invalid parameter (out of range) */
# define CRYIF_E_PARAM_VALUE                       (0x04u) /*!< Error code: API request called with invalid parameter (invalid value) */
# define CRYIF_E_ALREADY_INITIALIZED               (0x11u) /*!< Error code: The service CryIf_Init() is called while the module is already initialized */

/* ----- Modes ----- */
# define CRYIF_UNINIT                              (0x00u) /*!< State: Module was not initialized */
# define CRYIF_INIT                                (0x01u) /*!< State: Module is initialized */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CryIf_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Power-up memory initialization.
 *  \details     Initialize component variables at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *  \trace       CREQ-127559
 *********************************************************************************************************************/
FUNC(void, CRYIF_CODE) CryIf_InitMemory(void);

/**********************************************************************************************************************
 *  CryIf_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function.
 *  \details     This function initializes the module CryIf. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   ConfigPtr               Configuration structure for initializing the module
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         CryIf_InitMemory has been called unless CryIf_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace       CREQ-127559
 *********************************************************************************************************************/
FUNC(void, CRYIF_CODE) CryIf_Init(void);

# if (CRYIF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CryIf_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     CryIf_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-127557
 *********************************************************************************************************************/
FUNC(void, CRYIF_CODE) CryIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_VAR) versioninfo);
# endif

/**********************************************************************************************************************
 *  CryIf_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Process the received job.
 *  \details       This interface dispatches the received jobs to the configured crypto driver object.
 *  \param[in]     channelId               Holds the identifier of the crypto channel.
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
 *  \trace         CREQ-127560
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_ProcessJob(uint32 channelId,
                                                  P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job);

/**********************************************************************************************************************
 *  CryIf_CancelJob()
 *********************************************************************************************************************/
/*! \brief         Cancels the received job.
 *  \details       This interface removes the provided job from the underlying Crypto Driver Object queue.
 *  \param[in]     channelId               Holds the identifier of the crypto channel.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful, job has been removed.
 *  \return        E_NOT_OK                Request failed, job could not be removed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127561
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CancelJob(uint32 channelId,
                                                 P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job);

/**********************************************************************************************************************
 *  CryIf_KeyElementSet()
 *********************************************************************************************************************/
/*! \brief         Sets a key element.
 *  \details       This function shall dispatch the set key element function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *  \return        CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *  \return        CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127562
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementSet(uint32 cryIfKeyId,
                                                     uint32 keyElementId,
                                                     P2CONST(uint8, AUTOMATIC, CRYIF_APPL_VAR) keyPtr,
                                                     uint32 keyLength);

/**********************************************************************************************************************
 *  CryIf_KeySetValid()
 *********************************************************************************************************************/
/*! \brief         Sets the key to valid.
 *  \details       This function shall dispatch the set key valid function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key whose key elements shall be set to valid.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127563
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeySetValid(uint32 cryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_KeyElementGet()
 *********************************************************************************************************************/
/*! \brief         Exports the key element.
 *  \details       This function shall dispatch the get key element function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *  \return        CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127564
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementGet(uint32 cryIfKeyId,
                                                     uint32 keyElementId,
                                                     P2VAR(uint8, AUTOMATIC, CRYIF_APPL_VAR) resultPtr,
                                                     P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  CryIf_KeyElementCopy()
 *********************************************************************************************************************/
/*! \brief         Copy key element.
 *  \details       This function shall copy a key element from one key to a target key.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be the source for the
 *                                         copy operation.
 *  \param[in]     targetCryIfKeyId        Holds the identifier of the key whose key element shall be the destination
 *                                         element.
 *  \param[in]     targetKeyElementId      Holds the identifier of the key element which shall be the destination for
 *                                         the copy operation.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *  \return        CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *  \return        CRYPTO_E_KEY_EXTRACT_DENIED  Request failed, not allowed to extract key material.
 *  \return        CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *  \return        CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element sizes are not compatible.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127565
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopy(uint32 cryIfKeyId,
                                                      uint32 keyElementId,
                                                      uint32 targetCryIfKeyId,
                                                      uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  CryIf_KeyCopy()
 *********************************************************************************************************************/
/*! \brief         Copy the key
 *  \details       This function shall copy all key elements from the source key to a target key.
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
 *  \trace         CREQ-127566
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyCopy(uint32 cryIfKeyId,
                                               uint32 targetCryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_RandomSeed()
 *********************************************************************************************************************/
/*! \brief         Initialize the seed
 *  \details       This function shall dispatch the random seed function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key for which a new material shall be generated.
 *  \param[in]     seedPtr                 Holds a pointer to the memory location which contains the data to feed the
 *                                         seed.
 *  \param[in]     seedLength              Contains the length of the seed in bytes.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127567
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_RandomSeed(uint32 cryIfKeyId,
                                                  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_VAR) seedPtr,
                                                  uint32 seedLength);

/**********************************************************************************************************************
 *  CryIf_KeyGenerate()
 *********************************************************************************************************************/
/*! \brief         Generates a key.
 *  \details       This function shall dispatch the key generate function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key which is to be updated with the generated
 *                                         value.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127568
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyGenerate(uint32 cryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_KeyDerive()
 *********************************************************************************************************************/
/*! \brief         Derives a key.
 *  \details       This function shall dispatch the key derive function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryIfKeyId        Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127569
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyDerive(uint32 cryIfKeyId,
                                                 uint32 targetCryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*! \brief         Calculation of the public value.
 *  \details       This function shall dispatch the key exchange public value calculation function to the configured
 *                 crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127570
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcPubVal(uint32 cryIfKeyId,
                                                             P2VAR(uint8, AUTOMATIC, CRYIF_APPL_VAR) publicValuePtr,
                                                             P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  CryIf_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret.
 *  \details       This function shall dispatch the key exchange common shared secret calculation function to the
 *                 configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPublicValuePtr   Holds the pointer to the memory location which contains the partners
 *                                         public value.
 *  \param[in]     partnerPublicValueLength Contains the length of the partners public value in bytes.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127571
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcSecret(uint32 cryIfKeyId,
                                                             P2CONST(uint8, AUTOMATIC, CRYIF_APPL_VAR) partnerPublicValuePtr,
                                                             uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  CryIf_CertificateParse()
 *********************************************************************************************************************/
/*! \brief         Parse stored certificate.
 *  \details       This function shall dispatch the certificate parse function to the configured crypto driver object.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key slot in which the certificate has been
 *                                         stored.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127572
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateParse(uint32 cryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_CertificateVerify()
 *********************************************************************************************************************/
/*! \brief         Certificate verification.
 *  \details       Verifies the certificate stored in the key referenced by verifyCryptoKeyId with the certificate
 *                 stored in the key referenced by cryIfKeyId.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key which shall be used to validate the
 *                                         certificate.
 *  \param[in]     verifyCryIfKeyId        Holds the identifier of the key containing the certificate, which shall be
 *                                         verified.
 *  \param[out]    verifyPtr               Holds a pointer to the memory location which will contain the result of the
 *                                         certificate verification.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-127573
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateVerify(uint32 cryIfKeyId,
                                                         uint32 verifyCryIfKeyId,
                                                         P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_VAR) verifyPtr);

# define CRYIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* CRYIF_H */

/**********************************************************************************************************************
 *  END OF FILE: CryIf.h
 *********************************************************************************************************************/
