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
/**        \file  DoIP_Types.h
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Header file for type definitions of Diagnostic over IP
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if ( !defined(DOIP_TYPES_H) )
# define DOIP_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "DoIP_Cfg.h"
# include "SoAd.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define DOIP_INV_BUF_LEN                 0u
# define DOIP_UNUSED_PARAM                0u

# define DOIP_IDENT_RES_NUM               1u

# define DOIP_UHWID_LEN_BYTE              6u /* MAC address length */

# define DOIP_VTX_TCP_BUF_LEN_BYTE_TOTAL  21u

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if ( DOIP_VCHANNELS_TOTAL < 255u )
typedef uint8 DoIP_ChannelIdType;
#  define DOIP_INV_CHANNEL  0xFFu
# else
typedef uint16 DoIP_ChannelIdType;
#  define DOIP_INV_CHANNEL  0xFFFFu
# endif /* DOIP_VCHANNELS_TOTAL < 255u */

typedef struct
{
  DoIP_ChannelIdType Channel;
  PduInfoType        Msg;
  PduLengthType      TxHdrBufLen;
  uint8              TxHdrBuf[DOIP_VTX_TCP_BUF_LEN_BYTE_TOTAL];
} DoIP_TcpTxMgtType;

typedef struct
{
  uint16  HdrType;
  uint16  Param1;
  uint16  Param2;
  uint8   Param3;
  boolean CloseSocket;
} DoIP_TpTxPrepareType;

typedef struct
{
  PduLengthType                              MaxBufLen;
  CONSTP2VAR(uint8, TYPEDEF, DOIP_APPL_DATA) BufPtr;
} DoIP_IfTxBufType;

typedef struct
{
  SoAd_SockAddrInetXType RemoteAddr;
  uint32                 Param1;
  uint16                 Param2;
  uint16                 HdrType;
  uint16                 RetryAttempt;
  PduIdType              UdpCon;
} DoIP_UdpTxMgtType;

typedef struct
{
  P2CONST(uint8, TYPEDEF, DOIP_CONST) RoutActColPtr;
# if ( DOIP_NACK_APPEND_REQ == STD_ON )
  P2VAR(uint8, TYPEDEF, DOIP_CONST)   DiagAckNackBufPtr;
  uint32                              DiagAckNackNumByte;
# endif /* DOIP_NACK_APPEND_REQ == STD_ON */
  uint16                              TesterAddr;
  uint8                               RoutActNum;
  boolean                             CentralSecurity;
} DoIP_TesterType;

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_AuthFctPtrType)(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) Authentified,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) AuthenticationResData);

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_AuthWithRemAddrFctPtrType)(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) Authentified,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) AuthenticationResData,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, DOIP_APPL_VAR) RemAddrPtr);

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_ConfFctPtrType)(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) Confirmed,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ConfirmationResData);

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_ConfWithRemAddrFctPtrType)(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_VAR) Confirmed,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_VAR) ConfirmationResData,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, DOIP_APPL_VAR) RemAddrPtr);

typedef struct
{
  P2CONST(DoIP_ChannelIdType, TYPEDEF, DOIP_CONST) RxChannelColPtr;
  DoIP_AuthFctPtrType                              AuthFuncPtr;
  DoIP_ConfFctPtrType                              ConfFuncPtr;
  DoIP_ChannelIdType                               RxChannelNum;
# if ( DOIP_VROUT_ACT_AUTH_REM_ADDR_ENABLED == STD_ON )
  boolean                                          AuthFuncWithRemAddr;
# endif /* DOIP_VROUT_ACT_AUTH_REM_ADDR_ENABLED == STD_ON */
# if ( DOIP_VROUT_ACT_CONF_REM_ADDR_ENABLED == STD_ON )
  boolean                                          ConfFuncWithRemAddr;
# endif /* DOIP_VROUT_ACT_CONF_REM_ADDR_ENABLED == STD_ON */
  uint8                                            AuthReqLength;
  uint8                                            AuthResLength;
  uint8                                            ConfReqLength;
  uint8                                            ConfResLength;
  uint8                                            RoutingActivationNum;
} DoIP_RoutingActivationType;

typedef uint8 DoIP_StateType;

typedef uint8 DoIP_ActivationLineLocalType;
# define DOIP_ACTIVATION_LINE_LOCAL_INACTIVE           0u
# define DOIP_ACTIVATION_LINE_LOCAL_ACTIVE             1u
# define DOIP_ACTIVATION_LINE_LOCAL_INACTIVE_TO_ACTIVE 2u
# define DOIP_ACTIVATION_LINE_LOCAL_ACTIVE_TO_INACTIVE 3u

typedef uint8 DoIP_ActivationLineType;
# define DOIP_ACTIVATION_LINE_INACTIVE                 0u
# define DOIP_ACTIVATION_LINE_ACTIVE                   1u

