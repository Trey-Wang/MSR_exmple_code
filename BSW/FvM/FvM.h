/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file   FvM.h
 *        \brief  FVM header file
 *
 *      \details  This is the header file of the Freshness Value Manager.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Heiko Huebler                 vishho        Vector Informatik GmbH
 *  Florian Roehm                 visfrm        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-11-29  vishho
 *  01.01.00  2017-02-08  vishho  FEATC-1154: FvM support additional TMC Features
 *  01.01.01  2017-02-14  vishho  ESCAN00093994: Rest Counter Rx Sync not functional
 *  01.02.00  2017-02-20  vishho  FEATC-1186: Release TMC Security Modules: Freshness Handling (FvM)
 *            2017-02-28  visfrm  FEATC-1186: Release TMC Security Modules: Freshness Handling (FvM)
 *            2017-03-07  vishho  ESCAN00094274: FvM does not check the ClearAcceptanceWindow above boundary
 *  02.00.00  2017-05-04  vishho  STORYC-764: SecOC Start Timing in FvM
 *            2017-05-09  vishho  STORYC-766: Inform FV overflow
 *            2017-05-10  visfrm  STORYC-765: Overflow of Trip/Reset/Message Counter Behavior
 *            2017-05-12  vishho  ESCAN00094804: Compiler error: GetFreshnessValueLayoutInfoPreviousIdxOfRxFreshnessValueInfo not defined
 *            2017-05-16  visfrm  STORYC-163: API Pattern, ReviewHelper, MISRA for FvM
 *  02.01.00  2017-06-12  vishho  STORYC-161: 100% C/DC for FvM implementation
 *  02.02.00  2017-07-13  vishho  STORYC-1700: Inform FV overflow also in case of Tx Messages
 *            2017-07-18  vishho  STORYC-1556: FvM_Jaspar - transmit messages with default Trip/Reset counter, before SyncMessage is received
 *  02.03.00  2017-08-09  vishho  STORYC-2127: Cleanup FvM Backlog
 *********************************************************************************************************************/

#if !defined (FVM_H)
# define FVM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "FvM_Cfg.h"
# include "FvM_Lcfg.h"
# include "SecOC_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define FVM_VENDOR_ID                           (30u)
# define FVM_MODULE_ID                           (255u)

/* AUTOSAR Software specification version information */
# define FVM_AR_RELEASE_MAJOR_VERSION            (4u)
# define FVM_AR_RELEASE_MINOR_VERSION            (3u)
# define FVM_AR_RELEASE_REVISION_VERSION         (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define FVM_SW_MAJOR_VERSION                    (2u)
# define FVM_SW_MINOR_VERSION                    (3u)
# define FVM_SW_PATCH_VERSION                    (0u)

# define FVM_INSTANCE_ID_DET                     (0x00u)

/* ----- API service IDs ----- */
# define FVM_SID_INIT                            (0x00u) /*!< Service ID: FvM_Init() */
# define FVM_SID_GLOBAL_EXAMPLE_FUNCTION         (0x01u) /*!< Service ID: FvM_GlobalExampleFunction() */
# define FVM_SID_GET_VERSION_INFO                (0x10u) /*!< Service ID: FvM_GetVersionInfo() */
# define FVM_SID_GET_RX_FRESHNESS                (0x11u) /*!< Service ID: SecOC_GetRxFreshness() */
# define FVM_SID_GET_TX_FRESHNESS                (0x12u) /*!< Service ID: SecOC_GetTxFreshnessTruncData() */
# define FVM_SID_TX_FRESHNESS_CONFIRMATION       (0x13u) /*!< Service ID: FvM_TxFreshnessConfirmation() */
# define FVM_SID_VERIFICATION_STATUS_CALLOUT     (0x14u) /*!< Service ID: FvM_VerificationStatusCallout() */
# define FVM_SID_DEINIT                          (0x15u) /*!< Service ID: FvM_DeInit() */


/* ----- Error codes ----- */
# define FVM_E_NO_ERROR                          (0x00u) /*!< used to check if no error occurred - use a value unequal to any error code */
# define FVM_E_PARAM_CONFIG                      (0x0Au) /*!< Error code: API service FvM_Init() called with wrong parameter */
# define FVM_E_PARAM                             (0x0Bu) /*!< Error code: API service called with wrong parameter */
# define FVM_E_PARAM_POINTER                     (0x0Cu) /*!< Error code: API service used with invalid pointer parameter (NULL) */
# define FVM_E_UNINIT                            (0x10u) /*!< Error code: API service used without module initialization */
# define FVM_E_ALREADY_INITIALIZED               (0x11u) /*!< Error code: The service FvM_Init() is called while the module is already initialized  */
# define FVM_E_INVALID_REQUEST                   (0x12u) /*!< Error code: Invalid request */

/* ----- Modes ----- */
# define FVM_UNINIT                              (0x00u)
# define FVM_INIT                                (0x01u)

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
 *****************************************************************************************78****************************/

# define FVM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  FvM_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FVM_CODE) FvM_InitMemory(void);

/**********************************************************************************************************************
  FvM_DeInit
**********************************************************************************************************************/
/*! \brief       DeInitialization function
 *  \details     This function sets the module state to uninitialized.
 *  \trace       CREQ-117899
 *  \pre         Interrupts are disabled.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
**********************************************************************************************************************/
FUNC(void, FVM_CODE) FvM_DeInit(void);

