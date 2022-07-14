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
/*!        \file  E2EXf.h
 *        \brief  Vector AUTOSAR E2EXf header file
 *
 *      \details  This is the main header file of the Vector AUTOSAR module E2EXf.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Stephanie Schaaf              visssf        Vector Informatik GmbH
 *  Philipp Niethammer            vispnr        Vector Informatik GmbH
 *  Christian Fischer             viscfr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-09-21  visssf  ESCAN00091543 FEAT-1292: Support E2EXf transformer
 *  01.00.01  2016-12-22  visssf  ESCAN00093409 MISRA deviation: MISRA-C:2004 Rule 5.1
 *            2016-12-22  visssf  ESCAN00093410 Add missing MISRA justification comment
 *  01.01.00  2017-01-31  visssf  FEATC-835     FEAT-2252: Support for E2E profile 7
 *  01.02.00  2017-05-24  visssf  STORYC-681    E2EXf: Allow RTE and Transformers to be released independently
 *  01.03.00  2017-07-24  vispnr  ESCAN00095978 Introduction of SafeBswChecks configuration parameter
 *            2017-08-08  viscfr  ESCAN00096217 Optimization: Use MSN_DUMMY_STATEMENT macro in transformer modules
 *            2017-08-17  vispnr  STORYC-2015   Improved variant coverage, updated SIDs to match Autosar 4.3.0
 *********************************************************************************************************************/

#if !defined (E2EXF_H)
# define E2EXF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"
# include "E2EXf_LCfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define E2EXF_VENDOR_ID                         (30U)
# define E2EXF_MODULE_ID                         (0xB0U)

/* AUTOSAR Software specification version information */
# define E2EXF_AR_RELEASE_MAJOR_VERSION          (4U)
# define E2EXF_AR_RELEASE_MINOR_VERSION          (2U)
# define E2EXF_AR_RELEASE_REVISION_VERSION       (2U)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define E2EXF_SW_MAJOR_VERSION                  (1U)
# define E2EXF_SW_MINOR_VERSION                  (3U)
# define E2EXF_SW_PATCH_VERSION                  (0U)

# define E2EXF_INSTANCE_ID_DET                   (0x00U)

/* ----- API service IDs ----- */
# define E2EXF_SID_GET_VERSION_INFO              (0x00U) /*!< Service ID: E2EXf_GetVersionInfo() */
# define E2EXF_SID_INIT                          (0x01U) /*!< Service ID: E2EXf_Init() */
# define E2EXF_SID_DE_INIT                       (0x02U) /*!< Service ID: E2EXf_DeInit() */
# define E2EXF_SID_TRANSFORMER_ID                (0x03U) /*!< Service ID: E2EXf_<transformerId>() */
# define E2EXF_SID_INV_TRANSFORMER_ID            (0x04U) /*!< Service ID: E2EXf_Inv_<transformerId>() */

/* ----- Error codes ----- */
# define E2EXF_E_NO_ERROR                        (0x00U) /*!< used to check if no error occurred - use a value unequal to any error code */
# define E2EXF_E_UNINIT                          (0x01U) /*!< Error code: API service used without module initialization */
# define E2EXF_E_INIT_FAILED                     (0x02U) /*!< Error code: Invalid configuration set was selected  */
# define E2EXF_E_PARAM                           (0x03U) /*!< Error code: API service called with wrong parameter */
# define E2EXF_E_PARAM_POINTER                   (0x04U) /*!< Error code: API service called with invalid pointer  */

