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
 *         \file  TcpIp_Mld.h
 *        \brief  Implementation of Multicast Listener Discovery version 2 (MLDv2) for IPv6
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

#if !defined(IPV6_MLD_H)
#define IPV6_MLD_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

#include "TcpIp_IpV6_Cfg.h"
#if (IPV6_ENABLE_MLD == STD_ON)

/**********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "TcpIp_IpV6_Priv.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

#define IPV6_MLD_MSG_HOP_LIMIT 1U /* [RFC3810 5. Message Formats] */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

/* PRQA S 3410 1 */ /* MD_IpV6_19.10 */
#define IPV6_MLD_VAR(VAR_NAME) (IpV6_Mld_Data[IpCtrlIdx]->VAR_NAME)

/* PRQA L:FctLikeMacros */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/*
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 130   |      Code     |           Checksum            |  +- ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |    Maximum Response Code      |           Reserved            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  *                                                               *
 *  |                                                               |
 *  *                       Multicast Address                       *
 *  |                                                               |
 *  *                                                               *
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint16        MaxResDelayNbo;
  uint16        Reserved16Nbo;
  IpV6_AddrType MulticastAddr;
} IpV6_Mld_V1QueryHeaderType;

/*
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 130   |      Code     |           Checksum            |  +- ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |    Maximum Response Code      |           Reserved            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  *                                                               *
 *  |                                                               |
 *  *                       Multicast Address                       *
 *  |                                                               |
 *  *                                                               *
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | Resv  |S| QRV |     QQIC      |     Number of Sources (N)     |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint16        MaxResCodeNbo;
  uint16        Reserved16Nbo;
  IpV6_AddrType MulticastAddr;
  uint8         ReservedSQrv;
  uint8         Qqic;
  uint16        NumberOfSourcesNbo;
} IpV6_Mld_QueryHeaderType;

/*   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |  Type = 143   |    Reserved   |           Checksum            |  +- ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |           Reserved            |Nr of Mcast Address Records (M)|
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  .                                                               .
 */

typedef struct
{
  uint16        Reserved16Nbo;
  uint16        McAddrRecCountNbo;
} IpV6_Mld_ReportHeaderType;

/*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  Record Type  |  Aux Data Len |     Number of Sources (N)     |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  *                                                               *
 *  |                                                               |
 *  *                       Multicast Address                       *
 *  |                                                               |
 *  *                                                               *
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint8         Type;
  uint8         AuxDataLen;
  uint16        SrcAddrCountNbo;
  IpV6_AddrType McAddr;
} IpV6_Mld_ReportRecordHeaderType;

/*   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ \
 *  |     Type      |     Code      |          Checksum             |  +- ICMPv6 header
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ /
 *  |     Maximum Response Delay    |          Reserved             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                       Multicast Address                       +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct
{
  uint16        MaxResDelayNbo;
  uint16        Reserved16Nbo;
  IpV6_AddrType McAddr;
} IpV6_MldV1_MessageType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV6_Mld_VRxQuery()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Mld_VRxQuery(
  uint8                  IpCtrlIdx,
  IPV6_P2C(IpV6_HdrType) IpV6HdrPtr,
  IPV6_P2C(uint8)        IpPayloadPtr,
  uint16                 IcmpPayloadOfs,
  uint16                 IpPayloadLen);

/**********************************************************************************************************************
 *  IpV6_Mld_VTxReport()
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxGeneralQueryResponse(
  uint8                    IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Mld_VTxReportV1()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Mld_VTxReportV1(
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/**********************************************************************************************************************
 *  IpV6_Mld_VTxDoneV1()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Mld_VTxDoneV1(
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte);

/**********************************************************************************************************************
 *  IpV6_Mld_MulticastListen()
 **********************************************************************************************************************/
/*! \brief      Set/update filter mode for a given multicast address.
 *              Multiple sockets can specify a different listen mode for the same multicast address.
 *              These per-socket states are merged internally into a per-interface state.
 *              Reporting of multicast memberships is based on the per-interface state.
 *              See [RFC3810 3. The Service Interface for Requesting IP Multicast Reception] for further details.
 *  \param[in]  CtrlIdx       index of the controller/interface
 *  \param[in]  SckIdx        index of the socket to which the filter should be assigned
 *  \param[in]  McAddrPtr     multicast address
 *  \param[in]  SrcAddrsPtr   array of source addresses that are ALLOWED/BLOCKED (may be NULL_PTR if SrcAddrCount is Zero)
 *  \param[in]  SrcAddrCount  number of source addresses in source address array
 *  \param[in]  ModeIsInclude TRUE:  all multicast packets sent by one of the specified sources shall be received.
 *                                   (If SrcAddrCount is Zero, listening to the given multicast address will be stopped.)
 *                            FALSE: all multicast packets NOT sent by one of the specified sources shall be received.
 *                                   (If SrcAddrCount is Zero, all packets sent to the specified multicast address will be received.)
 *  \return     E_OK          multicast membership record updated. (Reports will be sent if necessary)
 *              E_NOT_OK      MLD disabled or invalid parameter
 *  \note       Initially there is an implicit INCLUDE {} filter for all multicast addresses. (not listening)
 *  \context    task-level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_MulticastListen(
  uint8                   IpCtrlIdx,
  uint8                   SckIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr,
  IPV6_P2C(IpV6_AddrType) SrcAddrsPtr,
  uint8                   SrcAddrCount,
  boolean                 ModeIsInclude);

/**********************************************************************************************************************
 *  IpV6_Mld_Init()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Mld_Init(uint8 IpCtrlIdx);

extern FUNC(void, IPV6_CODE) IpV6_Mld_VClearMcAddrEntry2(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr);

/**********************************************************************************************************************
 *  IpV6_Mld_SetActive()
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Mld_VSetActive(uint8 IpCtrlIdx, boolean Enable);

extern FUNC(void, IPV6_CODE) IpV6_Mld_MainFunction(uint8 IpCtrlIdx);

extern FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VMcAddrListFind(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr);

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (IPV6_ENABLE_MLD == STD_ON) */
#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif  /* IPV6_MLD_H */

/**********************************************************************************************************************
 *  END OF FILE: IpV6_Mld.h
 **********************************************************************************************************************/
