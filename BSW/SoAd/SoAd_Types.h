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
/**        \file  SoAd_Types.h
 *        \brief  Socket Adaptor types header file
 *
 *      \details  Vector static types header file for AUTOSAR Socket Adaptor module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/


#if !defined(SOAD_TYPES_H)
# define SOAD_TYPES_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd_Cfg.h"
# include "ComStack_Types.h"
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
# else
#  include "TcpIp_Types.h"
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES DEFINES
 *********************************************************************************************************************/
/*! Physical address length. */
# define SOAD_UHWID_LEN_BYTE              TCPIP_PHYS_ADDR_LEN_BYTE

/*! IPv4 address length. */
# define SOAD_IPV4_ADDR_LEN_BYTE          (4u)
/*! IPv6 address length. */
# define SOAD_IPV6_ADDR_LEN_BYTE          (16u)

/*! Invalid index for array of type uint8. */
# define SOAD_INV_BUF_IDX_8               (0xFFu)
/*! Invalid index for array of type uint16. */
# define SOAD_INV_BUF_IDX_16              (0xFFFFu)

/*! Invalid handle for handles of type uint8. */
# define SOAD_INV_HND_8                   (0xFFu)
/*! Invalid handle for handles of type uint16. */
# define SOAD_INV_HND_16                  (0xFFFFu)

/*! Invalid value to indicate that no frame priority is configured. */
# define SOAD_INV_FRAME_PRIORITY          (0xFFu)
/*! Invalid value to indicate that no keep alive is configured. */
# define SOAD_INV_KEEP_ALIVE_VALUE        (0xFFFFFFFFu)

/*! Maximum value for type uint32. */
# define SOAD_MAX_UINT_32                 (0xFFFFFFFFu)
/*! Maximum value for type uint32 used for uint32 timeouts. */
# define SOAD_MAX_TIMEOUT_32              (0xFFFFFFFFu)

/*! Indicates that no close is requested for a socket connection. */
# define SOAD_CLOSE_NONE                  (0x00u)
/*! Indicates that a socket connection is requested to be closed and set to reconnect. */
# define SOAD_CLOSE_RECONNECT             (0x01u)
/*! Indicates that a socket connection is requested to be closed and set to offline. */
# define SOAD_CLOSE_OFFLINE               (0x02u)
/*! Indicates that a socket connection is requested to be closed, set to offline and reset connection parameters. */
# define SOAD_CLOSE_OFFLINE_RESET         (0x03u)
/*! Indicates that a socket connection is requested to be closed by a remote entity. */
# define SOAD_CLOSE_REMOTE                (0x04u)

/*! Indicates that the change state function of user for IP address assignment shall be called. */
# define SOAD_CHG_FUNC_TYPE_IPASSIGN      (0x00u)
/*! Indicates that the change state function of user for socket connection mode shall be called. */
# define SOAD_CHG_FUNC_TYPE_SOCONMODE     (0x01u)

/*! Bit mask to check if a call to TxConfirmation of SoAd is pending. */
# define SOAD_TX_CONF_STRAT_BIT_PAT_CONF  (0x7FFFu) /* b 0111 1111 1111 1111 */
/*! Bit mask to check if SoAd shall call a TxConfirmation in own main function comtext. */
# define SOAD_TX_CONF_STRAT_BIT_PAT_MAIN  (0x8000u) /* b 1000 0000 0000 0000 */

/* ----- Pointer types ----- */
/* PRQA S 3453 4 */ /* MD_MSR_19.7 */  /* PRQA S 0850 4 */ /* MD_MSR_19.8 */
# define SOAD_P2CONST(Type)               P2CONST     (Type, AUTOMATIC, SOAD_APPL_DATA)
# define SOAD_P2VAR(Type)                 P2VAR       (Type, AUTOMATIC, SOAD_APPL_VAR)
# define SOAD_CONSTP2VAR(Type)            CONSTP2VAR  (Type, AUTOMATIC, SOAD_APPL_VAR)
# define SOAD_CONSTP2CONST(Type)          CONSTP2CONST(Type, AUTOMATIC, SOAD_CONST)

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Negative return value in case of BSD socket API. */
#  define SOAD_E_BSD_NOT_OK              (-1)
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Module internal return type used for Rx PDU verification. */
typedef uint8             SoAd_ReturnType;
# define SOAD_E_OK        (0u)    /*!< Rx PDU verification succeeded. */
# define SOAD_E_NOT_OK    (1u)    /*!< Rx PDU verification failed. */
# define SOAD_E_PENDING   (2u)    /*!< Rx PDU verification pending. */

/*! Single linear buffer segment used to handle one segment of TcpIp reception ring buffer. */
typedef struct
{
  uint8* DataPtr;                 /*!< Pointer to buffer segment. */
  uint16 Len;                     /*!< Length of buffer segment. */
} SoAd_BufSegType;

/*! Managment struct to handle TcpIp reception ring buffer. */
typedef struct
{
  SoAd_BufSegType BufSegs[SOAD_VBUF_SEG_SIZE_TOTAL];  /*!< Array of buffer segments. */
  uint16          Idx;                                /*!< Index of first segment. */
  uint16          Lvl;                                /*!< Level of used segments. */
} SoAd_BufSegMgtType;

/*! Unspecified socket address type. */
# define SOAD_AF_UNSPEC             (0x0000u)
/*! Invalid socket address type. */
# define SOAD_AF_INVALID            (0xFFFFu)
/*! Any/wildcard value for IPv4 address. */
# define SOAD_IPADDR_ANY            (0x00000000u)
/*! Any/wildcard value for IPv6 address. */
# define SOAD_IP6ADDR_ANY           (0x00000000u)

