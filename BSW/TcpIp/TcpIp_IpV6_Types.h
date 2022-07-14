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
 *         \file  TcpIp_IpV6_Types.h
 *        \brief  Implementation of Internet Protocol version 6 (IPv6) - Type Definitions
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

#if !defined(IPV6_TYPES_H)
#define IPV6_TYPES_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_IpV6_Cfg.h"

#include "Std_Types.h"
#include "EthIf.h"
#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* static header byte lengths */
#define IPV6_HDR_LEN                   40u
#define IPV6_FRAGMENT_EXT_HDR_LEN       8u

/* next header types */
#define IPV6_EXT_HDR_ID_HOP_BY_HOP      0u
#define IPV6_EXT_HDR_ID_DST_OPT        60u
#define IPV6_EXT_HDR_ID_ROUTING        43u
#define IPV6_EXT_HDR_ID_FRAGMENT       44u
#define IPV6_EXT_HDR_ID_AUTH           51u
#define IPV6_EXT_HDR_ID_ESP            50u

/* upper layer header types */
#define IPV6_HDR_ID_ICMP               58u
#define IPV6_HDR_ID_TCP                 6u
#define IPV6_HDR_ID_UDP                17u

#define IPV6_HDR_ID_NO_NEXT_HDR        59u

/* ip adddress configuration sources */
#define IPV6_ADDR_CFG_SRC_LL                     0U
#define IPV6_ADDR_CFG_SRC_STATIC                 1U
#define IPV6_ADDR_CFG_SRC_RA                     2U
#define IPV6_ADDR_CFG_SRC_DHCP                   3U
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
# define IPV6_ADDR_CFG_SRC_PRIV_EXT              4U
#endif
#define IPV6_ADDR_CFG_SRC_UNSPECIFIED          255U

/* extension header option types */
#define IPV6_EXT_HDR_OPT_TYPE_PAD_1         0u /*!< padding option, just one padding byte */
#define IPV6_EXT_HDR_OPT_TYPE_PAD_N         1u /*!< padding option for two or more padding bytes */

#define IPV6_HBH_OPT_TYPE_RTR_ALERT         5U /*!< Router Alert Option Type [RFC2711 IPv6 Router Alert Option] */
#define IPV6_HBH_OPT_RTR_ALERT_LEN          2U /*!< Router Alert Option Length [RFC2711 IPv6 Router Alert Option] */
#define IPV6_HBH_HDR_MIN_LEN                0U /*!< Minimum Length of Hop-by-Hop Options Header [RFC2460 4.3.] in units of 8 octets (not incliding first 8 octets) */

#define IPV6_TLV_OPT_DATA_OFFS   (uint8)2u /*!< Type-Length-Value option data begins after 1-byte type and 1-byte length field */

#define IPV6_BITS_IN_BYTE               8u /*!< each byte has 8 bits */
#define IPV6_ADDRESS_LEN_BYTE          16u /*!< length of an IPv6 address in bytes */
#define IPV6_ADDRESS_LEN_BIT (IPV6_ADDRESS_LEN_BYTE * IPV6_BITS_IN_BYTE) /*!< length of an IPv6 address in bits (128) */

/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

#define IPV6_P2C(VAR) P2CONST(VAR, AUTOMATIC, IPV6_APPL_DATA)
#define IPV6_P2V(VAR) P2VAR(VAR,   AUTOMATIC, IPV6_APPL_VAR)

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8  IpV6_ListIdxType; /*!< basic type for data structure indexes */

typedef uint8  IpV6_AddrIdType;
typedef uint8  IpV6_CtrlAddrIdxType;

typedef struct
{
  uint32 S;  /*!< seconds  */
  uint32 Ms; /*!< milliseconds (< 1000) */
} IpV6_TimeType;

typedef IpBase_AddrIn6Type IpV6_AddrType;

typedef struct
{
  IpV6_AddrType Prefix;
  uint8         PrefixLenBits;
} IpV6_AddrPrefixType;

/*! \brief main IPv6 header format. Every IPv6 packet begins with this header.
 *
 *  \verbatim
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |Version| Traffic Class |           Flow Label                  |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |         Payload Length        |  Next Header  |   Hop Limit   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                         Source Address                        +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                      Destination Address                      +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  \endverbatim
 *  \note This structure has to pe packed and MUST NOT be reordered since it is casted directly over a linear buffer.
 */
