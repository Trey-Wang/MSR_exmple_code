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
/**        \file  Scc.h
 *        \brief  Smart Charge Communication Header File
 *
 *      \details  Implements Vehicle 2 Grid communication according to the specifications ISO/IEC 15118-2,
 *                DIN SPEC 70121 and customer specific schemas.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * AUTHOR IDENTITY
 * --------------------------------------------------------------------------------------------------------------------
 * Name                          Initials      Company
 * --------------------------------------------------------------------------------------------------------------------
 * Alex Lunkenheimer             visalr        Vector Informatik GmbH
 * Fabian Eisele                 visefa        Vector Informatik GmbH
 * Phanuel Hieber                visphh        Vector Informatik GmbH
 * --------------------------------------------------------------------------------------------------------------------
 * REVISION HISTORY
 * --------------------------------------------------------------------------------------------------------------------
 * Version  Date       Author  Change Id     Description
 * --------------------------------------------------------------------------------------------------------------------
 * 01.00.00 2010-01-01 visalr  -             created
 * 01.xx.xx 2013-07-04 visalr  -             removed revision history for version 1.xx.xx
 * 02.xx.xx 2013-08-21 visefa  -             removed revision history for version 2.xx.xx
 * 04.00.00 2012-09-24 visefa  ESCAN00061627 TCP Timeout Handling not functional
 *          2012-10-22 visefa  ESCAN00062434 Implemented EIM AC charging according to ISO15118 DIS
 *          2013-01-18 visefa  ESCAN00064321 Implemented PnC AC charging according to ISO15118 DIS
 *          2013-01-23 visefa  ESCAN00064463 Added compatibility for AUTOSAR4
 * 04.00.01 2013-04-09 visefa  ESCAN00066501 OEM specific: Issues with transmission of COM signals
 *          2013-04-19 visefa  ESCAN00066791 OEM specific: SAScheduleList is not processed correctly
 * 04.00.02 2013-04-24 visefa  ESCAN00066883 OEM specific: Issue with transmission of SAScheduleList over CanTp
 * 04.00.03 2013-05-24 visefa  ESCAN00067642 Compiler error: last line of file ends without a newline
 * 04.01.00 2013-06-11 visefa  ESCAN00067976 Smaller NvM blocks are now read with ReadAll()
 *          2013-06-17 visefa  ESCAN00068105 EVCCID is now read from EthIf
 *          2013-07-01 visefa  ESCAN00068451 OEM specific: Added Vector defined state machine
 *          2013-07-01 visefa  ESCAN00068457 OEM specific: Update of implementation to latest spec (2013-05-02)
 *          2013-07-01 visefa  ESCAN00068458 OEM specific: Update of implementation to latest spec (2013-06-04)
 *          2013-07-01 visefa  ESCAN00068459 Incoming EXI encoded messages are now handled in the MainFunction()
 * 04.01.01 2013-07-29 visefa  ESCAN00069382 Issues with integration of XmlSecurity
 *          2013-08-06 visefa  ESCAN00069621 OEM specific: Issue at transition from ChargingStatus to PowerDelivery
 *          2013-08-08 visefa  ESCAN00069658 Rx and Tx messages now share the same Exi Stream Buffer
 *          2013-08-19 visefa  ESCAN00069936 OEM specific: Incorrect error handling in case of negative SAP response
 *          2013-08-22 visefa  ESCAN00069975 Issues during CertificateInstallation and CertificateUpdate
 *          2013-09-03 visefa  ESCAN00069998 OEM specific: Invalid V2GTP payload type is accepted for SDP responses
 *          2013-09-27 visefa  ESCAN00070685 OEM specific: Extended RTE interface by SCCStatus and ErrorStatus
 *          2013-09-27 visefa  ESCAN00070757 OEM specific: Amount of ChargingProfile entries is now variable
 *          2013-10-10 visefa  ESCAN00071013 OEM specific: Previous message set is not saved to NvM
 * 05.00.00 2013-12-20 visefa  ESCAN00072746 Implemented AC charging according to ISO15118 FDIS
 *          2013-12-20 visefa  ESCAN00072748 OEM specific: Update of implementation to latest spec (2013-12-09)
 * 05.00.01 2014-01-17 visefa  ESCAN00073053 MISRA corrections (2014-01-17)
 * 05.00.02 2014-01-21 visefa  ESCAN00073108 Code Maintenance (2014-01-21)
 * 05.01.00 2014-01-30 visefa  ESCAN00073281 OEM specific: Implemented DC charging according to ISO15118 FDIS
 *          2014-01-30 visefa  ESCAN00073283 OEM specific: Implemented DC charging according to DIN70121 RC6
 * 05.01.01 2014-02-25 visefa  ESCAN00073915 OEM specific: Added "DIN70121 only" support
 * 05.01.02 2014-03-10 visefa  ESCAN00074185 OEM specific: Resolved issues in the OEM StateMachine
 * 05.02.00 2014-03-25 visefa  ESCAN00074546 OEM specific: Added support for additional OEM
 * 05.02.01 2014-04-24 visefa  ESCAN00075209 X509IssuerName is now encoded according to RFC4514
 *          2014-04-29 visefa  ESCAN00075251 OEM specific: Changed the order of the AppProtocols
 * 05.02.02 2014-05-19 visefa  ESCAN00075440 Signatures of CertInstall/Update parameters aren't validated correctly
 *          2014-06-10 visefa  ESCAN00076172 OEM specific: Update of implementation to latest spec (2014-05-23)
 * 05.03.00 2014-06-16 visefa  ESCAN00076292 OEM specific: Implemented IC charging according to customer spec
 *          2014-06-27 visefa  ESCAN00076549 OEM specific: Value of a CAN signal is not correctly set
 *          2014-06-27 visefa  ESCAN00076550 Message Timeout when looping in cyclic messages
 * 05.03.01 2014-07-02 visefa  ESCAN00076652 OEM specific: Compiler error: "undeclared identifier"
 *          2014-07-03 visefa  ESCAN00076719 OEM specific: Retransmission of CANTP packets not working correctly
 *          2014-07-03 visefa  ESCAN00076723 OEM specific: Changed unit of Next Request Delay to 100ms
 * 06.00.00 2014-06-26 visefa  ESCAN00076507 Added support for Tp_TcpIp API of MSR4R8 and higher
 *          2014-07-10 visefa  ESCAN00076909 Added support for encrypted storage of PrivateKeys
 *          2014-07-17 visefa  ESCAN00077168 EXI Stream and one of the two EXI Struct buffers are no longer required
 *          2014-07-18 visefa  ESCAN00077203 OEM specific: Update of implementation to latest spec (2014-07-09)
 *          2014-08-06 visefa  ESCAN00077655 OEM specific: NvM is not included if not used
 * 06.01.00 2014-12-05 visefa  ESCAN00080039 OEM specific: Update of implementation to latest spec (2014-12-02)
 *          2014-12-05 visefa  ESCAN00080040 OEM specific: Next Request Delay is now based on MainFunction cycles
 * 06.02.00 2014-12-17 visefa  ESCAN00079925 Invalidated NvM blocks are not handled correctly
 *          2015-01-09 visefa  ESCAN00080478 OEM specific: SCC does not accept an UDS ACK according to ISO14229
 *          2015-01-19 visefa  ESCAN00080041 OEM specific: Implemented IC charging according to customer spec
 *          2015-01-21 visefa  ESCAN00080714 Added support for TX streaming
 *          2015-02-02 visefa  ESCAN00081707 OEM specific: Added ContrCertPrivKey password callback for CANTP
 *          2015-02-17 visefa  ESCAN00081296 OEM specific: Rework of IEC61851 handling
 *          2015-02-18 visefa  ESCAN00081333 OEM specific: Enumeration was not implemented as specified
 *          2015-02-18 visefa  ESCAN00081336 OEM specific: Parameter was not reset only on edge detection
 *          2015-02-18 visefa  ESCAN00081337 OEM specific: Signal regarding CAN communication was not reset properly
 *          2015-02-18 visefa  ESCAN00081338 OEM specific: Error event not correctly reported to application
 *          2015-02-20 visefa  ESCAN00081389 DC_EVSEChargeParameter were reported before EVSEProcessing == Finished
 * 06.03.00 2015-02-27 visefa  ESCAN00081514 Added support for SLAC handling
 * 06.03.01 2015-03-09 visefa  ESCAN00081668 Reworked NvM handling
 * 06.03.02 2015-03-17 visefa  ESCAN00081865 SLAC doesn't start after Firmware Download since QCA is not responding
 * 07.00.00 2015-04-01 visefa  ESCAN00082090 Added support for Tp_TcpIp API of MSR4R12 and higher
 *          2015-05-11 visefa  ESCAN00082930 OEM specific: Update of implementation to latest spec (2015-04-22)
 *          2015-06-08 visefa  ESCAN00083287 OEM specific: Issue during restart after error in SAP
 *          2015-06-08 visefa  ESCAN00083288 OEM specific: Issue during conversion of ChargingProfile
 *          2015-06-30 visefa  ESCAN00083734 OEM specific: Error event not correctly reported to application
 *          2015-07-15 visefa  ESCAN00084009 Incorporated findings of review
 * 07.00.01 2015-07-20 visefa  ESCAN00084090 Obsolete DET error occurs periodically
 * 07.00.02 2015-07-23 visefa  ESCAN00084190 OEM specific: Update of customer requirement
 * 07.00.03 2015-08-13 visefa  ESCAN00084513 OEM specific: Provisioning Data is not always being sent automatically
 * 07.01.00 2015-08-31 visefa  ESCAN00084887 OEM specific: ChargingProfile can now be stored in internal SCC buffer
 *          2015-08-28 visefa  ESCAN00084838 OEM specific: Provisioning Data API keeps returning E_PENDING
 *          2015-09-11 visefa  ESCAN00085007 MSR4: Scc_Cbk_TL_TCPAccepted callback was removed
 *          2015-09-11 visefa  ESCAN00084956 CPS check is not working
 * 07.01.01 2015-10-13 visefa  ESCAN00085826 OEM specific: ChargingStatus stop condition not met
 *          2015-10-13 visefa  ESCAN00085827 OEM specific: Requirement change of SessionStop error handling
 * 08.00.00 2015-11-20 visefa  ESCAN00086647 OEM specific: Update of implementation to latest spec (2015-10-08)
 *          2015-11-20 visefa  ESCAN00086648 Introduction of Scc_ReturnType
 *          2015-11-20 visefa  ESCAN00086643 Compiler error: ASR4 APIs of TcpIp used for ASR3
 *          2015-11-24 visefa  ESCAN00086707 Extended StackError enumeration by Request(Not)Acknowledged
 *          2016-01-14 visefa  ESCAN00087057 PrivKeys of ContrCert and ProvCert are only read when required
 *          2016-01-14 visefa  ESCAN00087378 SCC keeps trying to decode received response after a decode error
 *          2016-01-14 visefa  ESCAN00087465 OEM specific: SalesTariff without duration element is not being accepted
 *          2016-02-03 visefa  ESCAN00088041 Source of configuration is now configurable
 *          2016-02-03 visefa  ESCAN00088042 OEM specific: Update of implementation to latest spec (2016-02-01)
 *          2016-02-16 visefa  ESCAN00088324 OEM specific: PnC is selected for resumed session even if TLS is not used
 *          2016-02-16 visefa  ESCAN00088325 OEM specific: Single phase not selected if only three phase offered
 * 08.00.01 2016-04-21 visefa  ESCAN00089647 OEM specific: Incorrect calculation of timeouts
 * 08.01.00 2016-06-21 visefa  ESCAN00090581 Configure parameter reporting during EVSEProcessing(Ongoing)
 *          2016-06-24 visefa  ESCAN00090652 OEM specific: Update of implementation to latest spec (2016-05-20)
 *          2016-06-24 visefa  ESCAN00090652 Introduced EMAID validation during CertInstallation & CertUpdate
 *          2016-07-04 visefa  ESCAN00090812 OEM specific: Added Ethernet Test Mode
 * 08.01.01 2016-07-11 visefa  ESCAN00090928 OEM specific: MessageSet selection via SupportedMessageSet not working
 * 08.01.02 2016-08-29 visefa  ESCAN00091627 OEM specific: Compiler error: Pre-processor defines not set correctly
 * 09.00.00 2016-07-18 visefa  ESCAN00090914 Added support for Tp_TcpIp API of MSR4R15 and higher
 *          2016-07-18 visefa  ESCAN00091045 Removed 'V' prefix from internal functions
 * 09.01.00 2016-10-07 visefa  ESCAN00092040 Local static variable used that cannot be mapped using MemMap.h
 *          2016-10-07 visefa  ESCAN00092047 Variable in ZERO_INIT section has no value assignment
 *          2016-12-05 visefa  ESCAN00092926 Empty RootCertificateID sent if not all root cert slots filled
 *          2016-12-05 visefa  ESCAN00092814 TCP FIN cannot be transmitted in case SCC message timeout expires
 *          2016-12-05 visefa  ESCAN00092681 Wrong ResponseCodes are stored in error bit environment data
 *          2016-12-05 visefa  ESCAN00092923 Command to open S2 is sent too early
 *          2016-12-06 visefa  ESCAN00092919 Add option to block V2G communication
 *          2016-12-07 visefa  ESCAN00092842 Add separate MemMap.h sections for NvM RAM/ROM variables/constants
 *          2016-12-07 visefa  ESCAN00093202 OEM specific: Increased signal size
 *          2016-12-07 visefa  ESCAN00092920 eMAID check fails if '-' is used in eMAID
 * 09.02.00 2017-01-16 visefa  ESCAN00093559 OEM specific: Updated RTE wrapper (2017-01-13)
 *          2017-01-16 visefa  ESCAN00093564 OEM specific: EVEnergyCapacity is not sent with the correct Unit value
 *          2017-01-25 visphh  ESCAN00093440 Remove monitoring of TxConfirmation
 *          2017-02-01 visphh  ESCAN00093788 OEM specific: Timeout after ChargeParameterDiscoveryRes
 *          2017-02-02 visphh  ESCAN00093202 EAmount can now handle a uint32
 * 09.03.00 2017-03-22 visphh  ESCAN00092165 Support of Dynamic Length in CopyTxData in TcpIp
 * 09.03.01 2017-03-22 visphh  ESCAN00093554 The check for the EVSEMaximumPowerLimit can now be disabled
 * 09.04.00 2017-05-10 visphh  ESCAN00095100 OEM specific: Updated RTE wrapper (2017-05-09)
 * 09.04.01 2017-05-11 visphh  ESCAN00095119 Changed handling of ForceReset in Scc_ResetSessionID
 * 09.05.00 2017-05-29 visphh  ESCAN00095336 Scc_ConfigValue_StateM_AcceptUnsecureConnection undeclared identifier
 *          2017-05-29 visphh  ESCAN00094843 SCC wants to restart without SLAC even if no link is available
 *          2017-06-06 visphh  ESCAN00095186 Compiler warning: comma at end of enumerator list 
 *          2017-07-04 visphh  ESCAN00095008 OEM specific: Component TLS can not process other users in addition to component SCC
 *          2017-07-03 visphh  ESCAN00095709 XmlSec_ReturnType is now an enumeration instead of a uint8 using defines
 *          2017-07-12 visefa  ESCAN00095558 OEM specific: RemainingTimeToFullSoC is not sent in case of DIN SPEC 70121
 *          2017-07-12 visefa  ESCAN00095549 OEM specific: Update of implementation to latest spec (2017-04-07)
 *          2017-07-12 visefa  ESCAN00095158 OEM specific: Random values are being sent when CAN signal is SNA
 *          2017-07-12 visefa  ESCAN00095431 OEM specific: PnC is selected instead of EIM
 *          2017-07-12 visefa  ESCAN00095440 Added SessionID to the parameter interface
 *          2017-07-12 visefa  ESCAN00095539 Added V2GRequest and V2GResponse to the parameter interface
 *          2017-07-12 visefa  ESCAN00095161 Invalid SECCDiscoveryProtocolRes are not ignored
 *          2017-07-12 visefa  ESCAN00094502 OEM specific: PCID can't be read from Provisioning Certificate
 *          2017-07-03 visphh  ESCAN00095706 Add CSM (ASR4.3) Support
 *          2017-07-31 visphh  ESCAN00096116 No report of SLAC failure and ReadRootCerts failure via Scc_StackError
 * 09.05.01 2017-08-30 visphh  ESCAN00096485 OEM specific: DIN EVSEProcessing Callback returns RTE-Error
 * 09.06.00 2017-09-07 visphh  STORYC-2437   Extend Scc_GetCertDistinguishedNameObject() for Subject_DomainComponent
 *          2017-09-12 visphh  STORYC-2465   Report Scc_ReportError if Scc_MsgTrig is Scc_MsgTrig_None
 *          2017-09-13 visphh  ESCAN00096642 Function Scc_ChangeProvCertPrivKeyPassword() returns Pending instead of Busy
 *          2017-09-14 visphh  STORYC-2490   Add Callback to report an unknown leaf certificate during TLS handshake
 * 09.06.01 2017-09-21 visphh  ESCAN00096762 Scc_CheckForCpoInDc only available for PnC
 *********************************************************************************************************************/

