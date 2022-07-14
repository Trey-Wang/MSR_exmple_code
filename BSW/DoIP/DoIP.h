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
/**        \file  DoIP.h
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Header file of Diagnostic over IP
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Michael Dangelmaier           vismda        Vector Informatik GmbH
 *  Alex Lunkenheimer             visalr        Vector Informatik GmbH
 *  Philipp Christmann            vispcn        Vector Informatik GmbH
 *  Jens Bauer                    visjsb        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *   1.00.00  2013-10-18  vismda  -             Moved from SoAd
 *   1.00.01  2013-11-07  vismda  -             Issues in PduR_DoIP
 *            2013-11-18  vismda  ESCAN00071981 Diagnostic Message Ack/Nack with invalid length
 *            2013-11-18  vismda  ESCAN00071841 Issue: DoIP PduR Api and If
 *   1.01.00  2014-02-07  vismda  ESCAN00072620 DoIP module shall perform version check against DoIP version
 *            2014-02-07  vismda  ESCAN00072387 Compiler warning: inconsistent storage class specification for
 *                                              DoIP_VReSetVehicleAnnounce
 *            2014-02-07  vismda  ESCAN00072329 Issue when sending generic header nack and alive check request
 *            2014-02-07  vismda  ESCAN00072323 DET in DoIP_SoAdTpCopyRxData
 *   1.01.01  2014-04-30  vismda  ESCAN00075331 Endless loop at TcpIp buffer wrap around
 *   1.02.00  2014-05-08  vismda  ESCAN00074362 DoIP does not send expected Generic Header Negative Acknowledge
 *            2014-05-09  vismda  ESCAN00074459 AR4-650: Support ChangeParameter without Confirmation
 *            2014-05-09  vismda  ESCAN00071915 AR4-619: Support the StartOfReception API (with the PduInfoType),
 *                                              TxConfirmation and RxIndication according ASR4.1.2
 *            2014-05-14  vismda  ESCAN00074082 AR4-582: DoIP functionality to mirror request data in response data
 *            2014-06-13  vismda  ESCAN00076206 Local define for STATIC
 *   1.02.01  2014-06-20  visalr  ESCAN00076350 DoIP_Init: Signatures inconsistent in DoIP.c and DoIP.h
 *   1.02.02  2014-07-02  vismda  ESCAN00076643 No generic header negative acknowledge with error code 0x02
 *            2014-07-02  vismda  ESCAN00076645 Default Protocol Version 0xFF is not accepted
 *            2014-07-03  vismda  ESCAN00076712 No response to segmented TCP message
 *   1.03.00  2014-08-21  vismda  ESCAN00077970 Add DoIPGw support
 *   1.04.00  2014-07-22  vismda  ESCAN00071768 The EID in Vehicle Announcement messages has the invalid value
 *            2014-07-22  vismda  ESCAN00073293 Feature: Implement UDP zero copy for transmission
 *            2014-07-22  vismda  ESCAN00076651 Vehicle Identification Response sent before A_DoIP_Announce_Wait
 *            2014-09-18  vismda  ESCAN00078453 Wrong routing activation response SA when all sockets are registered
 *                                              and active
 *            2014-09-19  vismda  ESCAN00078472 Same diagnostic message negative acknowledgement is sent
 *            2014-09-25  vismda  ESCAN00077694 Compiler warning: Several warnings when the component is compiled with
 *                                              Visual Studio
 *            2014-09-25  vismda  ESCAN00073340 Extract DoIP from SoAd
 *            2014-09-25  vismda  ESCAN00078431 No alive check requests are sent
 *            2014-09-25  vismda  ESCAN00078446 No routing activation response code 0x03 is sent
 *            2014-09-26  vismda  ESCAN00078601 T_TCP_General_Inactivity timeout is not reset when ECU sends messages
 *            2014-09-30  vismda  ESCAN00078645 Wrong DET check in DoIP_SoAdTpCopyTxData
 *            2014-10-06  vismda  ESCAN00078770 Unexpected behavior if PduR_DoIPCopyRxData() returns BUFREQ_E_NOT_OK
 *            2014-10-06  vismda  ESCAN00078777 DoIP_CancelReceive() does not DET-check DoIPPduRRxId properly
 *            2014-10-16  vismda  ESCAN00078921 No routing activation response code 0x02
 *            2014-10-16  vismda  ESCAN00078975 EnableTxBeforeRx and Shutdown issues
 *            2014-10-16  vismda  ESCAN00078974 ECU shall discard DoIP payload types on wrong transport layer
 *            2014-10-24  vismda  ESCAN00078217 DoIP causes alignment exception on sending Vehicle Announcement
 *   1.05.00  2015-03-12  vismda  ESCAN00081759 Adjustments to support ASR 4.2.1 SoAd
 *   1.05.01  2015-04-28  vismda  ESCAN00082732 Parameter bufferSizePtr in PduR_SoAdTpCopyRxData not checked
 *   1.06.00  2015-07-10  vismda  ESCAN00079233 Incorrect behavior when ECU receives an empty diagnostic message
 *            2015-07-15  vismda  ESCAN00079880 Compiler error: DoIP_VCfgEId is undefined
 *            2015-07-15  vismda  ESCAN00079168 Call DoIP_SoAdTpCopyRxData() with invalid SduLength
 *            2015-07-16  vismda  ESCAN00079049 No communication after transmission cancellation
 *            2015-07-16  vismda  ESCAN00084033 Alignment exception on sending initial Vehicle Announcements
 *   1.06.01  2015-08-17  vismda  ESCAN00084562 MISRA/PClint warnings
 *   1.06.02  2015-11-05  vismda  ESCAN00086288 CoreException "misaligned access exception (MAE)" occurs
 *   2.00.00  2015-11-05  vismda  ESCAN00084454 Socket not closed after reception of Alive Check Response with invalid
 *                                              logical address
 *            2015-11-20  vismda  ESCAN00085184 FEAT-10: Implementation of DoIP according to AR 4.2.1 [AR4-507]
 *   2.00.01  2016-02-04  vismda  ESCAN00088064 No Activation Line switch to active in case DHCP address only
 *   2.01.00  2016-02-16  vismda  ESCAN00088317 Vehicle Identification returned unexpected Further Action value
 *            2016-02-17  vismda  ESCAN00088344 Unexpected power mode if <User>_DoIPGetPowerModeStatus fails
 *            2016-02-17  vismda  ESCAN00088351 TCP socket closed on routing activation authentication callback return
 *                                              value switch from DOIP_E_PENDING to E_OK
 *            2016-02-18  vismda  ESCAN00088393 Implement inactivity timer as specified in AUTOSAR
 *            2016-02-18  vismda  ESCAN00088393 No response to Routing Activation Request if confirmation is used
 *            2016-02-24  vismda  ESCAN00087700 FEAT-1698: Support UUDT on DoIP according to AR 4.2.2
 *            2016-02-25  vismda  ESCAN00087683 FEAT-1449: Release of FEAT-1253 (ETH Stack)
 *            2016-03-21  vismda  ESCAN00089003 Out-of-bounds access around DoIP_VSoAdRxPduId2SoConClosing[]
 *            2016-03-22  vismda  ESCAN00087995 Activation Line cannot be set to inactive
 *   2.01.01  2016-04-12  vismda  ESCAN00089407 Unexpected DET in DoIP_SoAdTpCopyTxData()
 *   2.02.00  2016-05-19  vismda  ESCAN00089429 Compiler warning: 'DoIP_VHandleGidSync' declared 'static' but never
 *                                              defined
 *            2016-05-19  vismda  ESCAN00089348 FEAT-1820: DoIP: Improved Vehicle Announcement Handling
 *            2016-06-16  vismda  ESCAN00090504 Vehicle Announcement with unexpected GID caused by misinterpretation of
 *                                              <User>_DoIPGetGidCallback return value
 *   3.00.00  2016-08-25  vismda  ESCAN00091628 DoIP discards first TCP DoIP message
 *            2016-09-05  vismda  FEATC-259     FEAT-1706: DoIP: Support call-outs for OEM-specific DoIP payload types
 *            2016-09-12  vismda  ESCAN00091842 No response to Routing Activation Request
 *            2016-10-14  vismda  ESCAN00092319 Missing support of required IP address assignment methods
 *            2016-10-20  vismda  FEATC-261     FEAT-1666: Adapted DoIP channel selection (DoIP address masking)
 *            2016-10-25  vismda  FEATC-256     FEAT-1816: Split MainFunctions to optimize gateway use-case
 *   3.00.01  2016-12-02  vismda  ESCAN00093151 Vehicle Identification Request with EID and invalid length is responded
 *                                              with unexpected error code
 *   3.01.00  2017-01-09  vispcn  FEATC-940     FEAT-1824: Support a component-specific SafeBSW configuration switch
 *            2017-01-25  vismda  ESCAN00093714 Compiler error around DoIP_VRequestIpAddrAssignment()
 *            2017-01-29  vismda  ESCAN00093879 FEAT-2750: Support multiple testers on different VLANs
 *            2017-02-08  vismda  FEATC-1143    FEAT-2778: Support Buffer Size up to 128kB for FBL use case
 *   4.00.00  2017-04-11  vismda  ESCAN00094727 Timeout for consecutive vehicle announcements is equal to initial
 *                                              timeout
 *            2017-05-08  vismda  STORYC-386    P3 CAD
 *   4.01.00  2017-05-22  vismda  STORYC-751    Support more than 255 DoIP Target Addresses
 *   4.02.00  2017-07-06  visjsb  STORYC-1203   DoIP: Callout for Diagnostic Firewall Use Case
 *   4.03.00  2017-07-13  visjsb  STORYC-387    P3 Implementation API Pattern
 *            2017-07-20  visjsb  ESCAN00095972 DoIP array index out of bounds
 *            2017-07-20  visjsb  STORYC-1476   P3 Cleanup Implementation
 *   4.04.00  2017-08-21  visjsb  STORYC-1795   P3 Code Refactoring / CDD Step 2
 *********************************************************************************************************************/