/*! Socket port type. */
typedef uint16                      SoAd_PortType;
# define SOAD_PORT_ANY              (0x0000u)         /*!< Any/wildcard value for port. */
# define SOAD_INV_PORT              (0xFFFFu)         /*!< Invalid value for port. */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Socket identifier type for BSD Socket API. */
typedef sint32                      SoAd_SocketIdType;
#  define SOAD_INV_SOCKET_ID        (-1)                        /*!< Invalid socket identifier. */
# else
/*! Socket identifier type for ASR Socket API. */
typedef TcpIp_SocketIdType          SoAd_SocketIdType;
#  define SOAD_INV_SOCKET_ID        ((TcpIp_SocketIdType)(-1))  /*!< Invalid socket identifier. */
# endif /* defined(SOAD_VSOCKET_API) && (STD_ON == SOAD_VSOCKET_API) */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Indicates that no flags shall be set in case of transmission over BSD Socket API. */
#  define SOAD_ZERO_FLAGS           (0x00)
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Socket event type defined for BSD Socket API. */
typedef uint8                       SoAd_EventType;
#  define SOAD_TCP_RESET            (0x01u)                   /*!< TCP socket connection reset by remote entity. */
#  define SOAD_TCP_CLOSED           (0x02u)                   /*!< TCP socket connection closed. */
#  define SOAD_TCP_FIN_RECEIVED     (0x03u)                   /*!< TCP socket connection received a FIN. */
#  define SOAD_UDP_CLOSED           (0x04u)                   /*!< UDP socket closed. */
# else
/*! Socket event type defined for ASR Socket API. */
typedef TcpIp_EventType             SoAd_EventType;
#  define SOAD_TCP_RESET            (TCPIP_TCP_RESET)         /*!< TCP socket connection reset by remote entity. */
#  define SOAD_TCP_CLOSED           (TCPIP_TCP_CLOSED)        /*!< TCP socket connection closed. */
#  define SOAD_TCP_FIN_RECEIVED     (TCPIP_TCP_FIN_RECEIVED)  /*!< TCP socket connection received a FIN. */
#  define SOAD_UDP_CLOSED           (TCPIP_UDP_CLOSED)        /*!< UDP socket closed. */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Socket domain type defined for BSD Socket API. */
typedef uint16                      SoAd_DomainType;
#  define SOAD_AF_INET              (0x02u)                   /*!< Domain type for IPv4. */
#  define SOAD_AF_INET6             (0x1Cu)                   /*!< Domain type for IPv6. */
# else
/*! Socket domain type defined for ASR Socket API. */
typedef TcpIp_DomainType            SoAd_DomainType;
#  define SOAD_AF_INET              (TCPIP_AF_INET)           /*!< Domain type for IPv4. */
#  define SOAD_AF_INET6             (TCPIP_AF_INET6)          /*!< Domain type for IPv6. */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Socket protocol type defined for BSD Socket API. */
typedef uint8                       SoAd_ProtocolType;
#  define SOAD_IPPROTO_TCP          (0x06u)                   /*!< Protocol type for TCP. */
#  define SOAD_IPPROTO_UDP          (0x11u)                   /*!< Protocol type for UDP. */
# else
/*! Socket protocol type defined for ASR Socket API. */
typedef TcpIp_ProtocolType          SoAd_ProtocolType;
#  define SOAD_IPPROTO_TCP          (TCPIP_IPPROTO_TCP)       /*!< Protocol type for TCP. */
#  define SOAD_IPPROTO_UDP          (TCPIP_IPPROTO_UDP)       /*!< Protocol type for UDP. */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! IP address state type defined for BSD Socket API. */
typedef uint8                           SoAd_IpAddrStateType;
#  define SOAD_IPADDR_STATE_ASSIGNED    (0x00u)                         /*!< IP address is assigned. */
#  define SOAD_IPADDR_STATE_ONHOLD      (0x01u)                         /*!< IP address is not (yet) available. */
#  define SOAD_IPADDR_STATE_UNASSIGNED  (0x02u)                         /*!< IP address is unassigned. */
# else
/*! IP address state type defined for ASR Socket API. */
typedef TcpIp_IpAddrStateType           SoAd_IpAddrStateType;
#  define SOAD_IPADDR_STATE_ASSIGNED    (TCPIP_IPADDR_STATE_ASSIGNED)   /*!< IP address is assigned. */
#  define SOAD_IPADDR_STATE_ONHOLD      (TCPIP_IPADDR_STATE_ONHOLD)     /*!< IP address is not (yet) available. */
#  define SOAD_IPADDR_STATE_UNASSIGNED  (TCPIP_IPADDR_STATE_UNASSIGNED) /*!< IP address is unassigned. */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Local address identifier type for BSD Socket API. */
typedef uint8                         SoAd_LocalAddrIdType;
/*! Local address identifier type for BSD Socket API used for loop iterator. */
typedef uint8_least                   SoAd_LocalAddrIdType_least;
#  define SOAD_IPADDRID_ANY           (0xFEU)                 /*!< Any/wildcard local address identifier. */
# else
/*! Local address identifier type for ASR Socket API. */
typedef TcpIp_LocalAddrIdType         SoAd_LocalAddrIdType;
#  define SOAD_IPADDRID_ANY           (TCPIP_LOCALADDRID_ANY) /*!< Any/wildcard local address identifier. */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Parameter identifier type to change socket specific parameter for BSD Socket API. */
typedef uint8                                   SoAd_ParamIdType;
/*! TCP Rx buffer size. */
#  define SOAD_PARAMID_TCP_RXWND_MAX            (0x00u)
/*! TCP Tx buffer size. */
#  define SOAD_PARAMID_TCP_TXWND_MAX            (0x01u)
/*! Indicates if keep alive enabled. */
#  define SOAD_PARAMID_TCP_KEEPALIVE            (0x02u)
/*! Keep alive time. */
#  define SOAD_PARAMID_TCP_KEEPALIVE_TIME       (0x03u)
/*! Number of keep alive probes. */
#  define SOAD_PARAMID_TCP_KEEPALIVE_PROBES_MAX (0x04u)
/*! Interval time of keep alive probes. */
#  define SOAD_PARAMID_TCP_KEEPALIVE_INTERVAL   (0x05u)
/*! Indicates if nagle algorithm enabled. */
#  define SOAD_PARAMID_TCP_NAGLE                (0x06u)
/*! Frame priority (VLAN). */
#  define SOAD_PARAMID_FRAMEPRIO                (0x07u)
# else
/*! Parameter identifier type to change socket specific parameter for ASR Socket API. */
typedef TcpIp_ParamIdType                       SoAd_ParamIdType;
/*! TCP Rx buffer size. */
#  define SOAD_PARAMID_TCP_RXWND_MAX            (TCPIP_PARAMID_TCP_RXWND_MAX)
/*! Frame priority (VLAN). */
#  define SOAD_PARAMID_FRAMEPRIO                (TCPIP_PARAMID_FRAMEPRIO)
/*! Indicates if nagle algorithm enabled. */
#  define SOAD_PARAMID_TCP_NAGLE                (TCPIP_PARAMID_TCP_NAGLE)
/*! Indicates if keep alive enabled. */
#  define SOAD_PARAMID_TCP_KEEPALIVE            (TCPIP_PARAMID_TCP_KEEPALIVE)
/*! Time to live. */
#  define SOAD_PARAMID_TTL                      (TCPIP_PARAMID_TTL)
/*! Keep alive time. */
#  define SOAD_PARAMID_TCP_KEEPALIVE_TIME       (TCPIP_PARAMID_TCP_KEEPALIVE_TIME)
/*! Number of keep alive probes. */
#  define SOAD_PARAMID_TCP_KEEPALIVE_PROBES_MAX (TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX)
/*! Interval time of keep alive probes. */
#  define SOAD_PARAMID_TCP_KEEPALIVE_INTERVAL   (TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL)
#  if ( defined(SOAD_VTCP_TX_BUF_MIN_ENABLED) && (STD_ON == SOAD_VTCP_TX_BUF_MIN_ENABLED) )
/*! TCP Tx buffer size. */
#   define SOAD_PARAMID_TCP_TXWND_MAX           (TCPIP_PARAMID_V_TCP_TXBUFSIZE)
#  endif /* defined(SOAD_VTCP_TX_BUF_MIN_ENABLED) && (STD_ON == SOAD_VTCP_TX_BUF_MIN_ENABLED) */
#  if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
/*! UDP Tx request list size to store transmissions if TxConfirmation is required. */
#   define SOAD_PARAMID_UDP_TXREQLISTSIZE       (TCPIP_PARAMID_V_UDP_TXREQLISTSIZE)
#  endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
#  if ( SOAD_VUDP_RETRY_ENABLED == STD_ON )
/*! UDP Tx retry list size to store transmissions if address resoltion (e.g. ARP) is not yet finished. */
#   define SOAD_PARAMID_UDP_TXRETRYLISTSIZE     (TCPIP_PARAMID_V_UDP_TXRETRYLISTSIZE)
#  endif /* SOAD_VUDP_RETRY_ENABLED == STD_ON */
/*! Indicates if TCP shall be used. */
#  define SOAD_PARAMID_USE_TLS                  (TCPIP_PARAMID_V_USE_TLS)
/*! TLS Tx buffer size. */
#  define SOAD_PARAMID_TLS_TXBUFSIZE            (TCPIP_PARAMID_V_TLS_TXBUFSIZE)
/*! TLS Rx buffer size. */
#  define SOAD_PARAMID_TLS_RXBUFSIZE            (TCPIP_PARAMID_V_TLS_RXBUFSIZE)
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
/*! TcpIp stack specific parameter used by SoAd to set parameter which has to be set for a specific stack. */
# define SOAD_PARAMID_STACK_SPECIFIC            (0xFEu)

/* TcpIp IP address assignment */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Parameter identifier type to change socket specific parameter for BSD Socket API. */
typedef uint8                                   SoAd_IpAddrAssignmentType;
/*! Static IP address. */
#  define SOAD_IPADDR_ASSIGNMENT_STATIC         (0x01u)
/*! Link local IP address with parameters for DoIP use-case. */
#  define SOAD_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP (0x02u)
/*! DHCP IP address. */
#  define SOAD_IPADDR_ASSIGNMENT_DHCP           (0x03u)
/*! Link local IP address. */
#  define SOAD_IPADDR_ASSIGNMENT_LINKLOCAL      (0x04u)
/*! Dynamic configured IPv6 address by Router Advertisement. */
#  define SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER    (0x05u)
# else
/*! Parameter identifier type to change socket specific parameter for ASR Socket API. */
typedef TcpIp_IpAddrAssignmentType              SoAd_IpAddrAssignmentType;
/*! Static IP address. */
#  define SOAD_IPADDR_ASSIGNMENT_STATIC         (TCPIP_IPADDR_ASSIGNMENT_STATIC)
/*! Link local IP address with parameters for DoIP use-case. */
#  define SOAD_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP (TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP)
/*! DHCP IP address. */
#  define SOAD_IPADDR_ASSIGNMENT_DHCP           (TCPIP_IPADDR_ASSIGNMENT_DHCP)
/*! Link local IP address. */
#  define SOAD_IPADDR_ASSIGNMENT_LINKLOCAL      (TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL)
/*! Dynamic configured IPv6 address by Router Advertisement. */
#  define SOAD_IPADDR_ASSIGNMENT_IPV6_ROUTER    (TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER)
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

/*! DHCP hostname option for IPv4. */
# define SOAD_DHCP_OPT_V4_HOSTNAME               (81u)
/*! DHCP hostname option for IPv6. */
# define SOAD_DHCP_OPT_V6_HOSTNAME               (39u)

/*! Indicates if local address is unicast or multicast. */
typedef uint8                                     SoAd_AddressTypeType;
# define SOAD_ADDRESS_TYPE_UNICAST                (0x00u)     /*!< Unicast local address. */
# define SOAD_ADDRESS_TYPE_MULTICAST              (0x01u)     /*!< Multicast local address. */

/*! Indicates if local address assignment is triggered manually by user or automatically. */
typedef uint8                                     SoAd_AssignTriggerType;
# define SOAD_ASSIGN_TRIGGER_AUTOMATIC            (0x00u)     /*!< Local address assignment triggered automatically. */
# define SOAD_ASSIGN_TRIGGER_MANUAL               (0x01u)     /*!< Local address assignment triggered manually. */

