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
 *         \file  TcpIp_Types.h
 *        \brief  Implementation of TcpIp Module - Type Definitions
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#ifndef TCPIP_TYPES_H
#define TCPIP_TYPES_H

/**************************************************************************************************
* Include files
**************************************************************************************************/
#include "ComStack_Types.h"
#include "Std_Types.h"
#include "TcpIp_Cfg.h"
#include "IpBase_Types.h"

/**************************************************************************************************
* Global defines
**************************************************************************************************/
#define TCPIP_PHYS_ADDR_LEN_BYTE   6

#define TCPIP_LOCALADDRID_INV 0xFFU
#define TCPIP_LOCALADDRID_ANY 0xFEU /* Any IP address on any IPvX controller */

#define TCPIP_IP_ADDR_ID_ANY  0xFFU

#define TCPIP_NDP_ENTRY_STATIC  0x00U
#define TCPIP_NDP_ENTRY_VALID   0x01U
#define TCPIP_NDP_ENTRY_STALE   0x02U

/**************************************************************************************************
* Macros
**************************************************************************************************/
#define TCPIP_P2V(ptrtype) P2VAR(ptrtype, AUTOMATIC, TCPIP_APPL_VAR)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_P2C(ptrtype) P2CONST(ptrtype, AUTOMATIC, TCPIP_APPL_DATA)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**************************************************************************************************
* Global data types and structures
**************************************************************************************************/
/* new types for ASR 4.2.1 -- start ----------------------------- */

typedef uint8  TcpIp_SocketIdType;   /* uint8 and uint16 is allowed */
#define TCPIP_INV_SOCKET_ID      TCPIP_MAX_NUM_SOCKETS

/* start ENUMERATION types */
typedef uint16 TcpIp_DomainType;
#define TCPIP_AF_UNDEFINED       0x0000U
#define TCPIP_AF_INET            0x0002U  /* IPv4 */
#define TCPIP_AF_INET6           0x001CU  /* IPv6 */

typedef uint8 TcpIp_ProtocolType;  /* enum */
#define TCPIP_IPPROTO_TCP        0x06U
#define TCPIP_IPPROTO_UDP        0x11U

typedef IpBase_TcpIpEventType TcpIp_EventType;  /* uint8 */
#define TCPIP_TCP_RESET                  0x01U  /* TCP connection was reset, TCP socket and all related resources have been released. */
#define TCPIP_TCP_CLOSED                 0x02U  /* TCP connection was closed successfully, TCP socket and all related resources have been released. */
#define TCPIP_TCP_FIN_RECEIVED           0x03U  /* A FIN signal was received on the TCP connection, TCP socket is still valid. */
#define TCPIP_UDP_CLOSED                 0x04U  /* UDP socket and all related resources have been released. */
#define TCPIP_TLS_HEARTBEAT_NO_RESPONSE  0x05u  /* Vector ext: TLS heartbeat response has not been received in time. */
#define TCPIP_TCP_CONNECTED              0x06U  /* Vector ext: TCP client connection to an external server was established successfully. (only used by TLS) */
#define TCPIP_TCP_ACCEPTED               0x07U  /* Vector ext: TCP server has accepted a connection from an external client. (only used by TLS) */

typedef uint8 TcpIp_ReturnType;
#define TCPIP_OK                 0x00U  /* operation completed successfully */
#define TCPIP_E_NOT_OK           0x01U  /* operation failed */
#define TCPIP_E_PHYS_ADDR_MISS   0x02U  /* operation failed because of an ARP cache miss */

