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
/**        \file  Scc_Types.h
 *        \brief  Smart Charge Communication Header File
 *
 *      \details  Implements Vehicle 2 Grid communication according to the specifications ISO/IEC 15118-2, 
 *                DIN SPEC 70121 and customer specific schemas. 
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Scc module. >> Scc.h
 *********************************************************************************************************************/ 
#if !defined (SCC_TYPES_H)
# define SCC_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Scc_Cfg.h"

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
#include "EthTrcv_30_Ar7000_Types.h"
#endif /* SCC_ENABLE_SLAC_HANDLING */
#include "Exi_SchemaTypes.h"
#include "IpBase_Types.h"
#include "TcpIp_Types.h"
#if ( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON )
#include "Tls_Types.h"
#endif /* SCC_ENABLE_TLS */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define Scc_V2GTPOffsets_Version        0u
#define Scc_V2GTPOffsets_InverseVersion 1u
#define Scc_V2GTPOffsets_PayloadType    2u
#define Scc_V2GTPOffsets_PayloadLength  4u

#define Scc_SDPReqOffsets_Security   0u
#define Scc_SDPReqOffsets_Protocol   1u
#define Scc_SDPResOffsets_IPAddress  0u
#define Scc_SDPResOffsets_Port      16u
#define Scc_SDPResOffsets_Security  18u
#define Scc_SDPResOffsets_Protocol  19u

#define SCC_IPvX_IPV6 TCPIP_AF_INET6

# if ( defined STATIC )
# else
#define STATIC static
# endif /* STATIC */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */

#define SCC_P2VARAPPLDATA(Type)        P2VAR(Type, AUTOMATIC, SCC_APPL_DATA)
#define SCC_P2CONSTAPPLDATA(Type)      P2CONST(Type, AUTOMATIC, SCC_APPL_DATA)
#define SCC_CONSTP2VARAPPLDATA(Type)   CONSTP2VAR(Type, AUTOMATIC, SCC_APPL_DATA)
#define SCC_CONSTP2CONSTAPPLDATA(Type) CONSTP2CONST(Type, AUTOMATIC, SCC_APPL_DATA)
#define SCC_P2VARNOINIT(Type)          P2VAR(Type, AUTOMATIC, SCC_VAR_NOINIT)
#define SCC_P2CONSTNOINIT(Type)        P2CONST(Type, AUTOMATIC, SCC_VAR_NOINIT)
#define SCC_CONSTP2VARNOINIT(Type)     CONSTP2VAR(Type, AUTOMATIC, SCC_VAR_NOINIT)
#define SCC_VARNOINIT(Type)            VAR(Type, SCC_VAR_NOINIT)
#define SCC_VARZEROINIT(Type)          VAR(Type, SCC_VAR_ZERO_INIT)
#define SCC_CONSTCONST(Type)           CONST(Type, SCC_CONST)
#define SCC_CONSTP2CONST(Type)         CONSTP2CONST(Type, AUTOMATIC, SCC_CONST)
#define SCC_FUNCCODE(Type)             FUNC(Type, SCC_CODE)
#define SCC_STATICFUNCCODE(Type)       STATIC FUNC(Type, SCC_STATIC_CODE)

#define SCC_P2CONSTCFGROOT(Type)       P2CONST(Type, AUTOMATIC, SCC_CONST)
#define SCC_P2CONSTCFG(Type)           P2CONST(Type, AUTOMATIC, SCC_CONST)

/* Changed API and Callback Names */

#define Scc_DeleteRootCertificate      Scc_DeleteRootCert
#define Scc_Cbk_TrcvLinkStateChg       Scc_Cbk_Eth_TransceiverLinkStateChange
#define Scc_LocalIpAddrAssignmentChg   Scc_Cbk_IP_AddressAssignmentChange
#define Scc_Cbk_LocalIpAssignmentChg   Scc_Cbk_IP_AddressAssignmentChange
#define Scc_RxIndication               Scc_Cbk_TL_RxIndication
#define Scc_TcpAccepted                Scc_Cbk_TL_TCPAccepted
#define Scc_TcpConnected               Scc_Cbk_TL_TCPConnected
#define Scc_TcpIpEvent                 Scc_Cbk_TL_TCPEvent