#if !defined (SCC_H)
#define SCC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Types.h"
#include "Scc_Cfg.h"
#include "Scc_Lcfg.h"
#include "Scc_PBcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject :   SYSSERVICE_SSWSCC CQComponent : Implementation */
#define SYSSERVICE_SSWSCC_VERSION          (0x0906u) /* BCD coded version number */
#define SYSSERVICE_SSWSCC_RELEASE_VERSION  (0x01u)   /* BCD coded release version number */

/* Software Version */
#define SCC_SW_MAJOR_VERSION 9u
#define SCC_SW_MINOR_VERSION 6u
#define SCC_SW_PATCH_VERSION 1u

/* Scc ModuleId */
#define SCC_VENDOR_ID    30u /* Vector ID */
#define SCC_MODULE_ID   255u /* Vector Module ID for Complex Device Drivers */
#define SCC_INSTANCE_ID 111u /* Vector Instance ID for SysService_SswScc */

/* Scc ApiIds */
#define SCC_API_ID_INIT                    0x01u /* Scc_Init() */
#define SCC_API_ID_RX_INDICATION           0x03u /* Scc_RxIndication() */
#define SCC_API_ID_IP_ADDRESS_CHG_CBK      0x04u /* Scc_Cbk_LocalIpAssignmentChg() */
#define SCC_API_ID_GET_VERSION_INFO        0x05u /* Scc_GetVersionInfo() */
#define SCC_API_ID_DIAG_DATA_READ_ACCESS   0x06u /* Scc_DiagDataReadAccess() */
#define SCC_API_ID_DIAG_DATA_WRITE_ACCESS  0x07u /* Scc_DiagDataWriteAccess() */
#define SCC_API_ID_TCP_CONNECTED           0x08u /* Scc_TcpConnected() */
#define SCC_API_ID_TCP_ACCEPTED            0x09u /* Scc_TcpAccepted() */
#define SCC_API_ID_TCP_IP_EVENT            0x0Au /* Scc_TcpIpEvent() */
#define SCC_API_ID_TCP_IP_CERT             0x0Bu /* Scc_Cbk_TLS_Cert() */

