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
 *         \file  TcpIp_IpV4.h
 *        \brief  Implementation of Internet Protocol version 4 (IPv4)
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Thorsten Albers               visal         Vector Informatik GmbH
 *  Frederik Dornemann            visfdn        Vector Informatik GmbH
 *  Knut Winkelbach               viswnk        Vector Informatik GmbH
 *  Jens Bauer                    visjsb        Vector Informatik GmbH
 *  Manisha Jadav                 vismjv        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  The following table lists the changes affecting the following files:
 *    TcpIp_IpV4.*
 *    TcpIp_IpV4_Cbk.h
 *    TcpIp_IpV4_Types.h
 *    TcpIp_IpV4_Priv.*
 *    TcpIp_IcmpV4.*
 *    TcpIp_IcmpV4_Cbk.h
 *    TcpIp_Arp.*
 *    TcpIp_Arp_Cbk.h
 *    TcpIp_DhcpV4.*
 *    TcpIp_DhcpV4_Cbk.h
 *
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  All changes of Tp_IpV4 Implementation 3.00.01 are included.
 *  -
 *  01.00.00  2015-06-18  visfdn  ESCAN00083592 FEAT-1248: Configuration of the TCP/IP stack according to AR 4.2.1
 *            2015-07-09  visal   ESCAN00082692 FEAT-1283: Use hardware-based checksum calculation (IPv4, IPv6,
 *                                                TCP, UDP and ICMP) if it is supported by the microcontroller
 *            2015-07-09  visal   -             Remove parameter SockIdxPtr from the internal API TcpIp_Tcp_RxIndication
 *  01.00.01  2015-08-21  visfdn  -             Resolved MISRA/PClint findings
 *  01.00.02  2015-10-14  visfdn  ESCAN00085527 IpV4 stops sending, ARP does not send new ARP request
 *  02.00.00  2015-11-27  visfdn  ESCAN00085636 FEAT-1481: Support IPv4 Fragmentation
 *            2015-11-27  visfdn  ESCAN00086826 Upper layer TX checksum calculation shall always by performed by IP or in HW
 *            2015-11-27  visal   ESCAN00086821 TcpIp: FEAT-10: Send the FQDN option via DHCP to identify the ECU
 *            2015-11-27  visal   ESCAN00086827 TcpIp: FEAT-10: Support triggering of IP address assignment
 *            2015-11-27  visal   ESCAN00086829 TcpIp: FEAT-10: Implement API to read the local physical address
 *            2015-12-04  visfdn  ESCAN00086986 Configuration of IPv4 default router shall be optional
 *            2015-12-04  visfdn  ESCAN00086987 Configuration of IPv4 default controller shall be optional
 *            2015-12-04  visfdn  ESCAN00084712 IpV4 does not reject sending packets to own IP address
 *            2015-12-04  visfdn  ESCAN00088099 IPv4 drops packets from other private networks
 *  02.00.01  2016-01-29  visal   ESCAN00087911 Missing checks for valid address IDs in DHCP option APIs may lead to NULL-Ptr exceptions
 *            2016-01-29  visal   ESCAN00087914 Minor code rework due to MISRA / Code Review findings
 *            2016-01-29  visfdn  ESCAN00087193 Alignment exception/trap in TcpIp_DhcpV4_RxIndication() on Tricore
 *  02.01.00  2016-01-29  visfdn  ESCAN00087556 Allow configuration of TcpIp lower layer other than EthIf
 *            2016-02-26  visal   ESCAN00088586 Compiler error: Undefined symbol 'IPV4_GET_UINT32_HBO' in file 'obj\TcpIp_DhcpV4.o'
 *            2016-03-30  visal   ESCAN00089112 IpAddrAssignment is not indicated to the socket owners
 *            2016-03-30  visal   ESCAN00089120 IpV4_Ip_RequestIpAddrAssignment generates false DET error
 *  02.01.01  2016-04-15  visfdn  ESCAN00089218 DHCP 'secs' field in  Discover and Request message does not match
 *            2016-04-15  visfdn  ESCAN00089254 DHCP retransmission strategy have no exponential backoff interval
 *  03.00.00  2016-05-18  visfdn  ESCAN00088842 Compiler error: unresolved external symbol EthIf_UpdatePhysAddrFilter in TcpIp_IpV4.c
 *            2016-05-18  visfdn  ESCAN00089974 TcpIp_GetIpAddr() does not provide values for IPv6 netmask and default router.
 *            2016-06-10  visfdn  ESCAN00089368 FEAT-1516: Support IP address assignment methods according to AR 4.2.1 for IPv4
 *            2016-06-22  visfdn  ESCAN00088858 DHCPv4 server does not respond correctly to a client with broadcast flag set to 0
 *  03.00.01  2016-08-11  visfdn  ESCAN00091418 ECU sometimes sends ARP requests for destinations that should be still in the ARP cache.
 *            2016-08-11  visfdn  ESCAN00091246 AutoIP selection mechanism for IPv4 link local addresses selects addresses outside valid range
 *            2016-08-11  visfdn  ESCAN00090089 IPv4 sends unnecessary ARP requests for default router
 *  03.01.00  2016-09-23  visfdn  ESCAN00092013 FEAT-2136: Storage of IPv4 link-local (AutoIP) address in NvM
 *  04.00.00  2016-10-14  visfdn  ESCAN00091989 Received DHCPNAK message is ignored by DHCPv4 client
 *            2016-10-14  visfdn  ESCAN00092002 DHCP DISCOVER messages are sent from source IP address of other address assignment method
 *            2016-10-24  visfdn  ESCAN00091166 Adapted source file defines according to filenames
 *            2016-10-24  visfdn  ESCAN00092485 Compiler error: Undefined symbol IpV4_Ip_VJoinMulticastGroup() in TcpIp_IpV4.c
 *            2016-10-14  visfdn  FEATC-258     FEAT-1816: Split MainFunctions to optimize gateway use-case
 *            2016-11-22  visfdn  ESCAN00092911 TcpIp_DhcpReadOption() returns E_NOT_OK when reading the FQDN option
 *  05.00.00  2017-01-10  viswnk  FEATC-662     FEAT-2340: Transmission of gratuitous ARP packages.
 *            2017-02-22  viswnk  FEATC-678     FEAT-2151: Extended Ethernet Bus Diagnostic
 *            2017-02-22  visfdn  ESCAN00085797 Deactivation of ARP Table Entry Timeout should be possible
 *            2017-02-22  visfdn  ESCAN00092548 Invalidate ARP entries only if the peer does not answer to ARP requests after the timeout
 *            2017-02-22  visfdn  ESCAN00094119 IPv4 multicast packets are sent to MAC broadcast address instead of corresponding MAC multicast address
 *            2017-03-28  visfdn  ESCAN00084757 ARP shall ignore packets with invalid IP/MAC addresses
 *            2017-03-28  visfdn  ESCAN00093688 ARP cache is updated even if target IP address in ARP packet addresses another node
 *            2017-03-28  visfdn  ESCAN00093381 UDP multicast packets are dropped due to invalid checksum even if checksum is correct
 *  05.00.01  2017-04-26  visfdn  ESCAN00094759 Compiler error: DhcpV4 IpCtrlIdx is const but a value is assigned
 *            2017-04-26  visfdn  ESCAN00094904 Compiler error: Declaration of IpV4_Arp_VSendMessage differs from Implementation
 *            2017-04-26  visfdn  ESCAN00094517 ECU sends too many ARP requests/announcements after conflict during linklocal address configuration
 *  06.00.00  2017-04-28  visjsb  STORYC-676    Enhancements for ETM (TcpIp: add "Clear NDP Cache" API)
 *            2017-05-10  visfdn  STORYC-144    Release of FEAT-1481 (IPv4 Fragmentation)
 *  06.01.00  2017-05-28  vismjv  STORYC-1044   Support of ASR4.3 API TcpIp_GetArpCacheEntries
 *            2017-06-08  visfdn  ESCAN00095164 Compiler error: Identifier IpCtrlIdx not declared
 *  06.02.00  2017-06-29  visfdn  STORYC-1687   Support of ICMP Destination Unreachable Messages and ASR4.3 Callback Up_IcmpMsgHandler
 *            2017-07-03  visfdn  ESCAN00095060 TcpIp calls Dns_UpdateIpV4Servers() with invalid pointer to DNS server addresses
 *  06.03.00  2017-07-13  viswnk  -             Removed unnecessary non AUTOSAR API IpV4_GetVersionInfo
 *            2017-07-19  visfdn  ESCAN00095870 DHCP DISCOVER messages not sent from source IP address 0.0.0.0 if another assignment method already has configured an IP address
 *  06.04.00  2017-08-17  visfdn  STORYC-347    P3 Code Refactoring / CDD Step 1
 *            2017-08-17  visfdn  -             Resolved review findings.
 *  06.04.01  2017-09-06  viswnk  ESCAN00096552 Compiler error: "identifier ArpTxMsgPtr not declared" for CPU_BYTE_ORDER HIGH_BYTE_FIRST
 *********************************************************************************************************************/

