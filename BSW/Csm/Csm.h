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
/*!        \file  Csm.h
 *        \brief  MICROSAR Crypto Service Manager (CSM)
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials       Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Markus Schneider              vismss         Vector Informatik GmbH
 *  Anant Gupta                   visgut         Vector Informatik GmbH
 *  Mathias Waldenburger          vismaw         Vector Informatik GmbH
 *  Philipp Ritter                visrpp         Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-10-31  vismss                Major rework for ASR 4.3
 *  01.00.01  2016-12-06  vismss                Adaption to the specification
 *  01.00.02  2016-12-16  vismss  ESCAN00093323 Correction of Csm_RandomGenerate
 *  01.00.03  2017-01-04  vismss  ESCAN00093383 Encapsulation of RTE related types
 *  01.00.04  2017-01-12  vismss  ESCAN00093703 Adaption of Csm_Types.h to the specification
 *  01.01.00  2017-02-16  visgut  FEATC-814     Support CSM 4.3
 *                                              Introduce Exclusive Areas to protect queue ressources
 *                                              Add missing DET Checks
 *                                              Support Csm_CancelJob
 *                                              Remove lower layer queue handling structures from Csm_Types.h
 *                                              Remove declaration of Csm_Mainfunction as will be declared in SchM_Csm.h
 *  01.02.00  2017-05-05  vismss  FEATC-1227    Added/Corrected Doxygen comments
 *                                              Adapted code to MISRA
 *                                              Adapted null pointer checks to operation modes
 *                                              Improved job queueing
 *  01.02.01  2017-05-25  vismss  ESCAN00095301 Correction of DET null pointer exceptions
 *  01.03.00  2017-06-08  vismaw  FEAT-2500     SafeBSW for CSM 4.3
 *                                ESCAN00095452 Job sorting is only triggered during MainFunction
 *  02.03.00  2017-08-22  visrpp  ESCAN00096371 After cancellation, job is executed anyway or ECU results in 
 *                                              unexpected behavior
 *                                ESCAN00096373 Synchronous job is executed even if there were asynchronous jobs with 
 *                                              higher priority queued before
 *                                ESCAN00096489 Callbacks are not notified to the RTE
 *********************************************************************************************************************/

#if !defined (CSM_H)
# define CSM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CSM_VENDOR_ID                           (30u)
# define CSM_MODULE_ID                           (110u)
# define CSM_INSTANCE_ID                         (0u)

/* AUTOSAR Software specification version information */
# define CSM_AR_RELEASE_MAJOR_VERSION            (0x04u)
# define CSM_AR_RELEASE_MINOR_VERSION            (0x03u)
# define CSM_AR_RELEASE_REVISION_VERSION         (0x00u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CSM_SW_MAJOR_VERSION                    (2u)
# define CSM_SW_MINOR_VERSION                    (3u)
# define CSM_SW_PATCH_VERSION                    (0u)

