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
 *         \file  TcpIp_Ndp.c
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

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_NDP_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_IpV6_Priv.h"
#include "TcpIp_IcmpV6.h"
#include "TcpIp_Ndp.h"
#if (IPV6_ENABLE_MLD == STD_ON)
# include "TcpIp_Mld.h"
#endif
#include "TcpIp_Priv.h"

#include "EthIf.h"

#if (IPV6_ENABLE_RFC6106_RDNSS_OPT == STD_ON)
# include "Dns.h"
#endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

#define IPV6_NDP_ENABLE_LL_ADDR_GATHERING             STD_ON
#define IPV6_NDP_ENABLE_TIMEOUT_OPTIMIZATION          STD_OFF

#define IPV6_NDP_NA_FLAG_ROUTER    0x80 /* (1<<7) */
#define IPV6_NDP_NA_FLAG_SOLICITED 0x40 /* (1<<6) */
#define IPV6_NDP_NA_FLAG_OVERRIDE  0x20 /* (1<<5) */

typedef struct
{
  unsigned int Solicited : 1;
  unsigned int Override : 1;
  unsigned int Router : 1;
  unsigned int TgtLLAddrChanged : 1;
  unsigned int SrcLLAddrChanged : 1;
} IpV6_NdpNaFlagsType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

#define IPV6_NDP_CALC_OPTION_LENGTH(LENGTH) (8 * (LENGTH))

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
#define IPV6_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
VAR(uint32, IPV6_VAR_NOINIT) IpV6_DbgNeighborCacheLookups;
VAR(uint32, IPV6_VAR_NOINIT) IpV6_DbgNeighborCacheHits;
VAR(uint32, IPV6_VAR_NOINIT) IpV6_DbgNeighborCacheOptimizedHits;
VAR(uint32, IPV6_VAR_NOINIT) IpV6_DbgDestinationCacheLookups;
VAR(uint32, IPV6_VAR_NOINIT) IpV6_DbgDestinationCacheHits;
VAR(uint32, IPV6_VAR_NOINIT) IpV6_DbgDestinationCacheOptimizedHits;
#endif

#define IPV6_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCInvalidateSrcAddrs()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDCInvalidateSrcAddrs(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDestinationUnreachableCbks()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDestinationUnreachableCbks(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VWriteLLAddrOpt()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VWriteLLAddrOpt(
  uint8 IpCtrlIdx,
  uint8 OptType,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr);

#if ((IPV6_ENABLE_NDP_INV_NA == STD_ON) || (IPV6_ENABLE_NDP_INV_NS == STD_ON))
/**********************************************************************************************************************
 *  IpV6_Ndp_VWriteAddrListOpt()
 **********************************************************************************************************************/
STATIC FUNC(uint16, IPV6_CODE) IpV6_Ndp_VWriteAddrListOpt(
  uint8 IpCtrlIdx,
  uint8 OptType,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr);
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLCheckLifetimes()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDRLCheckLifetimes(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCCheckLifetimes()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VNCCheckLifetimes(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_VSetAddrStateExpired()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VSetAddrStateExpired(IpV6_AddrIdType AddrId);

/**********************************************************************************************************************
 *  IpV6_Ndp_VSATCheckLifetimes()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VSATCheckLifetimes(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_Ndp_VSlaacRxPrefixInformation()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VSlaacRxPrefixInformation(
  uint8 IpCtrlIdx,
  P2CONST(IpV6_NdpOptPrefixInfoType, AUTOMATIC, IPV6_APPL_DATA) PrefixInfoPtr,
  boolean IsMulticastRa);

/**********************************************************************************************************************
 *  IpV6_Ndp_VProcessPrefixInformation()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VProcessPrefixInformation(
  uint8                               IpCtrlIdx,
  IPV6_P2C(IpV6_NdpOptPrefixInfoType) PrefixInfoPtr,
  boolean                             IsMulticastRa);

/**********************************************************************************************************************
 *  IpV6_Ndp_VValidateOptions()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VValidateOptions(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr,
  uint16 DataLen);

/**********************************************************************************************************************
 *  IpV6_Ndp_VDefaultRouterUpdate()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDefaultRouterUpdate(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  uint16                  Lifetime);

#if (IPV6_ENABLE_PATH_MTU == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VDCCheckLifetimes()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDCCheckLifetimes(uint8 IpCtrlIdx);
#endif

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VPrivExtConfigureTempAddr()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VPrivExtConfigureTempAddr(
  uint8                                      IpCtrlIdx,
  IPV6_P2V(IpV6_SourceAddressTableEntryType) SatEntryPtr);

/**********************************************************************************************************************
 *  IpV6_Ndp_VPrivExtExtendTempAddrLifetimes()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VPrivExtExtendTempAddrLifetimes(
  uint8                                      IpCtrlIdx,
  IPV6_P2C(IpV6_SourceAddressTableEntryType) SatEntryPtr);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCLookup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VNCLookup(
  uint8                      IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)    NetAddrPtr,
  IPV6_P2V(IpV6_ListIdxType) EntryIdxPtr)
{
  IpV6_ListIdxType EntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != NetAddrPtr,            IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != EntryIdxPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgNeighborCacheLookups++;
# endif

  /* check if supplied EntryIdx is already the right one */
  if ((*EntryIdxPtr < IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount)) && /* PRQA S 3415 */ /* MD_IpV6_3415 */
      (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_NeighborCache)[*EntryIdxPtr].NeighborAddress, *NetAddrPtr)))
  {

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgNeighborCacheOptimizedHits++;
# endif

    IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_NeighborCache)[*EntryIdxPtr].LastUsedTime);
    return E_OK;
  }
#endif

  EntryIdx = IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
    /* skip entry with the supplied EntryIdx since it has already been checked */
    if (EntryIdx == *EntryIdxPtr)
    {
      continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
    }
#endif

    if (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].NeighborAddress, *NetAddrPtr)) /* valid entry found? */
    {

#if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgNeighborCacheHits++;
#endif

      IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].LastUsedTime);
      *EntryIdxPtr = EntryIdx;
      return E_OK;
    }
  }

  return E_NOT_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VNCLookup() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VPLGetOrCreateEntry()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VPLGetOrCreateEntry(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) AddrPrefixPtr,
  uint8                   PrefixLength,
  IPV6_P2V(uint8)         EntryIdxPtr)
{

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPrefixPtr,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != EntryIdxPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (E_OK == IpV6_Ndp_VPLLookup(IpCtrlIdx, AddrPrefixPtr, PrefixLength, EntryIdxPtr))
  {
    /* Return existing entry. */
    return E_OK;
  }

  if (IPV6_CTRL_VAR(IpV6_PrefixListEntryCount) < IpV6_VCfgGetPrefixListSize())
  {
    /* Initialize new entry. */
    *EntryIdxPtr = IPV6_CTRL_VAR(IpV6_PrefixListEntryCount);
    IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_PrefixList)[IPV6_CTRL_VAR(IpV6_PrefixListEntryCount)].Prefix, *AddrPrefixPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_CTRL_VAR(IpV6_PrefixList)[IPV6_CTRL_VAR(IpV6_PrefixListEntryCount)].PrefixLenBits = PrefixLength;
    IPV6_CTRL_VAR(IpV6_PrefixListEntryCount)++;
    return E_OK;
  }

  return E_NOT_OK; /* no matching entry found */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VPLGetOrCreateEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VPLLookup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VPLLookup(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) AddrPrefixPtr,
  uint8                   PrefixLength,
  IPV6_P2V(uint8)         EntryIdxPtr)
{
  IpV6_ListIdxType EntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPrefixPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != EntryIdxPtr,   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  EntryIdx = IPV6_CTRL_VAR(IpV6_PrefixListEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

    if (PrefixLength == IPV6_CTRL_VAR(IpV6_PrefixList)[EntryIdx].PrefixLenBits)
    {
      if (E_OK == IpV6_VMatchPrefix(AddrPrefixPtr, &IPV6_CTRL_VAR(IpV6_PrefixList)[EntryIdx].Prefix.addr[0], IPV6_CTRL_VAR(IpV6_PrefixList)[EntryIdx].PrefixLenBits))
      {
        *EntryIdxPtr = EntryIdx;
        return E_OK;
      }
    }
  }

  return E_NOT_OK; /* no matching entry found */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VPLLookup() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VPLRemoveEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VPLRemoveEntry(
  uint8 IpCtrlIdx,
  uint8 EntryIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_PL_IDX(EntryIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_PL(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* check for valid index */
  if (EntryIdx < IPV6_CTRL_VAR(IpV6_PrefixListEntryCount))
  {
    IPV6_CTRL_VAR(IpV6_PrefixListEntryCount)--;

    /* copy last entry at the position of the removed entry to avoid stale entrys */
    if (EntryIdx != IPV6_CTRL_VAR(IpV6_PrefixListEntryCount))
    {
      IPV6_CTRL_VAR(IpV6_PrefixList)[EntryIdx] = IPV6_CTRL_VAR(IpV6_PrefixList)[IPV6_CTRL_VAR(IpV6_PrefixListEntryCount)];
    }
  }

  IPV6_END_CRITICAL_SECTION_PL(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* End of IpV6_Ndp_VPLRemoveEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLLookup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VDRLLookup(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) RouterAddrPtr,
  IPV6_P2V(uint8)         EntryIdxPtr)
{
  uint8 EntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != RouterAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != EntryIdxPtr,   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  EntryIdx = IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

    if (IPV6_ADDR_EQUALS(*RouterAddrPtr, IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].NextHopAddress))
    {
      *EntryIdxPtr = EntryIdx;
      return E_OK;
    }
  }

  return E_NOT_OK; /* no matching entry found */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VDRLLookup() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLCreateEntry()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VDRLCreateEntry(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) RouterAddrPtr,
  IPV6_P2V(uint8)         EntryIdxPtr)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != RouterAddrPtr,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != EntryIdxPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
#endif

  if (IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount) < IpV6_VCfgGetDefaultRouterListSize())
  {
    *EntryIdxPtr = IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount);
    IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DefaultRouterList)[IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount)].NextHopAddress, *RouterAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount)++;
    return E_OK;
  }

  return E_NOT_OK; /* no matching entry found */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VDRLCreateEntry() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLRemoveEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VDRLRemoveEntry(
  uint8            IpCtrlIdx,
  IpV6_ListIdxType EntryIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_DRL_IDX(EntryIdx),   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_DRL(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* check for valid index */
  if (EntryIdx < IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount))
  {
    IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount)--;

    /* copy last entry at the position of the removed entry to avoid stale entrys */
    if (EntryIdx != IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount))
    {
      IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx] = IPV6_CTRL_VAR(IpV6_DefaultRouterList)[IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount)];
    }
  }

  IPV6_END_CRITICAL_SECTION_DRL(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* End of IpV6_Ndp_VDRLRemoveEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCGetOrCreateEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VNCGetOrCreateEntry(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) AddrPtr,
  IPV6_P2V(uint8)         EntryIdxPtr)
{
  IpV6_ListIdxType EntryIdx;
  IpV6_ListIdxType NewEntryIdx = 0;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != AddrPtr,     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != EntryIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  EntryIdx = IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount);

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgNeighborCacheLookups++;
# endif

  /* check if supplied EntryIdx is already the right one */
  if ((*EntryIdxPtr < IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount)) && /* PRQA S 3415 */ /* MD_IpV6_3415 */
      (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_NeighborCache)[*EntryIdxPtr].NeighborAddress, *AddrPtr)))
  {

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgNeighborCacheOptimizedHits++;
# endif

    IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_NeighborCache)[*EntryIdxPtr].LastUsedTime);
    return;
  }
#endif

  /* iterate over all currently stored entries */
  while (0 < EntryIdx)
  {
    EntryIdx--;

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
    /* skip entry with the supplied EntryIdx since it has already been checked */
    if (EntryIdx == *EntryIdxPtr)
    {
      continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
    }
#endif

    /* if entry is found, return */
    if (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].NeighborAddress, *AddrPtr)) /* entry found? */
    {

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgNeighborCacheHits++;
# endif

      IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].LastUsedTime);
      *EntryIdxPtr = EntryIdx;
      return;
    }

    /* if list is full we remember the least recently used entry */
    if(IPV6_TIME_IS_LOWER(IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].LastUsedTime, IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].LastUsedTime)) /* oldest entry so far? */
    {
      NewEntryIdx = EntryIdx; /* save least recently used entry */
    }
  }

  /* entry not found */

  /* space left in list */
  if (IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount) < IpV6_VCfgGetNeighborCacheSize())
  {
    NewEntryIdx = IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount);
    IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount)++;
  }
  else
  {
    /* extisting entry will be rempoved from cache */
    IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NewEntryIdx, NULL_PTR);
  }

  /* store new entry by overwriting LRU or next free entry entry */
  IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].NeighborAddress, *AddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].State      = IPV6_REACHABLE_STATE_INCOMPLETE;
  IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].IsRouter   = FALSE;
  IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].RetryCount = 0;
  IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].NaPending  = FALSE;
#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
  IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].InvNaPending = FALSE;
#endif
  IPV6_TIME_SET(IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].LastUsedTime, 0, 0);
  IPV6_TIME_SET(IPV6_CTRL_VAR(IpV6_NeighborCache)[NewEntryIdx].NextEventTime, 0, 0);

  *EntryIdxPtr = NewEntryIdx;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VNCGetOrCreateEntry() */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCLookup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, AUTOMATIC) IpV6_Ndp_VDCLookup(
  uint8                      IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)    AddrPtr,
  IPV6_P2V(IpV6_ListIdxType) EntryIdxPtr)
{
  IpV6_ListIdxType EntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr,     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != EntryIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgDestinationCacheLookups++;
# endif

  /* check if supplied EntryIdx is already the right one */
  if ((*EntryIdxPtr < IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)) && /* PRQA S 3415 */ /* MD_IpV6_3415 */
    (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_DestinationCache)[*EntryIdxPtr].DestinationAddress, *AddrPtr)))
  {

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgDestinationCacheOptimizedHits++;
# endif

    IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_DestinationCache)[*EntryIdxPtr].LastUsedTime);
    return E_OK;
  }
#endif

  EntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
    /* skip entry with the supplied EntryIdx since it has already been checked */
    if (EntryIdx == *EntryIdxPtr)
    {
      continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
    }
#endif

    if (IPV6_ADDR_EQUALS(*AddrPtr, IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].DestinationAddress))
    {

# if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
    IpV6_DbgDestinationCacheHits++;
# endif

      IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].LastUsedTime);
      *EntryIdxPtr = EntryIdx;
      return E_OK;
    }
  }

  return E_NOT_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VDCLookup() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCGetOrCreateEntry()
 **********************************************************************************************************************/
FUNC(void, AUTOMATIC) IpV6_Ndp_VDCGetOrCreateEntry(
  uint8                      IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)    DstAddrPtr,
  IPV6_P2V(IpV6_ListIdxType) EntryIdxPtr)
{
  IpV6_ListIdxType EntryIdx;
  IpV6_ListIdxType NewEntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DstAddrPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != EntryIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  EntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);
  NewEntryIdx = 0;

  while (0 < EntryIdx)
  {
    EntryIdx--;

    if (IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].DestinationAddress, *DstAddrPtr))
    {
      IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].LastUsedTime);
      *EntryIdxPtr = EntryIdx;
      return;
    }

    /* if cache is full, remember least recently used entry */
    if (IPV6_TIME_IS_LOWER(IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].LastUsedTime, IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].LastUsedTime))
    {
      NewEntryIdx = EntryIdx;
    }
  }

  if (IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount) < IpV6_VCfgGetDestinationCacheSize())
  {
    NewEntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);
    IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)++;
  }

  IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].DestinationAddress, *DstAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].LastUsedTime);
  IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].SourceAddressIdx = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
#if (IPV6_ENABLE_PATH_MTU == STD_ON)
  if (IpV6_VCfgIsPathMtuEnabled())
  {
# if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
    if (IpV6_VCfgIsLinkMtuReconfigurationEnabled())
    {
      IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].Mtu         = IPV6_CTRL_VAR(IpV6_DefaultLinkMtu);
      IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].MtuReported = IPV6_CTRL_VAR(IpV6_DefaultLinkMtu);
    }
    else
# endif
    {
      IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].Mtu         = IpV6_VCfgGetDefaultLinkMtu();
      IPV6_CTRL_VAR(IpV6_DestinationCache)[NewEntryIdx].MtuReported = IpV6_VCfgGetDefaultLinkMtu();
    }
  }
#endif

  *EntryIdxPtr = NewEntryIdx;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VDCGetOrCreateEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCRemoveEntrys()
 **********************************************************************************************************************/
FUNC(void, AUTOMATIC) IpV6_Ndp_VDCRemoveEntrys(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) NextHopAddrPtr)
{
  uint8 EntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NextHopAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  EntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

    /* remove this entry if the next hop address matches address in NextHopAddrPtr */
    if (IPV6_ADDR_EQUALS(*NextHopAddrPtr, IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].NextHopAddress))
    {
      IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)--;

      /* copy last entry at the position of removed entry to avoid stale entry */
      if (EntryIdx != IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount))
      {
        IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx] = IPV6_CTRL_VAR(IpV6_DestinationCache)[IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)];
      }
    }
  }

  IPV6_END_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VDCRemoveEntrys() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCRemoveEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VDCRemoveEntry(
  uint8            IpCtrlIdx,
  IpV6_ListIdxType EntryIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_DC_IDX(EntryIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* check for valid index */
  if (EntryIdx < IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount))
  {
    IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)--;

    /* copy last entry at the position of the removed entry to avoid stale entrys */
    if (EntryIdx != IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount))
    {
      IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx] = IPV6_CTRL_VAR(IpV6_DestinationCache)[IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)];
    }
  }

  IPV6_END_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* End of IpV6_Ndp_VDCRemoveEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDCInvalidateSrcAddrs()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDCInvalidateSrcAddrs(uint8 IpCtrlIdx)
{
  uint8 EntryIdx;

  IPV6_BEGIN_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  EntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;
    IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].SourceAddressIdx = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
  }

  IPV6_END_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* End of IpV6_Ndp_VDCInvalidateSrcAddrs() */

