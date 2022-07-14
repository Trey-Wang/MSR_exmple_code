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
/*      \file  Crypto_30_LibCv_Services.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for service dispatch function prototypes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_SERVICE_H)
# define CRYPTO_30_LIBCV_SERVICE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm_Types.h"
# include "Crypto_30_LibCv_Cfg.h"

# if !defined (CRYPTO_30_LIBCV_LOCAL) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
#  define CRYPTO_30_LIBCV_LOCAL static
# endif

# if !defined (CRYPTO_30_LIBCV_LOCAL_INLINE) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
#  define CRYPTO_30_LIBCV_LOCAL_INLINE LOCAL_INLINE
# endif

# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif



/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
*********************************************************************************************************************/

#define CRYPTO_30_LIBCV_SERVICES_WATCHDOG_PTR                (0u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


# if (CRYPTO_30_LIBCV_SERVICE_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHash()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Hash job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHash(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_MAC_GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchMacGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the MacGenerate job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchMacGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif 

# if (CRYPTO_30_LIBCV_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchMacVerify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the MacVerify job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchMacVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherEncrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherEncrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchCipherDecrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherDecrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchCipherDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_AEADENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchAeadEncrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AeadEncrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAeadEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_AEADDECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchAeadDecrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AeadDecrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchAeadDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_SIGNATURE_GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SignatureGenerate job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_SIGNATURE_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureVerify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SignatureVerify job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_LIBCV_SERVICE_RANDOM == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandom()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Random job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
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
extern FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandom(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif
#endif
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_LIBCV_SERVICES.H
 *********************************************************************************************************************/
