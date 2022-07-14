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
 *         \file  TcpIp_DhcpV4Server.c
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv4 - Server (DHCPv4 Server)
 *
 *      \details  This file is part of the TcpIp DHCPv4 Server submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp DHCPv4 Server submodule. >> TcpIp_DhcpV4Server.h
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_DHCPV4SERVER_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_DHCPV4SERVER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Std_Types.h"
#include "IpBase.h"
#include "TcpIp_Priv.h"

#include "TcpIp_DhcpV4Server.h"
#include "TcpIp_DhcpV4Server_Lcfg.h"
#if defined(TCPIP_DHCPV4SERVER_GET_PORT_MAC_ADDR_FUNC)
# include "EthIf.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of TcpIp_DhcpV4Server.h file */
#if (  (TCPIP_DHCPV4SERVER_SW_MAJOR_VERSION != 3u) \
    || (TCPIP_DHCPV4SERVER_SW_MINOR_VERSION != 2u) \
    || (TCPIP_DHCPV4SERVER_SW_PATCH_VERSION != 0u) )
  #error "TcpIp_DhcpV4Server.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif

#define TCPIP_DHCPV4SERVER_MSG_FLAG_BROADCAST (1<<15)

/* Lengths of DHCP message header field in bytes */
#define TCPIP_DHCPV4SERVER_MSG_LEN_OP                1U
#define TCPIP_DHCPV4SERVER_MSG_LEN_HTYPE             1U
#define TCPIP_DHCPV4SERVER_MSG_LEN_HLEN              1U
#define TCPIP_DHCPV4SERVER_MSG_LEN_HOPS              1U
#define TCPIP_DHCPV4SERVER_MSG_LEN_XID               4U
#define TCPIP_DHCPV4SERVER_MSG_LEN_SECS              2U
#define TCPIP_DHCPV4SERVER_MSG_LEN_FLAGS             2U
#define TCPIP_DHCPV4SERVER_MSG_LEN_CIADDR            4U
#define TCPIP_DHCPV4SERVER_MSG_LEN_YIADDR            4U
#define TCPIP_DHCPV4SERVER_MSG_LEN_SIADDR            4U
#define TCPIP_DHCPV4SERVER_MSG_LEN_GIAADR            4U
#define TCPIP_DHCPV4SERVER_MSG_LEN_CHADDR           16U
#define TCPIP_DHCPV4SERVER_MSG_LEN_SNAME            64U
#define TCPIP_DHCPV4SERVER_MSG_LEN_BOOTFILE        128U
#define TCPIP_DHCPV4SERVER_MSG_LEN_MAGIC_COOKIE      4U

/* Offsets of DHCP message header fields in bytes */
#define TCPIP_DHCPV4SERVER_MSG_OFS_OP              0U
#define TCPIP_DHCPV4SERVER_MSG_OFS_HTYPE           (TCPIP_DHCPV4SERVER_MSG_OFS_OP           + TCPIP_DHCPV4SERVER_MSG_LEN_OP)
#define TCPIP_DHCPV4SERVER_MSG_OFS_HLEN            (TCPIP_DHCPV4SERVER_MSG_OFS_HTYPE        + TCPIP_DHCPV4SERVER_MSG_LEN_HTYPE)
#define TCPIP_DHCPV4SERVER_MSG_OFS_HOPS            (TCPIP_DHCPV4SERVER_MSG_OFS_HLEN         + TCPIP_DHCPV4SERVER_MSG_LEN_HLEN)
#define TCPIP_DHCPV4SERVER_MSG_OFS_XID             (TCPIP_DHCPV4SERVER_MSG_OFS_HOPS         + TCPIP_DHCPV4SERVER_MSG_LEN_HOPS)
#define TCPIP_DHCPV4SERVER_MSG_OFS_SECS            (TCPIP_DHCPV4SERVER_MSG_OFS_XID          + TCPIP_DHCPV4SERVER_MSG_LEN_XID)
#define TCPIP_DHCPV4SERVER_MSG_OFS_FLAGS           (TCPIP_DHCPV4SERVER_MSG_OFS_SECS         + TCPIP_DHCPV4SERVER_MSG_LEN_SECS)
#define TCPIP_DHCPV4SERVER_MSG_OFS_CIADDR          (TCPIP_DHCPV4SERVER_MSG_OFS_FLAGS        + TCPIP_DHCPV4SERVER_MSG_LEN_FLAGS)
#define TCPIP_DHCPV4SERVER_MSG_OFS_YIADDR          (TCPIP_DHCPV4SERVER_MSG_OFS_CIADDR       + TCPIP_DHCPV4SERVER_MSG_LEN_CIADDR)
#define TCPIP_DHCPV4SERVER_MSG_OFS_SIADDR          (TCPIP_DHCPV4SERVER_MSG_OFS_YIADDR       + TCPIP_DHCPV4SERVER_MSG_LEN_YIADDR)
#define TCPIP_DHCPV4SERVER_MSG_OFS_GIADDR          (TCPIP_DHCPV4SERVER_MSG_OFS_SIADDR       + TCPIP_DHCPV4SERVER_MSG_LEN_SIADDR)
#define TCPIP_DHCPV4SERVER_MSG_OFS_CHADDR          (TCPIP_DHCPV4SERVER_MSG_OFS_GIADDR       + TCPIP_DHCPV4SERVER_MSG_LEN_GIAADR)
#define TCPIP_DHCPV4SERVER_MSG_OFS_SNAME           (TCPIP_DHCPV4SERVER_MSG_OFS_CHADDR       + TCPIP_DHCPV4SERVER_MSG_LEN_CHADDR)
#define TCPIP_DHCPV4SERVER_MSG_OFS_BOOTFILE        (TCPIP_DHCPV4SERVER_MSG_OFS_SNAME        + TCPIP_DHCPV4SERVER_MSG_LEN_SNAME)
#define TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE    (TCPIP_DHCPV4SERVER_MSG_OFS_BOOTFILE     + TCPIP_DHCPV4SERVER_MSG_LEN_BOOTFILE)
#define TCPIP_DHCPV4SERVER_MSG_OFS_OPTIONS         (TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE + TCPIP_DHCPV4SERVER_MSG_LEN_MAGIC_COOKIE)

/* Minimum length of a valid DHCP message: Header + DHCP Message Type Option (3 byte) + End Option (1 byte)  */
#define TCPIP_DHCPV4SERVER_MIN_MSG_LEN             (TCPIP_DHCPV4SERVER_MSG_OFS_OPTIONS + 4)

#define TCPIP_DHCPV4SERVER_MSG_OP_BOOTREQUEST    1U
#define TCPIP_DHCPV4SERVER_MSG_OP_BOOTREPLY      2U

/* Magic cookie that follows the DHCP message header and marks the beginning of the options section [RFC2131 3. The Client-Server Protocol] and [RFC2132 2. BOOTP Extension/DHCP Option Field Format] */
#define TCPIP_DHCPV4SERVER_MSG_MAGIC_COOKIE      0x63825363U

/* DHCP Options and BOOTP Vendor Extensions [RFC2132]           Supported              */
/*                                                                 |                   */
#define TCPIP_DHCPV4SERVER_MSG_OPT_PAD                      0U /* [y] 3. 1. Pad Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_END                    255U /* [y] 3. 2. End Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_SUBNET_MASK              1U /* [y] 3. 3. Subnet Mask */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TIME_OFFSET              2U /* [_] 3. 4. Time Offset */
#define TCPIP_DHCPV4SERVER_MSG_OPT_ROUTER                   3U /* [y] 3. 5. Router Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TIME_SERVER              4U /* [_] 3. 6. Time Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NAME_SERVER              5U /* [_] 3. 7. Name Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_DOMAIN_NAME_SERVER       6U /* [_] 3. 8. Domain Name Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_LOG_SERVER               7U /* [_] 3. 9. Log Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_COOKIE_SERVER            8U /* [_] 3.10. Cookie Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_LPR_SERVER               9U /* [_] 3.11. LPR Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_IMPRESS_SERVER          10U /* [_] 3.12. Impress Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_RES_LOC_SERVER          11U /* [_] 3.13. Resource Location Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_HOSTNAME                12U /* [_] 3.14. Host Name Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_BOOTFILE_SIZE           13U /* [_] 3.15. Boot File Size Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MERIT_DUMPFILE          14U /* [_] 3.16. Merit Dump File */
#define TCPIP_DHCPV4SERVER_MSG_OPT_DOMAIN_NAME             15U /* [_] 3.17. Domain Name */
#define TCPIP_DHCPV4SERVER_MSG_OPT_SWAP_SERVER             16U /* [_] 3.18. Swap Server */
#define TCPIP_DHCPV4SERVER_MSG_OPT_ROOT_PATH               17U /* [_] 3.19. Root Path */
#define TCPIP_DHCPV4SERVER_MSG_OPT_EXT_PATH                18U /* [_] 3.20. Extensions Path */
#define TCPIP_DHCPV4SERVER_MSG_OPT_IP_FORWARD              19U /* [_] 4. 1. IP Forwarding Enable/Disable Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NON_LOC_SRC_RT          20U /* [_] 4. 2. Non-Local Source Routing Enable/Disable Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_POLICY_FILTER           21U /* [_] 4. 3. Policy Filter Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MAX_DATAGRAMSIZE        22U /* [_] 4. 4. Maximum Datagram Reassembly Size */
#define TCPIP_DHCPV4SERVER_MSG_OPT_DEFAULT_TTL             23U /* [_] 4. 5. Default IP Time-to-live */
#define TCPIP_DHCPV4SERVER_MSG_OPT_PMTU_TIMEOUT            24U /* [_] 4. 6. Path MTU Aging Timeout Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_PMTU_PLATEAU_TBL        25U /* [_] 4. 7. Path MTU Plateau Table Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_INTERFACE_MTU           26U /* [_] 5. 1. Interface MTU Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_ALL_SUBNETS_LOCAL       27U /* [_] 5. 2. All Subnets are Local Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_BROADCAST_ADDR          28U /* [_] 5. 3. Broadcast Address Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_PERFORM_MASK_DISC       29U /* [_] 5. 4. Perform Mask Discovery Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MASK_SUPPLIER           30U /* [_] 5. 5. Mask Supplier Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_PERFORM_RTR_DISC        31U /* [_] 5. 6. Perform Router Discovery Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_RTR_SOLICIT_ADDR        32U /* [_] 5. 7. Router Solicitation Address Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_STATIC_ROUTE            33U /* [_] 5. 8. Static Route Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TRAILER_ENCAP           34U /* [_] 6. 1. Trailer Encapsulation Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_ARP_CACHE_TIMEOUT       35U /* [_] 6. 2. ARP Cache Timeout Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_ETHRNET_ENCAP           36U /* [_] 6. 3. Ethernet Encapsulation Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TCP_DEFAULT_TTL         37U /* [_] 7. 1. TCP Default TTL Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TCP_KEEP_ALIVE_INT      38U /* [_] 7. 2. TCP Keepalive Interval Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TCP_KEEP_ALIVE_DATA     39U /* [_] 7. 3. TCP Keepalive Garbage Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NET_INFO_SERVICE        40U /* [_] 8. 1. Network Information Service Domain Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NET_INFO_SERVERS        41U /* [_] 8. 2. Network Information Servers Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NTP_SERVERS             42U /* [_] 8. 3. Network Time Protocol Servers Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_VENDOR_INFO             43U /* [_] 8. 4. Vendor Specific Information */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NETBIOS_                44U /* [_] 8. 5. NetBIOS over TCP/IP Name Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NETBIOS_SERVERS         45U /* [_] 8. 6. NetBIOS over TCP/IP Datagram Distribution Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NETBIOS_NODE_TYPE       46U /* [_] 8. 7. NetBIOS over TCP/IP Node Type Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NETBIOS_SCOPE           47U /* [_] 8. 8. NetBIOS over TCP/IP Scope Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_XWINDOW_FONT_SERVER     48U /* [_] 8. 9. X Window System Font Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_XWINDOW_DISP_MGR        49U /* [_] 8.10. X Window System Display Manager Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NIS_DOMAIN              64U /* [_] 8.11. Network Information Service+ Domain Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NIS_SERVERS             65U /* [_] 8.12. Network Information Service+ Servers Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MOBILE_IP_HOME_AGENT    68U /* [_] 8.13. Mobile IP Home Agent option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_SMTP_SERVER             69U /* [_] 8.14. Simple Mail Transport Protocol (SMTP) Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_POP3_SERVER             70U /* [_] 8.15. Post Office Protocol (POP3) Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_NNTP_SERVER             71U /* [_] 8.16. Network News Transport Protocol (NNTP) Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_WWW_SERVER              72U /* [_] 8.17. Default World Wide Web (WWW) Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_DEFAULT_FINGER_SERVER   73U /* [_] 8.18. Default Finger Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_IRC_SERVER              74U /* [_] 8.19. Default Internet Relay Chat (IRC) Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_ST_SERVER               75U /* [_] 8.20. StreetTalk Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_STDA_SERVER             76U /* [_] 8.21. StreetTalk Directory Assistance (STDA) Server Option */
#define TCPIP_DHCPV4SERVER_MSG_OPT_REQUESTED_IP_ADDR       50U /* [y] 9. 1. Requested IP Address */
#define TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME      51U /* [y] 9. 2. IP Address Lease Time */
#define TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD         52U /* [y] 9. 3. Option Overload (RX only) */
#define TCPIP_DHCPV4SERVER_MSG_OPT_TFTP_SERVER_NAME        66U /* [_] 9. 4. TFTP server name */
#define TCPIP_DHCPV4SERVER_MSG_OPT_BOOTFILE_NAME           67U /* [_] 9. 5. Bootfile name */
#define TCPIP_DHCPV4SERVER_MSG_OPT_DHCP_MSG_TYPE           53U /* [y] 9. 6. DHCP Message Type */
#define TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID               54U /* [y] 9. 7. Server Identifier */
#define TCPIP_DHCPV4SERVER_MSG_OPT_PARAM_REQUEST_LIST      55U /* [y] 9. 8. Parameter Request List */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MESSAGE                 56U /* [y] 9. 9. Message */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MAX_DHCP_MSG_SIZE       57U /* [_] 9.10. Maximum DHCP Message Size */
#define TCPIP_DHCPV4SERVER_MSG_OPT_RENEVAL_TIME_T1         58U /* [_] 9.11. Renewal (T1) Time Value */
#define TCPIP_DHCPV4SERVER_MSG_OPT_REBINDING_TIME_T2       59U /* [_] 9.12. Rebinding (T2) Time Value */
#define TCPIP_DHCPV4SERVER_MSG_OPT_VENDOR_CLASS_ID         60U /* [_] 9.13. Vendor class identifier */
#define TCPIP_DHCPV4SERVER_MSG_OPT_CLIENT_ID               61U /* [y] 9.14. Client-identifier */