/*! Local address type. */
typedef struct
{
  SoAd_AddressTypeType AddressType;                           /*!< Address type (unicacst/multicast). */
  SoAd_IpAddrAssignmentType AssignType;                       /*!< Address assignment (static/DHCP/...). */
  SoAd_AssignTriggerType AssignTrigger;                       /*!< Address trigger (automatic/manual). */
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  uint8 IfcIdx;                                               /*!< Interface index for BSD Socket API. */
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
} SoAd_LocalAddressType;

/*! Pointer to constant IP address. */
typedef P2CONST(uint32, AUTOMATIC, SOAD_CONST) SoAd_IpAddrConstPtrType;

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Generic socket address type for BSD Socket API. */
typedef struct
{
  SoAd_DomainType    domain;                          /*!< Domain type (IPv4/IPv6). */
} SoAd_SockAddrType;

/*! IPv4 socket address type for BSD Socket API. */
typedef struct
{
  SoAd_DomainType    domain;                          /*!< Domain type (IPv4/IPv6). */
  uint16             port;                            /*!< Port number. */
  uint32             addr[1];                         /*!< IPv4 address. */
} SoAd_SockAddrInetType;

/*! IPv6 socket address type for BSD Socket API. */
typedef struct
{
  SoAd_DomainType    domain;                          /*!< Domain type (IPv4/IPv6). */
  uint16             port;                            /*!< Port number. */
  uint32             addr[4];                         /*!< IPv6 address. */
} SoAd_SockAddrInet6Type;

/*! IPv4 IP address type for BSD Socket API. */
typedef struct
{
  uint32             addr[1];                         /*!< IPv4 address. */
} SoAd_IpAddrInetType;

/*! IPv6 IP address type for BSD Socket API. */
typedef struct
{
  uint32             addr[4];                         /*!< IPv6 address. */
} SoAd_IpAddrInet6Type;

/*! IP address type for BSD Socket API. */
typedef uint32 SoAd_IpAddrType;

#  if ( SOAD_VIPV6_ENABLED == STD_ON )
/*! Largest IP address struct in case of IPv6 for BSD Socket API in ASR format. */
#   define SoAd_IpAddrInetXType      SoAd_IpAddrInet6Type
/*! Largest IP address struct in case of IPv6 for BSD Socket API in BSD format. */
#   define sockaddr_inX              sockaddr_in6
#  else
/*! Largest IP address struct in case of IPv4 only for BSD Socket API in ASR format. */
#   define SoAd_IpAddrInetXType      SoAd_IpAddrInetType
/*! Largest IP address struct in case of IPv4 only for BSD Socket API in BSD format. */
#   define sockaddr_inX              sockaddr_in
#  endif
# else
/*! Generic socket address type for ASR Socket API. */
typedef TcpIp_SockAddrType          SoAd_SockAddrType;
/*! IPv4 socket address type for ASR Socket API. */
typedef TcpIp_SockAddrInetType      SoAd_SockAddrInetType;
/*! IPv6 socket address type for ASR Socket API. */
typedef TcpIp_SockAddrInet6Type     SoAd_SockAddrInet6Type;
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

/*! Unspecified socket address type used if IP address is unknown. */
typedef struct
{
  SoAd_DomainType domain;                                     /*!< Domain type (IPv4/IPv6). */
  uint16          port;                                       /*!< Port number. */
} SoAd_SockAddrUnspecType;

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Local IP address type containing static parameters for BSD Socket API. */
typedef struct
{
  P2CONST(SoAd_IpAddrType, AUTOMATIC, SOAD_CONST) addrPtr;    /*!< Pointer to IP address. */
  SoAd_DomainType domain;                                     /*!< Domain type (IPv4/IPv6). */
  uint8 netmask;                                              /*!< Netmask value. */
} SoAd_LocalIpAddrType;

/*! Local IP address type containing dynamic parameters for BSD Socket API. */
typedef struct
{
#  if ( SOAD_VIPV6_ENABLED == STD_ON )
  uint32 addr[4];                                             /*!< IP address in case of IPv6. */
#  else
  uint32 addr[1];                                             /*!< IP address in case of IPv4 only. */
#  endif /* SOAD_VIPV6_ENABLED == STD_ON */
  uint8 netmask;                                              /*!< Netmask value. */
  uint8 ifcIdx;                                               /*!< Interface index for BSD Socket API. */
} SoAd_LocalIpAddrDynType;

/*! Local IP address request state. */
typedef uint8                                   SoAd_LocalIpAddrReqStateType;
#  define SOAD_LOCAL_IP_ADDR_REQ_STATE_NONE     (0x00u)       /*!< IP address has no requests. */
#  define SOAD_LOCAL_IP_ADDR_REQ_STATE_REQUEST  (0x01u)       /*!< IP address requested. */
#  define SOAD_LOCAL_IP_ADDR_REQ_STATE_RELEASE  (0x02u)       /*!< IP address requested to release. */

/*! Type to handle addtional sockets in case of BSD Socket API to receive from broadcast/multicast addresses. */
typedef struct
{
  SoAd_SocketIdType LimNodeSocketId;                          /*!< Socket identifier for limited broadcast address. */
  SoAd_SocketIdType DirLinkSocketId;                          /*!< Socket identifier for directed broadcast address. */
} SoAd_MulticastAddrType;
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VIPV6_ENABLED == STD_ON )
/*! Largest IP address struct in case of IPv6 in ASR format. */
  typedef SoAd_SockAddrInet6Type      SoAd_SockAddrInetXType;
# else
/*! Largest IP address struct in case of IPv4 in ASR format. */
  typedef SoAd_SockAddrInetType       SoAd_SockAddrInetXType;
# endif /* SOAD_VIPV6_ENABLED == STD_ON */

/*! Module internal socket index type. */
typedef uint16                        SoAd_SockIdxType;
/*! Module internal socket index type used for loop iterator. */
typedef uint16_least                  SoAd_SockIdxType_least;
/*! Invalid value for module internal socket index type. */
# define SOAD_INV_SOCK_IDX            (0xFFFFu)

# define SOAD_SOCK_STATE_ESTABLISHED  (0x00u)                 /*!< Socket state established. */
# define SOAD_SOCK_STATE_CONNECT      (0x01u)                 /*!< Socket state connecting to remote entity. */
# define SOAD_SOCK_STATE_LISTEN       (0x02u)                 /*!< Socket state listen for incoming connections. */
# define SOAD_SOCK_STATE_CLOSING      (0x03u)                 /*!< Socket state closing. */
# define SOAD_SOCK_STATE_CLOSED       (0x04u)                 /*!< Socket state closed. */

/*! Socket connection mode type. */
typedef uint8                         SoAd_SoConModeType;
# define SOAD_SOCON_ONLINE            (0x00u)                 /*!< Socket connection mode online. */
# define SOAD_SOCON_RECONNECT         (0x01u)                 /*!< Socket connection mode reconnect. */
# define SOAD_SOCON_OFFLINE           (0x02u)                 /*!< Socket connection mode offline. */
/* ---------------------- */

/*! Socket connection mode filter type used in best match algorithm. */
typedef uint8                         SoAd_SoConModeFilterType;
# define SOAD_SOCON_FILTER_DISABLED   (0x00u)                 /*!< No filter active. */
# define SOAD_SOCON_FILTER_ON_OFF     (0x01u)                 /*!< Consider online or offline only. */
/* ---------------------- */

# if ( defined(SOAD_CONFIGURED_MAX_SOCK_CON) && (SOAD_CONFIGURED_MAX_SOCK_CON <= 255u) )
  /*! Socket connection identifier type if < 255 socket connections are configured. */
  typedef uint8                     SoAd_SoConIdType;
  /*! Socket connection identifier type for loop iterator if < 255 socket connections are configured. */
  typedef uint8_least               SoAd_SoConIdType_least;
  /*! Invalid value for socket connection identifier type if < 255 socket connections are configured. */
#  define SOAD_INV_SO_CON_ID         0xFFu
  /*! Indicates type of socket connection type if < 255 socket connections are configured. */
#  define SOAD_SO_CON_ID_TYPE_UINT16 STD_OFF
# else
  /*! Socket connection identifier type if >= 255 socket connections are configured. */
  typedef uint16                    SoAd_SoConIdType;
  /*! Socket connection identifier type for loop iterator if >= 255 socket connections are configured. */
  typedef uint16_least              SoAd_SoConIdType_least;
  /*! Invalid value for socket connection identifier type if >= 255 socket connections are configured. */
#  define SOAD_INV_SO_CON_ID         0xFFFFu
  /*! Indicates type of socket connection type if >= 255 socket connections are configured. */
#  define SOAD_SO_CON_ID_TYPE_UINT16 STD_ON
# endif /* defined(SOAD_CONFIGURED_MAX_SOCK_CON) && (SOAD_CONFIGURED_MAX_SOCK_CON <= 255u) */

/*! Socket connection index used in case of post-build since there might be invalid socket connection identifier. */
typedef SoAd_SoConIdType            SoAd_SoConIdxType;
/*! Socket connection index for loop iterator. */
typedef SoAd_SoConIdType_least      SoAd_SoConIdxType_least;
/*! Invalid value for socket connection index. */
# define SOAD_INV_SO_CON_IDX         SOAD_INV_SO_CON_ID
/* ------------------- */