#if !defined (TCPIP_IPV4_H)
# define TCPIP_IPV4_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
/*
--- LIMITAIONS OF THIS IMPLEMENTATION ------------------------------------------
- no fragmentation supported (rx and tx)
- no support of service types
*/

/*
|0              |8              |16             |24             |
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |Type of Service|         Total Length          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|     Fragment Offset     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |        Header Checksum        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Source Address                         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                     Destination Address                       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                   Options                     |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Example Internet Protocol header
*/
/*
Type of Service: 8 bits
Bits 0-2: Precedence.
Bit 3   : 0 = Normal Delay, 1 = Low Delay.
Bit 4   : 0 = Normal Throughput, 1 = High Throughput.
Bit 5   : 0 = Normal Relibility, 1 = High Relibility.
Bits 6-7: Reserved for Future Use.
   0     1     2     3     4     5     6     7
+-----+-----+-----+-----+-----+-----+-----+-----+
|   PRECEDENCE    |  D  |  T  |  R  |  0  |  0  |
+-----+-----+-----+-----+-----+-----+-----+-----+
Precedence
111 - Network Control
110 - Internetwork Control
101 - CRITIC/ECP
100 - Flash Override
011 - Flash
010 - Immediate
001 - Priority
000 - Routine
*/
/*
Flags: 3 bits
Bit 0: reserved, must be zero
Bit 1: (DF) 0 = May Fragment, 1 = Don't Fragment.
Bit 2: (MF) 0 = Last Fragment, 1 = More Fragments.
  0   1   2
+---+---+---+
|   | D | M |
| 0 | F | F |
+---+---+---+
*/
/*
- Tx: The IP module calls the If with CtrlIdx parameter, because only the IP module knows on which
  network interface a message should be sent.
- Fragmentation: If IP receives fragmented messages with TCP content, IP needs to buffer these
  fragments internally for reassemby.
- Fragmentation: If IP receives fragmented messages with UDP content, IP can request a buffer
  directly from the application and reassemble the fragments there.
*/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_IpV4_Types.h"
#include "TcpIp_IpV4_Cfg.h"
#include "TcpIp_IpV4_Priv.h"