/* ----- API service IDs ----- */
/* Service ID: Csm_Init() */
# define CSM_INIT_ID                             (0x00u)
/* Service ID: Csm_MainFunction() */
# define CSM_MAINFUNCTION_ID                     (0x01u)
/* Service ID: Csm_GetVersionInfo() */
# define CSM_GETVERSIONINFO_ID                   (0x3Bu)
/* Service ID: Csm_KeyElementSet() */
# define CSM_KEYELEMENTSET_ID                    (0x78u)
/* Service ID: Csm_KeySetValid() */
# define CSM_KEYSETVALID_ID                      (0x67u)
/* Service ID: Csm_KeyElementGet() */
# define CSM_KEYELEMENTGET_ID                    (0x68u)
/* Service ID: Csm_KeyElementCopy() */
# define CSM_KEYELEMENTCOPY_ID                   (0x71u)
/* Service ID: Csm_KeyCopy() */
# define CSM_KEYCOPY_ID                          (0x73u)
/* Service ID: Csm_RandomSeed() */
# define CSM_RANDOMSEED_ID                       (0x69u)
/* Service ID: Csm_KeyGenerate() */
# define CSM_KEYGENERATE_ID                      (0x6Au)
/* Service ID: Csm_KeyDerive() */
# define CSM_KEYDERIVE_ID                        (0x6Bu)
/* Service ID: Csm_KeyExchangeCalcPubVal() */
# define CSM_KEYEXCHANGECALCPUBVAL_ID            (0x6Cu)
/* Service ID: Csm_KeyExchangeCalcSecret() */
# define CSM_KEYEXCHANGECALCSECRET_ID            (0x6Du)
/* Service ID: Csm_CertificateParse() */
# define CSM_CERTIFICATEPARSE_ID                 (0x6Eu)
/* Service ID: Csm_CertificateVerify() */
# define CSM_CERTIFICATEVERIFY_ID                (0x74u)
/* Service ID: Csm_Hash() */
# define CSM_HASH_ID                             (0x5Du)
/* Service ID: Csm_MacGenerate() */
# define CSM_MACGENERATE_ID                      (0x60u)
/* Service ID: Csm_MacVerify() */
# define CSM_MACVERIFY_ID                        (0x61u)
/* Service ID: Csm_Encrypt() */
# define CSM_ENCRYPT_ID                          (0x5Eu)
/* Service ID: Csm_Decrypt() */
# define CSM_DECRYPT_ID                          (0x5Fu)
/* Service ID: Csm_AEADEncrypt() */
# define CSM_AEADENCRYPT_ID                      (0x62u)
/* Service ID: Csm_AEADDecrypt() */
# define CSM_AEADDECRYPT_ID                      (0x63u)
/* Service ID: Csm_SignatureGenerate() */
# define CSM_SIGNATUREGENERATE_ID                (0x76u)
/* Service ID: Csm_SignatureVerify() */
# define CSM_SIGNATUREVERIFY_ID                  (0x64u)
/* Service ID: Csm_SecureCounterIncrement() */
# define CSM_SECURECOUNTERINCREMENT_ID           (0x65u)
/* Service ID: Csm_SecureCounterRead() */
# define CSM_SECURECOUNTERREAD_ID                (0x66u)
/* Service ID: Csm_RandomGenerate() */
# define CSM_RANDOMGENERATE_ID                   (0x72u)
/* Service ID: Csm_CancelJob() */
# define CSM_CANCELJOB_ID                        (0x6Fu)
/* Service ID: Csm_CallbackNotification() */
# define CSM_CALLBACKNOTIFICATION_ID             (0x70u)

/* ----- Error codes ----- */
/* Error Code: Used to check if no error occurred */
# define CSM_E_NO_ERROR                          (0x00u)
/* Error Code: API request called with invalid parameter (Nullpointer) */
# define CSM_E_PARAM_POINTER                     (0x01u)
/* Error Code: API request called before initialization of CSM module */
# define CSM_E_UNINIT                            (0x05u)
/* Error Code: Initialization of CSM module failed */
# define CSM_E_INIT_FAILED                       (0x07u)
/* Error Code: Requested service is not initialized */
# define CSM_E_SERVICE_NOT_STARTED               (0x09u)
/* Error Code: API request called with invalid parameter (invalid method for selected service) */
# define CSM_E_PARAM_METHOD_INVALID              (0x03u)
/* Error code: The service Csm_Init() is called while the module is already initialized */
# define CSM_E_ALREADY_INITIALIZED               (0x11u)

/* ----- Modes ----- */
/* State: Uninitialized */
# define CSM_UNINIT                              (0x00u)
/* State: Initialized */
# define CSM_INITIALIZED                         (0x01u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CSM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* General Function Prototypes */
/**********************************************************************************************************************
 *  Csm_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the Csm.
 *  \details     Set all service states to initial idle.
 *  \pre         None
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-129270
 *  \trace       SPEC-2820365
 *********************************************************************************************************************/
FUNC( void, CSM_CODE)  Csm_Init( void );

/**********************************************************************************************************************
 *  Csm_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Power-up memory initialization.
 *  \details     Initialize component variables at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *********************************************************************************************************************/
FUNC( void, CSM_CODE ) Csm_InitMemory( void );

/**********************************************************************************************************************
 *  Csm_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Implements the API to be called cyclically to process the requested services.
 *  \details     Calls the configured main function of the specific Cry service if requested.
 *  \pre         Csm is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-2820481
 *********************************************************************************************************************/