#if (IPV6_ENABLE_PATH_MTU == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VDCCheckLifetimes()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDCCheckLifetimes(uint8 IpCtrlIdx)
{
  uint8 EntryIdx;

# if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
# endif

  /* #10 Iterate over all entries in the Destination Cache and reset MTU value to default if MTU timeout has expired. */
  IPV6_BEGIN_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  EntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

    if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].MtuTimeout))
    {
# if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
      if (IpV6_VCfgIsLinkMtuReconfigurationEnabled())
      {
        IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].Mtu         = IPV6_CTRL_VAR(IpV6_DefaultLinkMtu);
        IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].MtuReported = IPV6_CTRL_VAR(IpV6_DefaultLinkMtu);
      }
      else
# endif
      {
        IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].Mtu         = IpV6_VCfgGetDefaultLinkMtu();
        IPV6_CTRL_VAR(IpV6_DestinationCache)[EntryIdx].MtuReported = IpV6_VCfgGetDefaultLinkMtu();
      }
    }
  }

  IPV6_END_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* End of IpV6_Ndp_VDCCheckLifetimes() */
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_VDestinationUnreachableCbks()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDestinationUnreachableCbks(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr)
{
  IpBase_SockAddrIn6Type SockAddrIn6;

  SockAddrIn6.sin6_family = IPBASE_AF_INET6;
  SockAddrIn6.sin6_port = 0;
  IPV6_ADDR_COPY(SockAddrIn6.sin6_addr, *DstAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if (IPV6_ENABLE_ICMP_ECHO_REPLY == STD_ON)
  IpV6_Icmp_Cbk_DestinationUnreachable(DstAddrPtr);
#endif

  TcpIp_Cbk_VAddrResTimeout(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), (P2VAR(IpBase_SockAddrType, AUTOMATIC, IPV6_APPL_VAR))&SockAddrIn6); /* PRQA S 0310 */ /* MD_IpV6_0310 */
}

/**********************************************************************************************************************
 *  IpV6_Ndp_VWriteLLAddrOpt()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VWriteLLAddrOpt(
  uint8 IpCtrlIdx,
  uint8 OptType,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr)
{

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,               IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  DataPtr[0] = OptType;
  DataPtr[1] = 1; /* 1*8 byte */
  TCPIP_LL_GetPhysAddr(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), &DataPtr[2]);

} /* End of IpV6_Ndp_VWriteLLAddrOpt() */

#if ((IPV6_ENABLE_NDP_INV_NA == STD_ON) || (IPV6_ENABLE_NDP_INV_NS == STD_ON))
/**********************************************************************************************************************
 *  IpV6_Ndp_VWriteAddrListOpt()
 **********************************************************************************************************************/
STATIC FUNC(uint16, IPV6_CODE) IpV6_Ndp_VWriteAddrListOpt(
  uint8 IpCtrlIdx,
  uint8 OptType,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr)
{
  IpV6_CtrlAddrIdxType CtrlAddrIdx = 0;
  IpV6_ListIdxType     SaCount = 0;
  uint16               DataOffset;

# if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, 0);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,               IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  0);
# endif

  DataPtr[0] = OptType;
  /* DataPtr[1] (option length is written at end of this function */
  DataPtr[2] = 0;
  DataPtr[3] = 0;
  DataPtr[4] = 0;
  DataPtr[5] = 0;
  DataPtr[6] = 0;
  DataPtr[7] = 0;

  DataOffset = 8;

  while (CtrlAddrIdx < IpV6_VCfgGetSourceAddressTableSize())
  {
    if (IPV6_LADDR_STATE_INVALID != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
# if (IPV6_ENABLE_NDP_DAD == STD_ON)
      if (IPV6_LADDR_STATE_TENTATIVE != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
# endif
      {
        IPV6_ADDR_COPY(DataPtr[DataOffset], IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        DataOffset += IPV6_ADDRESS_LEN_BYTE;
        SaCount++;
      }

# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      if (IpV6_VCfgHasPrivExtConfig())
      {
        /* write all temporary addresses for this source address into buffer */

        IpV6_DListIdxType TempAddrIdx = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs.FirstIdx;

        while (IPV6_DLIST_END_IDX != TempAddrIdx)
        {
#  if (IPV6_ENABLE_NDP_DAD == STD_ON)
          if (IPV6_LADDR_STATE_TENTATIVE != IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].State)
#  endif
          {
            IPV6_ADDR_COPY(DataPtr[DataOffset], IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress); /* PRQA S 0310 */ /* MD_IpV6_0310 */
            DataOffset += IPV6_ADDRESS_LEN_BYTE;
            SaCount++;
          }

          IPV6_DLIST_SET_NEXT(TempAddrIdx, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs);
        }
      }
# endif
    }

    CtrlAddrIdx++;
  }

  /* write option length in units of 8 octets */
  DataPtr[1] = (uint8)(1 + (SaCount * 2)); /* (1 + (SaCount*2))*8 byte */

  return DataOffset;
} /* End of IpV6_Ndp_VWriteAddrListOpt() */
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_VValidateOptions()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VValidateOptions(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr,
  uint16 DataLen)
{
  uint16 DataOffset = 0;
  uint16 OptLenByte;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  while (DataOffset < DataLen)
  {
    if ((DataOffset + IPV6_TLV_OPT_DATA_OFFS) > DataLen)
    {
      /* incomplete option header */
      return E_NOT_OK;
    }

    /* OptType = DataPtr[DataOffset]; */
    OptLenByte = IPV6_NDP_CALC_OPTION_LENGTH(DataPtr[DataOffset+1]);

    /* RFC4861 6.1.2: All options must have a length that is greater than zero */
    if (0 == OptLenByte)
    {
      return E_NOT_OK;
    }

    if ((DataOffset + OptLenByte) > DataLen)
    {
      /* option length exceeds data length */
      return E_NOT_OK;
    }

    DataOffset += OptLenByte; /* skip to next option */
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VValidateOptions() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VAddSourceAddress()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VAddSourceAddress(
  VAR(uint8, AUTOMATIC) AddrId,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) AddressPtr,
  uint32  PreferredLifetime,
  uint32  ValidLifetime,
  uint32  DelayMs,
  uint8   DadMode)
{
  Std_ReturnType    Result = E_NOT_OK;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddressPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (!IPV6_ADDR_IS_EUI64(*AddressPtr))
  {
    /* We do not support IPv6 addresses that are not required to have 64Bit interface identifiers.
     * This check filters out all addresses starting with the binary value 000.
     * E.g. the Loopback and Unspecified addresses
     */
    return E_NOT_OK;
  }

  /* PRQA S 3415 1 */ /* MD_IpV6_3415 */
  if (IPV6_ADDR_IS_MULTICAST(*AddressPtr))
  {
    /* multicast addresses must not be assigned to a node */
    return E_NOT_OK;
  }

  if (0 < ValidLifetime)
  {
     /* add new source address to source address table */
    uint8                IpCtrlIdx   = IPV6_ADDR_ID_2_CTRL_IDX(AddrId); /* PRQA S 0781 */ /* MD_IpV6_05.06 */
    IpV6_CtrlAddrIdxType CtrlAddrIdx = IPV6_ADDR_ID_2_ADDR_IDX(AddrId);

    IpV6_SourceAddressTableEntryType *SatEntryPtr = &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx];

    /* initialize entry */
    IPV6_ADDR_COPY(SatEntryPtr->Address, *AddressPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_TIME_SET(SatEntryPtr->NextEventTime, 0, 0);

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (IpV6_VCfgHasPrivExtConfig())
    {
      /* clear list of temporary source addresses */
      IpV6_DListClear(&SatEntryPtr->TempAddrs);
    }
#endif

    IPV6_SET_LIFETIME_S(SatEntryPtr->PreferredTime, PreferredLifetime, (IpV6_TimeType*)NULL_PTR);
    IPV6_SET_LIFETIME_S(SatEntryPtr->ValidTime,     ValidLifetime,     (IpV6_TimeType*)NULL_PTR);

    /* Join solicited-node multicast group of this address.
     * This has to be done here before the address is configured (DAD is complete) because
     * we will only detect address configuration issues if we are able to receive NDP messages
     * that are sent to the solicited-node multicast address.
     */
    IPV6_VJOIN_SOLICITED_NODE_MULTICAST_GROUP(IpCtrlIdx, AddressPtr, TRUE);

    switch (DadMode)
    {
      /* Configure OPTIMISTIC address? */
    case IPV6_SRC_ADDR_DAD_MODE_OPTIMISTIC:
#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
      if (IpV6_VCfgIsOptimisticDadEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
      {
        SatEntryPtr->State         = IPV6_LADDR_STATE_OPTIMISTIC;
        SatEntryPtr->DadNsTxRemain = IpV6_VCfgGetDadTransmits();
        break; /* PRQA S 3333 */ /* MD_IpV6_14.6 */
      }
      else
#endif
      { /* PRQA S 3201 */ /* MD_IpV6_3201 */
        /* fall through */
      }

      /* Configure TENTATIVE address? */
    case IPV6_SRC_ADDR_DAD_MODE_NORMAL:
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
      if (0 < IpV6_VCfgGetDadTransmits()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
      {
        SatEntryPtr->State         = IPV6_LADDR_STATE_TENTATIVE;
        SatEntryPtr->DadNsTxRemain = IpV6_VCfgGetDadTransmits();
        break; /* PRQA S 3333 */ /* MD_IpV6_14.6 */
      }
      else
#endif
      { /* PRQA S 3201 */ /* MD_IpV6_3201 */
        /* fall through */
      }

      /* fall through */
    default:
      /* Configure address without performing DAD */
      if (!IPV6_TIME_EXPIRED(SatEntryPtr->PreferredTime))
      {
        SatEntryPtr->State         = IPV6_LADDR_STATE_PREFERRED;
      }
      else
      {
        SatEntryPtr->State         = IPV6_LADDR_STATE_DEPRECATED;
      }
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
      if (0 < IpV6_VCfgGetDadTransmits()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
      {
        SatEntryPtr->DadNsTxRemain = 0;
      }
#endif
    }

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    if (0 < IpV6_VCfgGetDadTransmits()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      if (0 < SatEntryPtr->DadNsTxRemain)
      {
        /* Set next event time for this address entry so that it will expire after DelayMs milliseconds.
           The delay will defer transmit of the first Neighbor Solicitation and sould be randomly chosen by the caller.
           See [RFC4861 5.4.2. Sending Neighbor Solicitation Messages] */
        IPV6_SET_EVENT_TIME_MS(SatEntryPtr->NextEventTime, DelayMs);

      }
      else
      {
        SatEntryPtr->NextEventTime = SatEntryPtr->PreferredTime;
      }

      /* If DAD is active and the address is in TENTATIVE state, it is not ready for use right now.
         IpV6_VHandleSourceAddrAssignmentChange() will be called from IpV6_Ndp_VSATCheckLifetimes() after DAD has been completed. */
      if (IPV6_LADDR_STATE_TENTATIVE != SatEntryPtr->State)
      {
        /* Notify upper layers that new address is available for use */
        IpV6_VHandleSourceAddrAssignmentChange(IpCtrlIdx, CtrlAddrIdx, TRUE);
      }
    }
    else
#endif
    { /* PRQA S 3201 */ /* MD_IpV6_3201 */
      /* Notify upper layers that new address is available for use */
      SatEntryPtr->NextEventTime = SatEntryPtr->PreferredTime;
      IpV6_VHandleSourceAddrAssignmentChange(IpCtrlIdx, CtrlAddrIdx, TRUE);
    }

    Result = E_OK;
  }
  else
  {
    /* ignore unconfigured address with zero lifetime */
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VAddSourceAddress() */ /* PRQA S 6010, 6030, 6060 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_PAR */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDRLCheckLifetimes()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDRLCheckLifetimes(uint8 IpCtrlIdx)
{
  uint8 EntryIdx;

  IPV6_BEGIN_CRITICAL_SECTION_DRL(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Send configured number of Router Solicitations while no default router is known. */
#if (IPV6_ENABLE_ROUTER_SOLICITATIONS == STD_ON)
  if (0 < IpV6_VCfgGetMaxRtrSolicitations()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    /* We start sending Router Solicitations until at least one default router has been discovered (rx of Router Advertisement)
     * or the maximum number of Router Solicitations has been sent.
     */

    if (0 == IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount))
    {
      /* no default routers known */

      if (IPV6_CTRL_VAR(IpV6_Ndp_RouterSolicitationTxCount) < IpV6_VCfgGetMaxRtrSolicitations())
      {
        /* router solicitations sending active */

        /* #20 Check if timeout for next Router Solicitation has expired. */
        if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_Ndp_RouterSolicitationNextTxTime)))
        {
          boolean IncludeSllaOpt;
          IpV6_AddrIdType AddrId;

          /* #30 Check if Source Link-Layer Option must be included in Router Solicitation. */
          if (IPV6_CTRL_STATE_LLADDR_READY <= IPV6_CTRL_VAR(IpV6_CtrlState))
          {
            /* IPV6_STATE_LLADDR_READY indicates that a valid link-local address has been stored in entry 0 of
             * the SourceAddressTable.
             */

#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
            if (IpV6_VCfgIsOptimisticDadEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
            {
              /* "A node MUST NOT send a Router Solicitation with a SLLAO from an Optimistic Address.
               *  Router Solicitations SHOULD be sent from a non-Optimistic or the Unspecified Address; however,
               *  they MAY be sent from an Optimistic Address as long as the SLLAO is not included."
               *  [RFC4429 3.2. Modifications to RFC 2461 Neighbor Discovery]
               */
              IncludeSllaOpt = (boolean)(IPV6_LADDR_STATE_OPTIMISTIC != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].State);
            }
            else
#endif
            { /* PRQA S 3201 */ /* MD_IpV6_3201 */
              IncludeSllaOpt = TRUE;
            }
          }
          else
          {
            /* use the unspecified address as source address for RS, since we do not have a valid address */
            IncludeSllaOpt = FALSE;
          }

          AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].IpAddrId;

          /* #40 Send one Router Solicitation and update timeout for next Router Solicitation. */
          if (E_OK == IpV6_Ndp_VTxRouterSolicitation(&AddrId, IncludeSllaOpt))
          {
            IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Ndp_RouterSolicitationNextTxTime), IpV6_VCfgGetRtrSolicitationInterval());
            IPV6_CTRL_VAR(IpV6_Ndp_RouterSolicitationTxCount)++;
          }
        }
      }
    }
  }
#endif

  /* #50 Remove default routers from list if their lifetime has expired. */
  EntryIdx = IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount);

  /* check expiration times of all entrys */
  while (0 < EntryIdx)
  {
    EntryIdx--;

    /* remove default router entry if lifetime has expired */
    if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].ExpireTime))
    {
      IpV6_Ndp_VDCRemoveEntrys(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].NextHopAddress);
      IpV6_Ndp_VDRLRemoveEntry(IpCtrlIdx, EntryIdx);
    }
  }

  IPV6_END_CRITICAL_SECTION_DRL(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* End of IpV6_Ndp_VDRLCheckLifetimes() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCRemoveEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VNCRemoveEntry(
  uint8            IpCtrlIdx,
  IpV6_ListIdxType EntryIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_NC_IDX(EntryIdx),    IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* check if index is valid */
  if (EntryIdx < IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount))
  {
    IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount)--;

    /* copy last entry at the position of the removed entry to avoid stale entrys */
    if (EntryIdx != IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount))
    {
      IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx] = IPV6_CTRL_VAR(IpV6_NeighborCache)[IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount)];
    }
  }
  IPV6_END_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* End of IpV6_Ndp_VNCRemoveEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCCheckLifetimes()
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
 *
 *
 *
 *
 *
 *
 */
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VNCCheckLifetimes(uint8 IpCtrlIdx)
{
  uint8 NcEntryIdx;
  uint8 DcEntryidx;
  IpV6_NeighborCacheEntryType *EntryPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Iterate over all entries in the Neighbor Cache. */
  NcEntryIdx = IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount);

  while (0 < NcEntryIdx)
  {
    NcEntryIdx--;

    EntryPtr = &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx];

    if (IPV6_REACHABLE_STATE_INCOMPLETE != IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State)
    {
      /* #20 Send pending Neighbor Advertisement. */
      if (TRUE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaPending)
      {
        IpV6_AddrIdType AddrId = IPV6_CTRL_VAR(IpV6_BcAddrId);

        /* try to transmit pending neighbor advertisement */
        if (E_OK == IpV6_Ndp_VTxNeighborAdvertisement(&AddrId, &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NeighborAddress, &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaTgtAddress, IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaRSOFlags))
        {
          IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaPending = FALSE;
        }
      }

#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
      /* #30 Send pending Inverse Neighbor Advertisement. */
      if (IpV6_VCfgIsNdpInvNsEnabled() && (TRUE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].InvNaPending)) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
      {
        /* try to transmit pending inverse advertisement */
        IpV6_AddrIdType AddrId = IPV6_CTRL_VAR(IpV6_BcAddrId);

        if (E_OK == IpV6_Ndp_VTxInverseAdvertisement(&AddrId, &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].InvNsSrcAddress, (IPV6_P2C(Eth_PhysAddrType))&IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0])) /* PRQA S 0310 */ /* MD_IpV6_0310 */
        {
          IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].InvNaPending = FALSE;
        }
      }