/*! Type to indicate state (set, wildcard, not set) of remote address. */
typedef uint8                           SoAd_RemAddrStateType;
                                              /*     NOT ANY SET */
# define SOAD_SOCON_IP_SET_PORT_SET      0x03u /* 00  00  00  11  */ /*!< IP address set, port set. */
# define SOAD_SOCON_IP_SET_PORT_ANY      0x06u /* 00  00  01  10  */ /*!< IP address set, port is wildcard. */
# define SOAD_SOCON_IP_SET_PORT_NOT      0x12u /* 00  01  00  10  */ /*!< IP address set, port not set. */
# define SOAD_SOCON_IP_ANY_PORT_SET      0x09u /* 00  00  10  01  */ /*!< IP address wildcard, port set. */
# define SOAD_SOCON_IP_ANY_PORT_ANY      0x0Cu /* 00  00  11  00  */ /*!< IP address wildcard, port is wildcard. */
# define SOAD_SOCON_IP_ANY_PORT_NOT      0x18u /* 00  01  10  00  */ /*!< IP address wildcard, port not set. */
# define SOAD_SOCON_IP_NOT_PORT_SET      0x21u /* 00  10  00  01  */ /*!< IP address not set, port set. */
# define SOAD_SOCON_IP_NOT_PORT_ANY      0x24u /* 00  10  01  00  */ /*!< IP address not set, port wildcard. */
# define SOAD_SOCON_IP_NOT_PORT_NOT      0x30u /* 00  11  00  00  */ /*!< IP address not set, port not set. */
# define SOAD_SOCON_MASK_IPADDR          0x2Au /* 00  10  10  10  */ /*!< Bit mask for IP address state. */
# define SOAD_SOCON_MASK_PORT            0x15u /* 00  01  01  01  */ /*!< Bit mask for port state. */
# define SOAD_SOCON_MASK_SET             0x03u /* 00  00  00  11  */ /*!< Bit mask for state set. */
# define SOAD_SOCON_MASK_ANY             0x0Cu /* 00  00  11  00  */ /*!< Bit mask for state is wildcard. */
# define SOAD_SOCON_MASK_NOT             0x30u /* 00  11  00  00  */ /*!< Bit mask for state not set. */

/*! PDU index type used in case of post-build since there might be invalid PDU identifier. */
typedef PduIdType                   SoAd_PduIdxType;
/*! Invalid value for PDU index. */
# define SOAD_INV_PDU_IDX           SOAD_INV_PDU_ID
/*! Invalid value for PDU index used for socket routes. */
# define SOAD_INV_SOCKROUTE_IDX     SOAD_INV_PDU_ID
/*! Invalid value for PDU index used for pdu routes. */
# define SOAD_INV_PDUROUTE_IDX      SOAD_INV_PDU_ID

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/*! PDU header identifier type. */
typedef uint32                      SoAd_PduHdrIdType;
/*! PDU header length type. */
typedef uint32                      SoAd_PduHdrLenType;
/*! Invalid value for PDU header identifier. */
#  define SOAD_INV_PDU_HDR_ID       (0xFFFFFFFFu)
/*! PDU header size. */
#  define SOAD_PDU_HDR_SIZE         (0x08u)
/*! PDU header identifier size. */
#  define SOAD_PDU_HDR_ID_SIZE      (0x04u)
/*! PDU header length size. */
#  define SOAD_PDU_HDR_LEN_SIZE     (0x04u)
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

/*! Module state type. */
typedef uint8                       SoAd_StateType;

/*! Upper layer type. */
typedef uint8                       SoAd_UpperLayerType;
# define SOAD_UL_TYPE_TP            (0x01u)           /*!< Upper layer type is TP-API. */
# define SOAD_UL_TYPE_IF            (0x00u)           /*!< Upper layer type is IF-API. */

/*! Trigger mode type for UDP Tx PDUs. */
typedef uint8                       SoAd_TxUdpTriggerModeType;
# define SOAD_TX_UDP_TRIGGER_ALWAYS (0x00u)           /*!< Triggers transmission with transmit request. */
# define SOAD_TX_UDP_TRIGGER_NEVER  (0x01u)           /*!< Does not trigger transmission with transmit request. */
# define SOAD_TX_UDP_TRIGGER_NONE   (0x02u)           /*!< No trigger mode configured. */

# if ( defined(SOAD_CONFIGURED_MAX_ROUT_GROUP) && (SOAD_CONFIGURED_MAX_ROUT_GROUP <= 255u) )
  /*! Routing group identifier type if < 255 routing groups are configured. */
  typedef uint8                    SoAd_RoutingGroupIdType;
  /*! Routing group identifier type for loop iterator if < 255 routing groups are configured. */
  typedef uint8_least              SoAd_RoutingGroupIdType_least;
  /*! Invalid value for routing group identifer if < 255 routing groups are configured. */
#  define SOAD_INV_ROUT_GRP_ID     0xFFu
# else
  /*! Routing group identifier type if >= 255 routing groups are configured. */
  typedef uint16                   SoAd_RoutingGroupIdType;
  /*! Routing group identifier type for loop iterator if >= 255 routing groups are configured. */
  typedef uint16_least             SoAd_RoutingGroupIdType_least;
  /*! Invalid value for routing group identifer if >= 255 routing groups are configured. */
#  define SOAD_INV_ROUT_GRP_ID     0xFFFFu
# endif /* defined(SOAD_CONFIGURED_MAX_ROUT_GROUP) && (SOAD_CONFIGURED_MAX_ROUT_GROUP <= 255u) */

/*! Routing group index used in case of post-build since there might be invalid routing group identifier. */
typedef SoAd_RoutingGroupIdType       SoAd_RoutingGroupIdxType;
/*! Routing group index for loop iterator. */
typedef SoAd_RoutingGroupIdType_least SoAd_RoutingGroupIdxType_least;
/*! Invalid value for routing group index. */
# define SOAD_INV_ROUT_GRP_IDX         SOAD_INV_ROUT_GRP_ID

/*! TCP Tx Queue type used to handle a TCP transmission and corresponding confirmation. */
typedef struct
{
  SoAd_PduIdxType   TxPduIdx;                         /*!< Tx PDU index. */
  PduLengthType     Len;                              /*!< length of transmitted data. */
} SoAd_TcpTxQueueType;

/*! TCP Tx Queue management type used to manage TCP Tx queue elements on a socket connection. */
typedef struct
{
  uint16 Idx;                                         /*!< Index of first queue element. */
  uint16 Lvl;                                         /*!< Level of used queue elements. */
} SoAd_TcpTxQueueMgtType;

/*! TCP Tx Queue configuration type containing e.g. cofigured size of TCP Tx Queue. */
typedef struct
{
  P2VAR(SoAd_TcpTxQueueType,      AUTOMATIC, SOAD_VAR_NOINIT) QueuePtr;     /*!< Pointer to queue. */
  P2VAR(SoAd_TcpTxQueueMgtType,   AUTOMATIC, SOAD_VAR_NOINIT) MgtPtr;       /*!< Pointer to management struct. */
  uint16                                                      Size;         /*!< Size of TCP Tx Queue. */
} SoAd_TcpTxQueueConfigType;

/*! Socket connection specific TLS configuration type. */
typedef struct
{
  uint32  TxBufSize;                                  /*!< Required TLS Tx buffer size. */
  uint32  RxBufSize;                                  /*!< Required TLS Rx buffer size. */
  uint8   FctIdTlsNotif;                              /*!< Socket creation notification identifier. */
} SoAd_TcpTlsConfigType;

/*! Tx management struct type to handle PDU transmissions. */
typedef struct
{
  SoAd_PduIdxType    TxPduIdx;                        /*!< Tx PDU index. */
  PduLengthType      TxBytesLen;                      /*!< Length of PDU. */
  PduLengthType      TxBytesPending;                  /*!< Pending length of PDU. */
  SduDataPtrType     TxBufPtr;                        /*!< Pointer corresponding Tx buffer. */
  boolean            CancelRequested;                 /*!< Indicates if cancel is requested. */
} SoAd_TxMgtType;

/*! Rx management struct type to handle PDU reception. */
typedef struct
{
  SoAd_BufSegMgtType BufSegMgt;                       /*!< Managment struct to handle TcpIp reception ring buffer. */
  uint16             RouteIdx;                        /*!< Socket route index. */
  PduLengthType      RxBytesPending;                  /*!< Pending length of PDU. */
  boolean            CancelRequested;                 /*!< Indicates if cancel is requested. */
} SoAd_RxMgtType;

/*! Type to define the size of EventQueue and TimeoutList realted handle (e.g. socket connection identifier). */
typedef uint8 SoAd_QueueListTypeType;
# define SOAD_QUEUE_LIST_TYPE_UINT8   0x00u           /*!< Queue or list handle type is uint8. */
# define SOAD_QUEUE_LIST_TYPE_UINT16  0x01u           /*!< Queue or list handle type is uint16. */