#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
#define IPV4_SW_MAJOR_VERSION         6u
#define IPV4_SW_MINOR_VERSION         4u
#define IPV4_SW_PATCH_VERSION         1u

/* supported Autosar version */
#define IPV4_AR_RELEASE_MAJOR_VERSION          4u
#define IPV4_AR_RELEASE_MINOR_VERSION          2u
#define IPV4_AR_RELEASE_REVISION_VERSION       1u

/* for compatibility */
#define IPV4_AR_MAJOR_VERSION       4u
#define IPV4_AR_MINOR_VERSION       0u
#define IPV4_AR_PATCH_VERSION       0u

#define IPV4_VENDOR_ID                            30u
/* Using same module ID as SoAd (56) (no own ID) */
#define IPV4_MODULE_ID                           170u
/* VInstance ID identifies SoAd submodule 0=SoAd, 1=TcpIp, 2=IpV4, 3=IpV6 */
#define IPV4_VINSTANCE_ID                          2u

/* ------------------------------------------------------ */

/* IPV4 ApiIds */
#define IPV4_API_ID_INIT                        0x00u
#define IPV4_API_ID_MAIN_FUNCTION               0x01u
#define IPV4_API_ID_VRESET                      0x02u
#define IPV4_API_ID_GET_VERSION_INFO            0x03u
#define IPV4_API_ID_GET_CFG_SRC                 0x04u
#define IPV4_API_ID_SET_NET_ADDR                0x05u
#define IPV4_API_ID_RESET_NET_ADDR              0x06u
#define IPV4_API_ID_IS_ADDR_ID_ACCEPTABLE       0x07u

#define IPV4_API_ID_V_ASSIGN_PHYS_ADDR          0x10u

/* IPV4 DET errors */
#define IPV4_E_NO_ERROR                         0x00u
#define IPV4_E_NOT_INITIALIZED                  0x01u
#define IPV4_E_INV_POINTER                      0x02u
#define IPV4_E_INV_PARAM                        0x03u
#define IPV4_E_INV_CTRL_IDX                     0x04u
#define IPV4_E_INV_SOCK_IDX                     0x05u
#define IPV4_E_INV_CONFIG                       0x06u
#define IPV4_E_INV_ADDR_ID                      0x07u

#define IPV4_CTRL_IDX_AUTO    0xFF
#define IPV4_CTRL_IDX_INVALID 0xFF

/* ------------------------------------------------------ */

/* TCPIP module state */
#define IPV4_STATE_UNINIT                       0x00u
#define IPV4_STATE_INIT                         0x01u

  /* 224.0.0.0/4: multicast address block */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)  /* BIGENDIAN */
# define IPV4_MULTICAST_PREFIX                  0xE0000000U
# define IPV4_MULTICAST_PREFIX_MASK             0xF0000000U /* 4Bit mask */
# define IPV4_PRIV_NET_10_0_0_0_PREFIX          0x0A000000U
# define IPV4_PRIV_NET_10_0_0_0_PREFIX_MASK     0xFF000000U /* 8Bit mask */
# define IPV4_PRIV_NET_172_16_0_0_PREFIX        0xAC100000U
# define IPV4_PRIV_NET_172_16_0_0_PREFIX_MASK   0xFFF00000U /* 12Bit mask */
# define IPV4_PRIV_NET_192_168_0_0_PREFIX       0xC0A80000U
# define IPV4_PRIV_NET_192_168_0_0_PREFIX_MASK  0xFFFF0000U /* 16Bit mask */
# define IPV4_LOCALNET_PREFIX                   0x7F000000U
# define IPV4_LOCALNET_PREFIX_MASK              0xFF000000U /* 8Bit mask */
#else  /* LITTLEENDIAN */
# define IPV4_MULTICAST_PREFIX                  0x000000E0U
# define IPV4_MULTICAST_PREFIX_MASK             0x000000F0U /* 4Bit mask */
# define IPV4_PRIV_NET_10_0_0_0_PREFIX          0x0000000AU
# define IPV4_PRIV_NET_10_0_0_0_PREFIX_MASK     0x000000FFU /* 8Bit mask */
# define IPV4_PRIV_NET_172_16_0_0_PREFIX        0x000010ACU
# define IPV4_PRIV_NET_172_16_0_0_PREFIX_MASK   0x0000F0FFU /* 12Bit mask */
# define IPV4_PRIV_NET_192_168_0_0_PREFIX       0x0000A8C0U
# define IPV4_PRIV_NET_192_168_0_0_PREFIX_MASK  0x0000FFFFU /* 16Bit mask */
# define IPV4_LOCALNET_PREFIX                   0x0000007FU
# define IPV4_LOCALNET_PREFIX_MASK              0x000000FFU /* 8Bit mask */
#endif

/* ------------------------------------------------------ */

#define IPV4_ADDR_ANY              0x00000000u

#define IPV4_ADDR_MATCH_NONE       0U
#define IPV4_ADDR_MATCH_EXACT      1U
#define IPV4_ADDR_MATCH_IF         2U
#define IPV4_ADDR_MATCH_EVER       3U

/* IP ApiIds */
#define IPV4_IP_API_ID_PROVIDE_TX_BUFFER        0x20u
#define IPV4_IP_API_ID_TRANSMIT                 0x21u
#define IPV4_IP_API_ID_GET_LOC_NET_ADDR         0x22u
#define IPV4_IP_API_ID_GET_LOC_NET_MASK         0x23u
#define IPV4_IP_API_ID_ADDR_LOSS_IND            0x24u
#define IPV4_IP_API_ID_DHCP_ADDR_IND            0x25u
#define IPV4_IP_API_ID_PROVIDE_NEXT_TX_BUFFER   0x26u