typedef uint8 TcpIp_ParamIdType;
#define TCPIP_PARAMID_TCP_RXWND_MAX             0x00U  /* uint32 */ /* Specifies the maximum TCP receive window for the socket. */
#define TCPIP_PARAMID_FRAMEPRIO                 0x01U  /* uint8 */  /* Specifies the frame priority for outgoing frames on the socket. */
#define TCPIP_PARAMID_TCP_NAGLE                 0x02U  /* uint8 */  /* Specifies if the Nagle Algorithm according to IETF RFC 896 is enabled or not. */
#define TCPIP_PARAMID_TCP_KEEPALIVE             0x03U  /* uint8 */ /* Specifies if TCP Keep Alive Probes are sent on the socket connection. (default = false) */
#define TCPIP_PARAMID_TTL                       0x04U  /* uint8 */  /* Specifies the time to live value for outgoing frames on the socket. */
#define TCPIP_PARAMID_TCP_KEEPALIVE_TIME        0x05U  /* uint32 */ /* Specifies the time in [s] between the last data packet sent (simple ACKs are not considered data) and the first keepalive probe. (0..INV, default = 7200) */
#define TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX  0x06U  /* uint16 */ /* Specifies the maximum number of times that a keepalive probe is retransmitted. (0..65535) */
#define TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL    0x07U  /* uint32 */ /* Specifies the interval in [s] between subsequent keepalive probes. (0..INV) */
/* TCPIP_PARAMID_VENDOR_SPECIFIC             0x80U     Start of vendor specific range of parameter IDs. */
#define TCPIP_PARAMID_V_TCP_UTOUPDATE           0x80U  /* uint16 */ /* new value for user timerout */
#define TCPIP_PARAMID_V_TCP_UTOENABLED          0x81U  /* BOOL */   /* enable usage of user timerout option */
#define TCPIP_PARAMID_V_TCP_UTOCHANGEABLE       0x82U  /* BOOL */   /* allow change of user timerout by received option */
#define TCPIP_PARAMID_V_TCP_TXBUFSIZE           0x83U  /* uint32 */ /* Specifies the TCP tx buffer size (to choose the matching buffer) */
#define TCPIP_PARAMID_V_IPV4_TYPE_OF_SERVICE    0x84U  /* uint8 */  /* specify the 'type of service' value used for sending IPv4 messages */
#define TCPIP_PARAMID_V_IPV6_FLOW_LABEL         0x85U  /* uint32 */ /* specify the 'flow label' value used for sending IPv6 messages */
#define TCPIP_PARAMID_V_IPV6_TRAFFIC_CLASS      0x86U  /* uint8 */  /* specify the 'traffic class' value used for sending IPv6 messages */
#define TCPIP_PARAMID_V_UDP_TXREQLISTSIZE       0x87U  /* uint8 */  /* choose the desired TxRequestList size */
#define TCPIP_PARAMID_V_UDP_TXRETRYLISTSIZE     0x88U  /* uint8 */  /* set the number of Eth tx buffers used in case of PhysAddrMiss */
#define TCPIP_PARAMID_V_PHYS_DEST_ADDR          0x89U  /* uint8[6] */ /* set physical destination address for outgoing packets (override ARP). */

#define TCPIP_PARAMID_V_USE_TLS                 0x8AU  /* uint8 */  /* Enable / disable the usage of TLS for the (TCP) socket */

#define TCPIP_PARAMID_V_TLS_TXBUFSIZE           0xC0U  /* uint32 */ /* Specifies the TLS tx buffer size */
#define TCPIP_PARAMID_V_TLS_RXBUFSIZE           0xC1U  /* uint32 */ /* Specifies the TLS rx buffer size */
#define TCPIP_PARAMID_V_TLS_SELECT_OCSP_REQEST  0xC2U  /* uint8 */  /* select OCSP requests for the connection */
#define TCPIP_PARAMID_V_TLS_HEARTBEAT_MODE      0xC3U  /* uint8 */  /* select heartbeat mode for the connection */
#define TCPIP_PARAMID_V_TLS_HEARTBEAT_PERIOD_S  0xC4U  /* uint32 */ /* specify heartbeat period for the connection */

/* define use in callback TcpIp_SocketOwnerTlsValidationResult */
#define TCPIP_TLS_VALIDATION_OK                 0x00u  /* The certificate chain is validated and ok, or the socket owner accepts the connection explicitly */
#define TCPIP_TLS_VALIDATION_UNKNOWN_CA         0x01u  /* The cert chain could not be validated because of an unknown CA */
#define TCPIP_TLS_VALIDATION_LAST_SIGN_INVALID  0x02u  /* The signature of the last cert chain element is invalid */
#define TCPIP_TLS_VALIDATION_INT_SIGN_INVALID   0x03u  /* The signature of an intermediate element of the cert chain is invalid */
#define TCPIP_TLS_VALIDATION_REFUSED_BY_OWNER   0x04u  /* The socket owner refused the connection explicitly */