# if ( CSM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  Csm_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Retrieves the version information of the Csm module.
 *  \details     Stores version information, i.e. Module Id, Vendor Id, Vendor specific version numbers to structure
 *               pointed by versioninfo.
 *  \param[out]  versioninfo
 *  \pre         GetVersionInfo API is enabled via pre-compile configuration.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-129269
 *  \trace       SPEC-2820367
 *********************************************************************************************************************/
FUNC( void, CSM_CODE ) Csm_GetVersionInfo( P2VAR( Std_VersionInfoType, AUTOMATIC, CSM_APPL_VAR ) versioninfo );
# endif

/**********************************************************************************************************************
 *  Csm_CancelJob()
 *********************************************************************************************************************/
/*! \brief         Cancels the given job
 *  \details       Removes the job in the Csm Queue and calls the job's callback with the result
                   CRYPTO_E_JOB_CANCELED. It also passes the cancellation command to the
                   CryIf to try to cancel the job in the Crypto Driver.
 *  \param[in]     jobId        Holds the identifier of the job using the CSM service.
 *  \param[in]     mode         Not used, just for interface compatibility provided
 *  \return        E_OK                         Request successful
 *                 E_NOT_OK                     Request failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-129244
 *  \trace         SPEC-2820473, SPEC-2820474, SPEC-2820475
 *********************************************************************************************************************/
FUNC( Std_ReturnType, CSM_CODE ) Csm_CancelJob(uint32 jobId, Crypto_OperationModeType mode);

/* Key Management Function Prototypes */
/**********************************************************************************************************************
 *  Csm_KeyElementSet()
 *********************************************************************************************************************/
/*! \brief         Sets the given key element bytes to the key identified by keyId.
 *  \details       -
 *  \param[in]     keyId                   Holds the identifier of the key for which a new material shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element to be written.
 *  \param[in]     keyPtr                  Holds the pointer to the key element bytes to be processed.
 *  \param[in]     keyLength               Contains the number of key element bytes.
 *  \return        E_OK                    Request successful
 *                 E_NOT_OK                Request failed
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed because write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element size does not match size of provided data.
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820439, SPEC-2820440
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet(uint32 keyId,
                                                 uint32 keyElementId,
                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                 uint32 keyLength);

/**********************************************************************************************************************
 *  Csm_KeySetValid()
 *********************************************************************************************************************/
/*! \brief         Sets the key state of the key identified by keyId to valid.
 *  \details       -
 *  \param[in]     keyId        Holds the identifier of the key for which a new material shall be validated.
 *  \return        E_OK         Request successful
 *                 E_NOT_OK     Request failed
 *                 CRYPTO_E_BUSY Request failed, Crypto Driver Object is busy
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820441, SPEC-2820442
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValid(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_KeyElementGet()
 *********************************************************************************************************************/
/*! \brief         Retrieves the key element bytes from a specific key element of the key identified by the keyId and
 *                 stores the key element in the memory location pointed by the key pointer.
 *  \details       -
 *  \param[in]     keyId                   Holds the identifier of the key from which a key element shall be extracted.
 *  \param[in]     keyElementId            Holds the identifier of the key element to be extracted.
 *  \param[out]    keyPtr                  Holds the pointer to the memory location where the key shall be copied to.
 *  \param[in,out] keyLengthPtr            Holds a pointer to the memory location in which the output buffer length in
 *                                         bytes is stored. On calling this function, this parameter shall contain the
 *                                         buffer length in bytes of the keyPtr. When the request has finished, the
 *                                         actual size of the written input bytes shall be stored.
 *  \return        E_OK                    Request successful
 *                 E_NOT_OK                Request failed
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed because read access was denied
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element size does not match size of provided data.
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820443, SPEC-2820444
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet(uint32 keyId,
                                                 uint32 keyElementId,
                                                 P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                 P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyElementCopy()
 *********************************************************************************************************************/