/* ----- Errors of safety transformers ----- */
/* Note: E_OK is defined in Std_Types as (0x00U) - Error code: The communication is safe. */
# define E_SAFETY_VALID_REP                     (0x01U) /*!< Error code: The data are valid according to safety, although data with a repeated counter were received. */
# define E_SAFETY_VALID_SEQ                     (0x02U) /*!< Error code: The data are valid according to safety, although a counter jump occurred. */
# define E_SAFETY_VALID_ERR                     (0x03U) /*!< Error code: The data are valid according to safety, although the check itself failed. */
# define E_SAFETY_NODATA_OK                     (0x20U) /*!< Error code: No data are available since initialization of transformer. */
# define E_SAFETY_NODATA_REP                    (0x21U) /*!< Error code: No data are available since initialization of transformer because a repeated counter was received. */
# define E_SAFETY_NODATA_SEQ                    (0x22U) /*!< Error code: No data are available since initialization of transformer and a counter jump occurred. */
# define E_SAFETY_NODATA_ERR                    (0x23U) /*!< Error code: No data are available since initialization of transformer. Therefore the check failed. */
# define E_SAFETY_INIT_OK                       (0x30U) /*!< Error code: Not enough data were received to use them. */
# define E_SAFETY_INIT_REP                      (0x31U) /*!< Error code: Not enough data were received to use them but some with a repeated counter were received. */
# define E_SAFETY_INIT_SEQ                      (0x32U) /*!< Error code: Not enough data were received to use them, additionally a counter jump occurred. */
# define E_SAFETY_INIT_ERR                      (0x33U) /*!< Error code: Not enough data were received to use them, additionally a check failed. */
# define E_SAFETY_INVALID_OK                    (0x40U) /*!< Error code: The data are invalid and cannot be used. */
# define E_SAFETY_INVALID_REP                   (0x41U) /*!< Error code: The data are invalid and cannot be used because a repeated counter was received. */
# define E_SAFETY_INVALID_SEQ                   (0x42U) /*!< Error code: The data are invalid and cannot be used due to a counter jump. */
# define E_SAFETY_INVALID_ERR                   (0x43U) /*!< Error code: The data are invalid and cannot be used because a check failed. */
# define E_SAFETY_SOFT_RUNTIMEERROR             (0x77U) /*!< Error code: A runtime error occured, safety properties could not be checked (state or status
                                                                          cannot be determined) but non-protected output data could be produced nonetheless. */
# define E_SAFETY_HARD_RUNTIMEERROR             (0xFFU) /*!< Error code: A runtime error occured, safety properties could not be checked and no output data could be produced. */


/* ----- Modes ----- */
# define E2EXF_UNINIT                            (0x00U)
# define E2EXF_INIT                              (0x01U)

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

# define E2EXF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  E2EXf_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, E2EXF_CODE) E2EXf_InitMemory(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  E2EXf_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module E2EXf. It initializes all variables and sets the module state to
 *               initialized.
 *  \trace       CREQ-131007
 *  \param[in]   config                  Pointer to a selected configuration structure, in the post-build-selectable
 *                                       variant. NULL in link-time variant.
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         E2EXf_InitMemory has been called unless E2EXf_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, E2EXF_CODE) E2EXf_Init(P2CONST(E2EXf_ConfigType, AUTOMATIC, E2EXF_PBCFG) config); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  E2EXf_DeInit()
 *********************************************************************************************************************/
/*! \brief       De-initialization function
 *  \trace       CREQ-131009
 *  \details     This function deinitializes the module E2EXf. It sets the module state to uninitialized.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, E2EXF_CODE) E2EXf_DeInit(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

# if (E2EXF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     E2EXf_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \trace       CREQ-131010
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, E2EXF_CODE) E2EXf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, E2EXF_APPL_VAR) versioninfo); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# endif

# if (E2EXF_P01_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P01()
 *********************************************************************************************************************/
/*! \brief         Transform function of E2E Transformer for E2Elib Profile 1
 *  \details       Protects the data to be transmitted.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P01 config structure.
 *  \param[in,out] state                   Pointer to P01 state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P01(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P01ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state);
# endif /* (E2EXF_P01_ENABLED == STD_ON) */

# if (E2EXF_P02_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P02()
 *********************************************************************************************************************/
/*! \brief         Transform function of E2E Transformer for E2Elib Profile 2
 *  \details       Protects the data to be transmitted.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P02 config structure.
 *  \param[in,out] state                   Pointer to P02 state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P02(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state);
# endif /* (E2EXF_P02_ENABLED == STD_ON) */

# if (E2EXF_P04_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P04()
 *********************************************************************************************************************/
/*! \brief         Transform function of E2E Transformer for E2Elib Profile 4
 *  \details       Protects the data to be transmitted.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P04 config structure.
 *  \param[in,out] state                   Pointer to P04 state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P04(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P04ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state);
# endif /* (E2EXF_P04_ENABLED == STD_ON) */

# if (E2EXF_P05_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P05()
 *********************************************************************************************************************/
/*! \brief         Transform function of E2E Transformer for E2Elib Profile 5
 *  \details       Protects the data to be transmitted.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P05 config structure.
 *  \param[in,out] state                   Pointer to P05 state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P05(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P05ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P05ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state);
# endif /* (E2EXF_P05_ENABLED == STD_ON) */

# if (E2EXF_P06_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P06()
 *********************************************************************************************************************/
/*! \brief         Transform function of E2E Transformer for E2Elib Profile 6
 *  \details       Protects the data to be transmitted.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P06 config structure.
 *  \param[in,out] state                   Pointer to P06 state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P06(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P06ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state);
# endif /* (E2EXF_P06_ENABLED == STD_ON) */