#define TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN                  2U /* All options except PAD and END option have a header with to fields 'Code' (1 byte) and 'Len' (1 byte) [RFC2132 2. BOOTP Extension/DHCP Option Field Format] */
#define TCPIP_DHCPV4SERVER_MSG_OPT_DHCP_MSG_TYPE_LEN        1U /* Option payload is always 1 byte */
#define TCPIP_DHCPV4SERVER_MSG_OPT_SUBNET_MASK_LEN          4U /* Option payload is always 4 bytes */
#define TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID_LEN            4U /* Option payload is always 4 bytes */
#define TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME_LEN   4U /* Option payload is always 4 bytes */
#define TCPIP_DHCPV4SERVER_MSG_OPT_REQUESTED_IP_ADDR_LEN    4U /* Option payload is always 4 bytes */
#define TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD_LEN      1U /* Option payload is always 1 bytes */
#define TCPIP_DHCPV4SERVER_MSG_OPT_MESSAGE_MIN_LEN          1U /* Option payload is at least 1 bytes */

/* Internal flags of requestable (supported) options. These flags are set in the ClientParamReqSet (TcpIp_DhcpV4Server_AddrLeaseRecType) */
#define TCPIP_DHCPV4SERVER_MSG_OPT_REQ_FLAG_SUBNET_MASK    (1<<0)
#define TCPIP_DHCPV4SERVER_MSG_OPT_REQ_FLAG_ROUTER         (1<<1)

#define TCPIP_DHCPV4SERVER_REQUESTABLE_OPTIONS_COUNT        2U

#define TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD_FILE     1U
#define TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD_SNAME    2U

#define TCPIP_DHCPV4SERVER_MSG_INVALID                      0U /* Indicates that the message has no or an invalid DHCP message type */
#define TCPIP_DHCPV4SERVER_RX_MSG_DHCPDISCOVER              1U /* RX / Client broadcast to locate available servers. */
#define TCPIP_DHCPV4SERVER_TX_MSG_DHCPOFFER                 2U /* TX / Server to client in response to DHCPDISCOVER with offer of configuration parameters.*/
#define TCPIP_DHCPV4SERVER_RX_MSG_DHCPREQUEST               3U /* RX / Client message to servers either (a) requesting offered parameters from one server and implicitly
                                                                *      declining offers from all others, (b) confirming correctness of previously allocated address after,
                                                                *      e.g., system reboot, or (c) extending the lease on a particular network address. */
#define TCPIP_DHCPV4SERVER_RX_MSG_DHCPDECLINE               4U /* RX / Client to server indicating network address is already in use. */
#define TCPIP_DHCPV4SERVER_TX_MSG_DHCPACK                   5U /* TX / Server to client with configuration parameters, including committed network address. */
#define TCPIP_DHCPV4SERVER_TX_MSG_DHCPNAK                   6U /* TX / Server to client indicating client's notion of network address is incorrect
                                                                *      (e.g., client has moved to new subnet) or client's lease as expired */
#define TCPIP_DHCPV4SERVER_RX_MSG_DHCPRELEASE               7U /* RX / Client to server relinquishing network address and canceling remaining lease. */
#define TCPIP_DHCPV4SERVER_RX_MSG_DHCPINFORM                8U /* RX / Client to server, asking only for local configuration parameters; client already has externally configured network address. */

#define TCPIP_DHCPV4SERVER_SERVER_PORT                     67U /* "DHCP messages from a client to a server are sent to the 'DHCP server' port (67)" [RFC2131 4.1 Constructing and sending DHCP messages] */
#define TCPIP_DHCPV4SERVER_CLIENT_PORT                     68U /* "DHCP messages from a server to a client are sent to the 'DHCP client' port (68)" [RFC2131 4.1 Constructing and sending DHCP messages] */

#define TCPIP_DHCPV4SERVER_ERROR_MESSAGE_NONE                          0U
#define TCPIP_DHCPV4SERVER_ERROR_MESSAGE_CLIENT_MOVED_SUBNET           1U
#define TCPIP_DHCPV4SERVER_ERROR_MESSAGE_INVALID_IP_ADDRESS_REQUESTED  2U
#define TCPIP_DHCPV4SERVER_ERROR_MESSAGE_COUNT                         2U /* Number of different error messages, the server may return in DHCKNAK messages. */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

/* Byte-Order conversion macros */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  /* Host byte-order is big endian (network byte-order) */
# define TCPIP_DHCPV4SERVER_UINT8_HTONS(UINT8_VALUE) (UINT8_VALUE)
# define TCPIP_DHCPV4SERVER_UINT16_HTONL(UINT16_VALUE) (UINT16_VALUE)
#else
  /* Host byte-order is little endian */
# define TCPIP_DHCPV4SERVER_UINT8_HTONS(UINT8_VALUE)   ((uint16)(  ((uint16)(UINT8_VALUE) << 8)))
# define TCPIP_DHCPV4SERVER_UINT16_HTONL(UINT16_VALUE) ((uint32)( (((uint32)(UINT16_VALUE) & 0x0000FF00u) <<  8) | \
                                                    (((uint32)(UINT16_VALUE) & 0x000000FFu) << 24) ))
#endif

#define TcpIp_DhcpV4Server_VCfgGetServerSocketId(SERVER_INSTANCE_IDX)                    (TcpIp_DhcpV4Server_SocketId[(SERVER_INSTANCE_IDX)])
#define TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(SERVER_INSTANCE_IDX)                   (TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[(SERVER_INSTANCE_IDX)].ServerConfigPtr)
#define TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecArray(SERVER_INSTANCE_IDX)                 (TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[(SERVER_INSTANCE_IDX)].AddrLeaseRecsPtr)
#define TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecPtr(SERVER_INSTANCE_IDX, ADDR_LEASE_IDX)  (&TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[(SERVER_INSTANCE_IDX)].AddrLeaseRecsPtr[ADDR_LEASE_IDX])

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef struct {
  uint16                                                           TxData_Length;
  uint8                                                            TxData_ServerInstanceIdx;
  uint8                                                            TxData_AddrAssignmentCfgIdx;
  uint8                                                            TxData_AddrLeaseRecIdx;
  uint8                                                            TxData_MsgType;
} TcpIp_DhcpV4Server_TxDataInfoType;

typedef struct {
  TCPIP_DHCPV4SERVER_P2C(uint8) Text;
  uint8                         Length;
} TcpIp_DhcpV4Server_ErrorMsgType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/* ==== VAR_ZERO_INIT_8BIT ==== */

#define TCPIP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Initialization state of the module */
STATIC VAR(uint8, TCPIP_VAR_ZERO_INIT) TcpIp_DhcpV4Server_State = TCPIP_DHCPV4SERVER_STATE_UNINIT;

#define TCPIP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ==== VAR_NOINIT_16BIT ==== */

#define TCPIP_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(uint16, TCPIP_VAR_NOINIT) TcpIp_DhcpV4Server_TimeMsec;

#define TCPIP_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ==== VAR_NOINIT_32BIT ==== */

#define TCPIP_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(uint32, TCPIP_VAR_NOINIT) TcpIp_DhcpV4Server_TimeSec;

#define TCPIP_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ==== VAR_NOINIT_UNSPECIFIED ==== */