/*! Bit pattern type used to define the specific bit pattern of an EventQueue. */
typedef uint8 SoAd_EventQueueBitPatternType;
# define SOAD_EVENT_QUEUE_BIT_PATTERN_TCPIP_SOCKETS    0x01u  /*!< Queue for module internal socket indexes. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_TP_RX_SO_CON     0x02u  /*!< Queue for socket connections with TP Rx PDU. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_LOCAL_ADDR       0x04u  /*!< Queue for local addresses. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_STATE_SO_CON     0x08u  /*!< Queue for socket connections (state) */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_TP_TX_SO_CON     0x10u  /*!< Queue for socket connections with TP Tx PDU. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_TCP_TX_SO_CON    0x20u  /*!< Queue for socket connections with TCP Tx. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_IF_UDP_PDU_ROUTE 0x40u  /*!< Queue for socket connections with UDP IF Tx PDU. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_IF_TX_ROUTE_GRP  0x80u  /*!< Queue for socket connections with IF Tx RouteGrp. */

/*! Management type for EventQueue. */
typedef struct
{
  uint16 Idx;                                         /*!< Index of first queue element. */
  uint16 Lvl;                                         /*!< Level of used queue elements. */
} SoAd_EventQueueMgtType;

/*! Management type for TimeoutList. */
typedef struct
{
  uint16 Lvl;                                         /*!< Level of used list elements. */
} SoAd_TimeoutListMgtType;

/*! EventQueue configuration type. */
typedef struct
{
  P2VAR(SoAd_EventQueueMgtType, AUTOMATIC, SOAD_VAR_NOINIT) EveMgtPtr;        /*!< Pointer to EventQueue management. */
  P2VAR(void,                   AUTOMATIC, SOAD_VAR_NOINIT) EveQuePtr;        /*!< Pointer to EventQueue. */
  uint16                                                    EveQueSize;       /*!< Size of EventQueue. */
  uint16                                                    EveQueLimit;      /*!< Limit of processed elements. */
  SoAd_QueueListTypeType                                    EveQueType;       /*!< EventQueue type (uint8/uint16). */
  SoAd_EventQueueBitPatternType                             EveQueBitPattern; /*!< EventQueue bit pattern. */
} SoAd_EventQueueType;

/*! EventQueue flag type used to indicate if a handle is already in list. */
typedef uint8 SoAd_EventQueueFlagType;

/*! TimeoutList element generic type. */
typedef struct
{
  uint32  Timeout;                                    /*!< Timeout value (value of global counter). */
} SoAd_TimeoutListElemType;

/*! TimeoutList element type used to store the timeout value and handle for TimeoutList of type uint8. */
typedef struct
{
  uint32  Timeout;                                    /*!< Timeout value. */
  uint8   HandleIdx;                                  /*!< Handle index. */
} SoAd_TimeoutListElem8Type;

/*! TimeoutList element type used to store the timeout value and handle for TimeoutList of type uint16. */
typedef struct
{
  uint32  Timeout;                                    /*!< Timeout value. */
  uint16  HandleIdx;                                  /*!< Handle index. */
} SoAd_TimeoutListElem16Type;

/*! TimeoutList configuration type. */
typedef struct
{
  P2VAR(SoAd_TimeoutListMgtType,  AUTOMATIC, SOAD_VAR_NOINIT) TimeMgtPtr;         /*!< Pointer to management. */
  P2VAR(SoAd_TimeoutListElemType, AUTOMATIC, SOAD_VAR_NOINIT) TimeListPtr;        /*!< Pointer to list. */
  P2VAR(void,                     AUTOMATIC, SOAD_VAR_NOINIT) TimeListIdxMapPtr;  /*!< Pointer to list/handle map. */
  uint16                                                      TimeListSize;       /*!< Size of list. */
  uint16                                                      TimeListIdxMapSize; /*!< Size of list/handle map. */
  SoAd_QueueListTypeType                                      TimeListType;       /*!< List type (uint8/uint16). */
} SoAd_TimeoutListType;

/*! Rx buffer struct type for each socket connection. */
typedef struct
{
  P2VAR(PduLengthType, AUTOMATIC, SOAD_VAR_NOINIT) IfRxBufferOffsetPtr; /*!< Pointer to IF Rx buffer offset. */
  P2VAR(uint16,        AUTOMATIC, SOAD_VAR_NOINIT) TpRxBufferOffsetPtr; /*!< Pointer to TP Rx buffer offset. */
  P2VAR(uint8,         AUTOMATIC, SOAD_VAR_NOINIT) IfRxBufferPtr;       /*!< Pointer to IF Rx buffer */
  P2VAR(uint8,         AUTOMATIC, SOAD_VAR_NOINIT) TpRxBufferPtr;       /*!< Pointer to TP Rx buffer */
} SoAd_RxBufferType;

/*! Tx buffer struct type for each socket connection. */
typedef struct
{
  P2VAR(uint16, AUTOMATIC, SOAD_VAR_NOINIT) TpTxBufferOffsetPtr;         /*!< Pointer to TP Tx buffer offset. */
  P2VAR(uint8,  AUTOMATIC, SOAD_VAR_NOINIT) TpTxBufferPtr;               /*!< Pointer to TP Tx buffer. */
} SoAd_TxBufferType;

/*! Socket route destination type. */
typedef struct
{
# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
  P2CONST(SoAd_RoutingGroupIdType, AUTOMATIC, SOAD_CONST) RoutGrpIdColPtr;  /*!< Pointer to routing group colletion. */
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
  PduIdType                RxPduId;                                         /*!< Rx PDU identifier. */
# if ( SOAD_VROUT_GROUP_RX_ENABLED == STD_ON)
  SoAd_RoutingGroupIdType  RoutGrpIdNum;                                    /*!< Number of routing groups. */
# endif /* SOAD_VROUT_GROUP_RX_ENABLED == STD_ON */
  SoAd_UpperLayerType      UpperLayer;                                      /*!< Upper layer type. */
  uint8                    UpperLayerId;                                    /*!< Upper layer identifier. */
# if ( SOAD_VMETA_DATA_RX_ENALBED == STD_ON )
  boolean                  MetaDataRxEnabled;                               /*!< Indicates if meta data are used. */
# endif /* SOAD_VMETA_DATA_RX_ENALBED == STD_ON */
} SoAd_SocketRouteDestType;

/*! Socket route type. */
typedef struct
{
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  SoAd_PduHdrIdType         RxPduHdrId;               /*!< PDU header identifier. */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
  SoAd_SoConIdxType         SoConIdx;                 /*!< Socket connection index. */
  SoAd_SocketRouteDestType  SockRouteDest;            /*!< Socket route destination. */
} SoAd_SocketRouteType;

/*! Socket route colletion type used to map socket connection index to socket routes. */
typedef struct
{
  uint16    SockRouteIdxStart;                        /*!< Index of first socket route. */
  uint16    SockRouteIdxNum;                          /*!< Number of socket routes. */
} SoAd_SocketRouteCollectionType;

/*! TCP no delay type to indicate if enabled, disabled or not specified. */
typedef uint8                             SoAd_TcpNoDelayType;
# define SOAD_TCP_NO_DELAY_ENABLED        0x00u       /*!< TCP no delay is enabled. */
# define SOAD_TCP_NO_DELAY_DISABLED       0x01u       /*!< TCP no delay is disabled. */
# define SOAD_TCP_NO_DELAY_NOT_SPECIFIED  0x02u       /*!< TCP no delay is not specified. */

/*! TCP socket configuration type of socket connection group. */
typedef struct
{
# if ( defined(SOAD_VTCP_KEEP_ALIVE_ENABLED) && (STD_ON == SOAD_VTCP_KEEP_ALIVE_ENABLED) )
  uint32              TcpKeepAliveTime;               /*!< Keep alive time. */
  uint32              TcpKeepAliveProbesMax;          /*!< Keep alive maximum probes. */
  uint32              TcpKeepAliveInterval;           /*!< Keep alive interval time. */
  boolean             TcpKeepAlive;                   /*!< Indicates if keep alive is enabled. */
# endif /* defined(SOAD_VTCP_KEEP_ALIVE_ENABLED) && (STD_ON == SOAD_VTCP_KEEP_ALIVE_ENABLED) */
# if ( defined(SOAD_VTCP_TX_BUF_MIN_ENABLED) && (STD_ON == SOAD_VTCP_TX_BUF_MIN_ENABLED) )
  uint16              TcpTxBufMin;                    /*!< Required TCP Tx buffer size. */
# endif /* defined(SOAD_VTCP_TX_BUF_MIN_ENABLED) && (STD_ON == SOAD_VTCP_TX_BUF_MIN_ENABLED) */
  SoAd_TcpNoDelayType TcpNoDelay;                     /*!< Indicates TCP no delay is enabled. */
  boolean             ImmedTpTxConf;                  /*!< Indicates if TCP immediate TxConfirmation is is enabled. */
  boolean             TcpInitiate;                    /*!< Indicates if TCP socket is client socket. */
} SoAd_TcpSocketConfigType;

