/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  vCanCcGbt.h
 *       Module:  vCanCcGbt
 *
 *  Description:  Main header of the vCanCcGbt module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Simon Gutjahr                 visgsi        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version  Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00  2017-06-09  visgsi  STORYC-1356    First Version.
 *  1.00.01  2017-07-17  visgsi  ESCAN00095910  Fixed wrong CHM message version number reported to application.
 *                               ESCAN00095912  Compile error undeclared identifier vCanCcGbt_CopyRxData,
 *                                              vCanCcGbt_StartOfReception, vCanCcGbt_TpRxIndication
 *  1.00.02  2017-08-11  visgsi  ESCAN00096211  Fixed Compiler warning: Duplicate definition of *TIMEOUT* symbols.
                                             -  Rework after review.
 *********************************************************************************************************************/
#if !defined (VCANCCGBT_H)
# define VCANCCGBT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "vCanCcGbt_Cfg.h"
# include "vCanCcGbt_Lcfg.h"
# include "vCanCcGbt_PBcfg.h"

# include "vCanCcGbt_Types.h"

# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Vendor and module identification */
# define VCANCCGBT_VENDOR_ID                      (30u)
# define VCANCCGBT_MODULE_ID                      (255u)

/* Vendor specific BSW module version information */
# define VCANCCGBT_SW_MAJOR_VERSION               (1u)
# define VCANCCGBT_SW_MINOR_VERSION               (0u)
# define VCANCCGBT_SW_PATCH_VERSION               (2u)

/*
 * Development Error Detection
 */
/* ----- API service IDs ----- */
/* Service ID: vCanCcGbt_Init() */
# define VCANCCGBT_SID_INIT                       (0x01u)
/* Service ID: vCanCcGbt_DeInit() */
# define VCANCCGBT_SID_DEINIT                     (0x02u)
/* Service ID: vCanCcGbt_GetVersionInfo() */
# define VCANCCGBT_SID_GETVERSIONINFO             (0x03u)
/* Service ID: vCanCcGbt_MainFunction() */
# define VCANCCGBT_SID_MAINFUNCTION               (0x04u)
/* Service ID: vCanCcGbt_TxConfirmation() */
# define VCANCCGBT_SID_TXCONFIRMATION             (0x40u)
/* Service ID: vCanCcGbt_TxConfirmation() */
# define VCANCCGBT_SID_TPTXCONFIRMATION           (0x41u)
/* Service ID: vCanCcGbt_RxIndication() */
# define VCANCCGBT_SID_RXINDICATION               (0x42u)
/* Proprietary service ID: vCanCcGbt_InitMemory() */
# define VCANCCGBT_SID_INITMEMORY                 (0x43u)
/* Proprietary service ID: vCanCcGbt_CopyTxData() */
# define VCANCCGBT_SID_COPYTXDATA                 (0x44u)
/* Proprietary service ID: vCanCcGbt_ChargerPlugged() */
# define VCANCCGBT_SID_CHARGERPLUGGED             (0x45u)
/* Proprietary service ID: vCanCcGbt_SuspendCharging() */
# define VCANCCGBT_SID_SUSPENDCHARGING            (0x46u)

/* -----Development Error codes ----- */
/* Proprietary development error code: No error occurred */
# define VCANCCGBT_E_NO_ERROR                     (0x00u)
/* Development error code: An API was called while the module was uninitialized */
# define VCANCCGBT_E_UNINIT                       (0x01u)
/* Development error code: The Init API was called twice */
# define VCANCCGBT_E_REINIT                       (0x02u)
/* Development error code: An API service was called with a NULL pointer */
# define VCANCCGBT_E_PARAM_POINTER                (0x03u)
/* Development error code: An API service was called with a wrong ID */
# define VCANCCGBT_E_INVALID_PDU_SDU_ID           (0x04u)
/* Development error code: vCanCcGbt_Init called with invalid init structure */
# define VCANCCGBT_E_INIT_FAILED                  (0x05u)

/* -----Runtime Error codes ----- */
/* Proprietary runtime error code: Timeout of transmission confirmation callback */
# define VCANCCGBT_E_TIMEOUT_TXCONF               (0x81u)
/* Proprietary runtime error code: Untimely CHM message */
# define VCANCCGBT_E_CHM_UNTIMELY                 (0x82u)
/* Proprietary runtime error code: Untimely CRM message */
# define VCANCCGBT_E_CRM_UNTIMELY                 (0x83u)
/* Proprietary runtime error code: Untimely CTS message */
# define VCANCCGBT_E_CTS_UNTIMELY                 (0x84u)
/* Proprietary runtime error code: Untimely CML message */
# define VCANCCGBT_E_CML_UNTIMELY                 (0x85u)
/* Proprietary runtime error code: Untimely CRO message */
# define VCANCCGBT_E_CRO_UNTIMELY                 (0x86u)
/* Proprietary runtime error code: Untimely CCS message */
# define VCANCCGBT_E_CCS_UNTIMELY                 (0x87u)
/* Proprietary runtime error code: Untimely CST message */
# define VCANCCGBT_E_CST_UNTIMELY                 (0x88u)
/* Proprietary runtime error code: Untimely CSD message */
# define VCANCCGBT_E_CSD_UNTIMELY                 (0x89u)
/* Proprietary runtime error code: Untimely CEM message */
# define VCANCCGBT_E_CEM_UNTIMELY                 (0x90u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*================================================ vCanCcGbt_START_SEC_CODE ===========================================*/
# define VCANCCGBT_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vCanCcGbt_InitMemory
 *********************************************************************************************************************/
/*! \brief       Sets the global vCanCcGbt state to uninitialized.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function should be used if the vCanCcGbt is not initialized by startup code.
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InitMemory(void);

/**********************************************************************************************************************
 *  vCanCcGbt_Init
 *********************************************************************************************************************/
/*! \brief       Initializes the vCanCcGbt.
 *  \details     -
 *  \param[in]   config          Pointer to configuration data structure.
 *  \pre         The module must be in the uninitialized state.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The config parameter is only required if the configuration is variant or changed at post-build time.
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_Init(P2CONST(vCanCcGbt_ConfigType, AUTOMATIC, VCANCCGBT_APPL_DATA) config);

/**********************************************************************************************************************
 *  vCanCcGbt_DeInit()
 *********************************************************************************************************************/
/*! \brief       Resets the vCanCcGbt module to the uninitialized state.
 *  \details     The module is not truly shut down before all services and callback functions have terminated.
 *  \pre         The module must be in the initialized state.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_DeInit(void);

# if (VCANCCGBT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  vCanCcGbt_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief       Returns the version information of the vCanCcGbt.
 *  \details     -
 *  \param[out]  VersionInfo     Pointer to the location where the version information shall be stored.
 *  \pre         The VersionInfo parameter must not be NULL.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void,
     VCANCCGBT_CODE) vCanCcGbt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) versionInfo);
# endif

/***********************************************************************************************************************
 *  vCanCcGbt_ChargerPlugged()
 *********************************************************************************************************************/
/*! \brief        Function to set the state of the physical connection to the Charger.
 *  \details      -
 *  \param[in]    isPlugged
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_ChargerPlugged(boolean isPlugged);

/***********************************************************************************************************************
 *  vCanCcGbt_SuspendCharging()
 *********************************************************************************************************************/
/*! \brief        Function to indicate the vCanCcGbt that the charging is finished.
 *  \details      -
 *  \param[in]    isSuspended
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_SuspendCharging(boolean isSuspended);

# define VCANCCGBT_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ vCanCcGbt_STOP_SEC_CODE ============================================*/

#endif /* VCANCCGBT_H */