#if ( !defined(DOIP_H) )
# define DOIP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "ComStack_Types.h"
# include "DoIP_Lcfg.h"
# include "DoIP_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define TP_ASRTPDOIP_VERSION                      (0x0403u) /* BCD coded version number */
# define TP_ASRTPDOIP_RELEASE_VERSION              (0x00u)   /* BCD coded release version number */

/* DoIP ModuleId */
# define DOIP_VENDOR_ID                            30u  /* Vector ID */
# define DOIP_MODULE_ID                            173u /* DoIP ID */

/* DoIP supported Autosar version */
# define DOIP_AR_RELEASE_MAJOR_VERSION             4u
# define DOIP_AR_RELEASE_MINOR_VERSION             2u
# define DOIP_AR_RELEASE_PATCH_VERSION             2u

/* DoIP software version */
# define DOIP_SW_MAJOR_VERSION                     4u
# define DOIP_SW_MINOR_VERSION                     4u
# define DOIP_SW_PATCH_VERSION                     0u

/* DoIP AUTOSAR ServiceIds */
# define DOIP_SID_GET_VERSION_INFO                 0x00u
# define DOIP_SID_INIT                             0x01u
# define DOIP_SID_MAIN_FUNCTION                    0x02u
# define DOIP_SID_TP_TRANSMIT                      0x03u
# define DOIP_SID_TP_CANCEL_TRANSMIT               0x04u
# define DOIP_SID_TP_CANCEL_RECEIVE                0x05u
# define DOIP_SID_SO_CON_MODE_CHG                  0x0Bu
# define DOIP_SID_LOC_IP_ADDR_ASSIGN_CHG           0x0Cu
# define DOIP_SID_ACTIVATION_LINE_SWITCH           0x0Fu
# define DOIP_SID_IF_TX_CONFIRMATION               0x40u
# define DOIP_SID_IF_RX_INDICATION                 0x42u
# define DOIP_SID_TP_COPY_TX_DATA                  0x43u
# define DOIP_SID_TP_COPY_RX_DATA                  0x44u
# define DOIP_SID_TP_RX_INDICATION                 0x45u
# define DOIP_SID_TP_START_OF_RECEPTION            0x46u
# define DOIP_SID_TP_TX_CONFIRMATION               0x48u
# define DOIP_SID_IF_TRANSMIT                      0x49u
# define DOIP_SID_IF_CANCEL_TRANSMIT               0x4Au

