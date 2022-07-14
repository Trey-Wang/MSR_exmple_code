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
 *         File:  Rtp.h
 *       Module:  Tp_Rtp
 *    Generator:  Tp_Rtp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Implementation of Realtime Transport Protocol (RTP)
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
 *  01.00.00  2016-08-10  vispcn  FEATC-253     FEAT-1879:  [Beta] Development of RTP
 *  01.00.01  2017-01-09  vispcn  ESCAN00093044 Compiler warning: Cast truncates constant value
 *  01.01.00  2017-01-09  vispcn  FEATC-942     FEAT-1824: Support a component-specific SafeBSW configuration switch
 *********************************************************************************************************************/

#if !defined(RTP_H)
#define RTP_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Rtp_Types.h"
#include "Rtp_Cfg.h"
#include "Rtp_Cbk.h"
#include "Rtp_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define RTP_VENDOR_ID                            (30u)
# define RTP_MODULE_ID                            (0xFFu)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define RTP_SW_MAJOR_VERSION                     (1u)
# define RTP_SW_MINOR_VERSION                     (1u)
# define RTP_SW_PATCH_VERSION                     (0u)

# define RTP_INSTANCE_ID_DET                      (0x00u)

/* ----- API service IDs ----- */
# define RTP_SID_INIT                             (0x10u) /*!< Service ID: Rtp_Init() */
# define RTP_SID_GET_VERSION_INFO                 (0x11u) /*!< Service ID: Rtp_GetVersionInfo() */
# define RTP_SID_MAIN_FUNCTION                    (0x12u) /*!< Service ID: Rtp_MainFunction() */
# define RTP_SID_TRANSMIT_FUNCTION                (0x13u) /*!< Service ID: Rtp_Transmit() */
# define RTP_SID_SET_CONTROL_INFO_FUNCTION        (0x14u) /*!< Service ID: Rtp_SetControlInformation() */

# define RTP_SID_RX_INDICATION                    (0x20u) /*!< Service ID: Rtp_SoAdIfRxIndication() */
# define RTP_SID_TRIGGER_TRANSMIT_FUNCTION        (0x21u) /*!< Service ID: Rtp_SoAdTriggerTransmit() */

/* ----- Error codes ----- */
# define RTP_E_NO_ERROR                           (0x00u) /*!< used to check if no error occurred - use a value unequal to any error code. */
# define RTP_E_PARAM_CONFIG                       (0x0Au) /*!< Error code: API service Rtp_Init() called with wrong parameter. */
# define RTP_E_PARAM_VALUE                        (0x0Bu) /*!< Error code: API called with invalid parameter value. */
# define RTP_E_PARAM_POINTER                      (0x0Cu) /*!< Error code: API service used with invalid pointer parameter (NULL). */
# define RTP_E_API_DISABLED                       (0x0Du) /*!< Error code: API not available. Configuration missmatch. */
# define RTP_E_UNINIT                             (0x10u) /*!< Error code: API service used without module initialization. */
# define RTP_E_ALREADY_INITIALIZED                (0x11u) /*!< Error code: The service Rtp_Init() is called while the module is already initialized. */

/* ----- Modes ----- */
# define RTP_UNINIT                               (uint8)0x00u
# define RTP_INIT                                 (uint8)0x01u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define RTP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Rtp_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for Rtp-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in Rtp sections. Used in case they are not initialized by the startup code.
 *  \param       none
 *  \return      void
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, RTP_CODE) Rtp_InitMemory(void);

/**********************************************************************************************************************
 * Rtp_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module Rtp. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   ConfigPtr               Configuration structure for initializing the module.
 *  \return      void
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         Rtp_InitMemory has been called unless Rtp_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace CREQ-116485
 *********************************************************************************************************************/
FUNC(void, RTP_CODE) Rtp_Init(
  P2CONST(Rtp_ConfigType, AUTOMATIC, RTP_APPL_CONST) ConfigPtr);

#if (RTP_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Rtp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Rtp_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfoPtr          Pointer to where to store the version information. Parameter must not be NULL.
 *  \return      void
 *  \pre         none
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-116486
 *********************************************************************************************************************/
