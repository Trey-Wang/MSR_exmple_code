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
 *         \file  TcpIp_Mld.c
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

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_MLD_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

#include "TcpIp_IpV6_Cfg.h"
#if (IPV6_ENABLE_MLD == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_IpV6_Priv.h"
#include "TcpIp_IcmpV6.h"
#include "TcpIp_IpV6_Priv.h"
#include "TcpIp_Mld.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

#define IPV6_MLD_RECORD_TYPE_IS_IN           1U /* (MODE_IS_INCLUDE) */
#define IPV6_MLD_RECORD_TYPE_IS_EX           2U /* (MODE_IS_EXCLUDE) */
#define IPV6_MLD_RECORD_TYPE_TO_IN           3U /* (CHANGE_TO_INCLUDE_MODE) */
#define IPV6_MLD_RECORD_TYPE_TO_EX           4U /* (CHNAGE_TO_EXCLUDE_MODE) */
#define IPV6_MLD_RECORD_TYPE_ALLOW           5U /* (ALLOW_NEW_SOURCES) */
#define IPV6_MLD_RECORD_TYPE_BLOCK           6U /* (BLOCK_OLD_SOURCES) */

/* [RFC3810 5.1.13. Query Variants] */
#define IPV6_MLD_QUERY_TYPE_GENERAL          1U /* "General Query" */
#define IPV6_MLD_QUERY_TYPE_ADDR             2U /* "Multicast Address Specific Query" */
#define IPV6_MLD_QUERY_TYPE_ADDR_SRC         3U /* "Multicast Address and Source Specific Query" */

#define IPV6_MLD_QUERY_TYPE_INVALID          0U /* Used to mark invalid queries */

#define IPV6_MLD_QUERY_MAX_SRC_ADDR_COUNT   89U /* On Ethernet links not more than 89 source addresses fit into a Query [RFC3810 5.1.10. Number of Sources (N)] */
#define IPv6_MLD_LISTEN_MAX_SRC_ADDR_COUNT  89U

#define IPV6_MLD_V1_MSG_LEN                 20U /* Length _without_ 4 byte ICMP header */
#define IPV6_MLD_V2_QUERY_MIN_LEN           24U /* Length _without_ 4 byte ICMP header */
#define IPV6_MLD_V1_MSG_HOP_LIMIT          255U
#define IPV6_MLD_V1_MSG_CODE                 0U

#define IPV6_MLD_ICMP_MSG_CODE               0U

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*
 *                             3x Subset of                                          Subset of                  Subsets of
 *                     IpV6_Mld_IfStateSrcAddrData[]                             IpV6_Mld_McMsData[]      IpV6_Mld_McSrcAddrData[]
 *                                  .                                                    .                          .
 *                                  .                                                    .                          .
 *  IpV6_Mld_IfStateSrcAddrType     .   IpV6_Mld_IfStateType     IpV6_Mld_McAddrType     .    IpV6_Mld_McMsType     .      IpV6_AddrType
 *  +-------------------------+     .   +-------------------+ -- +------------------+    .     +---------------+    .     +-------------+
 *  | SrcAddr                 |<-- n:1 ---{SrcAddrsAllow}   |    |  McAddr          |    .     | {SrcAddrList}---- 1:n -->| addr        |<--+
 *  | Count                   |<-- n:1 ---{SrcAddrsBlock}   |    | {McMsList}---------- 1:n -->|  SocketIdx    |    .     +-------------+   |
 *  +-------------------------+<-- n:1 ---{SrcAddrsKeep}    |<----- IfState         |          |  Include      |    .            ^          |
 *                                      |  ModeIsInclude    |    |  PR              |          +---------------+    .            |          |
 *                                      |  ModeChgTxCounter |    |  PRTimeout       |                               .            |          |
 *                                      |  Timeout          |    | {PRSrcAddrList}-------------------------------- 1:n ----------+          |
 *                                      +-------------------+ -- +------------------+                                                       n
 *                                                                      ^                                                                   :
 *                                                                      |                                                                   1
 *                                                                      n                                                                   |
 *                                                                      :   Subset of IpV6_Mld_McAddrData[]                                 |
 *                                                                      1                                                {MergedSrcList}----+
 *                                                                      |                                                  (temporary)
 *                                                            {IpV6_Mld_McAddrList}
 *                                (contains exactly one entry for each multicast group that we are member of)
 */
#define IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC CONST(IpV6_AddrType, IPV6_CONST) IpV6_Mld_ReportDstAddr = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16
  }
};

#define IPV6_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VCalcIfState(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) McMsListPtr,
  IPV6_P2V(IpV6_DListType) SrcListPtr,
  IPV6_P2V(boolean)        ModeIsIncludePtr);

STATIC FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VMcSrcAddrListFind(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) ListPtr,
  IPV6_P2C(IpV6_AddrType)  AddrPtr);

STATIC FUNC(BufReq_ReturnType, IPV6_CODE) IpV6_Mld_VProvideTxBuffer(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr,
  IPV6_P2V(uint8*)        BufPtrPtr,
  IPV6_P2V(uint16)        BufLenPtr,
  uint8                   Type,
  boolean                 AllowUnspecSrcAddr);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTransmit(
  uint8  IpCtrlIdx,
  uint16 PayloadLen);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VCancelTransmit(
  uint8 IpCtrlIdx);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VDumpFilteredIfStateSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) IfStateSrcAddrListPtr,
  IPV6_P2V(IpV6_DListType) SrcAddrFilterListPtr,
  IPV6_P2V(uint8)          BufPtr,
  IPV6_P2V(uint16)         BufPos,
  uint16                   BufLen,
  IPV6_P2V(uint8)          SrcAddrCount);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VDumpIfStateSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) IfStateSrcAddrListPtr,
  IPV6_P2V(uint8)          BufPtr,
  IPV6_P2V(uint16)         BufPos,
  uint16                   BufLen,
  IPV6_P2V(uint8)          SrcAddrCount);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VFilterSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2V(IpV6_DListType) SrcAddrListPtr,
  IPV6_P2C(IpV6_DListType) IfStateSrcAddrFilterListPtr);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VReset(
  uint8 IpCtrlIdx);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VChangeCompatMode(
  uint8   IpCtrlIdx,
  boolean V1CompatMode);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VUpdateSocketState(
  uint8                   IpCtrlIdx,
  uint8                   SckIdx,
  IpV6_DListIdxType       McAddrIdx,
  IPV6_P2C(IpV6_AddrType) SrcAddrsPtr,
  uint8                   SrcAddrCount,
  boolean                 Include);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VDumpSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) SrcAddrListPtr,
  IPV6_P2V(uint8)          BufPtr,
  IPV6_P2V(uint16)         BufPos,
  uint16                   BufLen,
  IPV6_P2V(uint8)          SrcAddrCount);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VDiffLists(
  uint8                                 IpCtrlIdx,
  IPV6_P2V(IpV6_DListType)              SearchListPtr,
  IPV6_P2V(IpV6_DListType)              TargetListPtr,
  IPV6_P2V(IpV6_DListType)              NotFoundListPtr,
  IPV6_P2V(IpV6_Mld_IfStateSrcAddrType) SearchListDataPtr,
  uint8                                 Robustness);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VUpdateIfState(
  uint8                          IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_IfStateType) IfStatePtr,
  IPV6_P2V(IpV6_DListType)       NewIfStateSrcListPtr,
  boolean                        NewIfStateModeIsInclude);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VScheduleAddrQuery(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrPtr,
  IPV6_P2C(IpV6_AddrType)       SrcAddrListPtr,
  uint8                         SrcAddrCount,
  uint32                        RespDelayMs);

STATIC FUNC(boolean, IPV6_CODE) IpV6_Icmp_VFindMldRouterAlertOption(
  IPV6_P2C(uint8) DataPtr,
  uint8           FirstHeader);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_V1ScheduleReport(
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrEntryPtr,
  uint32                        MaxResDelayMs);

STATIC FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VMcSrcAddrListFind(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) ListPtr,
  IPV6_P2C(IpV6_AddrType)  AddrPtr);

STATIC FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VIfStateSrcAddrListFind(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) ListPtr,
  IPV6_P2C(IpV6_AddrType)  AddrPtr);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VDecCnt(
  uint8                    IpCtrlIdx,
  IPV6_P2V(IpV6_DListType) SrcAddrListPtr,
  IPV6_P2V(IpV6_DListType) TgtSrcAddrListPtr);

STATIC FUNC(BufReq_ReturnType, IPV6_CODE) IpV6_Mld_VAddCurrentStateRecord(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrPtr,
  IPV6_P2V(uint8)               BufPtr,
  IPV6_P2V(uint16)              BufPos,
  uint16                        BufLen);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxMcAddrQueryResponse(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrPtr);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxMsgV1(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr,
  boolean                 Report);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VClearMcAddrEntry(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrEntryPtr);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxFilterModeChgReport(
  uint8                         IpCtrlIdx,
  IPV6_P2C(IpV6_Mld_McAddrType) McAddrPtr);

STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxSrcListChgReport(
  uint8                         IpCtrlIdx,
  IPV6_P2C(IpV6_Mld_McAddrType) McAddrPtr);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VRxQueryV1(
  uint8           IpCtrlIdx,
  IPV6_P2C(uint8) DataPtr);

STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VNotifyAllReportsSent(
  uint8                         IpCtrlIdx,
  IPV6_P2C(IpV6_Mld_McAddrType) McAddrEntryPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV6_Mld_VUpdateSocketState()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VUpdateSocketState(
  uint8                   IpCtrlIdx,
  uint8                   SckIdx,
  IpV6_DListIdxType       McAddrIdx,
  IPV6_P2C(IpV6_AddrType) SrcAddrsPtr,
  uint8                   SrcAddrCount,
  boolean                 Include)
{
  IpV6_DListIdxType McMsIdx, McSrcAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(((0 == SrcAddrCount) || (NULL_PTR != SrcAddrsPtr)), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM, E_NOT_OK);
#endif

  /* serach socket entry */
  McMsIdx = IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList.FirstIdx;

  while (IPV6_DLIST_END_IDX != McMsIdx)
  {
    if (IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SocketIdx == SckIdx)
    {
      /* existing entry for given socket found */
      break;
    }

    IPV6_DLIST_SET_NEXT(McMsIdx, IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList);
  }

  if (IPV6_DLIST_END_IDX == McMsIdx)
  {
    /* currently no membership registered for this socket and this multicast address */

    if ((TRUE == Include) && (0 == SrcAddrCount))
    {
      /* INCLUDE {} means the caller want's to stop listening the this multicast address */
      /* nothing to do in this case */
      return E_OK;
    }

    /* create new entry for given socket */
    if (E_OK != IpV6_DListAddElement(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList, IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList.LastIdx, &McMsIdx))
    {
      /* out of memory */
      return E_NOT_OK;
    }

    /* initialize new socket entry */
    IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SocketIdx = SckIdx;
  }

  /* clear all source address filter entries for this socket */
  IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList);

  if ((TRUE == Include) && (0 == SrcAddrCount))
  {
    /* INCLUDE {} means the caller want's to stop listening the this multicast address. remove membership entry for this socket. */
    IpV6_DListRemoveElement(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList, McMsIdx);

    /* If IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList is empty now, we have no more memberships for this multicast address and
     * should also delete the entry McAddrIdx for the multicast address in IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[] but we have to send
     * reports about leaving the multicast group. So the multicast address entry will be deleted in MainFunction when all
     * reports have been sent.
     */
    return E_OK;
  }

  /* update filter mode */
  IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].ModeIsInclude = Include;

  /* update filter list */
  while (0 < SrcAddrCount)
  {
    SrcAddrCount--;

    if (E_OK != IpV6_DListAddElement(&IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList, IPV6_DLIST_END_IDX, &McSrcAddrIdx))
    {
      /* out of memory */
      return E_NOT_OK;
    }

    IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[McSrcAddrIdx], SrcAddrsPtr[SrcAddrCount]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VUpdateSocketState() */

/**********************************************************************************************************************
 *  IpV6_Mld_VDiffLists()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VDiffLists(
  uint8                                 IpCtrlIdx,
  IPV6_P2V(IpV6_DListType)              SearchListPtr,
  IPV6_P2V(IpV6_DListType)              TargetListPtr,
  IPV6_P2V(IpV6_DListType)              NotFoundListPtr,
  IPV6_P2V(IpV6_Mld_IfStateSrcAddrType) SearchListDataPtr,
  uint8                                 Robustness)
{
  IpV6_ListIdxType SearchListIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SearchListPtr,     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != TargetListPtr,     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NotFoundListPtr,   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SearchListDataPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  SearchListIdx = SearchListPtr->FirstIdx;

  while (IPV6_DLIST_END_IDX != SearchListIdx)
  {
    IpV6_ListIdxType NextSearchListIdx = IPV6_DLIST_GET_NEXT(SearchListIdx, *SearchListPtr);
    IpV6_ListIdxType TargetListIdx     = IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, TargetListPtr, &SearchListDataPtr[SearchListIdx].SrcAddr);

    if (IPV6_DLIST_END_IDX != TargetListIdx)
    {
      /* serach list element is in target list */
      IpV6_DListRemoveElement(TargetListPtr, TargetListIdx);
    }
    else
    {
      IpV6_DListMoveElement(SearchListPtr, SearchListIdx, NotFoundListPtr, NotFoundListPtr->LastIdx);
      /* reset counter so this address will be included in the next reports */
      SearchListDataPtr[SearchListIdx].Count = Robustness;
    }

    SearchListIdx = NextSearchListIdx;
  }
} /* End of IpV6_Mld_VDiffLists() */