/* new types for ASR 4.2.1 -- end ----------------------------- */

typedef uint32         TcpIp_NetAddrType;
typedef uint32         TcpIp_DataType;

typedef uint8          TcpIp_SockHndType;

typedef IpBase_SockIdxType  TcpIp_SockIdxType;

typedef uint8          TcpIp_LocalAddrIdType;

/* Values of TcpIp_IpAddrStateType */
#define TCPIP_IPADDR_STATE_ASSIGNED   0U
#define TCPIP_IPADDR_STATE_ONHOLD     1U
#define TCPIP_IPADDR_STATE_UNASSIGNED 2U
typedef uint8          TcpIp_IpAddrStateType;

typedef uint16      TcpIp_TcpIpErrorType;

typedef struct
{
    TcpIp_DomainType   domain;
} TcpIp_SockAddrType;
typedef struct
{
    TcpIp_DomainType   domain;
    uint16             port;
    uint32             addr[1];
} TcpIp_SockAddrInetType;
typedef struct
{
    TcpIp_DomainType   domain;
    uint16             port;
    uint32             addr[4];
} TcpIp_SockAddrInet6Type;

/* ----- */
typedef uint8          TcpIp_SockUserIdxType;

typedef struct
{
  uint8              ConnStat;   /* connection status */
} TcpIp_SockConnStatusType;  /* connection status type */

typedef uint8          TcpIp_StateType;  /* module state */
/* ----- */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
typedef IpBase_SockAddrIn6Type TcpIp_SockAddrInXType;
#else
typedef IpBase_SockAddrInType TcpIp_SockAddrInXType;
#endif

typedef struct
{
  TcpIp_SockAddrInXType  RemSock;     /* remote IP address, port, family-type */
  TcpIp_SockAddrInXType  LocSock;     /* local IP address, port, family-type */
  TcpIp_LocalAddrIdType  BindLocalAddrId;
  uint8                  TxIpAddrId;

  boolean                IsTcpListenSocket;  /* Only valid for TCP sockets. Predefined for some sockets (sockets with highest ID).
                                                Default is 'FALSE'. Sockets that are derived from a listen socket also are set to 'FALSE'. */
  TcpIp_SocketIdType     TcpMasterListenSockId;  /* Id of the listen socket that opened this socket. Value is set to 'INV_SOCK_ID' if not used. */
  uint16                 TcpMaxNumListenSockets;  /* Maximum number of new parallel connections established on this listen connection. */
} TcpIp_SockType;

typedef void (*TcpIp_SocketOwnerRxIndicationType)(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(TcpIp_SockAddrType) RemoteAddrPtr,
  TCPIP_P2V(uint8)              BufPtr,
  uint16                        Length);

typedef void (*TcpIp_SocketOwnerTxConfirmationType)(
  TcpIp_SocketIdType            SocketId,
  uint16                        Length);

typedef Std_ReturnType (*TcpIp_SocketOwnerTcpAcceptedType)(
  TcpIp_SocketIdType            SocketId,
  TcpIp_SocketIdType            SocketIdConnected,
  TCPIP_P2V(TcpIp_SockAddrType) RemoteAddrPtr);

typedef void (*TcpIp_SocketOwnerTcpConnectedType)(
  TcpIp_SocketIdType            SocketId);

typedef void (*TcpIp_SocketOwnerTcpIpEventType)(
  TcpIp_SocketIdType            SocketId,
  IpBase_TcpIpEventType         Event);

typedef void (*TcpIp_SocketOwnerTlsValidationResultType)(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(uint8)              ValidationResultPtr,
  TCPIP_P2C(uint8)              CertChainPtr,
  uint32                        CertChainLen);

typedef BufReq_ReturnType (*TcpIp_SocketOwnerCopyTxDataType)(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(uint8)              BufPtr,
  uint16                        BufLength);

typedef BufReq_ReturnType (*TcpIp_SocketOwnerCopyTxDataDynType)(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(uint8)              BufPtr,
  TCPIP_P2V(uint16)             BufLengthPtr);

typedef void (*TcpIp_SocketOwnerLocalIpAddrAssignmentChgType)(
  TcpIp_LocalAddrIdType         LocalAddrId,
  TcpIp_IpAddrStateType         State);