#endif
    }

    switch (IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State)
    {
    case IPV6_REACHABLE_STATE_INCOMPLETE: 
      /* #40 Send pending Neighbor Solicitations if entry is in state INCOMPLETE */

      /* #50 Check if timeout for next Neighbor Solicitation has expired. */
      if (IPV6_TIME_EXPIRED(EntryPtr->NextEventTime)) /* timeout expired? */
      {
        /* #60 Send one Neighbor Solicitation if number of retries has not exceeded.  */
        if (EntryPtr->RetryCount < IpV6_VCfgGetMulticastSolicits())
        {
          /* Send NS */
          IpV6_AddrIdType AddrId;

          if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY != EntryPtr->NsSrcAddressIdx)
          {
            AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[EntryPtr->NsSrcAddressIdx].IpAddrId;
          }
          else
          {
            AddrId = IPV6_CTRL_VAR(IpV6_BcAddrId);
          }

          IPV6_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          if (E_OK == IpV6_Ndp_VTxNeigborSolicitation(&AddrId, &EntryPtr->NeighborAddress, FALSE, FALSE))
          {
            IPV6_SET_EVENT_TIME_MS(EntryPtr->NextEventTime, IPV6_CTRL_VAR(IpV6_RetransTimerMs));
            EntryPtr->RetryCount++;
          }

          IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
        /* #70 ELSE: Handle failed link layer address resolution. */
        else
        {
          /* address resolution failed! */
          /* #80 Remove all entries from Destination Cache that are using the unreachable neighbor as next hop and inform upper layers. */
          IPV6_BEGIN_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          DcEntryidx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);

          while (0 < DcEntryidx)
          {
            DcEntryidx--;

            /* remove this entry if the next hop address matches address in NextHopAddrPtr */
            if (IPV6_ADDR_EQUALS(EntryPtr->NeighborAddress, IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryidx].NextHopAddress))
            {
              IpV6_Ndp_VDestinationUnreachableCbks(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryidx].DestinationAddress);
              IpV6_Ndp_VDCRemoveEntry(IpCtrlIdx, DcEntryidx);
            }
          }

          IPV6_END_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          /* #90 Remove entry from neighbor Cache. */
          IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, NULL_PTR);
          IpV6_Ndp_VNCRemoveEntry(IpCtrlIdx, NcEntryIdx);
        }
      }

      break;

#if (IPV6_ENABLE_NDP_NUD == STD_ON)

    case IPV6_REACHABLE_STATE_DELAY:
      /* #100 Set entry state from DELAY to PROBE if probe delay has expired. */

      if (IPV6_TIME_EXPIRED(EntryPtr->NextEventTime)) /* probe delay expired? */
      {
        /* setting the entry state to PROBE will trigger probing this neighbor next time this function is called */
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_PROBE);
      }

      break;

    case IPV6_REACHABLE_STATE_PROBE:
      /* #110 Perform Neighbor Unreachability Detection (NUD) if entry is in state PROBE.  */

      if (IPV6_TIME_EXPIRED(EntryPtr->NextEventTime)) /* retrans delay expired? */
      {
        /* #120 Send one NUD Probe if configured number of probes has not been sent yet. */
        if (EntryPtr->RetryCount < IpV6_VCfgGetUnicastSolicits())
        {
          /* Send NUD probe NS */
          IpV6_AddrIdType AddrId;

          if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY != EntryPtr->NsSrcAddressIdx)
          {
            AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[EntryPtr->NsSrcAddressIdx].IpAddrId;
          }
          else
          {
            AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].IpAddrId;
          }

          IPV6_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          if (E_OK == IpV6_Ndp_VTxNeigborSolicitation(&AddrId, &EntryPtr->NeighborAddress, TRUE, FALSE))
          {
            IPV6_SET_EVENT_TIME_MS(EntryPtr->NextEventTime, IPV6_CTRL_VAR(IpV6_RetransTimerMs));
            EntryPtr->RetryCount++;
          }

          IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
        /* #130 ELSE: Handle unreachable Neighbor. */
        else
        {
          /* #140 Remove all entries from Destination Cache that are using the unreachable neighbor as next hop. */
          IpV6_Ndp_VDCRemoveEntrys(IpCtrlIdx, &EntryPtr->NeighborAddress);

          /* #150 Remove entry from Neighbor Cache. */
          IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, NULL_PTR);
          IpV6_Ndp_VNCRemoveEntry(IpCtrlIdx, NcEntryIdx);

          /* Remove all entries from destination cache that are using this neighbor as next hop */
          IpV6_Ndp_VDCRemoveEntrys(IpCtrlIdx, &EntryPtr->NeighborAddress);
        }
      }

      break;

    case IPV6_REACHABLE_STATE_REACHABLE:
      /* #160 Set entry state from REACHABLE to STALE if reachable time has expired. */
      /* expiration check may be omitted here, it is sufficent when an expired entry is set to stale when a packet is transmitted */

      if (IPV6_TIME_EXPIRED(EntryPtr->NextEventTime)) /* entry expired? */
      {
        /* change entry state to stale because no recent reachable confirmation has been received */
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
      }

      break;

#endif

    default: /* last possible case is IPV6_REACHABLE_STATE_STALE: */
      /* stale entrys are not modified until a packet should be sent */
      break;
    }
  }

  IPV6_END_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* End of IpV6_Ndp_VNCCheckLifetimes() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_Ndp_VChangeLocalAddrState()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VChangeLocalAddrState(
  IpV6_AddrIdType       AddrId,
  IpV6_LocAddrStateType NewState)
{
  uint8                IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    if (0 < IpV6_VCfgGetDadTransmits()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      if ((IPV6_LADDR_STATE_TENTATIVE == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State) && (IPV6_LADDR_STATE_TENTATIVE != NewState))
      {
        /* A TENTATIVE address has become valid. Notify upper layers. */
        IpV6_VHandleSourceAddrAssignmentChange(IpCtrlIdx, CtrlAddrIdx, TRUE);
      }
    }
#endif

    IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State = NewState;

    switch (NewState)
    {
      case IPV6_LADDR_STATE_PREFERRED:
        IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].NextEventTime = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].PreferredTime;
        break;

      default: /* case IPV6_LADDR_STATE_DEPRECATED: */
        IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].NextEventTime = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].ValidTime;
        break;
    }

    /* Invalidate all source addresses in Destination Cache. */
    IpV6_Ndp_VDCInvalidateSrcAddrs(IpCtrlIdx);
  }
  else
  {
    /* ERROR: This function may not be called with IpAddrIds referencing multicast addresses! */
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  }

} /* End of IpV6_Ndp_VChangeLocalAddrState() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VSetAddrStateExpired()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VSetAddrStateExpired(IpV6_AddrIdType AddrId)
{
  uint8                IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    IpV6_Ndp_VDCInvalidateSrcAddrs(IpCtrlIdx);

    IpV6_VHandleSourceAddrAssignmentChange(IpCtrlIdx, CtrlAddrIdx, FALSE);

    IpV6_VSATRemoveEntry(IpCtrlIdx, CtrlAddrIdx);
  }
  else
  {
    /* ERROR: This function may not be called with IpAddrIds referencing multicast addresses! */
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  }

} /* End of IpV6_Ndp_VSetAddrStateExpired() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VSATCheckLifetimes()
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
 */
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VSATCheckLifetimes(uint8 IpCtrlIdx)
{
  IpV6_CtrlAddrIdxType CtrlAddrIdx;
  
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListIdxType TempAddrIdx;
#endif

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Iterate over all entries in the Source Address Table. */

  CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

  while (0 < CtrlAddrIdx)
  {
    CtrlAddrIdx--;

    /* #20 Skip invalid/unused entries. */ 
    if (IPV6_LADDR_STATE_INVALID != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      IpV6_SourceAddressTableEntryType *SatEntryPtr = &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx];

      /* #30 Check if timeout for the entry has expired and action is required. */
      if (IPV6_TIME_EXPIRED(SatEntryPtr->NextEventTime))
      {
        /* #40 Change address state from PREFERRED to DEPRECATED. */
        if (IPV6_LADDR_STATE_PREFERRED == SatEntryPtr->State)
        {
          /* the preferred lifetime for this address has been expired.
           * This address is still valid but now deprecated for usage with new connections.
           * Source address selection will not select this address if preferred addresses exist.
           */
          IpV6_Ndp_VChangeLocalAddrState(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId, IPV6_LADDR_STATE_DEPRECATED);
        }
        /* #50 Remove address from Source Address Table if lifetime has expired. */
        else if (IPV6_LADDR_STATE_DEPRECATED == SatEntryPtr->State)
        {
          /* Valid lifetime of this deprecated address has been expired.
           * Remove this source address and notify upper layers.
           */
          IpV6_Ndp_VSetAddrStateExpired(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId);
        }
        /* #60 Perform Duplicate Address Detection (DAD) for addresses in state TENTATIVE or OPTIMISTIC. */
        else
        {
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
          if (0 < IpV6_VCfgGetDadTransmits()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
          {
            /* IPV6_LADDR_STATE_TENTATIVE or IPV6_LADDR_STATE_OPTIMISTIC */

            if (0 < SatEntryPtr->DadNsTxRemain)
            {
              /* #70 Send one DAD Neighbor Solicitation message and update next event time for this address. */
              IpV6_AddrIdType AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].IpAddrId;

              IPV6_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              if (E_OK == IpV6_Ndp_VTxNeigborSolicitation(&AddrId, &SatEntryPtr->Address, FALSE, TRUE))
              {
                SatEntryPtr->DadNsTxRemain--;
                IPV6_SET_EVENT_TIME_MS(SatEntryPtr->NextEventTime, IPV6_CTRL_VAR(IpV6_RetransTimerMs));  /* PRQA S 0715 */ /* MD_IpV6_0715 */

              }

              IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            }
            /* #80 Change address state to PREFERRED if configured number of DAD NS messages have been sent. */
            else
            {
              /* DAD for TENTATIVE or OPTIMISTIC address complete */
              /* this tentative address has not become invalid after performing duplicate address detection */
              IpV6_Ndp_VChangeLocalAddrState(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId, IPV6_LADDR_STATE_PREFERRED);
            }
          }
#endif
        }
      }

      /* #90 Handle Temporary Addresses (Privacy Extensions). */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      if (IpV6_VCfgHasPrivExtConfig())
      {
        TempAddrIdx = SatEntryPtr->TempAddrs.FirstIdx;

        while (IPV6_DLIST_END_IDX != TempAddrIdx)
        {
          IpV6_DListIdxType NextTempAddrIdx = IPV6_DLIST_GET_NEXT(TempAddrIdx, SatEntryPtr->TempAddrs);

          if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime)) /* entry expired? */
          {
            switch (IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent)
            {
            case IPV6_LADDR_EVENT_TO_PREFERRED:
              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].State = IPV6_LADDR_STATE_PREFERRED;

              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent = IPV6_LADDR_EVENT_TEMP_ADDR_REGEN;
              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime;
              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime.S -= IpV6_VCfgGetPrivExtRegenAdvance();
              IpV6_Ndp_VDCInvalidateSrcAddrs(IpCtrlIdx);
              break;

            case IPV6_LADDR_EVENT_TO_DEPRECATED:
              /* the preferred lifetime for this address has been expired.
               * This address is still valid but now deprecated for usage with new connections.
               * Source address selection will not select this address if preferred addresses exist.
               */
              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].State = IPV6_LADDR_STATE_DEPRECATED;

              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent = IPV6_LADDR_EVENT_EXPIRE;
              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].ValidTime;
              IpV6_Ndp_VDCInvalidateSrcAddrs(IpCtrlIdx);
              break;

            case IPV6_LADDR_EVENT_EXPIRE:
              /* Valid lifetime of this deprecated address has been expired.
               * Remove this source address and notify upper layers.
               */
              IpV6_DListRemoveElement(&SatEntryPtr->TempAddrs, TempAddrIdx);
              IpV6_Ndp_VDCInvalidateSrcAddrs(IpCtrlIdx);
              break;

            case IPV6_LADDR_EVENT_TEMP_ADDR_REGEN:

              IpV6_Ndp_VPrivExtConfigureTempAddr(IpCtrlIdx, SatEntryPtr);

              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent = IPV6_LADDR_EVENT_TO_DEPRECATED;
              IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime;

              IpV6_Ndp_VDCInvalidateSrcAddrs(IpCtrlIdx);
              break;

# if (IPV6_ENABLE_NDP_DAD == STD_ON)
            case IPV6_LADDR_EVENT_TX_DAD:
              if (0 < IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadNsTxRemain)
              {
                IpV6_AddrIdType AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].IpAddrId;

                IPV6_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                if (E_OK == IpV6_Ndp_VTxNeigborSolicitation(&AddrId, &IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress, FALSE, TRUE))
                {
                  IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadNsTxRemain--;

                  IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent = IPV6_LADDR_EVENT_TO_PREFERRED;
                  /* PRQA S 0715 1 */ /* MD_IpV6_0715 */
                  IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime, IPV6_CTRL_VAR(IpV6_RetransTimerMs));
                }

                IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              }
              break;
# endif

            default:
              /* should not happen */
              IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
              break;
            }
          }

          TempAddrIdx = NextTempAddrIdx;
        }
      }
#endif
    }
  }

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* End of IpV6_Ndp_VSATCheckLifetimes() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_Ndp_VDefaultRouterUpdate()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VDefaultRouterUpdate(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  uint16                  Lifetime)
{
  uint8 EntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,            IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (E_OK == IpV6_Ndp_VDRLLookup(IpCtrlIdx, SrcAddrPtr, &EntryIdx))
  {
    if (0 < Lifetime)
    {
      IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].ExpireTime, Lifetime, NULL_PTR);
    }
    else /* lifetime expired */
    {
      IpV6_Ndp_VDRLRemoveEntry(IpCtrlIdx, EntryIdx);

      /* we have to remove all entrys from the destination cache using the default router */

      IpV6_Ndp_VDCRemoveEntrys(IpCtrlIdx, SrcAddrPtr);
    }
  }
  else /* router not in default router list */
  {
    if (0 < Lifetime)
    {
      if (E_OK == IpV6_Ndp_VDRLCreateEntry(IpCtrlIdx, SrcAddrPtr, &EntryIdx))
      {
        IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].ExpireTime, Lifetime, NULL_PTR);
      }
      else /* no space left in default router list */
      {
        /* we have reached the maximum number of enries in the default router list, this router will be ignored */

      }
    }
    else /* lifetime expired */
    {
      /* ignore router information */
    }
  }
} /* End of IpV6_Ndp_VDefaultRouterUpdate() */

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VPrivExtConfigureTempAddr()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VPrivExtConfigureTempAddr(
  uint8                                      IpCtrlIdx,
  IPV6_P2V(IpV6_SourceAddressTableEntryType) SatEntryPtr)
{
  uint32 TempPreferredLifetime;
  uint32 TempValidLifetime;

  /* [RFC4941 3.3. Generating Temporary Addresses / 3.] */
  /* [RFC4941 3.3. Generating Temporary Addresses / 4.] */

  if (IpV6_Time.S < SatEntryPtr->PreferredTime.S)
  {
    TempPreferredLifetime = SatEntryPtr->PreferredTime.S - IpV6_Time.S;

    if (TempPreferredLifetime > IPV6_PRIVEXT_VAR(PrivExtTempPreferredLifetime))
    {
      TempPreferredLifetime = IPV6_PRIVEXT_VAR(PrivExtTempPreferredLifetime);
    }
  }
  else
  {
    TempPreferredLifetime = 0;
  }

  if (IpV6_Time.S < SatEntryPtr->ValidTime.S)
  {
    TempValidLifetime = SatEntryPtr->ValidTime.S - IpV6_Time.S;

    if (TempValidLifetime > IpV6_VCfgGetPrivExtTempValidLifetime())
    {
      TempValidLifetime = IpV6_VCfgGetPrivExtTempValidLifetime();
    }
  }
  else
  {
    TempValidLifetime = 0;
  }

  if (IpV6_VCfgGetPrivExtRegenAdvance() < TempPreferredLifetime)
  {
    IpV6_DListIdxType TempAddrIdx;

    /* [RFC4941 3.3. Generating Temporary Addresses 5.] */
    if (E_OK == IpV6_DListAddElement(&SatEntryPtr->TempAddrs, IPV6_DLIST_END_IDX, &TempAddrIdx))
    {
      /* insert new temporary address at the beginning */

      /* build temporary address from global prefix and current temporary interface identifier */
      IpV6_VBuildSourceAddress(&IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0], &SatEntryPtr->Address, &IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress);

      /* initialize address lifetimes */
      IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].CreationTime);
      IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime, TempPreferredLifetime, &IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime);
      IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].ValidTime,     TempValidLifetime,     &IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime);

      /* initialize DAD parameters */
# if (IPV6_ENABLE_NDP_DAD == STD_ON)
      if (0 < IpV6_VCfgGetDadTransmits()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
      {
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].State            = IPV6_LADDR_STATE_TENTATIVE;
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadNsTxRemain    = IpV6_VCfgGetDadTransmits();
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadRetriesRemain = IpV6_VCfgGetPrivExtTempIdGenRetries();
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent        = IPV6_LADDR_EVENT_TX_DAD;
        IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime, 0);
      }
      else