#define SCC_API_ID_V_TRIG_MSG                 0x20u /* Scc_TriggerV2G() */
#define SCC_API_ID_V_TIMEOUT_HANDLING         0x21u /* Scc_TimeoutHandling() */
#define SCC_API_ID_V_NVM_READ_ROOT_CERTS      0x22u /* Scc_NvmReadRootCerts() */
#define SCC_API_ID_V_CHECK_V2G_HEADER         0x23u /* Scc_CheckV2GHeader() */
#define SCC_API_ID_V_XCRYPT_PRIV_KEY          0x24u /* Scc_XcryptData() */
#define SCC_API_ID_V_VALIDATE_CERT_CHAIN      0x25u /* Scc_ValidateCertChain() */
#define SCC_API_ID_V_TRIG_NVM                 0x26u /* Scc_TriggerNVRAM() */
#define SCC_API_ID_V_ADJUST_AMOUNT_2_MULT     0x27u /* Scc_AdjustAmountToMultiplier() */
#define SCC_API_ID_V_REPORT_ERROR             0x28u /* Scc_ReportError() */
#define SCC_API_ID_V_INIT                     0x29u /* Scc_Init() */

#define SCC_API_ID_V_VALIDATE_ECDSA_SIG       0x2Bu /* Scc_ValidateECDSASignature() */
#define SCC_API_ID_V_GET_CERT_DN              0x2Cu /* Scc_GetCertDistinguishedNameObject() */
#define SCC_API_ID_V_GET_CERT_ISSUER          0x2Du /* Scc_GetCertIssuer() */
#define SCC_API_ID_V_ADJUST_VALUE_2_MULT      0x2Eu /* Scc_AdjustAmountToMultiplier() */