/**********************************************************************************************************************
 *  IpV6_Mld_VUpdateIfState()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VUpdateIfState(
  uint8                          IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_IfStateType) IfStatePtr,
  IPV6_P2V(IpV6_DListType)       NewIfStateSrcListPtr,
  boolean                        NewIfStateModeIsInclude)
{
  IpV6_DListIdxType NewIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != IfStatePtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != NewIfStateSrcListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* [RFC3810 6.1. Action on Change of Per-Interface State] */

  if (NewIfStateModeIsInclude != IfStatePtr->ModeIsInclude)
  {
    /* Interface Mode changed (INCLUDE to EXCLUDE or EXCLUDE to INCLUDE)
     * We have to sent a CHANGE_TO_EXCLUDE_MODE or CHNAGE_TO_INCLUDE_MODE report.
     */

    /* clear old interface state lists */
    IpV6_DListClear(&IfStatePtr->SrcAddrsAllow);
    IpV6_DListClear(&IfStatePtr->SrcAddrsBlock);
    IpV6_DListClear(&IfStatePtr->SrcAddrsKeep);

    /* move all elements from input (NewSrcListPtr) to SrcAddrsKeep list */
    while (IPV6_DLIST_END_IDX != NewIfStateSrcListPtr->FirstIdx)
    {
      if (E_OK != IpV6_DListAddElement(&IfStatePtr->SrcAddrsKeep, IfStatePtr->SrcAddrsKeep.LastIdx, &NewIdx))
      {
        /* out of memory */
        return E_NOT_OK;
      }

      IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[NewIdx].SrcAddr, IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[NewIfStateSrcListPtr->FirstIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[NewIdx].Count = 0;
      IpV6_DListRemoveElement(NewIfStateSrcListPtr, NewIfStateSrcListPtr->FirstIdx);
    }

    /* store new Mode and reset tx mode change tx counter, so mode change reports will be sent by MainFunction */
    IfStatePtr->ModeIsInclude    = NewIfStateModeIsInclude;
    IfStatePtr->ModeChgTxCounter = IPV6_MLD_VAR(Robustness);
  }
  else
  {
    /* mode has not changed. (still INCLUDE or EXCLUDE) */

    IPV6_P2V(IpV6_DListType) NewSrcAddrListPtr;
    IPV6_P2V(IpV6_DListType) OldSrcAddrListPtr;

    if (TRUE == NewIfStateModeIsInclude)
    {
      /* Old Mode: INCLUDE (A), New Mode: INCLUDE (B) */
      /* We have to send Reports with Records ALLOW(B-A), BLOCK (A-B) */

      NewSrcAddrListPtr = &IfStatePtr->SrcAddrsAllow; /* will contain sources that were not included before */
      OldSrcAddrListPtr = &IfStatePtr->SrcAddrsBlock; /* will contain sources that are not included anymore */
    }
    else
    {
      /* Old Mode: EXCLUDE (A), New Mode: EXCLUDE (B) */
      /* We have to send Reports with Records ALLOW(A-B), BLOCK (B-A) */

      NewSrcAddrListPtr = &IfStatePtr->SrcAddrsBlock; /* will contain sources that were not blocked before */
      OldSrcAddrListPtr = &IfStatePtr->SrcAddrsAllow; /* will contain sources that are not blocked anymore */
    }

    /* INCLUDE:
     * - Move all elements from ALLOW (NewSrcAddrListPtr) and KEEP lists to BLOCK list (OldSrcAddrListPtr) that are not in the INCLUDE list (NewIfStateSrcListPtr) of the new IfState.
     * - Remove all elements from INCLUDE list (NewIfStateSrcListPtr) that were in ALLOW (NewSrcAddrListPtr) or KEEP list.
     * EXCLUDE:
     * - Move all elements from BLOCK (NewSrcAddrListPtr) and KEEP lists to ALLOW list (OldSrcAddrListPtr) that are not in the EXCLUDE list (NewIfStateSrcListPtr) of the new IfState.
     * - Remove all elements from EXCLUDE list (NewIfStateSrcListPtr) that were in BLOCK (NewSrcAddrListPtr) or KEEP list.
     */
    IpV6_Mld_VDiffLists(IpCtrlIdx, NewSrcAddrListPtr,         NewIfStateSrcListPtr, OldSrcAddrListPtr, &IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[0], IPV6_MLD_VAR(Robustness) - IfStatePtr->ModeChgTxCounter);
    IpV6_Mld_VDiffLists(IpCtrlIdx, &IfStatePtr->SrcAddrsKeep, NewIfStateSrcListPtr, OldSrcAddrListPtr, &IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[0], IPV6_MLD_VAR(Robustness) - IfStatePtr->ModeChgTxCounter);

    /* All remaining elements in NewIfStateSrcList were not in ALLOW/BLOCK or KEEP list before. */

    /* INCLUDE:
     * - Remove all elements from BLOCK list (OldSrcAddrListPtr) that are in INCLUDE list (NewIfStateSrcListPtr)
     * - Add all elements from INCLUDE list (NewIfStateSrcListPtr) to ALLOW list that were not in ALLOW and KEEP list.
     * EXCLUDE:
     * - Remove all elements from ALLOW list (OldSrcAddrListPtr) that are in EXCUDE list (NewIfStateSrcListPtr)
     * - Add all elements from EXCLUDE list (NewIfStateSrcListPtr) to BLOCK list that were not on BLOCK and KEEP list.
     */
    while (IPV6_DLIST_END_IDX != NewIfStateSrcListPtr->FirstIdx)
    {
      /* remove the new source address that has to be included/excluded from block/allow list if neccessary */
      IpV6_DListIdxType Idx = IpV6_Mld_VIfStateSrcAddrListFind(IpCtrlIdx, OldSrcAddrListPtr, &IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[NewIfStateSrcListPtr->FirstIdx]);

      if (IPV6_DLIST_END_IDX != Idx)
      {
        IpV6_DListRemoveElement(OldSrcAddrListPtr, Idx);
      }

      if (E_OK == IpV6_DListAddElement(NewSrcAddrListPtr, NewSrcAddrListPtr->LastIdx, &NewIdx))
      {
        IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[NewIdx].SrcAddr, IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[NewIfStateSrcListPtr->FirstIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[NewIdx].Count = IPV6_MLD_VAR(Robustness);
        IpV6_DListRemoveElement(NewIfStateSrcListPtr, NewIfStateSrcListPtr->FirstIdx);
      }
    }
  }

  IPV6_SET_EVENT_TIME_MS(IfStatePtr->Timeout, 0);

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VUpdateIfState() */

/**********************************************************************************************************************
 *  IpV6_Mld_VMcAddrListFind()
 **********************************************************************************************************************/
FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VMcAddrListFind(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr)
{
  IpV6_DListIdxType Idx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_DLIST_END_IDX);
#endif

  Idx = IPV6_MLD_VAR(McAddrList).FirstIdx;

  while (IPV6_DLIST_END_IDX != Idx)
  {
    if (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[Idx].McAddr, *McAddrPtr))
    {
      break;
    }

    IPV6_DLIST_SET_NEXT(Idx, IPV6_MLD_VAR(McAddrList));
  }

  return Idx;
} /* End of IpV6_Mld_VMcAddrListFind() */

/**********************************************************************************************************************
 *  IpV6_Mld_MulticastListen()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_MulticastListen(
  uint8                   IpCtrlIdx,
  uint8                   SckIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr,
  IPV6_P2C(IpV6_AddrType) SrcAddrsPtr,
  uint8                   SrcAddrCount,
  boolean                 ModeIsInclude)
{
  IpV6_DListIdxType McAddrIdx;

  IpV6_DListType MergedSrcList;
  boolean        MergedFilterMode;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx),                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SOCK_IDX(SckIdx),                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr,                              IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(((0 == SrcAddrCount) || (NULL_PTR != SrcAddrsPtr)), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if ((!IpV6_VCfgHasMldConfig()) || (TRUE != IPV6_MLD_VAR(Enable)))
  {
    return E_NOT_OK;
  }

  if (!IPV6_ADDR_IS_MULTICAST(*McAddrPtr))
  {
    /* given address is not a multicast address */
    return E_NOT_OK;
  }

  if (IPV6_MULTICAST_SCOPE_LINK_LOCAL > IPV6_ADDR_MULTICAST_SCOPE(*McAddrPtr))
  {
    /* scope of given multicast address is invalid */
    return E_NOT_OK;
  }

  /* "The link-scope all-nodes multicast address, (FF02::1), is handled as
   *  a special case.  On all nodes -- that is all hosts and routers,
   *  including multicast routers -- listening to packets destined to the
   *  all-nodes multicast address, from all sources, is permanently enabled
   *  on all interfaces on which multicast listening is supported.  No MLD
   *  messages are ever sent regarding neither the link-scope all-nodes
   *  multicast address, nor any multicast address of scope 0 (reserved) or
   *  1 (node-local)."
   *  [RFC3810 6. Protocol Description for Multicast Address Listeners]
   */
  if (IPV6_ADDR_IS_ALL_NODES_LL(*McAddrPtr))
  {
    /* link-local all-nodes multicast address is implicitly active */
    return E_OK;
  }

  if (IPv6_MLD_LISTEN_MAX_SRC_ADDR_COUNT < SrcAddrCount)
  {
    /* maximum source addresses limit per multicast address exceeded. */
    return E_NOT_OK;
  }

  /* serach for any memberships for given muticast address */
  McAddrIdx = IpV6_Mld_VMcAddrListFind(IpCtrlIdx, McAddrPtr);

  if (IPV6_DLIST_END_IDX == McAddrIdx)
  {
    /* currently no membership registered for given multicast address */

    if ((TRUE == ModeIsInclude) && (0 == SrcAddrCount))
    {
      /* INCLUDE {} means the caller wants to stop listening the this multicast address */
      /* nothing to do in this case */
      return E_OK;
    }

    /* add new membership list for given address */
    if (E_OK != IpV6_DListAddElement(&IPV6_MLD_VAR(McAddrList), IPV6_MLD_VAR(McAddrList).LastIdx, &McAddrIdx))
    {
      /* out of memory */
      return E_NOT_OK;
    }

    /* initialize new entry to INCLUDE {} */
    IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McAddr, *McAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeIsInclude    = TRUE;
    IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeChgTxCounter = 0;
    IPV6_TIME_SET_INFINITY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout);
  }

  /* update or add filter mode and source address list for given socket and multicast address */
  if (E_OK != IpV6_Mld_VUpdateSocketState(IpCtrlIdx, SckIdx, McAddrIdx, SrcAddrsPtr, SrcAddrCount, ModeIsInclude))
  {
    return E_NOT_OK;
  }

  /* initialize temporary source address list */
  IpV6_DListInit(&MergedSrcList, &IPV6_MLD_VAR(McSrcAddrListDesc));

  /* Multicast memberships a stored on a per-socket basis but only reported on a per-interface basis.
   * Merge all per-socket multicast memberships into a per-interface filter mode and source address list
   */
  IpV6_Mld_VCalcIfState(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList, &MergedSrcList, &MergedFilterMode);

  if (IPv6_MLD_LISTEN_MAX_SRC_ADDR_COUNT < MergedSrcList.ElementCount)
  {
    /* action causes too much entries in the interface state list.
     * remove multicast membership for multicast address on given socket and return error to the application.
     */
    IpV6_DListIdxType McMsIdx;

    /* discard merged list */
    IpV6_DListClear(&MergedSrcList);

    /* serach socket entry */
    McMsIdx = IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList.FirstIdx;

    while (IPV6_DLIST_END_IDX != McMsIdx)
    {
      if (IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SocketIdx == SckIdx)
      {
        /* existing entry for given socket found */
        break;
      }

      IPV6_DLIST_SET_NEXT(McMsIdx, IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList);
    }

    if (IPV6_DLIST_END_IDX != McMsIdx)
    {
      /* clear all source address filter entries for this mebership */
      IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList);

      /* remove membership entry for this socket */
      IpV6_DListRemoveElement(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList, McMsIdx);
    }

    return E_NOT_OK;
  }

  /* merge new temporary interface state list with current interface state an schedule report transmissions */
  return IpV6_Mld_VUpdateIfState(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState, &MergedSrcList, MergedFilterMode);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_MulticastListen() */