# endif
      { /* PRQA S 3201 */ /* MD_IpV6_3201 */
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].State            = IPV6_LADDR_STATE_PREFERRED;
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent        = IPV6_LADDR_EVENT_TO_DEPRECATED;
        IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime    = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime;
      }
    }
    else
    {
      /* no space left in temporary address list */
      IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_DLIST_BUFFER_EMPTY);
    }
  }
  else
  {
    /* "A temporary address is created only if this calculated Preferred
      *  Lifetime is greater than REGEN_ADVANCE time units. In particular,
      *  an implementation MUST NOT create a temporary address with a zero
      *  Preferred Lifetime."
      *  [RFC4941 3.3. Generating Temporary Addresses / 5.]
      */
  }
} /* End of IpV6_Ndp_VPrivExtConfigureTempAddr() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VPrivExtExtendTempAddrLifetimes()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VPrivExtExtendTempAddrLifetimes(
  uint8                                      IpCtrlIdx,
  IPV6_P2C(IpV6_SourceAddressTableEntryType) SatEntryPtr)
{
  IpV6_DListIdxType TempAddrIdx;

  /* iterate over all temporary address entries and extend their lifetimes */
  TempAddrIdx = SatEntryPtr->TempAddrs.FirstIdx;

  while (IPV6_DLIST_END_IDX != TempAddrIdx)
  {
    IpV6_TimeType MaxNewLifetime;

    /* PREFERRED LIFETIME */

    IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime = SatEntryPtr->PreferredTime;

    MaxNewLifetime = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].CreationTime;
    MaxNewLifetime.S += IpV6_VCfgGetPrivExtTempPreferredLifetime();

    if (IPV6_TIME_IS_LOWER(MaxNewLifetime, IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime))
    {
      IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime = MaxNewLifetime;
    }

    /* VALID LIFETIME */

    IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].ValidTime = SatEntryPtr->ValidTime;

    MaxNewLifetime = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].CreationTime;
    MaxNewLifetime.S += IpV6_VCfgGetPrivExtTempValidLifetime();

    if (IPV6_TIME_IS_LOWER(MaxNewLifetime, IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].ValidTime))
    {
      IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].ValidTime = MaxNewLifetime;
    }

    IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].PreferredTime;

    IPV6_DLIST_SET_NEXT(TempAddrIdx, SatEntryPtr->TempAddrs);
  }
} /* End of IpV6_Ndp_VPrivExtExtendTempAddrLifetimes() */
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_VSlaacRxPrefixInformation()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VSlaacRxPrefixInformation(
  uint8 IpCtrlIdx,
  P2CONST(IpV6_NdpOptPrefixInfoType, AUTOMATIC, IPV6_APPL_DATA) PrefixInfoPtr,
  boolean IsMulticastRa)
{
  IpV6_CtrlAddrIdxType CtrlAddrIdx;
  uint32               ValidLifetime;     /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint32               PreferredLifetime; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint32               RemainingLifetime;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != PrefixInfoPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if ((IPV6_PREFIX_LEN_BYTE * IPV6_BITS_IN_BYTE) != PrefixInfoPtr->PrefixLength)
  {
    /* unsupported prefix length */
    return;
  }

  if (!IPV6_ADDR_IS_EUI64(PrefixInfoPtr->Prefix))
  {
    /* ignore prefix */
    return;
  }

  ValidLifetime     = IPV6_NTOHL(PrefixInfoPtr->ValidLifetimeNbo);
  PreferredLifetime = IPV6_NTOHL(PrefixInfoPtr->PreferredLifetimeNbo);

  /* ignore this prefix if preferred lifetime is bigger that valid lifetime */
  if (PreferredLifetime > ValidLifetime)
  {
    return;
  }

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  CtrlAddrIdx = 0; /* IpV6_VSATLookupPrefix() needs initialized index */

  /* lookup all addresses with advertised prefix that are already in source address list */
  if (E_OK == IpV6_VSATLookupPrefix(IpCtrlIdx, &PrefixInfoPtr->Prefix.addr[0], &CtrlAddrIdx, IPV6_ADDR_CFG_SRC_RA))
  {
    IpV6_SourceAddressTableEntryType *SatEntryPtr = &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx];

    /* update preferred lifetime */
    IPV6_SET_LIFETIME_S(SatEntryPtr->PreferredTime, PreferredLifetime, &SatEntryPtr->NextEventTime);

    if (IpV6_Time.S < SatEntryPtr->ValidTime.S)
    {
      RemainingLifetime = (SatEntryPtr->ValidTime.S - IpV6_Time.S);
    }
    else
    {
      RemainingLifetime = 0;
    }

    /* prevent quick invalidation of addresses by prefix information options with very small lifetimes (see RFC4862 5.5.3. e) */
    if ((ValidLifetime > RemainingLifetime) || (ValidLifetime > IpV6_VCfgGetSlaacMinLifetime()))
    {
      IPV6_SET_LIFETIME_S(SatEntryPtr->ValidTime, ValidLifetime, &SatEntryPtr->NextEventTime);
    }
    else if (RemainingLifetime > IpV6_VCfgGetSlaacMinLifetime())
    {
      /* New lifetime is smaller than current remaining lifetime and smaller than IPV6_NDP_SLAAC_MIN_VALID_LIFETIME
        * Truncate current lifetime to minimum valid lifetime.
        */
      IPV6_SET_LIFETIME_S(SatEntryPtr->ValidTime, IpV6_VCfgGetSlaacMinLifetime(), &SatEntryPtr->NextEventTime);
    }
    else
    {
      /* Do not update valid lifetime.
        * The remaining lifetime for this address is already less that IPV6_NDP_SLAAC_MIN_VALID_LIFETIME seconds.
        * Setting a shorter lifetime value here would make denail-of-service attacks easier.
        */
    }

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (IpV6_VCfgHasPrivExtConfig() && (IpV6_VCfgIsPrivExtUseOnLinkPrefixes() || (0 == (PrefixInfoPtr->LAReserved & IPV6_NDP_OPT_PREFIX_INFO_FLAG_ON_LINK)))) /* PRQA S 3355 */ /* MD_IpV6_3355 */ /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      /* prefix is not marked as beeing on-link, so privacy extensions should be applied */
      IpV6_Ndp_VPrivExtExtendTempAddrLifetimes(IpCtrlIdx, SatEntryPtr); /*lint !e522 */ /* This function has side-effects! (modifies global variables) */
    }
