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
/*      \file  Crypto_30_LibCv_KeyManagement.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_KEYMANAGEMENT_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"

#if ( CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif

#if (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON)
# include "actIX25519.h"
# include "actBigNum.h"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
*********************************************************************************************************************/
#if !defined (CRYPTO_30_LIBCV_LOCAL) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
# define CRYPTO_30_LIBCV_LOCAL static
#endif

#if !defined (CRYPTO_30_LIBCV_LOCAL_INLINE) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
# define CRYPTO_30_LIBCV_LOCAL_INLINE LOCAL_INLINE
#endif

/* PRQA S 3453 1 */ /*  MD_MSR_19.7 */
#define Crypto_30_LibCv_GetKeyElementWrittenLength(Index)        ((uint32)(((uint32)Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements((Index))) << 8u) | Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements((Index))+1)))
#define CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_KEY 64u
#define CRYPTO_30_LIBCV_SIZEOF_SHE_KEY 16u
#define CRYPTO_30_LIBCV_SIZEOF_SHE_DATA_BUFFER 48u

/* Supported Key Derivation Algorithms */
/* KDF_NIST_800-108 'KDF in Counter Mode' with SHA-256 as PRF */
#define CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256  1u
/* NIST.FIPS.186-4 'Key Pair Generation Using Extra Random Bits' with 'KDF in Counter Mode' as RBG */
#define CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB          2u

#define CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER       (256 + 12u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_PARENT_KEY         256u
#define CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY         256u
#define CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8  (256u + 8u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC         6u
#define CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC        38u
#define CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME                  32u
#define CRYPTO_30_LIBCV_SIZEOF_KDF_CONTEXT                4u
#define CRYPTO_30_LIBCV_SIZEOF_KDF_TARGET_KEY_LENGTH      2u

#define CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR                (0u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementCopy()
 *********************************************************************************************************************/
/*! \brief         Copy key element
 *  \details       Copies a key element to another key element in the same crypto driver.
 *  \param[in]     elementIndexSrc              Holds the element index of the source key element. Has to be in bounds.
 *  \param[in]     elementIndexDst              Holds the element index of the destination key element. Has to be in bounds.
 *  \param[in]     cryptoKeyIdDst               Holds the identifier of the key.
 *  \return        E_OK                         Request successful.
 *  \return        E_NOT_OK                     Request failed.
 *  \return        CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *  \return        CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *  \return        CRYPTO_E_KEY_EXTRACT_DENIED  Request failed, not allowed to extract key material.
 *  \return        CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopy(
  Crypto_30_LibCv_KeyElementsIterType elementIndexSrc,
  Crypto_30_LibCv_KeyElementsIterType elementIndexDst,
  uint32 cryptoKeyIdDst);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SetKeyElementWrittenLength()
 *********************************************************************************************************************/
/*! \brief        Sets the written length of a key element.
 *  \details       -
 *  \param[in]     keyElementId            Holds the identifier of the key element whose written length shall be set.
 *  \param[in]     keyElementLength        Written length of the key element
 *  \pre           keyElementId has to hold a valid identifier of the key element
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLength(
  uint32 keyElementId,
  uint32 keyElementLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SetKeyState()
 *********************************************************************************************************************/
/*! \brief         Sets the state of a key.
 *  \details       The function sets the key state and respectively the key states of all associated key elements.
 *  \param[in]     cryptoKeyId           Holds the identifier of the key.
 *  \param[in]     mask                  State of the key, consistent of different flags.
 *  \pre           cryptoKeyId has to hold a valid identifier of a key
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyState(
  uint32 cryptoKeyId,
  uint8 mask);

/**********************************************************************************************************************
*  Crypto_30_LibCv_SetKeyState()
*********************************************************************************************************************/
/*! \brief        Clears the state of a key.
*  \details       The function clears the key state and respectively the key states of all associated key elements.
*  \param[in]     cryptoKeyId           Holds the identifier of the key.
*  \param[in]     mask                  State of the key, consistent of different flags.
*  \pre           cryptoKeyId has to hold a valid identifier of a key
*  \context       TASK
*  \reentrant     TRUE
*  \synchronous   TRUE
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearKeyState(
  uint32 cryptoKeyId,
  uint8 mask);

#if (CRYPTO_30_LIBCV_SUPPORT_SHE_KEY_UPDATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel()
 *********************************************************************************************************************/
/*! \brief         The function derives a key from given input data.
 *  \details       The function derives a key from given input data. This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to input data from which the key is derived
 *  \param[in]     length                  Length of input data.
 *  \param[out]    outputPtr               Pointer to derived key
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputPtr has to be 16 Bytes
 *  \pre           Length of the buffer provided by dataPtr has to be a multiple of 16.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint32 length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateCmac()
 *********************************************************************************************************************/
/*! \brief         Generates Cmac based on given data and given key.
 *  \details       The function calculates the cmac based on the given data and the key. This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data from which the mac is calculated
 *  \param[in]     length                  Length of input data
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to derived key
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputPtr has to be 16 Bytes
 *  \pre           Length of the key provided by keyPtr has to be 16 Bytes
 *  \pre           Length of the provided dataPtr has to be a multiple of 16 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCmac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_()
 *********************************************************************************************************************/
/*! \brief         Updates key element based on SHE key update mechanism.
 *  \details       Interprets the given key buffer as M1M2M3 of the SHE key update mechanism and extracts relevant 
                   information for setting the key element. This is used during the SHE key updated protocol.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the identifier of the key element which shall be set
 *  \param[in]     keyPtr                  Holds the pointer to the key buffer which shall be used to update the key element
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdate(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateDecryptKey()
 *********************************************************************************************************************/
/*! \brief         Decrypts data based on given key.
 *  \details       The function decrypts the given data using the provided key. AES128 is used as underlying algorithm. 
                   This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data which shall be decrypted
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to buffer where the decrypted data will be copied to
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by dataPtr has to be 32 Bytes
 *  \pre           Length of the key provided by keyPtr has to be 16 Bytes
 *  \pre           Length of the buffer provided by outputPtr has to be 32 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateDecryptKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateEncryptKey()
 *********************************************************************************************************************/
/*! \brief         Encrypts data based on given key.
 *  \details       The function encrypts the given data using the provided key. AES128 is used as underlying algorithm.
                   This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data which shall be encrypted
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to buffer where the encrypted data will be copied to
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by dataPtr has to be 32 Bytes
 *  \pre           Length of the key provided by keyPtr has to be 16 Bytes
 *  \pre           Length of the buffer provided by outputPtr has to be 32 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateEncryptKey(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateVerifyM3()
 *********************************************************************************************************************/
/*! \brief         Verify M3
 *  \details       The function verifies the M3.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     elementIndex            Index of the key element
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *  \pre           Length of the buffer provided by KDFbufferPtr has to be 32 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateVerifyM3(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateExtractKey()
 *********************************************************************************************************************/
/*! \brief         Extract new Key
 *  \details       The function provides the new key.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     elementIndex            Index of the key element.
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *  \pre           Length of the buffer provided by KDFbufferPtr has to be 32 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateExtractKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateProofM4M5()
 *********************************************************************************************************************/
/*! \brief        Calculate M4 and M5
 *  \details       The function calculate the proof elements M4 and M5.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     outputElement           Index of the destination key element
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateProofM4M5(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  Crypto_30_LibCv_KeyElementsIterType outputElement,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength, P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

#endif

#if (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_CopyData()
 *********************************************************************************************************************/
/*! \brief        Copies data from source to destination.
 *  \details       -
 *  \param[in]     dataBuf                 Pointer to data which shall be copied
 *  \param[in]     dataLength              Number of bytes which shall be copied
 *  \param[out]    targetDataBuf           Pointer to buffer where the data shall be copied to
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CopyData(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataBuf,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetDataBuf,
  uint16 dataLength);
#endif

#if  (CRYPTO_30_LIBCV_SUPPORT_PUBLIC_KEY_EXCHANGE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_ClearData()
 *********************************************************************************************************************/
/*! \brief        Clears data buffer (overwrite with 0x00)
 *  \details       -
 *  \param[in]     dataBuf                 Pointer to data which shall be cleared
 *  \param[out]    targetDataBuf           Pointer to data buffer
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearData(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataBuf,
  uint16 dataLength);
#endif

#if (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_HandleInputParams()
 *********************************************************************************************************************/
