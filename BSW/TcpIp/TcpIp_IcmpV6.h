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
 *         \file  TcpIp_IcmpV6.h
 *        \brief  Implementation of Internet Control Message Protocol for IPv6 (ICMPv6)
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
 * IpV6_Icmp_Cfg.h:     IPV6_ICMP_CFG_H
 * IpV6_Icmp_Irq.h:     IPV6_ICMP_IRQ_H
 * IpV6_Icmp_Cbk.h:     IPV6_ICMP_CBK_H
 */
#if !defined(IPV6_ICMP_H)
#define IPV6_ICMP_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "Std_Types.h"

#include "TcpIp_IpV6.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/*  ICMPv6 error messages according to RFC 4443 */
#define IPV6_ICMP_MSG_TYPE_DEST_UNREACHABLE   1
#define IPV6_ICMP_MSG_TYPE_PKT_TOO_BIG        2
#define IPV6_ICMP_MSG_TYPE_TIME_EXCEEDED      3
#define IPV6_ICMP_MSG_TYPE_PARAM_PROBLEM      4

/* ICMPv6 informational messages according to RFC 4443 */
#define IPV6_ICMP_MSG_TYPE_ECHO_REQ         128
#define IPV6_ICMP_MSG_TYPE_ECHO_REPLY       129

/* ICMPv6 NDP messagges according to RFC 4861 */
#define IPV6_ICMP_MSG_TYPE_NDP_RS           133
#define IPV6_ICMP_MSG_TYPE_NDP_RA           134
#define IPV6_ICMP_MSG_TYPE_NDP_NS           135
#define IPV6_ICMP_MSG_TYPE_NDP_NA           136
#define IPV6_ICMP_MSG_TYPE_NDP_REDIR        137

/* ICMPv6 IND message according to RFC 3122 */
#define IPV6_ICMP_MSG_TYPE_NDP_INV_NS       141
#define IPV6_ICMP_MSG_TYPE_NDP_INV_NA       142

/* ICMPv6 MLD messagges according to RFC 3810 */
#define IPV6_ICMP_MSG_TYPE_MLD_QUERY        130
#define IPV6_ICMP_MSG_TYPE_MLD_REPORT       143
#define IPV6_ICMP_MSG_TYPE_MLD_V1_REPORT    131
#define IPV6_ICMP_MSG_TYPE_MLD_V1_DONE      132

#define IPV6_ICMP_MSG_TIME_EXCEEDED_CODE_HOPS_EXCEEDED   0U /* 0 - Hop limit exceeded in transit     */
#define IPV6_ICMP_MSG_TIME_EXCEEDED_CODE_TIME_EXCEEDED   1U /* 1 - Fragment reassembly time exceeded */

#define IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_ERR_HDR_FIELD   0U /* 0 - Erroneous header field encountered        */
#define IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_NXT_HDR 1U /* 1 - Unrecognized Next Header type encountered */
#define IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_OPTION  2U /* 2 - Unrecognized IPv6 option encountered      */

/* IPV6_ICMP_ERR_MSG_MAX_PAYLOAD_SIZE = IPV6_MINIMUM_MTU - IPV6_HDR_LEN - sizeof(IpV6_IcmpHeaderType) */
#define IPV6_ICMP_ERR_MSG_MAX_PAYLOAD_SIZE  (IPV6_MINIMUM_MTU - IPV6_HDR_LEN - 4U)

#define IPV6_NDP_RA_MIN_PAYLOAD_LEN         12U /* 16 - sizeof(IpV6_IcmpHeaderType) */
#define IPV6_NDP_NS_MIN_PAYLOAD_LEN         20U /* 24 - sizeof(IpV6_IcmpHeaderType) */
#define IPV6_NDP_NA_MIN_PAYLOAD_LEN         20U /* 24 - sizeof(IpV6_IcmpHeaderType) */
#define IPV6_NDP_REDIR_MIN_PAYLOAD_LEN      36U /* 40 - sizeof(IpV6_IcmpHeaderType) */
#define IPV6_NDP_IS_MIN_PAYLOAD_LEN         16U /* 24 - sizeof(IpV6_IcmpHeaderType) */
#define IPV6_NDP_IA_MIN_PAYLOAD_LEN         40U /* 48 - sizeof(IpV6_IcmpHeaderType) */