/**********************************************************************************************************************
 *  IpV6_Mld_VScheduleAddrQuery()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VScheduleAddrQuery(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrPtr,
  IPV6_P2C(IpV6_AddrType)       SrcAddrListPtr,
  uint8                         SrcAddrCount,
  uint32                        RespDelayMs)
{
  IpV6_DListIdxType SrcAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != McAddrPtr,                                 IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(((0 == SrcAddrCount) || (NULL_PTR != SrcAddrListPtr)), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (FALSE == McAddrPtr->PR)
  {
    /* currently not report scheduled */

    /*
     * 3. "If the received Query is a Multicast Address Specific Query or a
     *     Multicast Address and Source Specific Query and there is no
     *     pending response to a previous Query for this multicast address,
     *     then the Multicast Address Timer is used to schedule a report.  If
     *     the received Query is a Multicast Address and Source Specific
     *     Query, the list of queried sources is recorded to be used when
     *     generating a response."
     */

    IpV6_DListClear(&McAddrPtr->PRSrcAddrList);

    while (0 < SrcAddrCount)
    {
      SrcAddrCount--;

      if (E_OK != IpV6_DListAddElement(&McAddrPtr->PRSrcAddrList, McAddrPtr->PRSrcAddrList.LastIdx, &SrcAddrIdx))
      {
        /* out of memory */
        return;
      }

      IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[SrcAddrIdx], SrcAddrListPtr[SrcAddrCount]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    }

    McAddrPtr->PR = TRUE;
    IPV6_SET_EVENT_TIME_MS(McAddrPtr->PRTimeout, RespDelayMs);
  }
  else
  {
    /* report is scheduled */
    IpV6_TimeType NewReportTimeout;

    IPV6_SET_EVENT_TIME_MS(NewReportTimeout, RespDelayMs);

    if (IPV6_DLIST_IS_EMPTY(McAddrPtr->PRSrcAddrList) || (SrcAddrCount == 0))
    {
      /*
       * 4. "If there is already a pending response to a previous Query
       *     scheduled for this multicast address, and either the new Query is
       *     a Multicast Address Specific Query or the recorded source list
       *     associated with the multicast address is empty, then the multicast
       *     address source list is cleared and a single response is scheduled,
       *     using the Multicast Address Timer.  The new response is scheduled
       *     to be sent at the earliest of the remaining time for the pending
       *     report and the selected delay."
       */
      IpV6_DListClear(&McAddrPtr->PRSrcAddrList);
    }
    else
    {
      /*
       * 5. "If the received Query is a Multicast Address and Source Specific
       *     Query and there is a pending response for this multicast address
       *     with a non-empty source list, then the multicast address source
       *     list is augmented to contain the list of sources in the new Query,
       *     and a single response is scheduled using the Multicast Address
       *     Timer.  The new response is scheduled to be sent at the earliest
       *     of the remaining time for the pending report and the selected
       *     delay."
       */

      while (0 < SrcAddrCount)
      {
        SrcAddrCount--;

        SrcAddrIdx = IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, &McAddrPtr->PRSrcAddrList, &SrcAddrListPtr[SrcAddrCount]);

        if (IPV6_DLIST_END_IDX == SrcAddrIdx)
        {
          (void)IpV6_DListAddElement(&McAddrPtr->PRSrcAddrList, McAddrPtr->PRSrcAddrList.LastIdx, &SrcAddrIdx);
        }

        if (IPV6_DLIST_END_IDX != SrcAddrIdx)
        {
          IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[SrcAddrIdx], SrcAddrListPtr[SrcAddrCount]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        }
      }
    }
    IPV6_SET_EVENT_TIME_MS(NewReportTimeout, RespDelayMs);

    if (IPV6_TIME_IS_LOWER(NewReportTimeout, McAddrPtr->PRTimeout))
    {
      McAddrPtr->PRTimeout = NewReportTimeout;
    }

  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VScheduleAddrQuery() */

/**********************************************************************************************************************
 *  IpV6_Icmp_VFindMldRouterAlertOption()
 **********************************************************************************************************************/
STATIC FUNC(boolean, IPV6_CODE) IpV6_Icmp_VFindMldRouterAlertOption(
  IPV6_P2C(uint8) DataPtr,
  uint8           FirstHeader)
{
  uint16 OptOfs;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint8  OptType;
  uint8  OptDataLen;
  boolean MldRtrAlertOptFound = FALSE;
  uint16 HppByHopHdrLen;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, FALSE);
#endif

  if (IPV6_EXT_HDR_ID_HOP_BY_HOP != FirstHeader)
  {
    /* we expect that the IPv6 Hop-By-Hop Extension Header follows directly the IPv6 main header */
    return FALSE;
  }

  HppByHopHdrLen = IPV6_CALC_EXT_HEADER_LENGTH(DataPtr[1]);

  /* process TLV encoded options */

  OptOfs = 2; /* first option starts after NextHeader and HdrExtLen bytes */

  /* option loop */
  while (OptOfs < HppByHopHdrLen)
  {
    OptType = DataPtr[OptOfs]; /* first byte of TLV option indicates Type */

    if (OptType == IPV6_EXT_HDR_OPT_TYPE_PAD_1) /* Pad1 option */
    {
      /* Pad1 option consists only of the OptType (1 byte padding) */
      OptOfs++; /* next option starts directly after this one-byte option */
      continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
    }

    OptDataLen = DataPtr[OptOfs+1]; /* second byte of TLV option indicates DataLength in bytes */

    if (IPV6_HBH_OPT_TYPE_RTR_ALERT == OptType)
    {
      /* Router Alert Option found */
      if ((IPV6_HBH_OPT_RTR_ALERT_LEN == OptDataLen) && (0 == DataPtr[OptOfs + IPV6_TLV_OPT_DATA_OFFS]) && (0 == DataPtr[OptOfs + IPV6_TLV_OPT_DATA_OFFS + 1]))
      {
        MldRtrAlertOptFound = TRUE;
        break;
      }
    }

    OptOfs += IPV6_TLV_OPT_DATA_OFFS + OptDataLen; /* skip to next option */
  }

  return MldRtrAlertOptFound;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Icmp_VFindMldRouterAlertOption() */

/**********************************************************************************************************************
 *  IpV6_Mld_VChangeCompatMode()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VChangeCompatMode(
  uint8   IpCtrlIdx,
  boolean V1CompatMode)
{
  IpV6_DListIdxType McAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(V1CompatMode != IPV6_MLD_VAR(V1CompatMode), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  IPV6_MLD_VAR(ReportPendingCount) = 0;

  McAddrIdx = IPV6_MLD_VAR(McAddrList).FirstIdx;

  while (IPV6_DLIST_END_IDX != McAddrIdx)
  {
    IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PR = FALSE;

    IPV6_DLIST_SET_NEXT(McAddrIdx, IPV6_MLD_VAR(McAddrList));
  }

  IPV6_MLD_VAR(V1CompatMode) = V1CompatMode;
} /* End of IpV6_Mld_VChangeCompatMode() */

/**********************************************************************************************************************
 *  IpV6_Mld_V1ScheduleReport()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_V1ScheduleReport(
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrEntryPtr,
  uint32                        MaxResDelayMs)
{
  uint32 ResDelayMs;
  IpV6_TimeType ResTimeout;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != McAddrEntryPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  ResDelayMs = IpV6_VGetRandomValue(0, MaxResDelayMs);

  IPV6_SET_EVENT_TIME_MS(ResTimeout, ResDelayMs);

  if (FALSE == McAddrEntryPtr->PR)
  {
    McAddrEntryPtr->PR = TRUE;
    McAddrEntryPtr->PRTimeout = ResTimeout;
  }
  else if (IPV6_TIME_IS_LOWER(ResTimeout, McAddrEntryPtr->PRTimeout))
  {
    McAddrEntryPtr->PRTimeout = ResTimeout;
  }
  else
  {
    /* report already scheduled */
  }
} /* End of IpV6_Mld_V1ScheduleReport() */

/**********************************************************************************************************************
 *  IpV6_Mld_VRxQueryV1()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VRxQueryV1(
  uint8           IpCtrlIdx,
  IPV6_P2C(uint8) DataPtr)
{
  uint32                               MaxResDelayMs;
  IpV6_DListIdxType                    McAddrIdx;
  IPV6_P2C(IpV6_Mld_V1QueryHeaderType) QueryHdrPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  IPV6_SET_EVENT_TIME_MS(IPV6_MLD_VAR(V1QuerierPresentTimeout), IpV6_VCfgGetMldV1QuerierPresentTimeoutMs());

  QueryHdrPtr = (IPV6_P2C(IpV6_Mld_V1QueryHeaderType))DataPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  MaxResDelayMs = IPV6_NTOHS(QueryHdrPtr->MaxResDelayNbo);

  if (IPV6_ADDR_IS_UNSPECIFIED(QueryHdrPtr->MulticastAddr))
  {
    /* "General Query" */
    /* schedule report for all multicast address memberships */

    McAddrIdx = IPV6_MLD_VAR(McAddrList).FirstIdx;

    while (IPV6_DLIST_END_IDX != McAddrIdx)
    {
      IpV6_Mld_V1ScheduleReport(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx], MaxResDelayMs);

      IPV6_DLIST_SET_NEXT(McAddrIdx, IPV6_MLD_VAR(McAddrList));
    }
  }
  else
  {
    /* "Multicast Address Specific Query" */

    McAddrIdx = IpV6_Mld_VMcAddrListFind(IpCtrlIdx, &QueryHdrPtr->MulticastAddr);

    if (IPV6_DLIST_END_IDX != McAddrIdx)
    {
      /* schedule report for multicast address */
      IpV6_Mld_V1ScheduleReport(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx], MaxResDelayMs);
    }
  }
} /* End of IpV6_Mld_VRxQueryV1() */