/* DoIP Vector ServiceIds */
# define DOIP_SID_VACTIVATION_LINE_TO_ACTIVE       0xEBu
# define DOIP_SID_ENABLE_PDU_SIZE_ROUTING          0xECu
# define DOIP_SID_DISABLE_PDU_SIZE_ROUTING         0xEDu
# define DOIP_SID_VUDP_SINGLE_TRANSMIT             0xEEu
# define DOIP_SID_VTCP_TRANSMIT                    0xEFu

/* DoIP AUTOSAR DET errors */
# define DOIP_E_UNINIT                             0x01u
# define DOIP_E_PARAM_POINTER                      0x02u
# define DOIP_E_INVALID_PDU_SDU_ID                 0x03u
# define DOIP_E_INVALID_PARAMETER                  0x04u
# define DOIP_E_INIT_FAILED                        0x05u

/* DoIP Vector DET errors */
# define DOIP_E_NO_ERROR                           0x00u /* use a value unequal to any error code */
# define DOIP_E_ACTIVATION_LINE                    0xECu
# define DOIP_E_SOAD_CALL_FAILED                   0xEDu
# define DOIP_E_UNEXPECTED_ASSIGNMENT              0xEEu
# define DOIP_E_NOBUFS                             0xEFu

/* DoIP states */
# define DOIP_STATE_UNINIT                         0x00u
# define DOIP_STATE_INIT                           0x01u
# define DOIP_STATE_SHUTDOWN                       0x02u

