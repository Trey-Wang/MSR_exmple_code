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
/*!        \file  Csm_Types.h
 *        \brief  MICROSAR Crypto Service Manager (CSM)
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file (Csm.h).
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined (CSM_TYPES_H)
# define CSM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Rte_Csm_Type.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Crypto Stack Error Codes [SWS_Csm_01069] [SWS_Crypto_00042] */
# define CRYPTO_E_BUSY                                2u
# define CRYPTO_E_SMALL_BUFFER                        3u
# define CRYPTO_E_ENTROPY_EXHAUSTION                  4u
# define CRYPTO_E_QUEUE_FULL                          5u
# define CRYPTO_E_KEY_READ_FAIL                       6u
# define CRYPTO_E_KEY_WRITE_FAIL                      7u
# define CRYPTO_E_KEY_NOT_AVAILABLE                   8u
# define CRYPTO_E_KEY_NOT_VALID                       9u
# define CRYPTO_E_KEY_SIZE_MISMATCH                   10u
# define CRYPTO_E_COUNTER_OVERFLOW                    11u
# define CRYPTO_E_JOB_CANCELED                        12u

/* Key Elements [SWS_Csm_01022] */
# ifndef CRYPTO_KE_MAC_KEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_MAC_KEY                           1u
# endif

# ifndef CRYPTO_KE_MAC_PROOF /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_MAC_PROOF                         2u
# endif

# ifndef CRYPTO_KE_SIGNATURE_KEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_SIGNATURE_KEY                     1u
# endif

# ifndef CRYPTO_KE_RANDOM_SEED_STATE /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_RANDOM_SEED_STATE                 3u
# endif

# ifndef CRYPTO_KE_RANDOM_ALGORITHM /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_RANDOM_ALGORITHM                  4u
# endif

# ifndef CRYPTO_KE_CIPHER_KEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CIPHER_KEY                        1u
# endif

# ifndef CRYPTO_KE_CIPHER_IV /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CIPHER_IV                         5u
# endif

# ifndef CRYPTO_KE_CIPHER_PROOF /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CIPHER_PROOF                      6u
# endif

# ifndef CRYPTO_KE_CIPHER_2NDKEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CIPHER_2NDKEY                     7u
# endif

# ifndef CRYPTO_KE_KEYEXCHANGE_BASE /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYEXCHANGE_BASE                  8u
# endif

# ifndef CRYPTO_KE_KEYEXCHANGE_PRIVKEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYEXCHANGE_PRIVKEY               9u
# endif

# ifndef CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY             10u
# endif

# ifndef CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE           1u
# endif

# ifndef CRYPTO_KE_KEYEXCHANGE_ALGORITHM /* COV_CSM_NON_RTE */
/* e.g. X25519, RSA, ... */
#  define CRYPTO_KE_KEYEXCHANGE_ALGORITHM             12u
# endif

# ifndef CRYPTO_KE_KEYDERIVATION_PASSWORD /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYDERIVATION_PASSWORD            1u
# endif

# ifndef CRYPTO_KE_KEYDERIVATION_SALT /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYDERIVATION_SALT                13u
# endif

# ifndef CRYPTO_KE_KEYDERIVATION_ITERATIONS /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYDERIVATION_ITERATIONS          14u
# endif

# ifndef CRYPTO_KE_KEYDERIVATION_ALGORITHM /* COV_CSM_NON_RTE */
/* e.g. KDF2, KDFX963, ... */
#  define CRYPTO_KE_KEYDERIVATION_ALGORITHM           15u
# endif

# ifndef CRYPTO_KE_KEYGENERATE_KEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYGENERATE_KEY                   1u
# endif

# ifndef CRYPTO_KE_KEYGENERATE_SEED /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYGENERATE_SEED                  16u
# endif