FUNC(void, RTP_CODE) Rtp_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, RTP_APPL_DATA) VersionInfoPtr);
#endif  /* RTP_VERSION_INFO_API */

/**********************************************************************************************************************
 *  Rtp_Transmit()
 *********************************************************************************************************************/
/*! \brief       Triggers transmission of a RTP frame.
 *  \details     The relevant payload can be passed by the API via the PayloadPtr or a buffer of the required size
 *               can be requested if PayloadPtr == NULL_PTR.
 *  \param[in]   StreamHandleId   Handle ID of the relevant stream.
 *  \param[in]   InfoPtr          Structure containing relevant header and packet information.
 *  \param[in]   PayloadPtr       NULL_PTR    Data will be requested by additional [UL]_CopyTxData() function call.
                                  !NULL_PTR   Pointer to data which shall be transmitted.
 *  \param[in]   Length           Length of the data to be transmitted.
 *  \return      E_OK             Transmission was successful.
 *               E_NOT_OK         Transmission failed.
 *  \pre         Module is initialized, corresponding stream is configured with Rtp_SetControlInformation() and
 *               activated.
 *  \context     TASK
 *  \reentrant   TRUE for different streams
 *  \synchronous TRUE
 *  \trace CREQ-116487
 *********************************************************************************************************************/
FUNC(Std_ReturnType, RTP_CODE) Rtp_Transmit(
  uint16 StreamHandleId,
  P2CONST(Rtp_TxRtpInfoType, AUTOMATIC, RTP_APPL_DATA) InfoPtr,
  P2CONST(uint8, AUTOMATIC, RTP_APPL_DATA) PayloadPtr,
  uint16 Length);

/**********************************************************************************************************************
 *  Rtp_SetControlInformation()
 *********************************************************************************************************************/
/*! \brief       Updates the RTCP control information.
 *  \details     Stream has to be configured and enabled in order to start periodic transmission of RTCP frames.
 *               The timestamp values will be updated with the values passed at Rtp_Transmit() call.
 *  \param[in]   StreamHandleId           Handle ID of the relevant stream.
 *  \param[in]   StreamEnabled            Enables of disables transmission of RTCP control frames.
 *  \param[in]   AsTimestampPtr           Updates AsTimestamp value if Pointer != NULL_PTR
 *  \param[in]   RtpTimestampPtr          Updates RtpTimestamp value if Pointer != NULL_PTR
 *  \param[in]   GmTimeBaseIndicatorPtr   Updates GmTimeBaseIndicator value if Pointer != NULL_PTR
 *  \param[in]   GmIdentityPtr            Updates GmIdentity value if Pointer != NULL_PTR
 *  \return      E_OK                     Control information successfully set.
 *               E_NOT_OK                 Failed to set control information.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different streams
 *  \synchronous TRUE
 *  \trace CREQ-116490, CREQ-116494, CREQ-116495
 *********************************************************************************************************************/
FUNC(Std_ReturnType, RTP_CODE) Rtp_SetControlInformation(
  uint16 StreamHandleId,
  boolean StreamEnabled,
  P2CONST(uint32, AUTOMATIC, RTP_APPL_DATA) AsTimestampPtr,
  P2CONST(uint32, AUTOMATIC, RTP_APPL_DATA) RtpTimestampPtr,
  P2CONST(uint16, AUTOMATIC, RTP_APPL_DATA) GmTimeBaseIndicatorPtr,
  P2CONST(Rtp_GmIdentityType, AUTOMATIC, RTP_APPL_DATA) GmIdentityPtr);

#ifdef RTP_DECLARE_OWN_MAINFUNCTION
/**********************************************************************************************************************
 *  Rtp_MainFunction()
 *********************************************************************************************************************/
/*! \brief       MainFunction of RTP module.
 *  \details     Handles internal states and timers. This function has to be called cyclically.
 *  \param       none
 *  \return      void
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace CREQ-116489
 *********************************************************************************************************************/
FUNC(void, RTP_CODE) Rtp_MainFunction(void);
#endif

#define RTP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* RTP_H */

/**********************************************************************************************************************
 *  END OF FILE: Rtp.h
 *********************************************************************************************************************/