/**********************************************************************************************************************
 *  IpV6_Mld_VRxQuery()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Mld_VRxQuery(
  uint8                  IpCtrlIdx,
  IPV6_P2C(IpV6_HdrType) IpV6HdrPtr,
  IPV6_P2C(uint8)        IpPayloadPtr,
  uint16                 IcmpPayloadOfs,
  uint16                 IpPayloadLen)
{
  IPV6_P2C(IpV6_Mld_QueryHeaderType) QueryHdrPtr;
  uint32        MaxRespDelayMs;
  uint16        SrcAddrCount;
  IpV6_TimeType NewReportTimeout;
  uint32        ResDelayMs;
  uint16        IcmpPayloadLen;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != IpV6HdrPtr,   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != IpPayloadPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(0 < IpPayloadLen,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (FALSE == IpV6_Icmp_VFindMldRouterAlertOption(IpPayloadPtr, IpV6HdrPtr->NextHdr))
  {
    /* Packet MUST contain a Hop-by-Hop Extension Header with an MLD Router Alert option */
    return;
  }

  IcmpPayloadLen = IpPayloadLen - IcmpPayloadOfs;

  if (IPV6_MLD_V2_QUERY_MIN_LEN > IcmpPayloadLen)
  {
    if (IPV6_MLD_V1_MSG_LEN == IcmpPayloadLen)
    {
      if (FALSE == IPV6_MLD_VAR(V1CompatMode))
      {
        IpV6_Mld_VChangeCompatMode(IpCtrlIdx, TRUE); /*lint !e522 */ /* This function has side-effects! (modifies global variables) */
      }

      /* MLDv1 Query */
      IpV6_Mld_VRxQueryV1(IpCtrlIdx, &IpPayloadPtr[IcmpPayloadOfs]);
    }
    else
    {
      /* invalid message length */
    }

    /* do not process */
    return;
  }

  if (TRUE == IPV6_MLD_VAR(V1CompatMode))
  {
    IpV6_Mld_VRxQueryV1(IpCtrlIdx, &IpPayloadPtr[IcmpPayloadOfs]);
    return;
  }

  /* process MLDv2 Query */

  /* No check of DstAddr necessary.
   * [5.1.15. Destination Addresses for Queries]
   */

  QueryHdrPtr = (IPV6_P2C(IpV6_Mld_QueryHeaderType))&IpPayloadPtr[IcmpPayloadOfs]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  MaxRespDelayMs = IPV6_NTOHS(QueryHdrPtr->MaxResCodeNbo);

  if (0 != (MaxRespDelayMs & 0x8000))
  {
    /* If Maximum Response Code >=32768, Maximum Response Code represents a
     * floating-point value as follows:
     *
     *    0 1 2 3 4 5 6 7 8 9 A B C D E F
     *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *   |1| exp |          mant         |
     *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *
     *  Maximum Response Delay = (mant | 0x1000) << (exp+3)
     *  [RFC3810 5.1.3. Maximum Response Code]
     */
    MaxRespDelayMs = ((MaxRespDelayMs & 0x00000FFFU) | 0x00001000U) << (((MaxRespDelayMs >> 12) & 0x00000007U) + 3);
  }

  /* S, QRV and QQIC fields are only relevant for routers and will be ignored here */

  SrcAddrCount = IPV6_NTOHS(QueryHdrPtr->NumberOfSourcesNbo);

  if (SrcAddrCount > IPV6_MLD_QUERY_MAX_SRC_ADDR_COUNT)
  {
    /* more than IPV6_MLD_QUERY_MAX_ADDR_COUNT would exceed then MTU of Ethernet */
    return;
  }

  if ((sizeof(IpV6_Mld_QueryHeaderType) + (SrcAddrCount * sizeof(IpV6_AddrType))) > IcmpPayloadLen)
  {
    /* packet too short */
    return;
  }

  ResDelayMs = IpV6_VGetRandomValue(0, MaxRespDelayMs);
  IPV6_SET_EVENT_TIME_MS(NewReportTimeout, ResDelayMs);

  if ((0 < IPV6_MLD_VAR(ReportPendingCount)) && IPV6_TIME_IS_LOWER(IPV6_MLD_VAR(ReportTimeout), NewReportTimeout))
  {
    /* 1. "If there is a pending response to a previous General Query
      *     scheduled sooner than the selected delay, no additional response
      *     needs to be scheduled."
      */
    return;
  }

  if (IPV6_ADDR_IS_UNSPECIFIED(QueryHdrPtr->MulticastAddr))
  {
    /* "General Query" */
    if (0 == SrcAddrCount)
    {
      /* Schedule Report for General Query */

      /* 2. "If the received Query is a General Query, the Interface Timer is
       *     used to schedule a response to the General Query after the
       *     selected delay.  Any previously pending response to a General
       *     Query is canceled."
       */
      IPV6_MLD_VAR(ReportTimeout)      = NewReportTimeout;
      IPV6_MLD_VAR(ReportPendingCount) = 1;
    }
    else
    {
      /* General Querys must not contain any multicast source addresses */
    }
  }
  else
  {
    /* "Multicast Address Specific Query" or "Multicast Address and Source Specific Query" */

    IpV6_DListIdxType McAddrIdx = IpV6_Mld_VMcAddrListFind(IpCtrlIdx, &QueryHdrPtr->MulticastAddr);

    if (IPV6_DLIST_END_IDX != McAddrIdx)
    {
      /* We have a membership for this multicast address */

      IpV6_Mld_VScheduleAddrQuery(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx], (IPV6_P2C(IpV6_AddrType))&IpPayloadPtr[IcmpPayloadOfs + sizeof(IpV6_Mld_QueryHeaderType)], (uint8)SrcAddrCount, ResDelayMs); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    }
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VRxQuery() */

/**********************************************************************************************************************
 *  IpV6_Mld_VMcSrcAddrListFind()
 **********************************************************************************************************************/
STATIC FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VMcSrcAddrListFind(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) ListPtr,
  IPV6_P2C(IpV6_AddrType)  AddrPtr)
{
  IpV6_DListIdxType Idx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != ListPtr,                                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_DLIST_END_IDX);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr,                                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_DLIST_END_IDX);
  IpV6_CheckDetErrorReturnValue(ListPtr->DescPtr == &IPV6_MLD_VAR(McSrcAddrListDesc), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   IPV6_DLIST_END_IDX);
#endif

  Idx = ListPtr->FirstIdx;

  while (IPV6_DLIST_END_IDX != Idx)
  {
    if (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[Idx], *AddrPtr))
    {
      break;
    }

    IPV6_DLIST_SET_NEXT(Idx, *ListPtr);
  }

  return Idx;
} /* End of IpV6_Mld_VMcSrcAddrListFind() */

/**********************************************************************************************************************
 *  IpV6_Mld_VIfStateSrcAddrListFind()
 **********************************************************************************************************************/
STATIC FUNC(IpV6_DListIdxType, IPV6_CODE) IpV6_Mld_VIfStateSrcAddrListFind(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) ListPtr,
  IPV6_P2C(IpV6_AddrType)  AddrPtr)
{
  IpV6_DListIdxType Idx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != ListPtr,                                       IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_DLIST_END_IDX);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr,                                       IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_DLIST_END_IDX);
  IpV6_CheckDetErrorReturnValue(ListPtr->DescPtr == &IPV6_MLD_VAR(IfStateSrcAddrListDesc), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   IPV6_DLIST_END_IDX);
#endif

  Idx = ListPtr->FirstIdx;

  while (IPV6_DLIST_END_IDX != Idx)
  {
    if (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[Idx].SrcAddr, *AddrPtr))
    {
      break;
    }

    IPV6_DLIST_SET_NEXT(Idx, *ListPtr);
  }

  return Idx;
} /* End of IpV6_Mld_VIfStateSrcAddrListFind() */

/**********************************************************************************************************************
 *  IpV6_Mld_VDecCnt()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VDecCnt(
  uint8                    IpCtrlIdx,
  IPV6_P2V(IpV6_DListType) SrcAddrListPtr,
  IPV6_P2V(IpV6_DListType) TgtSrcAddrListPtr)
{
  IpV6_DListIdxType SrcAddrListIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrListPtr,                                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  /* TgtSrcAddrListPtr may be NULL_PTR */
  IpV6_CheckDetErrorReturnVoid(&IPV6_MLD_VAR(IfStateSrcAddrListDesc) == SrcAddrListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  SrcAddrListIdx = SrcAddrListPtr->FirstIdx;

  while (IPV6_DLIST_END_IDX != SrcAddrListIdx)
  {
    IpV6_DListIdxType NextSrcAddrListIdx = IPV6_DLIST_GET_NEXT(SrcAddrListIdx, *SrcAddrListPtr);

    if (1 < IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[SrcAddrListIdx].Count)
    {
      IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[SrcAddrListIdx].Count--;
    }
    else if (TgtSrcAddrListPtr != NULL_PTR)
    {
      IpV6_DListMoveElement(SrcAddrListPtr, SrcAddrListIdx, TgtSrcAddrListPtr, TgtSrcAddrListPtr->LastIdx);
      IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[SrcAddrListIdx].Count = 0;
    }
    else
    {
      IpV6_DListRemoveElement(SrcAddrListPtr, SrcAddrListIdx);
    }

    SrcAddrListIdx = NextSrcAddrListIdx;
  }
} /* End of IpV6_Mld_VDecCnt() */

/**********************************************************************************************************************
 *  IpV6_Mld_VCalcIfState()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VCalcIfState(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) McMsListPtr,
  IPV6_P2V(IpV6_DListType) SrcListPtr,
  IPV6_P2V(boolean)        ModeIsIncludePtr)
{
  IpV6_DListIdxType McMsIdx, McSrcAddrIdx;
  uint8 ExcludeEntryCount = 0;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != McMsListPtr,                             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcListPtr,                              IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(&IPV6_MLD_VAR(McMsListDesc) == McMsListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  /* build the intersection of all EXCLUDE lists */
  McMsIdx = McMsListPtr->FirstIdx;

  while (IPV6_DLIST_END_IDX != McMsIdx)
  {
    if (FALSE == IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].ModeIsInclude)
    {
      /* EXCLUDE source list */

      ExcludeEntryCount++;

      if (1 == ExcludeEntryCount)
      {
        /* This is the first EXCLUDE list. Copy all elements from this list to result list (SrcListPtr) */

        McSrcAddrIdx = IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList.FirstIdx;

        while (IPV6_DLIST_END_IDX != McSrcAddrIdx)
        {
          IpV6_DListIdxType NewIdx;

          if (E_OK != IpV6_DListAddElement(SrcListPtr, SrcListPtr->LastIdx, &NewIdx))
          {
            /* out of memory */
            return;
          }

          IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[NewIdx], IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[McSrcAddrIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */

          IPV6_DLIST_SET_NEXT(McSrcAddrIdx, IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList);
        }
      }
      else if (!IPV6_DLIST_IS_EMPTY(*SrcListPtr))
      {
        /* result list already contains elements */

        /* Build intersection of current EXCLUDE list and result list */
        /* Remove all elements from result list that are not in current EXCLUDE list */

        if (IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList))
        {
          /* current EXCLUDE list is empty. Intersection is {} */
          IpV6_DListClear(SrcListPtr);
        }
        else
        {
          /* remove all elements from output list that are not also excluded in this list */
          IpV6_DListIdxType SrcListIdx = SrcListPtr->FirstIdx;

          while (IPV6_DLIST_END_IDX != SrcListIdx)
          {
            IpV6_DListIdxType NextSrcListIdx = IPV6_DLIST_GET_NEXT(SrcListIdx, *SrcListPtr);

            if (IPV6_DLIST_END_IDX == IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList, &IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[SrcListIdx]))
            {
              /* excluded address is not excluded in this exclude set */
              IpV6_DListRemoveElement(SrcListPtr, SrcListIdx);
            }

            SrcListIdx = NextSrcListIdx;
          }
        }
      }
      else
      {
        /* intersection already {}. nothing to do. */
      }
    }

    IPV6_DLIST_SET_NEXT(McMsIdx, *McMsListPtr);
  }

  if (0 < ExcludeEntryCount)
  {
    /* "if *any* such record has a filter mode of EXCLUDE, then the filter
     *  mode of the interface record is EXCLUDE, and the source list of
     *  the interface record is the intersection of the source lists of
     *  all socket records in EXCLUDE mode, minus those source addresses
     *  that appear in any socket record in INCLUDE mode."
     *  [RFC3810 4.2. Per-Interface State]
     */
    (*ModeIsIncludePtr) = FALSE;

    /* ListPtr not contains intersection of all EXCLUDE entrys.
     * We now have to subtract the union of all INCLUDE entries from this set
     */

     /* iterate over all INCLUDE filters */
    McMsIdx = McMsListPtr->FirstIdx;

    while (IPV6_DLIST_END_IDX != McMsIdx)
    {
      if (TRUE == IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].ModeIsInclude)
      {
        /* INCLUDE list */

        McSrcAddrIdx = IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList.FirstIdx;

        while (IPV6_DLIST_END_IDX != McSrcAddrIdx)
        {
          IpV6_DListIdxType Idx =  IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, SrcListPtr, &IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[McSrcAddrIdx]);

          if (IPV6_DLIST_END_IDX != Idx)
          {
            IpV6_DListRemoveElement(SrcListPtr, Idx);
          }

          IPV6_DLIST_SET_NEXT(McSrcAddrIdx, IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList);
        }
      }

      IPV6_DLIST_SET_NEXT(McMsIdx, *McMsListPtr);
    }
  }
  else
  {
    /* "if *all* such records have a filter mode of INCLUDE, then the
     *  filter mode of the interface record is INCLUDE, and the source
     *  list of the interface record is the union of the source lists of
     *  all the socket records."
     *  [RFC3810 4.2. Per-Interface State]
     */
    (*ModeIsIncludePtr) = TRUE;

    /* build include set */

    /* iterate over all INCLUDE filters */
    McMsIdx = McMsListPtr->FirstIdx;

    while (IPV6_DLIST_END_IDX != McMsIdx)
    {
      if (TRUE == IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].ModeIsInclude)
      {
        /* INCLUDE list */

        McSrcAddrIdx = IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList.FirstIdx;

        while (IPV6_DLIST_END_IDX != McSrcAddrIdx)
        {
          if (IPV6_DLIST_END_IDX == IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, SrcListPtr, &IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[McSrcAddrIdx]))
          {
            /* element not in output list */

            IpV6_DListIdxType NewIdx;

            if (E_OK != IpV6_DListAddElement(SrcListPtr, SrcListPtr->LastIdx, &NewIdx))
            {
              /* out of memory */
              return;
            }

            IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[NewIdx], IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[McSrcAddrIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
          }

          IPV6_DLIST_SET_NEXT(McSrcAddrIdx, IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList);
        }
      }

      IPV6_DLIST_SET_NEXT(McMsIdx, *McMsListPtr);
    }
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VCalcIfState() */