/* DoIP header types */
# define DOIP_HDR_TYPE_GEN_NACK                    (uint16)0x0000u
# define DOIP_HDR_TYPE_VEHICLE_IDENT_REQ           (uint16)0x0001u
# define DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_EID       (uint16)0x0002u
# define DOIP_HDR_TYPE_VEHICLE_IDENT_REQ_VIN       (uint16)0x0003u
# define DOIP_HDR_TYPE_VEHICLE_ANNOUNCEMENT        (uint16)0x0004u
# define DOIP_HDR_TYPE_ROUT_ACTIV_REQ              (uint16)0x0005u
# define DOIP_HDR_TYPE_ROUT_ACTIV_RES              (uint16)0x0006u
# define DOIP_HDR_TYPE_ALIVE_CHECK_REQ             (uint16)0x0007u
# define DOIP_HDR_TYPE_ALIVE_CHECK_RES             (uint16)0x0008u
# define DOIP_HDR_TYPE_ENTITY_STATE_REQ            (uint16)0x4001u
# define DOIP_HDR_TYPE_ENTITY_STATE_RES            (uint16)0x4002u
# define DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_REQ    (uint16)0x4003u
# define DOIP_HDR_TYPE_DIAG_POWER_MODE_INFO_RES    (uint16)0x4004u
# define DOIP_HDR_TYPE_DIAG_REQ                    (uint16)0x8001u
# define DOIP_HDR_TYPE_DIAG_ACK                    (uint16)0x8002u
# define DOIP_HDR_TYPE_DIAG_NACK                   (uint16)0x8003u
# define DOIP_HDR_TYPE_OEM_SPECIFIC                (uint16)0xEEEEu
# define DOIP_HDR_TYPE_INVALID                     (uint16)0xEFFFu