/*! \brief         This function shall copy a key elements from one key to a target key.
 *  \details       -
 *  \param[in]     keyId                   Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be the source for the
 *                                         copy operation.
 *  \param[in]     targetKeyId             Holds the identifier of the key whose key element shall be the destination
 *                                         element.
 *  \param[in]     targetKeyElementId      Holds the identifier of the key element which shall be the destination for
 *                                         the copy operation.
 *  \return        E_OK                    Request successful
 *                 E_NOT_OK                Request failed
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed because read access was denied
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed because write access was denied
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element size does not match size of provided data.
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820451, SPEC-2820452
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy(uint32 keyId,
                                                  uint32 keyElementId,
                                                  uint32 targetKeyId,
                                                  uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  Csm_KeyCopy()
 *********************************************************************************************************************/
/*! \brief         This function shall copy all key elements from the source key to a target key.
 *  \details       -
 *  \param[in]     keyId                   Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     targetKeyId             Holds the identifier of the key whose key element shall be the destination
 *                                         element.
 *  \return        E_OK                    Request successful
 *                 E_NOT_OK                Request failed
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed because read access was denied
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed because write access was denied
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element size does not match size of provided data.
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820453, SPEC-2820454
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy(uint32 keyId,
                                           uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_RandomSeed()
 *********************************************************************************************************************/
/*! \brief         This function shall dispatch the random seed function to the configured crypto driver object.
 *  \details       -
 *  \param[in]     keyId        Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedPtr      Holds a pointer to the memory location which contains the data to feed the seed.
 *  \param[in]     seedLength   Contains the length of the seed in bytes.
 *  \return        E_OK         Request successful
 *                 E_NOT_OK     Request failed
 *                 CRYPTO_E_BUSY Request failed, Crypto Driver Object is busy
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820455, SPEC-2820456
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeed(uint32 keyId,
                                              P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                              uint32 seedLength);

/**********************************************************************************************************************
 *  Csm_KeyGenerate()
 *********************************************************************************************************************/
/*! \brief         Generates new key material and store it in the key identified by keyId.
 *  \details       -
 *  \param[in]     keyId        Holds the identifier of the key for which a new material shall be generated.
 *  \return        E_OK         Request successful
 *                 E_NOT_OK     Request failed
 *                 CRYPTO_E_BUSY Request failed, Crypto Driver Object is busy
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820460, SPEC-2820461
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_KeyDerive()
 *********************************************************************************************************************/
/*! \brief         Derives a new key by using the key elements in the given key identified by the keyId. The given key
 *                 contains the key elements for the password and salt. The derived key is stored in the key element
 *                 with the id 1 of the key identified by targetCryptoKeyId.
 *  \details       -
 *  \param[in]     keyId        Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetKeyId  Holds the identifier of the key which is used to store the derived key.
 *  \return        E_OK         Request successful
 *                 E_NOT_OK     Request failed
 *                 CRYPTO_E_BUSY Request failed, Crypto Driver Object is busy
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820462, SPEC-2820463, SPEC-2820464
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive(uint32 keyId,
                                             uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*! \brief         Calculates the public value of the current user for the key exchange and stores the public key in
 *                 the memory location pointed by the public value pointer.
 *  \details       -
 *  \param[in]     keyId                   Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful
 *                 E_NOT_OK                Request failed
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_SMALL_BUFFER   Request failed because the provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820465, SPEC-2820466
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal(uint32 keyId,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*! \brief         Calculates the shared secret key for the key exchange with the key material of the key identified by
 *                 the keyId and the partner public key. The shared secret key is stored as a key element in the same
 *                 key.
 *  \details       -
 *  \param[in]     keyId                    Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partner's
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Contains the length of the partner's public value in bytes.
 *  \return        E_OK                     Request successful
 *                 E_NOT_OK                 Request failed
 *                 CRYPTO_E_BUSY            Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_SMALL_BUFFER    Request failed because the provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820467, SPEC-2820468
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret(uint32 keyId,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                         uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_CertificateParse()
 *********************************************************************************************************************/
/*! \brief         This function shall dispatch the certificate parse function to the CRYIF.
 *  \details       -
 *  \param[in]     keyId        Holds the identifier of the key to be used for the certificate parsing.
 *  \return        E_OK         Request successful
 *                 E_NOT_OK     Request failed
 *                 CRYPTO_E_BUSY Request failed, Crypto Driver Object is busy
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820469, SPEC-2820470
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_CertificateVerify()
 *********************************************************************************************************************/