# ifndef CRYPTO_KE_KEYGENERATE_ALGORITHM /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_KEYGENERATE_ALGORITHM             17u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_DATA /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_DATA                  0u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_PARSING_FORMAT /* COV_CSM_NON_RTE */
/* e.g. X.509, C2X */
#  define CRYPTO_KE_CERTIFICATE_PARSING_FORMAT        18u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_CURRENT_TIME /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_CURRENT_TIME          19u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_VERSION /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_VERSION               20u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_SERIALNUMBER /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_SERIALNUMBER          21u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM   22u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_ISSUER /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_ISSUER                23u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE   24u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER    25u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_SUBJECT /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_SUBJECT               26u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY    1u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_EXTENSIONS /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_EXTENSIONS            27u
# endif

# ifndef CRYPTO_KE_CERTIFICATE_SIGNATURE /* COV_CSM_NON_RTE */
#  define CRYPTO_KE_CERTIFICATE_SIGNATURE             28u
# endif

/* [SWS_Csm_01028] Crypto_JobStateType */
typedef uint8 Crypto_JobStateType;
# define CRYPTO_JOBSTATE_IDLE                         0u
# define CRYPTO_JOBSTATE_ACTIVE                       1u
# define CRYPTO_JOBSTATE_PROGRESSING                  2u

/* [SWS_Csm_01028] Crypto_ServiceInfoType */
typedef uint8 Crypto_ServiceInfoType;
# define CRYPTO_HASH                                  0u
# define CRYPTO_MACGENERATE                           1u
# define CRYPTO_MACVERIFY                             2u
# define CRYPTO_ENCRYPT                               3u
# define CRYPTO_DECRYPT                               4u
# define CRYPTO_AEADENCRYPT                           5u
# define CRYPTO_AEADDECRYPT                           6u
# define CRYPTO_SIGNATUREGENERATE                     7u
# define CRYPTO_SIGNATUREVERIFY                       8u
# define CRYPTO_SECCOUNTERINCREMENT                   9u
# define CRYPTO_SECCOUNTERREAD                        10u
# define CRYPTO_RANDOMGENERATE                        11u
# define CRYPTO_RANDOM                                CRYPTO_RANDOMGENERATE
# define CRYPTO_SECURECOUNTERREAD                     CRYPTO_SECCOUNTERREAD
# define CRYPTO_SECURECOUNTERINCREMENT                CRYPTO_SECCOUNTERINCREMENT

/* [SWS_Csm_01047] Crypto_AlgorithmFamilyType */
typedef uint8 Crypto_AlgorithmFamilyType;
typedef uint8 Crypto_AlgorithmSecondaryFamilyType;
# define CRYPTO_ALGOFAM_NOT_SET                       0x00u
# define CRYPTO_ALGOFAM_SHA1                          0x01u
# define CRYPTO_ALGOFAM_SHA2_224                      0x02u
# define CRYPTO_ALGOFAM_SHA2_256                      0x03u
# define CRYPTO_ALGOFAM_SHA2_384                      0x04u
# define CRYPTO_ALGOFAM_SHA2_512                      0x05u
# define CRYPTO_ALGOFAM_SHA2_512_224                  0x06u
# define CRYPTO_ALGOFAM_SHA2_512_256                  0x07u
# define CRYPTO_ALGOFAM_SHA3_224                      0x08u
# define CRYPTO_ALGOFAM_SHA3_256                      0x09u
# define CRYPTO_ALGOFAM_SHA3_384                      0x0Au
# define CRYPTO_ALGOFAM_SHA3_512                      0x0Bu
# define CRYPTO_ALGOFAM_SHAKE128                      0x0Cu
# define CRYPTO_ALGOFAM_SHAKE256                      0x0Du
# define CRYPTO_ALGOFAM_RIPEMD160                     0x0Eu
# define CRYPTO_ALGOFAM_BLAKE_1_256                   0x0Fu
# define CRYPTO_ALGOFAM_BLAKE_1_512                   0x10u
# define CRYPTO_ALGOFAM_BLAKE_2s_256                  0x11u
# define CRYPTO_ALGOFAM_BLAKE_2s_512                  0x12u
# define CRYPTO_ALGOFAM_3DES                          0x13u
# define CRYPTO_ALGOFAM_AES                           0x14u
# define CRYPTO_ALGOFAM_CHACHA                        0x15u
# define CRYPTO_ALGOFAM_RSA                           0x16u
# define CRYPTO_ALGOFAM_ED25519                       0x17u
# define CRYPTO_ALGOFAM_BRAINPOOL                     0x18u
# define CRYPTO_ALGOFAM_ECCNIST                       0x19u
# define CRYPTO_ALGOFAM_SECURECOUNTER                 0x1Au
# define CRYPTO_ALGOFAM_RNG                           0x1Bu
# define CRYPTO_ALGOFAM_SIPHASH                       0x1Cu
# define CRYPTO_ALGOFAM_ECIES                         0x1Du
# define CRYPTO_ALGOFAM_CUSTOM                        0xFFu


