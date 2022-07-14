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
 *         \file  TcpIp_Ndp.h
 *        \brief  Implementation of Neighbor Discovery Protocol (NDP) for IPv6
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

/* configuration, interrupt handling and callback headers use different names
 * for the multiple inclusion protection define, shown in this example. The
 * appropriate names for these files can be taken from this list:
 *
 * IpV6_Icmp_Cfg.h:     IPV6_NDP_CFG_H
 * IpV6_Icmp_Irq.h:     IPV6_NDP_IRQ_H
 * IpV6_Icmp_Cbk.h:     IPV6_NDP_CBK_H
 */
#if !defined(IPV6_NDP_H)
#define IPV6_NDP_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "EthIf.h"
#include "Std_Types.h"

#include "TcpIp_IpV6.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

#define IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR    1u /* Source Link-Layer Address */
#define IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR    2u /* Target Link-Layer Address */
#define IPV6_NDP_OPT_TYPE_PREFIX_INFO       3u /* Prefix Information */
#define IPV6_NDP_OPT_TYPE_REDIR_HEADER      4u /* Redirected Header */
#define IPV6_NDP_OPT_TYPE_MTU               5u /* MTU */

#define IPV6_NDP_OPT_TYPE_SOURCE_ADDR_LIST  9u
#define IPV6_NDP_OPT_TYPE_TARGET_ADDR_LIST 10u

#define IPV6_NDP_OPT_TYPE_RDNSS            25u /* [RFC6106 5.1. Recursive DNS Server Option] */
#define IPV6_NDP_OPT_TYPE_DNSSL            31u /* [RFC6106 5.2. DNS Search List Option] */

#define IPV6_NDP_RA_FLAG_MANAGED              (1<<7) /* managed address configuration flag */
#define IPV6_NDP_RA_FLAG_OTHER                (1<<6) /* other configuration flag */

#define IPV6_NDP_OPT_PREFIX_INFO_FLAG_ON_LINK (1<<7) /* indicates prefix can be used for on-link determination */
#define IPV6_NDP_OPT_PREFIX_INFO_FLAG_SLAAC   (1<<6) /* indicates prefix can be used for SLAAC */

#define IPV6_NDP_OPT_LL_ADDR_LEN_BYTE       8u /* length of a (source/target)-link-layer address option in bytes */

#define IPV6_NDP_MSG_HOP_LIMIT            255u /* mandatory hop limit value for all NDP messages */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */
/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*  Router Solicitation Message Format (see RFC4861)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |     Type      |     Code      |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |                            Reserved                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint32 Reserved32; /* must be set to 0 when sent and ignored when received */
  /* [...] TLV encoded options */
} IpV6_NdpRsType;

/*  Router Advertisement Message Format (see RFC4861)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 134   |   Code = 0    |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  | Cur Hop Limit |M|O|  Reserved |       Router Lifetime         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Reachable Time                        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          Retrans Timer                        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint8  CurHopLimit;       /* Default Hop Count in IPv6 header of outgoing packets */
  uint8  MOReserved;        /* "(M)anaged address configuration" and "(O)ther configuration" flag. */
  uint16 RouterLifetimeNbo; /* lifetime of router in seconds */
  uint32 ReachableTimeNbo;  /* Time (ms) node assumes neighbor is reachable after reachability confirmation. (NUD) */
  uint32 RetransTimerNbo;   /* Time (ms) between retransmitted Neighbor Solicitation messages. (AR and NUD) */
  /* [...] TLV encoded options */
} IpV6_NdpRaType;

/*  Neighbor Solicitation Message Format (see RFC4861)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 135   |   Code = 0    |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |                           Reserved                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                       Target Address                          +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint32        Reserved32;
  IpV6_AddrType TargetAddress;
  /* [...] TLV encoded options */
} IpV6_NdpNsType;

/*  Neighbor Advertisement Message Format (see RFC4861)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 136   |   Code = 0    |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |R|S|O|Reserved |                 Reserved                      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                       Target Address                          +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint8         RSOReserved;
  uint8         Reserved8[3];
  IpV6_AddrType TargetAddress;
  /* [...] TLV encoded options */
} IpV6_NdpNaType;

/*  Redirect Message Format (see RFC4861)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 137   |   Code = 0    |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |                           Reserved                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                       Target Address                          +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                     Destination Address                       +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint32        Reserved32Nbo;
  IpV6_AddrType TargetAddress;
  IpV6_AddrType DestinationAddress;
  /* [...] TLV encoded options */
} IpV6_NdpRedirType;