/*! \brief         Verifies the certificate stored in the key referenced by verifyKeyId with the certificate stored in
 *                 the key referenced by keyId. Note: Only certificates stored in the same Crypto Driver can be
 *                 verified against each other. If the key element CRYPTO_KE_CERTIFICATE_CURRENT_TIME is used for the
 *                 verification of the validity period of the certificate indentified by verifyKeyId, it shall have the
 *                 same format as the timestamp in the certificate.
 *  \details       -
 *  \param[in]     keyId        Holds the identifier of the key which shall be used to validate the certificate.
 *  \param[in]     verifyKeyId  Holds the identifier of the key containing the certificate to be verified.
 *  \param[out]    verifyPtr    Holds a pointer to the memory location which will contain the result of the certificate
 *                              verification.
 *  \return        E_OK         Request successful
 *                 E_NOT_OK     Request failed
 *                 CRYPTO_E_BUSY Request failed, Crypto Driver Object is busy
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same keyId
 *  \synchronous   TRUE
 *  \pre           -
 *  \trace         SPEC-2820471, SPEC-2820472
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify(uint32 keyId,
                                                     uint32 verifyKeyId,
                                                     P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/* CSM Service Function Prototypes */
/**********************************************************************************************************************
 *  Csm_Hash()
 *********************************************************************************************************************/
/*! \brief         This interface shall be used for the hash computation service.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data for which the hash shall be computed.
 *  \param[in]     dataLength         Contains the number of bytes to be hashed.
 *  \param[out]    resultPtr          Contains the pointer to the data where the hash value shall be stored.
 *  \param[in,out] resultLengthPtr    Holds a pointer to the memory location in which the output length in bytes is
 *                                    stored. On calling this function, this parameter shall contain the size of the
 *                                    buffer provided by resultPtr. When the request has finished, the actual length of
 *                                    the returned value shall be stored.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129268
 *  \trace         SPEC-2820368, SPEC-2820369
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_Hash(uint32 jobId,
                                        Crypto_OperationModeType mode,
                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                        uint32 dataLength,
                                        P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                        P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerate()
 *********************************************************************************************************************/
/*! \brief         Uses the given data to perform a MAC generation and stores the MAC in the memory location pointed to
 *                 by the MAC pointer.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data for which the MAC shall be computed.
 *  \param[in]     dataLength         Contains the number of bytes for the MAC generation.
 *  \param[out]    macPtr             Contains the pointer to the data where the MAC shall be stored.
 *  \param[in,out] macLengthPtr       Holds a pointer to the memory location in which the output length in bytes is
 *                                    stored. On calling this function, this parameter shall contain the size of the
 *                                    buffer provided by macPtr. When the request has finished, the actual length of
 *                                    the returned MAC shall be stored.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129267
 *  \trace         SPEC-2820376, SPEC-2820377
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                               uint32 dataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacVerify()
 *********************************************************************************************************************/
/*! \brief         Verifies the given MAC by comparing if the MAC is generated with the given data.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data for which the MAC shall be verified.
 *  \param[in]     dataLength         Contains the number of data bytes for which the MAC shall be verified.
 *  \param[in]     macPtr             Holds a pointer to the MAC to be verified.
 *  \param[in]     macLength          Contains the MAC length in BITS to be verified.
 *  \param[out]    verifyPtr          Holds a pointer to the memory location, which will hold the result of the MAC
 *                                    verification.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129266
 *  \trace         SPEC-2820381, SPEC-2820382
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify(uint32 jobId,
                                             Crypto_OperationModeType mode,
                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                             uint32 dataLength,
                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                             uint32 macLength,
                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_Encrypt()
 *********************************************************************************************************************/
/*! \brief         Encrypts the given data and store the ciphertext in the memory location pointed by the result
 *                 pointer.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data to be encrypted.
 *  \param[in]     dataLength         Contains the number of bytes to encrypt.
 *  \param[out]    resultPtr          Contains the pointer to the data where the encrypted data shall be stored.
 *  \param[in,out] resultLengthPtr    Holds a pointer to the memory location in which the output length information is
 *                                    stored in bytes. On calling this function, this parameter shall contain the size
 *                                    of the buffer provided by resultPtr. When the request has finished, the actual
 *                                    length of the returned value shall be stored.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129265
 *  \trace         SPEC-2820386, SPEC-2820387
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt(uint32 jobId,
                                           Crypto_OperationModeType mode,
                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                           uint32 dataLength,
                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_Decrypt()
 *********************************************************************************************************************/