/*! UDP socket configuration type of socket connection group. */
typedef struct
{
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
  uint16          UdpTriggerTimeout;                  /*!< UDP trigger timeout value. */
  uint16          NPduUdpTxBufferMin;                 /*!< NPdu UDP Tx buffer size. */
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */
# if ( defined(SOAD_VUDP_ALIVE_TIMEOUT_ENALBED) && (STD_ON == SOAD_VUDP_ALIVE_TIMEOUT_ENALBED) )
  uint16          AliveTimeoutMaxCnt;                 /*!< Supervision alive timeout value . */
# endif /* defined(SOAD_VUDP_ALIVE_TIMEOUT_ENALBED) && (STD_ON == SOAD_VUDP_ALIVE_TIMEOUT_ENALBED) */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
  uint8           NPduUdpTxQueueSize;                 /*!< NPdu UDP Tx queue size. */
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON) */
# if ( SOAD_VUDP_RETRY_ENABLED == STD_ON )
  uint8           RetryQueueLimit;                    /*!< UDP retry queue limit. */
# endif /* SOAD_VUDP_RETRY_ENABLED == STD_ON */
# if ( SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON )
  uint8           ImmedIfTxConfListSize;              /*!< List size if immediate IF TxConfirmation is enabled. */
  boolean         ImmedIfTxConf;                      /*!< Indicates if immediate IF TxConfirmation is enabled. */
# endif /* SOAD_VUDP_IMMED_IF_TX_CONF == STD_ON */
# if ( defined(SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED) && (STD_ON == SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED) )
  boolean         UdpStrictHdrLenCheckEnabled;        /*!< Indicates if all Rx PDU headers shall be checked first. */
# endif /* defined(SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED) && (STD_ON == SOAD_VUDP_STRICT_HDR_LEN_CHECK_ENABLED) */
  boolean         UdpListenOnly;                      /*!< Indicates if socket connections are listen only. */
} SoAd_UdpSocketConfigType;

/*! Socket connection group configuration type. */
typedef struct
{
  P2CONST(SoAd_TcpSocketConfigType, AUTOMATIC, SOAD_CONST) SocketTcpPtr; /*!< Pointer to TCP socket configuration. */
  P2CONST(SoAd_UdpSocketConfigType, AUTOMATIC, SOAD_CONST) SocketUdpPtr; /*!< Pointer to UDP socket configuration. */
  SoAd_LocalAddrIdType    LocIpAddrId;                                   /*!< Local address identifier. */
  SoAd_PortType           LocalPort;                                     /*!< Local port value. */
  SoAd_SockIdxType        GrpSockIdx;                                    /*!< Socket index of TCP listen/UDP socket. */
  uint32                  SockIfRxBufMin;                                /*!< IF Rx buffer size. */
  uint16                  SockTpRxBufMin;                                /*!< TP Rx buffer size. */
# if ( SOAD_VTX_BUF_ENABLED == STD_ON )
  uint16                  SockTpTxBufMin;                                /*!< TP Tx buffer size. */
# endif /* SOAD_VTX_BUF_ENABLED == STD_ON */
  SoAd_ProtocolType       Protocol;                                      /*!< Protocol type (TCP/UDP). */
  SoAd_DomainType         Domain;                                        /*!< Domain (IPv4/IPv6). */
# if ( SOAD_VFRAME_PRIORITY_ENABLED == STD_ON )
  uint8                   FramePriority;                                 /*!< Frame priority (VLAN). */
# endif /* SOAD_VFRAME_PRIORITY_ENABLED == STD_ON */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  boolean                 PduHdrEnabled;                                 /*!< Indicates if PDU header enabled. */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
# if ( SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED == STD_ON )
  boolean                 BestMatchWithSockRouteEnabled;                 /*!< Best match with socket routes enabled. */
# endif /* SOAD_VBEST_MATCH_WITH_SOCK_ROUTE_ENABLED == STD_ON */
# if ( SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON )
  boolean                 BestMatchWithPduHeaderEnabled;                 /*!< Best match with PDU header enabled. */
# endif /* SOAD_VBEST_MATCH_WITH_PDU_HDR_ENABLED == STD_ON */
  boolean                 SockAutoSoConSetup;                            /*!< Indicates if auto setup is enabled. */
  boolean                 MsgAcceptFilterEnabled;                        /*!< Indicates if accept filter is enabled. */
} SoAd_SocketConnectionGroupType;

/*! Socket connection configuration type. */
typedef struct
{
# if ( SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON )
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_VAR_NOINIT) RcvRemSockPtr;       /*!< Pointer to received remote address. */
# endif /* SOAD_VGET_RCV_REMOTE_ADDR_ENABLED == STD_ON */
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_CONST) RemSockPtr;             /*!< Pointer to remote address. */
  P2CONST(SoAd_SocketConnectionGroupType, AUTOMATIC, SOAD_CONST) SockConGrpPtr; /*!< Pointer to group. */
# if ( SOAD_VTCP_USED == STD_ON )
  P2CONST(SoAd_TcpTxQueueConfigType, AUTOMATIC, SOAD_CONST) TcpTxQueuePtr;  /*!< Pointer to TCP Tx Queue. */
#  if ( SOAD_VTCP_TLS_ENABLED == STD_ON )
  P2CONST(SoAd_TcpTlsConfigType, AUTOMATIC, SOAD_CONST) TcpTlsConfigPtr;    /*!< Pointer to TCP Tls configuration. */
#  endif /* SOAD_VTCP_TLS_ENABLED == STD_ON */
# endif /* SOAD_VTCP_USED == STD_ON */
  SoAd_SockIdxType  SockIdx;                                                /*!< Pointer to socket index. */
  uint16            SoConChgFuncBitPattern;                                 /*!< Bit pattern for SoConModeChg. */
  uint16            IpAddrChgFuncBitPattern;                                /*!< Bit pattern for IpAddrChg. */
  SoAd_SoConIdType  SoConId;                                                /*!< Socket connection identifier. */
  uint8             RemAddrState;                                           /*!< Remote address state. */
# if ( SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON )
  boolean           VerifyRxPduEnabled;                                     /*!< Indicates if RxVerify is enabled. */
# endif /* SOAD_VVERIFY_RX_PDU_ENABLED == STD_ON */
} SoAd_SocketConnectionType;

/*! Socket connection collection type used to map socket index to socket connections. */
typedef struct
{
  SoAd_SoConIdType  SoConIdxStart;                    /*!< Index of first socket connection. */
  SoAd_SoConIdType  SoConIdxNum;                      /*!< Number of socket connections. */
} SoAd_SocketConnectionIdCollectionType;

/*! Bit pattern type to indicate if a routing group is enabled and/or triggered for transmission. */
typedef P2VAR(uint8, AUTOMATIC, SOAD_CONST) SoAd_RoutingGroupBitPatternPtrType;
# define SOAD_ROUT_GRP_BIT_PATTERN_NONE      0x00  /* 00000000 */       /*!< Not enabled, not triggered. */
# define SOAD_ROUT_GRP_BIT_PATTERN_ENABLED   0x01  /* 00000001 */       /*!< Routing group enabled. */
# define SOAD_ROUT_GRP_BIT_PATTERN_TRIGGER   0x02  /* 00000010 */       /*!< Rotuing group triggered. */

/*! Pdu route destination configuration type. */
typedef struct
{
# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  P2CONST(SoAd_RoutingGroupBitPatternPtrType, AUTOMATIC, SOAD_CONST) RoutGrpStateColPtr; /*!< Pointer to RoutGrpCol. */
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
  SoAd_SoConIdxType                               SoConIdx;             /*!< Socket connection index. */
  PduIdType                                       TxPduId;              /*!< Tx PDU index. */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  SoAd_PduHdrIdType                               TxPduHdrId;           /*!< Tx PDU header identifier. */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
  uint16                                          UdpTriggerTimeout;    /*!< UDP trigger timeout value. */
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */
# if ( SOAD_VROUT_GROUP_TX_ENABLED == STD_ON )
  SoAd_RoutingGroupIdType                         RoutGrpNum;           /*!< Number of related RouteGrps. */
# endif /* SOAD_VROUT_GROUP_TX_ENABLED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON )
  SoAd_TxUdpTriggerModeType                       TxUdpTrigMode;        /*!< Indicates trigger mode. */
# endif /* SOAD_VN_PDU_UDP_TX_ENABLED == STD_ON */
} SoAd_PduRouteDestType;

/*! Pdu route destination list type used to map socket connection index to Pdu route destinations. */
typedef struct
{
  const SoAd_PduRouteDestType*const*             PduRouteDestListPtr;   /*!< Pointer to pdu route destination list. */
  uint16                                         PduRouteDestListSize;  /*!< Size of list. */
} SoAd_SockConIdxPduRouteDestCollectionType;

/*! Routing group configuration type. */
typedef struct
{
  boolean         IsEnabledAtInit;                    /*!< Indicates if routing group is enabled at initialization. */
  boolean         TxTriggerable;                      /*!< Indicates if routing group is triggerable. */
} SoAd_RoutingGroupType;