/* DoIP protocol versions */
# define DOIP_PROTOCOL_VERSION_2010                (uint8)0x01u
# define DOIP_PROTOCOL_VERSION_2011                (uint8)0x01u
# define DOIP_PROTOCOL_VERSION_2012                (uint8)0x02u
# define DOIP_PROTOCOL_VERSION_DEFAULT             (uint8)0xFFu

/* DoIP generic nack codes */
# define DOIP_GEN_NACK_FORMAT                      (uint8)0x00u
# define DOIP_GEN_NACK_TYPE                        (uint8)0x01u
# define DOIP_GEN_NACK_MAX_LEN                     (uint8)0x02u
# define DOIP_GEN_NACK_OUT_OF_MEM                  (uint8)0x03u
# define DOIP_GEN_NACK_INV_LEN                     (uint8)0x04u
# define DOIP_GEN_NACK_OK                          (uint8)0xFFu
# define DOIP_GEN_NACK_LEN_BYTE                    (uint8)0x01u

/* DoIP routing activation nack codes */
# define DOIP_ROUT_ACTIV_RES_CODE_UNKNOWN_TESTER   (uint8)0x00u
# define DOIP_ROUT_ACTIV_RES_CODE_SOCKETS_IN_USE   (uint8)0x01u
# define DOIP_ROUT_ACTIV_RES_CODE_DIFFERENT_TESTER (uint8)0x02u
# define DOIP_ROUT_ACTIV_RES_CODE_ALREADY_ACTIV    (uint8)0x03u
# define DOIP_ROUT_ACTIV_RES_CODE_MISSING_AUTHENT  (uint8)0x04u
# define DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REJECTED (uint8)0x05u
# define DOIP_ROUT_ACTIV_RES_CODE_INV_ACTIV_TYPE   (uint8)0x06u
# define DOIP_ROUT_ACTIV_RES_CODE_SUCCESS          (uint8)0x10u
# define DOIP_ROUT_ACTIV_RES_CODE_CONFIRM_REQUIRED (uint8)0x11u

/* DoIP diagnostic request nack codes */
# define DOIP_DIAG_MSG_RES_CODE_ACK                (uint8)0x00u
# define DOIP_DIAG_MSG_RES_CODE_UNKNOWN_SRC_ADDR   (uint8)0x01u /* described in ISO/CD 13400 - not used */
# define DOIP_DIAG_MSG_RES_CODE_INVALID_SRC_ADDR   (uint8)0x02u
# define DOIP_DIAG_MSG_RES_CODE_UNKNOWN_TGT_ADDR   (uint8)0x03u
# define DOIP_DIAG_MSG_RES_CODE_MSG_TOO_LARGE      (uint8)0x04u
# define DOIP_DIAG_MSG_RES_CODE_OUT_OF_MEMORY      (uint8)0x05u
# define DOIP_DIAG_MSG_RES_CODE_TGT_UNREACHABLE    (uint8)0x06u
# define DOIP_DIAG_MSG_RES_CODE_UNKNOWN_NETWORK    (uint8)0x07u
# define DOIP_DIAG_MSG_RES_CODE_NETWORK_TRANSPROT  (uint8)0x08u

/* DoIP diagnostic power mode ack codes */
# define DOIP_DIAG_POWER_MODE_RES_CODE_NOT_READY   (uint8)0x00u
# define DOIP_DIAG_POWER_MODE_RES_CODE_READY       (uint8)0x01u