#define SCC_API_ID_V_EXI_DECODE_SAP_RES       0x40u /* Scc_Exi_DecodeSupportedAppProtocolRes() */

#define SCC_API_ID_V_EXITX_INV_ENERG_TRANS_M  0x41u /* invalid energy transfer mode selected */
#define SCC_API_ID_V_EXIRX_ISO_DECODE_MESSAGE 0x42u /* Scc_ExiRx_ISO_DecodeMessage() */
#define SCC_API_ID_V_EXIRX_DIN_DECODE_MESSAGE 0x43u /* Scc_ExiRx_DIN_DecodeMessage() */
#define SCC_API_ID_V_CERTIFICATE_UPDATE_RES   0x44u /* Scc_ExiRx_ISO_CertificateUpdateRes() */
#define SCC_API_ID_V_CERTIFICATE_INSTALL_RES  0x45u /* Scc_ExiRx_ISO_CertificateInstallationRes() */
#define SCC_API_ID_V_EXITX_ISO_ENCODE_MESSAGE 0x46u /* Scc_ExiTx_ISO_EncodeMessage() */
#define SCC_API_ID_V_EXITX_DIN_ENCODE_MESSAGE 0x47u /* Scc_ExiTx_DIN_EncodeMessage() */
#define SCC_API_ID_V_EXI_INIT_WS              0x48u /* Scc_Exi_Init(En/De)codingWorkspace() */
#define SCC_API_ID_V_EXI_FIN_EXI_STREAM       0x49u /* Scc_Exi_EncodeAndTransmitExiStream() */