/*! Routing group to socket route collection type. */
typedef struct
{
  P2CONST(uint16,                             AUTOMATIC, SOAD_CONST) IdColPtr;    /*!< Pointer to ID collection. */
  P2CONST(SoAd_RoutingGroupBitPatternPtrType, AUTOMATIC, SOAD_CONST) StateColPtr; /*!< Pointer to state collection. */
  uint16                                                             IdNum;       /*!< Number of socket routes. */
} SoAd_RoutingGroupIdSocketRouteCollectionType;

/*! Routing group to pdu route destination collection type. */
typedef struct
{
  const SoAd_PduRouteDestType*const*                        PduRouteDestListPtr;  /*!< Pointer to destinations. */
  P2CONST(SoAd_RoutingGroupBitPatternPtrType, AUTOMATIC, SOAD_CONST) StateColPtr; /*!< Pointer to state collection. */
  uint16                                                    PduRouteDestListSize; /*!< Number of destinations. */
} SoAd_RoutingGroupIdPduRouteCollectionType;

/*! Pdu route configuration type. */
typedef struct
{
  const SoAd_PduRouteDestType*const*  PduRouteDestListPtr;          /*!< Pointer to destinations. */
  uint16                              PduRouteDestListSize;         /*!< Number of destinations. */
  SoAd_UpperLayerType                 UpperLayer;                   /*!< Upper layer type. */
  uint8                               UpperLayerId;                 /*!< Upper layer identifier. */
  boolean                             TxConfEnabled;                /*!< Indicates if TxConfirmation is enabled. */
# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
  boolean                             AllPduRouteDestWithPduHdr;    /*!< Indicates if PDU header is enabled global. */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */
# if ( SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON )
  boolean                             IfTriggerTransmitEnabled;     /*!< Indicates if trigger transmit is enabled. */
# endif /* SOAD_VIF_TRIGGER_TRANSMIT_ENABLED == STD_ON */
# if ( SOAD_VTX_TP_OPTIMIZED_ENALBED == STD_ON )
  boolean                             TpOptimized;                  /*!< Indicates if TP optimization is enabled. */
# endif /* SOAD_VTX_TP_OPTIMIZED_ENALBED == STD_ON */
# if ( SOAD_VMETA_DATA_TX_ENALBED == STD_ON )
  boolean                             MetaDataTxEnabled;            /*!< Indicates if meta data are used. */
# endif /* SOAD_VMETA_DATA_TX_ENALBED == STD_ON */
} SoAd_PduRouteType;

/*! NPdu UDP Tx management type. */
typedef struct
{
  uint32 TotalLen;                                    /*!< Total length of nPdu. */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
  uint16 RetryCnt;                                    /*!< Retry used to send multiple frames if nPdu is too big. */
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
  boolean TransmissionActive;                         /*!< Indicates if transmission of nPdu is already active. */
} SoAd_NPduUdpTxMgtType;

/*! NPdu UDP Tx buffer management type. */
typedef struct
{
  uint16 Len;                                         /*!< Length of nPdu. */
} SoAd_NPduUdpTxBufMgtType;

/*! NPdu UDP Tx queue management type. */
typedef struct
{
  uint8 Lvl;                                          /*!< Length of nPdu queue. */
} SoAd_NPduUdpTxQueueMgtType;

/*! NPdu UDP Tx queue type. */
typedef struct
{
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
  uint16            BufIdx;                           /*!< Start index of PDU in buffer. */
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
  SoAd_PduIdxType   TxPduIdx;                         /*!< Tx PDU index. */
  PduLengthType     Len;                              /*!< Length of PDU. */
  boolean           Copied;                           /*!< Indicates if PDU could be copied to tx buffer. */
} SoAd_NPduUdpTxQueueType;

/*! NPdu UDP Tx type used to manage the entire nPdu UDP Tx feature. */
typedef struct
{
  P2VAR(SoAd_NPduUdpTxMgtType, AUTOMATIC, SOAD_VAR_NOINIT)      MgtPtr;       /*!< Pointer to management struct. */
# if ( SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON )
  P2VAR(uint8, AUTOMATIC, SOAD_VAR_NOINIT)                      BufPtr;       /*!< Pointer to buffer. */
  P2VAR(SoAd_NPduUdpTxBufMgtType, AUTOMATIC, SOAD_VAR_NOINIT)   BufMgtPtr;    /*!< Pointer to buffer management. */
# endif /* SOAD_VN_PDU_UDP_TX_BUF_ENABLED == STD_ON */
# if ( SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON )
  P2VAR(SoAd_NPduUdpTxQueueType, AUTOMATIC, SOAD_VAR_NOINIT)    QueuePtr;     /*!< Pointer to queue. */
  P2VAR(SoAd_NPduUdpTxQueueMgtType, AUTOMATIC, SOAD_VAR_NOINIT) QueueMgtPtr;  /*!< Pointer to queue management. */
# endif /* SOAD_VN_PDU_UDP_TX_QUEUE_ENABLED == STD_ON */
  SoAd_SoConIdType                                              SoConIdx;     /*!< Socket connection index. */
} SoAd_NPduUdpTxType;

/*! NPdu UDP Tx return type used in nPdu related functions to add elements to buffer or queue. */
typedef uint8                     SoAd_NPduReturnType;
# define SOAD_NPDU_OK             (0x00u)             /*!< Added or updated element. */
# define SOAD_NPDU_OVFL_NEW       (0x01u)             /*!< New PDU does not fit into nPdu. */
# define SOAD_NPDU_OVFL_UPDATE    (0x02u)             /*!< Updated PDU does not fit into nPdu. */
# define SOAD_NPDU_NOT_OK         (0x03u)             /*!< Did not add or update nPdu. */

/*! Trigger transmit buffer type. */
typedef struct
{
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr;      /*!< Pointer to trigger transmit buffer. */
  PduLengthType                          MaxLen;      /*!< Lengthof trigger transmit buffer. */
} SoAd_IfTriggerTransmitBufferType;

/*! TcpIp IP controller type. */
typedef struct
{
  P2CONST(SoAd_LocalAddrIdType, AUTOMATIC, SOAD_CONST)  AddrIdListPtr;  /*!< Pointer to local address list. */
  uint8                                                 AddrIdListNum;  /*!< Number of local addresses. */
# if ( SOAD_VUDP_RETRY_ENABLED == STD_ON )
  boolean                                               RetryEnabled;   /*!< Indicates if UDP retry list is enabled. */
# endif /* SOAD_VUDP_RETRY_ENABLED == STD_ON */
} SoAd_TcpIpCtrlType;

# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
/*! Local address to socket index collection type for BSD socket API. */
typedef struct
{
  P2CONST(SoAd_SockIdxType, AUTOMATIC, SOAD_CONST)  SockIdxColPtr;      /*!< Pointer to socket index collection. */
  SoAd_SockIdxType                                  SockIdxNum;         /*!< Number of socket indexes. */
} SoAd_LocalIpAddrSockIdxCollectionType;

/*! Tx socket buffer type for BSD socket API. */
typedef uint8       SoAd_SockTxBufIdxType;
/*! Tx socket buffer type for for BSD socket API for loop iterator. */
typedef uint8_least SoAd_SockTxBufIdxType_least;

/*! Tx socket buffer management type for BSD socket API. */
typedef struct
{
  uint16  SockIdx;                                    /*!< Socket index. */
  uint16  Len;                                        /*!< Length of data to be transmitted. */
} SoAd_SocketTxBufferMgtType;

/*! Rx socket buffer type for BSD socket API. */
typedef struct
{
  uint8*  BufPtr;                                     /*!< Pointer to buffer. */
  uint16  MaxLen;                                     /*!< Buffer length. */
} SoAd_SocketRxBufferType;

/*! Rx socket buffer management type for BSD socket API used as ring buffer. */
typedef struct
{
  uint16  Offset;                                     /*!< Offset in buffer. */
  uint16  Len;                                        /*!< Length of data in buffer. */
} SoAd_SocketRxBufferMgtType;
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/*! Rx PDU reception notification function pointer. */
typedef P2FUNC(Std_ReturnType, SOAD_CODE, SoAd_VerifyRxPduFctPtrType) (
                                            CONSTP2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
                                            CONSTP2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
                                            SoAd_PduHdrIdType PduHdrId,
                                            CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

# if ( !defined(SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT) )
/*! Number of retries to send a nPdu if nPdu becomes larger (caused by larger PDUs in trigger transmit calls). */
#  define SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT          2u
# else
#  if ( (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT == 0u) || \
      (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT > SOAD_INV_BUF_IDX_8) )
#   error "SoAd.h: value of SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT was defined to an invalid value!"
#  endif /* (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT == 0u) || \
  (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT > SOAD_INV_BUF_IDX_8) */
# endif /* !defined(SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT) */

# if ( !defined(SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN) )
/*! Number of retries in main function to send a nPdu if transmission to TcpIp stack fails. */
#  define SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN            200u
# else
#  if ( (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN == 0u) || \
      (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN > SOAD_INV_BUF_IDX_16) )
#   error "SoAd.h: value of SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN was defined to an invalid value!"
#  endif /* (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN == 0u) || \
  (SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN > SOAD_INV_BUF_IDX_16) */