/*! \brief        Get and check input paremeters of the KDF
 *  \details      -
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[out]   deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[out]   prfInputBuffer          Input for the PRF
 *  \param[out]   prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   primeMinus1PtrPtr       Pointer to the position of the prime minus-1 data within the salt
 *  \param[out]   primeLength             Length of the prime-1
 *  \param[out]   targetKeyLength         Length of the target key
 *  \param[out]   parentKeyLength         Length of the parent key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *  \pre          Length of the buffer provided by prfInputBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER Bytes.
 *  \pre          Length of the buffer provided by saltBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams(
  CONST(uint32, AUTOMATIC) cryptoKeyId,
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint8 *, AUTOMATIC, AUTOMATIC) primeMinus1PtrPtr,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) primeLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) parentKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt()
 *********************************************************************************************************************/
/*! \brief        Get and check the salt input paramter of the KDF
 *  \details      -
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[out]   prfInputBuffer          Input for the PRF
 *  \param[out]   prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   primeMinus1PtrPtr       Pointer to the position of the prime minus-1 data within the salt
 *  \param[out]   primeLength             Length of the prime-1
 *  \param[out]   targetKeyLength         Length of the target key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *  \pre          Length of the buffer provided by prfInputBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER Bytes.
 *  \pre          Length of the buffer provided by saltBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt(CONST(uint32, AUTOMATIC) cryptoKeyId,
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint8 *, AUTOMATIC, AUTOMATIC) primeMinus1PtrPtr,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) primeLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength()
 *********************************************************************************************************************/
/*! \brief        Get and check the target key length from the salt input paramter of the KDF
 *  \details      -
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[in]    saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *  \pre          Length of the buffer provided by saltBuffer has to be CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) deriveAlgorithm,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_CallPRF()
 *********************************************************************************************************************/
/*! \brief        Call the PRF function of the KDF
 *  \details      -
 *  \param[in]    prfInputBuffer          Input for the PRF
 *  \param[in]    prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   targetKey               Buffer for the target key
 *  \param[in]    targetKeyLength         Length of the target key
 *  \param[in]    parentKeyLength         Length of the parent key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by targetKey has to be CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8 Bytes.
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_CallPRF(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  CONST(uint32, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  CONST(uint32, AUTOMATIC) targetKeyLength, CONST(uint32, AUTOMATIC) parentKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Kdf_AsymDevideByPrimeMinusOne()
 *********************************************************************************************************************/
/*! \brief        Divides the given target key by the given prime-1 and adds one to the result
 *  \details      -
 *  \param[out]   targetKey               Buffer for the target key
 *  \param[in]    targetKeyLength         Length of the target key
 *  \param[in]    primeMinus1PtrPtr       Pointer to the position of the prime minus-1 data within the salt
 *  \param[in]    primeLength             Length of the prime-1
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by targetKey has to be CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8 Bytes.
*********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_AsymDevideByPrimeMinusOne(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  CONST(uint32, AUTOMATIC) targetKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr,
  CONST(uint32, AUTOMATIC) primeLength);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_KeyElementCopy()
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
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopy(
  Crypto_30_LibCv_KeyElementsIterType elementIndexSrc,
  Crypto_30_LibCv_KeyElementsIterType elementIndexDst,
  uint32 cryptoKeyIdDst)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndexSrc;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndexDst;
  uint32 i;

  {
    /* ----- Implementation ------------------------------------------------- */

    Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndexDst, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */

    /* Are both keys not virtual ?*/
    /* #30 Handle not virtual key elements */
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALIDXOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
    if ((Crypto_30_LibCv_IsKeyElementVirtual(elementIndexSrc) == FALSE)
      && (Crypto_30_LibCv_IsKeyElementVirtual(elementIndexDst) == FALSE))
#endif
    {
      /* Check if the available data is not bigger than the maximum size of the destination element */
      /* #35 Copy key element data if the size is matching */
      if ((Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc) == Crypto_30_LibCv_GetKeyElementLength(elementIndexDst)) ||
        ((Crypto_30_LibCv_IsKeyElementPartial(elementIndexDst) == TRUE) && (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_LibCv_GetKeyElementLength(elementIndexDst))))
      {
        i = 0u;
        keyStorageIndexSrc = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc);
        keyStorageIndexDst = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexDst);
        /* Copy the source element to the destination element */
        while (i < Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc)) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        {
          Crypto_30_LibCv_SetKeyStorage(keyStorageIndexDst, Crypto_30_LibCv_GetKeyStorage(keyStorageIndexSrc)); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

          keyStorageIndexSrc++;
          keyStorageIndexDst++;
          i++;
        }
        /* store the written length in the destination element */
        Crypto_30_LibCv_SetKeyElementWrittenLength(elementIndexDst, i);
        retVal = E_OK;
      }
      else
      {
        /* The destination element is to short to store the available data of the source element */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
    }
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
    /* #40 Handle virtual key elements */
    /* #45 Both key elements are virtual */
    /* Are both elements virtual (This use-case should normally not happen) */
    else if ((Crypto_30_LibCv_IsKeyElementVirtual(elementIndexSrc) == TRUE)
      && (Crypto_30_LibCv_IsKeyElementVirtual(elementIndexDst) == TRUE))
    {
      /* #50 Check if the available data is not bigger than the maximum size of the destination element */
      if (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_LibCv_GetKeyElementLength(elementIndexDst))
      {
        /* #55 Copy the data from the source element to the destination element */
        for ( /* start values */
          i = 0u,
          keyStorageIndexSrc = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc) + (Crypto_30_LibCv_KeyStorageIterType)Crypto_30_LibCv_GetKeyElementVirtualOffset(elementIndexSrc),
          keyStorageIndexDst = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexDst) + (Crypto_30_LibCv_KeyStorageIterType)Crypto_30_LibCv_GetKeyElementVirtualOffset(elementIndexSrc);
        /* loop condition */
        i < Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc);
        /* Increment indices */
        keyStorageIndexSrc++,
          keyStorageIndexDst++,
          i++)
        {
          Crypto_30_LibCv_SetKeyStorage(keyStorageIndexDst, Crypto_30_LibCv_GetKeyStorage(keyStorageIndexSrc));
        }
        /* #60 store the written length in the destination element */
        Crypto_30_LibCv_SetKeyElementWrittenLength(elementIndexDst, i);
        retVal = E_OK;
      }
      else
      {
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
    }
    /* #65 Source key element is virtual, destination key element is not virtual */
    else if ((Crypto_30_LibCv_IsKeyElementVirtual(elementIndexSrc) == TRUE)
      && (Crypto_30_LibCv_IsKeyElementVirtual(elementIndexDst) == FALSE))
    {
      /* #70 Copy the data from the source element to the destination element */
      keyStorageIndexSrc = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc) + (Crypto_30_LibCv_KeyStorageIterType)Crypto_30_LibCv_GetKeyElementVirtualOffset(elementIndexSrc);
      keyStorageIndexDst = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexDst);
      for (i = 0u; ((i < Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc)) && (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexDst))); i++)
      {
        Crypto_30_LibCv_SetKeyStorage(keyStorageIndexDst, Crypto_30_LibCv_GetKeyStorage(keyStorageIndexSrc));
        keyStorageIndexSrc++;
        keyStorageIndexDst++;
      }
      /* #75 store the written length in the destination element */
      Crypto_30_LibCv_SetKeyElementWrittenLength(elementIndexDst, i);
      retVal = E_OK;
    }
    /* #80 Source key element is not virtual, destination key element is virtual */
    else
    {
      /* copy from normal element to virtual element not possible */
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
#endif

    if (retVal == E_OK)
    {
      Crypto_30_LibCv_ClearKeyState(cryptoKeyIdDst, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
    }
    else
    {
      Crypto_30_LibCv_SetKeyElementStateByMask(elementIndexDst, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    }

  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SetKeyElementWrittenLength()
 **********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLength(uint32 keyElementId, uint32 keyElementLength)
{

  /* #10 Set high- and low-byte for keyElement written length */
  Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements(keyElementId), (uint8)(keyElementLength >> 8)); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements(keyElementId) + 1, (uint8)keyElementLength); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
}

/**********************************************************************************************************************
* Crypto_30_LibCv_SetKeyState()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyState(uint32 cryptoKeyId, uint8 mask)
{
  Crypto_30_LibCv_KeyElementsIterType elementIndex;

  /* #10 Set key state for all key elements */

  /* Loop over all elements of the source key */
  for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, mask); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  }
}

/**********************************************************************************************************************
* Crypto_30_LibCv_ClearKeyState()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearKeyState(uint32 cryptoKeyId, uint8 mask)
{
  Crypto_30_LibCv_KeyElementsIterType elementIndex;

  /* #10 Clear key state for all key elements */

  /* Loop over all elements of the source key */
  for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, mask); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  }
}