typedef struct
{
  uint32         VersionTcFlNbo; /*!< 4-bit version number (6), 8-bit traffic class, 20-bit flow label (Network-Byte-Order) */
  uint16         PayloadLenNbo;  /*!< number of octets following this header (Network-Byte-Order) */
  uint8          NextHdr;        /*!< type of header immediately following this header */
  uint8          HopLimit;       /*!< packet is discarded if Hop Limit is zero */
  IpV6_AddrType  SrcAddr;        /*!< 128-bit source IPv6 address */
  IpV6_AddrType  DstAddr;        /*!< 128-bit destination IPv6 address */
} IpV6_HdrType;

#if ((IPV6_ENABLE_RX_FRAGMENTATION == STD_ON) || (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON))
/*! \brief Fragment Extension Header
 *
 *  This header has a fixed length of IPV6_FRAGMENT_EXT_HDR_LEN.
 *  \verbatim
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  Next Header  |   Reserved    |      Fragment Offset    |Res|M|
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Identification                        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  \endverbatim
 *  \note This structure has to pe packed and MUST NOT be reordered since it is casted directly over a linear buffer.
 */
typedef struct
{
  uint8  NextHdr;            /*!< type of header immediately following this header */
  uint8  Reserved;           /*!< length of this header in 8-octet units, not including the first 8 octets */
  uint16 FragmentOffsetNbo;  /*!< 13-bit fragment offset in 8-octet units, 2-bit reserved, 1-bit M-flag (more fragments) (Network-Byte-Order) */
  uint32 IdNbo;              /*!< 32-bit identifiation (Network-Byte-Order) */
} IpV6_FragmentExtHdrType;

# define IPV6_FRAGMENT_EXT_HDR_FIELD_POS_NEXT_HEADER     0u
# define IPV6_FRAGMENT_EXT_HDR_FIELD_POS_FRAGMENT_OFFSET 2u

#endif

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
typedef struct
{
  uint16 StartOfs;
  uint16 EndOfs;
} IpV6_SegmentType;

typedef struct
{
  uint8            SegCount; /*!< maximum number of segments */
  uint8            SegUsed;  /*!< currently used segements */
  P2VAR(IpV6_SegmentType, AUTOMATIC, IPV6_APPL_VAR) Segments;
} IpV6_SegmentListType;

typedef struct
{
  uint32                                 FragmentId;        /*!< fragment identification from fragment extension header */
  IpV6_AddrType                          SrcAddr;           /*!< source address of fragments */
  IpV6_SegmentListType                   RxSegList;         /*!< list of data segments that have already been received */
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) Buffer;            /*!< pointer to data buffer for the reassembled packet */
  IpV6_CtrlAddrIdxType                   IpCtrlAddrIdx;     /*!< destination address of fragments */
  boolean                                FirstReceived;     /*!< indicates if the first fragment (Offset==0) has been received */
  boolean                                LastReceived;      /*!< indicates if the last fragment has been received */
  uint16                                 FirstFragHdrOfs;   /*!< offset of fragmentable part in reassembled packet (part following the fragment header) */
  uint16                                 OrigPktPayloadLen; /*!< payload length of reassembled IPv6 packet */
  uint16                                 FirstFragPktLen;   /*!< packet length of the first fragment (needed for sending correct TimeExceeded message) */
  IpV6_TimeType                          ReassemblyTimeout; /*!< absolute timeout for reassembly */
  uint8                                  Status;            /*!< status of this reassembly buffer */
  uint16                                 NextHdrFieldOfs;   /*!< offset of the next header field in the header preceeding the fragment header */
} IpV6_ReassemblyBufferDescType;
#endif

#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
typedef struct
{
  uint32                                 FragmentId;        /*!< fragment identification from fragment extension header */
  uint16                                 UnfragPartLen;     /*!< length of unfragmentable part in bytes */
  uint16                                 FragPartLen;       /*!< length of fragmentable part in bytes */
  uint16                                 MaxFragLen;        /*!< length of a single fragment */
  uint16                                 TxOfs;             /*!< indicates how many bytes of unfragmentable part have been transmitted */
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) Buffer;            /*!< pointer to data buffer containing the packet to be fragmented */
  Eth_PhysAddrType                       NextHopPhysAddr;   /*!< link-layer address of next hop */
  uint8                                  Status;            /*!< indicates if this buffer conatins fragments to send */
} IpV6_FragmentTxBufferDescType;
#endif

#if (IPV6_ENABLE_DLISTS == STD_ON)

typedef uint8 IpV6_DListIdxType;