#define SCC_API_ID_V_EXI_ISO_CURRENT_DEMAND   0x4Bu /* Scc_ExiRx_ISO_CurrentDemandRes() */
#define SCC_API_ID_V_EXI_ISO_CHARGING_STATUS  0x4Cu /* Scc_ExiRx_ISO_ChargingStatusRes() */
#define SCC_API_ID_V_CERTIFICATE_UPDATE_REQ   0x4Du /* Scc_ExiRx_ISO_CertificateUpdateRes() */
#define SCC_API_ID_V_CERTIFICATE_INSTALL_REQ  0x4Eu /* Scc_ExiRx_ISO_CertificateInstallationRes() */
#define SCC_API_ID_V_AUTHORIZATION_REQ        0x4Fu /* Scc_ExiTx_ISO_AuthorizationReq() */
#define SCC_API_ID_V_METERING_RECEIPT_REQ     0x50u /* Scc_ExiTx_ISO_MeteringReceiptReq() */

#if ( defined SCC_OEM_VECTOR )
#define SCC_API_ID_V_STATEM_MAIN_FUNCTION     0xA0u /* Scc_StateM_MainFunction() */
#define SCC_API_ID_V_STATEM_GLOBAL_TIMER      0xA1u /* Scc_StateM_VGlobalTimerChecks () */
#define SCC_API_ID_V_STATEM_PREP_SERV_DETAIL  0xA2u /* Scc_StateM_PrepareServiceDetailReq() */
#define SCC_API_ID_V_STATEM_SET_MSG_STATUS    0xA3u /* Scc_StateM_Set_Core_MsgStatus() */
#define SCC_API_ID_V_STATEM_PROC_CHARGE_PARAM 0xA4u /* Scc_StateM_ProcessChargeParameterDiscoveryRes() */
#define SCC_API_ID_V_STATEM_PROC_SERV_DISC    0xA5u /* Scc_StateM_ProcessServiceDiscoveryRes() */
#define SCC_API_ID_V_STATEM_PROC_POW_DEL      0xA6u /* Scc_StateM_ProcessPowerDeliveryRes() */
#define SCC_API_ID_V_STATEM_CORE_GET_CBK      0xA9u /* Scc_StateM_Get_Xyz_Zyx() */
#endif