#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(TcpIp_DhcpV4Server_TxDataInfoType, TCPIP_VAR_NOINIT) TcpIp_DhcpV4Server_TxDataInfo;

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ==== CONST_UNSPECIFIED ==== */

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA S 0310 3 */ /* MD_TcpIp_DhcpV4Server_0310 */
STATIC CONST(TcpIp_DhcpV4Server_ErrorMsgType, TCPIP_CONST) TcpIp_DhcpV4Server_ErrorMsgs[TCPIP_DHCPV4SERVER_ERROR_MESSAGE_COUNT] = { /* PRQA S 3218 */ /* MD_TcpIp_DhcpV4Server_3218 */
  { (TCPIP_DHCPV4SERVER_P2C(uint8))"Client moved subnet.",          20 }, 
  { (TCPIP_DHCPV4SERVER_P2C(uint8))"Invalid IP address requested.", 29 },
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define TCPIP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VInitAddrLeaseRec()
 **********************************************************************************************************************/
/*! \brief        Initializes/Invalidates an Address Lease Record.
 *  \details      This function sets the client identifier to 0, so that this record is not associated with a client anymore.
 *  \param[inout] addrLeaseRecPtr    IP address lease record.
 *  \param[in]    clientIdPtr        Client identifier (array of bytes).
 *  \param[in]    clientIdLen        Length of the client identifier in bytes.
 *  \context      TASK
 *  \reentrant    FALSE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VInitAddrLeaseRec(
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr,
  TCPIP_DHCPV4SERVER_P2C(uint8)                               clientIdPtr,
  uint8                                                       clientIdLen);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetAddrAssignmentCfgIdxForMacAddr()
 **********************************************************************************************************************/
/*! \brief        Retrieves a suitable address assignment configuration based on the clients MAC address.
 *  \details      If no MAC address specific configuration is found this function returns a default configuration or
 *                no configuration if there is no default.
 *  \param[in]    serverInstanceIdx  DHCP server instance.
 *  \param[in]    physAddrPtr        Physical (MAC) address of the client (6 byte).
 *  \return                          AddressAssignmentConfigIndex or TCPIP_DHCPV4SERVER_INV_IDX if no matching
 *                                   configuration was found.
 *  \context      TASK
 *  \reentrant    FALSE
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetAddrAssignmentCfgIdxForMacAddr(
  uint8                                                       serverInstanceIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                               physAddrPtr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetServerInstanceIdxForSocket()
 **********************************************************************************************************************/
/*! \brief        Retrieves the corresponding DHCP server instance for a UDP socket handle.
 *  \details      -
 *  \param[in]    SoHandle  Handle of the TcpIp (UDP) socket.
 *  \return                 Index of the DHCP server instance or TCPIP_DHCPV4SERVER_INV_IDX if the socket does not belong
 *                          to a DHCP server.
 *  \context      TASK
 *  \reentrant    FALSE
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetServerInstanceIdxForSocket(TcpIp_SockHndType SoHandle); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VCopyAndPad()
 **********************************************************************************************************************/
/*! \brief        Copies bytes from source to destination.
 *  \details      If source is shorter that the destination, the destination will be filled up with zeroes.
 *  \param[in]    dstPtr    Destination buffer.
 *  \param[out]   dstLen    Length of destination in bytes (MUST at least be equal to srcLen).
 *  \param[in]    srcPtr    Source buffer.
 *  \param[in]    srcLen    Length of source in bytes.
 *  \return                 Index of the DHCP server instance or TCPIP_DHCPV4SERVER_INV_IDX if the socket does not belong
 *                          to a DHCP server.
 *  \context      TASK
 *  \reentrant    FALSE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VCopyAndPad(
  TCPIP_DHCPV4SERVER_P2V(uint8) dstPtr,
  uint16                        dstLen,
  TCPIP_DHCPV4SERVER_P2C(uint8) srcPtr,
  uint16                        srcLen);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VAddOptions()
 **********************************************************************************************************************/
/*! \brief        Adds DHCP options to a message based on the received client information and the message type.
 *  \details      -
 *  \param[in]    ServerInstanceIdx    DHCP server instance.
 *  \param[out]   BufPtr               DHCP message buffer. (May be NULL_PTR in order to perform a dry run that
 *                                     calculates the required buffer size)
 *  \param[in]    AddrLeaseRecIdx      Client address lease record.
 *  \param[in]    MsgType              DHCP Message Type.
 *  \return                            Length of all added options in byte.
 *  \context      TASK
 *  \reentrant    FALSE
 **********************************************************************************************************************/
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4Server_VAddOptions(
  uint8                                                       ServerInstanceIdx,
  TCPIP_DHCPV4SERVER_P2V(uint8)                               BufPtr,
  uint8                                                       AddrLeaseRecIdx,
  uint8                                                       MsgType);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetServerAddr()
 **********************************************************************************************************************/
/*! \brief        Returns the IP address of the specified DHCP server instance.
 *  \details      -
 *  \param[in]    serverInstanceIdx    DHCP server instance.
 *  \return                            IPv4 address of the DHCP server instance.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(IpBase_AddrInType, TCPIP_CODE) TcpIp_DhcpV4Server_VGetServerAddr(
  uint8 serverInstanceIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetClientAddr()
 **********************************************************************************************************************/
/*! \brief        Returns the IP address corresponding to the specified address lease record.
 *  \details      -
 *  \param[in]    addrAssignmentCfgPtr    Address assignment configuration.
 *  \param[in]    addrLeaseRecIdx         Client address lease record.
 *  \return                               Client IPv4 address corresponding to the address lease record.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(IpBase_AddrInType, TCPIP_CODE) TcpIp_DhcpV4Server_VGetClientAddr(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgPtr,
  uint8                                                            addrLeaseRecIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VTxMessage()
 **********************************************************************************************************************/
/*! \brief        Transmits a DHCP message to the client.
 *  \details      -
 *  \param[in]    serverInstanceIdx     DHCP server instance.
 *  \param[in]    addrAssignmentCfgIdx  IP address assignment configuration.
 *  \param[in]    addrLeaseRecIdx       Client address lease record.
 *  \param[in]    msgType               DHCP Message Type.
 *  \return       E_OK                  Message successfully transmitted.
 *  \return       E_NOT_OK              Transmit failed.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VTxMessage(
  uint8                                                            serverInstanceIdx,
  uint8                                                            addrAssignmentCfgIdx,
  uint8                                                            addrLeaseRecIdx,
  uint8                                                            msgType);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VSetupUdpSocket()
 **********************************************************************************************************************/
/*! \brief        Opens and binds a DHCPv4 server UDP socket to the specified IP address.
 *  \details      -
 *  \param[in]    BindLocalAddrId  Id of the local address the socket shall be bind to.
 *  \return                        TcpIp socket handle to the DHCP server socket.
 *  \return                        TCPIP_INV_SOCKET_ID if the socket could not be opened or bound.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(TcpIp_SocketIdType, TCPIP_CODE) TcpIp_DhcpV4Server_VSetupUdpSocket(TcpIp_LocalAddrIdType BindLocalAddrId);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VInitSocketConfig
 **********************************************************************************************************************/
/*! \brief        Initializes all IP address assignment configurations for a socket (DHCP server instance).
 *  \details      Resets all address lease records of all address assignment configurations for the specified socket.
 *  \param[in]    SocketCfgsIdx     DHCP server socket configuration (Not a socket handle).
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VInitSocketConfig(uint8 SocketCfgsIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VOpenSocket
 **********************************************************************************************************************/
/*! \brief        Opens the specified DHCP server UDP socket.
 *  \details      -
 *  \param[in]    socketCfgsIdx     DHCP server socket configuration (Not a socket handle).
 *  \return       E_OK              Socket successfully opened.
 *  \return       E_NOT_OK          Socket could not be opened.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VOpenSocket(uint8 socketCfgsIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VCloseSocket
 **********************************************************************************************************************/
/*! \brief        Closes the specified DHCP server UDP socket.
 *  \details      -
 *  \param[in]    socketCfgsIdx     DHCP server socket configuration (Not a socket handle).
 *  \return       E_OK              Socket closed.
 *  \return       E_NOT_OK          Socket could not be closed.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VCloseSocket(uint8 socketCfgsIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VUpdateLeaseTime
 **********************************************************************************************************************/
/*! \brief        Updates the lease time of an address lease record based on the requested lease time.
 *  \details      -
 *  \param[in]    serverCfgPtr        Configuration of the DHCP server instance.
 *  \param[in]    addrLeaseRecPtr     Address lease record.
 *  \param[in]    requestedLeaseTime  Requested lease time in seconds (0 means default).
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VUpdateLeaseTime(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType) serverCfgPtr,
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr, 
  uint32                                                      requestedLeaseTime);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VParseParamReqestListOption
 **********************************************************************************************************************/
/*! \brief        Parses a received 'Parameter Request List' option and returns a set of requested and supported options.
 *  \details      -
 *  \param[in]    msgDataPtr        Received DHCP message.
 *  \param[in]    msgDataOfs        Position of the 'Parameter Request List' option payload in received DHCP message (byte).
 *  \param[in]    optLen            Length of parameter request list in bytes.
 *  \return                         Set (bit-field) of the requested and supported DHCP options.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VParseParamReqestListOption(
  TCPIP_DHCPV4SERVER_P2C(uint8) msgDataPtr, uint16 msgDataOfs, uint8 optLen);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VUpdateClientInfo
 **********************************************************************************************************************/
/*! \brief        Updates/stores client specific information in an address lease record.
 *  \details      -
 *  \param[in]    addrLeaseRecPtr   IP address lease record.
 *  \param[in]    msgDataPtr        Received DHCP message.
 *  \param[in]    msgDataLen        Length of received DHCP message in bytes.
 *  \param[in]    knownOptionsPtr   Value/Position of known options that are contained in the message.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VUpdateClientInfo(
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr, 
  TCPIP_DHCPV4SERVER_P2C(uint8)                               msgDataPtr, 
  uint16                                                      msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetAddrLeaseRecIdxByIpAddr
 **********************************************************************************************************************/
/*! \brief        Retrieves an address lease record based on the IP address of the client.
 *  \details      -
 *  \param[in]    addrAssignmentCfgPtr  IP address assignment configuration.
 *  \param[in]    clientAddr            IPv4 address of the client.
 *  \return                             IP address lease record or TCPIP_DHCPV4SERVER_INV_IDX if IP address does not
 *                                      belong to any record.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetAddrLeaseRecIdxByIpAddr(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgPtr,
  IpBase_AddrInType                                                clientAddr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx
 **********************************************************************************************************************/
/*! \brief        Searches (and optionally creates) an address lease record using the client identifier.
 *  \details      If there exists no record for the client, the record belonging to the requested IP address will be
 *                provided if is is not already in use.
 *  \param[in]    serverInstanceIdx     DHCP server instance,
 *  \param[in]    addrAssignmentCfgIdx  IP address assignment configuration.
 *  \param[in]    clientIdPtr           Client identifier.
 *  \param[in]    clientIdLen           Length of client identifier in bytes.
 *  \param[in]    reqIpAddr             Requested IPv4 address or 0 if no specific address is requested.
 *  \param[in]    createIfNotExist      Specifies whether a free entry shall be returned and assigned to the client if possible.
 *  \return                             Index of the IP address lease record or TCPIP_DHCPV4SERVER_INV_IDX if no lease
 *                                      exists or could be assigned.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx(
  uint8                          serverInstanceIdx,
  uint8                          addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)  clientIdPtr,
  uint8                          clientIdLen,
  IpBase_AddrInType              reqIpAddr,
  boolean                        createIfNotExist); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VCmpClientId
 **********************************************************************************************************************/
/*! \brief        Checks if a client identifier is bound to a specific IP address lease record.
 *  \details      -
 *  \param[in]    addrLeaseRecPtr       IP address lease record.
 *  \param[in]    clientIdPtr           Client identifier.
 *  \param[in]    clientIdLen           Length of the client identifier in bytes.
 *  \param[in]    clientIdLen           Length of client identifier in bytes.
 *  \return       TRUE                  Client identifier belongs the the IP address lease record.
 *  \return       FALSE                 Client identifier does not belong the the IP address lease record.
 *  \context      TASK
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV4Server_VCmpClientId(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr,
  TCPIP_DHCPV4SERVER_P2C(uint8)                               clientIdPtr,
  uint8                                                       clientIdLen);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VHandleDhcpDiscover
 **********************************************************************************************************************/
/*! \brief        Handles received DHCP DISCOVER message.
 *  \details      -
 *  \param[in]    serverInstanceIdx     DHCP server instance.
 *  \param[in]    addrAssignmentCfgIdx  IP address assignment configuration.
 *  \param[in]    msgDataPtr            Received DHCP message.
 *  \param[in]    msgDataLen            Length of received DHCP message in bytes.
 *  \param[in]    knownOptionsPtr       Value/Position of known options that are contained in the message.
 *  \context      ISR
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VHandleDhcpDiscover(
  uint8                                                           serverInstanceIdx, 
  uint8                                                           addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                                   msgDataPtr, 
  uint16                                                          msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VHandleDhcpRequest
 **********************************************************************************************************************/
/*! \brief        Handles received DHCP REQUEST message.
 *  \details      -
 *  \param[in]    serverInstanceIdx     DHCP server instance.
 *  \param[in]    addrAssignmentCfgIdx  IP address assignment configuration.
 *  \param[in]    msgDataPtr            Received DHCP message.
 *  \param[in]    msgDataLen            Length of received DHCP message in bytes.
 *  \param[in]    knownOptionsPtr       Value/Position of known options that are contained in the message.
 *  \context      ISR
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VHandleDhcpRequest(
  uint8                                                           serverInstanceIdx, 
  uint8                                                           addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                                   msgDataPtr, 
  uint16                                                          msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VHandleDhcpDeclineRelease
 **********************************************************************************************************************/
/*! \brief        Handles received DHCP DECLINE or RELEASE message.
 *  \details      -
 *  \param[in]    serverInstanceIdx     DHCP server instance.
 *  \param[in]    addrAssignmentCfgIdx  IP address assignment configuration.
 *  \param[in]    msgDataPtr            Received DHCP message.
 *  \param[in]    msgDataLen            Length of received DHCP message in bytes.
 *  \param[in]    knownOptionsPtr       Value/Position of known options that are contained in the message.
 *  \context      ISR
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VHandleDhcpDeclineRelease(
  uint8                                                           serverInstanceIdx, 
  uint8                                                           addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                                   msgDataPtr, 
  uint16                                                          msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VInitKnownOptionsInfo
 **********************************************************************************************************************/
/*! \brief        Initializes a known DHCP options structure.
 *  \details      -
 *  \param[in]    knownOptionsPtr       Structure containing values or position of known DHCP options.
 *  \context      ANY
 *  \reentrant    TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VInitKnownOptionsInfo(TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VParseOptions
 **********************************************************************************************************************/
/*! \brief        Parses all DHCP options contained in a received message.
 *  \details      -
 *  \param[in]    msgDataPtr       Received DHCP message.
 *  \param[in]    msgDataOfs       Position of the first option in the message that shall be parsed.
 *  \param[in]    msgDataLen       Length of the complete message in bytes.
 *  \param[inout] knownOptionsPtr  Structure containing values or position of known DHCP options.
 *  \context    ANY
 *  \reentrant  TRUE
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VParseOptions(
  TCPIP_DHCPV4SERVER_P2C(uint8) msgDataPtr, uint16 msgDataOfs, uint16 msgDataLen, 
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VInitAddrLeaseRec()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VInitAddrLeaseRec(
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr,
  TCPIP_DHCPV4SERVER_P2C(uint8)                               clientIdPtr,
  uint8                                                       clientIdLen)
{
  addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_INIT;

  if (clientIdLen > TCPIP_DHCPV4SERVER_MAX_CLIENT_ID_LEN)
  {
    /* The client identifier is longer than the configured storage space.
     * Identifier will be truncated. This is no problem if the truncated identifier is still unique
     * but it is highly recommended to configure more storage space for the client identifiers */
    clientIdLen = TCPIP_DHCPV4SERVER_MAX_CLIENT_ID_LEN;
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_GET_CLIENT_REC_IDX, TCPIP_DHCPV4SERVER_E_OUT_OF_MEMORY); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* create new record */
  addrLeaseRecPtr->ClientIdLen = clientIdLen;

  /* PRQA S 0310, 3305 2 */ /* MD_TcpIp_DhcpV4Server_0310, MD_TcpIp_DhcpV4Server_3305 */
  IpBase_Copy((IpBase_CopyDataType *)&addrLeaseRecPtr->ClientId[0],
        (const IpBase_CopyDataType *)clientIdPtr, clientIdLen);

} /* TcpIp_DhcpV4Server_VInitAddrLeaseRec() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VCmpClientId()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV4Server_VCmpClientId(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr,
  TCPIP_DHCPV4SERVER_P2C(uint8)                               clientIdPtr,
  uint8                                                       clientIdLen)
{
  boolean result;

  if (clientIdLen > TCPIP_DHCPV4SERVER_MAX_CLIENT_ID_LEN)
  {
    /* The client identifier is longer than the configured storage space.
     * Identifier will be truncated. This is no problem if the truncated identifier is still unique
     * but it is highly recommended to configure more storage space for the client identifiers */
    clientIdLen = TCPIP_DHCPV4SERVER_MAX_CLIENT_ID_LEN;
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_GET_CLIENT_REC_IDX, TCPIP_DHCPV4SERVER_E_OUT_OF_MEMORY); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (addrLeaseRecPtr->ClientIdLen == clientIdLen)
  {
    uint8_least clientIdDataIdx;
    result = TRUE;

    for (clientIdDataIdx = 0; clientIdDataIdx < clientIdLen; clientIdDataIdx++)
    {
      if (addrLeaseRecPtr->ClientId[clientIdDataIdx] != clientIdPtr[clientIdDataIdx])
      {
        /* client identifier does not match */
        result = FALSE;
        break;
      }
    }
  }
  else
  {
    result = FALSE;
  }

  return result;
} /* TcpIp_DhcpV4Server_VCmpClientId() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx(
  uint8                          serverInstanceIdx,
  uint8                          addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)  clientIdPtr,
  uint8                          clientIdLen,
  IpBase_AddrInType              reqIpAddr,
  boolean                        createIfNotExist)
{
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      serverCfgPtr        = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(serverInstanceIdx);
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType)      serverAddrLeaseRecs = TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecArray(serverInstanceIdx);
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgPtr;
  uint8_least                                                      addrLeaseRecIdxMatch  = TCPIP_DHCPV4SERVER_INV_IDX;
  uint8_least                                                      addrLeaseRecIdxNew    = TCPIP_DHCPV4SERVER_INV_IDX;      
  uint8_least                                                      addrLeaseRecIdx;

  TCPIP_ASSERT_MSG(addrAssignmentCfgIdx < serverCfgPtr->AddrAssignmentCfgCount, "addrAssignmentCfgIdx < serverCfgPtr->AddrAssignmentCfgCount");

  addrAssignmentCfgPtr = &serverCfgPtr->AddrAssignmentCfgsPtr[addrAssignmentCfgIdx];

  {
    const uint8 addrLeaseRecIdxFirst = (uint8)(addrAssignmentCfgPtr->AddrLeaseRecStartOfs);
    const uint8 addrLeaseRecIdxEnd   = (uint8)(addrLeaseRecIdxFirst + addrAssignmentCfgPtr->DhcpAddrCount);

    for (addrLeaseRecIdx = addrLeaseRecIdxFirst; addrLeaseRecIdx < addrLeaseRecIdxEnd; addrLeaseRecIdx++)
    { 
      if (TcpIp_DhcpV4Server_VCmpClientId(&serverAddrLeaseRecs[addrLeaseRecIdx], clientIdPtr, clientIdLen) == TRUE)
      {
        /* Valid address lease record found. Stop searching. */
        addrLeaseRecIdxMatch = addrLeaseRecIdx;
        break;
      }

      /* Record oldest expired lease record. (will be required later if no matching lease is found) */
      if (serverAddrLeaseRecs[addrLeaseRecIdx].LeaseState == TCPIP_DHCPV4SERVER_LEASE_STATE_EXPIRED)
      {
        if ((TCPIP_DHCPV4SERVER_INV_IDX == addrLeaseRecIdxNew) || (serverAddrLeaseRecs[addrLeaseRecIdx].LeaseTimeSec < serverAddrLeaseRecs[addrLeaseRecIdxNew].LeaseTimeSec))
        {
          addrLeaseRecIdxNew = addrLeaseRecIdx;
        }
      }
    }
  }

  if (addrLeaseRecIdxMatch == TCPIP_DHCPV4SERVER_INV_IDX)
  {
    /* No matching address lease record found. */
    if (reqIpAddr != 0)
    {
      /* Client has explicitly requested an IP address. Check if requested IP address is available. */
      uint8 addrLeaseRecIdxRequested = TcpIp_DhcpV4Server_VGetAddrLeaseRecIdxByIpAddr(addrAssignmentCfgPtr, reqIpAddr);

      if (addrLeaseRecIdxRequested != TCPIP_DHCPV4SERVER_INV_IDX)
      {
        if (serverAddrLeaseRecs[addrLeaseRecIdxRequested].LeaseState == TCPIP_DHCPV4SERVER_LEASE_STATE_EXPIRED)
        {
          /* Requested IP address is available. */
          addrLeaseRecIdxNew = addrLeaseRecIdxRequested;
        }
      }
    }

    if ((createIfNotExist == TRUE) && (addrLeaseRecIdxNew != TCPIP_DHCPV4SERVER_INV_IDX))
    {
      /* Assign address lease recied to client. */
      addrLeaseRecIdxMatch = addrLeaseRecIdxNew;
      TcpIp_DhcpV4Server_VInitAddrLeaseRec(&serverAddrLeaseRecs[addrLeaseRecIdxMatch], clientIdPtr, clientIdLen);
    }
  }

  return (uint8)addrLeaseRecIdxMatch;
} /* TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010, 6030, 6060 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetAddrAssignmentCfgIdxForMacAddr()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetAddrAssignmentCfgIdxForMacAddr(
  uint8                                                       serverInstanceIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                               physAddrPtr)
{
  /* Get switch port information and check if there is an address assignment configuration */