#define IPV4_IP_API_ID_RX_INDICATION            0x28u
#define IPV4_IP_API_ID_TX_CONFIRMATION          0x29u
#define IPV4_IP_API_ID_TRCV_LNK_ST_CH           0x2Au

#define IPV4_IP_API_ID_INIT                     0x30u
#define IPV4_IP_API_ID_MAIN_FUNCTION            0x31u
#define IPV4_IP_API_ID_RESET                    0x32u
#define IPV4_IP_API_ID_ADDR_CONFL_IND           0x33u

#define IPV4_IP_API_ID_RESET_SOCKET             0x34u
#define IPV4_IP_API_ID_SET_TIME_TO_LIVE         0x35u
#define IPV4_IP_API_ID_SET_TYPE_OF_SERVICE      0x36u
#define IPV4_IP_API_ID_SET_ETHIF_FRAME_PRIO     0x37u

#define IPV4_IP_API_ID_REQ_IP_ADDR_ASSIGN       0x38u

#define IPV4_IP_API_ID_GET_LAST_DUPLICATE_DHCP_ADDR_DID 0x39u

#define IPV4_IP_API_ID_V_INTERNAL_FUNCTION      0xFFu

/* IP overall states */
#define IPV4_IP_UNINIT                          0x00u
#define IPV4_IP_INIT_MEMORY                     0x01u
#define IPV4_IP_INIT                            0x02u

/* IP states per controller */
#define IPV4_IP_OFFLINE                         0x00u
#define IPV4_IP_ONLINE_PASSIVE                  0x01u
#define IPV4_IP_ONLINE_ACTIVE                   0x02u

#define IPV4_IP_DEF_HEADER_LENGTH_BYTE          20u  /* default IP header length */

#define IPV4_IP_HEADER_VERSION_V4               0x4u
#define IPV4_IP_HEADER_IHL_MIN                  0x5u

/* Type of Service: 8 bits */
#define IPV4_IP_TOS_PRC_MASK  0xE0u  /* Precedence */
#define IPV4_IP_TOS_PRC_NC    0xE0u  /* 111 - Network Control */
#define IPV4_IP_TOS_PRC_INC   0xC0u  /* 110 - Internetwork Control */
#define IPV4_IP_TOS_PRC_ECP   0xA0u  /* 101 - CRITIC/ECP */
#define IPV4_IP_TOS_PRC_FLO   0x80u  /* 100 - Flash Override */
#define IPV4_IP_TOS_PRC_FL    0x60u  /* 011 - Flash */
#define IPV4_IP_TOS_PRC_IMM   0x40u  /* 010 - Immediate */
#define IPV4_IP_TOS_PRC_PRI   0x20u  /* 001 - Priority */
#define IPV4_IP_TOS_PRC_ROU   0x00u  /* 000 - Routine */

#define IPV4_IP_TOS_DEL_MASK  0x10u  /* Delay */
#define IPV4_IP_TOS_DEL_NORM  0x00u  /* Normal Delay */
#define IPV4_IP_TOS_DEL_LOW   0x10u  /* Low Delay */

#define IPV4_IP_TOS_THP_MASK  0x08u  /* Throughput */
#define IPV4_IP_TOS_THP_NORM  0x00u  /* Normal Throughput */
#define IPV4_IP_TOS_THP_HIGH  0x08u  /* High Throughput */

#define IPV4_IP_TOS_REL_MASK  0x04u  /* Reliability */
#define IPV4_IP_TOS_REL_NORM  0x00u  /* Normal Reliability */
#define IPV4_IP_TOS_REL_HIGH  0x04u  /* High Reliability */

#define IPV4_IP_TOS_RES_MASK  0x03u  /* Reserved */

/* Flags: 3 bits */
#define IPV4_IP_FLAGS_MASK            0xE000u
#define IPV4_IP_FLAGS_DONT_FRAGMENT   0x4000u
#define IPV4_IP_FLAGS_MORE_FRAGMENTS  0x2000u

#define IPV4_IP_FRAGMENT_OFFSET_MASK  0x1FFFu
#define IPV4_IP_FRAGMENT_BLOCK_SIZE        8u

#define IPV4_IP_DEF_FLAGS_OFFSET_NBO IPV4_HTONS(IPV4_IP_FLAGS_DONT_FRAGMENT)

/* Protocol: 8 bits */
#define IPV4_IP_PROTOCOL_ICMP                1u
#define IPV4_IP_PROTOCOL_IGMP                2u
#define IPV4_IP_PROTOCOL_TCP                 6u
#define IPV4_IP_PROTOCOL_UDP                17u

#define IPV4_IP_MAX_ETHIF_FRAME_PRIO         7u

#define IPV4_IP_FRAME_TYPE            (0x0800u)

#define IPV4_IP_LL_NET_ADDR_START_NBO  (0xA9FE0100u)
#define IPV4_IP_LL_NET_ADDR_COUNT      (0x0000FE00u) /* 0xA9FE0100u (169.254.1.0) + (0x0000FE00u - 1) = 0xA9FEFEFFu (169.254.254.255) / [RFC3927 2.1. Link-Local Address Selection] */

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
/* LITTLEENDIAN */
#  define IPV4_IP_LL_NET_ADDR_NET      (0x0000FEA9u)  /* 169.254/16 reverse */
#  define IPV4_IP_LL_NET_ADDR_MASK     (0x0000FFFFu)  /* reverse */
#else
/* BIGENDIAN */
#  define IPV4_IP_LL_NET_ADDR_NET      (0xA9FE0000u)  /* 169.254/16 */
#  define IPV4_IP_LL_NET_ADDR_MASK     (0xFFFF0000u)
#endif /* CPU_BYTE_ORDER */