typedef void (*TcpIp_PhysAddrTableChgCbkType)(
  uint8                         CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,
  TCPIP_P2V(uint8)              PhysAddrPtr,
  boolean                       Valid);

typedef void (*TcpIp_DuplicateAddrDetectionCbkType)(
  TcpIp_LocalAddrIdType         IpAddrId,
  TCPIP_P2C(TcpIp_SockAddrType) IpAddrPtr,
  TCPIP_P2C(uint8)              LocalPhysAddrPtr,
  TCPIP_P2C(uint8)              RemotePhysAddrPtr);

typedef void (*TcpIp_IcmpMsgHandlerCbkType)(
  TcpIp_LocalAddrIdType         LocalAddrId,
  TCPIP_P2C(TcpIp_SockAddrType) RemoteAddrPtr,
  uint8                         Ttl,
  uint8                         Type,
  uint8                         Code,
  uint16                        DataLength,
  TCPIP_P2V(uint8)              DataPtr);

typedef void (*TcpIp_IcmpMultiPartMsgHandlerCbkType)(
  TcpIp_LocalAddrIdType         LocalAddrId,
  TCPIP_P2C(TcpIp_SockAddrType) RemoteAddrPtr,
  uint8                         Ttl,
  uint8                         Type,
  uint8                         Code,
  uint16                        DataLength,
  TCPIP_P2V(uint8)              DataPtr,
  uint16                        MultiPartDataLength,
  TCPIP_P2V(uint8)              MultiPartDataPtr);

typedef struct 
{
  TcpIp_SocketOwnerRxIndicationType          RxIndicationFuncPtr;
  TcpIp_SocketOwnerTxConfirmationType        TxConfirmationFuncPtr;
  TcpIp_SocketOwnerTcpAcceptedType           TcpAcceptedFuncPtr;
  TcpIp_SocketOwnerTcpConnectedType          TcpConnectedFuncPtr;
  TcpIp_SocketOwnerTcpIpEventType            TcpIpEventFuncPtr;
  TcpIp_SocketOwnerTlsValidationResultType   TlsValidationResultFuncPtr;
  TcpIp_SocketOwnerCopyTxDataType            CopyTxDataFuncPtr;
  TcpIp_SocketOwnerCopyTxDataDynType         CopyTxDataDynFuncPtr;
} TcpIp_SocketOwnerCfgType;

typedef struct
{
  IpBase_PortType SrcPort;
  IpBase_PortType TgtPort;
  uint16          LenByte;
  uint16          Checksum;
} TcpIp_Udp_HdrType;

#if defined (TCPIP_CONFIGURATION_VARIANT) && (TCPIP_CONFIGURATION_VARIANT != TCPIP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
typedef uint8      TcpIp_ConfigType;  /* a real config type is only needed for postbuild variant */
#endif

#if (STD_ON != TCPIP_SUPPORT_SOAD)
typedef IpBase_PbufType        SoAd_TcpIpPbufType;
#endif
typedef IpBase_PbufType        TcpIp_PbufType;

typedef uint8 TcpIp_DhcpV4_OptSelectorType;

typedef uint32 TcpIp_DhcpV6_OptSelectorType;

#define TCPIP_DHCPV4_OPT_ID_CLIENT_FQDN        81u
#define TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN        39u

/* Specification of IP address assignment policy. */
typedef uint8 TcpIp_IpAddrAssignmentType;
#define TCPIP_IPADDR_ASSIGNMENT_STATIC           0u  /* Static configured IPv4/IPv6 address */
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP   1u  /* Linklocal IPv4/IPv6 address assignment using DoIP parameters. */
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL        1u  /* Linklocal IPv4/IPv6 address assignment. */
#define TCPIP_IPADDR_ASSIGNMENT_DHCP             2u  /* Dynamic configured IPv4/IPv6 address by DHCP. */
#define TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER      3u  /* Dynamic configured IPv4/IPv6 address by Router Advertisement. */
#define TCPIP_IPADDR_ASSIGNMENT_ANY           0xFFu  /* Specifies ANY address assignment type for use with Vector specific API TcpIp_ReleaseSpecificIpAddrAssignment(). */