/* PRQA L:MACROS_FUNCTION_LIKE */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* PRQA S 0777 EXT_IDENTIFIER_LENGTH */ /* MD_Scc_0777_0779 */
/* PRQA S 0779 IDENTIFIER_LENGTH */ /* MD_Scc_0777_0779 */

#define Scc_SockAddrType    TcpIp_SockAddrType
#define Scc_SockAddrIn6Type TcpIp_SockAddrInet6Type
#define Scc_PbufType        TcpIp_PbufType
#define Scc_SocketType      TcpIp_SocketIdType

typedef unsigned int Scc_BitType;

typedef enum Scc_returnType
{
  Scc_ReturnType_OK = E_OK,
  Scc_ReturnType_NotOK = E_NOT_OK,
  Scc_ReturnType_Pending = 2u,
  Scc_ReturnType_Busy = 3u
} Scc_ReturnType;

typedef enum Scc_stateType
{
  Scc_State_Uninitialized,
  Scc_State_Initialized,
  Scc_State_SDPComplete,
  Scc_State_TLConnecting,
  Scc_State_TLConnected,
  Scc_State_SAPComplete,
  Scc_State_Connected,
  Scc_State_Disconnected,
  Scc_State_ShuttingDown,
  Scc_State_ShutDown
} Scc_StateType;

typedef enum Scc_msgTrigType
{
  Scc_MsgTrig_None,
#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  Scc_MsgTrig_SLAC,
#endif /* SCC_ENABLE_SLAC_HANDLING */
  Scc_MsgTrig_SECCDiscoveryProtocol,
  Scc_MsgTrig_TransportLayer,
  Scc_MsgTrig_SupportedAppProtocol,
  Scc_MsgTrig_SessionSetup,
  Scc_MsgTrig_ServiceDiscovery,
  Scc_MsgTrig_ReadContrCertChain,
  Scc_MsgTrig_ServiceDetail,
  Scc_MsgTrig_PaymentServiceSelection, /* Scc_MsgTrig_ServicePaymentSelection */
  Scc_MsgTrig_ReadRootCerts,
  Scc_MsgTrig_ReadProvCert,
  Scc_MsgTrig_CertificateInstallation,
  Scc_MsgTrig_CertificateUpdate,
  Scc_MsgTrig_PaymentDetails,
  Scc_MsgTrig_Authorization, /* Scc_MsgTrig_ContractAuthentication */
  Scc_MsgTrig_ChargeParameterDiscovery,
  Scc_MsgTrig_PowerDelivery,
  Scc_MsgTrig_MeteringReceipt,
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  Scc_MsgTrig_ChargingStatus,
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_MsgTrig_CableCheck,
  Scc_MsgTrig_PreCharge,
  Scc_MsgTrig_CurrentDemand,
  Scc_MsgTrig_WeldingDetection,
#endif /* SCC_CHARGING_DC */
  Scc_MsgTrig_SessionStop,
  Scc_MsgTrig_StopCommunicationSession
} Scc_MsgTrigType;