/**********************************************************************************************************************
 *  IpV6_Mld_VAddCurrentStateRecord()
 **********************************************************************************************************************/
STATIC FUNC(BufReq_ReturnType, IPV6_CODE) IpV6_Mld_VAddCurrentStateRecord(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrPtr,
  IPV6_P2V(uint8)               BufPtr,
  IPV6_P2V(uint16)              BufPos,
  uint16                        BufLen)
{
  IPV6_P2V(IpV6_Mld_ReportRecordHeaderType) RecordHdrPtr;
  uint8 SrcAddrCount;
  boolean TxBufferOverflow = FALSE;
  uint16 BufPosTmp = *BufPos;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != BufPtr,    IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((*BufPos) < BufLen,    IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   BUFREQ_E_NOT_OK);
#endif

  if ((BufPosTmp + sizeof(IpV6_Mld_ReportRecordHeaderType)) > BufLen)
  {
    /* record does not fit into buffer */
    return BUFREQ_E_OVFL;
  }

  RecordHdrPtr = (IPV6_P2V(IpV6_Mld_ReportRecordHeaderType))&BufPtr[BufPosTmp]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IPV6_ADDR_COPY(RecordHdrPtr->McAddr, McAddrPtr->McAddr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  RecordHdrPtr->AuxDataLen = 0;

  BufPosTmp += sizeof(IpV6_Mld_ReportRecordHeaderType);

  SrcAddrCount = 0;

  if (IPV6_DLIST_IS_EMPTY(McAddrPtr->PRSrcAddrList))
  {
    /* Answer Multicast Address Specific Query */
    /* [RFC3810 6.3. Action on Timer Expiration (2.)] */

    if (TRUE == McAddrPtr->IfState.ModeIsInclude)
    {
      /* MODE_IS_INCLUDE */
      RecordHdrPtr->Type = IPV6_MLD_RECORD_TYPE_IS_IN;
      if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsAllow, BufPtr, &BufPosTmp, BufLen, &SrcAddrCount))
      {
        TxBufferOverflow = TRUE;
      }
    }
    else
    {
      /* MODE_IS_EXCLUDE */
      RecordHdrPtr->Type = IPV6_MLD_RECORD_TYPE_IS_EX;
      if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsBlock, BufPtr, &BufPosTmp, BufLen, &SrcAddrCount))
      {
        TxBufferOverflow = TRUE;
      }
    }

    if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsKeep,  BufPtr, &BufPosTmp, BufLen, &SrcAddrCount))
    {
      TxBufferOverflow = TRUE;
    }
  }
  else
  {
    /* Answer Multicast Address and Source Specific Query */

    /* Even if the mode is EXCLUDE we have to send a MODE_IS_INCLUDE Record containing all the requested source addresses that we do not exclude. */
    RecordHdrPtr->Type = IPV6_MLD_RECORD_TYPE_IS_IN;

    if (TRUE == McAddrPtr->IfState.ModeIsInclude)
    {
      /* INCLUDE(A) / Send IS_IN(A * McAddrPtr->PRSrcAddrList)
       * [RFC3810 6.3. Action on Timer Expiration (3.)]
       */

      if (E_OK != IpV6_Mld_VDumpFilteredIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsAllow, &McAddrPtr->PRSrcAddrList, BufPtr, &BufPosTmp, BufLen, &SrcAddrCount))
      {
        TxBufferOverflow = TRUE;
      }

      if (E_OK != IpV6_Mld_VDumpFilteredIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsKeep,  &McAddrPtr->PRSrcAddrList, BufPtr, &BufPosTmp, BufLen, &SrcAddrCount))
      {
        TxBufferOverflow = TRUE;
      }
    }
    else
    {
      /* EXCLUDE(A) / Send IS_IN(McAddrPtr->PRSrcAddrList - A)
       * [RFC3810 6.3. Action on Timer Expiration (3.)]
       */

      /* remove all source addresses from query list, that we do EXCLUDE */
      IpV6_Mld_VFilterSrcAddrList(IpCtrlIdx, &McAddrPtr->PRSrcAddrList, &McAddrPtr->IfState.SrcAddrsBlock);
      IpV6_Mld_VFilterSrcAddrList(IpCtrlIdx, &McAddrPtr->PRSrcAddrList, &McAddrPtr->IfState.SrcAddrsKeep);

      /* send list with all requestes source addresses that we do not exclude */
      if (E_OK != IpV6_Mld_VDumpSrcAddrList(IpCtrlIdx, &McAddrPtr->PRSrcAddrList, BufPtr, &BufPosTmp, BufLen, &SrcAddrCount))
      {
        TxBufferOverflow = TRUE;
      }
    }

    IpV6_DListClear(&McAddrPtr->PRSrcAddrList);
  }

  if (TRUE == TxBufferOverflow)
  {
    /* insufficient buffer size */
    return BUFREQ_E_OVFL;
  }

  if ((IPV6_MLD_RECORD_TYPE_IS_IN == RecordHdrPtr->Type) && (0 == SrcAddrCount))
  {
    return BUFREQ_E_NOT_OK;
  }

  RecordHdrPtr->SrcAddrCountNbo = IPV6_UINT8_HTONS(SrcAddrCount);
  (*BufPos) = BufPosTmp;

  return BUFREQ_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VAddCurrentStateRecord() */

/**********************************************************************************************************************
 *  IpV6_Mld_VTxMcAddrQueryResponse()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxMcAddrQueryResponse(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrPtr)
{
  IPV6_P2V(uint8)                     MldBufPtr;
  uint16                              MldBufLen;
  IPV6_P2V(IpV6_Mld_ReportHeaderType) ReportHdrPtr;
  uint16                              MldBufPos;
  BufReq_ReturnType                   AddRecordResult;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  MldBufLen = IpV6_VCfgGetMldMaxReportMsgSize();

  if (BUFREQ_OK != IpV6_Mld_VProvideTxBuffer(IpCtrlIdx, &IpV6_Mld_ReportDstAddr, &MldBufPtr, &MldBufLen, IPV6_ICMP_MSG_TYPE_MLD_REPORT, TRUE))
  {
    return E_NOT_OK;
  }

  ReportHdrPtr = (IPV6_P2V(IpV6_Mld_ReportHeaderType))&MldBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  ReportHdrPtr->Reserved16Nbo = 0;

  MldBufPos = sizeof(IpV6_Mld_ReportHeaderType);

  AddRecordResult = IpV6_Mld_VAddCurrentStateRecord(IpCtrlIdx, McAddrPtr, MldBufPtr, &MldBufPos, MldBufLen);

  if (BUFREQ_OK != AddRecordResult)
  {
    /* buffer overflow or record empty */
    IpV6_Mld_VCancelTransmit(IpCtrlIdx);

    if (BUFREQ_E_OVFL == AddRecordResult)
    {
      return E_NOT_OK;
    }

    return E_OK;
  }

  ReportHdrPtr->McAddrRecCountNbo = IPV6_UINT8_HTONS(1);

  return IpV6_Mld_VTransmit(IpCtrlIdx, MldBufPos);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VTxMcAddrQueryResponse() */

/**********************************************************************************************************************
 *  IpV6_Mld_VTxGeneralQueryResponse()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxGeneralQueryResponse(
  uint8                    IpCtrlIdx)
{
  IPV6_P2V(uint8)                           MldBufPtr;
  uint16                                    MldBufLen;
  IPV6_P2V(IpV6_Mld_ReportHeaderType)       ReportHdrPtr;
  uint16                                    MldBufPos;
  IpV6_DListIdxType                         McAddrIdx;
  uint16                                    RecordCount;
  BufReq_ReturnType                         AddRecordResult;

  MldBufLen = IpV6_VCfgGetMldMaxReportMsgSize();

  /* get index of first multicast address */
  McAddrIdx = IPV6_MLD_VAR(McAddrList).FirstIdx;

  while (IPV6_DLIST_END_IDX != McAddrIdx)
  {
    /* get tx buffer and write MLDv2 report header */

    if (BUFREQ_OK != IpV6_Mld_VProvideTxBuffer(IpCtrlIdx, &IpV6_Mld_ReportDstAddr, &MldBufPtr, &MldBufLen, IPV6_ICMP_MSG_TYPE_MLD_REPORT, TRUE))
    {
      return E_NOT_OK;
    }

    ReportHdrPtr = (IPV6_P2V(IpV6_Mld_ReportHeaderType))&MldBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
    ReportHdrPtr->Reserved16Nbo = 0;

    MldBufPos = sizeof(IpV6_Mld_ReportHeaderType);

    /* add a record for each multicast address. */
    /* if not all records fit into one message, the remaining record will be sent in a new message */
    /* [RFC3810 5.2.15. Multicast Listener Report Size] */

    RecordCount = 0;

    while (IPV6_DLIST_END_IDX != McAddrIdx)
    {
      AddRecordResult = IpV6_Mld_VAddCurrentStateRecord(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx], MldBufPtr, &MldBufPos, MldBufLen);

      if (BUFREQ_OK == AddRecordResult)
      {
        RecordCount++;
      }
      else if (BUFREQ_E_OVFL == AddRecordResult)
      {
        if (0 == RecordCount)
        {
          /* record does not fit into any message. skip this record */
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
          IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif
        }
        else
        {
          /* record does not fit into remaining buffer. */
          break;
        }
      }
      else
      {
        /* record empty. skip this record. */
      }

      IPV6_DLIST_SET_NEXT(McAddrIdx, IPV6_MLD_VAR(McAddrList));
    }

    if (0 == RecordCount)
    {
      /* do not sent report if it does not contain any records */
      IpV6_Mld_VCancelTransmit(IpCtrlIdx);
      return E_OK;
    }

    ReportHdrPtr->McAddrRecCountNbo = IPV6_HTONS(RecordCount);

    if (E_OK != IpV6_Mld_VTransmit(IpCtrlIdx, MldBufPos))
    {
      return E_NOT_OK;
    }
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VTxGeneralQueryResponse() */