#if (CRYPTO_30_LIBCV_SUPPORT_SHE_KEY_UPDATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel()
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(
  P2CONST( uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint32 length,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  /*
    AES-MP(x i ): OUT_i  = ENC_(ECB,OUT_i-1) (x i )  XOR x_i  XOR OUT_i-1;  i > 0;   OUT_0  = 0;
  */
  eslt_WorkSpaceAES128Block workSpaceAes128Block;
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  uint8 Out[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  uint8 Out_last[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  uint8 tempCalcByte;
  boolean exitLoop = FALSE;
  eslt_ErrorCode eslRetVal;

  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) tempPlainPtr = dataPtr;

  /* #10 initialize all parameters */
  /* Init Out_i-1 (Out_last) */
  for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
  {
    Out[i] = 0u; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    Out_last[i] = 0u; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
  }

  /* #15 initialize workspace */
  if (ESL_ERC_NO_ERROR == esl_initWorkSpaceHeader(&workSpaceAes128Block.header, /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
    ESL_MAXSIZEOF_WS_AES128,
    CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR))
  {
    /* #20 Calculate Block-Hash until data size is reached */
    while ((length > 0) && (exitLoop == FALSE))
    {
      /* Set Key */
      /* #13 Set old Hash as round Key */
      eslRetVal = esl_initEncryptAES128Block(&workSpaceAes128Block, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))Out_last); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */

      /* Encrypt */
      /* #14 Calculate new Hash */
      if (eslRetVal == ESL_ERC_NO_ERROR)
      {
        eslRetVal = esl_encryptAES128Block(&workSpaceAes128Block, tempPlainPtr, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))Out); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
      }

      if (eslRetVal == ESL_ERC_NO_ERROR)
      {
        /* #25 Exor new and old hash */
        for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
        {
          /* Calculate Out and Out_last */
          tempCalcByte = (uint8)(Out_last[i] ^ Out[i] ^ tempPlainPtr[i]);
          Out_last[i] = tempCalcByte; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
          Out[i] = tempCalcByte; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
        }
        /* Store new data pointer */
        tempPlainPtr = &tempPlainPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
        /* Update length */
        length -= CRYPTO_30_LIBCV_SIZEOF_SHE_KEY;
      }
      else
      {
        exitLoop = TRUE;
      }
    }
  }

  /* #30 Save Result */
  if (length == 0u)
  {
    retVal = E_OK;

    for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
    {
      outputPtr[i] = Out[i]; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCmac()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCmac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceCMACAES workSpaceCmacAes;
  uint32 tempLength = length;

  /* #10 Init work space */
  if (ESL_ERC_NO_ERROR == esl_initWorkSpaceHeader(&workSpaceCmacAes.header, /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
                                                  ESL_MAXSIZEOF_WS_CMACAES,
                                                  CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR))
  {
    /* #15 Init CMAC */
    if (ESL_ERC_NO_ERROR == esl_initCMACAES128(&workSpaceCmacAes, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
                                               16u,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))keyPtr))
    {
      /* #20 Calculate Block-CMAC until data size is reached */
      while (tempLength > 0)
      {
        /* #25 Update Calculation */
        if (ESL_ERC_NO_ERROR == esl_updateCMACAES128(&workSpaceCmacAes, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_PTR */
                                                     16u,
                                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&dataPtr[length - tempLength]))
        {
          tempLength -= 16u;
        }
        else
        {
          break;
        }
      }
      if (tempLength == 0)
      {
        /* Finalize */
        if (ESL_ERC_NO_ERROR == esl_finalizeCMACAES128(&workSpaceCmacAes, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_OUTPUT_PTR */
                                                       outputPtr))
        {
          retVal = E_OK;
        }
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateDecryptKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateDecryptKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceAES128Block workSpaceAes128Block;
  uint8_least i;
  eslt_ErrorCode eslRet;

  /* #10 Init work space */
  eslRet = esl_initWorkSpaceHeader(&workSpaceAes128Block.header, ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  /* #15 Init AES decryption */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initDecryptAES128Block(&workSpaceAes128Block, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))keyPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
  }

  /* #20 Decrypt first and second data blocks */
  /* Decrypt first block */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_decryptAES128Block(&workSpaceAes128Block, dataPtr, outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
  }
  /* Decrypt second block */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_decryptAES128Block(&workSpaceAes128Block, &dataPtr[16u], &outputPtr[16u]); /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #25 copy result to output buffer */
    for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
    {
      outputPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + i] = (uint8)(outputPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + i] ^ dataPtr[i]); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    }
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateEncryptKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateEncryptKey(
  P2CONST( uint8, AUTOMATIC, AUTOMATIC ) dataPtr,
  P2CONST ( uint8, AUTOMATIC, AUTOMATIC ) keyPtr,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceAES128Block workSpaceAes128Block;

  /* #10 Init work space */
  if (ESL_ERC_NO_ERROR == esl_initWorkSpaceHeader(&(workSpaceAes128Block.header), /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
                                                  ESL_MAXSIZEOF_WS_AES128,
                                                  CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR))
  {
    /* #15 Init AES encryption */
    if (ESL_ERC_NO_ERROR == esl_initEncryptAES128Block(&workSpaceAes128Block, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
                                                       (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))keyPtr))
    {
      /* #20 encrypt data block */
      /* Encrypt block */
      if (ESL_ERC_NO_ERROR == esl_encryptAES128Block(&workSpaceAes128Block, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
                                                      dataPtr,
                                                      outputPtr))
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_SheKeyUpdateVerifyM3()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateVerifyM3(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY]; /* K2 */
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);

  /* #10 Generate K2 */
  /* KDF AuthKey|Key_Update_Mac_C */
  for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
  {
    KDFbufferPtr[i] = Crypto_30_LibCv_GetKeyStorage(keyStorageIndex + i); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }

  if (E_OK == Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key)) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {

    /* #15 Calculate M3 / CMAC and verify */
    /* M3 proof:  M3* = outputBufferPtr */
    if (E_OK == Crypto_30_LibCv_SheKeyUpdateCmac(keyPtr, 16u + 32u, key, outputBufferPtr)) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
    {
      retVal = E_OK;
      /* Check (M3 = M3*) */
      for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
      {
        if (keyPtr[i + CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + 32u] != outputBufferPtr[i])
        {
          retVal = E_NOT_OK;
        }
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_SheKeyUpdateExtractKey()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateExtractKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];

  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Generate K1 */
  /* Set Key_Update_Enc_C constant */
  KDFbufferPtr[17u] = 0x01u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

  if (E_OK == Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key)) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {
    for (i = 0u; i < 32u; i++)
    {
      outputBufferPtr[i] = 0u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    }
    /* #15 Decrypt C,K,F */
    if (E_OK == Crypto_30_LibCv_SheKeyUpdateDecryptKey(&keyPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], key, outputBufferPtr)) /* SBSW_CRYPTO_30_LIBCV_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK */
    {
      /* #20 Store new Key and key length. */
      for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
      {
        Crypto_30_LibCv_SetKeyStorage(keyStorageIndex + i, outputBufferPtr[16u + i]); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

        /*  KDF Key|Key_Update_Enc_C */
        KDFbufferPtr[i] = outputBufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + i]; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
      }

      Crypto_30_LibCv_SetKeyElementWrittenLength(elementIndex, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY);
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_SheKeyUpdateProofM4M5()
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
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateProofM4M5(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  Crypto_30_LibCv_KeyElementsIterType outputElement,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY]; /* Key3 and Key4 */
  uint8 dataBuffer[CRYPTO_30_LIBCV_SIZEOF_SHE_DATA_BUFFER];

  /* #10 Generate K3 */
  if (E_OK == Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key)) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {
    /* #15 Generate M4* */
    outputBufferPtr[3] |= 0x08u; /* Add leading zero for padding */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    outputBufferPtr[3] &= 0xF8u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    for (i = 4u; i < 16u; i++)
    {
      /* Add padding */
      outputBufferPtr[i] = 0u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    }

    /* #20 Copy first block of M4 */
    for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
    {
      dataBuffer[i] = keyPtr[i]; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    }

    /* #25 Generate and store second block of M4 */
    if (E_OK == Crypto_30_LibCv_SheKeyUpdateEncryptKey(outputBufferPtr, key, &dataBuffer[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY])) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
    {
      /* Set Key_Update_Mac_C constant */
      KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY+1u] = 0x02u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

      /* 30 Generate K4 */
      if (E_OK == Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key)) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
      { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        /* Generate M5 */
        retVal = Crypto_30_LibCv_SheKeyUpdateCmac(dataBuffer, 32u, key, &dataBuffer[32]); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

        /* #35 Copy M4 & M5 */
        for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_DATA_BUFFER; i++)
        {
          Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(outputElement) + i, dataBuffer[i]); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
        }

        /* #40 Store written length */
        Crypto_30_LibCv_SetKeyElementWrittenLength(outputElement, CRYPTO_30_LIBCV_SIZEOF_SHE_DATA_BUFFER); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdate()
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
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdate(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr)
{
  /* #10 Init KDF Parameters */
  /* KDF Buffer */
  uint8 KDFbuffer[32] =
  {
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x01u, 0x02u, 0x53u, 0x48u, 0x45u, 0x00u, 0x80u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xB0u
  };

  /* #15 Init Workspace */
  /* Working Buffers */
  uint8 outputBuffer[32];

  Std_ReturnType retVal = E_NOT_OK;
  uint8 outputElementAvailable = FALSE;
  uint8 updateKey = TRUE;

  Crypto_30_LibCv_KeyElementsIterType outputElement;

  /* #20 Determine proof output slot */
  for (outputElement = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId);
    outputElement < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId);
    outputElement++)
  {
    if ((Crypto_30_LibCv_GetIdOfKeyElements(outputElement) == CRYPTO_KE_MAC_PROOF)
      || (Crypto_30_LibCv_GetIdOfKeyElements(outputElement) == CRYPTO_KE_CIPHER_PROOF))
    {
      if ((Crypto_30_LibCv_GetKeyStorageEndIdxOfKeyElements(outputElement) - Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(outputElement)) >= (uint16)48u)
      {
        outputElementAvailable = TRUE;
      }
      else
      {
        updateKey = FALSE;
      }
      break;
    }
  }

  if (updateKey == TRUE)
  {
    /* #25 Verify M3 */
    if (Crypto_30_LibCv_SheKeyUpdateVerifyM3(keyPtr, elementIndex, KDFbuffer, sizeof(KDFbuffer), outputBuffer) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
    {
      /* #30 Extract Key */
      retVal = Crypto_30_LibCv_SheKeyUpdateExtractKey(keyPtr, elementIndex, KDFbuffer, sizeof(KDFbuffer), outputBuffer); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */

      if (retVal == E_OK)
      {
        /* #35 Generate M4 & M5, if proof slot is available */
        if (outputElementAvailable == TRUE)
        {
          /* Set Key_Update_Enc_C constant */
          KDFbuffer[17u] = 0x01u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

          retVal = Crypto_30_LibCv_SheKeyUpdateProofM4M5(keyPtr, outputElement, KDFbuffer, sizeof(KDFbuffer), outputBuffer); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
        }
      }
    }
  }

  return retVal;
}
#endif /* CRYPTO_30_LIBCV_SUPPORT_SHE_KEY_UPDATE == STD_ON */