/* [SWS_Csm_01048] Crypto_AlgorithmModeType */
typedef uint8 Crypto_AlgorithmModeType;
# define CRYPTO_ALGOMODE_NOT_SET                      0x00u
# define CRYPTO_ALGOMODE_ECB                          0x01u
# define CRYPTO_ALGOMODE_CBC                          0x02u
# define CRYPTO_ALGOMODE_CFB                          0x03u
# define CRYPTO_ALGOMODE_OFB                          0x04u
# define CRYPTO_ALGOMODE_CTR                          0x05u
# define CRYPTO_ALGOMODE_XTS                          0x06u
# define CRYPTO_ALGOMODE_GCM                          0x07u
# define CRYPTO_ALGOMODE_RSAES_OAEP                   0x08u
# define CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5             0x09u
# define CRYPTO_ALGOMODE_RSASSA_PSS                   0x0Au
# define CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5            0x0Bu
# define CRYPTO_ALGOMODE_8ROUNDS                      0x0Cu
# define CRYPTO_ALGOMODE_12ROUNDS                     0x0Du
# define CRYPTO_ALGOMODE_20ROUNDS                     0x0Eu
# define CRYPTO_ALGOMODE_HMAC                         0x0Fu
# define CRYPTO_ALGOMODE_CMAC                         0x10u
# define CRYPTO_ALGOMODE_GMAC                         0x11u
# define CRYPTO_ALGOMODE_CTRDRBG                      0x12u
# define CRYPTO_ALGOMODE_SIPHASH_2_4                  0x13u
# define CRYPTO_ALGOMODE_SIPHASH_4_8                  0x14u
# define CRYPTO_ALGOMODE_CUSTOM                       0xFFu


/* [SWS_Csm_01008] Crypto_AlgorithmInfoType */
typedef struct
{
  Crypto_AlgorithmFamilyType family;
  uint32 keyLength;
  Crypto_AlgorithmModeType mode;
  Crypto_AlgorithmSecondaryFamilyType secondaryFamily;
} Crypto_AlgorithmInfoType;

/* Operation Modes */
# ifndef Rte_TypeDef_Crypto_OperationModeType /* COV_CSM_NON_RTE */
typedef uint8 Crypto_OperationModeType;
# endif

# ifndef CRYPTO_OPERATIONMODE_START /* COV_CSM_NON_RTE */
#  define CRYPTO_OPERATIONMODE_START                  1u
# endif
# ifndef CRYPTO_OPERATIONMODE_UPDATE /* COV_CSM_NON_RTE */
#  define CRYPTO_OPERATIONMODE_UPDATE                 2u
# endif
# ifndef CRYPTO_OPERATIONMODE_FINISH /* COV_CSM_NON_RTE */
#  define CRYPTO_OPERATIONMODE_FINISH                 4u
# endif

/* [SWS_Crypto_00016] */ /* [SWS_Crypto_00017] */
# ifndef CRYPTO_OPERATIONMODE_STREAMSTART /* COV_CSM_NON_RTE */
#  define CRYPTO_OPERATIONMODE_STREAMSTART ( CRYPTO_OPERATIONMODE_START | CRYPTO_OPERATIONMODE_UPDATE )
# endif
# ifndef CRYPTO_OPERATIONMODE_SINGLECALL /* COV_CSM_NON_RTE */
#  define CRYPTO_OPERATIONMODE_SINGLECALL ( CRYPTO_OPERATIONMODE_START | CRYPTO_OPERATIONMODE_UPDATE | CRYPTO_OPERATIONMODE_FINISH )
# endif