#if defined(TCPIP_DHCPV4SERVER_GET_PORT_MAC_ADDR_FUNC)
  uint8 ethIfSwitchIdx     = 0;  /* PRQA S 0781 */ /* MD_TcpIp_DhcpV4Server_0781 */
  uint8 ethIfSwitchPortIdx = 0;
  uint8 addrAssignmentCfgIdxMatch = TCPIP_DHCPV4SERVER_INV_IDX;

  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      serverCfgPtr          = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(serverInstanceIdx);
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgsPtr = serverCfgPtr->AddrAssignmentCfgsPtr;  /* PRQA S 0781 */ /* MD_TcpIp_DhcpV4Server_0781 */

  if (E_OK == TCPIP_DHCPV4SERVER_GET_PORT_MAC_ADDR_FUNC(physAddrPtr, &ethIfSwitchIdx, &ethIfSwitchPortIdx))
  {
    /* Switch port information available for the clients MAC address */

    if (serverCfgPtr->EthIfSwitchIdx == ethIfSwitchIdx)
    {
      uint8_least addrAssignmentCfgIdx;

      for (addrAssignmentCfgIdx = 0; addrAssignmentCfgIdx < serverCfgPtr->AddrAssignmentCfgCount; addrAssignmentCfgIdx++)
      {
        if (addrAssignmentCfgsPtr[addrAssignmentCfgIdx].EthSwitchPortIdx == ethIfSwitchPortIdx)
        {
          /* Found address assignment configuration */
          addrAssignmentCfgIdxMatch = (uint8)addrAssignmentCfgIdx;
        }
      }
    }
  }

  if (addrAssignmentCfgIdxMatch == TCPIP_DHCPV4SERVER_INV_IDX)
  {
    /* no default address assignment configuration found.
     * Check if a default address assignment configuration exists (will always be the at index 0)
     */
    if (addrAssignmentCfgsPtr[0].EthSwitchPortIdx == TCPIP_DHCPV4SERVER_ANY_SWITCH_OR_PORT_IDX)
    {
      /* there exists no switch independent address assignment configuration */
      addrAssignmentCfgIdxMatch = 0;
    }
  }

  return addrAssignmentCfgIdxMatch;
#else
  /* PRQA S 3112, 3199 2 */ /* MD_MSR_14.2 */
  TCPIP_DHCPV4SERVER_UNUSED_PARAMETER(serverInstanceIdx);
  TCPIP_DHCPV4SERVER_UNUSED_PARAMETER(physAddrPtr);
  /* return default address assignment configuration */
  return 0;
#endif
} /* TcpIp_DhcpV4Server_VGetAddrAssignmentCfgIdxForMacAddr() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetServerInstanceIdxForSocket()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetServerInstanceIdxForSocket(TcpIp_SockHndType SoHandle)
{
  uint8 EntryIdx = TCPIP_DHCPV4SERVER_SOCKET_COUNT;

  while (0 < EntryIdx)
  {
    EntryIdx--;

    if (TcpIp_DhcpV4Server_SocketId[EntryIdx] == SoHandle)
    {
      return EntryIdx;
    }
  }

  return TCPIP_DHCPV4SERVER_INV_IDX;
} /* TcpIp_DhcpV4Server_VGetServerInstanceIdxForSocket() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VCopyAndPad()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VCopyAndPad(
  TCPIP_DHCPV4SERVER_P2V(uint8) dstPtr,
  uint16                        dstLen,
  TCPIP_DHCPV4SERVER_P2C(uint8) srcPtr,
  uint16                        srcLen)

{
  uint16_least bufPos;

  TCPIP_ASSERT( dstPtr != NULL_PTR);
  TCPIP_ASSERT((srcPtr != NULL_PTR) || (srcLen == 0));
  TCPIP_ASSERT( dstLen >= srcLen);

  if (NULL_PTR != srcPtr)
  {
    for (bufPos = 0; bufPos < srcLen; bufPos++)
    {
      dstPtr[bufPos] = srcPtr[bufPos];
    }
  }

  for (bufPos = srcLen; bufPos < dstLen; bufPos++)
  {
    dstPtr[bufPos] = 0;
  }

} /* TcpIp_DhcpV4Server_VCopyAndPad() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VAddOptions()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4Server_VAddOptions(
  uint8                                                       ServerInstanceIdx,
  TCPIP_DHCPV4SERVER_P2V(uint8)                               BufPtr,
  uint8                                                       AddrLeaseRecIdx,
  uint8                                                       MsgType)
{
  uint16 DataLen = 0;

  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      ServerCfgPtr    = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(ServerInstanceIdx);
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType)      AddrLeaseRecPtr = &TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecArray(ServerInstanceIdx)[AddrLeaseRecIdx];

  /* [RFC2132 9.7. Server Identifier] */
  if (NULL_PTR != BufPtr)
  {
    IpBase_AddrInType serverAddr = TcpIp_DhcpV4Server_VGetServerAddr(ServerInstanceIdx);

    BufPtr[DataLen + 0] = TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID;
    BufPtr[DataLen + 1] = TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID_LEN;

    IPBASE_PUT_UINT32(BufPtr, DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN, serverAddr);
  }
  DataLen += TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID_LEN;

  /* [RFC2132 9.14. Client-identifier] */
  if (NULL_PTR != BufPtr)
  {
    BufPtr[DataLen + 0] = TCPIP_DHCPV4SERVER_MSG_OPT_CLIENT_ID;
    BufPtr[DataLen + 1] = AddrLeaseRecPtr->ClientIdLen;

    TcpIp_DhcpV4Server_VCopyAndPad(&BufPtr[DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN], AddrLeaseRecPtr->ClientIdLen,
                                   &AddrLeaseRecPtr->ClientId[0],                         AddrLeaseRecPtr->ClientIdLen);
  }
  DataLen += TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + AddrLeaseRecPtr->ClientIdLen;

  /* [RFC2132 9.2. IP Address Lease Time] */
  if ((TCPIP_DHCPV4SERVER_TX_MSG_DHCPOFFER == MsgType) || (TCPIP_DHCPV4SERVER_TX_MSG_DHCPACK == MsgType))
  {
    if (NULL_PTR != BufPtr)
    {
      BufPtr[DataLen + 0] = TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME;
      BufPtr[DataLen + 1] = TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME_LEN;

      if (TCPIP_DHCPV4SERVER_TIME_INFINITE > AddrLeaseRecPtr->LeaseTimeSec)
      {
        uint32 LeaseTimeRemainSec = AddrLeaseRecPtr->LeaseTimeSec - TcpIp_DhcpV4Server_TimeSec;
        IPBASE_PUT_UINT32(BufPtr, DataLen+TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN, LeaseTimeRemainSec);
      }
      else
      {
        /* Write infinite time value */
        BufPtr[DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + 0] = 0xFF;
        BufPtr[DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + 1] = 0xFF;
        BufPtr[DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + 2] = 0xFF;
        BufPtr[DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + 3] = 0xFF;
      }
    }
    DataLen += TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME_LEN;
  }

  /* "If both the subnet mask and the router option are specified in a DHCP reply,
   *  the subnet mask option MUST be first." [RFC2132 3.3. Subnet Mask]
   */

  /* [RFC2132 3.3. Subnet Mask] */
  if ((TCPIP_DHCPV4SERVER_TX_MSG_DHCPOFFER == MsgType) ||
      (TCPIP_DHCPV4SERVER_TX_MSG_DHCPACK   == MsgType) ||
      (0 != (AddrLeaseRecPtr->ClientParamReqSet & TCPIP_DHCPV4SERVER_MSG_OPT_REQ_FLAG_SUBNET_MASK)))
  {
    if (0 != ServerCfgPtr->AddrPrefixLength)
    {
      if (NULL_PTR != BufPtr)
      {
        uint32 SubnetMask = (0xFFFFFFFFU << (32U - ServerCfgPtr->AddrPrefixLength));

        BufPtr[DataLen + 0] = TCPIP_DHCPV4SERVER_MSG_OPT_SUBNET_MASK;
        BufPtr[DataLen + 1] = TCPIP_DHCPV4SERVER_MSG_OPT_SUBNET_MASK_LEN;
        IPBASE_PUT_UINT32(BufPtr, DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN, SubnetMask);
      }
      DataLen += TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + TCPIP_DHCPV4SERVER_MSG_OPT_SUBNET_MASK_LEN;
    }
  }

  /* [RFC2132 3.5. Router Option] */
  if (0 != (AddrLeaseRecPtr->ClientParamReqSet & TCPIP_DHCPV4SERVER_MSG_OPT_REQ_FLAG_ROUTER))
  {
    if (0 != ServerCfgPtr->DefaultRouterAddr)
    {
      if (NULL_PTR != BufPtr)
      {
        BufPtr[DataLen + 0] = TCPIP_DHCPV4SERVER_MSG_OPT_ROUTER;
        BufPtr[DataLen + 1] = 4;
        IPBASE_PUT_UINT32(BufPtr, DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN, ServerCfgPtr->DefaultRouterAddr);
      }
      DataLen += TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + 4;
    }
  }

  /* [RFC2132 9.9. Message] */
  TCPIP_ASSERT(AddrLeaseRecPtr->ErrorMsgId <= TCPIP_DHCPV4SERVER_ERROR_MESSAGE_COUNT);

  if (AddrLeaseRecPtr->ErrorMsgId > 0)
  {
    uint8_least msgIdx    = AddrLeaseRecPtr->ErrorMsgId - 1;
    uint8       msgLength = TcpIp_DhcpV4Server_ErrorMsgs[msgIdx].Length;
    
    TCPIP_ASSERT(MsgType == TCPIP_DHCPV4SERVER_TX_MSG_DHCPNAK); /* Error messages shall only be sent in NAK messages. */

    if (NULL_PTR != BufPtr)
    {
      BufPtr[DataLen + 0] = TCPIP_DHCPV4SERVER_MSG_OPT_MESSAGE;
      BufPtr[DataLen + 1] = msgLength;

      IpBase_Copy(&BufPtr[DataLen + TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN], &TcpIp_DhcpV4Server_ErrorMsgs[msgIdx].Text[0], msgLength);

      AddrLeaseRecPtr->ErrorMsgId = TCPIP_DHCPV4SERVER_ERROR_MESSAGE_NONE;
    }

    DataLen += TCPIP_DHCPV4SERVER_MSG_OPT_HDR_LEN + msgLength;
  }

  return DataLen;
} /* TcpIp_DhcpV4Server_VAddOptions() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetServerAddr()
 **********************************************************************************************************************/