/**********************************************************************************************************************
 *  IpV6_Mld_VTxMsgV1()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxMsgV1(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr,
  boolean                 Report)
{
  uint16                           MldBufLen;
  IPV6_P2V(uint8)                  MldBufPtr;
  IPV6_P2V(IpV6_MldV1_MessageType) MsgPtr;
  uint8                            V1ReportType;
  IPV6_P2C(IpV6_AddrType)          DstAddrPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (TRUE == Report)
  {
    V1ReportType = IPV6_ICMP_MSG_TYPE_MLD_V1_REPORT;
    DstAddrPtr = McAddrPtr;
  }
  else
  {
    V1ReportType = IPV6_ICMP_MSG_TYPE_MLD_V1_DONE;
    DstAddrPtr = &IpV6_AddrAllRoutersLL;
  }

  MldBufLen = sizeof(IpV6_MldV1_MessageType);

  if (BUFREQ_OK != IpV6_Mld_VProvideTxBuffer(IpCtrlIdx, DstAddrPtr, &MldBufPtr, &MldBufLen, V1ReportType, TRUE))
  {
    return E_NOT_OK;
  }

  MsgPtr = (IPV6_P2V(IpV6_MldV1_MessageType))&MldBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  MsgPtr->MaxResDelayNbo = 0; /* response delay is unused in Report and Done messages */
  MsgPtr->Reserved16Nbo  = 0;

  IPV6_ADDR_COPY(MsgPtr->McAddr, *McAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  return IpV6_Mld_VTransmit(IpCtrlIdx, sizeof(IpV6_MldV1_MessageType));
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VTxMsgV1() */

/**********************************************************************************************************************
 *  IpV6_Mld_VSetActive()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Mld_VSetActive(uint8 IpCtrlIdx, boolean Enable)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  if (IPV6_MLD_VAR(Enable) != Enable)
  {
    IPV6_MLD_VAR(Enable) = Enable;

    if (FALSE == Enable)
    {
      IpV6_Mld_VReset(IpCtrlIdx);
    }
    else if (IPV6_CTRL_STATE_LLADDR_READY <= IPV6_CTRL_VAR(IpV6_CtrlState))
    {
      IpV6_AddrType SolNodeAddr;   /* PRQA S 0759 */ /* MD_IpV6_0759 */
      uint8         OldRobustness;

      /* add entry for link-local solicited-node address without resending reports. */
      OldRobustness = IPV6_MLD_VAR(Robustness);
      IPV6_MLD_VAR(Robustness) = 0;

      IpV6_VGetSolicitedNodeAddr(&SolNodeAddr, &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address, IPV6_MULTICAST_SCOPE_LINK_LOCAL);
      (void)IpV6_Mld_MulticastListen(IpCtrlIdx, 0, &SolNodeAddr, (IpV6_AddrType*)NULL_PTR, 0, FALSE);

      IPV6_MLD_VAR(Robustness) = OldRobustness;
    }
    else
    {
      /* do nothing */
    }
  }
} /* End of IpV6_Mld_VSetActive() */

/**********************************************************************************************************************
 *  IpV6_Mld_VClearMcAddrEntry()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VClearMcAddrEntry(
  uint8                         IpCtrlIdx,
  IPV6_P2V(IpV6_Mld_McAddrType) McAddrEntryPtr)
{
  IpV6_DListIdxType McMsIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != McAddrEntryPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  McMsIdx = McAddrEntryPtr->McMsList.FirstIdx;

  while (IPV6_DLIST_END_IDX != McMsIdx)
  {
    IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList);

    IPV6_DLIST_SET_NEXT(McMsIdx, McAddrEntryPtr->McMsList);
  }

  IpV6_DListClear(&McAddrEntryPtr->McMsList);

  IpV6_DListClear(&McAddrEntryPtr->IfState.SrcAddrsAllow);
  IpV6_DListClear(&McAddrEntryPtr->IfState.SrcAddrsBlock);
  IpV6_DListClear(&McAddrEntryPtr->IfState.SrcAddrsKeep);

  McAddrEntryPtr->PR = FALSE;
  IpV6_DListClear(&McAddrEntryPtr->PRSrcAddrList);
} /* End of IpV6_Mld_VClearMcAddrEntry() */

/**********************************************************************************************************************
 *  IpV6_Mld_VClearMcAddrEntry2()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Mld_VClearMcAddrEntry2(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) McAddrPtr)
{
  IpV6_DListIdxType McAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  /* serach for any memberships for given muticast address */
  McAddrIdx = IpV6_Mld_VMcAddrListFind(IpCtrlIdx, McAddrPtr);

  if (IPV6_DLIST_END_IDX != McAddrIdx)
  {
    IpV6_Mld_VClearMcAddrEntry(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]);
    IpV6_DListRemoveElement(&IPV6_MLD_VAR(McAddrList), McAddrIdx);
  }
} /* End of IpV6_Mld_VClearMcAddrEntry2() */

/**********************************************************************************************************************
 *  IpV6_Mld_VReset()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VReset(
  uint8 IpCtrlIdx)
{
  IpV6_DListIdxType McAddrIdx;

  IPV6_MLD_VAR(Robustness) = IpV6_VCfgGetMldRobustness();
  IPV6_MLD_VAR(V1CompatMode) = FALSE;

  IPV6_MLD_VAR(ReportPendingCount) = 0;

  McAddrIdx = IPV6_MLD_VAR(McAddrList).FirstIdx;

  while (IPV6_DLIST_END_IDX != McAddrIdx)
  {
    IpV6_Mld_VClearMcAddrEntry(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]);

    IPV6_DLIST_SET_NEXT(McAddrIdx, IPV6_MLD_VAR(McAddrList));
  }

  IpV6_DListClear(&IPV6_MLD_VAR(McAddrList));

} /* End of IpV6_Mld_VReset() */

/**********************************************************************************************************************
 *  IpV6_Mld_Init()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Mld_Init(uint8 IpCtrlIdx)
{
  IpV6_DListIdxType McMsIdx, McAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_MLD_INIT, IPV6_E_INV_CTRL_IDX);
#endif

  IPV6_MLD_VAR(Enable) = FALSE;
  IPV6_MLD_VAR(Robustness) = IpV6_VCfgGetMldRobustness();
  IPV6_MLD_VAR(V1CompatMode) = FALSE;

  IPV6_MLD_VAR(ReportPendingCount) = 0;

  /* init list descriptors */
  IpV6_DListInitDesc(&IPV6_MLD_VAR(McAddrListDesc),         &IPV6_CTRL_VAR(IpV6_Mld_McAddrListNodes)[0],         IpV6_VCfgGetMldIfStatePoolSize());
  IpV6_DListInitDesc(&IPV6_MLD_VAR(McMsListDesc),           &IPV6_CTRL_VAR(IpV6_Mld_McMsListNodes)[0],           IpV6_VCfgGetMldSocketMembershipPoolSize());
  IpV6_DListInitDesc(&IPV6_MLD_VAR(McSrcAddrListDesc),      &IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrListNodes)[0],      IpV6_VCfgGetMldSrcAddrPoolSize());
  IpV6_DListInitDesc(&IPV6_MLD_VAR(IfStateSrcAddrListDesc), &IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrListNodes)[0], IpV6_VCfgGetMldIfStateSrcAddrPoolSize());

  /* init main multicast membership list */
  IpV6_DListInit(&IPV6_MLD_VAR(McAddrList), &IPV6_MLD_VAR(McAddrListDesc));

  /* init data of multicast membership list */
  for (McAddrIdx = 0; McAddrIdx < IpV6_VCfgGetMldIfStatePoolSize(); McAddrIdx++)
  {
    /* init per socket multicast membership list */
    IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList,              &IPV6_MLD_VAR(McMsListDesc));

    /* init interface state source address list */
    IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsAllow, &IPV6_MLD_VAR(IfStateSrcAddrListDesc));
    IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsBlock, &IPV6_MLD_VAR(IfStateSrcAddrListDesc));
    IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsKeep,  &IPV6_MLD_VAR(IfStateSrcAddrListDesc));

    IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PRSrcAddrList, &IPV6_MLD_VAR(McSrcAddrListDesc));

    IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PR = FALSE;
    IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeIsInclude = FALSE;
  }

  /* init source address list of each multicast address membership data element */
  for (McMsIdx = 0; McMsIdx < IpV6_VCfgGetMldSocketMembershipPoolSize(); McMsIdx++)
  {
    IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_Mld_McMsData)[McMsIdx].SrcAddrList, &IPV6_MLD_VAR(McSrcAddrListDesc));
  }

  IpV6_SetHopLimit(IPV6_SOCK_IDX_MLD, IPV6_MLD_MSG_HOP_LIMIT);
} /* End of IpV6_Mld_Init() */

/**********************************************************************************************************************
 *  IpV6_Mld_VFilterSrcAddrList()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VFilterSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2V(IpV6_DListType) SrcAddrListPtr,
  IPV6_P2C(IpV6_DListType) IfStateSrcAddrFilterListPtr)
{
  IpV6_DListIdxType IfStateSrcAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrListPtr,                                                    IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != IfStateSrcAddrFilterListPtr,                                       IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(&IPV6_MLD_VAR(IfStateSrcAddrListDesc) == IfStateSrcAddrFilterListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  IfStateSrcAddrIdx = IfStateSrcAddrFilterListPtr->FirstIdx;

  while(IPV6_DLIST_END_IDX != IfStateSrcAddrIdx)
  {
    IpV6_DListIdxType SrcAddrListIdx = IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, SrcAddrListPtr, &IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[IfStateSrcAddrIdx].SrcAddr);

    if (IPV6_DLIST_END_IDX != SrcAddrListIdx)
    {
      IpV6_DListRemoveElement(SrcAddrListPtr, SrcAddrListIdx);
    }

    IPV6_DLIST_SET_NEXT(IfStateSrcAddrIdx, *IfStateSrcAddrFilterListPtr);
  }
} /* End of IpV6_Mld_VFilterSrcAddrList() */

/**********************************************************************************************************************
 *  IpV6_Mld_VDumpFilteredIfStateSrcAddrList()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VDumpFilteredIfStateSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) IfStateSrcAddrListPtr,
  IPV6_P2V(IpV6_DListType) SrcAddrFilterListPtr,
  IPV6_P2V(uint8)          BufPtr,
  IPV6_P2V(uint16)         BufPos,
  uint16                   BufLen,
  IPV6_P2V(uint8)          SrcAddrCount)
{
  IpV6_DListIdxType SrcAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 5 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != IfStateSrcAddrListPtr,                                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrFilterListPtr,                                   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != BufPtr,                                                 IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((*BufPos) < BufLen,                                                 IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(&IPV6_MLD_VAR(IfStateSrcAddrListDesc) == IfStateSrcAddrListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  SrcAddrIdx = IfStateSrcAddrListPtr->FirstIdx;

  while(IPV6_DLIST_END_IDX != SrcAddrIdx)
  {
    IpV6_DListIdxType Idx = IpV6_Mld_VMcSrcAddrListFind(IpCtrlIdx, SrcAddrFilterListPtr, &IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[SrcAddrIdx].SrcAddr);

    if (IPV6_DLIST_END_IDX != Idx)
    {
      if (((*BufPos) + sizeof(IpV6_AddrType)) > BufLen)
      {
        /* not enough space left in buffer */
        return E_NOT_OK;
      }

      IpV6_DListRemoveElement(SrcAddrFilterListPtr, Idx);

      IPV6_ADDR_COPY(BufPtr[*BufPos], IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[SrcAddrIdx].SrcAddr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      (*BufPos) += sizeof(IpV6_AddrType);

      (*SrcAddrCount)++;
    }

    IPV6_DLIST_SET_NEXT(SrcAddrIdx, *IfStateSrcAddrListPtr);
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VDumpFilteredIfStateSrcAddrList() */