typedef uint8 DoIP_UdpConnectionType;
# define DOIP_UDP_CON_TYPE_BROADCAST                   0u
# define DOIP_UDP_CON_TYPE_UNICAST                     1u

# define DOIP_E_PENDING                                0x10u

typedef uint8 DoIP_IpAddrReqStateType;
# define DOIP_IP_ADDR_REQ_STATE_NONE                   0x00u
# define DOIP_IP_ADDR_REQ_STATE_REQUESTED              0x01u
# define DOIP_IP_ADDR_REQ_STATE_ASSIGNED               0x02u
# define DOIP_IP_ADDR_REQ_STATE_RELEASE                0x04u
# define DOIP_IP_ADDR_REQ_STATE_REQUESTED_INV          0xFEu
# define DOIP_IP_ADDR_REQ_STATE_ASSIGNED_INV           0xFDu
# define DOIP_IP_ADDR_REQ_STATE_RELEASE_INV            0xFBu

typedef uint8 DoIP_RoutActivStateType;
# define DOIP_ROUT_ACTIV_STATE_NONE                    0u
# define DOIP_ROUT_ACTIV_STATE_AUTH_PENDING            1u
# define DOIP_ROUT_ACTIV_STATE_CONF_PENDING            2u
# define DOIP_ROUT_ACTIV_STATE_ACTIVATED               3u

typedef uint8 DoIP_IpAddrAssignmentType;
# define DOIP_IPADDR_ASSIGN_PATTERN_STATIC             1u
# define DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL_DOIP     2u
# define DOIP_IPADDR_ASSIGN_PATTERN_DHCP               4u
# define DOIP_IPADDR_ASSIGN_PATTERN_LINKLOCAL          8u
# define DOIP_IPADDR_ASSIGN_PATTERN_IPV6_ROUTER        16u

# define DOIP_IPADDR_ASSIGN_TRIGGER_AUTOMATIC          0u
# define DOIP_IPADDR_ASSIGN_TRIGGER_MANUAL             1u

typedef struct
{
  uint8 AssignIdx;
  uint8 AssignNum;
} DoIP_IpAddrAssignmentCollectionType;

typedef struct
{
  uint16                  TesterAddr;
  uint8                   TesterIdx;
  uint8                   RoutActIdx;
  DoIP_RoutActivStateType ActivState;
} DoIP_RoutActivSockMgtType;

typedef struct
{
  uint8     OemSpecReq[4u];
  uint8     OemSpecRes[4u];
  uint16    TesterAddr;
  uint16    AliveCheckCnt;
  PduIdType AliveCheckList[DOIP_VMAX_CONS_PER_LOC_ADDR];
  PduIdType TcpCon;
  uint8     AliveCheckElemNum;
  uint8     TesterIdx;
  uint8     RoutActIdx;
  boolean   OemSpecUsed;
} DoIP_RoutActivHandlerType;

typedef struct
{
  SoAd_SockAddrInetXType RemIpAddr;
  SoAd_SockAddrInetXType LocIpAddr;
} DoIP_RemLocIpAddrType;

typedef uint8 DoIP_PowerStateType; /* required by AUTOSAR but DoIP_PowerModeType would be a better */
typedef DoIP_PowerStateType DoIP_PowerModeType;
# define DOIP_POWER_MODE_NOT_READY                 0x00u
# define DOIP_POWER_MODE_READY                     0x01u
# define DOIP_POWER_MODE_NOT_SUPPORTED             0x02u

typedef uint8 DoIP_ApiType;
# define DOIP_API_TYPE_TP                          0x00u
# define DOIP_API_TYPE_IF                          0x01u

typedef uint8 DoIP_OemPayloadTypeFlagType;
# define DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_UDP       (uint8)0x00u
# define DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP       (uint8)0x01u
# define DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_NOT_ACTIV (uint8)0x00u
# define DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_ACTIV     (uint8)0x02u

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_OemPayloadTypeFctPtrType)(
  uint16 RxPayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) RxUserData,
  DoIP_OemPayloadTypeFlagType Flags,
  P2VAR(uint16, AUTOMATIC, DOIP_CONST) TxPayloadType,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_CONST) TxUserData);

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_VerifyTargetAddrFctPtrType)(
  uint16 TargetAddr);

typedef P2FUNC(Std_ReturnType, DOIP_CODE, DoIP_VerifyRxPduFctPtrType)(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, DOIP_CONST) LocalAddrPtr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, DOIP_CONST) RemoteAddrPtr,
  uint16 SourceAddr,
  uint16 TargetAddr,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr);

/**********************************************************************************************************************
  POST BUILD CONFIGURATION
**********************************************************************************************************************/

typedef uint8 DoIP_ConfigType;

#endif /* !defined(DOIP_TYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: DoIP_Types.h
 *********************************************************************************************************************/