#endif
  }
  else
  {
    /* no address configured for this prefix so far */

    if ((0 < PreferredLifetime) && (0 < ValidLifetime))
    {
      /* create new source address entry for this prefix */
      IpV6_AddrType        SrcAddrCandidate; /* PRQA S 0759 */ /* MD_IpV6_0759 */
      boolean              FreeCtrlAddrIdxFound = FALSE;

      /* locate free source address entry for configuration source RA */
      CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

      while (0 < CtrlAddrIdx)
      {
        CtrlAddrIdx--;

        if (IPV6_ADDR_CFG_SRC_RA == IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].AddressAssignVariant)
        {
          if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
          {
            /* free address container for configuration source RA found */
            FreeCtrlAddrIdxFound = TRUE;
            break;
          }
        }
      }

      if (TRUE == FreeCtrlAddrIdxFound)
      {
        IpV6_AddrIdType AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId;

        /* a prefix must have a specific length so that in combination with the interface identifier the address is 128 bytes long */
        /* combine prefix and interface identifier to an address */
        IpV6_VBuildSourceAddress(&IPV6_CTRL_VAR(IpV6_InterfaceIdentifier)[0], &PrefixInfoPtr->Prefix, &SrcAddrCandidate);

#if (IPV6_ENABLE_SLAAAC_DELAY == STD_ON)
        if (IpV6_VCfgIsSlaacDelayEnabled() && (TRUE == IsMulticastRa)) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
        {
          /* The RA was sent to a multicast destination, so we SHOULD wait a random delay before
            * starting address configuration.
            * "The delay will avoid similar congestion when multiple nodes are going to configure
            *  addresses by receiving the same single multicast router advertisement."
            *  [RFC4862 5.4.2. Sending Neighbor Solicitation Messages]
            */
          uint32 DelayMs = IpV6_VGetRandomValue(0, IpV6_VCfgGetMaxSlaacDelay());
          (void)IpV6_Ndp_VAddSourceAddress(AddrId, &SrcAddrCandidate, PreferredLifetime, ValidLifetime, DelayMs, IPV6_SRC_ADDR_DAD_MODE_OPTIMISTIC);
        }
        else
#endif
        { /* PRQA S 3201 */ /* MD_IpV6_3201 */
          (void)IpV6_Ndp_VAddSourceAddress(AddrId, &SrcAddrCandidate, PreferredLifetime, ValidLifetime, 0, IPV6_SRC_ADDR_DAD_MODE_OPTIMISTIC);
        }

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
        if (IpV6_VCfgHasPrivExtConfig() && (IpV6_VCfgIsPrivExtUseOnLinkPrefixes() || (0 == (PrefixInfoPtr->LAReserved & IPV6_NDP_OPT_PREFIX_INFO_FLAG_ON_LINK)))) /* PRQA S 3355 */ /* MD_IpV6_3355 */ /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
        {
          /* prefix is not marked as beeing on-link, so privacy extensions should be applied */
          IpV6_Ndp_VPrivExtConfigureTempAddr(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx]);
        }
#endif
      }
      else
      {
        /* no free source address table entry for configuration source RA found */
      }
    }
    else /* prefix information expired */
    {
      /* we have no address configured for this expired prefix. ignore information */
    }
  }

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VSlaacRxPrefixInformation() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_Ndp_VProcessPrefixInformation()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VProcessPrefixInformation(
  uint8                               IpCtrlIdx,
  IPV6_P2C(IpV6_NdpOptPrefixInfoType) PrefixInfoPtr,
  boolean                             IsMulticastRa)
{
  uint8 EntryIdx;
  uint32 ValidLifetime; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != PrefixInfoPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (IPV6_ADDR_IS_LINK_LOCAL(PrefixInfoPtr->Prefix) || IPV6_ADDR_IS_MULTICAST(PrefixInfoPtr->Prefix))
  {
    /* ignore link-local and multicast prefixes */
    return;
  }

  ValidLifetime = IPV6_NTOHL(PrefixInfoPtr->ValidLifetimeNbo);

  /* PROCESS ON-LINK FLAG */
  if (0 != (PrefixInfoPtr->LAReserved & IPV6_NDP_OPT_PREFIX_INFO_FLAG_ON_LINK)) /* on-link prefix? */
  {
    if (IpV6_VIsStaticOnLinkPrefix(IpCtrlIdx, &PrefixInfoPtr->Prefix, PrefixInfoPtr->PrefixLength) == TRUE)
    {
      /* This prefix or a prefix that covers this prefix is statically configured. Ignore prefix information. */
    }
    else
    {
      /* Check if this prefix is already known. */
      if (E_OK == IpV6_Ndp_VPLLookup(IpCtrlIdx, &PrefixInfoPtr->Prefix, PrefixInfoPtr->PrefixLength, &EntryIdx))
      {
        if (0 < ValidLifetime)
        {
          /* extend lifetime */
          IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_PrefixList)[EntryIdx].ExpireTime, ValidLifetime, (P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_VAR))NULL_PTR);
        }
        else
        {
          /* lifetime expired, remove prefix entry */
          IpV6_Ndp_VPLRemoveEntry(IpCtrlIdx, EntryIdx);
        }
      }
      else /* prefix unknown */
      {
        /* create prefix entry if lifetime not expired */
        if (0 < ValidLifetime)
        {
          if (E_OK == IpV6_Ndp_VPLGetOrCreateEntry(IpCtrlIdx, &PrefixInfoPtr->Prefix, PrefixInfoPtr->PrefixLength, &EntryIdx))
          {
            IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_PrefixList)[EntryIdx].ExpireTime, ValidLifetime, (P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_VAR))NULL_PTR);
          }
        }
        else /* prefix lifetime expired */
        {
          /* ignore prefix information */
        }
      }
    }
  }
  else /* no on-link information */
  {
    /* nothing to to here */
  }

  /* PROCESS SLAAC FLAG */
  if (0 != (PrefixInfoPtr->LAReserved & IPV6_NDP_OPT_PREFIX_INFO_FLAG_SLAAC)) /* autoconfiguration */
  {
    IpV6_Ndp_VSlaacRxPrefixInformation(IpCtrlIdx, PrefixInfoPtr, IsMulticastRa);
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV6_Ndp_VProcessPrefixInformation() */ /* PRQA S 6080 */ /* MS_MSR_STMIF */

#if (IPV6_ENABLE_RFC6106_RDNSS_OPT == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VProcessRdnssOption()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VProcessRdnssOption(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr)
{
  P2CONST(IpV6_NdpOptRdnssType, AUTOMATIC, IPV6_APPL_DATA) RdnssOptPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  RdnssOptPtr = (P2CONST(IpV6_NdpOptRdnssType, AUTOMATIC, IPV6_APPL_DATA))DataPtr;

  if (3 <= RdnssOptPtr->Length)
  {
    /* "The number of addresses is determined by the Length field.
     *  That is, the number of addresses is equal to (Length - 1) / 2."
     */
    uint8 AddrCount = (RdnssOptPtr->Length - 1) >> IPV6_RSHIFT_DIV_2;

    Dns_UpdateIpV6Servers((IpBase_AddrIn6Type*)&DataPtr[sizeof(IpV6_NdpOptRdnssType)], AddrCount, IPV6_NTOHL(RdnssOptPtr->LifetimeNbo), 0);
  }
  else
  {
    /* minimum value of length field is 3 */
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV6_Ndp_VProcessRdnssOption() */
#endif

#if (IPV6_ENABLE_RFC6106_DNSSL_OPT == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VProcessDnsslOption()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_Ndp_VProcessDnsslOption(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) DataPtr)
{
  P2CONST(IpV6_NdpOptDnsslType, AUTOMATIC, IPV6_APPL_DATA) DnsslOptPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  DnsslOptPtr = (P2CONST(IpV6_NdpOptDnsslType, AUTOMATIC, IPV6_APPL_DATA))DataPtr;  /* PRQA S 0310 */ /* MD_IpV6_0310 */

  if (2 <= DnsslOptPtr->Length)
  {
    /* message length in bytes: 8 * (1 + AddrCount * 2) | AddrCount: Number of IPv6 addresses */
    uint16 DomainNameListLength = (uint16)(DnsslOptPtr->Length - 1) << IPV6_LSHIFT_MUL_8;
    uint16 DomainNameListEndPos = (DnsslOptPtr->Length << IPV6_LSHIFT_MUL_8) - 1;
    uint16 ZeroByteCount = 0;

    /* remove padding */
    while ((DomainNameListEndPos > sizeof(IpV6_NdpOptDnsslType)) && (0 == DataPtr[DomainNameListEndPos]))
    {
      ZeroByteCount++;
      DomainNameListEndPos--;
    }

    if (0 < ZeroByteCount)
    {
      DomainNameListLength -= (ZeroByteCount - 1);

      /* TODO: */
    }
  }
  else
  {
    /* minumum value of length field is 2 */
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV6_Ndp_VProcessDnsslOption() */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxRouterAdvertisement()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VRxRouterAdvertisement(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte)
{
  uint8                                                     IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType                                      CtrlAddrIdx;
  P2CONST(IpV6_NdpRaType, AUTOMATIC, IPV6_APPL_DATA)        NdpRaPtr;
  uint16                                                    DataOffset; /* PRQA S 781 */ /* MD_IpV6_1.1 */
  uint8                                                     OptType;
  uint16                                                    OptLenByte;
  P2CONST(IpV6_NdpOptLLAddrType, AUTOMATIC, IPV6_APPL_DATA) SourceLinkLayerAddressOptPtr = (P2CONST(IpV6_NdpOptLLAddrType, AUTOMATIC, IPV6_APPL_DATA))NULL_PTR;
  IpV6_ListIdxType NcEntryIdx;
  boolean                                                   IsMulticastRa;
#if (IPV6_ENABLE_NDP_NUD == STD_ON)
# if (IPV6_ENABLE_DYN_REACHABLE_TIME == STD_ON)
  uint32                                                    RaReachableTime;
# endif
#endif

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  /* source address of router advertisement must always be the link-local address of the router */
  if (IPV6_ADDR_TYPE_LL_UNICAST != IpV6_VGetAddrType(SrcAddrPtr))
  {
    return; /* silently ignore invalid router advertisement */
  }

  if (E_OK != IpV6_Ndp_VValidateOptions(&DataPtr[sizeof(IpV6_NdpRaType)], (uint16)(LenByte - sizeof(IpV6_NdpRaType))))
  {
    return; /* silently ignore invalid router advertisement */
  }

  /* received valid router advertisement */

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  IsMulticastRa = (boolean)!IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx);

  NdpRaPtr = (P2CONST(IpV6_NdpRaType, AUTOMATIC, IPV6_APPL_DATA))DataPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if (IPV6_ENABLE_DHCP == STD_ON)
  if ((IPV6_DHCPV6_MODE_AUTOMATIC == IpV6_VCfgGetDhcpMode()) && (FALSE == TcpIp_DhcpV6_IsActive(IpCtrlIdx))) /* PRQA S 3415 */ /* MD_IpV6_12.4 */
  {
    if (0 != (NdpRaPtr->MOReserved & IPV6_NDP_RA_FLAG_MANAGED))
    {
      /* Managed address configuration flag set.
       * Addresses are availabe via DHCPv6.
       */

      if (FALSE == TcpIp_DhcpV6_IsActive(IpCtrlIdx))
      {
        if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IpV6_VCfgGetDhcpCtrlAddrIdx()].State)
        {
          (void)TcpIp_DhcpV6_StartAddressAssignment(IpCtrlIdx);
        }
      }
    }
    else if (0 != (NdpRaPtr->MOReserved & IPV6_NDP_RA_FLAG_OTHER))
    {
      /* Other configuration flag set.
       * Other configuration information is available via DHCPv6.
       */

      if (FALSE == TcpIp_DhcpV6_IsActive(IpCtrlIdx))
      {
        (void)TcpIp_DhcpV6_RequestInformation(IpCtrlIdx);
      }
    }
    else
    {
      /* "Managed address configuration" and "Other configuration" flags not set */
    }
  }
#endif

#if (IPV6_ENABLE_DYN_HOP_LIMIT == STD_ON)
  if (IpV6_VCfgIsDynHopLimitEnabled())
  {
    /* "If the received Cur Hop Limit value is non-zero, the host SHOULD set
        its CurHopLimit variable to the received value."
        [RFC4861 6.3.4.] */

    if (0 < NdpRaPtr->CurHopLimit)
    {
      IPV6_CTRL_VAR(IpV6_CurHopLimit) = NdpRaPtr->CurHopLimit;
    }
  }
#endif

#if (IPV6_ENABLE_NDP_NUD == STD_ON)
# if (IPV6_ENABLE_DYN_REACHABLE_TIME == STD_ON)
  if (IpV6_VCfgIsDynamicReachableTimeEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    /* "If the received Reachable Time value is non-zero, the host SHOULD set
        its BaseReachableTime variable to the received value."
        [RFC4861 6.3.4.] */

    RaReachableTime = IPV6_HTONL(NdpRaPtr->ReachableTimeNbo);

    if (0 < RaReachableTime)
    {
#  if (IPV6_ENABLE_RND_REACHABLE_TIME == STD_ON)
      if (IpV6_VCfgIsRandomReachableTimeEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
      {
        /* Behaviour according to [RFC4861 6.3.4.]:
           "ReachableTime is computed as a uniformly distributed random value between
            MIN_RANDOM_FACTOR and MAX_RANDOM_FACTOR times the BaseReachableTime.
            Using a random component eliminates the possibility that Neighbor Unreachability
            Detection messages will synchronize with each other." */

        if (RaReachableTime != IPV6_CTRL_VAR(IpV6_BaseReachableTimeMs))
        {
          /* reachable time changed */
          IPV6_CTRL_VAR(IpV6_BaseReachableTimeMs) = RaReachableTime;

          IPV6_CTRL_VAR(IpV6_ReachableTimeMs) = IpV6_VRandomizeValue(IpCtrlIdx, IPV6_CTRL_VAR(IpV6_BaseReachableTimeMs));

        }
      }
      else
#  endif
      { /* PRQA S 3201 */ /* MD_IpV6_3201 */
        /* do not randomize reachable time */

        if (RaReachableTime != IPV6_CTRL_VAR(IpV6_ReachableTimeMs))
        {
          /* reachable time changed */
          IPV6_CTRL_VAR(IpV6_ReachableTimeMs) = RaReachableTime;
        }
      }
    }
  }
# endif
#endif

#if (IPV6_ENABLE_DYN_RETRANS_TIMER == STD_ON)
  if (IpV6_VCfgIsDynamicRetransTimerEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    /* "The RetransTimer variable SHOULD be copied from the Retrans Timer
        field, if the received value is non-zero."
        [RFC4861 6.3.4.] */

    if (0 < IPV6_NTOHL(NdpRaPtr->RetransTimerNbo))
    {
      IPV6_CTRL_VAR(IpV6_RetransTimerMs) = IPV6_NTOHL(NdpRaPtr->RetransTimerNbo);
    }
  }
#endif

  /* update content of default router list */
  IpV6_Ndp_VDefaultRouterUpdate(IpCtrlIdx, SrcAddrPtr, IPV6_NTOHS(NdpRaPtr->RouterLifetimeNbo));

  DataOffset = sizeof(IpV6_NdpRaType);

  /* process TLV encoded options */

  while (DataOffset < LenByte)
  {
    OptType = DataPtr[DataOffset];
    OptLenByte = 8 * DataPtr[DataOffset+1];

    switch (OptType)
    {
    /* OPTION: Source Link-Layer Address */
    case IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR:

      SourceLinkLayerAddressOptPtr = (P2CONST(IpV6_NdpOptLLAddrType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

      if (IPV6_LL_ADDR_IS_INVALID(SourceLinkLayerAddressOptPtr->addr))
      {
        /* ignore invalid source link-layer address option */
        SourceLinkLayerAddressOptPtr = NULL_PTR;
      }

      break;

    /* OPTION: Prefix Information */
    case IPV6_NDP_OPT_TYPE_PREFIX_INFO: /* network prefix */

      if (sizeof(IpV6_NdpOptPrefixInfoType) == OptLenByte)
      {
        IpV6_Ndp_VProcessPrefixInformation(IpCtrlIdx, (P2CONST(IpV6_NdpOptPrefixInfoType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[DataOffset], IsMulticastRa); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      }
      else
      {
        /* invalid Prefix Information option length */
      }

      break;

    /* OPTION: MTU */
    case IPV6_NDP_OPT_TYPE_MTU:

      if (sizeof(IpV6_NdpOptMtuType) == OptLenByte)
      {
#if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
        if (IpV6_VCfgIsLinkMtuReconfigurationEnabled())
        {
          P2CONST(IpV6_NdpOptMtuType, AUTOMATIC, IPV6_APPL_DATA) MtuOptPtr;
          uint32 MtuValue;

          MtuOptPtr = (P2CONST(IpV6_NdpOptMtuType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

          MtuValue = IPV6_NTOHL(MtuOptPtr->MTUNbo);

          if (0xFFFFU >= MtuValue)
          {
            IpV6_VUpdateDefaultLinkMtu(IpCtrlIdx, (uint16)MtuValue);
          }
        }
#endif
      }
      else
      {
        /* invalid MTU option length */
      }

      break;

#if (IPV6_ENABLE_RFC6106_RDNSS_OPT == STD_ON)

    /* OPTION: RDNSS */
    case IPV6_NDP_OPT_TYPE_RDNSS:

      /* [RFC6106 5.1. Recursive DNS Server Option] */

      IpV6_Ndp_VProcessRdnssOption(&DataPtr[DataOffset]);
      break;
#endif

#if (IPV6_ENABLE_RFC6106_DNSSL_OPT == STD_ON)

    /* OPTION: DNSSL */
    case IPV6_NDP_OPT_TYPE_DNSSL:

      /* [RFC6106 5.2. DNS Search List Option] */

      IpV6_Ndp_VProcessDnsslOption(&DataPtr[DataOffset]);
      break;
#endif

    /* OPTION: UNKNOWN */
    default:
      /* ignore this option */
      break;
    }

    DataOffset += OptLenByte; /* skip to next option */
  }

  /* update router entry in neighbor cache if link-layer address has changed */

  IPV6_BEGIN_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (E_OK == IpV6_Ndp_VNCLookup(IpCtrlIdx, SrcAddrPtr, &NcEntryIdx))
  {
    IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter = TRUE;

    if (NULL_PTR != SourceLinkLayerAddressOptPtr) /* source link-layer address included? */
    {
      if ((IPV6_REACHABLE_STATE_INCOMPLETE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State) || /* PRQA S 3415 */ /* MD_IpV6_3415 */
        (FALSE == TcpIp_CmpLLAddr(&SourceLinkLayerAddressOptPtr->addr[0], &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0]))) /* different link-layer address? */
      {
        IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, &SourceLinkLayerAddressOptPtr->addr[0]);
        IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State = IPV6_REACHABLE_STATE_STALE;
      }
    }
  }
#if (IPV6_NDP_ENABLE_LL_ADDR_GATHERING == STD_ON)
  else /* no neighbor cache entry exists */
  {
    if (NULL_PTR != SourceLinkLayerAddressOptPtr) /* source link-layer address included? */
    {
      IpV6_Ndp_VNCGetOrCreateEntry(IpCtrlIdx, SrcAddrPtr, &NcEntryIdx);

      IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, &SourceLinkLayerAddressOptPtr->addr[0]);
      IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State = IPV6_REACHABLE_STATE_STALE;
      IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter = TRUE;
    }
  }
#endif

  IPV6_END_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VRxRouterAdvertisement() */ /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxNeighborSolicitation()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VRxNeighborSolicitation(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         PhysAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte)
{
  uint8                    IpCtrlIdx;      /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint16                   DataOffset;
  uint16                   OptLenByte;
  IPV6_P2C(IpV6_NdpNsType) NdpNsPtr;
  IpV6_AddrTypeType        SrcAddrType;
  IpV6_LocAddrStateType    MatchingSrcAddrState;
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListIdxType        SatTempAddrIdx;
#endif
  IpV6_ListIdxType         NcEntryIdx;
  IpV6_CtrlAddrIdxType     CtrlAddrIdxTgt; /* controller address index of target address in neighbor solicitation */
  IPV6_P2C(uint8)          SrcLinkLayerAddrPtr = (IPV6_P2C(uint8))NULL_PTR;
  uint8                    ResponseNaRSOFlags = 0;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  NdpNsPtr = (P2CONST(IpV6_NdpNsType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  SrcAddrType = IpV6_VGetAddrType(SrcAddrPtr);

  /* validation checks (RFC4861 7.1.1) */

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  /* block used for limitation of variable scope */
  {
    IpV6_CtrlAddrIdxType CtrlAddrIdx;
    IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

    if (IPV6_ADDR_TYPE_UNSPECIFIED == SrcAddrType)
    {
      /* ip destination address must be a solicited node address */
      if ((IPV6_CTRL_ADDR_IDX_INV_OR_ANY != CtrlAddrIdx) || (E_OK != IpV6_VMatchPrefix(IPV6_CTRL_VAR(IpV6_LastBcAddrPtr), &IpV6_PrefixSolicitedNode.Prefix.addr[0], IpV6_PrefixSolicitedNode.PrefixLenBits))) /* PRQA S 3415 */ /* MD_IpV6_12.4 */
      {
        return; /* silently ignore invalid message */
      }
    }
  }

  if (E_OK != IpV6_Ndp_VValidateOptions(&DataPtr[sizeof(IpV6_NdpNsType)], (uint16)(LenByte - sizeof(IpV6_NdpNsType))))
  {
    return; /* silently discard invalid solicitation */
  }

  /* process options */

  DataOffset = sizeof(IpV6_NdpNsType);

  while (DataOffset < LenByte)
  {
    OptLenByte = DataPtr[DataOffset+1] * 8;

    switch (DataPtr[DataOffset])
    {
      /* OPTION: Source Link-Layer Address */
      case IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR:

        SrcLinkLayerAddrPtr = &DataPtr[DataOffset+2];

        if (IPV6_LL_ADDR_IS_INVALID(SrcLinkLayerAddrPtr))
        {
          /* ignore invalid source link-layer address */
          SrcLinkLayerAddrPtr = NULL_PTR;
        }

        break;

      default:
        break;
    }

    DataOffset += OptLenByte; /* skip to next option */
  }

  if ((IPV6_ADDR_TYPE_UNSPECIFIED == SrcAddrType) && (NULL_PTR != SrcLinkLayerAddrPtr))
  {
    /* The Source Link-Layer Address option MUST NOT be included if the source address is
     * the unspecified address.
     */
    return; /* discard the packet */
  }

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (E_OK != IpV6_VSATLookup(IpCtrlIdx, &NdpNsPtr->TargetAddress, &CtrlAddrIdxTgt, &SatTempAddrIdx))
#else
  if (E_OK != IpV6_VSATLookup(IpCtrlIdx, &NdpNsPtr->TargetAddress, &CtrlAddrIdxTgt))
#endif
  {
    /* neighbor solicitation for other node */
    return; /* silently ignore this message */
  }

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (IPV6_DLIST_END_IDX != SatTempAddrIdx)
  {
    MatchingSrcAddrState = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[SatTempAddrIdx].State;
  }
  else
#endif
  {
    MatchingSrcAddrState = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].State;
  }

  if ((IPV6_LADDR_STATE_PREFERRED  == MatchingSrcAddrState) ||
      (IPV6_LADDR_STATE_DEPRECATED == MatchingSrcAddrState))
  {
    /* solicitation for valid local address */
    if (IPV6_ADDR_TYPE_UNSPECIFIED == SrcAddrType)
    {
      /* Sender of the solicitation is performing DAD for one of our source addresses. Send answer... */

      IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).TargetAddr, NdpNsPtr->TargetAddress); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).RSOFlags = IPV6_NDP_NA_FLAG_OVERRIDE;
      IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).Pending  = TRUE;
    }
    else
    {
      ResponseNaRSOFlags = IPV6_NDP_NA_FLAG_SOLICITED | IPV6_NDP_NA_FLAG_OVERRIDE;
    }
  }
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
# if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
  else if (IPV6_LADDR_STATE_OPTIMISTIC == MatchingSrcAddrState)
  {
    /* solicitation for optimistic address (Optimistic DAD) */
    /* [RFC4429 3.3 Modifications to RFC 2462 Stateless Address Autoconfiguration]
      * modifies [RFC4862 5.4.3. Receiving Neighbor Solicitation Messages]
      */
    if (IPV6_ADDR_TYPE_UNSPECIFIED == SrcAddrType)
    {
      /* "The node MUST NOT reply to a Neighbor Solicitation for an Optimistic Address
        *  from the unspecified address. Receipt of such an NS indicates that the address
        *  is a duplicate, and it MUST be deconfigured as per the behaviour specified in
        *  RFC 2462 for Tentative addresses."
        */
      IpV6_VHandleDADFailure(PhysAddrPtr, IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdxTgt].IpAddrId, IPV6_DAD_FAIL_NS);
      IpV6_VSATRemoveEntry(IpCtrlIdx, CtrlAddrIdxTgt);
    }
    else
    {
      /* "The node MUST reply to a Neighbor Solicitation for an Optimistic Address from
        *  a unicast address, but the reply MUST have the Override flag cleared (O=0)."
        */
      ResponseNaRSOFlags = IPV6_NDP_NA_FLAG_SOLICITED;
    }
  }
# endif
  else /* if (IPV6_LADDR_STATE_TENTATIVE == MatchingSrcAddrState) */
  {
    /* solicitation for tentative address SLAAC (RFC4862 5.4.3) */

    if (IPV6_ADDR_TYPE_UNICAST == SrcAddrType)
    {
      /* the sender of this solicitation is performing address resolution for tentative address, we MUST NOT send a response */
    }
    else if (IPV6_ADDR_TYPE_UNSPECIFIED == SrcAddrType)
    {
      /* someone else is performig DAD for this tentative address.
       * If link-layer loops back packets it colud be ourself otherwise someone else also wants to use this address.
       */

# if (IPV6_MULTICAST_LOOPBACK == STD_ON)
      if (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].DadNsRxCount < (IPV6_NDP_DUP_ADDR_DETECT_TRANSMITS - IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].DadNsTxReamin))
      {
        IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].DadNsRxCount++;
      }
      else
# endif
      {
# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
        if (IpV6_VCfgHasPrivExtConfig() && (IPV6_DLIST_END_IDX != SatTempAddrIdx))
        {
          /* DAD for temporary address failed */
          if (E_OK != IpV6_VHandleTempAddrDADFailure(IpCtrlIdx, CtrlAddrIdxTgt, SatTempAddrIdx, IPV6_DAD_FAIL_NS))
          {
            /* deconfigure all temporary addresses for this prefix */
            IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].TempAddrs);
          }
        }
        else
# endif
        { /* PRQA S 3201 */ /* MD_IpV6_3201 */
          /* DAD for non-temporary address failed */
          IpV6_VHandleDADFailure(PhysAddrPtr, IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdxTgt].IpAddrId, IPV6_DAD_FAIL_NS);
          IpV6_VSATRemoveEntry(IpCtrlIdx, CtrlAddrIdxTgt);
        }
      }
    }
    else
    {
      /* nothing to do here */
    }
  }
#endif

  if ((ResponseNaRSOFlags & IPV6_NDP_NA_FLAG_SOLICITED) != 0)
  {
    /* schedule sending of solicited Neighbor Advertisement */

    /* NS for valid local address (RFC4861 7.2.3) */
    /* create or update neighbor entry and store link-layer address of sender */
    IpV6_Ndp_VNCGetOrCreateEntry(IpCtrlIdx, SrcAddrPtr, &NcEntryIdx);

    if (NULL_PTR != SrcLinkLayerAddrPtr)
    {
      /* record link layer address, if contained in NS */
      if ((IPV6_REACHABLE_STATE_INCOMPLETE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State) || /* PRQA S 3415 */ /* MD_IpV6_3415 */
          (FALSE == TcpIp_CmpLLAddr(&IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0], SrcLinkLayerAddrPtr)))
      {
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
        IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, SrcLinkLayerAddrPtr);
      }
    }

    IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaTgtAddress, NdpNsPtr->TargetAddress); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaRSOFlags = ResponseNaRSOFlags;
    IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NaPending  = TRUE;
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VRxNeighborSolicitation() */ /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxNeighborAdvertisement()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VRxNeighborAdvertisement(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         PhysAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte)
{
  uint8                                                IpCtrlIdx;                 /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  P2CONST(IpV6_NdpNaType, AUTOMATIC, IPV6_APPL_DATA)   NaHdrPtr;
  IpV6_NdpNaFlagsType                                  Flags = { 0, 0, 0, 0, 0 }; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint16                                               DataOffset;
  uint16                                               OptLenByte;
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA)            TgtLLAddrPtr = (P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA))NULL_PTR;
  uint8                                                NcEntryIdx;
  IpV6_CtrlAddrIdxType                                 CtrlAddrIdxTgt; /* controller address index of the target address in the neighbor solicitation */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListIdxType                                    SatTempAddrIdx;