#if !defined (IPV4_LOCAL)
# define IPV4_LOCAL static
#endif

#if !defined (IPV4_LOCAL_INLINE)
# define IPV4_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

#define IPV4_ADDRS_IN_SAME_SUBNET(ADDR_A, ADDR_B, SUBNET_MASK)       (((ADDR_A) & (SUBNET_MASK)) == ((ADDR_B) & (SUBNET_MASK)))
#define IPV4_ADDR_IN_SUBNET(ADDR, SUBNET_PREFIX, SUBNET_PREFIX_MASK) ((SUBNET_PREFIX) == ((ADDR) & (SUBNET_PREFIX_MASK)))
#define IPV4_ADDR_IS_MULTICAST(ADDR) (IPV4_ADDR_IN_SUBNET((ADDR), IPV4_MULTICAST_PREFIX, IPV4_MULTICAST_PREFIX_MASK))

/* limited broadcast, directed broadcast */
#define IPV4_ADDR_IS_BROADCAST(ADDR) ( ((ADDR) == TCPIP_INADDR_BROADCAST) || \
                                       ((ADDR) == (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr | ~(IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask))) )

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8  IpV4_Ip_GlobStateType;  /* global state */

typedef struct
{
  uint8             Ver_Ihl;  /* Ihl in dword */
  uint8             Tos;
  uint16            TotLen;  /* length in byte */
  uint16            Ident;
  uint16            Flags_Offset;
  uint8             Ttl;     /* Time to live */
  uint8             Prot;
  uint16            Checksum;
  IpBase_AddrInType SrcAddr;
  IpBase_AddrInType DestAddr;
} IpV4_Ip_HdrType;

typedef void IpV4_Ip_ConfigType;

#define IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#if IPV4_DEV_ERROR_DETECT == STD_ON
extern VAR(IpV4_StateType, IPV4_VAR_NOINIT)      IpV4_State;
#endif
  /* IPV4_DEV_ERROR_DETECT == STD_ON */

#define IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define IPV4_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/***********************************************************************************************************************
 *  IpV4_InitMemory
 **********************************************************************************************************************/
/*! \brief      Initializes internal module state variables.
 *  \details    -
 *  \return     void
 *  \pre        -
 *  \context    system startup / task level
 *  \reentrant  FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_InitMemory( void );

/***********************************************************************************************************************
 *  IpV4_Init
 **********************************************************************************************************************/
/*! \brief        Initialization of the IpV4 component.
 *  \details      -
 *  \param[in]    CfgPtr      pointer to post-build configuration
 *  \return       void
 *  \pre          - 
 *  \context      system startup / task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \note         This function has to be called after IpV4_InitMemory() and before any other function of this module.
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Init(
    P2CONST(IpV4_ConfigType, IPV4_CONST, IPV4_CONST) CfgPtr );

/***********************************************************************************************************************
 *  IpV4_MainFunction
 **********************************************************************************************************************/
/*! \brief        This function handles the IPV4 global status
 *  \details      -
 *  \pre          
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \note         This function is called periodically every IPV4_MAIN_FCT_PERIOD_MSEC ms
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_MainFunction(void); /* PRQA S 3451 */ /* MD_IpV4_3451 */

/***********************************************************************************************************************
 *  IpV4_Ip_ProvideTxBuffer
 **********************************************************************************************************************/
/*! \brief         Obtains one or more TX buffers from the ethernet layer ant returns the first of these buffers.
 *  \details       This function checks if the destination is reachable and the 
 *  \param[in,out] addrIdPtr             In:  Pointer to source IP address identifier<br />
 *                                       Out: Selected source IP address identifier if ANY was used as input.
 *  \param[in]     socketId              Index of the TcpIp socket.
 *  \param[in]     destAddrPtr           Pointer to the destination address.
 *  \param[out]    ipBufferPtr           Pointer to the first IP payload buffer.
 *  \param[in,out] ipBufferLenPtr        In:  Pointer to the IP payload length in bytes.<br />
 *                                       Out: Length of the first IP payload buffer in bytes (see notes).
 *  \param[in]     ipTxReqDescrPtr       Pointer to TX request descriptor that was used at IpV4_Ip_ProvideTxBuffer() call.
 *  \param[in]     forceProvideBuffer    Indicates whether the buffer shall be provided even if link layer address
 *                                       resolution for the destination address is incomplete.
 *  \return        BUFREQ_OK             Buffer provided.
 *  \return        BUFREQ_E_NOT_OK       Buffer could not be provided.
 *  \return        BUFREQ_E_BUSY         Ethernet layer is currently not able to provide buffer or link layer address
 *                                       resolution is incomplete. Try again later.
  *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note                                This function may provide more than one buffer if the requested
 *                                       payload size is too large to fit into one IP packet. In that case only the first
 *                                       buffer fragment is returned by this function. The additional buffer fragments
 *                                       can be obtained via IpV4_Ip_ProvideNextTxBuffer().
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Ip_ProvideTxBuffer(
    IPV4_P2V(IpV4_AddrIdType)                  addrIdPtr,
    TcpIp_SocketIdType                         socketId,
    IPV4_P2C(IpBase_SockAddrInType)            destAddrPtr,
    IPV4_P2V(uint8)                           *ipBufferPtr,
    IPV4_P2V(uint16)                           ipBufferLenPtr,
    IPV4_P2V(TcpIp_IpTxRequestDescriptorType)  ipTxReqDescrPtr,
    boolean                                    forceProvideBuffer);

/***********************************************************************************************************************
 *  IpV4_Ip_FragmentProvideTxBuffer
 **********************************************************************************************************************/