STATIC FUNC(IpBase_AddrInType, TCPIP_CODE) TcpIp_DhcpV4Server_VGetServerAddr(
  uint8 serverInstanceIdx)
{
  IpBase_SockAddrInType ServerSockAddrIn;

  (void)TcpIp_GetIpAddr(TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[serverInstanceIdx].SoBindLocalAddrId, (TcpIp_SockAddrType*)&ServerSockAddrIn, NULL_PTR, NULL_PTR); /* PRQA S 0310 */ /* MD_TcpIp_DhcpV4Server_0310 */

  return IPBASE_HTON32(ServerSockAddrIn.sin_addr);
} /* TcpIp_DhcpV4Server_VGetServerAddr() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetClientAddr()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(IpBase_AddrInType, TCPIP_CODE) TcpIp_DhcpV4Server_VGetClientAddr(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgPtr,
  uint8                                                            addrLeaseRecIdx)
{
  uint8 dhcpAddrIdx;

  TCPIP_ASSERT(addrLeaseRecIdx >= addrAssignmentCfgPtr->AddrLeaseRecStartOfs);
  TCPIP_ASSERT(addrLeaseRecIdx <  addrAssignmentCfgPtr->AddrLeaseRecStartOfs + addrAssignmentCfgPtr->DhcpAddrCount);

  dhcpAddrIdx = (uint8)(addrLeaseRecIdx - addrAssignmentCfgPtr->AddrLeaseRecStartOfs);

  return (addrAssignmentCfgPtr->DhcpAddrFirst + dhcpAddrIdx);
} /* TcpIp_DhcpV4Server_VGetClientAddr() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VGetAddrLeaseRecIdxByIpAddr()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VGetAddrLeaseRecIdxByIpAddr(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgPtr,
  IpBase_AddrInType                                                clientAddr)
{
  uint8 addrLeaseRecIdx;

  if ((clientAddr >= addrAssignmentCfgPtr->DhcpAddrFirst) && (clientAddr < (addrAssignmentCfgPtr->DhcpAddrFirst + addrAssignmentCfgPtr->DhcpAddrCount)))
  {
    uint8 dhcpAddrIdx = (uint8)(clientAddr - addrAssignmentCfgPtr->DhcpAddrFirst);

    TCPIP_ASSERT(dhcpAddrIdx < addrAssignmentCfgPtr->DhcpAddrCount);

    addrLeaseRecIdx = (uint8)(addrAssignmentCfgPtr->AddrLeaseRecStartOfs + dhcpAddrIdx);
  }
  else
  {
    /* clientAddr is not in the IP address range of the given address assignment configuration. */
    addrLeaseRecIdx = TCPIP_DHCPV4SERVER_INV_IDX;
  }

  return addrLeaseRecIdx;
} /* TcpIp_DhcpV4Server_VGetAddrLeaseRecIdxByIpAddr() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VTxMessage()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VTxMessage(
  uint8                                                            serverInstanceIdx,
  uint8                                                            addrAssignmentCfgIdx,
  uint8                                                            addrLeaseRecIdx,
  uint8                                                            msgType)
{
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      serverCfgPtr         = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(serverInstanceIdx);
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgPtr = &serverCfgPtr->AddrAssignmentCfgsPtr[addrAssignmentCfgIdx];
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType)      addrLeaseRecPtr      = &TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecArray(serverInstanceIdx)[addrLeaseRecIdx];
  uint16                                                           msgLen = TCPIP_DHCPV4SERVER_MIN_MSG_LEN;
  TcpIp_SockAddrInetType                                           destSockAddr;
  Std_ReturnType                                                   retVal = E_OK;

  destSockAddr.domain = TCPIP_AF_INET;

  /* Set destination IP address and UDP port of the message based on previously received message from the client. */

  if (addrLeaseRecPtr->ClientGiAddr == 0)
  {
    if (((addrLeaseRecPtr->ClientFlags & TCPIP_DHCPV4SERVER_MSG_FLAG_BROADCAST) != 0) || (msgType == TCPIP_DHCPV4SERVER_TX_MSG_DHCPNAK))
    {
      destSockAddr.addr[0] = 0xFFFFFFFFU;
    }
    else
    {
      IpBase_AddrInType clientAddr = TcpIp_DhcpV4Server_VGetClientAddr(addrAssignmentCfgPtr, addrLeaseRecIdx);

      destSockAddr.addr[0] = IPV4_HTONL(clientAddr);
    
      /* Override physical destination address if required. */

      if (TcpIp_ChangeParameter(TcpIp_DhcpV4Server_VCfgGetServerSocketId(serverInstanceIdx), TCPIP_PARAMID_V_PHYS_DEST_ADDR, &addrLeaseRecPtr->ClientChAddr[0]) != E_OK)
      {
        TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_TX_MESSAGE, TCPIP_DHCPV4SERVER_E_INTERNAL_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }

    destSockAddr.port = TCPIP_DHCPV4SERVER_UINT8_HTONS(TCPIP_DHCPV4SERVER_CLIENT_PORT);
  }
  else
  {
    /* "If the 'giaddr' field in a DHCP message from a client is non-zero,
     *  the server sends any return messages to the 'DHCP server' port on the
     *  BOOTP relay agent whose address appears in 'giaddr'."
     *  [IETF RFC 2131 4.1 Constructing and sending DHCP messages (p. 23)]
     */
    destSockAddr.addr[0] = IPV4_HTONL(addrLeaseRecPtr->ClientGiAddr);
    destSockAddr.port = TCPIP_DHCPV4SERVER_UINT8_HTONS(TCPIP_DHCPV4SERVER_SERVER_PORT);
  }

  /* Calculate Message Length. */

  msgLen += TcpIp_DhcpV4Server_VAddOptions(serverInstanceIdx, NULL_PTR, addrLeaseRecIdx, msgType);

  /* Prepare for indirect data provision via CopyTxData. */

  TcpIp_DhcpV4Server_TxDataInfo.TxData_ServerInstanceIdx    = serverInstanceIdx;
  TcpIp_DhcpV4Server_TxDataInfo.TxData_MsgType              = msgType;
  TcpIp_DhcpV4Server_TxDataInfo.TxData_Length               = msgLen;
  TcpIp_DhcpV4Server_TxDataInfo.TxData_AddrAssignmentCfgIdx = addrAssignmentCfgIdx;
  TcpIp_DhcpV4Server_TxDataInfo.TxData_AddrLeaseRecIdx      = addrLeaseRecIdx;

  /* Call TcpIp_UdpTransmit(). */

  if (TCPIP_OK != TcpIp_UdpTransmit(TcpIp_DhcpV4Server_VCfgGetServerSocketId(serverInstanceIdx), NULL_PTR, (TCPIP_DHCPV4SERVER_P2V(TcpIp_SockAddrType))&destSockAddr, msgLen)) /* PRQA S 0310 */ /* MD_TcpIp_DhcpV4Server_0310 */
  {
    /* Transmit failed. */
    retVal = E_NOT_OK;
  }

  /* Unset physical destination address in IP. */

  if (TcpIp_ChangeParameter(TcpIp_DhcpV4Server_VCfgGetServerSocketId(serverInstanceIdx), TCPIP_PARAMID_V_PHYS_DEST_ADDR, NULL_PTR) != E_OK)
  {
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_TX_MESSAGE, TCPIP_DHCPV4SERVER_E_INTERNAL_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  TcpIp_DhcpV4Server_TxDataInfo.TxData_Length               = 0; /* No TcpIp_UdpTransmit() pending */

  return retVal;
} /* TcpIp_DhcpV4Server_VTxMessage() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_InitMemory()
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_InitMemory(void)
{
    TcpIp_DhcpV4Server_State = TCPIP_DHCPV4SERVER_STATE_UNINIT;

} /* TcpIp_DhcpV4Server_InitMemory() */

/**********************************************************************************************************************
 * TcpIp_DhcpV4Server_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_Init( P2CONST(TcpIp_DhcpV4Server_ConfigType, AUTOMATIC, TCPIP_PBCFG) ConfigPtr)
{
  uint8_least socketCfgsIdx;

  /* ----- Development Error Checks ------------------------------------- */
  TcpIp_DhcpV4Server_CheckDetErrorReturnVoid(TcpIp_DhcpV4Server_State == TCPIP_DHCPV4SERVER_STATE_UNINIT, TCPIP_DHCPV4SERVER_SID_INIT, TCPIP_DHCPV4SERVER_E_ALREADY_INITIALIZED); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* ----- */

  TCPIP_DHCPV4SERVER_UNUSED_PARAMETER(ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  for (socketCfgsIdx = 0; socketCfgsIdx < TCPIP_DHCPV4SERVER_SOCKET_COUNT; socketCfgsIdx++)
  {
    TcpIp_DhcpV4Server_VInitSocketConfig((uint8)socketCfgsIdx);
    TcpIp_DhcpV4Server_IpAddrAssignmentState[socketCfgsIdx] = TCPIP_IPADDR_STATE_UNASSIGNED; 
  }

  TcpIp_DhcpV4Server_TimeSec  = 0;
  TcpIp_DhcpV4Server_TimeMsec = 0;

  TcpIp_DhcpV4Server_TxDataInfo.TxData_Length = 0; /* No TcpIp_UdpTransmit() pending */

  TcpIp_DhcpV4Server_State = TCPIP_DHCPV4SERVER_STATE_INIT;

} /* TcpIp_DhcpV4Server_Init() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_GetVersionInfo()
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, TCPIP_APPL_DATA) VersionInfoPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  TcpIp_DhcpV4Server_CheckDetErrorReturnVoid(NULL_PTR != VersionInfoPtr, TCPIP_DHCPV4SERVER_SID_GET_VERSION_INFO, TCPIP_DHCPV4SERVER_E_INV_PTR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* ----- */

  VersionInfoPtr->vendorID         = TCPIP_DHCPV4SERVER_VENDOR_ID;
  VersionInfoPtr->moduleID         = TCPIP_DHCPV4SERVER_MODULE_ID;
  VersionInfoPtr->sw_major_version = TCPIP_DHCPV4SERVER_SW_MAJOR_VERSION;
  VersionInfoPtr->sw_minor_version = TCPIP_DHCPV4SERVER_SW_MINOR_VERSION;
  VersionInfoPtr->sw_patch_version = TCPIP_DHCPV4SERVER_SW_PATCH_VERSION;

} /* TcpIp_DhcpV4Server_GetVersionInfo() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_LocalIpAddrAssignmentChg()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_LocalIpAddrAssignmentChg(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrStateType State)
{
  uint8_least socketCfgsIdx;

  /* Locate DHCP server instance that uses the LocalAddrId. */

  for (socketCfgsIdx = 0; socketCfgsIdx < TCPIP_DHCPV4SERVER_SOCKET_COUNT; socketCfgsIdx++)
  {
    if (LocalAddrId == TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[socketCfgsIdx].SoBindLocalAddrId)
    {
      TCPIP_ASSERT(TcpIp_DhcpV4Server_IpAddrAssignmentState[socketCfgsIdx] != State);

      TcpIp_DhcpV4Server_IpAddrAssignmentState[socketCfgsIdx] = State;

      /* Open/close DHCP server UDP socket based on the new state of the IP address. */

      if (TCPIP_IPADDR_STATE_ASSIGNED == State)
      {
        (void)TcpIp_DhcpV4Server_VOpenSocket((uint8)socketCfgsIdx);
      }
      else
      {
        if (TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] != TCPIP_INV_SOCKET_ID)
        {
          (void)TcpIp_DhcpV4Server_VCloseSocket((uint8)socketCfgsIdx);
        }
      }

      break; /* max one socket for each LocalAddrId */
    }
  }
} /* TcpIp_DhcpV4Server_LocalIpAddrAssignmentChg() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_CopyTxData()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_CopyTxData(
  TcpIp_SocketIdType            SocketId,
  TCPIP_DHCPV4SERVER_P2V(uint8) BufPtr,
  uint16                        Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8             errorId = TCPIP_E_NO_ERROR;
  BufReq_ReturnType retVal  = BUFREQ_E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if (BufPtr == NULL_PTR)
  {
    errorId = TCPIP_DHCPV4SERVER_E_INV_PTR;
  }
  else if ((0 == TcpIp_DhcpV4Server_TxDataInfo.TxData_Length) || (TcpIp_DhcpV4Server_VCfgGetServerSocketId(TcpIp_DhcpV4Server_TxDataInfo.TxData_ServerInstanceIdx) != SocketId))
  {
    /* unexpected call of CopyTxData. No transmit requested for this socket id. */
    errorId = TCPIP_DHCPV4SERVER_E_INV_PARAM;
  }
  else if (Length != TcpIp_DhcpV4Server_TxDataInfo.TxData_Length)
  {
    /* TcpIp must call CopyTxData only once during TcpIp_UdpTransmit(). Value of parameter Length must match value of parameter Length supplied in TcpIp_UdpTransmit(). */
    errorId = TCPIP_DHCPV4SERVER_E_OUT_OF_MEMORY;
  }
  else