#endif
  IpV6_ListIdxType                                     DrlEntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#else
  IPV6_UNUSED_PARAMETER(SrcAddrPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  NaHdrPtr = (P2CONST(IpV6_NdpNaType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[0];  /* PRQA S 0310 */ /* MD_IpV6_0310 */

  /* perform message validation */

  /* ignore message if target address is multicast (RFC4861 7.1.2) */
  if (IPV6_ADDR_TYPE_MULTICAST == IpV6_VGetAddrType(&NaHdrPtr->TargetAddress))
  {
    return; /* ignore invalid message */
  }

  Flags.Router    = (boolean)((0 != (NaHdrPtr->RSOReserved & IPV6_NDP_NA_FLAG_ROUTER))    ? TRUE : FALSE);
  Flags.Solicited = (boolean)((0 != (NaHdrPtr->RSOReserved & IPV6_NDP_NA_FLAG_SOLICITED)) ? TRUE : FALSE);
  Flags.Override  = (boolean)((0 != (NaHdrPtr->RSOReserved & IPV6_NDP_NA_FLAG_OVERRIDE))  ? TRUE : FALSE);
  Flags.SrcLLAddrChanged = FALSE;

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  /* block used for limitation of variable scope */
  {
    IpV6_CtrlAddrIdxType CtrlAddrIdx;
    IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

    /* ignore message if destination address is multicast and the solicited falg is set (RFC4861 7.1.2) */
    if ((!IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx)) && (1 == Flags.Solicited))
    {
      return; /* ignore invalid message */
    }
  }

  if (E_OK != IpV6_Ndp_VValidateOptions(&DataPtr[sizeof(IpV6_NdpNsType)], (uint16)(LenByte - sizeof(IpV6_NdpNsType))))
  {
    return; /* silently discard neighbor advertisement with invalid options */
  }

  /* Valid Neighbor Advertisement received */

  /* DAD */

  /* Neighbor advertisements should not have a target address, we use */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (E_OK == IpV6_VSATLookup(IpCtrlIdx, &NaHdrPtr->TargetAddress, &CtrlAddrIdxTgt, &SatTempAddrIdx))
#else
  if (E_OK == IpV6_VSATLookup(IpCtrlIdx, &NaHdrPtr->TargetAddress, &CtrlAddrIdxTgt))
#endif
  {
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    IpV6_LocAddrStateType MatchingSrcAddrState;

# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (IPV6_DLIST_END_IDX != SatTempAddrIdx)
    {
      MatchingSrcAddrState = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[SatTempAddrIdx].State;
    }
    else
# endif
    {
      MatchingSrcAddrState = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].State;
    }

    if ((IPV6_LADDR_STATE_PREFERRED  != MatchingSrcAddrState) &&
        (IPV6_LADDR_STATE_DEPRECATED != MatchingSrcAddrState))
    {
      /* DAD fail. tentative address is not unique! we must not use it! */
# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      if (IpV6_VCfgHasPrivExtConfig() && (IPV6_DLIST_END_IDX != SatTempAddrIdx))
      {
        /* DAD for temporary address failed */
        if (E_OK != IpV6_VHandleTempAddrDADFailure(IpCtrlIdx, CtrlAddrIdxTgt, SatTempAddrIdx, IPV6_DAD_FAIL_NA))
        {
          /* deconfigure all temporary addresses for this prefix */
          IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdxTgt].TempAddrs);
        }
      }
      else
# endif
      { /* PRQA S 3201 */ /* MD_IpV6_3201 */
        /* DAD for non-temporary address failed */
        IpV6_VHandleDADFailure(PhysAddrPtr, IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdxTgt].IpAddrId, IPV6_DAD_FAIL_NA);
        IpV6_VSATRemoveEntry(IpCtrlIdx, CtrlAddrIdxTgt);
      }
    }
    else