/*! \brief         Provides an additional IP TX buffer fragment.
 *  \details       IpV4_Ip_ProvideTxBuffer() may return a buffer that is smaller than the requested length.
 *                 In that case additional buffer fragments can be obtained via this function.
 *  \param[in]     ipTxReqDescrPtr  Pointer to TX request descriptor that was used at IpV4_Ip_ProvideTxBuffer() call.
 *  \param[out]    bufPtrPtr        Pointer to the next IP buffer fragment.
 *  \param[out]    bufLenPtr        Length of the next IP buffer fragment in bytes.
 *  \return        E_OK             Next buffer fragment provided. (*bufPtrPtr and *bufLenPtr updated)
 *  \return        E_NOT_OK         No next buffer fragment available.
 *  \pre           IpV4_Ip_ProvideTxBuffer() must be called first.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_FragmentProvideTxBuffer(
  IPV4_P2V(TcpIp_IpTxRequestDescriptorType)  ipTxReqDescrPtr,
  IPV4_P2V(uint8)                           *bufPtrPtr,
  IPV4_P2V(uint16)                           bufLenPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_Transmit
 **********************************************************************************************************************/
/*! \brief        Triggers transmission of all buffers that have been provided via IpV4_ProvideTxBuffer()
 *  \details      Only the required number of buffers for transmitting ipPayloadLen bytes will be used.
 *                All provided but unused buffers will be released.
 *                An IP packet that requires only one fragment is an unfragmented/normal IP packet.
 *  \param[in]    ipTxReqDescrPtr       Pointer to TX request descriptor that was used at IpV4_Ip_ProvideTxBuffer() call.
 *  \param[in]    ipPayloadLen          Number of bytes to transmit.
 *                                      This implementations allows two special cases:
 *                                       1: The value of ipPayloadLen may be larger than requested by IpV4_Ip_ProvideTxBuffer() if
 *                                          there was enough space left in the last provided buffer fragment.
 *                                       2: The value of ipPayloadLen may be less than requested by IpV4_ProvideTxBuffer().
 *                                          Even if there were multiple buffers provided, only the required number of fragments will be sent.
 *                                          Previously provided but unused buffers will be released by this function.
 *  \param[in]    ipProtocol            IP protocol value. (TCP, UDP, ICMP)
 *  \param[in]    reqTxConfirmation     Indicates whether TX confirmation is required from lower layer (EthIf).
 *  \param[in]    ulTxReqIdx            Upper layer TX request identifier.
 *  \param[in]    retryOnPhysAddrMiss   Do not release ethernet buffer if physical address could not be resolved.
 *  \return       TCPIP_OK                  Transmission triggered.
 *  \return       TCPIP_E_NOT_OK            Transmit failed.
 *  \return       TCPIP_E_PHYS_ADDR_MISS    Transmit failed due to ARP cache miss.
 *  \pre          A previous call of IpV4_Ip_ProvideTxBuffer() must have been successful.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Ip_Transmit(
    IPV4_P2C(TcpIp_IpTxRequestDescriptorType) ipTxReqDescrPtr, /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
    uint16                                    ipPayloadLen,
    uint8                                     ipProtocol,
    boolean                                   reqTxConfirmation,
    uint8                                     ulTxReqIdx,
    boolean                                   retryOnPhysAddrMiss);

/***********************************************************************************************************************
 *  IpV4_Ip_GetPhysAddr
 **********************************************************************************************************************/
/*! \brief        Obtains the physical source address used by the EthIf controller implicitly specified via IpAddrId.
 *  \details      -
 *  \param[in]    IpAddrId           Local address identifier implicitly specifing the EthIf controller for which the
 *                                   physical address shall be obtained.
 *  \param[out]   PhysAddrPtr        Pointer to the memory where the physical source address (MAC address) in network
 *                                   byte order is stored
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) IpV4_Ip_GetPhysAddr(
    IpV4_AddrIdType IpAddrId,
    IPV4_P2V(uint8) PhysAddrPtr );

/***********************************************************************************************************************
 *  IpV4_Ip_GetRemotePhysAddr
 **********************************************************************************************************************/
/*! \brief        Lookup the physical address for a remote network address. 
 *  \details      TcpIp_GetRemotePhysAddr queries the IP/physical address translation table specified by CtrlIdx and
 *                returns the physical address related to the IP address specified by IpAddrPtr. In case no physical
 *                address can be retrieved and parameter initRes is TRUE, address resolution for the specified IP
 *                address is initiated on the local network.
 *  \param[in]    CtrlIdx             EthIf controller index to identify the related ARP/NDP table.
 *  \param[in]    IpAddr              specifies the IP address for which the physical address shall be retrieved
 *  \param[in]    InitRes             specifies if the address resolution shall be initiated (TRUE) or not (FALSE) in
 *                                    case the physical address related to the specified IP address is currently
 *                                    unknown.
 *  \param[out]   PhysAddrPtr         Pointer to the memory where the physical address (MAC address) related to the
 *                                    specified IP address is stored in network byte order.
 *  \return       TCPIP_E_OK               specified IP address resolved, physical address provided via PhysAddrPtr 
 *                TCPIP_E_PHYS_ADDR_MISS   physical address currently unknown (address resolution initiated if initRes set to TRUE)
 *  \pre          -
 *  \context      task level
 **********************************************************************************************************************/
FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Ip_GetRemotePhysAddr(
    uint8              CtrlIdx,
    IpBase_AddrInType  IpAddr,
    IPV4_P2V(uint8)    PhysAddrPtr,
    boolean            InitRes);

/***********************************************************************************************************************
 *  IpV4_GetCtrlIdx
 **********************************************************************************************************************/
/*! \brief        IpV4_GetCtrlIdx returns the index of the EthIf controller related to LocalAddrId.
 *  \details      -
 *  \param[in]    AddrId              Local IP address identifier implicitly specifing the EthIf controller that shall
 *                                    be returned
 *  \param[out]   CtrlIdxPtr          Pointer to the memory where the index of the EthIf controller related to
 *                                    LocalAddrId is stored
 *  \return       E_OK                The request was successful 
 *                E_NOT_OK            The request was not successful
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_GetCtrlIdx(
    IpV4_AddrIdType AddrId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) CtrlIdxPtr );

/***********************************************************************************************************************
 *  IpV4_Ip_GetLocNetAddr
 **********************************************************************************************************************/
/*! \brief        Request the current IP address for a given IP address id
 *  \details      Reading the local IP address is mostly used for diagnostic purposes.
 *  \param[in]    AddrId                id of the IPv4 address
 *  \param[out]   NetAddrPtr            pointer for the local network address
 *  \return       E_OK                  local network address returned
 *  \return       E_NOT_OK              local network address access failed
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_GetLocNetAddr(
  IpV4_AddrIdType AddrId,
  P2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA) NetAddrPtr );

/***********************************************************************************************************************
 *  IpV4_Ip_GetDefaultRouterNetAddr
 **********************************************************************************************************************/
/*! \brief        Read the network address of the default router/gateway.
 *  \details      -
 *  \param[in]    ipAddrId              IP address identifier (used to specify the IP controller)
 *  \param[out]   defRouterNetAddrPtr   Address of the default router.
 *  \pre
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_GetDefaultRouterNetAddr(
  IpV4_AddrIdType                                     ipAddrId,
  P2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA) defRouterNetAddrPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_GetLocNetMask
 **********************************************************************************************************************/
/*! \brief        Request the current net mask for a given IP address id
 *  \details      Reading the local network mask is mostly used for diagnostic purposes.
 *  \param[in]    AddrId                id of the IPv4 address
 *  \param[out]   NetMaskPtr            pointer for the local network mask
 *  \return       E_OK                  local network mask returned
 *  \return       E_NOT_OK              local network mask access failed
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_GetLocNetMask(
    IpV4_AddrIdType AddrId,
    P2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA) NetMaskPtr );

/***********************************************************************************************************************
 *  IpV4_Ip_GetActiveAddrAssignmentMethod
 **********************************************************************************************************************/
/*! \brief      Get assignment method of the active unicast or multicast IP address.
 *  \details    -
 *  \param[in]  ipAddrId                                  IP address identifier of a unicast or multicast address.
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE       Unicast/multicast address not active.
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC     Unicast/multicast address was set statically.
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP       Unicast address assigned via DHCP.
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL  Unicast address assigned via AUTO-IP.
 *  \pre        -
 *  \context    ANY
 *  \reentrant  TRUE for IP address identifiers referencing different IP controllers.
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(uint8, IPV4_CODE) IpV4_Ip_GetActiveAddrAssignmentMethod(
  IpV4_AddrIdType ipAddrId);

/***********************************************************************************************************************
 *  IpV4_Ip_RequestIpAddrAssignment
 **********************************************************************************************************************/
/*! \brief        Initiate the local IP address assignment for the IP address specified by ipAddrId.
 *  \details      -
 *  \param[in]    IpAddrId            IP address identifier of the IP address for which an assignment is requested.
 *  \param[in]    addrAssgnmentType   Type of IP address assignment which shall be initiated.
 *  \param[in]    localIpAddr         IP address which shall be assigned to the EthIf controller indirectly 
 *                                    specified via LocalAddrId. Note: This parameter is only used in case the
 *                                    parameters Type is set to TCPIP_IPADDR_ASSIGNMENT_STATIC.
 *  \param[in]    netmaskCidr         Network mask of IPv4 address in CIDR Notation.
 *  \param[in]    defaultRouter       IP address of the default router (IPV4_ADDR_ANY specifies no default router).
 *                                    (gateway) is stored (struct member 'port' is not used and of
 *                                    arbitrary value)
 *  \return       E_OK                The request has been accepted.
 *  \return       E_NOT_OK            The request has not been accepted
 *  \pre          -
 *  \context      task level
 *  \reentrant    TRUE for IP address identifiers referencing different IP controllers.
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_RequestIpAddrAssignment(
    uint8                       ipAddrId,
    TcpIp_IpAddrAssignmentType  addrAssgnmentType,
    IpBase_AddrInType           localIpAddr,
    uint8                       netmaskCidr,
    IpBase_AddrInType           defaultRouter);

/***********************************************************************************************************************
 *  IpV4_Ip_ReleaseIpAddrAssignment
 **********************************************************************************************************************/
/*! \brief       Release all or a specific address assignment for an IP address.
 *  \details     -
 *  \param[in]   ipAddrId            IP address identifier of the IP address for which an assignment shall be released.
 *  \param[in]   addrAssignmentType  Type of assignment that shall be released.
 *                                   Value: TCPIP_IPADDR_ASSIGNMENT_(STATIC|LINKLOCAL|DHCP) or 0xFF (all).
 *  \return       E_OK      Address assignment released.
 *  \return       E_NOT_OK  Invalid address identifier or no address assignment could be released for this address.
 *  \context      ANY
 *  \reentrant    TRUE for different IP address identifiers.
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_ReleaseIpAddrAssignment(
    uint8                      ipAddrId,
    TcpIp_IpAddrAssignmentType addrAssignmentType);

/**********************************************************************************************************************
 *  IpV4_Ip_IsAddrIdAcceptable()
 **********************************************************************************************************************/