typedef enum Scc_msgStatusType
{
  Scc_MsgStatus_None,
#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
  Scc_MsgStatus_SLAC_OK,
  Scc_MsgStatus_SLAC_Failed,
#endif /* SCC_ENABLE_SLAC_HANDLING */
  Scc_MsgStatus_SECCDiscoveryProtocol_OK,
  Scc_MsgStatus_SECCDiscoveryProtocol_Failed,
  Scc_MsgStatus_TransportLayer_OK,
  Scc_MsgStatus_TransportLayer_Failed,
  Scc_MsgStatus_SupportedAppProtocol_OK,
  Scc_MsgStatus_SupportedAppProtocol_Failed,
  Scc_MsgStatus_SessionSetup_OK,
  Scc_MsgStatus_SessionSetup_Failed,
  Scc_MsgStatus_ServiceDiscovery_OK,
  Scc_MsgStatus_ServiceDiscovery_Failed,
  Scc_MsgStatus_ReadContrCertChain_OK,
  Scc_MsgStatus_ReadContrCertChain_Failed,
  Scc_MsgStatus_ServiceDetail_OK,
  Scc_MsgStatus_ServiceDetail_Failed,
  Scc_MsgStatus_PaymentServiceSelection_OK, /* Scc_MsgStatus_ServicePaymentSelection_OK */
  Scc_MsgStatus_PaymentServiceSelection_Failed, /* Scc_MsgStatus_ServicePaymentSelection_Failed */
  Scc_MsgStatus_ReadRootCerts_OK,
  Scc_MsgStatus_ReadRootCerts_Failed,
  Scc_MsgStatus_ReadProvCert_OK,
  Scc_MsgStatus_ReadProvCert_Failed,
  Scc_MsgStatus_CertificateInstallation_OK,
  Scc_MsgStatus_CertificateInstallation_Failed,
  Scc_MsgStatus_CertificateUpdate_OK,
  Scc_MsgStatus_CertificateUpdate_Failed,
  Scc_MsgStatus_PaymentDetails_OK,
  Scc_MsgStatus_PaymentDetails_Failed,
  Scc_MsgStatus_Authorization_OK, /* Scc_MsgStatus_ContractAuthentication_OK */
  Scc_MsgStatus_Authorization_Failed, /* Scc_MsgStatus_ContractAuthentication_Failed */
  Scc_MsgStatus_ChargeParameterDiscovery_OK,
  Scc_MsgStatus_ChargeParameterDiscovery_Failed,
  Scc_MsgStatus_PowerDelivery_OK,
  Scc_MsgStatus_PowerDelivery_Failed,
  Scc_MsgStatus_MeteringReceipt_OK,
  Scc_MsgStatus_MeteringReceipt_Failed,
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  Scc_MsgStatus_ChargingStatus_OK,
  Scc_MsgStatus_ChargingStatus_Failed,
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_MsgStatus_CableCheck_OK,
  Scc_MsgStatus_CableCheck_Failed,
  Scc_MsgStatus_PreCharge_OK,
  Scc_MsgStatus_PreCharge_Failed,
  Scc_MsgStatus_CurrentDemand_OK,
  Scc_MsgStatus_CurrentDemand_Failed,
  Scc_MsgStatus_WeldingDetection_OK,
  Scc_MsgStatus_WeldingDetection_Failed,
#endif /* SCC_CHARGING_DC */
  Scc_MsgStatus_SessionStop_OK,
  Scc_MsgStatus_SessionStop_Failed,
  Scc_MsgStatus_StopCommunicationSession_OK,
  Scc_MsgStatus_StopCommunicationSession_Failed
} Scc_MsgStatusType;

typedef enum Scc_msgStateType
{
  Scc_MsgState_WaitForNextRequest,
  Scc_MsgState_StreamingRequest,
  Scc_MsgState_RequestSent,
  Scc_MsgState_ResponseReceived
} Scc_MsgStateType;

typedef enum Scc_stackErrorType
{
  Scc_StackError_NoError,
  Scc_StackError_SLAC,
  Scc_StackError_TransportLayer,
  Scc_StackError_InvalidTxParameter,
  Scc_StackError_Timeout,
  Scc_StackError_IpBase,
  Scc_StackError_Exi,
  Scc_StackError_XmlSecurity,
  Scc_StackError_Crypto,
  Scc_StackError_InvalidRxMessage,
  Scc_StackError_InvalidRxParameter,
  Scc_StackError_NegativeResponseCode,
  Scc_StackError_NvM
} Scc_StackErrorType;

typedef enum Scc_sdpSecurityType
{
  Scc_SDPSecurity_Tls = 0x00u,
  Scc_SDPSecurity_None = 0x10u
} Scc_SDPSecurityType;

typedef enum Scc_sapSchemaIDType
{
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0 )
  Scc_SAPSchemaIDs_ISO,
#endif /* SCC_SCHEMA_ISO */
#if ( defined SCC_SCHEMA_ISO_ED2_CD ) && ( SCC_SCHEMA_ISO_ED2_CD != 0 )
  Scc_SAPSchemaIDs_ISO_Ed2_CD,
#endif /* SCC_SCHEMA_ISO_ED2_CD */
#if ( defined SCC_SCHEMA_DIN ) && ( SCC_SCHEMA_DIN != 0 )
  Scc_SAPSchemaIDs_DIN,