#endif
  {
    /* ----- Local Variables ---------------------------------------------- */
    uint8                                                            AddrLeaseRecIdx      =  TcpIp_DhcpV4Server_TxDataInfo.TxData_AddrLeaseRecIdx;
    uint8                                                            AddrAssignmentCfgIdx =  TcpIp_DhcpV4Server_TxDataInfo.TxData_AddrAssignmentCfgIdx;
    TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      ServerCfgPtr         =  TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(TcpIp_DhcpV4Server_TxDataInfo.TxData_ServerInstanceIdx);
    TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) AddrAssignmentCfgPtr = &ServerCfgPtr->AddrAssignmentCfgsPtr[AddrAssignmentCfgIdx];
    TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType)      AddrLeaseRecPtr      = &TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecArray(TcpIp_DhcpV4Server_TxDataInfo.TxData_ServerInstanceIdx)[AddrLeaseRecIdx];
    uint8                                                            MsgType              =  TcpIp_DhcpV4Server_TxDataInfo.TxData_MsgType;
    IpBase_AddrInType                                                ClientAddr           =  TcpIp_DhcpV4Server_VGetClientAddr(AddrAssignmentCfgPtr, AddrLeaseRecIdx);  

    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_OP]     = TCPIP_DHCPV4SERVER_MSG_OP_BOOTREPLY;
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_HTYPE]  = TCPIP_DHCPV4SERVER_MSG_HTYPE_10MBIT_ETH;
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_HLEN]   = TCPIP_DHCPV4SERVER_MSG_HLEN_10MBIT_ETH;
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_HOPS]   = TCPIP_DHCPV4SERVER_MSG_HOPS;

    IPBASE_PUT_UINT32(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_XID, AddrLeaseRecPtr->ClientXid);
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_SECS + 0] = 0;
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_SECS + 1] = 0;

    /* Set broadcast flag if NAK shall be sent as broadcast. */
    if (AddrLeaseRecPtr->LeaseState == TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK_BROADCAST)
    {
      IPBASE_PUT_UINT16(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_FLAGS, (AddrLeaseRecPtr->ClientFlags | TCPIP_DHCPV4SERVER_MSG_FLAG_BROADCAST));
    }
    else
    {
      IPBASE_PUT_UINT16(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_FLAGS, (AddrLeaseRecPtr->ClientFlags));
    }

    /* Set address fields depending on message type. */
    {
      IpBase_AddrInType ciAddr = 0;                             /* "Client IP address; only filled in if client is in BOUND, RENEW or REBINDING state and can respond to ARP requests." */
      IpBase_AddrInType yiAddr = 0;                             /* "'your' (client) IP address." */
      IpBase_AddrInType siAddr = 0;                             /* "IP address of next server to use in bootstrap; returned in DHCPOFFER, DHCPACK by server." */
      IpBase_AddrInType giAddr = AddrLeaseRecPtr->ClientGiAddr; /* "Relay agent IP address, used in booting via a relay agent." */

      switch (MsgType)
      {
       case TCPIP_DHCPV4SERVER_TX_MSG_DHCPOFFER:
        yiAddr = ClientAddr;
        break;

      case TCPIP_DHCPV4SERVER_TX_MSG_DHCPACK:
        yiAddr = ClientAddr;
        break;

      default:
        /* Nothing to set. */
        break;
      }

      IPBASE_PUT_UINT32(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_CIADDR, ciAddr);
      IPBASE_PUT_UINT32(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_YIADDR, yiAddr);
      IPBASE_PUT_UINT32(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_SIADDR, siAddr);
      IPBASE_PUT_UINT32(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_GIADDR, giAddr);
    }

    /* Write client hardware address to 16 byte 'chaddr' field and pad with zeroes */
    TcpIp_DhcpV4Server_VCopyAndPad(&BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_CHADDR], TCPIP_DHCPV4SERVER_MSG_LEN_CHADDR,
                                    &AddrLeaseRecPtr->ClientChAddr[0], TCPIP_DHCPV4SERVER_MSG_HLEN_10MBIT_ETH);

    if ((TCPIP_DHCPV4SERVER_TX_MSG_DHCPOFFER == MsgType) || (TCPIP_DHCPV4SERVER_TX_MSG_DHCPACK == MsgType))
    {
      /* Write server host name to 64 byte 'sname' field and pad with zeroes */
      TcpIp_DhcpV4Server_VCopyAndPad(&BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_SNAME], TCPIP_DHCPV4SERVER_MSG_LEN_SNAME,
        ServerCfgPtr->HostnamePtr, ServerCfgPtr->HostnameLen);
    }
    else
    {
      /* Clear (zero fill) 'sname' and 'file' fields since they are only used in OFFER and ACK */
      TcpIp_DhcpV4Server_VCopyAndPad(&BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_SNAME], TCPIP_DHCPV4SERVER_MSG_LEN_SNAME, NULL_PTR, 0);
    }

    TcpIp_DhcpV4Server_VCopyAndPad(&BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_BOOTFILE], TCPIP_DHCPV4SERVER_MSG_LEN_BOOTFILE, NULL_PTR, 0);

    /* Write DHCP magic cookie */
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE + 0] = (uint8)((TCPIP_DHCPV4SERVER_MSG_MAGIC_COOKIE & 0xFF000000u) >> 24);
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE + 1] = (uint8)((TCPIP_DHCPV4SERVER_MSG_MAGIC_COOKIE & 0x00FF0000u) >> 16);
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE + 2] = (uint8)((TCPIP_DHCPV4SERVER_MSG_MAGIC_COOKIE & 0x0000FF00u) >> 8);
    BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE + 3] = (uint8)((TCPIP_DHCPV4SERVER_MSG_MAGIC_COOKIE & 0x000000FFu) >> 0);

    /* Add Options */
    {
      uint16 PayloadOfs = TCPIP_DHCPV4SERVER_MSG_OFS_OPTIONS;

      /* [RFC2132 9.6. DHCP Message Type] */
      BufPtr[PayloadOfs] = TCPIP_DHCPV4SERVER_MSG_OPT_DHCP_MSG_TYPE;
      PayloadOfs++;
      BufPtr[PayloadOfs] = TCPIP_DHCPV4SERVER_MSG_OPT_DHCP_MSG_TYPE_LEN;
      PayloadOfs++;
      BufPtr[PayloadOfs] = MsgType;
      PayloadOfs++;

      PayloadOfs += TcpIp_DhcpV4Server_VAddOptions(TcpIp_DhcpV4Server_TxDataInfo.TxData_ServerInstanceIdx, &BufPtr[PayloadOfs], AddrLeaseRecIdx, MsgType);

      /* [RFC2132 3.2. End Option] */
      BufPtr[PayloadOfs] = TCPIP_DHCPV4SERVER_MSG_OPT_END;
      PayloadOfs++;

#if (TCPIP_DHCPV4SERVER_DEV_ERROR_DETECTION == STD_ON)
      if (PayloadOfs != Length)
      {
        TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_TX_MESSAGE, TCPIP_DHCPV4SERVER_E_INV_MSG_LEN);  /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
#endif
    }

    retVal = BUFREQ_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_DHCPV4SERVER_MODULE_ID, TCPIP_DHCPV4SERVER_INSTANCE_ID_DET, TCPIP_DHCPV4SERVER_SID_TX_MESSAGE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_DhcpV4Server_CopyTxData() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STIMF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VHandleDhcpDiscover()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VHandleDhcpDiscover(
  uint8                                                           serverInstanceIdx, 
  uint8                                                           addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                                   msgDataPtr, 
  uint16                                                          msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr)
{
  uint8 addrLeaseRecIdx = TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx(serverInstanceIdx, addrAssignmentCfgIdx, &msgDataPtr[knownOptionsPtr->ClientId.OptOfs], knownOptionsPtr->ClientId.OptLen, knownOptionsPtr->ReqIpAddr, TRUE);

  if (addrLeaseRecIdx != TCPIP_DHCPV4SERVER_INV_IDX)
  {
    TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType) serverCfgPtr    = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(serverInstanceIdx);
    TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr = TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecPtr(serverInstanceIdx, addrLeaseRecIdx);

    TcpIp_DhcpV4Server_VUpdateClientInfo(addrLeaseRecPtr, msgDataPtr, msgDataLen, knownOptionsPtr);
    TcpIp_DhcpV4Server_VUpdateLeaseTime(serverCfgPtr, addrLeaseRecPtr, knownOptionsPtr->LeaseTime);

    addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_OFFER;
  }
} /* TcpIp_DhcpV4Server_VHandleDhcpDiscover() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VHandleDhcpRequest()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VHandleDhcpRequest(
  uint8                                                           serverInstanceIdx, 
  uint8                                                           addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                                   msgDataPtr, 
  uint16                                                          msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr)
{
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType) serverCfgPtr = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(serverInstanceIdx);
  
  /* Get address lease record for the client. */
  uint8 addrLeaseRecIdx = TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx(serverInstanceIdx, addrAssignmentCfgIdx, &msgDataPtr[knownOptionsPtr->ClientId.OptOfs], knownOptionsPtr->ClientId.OptLen, 0, FALSE);

  if (addrLeaseRecIdx == TCPIP_DHCPV4SERVER_INV_IDX)
  {
    /* No address lease record for this client.
      * -> Ignore message.
      */
  }
  else
  {
    TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr = TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecPtr(serverInstanceIdx, addrLeaseRecIdx);
    IpBase_AddrInType                                           clientIpAddr    = TcpIp_DhcpV4Server_VGetClientAddr(&serverCfgPtr->AddrAssignmentCfgsPtr[addrAssignmentCfgIdx], addrLeaseRecIdx);
    IpBase_AddrInType                                           msgGiAddr;
    boolean                                                     clientMovedToOtherSubnet;
    
    /* If value of 'giaddr' differs from the previous received value, the client has moved to another subnet. */
    IPBASE_GET_UINT32(msgDataPtr, TCPIP_DHCPV4SERVER_MSG_OFS_GIADDR, msgGiAddr);
    clientMovedToOtherSubnet = (boolean)(addrLeaseRecPtr->ClientGiAddr != msgGiAddr);

    /* Determine state of the client. */
    if (knownOptionsPtr->ServerId != 0)
    {
      /* "If the DHCPREQUEST message contains a 'server identifier' option,
        *  the message is in response to a DHCPOFFER message."
        *   -> Client is in SELECTING state.
        */
      IpBase_AddrInType thisServerAddr = TcpIp_DhcpV4Server_VGetServerAddr(serverInstanceIdx);

      if (knownOptionsPtr->ServerId != thisServerAddr)
      {
        /* Message addressed to different server.
         * -> Ignore message. 
         */
      }
      else if (knownOptionsPtr->ReqIpAddr == 0)
      {
        /* Requested IP address MUST be specified by client in SELECTING state.
         * -> Ignore message.
         */
      }
      else 
      {
        /* Store client information from the message for later usage. */
        TcpIp_DhcpV4Server_VUpdateClientInfo(addrLeaseRecPtr, msgDataPtr, msgDataLen, knownOptionsPtr);

        /* If value of 'giaddr' differs from the previous received value, the client has moved to another subnet. */
        if (clientMovedToOtherSubnet == TRUE)
        {
          /* Client has moved to new subnet. 
            * -> Tx NAK.
            */
          addrLeaseRecPtr->ErrorMsgId = TCPIP_DHCPV4SERVER_ERROR_MESSAGE_CLIENT_MOVED_SUBNET;
          addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK;
        }
        else if (knownOptionsPtr->ReqIpAddr != clientIpAddr)
        {
          /* Requested IP address does not match currently assigned IP address. 
            * -> Tx NAK.
            */
          addrLeaseRecPtr->ErrorMsgId = TCPIP_DHCPV4SERVER_ERROR_MESSAGE_INVALID_IP_ADDRESS_REQUESTED;
          addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK;
        }
        else
        {
          /* Update lease time and tx ACK. */
          TcpIp_DhcpV4Server_VUpdateLeaseTime(serverCfgPtr, addrLeaseRecPtr, knownOptionsPtr->LeaseTime);
          addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_ACK;
        }
      }
    }
    else 
    {
      /* 'Server Identifier' is not set in REQUEST message. 
       * -> Client is in INIT-REBOOT, RENEWING or REBINDING state.
       */
      if (knownOptionsPtr->ReqIpAddr != 0)
      {
        /* REQUEST message contains 'Requested IP Address' Option.
         * -> client is in INIT-REBOOT state.
         */

        TcpIp_DhcpV4Server_VUpdateClientInfo(addrLeaseRecPtr, msgDataPtr, msgDataLen, knownOptionsPtr);

        if ((knownOptionsPtr->ReqIpAddr != clientIpAddr) || (clientMovedToOtherSubnet == TRUE))
        {
          /* "Server SHOULD send a DHCPNAK message to the client if the 'requested IP address'
            *  is incorrect, or is on the wrong network."
            */
          addrLeaseRecPtr->ErrorMsgId = TCPIP_DHCPV4SERVER_ERROR_MESSAGE_INVALID_IP_ADDRESS_REQUESTED;

          if (msgGiAddr == 0)
          {
            addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK;
          }
          else
          {
            /* "If 'giaddr' is set in the DHCPREQUEST message, the client is on a
              *  different subnet.  The server MUST set the broadcast bit in the
              *  DHCPNAK, so that the relay agent will broadcast the DHCPNAK to the
              *  client, because the client may not have a correct network address
              *  or subnet mask, and the client may not be answering ARP requests." 
              *  [RFC2131 4.3.2 DHCPREQUEST message]
              */
            addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK_BROADCAST;
          }
        }
        else
        {
          /* Update IP address lease time and tx ACK. */
          TcpIp_DhcpV4Server_VUpdateLeaseTime(serverCfgPtr, addrLeaseRecPtr, knownOptionsPtr->LeaseTime);
          addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_ACK;
        }
      }
      else
      {
        /* REQUEST message does not contain 'Server Identifier' and 'Requested IP Address' Option but 'ciaddr' field is set.
         * -> Client is in RENEWING (unicast) or REBINDING (broadcast) state.
         */

        IpBase_AddrInType msgCiAddr;

        IPBASE_GET_UINT32(msgDataPtr, TCPIP_DHCPV4SERVER_MSG_OFS_CIADDR, msgCiAddr); /* ciaddr        4  Client IP address; only filled in if client is in BOUND, RENEW or REBINDING state and can respond to ARP requests. */

        if (msgCiAddr == 0)
        {
          /* Invalid request message.
            * -> Ignore message.
            */
        }
        else
        {
          TcpIp_DhcpV4Server_VUpdateClientInfo(addrLeaseRecPtr, msgDataPtr, msgDataLen, knownOptionsPtr);
          
          if (msgCiAddr != clientIpAddr)
          {
            /* IP address of the client does not match assigned Ip address.
              * -> Ignore message.
              */
          }
          else if (clientMovedToOtherSubnet == TRUE)
          {
            /* Client has moved to new subnet. 
              * -> Tx NAK.
              */
            addrLeaseRecPtr->ErrorMsgId = TCPIP_DHCPV4SERVER_ERROR_MESSAGE_CLIENT_MOVED_SUBNET;
            addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK;
          }
          else 
          {
            /* Update IP address lease time and tx ACK. */
            TcpIp_DhcpV4Server_VUpdateLeaseTime(serverCfgPtr, addrLeaseRecPtr, knownOptionsPtr->LeaseTime);
            addrLeaseRecPtr->LeaseState = TCPIP_DHCPV4SERVER_LEASE_STATE_TX_ACK;
          }
        }
      }
    }
  }
} /* TcpIp_DhcpV4Server_VHandleDhcpRequest() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VHandleDhcpDeclineRelease()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VHandleDhcpDeclineRelease(
  uint8                                                           serverInstanceIdx, 
  uint8                                                           addrAssignmentCfgIdx,
  TCPIP_DHCPV4SERVER_P2C(uint8)                                   msgDataPtr, 
  uint16                                                          msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr)
{
  uint8 addrLeaseRecIdx;

  TCPIP_ASSERT((knownOptionsPtr->ClientId.OptOfs + knownOptionsPtr->ClientId.OptLen) <= msgDataLen);

#if !defined(TCPIP_DHCPV4SEVER_VENABLE_ASSERTIONS)
  TCPIP_DHCPV4SERVER_UNUSED_PARAMETER(msgDataLen); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  addrLeaseRecIdx = TcpIp_DhcpV4Server_VGetAddrLeaseRecIdx(serverInstanceIdx, addrAssignmentCfgIdx, &msgDataPtr[knownOptionsPtr->ClientId.OptOfs], knownOptionsPtr->ClientId.OptLen, 0, FALSE);

  if (addrLeaseRecIdx != TCPIP_DHCPV4SERVER_INV_IDX)
  {
    TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr = TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecPtr(serverInstanceIdx, addrLeaseRecIdx);
    /* set lease of this client to expired. It may bu used for the same client again. */
    addrLeaseRecPtr->LeaseState  = TCPIP_DHCPV4SERVER_LEASE_STATE_EXPIRED;

    if (knownOptionsPtr->MessageType == TCPIP_DHCPV4SERVER_RX_MSG_DHCPDECLINE)
    {
      addrLeaseRecPtr->ClientIdLen = 0;
    }
  }
} /* TcpIp_DhcpV4Server_VHandleDhcpDeclineRelease() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VInitKnownOptionsInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VInitKnownOptionsInfo(TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr)
{
  knownOptionsPtr->ClientId.OptOfs = 0;
  knownOptionsPtr->ClientId.OptLen = 0;
  knownOptionsPtr->MessageType     = TCPIP_DHCPV4SERVER_MSG_INVALID;
#if (TCPIP_DHCPV4SERVER_REQUEST_LEASE_TIME_ENABLED == STD_ON)
  knownOptionsPtr->LeaseTime       = 0;
#endif
  knownOptionsPtr->ParamReqSet     = 0;
  knownOptionsPtr->ReqIpAddr       = 0;
  knownOptionsPtr->ServerId        = 0;
  knownOptionsPtr->OptionOverload  = 0;
} /* TcpIp_DhcpV4Server_VInitKnownOptionsInfo() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VParseOptions()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VParseOptions(
  TCPIP_DHCPV4SERVER_P2C(uint8) msgDataPtr, uint16 msgDataOfs, uint16 msgDataLen, 
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr)
{
  Std_ReturnType retVal = E_OK;

  boolean endOptFound = FALSE;

  TCPIP_ASSERT((msgDataOfs == TCPIP_DHCPV4SERVER_MSG_OFS_OPTIONS) || (msgDataOfs == TCPIP_DHCPV4SERVER_MSG_OFS_BOOTFILE) || (msgDataOfs == TCPIP_DHCPV4SERVER_MSG_OFS_SNAME));

  while ((endOptFound == FALSE) && (retVal == E_OK) && (msgDataOfs < msgDataLen))
  {
    uint8 optType = msgDataPtr[msgDataOfs];
    msgDataOfs++;

    /* Check for single byte END and PAD options. All other options have a 1 byte length field */

    if (optType == TCPIP_DHCPV4SERVER_MSG_OPT_END)
    {
      /* End option has been found. Stop Processing of the following data. */
      endOptFound = TRUE;
    }
    else if (optType == TCPIP_DHCPV4SERVER_MSG_OPT_PAD)
    {
      /* Skip pad option */
    }
    else if (msgDataOfs < msgDataLen)
    {
      uint8 optLen = msgDataPtr[msgDataOfs]; /* PRQA S 0781 */ /* MD_TcpIp_DhcpV4Server_0781 */
      msgDataOfs++;

      if ((msgDataOfs + optLen) < msgDataLen)
      {
        switch(optType)
        {
        /* [RFC2132 9.6. DHCP Message Type] */
        case TCPIP_DHCPV4SERVER_MSG_OPT_DHCP_MSG_TYPE:
          if (TCPIP_DHCPV4SERVER_MSG_OPT_DHCP_MSG_TYPE_LEN == optLen)
          {
            knownOptionsPtr->MessageType = msgDataPtr[msgDataOfs];
          }
          else
          {
            retVal = E_NOT_OK;
          }
          break;

        /* [RFC2132 9.14. Client-identifier] */
        case TCPIP_DHCPV4SERVER_MSG_OPT_CLIENT_ID:
          /* ClientIdPtr currently points the the 'chaddr' field in the DHCP message header since the hardware address
           * is also used as client identifier if no client identifier option is received.
           * Update ClientIdPtr and ClientIdLen:
           */
          knownOptionsPtr->ClientId.OptOfs = msgDataOfs;
          knownOptionsPtr->ClientId.OptLen = optLen;
          break;

        /* [RFC2132 9.1. Requested IP Address] */
        case TCPIP_DHCPV4SERVER_MSG_OPT_REQUESTED_IP_ADDR:
          if (TCPIP_DHCPV4SERVER_MSG_OPT_REQUESTED_IP_ADDR_LEN == optLen)
          {
            IPBASE_GET_UINT32(msgDataPtr, msgDataOfs, knownOptionsPtr->ReqIpAddr);
          }
          else
          {
            retVal = E_NOT_OK;
          }
          break;

        /* [RFC2132 9.2. IP Address Lease Time] */
#if (TCPIP_DHCPV4SERVER_REQUEST_LEASE_TIME_ENABLED == STD_ON)
        case TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME:
          if (TCPIP_DHCPV4SERVER_MSG_OPT_IP_ADDR_LEASE_TIME_LEN == optLen)
          {
            IPBASE_GET_UINT32(msgDataPtr, msgDataOfs, knownOptionsPtr->LeaseTime);
          }
          else
          {
            retVal = E_NOT_OK;
          }
          break;
#endif

        /* [RFC2132 9.3. Option Overload] */
        case TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD:
          if (TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD_LEN == optLen)
          {
            knownOptionsPtr->OptionOverload = msgDataPtr[msgDataOfs];
          }
          else
          {
            retVal = E_NOT_OK;
          }
          break;

        /* [RFC2132 9.8. Parameter Request List] */
        case TCPIP_DHCPV4SERVER_MSG_OPT_PARAM_REQUEST_LIST:
          knownOptionsPtr->ParamReqSet = TcpIp_DhcpV4Server_VParseParamReqestListOption(msgDataPtr, msgDataOfs, optLen);
          break;

        /* [RFC2132 9.7. Server Identifier] */
        case TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID:
          if (TCPIP_DHCPV4SERVER_MSG_OPT_SERVER_ID_LEN == optLen)
          {
            IPBASE_GET_UINT32(msgDataPtr, msgDataOfs, knownOptionsPtr->ServerId);
          }
          else
          {
            retVal = E_NOT_OK;
          }
          break;

        default:
          /* Unknown Option will be skipped */
          break;
        }

        if (retVal == E_OK)
        {
          msgDataOfs += optLen;
        }
      }
      else
      {
        /* ERROR: Option length exceeds remaining message length. */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* ERROR: Option length field missing. */
      retVal = E_NOT_OK;
    }
  }

  if (endOptFound == FALSE)
  {
    /* "The last option must always be the 'end' option."
     * [RFC2131 4.1 Constructing and sending DHCP messages]
     */
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_DhcpV4Server_VParseOptions() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VParseParamReqestListOption()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4Server_VParseParamReqestListOption(
  TCPIP_DHCPV4SERVER_P2C(uint8) msgDataPtr, uint16 msgDataOfs, uint8 optLen)
{
  /* client has send parameter request list option */
  uint8_least reqParamIdx;
  uint8 paramReqSet = 0;

  for (reqParamIdx = 0; reqParamIdx < optLen; reqParamIdx++)
  {
    /* filter requestable parameters */
    switch (msgDataPtr[msgDataOfs + reqParamIdx])
    {
    case TCPIP_DHCPV4SERVER_MSG_OPT_SUBNET_MASK:
      paramReqSet |= TCPIP_DHCPV4SERVER_MSG_OPT_REQ_FLAG_SUBNET_MASK;
      break;

    case TCPIP_DHCPV4SERVER_MSG_OPT_ROUTER:
      paramReqSet |= TCPIP_DHCPV4SERVER_MSG_OPT_REQ_FLAG_ROUTER;
      break;

    default:
      /* this parameter may not be requested and is ignored. */
      break;
    }
  }

  return paramReqSet;
} /* TcpIp_DhcpV4Server_VParseParamReqestListOption() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VUpdateLeaseTime()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VUpdateLeaseTime(
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType) serverCfgPtr,
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr, 
  uint32                                                      requestedLeaseTime)
{
  uint32 UpdatedLeaseTimeSec;

#if (TCPIP_DHCPV4SERVER_REQUEST_LEASE_TIME_ENABLED == STD_ON)
  if (requestedLeaseTime > 0)
  {
    if (requestedLeaseTime < serverCfgPtr->LeaseTimeSecMin)
    {
      UpdatedLeaseTimeSec = serverCfgPtr->LeaseTimeSecMin;
    }
    else if (requestedLeaseTime > serverCfgPtr->LeaseTimeSecMax)
    {
      UpdatedLeaseTimeSec = serverCfgPtr->LeaseTimeSecMax;
    }
    else
    {
      UpdatedLeaseTimeSec = requestedLeaseTime;
    }
  }
  else
#endif
  {
    UpdatedLeaseTimeSec = serverCfgPtr->LeaseTimeSecDefault;
  }

  if ((TCPIP_DHCPV4SERVER_TIME_INFINITE - TcpIp_DhcpV4Server_TimeSec) < UpdatedLeaseTimeSec)
  {
    addrLeaseRecPtr->LeaseTimeSec = TCPIP_DHCPV4SERVER_TIME_INFINITE;
  }
  else
  {
    addrLeaseRecPtr->LeaseTimeSec = TcpIp_DhcpV4Server_TimeSec + UpdatedLeaseTimeSec;
  }
} /* TcpIp_DhcpV4Server_VUpdateLeaseTime() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VUpdateClientInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VUpdateClientInfo(
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr, 
  TCPIP_DHCPV4SERVER_P2C(uint8)                               msgDataPtr, 
  uint16                                                      msgDataLen,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_KnownOptionsInfoType) knownOptionsPtr)
{
  TCPIP_ASSERT(msgDataLen >= TCPIP_DHCPV4SERVER_MSG_OFS_OPTIONS);

#if !defined(TCPIP_DHCPV4SEVER_VENABLE_ASSERTIONS)
  TCPIP_DHCPV4SERVER_UNUSED_PARAMETER(msgDataLen); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  /* Record received client parameters that will be needed when we send the response */
  IPBASE_GET_UINT32(msgDataPtr, TCPIP_DHCPV4SERVER_MSG_OFS_XID,    addrLeaseRecPtr->ClientXid);    /* Transaction ID, a random number chosen by the client */
  IPBASE_GET_UINT16(msgDataPtr, TCPIP_DHCPV4SERVER_MSG_OFS_FLAGS,  addrLeaseRecPtr->ClientFlags);  /* Flags */
  IPBASE_GET_UINT32(msgDataPtr, TCPIP_DHCPV4SERVER_MSG_OFS_GIADDR, addrLeaseRecPtr->ClientGiAddr); /* gateway ip address */
  addrLeaseRecPtr->ClientParamReqSet = knownOptionsPtr->ParamReqSet;

  /* Store client hardware address */
  TcpIp_DhcpV4Server_VCopyAndPad(&addrLeaseRecPtr->ClientChAddr[0], TCPIP_DHCPV4SERVER_MSG_HLEN_10MBIT_ETH,
                                  &msgDataPtr[TCPIP_DHCPV4SERVER_MSG_OFS_CHADDR], TCPIP_DHCPV4SERVER_MSG_HLEN_10MBIT_ETH);
} /* TcpIp_DhcpV4Server_VUpdateClientInfo() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_RxIndication()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_RxIndication(
  TcpIp_SocketIdType                         SocketId,
  TCPIP_DHCPV4SERVER_P2C(TcpIp_SockAddrType) RemoteAddrPtr,  /* PRQA S 3673 */ /* MD_TcpIp_DhcpV4Server_3673 */
  TCPIP_DHCPV4SERVER_P2C(uint8)              BufPtr,
  uint16                                     Length)
{
  uint8                                                       serverInstanceIdx;
  TcpIp_DhcpV4Server_KnownOptionsInfoType                     knownOptions;

  /* ----- Development Error Checks ------------------------------------- */
  TcpIp_DhcpV4Server_CheckDetErrorReturnVoid(NULL_PTR != BufPtr, TCPIP_DHCPV4SERVER_SID_RX_INDICATION, TCPIP_DHCPV4SERVER_E_INV_PTR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* ----- */

  TCPIP_DHCPV4SERVER_UNUSED_PARAMETER(RemoteAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* Check if there are address assignment configurations for this socket handle */
  serverInstanceIdx = TcpIp_DhcpV4Server_VGetServerInstanceIdxForSocket(SocketId);

  if (TCPIP_DHCPV4SERVER_INV_IDX == serverInstanceIdx)
  {
    /* no address assignment configuration for this socket/interface/VLAN */
    return;
  }

  /* Check DHCP message structure */

  if (Length < TCPIP_DHCPV4SERVER_MIN_MSG_LEN)
  {
    /* Each DHCP message must contain at least the header, the DHCP options cookie, the DHCP message option and an end option */
    return; /* ignore invalid message */
  }

  /* Message op code / message type. 1 = BOOTREQUEST, 2 = BOOTREPLY */
  if (TCPIP_DHCPV4SERVER_MSG_OP_BOOTREQUEST != BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_OP])
  {
    /* A DHCP server does only answer to BOOTREQUEST messages */
    return; /* ignore invalid message */
  }

  /* Hardware address type, see ARP section in "Assigned Numbers" RFC; e.g., '1' = 10mb ethernet. */
  if (TCPIP_DHCPV4SERVER_MSG_HTYPE_10MBIT_ETH != BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_HTYPE])
  {
    /* Only the ethernet hardware type is supported */
    return; /* ignore invalid message */
  }

  /* Hardware address length (e.g.  '6' for 10mb ethernet). */
  if (TCPIP_DHCPV4SERVER_MSG_HLEN_10MBIT_ETH != BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_HLEN])
  {
    /* The length of each hardware (ethernet MAC) address must be 6 */
    return; /* ignore invalid message */
  }

  /* Contents of filed 'hops'   (1 byte) is ignored by the server */
  /* Contents of field 'sname' (64 bytes, Optional server host name, null terminated string) is ignored by the server */
  /* Contents of field 'file' (128 bytes, Boot file name, null terminated string; "generic" name or null in DHCPDISCOVER, fully qualified directory-path name in DHCPOFFER) is ignored by the server */

  {
    uint32 dhcpMagicCookie;

    /* Check DHCP magic cookie which is located directly after the DHCP header */
    IPBASE_GET_UINT32(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_MAGIC_COOKIE, dhcpMagicCookie);

    if (dhcpMagicCookie != TCPIP_DHCPV4SERVER_MSG_MAGIC_COOKIE)
    {
      /* invalid magic cookie, corrupted DHCP messages assumed */
      return;
    }
  }

  /* Message header seems to be ok. */

  /* DHCP client configuration has been found. Client can be configured by the server */

  {
    Std_ReturnType optionParseResult;

    TcpIp_DhcpV4Server_VInitKnownOptionsInfo(&knownOptions);

    optionParseResult = TcpIp_DhcpV4Server_VParseOptions(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_OPTIONS, Length, &knownOptions);

    if ((optionParseResult == E_OK) && ((knownOptions.OptionOverload & TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD_FILE) != 0))
    {
      optionParseResult = TcpIp_DhcpV4Server_VParseOptions(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_BOOTFILE, TCPIP_DHCPV4SERVER_MSG_OFS_BOOTFILE + TCPIP_DHCPV4SERVER_MSG_LEN_BOOTFILE, &knownOptions);
    }

    if ((optionParseResult == E_OK) && ((knownOptions.OptionOverload & TCPIP_DHCPV4SERVER_MSG_OPT_OPTION_OVERLOAD_SNAME) != 0))
    {
      optionParseResult = TcpIp_DhcpV4Server_VParseOptions(BufPtr, TCPIP_DHCPV4SERVER_MSG_OFS_SNAME, TCPIP_DHCPV4SERVER_MSG_OFS_SNAME + TCPIP_DHCPV4SERVER_MSG_LEN_SNAME, &knownOptions);
    }

    if (optionParseResult == E_OK)
    {
      uint8 addrAssignmentCfgIdx;

      if (knownOptions.ClientId.OptLen == 0)
      {
        knownOptions.ClientId.OptOfs = TCPIP_DHCPV4SERVER_MSG_OFS_CHADDR;
        knownOptions.ClientId.OptLen = TCPIP_DHCPV4SERVER_MSG_HLEN_10MBIT_ETH;
      }

      /* DHCP message options parsed */
      /* Get or create the client record which holds the lease information for this client */

      addrAssignmentCfgIdx = TcpIp_DhcpV4Server_VGetAddrAssignmentCfgIdxForMacAddr(serverInstanceIdx, &BufPtr[TCPIP_DHCPV4SERVER_MSG_OFS_CHADDR]);

      if (addrAssignmentCfgIdx != TCPIP_DHCPV4SERVER_INV_IDX)
      {
        /* Check DHCP message type option */
        switch (knownOptions.MessageType)
        {
        case TCPIP_DHCPV4SERVER_RX_MSG_DHCPDISCOVER:
          /* [RFC2131 4.3.1 DHCPDISCOVER message]
            * - Choose network address for client
            *   - If no address available report error to DEM (MAY)
            *   - Normal address selection
            *     - active client binding
            *     - expired client binding
            *     - address of requested ip address option (if valid and available)
            *     - use any or configured address from the pool
            *   - If client has requested a specific lease time this time MAY be used
            * - Send OFFER to client
            */

          TcpIp_DhcpV4Server_VHandleDhcpDiscover(serverInstanceIdx, addrAssignmentCfgIdx, BufPtr, Length, &knownOptions);
          break;

        case TCPIP_DHCPV4SERVER_RX_MSG_DHCPREQUEST:
          /* "A DHCPREQUEST message may come from a client responding to a DHCPOFFER message from a server, #
            *  from a client verifying a previously allocated IP address or from a client extending the lease on a
            *  network address. [RFC2131 4.3.2 DHCPREQUEST message]
            */

          TcpIp_DhcpV4Server_VHandleDhcpRequest(serverInstanceIdx, addrAssignmentCfgIdx, BufPtr, Length, &knownOptions);
          break;

        case TCPIP_DHCPV4SERVER_RX_MSG_DHCPDECLINE:
          /* fall through */
        case TCPIP_DHCPV4SERVER_RX_MSG_DHCPRELEASE:
          TcpIp_DhcpV4Server_VHandleDhcpDeclineRelease(serverInstanceIdx, addrAssignmentCfgIdx, BufPtr, Length, &knownOptions);
          break;

        default:
          /* ignore this message */
          break;
        }
      }
      else
      {
        /* no address assignment configuration for this client at the current switch port on this socket/interface/VLAN has been found */
      }
    }
  }
  
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_DhcpV4Server_RxIndication() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_TcpIpEvent()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_TcpIpEvent(TcpIp_SocketIdType SocketId, IpBase_TcpIpEventType Event)
{
  if (Event == TCPIP_UDP_CLOSED)
  {
    uint8_least socketCfgIdx;

    for (socketCfgIdx = 0; socketCfgIdx < TCPIP_DHCPV4SERVER_SOCKET_COUNT; socketCfgIdx++)
    {
      if (TcpIp_DhcpV4Server_SocketId[socketCfgIdx] == SocketId)
      {
        TcpIp_DhcpV4Server_SocketId[socketCfgIdx] = TCPIP_INV_SOCKET_ID;
      }
    }
  }
} /* TcpIp_DhcpV4Server_TcpIpEvent() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_MainFunction(void)
{
  uint8_least serverInstanceIdx;

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_DhcpV4Server_CheckDetErrorReturnVoid(TcpIp_DhcpV4Server_State != TCPIP_DHCPV4SERVER_STATE_UNINIT, TCPIP_DHCPV4SERVER_SID_MAIN_FUNCTION, TCPIP_DHCPV4SERVER_E_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* ------------------------------------------------- */

  /* #10 Increment module time counter used for timeout handling. */
  TcpIp_DhcpV4Server_TimeMsec += TCPIP_DHCPV4SERVER_MAIN_FUNCTION_INTERVAL_MSEC;

  if (1000 <= TcpIp_DhcpV4Server_TimeMsec)
  {
    TcpIp_DhcpV4Server_TimeMsec -= 1000;
    TcpIp_DhcpV4Server_TimeSec++;
  }

  /* #20 Iterate over all configured DHCP Server instances. */
  for (serverInstanceIdx = 0; serverInstanceIdx < TCPIP_DHCPV4SERVER_SOCKET_COUNT; serverInstanceIdx++)
  {
    /* #30 Check if UDP socket is set up for DHCP Server instance. */
    if (TCPIP_INV_SOCKET_ID != TcpIp_DhcpV4Server_VCfgGetServerSocketId(serverInstanceIdx))
    {
      TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      serverCfgPtr          = TcpIp_DhcpV4Server_VCfgGetServerConfigPtr(serverInstanceIdx);
      uint8_least                                                      addrAssignmentCfgIdx;
      TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) addrAssignmentCfgsPtr = serverCfgPtr->AddrAssignmentCfgsPtr;  /* PRQA S 0781 */ /* MD_TcpIp_DhcpV4Server_0781 */

      /* #40 Iterate over all Address Assignment Configurations of the DHCP Server instance. */
      for (addrAssignmentCfgIdx = 0; addrAssignmentCfgIdx < serverCfgPtr->AddrAssignmentCfgCount; addrAssignmentCfgIdx++)
      {
        const uint8_least addrLeaseRecIdxFirst = (uint8_least)(addrAssignmentCfgsPtr[addrAssignmentCfgIdx].AddrLeaseRecStartOfs);
        const uint8_least addrLeaseRecIdxEnd   = (uint8_least)(addrLeaseRecIdxFirst + addrAssignmentCfgsPtr[addrAssignmentCfgIdx].DhcpAddrCount);
        uint8_least addrLeaseRecIdx;

        TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecsPtr = TcpIp_DhcpV4Server_VCfgGetAddrLeaseRecArray(serverInstanceIdx); /* PRQA S 0781 */ /* MD_TcpIp_DhcpV4Server_0781 */

        /* #50 Iterate over all Address Lease Records of the current Address Assignment Configuration. */
        for (addrLeaseRecIdx = addrLeaseRecIdxFirst; addrLeaseRecIdx < addrLeaseRecIdxEnd; addrLeaseRecIdx++)
        {
          TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType) addrLeaseRecPtr = &addrLeaseRecsPtr[addrLeaseRecIdx];

          /* #60 Update Address Lease Record based on current status. */
          switch (addrLeaseRecPtr->LeaseState)
          {
          case TCPIP_DHCPV4SERVER_LEASE_STATE_TX_OFFER:
            /* #70 [TX_OFFER] Transmit OFFER to client and switch to state RX_REQUEST. */
            if (E_OK == TcpIp_DhcpV4Server_VTxMessage((uint8)serverInstanceIdx, (uint8)addrAssignmentCfgIdx, (uint8)addrLeaseRecIdx, TCPIP_DHCPV4SERVER_TX_MSG_DHCPOFFER))
            {
              /* Lease entry will be set to expired if the client does not request the lease within TCPIP_DHCPV4SERVER_OFFER_VALID_TIME_SEC */
              addrLeaseRecPtr->LeaseState        = TCPIP_DHCPV4SERVER_LEASE_STATE_RX_REQUEST;
              addrLeaseRecPtr->ExpirationTimeSec = (TcpIp_DhcpV4Server_TimeSec + serverCfgPtr->OfferValidTimeSec);
            }
            break;

          case TCPIP_DHCPV4SERVER_LEASE_STATE_RX_REQUEST:
            /* #80 [RX_REQUEST] Switch state to EXPIRED is client did not sent REQUEST until timeout expired. */
            if (addrLeaseRecPtr->ExpirationTimeSec < TcpIp_DhcpV4Server_TimeSec)
            {
              /* OFFER expired. Did not receive REQUEST in time */
              addrLeaseRecPtr->LeaseState        = TCPIP_DHCPV4SERVER_LEASE_STATE_EXPIRED;
              addrLeaseRecPtr->LeaseTimeSec      = 0;
              addrLeaseRecPtr->ExpirationTimeSec = 0;
            }
            break;

          case TCPIP_DHCPV4SERVER_LEASE_STATE_TX_ACK:
            /* #90 [TX_ACK] Transmit ACK to client and switch to state REQUEST. */
            if (E_OK == TcpIp_DhcpV4Server_VTxMessage((uint8)serverInstanceIdx, (uint8)addrAssignmentCfgIdx, (uint8)addrLeaseRecIdx, TCPIP_DHCPV4SERVER_TX_MSG_DHCPACK))
            {
              /* Lease entry will be set to expired if the lease time is not updated before it has been expired */
              addrLeaseRecPtr->LeaseState        = TCPIP_DHCPV4SERVER_LEASE_STATE_RX_REQUEST;
              addrLeaseRecPtr->ExpirationTimeSec = addrLeaseRecPtr->LeaseTimeSec;
            }
            break;

          case TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK:
            /* fall through. */
          case TCPIP_DHCPV4SERVER_LEASE_STATE_TX_NAK_BROADCAST:
            /* #100 [TX_NAK] Transmit Negative ACK to client and switch to state EXPIRED. */
            if (E_OK == TcpIp_DhcpV4Server_VTxMessage((uint8)serverInstanceIdx, (uint8)addrAssignmentCfgIdx, (uint8)addrLeaseRecIdx, TCPIP_DHCPV4SERVER_TX_MSG_DHCPNAK))
            {
              /* Lease entry will be set to expired if the lease time is not updated before it has been expired */
              addrLeaseRecPtr->LeaseState        = TCPIP_DHCPV4SERVER_LEASE_STATE_EXPIRED;
              addrLeaseRecPtr->LeaseTimeSec      = 0;
              addrLeaseRecPtr->ExpirationTimeSec = 0;
            }
            break;

          default:
            /* No action required for this lease record */
            break;
          }
        }
      }
    }
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_DhcpV4Server_MainFunction() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VSetupUdpSocket
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(TcpIp_SocketIdType, TCPIP_CODE) TcpIp_DhcpV4Server_VSetupUdpSocket(TcpIp_LocalAddrIdType BindLocalAddrId)
{
  TcpIp_SocketIdType DhcpServerSocketId = TCPIP_INV_SOCKET_ID;
  uint16             DhcpServerPort     = TCPIP_DHCPV4SERVER_SERVER_PORT;

  /* Open UDP socket */
  if (E_OK != TcpIp_TcpIp_DhcpV4ServerGetSocket(TCPIP_AF_INET, TCPIP_IPPROTO_UDP, &DhcpServerSocketId))
  {
    /* ERROR: UDP socket could not be bound to the server address and port combination */
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_VINTERNAL_FUNCTION, TCPIP_DHCPV4SERVER_E_UDP_SOCKET_GET_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return TCPIP_INV_SOCKET_ID;
  }

  if (E_OK != TcpIp_Bind(DhcpServerSocketId, BindLocalAddrId, &DhcpServerPort))
  {
    /* ERROR: UDP socket could not be opened */
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_VINTERNAL_FUNCTION, TCPIP_DHCPV4SERVER_E_UDP_SOCKET_BIND_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return TCPIP_INV_SOCKET_ID;
  }

  return DhcpServerSocketId;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_DhcpV4Server_VSetupUdpSocket() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VInitSocketConfig
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV4Server_VInitSocketConfig(uint8 SocketCfgsIdx)
{
  /* PRQA S 0781 3 */ /* MD_TcpIp_DhcpV4Server_0781 */
  TCPIP_DHCPV4SERVER_P2V(TcpIp_DhcpV4Server_AddrLeaseRecType)      AddrLeaseRecsPtr      = TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[SocketCfgsIdx].AddrLeaseRecsPtr;
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_ServerConfigType)      ServerConfigPtr       = TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[SocketCfgsIdx].ServerConfigPtr;
  TCPIP_DHCPV4SERVER_P2C(TcpIp_DhcpV4Server_AddrAssignmentCfgType) AddrAssignmentCfgsPtr = ServerConfigPtr->AddrAssignmentCfgsPtr;
  uint8_least                                                      addrAssignmentCfgIdx;

  for (addrAssignmentCfgIdx = 0; addrAssignmentCfgIdx < ServerConfigPtr->AddrAssignmentCfgCount; addrAssignmentCfgIdx++)
  {
          uint8_least addrLeaseRecIdx    = (uint8_least)(AddrAssignmentCfgsPtr[addrAssignmentCfgIdx].AddrLeaseRecStartOfs); /* PRQA S 0781 */ /* MD_TcpIp_DhcpV4Server_0781 */
    const uint8_least addrLeaseRecEndIdx = (uint8_least)(addrLeaseRecIdx + AddrAssignmentCfgsPtr[addrAssignmentCfgIdx].DhcpAddrCount);

    for (; addrLeaseRecIdx < addrLeaseRecEndIdx; addrLeaseRecIdx++)
    {
      AddrLeaseRecsPtr[addrLeaseRecIdx].LeaseState        = TCPIP_DHCPV4SERVER_LEASE_STATE_EXPIRED;
      AddrLeaseRecsPtr[addrLeaseRecIdx].ErrorMsgId        = TCPIP_DHCPV4SERVER_ERROR_MESSAGE_NONE;
      AddrLeaseRecsPtr[addrLeaseRecIdx].ClientIdLen       = 0;
      AddrLeaseRecsPtr[addrLeaseRecIdx].LeaseTimeSec      = 0;
      AddrLeaseRecsPtr[addrLeaseRecIdx].ExpirationTimeSec = 0;
    }
  }

  TcpIp_DhcpV4Server_SocketId[SocketCfgsIdx]              = TCPIP_INV_SOCKET_ID;
} /* TcpIp_DhcpV4Server_VInitSocketConfig() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VOpenSocket
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VOpenSocket(uint8 socketCfgsIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] == TCPIP_INV_SOCKET_ID)
  {
    TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] = TcpIp_DhcpV4Server_VSetupUdpSocket(TcpIp_DhcpV4Server_Socket2AddrAssignmentCfgsMap[socketCfgsIdx].SoBindLocalAddrId);

    if (TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] != TCPIP_INV_SOCKET_ID)
    {
      retVal = E_OK;
    }
  }
  else
  {
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_VINTERNAL_FUNCTION, TCPIP_DHCPV4SERVER_E_UDP_SOCKET_GET_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
  }

  return retVal;
} /* TcpIp_DhcpV4Server_VOpenSocket() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_VCloseSocket
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_VCloseSocket(uint8 socketCfgsIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* IP address of the DHCPv4 server socket has been lost */
  if (TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] != TCPIP_INV_SOCKET_ID)
  {
    /* close socket */
    if (TcpIp_Close(TcpIp_DhcpV4Server_SocketId[socketCfgsIdx], FALSE) == E_OK)
    {
      /* OK: UDP socket closed */
      TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] = TCPIP_INV_SOCKET_ID;
      TcpIp_DhcpV4Server_VInitSocketConfig((uint8)socketCfgsIdx);
      retVal = E_OK;
    }
    else
    {
      /* ERROR: UDP socket could not be closed. */
      TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_VINTERNAL_FUNCTION, TCPIP_DHCPV4SERVER_E_UDP_SOCKET_CLOSE_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    /* UDP socket is not open. */
    TcpIp_DhcpV4Server_ReportDetError(TCPIP_DHCPV4SERVER_SID_VINTERNAL_FUNCTION, TCPIP_DHCPV4SERVER_E_UDP_SOCKET_CLOSE_ERROR); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
  }

  return retVal;
} /* TcpIp_DhcpV4Server_VCloseSocket() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_Start
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_Start(void)
{
  Std_ReturnType retVal = E_OK;
  uint8_least    socketCfgsIdx;

  for (socketCfgsIdx = 0; socketCfgsIdx < TCPIP_DHCPV4SERVER_SOCKET_COUNT; socketCfgsIdx++)
  {
    if ((TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] == TCPIP_INV_SOCKET_ID) && (TcpIp_DhcpV4Server_IpAddrAssignmentState[socketCfgsIdx] == TCPIP_IPADDR_STATE_ASSIGNED))
    {
      if (TcpIp_DhcpV4Server_VOpenSocket((uint8)socketCfgsIdx) == E_NOT_OK)
      {
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
} /* TcpIp_DhcpV4Server_Start() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4Server_Stop
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4Server_Stop(void)
{
  Std_ReturnType retVal = E_OK;
  uint8_least    socketCfgsIdx;

  for (socketCfgsIdx = 0; socketCfgsIdx < TCPIP_DHCPV4SERVER_SOCKET_COUNT; socketCfgsIdx++)
  {
    if (TcpIp_DhcpV4Server_SocketId[socketCfgsIdx] != TCPIP_INV_SOCKET_ID)
    {
      if (TcpIp_DhcpV4Server_VCloseSocket((uint8)socketCfgsIdx) == E_NOT_OK)
      {
        retVal = E_NOT_OK;
      }
    }
  }

  if (retVal == E_OK)
  {
    TcpIp_DhcpV4Server_TimeSec  = 0;
    TcpIp_DhcpV4Server_TimeMsec = 0;

    TcpIp_DhcpV4Server_TxDataInfo.TxData_Length = 0; /* No TcpIp_UdpTransmit() pending */
  }

  return retVal;
} /* TcpIp_DhcpV4Server_Stop() */

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_TcpIp_DhcpV4Server_0770: rule 14.5
      Reason:     A 'continue' statement has been used for efficiency reasons.
      Risk:       Poor readability of the code, unpredicted program flow.
      Prevention: Program flow has been verified by component tests and review.
 MD_TcpIp_DhcpV4Server_0781:
      Reason:     Name is used as a structure/union member as well as a variable name in order to make clear that these values are related to each other.
      Risk:       None.
      Prevention: N/A.
 MD_TcpIp_DhcpV4Server_3673:
      Reason:     Pointer to variable is specified by AUTOSAR or passed to an AUTOSAR API and therefore cannot be a pointer to const.
      Risk:       None.
      Prevention: N/A.
 MD_TcpIp_DhcpV4Server_3218:
     Description: Rule 8.7
                  Objects shall be defined at block scope if they are only accessed from within a single function.
     Reason:      Vector style guide prevents usage of static variables in function scope
     Risk:        None.
     Prevention:  None.
*/

#endif  /* (TCPIP_DHCPV4SERVER_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_DhcpV4Server.c
 *********************************************************************************************************************/