/* SCC DET errors */
#define SCC_DET_NOT_INITIALIZED  0x01u /* This module is not initialized yet */
#define SCC_DET_INV_POINTER      0x02u /* The provided pointer(s) is/are invalid */
#define SCC_DET_INV_PARAM        0x03u /* The provided parameter(s) is/are invalid */
#define SCC_DET_INV_STATE        0x04u /* The state machine ended up in an invalid state */
#define SCC_DET_EXI              0x05u /* An error occurred during an interaction with Exi */
#define SCC_DET_XML_SEC          0x06u /* An error occurred during an interaction with XmlSecurity */
#define SCC_DET_CRYPTO           0x07u /* An error occurred during an interaction with Crypto */
#define SCC_DET_IP_BASE          0x08u /* An error occurred during an interaction with IpBase */
#define SCC_DET_TL               0x09u /* An error occurred during an interaction with the transport layer (TLS/TCP) */
#define SCC_DET_EXT_CMP_NOT_INIT 0x0Au /* An external component was not initialized */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - no init */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - const */
#define SCC_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SCC_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - no init - nvm */
#define SCC_START_SEC_VAR_NOINIT_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( defined SCC_SESSION_ID_NVM_BLOCK_LEN )
extern SCC_VARNOINIT(uint8) Scc_SessionIDNvm[SCC_SESSION_ID_NVM_BLOCK_LEN];
#endif /* SCC_SESSION_ID_NVM_BLOCK_LEN */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* 8bit variables - const - nvm */
#define SCC_START_SEC_CONST_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )) || (( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )))
extern SCC_CONSTCONST(uint8) Scc_SessionIDNvmRomDefault[SCC_SESSION_ID_NVM_BLOCK_LEN];
#endif /* SCC_SCHEMA_ISO, SCC_SCHEMA_ISO_ED2_CD */