#endif /* SCC_SCHEMA_DIN */
  Scc_SAPSchemaIDs_NumOfSupportedSchemas, /* not to be used by customer */
  Scc_SAPSchemaIDs_None
} Scc_SAPSchemaIDType;

typedef enum Scc_diagParamsType
{
  Scc_DP_ContractCertificateChainSize      = 0x00u,
  Scc_DP_ContractCertificate               = 0x01u,
  Scc_DP_ContractCertificatePrivateKey     = 0x02u,
  Scc_DP_EMAID                             = 0x03u,
  Scc_DP_ProvisioningCertificate           = 0x04u,
  Scc_DP_ProvisioningCertificatePrivateKey = 0x05u,
  Scc_DP_PCID                              = 0x06u,
  /* starting from here, only bit masks are allowed */
  Scc_DP_ContractSubCertificate1           = 0x10u,
  Scc_DP_RootCertificate1                  = 0x20u
} Scc_DiagParamsType;

typedef enum Scc_serviceIDType
{
  Scc_ServiceID_Certificate = 0x02u,
  Scc_ServiceID_Internet = 0x03u
} Scc_ServiceIDType;

typedef enum Scc_parameterSetIDType
{
  Scc_ParameterSetID_Certificate_Installation = 0x01u,
  Scc_ParameterSetID_Certificate_Update       = 0x02u,
  Scc_ParameterSetID_Internet_Https           = 0x04u
} Scc_ParameterSetIDType;

typedef enum Scc_nvmBlockReadStateType
{
  Scc_NvMBlockReadState_NotSet,
  Scc_NvMBlockReadState_ReadRequested,
  Scc_NvMBlockReadState_Error,
  Scc_NvMBlockReadState_ReadFinished,
  Scc_NvMBlockReadState_Processed,
  Scc_NvMBlockReadState_Invalidated
} Scc_NvMBlockReadStateType;

typedef enum Scc_firmwareDownloadStatusType
{
  Scc_FirmwareDownloadStatus_Unknown,
  Scc_FirmwareDownloadStatus_ErrorOccurred,
  Scc_FirmwareDownloadStatus_Complete
} Scc_FirmwareDownloadStatusType;

typedef enum Scc_tcpSocketStateType
{
  Scc_TCPSocketState_Closed,
  Scc_TCPSocketState_Connecting,
  Scc_TCPSocketState_Connected,
  Scc_TCPSocketState_Disconnecting
} Scc_TCPSocketStateType;

typedef enum Scc_xcryptDataOperationModeType
{
  Scc_XDOM_Decrypt,
  Scc_XDOM_Encrypt
} Scc_XcryptDataOperationModeType;

typedef enum Scc_contrCertStatusType
{
  Scc_CCS_NoContrCertAvailable,
  Scc_CCS_ContrCertAvailable,
  Scc_CCS_ContrCertExpiresSoon,
  Scc_CCS_ContrCertExpired
} Scc_ContrCertStatusType;

typedef union Scc_Exi_structBufType
{ /* PRQA S 0750 */ /* MD_Scc_0750_0759 */
  uint8  Buf8[SCC_EXI_STRUCT_BUF_LEN];
  uint32 Buf32[SCC_EXI_STRUCT_BUF_LEN/4];
} Scc_Exi_StructBufType;

typedef struct Scc_bufferPointerType
{
  SCC_P2VARAPPLDATA(uint8) BufferPtr;
  uint16                   BufferLen;
} Scc_BufferPointerType;

typedef struct Scc_txRxBufferPointerType
{
  SCC_P2VARAPPLDATA(IpBase_PbufType) PbufPtr;
  boolean                            FirstPart;
  boolean                            StreamComplete;
} Scc_TxRxBufferPointerType;

typedef struct Scc_supportedSAPSchemasType
{
  Scc_SAPSchemaIDType Schema[Scc_SAPSchemaIDs_NumOfSupportedSchemas];
  uint8               Priority[Scc_SAPSchemaIDs_NumOfSupportedSchemas];
} Scc_SupportedSAPSchemasType;

typedef enum Scc_forceSAPSchemasType
{
  Scc_ForceSAPSchemas_None,
  Scc_ForceSAPSchemas_ISO,
  Scc_ForceSAPSchemas_DIN
} Scc_ForceSAPSchemasType;