/*  [RFC3122 2.1 Inverse Neighbor Discovery Solicitation Message]
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |     Type      |     Code      |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |                            Reserved                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint32 Reserved32; /* must be set to 0 when sent and ignored when received */
  /* [...] TLV encoded options */
} IpV6_NdpIsType;

/*  [RFC3122 2.2 Inverse Neighbor Discovery Advertisement Message]
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |     Type      |     Code      |          Checksum             |  } ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |                            Reserved                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 */
typedef struct
{
  uint32 Reserved32; /* must be set to 0 when sent and ignored when received */
  /* [...] TLV encoded options */
} IpV6_NdpIaType;

/* NDP OPTIONS */

/*  Source/Target Link-layer Address */
/*
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |    Length     |    Link-Layer Address ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8 Type;   /* 1 for Source Link-layer Address, 2 for Target Link-layer Address */
  uint8 Length; /* length in units of 8 octets (1 for ethernet link-layer) */
  uint8 addr[ETH_PHYS_ADDR_LEN_BYTE];
} IpV6_NdpOptLLAddrType;

/*  Prefix Information
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |    Length     | Prefix Length |L|A| Reserved1 |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Valid Lifetime                        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                       Preferred Lifetime                      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           Reserved2                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                            Prefix                             +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8  Type;                 /* must be 3 */
  uint8  Length;               /* must be 4 (4 * 8 octests) */
  uint8  PrefixLength;         /* number of leading bits in the Prefix that are valid */
  uint8  LAReserved;           /* unused, must be set to zero by sender and ignored by receiver */
  uint32 ValidLifetimeNbo;     /* time in seconds the prefix is valid (network byte order) */
  uint32 PreferredLifetimeNbo; /* time in seconds addresses with this prefix are preferred (network byte order) */
  uint32 Reserved32Nbo;        /* unused, must be set to zero by sender and ignored by receiver (network byte order) */
  IpV6_AddrType  Prefix;       /* */
} IpV6_NdpOptPrefixInfoType;

/*  Redirected Header
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |    Length     |            Reserved           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           Reserved                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  ~                       IP header + data                        ~
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8  Type;          /* must be 4 */
  uint8  Length;        /* option length in units of 8 octets */
  uint16 Reserved16Nbo; /* unused, must be set to zero be sender and ignored by receiver */
  uint32 Reserved32Nbo; /* unused, must be set to zero be sender and ignored by receiver */
  /* IP header + data */
} IpV6_NdpOptRedirHeaderType;

/*  MTU
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |    Length     |           Reserved            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                              MTU                              |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8  Type;          /* must be 5 */
  uint8  Length;        /* must be 1 (1 * 8 octets) */
  uint16 Reserved16Nbo; /* unused, must be set to zero by sender and ignored by receiver */
  uint32 MTUNbo;        /* recommended MTU for the link */
} IpV6_NdpOptMtuType;

#if (IPV6_ENABLE_RFC6106_RDNSS_OPT == STD_ON)
/*  Recursive DNS Server (RDNSS) Option Format (see RFC6106 5.1.)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |     Length    |           Reserved            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           Lifetime                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  :            Addresses of IPv6 Recursive DNS Servers            :
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8  Type;          /*!< "Identifier of the RDNSS option type as assigned by the IANA: 25" */
  uint8  Length;        /*!< "The length of the option (including the Type and Length fields) is in units of 8 octets." */
  uint16 Reserved16Nbo; /*!< Reserved field */
  uint32 LifetimeNbo;   /*!< "The maximum time, in seconds (relative to the time the packet is sent), over which this RDNSS address MAY be used for name resolution." */
} IpV6_NdpOptRdnssType;
#endif

#if (IPV6_ENABLE_RFC6106_DNSSL_OPT == STD_ON)
/*  DNS Search List (DNSSL) Option Format (see RFC6106 5.2.)
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |     Length    |           Reserved            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           Lifetime                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  :                Domain Names of DNS Search List                :
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8  Type;          /*!< "Identifier of the DNSSL option type as assigned by the IANA: 31" */
  uint8  Length;        /*!< "The length of the option (including the Type and Length fields) is in units of 8 octets." */
  uint16 Reserved16Nbo; /*!< Reserved filed */
  uint32 LifetimeNbo;   /*!< "The maximum time, in seconds (relative to the time the packet is sent), over which this DNSSL domain name MAY be used for name resolution." */
} IpV6_NdpOptDnsslType;
#endif