#define SCC_STOP_SEC_CONST_8BIT_NVM
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Scc_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      Scc_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
                The versions are BCD-coded.
 *  \param[in]  VersionInfoPtr        pointer for version info
 *  \context    initialization or task level
 *********************************************************************************************************************/
#if ( defined SCC_VERSION_INFO_API ) && ( SCC_VERSION_INFO_API == STD_ON )
SCC_FUNCCODE(void) Scc_GetVersionInfo(SCC_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr);
#endif /* SCC_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  Scc_InitMemory
 *********************************************************************************************************************/
/*! \brief      initializes global variables
 *  \pre        has to be called before any other calls to the module
 *  \context    task level
 *  \note       AUTOSAR extension
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_InitMemory(void);

/**********************************************************************************************************************
 *  Scc_Init
 *********************************************************************************************************************/
/*! \brief      stores the start address of the post build time configuration of the module
 *                and may be used to initialize the data  structures
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \pre        has to be called before useage of the module
 *  \context    initialization
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Init(SCC_P2CONSTCFGROOT(Scc_ConfigType) CfgPtr);

/**********************************************************************************************************************
 *  Scc_MainFunction
 *********************************************************************************************************************/
/*! \brief      re-initialize closed connections and transmit pending data
 *  \context    task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_MainFunction(void);

/**********************************************************************************************************************
 *  Scc_ResetSessionID
 *********************************************************************************************************************/
/*! \brief      resets the SessionID to '0x00'
 *  \pre        will only have effect outside of a V2G session
 *  \param[in]  ForceReset      force the reset, even when a V2G Communication Session is ongoing
 *  \return     E_OK            SessionID has been reset
 *  \return     E_NOT_OK        a V2G session is currently active, please try again later
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_ResetSessionID(boolean ForceReset);

#if ( defined SCC_NUM_OF_DYN_CONFIG_PARAMS ) && ( SCC_NUM_OF_DYN_CONFIG_PARAMS != 0 )
/**********************************************************************************************************************
 *  Scc_DynConfigDataReadAccess
 *********************************************************************************************************************/