/**********************************************************************************************************************
 *  IpV6_Mld_VDumpIfStateSrcAddrList()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VDumpIfStateSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) IfStateSrcAddrListPtr,
  IPV6_P2V(uint8)          BufPtr,
  IPV6_P2V(uint16)         BufPos,
  uint16                   BufLen,
  IPV6_P2V(uint8)          SrcAddrCount)
{
  IpV6_DListIdxType SrcAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != IfStateSrcAddrListPtr,                                       IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(&IPV6_MLD_VAR(IfStateSrcAddrListDesc) == IfStateSrcAddrListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != BufPtr,                                                      IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((*BufPos) < BufLen,                                                      IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  SrcAddrIdx = IfStateSrcAddrListPtr->FirstIdx;

  while(IPV6_DLIST_END_IDX != SrcAddrIdx)
  {
    if (((*BufPos) + sizeof(IpV6_AddrType)) > BufLen)
    {
      return E_NOT_OK;
    }

    IPV6_ADDR_COPY(BufPtr[*BufPos], IPV6_CTRL_VAR(IpV6_Mld_IfStateSrcAddrData)[SrcAddrIdx].SrcAddr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    (*BufPos) += sizeof(IpV6_AddrType);

    (*SrcAddrCount)++;

    IPV6_DLIST_SET_NEXT(SrcAddrIdx, *IfStateSrcAddrListPtr);
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VDumpIfStateSrcAddrList() */

/**********************************************************************************************************************
 *  IpV6_Mld_VDumpSrcAddrList()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VDumpSrcAddrList(
  uint8                    IpCtrlIdx,
  IPV6_P2C(IpV6_DListType) SrcAddrListPtr,
  IPV6_P2V(uint8)          BufPtr,
  IPV6_P2V(uint16)         BufPos,
  uint16                   BufLen,
  IPV6_P2V(uint8)          SrcAddrCount)
{
  IpV6_DListIdxType SrcAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrListPtr,                             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(&IPV6_MLD_VAR(McSrcAddrListDesc) == SrcAddrListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != BufPtr,                                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((*BufPos) < BufLen,                                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  SrcAddrIdx = SrcAddrListPtr->FirstIdx;

  while(IPV6_DLIST_END_IDX != SrcAddrIdx)
  {
    if (((*BufPos) + sizeof(IpV6_AddrType)) > BufLen)
    {
      return E_NOT_OK;
    }

    IPV6_ADDR_COPY(BufPtr[*BufPos], IPV6_CTRL_VAR(IpV6_Mld_McSrcAddrData)[SrcAddrIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    (*BufPos) += sizeof(IpV6_AddrType);

    (*SrcAddrCount)++;

    IPV6_DLIST_SET_NEXT(SrcAddrIdx, *SrcAddrListPtr);
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VDumpSrcAddrList() */

/**********************************************************************************************************************
 *  IpV6_Mld_VTxFilterModeChgReport()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxFilterModeChgReport(
  uint8                         IpCtrlIdx,
  IPV6_P2C(IpV6_Mld_McAddrType) McAddrPtr)
{
  IPV6_P2V(uint8)                           MldBufPtr;
  uint16                                    MldBufLen;
  IPV6_P2V(IpV6_Mld_ReportHeaderType)       ReportHdrPtr;
  IPV6_P2V(IpV6_Mld_ReportRecordHeaderType) RecordHdrPtr;
  uint16                                    DataPos;
  uint8                                     SrcAddrCount;
  boolean TxBufferOverflow = FALSE;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  MldBufLen = IpV6_VCfgGetMldMaxReportMsgSize();

  if (BUFREQ_OK != IpV6_Mld_VProvideTxBuffer(IpCtrlIdx, &IpV6_Mld_ReportDstAddr, &MldBufPtr, &MldBufLen, IPV6_ICMP_MSG_TYPE_MLD_REPORT, TRUE))
  {
    return E_NOT_OK;
  }

  ReportHdrPtr = (IPV6_P2V(IpV6_Mld_ReportHeaderType))MldBufPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  ReportHdrPtr->Reserved16Nbo = 0;
  ReportHdrPtr->McAddrRecCountNbo = IPV6_UINT8_HTONS(1);

  DataPos = sizeof(IpV6_Mld_ReportHeaderType);

  /* Add Filter Mode Change Record */

  RecordHdrPtr = (IPV6_P2V(IpV6_Mld_ReportRecordHeaderType))&MldBufPtr[DataPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  RecordHdrPtr->Type       = (uint8)((TRUE == McAddrPtr->IfState.ModeIsInclude) ? IPV6_MLD_RECORD_TYPE_TO_IN : IPV6_MLD_RECORD_TYPE_TO_EX);
  RecordHdrPtr->AuxDataLen = 0;

  IPV6_ADDR_COPY(RecordHdrPtr->McAddr, *McAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  DataPos += sizeof(IpV6_Mld_ReportRecordHeaderType);

  SrcAddrCount = 0;

  if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsKeep, MldBufPtr, &DataPos, MldBufLen, &SrcAddrCount))
  {
    TxBufferOverflow = TRUE;
  }

  if (TRUE == McAddrPtr->IfState.ModeIsInclude)
  {
    if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsAllow, MldBufPtr, &DataPos, MldBufLen, &SrcAddrCount))
    {
      TxBufferOverflow = TRUE;
    }
  }
  else
  {
    if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsBlock, MldBufPtr, &DataPos, MldBufLen, &SrcAddrCount))
    {
      TxBufferOverflow = TRUE;
    }
  }

  if (TRUE == TxBufferOverflow)
  {
    IpV6_Mld_VCancelTransmit(IpCtrlIdx);
    return E_NOT_OK;
  }

  RecordHdrPtr->SrcAddrCountNbo = IPV6_UINT8_HTONS(SrcAddrCount);

  return IpV6_Mld_VTransmit(IpCtrlIdx, DataPos);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VTxFilterModeChgReport() */

/**********************************************************************************************************************
 *  IpV6_Mld_VProvideTxBuffer()
 **********************************************************************************************************************/
STATIC FUNC(BufReq_ReturnType, IPV6_CODE) IpV6_Mld_VProvideTxBuffer(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr,
  IPV6_P2V(uint8*)        BufPtrPtr,
  IPV6_P2V(uint16)        BufLenPtr,
  uint8                   Type,
  boolean                 AllowUnspecSrcAddr)
{
  uint16                        IpBufPos;
  IpV6_AddrIdType               AddrId;
  IPV6_P2V(IpV6_IcmpHeaderType) IcmpHdrPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != BufPtrPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != BufLenPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(0 < (*BufLenPtr),       IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   BUFREQ_E_NOT_OK);
#endif

  if ((IPV6_CTRL_STATE_LLADDR_READY <= IPV6_CTRL_VAR(IpV6_CtrlState)) || (TRUE == AllowUnspecSrcAddr))
  {
    AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].IpAddrId;
  }
  else
  {
    return BUFREQ_E_NOT_OK;
  }

  /* Request additional space for Hop-by-Hop header */
  (*BufLenPtr) += IPV6_CALC_EXT_HEADER_LENGTH(IPV6_HBH_HDR_MIN_LEN);

  if (BUFREQ_OK != IpV6_ProvideTxBuffer(&AddrId, IPV6_SOCK_IDX_MLD, &IPV6_MLD_VAR(TxBufIdx), DstAddrPtr, &IPV6_MLD_VAR(TxBufPtr), BufLenPtr))
  {
    return BUFREQ_E_NOT_OK;
  }

  /* Write Hop-by-Hop Options Header [RFC2460 4.3] containing IPv6 Router Alert Option [RFC2711]. */

  /* Header */
  IPV6_MLD_VAR(TxBufPtr)[0] = IPV6_HDR_ID_ICMP;     /* Next Header */
  IPV6_MLD_VAR(TxBufPtr)[1] = IPV6_HBH_HDR_MIN_LEN; /* Hdr Ext Len 0 == 8 + (0 * 8) */

  /* [RFC2711 2.1 Syntax] */
  IPV6_MLD_VAR(TxBufPtr)[2] = IPV6_HBH_OPT_TYPE_RTR_ALERT; /* Type: 5 */
  IPV6_MLD_VAR(TxBufPtr)[3] = IPV6_HBH_OPT_RTR_ALERT_LEN;  /* Length: 2 bytes */
  /* Value 0: Datagram contains a Multicast Listener Discovery message [RFC2710] */
  IPV6_MLD_VAR(TxBufPtr)[4] = 0;
  IPV6_MLD_VAR(TxBufPtr)[5] = 0;

  /* 2-byte padding (PadN option) */
  IPV6_MLD_VAR(TxBufPtr)[6] = IPV6_EXT_HDR_OPT_TYPE_PAD_N;
  IPV6_MLD_VAR(TxBufPtr)[7] = 0;

  IpBufPos = IPV6_CALC_EXT_HEADER_LENGTH(IPV6_HBH_HDR_MIN_LEN);

  IcmpHdrPtr = (IPV6_P2V(IpV6_IcmpHeaderType))&IPV6_MLD_VAR(TxBufPtr)[8]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  IcmpHdrPtr->Type = Type;
  IcmpHdrPtr->Code = IPV6_MLD_ICMP_MSG_CODE;
  IcmpHdrPtr->ChecksumNbo = 0; /* set to 0 for calculation. Has to be updated before IpV6_Transmit() */

  IpBufPos += sizeof(IpV6_IcmpHeaderType);

  (*BufPtrPtr) = &IPV6_MLD_VAR(TxBufPtr)[IpBufPos];
  (*BufLenPtr) -= IpBufPos;

  return BUFREQ_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VProvideTxBuffer() */

/**********************************************************************************************************************
 *  IpV6_Mld_VCancelTransmit()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VCancelTransmit(
  uint8 IpCtrlIdx)
{
  (void)IpV6_CancelTransmit(IPV6_SOCK_IDX_MLD, IPV6_MLD_VAR(TxBufIdx));
} /* End of IpV6_Mld_VCancelTransmit() */

/**********************************************************************************************************************
 *  IpV6_Mld_VTransmit()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTransmit(
  uint8  IpCtrlIdx,
  uint16 PayloadLen)
{
  IPV6_P2V(IpV6_IcmpHeaderType) IcmpHdrPtr;

  IcmpHdrPtr = (IPV6_P2V(IpV6_IcmpHeaderType))&IPV6_MLD_VAR(TxBufPtr)[8]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IcmpHdrPtr->ChecksumNbo = IpV6_Icmp_VCalculateChecksumNbo(IpV6_GetSrcAddrPtr(IPV6_SOCK_IDX_MLD), IpV6_GetDstAddrPtr(IPV6_SOCK_IDX_MLD), &IPV6_MLD_VAR(TxBufPtr)[8], sizeof(IpV6_IcmpHeaderType) + PayloadLen);

  return IpV6_Transmit(IPV6_SOCK_IDX_MLD, IPV6_MLD_VAR(TxBufIdx), IPV6_EXT_HDR_ID_HOP_BY_HOP, 8 + sizeof(IpV6_IcmpHeaderType) + PayloadLen, FALSE, 0, FALSE);
} /* End of IpV6_Mld_VTransmit() */