/*! \brief         Decrypts the given encrypted data and store the decrypted plaintext in the memory location pointed
 *                 by the result pointer.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data to be decrypted.
 *  \param[in]     dataLength         Contains the number of bytes to decrypt.
 *  \param[out]    resultPtr          Contains the pointer to the data where the decrypted data shall be stored.
 *  \param[in,out] resultLengthPtr    Holds a pointer to the memory location in which the output length information is
 *                                    stored in bytes. On calling this function, this parameter shall contain the size
 *                                    of the buffer provided by resultPtr. When the request has finished, the actual
 *                                    length of the returned value shall be stored.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129264
 *  \trace         SPEC-2820388, SPEC-2820389
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt(uint32 jobId,
                                           Crypto_OperationModeType mode,
                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                           uint32 dataLength,
                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncrypt()
 *********************************************************************************************************************/
/*! \brief         Uses the given input data to perform a AEAD encryption and stores the ciphertext and the MAC in the
 *                 memory locations pointed by the ciphertext pointer and Tag pointer.
 *  \details       -
 *  \param[in]     jobId                Holds the identifier of the job using the CSM service.
 *  \param[in]     mode                 Indicates which operation mode(s) to perfom.
 *  \param[in]     plaintextPtr         Contains the pointer to the data to be encrypted.
 *  \param[in]     plaintextLength      Contains the number of bytes to encrypt.
 *  \param[in]     associatedDataPtr    Contains the pointer to the associated data.
 *  \param[in]     associatedDataLength Contains the number of bytes of the associated data.
 *  \param[out]    ciphertextPtr        Contains the pointer to the data where the encrypted data shall be stored.
 *  \param[in,out] ciphertextLengthPtr  Holds a pointer to the memory location in which the output length in bytes of
 *                                      the ciphertext is stored. On calling this function, this parameter shall
 *                                      contain the size of the buffer in bytes provided by resultPtr. When the request
 *                                      has finished, the actual length of the returned value shall be stored.
 *  \param[out]    tagPtr               Contains the pointer to the data where the Tag shall be stored.
 *  \param[in,out] tagLengthPtr         Holds a pointer to the memory location in which the output length in bytes of
 *                                      the Tag is stored. On calling this function, this parameter shall contain the
 *                                      size of the buffer in bytes provided by resultPtr. When the request has
 *                                      finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                 Request successful
 *                 E_NOT_OK             Request failed
 *                 CRYPTO_E_BUSY        Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL  Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129263
 *  \trace         SPEC-2820417, SPEC-2820418
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                               uint32 plaintextLength,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                               uint32 associatedDataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecrypt()
 *********************************************************************************************************************/
/*! \brief         Uses the given data to perform an AEAD decryption and stores the plaintext and the result of
 *                 verification in the memory locations pointed by the plaintext pointer and verifyPtr pointer.
 *  \details       -
 *  \param[in]     jobId                Holds the identifier of the job using the CSM service.
 *  \param[in]     mode                 Indicates which operation mode(s) to perfom.
 *  \param[in]     ciphertextPtr        Contains the pointer to the data to be decrypted.
 *  \param[in]     ciphertextLength     Contains the number of bytes to decrypt.
 *  \param[in]     associatedDataPtr    Contains the pointer to the associated data.
 *  \param[in]     associatedDataLength Contains the number of bytes of the associated data.
 *  \param[in]     tagPtr               Contains the pointer to the data where the Tag shall be stored.
 *  \param[in]     tagLength            Contains the length in bytes of the Tag to be verified.
 *  \param[out]    plaintextPtr         Contains the pointer to the data where the encrypted data shall be stored.
 *  \param[in,out] plaintextLengthPtr   Holds a pointer to the memory location in which the output length in bytes of
 *                                      the ciphertext is stored. On calling this function, this parameter shall
 *                                      contain the size of the buffer in bytes provided by resultPtr. When the request
 *                                      has finished, the actual length of the returned value shall be stored.
 *  \param[out]    verifyPtr            Contains the pointer to the result of the verification.
 *  \return        E_OK                 Request successful
 *                 E_NOT_OK             Request failed
 *                 CRYPTO_E_BUSY        Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL  Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129262
 *  \trace         SPEC-2820419, SPEC-2820420
 *********************************************************************************************************************/
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
                                               P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerate()
 *********************************************************************************************************************/