/*! \brief       configuration data read access
 *  \param[in]   DataID         data identifier
 *  \param[out]  DataPtr        pointer for diagnostic data
 *  \return      E_OK           configuration data was successfully read
 *  \return      E_NOT_OK       invalid DataID
 *  \context     task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_DynConfigDataReadAccess(Scc_DynConfigParamsType DataID,
  SCC_P2VARAPPLDATA(uint16) DataPtr);
#endif /* SCC_NUM_OF_DYN_CONFIG_PARAMS */

#if ( defined SCC_NUM_OF_DYN_CONFIG_PARAMS ) && ( SCC_NUM_OF_DYN_CONFIG_PARAMS != 0 )
/**********************************************************************************************************************
 *  Scc_DynConfigDataWriteAccess
 *********************************************************************************************************************/
/*! \brief       configuration data write access
 *  \param[in]   DataID         data identifier
 *  \param[in]   Data           configuration data that shall be written to NVRAM
 *  \return      E_OK           configuration data was successfully written
 *  \return      E_NOT_OK       invalid DataID
 *  \context     task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_DynConfigDataWriteAccess(Scc_DynConfigParamsType DataID, uint16 Data);
#endif /* SCC_NUM_OF_DYN_CONFIG_PARAMS */

#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )
/**********************************************************************************************************************
 *  Scc_ChangeContrCertPrivKeyPassword
 *********************************************************************************************************************/
/*! \brief      changes the key of the stored private key
 *  \pre        not available during an active V2G session
 *  \param[in]  NewPasswordPtr   pointer to the new password
 *  \param[in]  NewSaltPtr       pointer to the new salt (optional, if not used set to NULL)
 *  \return     OK               key was changed successfully
 *  \return     Busy             change of key currently not possible because of an active V2G session
 *  \return     Pending          change of key ongoing
 *  \return     NotOK            an error occurred while changing the key
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ChangeContrCertPrivKeyPassword(
  SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewPasswordPtr, SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewSaltPtr);
#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

#if ( defined SCC_ENABLE_CERTIFICATE_INSTALLATION ) && ( SCC_ENABLE_CERTIFICATE_INSTALLATION == STD_ON )
/**********************************************************************************************************************
 *  Scc_ChangeProvCertPrivKeyPassword
 *********************************************************************************************************************/
/*! \brief      changes the key of the stored private key
 *  \pre        not available during an active V2G session
 *  \param[in]  NewPasswordPtr   pointer to the new password
 *  \param[in]  NewSaltPtr       pointer to the new salt (optional, if not used set to NULL)
 *  \return     OK               key was changed successfully
 *  \return     Busy             change of key currently not possible because of an active V2G session
 *  \return     Pending          change of key ongoing
 *  \return     NotOK            an error occurred while changing the key
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ChangeProvCertPrivKeyPassword(
  SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewPasswordPtr, SCC_P2CONSTAPPLDATA(Scc_BufferPointerType) NewSaltPtr);
#endif /* SCC_ENABLE_CERTIFICATE_INSTALLATION */

#if ( defined SCC_ENABLE_CERTIFICATE_INSTALLATION ) && ( SCC_ENABLE_CERTIFICATE_INSTALLATION == STD_ON )
/**********************************************************************************************************************
 *  Scc_ValidateProvCertKeyPair
 *********************************************************************************************************************/
/*! \brief      validates the provisioning certificate by checking the private key against the public key
 *  \pre        provisioning certificate has to be read from NVRAM
 *  \return     OK             private and public key pair validated successfully
 *  \return     Pending        provisioning certificate or its private key are still loaded from NVRAM
 *  \return     NotOK          NvM read error, or public and private key are no pair
 *  \context    task level
 *********************************************************************************************************************/
SCC_FUNCCODE(Scc_ReturnType) Scc_ValidateProvCertKeyPair(void);
#endif /* SCC_ENABLE_CERTIFICATE_INSTALLATION */

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* SCC_H */
/**********************************************************************************************************************
 *  END OF FILE: Scc.h
 *********************************************************************************************************************/
