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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  EthFw.h
 *       Module:  SysService_FwEth
 *    Generator:  SysService_FwEth.jar (DaVinci Configurator Pro)
 *
 *  Description:  Implementation of Firewall for Ethernet (ETHFW)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Philipp Christmann            vispcn        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-11-04  vispcn  FEATC-889     FEAT-2354: Firewall concept for Ethernet
 *  01.00.01  2017-08-03  vispcn  ESCAN00096153 Compiler error: Undefined/undeclared identifier
 *********************************************************************************************************************/

#if !defined(ETHFW_H)
#define ETHFW_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "EthFw_Types.h"
#include "EthFw_Cfg.h"
#include "EthFw_Cbk.h"
#include "EthFw_Lcfg.h"
#include "EthFw_PBcfg.h"
#include "EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define ETHFW_VENDOR_ID                            (30u)
# define ETHFW_MODULE_ID                            (0xFFu)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define ETHFW_SW_MAJOR_VERSION                     (1u)
# define ETHFW_SW_MINOR_VERSION                     (0u)
# define ETHFW_SW_PATCH_VERSION                     (1u)

# define ETHFW_INSTANCE_ID_DET                      (0x00u)

/* ----- API service IDs ----- */
# define ETHFW_SID_INIT                             (0x10u) /*!< Service ID: EthFw_Init() */
# define ETHFW_SID_GET_VERSION_INFO                 (0x11u) /*!< Service ID: EthFw_GetVersionInfo() */
# define ETHFW_SID_IS_RX_FRAME_ALLOWED              (0x12u) /*!< Service ID: EthFw_IsFrameRxAllowed() */
# define ETHFW_SID_IS_TX_FRAME_ALLOWED              (0x13u) /*!< Service ID: EthFw_IsFrameTxAllowed() */

/* ----- Error codes ----- */
# define ETHFW_E_NO_ERROR                           (0x00u) /*!< used to check if no error occurred - use a value unequal to any error code. */
# define ETHFW_E_PARAM_CONFIG                       (0x01u) /*!< Error code: API service EthFw_Init() called with wrong parameter. */
# define ETHFW_E_PARAM_POINTER                      (0x02u) /*!< Error code: API service used with invalid pointer parameter (NULL). */
# define ETHFW_E_UNINIT                             (0x03u) /*!< Error code: API service used without module initialization. */
# define ETHFW_E_ALREADY_INITIALIZED                (0x04u) /*!< Error code: The service EthFw_Init() is called while the module is already initialized. */
# define ETHFW_E_INIT_FAILED                        (0x05u) /*!< Error code: The service EthFw_Init() failed. */

/* ----- Modes ----- */
# define ETHFW_UNINIT                               (uint8)0x00u
# define ETHFW_INIT                                 (uint8)0x01u

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define ETHFW_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if(ETHFW_USE_INIT_POINTER == STD_ON)
extern P2CONST(EthFw_PBConfigType, ETHFW_VAR_ZERO_INIT, ETHFW_PBCFG) EthFw_ConfigDataPtr;
#endif

#define ETHFW_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHFW_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthFw_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for EthFw-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in EthFw sections. Used in case they are not initialized by the startup code.
 *  \param       none
 *  \return      void
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-120296
 *********************************************************************************************************************/
FUNC(void, ETHFW_CODE) EthFw_InitMemory(void);

/**********************************************************************************************************************
 * EthFw_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module EthFw. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   ConfigPtr               Configuration structure for initializing the module.
 *  \return      void
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         EthFw_InitMemory has been called unless EthFw_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace       CREQ-120296
 *********************************************************************************************************************/
FUNC(void, ETHFW_CODE) EthFw_Init(
  P2CONST(EthFw_ConfigType, AUTOMATIC, ETHFW_APPL_CONST) ConfigPtr);

#if (ETHFW_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  EthFw_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     EthFw_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfoPtr          Pointer to where to store the version information. Parameter must not be NULL.
 *  \return      void
 *  \pre         none
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-120297
 *********************************************************************************************************************/
FUNC(void, ETHFW_CODE) EthFw_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHFW_APPL_DATA) VersionInfoPtr);
#endif  /* ETHFW_VERSION_INFO_API */

#define ETHFW_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* ETHFW_H */

/**********************************************************************************************************************
 *  END OF FILE: EthFw.h
 *********************************************************************************************************************/