#if (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_CopyData()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CopyData(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataBuf,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetDataBuf,
  uint16 dataLength)
{
  uint16_least byteIdx;
  /* #10 Copy data bytewise to the target buffer */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++)
  {
    targetDataBuf[byteIdx] = dataBuf[byteIdx]; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
}
#endif

#if  (CRYPTO_30_LIBCV_SUPPORT_PUBLIC_KEY_EXCHANGE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_ClearData()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearData(uint8* dataBuf, uint16 dataLength)
{
  uint16_least byteIdx;
  /* #10 Clear data bytewise */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++)
  {
    dataBuf[byteIdx] = 0x00u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
}
#endif

#if (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON)
/**********************************************************************************************************************
* Crypto_30_LibCv_Local_Kdf_HandleInputParams()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams(
  CONST( uint32, AUTOMATIC ) cryptoKeyId,
  CONST( uint32, AUTOMATIC ) targetCryptoKeyId,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) deriveAlgorithm,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) prfInputBuffer,
  P2VAR( uint32, AUTOMATIC, AUTOMATIC ) prfInputBufferLength,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) saltBuffer,
  P2VAR( uint8*, AUTOMATIC, AUTOMATIC ) primeMinus1PtrPtr,
  P2VAR( uint16, AUTOMATIC, AUTOMATIC ) primeLength,
  P2VAR( uint32, AUTOMATIC, AUTOMATIC ) targetKeyLength,
  P2VAR( uint32, AUTOMATIC, AUTOMATIC ) parentKeyLength )
{
  Std_ReturnType retVal; 

  uint32 labelLength = 4u;
  uint32 algorithmLength = 1u;

  (*parentKeyLength) = CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_PARENT_KEY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

  /* #1 Get Algorithm from the algorithm key element */
  retVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_ALGORITHM, deriveAlgorithm, &algorithmLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if( ( (*deriveAlgorithm) != CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 ) &&
      ( (*deriveAlgorithm) != CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB ) )
  {
    retVal = E_NOT_OK;
  }

  /* #2 Get Parent Key from the password key element and write it to the PRF input buffer */
  if (retVal == E_OK)
  {
    retVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, prfInputBuffer, parentKeyLength ); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    (*prfInputBufferLength) += *parentKeyLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
  }

  /* #3 Get Label from the Label key element and write it to the PRF input buffer */
  if (retVal == E_OK)
  {
    /* Skip one byte for iteration counter */
    (*prfInputBufferLength)++; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    retVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL, &(prfInputBuffer[*prfInputBufferLength]), &labelLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    (*prfInputBufferLength) += labelLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
  }

  /* #4 Add constant byte 0x00 to the PRF input buffer */
  prfInputBuffer[*prfInputBufferLength] = 0x00u; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  (*prfInputBufferLength)++; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

  /* #5 Get and check the Salt element */
  if( retVal == E_OK )
  {
    retVal = Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt( cryptoKeyId, targetCryptoKeyId,  deriveAlgorithm, prfInputBuffer, prfInputBufferLength, saltBuffer, primeMinus1PtrPtr, primeLength, targetKeyLength ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  /* #6 Convert target key length to bits and little endian and copy it to the input buffer */
  if (retVal == E_OK)
  {
    /* Set k in bits and little endian (lo byte first ) */
    prfInputBuffer[*prfInputBufferLength] = (uint8)((uint16)((*targetKeyLength << 3u)) & 0x00FFu); /* lo byte */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    prfInputBuffer[*prfInputBufferLength+1] = (uint8)((uint16)((*targetKeyLength ) >> 5u) & 0x00FFu); /* hi byte */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    (*prfInputBufferLength) += CRYPTO_30_LIBCV_SIZEOF_KDF_TARGET_KEY_LENGTH; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt(
  CONST( uint32, AUTOMATIC ) cryptoKeyId,
  CONST( uint32, AUTOMATIC ) targetCryptoKeyId,
  P2CONST( uint8, AUTOMATIC, AUTOMATIC ) deriveAlgorithm,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) prfInputBuffer,
  P2VAR( uint32, AUTOMATIC, AUTOMATIC ) prfInputBufferLength,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) saltBuffer,
  P2VAR( uint8*, AUTOMATIC, AUTOMATIC ) primeMinus1PtrPtr,
  P2VAR( uint16, AUTOMATIC, AUTOMATIC ) primeLength,
  P2VAR( uint32, AUTOMATIC, AUTOMATIC ) targetKeyLength )
{
  Std_ReturnType retVal = E_OK;

  uint32 saltLength = CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC;

  /* #1 Load Salt / Context from the salt key element */
  /* 4 byte Context | 2 byte target Key Length in Big Endian (k) | 32 byte curve specific Prime-1 */
  /* For Asym keys the salt contains also the 32 byte prime-1 */
  if ( (*deriveAlgorithm) == CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB )
  {
    /* Asym key */
    saltLength = CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC;
  }

  retVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_SALT, saltBuffer, &saltLength ); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  /* #2 Check expected salt length according to the algorithm */
  if( retVal == E_OK )
  {
    if( (*deriveAlgorithm) == CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 )
    {
      if( saltLength < CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC )
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      if ((*deriveAlgorithm) == CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB) /* COV_CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM */
      {
        if( saltLength < CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC )
        {
          retVal = E_NOT_OK;
        }
        else
        {
          /* #3 Set the prime minus-1 pointer to the start address of the value within the salt buffer and set the data length */
          (*primeMinus1PtrPtr) = &(saltBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_SYMMETRIC]); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
          (*primeLength) = CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
        }
      }
    }
  }

  /* #4 Copy the 4 byte Context into the PRF input buffer */
  if( retVal == E_OK )
  {
    Crypto_30_LibCv_CopyData( saltBuffer, &(prfInputBuffer[*prfInputBufferLength]), CRYPTO_30_LIBCV_SIZEOF_KDF_CONTEXT); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    (*prfInputBufferLength) += CRYPTO_30_LIBCV_SIZEOF_KDF_CONTEXT; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

    retVal = Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength( targetCryptoKeyId, deriveAlgorithm, saltBuffer, targetKeyLength ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }
  return retVal;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(
  CONST( uint32, AUTOMATIC ) targetCryptoKeyId,
  P2CONST( uint8, AUTOMATIC, AUTOMATIC ) deriveAlgorithm,
  P2CONST( uint8, AUTOMATIC, AUTOMATIC ) saltBuffer,
  P2VAR( uint32, AUTOMATIC, AUTOMATIC ) targetKeyLength )
{
  Std_ReturnType retVal = E_OK;

  uint32 configuredTargetKeyLength = 0u;

  /* #1 Get configured target key length */
  Crypto_30_LibCv_KeyElementsIterType elementIndex = Crypto_30_LibCv_Local_KeyElementSearch( targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD );
  if( elementIndex < Crypto_30_LibCv_GetSizeOfKeyElements() )
  {
    /* Get Configured Target Key Length */
    configuredTargetKeyLength = Crypto_30_LibCv_GetKeyElementLength(elementIndex);
  }
  else
  {
    retVal = E_NOT_OK;
  }

  /* #2 Compare the configured length against the target key length given in salt */
  /* The keyLength (k) in salt is given in bytes and Big Endian Format. This has to be converted to size in bits and little endian format later */
  (*targetKeyLength) = ((uint16)((((uint16)(saltBuffer[4])) << 8) | ((uint16)(saltBuffer[5] & 0x00FFu)))); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */

  /* Check valid target key length */
  if( ( *targetKeyLength > CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY ) || ( (*targetKeyLength) > configuredTargetKeyLength ) )
  {
    retVal = E_NOT_OK;
  }

  /* #3 For Asym keys check that length is supported by used prime numbers */
  if( (*deriveAlgorithm) == CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB )
  {
    if( (*targetKeyLength) != CRYPTO_30_LIBCV_SIZEOF_KDF_PRIME )
    {
      retVal = E_NOT_OK;
    }

    /* #4 For Asymmetric key derivation target key has to be extended by 8 bytes, because it is divided by prime-1 in the second step */
    (*targetKeyLength) += 8u; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_Kdf_CallPRF( )
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_CallPRF(
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) prfInputBuffer,
  CONST( uint32, AUTOMATIC ) prfInputBufferLength,
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) targetKey,
  CONST( uint32, AUTOMATIC ) targetKeyLength,
  CONST( uint32, AUTOMATIC ) parentKeyLength
  )
{
  Std_ReturnType retVal = E_OK;
  uint16_least derivedLength;
  uint8 i = 1u;

  /* #1 Iterate over the target key length */
  for( derivedLength = 0u; derivedLength < targetKeyLength; derivedLength+=ESL_SIZEOF_SHA256_DIGEST)
  {
    eslt_WorkSpaceSHA256 workspaceSHA256;
    uint8 hash[ESL_SIZEOF_SHA256_DIGEST] = { 0u };

    /* #11 Set the iteration counter i in the PRF input buffer. Inserted after parent key */
    prfInputBuffer[parentKeyLength] = i; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    i++;

    /* #12 Call SHA-256 as PRF */
    retVal = E_NOT_OK;
    /* CRYPTO_OPERATIONMODE_START: */
    if (esl_initWorkSpaceHeader(&workspaceSHA256.header, ESL_MAXSIZEOF_WS_SHA256, CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
    {
      if ( esl_initSHA256(&workspaceSHA256) == ESL_ERC_NO_ERROR ) /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
      {
        /* CRYPTO_OPERATIONMODE_UPDATE: */
        if ( esl_updateSHA256(&workspaceSHA256, (eslt_Length)prfInputBufferLength, prfInputBuffer) == ESL_ERC_NO_ERROR ) /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_PTR */
        {
          /* CRYPTO_OPERATIONMODE_FINISH: */
          if ( esl_finalizeSHA256(&workspaceSHA256, hash) == ESL_ERC_NO_ERROR ) /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
          {
            retVal = E_OK;
          }
        }
      }
    }

    /* #13 Copy the PRF result to the end of the target key buffer */
    if (retVal == E_OK)
    {
      if ( ( derivedLength + ESL_SIZEOF_SHA256_DIGEST ) <= targetKeyLength )
      {
        Crypto_30_LibCv_CopyData( hash, &(targetKey[derivedLength]), ESL_SIZEOF_SHA256_DIGEST ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
      else
      {
        Crypto_30_LibCv_CopyData( hash, &(targetKey[derivedLength]), (uint16)(targetKeyLength - derivedLength) ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
    }
    else
    {
      break;
    }
    /* END Calculate Hash */
  }
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_Kdf_AsymDevideByPrimeMinusOne( )
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Kdf_AsymDevideByPrimeMinusOne(
  P2VAR( uint8, AUTOMATIC, AUTOMATIC ) targetKey,
  CONST( uint32, AUTOMATIC ) targetKeyLength,
  P2CONST( uint8, AUTOMATIC, AUTOMATIC ) primeMinus1Ptr,
  CONST( uint32, AUTOMATIC ) primeLength)
{
  Std_ReturnType retVal = E_OK;
  /* #1 Use the library function actBNReduce to divide big numbers */
  /*  r = a mod b
  **
  **  int actBNReduce(actBNDIGIT* a,
  **                  actBNLENGTH a_length,
  **                  actBNDIGIT* b,
  **                  actBNLENGTH b_length,
  **                  actBNDIGIT* r,
  **                  void (*watchdog) (void));
  **
  **   BigNum reduce ( r = a mod b ).
  **
  ** input:                                              (length in digits)
  ** - a:       first number (will be destroyed)              a_length+1
  ** - b:       second number (will be returned const         b_length
  **            but touched by the normalization)
  ** - r:       result buffer                                 b_length+1
  **
  ** output:
  ** - a:       the normalized result a mod b                 b_length
  ** - b:       second number                                 b_length
  ** - r:       the result a mod b                            b_length
  ** - returns: -1    if exact b_length <= 1 (error)
  **             0    else (success) */

# if ( actBN_BYTES_PER_DIGIT == 1 )  /* COV_CRYPTO_30_LIBCV_ACTBN_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)targetKeyLength;
  actBNDIGIT b[256];
  actBNLENGTH b_length = (actBNLENGTH)primeLength;
  actBNDIGIT r[264];
# elif ( actBN_BYTES_PER_DIGIT == 2 )  /* COV_CRYPTO_30_LIBCV_ACTBN_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264>>1]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)(targetKeyLength >> 1u);
  actBNDIGIT b[256>>1];
  actBNLENGTH b_length = (actBNLENGTH)(primeLength >> 1u);
  actBNDIGIT r[264>>1];
# elif ( actBN_BYTES_PER_DIGIT == 4 )  /* COV_CRYPTO_30_LIBCV_ACTBN_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264>>2]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)(targetKeyLength >> 2u);
  actBNDIGIT b[256>>2];
  actBNLENGTH b_length = (actBNLENGTH)(primeLength >> 2u);
  actBNDIGIT r[264>>2];
# else
#  error "Value of actBN_BYTES_PER_DIGIT is not supported"
# endif

  uint8 one = 1u;
  int res; /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */

  /* #11 Set the so far calculated target key as dividend */
  actBNSetOctetString(a, a_length, targetKey, (int)targetKeyLength); /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

  /* #12 Set the prime-1 as divisor */
  actBNSetOctetString(b, b_length, primeMinus1Ptr, (int)primeLength); /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

  /* #13 Call actBNReduce and store the value in targetKey */
  res = actBNReduce(a, a_length, b, b_length, r, NULL_PTR); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  if (res == 0) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
  {
    /* #2 Add one and return the result */
    actBNSetOctetString(a, a_length, &one, 1u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    (void)actBNAdd(r, a, r, a_length); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    actBNOctetString(targetKey, (int)targetKeyLength, r, a_length); /* PRQA S 5013 */ /* MD_CRYPTO_30_LIBCV_5013 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}
#endif  /* (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_KeyElementSearch()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
/* [SWS_Crypto_91006] */
FUNC(Crypto_30_LibCv_KeyElementsIterType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId)
{
  Crypto_30_LibCv_KeyElementsIterType elementIndex;
  boolean indexFound = FALSE;

  /* #10 Iterate over all elements in the key to find a matching element */
  for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    /* Check if it is the right element */
    if (keyElementId == Crypto_30_LibCv_GetIdOfKeyElements(elementIndex))
    {
      indexFound = TRUE;
      break;
    }
  }

  if (indexFound == FALSE)
  {
    elementIndex = Crypto_30_LibCv_GetSizeOfKeyElements();
  }
  return elementIndex;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyElementIdsGet()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementIdsGet(
  uint32 cryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyElementIdsPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyElementIdsLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00161] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00162] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (keyElementIdsPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00162] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (keyElementIdsLengthPtr == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    Crypto_30_LibCv_KeyElementsIterType elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId);
    uint8 idx = 0u;
    retVal = E_OK;

    /* #20 Iterate over all Key Element Ids */
    while (elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId))
    {
      /* #25 Copy Key Element Id, if the buffer is to small return with error */
      if (idx < *(keyElementIdsLengthPtr))
      {
        keyElementIdsPtr[idx] = Crypto_30_LibCv_GetIdOfKeyElements(elementIndex); /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
      }
      else
      {
        /* provided array is too small to store all element ids */
        retVal = CRYPTO_E_SMALL_BUFFER;
        /* [SWS_Crypto_00164] Check that provided buffer is big enough */
        errorId = CRYPTO_E_SMALL_BUFFER;
        break;
      }

      idx++;
      elementIndex++;
    }

    /* #30 save written data length */
    *keyElementIdsLengthPtr = idx; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_ELEMENT_IDS_GET,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyCopy()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyCopy(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, retValBuf;
  Crypto_30_LibCv_KeyElementsIterType elementIndexSrc;
  Crypto_30_LibCv_KeyElementsIterType elementIndexDst;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00156] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00157] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00158] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    /* #20 Loop over all elements of the source key */
    for (elementIndexSrc = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndexSrc < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndexSrc++)
    {
      /* #25 Loop over all elements of the destination key to find a matching element for the current source element */
      elementIndexDst = Crypto_30_LibCv_Local_KeyElementSearch(targetCryptoKeyId, Crypto_30_LibCv_GetIdOfKeyElements(elementIndexSrc));

      if (elementIndexDst < Crypto_30_LibCv_GetSizeOfKeyElements())
      {
        /* #30 copy key element with Crypto_30_LibCv_Local_KeyElementCopy */
        retValBuf = Crypto_30_LibCv_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
        if (retVal != E_OK)
        {
          retVal = retValBuf;
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_COPY,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyElementCopy()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementCopy(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 targetCryptoKeyId,
  uint32 targetKeyElementId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyElementsIterType elementIndexSrc;
  Crypto_30_LibCv_KeyElementsIterType elementIndexDst;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00149] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00150] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00151] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    /* [SWS_Crypto_00152] if keyElementId is out of valid range return E_NOT_OK */
    /* [SWS_Crypto_00153] if targetKeyElementId is out of valid range return E_NOT_OK */
    retVal = E_NOT_OK;
    /* #20 Find source element. Otherwise return with Error. */
    elementIndexSrc = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId);

    if (elementIndexSrc < Crypto_30_LibCv_GetSizeOfKeyElements())
    {
      /* #25 Find destination element. Otherwise return with Error. */
      elementIndexDst = Crypto_30_LibCv_Local_KeyElementSearch(targetCryptoKeyId, targetKeyElementId);

      if (elementIndexDst < Crypto_30_LibCv_GetSizeOfKeyElements())
      {
        /* [SWS_Crypto_00152] Check that keyElementId is in valid range */
        /* [SWS_Crypto_00153] Check that targetKeyElementId is in valid range */
        errorId = CRYPTO_E_NO_ERROR;

        /* #30 copy key element with Crypto_30_LibCv_Local_KeyElementCopy */
        retVal = Crypto_30_LibCv_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_ELEMENT_COPY,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyElementSetInternal()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSetInternal(uint32 elementIndex, P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr, uint32 keyLength)
{
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex = 0u;

  keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Copy keyPtr to key storage */
  while (keyPtrIndex < keyLength)
  { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
    Crypto_30_LibCv_SetKeyStorage(keyStorageIndex, keyPtr[keyPtrIndex]); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
    keyStorageIndex++;
    keyPtrIndex++;
  }

  Crypto_30_LibCv_SetKeyElementWrittenLength(elementIndex, keyPtrIndex);
}

#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
/**********************************************************************************************************************
* Crypto_30_LibCv_KeyElementSetInternalVirtual()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSetInternalVirtual(uint32 cryptoKeyId, uint32 elementIndex, P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr, uint32 keyLength)
{
  Crypto_30_LibCv_KeyElementsIterType virtualElementIndex;
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex;

  virtualElementIndex = elementIndex;
  /* set root element only with the virtual element */
  /* #10 find root key element */
  for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    if (Crypto_30_LibCv_GetKeyElementInfoVirtualIdxOfKeyElementInfo(virtualElementIndex) == Crypto_30_LibCv_GetIdOfKeyElements(elementIndex))
    {
      /* #15 check if the length is matching  */
      if (Crypto_30_LibCv_GetKeyElementLength(elementIndex) >= Crypto_30_LibCv_GetKeyElementVirtualOffset(virtualElementIndex) + Crypto_30_LibCv_GetKeyElementWrittenLength(virtualElementIndex))
      {
        /* #20 copy key to root key element  */
        for (keyPtrIndex = 0u, keyStorageIndex = (Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex) + (Crypto_30_LibCv_KeyStorageIterType)Crypto_30_LibCv_GetKeyElementVirtualOffset(virtualElementIndex)); keyPtrIndex < Crypto_30_LibCv_GetKeyElementWrittenLength(virtualElementIndex); keyStorageIndex++, keyPtrIndex++)
        {
          Crypto_30_LibCv_SetKeyStorage(keyStorageIndex, keyPtr[keyPtrIndex]);
        }
        Crypto_30_LibCv_SetKeyElementWrittenLength(elementIndex, keyPtrIndex);
      }
    }
    retVal = E_OK;
  }
  retVal = E_OK;

  return retVal;
}
#endif

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_KeyElementSet()
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
 */
/* [SWS_Crypto_91004] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  Crypto_30_LibCv_KeyElementsIterType elementIndex;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Find key element, otherwise return with Error */

  elementIndex = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId);

  if (elementIndex < Crypto_30_LibCv_GetSizeOfKeyElements())
  {
    Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    /* #15 Handle not virtual key element */
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
    if (Crypto_30_LibCv_IsKeyElementVirtual(elementIndex) == FALSE)
#endif
    {
      /* #20 Copy data if there is enough free space */
      /* Check if the available data is not bigger than the maximum size of the destination element */
      if ((keyLength == Crypto_30_LibCv_GetKeyElementLength(elementIndex)) || ((Crypto_30_LibCv_IsKeyElementPartial(elementIndex) == TRUE) && (keyLength <= Crypto_30_LibCv_GetKeyElementLength(elementIndex))))
      {
        Crypto_30_LibCv_KeyElementSetInternal(elementIndex, keyPtr, keyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
        retVal = E_OK;
      }
#if (CRYPTO_30_LIBCV_SUPPORT_SHE_KEY_UPDATE == STD_ON)
      /* #25 Else: if key size matching update using SHE key update protocol */
      /* Key update using SHE key update protocol */
      else if ((keyLength == CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_KEY) && (Crypto_30_LibCv_GetKeyElementLength(elementIndex) == CRYPTO_30_LIBCV_SIZEOF_SHE_KEY))
      {
        if ((Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex)) == CRYPTO_30_LIBCV_WA_ENCRYPTED)
          && (Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex)) == CRYPTO_30_LIBCV_RA_DENIED))
        {
          retVal = Crypto_30_LibCv_SheKeyUpdate(cryptoKeyId, elementIndex, keyPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
        }
        else
        {
          retVal = CRYPTO_E_KEY_WRITE_FAIL;
        }
      }
#endif
      else
      {
        /* 30 Else: Key length is to large */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH; /* [SWS_Crypto_00146] */
      }
    }
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
    /* #35 Else handle virtual key */
    else
    {
      /* #40 Copy key element data if data size match or partial access is enabled  */
      if (((Crypto_30_LibCv_GetKeyStorageEndIdxOfKeyElements(elementIndex) - Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex)) == keyLength)
        || (Crypto_30_LibCv_IsKeyElementPartial(elementIndex) == TRUE))
      {
        retVal = Crypto_30_LibCv_KeyElementSetInternalVirtual(cryptoKeyId, elementIndex, keyPtr, keyLength);
      }
      else
      {
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH; /* [SWS_Crypto_00146] */
      }
    }
#endif

    if (retVal == E_OK)
    {
      Crypto_30_LibCv_ClearKeyState(cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_LIBCV_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
    }
    else
    {
      Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyElementSet()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91004] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized == CRYPTO_30_LIBCV_UNINIT)
  {
    errorId = CRYPTO_E_UNINIT; /* [SWS_Crypto_00075] */
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    errorId = CRYPTO_E_PARAM_HANDLE; /* [SWS_Crypto_00076] */
  }
  else if (keyPtr == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* [SWS_Crypto_00078] */
  }
  else if (keyLength == 0)
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* [SWS_Crypto_00079] */
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #20 set key element using Crypto_30_LibCv_Local_KeyElementSet */
    retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, keyElementId, keyPtr, keyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    if (retVal == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      /* [SWS_Crypto_00077] check if keyElementId is in valid range */
      errorId = CRYPTO_E_PARAM_HANDLE;
      retVal = E_NOT_OK;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_ELEMENT_SET,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyValidSet()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91005] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyValidSet(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    errorId = CRYPTO_E_UNINIT; /* [SWS_Crypto_00082] */
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    errorId = CRYPTO_E_PARAM_HANDLE; /* [SWS_Crypto_00083] */
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */

    /* #20 Set the given KeyId valid */
    Crypto_30_LibCv_SetKeyState(cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK);

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #25 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_VALID_SET,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_Local_KeyElementGet()
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
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  Crypto_30_LibCv_KeyElementsIterType elementIndex;
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
  Crypto_30_LibCv_KeyElementsIterType virtualElementIndex;
#endif
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex;

  {
    /* ----- Implementation ------------------------------------------------- */

    /* #10 Search Key Element */
    elementIndex = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId);
    if (elementIndex < Crypto_30_LibCv_GetSizeOfKeyElements())
    {
      /* #15 Check if the Key element is valid */
      if (TRUE == Crypto_30_LibCv_IsKeyElementValid(elementIndex))
      {
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
        /* Check if the element is not virtual */
        if (Crypto_30_LibCv_IsKeyElementVirtual(elementIndex) == FALSE)
#endif
        {
          /* #20 Handle not virtual key */
          /* Check if the buffer for the result is large enough.
          Depending on the actual length of the element the data is stored or an error is generated if the buffer is to small */

          /* #25 Copy Key element to outputbuffer if size is adequate */
          if (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex) <= *resultLengthPtr)
          {
            keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
            for (keyPtrIndex = 0u; keyPtrIndex < Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex); keyPtrIndex++) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            {
              resultPtr[keyPtrIndex] = Crypto_30_LibCv_GetKeyStorage(keyStorageIndex); /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
              keyStorageIndex++;
            }
            /* [SWS_Crypto_00092] */
            *resultLengthPtr = keyPtrIndex; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
            retVal = E_OK;
          }
          else
          {
            /* [SWS_Crypto_00093] */
            retVal = CRYPTO_E_SMALL_BUFFER;
          }
        }
#if (CRYPTO_30_LIBCV_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_LIBCV_VIRTUALKEY */
        /* #30 Handle virtual key */
        else /* Element is virtual */
        {
          /* #35 Copy Key element to outputbuffer if size is adequate */
          /* Check if the buffer for the result is great enough.
          Depending on the partial access property of the element the data is stored or an error is generated if the buffer is to small */
          if ((Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex) <= *resultLengthPtr)
            || ((Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex) > *resultLengthPtr)
            && (Crypto_30_LibCv_IsKeyElementPartial(elementIndex) == FALSE)))
          {
            /* store the handle for the virtual element for later use */
            virtualElementIndex = elementIndex;

            /* Iterate over the whole key to find the root element of the virtual element */
            for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
            {
              /* Check if the element is the root of the virtual element */
              if (Crypto_30_LibCv_GetKeyElementVirtualRoot(virtualElementIndex) == Crypto_30_LibCv_GetIdOfKeyElements(elementIndex))
              {
                /* Copy the data from the root element to the result buffer */

                for (keyPtrIndex = 0u, keyStorageIndex = (Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex) + (Crypto_30_LibCv_KeyStorageIterType)Crypto_30_LibCv_GetKeyElementVirtualOffset(virtualElementIndex)); keyPtrIndex < Crypto_30_LibCv_GetKeyElementWrittenLength(virtualElementIndex); keyStorageIndex++, keyPtrIndex++)
                {
                  /* write until buffer is full or no more data available */
                  if (*resultLengthPtr > keyPtrIndex)
                  {
                    resultPtr[keyPtrIndex] = Crypto_30_LibCv_GetKeyStorage(keyStorageIndex);
                  }
                }
                /* [SWS_Crypto_00092] */
                *resultLengthPtr = keyPtrIndex;
              }
              retVal = E_OK;
            }
          }
          else
          {
            /* [SWS_Crypto_00093] */
            return CRYPTO_E_SMALL_BUFFER;
          }
        }
#endif
      }
      /* #40 Else key is not valid */
      else
      {
        /* [SWS_Crypto_00039] */
        retVal = CRYPTO_E_KEY_NOT_VALID;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyElementGet()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00085] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00086] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (resultPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00088] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00089] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (*resultLengthPtr == 0u)
  {
    /* [SWS_Crypto_00090] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */

    /* #20 get key element using Crypto_30_LibCv_Local_KeyElementGet */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
    retVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, keyElementId, resultPtr, resultLengthPtr); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

    if (retVal == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      /* [SWS_Crypto_00087] Check that keyElementId is in valid range */
      errorId = CRYPTO_E_PARAM_HANDLE;
      retVal = E_NOT_OK;
    }
    else if (retVal == CRYPTO_E_KEY_NOT_VALID)
    {
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
    else
    {
      /* keep retVal */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_ELEMENT_GET,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_RandomSeed()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91013] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RandomSeed(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00128] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00129] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (entropyPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00130] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (entropyLength == 0u)
  {
    /* [SWS_Crypto_00131] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    Crypto_30_LibCv_KeyElementsIterType elementIndex;

    /* #20 Generate and Store Random Seed */
    if (Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, entropyPtr, entropyLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
    {
      elementIndex = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE);
      /* The elementIndex do not need to be checked. The Code is only reachable if the key elment is available. (Crypto_30_LibCv_Local_KeyElementSet) */
      Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_MASK | CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_RNG_SEED */
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #25 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_RANDOM_SEED,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyGenerate()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

/* [SWS_Crypto_91007] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyGenerate(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00094] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00095] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_GENERATE,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyDerive()
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
 *
 *
 */
/* [SWS_Crypto_91008] */
/* PRQA S 0715 KEYDERIVE0715 */ /* MD_MSR_1.1_715 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId )
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00097] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00098] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_?????] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
#if (CRYPTO_30_LIBCV_SUPPORT_KEY_DERIVE == STD_ON)
    uint8 deriveAlgorithm = 0xFFu;

    /* Input buffer for the Pseudo Random Function. */
    /* Buffer size is 256 byte maximum parent key length + 12 byte ( 1 byte iteration | 4 byte Label | 1 byte 0x00 | 6 byte of salt ( 2 byte TypeID | 2 byte training Counter | 2 byte target Key Length in littel endian (k) ) ) */
    uint8 prfInputBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_PRF_INPUT_BUFFER];
    uint32 prfInputBufferLength = 0u;

    uint8 saltBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_SALT_ASYMMETRIC];
    uint8* primeMinus1Ptr = NULL_PTR;
    uint16 primeLength = 0u;

    uint8 targetKey[CRYPTO_30_LIBCV_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8] = { 0 }; /* 256+8 for asym KDF */
    uint32 targetKeyLength;

    uint32 parentKeyLength;

    /* #2 Handle input parameter */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
    retVal = Crypto_30_LibCv_Local_Kdf_HandleInputParams( cryptoKeyId, targetCryptoKeyId, &deriveAlgorithm, prfInputBuffer, &prfInputBufferLength, saltBuffer, &primeMinus1Ptr, &primeLength, &targetKeyLength, &parentKeyLength ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

    /* #3 Call PRF */
    if (retVal == E_OK)
    {
      retVal = Crypto_30_LibCv_Local_Kdf_CallPRF( prfInputBuffer, prfInputBufferLength, targetKey, targetKeyLength, parentKeyLength ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }

    /* #4 Handle the results of the PRF according to the algorithm */
    if( retVal == E_OK )
    {
      if( deriveAlgorithm == CRYPTO_30_LIBCV_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256 )
      {
        /* #41 Symmetric Key Derive finished here. Copy calculated key to key element */
        retVal = Crypto_30_LibCv_Local_KeyElementSet( targetCryptoKeyId, 1, targetKey, targetKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      }
      else if (deriveAlgorithm == CRYPTO_30_LIBCV_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB) /* COV_CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM */
      {
        /* #42 For Asymmetric Key Derive: */

        /* #421 Calculate private key for asymmetric keys only by dividing key by given prime-1 */
        retVal = Crypto_30_LibCv_Local_Kdf_AsymDevideByPrimeMinusOne( targetKey, targetKeyLength, primeMinus1Ptr, primeLength ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

        if( retVal == E_OK )
        {
          /* Skip the first 8 additional bytes */
          /* 422 Asymmetric Key Derive finished. Copy calculated key to key element */
          retVal = Crypto_30_LibCv_Local_KeyElementSet(targetCryptoKeyId, 1, &(targetKey[8]), targetKeyLength - 8); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        }
      }
      else
      {
        /* No other algorithm type supported */
        retVal = E_NOT_OK;
      }
    }

    /* #5 If key was derived successfully, mark it as valid */
    if (retVal == E_OK)
    {
      Crypto_30_LibCv_SetKeyState( targetCryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK);
    }
    else
    {
      retVal = E_NOT_OK;
    }
#else
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(targetCryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID, CRYPTO_30_LIBCV_INSTANCE_ID, CRYPTO_30_LIBCV_SID_KEY_DERIVE, errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}  /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
/* PRQA L:KEYDERIVE0715 */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyExchangeCalcPubVal()
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
 */
/* [SWS_Crypto_91009] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_API_REQUIREMENT */
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_API_REQUIREMENT */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00103] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00104] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (publicValuePtr == NULL_PTR)
  {
    /* [SWS_Crypto_00105] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (publicValueLengthPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00106] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (*publicValueLengthPtr == 0u)
  {
    /* [SWS_Crypto_00107] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  /* vismaw: the retVal is already captured in the methods body */
  /* [SWS_Crypto_00110] */
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */

#if  (CRYPTO_30_LIBCV_SUPPORT_PUBLIC_KEY_EXCHANGE == STD_ON)
    uint8 publicKey[ESL_SIZEOF_X25519_PUBLIC_KEY];
    eslt_Length publicKeyLength = ESL_SIZEOF_X25519_PUBLIC_KEY;
    uint8_least i;
    eslt_WorkSpaceX25519 localWsX25519;
    eslt_ErrorCode eslRet;
    Std_ReturnType retValKeyElementSetPrivateKey = E_NOT_OK;
    Std_ReturnType retValKeyElementSetPublicKey = E_NOT_OK;

    /* #20 Init Workspace */
    eslRet = esl_initWorkSpaceHeader(&localWsX25519.header, ESL_SIZEOF_WS_X25519, CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      eslRet = esl_initECDH((CRYPTOCV_P2VAR_PARA(eslt_WorkSpace))&localWsX25519, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
    }

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      /* #25 Calculate Keys*/

      if (esl_generateEphemeralKeyPairECDH((CRYPTOCV_P2VAR_PARA(eslt_WorkSpace))&localWsX25519, publicKey, &publicKeyLength) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
      {
        /* #30 Store public and private key to key elements if calculation was successfully */
        retValKeyElementSetPrivateKey = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, ((actX25519Struct*)localWsX25519.wsX25519)->privateKey, ESL_SIZEOF_X25519_PRIVATE_KEY); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        retValKeyElementSetPublicKey = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, publicKey, ESL_SIZEOF_X25519_PUBLIC_KEY); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      }

      if ((retValKeyElementSetPrivateKey == E_OK) && (retValKeyElementSetPublicKey == E_OK))
      {
        /* #35 Copy public key to output buffer and overwrite local copy if store key was successfully */
        /* [SWS_Crypto_00109] return public key */
        if (*publicValueLengthPtr >= ESL_SIZEOF_X25519_PUBLIC_KEY) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */

          for (i = 0u; i < ESL_SIZEOF_X25519_PUBLIC_KEY; i++)
          {
            publicValuePtr[i] = publicKey[i]; /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */

            /* delete buffer content from RAM */
            publicKey[i] = 0x00u; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
          }
          *publicValueLengthPtr = ESL_SIZEOF_X25519_PUBLIC_KEY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
          retVal = E_OK;
        }
        else
          /* #40 If buffer is too small return with error */
        { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          retVal = CRYPTO_E_SMALL_BUFFER;

# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
          /* [SWS_Crypto_00110] */
          errorId = CRYPTO_E_PARAM_VALUE;
# endif
        }
      }
    }
#else
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(publicValuePtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(publicValueLengthPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #45 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_PUB_VAL,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_KeyExchangeCalcSecret()
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
 */
/* [SWS_Crypto_91010] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00111] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00112] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    /* [SWS_Crypto_00113] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (partnerPublicValueLength == 0u)
  {
    /* [SWS_Crypto_00115] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else if (partnerPublicValueLength != ESL_SIZEOF_X25519_PUBLIC_KEY)
  {

  }
  else
#else
  if (partnerPublicValueLength != ESL_SIZEOF_X25519_PUBLIC_KEY)
  {

  }
  else
#endif
  {
    /* ----- Runtime Error Check ------------------------------------------------- */
#if  (CRYPTO_30_LIBCV_SUPPORT_PUBLIC_KEY_EXCHANGE == STD_ON)

    /* ----- Implementation ------------------------------------------------- */
    uint8 keyExchangePrivateKey[ESL_SIZEOF_X25519_PRIVATE_KEY];
    uint8 sharedSecret[ESL_SIZEOF_X25519_SHARED_SECRET];
    uint32 keyExchangePrivateKeyLength = ESL_SIZEOF_X25519_PRIVATE_KEY;
    eslt_Length sharedSecretLength_eslt = ESL_SIZEOF_X25519_SHARED_SECRET;
    eslt_WorkSpaceX25519 localWsX25519;
    eslt_ErrorCode eslRet;

    /* #20 Initialize workspace */
    /* initialize Ed25519 */
    eslRet = esl_initWorkSpaceHeader(&localWsX25519.header, ESL_SIZEOF_WS_X25519, CRYPTO_30_LIBCV_KEY_M_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      eslRet = esl_initECDH((CRYPTOCV_P2VAR_PARA(eslt_WorkSpace))&localWsX25519, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
    }

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      /* #25 Load private key */
      /* copy content of key elements to local buffers */
      Std_ReturnType localRetVal;

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();
      localRetVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, keyExchangePrivateKey, &keyExchangePrivateKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_2();

      if (E_OK == localRetVal)
      {
        /* import private key from key elements */
        if (esl_importStaticPrivateKeyECDH((CRYPTOCV_P2VAR_PARA(eslt_WorkSpace))&localWsX25519, keyExchangePrivateKey) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
        {
          /* #30 generate secret by using public key from key element */
          eslRet = esl_generateSharedSecretECDH((CRYPTOCV_P2VAR_PARA(eslt_WorkSpace))&localWsX25519, (CRYPTOCV_P2VAR_PARA(eslt_Byte))partnerPublicValuePtr, sharedSecret, &sharedSecretLength_eslt); /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

          if (eslRet == ESL_ERC_NO_ERROR)
          {
            /* #35 Store calculated sharedSecret */
            retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, sharedSecret, (uint32)sharedSecretLength_eslt); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
            /* #40 delete buffer content from RAM */

            Crypto_30_LibCv_ClearData(keyExchangePrivateKey, ESL_SIZEOF_X25519_KEY); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
            Crypto_30_LibCv_ClearData(sharedSecret, ESL_SIZEOF_X25519_KEY); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
          }
        }
      }
    }
#else
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(partnerPublicValuePtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(partnerPublicValueLength); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #45 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_SECRET,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_LibCv_CertificateParse()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CertificateParse(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00168] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00169] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */

    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_CERTIFICATE_PARSE,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_LibCv_CertificateVerify()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CertificateVerify(
  uint32 cryptoKeyId,
  uint32 verifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_ModuleInitialized != CRYPTO_30_LIBCV_INITIALIZED)
  {
    /* [SWS_Crypto_00172] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00173] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (verifyCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00174] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (verifyPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00175] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(verifyCryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(verifyPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_CERTIFICATE_VERIFY,
      errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyManagement.c
 *********************************************************************************************************************/