# if (E2EXF_P07_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P07()
 *********************************************************************************************************************/
/*! \brief         Transform function of E2E Transformer for E2Elib Profile 7
 *  \details       Protects the data to be transmitted.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P07 config structure.
 *  \param[in,out] state                   Pointer to P07 state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P07(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P07ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state);
# endif /* (E2EXF_P07_ENABLED == STD_ON) */

# if (E2EXF_P01_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P01_Inv()
 *********************************************************************************************************************/
/*! \brief         Inverse transform function of E2E Transformer for E2Elib Profile 1
 *  \details       Checks the received data. If the data can be used by the caller, then the function returns E_OK.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P01 config structure.
 *  \param[in,out] state                   Pointer to P01 state structure.
 *  \param[in]     smConfig                Pointer to SM config structure.
 *  \param[in,out] smState                 Pointer to SM state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P01_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P01CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState);
# endif /* (E2EXF_P01_ENABLED == STD_ON) */

# if (E2EXF_P02_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P02_Inv()
 *********************************************************************************************************************/
/*! \brief         Inverse transform function of E2E Transformer for E2Elib Profile 2
 *  \details       Checks the received data. If the data can be used by the caller, then the function returns E_OK.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P02 config structure.
 *  \param[in,out] state                   Pointer to P02 state structure.
 *  \param[in]     smConfig                Pointer to SM config structure.
 *  \param[in,out] smState                 Pointer to SM state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P02_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState);
# endif /* (E2EXF_P02_ENABLED == STD_ON) */

# if (E2EXF_P04_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P04_Inv()
 *********************************************************************************************************************/
/*! \brief         Inverse transform function of E2E Transformer for E2Elib Profile 4
 *  \details       Checks the received data. If the data can be used by the caller, then the function returns E_OK.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P04 config structure.
 *  \param[in,out] state                   Pointer to P04 state structure.
 *  \param[in]     smConfig                Pointer to SM config structure.
 *  \param[in,out] smState                 Pointer to SM state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P04_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P04ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState);
# endif /* (E2EXF_P04_ENABLED == STD_ON) */

# if (E2EXF_P05_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P05_Inv()
 *********************************************************************************************************************/
/*! \brief         Inverse transform function of E2E Transformer for E2Elib Profile 5
 *  \details       Checks the received data. If the data can be used by the caller, then the function returns E_OK.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P05 config structure.
 *  \param[in,out] state                   Pointer to P05 state structure.
 *  \param[in]     smConfig                Pointer to SM config structure.
 *  \param[in,out] smState                 Pointer to SM state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P05_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P05ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P05CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState);
# endif /* (E2EXF_P05_ENABLED == STD_ON) */

# if (E2EXF_P06_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P06_Inv()
 *********************************************************************************************************************/
/*! \brief         Inverse transform function of E2E Transformer for E2Elib Profile 6
 *  \details       Checks the received data. If the data can be used by the caller, then the function returns E_OK.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P06 config structure.
 *  \param[in,out] state                   Pointer to P06 state structure.
 *  \param[in]     smConfig                Pointer to SM config structure.
 *  \param[in,out] smState                 Pointer to SM state structure.
 *  \return                                Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P06_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P06ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState);
# endif /* (E2EXF_P06_ENABLED == STD_ON) */

# if (E2EXF_P07_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_P07_Inv()
 *********************************************************************************************************************/
/*! \brief         Inverse transform function of E2E Transformer for E2Elib Profile 7
 *  \details       Checks the received data. If the data can be used by the caller, then the function returns E_OK.
 *  \param[in,out] buffer                  Pointer to buffer.
 *  \param[out]    bufferLength            Pointer to buffer length.
 *  \param[in]     inputBuffer             Pointer to input buffer.
 *  \param[in]     inputBufferLength       Input buffer length.
 *  \param[in]     options                 Pointer to E2E transformer options.
 *  \param[in]     config                  Pointer to P07 config structure.
 *  \param[in,out] state                   Pointer to P07 state structure.
 *  \param[in]     smConfig                Pointer to SM config structure.
 *  \param[in,out] smState                 Pointer to SM state structure.
 *  \return                               Standard return value, indicating success/error.
 *  \pre           Module is initialized.
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2EXF_CODE) E2EXf_P07_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint16, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint16 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P07ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState);
# endif /* (E2EXF_P07_ENABLED == STD_ON) */

# define E2EXF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* E2EXF_H */

/**********************************************************************************************************************
 *  END OF FILE: E2EXf.h
 *********************************************************************************************************************/