#define IPV6_ICMP_HEADER_OFS_TYPE            0U
#define IPV6_ICMP_HEADER_OFS_CODE            1U
#define IPV6_ICMP_HEADER_OFS_CHECKSUM        2U

#define IPV6_ICMP_HEADER_OFS_MULTIPART       4U
#define IPV6_ICMP_HEADER_LEN_MULTIPART       4U

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */
/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* example function pointer type definition */
typedef P2FUNC(void, IPV6_APPL_CODE, IpV6_Icmp_NotificationType) (void);

typedef struct
{
  uint8 Type;
  uint8 Code;
  uint16 ChecksumNbo;
} IpV6_IcmpHeaderType;

typedef struct
{
  uint16 IdentifierNbo;
  uint16 SequenceNumberNbo;
} IpV6_IcmpEchoRequestType;

typedef struct
{
  uint16 IdentifierNbo;
  uint16 SequenceNumberNbo;
} IpV6_IcmpEchoReplyType;

typedef struct
{
  uint32 PointerNbo; /* "Identifies the octet offset within the invoking packet where the error was detected." (stored in network byte order) */
} IpV6_IcmpParameterProblemErrMsgType;

typedef struct
{
  uint32 MtuNbo;
} IpV6_IcmpPacketTooBigMsgType;

typedef struct
{
  uint8  Length;
  uint8  Reserved;
  uint16 Reserved16;
} IpV6_IcmpTimeExceededMsgType;

typedef struct
{
  uint8  Length;
  uint8  Reserved;
  uint16 Reserved16;
} IpV6_IcmpDestUnreachableMsgType;