/*! \brief        This function checks whether the address identifier of a received packet is acceptable for a socket
 *                bound to a specific address identifier.
 *                The check is performed according to the acceptance filter given in [SWS_TCPIP_00106].
 *  \param[in]    BindIpAddrId           ip address identifier to which the socket has been bound
 *  \param[in]    RecvIpAddrId           ip address identifier of the received packet
 *  \param[out]   MatchType              specifies the type of the match (e.g. TCPIP_IF_MATCH, TCPIP_EXACT_MATCH)
 *  \return       TRUE                   packet is acceptable
 *  \return       FALSE                  packet is not acceptable
 *  \pre          -
 *  \context      interrupt or task level
 *  \reentrant    FALSE.
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(boolean, IPV4_CODE) IpV4_Ip_IsAddrIdAcceptable(IpV4_AddrIdType BindIpAddrId, IpV4_AddrIdType RecvIpAddrId, IPV4_P2V(uint8) MatchType);

/***********************************************************************************************************************
 *  IpV4_Ip_ResetSocket
 **********************************************************************************************************************/
/*! \brief        Reset parameters (e.g.: TTL, ToS, Frame Priority) for a specific socket to default values
 *  \details      -
 *  \param[in]    SockIdx             socket index
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_ResetSocket(TcpIp_SockIdxType SockIdx);

/***********************************************************************************************************************
 *  IpV4_Ip_SetTimeToLive
 **********************************************************************************************************************/
/*! \brief        Set socket specific value of the 'Time To Live' IPv4 header field used in outgoing packets
 *  \details      -
 *  \param[in]    SockIdx             socket index
 *  \param[in]    Value               value of the 'Time to Live' field
 *  \pre        -
 *  \context      task level
 *  \reentrant    FALSE.
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_SetTimeToLive(TcpIp_SockIdxType SockIdx, uint8 Value);

/***********************************************************************************************************************
 *  IpV4_Ip_SetTypeOfService
 **********************************************************************************************************************/
/*! \brief          Set socket specific value of the 'Type of Service' IPv4 header field used in outgoing packets
 *  \details        -
 *  \param[in]     SockIdx             socket index
 *  \param[in]     Value               value of the 'Type of Service' field
 *  \pre        -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_SetTypeOfService(TcpIp_SockIdxType SockIdx, uint8 Value);

/***********************************************************************************************************************
 *  IpV4_Ip_SetEthIfFramePrio
 **********************************************************************************************************************/
/*! \brief        Set socket specific Frame Priority value that is passed to the lower layer (EthIf)
 *  \details      -
 *  \param[in]    SockIdx             socket index
 *  \param[in]    Value               Frame priority value (0-7)
 *  \return       E_OK                Frame priority updated
 *  \return       E_NOT_OK            Value out of range
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_SetEthIfFramePrio(TcpIp_SockIdxType SockIdx, uint8 Value);

/***********************************************************************************************************************
 *  IpV4_Ip_SetPhysDestAddr
 **********************************************************************************************************************/
/*! \brief        Set socket specific value for the physical (MAC) destination address that is used in the outgoing packets.
 *  \details      -
 *  \param[in]    sockIdx             socket index
 *  \param[in]    physDestAddrPtr     Specifies the physical address value. (May be NULL_PTR to unset the value and reaneble ARP).
 *  \pre        -
 *  \context      
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \note         ARP will be disabled for the socket if a MAc address has been set.
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_SetPhysDestAddr(TcpIp_SockIdxType sockIdx, IPV4_P2C(uint8) physDestAddrPtr);

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_AddrConflictInd
 **********************************************************************************************************************/
/*! \brief        Indicate an address conflict in the ARP.
 *  \details      This function is used for IP LL address configuration.
 *  \param[in]    AddrId             ip address identifier
 *  \pre         -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_AddrConflictInd(
    IpV4_AddrIdType AddrId );

#endif  /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON */

/***********************************************************************************************************************
 *  IpV4_Ip_LlAddrInUse
 **********************************************************************************************************************/
/*! \brief        This function handles address conflicts
 *  \details       -
 *  \param[in]    IpCtrlIdx             ip controller index
 *  \param[in]    IpAddr                ip address that seems to be in use by other host
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_LlAddrInUse(
    uint8             IpCtrlIdx,
    IpBase_AddrInType IpAddr);

#if defined TCPIP_INV_DEM_EVENT_ID
# if (TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR > 0)
/***********************************************************************************************************************
 *  IpV4_GetLastDuplicateDhcpAddrDid
 **********************************************************************************************************************/
/*! \brief        This function returns the DID for DEM event TCPIP_E_DUPLICATE_DHCP_ADDR
 *  \details      -
 *  \param[in]    IpCtrlIdx             ip controller index
 *  \param[out]   IpAddrPtr             (own) ip address that seems to be in use by other host
 *  \param[out]   MacAddr               mac address of the other host
 *  \pre          -
 *  \context      task level
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_GetLastDuplicateDhcpAddrDid(
    uint8                                                         IpCtrlIdx,
    CONSTP2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA)      IpAddrPtr,
    CONSTP2VAR(IpBase_EthPhysAddrType, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr
    );
# endif
#endif

#define IPV4_STOP_SEC_CODE
#include "MemMap.h"

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
#endif  /* IPV4_IP_H */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Ip.h
 *********************************************************************************************************************/