#if ((IPV6_ENABLE_NDP_INV_NA == STD_ON) || (IPV6_ENABLE_NDP_INV_NS == STD_ON))
/*  Source/Target Address List
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |    Length   |                                 |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+        -       -       -        +
 *  |                          Reserved                             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                        IPv6 Address                           +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |
 *  ~
 *  |
 *  +-+-+-+-+...
 */
typedef struct
{
  uint8  Type;          /*!< 9 for Source Address List, 10 for Target Address List */
  uint8  Length;        /*!< 3 + (n - 1) for n Addresses */
  uint16 Reserved16Nbo; /*!< Reserved filed */
  uint32 Reserved32Nbo; /*!< Reserved filed */
} IpV6_NdpOptAddrListType;
#endif

typedef struct
{
  IpV6_AddrType addr;
} IpV6_Ndp_RouterEntryType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define IPV6_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV6_Ndp_VTxNeighborAdvertisement()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxNeighborAdvertisement(
  IPV6_P2V(IpV6_AddrIdType) AddrIdPtr,
  IPV6_P2C(IpV6_AddrType)   DstAddrPtr,
  IPV6_P2C(IpV6_AddrType)   TargetAddrPtr,
  uint8                     RSOFlags);

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxRouterAdvertisement()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VRxRouterAdvertisement(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxNeighborSolicitation()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VRxNeighborSolicitation(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         PhysAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxNeighborAdvertisement()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VRxNeighborAdvertisement(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         PhysAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxRedirect()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VRxRedirect(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/**********************************************************************************************************************
 *  IpV6_Ndp_VTxNeigborSolicitation()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxNeigborSolicitation(
  IPV6_P2V(IpV6_AddrIdType) SrcAddrIdPtr,
  IPV6_P2C(IpV6_AddrType)   TargetAddrPtr,
  boolean                   Unicast,
  boolean                   ForceUnspecSrcAddr);

#if (IPV6_ENABLE_ROUTER_SOLICITATIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VTxRouterSolicitation()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxRouterSolicitation(
  IPV6_P2V(IpV6_AddrIdType) SrcAddrIdPtr,
  boolean                   IncludeSllaOpt);
#endif

extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxInverseAdvertisement(
  IPV6_P2V(IpV6_AddrIdType)  SrcAddrIdPtr,
  IPV6_P2C(IpV6_AddrType)    DstAddrPtr,
  IPV6_P2C(Eth_PhysAddrType) TgtLLAddrPtr);

extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxInverseSolicitation(
  IpV6_AddrIdType            AddrId,
  IPV6_P2C(uint8)            TgtLLAddrPtr,
  boolean                    IncludeSrcAddrList);

extern FUNC(void, IPV6_CODE) IpV6_Ndp_VRxInverseSolicitation(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

extern FUNC(void, IPV6_CODE) IpV6_Ndp_VRxInverseAdvertisement(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_SendInverseSolicitation(
  uint8                      IpCtrlIdx,
  IPV6_P2C(uint8)            TgtLLAddrPtr,
  boolean                    IncludeSrcAddrList);

/**********************************************************************************************************************
 *  IpV6_Ndp_VTxDad()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxDad(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VAddSourceAddress()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VAddSourceAddress(
  VAR(uint8, AUTOMATIC) AddrId,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) AddressPtr,
  uint32  PreferredLifetime,
  uint32  ValidLifetime,
  uint32  DelayMs,
  uint8   DadMode);

/**********************************************************************************************************************
 *  IpV6_Ndp_VChangeLocalAddrState()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VChangeLocalAddrState(
  IpV6_AddrIdType       AddrId,
  IpV6_LocAddrStateType NewState);

/* === PREFIX LIST FUNCTIONS === */

/**********************************************************************************************************************
 *  IpV6_Ndp_VPLLookup()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VPLLookup(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) AddrPrefixPtr,
  uint8                   PrefixLength,
  IPV6_P2V(uint8)         EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VPLGetOrCreateEntry()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VPLGetOrCreateEntry(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) AddrPrefixPtr,
  uint8                   PrefixLength,
  IPV6_P2V(uint8)         EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VPLRemoveEntry()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VPLRemoveEntry(
  uint8 IpCtrlIdx,
  uint8 EntryIdx);

/* === DESTINATION CACHE FUNCTIONS === */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCLookup()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, AUTOMATIC) IpV6_Ndp_VDCLookup(
  uint8                      IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)    AddrPtr,
  IPV6_P2V(IpV6_ListIdxType) EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCGetOrCreateEntry()
 **********************************************************************************************************************/
extern FUNC(void, AUTOMATIC) IpV6_Ndp_VDCGetOrCreateEntry(
  uint8                      IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)    DstAddrPtr,
  IPV6_P2V(IpV6_ListIdxType) EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCRemoveEntry()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VDCRemoveEntry(
  uint8            IpCtrlIdx,
  IpV6_ListIdxType EntryIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCRemoveEntrys()
 **********************************************************************************************************************/
extern FUNC(void, AUTOMATIC) IpV6_Ndp_VDCRemoveEntrys(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) NextHopAddrPtr);

/* === DEFAULT ROUTER LIST FUNCTIONS === */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLCreateEntry()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VDRLCreateEntry(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) RouterAddrPtr,
  IPV6_P2V(uint8)         EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLLookup()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VDRLLookup(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) RouterAddrPtr,
  IPV6_P2V(uint8)         EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLRemoveEntry()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VDRLRemoveEntry(
  uint8            IpCtrlIdx,
  IpV6_ListIdxType EntryIdx);

/* === NEIGHBOR CACHE FUNCTIONS === */

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCLookup()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VNCLookup(
  uint8                      IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)    NetAddrPtr,
  IPV6_P2V(IpV6_ListIdxType) EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCGetOrCreateEntry()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VNCGetOrCreateEntry(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) AddrPtr,
  IPV6_P2V(uint8)         EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCUpdateEntry()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VNCUpdateEntry(
  uint8                   IpCtrlIdx,
  IpV6_ListIdxType        EntryIdx,
  IpV6_ReachableStateType NewState);

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCRemoveEntry()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VNCRemoveEntry(
  uint8            IpCtrlIdx,
  IpV6_ListIdxType EntryIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCUpdateLinkLayerAddress()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VNCUpdateLinkLayerAddress(
  uint8           IpCtrlIdx,
  uint8           NcEntryIdx,
  IPV6_P2C(uint8) NewLinkLayerAddressPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VStart()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_VStart(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_Init()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_Init(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_MainFunction()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Ndp_MainFunction(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  IpV6_Ndp_ClearCache
 **********************************************************************************************************************/
/*! \brief        IpV6_Ndp_ClearCache clears the address resolution cache
 *  \param[in]    IpAddrId            Local address identifier implicitely specifing the IPv6 controller that shall
 *                                    be cleared
 *  \return       E_OK                The request was successful 
 *                E_NOT_OK            The request was not successful
 *  \context      task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_ClearCache(
    IpV6_AddrIdType IpAddrId );

/***********************************************************************************************************************
 *  IpV6_Ndp_GetCacheEntries
 **********************************************************************************************************************/
/*! \brief        IpV6_Ndp_GetCacheEntries returns the address resolution cache
 *  \details      Copies entries from the physical address cache of the IPv6 instance that is active on the EthIf
 *                controller specified by ctrlIdx into a user provided buffer. The function will copy all or
 *                numberOfElements into the output list. If input value of numberOfElements is 0 the function will
 *                not copy any data but only return the number of valid entries in the cache. EntryListPtr may be
 *                NULL_PTR in this case.
 *  \param[in]    ctrlIdx             EthIf controller index to identify the related NDP table.
 *  \param[inout] numberOfElements    In: Maximum number of entries that can be stored in Parameters output entryListPtr. 
 *                                    Out: Number of entries written to output entryListPtr
 *                                    (Number of all entries in the cache if input value is 0).
 *  \param[out]   entryListPtr        Pointer to memory where the list of cache entries shall be stored.
 *  \return       E_OK                physical address cache could be read.
 *  \return       E_NOT_OK            physical address cache could not be read.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        CREQ-115906
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) IpV6_Ndp_GetCacheEntries(
    uint8 ctrlIdx,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_VAR) numberOfElements,
    P2VAR(TcpIp_NdpCacheEntryType, AUTOMATIC, TCPIP_APPL_VAR) entryListPtr );

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif  /* IPV6_NDP_H */

/**********************************************************************************************************************
 *  END OF FILE: IpV6_Ndp.h
 **********************************************************************************************************************/