/*! \brief         Uses the given data to perform the signature calculation and stores the signature in the memory
 *                 location pointed by the result pointer.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data to be signed.
 *  \param[in]     dataLength         Contains the number of bytes to sign.
 *  \param[out]    resultPtr          Contains the pointer to the data where the signature shall be stored.
 *  \param[in,out] resultLengthPtr    Holds a pointer to the memory location in which the output length in bytes is
 *                                    stored. On calling this function, this parameter shall contain the size of the
 *                                    buffer in bytes provided by resultPtr. When the request has finished, the actual
 *                                    length of the returned value shall be stored.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129261
 *  \trace         SPEC-2820421, SPEC-2820422
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate(uint32 jobId,
                                                     Crypto_OperationModeType mode,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                     uint32 dataLength,
                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerify()
 *********************************************************************************************************************/
/*! \brief         Verifies the given signature by comparing with a generated signature.
 *  \details       -
 *  \param[in]     jobId              Holds the identifier of the job using the CSM service.
 *  \param[in]     mode               Indicates which operation mode(s) to perfom.
 *  \param[in]     dataPtr            Contains the pointer to the data to be verified.
 *  \param[in]     dataLength         Contains the number of bytes to be verified.
 *  \param[in]     signaturePtr       Holds a pointer to the signature to be verified.
 *  \param[in]     signatureLength    Contains the signature length in bytes.
 *  \param[out]    verifyPtr          Holds a pointer to the memory location, which will hold the result of the
 *                                    signature verification.
 *  \return        E_OK               Request successful
 *                 E_NOT_OK           Request failed
 *                 CRYPTO_E_BUSY      Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER The provided buffer is too small to store the result
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129260
 *  \trace         SPEC-2820427, SPEC-2820428
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify(uint32 jobId,
                                                   Crypto_OperationModeType mode,
                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                   uint32 dataLength,
                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                   uint32 signatureLength,
                                                   P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_RandomGenerate()
 *********************************************************************************************************************/
/*! \brief         This interface shall be used for generation of random numbers.
 *  \details       -
 *  \param[in]     jobId                    Holds the identifier of the job using the CSM service.
 *  \param[out]    resultPtr                Holds a pointer to the memory location which will hold the result of the
 *                                          random number generation.
 *  \param[in,out] resultLengthPtr          Holds a pointer to the memory location in which the result length in bytes
 *                                          is stored. On calling this function, this parameter shall contain the
 *                                          number of random bytes, which shall be stored to the buffer provided by
 *                                          resultPtr. When the request has finished, the actual length of the returned
 *                                          value shall be stored.
 *  \return        E_OK                     Request successful
 *                 E_NOT_OK                 Request failed
 *                 CRYPTO_E_BUSY            Request failed, service is busy
 *                 CRYPTO_E_QUEUE_FULL      Request failed, the queue is full
 *                 CRYPTO_E_SMALL_BUFFER    The provided buffer is too small to store the result
 *                 CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy of random number generator is exhausted
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *  \pre           Service is idle.
 *  \trace         CREQ-129257
 *  \trace         SPEC-2820436, SPEC-2820437
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerate(uint32 jobId,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

# define CSM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*!
* \exclusivearea CSM_EXCLUSIVE_AREA_0
* Ensures consistency of global RAM variables.
* \protects Job queue and queue access variables.
* \usedin Csm_MainFunction, Csm_CancelJob, Csm_ProcessJob
* \exclude All functions provided by Csm.
* \length SHORT in service API's, LONG in service API's when jobs must be sorted first, LONG in Csm_MainFunction and Csm_CancelJob.
* \endexclusivearea
*/

# endif /* CSM_H */
/**********************************************************************************************************************
 *  END OF FILE: CSM.H
 *********************************************************************************************************************/