/* DList element (array of elements is managed by descriptor) */
typedef struct
{
  IpV6_DListIdxType            PrevIdx; /* index of the predecessor */
  IpV6_DListIdxType            NextIdx; /* index of the successor */
} IpV6_DListNodeType;

/* DList descriptor (can be shared by multiple instances) */
typedef struct
{
  IpV6_DListIdxType            FirstFreeIdx; /* index of first element in free list */
  IpV6_DListIdxType            LastFreeIdx;  /* index of last element in free list */
  uint8                        ElementCount; /* number of elements in list */
  IPV6_P2V(IpV6_DListNodeType) NodePtr;      /* pointer to element array of this list */
} IpV6_DListDescType;

/* DList instance */
typedef struct
{
  IpV6_DListIdxType            FirstIdx;     /* index of first element in list */
  IpV6_DListIdxType            LastIdx;      /* index of last element in list */
  uint8                        ElementCount; /* number of elements in list */
  IPV6_P2V(IpV6_DListDescType) DescPtr;      /* pointer to list descriptor of the list */
} IpV6_DListType;

#endif

typedef enum
{
  IPV6_REACHABLE_STATE_INCOMPLETE,
  IPV6_REACHABLE_STATE_STALE,
  IPV6_REACHABLE_STATE_DELAY,
  IPV6_REACHABLE_STATE_PROBE,
  IPV6_REACHABLE_STATE_REACHABLE
} IpV6_ReachableStateType;

typedef enum
{
  IPV6_LADDR_STATE_INVALID    = 0,
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
  IPV6_LADDR_STATE_TENTATIVE  = 1,
# if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
  IPV6_LADDR_STATE_OPTIMISTIC = 2, /* [RFC4429 1.3. Address Types] */
# endif
#endif
  IPV6_LADDR_STATE_PREFERRED  = 3,
  IPV6_LADDR_STATE_DEPRECATED = 4
} IpV6_LocAddrStateType;

typedef enum
{
  IPV6_MULTICAST_SCOPE_RESERVED    = 0x0,
  IPV6_MULTICAST_SCOPE_IF_LOCAL    = 0x1,
  IPV6_MULTICAST_SCOPE_LINK_LOCAL  = 0x2,
  IPV6_MULTICAST_SCOPE_ADMIN_LOCAL = 0x4,
  IPV6_MULTICAST_SCOPE_SITE_LOCAL  = 0x5, /* deprecated */
  IPV6_MULTICAST_SCOPE_ORG_LOCAL   = 0x8,
  IPV6_MULTICAST_SCOPE_GLOBAL      = 0xE
} IpV6_MulticastScopeType;

typedef enum
{
  IPV6_MULTICAST_TYPE_ALL_NODES,
  IPV6_MULTICAST_TYPE_ALL_ROUTERS,
  IPV6_MULTICAST_TYPE_SOLICITED_NODE
} IpV6_MulticastType;

/* IpV6_NeighborCacheEntryType */
typedef struct
{
  IpV6_AddrType           NeighborAddress;    /*!< [16 byte] address of the neighbor */
#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
  IpV6_AddrType           InvNsSrcAddress;    /*!< [16 byte] source address of received inverse neighbor solicitation (RFC4861 7.2.2.) */
#endif
  IpV6_AddrType           NaTgtAddress;       /*!< [16 byte] target address of outgoing neighbor advertisement (if solicited) */
  IpV6_TimeType           NextEventTime;      /*!< [ 8 byte] next expiration time */
  IpV6_TimeType           LastUsedTime;       /*!< [ 8 byte] last used time */
  Eth_PhysAddrType        LinkLayerAddress;   /*!< [ 6 byte] link-layer address of the neighbor */
  IpV6_ReachableStateType State;              /*!< [ 1 byte] current reachability state of the neighbor */
  uint8                   NaRSOFlags;         /*!< [ 1 byte] flags that should be set in outgoing neighbor advertisement (if solicited) */
  uint8                   RetryCount;         /*!< [ 1 byte] number of AR, NUD probes */
  IpV6_CtrlAddrIdxType    NsSrcAddressIdx;    /*!< [ 1 byte] source address of received neighbor solicitation (RFC4861 7.2.2.) */
  boolean                 IsRouter;           /*!< [ 1 byte] indicates wether the neighbor is a router */
  boolean                 NaPending;          /*!< [ 1 byte] indicates whether an advertisement has been solicited and should be sent */
#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
  boolean                 InvNaPending;       /*!< [ 1 byte] indicates whether an inverse advertisement has been solicited and should be sent */
#endif                                        /*   ========= */
  /* PendingPackets */                        /*    77 byte + padding */
} IpV6_NeighborCacheEntryType;