#endif
    {
      /* Someone has sent a neighbor advertisement with an address which is used by us */
      /* #-- Handle duplicate address. This handling includes PhysAddr comparison! */
      TcpIp_VDuplicateAddrDetected(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdxTgt].IpAddrId), &NaHdrPtr->TargetAddress.addr[0], PhysAddrPtr);

    }

    return; /* Neighbor advertisement for own ip address received */
  }

  /* End DAD */

  if (E_OK != IpV6_Ndp_VNCLookup(IpCtrlIdx, &NaHdrPtr->TargetAddress, &NcEntryIdx)) /* no neighbor cache entry exists? */
  {
    /* no entry found in neighbor cache, ignore the advertisement (RFC4861 7.2.5)*/
    return;
  }

  DataOffset = sizeof(IpV6_NdpNsType);

  while (DataOffset < LenByte)
  {
    OptLenByte = DataPtr[DataOffset+1] * 8;

    switch (DataPtr[DataOffset])
    {
      /* OPTION: Target Link-Layer Address */
      case IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR:

        TgtLLAddrPtr = &DataPtr[DataOffset+2];

        if (IPV6_LL_ADDR_IS_INVALID(TgtLLAddrPtr))
        {
          /* ignore invalid target link-layer address */
          TgtLLAddrPtr = NULL_PTR;
        }
      break;

      /* OPTION: UNKNOWN */
      default:
        break;
    }

    DataOffset += OptLenByte; /* skip to next option */
  }

  /* end of processing TLV options */

  if ((NULL_PTR != TgtLLAddrPtr) && /* PRQA S 3415 */ /* MD_IpV6_3415 */
      (FALSE == TcpIp_CmpLLAddr(TgtLLAddrPtr, &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0])))
  {
    Flags.TgtLLAddrChanged = 1;
  }
  else
  {
    Flags.TgtLLAddrChanged = 0;
  }

  if (IPV6_REACHABLE_STATE_INCOMPLETE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State)
  {
      if (NULL_PTR == TgtLLAddrPtr) /* no target link-layer address? */
      {
        /* we don't have any link-layer address of the node so far,
           since this advertisement does not contain one, it is useless */
        /* silently discard the advertisement */
        return;
      }

      /* record link-layer address */
      IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, TgtLLAddrPtr);

      if (1 == Flags.Solicited) /* solicited? */
      {
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_REACHABLE);
      }
      else /* not solicited */
      {
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
      }

      if (1 == Flags.Router) /* IsRouter flag set? */
      {
        IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter = TRUE;
      }

      /* NOTE: the override flag is ignored if the entry is in incomplete state */

      /* If there are any queued packets for this neighbor, trigger sending.
       * Since we do not use packets queues currently nothig has do be done here.
       */
  }
  else /* other state than INCOMPLETE */
  {
    if (0 == Flags.Override) /* not override? */
    {
      if (1 == Flags.TgtLLAddrChanged) /* different link-layer address than cached I. */
      {
        if (IPV6_REACHABLE_STATE_REACHABLE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State) /* a. */
        {
          /* set entry to stale but ignore changed link-layer address */
          IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
        }
        else /* other state b. */
        {
          /* ignore the advertisement */
        }
      }
    }

    if ((1 == Flags.Override) || (0 == Flags.TgtLLAddrChanged)) /* II. */
    {
      if ((1 == Flags.TgtLLAddrChanged) && (NULL_PTR != TgtLLAddrPtr))
      {
        /* record link-layer address (if changed) */
        IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, TgtLLAddrPtr);
      }

      if (1 == Flags.Solicited) /* solicited? */
      {
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_REACHABLE);
      }
      else if (1 == Flags.TgtLLAddrChanged) /* not solicited, but different link-layer address */
      {
        IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
      }
      else /* not solicited, no changed target link-layer address */
      {
        /* entry state remains uncahnged */
      }

      if (1 == Flags.Router) /* IsRouter? */
      {
        if (FALSE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter)
        {
          /* host has become a router. just update the flag in the cache */
          IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter = TRUE;
        }
      }
      else /* not IsRouter? */
      {
        if (TRUE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter)
        {
          /* host is no router anymore. */
          IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter = FALSE;

          /*  remove the host from the router list */
          if (E_OK == IpV6_Ndp_VDRLLookup(IpCtrlIdx, &NaHdrPtr->TargetAddress, &DrlEntryIdx))
          {
            IpV6_Ndp_VDRLRemoveEntry(IpCtrlIdx, DrlEntryIdx);
          }

          /* update all destination cache entries using this neighbor as a router */
          IpV6_Ndp_VDCRemoveEntrys(IpCtrlIdx, &NaHdrPtr->TargetAddress);
        }
      }
    }
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VRxNeighborAdvertisement() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxRedirect()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VRxRedirect(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte)
{
  uint8                       IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IPV6_P2C(IpV6_NdpRedirType) RedirMsg;
  IpV6_ListIdxType            DcEntryIdx;
  IpV6_ListIdxType            NcEntryIdx;
  Std_ReturnType              TargetEqualsDestination;
  uint16                      DataOffset;
  uint16                      OptLenByte;
  IPV6_P2C(uint8)             TgtLLAddrPtr = (IPV6_P2C(uint8))NULL_PTR;
  Std_ReturnType              DcLookupResult;
  IPV6_P2C(IpV6_AddrType)     NextHopAddrPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  /* perform message validation (see RFC4861 8.1) */

  /* routers must always send redirects from their link-local address */
  if (!IPV6_ADDR_IS_LINK_LOCAL(*SrcAddrPtr))
  {
    return; /* silently ignore invalid redirect */
  }

  RedirMsg = (P2CONST(IpV6_NdpRedirType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  /* "The IP source address of the Redirect [must be] the same as the current
      first-hop router for the specified ICMP Destination Address." */

  /* get the address of the first-hop router for the destination */

  DcLookupResult = IpV6_Ndp_VDCLookup(IpCtrlIdx, &RedirMsg->DestinationAddress, &DcEntryIdx);

  if (E_OK == DcLookupResult)
  {
    /* we have a destination cache entry with the first-hop router address */
    NextHopAddrPtr = &IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress;
  }
  else
  {
    boolean DefaultRouterReachable;
    /* we do not have an entry in the destination cache. */

    /* determine first-hop router address. */

    if (TRUE == IpV6_VIsDestinationOnLink(IpCtrlIdx, &RedirMsg->DestinationAddress))
    {
      /* ignore this redirect since the destination address is on-link */
      return;
    }

    NextHopAddrPtr = IpV6_VGetDefaultRouterAddr(IpCtrlIdx, &RedirMsg->DestinationAddress, &DefaultRouterReachable);

    if (NULL_PTR == NextHopAddrPtr)
    {
      /* no default router is known, we have to ignore this redirect */
      return;
    }
  }

  /* check if redirect was sent by first-hop router for this destination */

  if (!IPV6_ADDR_EQUALS(*SrcAddrPtr, *NextHopAddrPtr))
  {
    /* the redirect doesn't came from the router we use as next hop for the destination address */
    return; /* silently ignore invalid redirect */
  }

  /* the destination address must not be a multicast address */
  if (IPV6_ADDR_IS_MULTICAST(RedirMsg->DestinationAddress))
  {
    return; /* silently ignore redirect with multicast destination address */
  }

  TargetEqualsDestination = IpV6_CmpNetAddr(&RedirMsg->TargetAddress, &RedirMsg->DestinationAddress);

  /* the target address (our new next hop for the destination) must be the link-local address
   * of a router, or the same as the destination address which indicats redirect to on-link destination (neighbor) */
  if ((!IPV6_ADDR_IS_LINK_LOCAL(RedirMsg->TargetAddress)) && (!TargetEqualsDestination))
  {
    return; /* silently ignore invalid redirect */
  }

  if (E_OK != IpV6_Ndp_VValidateOptions(&DataPtr[sizeof(IpV6_NdpRedirType)], (uint16)(LenByte - sizeof(IpV6_NdpRedirType))))
  {
    return; /* silently ignore redirect with invalid options */
  }

  /* valid redirect received */

  /* process TLV options */

  DataOffset = sizeof(IpV6_NdpRedirType);

  while (DataOffset < LenByte)
  {
    OptLenByte = DataPtr[DataOffset+1] * 8;

    switch (DataPtr[DataOffset])
    {
      /* OPTION: Target Link-Layer Address */
      case IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR:

        TgtLLAddrPtr = &DataPtr[DataOffset+2];

        if (IPV6_LL_ADDR_IS_INVALID(TgtLLAddrPtr))
        {
          /* ignore invalid source link-layer address */
          TgtLLAddrPtr = NULL_PTR;
        }
      break;

      /* OPTION: Redirected Header */
      case IPV6_NDP_OPT_TYPE_REDIR_HEADER:

        /* TODO: Process this option? */

      break;

      /* OPTION: UNKNOWN */
      default:
        break;
        /* skip unkown options */
    }

    DataOffset += OptLenByte; /* skip to next option */
  }

  /* end of processing TLV options */

  /* update next hop address of destination cache entry */

  if (E_OK != DcLookupResult)
  {
    IpV6_Ndp_VDCGetOrCreateEntry(IpCtrlIdx, &RedirMsg->DestinationAddress, &DcEntryIdx);
  }

  IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress, RedirMsg->TargetAddress); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  /* if a link-layer address for the target (the next hop) was provied, update or create a neighbor cache
   * entry, if the link-layer address is not known already */
  if (NULL_PTR  != TgtLLAddrPtr)
  {

    if (E_OK == IpV6_Ndp_VNCLookup(IpCtrlIdx, &RedirMsg->TargetAddress, &NcEntryIdx))
    {
      if ((IPV6_REACHABLE_STATE_INCOMPLETE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State) || /* PRQA S 3415 */ /* MD_IpV6_3415 */
          (FALSE == TcpIp_CmpLLAddr(&IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0], TgtLLAddrPtr)))
      {
        IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, TgtLLAddrPtr);
        IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State = IPV6_REACHABLE_STATE_STALE;
      }
      else
      {
        /* link-layer already cached */
      }
    }
    else
    {
      IpV6_Ndp_VNCGetOrCreateEntry(IpCtrlIdx, &RedirMsg->TargetAddress, &NcEntryIdx);

      IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, TgtLLAddrPtr);
      IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State = IPV6_REACHABLE_STATE_STALE;
    }

    if (!TargetEqualsDestination)
    {
      /* different target and destination addresses indicate, that the target is a router */
      IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].IsRouter = TRUE;
    }
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VRxRedirect() */ /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VRxInverseSolicitation()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VRxInverseSolicitation(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte)
{
  uint8                           IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint16                          DataOffset;
  IPV6_P2C(IpV6_NdpOptLLAddrType) SrcLLAddrOptPtr   = (IPV6_P2C(IpV6_NdpOptLLAddrType))NULL_PTR;
  IPV6_P2C(IpV6_NdpOptLLAddrType) TgtLLAddrOptPtr   = (IPV6_P2C(IpV6_NdpOptLLAddrType))NULL_PTR;
  IPV6_P2C(IpV6_AddrType)         SrcAddrListPtr    = (IPV6_P2C(IpV6_AddrType))NULL_PTR;
  IPV6_P2C(IpV6_NdpOptMtuType)    MtuOptPtr         = (IPV6_P2C(IpV6_NdpOptMtuType))NULL_PTR;
  uint8                           SrcAddrCount      = 0;
  IpV6_ListIdxType                NcEntryIdx;
  Eth_PhysAddrType                ThisTgtLLAddr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  DataOffset = 4; /* ignore and skip 32bit reserved filed */

  /* NDP option valiadtion roules also apply to [RFC3122] */
  if (E_OK != IpV6_Ndp_VValidateOptions(&DataPtr[DataOffset], LenByte - DataOffset))
  {
    return; /* silently discard inverse solicitation with invalid options */
  }

  while (DataOffset < LenByte)
  {
    uint8  OptType = DataPtr[DataOffset];
    uint16 OptLenByte = IPV6_NDP_CALC_OPTION_LENGTH(DataPtr[DataOffset+1]);

    switch (OptType)
    {
      case IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR:
        /* mandatory */
        SrcLLAddrOptPtr = (IPV6_P2C(IpV6_NdpOptLLAddrType))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        if (IPV6_LL_ADDR_IS_INVALID(SrcLLAddrOptPtr->addr))
        {
          /* ignore invalid source link layer address option */
          SrcLLAddrOptPtr = NULL_PTR;
        }
      break;

      case IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR:
        /* mandatory */
        TgtLLAddrOptPtr = (IPV6_P2C(IpV6_NdpOptLLAddrType))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        if (IPV6_LL_ADDR_IS_INVALID(TgtLLAddrOptPtr->addr))
        {
          /* ignore invalid target link layer address option */
          TgtLLAddrOptPtr = NULL_PTR;
        }
      break;

      case IPV6_NDP_OPT_TYPE_SOURCE_ADDR_LIST:
        /* optional */
        {
          uint16 AddrListLenByte = (uint16)(OptLenByte - sizeof(IpV6_NdpOptAddrListType));

          if ((0 < AddrListLenByte) && (0 == (AddrListLenByte % IPV6_ADDRESS_LEN_BYTE)))
          {
            SrcAddrCount   = (uint8)(AddrListLenByte / IPV6_ADDRESS_LEN_BYTE);
            SrcAddrListPtr = (IPV6_P2C(IpV6_AddrType))&DataPtr[DataOffset + sizeof(IpV6_NdpOptAddrListType)]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
          }
        }
      break;

      case IPV6_NDP_OPT_TYPE_MTU:
        /* optional */
        MtuOptPtr = (IPV6_P2C(IpV6_NdpOptMtuType))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
      break;

      default:
        /* silently ignore unknown option */
      break;
    }

    DataOffset += OptLenByte;
  }

  if ((NULL_PTR == SrcLLAddrOptPtr) || (NULL_PTR == TgtLLAddrOptPtr))
  {
    /* mandatory option missing. silently ignore invalid message */
    return;
  }

  TCPIP_LL_GetPhysAddr(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), &ThisTgtLLAddr[0]);

  if (FALSE == TcpIp_CmpLLAddr(&TgtLLAddrOptPtr->addr[0], &ThisTgtLLAddr[0]))
  {
    /* ignore solicitation sent to different target */
    return;
  }

  if (NULL_PTR != SrcAddrListPtr)
  {
    /* optional Source Address List Option present */
    /* notify upper layers */
#if (0 < IPV6_IND_ADDR_LIST_RECEIVED_CBK_COUNT)
    IpV6_IndAddrListReceivedCbks(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), (IPV6_P2C(Eth_PhysAddrType))&SrcLLAddrOptPtr->addr[0], SrcAddrListPtr, SrcAddrCount); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif
  }

  if (NULL_PTR != MtuOptPtr)
  {
    /* optional MTU Option present */
  }

  IpV6_Ndp_VNCGetOrCreateEntry(IpCtrlIdx, SrcAddrPtr, &NcEntryIdx);

  /* record link layer address, contained in IS */
  if ((IPV6_REACHABLE_STATE_INCOMPLETE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State) || /* PRQA S 3415 */ /* MD_IpV6_3415 */
     (FALSE == TcpIp_CmpLLAddr(&IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0], SrcLLAddrOptPtr->addr)))
  {
    IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
    IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, SrcLLAddrOptPtr->addr);
  }

  IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].InvNsSrcAddress, *SrcAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].InvNaPending  = TRUE;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VRxInverseSolicitation() */ /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  IpV6_Ndp_VTxInverseAdvertisement()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxInverseAdvertisement(
  IPV6_P2V(IpV6_AddrIdType)  SrcAddrIdPtr,
  IPV6_P2C(IpV6_AddrType)    DstAddrPtr,
  IPV6_P2C(Eth_PhysAddrType) TgtLLAddrPtr)
{
  uint8                           IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  TcpIp_IpTxRequestDescriptorType IpTxReqDesc;
  IPV6_P2V(uint8)                 IcmpBufPtr;
  uint16                          IcmpBufLen;
  Std_ReturnType                  Result;
  uint16                          IcmpBufOffset;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrIdPtr,             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(*SrcAddrIdPtr), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr,               IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   *SrcAddrIdPtr);

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  IcmpBufLen = (sizeof(IpV6_NdpIaType) + sizeof(IpV6_NdpOptLLAddrType) + sizeof(IpV6_NdpOptAddrListType) + (IpV6_VCfgGetSourceAddressTableSize() * IPV6_ADDRESS_LEN_BYTE));

  if (BUFREQ_OK == IpV6_Icmp_ProvideTxBuffer(SrcAddrIdPtr, IPV6_SOCK_IDX_NDP, &IpTxReqDesc, DstAddrPtr, &IcmpBufPtr, &IcmpBufLen))
  {
    IPV6_P2V(IpV6_NdpIaType) IaHdrPtr = (IPV6_P2V(IpV6_NdpIaType))&IcmpBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IaHdrPtr->Reserved32 = 0;
    IcmpBufOffset = sizeof(IpV6_NdpIaType);

    /* add mandatory Source Link-Layer Address Option */
    IpV6_Ndp_VWriteLLAddrOpt(IpCtrlIdx, IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR, &IcmpBufPtr[IcmpBufOffset]);
    IcmpBufOffset += sizeof(IpV6_NdpOptLLAddrType);

    /* add mandatory Target Link-Layer Address Option */
    IcmpBufPtr[IcmpBufOffset+0] = IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR;
    IcmpBufPtr[IcmpBufOffset+1] = 1; /* 1*8 byte */
    IPV6_LL_ADDR_COPY(&IcmpBufPtr[IcmpBufOffset+2], TgtLLAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IcmpBufOffset += sizeof(IpV6_NdpOptLLAddrType);

    /* add mandatory Target Address List Option */
    IcmpBufOffset += IpV6_Ndp_VWriteAddrListOpt(IpCtrlIdx, IPV6_NDP_OPT_TYPE_TARGET_ADDR_LIST, &IcmpBufPtr[IcmpBufOffset]);
    /* BufOffset invalid from this point */

    if (E_OK == IpV6_Icmp_Transmit(IPV6_SOCK_IDX_NDP, &IpTxReqDesc, IPV6_ICMP_MSG_TYPE_NDP_INV_NA, 0, IcmpBufOffset))
    {
      Result = E_OK;
    }
    else
    {
      Result = E_NOT_OK;
    }
  }
  else
  {
    Result = E_NOT_OK;
  }

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VTxInverseAdvertisement() */
#endif

#if (IPV6_ENABLE_NDP_INV_NA == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VTxInverseSolicitation()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxInverseSolicitation(
  IpV6_AddrIdType            AddrId,
  IPV6_P2C(uint8)            TgtLLAddrPtr,
  boolean                    IncludeSrcAddrList)
{
  uint8                           IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  TcpIp_IpTxRequestDescriptorType IpTxReqDesc;
  IPV6_P2V(uint8)                 IcmpBufPtr;
  uint16                          IcmpBufLen;
  Std_ReturnType                  Result;
  uint16                          IcmpBufOffset;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != TgtLLAddrPtr,      IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  IcmpBufLen = (sizeof(IpV6_NdpIsType) + (2 * sizeof(IpV6_NdpOptLLAddrType)));

  if (TRUE == IncludeSrcAddrList)
  {
    IcmpBufLen += (sizeof(IpV6_NdpOptAddrListType) + (IpV6_VCfgGetSourceAddressTableSize() * IPV6_ADDRESS_LEN_BYTE));
  }

  if (BUFREQ_OK == IpV6_Icmp_ProvideTxBuffer(&AddrId, IPV6_SOCK_IDX_NDP, &IpTxReqDesc, &IpV6_AddrAllNodesLL, &IcmpBufPtr, &IcmpBufLen))
  {
    IPV6_P2V(IpV6_NdpIsType) IsHdrPtr = (IPV6_P2V(IpV6_NdpIsType))&IcmpBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IsHdrPtr->Reserved32 = 0;
    IcmpBufOffset = sizeof(IpV6_NdpIsType);

    /* add mandatory Source Link-Layer Address Option */
    IpV6_Ndp_VWriteLLAddrOpt(IpCtrlIdx, IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR, &IcmpBufPtr[IcmpBufOffset]);
    IcmpBufOffset += sizeof(IpV6_NdpOptLLAddrType);

    /* add mandatory Target Link-Layer Address Option */
    IcmpBufPtr[IcmpBufOffset+0] = IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR;
    IcmpBufPtr[IcmpBufOffset+1] = 1; /* 1*8 byte */
    IPV6_LL_ADDR_COPY(&IcmpBufPtr[IcmpBufOffset+2], TgtLLAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IcmpBufOffset += sizeof(IpV6_NdpOptLLAddrType);

    if (TRUE == IncludeSrcAddrList)
    {
      /* add optional Source Address List Option */
      IcmpBufOffset += IpV6_Ndp_VWriteAddrListOpt(IpCtrlIdx, IPV6_NDP_OPT_TYPE_SOURCE_ADDR_LIST, &IcmpBufPtr[IcmpBufOffset]);
    }

    IPV6_CTRL_VAR(IpV6_IcmpNextHopLLAddrOverride) = TRUE;
    IPV6_LL_ADDR_COPY(IPV6_CTRL_VAR(IpV6_IcmpNextHopLLAddrOverrideValue), TgtLLAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

    if (E_OK == IpV6_Icmp_Transmit(IPV6_SOCK_IDX_NDP, &IpTxReqDesc, IPV6_ICMP_MSG_TYPE_NDP_INV_NS, 0, IcmpBufOffset))
    {
      Result = E_OK;
    }
    else
    {
      Result = E_NOT_OK;
    }
  }
  else
  {
    Result = E_NOT_OK;
  }

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VTxInverseSolicitation() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VRxInverseAdvertisement()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VRxInverseAdvertisement(
  IpV6_AddrIdType         AddrId,
  IPV6_P2C(IpV6_AddrType) SrcAddrPtr,
  IPV6_P2C(uint8)         DataPtr,
  uint16                  LenByte)
{
  uint8                           IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint16                          DataOffset;
  IPV6_P2C(IpV6_NdpOptLLAddrType) SrcLLAddrOptPtr   = (IPV6_P2C(IpV6_NdpOptLLAddrType))NULL_PTR;
  IPV6_P2C(IpV6_NdpOptLLAddrType) TgtLLAddrOptPtr   = (IPV6_P2C(IpV6_NdpOptLLAddrType))NULL_PTR;
  IPV6_P2C(IpV6_AddrType)         TgtAddrListPtr    = (IPV6_P2C(IpV6_AddrType))NULL_PTR;
  IPV6_P2C(IpV6_NdpOptMtuType)    MtuOptPtr         = (IPV6_P2C(IpV6_NdpOptMtuType))NULL_PTR;
  uint8                           TgtAddrCount      = 0;
  uint8                           TgtAddrListIdx;
  IpV6_ListIdxType                NcEntryIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#else
  IPV6_UNUSED_PARAMETER(SrcAddrPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  DataOffset = 4; /* ignore and skip 32bit reserved filed */

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  /* NDP option valiadtion roules also apply to [RFC3122] */
  if (E_OK != IpV6_Ndp_VValidateOptions(&DataPtr[DataOffset], LenByte - DataOffset))
  {
    return; /* silently discard inverse advertisement with invalid options */
  }

  while (DataOffset < LenByte)
  {
    uint8  OptType = DataPtr[DataOffset];
    uint16 OptLenByte = IPV6_NDP_CALC_OPTION_LENGTH(DataPtr[DataOffset+1]);

    switch (OptType)
    {
      case IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR:
        /* mandatory */
        SrcLLAddrOptPtr = (IPV6_P2C(IpV6_NdpOptLLAddrType))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        if (IPV6_LL_ADDR_IS_INVALID(SrcLLAddrOptPtr->addr))
        {
          /* ignore invalid source link-layer address */
          SrcLLAddrOptPtr = NULL_PTR;
        }
      break;

      case IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR:
        /* mandatory */
        TgtLLAddrOptPtr = (IPV6_P2C(IpV6_NdpOptLLAddrType))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        if (IPV6_LL_ADDR_IS_INVALID(TgtLLAddrOptPtr->addr))
        {
          /* ignore invalid target link-layer address */
          TgtLLAddrOptPtr = NULL_PTR;
        }
      break;

      case IPV6_NDP_OPT_TYPE_TARGET_ADDR_LIST:
        /* mandatory */
        {
          uint16 AddrListLenByte = (uint16)(OptLenByte - sizeof(IpV6_NdpOptAddrListType));

          if ((0 < AddrListLenByte) && (0 == (AddrListLenByte % IPV6_ADDRESS_LEN_BYTE)))
          {
            TgtAddrCount   = (uint8)(AddrListLenByte / IPV6_ADDRESS_LEN_BYTE);
            TgtAddrListPtr = (IPV6_P2C(IpV6_AddrType))&DataPtr[DataOffset + sizeof(IpV6_NdpOptAddrListType)]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
          }
        }
      break;

      case IPV6_NDP_OPT_TYPE_MTU:
        /* optional */
        MtuOptPtr = (IPV6_P2C(IpV6_NdpOptMtuType))&DataPtr[DataOffset]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
      break;

      default:
        /* silently ignore unknown option */
      break;
    }

    DataOffset += OptLenByte;
  }

  if ((NULL_PTR == SrcLLAddrOptPtr) || (NULL_PTR == TgtLLAddrOptPtr) || (NULL_PTR == TgtAddrListPtr))
  {
    /* mandatory option missing. silently ignore invalid message */
    return;
  }

  if (NULL_PTR != MtuOptPtr)
  {
    /* optional MTU Option present */
  }

  if (0 < IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount)
  {
    if (TRUE == TcpIp_CmpLLAddr(TgtLLAddrOptPtr->addr, IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TargetPhysAddr))
    {
      /* answer to pending inverse solicitatin received. stop sending retransmissions */
      IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount = 0;
    }
  }

# if (IPV6_ENABLE_NDP_INV_NA_NC_UPDATE == STD_ON)
  if (IpV6_VCfgIsNdpInvNaNcUpdateEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    TgtAddrListIdx = TgtAddrCount;

    /* Lookup each address of the target address list in NeighborCache and record link-layer address if there exists an INCOMPLETE entry */
    while (TgtAddrListIdx > 0)
    {
      TgtAddrListIdx--;

      if (E_OK == IpV6_Ndp_VNCLookup(IpCtrlIdx, &TgtAddrListPtr[TgtAddrListIdx], &NcEntryIdx))
      {
        if (IPV6_REACHABLE_STATE_INCOMPLETE == IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State)
        {
          /* record link-layer address and set entry state to STALE.
           * This will allow communication with the neighbor but cause NUD to verify the entry.
           */
          IpV6_Ndp_VNCUpdateLinkLayerAddress(IpCtrlIdx, NcEntryIdx, SrcLLAddrOptPtr->addr);
          IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, NcEntryIdx, IPV6_REACHABLE_STATE_STALE);
        }
      }
    }
  }
# endif

# if (0 < IPV6_IND_ADDR_LIST_RECEIVED_CBK_COUNT)
  IpV6_IndAddrListReceivedCbks(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), (IPV6_P2C(Eth_PhysAddrType))SrcLLAddrOptPtr->addr, TgtAddrListPtr, TgtAddrCount); /* PRQA S 0310 */ /* MD_IpV6_0310 */
# endif

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VRxInverseAdvertisement() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_VTxNeighborAdvertisement()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxNeighborAdvertisement(
  IPV6_P2V(IpV6_AddrIdType) AddrIdPtr,
  IPV6_P2C(IpV6_AddrType)   DstAddrPtr,
  IPV6_P2C(IpV6_AddrType)   TargetAddrPtr,
  uint8                     RSOFlags)
{
  uint8                                           IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  TcpIp_IpTxRequestDescriptorType                 IpTxReqDesc;
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR)          IcmpBufPtr;
  uint16                                          IcmpBufLen;
  uint16                                          IcmpPayloadLen = 0;
  P2VAR(IpV6_NdpNaType, AUTOMATIC, IPV6_APPL_VAR) NdpNaHdrPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 5 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,               IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_NOT_INITIALIZED, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrIdPtr,             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,     E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(*AddrIdPtr), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID,     E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr,            IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,     E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != TargetAddrPtr,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,     E_NOT_OK);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   *AddrIdPtr);

  IcmpBufLen = sizeof(IpV6_NdpNaType) + 8; /* 8 = LL option length */

  if (BUFREQ_OK != IpV6_Icmp_ProvideTxBuffer(AddrIdPtr, IPV6_SOCK_IDX_NDP, &IpTxReqDesc, DstAddrPtr, &IcmpBufPtr, &IcmpBufLen))
  {
    return E_NOT_OK;
  }

  NdpNaHdrPtr = (P2VAR(IpV6_NdpNaType, AUTOMATIC, IPV6_APPL_VAR))&IcmpBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  NdpNaHdrPtr->RSOReserved = (uint8)(RSOFlags & 0xE0u); /* Mask: 11100000 */
  NdpNaHdrPtr->Reserved8[0] = 0;
  NdpNaHdrPtr->Reserved8[1] = 0;
  NdpNaHdrPtr->Reserved8[2] = 0;
  IPV6_ADDR_COPY(NdpNaHdrPtr->TargetAddress, *TargetAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IcmpPayloadLen += sizeof(IpV6_NdpNsType);

  IpV6_Ndp_VWriteLLAddrOpt(IpCtrlIdx, IPV6_NDP_OPT_TYPE_TARGET_LL_ADDR, &IcmpBufPtr[IcmpPayloadLen]);
  IcmpPayloadLen += 8;

  return IpV6_Icmp_Transmit(IPV6_SOCK_IDX_NDP, &IpTxReqDesc, IPV6_ICMP_MSG_TYPE_NDP_NA, 0, IcmpPayloadLen);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VTxNeighborAdvertisement() */

#if (IPV6_ENABLE_ROUTER_SOLICITATIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_VTxRouterSolicitation()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxRouterSolicitation(
  IPV6_P2V(IpV6_AddrIdType) SrcAddrIdPtr,
  boolean                   IncludeSllaOpt)
{
  uint8                                           IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  TcpIp_IpTxRequestDescriptorType                 IpTxReqDesc;
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR)          IcmpBufPtr;
  uint16                                          IcmpBufLen;
  uint16                                          IcmpPayloadLen = 0;
  P2VAR(IpV6_NdpRsType, AUTOMATIC, IPV6_APPL_VAR) NdpRsHdrPtr;

# if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_NOT_INITIALIZED, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrIdPtr,             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,     E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(*SrcAddrIdPtr), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID,     E_NOT_OK);
# endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, *SrcAddrIdPtr);

  IcmpBufLen = sizeof(IpV6_NdpRsType);

  if (BUFREQ_OK != IpV6_Icmp_ProvideTxBuffer(SrcAddrIdPtr, IPV6_SOCK_IDX_NDP, &IpTxReqDesc, &IpV6_AddrAllRoutersLL, &IcmpBufPtr, &IcmpBufLen))
  {
    return E_NOT_OK;
  }

  NdpRsHdrPtr = (P2VAR(IpV6_NdpRsType, AUTOMATIC, IPV6_APPL_VAR))&IcmpBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  NdpRsHdrPtr->Reserved32 = 0;
  IcmpPayloadLen += sizeof(IpV6_NdpRsType);

  if (TRUE == IncludeSllaOpt)
  {
    /* if the source address is not the unspecified address, the source link-layer address option SHOULD be
       included in the solicitation */

    IpV6_Ndp_VWriteLLAddrOpt(IpCtrlIdx, IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR, &IcmpBufPtr[IcmpPayloadLen]);
    IcmpPayloadLen += 8; /* size of source link-layer address option */
  }

  return IpV6_Icmp_Transmit(IPV6_SOCK_IDX_NDP, &IpTxReqDesc, IPV6_ICMP_MSG_TYPE_NDP_RS, 0, IcmpPayloadLen);

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VTxRouterSolicitation() */
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_VTxNeigborSolicitation()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_VTxNeigborSolicitation(
  IPV6_P2V(IpV6_AddrIdType) SrcAddrIdPtr,
  IPV6_P2C(IpV6_AddrType)   TargetAddrPtr,
  boolean                   Unicast,
  boolean                   ForceUnspecSrcAddr)
{
  TcpIp_IpTxRequestDescriptorType                 IpTxReqDesc;
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR)          IcmpBufPtr;
  uint16                                          IcmpBufLen;
  uint16                                          IcmpPayloadLen = 0;
  IpV6_AddrType                                   DestAddr;    /* PRQA S 0759 */ /* MD_IpV6_0759 */  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  P2VAR(IpV6_NdpNsType, AUTOMATIC, IPV6_APPL_VAR) NsPacketPtr;
  IPV6_P2V(IpV6_AddrType)                         SrcAddrPtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,                  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_NOT_INITIALIZED, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrIdPtr,             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,     E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(*SrcAddrIdPtr), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID,     E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != TargetAddrPtr,            IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,     E_NOT_OK);