/* DHCP-Options DoIP-Tester */
# define DOIP_DHCP_OPT_TABLE_SIZE                  (uint8)0x04u
# define DOIP_DHCP_OPT_SUB_LEN                     (uint8)0x05u /* enterNum + subOptionLen */
# define DOIP_DHCP_OPT_CODE                        (uint8)0x01u
# define DOIP_DHCP_OPT_LEN                         (uint8)0x05u
# define DOIP_DHCP_OPT_IPV4_ADDR_SIZE              (uint8)0x04u
# define DOIP_DHCP_OPT_IPV6_ADDR_SIZE              (uint8)0x10u
# define DOIP_DHCP_OPT_125_MIN_LEN                 (uint8)0x0Bu /* enterNum + subOptionLen + hdr + addrSize */
# define DOIP_DHCP_OPT_17_MIN_LEN                  (uint8)0x17u /* enterNum + subOptionLen + hdr + addrSize */
# define DOIP_DHCP_OPT_ENTERPRISE_NUM              (uint32)0x00000C7Bu

/* Announcement reset types */
# define DOIP_ANNOUNCE_SET                         (uint8)0x00u
# define DOIP_ANNOUNCE_RESET                       (uint8)0x01u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define DOIP_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  DoIP_InitMemory()
 *********************************************************************************************************************/
/*! \brief          Power-up memory initialization
 *  \details        Initializes component variables in *_INIT_* sections at power up.
 *  \pre            Module is uninitialized.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Use this function in case these variables are not initialized by the startup code.
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_InitMemory(void);

/**********************************************************************************************************************
 *  DoIP_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes component
 *  \details        Initializes all component variables and sets the component state to initialized. This service
 *                  initializes all global variables of the DoIP module. After return of this service the DoIP module
 *                  is operational.
 *  \param[in]      DoIPConfigPtr   Pointer to the configuration data of the DoIP module.
 *  \pre            Interrupts are disabled.
 *  \pre            Module is uninitialized.
 *  \pre            DoIP_InitMemory has been called unless DoIP_State is initialized by start-up code.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_Init(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_CONST) DoIPConfigPtr);

# if ( DOIP_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  DoIP_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief          Returns the version information
 *  \details        Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]     versioninfo   Pointer to where to store the version information of this module. Parameter must not
 *                                be NULL.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_VERSION_INFO_API
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_APPL_VAR) versioninfo);
# endif /* DOIP_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  DoIP_TpTransmit()
 *********************************************************************************************************************/
/*! \brief          Requests transmission of a specific TP PDU
 *  \details        This service is called to request the transfer data from the PduRouter to the SoAd. It is used to
 *                  indicate the transmission which will be performed by SoAd or in the DoIP_Mainfunction. Within the
 *                  provided DoIPPduRTxInfoPtr only SduLength is valid (no data)! If this function returns E_OK then
 *                  the SoAd module will raise a subsequent call to PduR_DoIPCopyTxData via DoIP_SoAdTpCopyRxData in
 *                  order to get the data to send.
 *  \param[in]      DoIPPduRTxId        DoIP unique identifier of the PDU to be transmitted by the PduR.
 *  \param[in]      DoIPPduRTxInfoPtr   Tx Pdu information structure which contains the length of the DoIPTxMessage.
 *  \return         E_OK                The request has been accepted.
 *  \return         E_NOT_OK            The request has not been accepted, e.g. parameter check has failed or no
 *                                      resources are available for transmission.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different DoIPPduRTxIds. Non reentrant for the same DoIPPduRTxId.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpTransmit(
  PduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) DoIPPduRTxInfoPtr);

/**********************************************************************************************************************
 *  DoIP_TpCancelTransmit()
 *********************************************************************************************************************/
/*! \brief          Requests transmission cancellation of a specific TP PDU
 *  \details        This service primitive is used to cancel the transfer of pending DoIPPduRTxIds. The connection is
 *                  identified by DoIPPduRTxId. If the function returns the cancellation is requested but not yet
 *                  performed.
 *  \param[in]      DoIPPduRTxId  DoIP unique identifier of the PDU to be canceled by the PduR.
 *  \return         E_OK          Transmit cancellation request of the specified DoIPPduRTxId is accepted.
 *  \return         E_NOT_OK      The transmit cancellation request of the DoIPPduRTxId has been rejected.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different DoIPPduRTxIds. Non reentrant for the same DoIPPduRTxIds.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelTransmit(
  PduIdType DoIPPduRTxId);

/**********************************************************************************************************************
 *  DoIP_TpCancelReceive()
 *********************************************************************************************************************/