/* Parameter IDs for diagnostic read access */
typedef uint32 TcpIp_DiagParamsType;
#define TCPIP_DP_SOCKET_LOCAL_PORT               0x00U
#define TCPIP_DP_SOCKET_TCP_CURRENT_ACK_NUM      0x01U
#define TCPIP_DP_SOCKET_TCP_NEXT_SEQ_NUM         0x02U
#define TCPIP_DP_INVALID                         0x03U  /* this id is the highest supported parameter id */

#if (TCPIP_ENABLE_DLISTS == STD_ON)

typedef uint8 TcpIp_DListIdxType;

/* DList element (array of elements is managed by descriptor) */
typedef struct
{
  TcpIp_DListIdxType             PrevIdx; /* index of the predecessor */
  TcpIp_DListIdxType             NextIdx; /* index of the successor */
} TcpIp_DListNodeType;

/* DList descriptor (can be shared by multiple instances) */
typedef struct
{
  TcpIp_DListIdxType             FirstFreeIdx; /* index of first element in free list */
  TcpIp_DListIdxType             LastFreeIdx;  /* index of last element in free list */
  uint8                          ElementCount; /* number of elements in free list */
  TCPIP_P2V(TcpIp_DListNodeType) NodePtr;      /* pointer to element array of this list */
} TcpIp_DListDescType;

/* DList instance */
typedef struct
{
  TcpIp_DListIdxType             FirstIdx;     /* index of first element in list */
  TcpIp_DListIdxType             LastIdx;      /* index of last element in list */
  uint8                          ElementCount; /* number of elements in list */
  TCPIP_P2V(TcpIp_DListDescType) DescPtr;      /* pointer to list descriptor of the list */
} TcpIp_DListType;

#endif

typedef struct
{
  uint32  DestAddr;         /*<! IP destination address */
  uint8  *EthPayloadPtr;    /*<! Pointer to ethernet payload */
  uint16  EthPayloadLen;    /*<! Length of ethernet payload (buffer) in bytes */
  uint16  IpPayloadOfs;     /*<! Offset of IP payload (length of IP header) inside ethernet payload in bytes */
  uint8   BufIdx;           /*<! Index of the first ethernet buffer for the IP packet. */
  boolean IsEthBuf;         /*<! Indicates whether BufIdx refers to an EthIf buffer or an internal IPv6 fragment buffer */
  uint8   IpAddrId;         /*<! IP address identifier of the address that will be used as source address */
  uint8   CtrlIdx;          /*<! Index of the EthIf controller */
  uint8   CurBufIdx;        /*<! Index of the currently active ethernet buffer. Only used for IPv4 fragmentation. */
  uint8   SockIdx;          /*<! Index of the TcpIp socket this request belongs to. */
} TcpIp_IpTxRequestDescriptorType;

typedef struct
{
  TcpIp_IpTxRequestDescriptorType IpTxRequestDesc;  /* IP TX request descriptor */
  TcpIp_SockAddrInXType           Destination;      /* IP destination address of the pending packet */
  uint16                          UdpPayloadLen;    /* length of the UDP payload in bytes */
  uint8                           UdpTxReqTabIdx;   /* UDP tx request table index (used for TxConfirmation) */
} TcpIp_Udp_RetryQueueElementType;  /* Used for UDP tx chache in case of PhysAddrMiss */

/* Struct for function TcpIp_GetArpCacheEntries */
typedef struct
{
  uint32   InetAddr[1];   /* IpV4 address in Network byte order */
  uint8    PhysAddr[6];   /* Physical address in Network byte order */
  uint8    State;         /* State of the address Entry */
} TcpIp_ArpCacheEntryType; 

#define TCPIP_ARP_ENTRY_STATIC  0u
#define TCPIP_ARP_ENTRY_VALID   1u
#define TCPIP_ARP_ENTRY_STALE   2u

typedef struct
{
  uint32 Inet6Addr[4]; /* IPv6 address in network byte order */
  uint8  PhysAddr[6];  /* physical address in network byte order */
  uint8  State;        /* state of the address entry */
} TcpIp_NdpCacheEntryType;

#endif

  /* TCPIP_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Types.h
 *********************************************************************************************************************/