#endif

  IPV6_ADDR_COPY(DestAddr, *TargetAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  if (FALSE == Unicast) /* send packet to solicited node multicast address */
  {
    IpV6_VGetSolicitedNodeAddr(&DestAddr, TargetAddrPtr, IPV6_MULTICAST_SCOPE_LINK_LOCAL);
  }
  else
  {
    IPV6_ADDR_COPY(DestAddr, *TargetAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  }

  IcmpBufLen = sizeof(IpV6_NdpNsType);

  if (BUFREQ_OK != IpV6_Icmp_ProvideTxBuffer(SrcAddrIdPtr, IPV6_SOCK_IDX_NDP, &IpTxReqDesc, &DestAddr, &IcmpBufPtr, &IcmpBufLen))
  {
    return E_NOT_OK;
  }

  NsPacketPtr = (P2VAR(IpV6_NdpNsType, AUTOMATIC, IPV6_APPL_VAR))&IcmpBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  NsPacketPtr->Reserved32 = 0;
  IPV6_ADDR_COPY(NsPacketPtr->TargetAddress, *TargetAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IcmpPayloadLen += sizeof(IpV6_NdpNsType);

  SrcAddrPtr = IpV6_GetSrcAddrPtr(&IpTxReqDesc);

  if (TRUE == ForceUnspecSrcAddr)
  {
    IPV6_ADDR_COPY(*SrcAddrPtr, IpV6_AddrUnspecified); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  }
  else if ((NULL_PTR == SrcAddrPtr) || (!IPV6_ADDR_IS_UNSPECIFIED(*SrcAddrPtr))) /* PRQA S 3415 */ /* MD_IpV6_3415 */
  {
    /* If source address is not the unspecified address (DAD solicitations), provide our link-layer address,
     * so target can directly respond to it with the neighbor advertisement. (RFC4861 4.3.)
     */
    IpV6_Ndp_VWriteLLAddrOpt(IPV6_ADDR_ID_2_CTRL_IDX(*SrcAddrIdPtr), IPV6_NDP_OPT_TYPE_SOURCE_LL_ADDR, &IcmpBufPtr[IcmpPayloadLen]);
    IcmpPayloadLen += IPV6_NDP_OPT_LL_ADDR_LEN_BYTE;
  }
  else
  {
    /* nothing to do in this case */
  }

  return IpV6_Icmp_Transmit(IPV6_SOCK_IDX_NDP, &IpTxReqDesc, IPV6_ICMP_MSG_TYPE_NDP_NS, 0, IcmpPayloadLen);

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_VTxNeigborSolicitation() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCUpdateEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VNCUpdateEntry(
  uint8                   IpCtrlIdx,
  IpV6_ListIdxType        EntryIdx,
  IpV6_ReachableStateType NewState)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_NC_IDX(EntryIdx),    IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

#if (IPV6_ENABLE_NDP_NUD == STD_ON)
  if (IpV6_VCfgIsNudEnabled() && (IPV6_REACHABLE_STATE_REACHABLE == NewState)) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].NextEventTime, IPV6_CTRL_VAR(IpV6_ReachableTimeMs));
  }
#endif

  IPV6_CTRL_VAR(IpV6_NeighborCache)[EntryIdx].State = NewState;
} /* End Of IpV6_Ndp_VNCUpdateEntry() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VNCUpdateLinkLayerAddress()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VNCUpdateLinkLayerAddress(
  uint8           IpCtrlIdx,
  uint8           NcEntryIdx,
  IPV6_P2C(uint8) NewLinkLayerAddressPtr)
{
  TcpIp_SockAddrInet6Type SockAddrInet6;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_NC_IDX(NcEntryIdx),  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

  SockAddrInet6.domain  = TCPIP_AF_INET6;
  IPV6_ADDR_COPY(*(uint8*)&SockAddrInet6.addr[0], IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].NeighborAddress);  /* PRQA S 0310 */ /* MD_IpV6_0310 */
  SockAddrInet6.port    = TCPIP_PORT_ANY;

  if (NULL_PTR != NewLinkLayerAddressPtr)
  {
    IPV6_LL_ADDR_COPY(&IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0], NewLinkLayerAddressPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  }
  else
  {
    uint8_least AddrByteIdx = ETH_PHYS_ADDR_LEN_BYTE;

    while (0 < AddrByteIdx)
    {
      AddrByteIdx--;
      IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[AddrByteIdx] = 0x00U;
    }
  }

  TcpIp_VPhysAddrTableChg(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), (TCPIP_P2V(TcpIp_SockAddrType))&SockAddrInet6, &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0], (boolean)(NULL_PTR != NewLinkLayerAddressPtr)); /* PRQA S 0310 */ /* MD_IpV6_0310 */

} /* End Of IpV6_Ndp_VNCUpdateLinkLayerAddress() */

/**********************************************************************************************************************
 *  IpV6_Ndp_VStart()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_VStart(uint8 IpCtrlIdx)
{
  uint32           DelayMs;
  IpV6_AddrType    SrcAddrCandidate;    /* PRQA S 0759 */ /* MD_IpV6_0759 */

  /* Reset CurHopLimit, ReachableTime and RestransTimer to configured default values.
     These values may be updated by received Router Advertisements. */
  IPV6_CTRL_VAR(IpV6_CurHopLimit)    = IpV6_VCfgGetDefaultHopLimit();
  IPV6_CTRL_VAR(IpV6_RetransTimerMs) = IpV6_VCfgGetRetransTimer();

#if (IPV6_ENABLE_NDP_NUD == STD_ON)
  if (IpV6_VCfgIsNudEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    /* configure ReachableTime for Neighbor Unreachability Detection.
       This value may be changed by received router advertisements. */

# if (IPV6_ENABLE_RND_REACHABLE_TIME == STD_ON)
    if (IpV6_VCfgIsRandomReachableTimeEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      /* Set randomized default ReachableTime. */
      IPV6_CTRL_VAR(IpV6_BaseReachableTimeMs) = IpV6_VCfgGetReachableTime();
      IPV6_CTRL_VAR(IpV6_ReachableTimeMs)     = IpV6_VRandomizeValue(IpCtrlIdx, IPV6_CTRL_VAR(IpV6_BaseReachableTimeMs));

    }
    else
# endif
    { /* PRQA S 3201 */ /* MD_IpV6_3201 */
      /* Set configured ReachableTime. */
      IPV6_CTRL_VAR(IpV6_ReachableTimeMs)     = IpV6_VCfgGetReachableTime();
    }
  }
#endif

#if (IPV6_ENABLE_ROUTER_SOLICITATIONS == STD_ON)
  if (0 < IpV6_VCfgGetMaxRtrSolicitations()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    /* Schedule transmission of first Router Advertisement. */
    IPV6_CTRL_VAR(IpV6_Ndp_RouterSolicitationTxCount) = 0;

# if (IPV6_ENABLE_RND_ROUTER_SOLICITATION_DELAY == STD_ON)
    if (IpV6_VCfgIsRndRtrSolicitationDelayEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      DelayMs = IpV6_VGetRandomValue(0, IpV6_VCfgGetMaxRtrSolicitationDelay());
    }
    else
# endif
    { /* PRQA S 3201 */ /* MD_IpV6_3201 */
      DelayMs = IpV6_VCfgGetMaxRtrSolicitationDelay();
    }

    IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Ndp_RouterSolicitationNextTxTime), DelayMs);
  }
#endif

  /* build link-local address */
  IpV6_MemCpy(&SrcAddrCandidate.addr[0], &IpV6_PrefixLinkLocal.Prefix.addr[0], 16 - IPV6_INTERFACE_IDENTIFIER_LEN_BYTE); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IpV6_MemCpy(&SrcAddrCandidate.addr[16 - IPV6_INTERFACE_IDENTIFIER_LEN_BYTE], &IPV6_CTRL_VAR(IpV6_InterfaceIdentifier)[0], IPV6_INTERFACE_IDENTIFIER_LEN_BYTE); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  /* join all-nodes multicast group */
  IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IpV6_AddrAllNodesLL, TRUE);
  /* The solicited-node multicast groups for source addresses will be joines in IpV6_Ndp_VAddSourceAddress() */

#if (IPV6_ENABLE_MLD == STD_ON)
  if (IpV6_VCfgHasMldConfig())
  {
    /* If MLD is enabled, we must wait until all reports for the solicited node address have been sent
     * before sending the first DAD Neighbor Solicitation. A random delay of the first DAD NS will be caused by the MLD.
     * We set the worst-case delay that could be caused by MLD here. When MLD has finished sending all Reports, DAD
     * for the link-local address will be triggered by readjusting the delay in IpV6_Mld_VNotifyAllReportsSent()
     */
    DelayMs = (IPV6_MLD_VAR(Robustness) * IpV6_VCfgGetMldMaxUnsolicitedReportIntMs());
  }
  else
#endif
#if (IPV6_ENABLE_SLAAAC_DELAY == STD_ON)
  if (IpV6_VCfgIsSlaacDelayEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    DelayMs = IpV6_VGetRandomValue(0, IpV6_VCfgGetMaxSlaacDelay());
  }
  else
#endif
  { /* PRQA S 3201 */ /* MD_IpV6_3201 */
    DelayMs = 0;
  }

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* trigger configuration of link-local source adress */
  (void)IpV6_Ndp_VAddSourceAddress(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].IpAddrId, &SrcAddrCandidate, IPV6_LIFETIME_UNLIMITED, IPV6_LIFETIME_UNLIMITED, DelayMs, 2);

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* End of IpV6_Ndp_VStart() */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (IPV6_ENABLE_NDP_INV_NA == STD_ON)
/**********************************************************************************************************************
 *  IpV6_Ndp_SendInverseSolicitation()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_SendInverseSolicitation(
  uint8                      IpCtrlIdx,
  IPV6_P2C(uint8)            TgtLLAddrPtr,
  boolean                    IncludeSrcAddrList)
{
  if (0 < IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount)
  {
    return E_NOT_OK;
  }

  IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).IncludeSrcAddrList = IncludeSrcAddrList;
  IPV6_LL_ADDR_COPY(IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TargetPhysAddr, TgtLLAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount = IpV6_VCfgGetMulticastSolicits();
  IPV6_SET_CURRENT_TIME(IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).Timeout);

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Ndp_SendInverseSolicitation() */
#endif

/**********************************************************************************************************************
 *  IpV6_Ndp_Init()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Ndp_Init(uint8 IpCtrlIdx)
{
#if (IPV6_NDP_ENABLE_TIMEOUT_OPTIMIZATION == STD_ON)
  IpV6_NextSourceAddressEventTimeMs = 0;
  IpV6_NextNeighborCacheEventTimeMs = 0;
  IpV6_NextDefaultRouterEventTimeMs = 0;
  IpV6_NextPrefixListEventTimeMs = 0;
#endif

#if defined(IPV6_VENABLE_CACHE_LOOKUP_OPTIMIZATION_DBG)
  IpV6_DbgNeighborCacheLookups          = 0;
  IpV6_DbgNeighborCacheHits             = 0;
  IpV6_DbgNeighborCacheOptimizedHits    = 0;
  IpV6_DbgDestinationCacheLookups       = 0;
  IpV6_DbgDestinationCacheHits          = 0;
  IpV6_DbgDestinationCacheOptimizedHits = 0;
#endif

  IpV6_VSATRemoveAllEntrys(IpCtrlIdx);
  IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount)     = 0;
  IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount) = 0;
  IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount)  = 0;
  IPV6_CTRL_VAR(IpV6_PrefixListEntryCount)        = 0;

#if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
  IPV6_CTRL_VAR(IpV6_DefaultLinkMtu) = IpV6_VCfgGetDefaultLinkMtu();
#endif

  IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).Pending = FALSE;
#if (IPV6_ENABLE_NDP_INV_NA == STD_ON)
  IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount = 0;
  IPV6_TIME_SET_INFINITY(IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).Timeout);
#endif

  IpV6_SetHopLimit(IPV6_SOCK_IDX_NDP, IPV6_NDP_MSG_HOP_LIMIT);
} /* End of IpV6_Ndp_Init() */

/**********************************************************************************************************************
 *  IpV6_Ndp_MainFunction()
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
 */
FUNC(void, IPV6_CODE) IpV6_Ndp_MainFunction(uint8 IpCtrlIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_NDP_MAIN_FUNCTION, IPV6_E_INV_CTRL_IDX);
#endif

  /* #10 Send pending Duplicate Address Detection response. */
  if (TRUE == IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).Pending)
  {
    IpV6_AddrIdType AddrId = IPV6_CTRL_VAR(IpV6_BcAddrId);

    /* Defend our addresses by sending neighbor advertisements if we received a DAD neighbor solicitation for one of our addresses by another node. */
    if (E_OK == IpV6_Ndp_VTxNeighborAdvertisement(&AddrId, &IpV6_AddrAllNodesLL, &IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).TargetAddr, IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).RSOFlags))
    {
      IPV6_CTRL_VAR(IpV6_Ndp_PendingDadNa).Pending = FALSE;
    }
  }

  /* #20 Process pending Inverse Neighbor Solicitation messages. */
#if (IPV6_ENABLE_NDP_INV_NA == STD_ON)
  if (IpV6_VCfgIsNdpInvNaEnabled() && IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).Timeout)) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    if (0 < IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount)
    {
      /* #30 Try to send Inv NS message. */
      if (E_OK == IpV6_Ndp_VTxInverseSolicitation(IPV6_CTRL_VAR(IpV6_BcAddrId), &IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TargetPhysAddr[0], IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).IncludeSrcAddrList))
      {
        /* schedule retransmission */
        IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).Timeout, IPV6_CTRL_VAR(IpV6_RetransTimerMs));
        IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TxCount--;
      }
    }
    else
    {
      /* #40 Notify upper layer about expired timeout. */
# if (0 < IPV6_IND_ADDR_LIST_RECEIVED_CBK_COUNT)
      IpV6_IndAddrListReceivedCbks(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), (IPV6_P2C(Eth_PhysAddrType))&IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).TargetPhysAddr[0], (IPV6_P2C(IpV6_AddrType))NULL_PTR, 0); /* PRQA S 0310 */ /* MD_IpV6_0310 */
# endif
      IPV6_TIME_SET_INFINITY(IPV6_CTRL_VAR(IpV6_Ndp_PendingInvNs).Timeout);
    }
  }
#endif

  /* #50 Process state of entries in Source Address Table. */
  IpV6_Ndp_VSATCheckLifetimes(IpCtrlIdx);

  /* #60 Process state of entries in Default Router List. */
  IpV6_Ndp_VDRLCheckLifetimes(IpCtrlIdx);

  /* #70 Process state of entries in Neighbor Cache. */
  IpV6_Ndp_VNCCheckLifetimes(IpCtrlIdx);

#if (IPV6_ENABLE_PATH_MTU == STD_ON)
  /* #80 Process state of entries in Destination Cache. */
  if (0 < IpV6_VCfgGetPathMtuTimeout())
  {
    IpV6_Ndp_VDCCheckLifetimes(IpCtrlIdx);
  }
#endif
} /* End of IpV6_Ndp_MainFunction() */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  IpV6_Ndp_ClearCache
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV6_CODE) IpV6_Ndp_ClearCache(
    IpV6_AddrIdType IpAddrId )
{
  Std_ReturnType retVal = E_NOT_OK;
  
  /* #10 Check input parameter */
  if(IPV6_IS_VALID_ADDR_ID(IpAddrId))
  {
    /* #20 Clear cache */
    uint8 IpCtrlIdx = IPV6_ADDR_ID_2_CTRL_IDX(IpAddrId);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
    IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount) = 0;
    retVal = E_OK;
  }

  return retVal;
}

/***********************************************************************************************************************
 *  IpV6_Ndp_GetCacheEntries
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV6_Ndp_GetCacheEntries(
    uint8 ctrlIdx,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_VAR) numberOfElements,
    P2VAR(TcpIp_NdpCacheEntryType, AUTOMATIC, TCPIP_APPL_VAR) entryListPtr )
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 IpCtrlIdx = 0;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  /* #10 Check input parameters. */
  if (IPV6_CTRLIDX_ETHIF_2_IP(ctrlIdx, IpCtrlIdx) == E_OK)
  {
    IpV6_ListIdxType idxEntryCount = 0;
    
    /* #20 Check if cache entries have to be copied or counted. */
    if(*numberOfElements == 0)
    {
      IpV6_ListIdxType idxCache;

      /* #30 Count cache entries. */
      *numberOfElements = IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount);

      for(idxCache = 0; idxCache < *numberOfElements; idxCache++)
      {
        if(IPV6_CTRL_VAR(IpV6_NeighborCache)[idxCache].State != IPV6_REACHABLE_STATE_INCOMPLETE)
        {
          idxEntryCount++;
        }
      }

      *numberOfElements = idxEntryCount;

      retVal = E_OK;
    }
    else
    {
      IpV6_ListIdxType idxCache;

      /* #40 Copy cache entries. */
      *numberOfElements = IPV6_MIN(*numberOfElements, IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount));

      for(idxCache = 0; idxCache < *numberOfElements; idxCache++)
      {
        if(IPV6_CTRL_VAR(IpV6_NeighborCache)[idxCache].State != IPV6_REACHABLE_STATE_INCOMPLETE)
        {
          if(IPV6_CTRL_VAR(IpV6_NeighborCache)[idxCache].State == IPV6_REACHABLE_STATE_REACHABLE)
          {
            entryListPtr[idxEntryCount].State = TCPIP_NDP_ENTRY_VALID;
          }
          else
          {
            entryListPtr[idxEntryCount].State = TCPIP_NDP_ENTRY_STALE;
          }
          
          IPV6_LL_ADDR_COPY(entryListPtr[idxEntryCount].PhysAddr,     IPV6_CTRL_VAR(IpV6_NeighborCache)[idxCache].LinkLayerAddress);
          IPV6_ADDR_COPY(   entryListPtr[idxEntryCount].Inet6Addr[0], IPV6_CTRL_VAR(IpV6_NeighborCache)[idxCache].NeighborAddress);  /* PRQA S 0310 */ /* MD_IpV6_0310 */
          idxEntryCount++;
        }
      }

      *numberOfElements = idxEntryCount;

      retVal = E_OK;
    }
  }

  return retVal;
}  /* IpV6_Ndp_GetCacheEntries() */ /* PRQA S 6080 */ /* MD_MSR_STIMF */

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV6_Ndp.c
 *********************************************************************************************************************/