/**********************************************************************************************************************
 *  IpV6_Mld_VTxSrcListChgReport()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Mld_VTxSrcListChgReport(
  uint8                         IpCtrlIdx,
  IPV6_P2C(IpV6_Mld_McAddrType) McAddrPtr)
{
  IPV6_P2V(uint8)                           MldBufPtr;
  uint16                                    MldBufLen;
  IPV6_P2V(IpV6_Mld_ReportHeaderType)       ReportHdrPtr;
  IPV6_P2V(IpV6_Mld_ReportRecordHeaderType) RecordHdrPtr;
  uint16                                    MldBufPos;
  uint8                                     RecordCount;
  uint8                                     SrcAddrCount;
  boolean TxBufferOverflow = FALSE;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  MldBufLen = IpV6_VCfgGetMldMaxReportMsgSize();

  if (BUFREQ_OK != IpV6_Mld_VProvideTxBuffer(IpCtrlIdx, &IpV6_Mld_ReportDstAddr, &MldBufPtr, &MldBufLen, IPV6_ICMP_MSG_TYPE_MLD_REPORT, TRUE))
  {
    return E_NOT_OK;
  }

  ReportHdrPtr = (IPV6_P2V(IpV6_Mld_ReportHeaderType))&MldBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  ReportHdrPtr->Reserved16Nbo = 0;

  MldBufPos = sizeof(IpV6_Mld_ReportHeaderType);

  /* Add Source List Chnage Records */

  RecordCount = 0;

  if (!IPV6_DLIST_IS_EMPTY(McAddrPtr->IfState.SrcAddrsAllow))
  {
    RecordHdrPtr = (IPV6_P2V(IpV6_Mld_ReportRecordHeaderType))&MldBufPtr[MldBufPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

    RecordHdrPtr->Type       = IPV6_MLD_RECORD_TYPE_ALLOW;
    RecordHdrPtr->AuxDataLen = 0;

    IPV6_ADDR_COPY(RecordHdrPtr->McAddr, *McAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

    MldBufPos += sizeof(IpV6_Mld_ReportRecordHeaderType);

    SrcAddrCount = 0;

    if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsAllow, MldBufPtr, &MldBufPos, MldBufLen, &SrcAddrCount))
    {
      TxBufferOverflow = TRUE;
    }

    RecordHdrPtr->SrcAddrCountNbo = IPV6_UINT8_HTONS(SrcAddrCount);
    RecordCount++;
  }

  if (!IPV6_DLIST_IS_EMPTY(McAddrPtr->IfState.SrcAddrsBlock))
  {
    if ((MldBufPos + sizeof(IpV6_Mld_ReportRecordHeaderType)) <= MldBufLen)
    {
      RecordHdrPtr = (IPV6_P2V(IpV6_Mld_ReportRecordHeaderType))&MldBufPtr[MldBufPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

      RecordHdrPtr->Type       = IPV6_MLD_RECORD_TYPE_BLOCK;
      RecordHdrPtr->AuxDataLen = 0;

      IPV6_ADDR_COPY(RecordHdrPtr->McAddr, *McAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

      MldBufPos += sizeof(IpV6_Mld_ReportRecordHeaderType);

      SrcAddrCount = 0;

      if (E_OK != IpV6_Mld_VDumpIfStateSrcAddrList(IpCtrlIdx, &McAddrPtr->IfState.SrcAddrsBlock, MldBufPtr, &MldBufPos, MldBufLen, &SrcAddrCount))
      {
        TxBufferOverflow = TRUE;
      }

      RecordHdrPtr->SrcAddrCountNbo = IPV6_UINT8_HTONS(SrcAddrCount);
      RecordCount++;
    }
    else
    {
      TxBufferOverflow = TRUE;
    }
  }

  if ((0 == RecordCount) || (TRUE == TxBufferOverflow))
  {
    IpV6_Mld_VCancelTransmit(IpCtrlIdx);
    return E_OK;
  }

  ReportHdrPtr->McAddrRecCountNbo = IPV6_UINT8_HTONS(RecordCount);

  return IpV6_Mld_VTransmit(IpCtrlIdx, MldBufPos);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_VTxSrcListChgReport() */

/**********************************************************************************************************************
 *  IpV6_Mld_VNotifyAllReportsSent()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Mld_VNotifyAllReportsSent(
  uint8                         IpCtrlIdx,
  IPV6_P2C(IpV6_Mld_McAddrType) McAddrEntryPtr)
{
  if (IPV6_DLIST_IS_EMPTY(McAddrEntryPtr->McMsList))
  {

    if (E_OK == IpV6_VMatchPrefix(&McAddrEntryPtr->McAddr, &IpV6_PrefixSolicitedNode.Prefix.addr[0], IpV6_PrefixSolicitedNode.PrefixLenBits))
    {

      if ((IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address.addr[13] == McAddrEntryPtr->McAddr.addr[13]) &&
          (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address.addr[14] == McAddrEntryPtr->McAddr.addr[14]) &&
          (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address.addr[15] == McAddrEntryPtr->McAddr.addr[15]))
      {

      }
    }
  }
  else
  {

    if (E_OK == IpV6_VMatchPrefix(&McAddrEntryPtr->McAddr, &IpV6_PrefixSolicitedNode.Prefix.addr[0], IpV6_PrefixSolicitedNode.PrefixLenBits))
    {

      if ((IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address.addr[13] == McAddrEntryPtr->McAddr.addr[13]) &&
          (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address.addr[14] == McAddrEntryPtr->McAddr.addr[14]) &&
          (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].Address.addr[15] == McAddrEntryPtr->McAddr.addr[15]))
      {

#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
        if ((IPV6_CTRL_VAR(IpV6_CtrlState) < IPV6_CTRL_STATE_LLADDR_READY) || (IPV6_LADDR_STATE_OPTIMISTIC == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].State))
#else
        if (IPV6_CTRL_VAR(IpV6_CtrlState) < IPV6_CTRL_STATE_LLADDR_READY)
#endif
        {
          IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].NextEventTime, 0);
        }
      }
    }
  }
} /* End of IpV6_Mld_VNotifyAllReportsSent() */

/**********************************************************************************************************************
 *  IpV6_Mld_MainFunction()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Mld_MainFunction(uint8 IpCtrlIdx)
{
  IpV6_DListIdxType McAddrIdx;
  Std_ReturnType    TransmitResult;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_MLD_MAIN_FUNCTION, IPV6_E_INV_CTRL_IDX);
#endif

  if (FALSE == IPV6_MLD_VAR(Enable))
  {
    return;
  }

  if (TRUE == IPV6_MLD_VAR(V1CompatMode))
  {
    /* MLDv1 Compatibility Mode */

    if (IPV6_TIME_EXPIRED(IPV6_MLD_VAR(V1QuerierPresentTimeout)))
    {
      /* No more MLDv1 modes discovered. Switch from compatibility mode to normal operation. */
      IpV6_Mld_VChangeCompatMode(IpCtrlIdx, FALSE); /*lint !e522 */ /* This function has side-effects! (modifies global variables) */
      return;
    }
  }
  else
  {
    /* MLDv2 only */
    if (0 < IPV6_MLD_VAR(ReportPendingCount))
    {
      if (IPV6_TIME_EXPIRED(IPV6_MLD_VAR(ReportTimeout)))
      {
        if (E_OK == IpV6_Mld_VTxGeneralQueryResponse(IpCtrlIdx))
        {
          IPV6_MLD_VAR(ReportPendingCount)--;
          IPV6_SET_EVENT_TIME_MS(IPV6_MLD_VAR(ReportTimeout), 1000);
        }
      }
    }
  }

  /* Iterate over all multicast memberships */

  McAddrIdx = IPV6_MLD_VAR(McAddrList).FirstIdx;

  while (IPV6_DLIST_END_IDX != McAddrIdx)
  {
    IpV6_DListIdxType NextMcAddrIdx = IPV6_DLIST_GET_NEXT(McAddrIdx, IPV6_MLD_VAR(McAddrList));

    /* Report interface state changes by sending unsilicited reports */
    if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout))
    {
      if (0 < IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeChgTxCounter)
      {
        /* Transmit Filter Mode Change Report */

        if (TRUE == IPV6_MLD_VAR(V1CompatMode))
        {
          /* MLDv1 */
          TransmitResult = IpV6_Mld_VTxMsgV1(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McAddr, (boolean)(!IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList)));
        }
        else
        {
          /* MLDv2 */
          TransmitResult = IpV6_Mld_VTxFilterModeChgReport(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]);
        }

        if (E_OK == TransmitResult)
        {
          IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeChgTxCounter--;

          if (TRUE == IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeIsInclude)
          {
            IpV6_Mld_VDecCnt(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsAllow, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsKeep);
          }
          else
          {
            IpV6_Mld_VDecCnt(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsBlock, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsKeep);
          }

          if (IpV6_VCfgGetMldMinUnsolicitedReportIntMs() < IpV6_VCfgGetMldMaxUnsolicitedReportIntMs()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
          {
            uint32 DelayMs = IpV6_VGetRandomValue(IpV6_VCfgGetMldMinUnsolicitedReportIntMs(), IpV6_VCfgGetMldMaxUnsolicitedReportIntMs());
            IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout, DelayMs);
          }
          else
          { /* PRQA S 3201 */ /* MD_IpV6_3201 */
            IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout, IpV6_VCfgGetMldMinUnsolicitedReportIntMs());
          }
        }
      }
      else if ((!IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsAllow)) || (!IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsBlock)))
      {
        /* Transmit Source List Change Report */

        if (TRUE == IPV6_MLD_VAR(V1CompatMode))
        {
          /* MLDv1 */
          TransmitResult = IpV6_Mld_VTxMsgV1(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McAddr, (boolean)(!IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList)));
        }
        else
        {
          /* MLDv2 */
          TransmitResult = IpV6_Mld_VTxSrcListChgReport(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]);
        }

        if (E_OK == TransmitResult)
        {
          if (TRUE == IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeIsInclude)
          {
            IpV6_Mld_VDecCnt(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsAllow, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsKeep);
            IpV6_Mld_VDecCnt(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsBlock, (IPV6_P2V(IpV6_DListType))NULL_PTR);
          }
          else
          {
            IpV6_Mld_VDecCnt(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsBlock, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsKeep);
            IpV6_Mld_VDecCnt(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsAllow, (IPV6_P2V(IpV6_DListType))NULL_PTR);
          }

          if (IpV6_VCfgGetMldMinUnsolicitedReportIntMs() < IpV6_VCfgGetMldMaxUnsolicitedReportIntMs()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
          {
            uint32 DelayMs = IpV6_VGetRandomValue(IpV6_VCfgGetMldMinUnsolicitedReportIntMs(), IpV6_VCfgGetMldMaxUnsolicitedReportIntMs());
            IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout, DelayMs);
          }
          else
          { /* PRQA S 3201 */ /* MD_IpV6_3201 */
            IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout, IpV6_VCfgGetMldMinUnsolicitedReportIntMs());
          }
        }
      }
      else
      {
        /* nothing to do for this muticast address */
      }

      if ((0 == IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.ModeChgTxCounter) &&
          IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsAllow) &&
          IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.SrcAddrsBlock))
      {
        /* all retransmissions sent */
        IPV6_TIME_SET_INFINITY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].IfState.Timeout);

        IpV6_Mld_VNotifyAllReportsSent(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]);

        if (IPV6_DLIST_IS_EMPTY(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McMsList))
        {
          /* no memberships left for this multicast address.
           * remove multicast address entry
           */
          IpV6_Mld_VClearMcAddrEntry(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]);
          IpV6_DListRemoveElement(&IPV6_MLD_VAR(McAddrList), McAddrIdx);
        }
      }
    }

    /* Send responses to multicast address specific queries */
    if (TRUE == IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PR)
    {
      if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PRTimeout))
      {
        if (TRUE == IPV6_MLD_VAR(V1CompatMode))
        {
          /* MLDv1 */
          if (E_OK == IpV6_Mld_VTxMsgV1(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].McAddr, TRUE))
          {
            IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PR = FALSE;
          }
        }
        else
        {
          /* MLDv2 */
          if (E_OK == IpV6_Mld_VTxMcAddrQueryResponse(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx]))
          {
            IPV6_CTRL_VAR(IpV6_Mld_McAddrData)[McAddrIdx].PR = FALSE;
          }
        }
      }
    }

    McAddrIdx = NextMcAddrIdx;
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Mld_MainFunction() */

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (IPV6_ENABLE_MLD == STD_ON) */
#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: IpV6_Mld.c
 *********************************************************************************************************************/