# endif /* !defined(SOAD_VN_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN) */

/**********************************************************************************************************************
  POST BUILD CONFIGURATION
**********************************************************************************************************************/
typedef struct
{
  SOAD_CONSTP2VAR(SoAd_EventQueueFlagType)                          EventQueueFlagsListPtr;
  SOAD_CONSTP2CONST(SoAd_EventQueueType)                            EventQueuesListPtr;
  SOAD_CONSTP2CONST(SoAd_TimeoutListType)                           TimeoutListsListPtr;
  SOAD_CONSTP2VAR(SoAd_SockAddrInetXType)                           SockConIdx2RemSockDynListPtr;
  SOAD_CONSTP2VAR(SoAd_SoConModeType)                               SockConIdx2StateListPtr;
  SOAD_CONSTP2VAR(SoAd_IpAddrStateType)                             SockConIdx2IpAddrStateListPtr;
  SOAD_CONSTP2VAR(SoAd_PortType)                                    SockIdx2LocPortDynListPtr;
  SOAD_CONSTP2VAR(SoAd_SocketIdType)                                SockIdx2SocketIdListPtr;
  SOAD_CONSTP2VAR(boolean)                                          SockConIdx2FlagAbortListPtr;
  SOAD_CONSTP2VAR(uint8)                                            SockConIdx2CloseModeListPtr;
  SOAD_CONSTP2VAR(uint8)                                            SockIdx2SockStateListPtr;
  SOAD_CONSTP2VAR(uint8)                                            SockConIdx2FlagCntOpClListPtr;
  SOAD_CONSTP2VAR(uint8)                                            SockConIdx2RemAddrStateListPtr;
  SOAD_CONSTP2CONST(SoAd_SocketConnectionType)                      SockConsListPtr;
  SOAD_CONSTP2CONST(SoAd_SocketConnectionIdCollectionType)          SockIdx2SoConIdxColListPtr;
  SOAD_CONSTP2CONST(SoAd_SoConIdxType)                              SockConId2SockConIdxMapPtr;
  SOAD_CONSTP2VAR(SoAd_TxMgtType)                                   SockConIdx2TxMgtListPtr;
  SOAD_CONSTP2VAR(SoAd_SoConIdType)                                 SockIdx2ActiveSoConIdxListPtr;
  SOAD_CONSTP2VAR(uint16)                                           TxPduIdx2PendingTxConfListPtr;
  SOAD_CONSTP2CONST(SoAd_SockConIdxPduRouteDestCollectionType)      SockConIdx2PduRouteDestColListPtr;
  SOAD_CONSTP2CONST(SoAd_PduRouteType)                              TxPduIdx2PduRouteListPtr;
  SOAD_CONSTP2CONST(PduIdType)                                      TxPduIdx2TxConfPduIdListPtr;
  SOAD_CONSTP2CONST(SoAd_PduIdxType)                                TxPduId2PduRouteIdxMapPtr;
  SOAD_CONSTP2VAR(SoAd_RxMgtType)                                   SockConIdx2RxMgtListPtr;
  SOAD_CONSTP2VAR(uint32)                                           SockIdx2SkipBytesListPtr;
  SOAD_CONSTP2CONST(SoAd_RxBufferType)                              SockConIdx2RxBufferListPtr;
  SOAD_CONSTP2CONST(SoAd_SocketRouteType)                           SockRoutesListPtr;
  SOAD_CONSTP2CONST(SoAd_SocketRouteCollectionType)                 SockConIdx2SockRouteColListPtr;
  SOAD_CONSTP2CONST(SoAd_PduIdxType)                                RxPduId2SockRouteIdxMapPtr;
  SOAD_CONSTP2CONST(uint16)                                         SockConIdx2NPduUdpTxIdxListPtr;
  SOAD_CONSTP2CONST(SoAd_NPduUdpTxType)                             NPduUdpTxListPtr;
  SOAD_CONSTP2CONST(uint16)                                         SockConIdx2IfTriggerTransmitBufIdxListPtr;
  SOAD_CONSTP2CONST(SoAd_IfTriggerTransmitBufferType)               IfTriggerTransmitBufferListPtr;
  SOAD_CONSTP2CONST(SoAd_TxBufferType)                              SockConIdx2TxBufferListPtr;
  SOAD_CONSTP2CONST(SoAd_RoutingGroupType)                          RouteGrpIdx2RoutingGroupListPtr;
  SOAD_CONSTP2CONST(SoAd_RoutingGroupIdxType)                       RouteGrpId2RouteGrpIdxMapPtr;
  SOAD_CONSTP2CONST(SoAd_RoutingGroupIdPduRouteCollectionType)      RouteGrpIdx2PduRouteDestColListPtr;
  SOAD_CONSTP2CONST(SoAd_RoutingGroupIdSocketRouteCollectionType)   RouteGrpIdx2SocketRouteColListPtr;
  SOAD_CONSTP2VAR(uint8)                                            IfRouteGrpTransmitBufPtr;
  SOAD_CONSTP2VAR(uint8)                                            VerifyRxPduBufferPtr;
  SOAD_CONSTP2VAR(uint8)                                            MetaDataRxBufferPtr;
  SOAD_CONSTP2CONST(SoAd_LocalAddressType)                          LocalAddrPtr;
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  SOAD_CONSTP2CONST(SoAd_LocalIpAddrType)                           AddrId2LocalIpAddrPtr;
  SOAD_CONSTP2VAR(SoAd_LocalIpAddrDynType)                          AddrId2LocalIpAddrDynPtr;
  SOAD_CONSTP2VAR(SoAd_LocalIpAddrReqStateType)                     AddrId2LocalIpAddrReqStatePtr;
  SOAD_CONSTP2CONST(SoAd_LocalIpAddrSockIdxCollectionType)          AddrId2SockIdxColPtr;
  SOAD_CONSTP2VAR(SoAd_MulticastAddrType)                           SockIdx2MulticastAddrPtr;
  SOAD_CONSTP2VAR(SoAd_SocketTxBufferMgtType)                       SockTxBufIdx2MgtPtr;
  SOAD_CONSTP2VAR(SoAd_SocketRxBufferMgtType)                       SockIdx2RxMgtPtr;
  SOAD_CONSTP2CONST(SoAd_SocketRxBufferType)                        SockIdx2RxBufPtr;
  SOAD_CONSTP2VAR(uint8)                                            SocketTxBufferPtr;
  SOAD_CONSTP2VAR(uint8)                                            SocketRxBufferPtr;
# else
  SOAD_CONSTP2CONST(uint8)                                          AddrId2TcpIpCtrlPtr;
  SOAD_CONSTP2CONST(SoAd_TcpIpCtrlType)                             TcpIpCtrlPtr;
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
  uint16                                                            EventQueueFlagsArraySize;
  uint8                                                             EventQueueTotal;
  uint8                                                             TimeoutListTotal;
  uint8                                                             EventQueueIdxSockIdx;
  uint8                                                             EventQueueIdxTpRxSoCon;
  uint8                                                             EventQueueIdxLocalAddr;
  uint8                                                             EventQueueIdxStateSoCon;
  uint8                                                             EventQueueIdxTpTxSoCon;
  uint8                                                             EventQueueIdxTcpTxSoCon;
  uint8                                                             EventQueueIdxIfUdpPduRoute;
  uint8                                                             EventQueueIdxIfTxRouteGrp;
  uint8                                                             TimeoutListIdxUdpAlive;
  uint8                                                             TimeoutListIdxNPduUdpTx;
  SoAd_SoConIdType                                                  SockConTotal;
  SoAd_SoConIdxType                                                 SockConTotalWithInv;
  SoAd_RoutingGroupIdType                                           RouteGrpTotal;
  SoAd_RoutingGroupIdxType                                          RouteGrpTotalWithInv;
  PduIdType                                                         SockRouteTotal;
  SoAd_PduIdxType                                                   SockRouteTotalWithInv;
  PduIdType                                                         PduRouteTotal;
  SoAd_PduIdxType                                                   PduRouteTotalWithInv;
  SoAd_SockIdxType                                                  TcpIpSocketCnt;
  PduLengthType                                                     IfRouteGrpTransmitBufSize;
  PduLengthType                                                     MaxIfRouteGrpTransmitPduSize;
  PduLengthType                                                     MetaDataRxBufferSize;
  uint16                                                            SockConIdx2NPduUdpTxIdxSize;
  uint16                                                            AddrIdTotal;
# if ( SOAD_VSOCKET_BSD_API_ENABLED == STD_ON )
  uint16                                                            SocketTxBufferSize;
  uint16                                                            SocketRxBufferSize;
  uint8                                                             SocketTxBufferNum;
  boolean                                                           LingerTimeoutEnabled;
# endif /* SOAD_VSOCKET_BSD_API_ENABLED == STD_ON */
  boolean                                                           TcpKeepAliveEnabled;
  uint16                                                            FinalMagicNumber;
} SoAd_ConfigType;

#endif /* !defined(SOAD_TYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Types.h
 *********************************************************************************************************************/