/* [SWS_Csm_01049] Crypto_ProcessingType */
typedef uint8 Crypto_ProcessingType;
# define CRYPTO_PROCESSING_ASYNC                      0u
# define CRYPTO_PROCESSING_SYNC                       1u

# define CSM_ASYNCHRONOUS CRYPTO_PROCESSING_ASYNC
# define CSM_SYNCHRONOUS CRYPTO_PROCESSING_SYNC

/* [SWS_Csm_01024] Crypto_VerifyResultType */
# ifndef Rte_TypeDef_Crypto_VerifyResultType /* COV_CSM_NON_RTE */
typedef uint8 Crypto_VerifyResultType;
# endif

# ifndef CRYPTO_E_VER_OK /* COV_CSM_NON_RTE */
#  define CRYPTO_E_VER_OK                             0u
# endif
# ifndef CRYPTO_E_VER_NOT_OK /* COV_CSM_NON_RTE */
#  define CRYPTO_E_VER_NOT_OK                         1u
# endif

# ifndef CSM_E_VER_OK /* COV_CSM_NON_RTE */
#  define CSM_E_VER_OK                                CRYPTO_E_VER_OK
# endif
# ifndef CSM_E_VER_NOT_OK /* COV_CSM_NON_RTE */
#  define CSM_E_VER_NOT_OK                            CRYPTO_E_VER_NOT_OK
# endif


/* [SWS_Csm_00930] Each crypto primitive configuration shall be realized as a constant structure of type Crypto_PrimitiveInfoType. */
/* [SWS_Csm_01011] */
typedef struct
{
  const uint32 length;
  const Crypto_ServiceInfoType service;
  const Crypto_AlgorithmInfoType algorithm;
} Crypto_PrimitiveInfoType;

/* [SWS_Csm_00932] Each user primitive configuration shall be realized as a constant structure of type Crypto_JobPrimitiveInfoType. */
/* [SWS_Csm_01012] */
typedef struct
{
  const uint32 callbackId;
  const Crypto_PrimitiveInfoType* primitiveInfo;
  const uint32 secureCounterId;
  const uint32 cryIfKeyId;
  const Crypto_ProcessingType processingType;
  const boolean callbackUpdateNotification;
} Crypto_JobPrimitiveInfoType;

/* [SWS_Csm_01009] Crypto_JobPrimitiveInputOutputType */
typedef struct
{
  const uint8* inputPtr;
  uint32 inputLength;
  const uint8* secondaryInputPtr;
  uint32 secondaryInputLength;
  const uint8* tertiaryInputPtr;
  uint32 tertiaryInputLength;

  uint8* outputPtr;
  uint32* outputLengthPtr;
  uint8* secondaryOutputPtr;
  uint32* secondaryOutputLengthPtr;

  Crypto_VerifyResultType* verifyPtr;
  uint64* output64Ptr;
  Crypto_OperationModeType mode;
} Crypto_JobPrimitiveInputOutputType;

typedef struct
{
  const uint32 jobId;
  const uint32 jobPriority;
} Crypto_JobInfoType;

/* [SWS_Csm_01013] Crypto_JobType */
typedef struct
{
  Crypto_JobStateType state;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput;
  const Crypto_JobPrimitiveInfoType* jobPrimitiveInfo;
  const Crypto_JobInfoType * jobInfo;
  uint32 cryptoKeyId;
} Crypto_JobType;

typedef struct
{
  void (* callbackFunction)( const Std_ReturnType result);
} Csm_CallbackStackType;

typedef Crypto_JobType* Crypto_30_CryWrapper_QueueElementType;
typedef Crypto_JobType* Crypto_30_LibCv_QueueElementType;
typedef Crypto_JobType* Csm_QueueElementType;

typedef struct
{
  uint8 localQueueIndex;
  uint8 reservedIndexes;
  uint8 sortNeeded;
  uint8 pauseProcessing;
} Csm_QueueStateType;

/* End of Service Config Types */

#endif  /* CSM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: CSM_TYPES.H
 *********************************************************************************************************************/