/**********************************************************************************************************************
 * FvM_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module FvM. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   ConfigPtr               Configuration structure for initializing the module
 *  \trace       CREQ-117898
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         FvM_InitMemory has been called unless FvM_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FVM_CODE) FvM_Init(P2CONST(FvM_ConfigType, AUTOMATIC, FVM_PBCFG) ConfigPtr);

# if (FVM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  FvM_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     FvM_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         none
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      FVM_VERSION_INFO_API
 *********************************************************************************************************************/
FUNC(void, FVM_CODE) FvM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FVM_APPL_VAR) versioninfo);
# endif

#if(FVM_RXFRESHNESSVALUEINFO == STD_ON) /* COV_FVM_VAR_SYNC_MSG_ALWAYS_ON */
/**********************************************************************************************************************
 *  FvM_VerificationStatusCallout()
 *********************************************************************************************************************/
/*! \brief       Called to indicate the verification status.
 *  \details     FvM_VerificationStatusCallout() is called by the SecOC to indicate if the verification was successful or failed.
 *  \param[in]   verificationStatus             status of the verification and freshness value id.
 *  \pre         none
 *  \context     TASK|ISR2
 *  \reentrant   TRUE, for different freshness value ids.
 *  \synchronous TRUE
 *  \config      FVM_RXFRESHNESSVALUEINFO
 */
FUNC(void, FVM_CODE) FvM_VerificationStatusCallout(SecOC_VerificationStatusType verificationStatus);
#endif

#if(FVM_TXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
 *  SecOC_SPduTxConfirmation()
 *********************************************************************************************************************/
/*! \brief       Called to indicate the successful initiated transmission.
 *  \details     SecOC_SPduTxConfirmation() is called by the SecOC to indicate that a transmission was successful initiated.
 *  \param[in]   SecOCFreshnessValueID           fershness value id.
 *  \pre         none
 *  \context     TASK|ISR2
 *  \reentrant   TRUE, for different freshness value ids.
 *  \synchronous TRUE
 *  \config      FVM_TXFRESHNESSVALUEINFO
 */
#endif

/***********************************************************************************************************************
 *  SecOC_GetTxFreshnessTruncData()
 **********************************************************************************************************************/
/*!
 * \brief          Getter for freshness values on transmission side.
 * \details        This interface is used by the SecOC to obtain the current freshness value.
 * \param[in]      SecOCFreshnessValueID      Holds the identifier of the freshness value.
 * \param[out]     SecOCFreshnessValue        Holds the current freshness value.
 * \param[in, out] SecOCFreshnessValueLength Holds the length of the provided freshness in bits.
 * \param[out]     SecOCTruncatedFreshnessValue Holds the truncated freshness to be included into the Secured I-PDU.
 * \param[in, out] SecOCTruncatedFreshnessValueLength  Provides the truncated freshness length configured for this freshness.
 * \return         Std_ReturnType
 *                 E_OK:     request successful
 *                 E_NOT_OK: request failed, a freshness value cannot be provided due to
 *                           general issues for freshness or this FreshnessValueId.
 *                 E_BUSY:   The freshness information can temporarily not be provided
 * \context        TASK
 * \reentrant      TRUE, for different freshness value ids.
 * \synchronous    TRUE
 * \config         FVM_TXFRESHNESSVALUEINFO
 */

/***********************************************************************************************************************
 *  SecOC_GetRxFreshness()
 **********************************************************************************************************************/
/*!
 * \brief      Getter for freshness values on reception side.
 * \details    This interface is used by the SecOC to obtain the current freshness value.
 * \param[in]  SecOCFreshnessValueID      Holds the identifier of the freshness value.
 * \param[in]  SecOCTruncatedFreshnessValue Holds the truncated freshness value that was contained in the Secured I-PDU.
 * \param[in]  SecOCTruncatedFreshnessValueLength  Holds the length in bits of the truncated freshness value.
 * \param[in]  SecOCAuthVerifyAttempts     Holds the number of authentication verify attempts of this PDU since the last reception.
 *                                         The value is 0 for the first attempt and incremented on every unsuccessful verification attempt.
 * \param[out]     SecOCFreshnessValue         Holds the number of authentication verify attempts of this PDU since the last reception.
 *                                             The value is 0 for the first attempt and incremented on every unsuccessful verification attempt.
 * \param[in, out] SecOCFreshnessValueLength Holds the length in bits of the freshness value.
 * \return         Std_ReturnType
 *                  E_OK:     request successful
 *                  E_NOT_OK: request failed, a freshness value cannot be provided due to
 *                            general issues for freshness or this FreshnessValueId.
 *                  E_BUSY:   The freshness information can temporarily not be provided
 * \context        TASK
 * \reentrant      TRUE, for different freshness value ids.
 * \synchronous    TRUE
 * \config         FVM_RXFRESHNESSVALUEINFO
 */

# define FVM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/*!
* \exclusivearea FVM_EXCLUSIVE_AREA_RX_FRESHNESS
* Ensures consistency of global RAM variables.
* \protects Global RAM variables used of Rx path.
* \usedin SecOC_GetRxFreshness, FvM_VerificationStatusCallout
* \exclude All functions provided by FvM.
* \length SHORT depending on the API and the call graph.
* \endexclusivearea
*/

/*!
* \exclusivearea FVM_EXCLUSIVE_AREA_TX_FRESHNESS
* Ensures consistency of global RAM variables.
* \protects Global RAM variables used of Tx path.
* \usedin SecOC_GetTxFreshnessTruncData, SecOC_SPduTxConfirmation
* \exclude All functions provided by FvM.
* \length SHORT depending on the API and the call graph.
* \endexclusivearea
*/


#endif /* FVM_H */

/**********************************************************************************************************************
 *  END OF FILE: FvM.h
 *********************************************************************************************************************/