/*! \brief          Requests reception cancellation of a specific TP PDU
 *  \details        By calling this API with the corresponding DoIPPduRRxId the currently ongoing data reception is
 *                  terminated immediately. If the function returns the cancellation is requested but not yet
 *                  performed.
 *  \param[in]      DoIPPduRRxId  DoIP unique identifier of the PDU for which reception shall be canceled by the PduR.
 *  \return         E_OK          Reception was canceled successfully.
 *  \return         E_NOT_OK      Reception was not canceled.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different DoIPPduRTxIds. Non reentrant for the same DoIPPduRTxId.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelReceive(
  PduIdType DoIPPduRRxId);

/**********************************************************************************************************************
 *  DoIP_IfTransmit()
 *********************************************************************************************************************/
/*! \brief          Requests transmission of a specific IF PDU
 *  \details        -
 *  \param[in]      id        Identification of the IF PDU.
 *  \param[in]      info      Length and pointer to the buffer of the IF PDU.
 *  \return         E_OK      Request is accepted by the destination module.
 *  \return         E_NOT_OK  Request is not accepted by the destination module.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different ids. Non reentrant for the same id.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfTransmit(
  PduIdType id,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info);

/**********************************************************************************************************************
 *  DoIP_IfCancelTransmit()
 *********************************************************************************************************************/
/*! \brief          Requests transmission cancellation of a specific IF PDU
 *  \details        -
 *  \param[in]      id        Identification of the IF PDU to be cancelled.
 *  \return         E_OK      Cancellation was executed successfully by the destination module.
 *  \return         E_NOT_OK  Cancellation was rejected by the destination module.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE - Reentrant for different ids. Non reentrant for the same id.
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfCancelTransmit(
  PduIdType id);

/**********************************************************************************************************************
 *  DoIP_TpChangeParameter()
 *********************************************************************************************************************/
/*! \brief          Implemented to support generic PduR modules.
 *  \details        -
 *  \param[in]      id          Identification of the TP PDU.
 *  \param[in]      parameter   Parameter identifier
 *  \param[in]      value       Parameter value
 *  \return         E_OK        Request is accepted.
 *  \return         E_NOT_OK    Request is not accepted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \note           No functionality is implemented.
 *  \note           Always returns E_NOT_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpChangeParameter(
  PduIdType id,
  TPParameterType parameter,
  uint16 value);

/**********************************************************************************************************************
 *  DoIP_EnablePduSizeRouting()
 *********************************************************************************************************************/
/*! \brief          Activates the DoIP packet size dependent routing.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_VSUPPORT_PDU_SIZE_ROUTING
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_EnablePduSizeRouting(void);

/**********************************************************************************************************************
 *  DoIP_DisablePduSizeRouting()
 *********************************************************************************************************************/
/*! \brief          Deactivates the DoIP packet size dependent routing.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         DOIP_VSUPPORT_PDU_SIZE_ROUTING
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_DisablePduSizeRouting(void);

/**********************************************************************************************************************
 *  DoIP_Shutdown()
 *********************************************************************************************************************/
/*! \brief          Shutdown of SoAd.
 *  \details        All sockets will be closed and modules change to special shutdown state.
 *  \return         E_OK                Shutdown request was accepted.
 *  \return         E_NOT_OK            Shutdown request was not accepted.
 *  \return         SOAD_E_INPROGRESS   Shutdown is in progress.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         DOIP_VSUPPORT_SHUTDOWN
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Shutdown(void);

# define DOIP_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* !defined(DOIP_H) */

/**********************************************************************************************************************
 *  END OF FILE: DoIP.h
 *********************************************************************************************************************/