#if ( defined SCC_OEM_VECTOR )

typedef enum Scc_StateM_stateType
{
  Scc_SMS_Uninitialized,
  Scc_SMS_Initialized,
  Scc_SMS_SLAC,
  Scc_SMS_WaitForIPAddress,
  Scc_SMS_SECCDiscoveryProtocol,
  Scc_SMS_TLConnection,
  Scc_SMS_SupportedAppProtocol,
  Scc_SMS_SessionSetup,
  Scc_SMS_ServiceDiscovery,
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0u )
  Scc_SMS_ServiceDetail,
#endif /* SCC_SCHEMA_ISO */
  Scc_SMS_PaymentServiceSelection,
  Scc_SMS_Authorization,
  Scc_SMS_ChargeParameterDiscovery,
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_SMS_CableCheck,
  Scc_SMS_PreCharge,
#endif /* SCC_CHARGING_DC */
  Scc_SMS_PowerDelivery,
#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  Scc_SMS_ChargingStatus,
#endif /* SCC_CHARGING_AC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_SMS_CurrentDemand,
#endif /* SCC_CHARGING_DC */
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_SMS_WeldingDetection,
#endif /* SCC_CHARGING_DC */
  Scc_SMS_SessionStop,
  Scc_SMS_StopCommunicationSession,
  Scc_SMS_Stopped
} Scc_StateM_StateType;

typedef enum Scc_StateM_msgStateType
{
  Scc_SMMS_PreparingRequest,
  Scc_SMMS_WaitingForResponse,
  Scc_SMMS_ProcessingResponse
#if ( defined SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW ) && ( SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW == STD_ON )
  , Scc_SMMS_WaitingForApplication
#endif /* SCC_ENABLE_STATEM_APPL_CONTROLLED_FLOW */
} Scc_StateM_MsgStateType;

typedef enum Scc_StateM_chargingControlType
{
  Scc_ChargingControl_None,
  Scc_ChargingControl_NegotiateChargingParameters,
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_ChargingControl_PreChargeCompleted,
#endif /* SCC_CHARGING_DC */
  Scc_ChargingControl_StartPowerDelivery,
#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )
  Scc_ChargingControl_StartWeldingDetection,
#endif /* SCC_CHARGING_DC */
#if ( defined SCC_SCHEMA_ISO ) && ( SCC_SCHEMA_ISO != 0u )
  Scc_ChargingControl_Renegotiation,
#endif /* SCC_SCHEMA_ISO */
  Scc_ChargingControl_StopCharging
} Scc_StateM_ChargingControlType;

typedef enum Scc_StateM_functionControlType
{
  Scc_FunctionControl_None,
  Scc_FunctionControl_ChargingMode,
  Scc_FunctionControl_Reset
} Scc_StateM_FunctionControlType;

typedef enum Scc_StateM_stateMachineErrorType
{
  Scc_SMER_NoError,
  Scc_SMER_Timer_IPAddressWaitTimer,
  Scc_SMER_Timer_SequencePerformanceTimer,
  Scc_SMER_Timer_CommunicationSetupTimer,
  Scc_SMER_Timer_ReadyToChargeTimer,
  Scc_SMER_Timer_AuthorizationOngoingTimer,
  Scc_SMER_Timer_ChargeParameterDiscoveryOngoingTimer,
  Scc_SMER_Timer_CableCheckOngoingTimer,
  Scc_SMER_Timer_PreChargeOngoingTimer,
  Scc_SMER_Initialization_ContractSelectionFailed,
  Scc_SMER_SECCDiscoveryProtocol_NoSharedSecurityOption,
  Scc_SMER_ServiceDiscovery_InvalidEnergyTransferMode,
  Scc_SMER_ServiceDiscovery_NoSharedPaymentOption,
  Scc_SMER_ServiceDiscovery_NoContractAndNoCertificateServiceAvailable,
  Scc_SMER_ServiceDetail_NoContractAndNoInstallationServiceAvailable,
  Scc_SMER_StackError
} Scc_StateM_StateMachineErrorType;