/* IpV6_DestinationCacheEntryType */
typedef struct
{
  IpV6_AddrType        DestinationAddress;
  IpV6_AddrType        NextHopAddress;     /*!< address of next hop neighbor (should be in neighbor cache) */
  IpV6_TimeType        LastUsedTime;       /*!< timestamp of last usage (for LRU replacement) */
  IpV6_CtrlAddrIdxType SourceAddressIdx;   /*!< selected source address for this destination */
#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
  boolean              DestinationOnLink;
#endif
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
  IpV6_ListIdxType NcIdxHint;
#endif
#if (IPV6_ENABLE_PATH_MTU == STD_ON)
  uint16           MtuReported;
  uint16           Mtu;
  IpV6_TimeType    MtuTimeout;
#endif
} IpV6_DestinationCacheEntryType;

/* IpV6_PrefixListEntryType */
typedef struct
{
  IpV6_AddrType       Prefix;
  IpV6_TimeType       ExpireTime;
  uint8               PrefixLenBits;
} IpV6_PrefixListEntryType;

/* IpV6_DefaultRouterListEntryType */
typedef struct
{
  IpV6_AddrType    NextHopAddress;
  IpV6_TimeType    ExpireTime;
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
  IpV6_ListIdxType NcIdxHint;
#endif
} IpV6_DefaultRouterListEntryType;

/* IpV6_DefaultRouterListEntryType (according to RFC 4291 2.4.) */
typedef enum
{
  IPV6_ADDR_TYPE_UNSPECIFIED, /*!< ::/128     0000:0000:0000:0000:0000:0000:0000:0000 */
  IPV6_ADDR_TYPE_LOOPBACK,    /*!< ::1/128    0000:0000:0000:0000:0000:0000:0000:0001 */
  IPV6_ADDR_TYPE_MULTICAST,   /*!< ff00::/8   FF__:____:____:____:____:____:____:____ */
  IPV6_ADDR_TYPE_LL_UNICAST,  /*!< fe80::/10  FE80:0000:0000:0000:____:____:____:____ */
  IPV6_ADDR_TYPE_UNICAST,     /*!< everythis else */
  IPV6_ADDR_TYPE_INVALID
} IpV6_AddrTypeType;

typedef struct
{
  IpV6_AddrType              Address;          /*!< address */
  IpV6_TimeType              NextEventTime;    /*!< absolute time of next event for this address */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListType             TempAddrs;        /*!< dynamic list containing temporary addresses for this prefix */
#endif
  IpV6_TimeType              ValidTime;        /*!< absolute time until this address is valid */
  IpV6_TimeType              PreferredTime;    /*!< absolute time until this address is preferred */
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
  uint8                      DadNsTxRemain;    /*!< number of DAD NS that have to be sent for this address */
# if (IPV6_MULTICAST_LOOPBACK == STD_ON)
  uint8                      DadNsRxCount;     /*!< number of DAD NS that have been received for this address */
# endif
#endif
  IpV6_LocAddrStateType      State;            /*!< current state of this address */
} IpV6_SourceAddressTableEntryType;

typedef uint8 IpV6_AddrEventType;

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
#define IPV6_LADDR_EVENT_TX_DAD           0
#endif
#define IPV6_LADDR_EVENT_TO_PREFERRED     1
#define IPV6_LADDR_EVENT_TO_DEPRECATED    2
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
#define IPV6_LADDR_EVENT_TEMP_ADDR_REGEN  3
#endif
#define IPV6_LADDR_EVENT_EXPIRE           4

typedef struct
{
  IpV6_AddrType              TempAddress;      /*!< temporary address */
  IpV6_TimeType              ValidTime;        /*!< absolute time until this address is valid */
  IpV6_TimeType              PreferredTime;    /*!< absolute time until this address is preferred */
  IpV6_TimeType              CreationTime;     /*!< absolute creation time of this address */
  IpV6_TimeType              NextEventTime;    /*!< absolute time of next event for this address */
  IpV6_AddrEventType         NextEvent;        /*!< type of next event for this address */
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
  uint8                      DadNsTxRemain;    /*!< number of DAD NS that have to be transmitted for this address */
  uint8                      DadRetriesRemain; /*!< number of DAD retries left for this address */
#endif
  IpV6_LocAddrStateType      State;            /*!< current state of this address */
} IpV6_TempSrcAddrType;

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif
/* IPV6_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: IpV6_Types.h
 *********************************************************************************************************************/