typedef struct
{
  uint8  VerReserved;
  uint8  Reserved;
  uint16 ChecksumNbo;
} IpV6_IcmpMultiPartExtHdr;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define IPV6_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Requirement Specification Template version */

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
 *  IpV6_Icmp_Init()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Icmp_Init(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Icmp_MainFunction()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Icmp_MainFunction(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Icmp_RxIndication()
 **********************************************************************************************************************/
/*! \brief         Receives an ICMPv6 packet.
 *  \details       -
 *  \param[in]     addrId              IPv6 address identifier.
 *  \param[in]     ethPayloadPtr       Payload of ethernet packet (starts with IPv6 header).
 *  \param[in]     ethPayloadLen       Length of ethernet payload in bytes.
 *  \param[in]     physAddrPtr         Physical address of the remote host.
 *  \param[in]     icmpHdrPos          Position of the ICMPv6 header inside of 'ethPayloadPtr'.
 *  \param[in]     checksumCalculated  Indicates if the ICMPv6 checksum was already verified by the hardware.
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Icmp_RxIndication(
  IpV6_AddrIdType         addrId,
  IPV6_P2V(uint8)         ethPayloadPtr,
  uint16                  ethPayloadLen,
  IPV6_P2C(uint8)         physAddrPtr,
  uint16                  icmpHdrPos,
  boolean                 checksumCalculated);

/**********************************************************************************************************************
 *  IpV6_Icmp_Transmit()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Icmp_Transmit(
  TcpIp_SockIdxType                         SockIdx,
  IPV6_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr,
  uint8                                     Type,
  uint8                                     Code,
  uint16                                    PayloadLen);

/**********************************************************************************************************************
 *  IpV6_Icmp_ProvideTxBuffer()
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, TCPIP_CODE) IpV6_Icmp_ProvideTxBuffer(
    IPV6_P2V(IpV6_AddrIdType)                 AddrIdPtr,
    TcpIp_SockIdxType                         SockIdx,
    IPV6_P2V(TcpIp_IpTxRequestDescriptorType) TxReqDescPtr,
    IPV6_P2C(IpV6_AddrType)                   DstAddrPtr,
    IPV6_P2V(uint8*)                          BufPtr,
    IPV6_P2V(uint16)                          BufLenPtr);

/***********************************************************************************************************************
 *  IpV6_Icmp_TxRawMessage
 **********************************************************************************************************************/
/*! \brief         Send a raw ICMPv6 message of specified Type and Code.
 *  \details       -
 *  \param[in,out] IpAddrIdPtr      IPv6 address identifier.
 *                                  If this is the ID of an AutoSelect or multicast address, the selected IP address id is returned.
 *  \param[in]     DestIpAddrPtr    Destination IPv6 address value.
 *  \param[in]     HopLimit         HopLimit value of the outgoing IPv6 packet.
 *  \param[in]     Type             Value of the Type field in the ICMPv6 message header.
 *  \param[in]     Code             Value of the Code field in the ICMPv6 message header.
 *  \param[in]     DataLength       Length of the message payload.
 *  \param[in]     DataPtr          Message payload.
 *  \return        BUFREQ_OK        Message was transmitted.
 *  \return        BUFREQ_E_BUSY    Message was not transmitted (link-layer address resolution pending or no ethernet buffer available).
 *  \return        BUFREQ_E_NOT_OK  Message was not transmitted (other error).
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        
 *  \pre           -
 *  \trace         CREQ-115987
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, TCPIP_CODE) IpV6_Icmp_TxRawMessage(
    TCPIP_P2V(IpV6_AddrIdType)     IpAddrIdPtr,
    TCPIP_P2C(IpV6_AddrType)       DestIpAddrPtr,
    uint8                          HopLimit,
    uint8                          Type,
    uint8                          Code,
    uint16                         DataLength,
    TCPIP_P2C(uint8)               DataPtr);

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Icmp_VTxErrParameterProblem()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Icmp_VTxErrParameterProblem(
  IpV6_AddrIdType                           AddrId,
  uint8 Code,
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) IpV6PktDataPtr,
  uint16 IpV6PktDataLen,
  uint16 IpV6PktDataOffs);
#endif

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Icmp_VTxTimeExceeded()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Icmp_VTxTimeExceeded(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  uint8                   Code,
  IPV6_P2V(uint8*)        OrigPktDataPtr,
  IPV6_P2V(uint16)        OrigPktDataLenPtr,
  IPV6_P2C(uint8)         MultiPartExtPtr,
  uint16                  MultiPartExtLen);
#endif

#if (IPV6_ENABLE_ICMP_ECHO_REPLY == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Icmp_Cbk_DestinationUnreachable()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Icmp_Cbk_DestinationUnreachable(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr);
#endif

/**********************************************************************************************************************
 *  IpV6_Icmp_VCalculateChecksumNbo()
 **********************************************************************************************************************/
FUNC(uint16, IPV6_CODE) IpV6_Icmp_VCalculateChecksumNbo(
  IPV6_P2C(IpV6_AddrType) SrcNetAddrPtr,
  IPV6_P2C(IpV6_AddrType) DstNetAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/***********************************************************************************************************************
 *  IpV6_Icmp_StoreChecksumInHeader
 **********************************************************************************************************************/
/*! \brief       Stores a calculated ICMPv6 checksum value at the correct position into the ICMPv6 header
 *  \param[in]   Checksum              Finalized checksum value.
 *  \param[in]   IcmpHdrPtr            Pointer the location of the ICMPv6 header.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Icmp_StoreChecksumInHeader(
    uint16           Checksum,
    IPV6_P2V(uint8) IcmpHdrPtr);

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif  /* IPV6_ICMP_H */
/**********************************************************************************************************************
 *  END OF FILE: IpV6_Icmp.h
 **********************************************************************************************************************/