typedef enum Scc_StateM_stateMachineStatusType
{
  Scc_StateMachineStatus_None,
  Scc_StateMachineStatus_Initialized,
  Scc_StateMachineStatus_SLAC,
  Scc_StateMachineStatus_WaitForIP,
  Scc_StateMachineStatus_SECCDiscoveryProtocol,
  Scc_StateMachineStatus_TLConnection,
  Scc_StateMachineStatus_SupportedAppProtocol,
  Scc_StateMachineStatus_SessionSetup,
  Scc_StateMachineStatus_ServiceDiscovery,
  Scc_StateMachineStatus_ServiceDetail,
  Scc_StateMachineStatus_PaymentServiceSelection,
  Scc_StateMachineStatus_CertificateInstallation,
  Scc_StateMachineStatus_CertificateUpdate,
  Scc_StateMachineStatus_PaymentDetails,
  Scc_StateMachineStatus_Authorization,
  Scc_StateMachineStatus_ChargeParameterDiscovery,
  Scc_StateMachineStatus_CableCheck,
  Scc_StateMachineStatus_PreCharge,
  Scc_StateMachineStatus_PowerDelivery,
  Scc_StateMachineStatus_ChargingStatus,
  Scc_StateMachineStatus_CurrentDemand,
  Scc_StateMachineStatus_MeteringReceipt,
  Scc_StateMachineStatus_WeldingDetection,
  Scc_StateMachineStatus_SessionStop,
  Scc_StateMachineStatus_StopCommunicationSession,
  Scc_StateMachineStatus_Finished,
  Scc_StateMachineStatus_Error_WaitingForRetry,
  Scc_StateMachineStatus_Error_Stopped
} Scc_StateM_StateMachineStatusType;

typedef enum Scc_StateM_energyTransferModeType
{
  Scc_EnergyTransferMode_None          = 0x00u,
  Scc_EnergyTransferMode_AC_1P         = 0x01u,
  Scc_EnergyTransferMode_AC_3P         = 0x02u,
  Scc_EnergyTransferMode_DC_Core       = 0x04u,
  Scc_EnergyTransferMode_DC_Extended   = 0x08u,
  Scc_EnergyTransferMode_DC_Combo_Core = 0x10u,
  Scc_EnergyTransferMode_DC_Unique     = 0x20u
} Scc_StateM_EnergyTransferModeType;

typedef enum Scc_StateM_paymentPrioritizationType
{
  Scc_PaymentPrioritization_OnlyEIM,
  Scc_PaymentPrioritization_OnlyPnC,
  Scc_PaymentPrioritization_PrioritizePnC
} Scc_StateM_PaymentPrioritizationType;

typedef enum Scc_StateM_paymentOptionType
{
  Scc_PaymentOption_None,
  Scc_PaymentOption_EIM,
  Scc_PaymentOption_PnC
} Scc_StateM_PaymentOptionType;

typedef enum Scc_StateM_evseProcessingType
{
  Scc_EVSEProcessing_Ongoing,
  Scc_EVSEProcessing_Finished
} Scc_StateM_EVSEProcessingType;

typedef enum Scc_StateM_controlPilotStateType
{
  Scc_ControlPilotState_None,
  Scc_ControlPilotState_State_A_E_F,
  Scc_ControlPilotState_State_B,
  Scc_ControlPilotState_State_C_D
} Scc_StateM_ControlPilotStateType;

typedef enum Scc_StateM_pwmStateType
{
  Scc_PWMState_ChargingNotAllowed,
  Scc_PWMState_HLCOptional,
  Scc_PWMState_HLCOptional_BCActive,
  Scc_PWMState_HLCOnly
} Scc_StateM_PWMStateType;

typedef enum Scc_StateM_chargeStatusType
{
  Scc_ChargeStatus_Preparing,
  Scc_ChargeStatus_Starting,
  Scc_ChargeStatus_Renegotiating,
  Scc_ChargeStatus_Stopping
} Scc_StateM_ChargeStatusType;

#endif /* SCC_OEM_VECTOR */

typedef uint8 Scc_ConfigType;

/* PRQA L:EXT_IDENTIFIER_LENGTH */
/* PRQA L:IDENTIFIER_LENGTH */

#endif /* SCC_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: Scc_Types.h
 *********************************************************************************************************************/
