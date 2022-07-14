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
 *         \file  TcpIp_IpV6.c
 *        \brief  Implementation of Internet Protocol version 6 (IPv6)
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

#define TCPIP_IPV6_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_IpV6_Priv.h"
#include "TcpIp_IcmpV6.h"
#include "TcpIp_Ndp.h"
#include "TcpIp_IpV6_Cbk.h"
#include "TcpIp_Priv.h"
#if (IPV6_ENABLE_MLD == STD_ON)
# include "TcpIp_Mld.h"
#endif

#if (IPV6_ENABLE_UL_TCP == STD_ON)
# include "TcpIp_Tcp_Cbk.h"
#endif

#if (IPV6_ENABLE_DHCP == STD_ON)
# include "TcpIp_DhcpV6.h"
# include "TcpIp_DhcpV6_Cbk.h"
#endif

#include "TcpIp_Udp_Cbk.h"

#include "EthIf.h"
#include "IpBase_String.h"

#if (IPV6_ENABLE_ETHSM == STD_ON)
# include "EthSM.h"
#endif

#if (IPV6_ENABLE_ADDR_NVM_STORAGE_SUPPORT == STD_ON)
# include "NvM.h"
#endif

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
# include "Crypto.h"
#endif

#if defined(IPV6_JOIN_MULTICAST_GROUP_CALLOUT_INCLUDE)
# include IPV6_JOIN_MULTICAST_GROUP_CALLOUT_INCLUDE
#endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of TcpIp_IpV6.h file */
#if (  (IPV6_SW_MAJOR_VERSION != 6u) \
    || (IPV6_SW_MINOR_VERSION != 3u) \
    || (IPV6_SW_PATCH_VERSION != 0u) )
  #error "TcpIp_IpV6.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

#define IPV6_VENABLE_HOP_BY_HOP_EXT_HDR_SKIPPING        STD_OFF
#define IPV6_VENABLE_DESTINATION_OPT_EXT_HDR_SKIPPING   STD_OFF

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

#define IPV6_HEADER_VERSION_OFFS   28
#define IPV6_HEADER_TC_OFFS        20

#define IPV6_HEADER_GET_VERSION(x) (((x) >> IPV6_HEADER_VERSION_OFFS) & 0x0000000Fu)
#define IPV6_HEADER_GET_TC(x)      (((x) >> IPV6_HEADER_TC_OFFS) & 0x000000FFu)
#define IPV6_HEADER_GET_FL(x)      ((x) & 0x000FFFFFu)

#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
# define IPV6_HEADER_VERSION_TC_FL_ZERO_NBO  (IPV6_HEADER_VERSION << IPV6_HEADER_VERSION_OFFS)
# define IPV6_HEADER_MASK_VERSION_NBO        0xF0000000u
# define IPV6_HEADER_MASK_TRAFFIC_CLASS_NBO  0x0FF00000u
# define IPV6_HEADER_MASK_FLOW_LABEL_NBO     0x000FFFFFu
#else
# define IPV6_HEADER_VERSION_TC_FL_ZERO_NBO  (IPV6_HEADER_VERSION << 4)
# define IPV6_HEADER_MASK_VERSION_NBO        0x000000F0u
# define IPV6_HEADER_MASK_TRAFFIC_CLASS_NBO  0x0000F00Fu
# define IPV6_HEADER_MASK_FLOW_LABEL_NBO     0xFFFF0F00u
#endif

/* PRQA S 3453 5 */ /* MD_MSR_19.7 */
#if (IPV6_ENABLE_ETHSM == STD_ON)
# define ETHSM_TCPIP_MODE_INDICATION(CtrlIdx, IsIpAddrAssigned) (void)EthSM_VTcpIpModeIndication((CtrlIdx), (IsIpAddrAssigned))
#else
# define ETHSM_TCPIP_MODE_INDICATION(CtrlIdx, IsIpAddrAssigned)
#endif

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define IPV6_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define IPV6_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint8, IPV6_VAR_ZERO_INIT) IpV6_State = IPV6_STATE_UNINIT; /* current state of the IPv6 node */

#define IPV6_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(IpV6_TimeType, IPV6_VAR_NOINIT) IpV6_Time; /* holds the current time since initialization of the the IpV6 module */

#if (IPV6_ENABLE_RX_DBG_COUNTERS == STD_ON)
VAR(IpV6_Dbg_RxCountersType, IPV6_VAR_NOINIT) IpV6_Dbg_RxCounters[IpV6_VCfgGetMaxNumCtrls()];
#endif

#if (IPV6_ENABLE_TX_DBG_COUNTERS == STD_ON)
VAR(IpV6_Dbg_TxCountersType, IPV6_VAR_NOINIT) IpV6_Dbg_TxCounters[IpV6_VCfgGetMaxNumCtrls()];
#endif

#define IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* link-local prefix [fe80::]/64 */
CONST(IpV6_AddrPrefixType, IPV6_CONST) IpV6_PrefixLinkLocal = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    {
      0xFE, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
  },
  64
};

/* [ff02::1:ff__:_____/104 */
CONST(IpV6_AddrPrefixType, IPV6_CONST) IpV6_PrefixSolicitedNode = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    {
      0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00
    }
  },
  104
};

/* [::1] */
CONST(IpV6_AddrType, IPV6_CONST) IpV6_AddrLoopback = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  }
};

/* [::] */
CONST(IpV6_AddrType, IPV6_CONST) IpV6_AddrUnspecified = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  }
};

/* [ff01::1] IL = interface local scope */
CONST(IpV6_AddrType, IPV6_CONST) IpV6_AddrAllNodesIL = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  }
};

/* [ff02::1] LL = link local scope */
CONST(IpV6_AddrType, IPV6_CONST) IpV6_AddrAllNodesLL = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
  }
};

/* All-routers [ff02::2] LL = link local scope */
CONST(IpV6_AddrType, IPV6_CONST) IpV6_AddrAllRoutersLL = /* PRQA S 0759 */ /* MD_IpV6_0759 */
{
  {
    0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
  }
};

#define IPV6_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV6_VGetInterfaceIdentifier()
 **********************************************************************************************************************/
STATIC INLINE FUNC(void, IPV6_CODE) IpV6_VGetInterfaceIdentifier(
  uint8 IpCtrlIdx,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) DstBufPtr);

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VPrivExtGenerateTempIdentifier()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VPrivExtGenerateTempIdentifier(
  uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_VPrivExtGetNextTempIdentifier()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VPrivExtGetNextTempIdentifier(
  uint8                   IpCtrlIdx,
  IPV6_P2V(IpV6_AddrType) AddrPtr);
#endif

/**********************************************************************************************************************
 *  IpV6_VGetCommonPrefixLen()
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV6_CODE) IpV6_VGetCommonPrefixLen(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) Addr1Ptr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) Addr2Ptr);

/**********************************************************************************************************************
 *  IpV6_VMakeInterfaceIdentifier()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VMakeInterfaceIdentifier(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) EthPhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) DstBufPtr);

/**********************************************************************************************************************
 *  IpV6_VHandleUnknownTlvOption()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VHandleUnknownTlvOption(
  IpV6_AddrIdType  AddrId,
  IPV6_P2C(uint8)  IpV6PktDataPtr,
  uint16           IpV6PktDataLen,
  uint16           UnknownOptOffset);

/**********************************************************************************************************************
 *  IpV6_VSelectSourceAddress()
 **********************************************************************************************************************/
STATIC FUNC(IpV6_CtrlAddrIdxType, IPV6_CODE) IpV6_VSelectSourceAddress(
  uint8                           IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)         DstAddrPtr);

/**********************************************************************************************************************
 *  IpV6_VSourceAddrIsBetterThan()
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV6_CODE) IpV6_VSourceAddrIsBetterThan(
  uint8                IpCtrlIdx,
  IpV6_CtrlAddrIdxType SrcAddrIdxA,
  IpV6_CtrlAddrIdxType SrcAddrIdxB,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr);

/**********************************************************************************************************************
 *  IpV6_VMulticastAddr2LinkLayerAddr()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VMulticastAddr2LinkLayerAddr(
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) PhysAddrPtr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) NetAddrPtr);

/**********************************************************************************************************************
 *  IpV6_VIngressFilter()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VIngressFilter(
  uint8                          IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)        DstAddrPtr,
  IPV6_P2V(IpV6_CtrlAddrIdxType) CtrlAddrIdxPtr
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
 ,IPV6_P2V(IpV6_DListIdxType)    TempAddrIdxPtr
#endif
#if (IPV6_ENABLE_MLD == STD_ON)
 ,IPV6_P2V(IpV6_DListIdxType)    McAddrIdxPtr
#endif
  );

/**********************************************************************************************************************
 *  IpV6_VResolveLinkLayerAddr()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VResolveLinkLayerAddr(
  uint8                                             IpCtrlIdx,
  IpV6_CtrlAddrIdxType                              CtrlAddrIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR)            PhysAddrPtr,
  P2VAR(IpV6_ListIdxType, AUTOMATIC, IPV6_APPL_VAR) NcEntryIdxPtr);

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VSegmentListInsert()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSegmentListInsert(
  P2VAR(IpV6_SegmentListType, AUTOMATIC, IPV6_APPL_VAR) ListPtr,
  uint8 InsertIdx);

/**********************************************************************************************************************
 *  IpV6_VSegmentListRemove()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSegmentListRemove(
  P2VAR(IpV6_SegmentListType, AUTOMATIC, IPV6_APPL_VAR) ListPtr,
  uint8 RemoveIdx);

/**********************************************************************************************************************
 *  IpV6_VSegmentListAdd()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSegmentListAdd(
  P2VAR(IpV6_SegmentListType, AUTOMATIC, IPV6_APPL_VAR) ListPtr,
  uint16 SegOfs,
  uint16 SegLen);

/**********************************************************************************************************************
 *  IpV6_VGetReassemblyBufferIdx()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VGetReassemblyBufferIdx(
  IpV6_AddrIdType                                   AddrId,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) SrcAddrPtr,
  uint32 FragmentId,
  P2VAR(IpV6_ListIdxType, AUTOMATIC, IPV6_APPL_VAR) FragmentBufIdx);

/**********************************************************************************************************************
 *  IpV6_VRxFragments()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VRxFragments(uint8 IpCtrlIdx);

#endif /* (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON) */

/**********************************************************************************************************************
 *  IpV6_VProcessFragmentHeader()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VProcessFragmentHeader(
  IpV6_AddrIdType AddrId,
  IPV6_P2C(uint8) DataPtr,
  uint16          DataLen,
  uint16          FragHdrOfs,
  uint16          PrevHdrOfs);

#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VGetFragmentTxBufferIdx()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VGetFragmentTxBufferIdx(
  uint8                                             IpCtrlIdx,
  P2VAR(IpV6_ListIdxType, AUTOMATIC, IPV6_APPL_VAR) FragmentBufIdx);

/**********************************************************************************************************************
 *  IpV6_VTxFragments()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VTxFragments(uint8 IpCtrlIdx);
#endif

#if ((IPV6_ENABLE_RX_FRAGMENTATION == STD_ON) || (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON))
/**********************************************************************************************************************
 *  IpV6_VInitFragments()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VInitFragments(uint8 IpCtrlIdx);
#endif

#if ((IPV6_ENABLE_ADDR_NVM_STORAGE_SUPPORT == STD_ON) || (IPV6_DEFAULT_ADDRESS_COUNT > 0))
/**********************************************************************************************************************
 *  IpV6_VConfigureSourceAddresses()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VConfigureSourceAddresses(uint8 IpCtrlIdx);
#endif

#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
/**********************************************************************************************************************
 *  IpV6_VConfigureDynamicMulticastAddresses()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VConfigureDynamicMulticastAddresses(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_VConfigureStaticMulticastAddresses()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VConfigureStaticMulticastAddresses(uint8 IpCtrlIdx);
#endif

/**********************************************************************************************************************
 *  IpV6_VCtrlStateStartup()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VCtrlStateStartup(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_VCtrlStateOnline()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VCtrlStateOnline(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_VCtrlStateShutdown()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VCtrlStateShutdown(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV6_VMakeInterfaceIdentifier()
 **********************************************************************************************************************/
/*! \brief        Convert an ethernet link-layer address (48bit MAC address) into a 64bit interface
 *                identifier (according to RFC 2464 4.)
 *  \param[in]    EthPhysAddr  link-layer (physical) address of the interface
 *  \param[out]   DstBufPtr    pointer to an 8-byte buffer where the identifier will be stored
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VMakeInterfaceIdentifier(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) EthPhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) DstBufPtr)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != EthPhysAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DstBufPtr,      IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  DstBufPtr[0] = (uint8)(EthPhysAddrPtr[0] ^ 0x02U); /* toggle "Universal/Local" bit */
  DstBufPtr[1] = EthPhysAddrPtr[1];
  DstBufPtr[2] = EthPhysAddrPtr[2];

  DstBufPtr[3] = 0xFF;
  DstBufPtr[4] = 0xFE;

  DstBufPtr[5] = EthPhysAddrPtr[3];
  DstBufPtr[6] = EthPhysAddrPtr[4];
  DstBufPtr[7] = EthPhysAddrPtr[5];

} /* End of IpV6_VMakeInterfaceIdentifier() */

/**********************************************************************************************************************
 *  IpV6_VGetInterfaceIdentifier()
 **********************************************************************************************************************/
/*! \brief       Get 64bit interface identifier from physical address of a given controller
 *  \param[in]   IpCtrlIdx  ip controller index
 *  \param[out]  DstBufPtr  pointer to an 8-byte buffer where the identifier will be stored
 **********************************************************************************************************************/
STATIC INLINE FUNC(void, IPV6_CODE) IpV6_VGetInterfaceIdentifier(
  uint8 IpCtrlIdx,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) DstBufPtr)
{
  Eth_PhysAddrType EthPhysAddr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DstBufPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  TCPIP_LL_GetPhysAddr(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), &EthPhysAddr[0]);
  IpV6_VMakeInterfaceIdentifier(&EthPhysAddr[0], DstBufPtr);

} /* End of IpV6_VGetInterfaceIdentifier() */

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VPrivExtGenerateTempIdentifier()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VPrivExtGenerateTempIdentifier(
  uint8 IpCtrlIdx)
{
  eslt_WorkSpaceMD5 Md5HashWs;

  /* [RFC4941 3.2.1. When Stable Storage Is Present / 2. ] */

  if (ESL_ERC_NO_ERROR != esl_initWorkSpaceHeader(&Md5HashWs.header, ESL_MAXSIZEOF_WS_MD5, NULL_PTR))
  {
    return E_NOT_OK;  /* error */
  }

  if(ESL_ERC_NO_ERROR != esl_initMD5(&Md5HashWs))  /* MD5 WS init */
  {
    return E_NOT_OK;  /* error */
  }

  (void)esl_updateMD5(&Md5HashWs, (2*IPV6_INTERFACE_IDENTIFIER_LEN_BYTE), &IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0]);

  if(ESL_ERC_NO_ERROR != esl_finalizeMD5(&Md5HashWs, &IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0]))
  {
    /* error */
    return E_NOT_OK;
  }

  /* [RFC4941 3.2.1. When Stable Storage Is Present / 3. ] */

  IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0] = (uint8)(IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0] & (~0x02U)); /* Clear 2nd bit in most significant byte |??????_?| */

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VPrivExtGenerateTempIdentifier() */

/**********************************************************************************************************************
 *  IpV6_VPrivExtGetNextTempIdentifier()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VPrivExtGetNextTempIdentifier(
  uint8                   IpCtrlIdx,
  IPV6_P2V(IpV6_AddrType) AddrPtr) /* PRQA S 3673 */ /* This parameter is changed by IpV6_MemCpy() */
{
  uint8_least CycleCounter  = IPV6_PRIV_EXT_TEMP_IDGEN_RND_RETRIES;

  while ((TRUE == IpV6_MemCmp32(&AddrPtr->addr[8], &IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0], IPV6_INTERFACE_IDENTIFIER_LEN_BYTE / 4)))
  {
    /* Current temporary interface identifier is equal to the identifier of the address.
     * Generate new temporery identifier and try again.
     */

    if (0 < CycleCounter)
    {
      CycleCounter--;
      (void)IpV6_VPrivExtGenerateTempIdentifier(IpCtrlIdx);
    }
    else
    {
      /* too many retries */
      IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
      return E_NOT_OK;
    }
  }

  /* Current temporary interface identifier is different than the identifier of the addres.
   * Update itentifier of the given address with current temporary ientifier.
   */
  IpV6_MemCpy(&AddrPtr->addr[IPV6_PREFIX_LEN_BYTE], &IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0], IPV6_INTERFACE_IDENTIFIER_LEN_BYTE); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VPrivExtGetNextTempIdentifier() */

#endif

/**********************************************************************************************************************
 *  IpV6_VMulticastAddr2LinkLayerAddr()
 **********************************************************************************************************************/
/*! \brief        Map an IPv6 multicast network-layer address to the corresponding link-layer multicast address
 *  \param[out]   PhysAddrPtr  pointer to the link-layer address variable
 *  \param[in]    NetAddrPtr   pointer to the network-layer IPv6 multicast address
 **********************************************************************************************************************/
STATIC INLINE FUNC(void, IPV6_CODE) IpV6_VMulticastAddr2LinkLayerAddr(
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) PhysAddrPtr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) NetAddrPtr)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != PhysAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NetAddrPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  /* map IPv6 multicast address to ethernet multicast address according to RFC 2464 section 7 */
  PhysAddrPtr[0] = 0x33;
  PhysAddrPtr[1] = 0x33;
  PhysAddrPtr[2] = NetAddrPtr->addr[12];
  PhysAddrPtr[3] = NetAddrPtr->addr[13];
  PhysAddrPtr[4] = NetAddrPtr->addr[14];
  PhysAddrPtr[5] = NetAddrPtr->addr[15];

} /* End of IpV6_VMulticastAddr2LinkLayerAddr() */

/**********************************************************************************************************************
 *  IpV6_VResolveLinkLayerAddr()
 **********************************************************************************************************************/
/*! \brief         Lookup the link-layer address of a node in the neighbor cache or start address resolution.
 *                 If the provided network address is a multicast address the corresponding ethernet multicast address
 *                 will be returned.
 *  \param[in]     IpCtrlIdx     ip controller index
 *  \param[in]     CtrlAddrIdx   index of the controller address that will be used for address resolution (NS) packets.
 *                               (may be IPV6_CTRL_ADDR_IDX_INV)
 *  \param[in]     DstAddrPtr    IPv6 network address of the node
 *  \param[out]    PhysAddrPtr   link layer address of the node
 *  \param[in,out] NcEntryIdxPtr neighbor cache index. If CACHE_LOOKUP_OPTIMIZATION is enabled this index will be used
 *                               as lookup hint.
 *  \return        E_OK          link-layer address known
 *  \return        E_NOT_OK      link-layer (currently) unknown, address resolution triggered
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VResolveLinkLayerAddr(
  uint8                                             IpCtrlIdx,
  IpV6_CtrlAddrIdxType                              CtrlAddrIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR)            PhysAddrPtr,
  P2VAR(IpV6_ListIdxType, AUTOMATIC, IPV6_APPL_VAR) NcEntryIdxPtr)
{
  Std_ReturnType Result;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 5 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != PhysAddrPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
                                         /* CtrlAddrIdx may be IPV6_CTRL_ADDR_IDX_INV, this parameter is optional */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr,            IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != NcEntryIdxPtr,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
#endif

  Result = E_OK;

  if (IPV6_ADDR_IS_MULTICAST(*DstAddrPtr))
  {
    /* map IPv6 multicast address to ethernet multicast address according to RFC 2464 section 7 */
    IpV6_VMulticastAddr2LinkLayerAddr(PhysAddrPtr, DstAddrPtr);
    return E_OK;
  }

  IPV6_BEGIN_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* lookup neighbor cache for link-layer address of the next hop neighbor */
  IpV6_Ndp_VNCGetOrCreateEntry(IpCtrlIdx, DstAddrPtr, NcEntryIdxPtr);

  if ((IPV6_REACHABLE_STATE_INCOMPLETE != IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].State))
  {
    /* ok, we can send the packet */
    IPV6_LL_ADDR_COPY(PhysAddrPtr, &IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].LinkLayerAddress[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */

    /* start NUD */
    if (IPV6_REACHABLE_STATE_STALE == IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].State)
    {
      /* delay sending of NUD probes */
      IpV6_Ndp_VNCUpdateEntry(IpCtrlIdx, *NcEntryIdxPtr, IPV6_REACHABLE_STATE_DELAY);
      IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].RetryCount = 0;
      IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].NextEventTime, IpV6_VCfgGetNudFirstProbeDelay());
      IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].NsSrcAddressIdx = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
    }
  }
  else
  {
    /* address resolution in progress */

    IPV6_CTRL_VAR(IpV6_NeighborCache)[*NcEntryIdxPtr].NsSrcAddressIdx = CtrlAddrIdx;

    Result = E_NOT_OK;
  }

  IPV6_END_CRITICAL_SECTION_NC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VResolveLinkLayerAddr() */

/**********************************************************************************************************************
 *  IpV6_VIngressFilter()
 **********************************************************************************************************************/
/*! \brief     Check if a given destination address is one of our local source addresses
 *  \param[in] IpCtrlIdx   ip controller index
 *  \param[in] DstAddrPtr  destination address
 *  \return    E_OK        destination address is one of the source addresses of this node
 *  \return    E_NOT_OK    destination address does not belong to this node, discard packet
 *  \context   Call MUST be protected with IPV6_BEGIN_CRITICAL_SECTION_SAT() or IPV6_BEGIN_CRITICAL_SECTION()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VIngressFilter(
  uint8                          IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)        DstAddrPtr,
  IPV6_P2V(IpV6_CtrlAddrIdxType) CtrlAddrIdxPtr
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
 ,IPV6_P2V(IpV6_DListIdxType)    TempAddrIdxPtr
#endif
#if (IPV6_ENABLE_MLD == STD_ON)
 ,IPV6_P2V(IpV6_DListIdxType)    McAddrIdxPtr
#endif
  )
{
  IpV6_AddrTypeType     DstAddrType;

  /*
   * Check whether destination address is one of the following:
   * - Unicast
   *   - Address in SourceAddressTable | CtrlAddrIdx
   *   - Temporaray Address of an address in SourceAddressTable (PrivExt) | CtrlAddrIdx + TempAddrIdx
   * - Multicast
   *   - All-Nodes Multicast Address | CtrlAddrIdx
   *   - Solicited Node Multicast Address
   *     - If PrivExt are active the node may have multiple interface identifiers an thus multiple solicited node addresses
   *   - Joinded Multicast Address (MLDv2)
   */

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 7 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr,     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != CtrlAddrIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_CheckDetErrorReturnValue(NULL_PTR != TempAddrIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
# endif
# if (IPV6_ENABLE_MLD == STD_ON)
  IpV6_CheckDetErrorReturnValue(NULL_PTR != McAddrIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
# endif
#endif

  /* determine type of destination address */
  DstAddrType = IpV6_VGetAddrType(DstAddrPtr);

  if ((IPV6_ADDR_TYPE_LL_UNICAST == DstAddrType) || (IPV6_ADDR_TYPE_UNICAST == DstAddrType))
  {
    /* packet destination is unicast */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (E_OK == IpV6_VSATLookup(IpCtrlIdx, DstAddrPtr, CtrlAddrIdxPtr, TempAddrIdxPtr))
#else
    if (E_OK == IpV6_VSATLookup(IpCtrlIdx, DstAddrPtr, CtrlAddrIdxPtr))
#endif
    {
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
      IpV6_LocAddrStateType SrcAddrState;

# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      if (IPV6_DLIST_END_IDX != (*TempAddrIdxPtr))
      {
        SrcAddrState = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[(*TempAddrIdxPtr)].State;
      }
      else
# endif
      {
        SrcAddrState = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[(*CtrlAddrIdxPtr)].State;
      }

      if (IPV6_LADDR_STATE_TENTATIVE != SrcAddrState)
#endif
      {
        /* accept packets going to one of our unicast source addresses */
        return E_OK;
      }
    }
  }
  else if (IPV6_ADDR_TYPE_MULTICAST == DstAddrType)
  {
    /* packet destination is multicast */

    if (IPV6_ADDR_MULTICAST_HAS_VALID_SCOPE(*DstAddrPtr))
    {
      /* check for solicited-node prefix [FF02:0:0:0:0:1:FF__:____]/104 */
      if (E_OK == IpV6_VMatchPrefix(DstAddrPtr, &IpV6_PrefixSolicitedNode.Prefix.addr[0], IpV6_PrefixSolicitedNode.PrefixLenBits))
      {
        IpV6_CtrlAddrIdxType  CtrlAddrIdx;
        for (CtrlAddrIdx = 0; CtrlAddrIdx < IpV6_VCfgGetSourceAddressTableSize(); CtrlAddrIdx++)
        {
          /* compare last three bytes of address */

          if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
          {
            /* skip invalid entry */
            continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
          }

          if ((IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address.addr[13] == DstAddrPtr->addr[13]) &&
              (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address.addr[14] == DstAddrPtr->addr[14]) &&
              (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address.addr[15] == DstAddrPtr->addr[15]))
          {
            /* solicited-node multicast for one of our source addresses */
            (*CtrlAddrIdxPtr) = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
            return E_OK;
          }

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
          if (IpV6_VCfgHasPrivExtConfig())
          {
            IpV6_DListIdxType TempAddrIdx = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs.FirstIdx;

            while (IPV6_DLIST_END_IDX != TempAddrIdx)
            {
              if ((IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress.addr[13] == DstAddrPtr->addr[13]) &&
                  (IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress.addr[14] == DstAddrPtr->addr[14]) &&
                  (IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress.addr[15] == DstAddrPtr->addr[15]))
              {
                /* solicited-node multicast for one of our source addresses */
                (*CtrlAddrIdxPtr) = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
                return E_OK;
              }

              IPV6_DLIST_SET_NEXT(TempAddrIdx, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs);
            }
          }
#endif
        }
      }

      /* all-nodes link-local address [FF02:0:0:0:0:0:0:1] */
      if (IPV6_ADDR_EQUALS(*DstAddrPtr, IpV6_AddrAllNodesLL))
      {
        (*CtrlAddrIdxPtr) = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
        return E_OK;
      }

      /* block used for limitation of variable scope */
      {
        uint8 MulticastAddrIdx;

        /* search for match in dynamic multicast addresses */
        for (MulticastAddrIdx = 0; MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_DynamicMulticastAddressCount); MulticastAddrIdx++)
        {
          if (IPV6_ADDR_EQUALS(*DstAddrPtr, IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx]))
          {
            (*CtrlAddrIdxPtr) = IPV6_MULTICAST_ADDR_IDX_TO_CTRL_ADDR_IDX(MulticastAddrIdx);
            return E_OK;
          }
        }

#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
        /* continue search in static multicast addresses (static addresses are stored after dynamic addresses) */
        for (/* Continue with MulticastAddrIdx */; MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_MulticastAddressCount); MulticastAddrIdx++)
        {
          uint8 TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].DefaultAddrIdx;

          if (IPV6_ADDR_EQUALS(*DstAddrPtr, IpV6_DefaultAddresses[TmpDefaultAddrIdx]))
          {
            (*CtrlAddrIdxPtr) = IPV6_MULTICAST_ADDR_IDX_TO_CTRL_ADDR_IDX(MulticastAddrIdx);
            return E_OK;
          }
        }
#endif
      }

#if (IPV6_ENABLE_MLD == STD_ON)
      if (IpV6_VCfgHasMldConfig())
      {
        /* check if there is any membership for this multicast address */
        (*McAddrIdxPtr) = IpV6_Mld_VMcAddrListFind(IpCtrlIdx, DstAddrPtr);

        if (IPV6_DLIST_END_IDX != (*McAddrIdxPtr))
        {
          (*CtrlAddrIdxPtr) = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
          return E_OK;
        }
      }
#endif
    }
    else
    {
      /* Ignore destination address with invalid scope */
    }
  }
  else
  {
   /*
    * Ignore the following destination address types:
    * - Unspecified ::/128
    * - Loopback    ::1/128
    */
  }

  return E_NOT_OK; /* drop all other packets */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VIngressFilter() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_VGetCommonPrefixLen()
 **********************************************************************************************************************/
/*! \brief      Returns longest common prefix length of two addresses in bits
 *  \param[in]  Addr1Ptr  first address
 *  \param[in]  Addr2Ptr  second address
 *  \return     number of common prefix bits
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV6_CODE) IpV6_VGetCommonPrefixLen(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) Addr1Ptr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) Addr2Ptr)
{
  uint8 MatchingBits = 0;
  uint8 AddrByteIdx;
  uint8 diff;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != Addr1Ptr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, 0);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != Addr2Ptr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, 0);
#endif

  for (AddrByteIdx = 0; AddrByteIdx < IPV6_ADDRESS_LEN_BYTE; AddrByteIdx++)
  {
    if (Addr1Ptr->addr[AddrByteIdx] != Addr2Ptr->addr[AddrByteIdx])
    {
      diff = (uint8)(Addr1Ptr->addr[AddrByteIdx] ^ Addr2Ptr->addr[AddrByteIdx]);

      while ((diff & 0x80) != 0) /* 0x80 = 10000000 */
      {
        diff <<= 1;
        MatchingBits++;
      }
      break;
    }

    MatchingBits += IPV6_BITS_IN_BYTE;
  }

  return MatchingBits;
} /* End of IpV6_VGetCommonPrefixLen() */

/**********************************************************************************************************************
 *  IpV6_VHandleUnknownTlvOption()
 **********************************************************************************************************************/
/*! \brief      Called when an unknown option in destination or hop-by-hop extension header has been found
 *  \param[in]  AddrId            ip address identifier
 *  \param[in]  IpV6PktDataPtr    pointer to the IPv6 packet starting with the IPv6 header
 *  \param[in]  IpV6PktDataLen    length of the IPv6 packet (IPv6 header and playload) in bytes
 *  \param[in]  UnknownOptOffset  offset of the unknown option in bytes relative to IpV6PktDataPtr
 *  \return     E_OK              option can be skipped (processing of the packet can be continued)
 *  \return     E_NOT_OK          option must not be skipped (discard whole IPv6 packet)
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VHandleUnknownTlvOption(
  IpV6_AddrIdType  AddrId,
  IPV6_P2C(uint8)  IpV6PktDataPtr,
  uint16           IpV6PktDataLen,
  uint16           UnknownOptOffset)
{
  Std_ReturnType Result;
  uint8 OptType;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != IpV6PktDataPtr,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((UnknownOptOffset <= IpV6PktDataLen), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM, E_NOT_OK);
#endif

  OptType = IpV6PktDataPtr[UnknownOptOffset];
  /* OptLength = IpV6PktDataPtr[UnknownOptOffset+1]; */

  /* if the option is not recognized the two most significant bits of the OptionType indicate what to do */
  switch (OptType >> 6)
  {
    case 0: /* skip over this option and continue processing the header */
      /* just ignore the option */
      Result = E_OK;
    break;

    case 1: /* silently discard the packet */

      Result = E_NOT_OK; /* stop processing the packet */
    break;

    case 2: /* discard the packet, send ICMP parameter problem message Code 2 to source address of the packet */

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
      IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_OPTION, IpV6PktDataPtr, IpV6PktDataLen, UnknownOptOffset);
#endif

      Result = E_NOT_OK; /* stop processing the packet */
    break;

    /* only 4 possible cases */
    default: /* case 3: discard the packet, only if not multicast destination: ICMP Message Parameter problem code 2 */

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
      if (!IPV6_ADDR_IS_MULTICAST(((const IpV6_HdrType*)IpV6PktDataPtr)->DstAddr)) /* PRQA S 0310 */ /* MD_IpV6_0310 */
      {
        IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_OPTION, IpV6PktDataPtr, IpV6PktDataLen, UnknownOptOffset);
      }
#endif

      Result = E_NOT_OK; /* stop processing the packet */
  }

  return Result;
} /* End of IpV6_VHandleUnknownTlvOption() */

/**********************************************************************************************************************
 *  IpV6_MemCmp32()
 **********************************************************************************************************************/
/*! \brief      This method compares two memory areas with 32-bit granularity (32-bit "blocks").
 *  \param[in]  NetAddr1Ptr   first address
 *  \param[in]  NetAddr2Ptr   second address
 *  \return     TRUE          addresses are equal
 *  \return     FALSE         addresses are not equal
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_MemCmp32(
  IPV6_P2C(uint8) Data1Ptr,
  IPV6_P2C(uint8) Data2Ptr,
  uint8 Blocks)
{
  P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA) Data1Ptr32 = (P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA))Data1Ptr; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA) Data2Ptr32 = (P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA))Data2Ptr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != Data1Ptr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, FALSE);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != Data2Ptr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, FALSE);
#endif

  while (0 < Blocks)
  {
    Blocks--;

    if (Data1Ptr32[Blocks] != Data2Ptr32[Blocks])
    {
      return FALSE;
    }
  }

  return TRUE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_MemCmp32() */

/**********************************************************************************************************************
 *  IpV6_CmpNetAddr()
 **********************************************************************************************************************/
/*! \brief      This method compares two IPv6 addresses.
 *  \param[in]  NetAddr1Ptr   first address
 *  \param[in]  NetAddr2Ptr   second address
 *  \return     TRUE          addresses are equal
 *  \return     FALSE         addresses are not equal
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_CmpNetAddr(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) NetAddr1Ptr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) NetAddr2Ptr)
{
  P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA) NetAddr1Ptr32 = (P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA))NetAddr1Ptr; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA) NetAddr2Ptr32 = (P2CONST(uint32, AUTOMATIC, IPV6_APPL_DATA))NetAddr2Ptr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != NetAddr1Ptr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, FALSE);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != NetAddr2Ptr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, FALSE);
#endif

  if ((NetAddr1Ptr32[3] != NetAddr2Ptr32[3]) ||
      (NetAddr1Ptr32[2] != NetAddr2Ptr32[2]) ||
      (NetAddr1Ptr32[1] != NetAddr2Ptr32[1]) ||
      (NetAddr1Ptr32[0] != NetAddr2Ptr32[0]))
  {
    return FALSE;
  }

  return TRUE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_CmpNetAddr() */

/**********************************************************************************************************************
 *  IpV6_VGetSolicitedNodeAddr()
 **********************************************************************************************************************/
/*! \brief          Transform an IPv6 address into a special address type
 *  \param[in,out]  NetAddrPtr pointer to the address
 *  \param[in]      Scope   scope of the address
 *  \param[in]      Type    type of the address
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VGetSolicitedNodeAddr(
  IPV6_P2V(IpV6_AddrType) MulticastAddrPtr,
  IPV6_P2C(IpV6_AddrType) NetAddrPtr,
  IpV6_MulticastScopeType Scope)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != MulticastAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  MulticastAddrPtr->addr[0] = (uint8)0xFF;
  MulticastAddrPtr->addr[1] = (uint8)(Scope & 0x0F);

  IpV6_MemSet(&MulticastAddrPtr->addr[2], 0, 9);

  MulticastAddrPtr->addr[11] = 1;
  MulticastAddrPtr->addr[12] = 0xFF;
  MulticastAddrPtr->addr[13] = NetAddrPtr->addr[13];
  MulticastAddrPtr->addr[14] = NetAddrPtr->addr[14];
  MulticastAddrPtr->addr[15] = NetAddrPtr->addr[15];
} /* End of IpV6_VGetSolicitedNodeAddr() */

/**********************************************************************************************************************
 *  IpV6_MemSet()
 **********************************************************************************************************************/
/*! \brief      Fill memory with a specific value
 *  \param[out] TargetPtr  pointer to start of memory segemnt
 *  \param[in]  Value      value that will be written to each byte of the memory segment
 *  \param[in]  Length     length of the memory segment in bytes
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_MemSet(
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_DATA) TargetPtr,
  uint8 Value,
  uint16 Length)
{
  uint16 i;

  for (i = 0; i < Length; i++)
  {
    TargetPtr[i] = Value;
  }
} /* End of IpV6_MemSet() */

/**********************************************************************************************************************
 *  IpV6_VGetAddrType()
 **********************************************************************************************************************/
/*! \brief        Determine the type of an IPv6 address according to RFC4291 2.4.
 *  \param[in]    pointer to the IPv6 address
 *  \return       IPV6_ADDR_TYPE_MULTICAST    address is a multicast address
 *  \return       IPV6_ADDR_TYPE_LL_UNICAST   address is a link-local unicast address
 *  \return       IPV6_ADDR_TYPE_LOOPBACK     address is the local loopback address
 *  \return       IPV6_ADDR_TYPE_UNSPECIFIED  address is the unspecified (all-zeros) address
 *  \return       IPV6_ADDR_TYPE_UNICAST      address is a unicast address (none of the special types above)
 **********************************************************************************************************************/
FUNC(IpV6_AddrTypeType, IPV6_CODE) IpV6_VGetAddrType(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) AddrPtr)
{
  IpV6_AddrTypeType AddrType;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_ADDR_TYPE_INVALID);
#endif

  if (IPV6_ADDR_IS_MULTICAST(*AddrPtr)) /* multicast prefix: FF00::/8 */
  {
    AddrType = IPV6_ADDR_TYPE_MULTICAST;
  }
  else if (IPV6_ADDR_IS_LINK_LOCAL(*AddrPtr)) /* link-local unicast prefix: FE80::/10 */
  {
    AddrType = IPV6_ADDR_TYPE_LL_UNICAST;
  }
  else if (IPV6_ADDR_IS_LOOPBACK(*AddrPtr)) /* loopback address ::1/128 */
  {
    AddrType = IPV6_ADDR_TYPE_LOOPBACK;
  }
  else if (IPV6_ADDR_IS_UNSPECIFIED(*AddrPtr)) /* unspecified address ::/128 */
  {
    AddrType = IPV6_ADDR_TYPE_UNSPECIFIED;
  }
  else
  {
    AddrType = IPV6_ADDR_TYPE_UNICAST;
  }

  return AddrType;
} /* End of IpV6_VGetAddrType() */

/* PREFIX LIST FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VMatchPrefix()
 **********************************************************************************************************************/
/*! \brief     Determine if a given prefix does match to a specific address
 *  \param[in] AddrPtr    pointer to the address that will be machted against the prefix
 *  \param[in] PrefixPtr  pointer to the prefix
 *  \return    TRUE       the address starts with the given prefix
 *  \return    FALSE      the address does not start with the given prefix
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VMatchPrefix(
  IPV6_P2C(IpV6_AddrType) AddrPtr,
  IPV6_P2C(uint8)         PrefixPtr,
  uint8                   PrefixLenBits)
{
  uint8 i = 0;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr,   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != PrefixPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* compare first n bytes of the prefix */
  while (IPV6_BITS_IN_BYTE <= PrefixLenBits)
  {
    if (AddrPtr->addr[i] != PrefixPtr[i])
    {
      return E_NOT_OK;
    }

    i++;
    PrefixLenBits -= IPV6_BITS_IN_BYTE;
  }

  /* if prefix length is not multiple of 8, we have to compare the first PrefixLenBits bits of the next byte */
  if (0 < PrefixLenBits)
  {
    uint8 ByteMask;
    /* create a mask that masks the first PrefixLenBits bits in a byte */
    ByteMask = (uint8)(0xFFu << (IPV6_BITS_IN_BYTE - PrefixLenBits)); /* 11..00.. */

    /* check if the first PrefixLenBits of prefix and address are equal */
    if ((AddrPtr->addr[i] & ByteMask) != (PrefixPtr[i] & ByteMask))
    {
      return E_NOT_OK;
    }
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VMatchPrefix() */

/**********************************************************************************************************************
 *  IpV6_VIsStaticOnLinkPrefix()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, IPV6_CODE) IpV6_VIsStaticOnLinkPrefix(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) PrefixPtr,
  uint8                   PrefixBitLen)
{
  uint8 ctrlAddrIdx;

#if (IPV6_ENABLE_STATIC_ON_LINK_PREFIXES == STD_ON)
  /* #10 Determine whether the given prefix is covered by one of the statically configured on-link prefixes of the controller. */

  if (IpV6_VCfgGetStaticOnLinkPrefixCount() > 0)
  {
    uint8_least prefixListIdx;

    for (prefixListIdx = 0; prefixListIdx < IpV6_VCfgGetStaticOnLinkPrefixCount(); prefixListIdx++)
    {
      if (IpV6_VCfgGetStaticOnLinkPrefixLength(prefixListIdx) <= PrefixBitLen)
      {
        if (E_OK == IpV6_VMatchPrefix(PrefixPtr, &IpV6_VCfgGetStaticOnLinkPrefixValue(prefixListIdx).addr[0], IpV6_VCfgGetStaticOnLinkPrefixLength(prefixListIdx)))
        {
          /* Given prefix is covered by one of the statically configured on-link address prefixes. */
          return TRUE;
        }
      }
    }
  }
#else
  IPV6_UNUSED_PARAMETER(PrefixBitLen);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  /* #20 Determine whether the given prefix is covered by the prefix of a local source address that has been marked to be on-link in the configuration. */

  for (ctrlAddrIdx = 0; ctrlAddrIdx < IpV6_VCfgGetSourceAddressTableSize(); ctrlAddrIdx++)
  {
    if (IPV6_CTRL_VAR(IpV6_SourceAddressTable)[ctrlAddrIdx].State != IPV6_LADDR_STATE_INVALID)
    {
      if (IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[ctrlAddrIdx].PrefixIsOnLink == TRUE)
      {
        if (IPV6_CMP_64BIT_PREFIX(*PrefixPtr, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[ctrlAddrIdx].Address) == TRUE)
        {
          /* Given prefix is covered by the prefix of a statically configured source address with an on-link prefix. */
          return TRUE;
        }
      }
    }
  }

  return FALSE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VIsStaticOnLinkPrefix() */

/**********************************************************************************************************************
 *  IpV6_VIsDestinationOnLink()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, IPV6_CODE) IpV6_VIsDestinationOnLink(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr)
{
  IpV6_AddrTypeType DstAddrType;
  IpV6_ListIdxType  PlEntryIdx;

  DstAddrType = IpV6_VGetAddrType(DstAddrPtr);

  /* #10 Check whether destination address is a link-local fe80::/10 or multicast address. */

  /* we have to determine the next hop neighbor first */
  if ((IPV6_ADDR_TYPE_MULTICAST == DstAddrType) || (IPV6_ADDR_TYPE_LL_UNICAST == DstAddrType)) /* multicast and link local prefix is always on-link */
  {
    return TRUE;
  }

  /* #20 Check whether destination address is covered by a statically configured on-link prefix. */

  if (IpV6_VIsStaticOnLinkPrefix(IpCtrlIdx, DstAddrPtr, 128) == TRUE)
  {
    return TRUE;
  }

  /* #30 Check whether destination address is covered by a prefix in the dynamic on-link prefix list. */

  PlEntryIdx = IPV6_CTRL_VAR(IpV6_PrefixListEntryCount);

  while (0 < PlEntryIdx)
  {
    PlEntryIdx--;

    if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_PrefixList)[PlEntryIdx].ExpireTime))
    {
      IpV6_Ndp_VPLRemoveEntry(IpCtrlIdx, PlEntryIdx);
    }
    else
    {
      if (E_OK == IpV6_VMatchPrefix(DstAddrPtr, &IPV6_CTRL_VAR(IpV6_PrefixList)[PlEntryIdx].Prefix.addr[0], IPV6_CTRL_VAR(IpV6_PrefixList)[PlEntryIdx].PrefixLenBits))
      {
        /* destination is on-link, so next hop is the destination node */
        return TRUE;
      }
    }
  }

  return FALSE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VIsDestinationOnLink() */

/* DEFAULT ROUTER TABLE FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VGetDefaultRouterAddr()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA), IPV6_CODE) IpV6_VGetDefaultRouterAddr(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr,
  IPV6_P2V(boolean)       IsReachable)
{
  IpV6_ListIdxType EntryIdx;
  IpV6_ListIdxType NcEntryIdx;
  boolean ReachableRouterFound = FALSE;

  IPV6_UNUSED_PARAMETER(DstAddrPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Check if list of default routers is empty. */

  if (0 == IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount))
  {
    return (IpV6_AddrType*)NULL_PTR;
  }

  /* #20 Iterate over list of default routers and stop if a reachable router is found. */

  EntryIdx = IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount);

  while (0 < EntryIdx)
  {
    EntryIdx--;

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
    NcEntryIdx = IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].NcIdxHint;
#endif

    if (E_OK == IpV6_Ndp_VNCLookup(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].NextHopAddress, &NcEntryIdx))
    {
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
      /* store updated neighbor cache index */
      IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].NcIdxHint = NcEntryIdx;
#endif

      if (IPV6_REACHABLE_STATE_INCOMPLETE != IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State)
      {
        /* we found a router which is known to be reachable. select this router. */
        ReachableRouterFound = TRUE;
        break;
      }
    }
  }

  /* #30 If not reachable router was found select default router from the list using round-robin strategy. */

  if (FALSE == ReachableRouterFound)
  {
    IPV6_CTRL_VAR(IpV6_NextRouterProbeIdx)++;

    if (IPV6_CTRL_VAR(IpV6_NextRouterProbeIdx) < IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount))
    {
      EntryIdx = IPV6_CTRL_VAR(IpV6_NextRouterProbeIdx);
    }
    else
    {
      EntryIdx = 0;
      IPV6_CTRL_VAR(IpV6_NextRouterProbeIdx) = 0;
    }
  }

  (*IsReachable) = ReachableRouterFound;
  return &IPV6_CTRL_VAR(IpV6_DefaultRouterList)[EntryIdx].NextHopAddress;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VGetDefaultRouterAddr() */

/* SOURCE ADDRESS HANDLING FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VBuildSourceAddress()
 **********************************************************************************************************************/
/*! \brief      Create a source IPv6 address from an interface identifier and prefix
 *  \param[in]  IfIdPtr       pointer to the interface identifier
 *  \param[in]  PrefixPtr     pointer to an IPv6 address that starts with the given prefix
 *  \param[in]  PrefixLenBits length of the prifix in bits (1-128)
 *  \param[out] SrcAddrPtr    pointer to an IPv6 address variable where the source address is written to
 *  \return     E_OK          no error
 *  \return     E_NOT_OK      the prefix length does not fit to the interface identifier length
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VBuildSourceAddress(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA)         IfIdPtr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) PrefixPtr,
  P2VAR(IpV6_AddrType, AUTOMATIC, IPV6_APPL_VAR)    SrcAddrPtr) /* PRQA S 3673 */ /* This parameter is changed by IpV6_MemCpy() */
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != PrefixPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  IpV6_MemCpy(&SrcAddrPtr->addr[0],                    &PrefixPtr->addr[0], IPV6_PREFIX_LEN_BYTE);               /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IpV6_MemCpy(&SrcAddrPtr->addr[IPV6_PREFIX_LEN_BYTE], &IfIdPtr[0],         IPV6_INTERFACE_IDENTIFIER_LEN_BYTE); /* PRQA S 0310 */ /* MD_IpV6_0310 */

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VBuildSourceAddress() */

/**********************************************************************************************************************
 *  IpV6_VIsValidSourceAddress()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*/
FUNC(boolean, IPV6_CODE) IpV6_VIsValidSourceAddress(
  uint8                           IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)         SrcAddrPtr,
  boolean                         AllowUnspecAddr,
  IPV6_P2V(IpV6_LocAddrStateType) SrcAddrStatePtr,
  IPV6_P2V(IpV6_CtrlAddrIdxType)  CtrlAddrIdxPtr)
{
  Std_ReturnType Result;
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListIdxType SatTempAddrIdx;
#endif

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (E_OK == IpV6_VSATLookup(IpCtrlIdx, SrcAddrPtr, CtrlAddrIdxPtr, &SatTempAddrIdx))
#else
  if (E_OK == IpV6_VSATLookup(IpCtrlIdx, SrcAddrPtr, CtrlAddrIdxPtr))
#endif
  {
    IpV6_LocAddrStateType SrcAddrState;

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (IPV6_DLIST_END_IDX != SatTempAddrIdx)
    {
      SrcAddrState = IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[SatTempAddrIdx].State;
    }
    else
#endif
    {
      SrcAddrState = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[*CtrlAddrIdxPtr].State;
    }

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    if (IPV6_LADDR_STATE_TENTATIVE == SrcAddrState)
    {
      Result = FALSE;
    }
    else
#endif
    {
      Result = TRUE;
    }

    (*SrcAddrStatePtr) = SrcAddrState;
  }
  else if ((TRUE == AllowUnspecAddr) && IPV6_ADDR_IS_UNSPECIFIED(*SrcAddrPtr)) /* PRQA S 3415 */ /* MD_IpV6_3415 */
  {
    (*CtrlAddrIdxPtr)  = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
    (*SrcAddrStatePtr) = IPV6_LADDR_STATE_INVALID;
    Result = TRUE;
  }
  else
  {
    (*CtrlAddrIdxPtr)  = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
    (*SrcAddrStatePtr) = IPV6_LADDR_STATE_INVALID;
    Result = FALSE;
  }

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return Result;
} /* End of IpV6_VIsValidSourceAddress() */

/**********************************************************************************************************************
 *  IpV6_VSourceAddrIsBetterThan()
 **********************************************************************************************************************/
/*! \brief      Decide which of two source addresses should be preferred to reach a specific destination.
 *              (see RFC 3484 5.)
 *  \param[in]  IpCtrlIdx    ip controller index
 *  \param[in]  SrcAddrIdxA  source address table index of first address
 *  \param[in]  SrcAddrIdxB  source address table index of second address
 *  \param[out] DstAddrPtr   destination address
 *  \return     IPV6_CMP_RES_PREFER_A      use address A
 *  \return     IPV6_CMP_RES_PREFER_B      use address B
 *  \return     IPV6_CMP_RES_NO_PREFERENCE no preference
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV6_CODE) IpV6_VSourceAddrIsBetterThan(
  uint8                IpCtrlIdx,
  IpV6_CtrlAddrIdxType SrcAddrIdxA,
  IpV6_CtrlAddrIdxType SrcAddrIdxB,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr)
{
  uint8 PrefixLenA;
  uint8 PrefixLenB;
  IpV6_SourceAddressTableEntryType *AddrA;
  IpV6_SourceAddressTableEntryType *AddrB;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx),  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, 0);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SAT_IDX(SrcAddrIdxA), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX, 0);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SAT_IDX(SrcAddrIdxB), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX, 0);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr,             IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  0);
#endif

  AddrA = &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[SrcAddrIdxA];
  AddrB = &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[SrcAddrIdxB];

  if (SrcAddrIdxA == SrcAddrIdxB)
  {
    return IPV6_CMP_RES_NO_PREFERENCE;
  }

  /* Rule 1: Prefer same address */
  if (IPV6_ADDR_EQUALS(AddrA->Address, *DstAddrPtr))
  {
    return IPV6_CMP_RES_PREFER_A;
  }
  else if (IPV6_ADDR_EQUALS(AddrB->Address, *DstAddrPtr))
  {
    return IPV6_CMP_RES_PREFER_B;
  }
  else
  {
    /* no preference so far, goto next rule */
  }

  /* Rule 2: Prefer appropriate scope */

  /* The scope of in IPv6 unicast address can be either link-local or global.
   * The deprecated site-local scope is handled like global scope according
   * to [RFC4291 2.5.7. Site-Local IPv6 Unicast Addresses]
   */

  if ((IPV6_CTRL_ADDR_IDX_LINK_LOCAL == SrcAddrIdxA) || (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == SrcAddrIdxB))
  {
    /* One of the addresses A or B is the link-local address, so the other address must have bigger scope.
     * If scope of destination address is bigger than link-local we have to prefer the non-link-local address,
     * otherwise we have to prefer the link-local address.
     */

    uint8 DstAddrScope;

    /* determine scope of destination address */
    if (IPV6_ADDR_IS_MULTICAST(*DstAddrPtr))
    {
      DstAddrScope = IPV6_ADDR_MULTICAST_SCOPE(*DstAddrPtr);
    }
    else if (IPV6_ADDR_IS_LINK_LOCAL(*DstAddrPtr))
    {
      /* unicast link-local is mapped to multicast link-local scope [RFC3484 3.1. Scope Comparisons] */
      DstAddrScope = IPV6_MULTICAST_SCOPE_LINK_LOCAL;
    }
    else
    {
      /* unicast global mapped to multicast global scope [RFC3484 3.1. Scope Comparisons] */
      DstAddrScope = IPV6_MULTICAST_SCOPE_GLOBAL;
    }

    if (IPV6_MULTICAST_SCOPE_LINK_LOCAL < DstAddrScope)
    {
      /* scope of destination address is greater than link-local */
      if (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == SrcAddrIdxA)
      {
        return IPV6_CMP_RES_PREFER_B;
      }
      else
      {
        return IPV6_CMP_RES_PREFER_A;
      }
    }
    else
    {
      /* scope of destination address is not greater than link-local */
      if (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == SrcAddrIdxA)
      {
        return IPV6_CMP_RES_PREFER_A;
      }
      else
      {
        return IPV6_CMP_RES_PREFER_B;
      }
    }
  }
  else
  {
    /* Source address A and B have same scope. No preference so far, goto next rule */
  }

  /* Rule 3: Avoid deprecated addresses */
  if ((IPV6_LADDR_STATE_PREFERRED == AddrA->State) && (IPV6_LADDR_STATE_PREFERRED != AddrB->State))
  {
    return IPV6_CMP_RES_PREFER_A;
  }

  if ((IPV6_LADDR_STATE_PREFERRED == AddrB->State) && (IPV6_LADDR_STATE_PREFERRED != AddrA->State))
  {
    return IPV6_CMP_RES_PREFER_B;
  }

  /* Rule 4: Prefer home addresses */
  /* NOT IMPLEMENTED */

  /* Rule 5: Prefer outgoing interface */
  /* ONLY ONE INTERFACE SUPPORTED */

  /* Rule 6: Prefer matching label */

  /* Rule 7: Prefer public addresses (over temporary addresses) */
  /* Rule implemented outside this function */

  /* Rule 8: Use longest matching prefix */

  PrefixLenA = IpV6_VGetCommonPrefixLen(&AddrA->Address, DstAddrPtr);
  PrefixLenB = IpV6_VGetCommonPrefixLen(&AddrB->Address, DstAddrPtr);

  if (PrefixLenA > PrefixLenB)
  {
    return IPV6_CMP_RES_PREFER_A; /* Select source address A */
  }

  if (PrefixLenB > PrefixLenA)
  {
    return IPV6_CMP_RES_PREFER_B; /* Select source address B */
  }

  return IPV6_CMP_RES_NO_PREFERENCE; /* tie */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSourceAddrIsBetterThan() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  IpV6_VSelectSourceAddress()
 **********************************************************************************************************************/
/*! \brief      Get the index of the source address that should be used to reach a specific destination.
 *  \param[in]  IpCtrlIdx    ip controller index
 *  \param[in]  DstAddrPtr   IPv6 address of the destination that shall be reached
 *  \param[out] IdxPtr       index of the source address table entry
 *  \return     E_OK         suitable address found (Idx is valid)
 *  \return     E_NOT_OK     no suitable address found (Idx unchanged)
 **********************************************************************************************************************/
STATIC FUNC(IpV6_CtrlAddrIdxType, IPV6_CODE) IpV6_VSelectSourceAddress(
  uint8                           IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)         DstAddrPtr)
{
  IpV6_CtrlAddrIdxType    CtrlAddrIdx;
  IpV6_CtrlAddrIdxType    SelectedCtrlAddrIdx;
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListIdxType       TempAddrIdx;
#endif

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_CTRL_ADDR_IDX_INV_OR_ANY);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr,            IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, IPV6_CTRL_ADDR_IDX_INV_OR_ANY);
#endif

  if (IPV6_CTRL_STATE_LLADDR_READY > IPV6_CTRL_VAR(IpV6_CtrlState))
  {
    /* we have no source adresses */
    return IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
  }

  SelectedCtrlAddrIdx = 0; /* default: Use first source address in SourceAddressTable, this is always our one and only link-local address */

  /* if we have more source addresses, see if we should prefer one of them */
  for(CtrlAddrIdx = 1; CtrlAddrIdx < IpV6_VCfgGetSourceAddressTableSize(); CtrlAddrIdx++)
  {
    if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      /* skip invalid entry */
      continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
    }

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    if (IPV6_LADDR_STATE_TENTATIVE != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
#endif
    {
      if (IPV6_CMP_RES_PREFER_A == IpV6_VSourceAddrIsBetterThan(IpCtrlIdx, CtrlAddrIdx, SelectedCtrlAddrIdx, DstAddrPtr))
      {
        SelectedCtrlAddrIdx = CtrlAddrIdx;
      }
    }
  }

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (IpV6_VCfgHasPrivExtConfig())
  {
    /* We should prefer a temoraray address if there is one configured for the selected address
     * [RFC6724 Rule 7: Prefer temporary addresses]
     */

    /* iterate over temporary addresses */
    TempAddrIdx = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[SelectedCtrlAddrIdx].TempAddrs.FirstIdx;

    while (IPV6_DLIST_END_IDX != TempAddrIdx)
    {
  # if (IPV6_ENABLE_NDP_DAD == STD_ON)
      if (IPV6_LADDR_STATE_PREFERRED == IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].State)
  # endif
      {
        /* The most recently created temporaray address is always the first element in the list.
         * We have to consider only the first non-tentative address here, since the older entries will not result in better matches.
         * The break statement will leave TempAddrIdx on a valid value. Otherwise the value will be IPV6_DLIST_END_IDX to indicate
         * there was no useable temporary address.
         */
        break;
      }

      IPV6_DLIST_SET_NEXT(TempAddrIdx, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[SelectedCtrlAddrIdx].TempAddrs);
    }
  }
#endif

  /* verfiy that scope of selected source address is big enough for destination address */
  if (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == SelectedCtrlAddrIdx) /* link-local address shall be used as source address? */
  {
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    if (IPV6_LADDR_STATE_TENTATIVE == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_ADDR_IDX_LINK_LOCAL].State)
    {
      return IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
    }
#endif

    if ((!IPV6_ADDR_IS_LINK_LOCAL(*DstAddrPtr)) && (!(IPV6_ADDR_IS_MULTICAST(*DstAddrPtr) && (IPV6_ADDR_MULTICAST_SCOPE(*DstAddrPtr) <= IPV6_MULTICAST_SCOPE_LINK_LOCAL))))
    {
      return IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
    }
  }

  return SelectedCtrlAddrIdx;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSelectSourceAddress() */

/**********************************************************************************************************************
 *  IpV6_VSATLookupPrefix()
 **********************************************************************************************************************/
/*! \brief      Lookup index of an entry in the source address table by address prefix
 *  \param[in]     IpCtrlIdx      ip controller index
 *  \param[in]     AddrPrefixPtr  ip address prefix (64bit)
 *  \param[in,out] CtrlAddrIdxPtr in: start index, out: matching index
 *  \param[in]     AddrCfgSrc     Filter for specific configuration source of address.
 *                                (If set to IPV6_ADDR_CFG_SRC_UNSPECIFIED, CfgSrc is ignored)
 *  \return     E_OK      entry has been found (EntryIdx is valid)
 *  \return     E_NOT_OK  entry not found (EntryIdx unchanged)
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSATLookupPrefix(
  uint8                          IpCtrlIdx,
  IPV6_P2C(uint8)                AddrPrefixPtr,
  IPV6_P2V(IpV6_CtrlAddrIdxType) CtrlAddrIdxPtr,
  uint8                          AddrCfgSrc)
{
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPrefixPtr,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != CtrlAddrIdxPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER,  E_NOT_OK);
#endif

  CtrlAddrIdx = *CtrlAddrIdxPtr;

  while (CtrlAddrIdx < IpV6_VCfgGetSourceAddressTableSize())
  {
    if (IPV6_LADDR_STATE_INVALID != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      if (TRUE == IpV6_MemCmp32(&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address.addr[0], AddrPrefixPtr, (IPV6_PREFIX_LEN_BYTE / 4)))
      {
        if ((IPV6_ADDR_CFG_SRC_UNSPECIFIED == AddrCfgSrc) || (IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].AddressAssignVariant == AddrCfgSrc))
        {
          *CtrlAddrIdxPtr = CtrlAddrIdx;
          return E_OK;
        }
      }
    }

    CtrlAddrIdx++;
  }

  return E_NOT_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSATLookupPrefix() */

/**********************************************************************************************************************
 *  IpV6_VSATLookup()
 **********************************************************************************************************************/
/*! \brief         Lookup index of an entry in the source address table by address
 *  \param[in]     IpCtrlIdx         ip controller index
 *  \param[in]     SrcAddrPtr        ip address
 *  \param[in,out] CtrlAddrIdxPtr    in: start index, out: matching index
 *  \param[out]    SatTempAddrIdxPtr index of the temporary address
 *  \return     E_OK      entry has been found (EntryIdx is valid)
 *  \return     E_NOT_OK  entry not found (EntryIdx unchanged)
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSATLookup(
  uint8                                                 IpCtrlIdx,
  P2CONST(IpV6_AddrType,      AUTOMATIC, IPV6_APPL_VAR) SrcAddrPtr,
  P2VAR(IpV6_CtrlAddrIdxType, AUTOMATIC, IPV6_APPL_VAR) CtrlAddrIdxPtr
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
 ,P2VAR(IpV6_DListIdxType, AUTOMATIC, IPV6_APPL_VAR)    SatTempAddrIdxPtr
#endif
  )
{
  Std_ReturnType Result = E_NOT_OK;
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrPtr,     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != CtrlAddrIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

  while ((E_NOT_OK == Result) && (0 < CtrlAddrIdx))
  {
    CtrlAddrIdx--;

    if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      /* skip invalid entry */
      continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
    }

    if (TRUE == IPV6_CMP_64BIT_PREFIX(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address, *SrcAddrPtr))
    {
      /* prefix known */
      (*CtrlAddrIdxPtr) = CtrlAddrIdx;

      if (TRUE == IPV6_CMP_64BIT_INTERFACE_IDENTIFIER(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address, *SrcAddrPtr))
      {
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
        (*SatTempAddrIdxPtr) = IPV6_DLIST_END_IDX;
#endif
        Result = E_OK;
      }
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      else if (IpV6_VCfgHasPrivExtConfig())
      {
        IpV6_DListIdxType SatTempAddrIdx;

        SatTempAddrIdx = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs.FirstIdx;

        while (IPV6_DLIST_END_IDX != SatTempAddrIdx)
        {
          if (TRUE == IPV6_CMP_64BIT_INTERFACE_IDENTIFIER(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[SatTempAddrIdx].TempAddress, *SrcAddrPtr))
          {
            (*SatTempAddrIdxPtr) = SatTempAddrIdx;
            Result = E_OK;
            break;
          }

          IPV6_DLIST_SET_NEXT(SatTempAddrIdx, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs);
        }
      }
#endif
      else
      {
        /* interface identifier odes not match */
      }
    }
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSATLookup() */

/**********************************************************************************************************************
 *  IpV6_VSATRemoveEntry()
 **********************************************************************************************************************/
/*! \brief      Remove an entry from the source address table
 *  \param[in]  ip address index
 *  \context    Call MUST be protected with IPV6_BEGIN_CRITICAL_SECTION_SAT() or IPV6_BEGIN_CRITICAL_SECTION()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VSATRemoveEntry(uint8 IpCtrlIdx, IpV6_CtrlAddrIdxType CtrlAddrIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_SAT_IDX(CtrlAddrIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX);
#endif

  IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State = IPV6_LADDR_STATE_INVALID;

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (IpV6_VCfgHasPrivExtConfig())
  {
    /* deconfigure all temporaray addresses */
    IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs);
  }
#endif

} /* End of IpV6_VSATRemoveEntry() */

/**********************************************************************************************************************
 *  IpV6_VSATRemoveAllEntrys()
 **********************************************************************************************************************/
/*! \brief      Remove all entrys from the source address table (init)
 *  \context    Call MUST be protected with IPV6_BEGIN_CRITICAL_SECTION_SAT() or IPV6_BEGIN_CRITICAL_SECTION()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VSATRemoveAllEntrys(uint8 IpCtrlIdx)
{
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

  /* clear all temporaray address lists */
  CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

  while (0 < CtrlAddrIdx)
  {
    CtrlAddrIdx--;

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (IpV6_VCfgHasPrivExtConfig())
    {
      /* deconfigure all temporary addresses */
      IpV6_DListClear(&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs);
    }
#endif

    IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State = IPV6_LADDR_STATE_INVALID;
  }
} /* End of IpV6_VSATRemoveAllEntrys() */

/**********************************************************************************************************************
 *  IpV6_VHandleSourceAddrAssignmentChange()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VHandleSourceAddrAssignmentChange(uint8 IpCtrlIdx, uint8 CtrlAddrIdx, boolean AddrAssigned)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_CTRL_IDX);
#endif

  if (!IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* ERROR: This function may only be called with AddrIds referencing unicast addresses */
    IpV6_DetReportInternalError(IPV6_E_INV_ADDR_ID);
    return;
  }

  if (TRUE == AddrAssigned)
  {
    /* Address is now ready for use */

    if (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == CtrlAddrIdx)
    {
      /* Autoconfigured link-local address fe80::/10 */

      if (IPV6_CTRL_STATE_LLADDR_READY > IPV6_CTRL_VAR(IpV6_CtrlState))
      {
        IPV6_CHANGE_STATE(IPV6_CTRL_STATE_LLADDR_READY);

#if (IPV6_ENABLE_MLD == STD_ON)
        if (IpV6_VCfgHasMldConfig())
        {
          IpV6_AddrType LLSolNodeAddr; /* PRQA S 0759 */ /* MD_IpV6_0759 */

          /* "Once a valid link-local address is available, a node SHOULD
           *  generate new MLDv2 Report messages for all multicast addresses joined
           *  on the interface."
           *  [RFC3810 5.2.13. Source Addresses for Reports]
           */
          IpV6_VGetSolicitedNodeAddr(&LLSolNodeAddr, &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address, IPV6_MULTICAST_SCOPE_LINK_LOCAL);

          IpV6_Mld_VClearMcAddrEntry2(IpCtrlIdx, &LLSolNodeAddr);

          (void)IpV6_Mld_MulticastListen(IpCtrlIdx, 0, &LLSolNodeAddr, (const IpV6_AddrType*)NULL_PTR, 0, FALSE);
        }
#endif
      }

      /* after the link-local address is ready we start sending router solicitations (if enabled) */
    }
    else
    {
      /* Global address is ready */
      if (IPV6_CTRL_STATE_GBLADDR_READY > IPV6_CTRL_VAR(IpV6_CtrlState))
      {
        IPV6_CHANGE_STATE(IPV6_CTRL_STATE_GBLADDR_READY);
      }
    }
  }
  else
  {
    /* Address is no longer ready for use */

#if (IPV6_ENABLE_DHCP == STD_ON)
    if (IPV6_DHCPV6_MODE_DISABLED != IpV6_VCfgGetDhcpMode())
    {
      (void)TcpIp_DhcpV6_Reset(IpCtrlIdx);
    }
#endif

    /* Leave solicited node multicast group of this address.
     * Note: The same interface identifier may be used for multiple source addresses.
     *       So the IpV6 will join and leave the same solicited-node multicast group multiple times.
     *       The number of "joins" is consistent with the number of "leaves" for the same multicast group.
     */
    IPV6_VJOIN_SOLICITED_NODE_MULTICAST_GROUP(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address, FALSE);
  }

#if (IPV6_ENABLE_TCPIP == STD_ON)
  /* Notify upper-layer */
  {
    TcpIp_IpAddrStateType AddrState;

    if (TRUE == AddrAssigned)
    {
      AddrState = TCPIP_IPADDR_STATE_ASSIGNED;
    }
    else
    {
      AddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
    }

    TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId), AddrState); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  }
#endif

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VHandleSourceAddrAssignmentChange() */

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VHandleDADFailure()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VHandleDADFailure(
  IPV6_P2C(uint8)   PhysAddrPtr,
  IpV6_AddrIdType   AddrId,
  uint8             EventType)
{
  uint8                IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

# if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_ADDR_ID(AddrId),    IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
# endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (!IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* ERROR: DAD is only performed for unicast addresses. This function may only be called with AddrIds referencing unicast addresses */
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID);
    return;
  }

  IPV6_UNUSED_PARAMETER(EventType);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  if (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == CtrlAddrIdx)
  {
    /* Address configuration for link-local address has failed */
    
    /* #-- Handle duplicate address. This handling includes PhysAddr comparison! */
    TcpIp_VDuplicateAddrDetected(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(AddrId), (uint8*)(&(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address)), PhysAddrPtr);  /* PRQA S 0310 */ /* MD_IpV6_0310 */

    IPV6_CHANGE_STATE(IPV6_CTRL_STATE_LLADDR_DAD_FAIL);
  }
  else
  {
# if (IPV6_ENABLE_DHCP == STD_ON)
    if (IPV6_DHCPV6_MODE_DISABLED != IpV6_VCfgGetDhcpMode())
    {
      /* DAD for DHCPv6 configured address failed. Notify DHCPv6 */
      if (IPV6_ADDR_CFG_SRC_DHCP == IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].AddressAssignVariant)
      {
        (void)TcpIp_DhcpV6_Decline(IpCtrlIdx, (IpBase_AddrIn6Type*)&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address);
      }
    }
# endif
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VHandleDADFailure() */

# if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VHandleTempAddrDADFailure()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VHandleTempAddrDADFailure(uint8 IpCtrlIdx, IpV6_ListIdxType SatEntryIdx, IpV6_DListIdxType TempAddrIdx, uint8 EventType)
{
  Std_ReturnType Result;

  IPV6_UNUSED_PARAMETER(EventType);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#  if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SAT_IDX(SatEntryIdx),  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((IPV6_DLIST_END_IDX != TempAddrIdx), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_DATA_IDX, E_NOT_OK);
#  endif

  if (0 < IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadRetriesRemain)
  {
    IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadRetriesRemain--;
    IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].DadNsTxRemain = IpV6_VCfgGetDadTransmits();

    Result = IpV6_VPrivExtGetNextTempIdentifier(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].TempAddress);

    if (E_OK == Result)
    {
      /* schedule start of DAD for new random address */
      IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEvent = IPV6_LADDR_EVENT_TX_DAD;
      IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_TempSrcAddrData)[TempAddrIdx].NextEventTime, 10); /* TODO: Check delay */
    }
  }
  else
  {
    Result = E_NOT_OK;
    /* TODO: Retries excceded */
  }

  return Result;
} /* End of IpV6_VHandleTempAddrDADFailure() */
# endif
#endif

#if ((IPV6_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON) || (IPV6_ENABLE_MLD == STD_ON))
/**********************************************************************************************************************
 *  IpV6_VJoinMulticastGroup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VJoinMulticastGroup(
  uint8 IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) MulticastAddrPtr,
  boolean Join)
{
  Std_ReturnType Result;

# if (IPV6_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
  Eth_PhysAddrType PhysMulticastAddr;
# endif

# if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != MulticastAddrPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
# endif

# if defined(IPV6_JOIN_MULTICAST_GROUP_CALLOUT)
  IPV6_JOIN_MULTICAST_GROUP_CALLOUT(IpCtrlIdx, MulticastAddrPtr, Join);
# endif

# if (IPV6_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
  IpV6_VMulticastAddr2LinkLayerAddr(&PhysMulticastAddr[0], MulticastAddrPtr);

  if (TRUE == Join)
  {
    Result = TCPIP_LL_UpdatePhysAddrFilter(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), &PhysMulticastAddr[0], ETH_ADD_TO_FILTER);
  }
  else
  {
    Result = TCPIP_LL_UpdatePhysAddrFilter(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), &PhysMulticastAddr[0], ETH_REMOVE_FROM_FILTER);
  }

# endif

# if (IPV6_ENABLE_MLD == STD_ON)
  if (IpV6_VCfgHasMldConfig() && (E_OK == Result))
  {
    Result = IpV6_Mld_MulticastListen(IpCtrlIdx, 0x00, MulticastAddrPtr, (IPV6_P2C(IpV6_AddrType))NULL_PTR, 0, !Join);
  }
# endif

  return Result;
} /* End of IpV6_VJoinMulticastGroup() */

/**********************************************************************************************************************
 *  IpV6_VJoinSolicitedNodeMulticastGroup()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VJoinSolicitedNodeMulticastGroup(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) UnicastAddrPtr,
  boolean                                           Join)
{
  IpV6_AddrType UnicastAddrSolicitedNodeAddr; /* PRQA S 0759 */ /* MD_IpV6_0759 */

  IpV6_VGetSolicitedNodeAddr(&UnicastAddrSolicitedNodeAddr, UnicastAddrPtr, IPV6_MULTICAST_SCOPE_LINK_LOCAL);

  (void)IpV6_VJoinMulticastGroup(IpCtrlIdx, &UnicastAddrSolicitedNodeAddr, Join);

} /* End of IpV6_VJoinSolicitedNodeMulticastGroup() */
#endif

#if (IPV6_ENABLE_RND_FUNCTIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VGetRandomValue()
 **********************************************************************************************************************/
/*! \brief      Get a random value between [MinValue...MaxValue]
 *  \param[in]  MinValue   minimum value
 *  \param[in]  MaxValue   maximum value
 *  \return     Random value between [MinValue...MaxValue]
 **********************************************************************************************************************/
FUNC(uint32, IPV6_CODE) IpV6_VGetRandomValue(uint32 MinValue, uint32 MaxValue)
{
  uint32 ValueRange;
  uint32 RndValue;

  ValueRange = (MaxValue + 1) - MinValue;

  if (ValueRange <= 0xFFFFU)
  {
    /* 16bit random number [0..0xFFFF] is sufficient */
    RndValue = (uint32)TcpIp_VIpGetRandNoFct();
  }
  else
  {
    /* create 32bit random number [0..0xFFFFFFFF] by combining two 16bit random numbers */
    RndValue = (TcpIp_VIpGetRandNoFct() << 8) | TcpIp_VIpGetRandNoFct();
  }

  return MinValue + (RndValue % ValueRange);

} /* End of IpV6_VGetRandomValue() */

/**********************************************************************************************************************
 *  IpV6_VRandomizeValue()
 **********************************************************************************************************************/
/*! \brief      Randomize a value by multiplying it with a random factor
 *  \param[in]  BaseValue value to be multiplied with random factor
 *  \return     BaseValue multiplied with random factor
 **********************************************************************************************************************/
FUNC(uint32, IPV6_CODE) IpV6_VRandomizeValue(
  uint8  IpCtrlIdx,
  uint32 BaseValue)
{
  uint32 MinValue;
  uint32 MaxValue;

  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  MinValue = (BaseValue * IpV6_VCfgGetMinRandomFactor()) / 10U;
  MaxValue = (BaseValue * IpV6_VCfgGetMaxRandomFactor()) / 10U;

  return IpV6_VGetRandomValue(MinValue, MaxValue);

} /* End of IpV6_VRandomizeValue() */
#endif /* (IPV6_ENABLE_RND_FUNCTIONS == STD_ON) */

/* RX HEADER PROCESSING FUNCTIONS */
#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VSegmentListInsert()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSegmentListInsert(
  P2VAR(IpV6_SegmentListType, AUTOMATIC, IPV6_APPL_VAR) ListPtr,
  uint8 InsertIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != ListPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(InsertIdx <= ListPtr->SegUsed, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  if ((ListPtr->SegUsed >= ListPtr->SegCount) || (InsertIdx > ListPtr->SegUsed))
  {
    /* list full or index out of bounds */
    return E_NOT_OK;
  }

  if (0 < ListPtr->SegUsed)
  {
    /* shift all segments after insert position to the right */
    uint8 LastSegIdx = ListPtr->SegUsed;

    while (InsertIdx < LastSegIdx)
    {
      ListPtr->Segments[LastSegIdx] = ListPtr->Segments[LastSegIdx-1];
      LastSegIdx--;
    }
  }

  ListPtr->SegUsed++;

  return E_OK;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSegmentListInsert() */

/**********************************************************************************************************************
 *  IpV6_VSegmentListRemove()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSegmentListRemove(
  P2VAR(IpV6_SegmentListType, AUTOMATIC, IPV6_APPL_VAR) ListPtr,
  uint8 RemoveIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != ListPtr,          IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(RemoveIdx < ListPtr->SegUsed, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  if ((0 == ListPtr->SegUsed) || (RemoveIdx >= ListPtr->SegUsed))
  {
    /* list empty or index out of bounds */
    return E_NOT_OK;
  }

  /* shift following segments to the left */
  while (RemoveIdx < (ListPtr->SegUsed - 1))
  {
    ListPtr->Segments[RemoveIdx] = ListPtr->Segments[RemoveIdx+1];
    RemoveIdx++;
  }

  ListPtr->SegUsed--;

  return E_OK;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSegmentListRemove() */

/**********************************************************************************************************************
 *  IpV6_VSegmentListAdd()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSegmentListAdd(
  P2VAR(IpV6_SegmentListType, AUTOMATIC, IPV6_APPL_VAR) ListPtr,
  uint16 SegOfs,
  uint16 SegLen)
{
  uint8 SegIdx;
  uint16 SegEndOfs;
  Std_ReturnType Result = E_OK;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != ListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  SegEndOfs = SegOfs + SegLen;

  /* locate insert position */
  SegIdx = ListPtr->SegUsed;

  while ((0 < SegIdx) && (ListPtr->Segments[SegIdx-1].StartOfs >= SegEndOfs))
  {
    SegIdx--;
  }

  if ((0 < SegIdx) && (SegOfs < ListPtr->Segments[SegIdx-1].EndOfs))
  {
    /* segment overlaps with previous segment */
    return E_NOT_OK;
  }

  if ((SegIdx < ListPtr->SegUsed) && (SegEndOfs > ListPtr->Segments[SegIdx].StartOfs))
  {
    /* segment overlaps with next segment */
    return E_NOT_OK;
  }

  if ((0 < SegIdx) && (ListPtr->Segments[SegIdx-1].EndOfs == SegOfs))
  {
    /* new segment can be appended to previous segment */
    /* ----|#PREV#|*NEW*|----|#NEXT#|---- */
    ListPtr->Segments[SegIdx-1].EndOfs += SegLen;

    if ((SegIdx < ListPtr->SegUsed) && (ListPtr->Segments[SegIdx-1].EndOfs == ListPtr->Segments[SegIdx].StartOfs))
    {
      /* new segment fits exactly between previous and next segment */
      /* ----|#PREV#|***NEW***|#NEXT#|---- */
      ListPtr->Segments[SegIdx-1].EndOfs = ListPtr->Segments[SegIdx].EndOfs;

      if (E_OK != IpV6_VSegmentListRemove(ListPtr, SegIdx))
      {
        Result = E_NOT_OK;
      }
    }
  }
  else if ((SegIdx < ListPtr->SegUsed) && (SegEndOfs == ListPtr->Segments[SegIdx].StartOfs))
  {
    /* new segment can be prepended to next segment */
    /* ----|#PREV#|----|*NEW*|#NEXT#|---- */
    ListPtr->Segments[SegIdx].StartOfs = SegOfs;
  }
  else
  {
    /* new segment is isolated */
    /* ----|?PREV?|--|*NEW*|--|?NEXT?|---- */
    if (E_OK == IpV6_VSegmentListInsert(ListPtr, SegIdx))
    {
      ListPtr->Segments[SegIdx].StartOfs = SegOfs;
      ListPtr->Segments[SegIdx].EndOfs   = SegEndOfs;
    }
    else
    {
      Result = E_NOT_OK;
    }
  }

  return Result;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VSegmentListAdd() */

/**********************************************************************************************************************
 *  IpV6_VGetReassemblyBufferIdx()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VGetReassemblyBufferIdx(
  IpV6_AddrIdType                                   AddrId,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) SrcAddrPtr,
  uint32 FragmentId,
  P2VAR(IpV6_ListIdxType, AUTOMATIC, IPV6_APPL_VAR) FragmentBufIdx)
{
  IpV6_ListIdxType     BufIdx;      /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_ListIdxType     UnusedBufIdx = IPV6_INV_LIST_IDX;
  uint8                IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != SrcAddrPtr,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  BufIdx = IpV6_VCfgGetReassemblyBufferCount();

  while (0 < BufIdx)
  {
    BufIdx--;

    if (IPV6_FRAG_STATUS_UNUSED != IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status)
    {
      if (FragmentId == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FragmentId)
      {
        if (IPV6_ADDR_EQUALS(*SrcAddrPtr, IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].SrcAddr))
        {
          if (IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].IpCtrlAddrIdx == CtrlAddrIdx)
          {
            /* match found. */
            *FragmentBufIdx = BufIdx;
            return E_OK;
          }
        }
      }
    }
    else if (IPV6_INV_LIST_IDX == UnusedBufIdx)
    {
      /* store index of free buffer in case we need one */
      UnusedBufIdx = BufIdx;
    }
    else
    {
      /* buffer in use for other fragments */
    }
  }

  if (IPV6_INV_LIST_IDX != UnusedBufIdx)
  {
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].Status            = IPV6_FRAG_STATUS_ACTIVE;
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].FragmentId        = FragmentId;
    IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].SrcAddr, *SrcAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].IpCtrlAddrIdx     = IPV6_ADDR_ID_2_ADDR_IDX(AddrId);
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].RxSegList.SegUsed = 0;
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].FirstFragHdrOfs   = 0;
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].FirstFragPktLen   = 0;
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].OrigPktPayloadLen = 0;
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].FirstReceived     = FALSE;
    IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].LastReceived      = FALSE;
    IPV6_SET_EVENT_TIME_MS(IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[UnusedBufIdx].ReassemblyTimeout, IpV6_VCfgGetReassemblyTimeout());
    *FragmentBufIdx = UnusedBufIdx;
    return E_OK;
  }

  return E_NOT_OK;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VGetReassemblyBufferIdx() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VGetFragmentTxBufferIdx()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VGetFragmentTxBufferIdx(
  uint8                                             IpCtrlIdx,
  P2VAR(IpV6_ListIdxType, AUTOMATIC, IPV6_APPL_VAR) FragmentBufIdx)
{
  IpV6_ListIdxType BufIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  BufIdx = IpV6_VCfgGetTxFragmentBufferCount();

  while (0 < BufIdx)
  {
    BufIdx--;

    if (IPV6_FRAG_TX_STATUS_UNUSED == IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].Status)
    {
      IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].Status = IPV6_FRAG_TX_STATUS_PROVIDED;
      *FragmentBufIdx = BufIdx;
      return E_OK;
    }
  }

  return E_NOT_OK;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VGetFragmentTxBufferIdx() */
#endif

/**********************************************************************************************************************
 *  IpV6_VProcessFragmentHeader()
 **********************************************************************************************************************/
/*! \brief      Process fragment header of recieved packet
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, IPV6_CODE) IpV6_VProcessFragmentHeader(
  IpV6_AddrIdType AddrId,
  IPV6_P2C(uint8) DataPtr,
  uint16          DataLen,
  uint16          FragHdrOfs,
  uint16          PrevHdrOfs)
{
  uint8   IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  boolean LastFrag;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(0 < DataLen,                   IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(FragHdrOfs < DataLen,          IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM,   E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_ADDR_ID, E_NOT_OK);
#endif

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_OFF)
  IPV6_UNUSED_PARAMETER(PrevHdrOfs);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);

  /* ignore packet if fragment header length exceeds remaining payload length */
  if ((FragHdrOfs + IPV6_FRAGMENT_EXT_HDR_LEN) > DataLen)
  {

    IPV6_DBG_COUNTER_RX_INC(DropInvNxtHdrLen);
    return E_NOT_OK;
  }

  LastFrag = (boolean)(0 == (DataPtr[FragHdrOfs+3] & 0x01u)); /* M(ore) bit is not set */

  if (FALSE == LastFrag)
  {
    if (0 != (DataLen & IPV6_MASK_MOD_8)) /* Payload not a multiple of 8 */
    {
      /* The fragment packet must be discarded if the M-bit in the fragment header is set and the
         payload of the packet is not a multiple of 8 octets. (see [RFC2460 4.5.])*/

      /* Send ICMP Parameter Problem Code 0 according to [RFC2460 4.5.] */
#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
      IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_ERR_HDR_FIELD, DataPtr, DataLen, IPV6_HEADER_PLD_LEN_FIELD_OFFS); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif
      return E_NOT_OK;
    }
  }

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
  if (IpV6_VCfgIsRxFragmentationEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    IPV6_P2C(IpV6_HdrType)            IpV6HdrPtr;
    IPV6_P2C(IpV6_FragmentExtHdrType) FragHdrPtr;

    uint16           FragOfs;
    uint16           FragPayloadOfs;
    uint16           FragLen;
    uint32           FragId;
    IpV6_ListIdxType FragBufIdx;

    /*                                               |FragPayloadOfs
     *                                               | _________________________________________fragmentable_part_..
     *                                                /
     *    unfragmentable_part   IPV6_FRAGMENT_EXT_HDR_LEN                      FragmentLen
     *   __/\_____________________   ______/\_______   _____________________________/\____________________________
     *  /                         \ /               \ /                                                           \
     * +-------------+- - - - - - -+-----------------+- - - - - - -+--------------------+---------------------- - -+
     * | IPv6 Header | ext headers | fragment header | ext headers | UL protocol header | UL payload               |
     * +-------------+- - - - - - -+-----------------+- - - - - - -+--------------------+---------------------- - -+
     *                \____________________________________________  _____________________________________________/
     *                             |                               \/
     *                             |FragHdrOfs                IPv6 payload
     *  \_______________________________________________  ________________________________________________________/
     *                                                  \/
     *                                                DataLen
     */

    /* PRQA S 0310 2 */ /* MD_IpV6_0310 */
    IpV6HdrPtr = (P2CONST(IpV6_HdrType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[0];
    FragHdrPtr = (P2CONST(IpV6_FragmentExtHdrType, AUTOMATIC, IPV6_APPL_DATA))&DataPtr[FragHdrOfs];

    FragId         = IPV6_NTOHL(FragHdrPtr->IdNbo);
    FragOfs        = (uint16)(IPV6_NTOHS(FragHdrPtr->FragmentOffsetNbo) & 0xFFF8U); /* fragment offset in bytes (original packet) */
    FragPayloadOfs = (FragHdrOfs + IPV6_FRAGMENT_EXT_HDR_LEN);                      /* offset of fragment payload (in current packet) */
    FragLen        = (DataLen - FragPayloadOfs);                                    /* length of fragment */

    if (0x0000FFFFU < ((uint32)FragHdrOfs + FragOfs + FragLen))
    {
#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
      IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_ERR_HDR_FIELD, DataPtr, DataLen, FragHdrOfs + IPV6_FRAGMENT_EXT_HDR_FIELD_POS_FRAGMENT_OFFSET); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif
      return E_NOT_OK;
    }

    if (IPV6_EXT_HDR_ID_HOP_BY_HOP == FragHdrPtr->NextHdr)
    {
#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
      IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_NXT_HDR, DataPtr, DataLen, FragHdrOfs + IPV6_FRAGMENT_EXT_HDR_FIELD_POS_NEXT_HEADER); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif

      IPV6_DBG_COUNTER_RX_INC(DropHdrOrder);
      return E_NOT_OK;
    }

    if (E_OK == IpV6_VGetReassemblyBufferIdx(AddrId, &IpV6HdrPtr->SrcAddr, FragId, &FragBufIdx))
    {
      /* got index of fragment reassembly buffer */

      if (IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Status != IPV6_FRAG_STATUS_ACTIVE)
      {
        /* buffer state indicates error or already completed assembly. Ignore this fragment */
        return E_NOT_OK;
      }

      if (IpV6_VCfgGetReassemblyBufferSize() < (FragHdrOfs + FragOfs + FragLen))
      {
        /* reassembled packet will be too lagre for reassembly buffer! */
        /* packet cannot be received, ignore all fragments */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Status = IPV6_FRAG_STATUS_TOO_BIG;
        return E_NOT_OK;
      }

      if (IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].RxSegList.SegUsed == 0)
      {
        /* first received fragment. This may not be the first fragment (FragOfs == 0)! */
        /* the length of the unfragmentable part in each fragment should be equal */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstFragHdrOfs = FragHdrOfs;
      }
      else if ((0 == FragOfs) && (IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstFragHdrOfs != FragHdrOfs))
      {
        /* length of unfragmentable part has changed between first received and first (FragOfs == 0) fragment!
         * We would have to move the entire reassembly buffer content in order to adjust the offset in the original packet.
         * This is not supported.
         */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Status = IPV6_FRAG_STATUS_INV_UNFRAG_LEN;
        return E_NOT_OK;
      }
      else
      {
        /* length of unfragmentable part in received fragments may change as long as the unfragmentable parts
         * of the first received and the first fragment (FrgOfs == 0) have equal lengths.
         * According to [RFC2460 4.5 Fragment Header] this is should not occur but is not considered as an error.
         */
      }

      if (0 == FragOfs)
      {
        /* this is the first fragment */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstReceived   = TRUE;
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstFragPktLen = DataLen;

        /* copy unfragmentable part (all headers before fragment header) to reassembly buffer */
        IpV6_MemCpy(&IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Buffer[0], &DataPtr[0], FragHdrOfs); /* PRQA S 0310 */ /* MD_IpV6_0310 */

        /* add unfragmentable part to segment list */
        if (E_OK != IpV6_VSegmentListAdd(&IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].RxSegList, 0, FragHdrOfs))
        {
          /* too much out-of-order segements received. Check for overlapping fragments according to [RFC5722] not possible.
           * Discard all fragments.
           */
          IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Status = IPV6_FRAG_STATUS_SEGM_ERROR;
          return E_NOT_OK;
        }

        /* set next header value of the last header in unfragmentable part to the header value of the header following the fragment header */
        if (0 == PrevHdrOfs)
        {
          /* there are no extension headers between IPv6 header and fragment header.
           * Set next header value in IPv6 header to the value of the header following the fragment header. */
          IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Buffer[IPV6_HEADER_NXT_HDR_FIELD_OFFS] = FragHdrPtr->NextHdr;
          IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].NextHdrFieldOfs                        = IPV6_HEADER_NXT_HDR_FIELD_OFFS;
        }
        else
        {
          /* there is at least one extension header between the IPv6 header and the fragment header */
          /* set next header value in the extension header before the fragment header to the next header value of the header following the fragment header */
          IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Buffer[PrevHdrOfs] = FragHdrPtr->NextHdr;
          IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].NextHdrFieldOfs    = PrevHdrOfs;
        }

        /* add length of headers between IPv6 and fragment extension header (unfragmentable part) */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].OrigPktPayloadLen += (FragHdrOfs - IPV6_HDR_LEN);
      }

      /* copy fragment payload into reassembly buffer */
      IpV6_MemCpy(&IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Buffer[IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstFragHdrOfs + FragOfs], &DataPtr[FragPayloadOfs], FragLen); /* PRQA S 0310 */ /* MD_IpV6_0310 */

      if (E_OK != IpV6_VSegmentListAdd(&IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].RxSegList, IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstFragHdrOfs + FragOfs, FragLen))
      {
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Status = IPV6_FRAG_STATUS_SEGM_ERROR;
        return E_NOT_OK;
      }

      if (TRUE == LastFrag)
      {
        /* this is the last fragment */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].LastReceived = TRUE;

        /* add length of fragmentable part */
        IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].OrigPktPayloadLen += (FragOfs + FragLen);
      }

      if ((TRUE == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].FirstReceived) && (TRUE == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].LastReceived))
      {
        if ((1 == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].RxSegList.SegUsed) && (0 == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].RxSegList.Segments[0].StartOfs))
        {
          /* reassembly of packet complete. Reassembled packet will be processed within next main function */
          IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[FragBufIdx].Status = IPV6_FRAG_STATUS_COMPLETE;
        }
      }
    }
    else
    {
      /* all fragment reassembly buffers in use */
      /* packet cannot be received */
      return E_NOT_OK;
    }

    return E_OK;
  }
  else
#endif
  {

    return E_NOT_OK;
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_VProcessFragmentHeader() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VRxFragments()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VRxFragments(uint8 IpCtrlIdx)
{
  IpV6_ListIdxType BufIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  BufIdx = IpV6_VCfgGetReassemblyBufferCount();

  while (0 < BufIdx)
  {
    BufIdx--;

    if (IPV6_FRAG_STATUS_UNUSED == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status)
    {
      /* skip */
    }
    else if (IPV6_FRAG_STATUS_COMPLETE == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status)
    {
      /* fragment reassembly complete */

      /* update payload length field in IPv6 header of reassembled packet */
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer[4] = (uint8)(IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].OrigPktPayloadLen >> 8);
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer[5] = (uint8)(IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].OrigPktPayloadLen);

      /* call IpV6_RxIndication() with reassembled packet */
      IpV6_RxIndication(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), IPV6_ETHER_TYPE_ID, FALSE, (uint8*)NULL_PTR, &IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer[0], IPV6_HDR_LEN + IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].OrigPktPayloadLen); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status = IPV6_FRAG_STATUS_UNUSED;
    }
    else if (IPV6_TIME_EXPIRED(IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].ReassemblyTimeout))
    {
      /* fragment reassembly time expired */
      if ((TRUE == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FirstReceived) && (IPV6_FRAG_STATUS_ACTIVE == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status))
      {
        IpV6_AddrIdType AddrId;

        IPV6_P2C(IpV6_HdrType)            IpV6HdrPtr;
        IPV6_P2V(IpV6_FragmentExtHdrType) FragHdrPtr;
        IPV6_P2V(uint8)                   OrigPktBufPtr;
        uint16                            OrigPktBufLen;
        uint16                            OrigPktBufPos = 0;

        IpV6HdrPtr = (IPV6_P2C(IpV6_HdrType))&IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        OrigPktBufLen = (IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FirstFragPktLen);

        if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY == IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].IpCtrlAddrIdx)
        {
          AddrId = IPV6_CTRL_VAR(IpV6_BcAddrId);
        }
        else
        {
          AddrId = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].IpCtrlAddrIdx].IpAddrId;
        }

        if (E_OK == IpV6_Icmp_VTxTimeExceeded(AddrId, &IpV6HdrPtr->SrcAddr, IPV6_ICMP_MSG_TIME_EXCEEDED_CODE_TIME_EXCEEDED, &OrigPktBufPtr, &OrigPktBufLen, NULL_PTR, 0))
        {
          /* copy original packet unfragmentable part into icmp tx buffer */
          IpV6_MemCpy(&OrigPktBufPtr[OrigPktBufPos], &IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer[0], IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FirstFragHdrOfs); /* PRQA S 0310 */ /* MD_IpV6_0310 */
          OrigPktBufPos += IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FirstFragHdrOfs;

          /* rebuild first segment by injecting fragment header between unfragmentable und fragmentsble part */
          FragHdrPtr = (IPV6_P2V(IpV6_FragmentExtHdrType))&OrigPktBufPtr[OrigPktBufPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
          OrigPktBufPos += sizeof(IpV6_FragmentExtHdrType);

          FragHdrPtr->NextHdr           = OrigPktBufPtr[IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].NextHdrFieldOfs];

          OrigPktBufPtr[IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].NextHdrFieldOfs] = IPV6_EXT_HDR_ID_FRAGMENT;

          FragHdrPtr->Reserved          = 0;
          FragHdrPtr->FragmentOffsetNbo = IPV6_UINT8_HTONS(1U); /* Set fragment offset to 0 and M-flag to 1 */
          FragHdrPtr->IdNbo             = IPV6_HTONL(IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FragmentId);

          /* copy original packet fragmentable part into icmp tx buffer */
          IpV6_MemCpy(&OrigPktBufPtr[OrigPktBufPos], &IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer[IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].FirstFragHdrOfs], (OrigPktBufLen - OrigPktBufPos)); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        }
        else
        {
          /* cannot sent TimeExceeded message */
        }
      }

      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status = IPV6_FRAG_STATUS_UNUSED;
    }
    else
    {
      /* fragment reassembly still in progress */
    }
  }
} /* End of IpV6_VRxFragments() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VTxFragments()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VTxFragments(uint8 IpCtrlIdx)
{
  IpV6_ListIdxType BufIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  uint16 EthBufOfs;

  BufIdx = IpV6_VCfgGetTxFragmentBufferCount();

  while (0 < BufIdx)
  {
    P2VAR(IpV6_FragmentTxBufferDescType, AUTOMATIC, IPV6_APPL_VAR) FragBufPtr;
    P2CONST(IpV6_HdrType, AUTOMATIC, IPV6_APPL_DATA) OrgIpV6HdrPtr;
    P2VAR(IpV6_HdrType, AUTOMATIC, IPV6_APPL_VAR) FragIpV6HdrPtr;
    uint8  EthBufIdx;
    uint8 *EthBufPtr;
    uint16 EthBufLen;
    uint16 FragLen;
    boolean More = FALSE;

    BufIdx--;

    FragBufPtr = (P2VAR(IpV6_FragmentTxBufferDescType, AUTOMATIC, IPV6_APPL_VAR))&IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx];

    if (IPV6_FRAG_TX_STATUS_TRANSMIT == FragBufPtr->Status)
    {
      OrgIpV6HdrPtr = (P2CONST(IpV6_HdrType, AUTOMATIC, IPV6_APPL_DATA))&FragBufPtr->Buffer[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

      FragLen = (FragBufPtr->FragPartLen - FragBufPtr->TxOfs); /* remaining fragment length */

      if (FragLen > FragBufPtr->MaxFragLen)
      {
        /* remaining fragmnetable part does not fit into one fragment */
        FragLen = FragBufPtr->MaxFragLen;
        FragLen -= (FragLen & IPV6_MASK_MOD_8);
        More = TRUE;
      }

      EthBufLen = FragLen + IPV6_FRAGMENT_EXT_HDR_LEN + FragBufPtr->UnfragPartLen;

      if (BUFREQ_OK == TCPIP_LL_ProvideTxBuffer(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), IPV6_ETHER_TYPE_ID, IPV6_CTRL_VAR(IpV6_EthCtrlFramePriorityDefault), &EthBufIdx, (Eth_DataType**)&EthBufPtr, &EthBufLen)) /* PRQA S 0310 */ /* MD_IpV6_0310 */
      {
        P2VAR(IpV6_FragmentExtHdrType, AUTOMATIC, IPV6_APPL_VAR) FragHdrPtr;

        /* copy unfragmentable part into tx buffer of fragment packet */
        IpV6_MemCpy(EthBufPtr, &FragBufPtr->Buffer[0], FragBufPtr->UnfragPartLen); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        EthBufOfs = FragBufPtr->UnfragPartLen;

        FragIpV6HdrPtr = (P2VAR(IpV6_HdrType, AUTOMATIC, IPV6_APPL_DATA))&EthBufPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        FragIpV6HdrPtr->NextHdr = IPV6_EXT_HDR_ID_FRAGMENT;
        FragIpV6HdrPtr->PayloadLenNbo = IPV6_HTONS((FragBufPtr->UnfragPartLen - IPV6_HDR_LEN) + IPV6_FRAGMENT_EXT_HDR_LEN + FragLen);

        /* add fragment header for fragment oacket */
        FragHdrPtr = (P2VAR(IpV6_FragmentExtHdrType, AUTOMATIC, IPV6_APPL_VAR))&EthBufPtr[EthBufOfs]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

        FragHdrPtr->IdNbo = IPV6_HTONL(FragBufPtr->FragmentId);
        FragHdrPtr->NextHdr = OrgIpV6HdrPtr->NextHdr;
        FragHdrPtr->Reserved = 0;
        FragHdrPtr->FragmentOffsetNbo = IPV6_HTONS((FragBufPtr->TxOfs & 0xFFF8U) | More);

        EthBufOfs += IPV6_FRAGMENT_EXT_HDR_LEN;

        /* add fragment payload */
        IpV6_MemCpy(&EthBufPtr[EthBufOfs], &FragBufPtr->Buffer[FragBufPtr->UnfragPartLen + FragBufPtr->TxOfs], FragLen); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        EthBufOfs += FragLen;

        if (E_OK == TCPIP_LL_Transmit(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), EthBufIdx, IPV6_ETHER_TYPE_ID, FALSE, EthBufOfs, &FragBufPtr->NextHopPhysAddr[0]))
        {
          /* fragment successfully sent */
          FragBufPtr->TxOfs += FragLen;

          if (FragBufPtr->TxOfs >= FragBufPtr->FragPartLen)
          {
            /* all fragments have been transmitted, mark packet buffer as free */
            FragBufPtr->Status = IPV6_FRAG_TX_STATUS_UNUSED;
          }
        }
      }
    }
  }
} /* End of IpV6_VTxFragments() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if ((IPV6_ENABLE_RX_FRAGMENTATION == STD_ON) || (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON))
/**********************************************************************************************************************
 *  IpV6_VInitFragments()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VInitFragments(uint8 IpCtrlIdx)
{
  IpV6_ListIdxType BufIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

# if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
  if (IpV6_VCfgIsRxFragmentationEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    BufIdx = IpV6_VCfgGetReassemblyBufferCount();

    while (0 < BufIdx)
    {
      BufIdx--;

      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Status = IPV6_FRAG_STATUS_UNUSED;
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].Buffer = &IPV6_CTRL_VAR(IpV6_ReassemblyDataBuffer)[BufIdx * IpV6_VCfgGetReassemblyBufferSize()];
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].RxSegList.Segments = &IPV6_CTRL_VAR(IpV6_ReassemblySegments)[BufIdx * IpV6_VCfgGetReassemblySegmentCount()];
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].RxSegList.SegCount = IpV6_VCfgGetReassemblySegmentCount();
      IPV6_CTRL_VAR(IpV6_ReassemblyBufferDescriptors)[BufIdx].RxSegList.SegUsed = 0;
    }
  }
# endif

# if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
  if (IpV6_VCfgIsTxFragmentationEnabled()) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
  {
    IPV6_CTRL_VAR(IpV6_NextTxFragmentId) = 0;

    BufIdx = IpV6_VCfgGetTxFragmentBufferCount();

    while (0 < BufIdx)
    {
      BufIdx--;

      IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].Status = IPV6_FRAG_TX_STATUS_UNUSED;
      IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].Buffer = &IPV6_CTRL_VAR(IpV6_FragmentTxDataBuffer)[BufIdx * IpV6_VCfgGetTxFragmentBufferSize()];
    }
  }
# endif

} /* End of IpV6_VInitFragments() */
#endif

#if ((IPV6_ENABLE_ADDR_NVM_STORAGE_SUPPORT == STD_ON) || (IPV6_DEFAULT_ADDRESS_COUNT > 0))
/**********************************************************************************************************************
 *  IpV6_VConfigureSourceAddresses()
 **********************************************************************************************************************/
/*! \brief      Assign stored NvM or default addresses.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VConfigureSourceAddresses(uint8 IpCtrlIdx)
{
  IpV6_CtrlAddrIdxType CtrlAddrIdx;

  /* clear all temporaray address lists */
  CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

  while (0 < CtrlAddrIdx)
  {
    IPV6_P2C(IpV6_AddrType) AddressPtr = NULL_PTR;

    CtrlAddrIdx--;

#if (IPV6_ENABLE_ADDR_NVM_STORAGE_SUPPORT == STD_ON)
    {
      uint16_least NvmDataOffset;
      /* check if source address has an NvM block assigned that contains a valid address */
      NvmDataOffset = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].TcpIpNvmDataOffset;

      if (TCPIP_NVM_BLOCK_SIZE > NvmDataOffset)
      {
        IPV6_P2V(IpV6_AddrType) NvmAddrPtr = IPV6_NVM_GET_ADDR_PTR(NvmDataOffset); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        /* address has NvM block. Check if the NvM block contains a valid unicast address. */
        if (IPV6_ADDR_IS_EUI64(*NvmAddrPtr) && (!IPV6_ADDR_IS_MULTICAST(*NvmAddrPtr)))
        {
          AddressPtr = NvmAddrPtr;
        }
      }
    }
#endif

#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
    if (NULL_PTR == AddressPtr)
    {
      /* no valid address found in the NvM. Check for configured default address */
      uint8 TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].DefaultAddrIdx;

      if (IPV6_DEFAULT_ADDR_IDX_INV > TmpDefaultAddrIdx)
      {
        AddressPtr = &IpV6_DefaultAddresses[TmpDefaultAddrIdx];
      }
    }
#endif

    if (NULL_PTR != AddressPtr)
    {
      /* Address has been found in the NvM or default address will be configured */
      if (E_NOT_OK == IpV6_Ndp_VAddSourceAddress(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId, AddressPtr, IPV6_LIFETIME_UNLIMITED, IPV6_LIFETIME_UNLIMITED, 0, IPV6_SRC_ADDR_DAD_MODE_NONE))
      {
        IpV6_DetReportInternalError(IPV6_E_INV_ADDR_CONFIG);
      }
    }
  }
} /* End of IpV6_VConfigureSourceAddresses() */
#endif

#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
/**********************************************************************************************************************
 *  IpV6_VConfigureDynamicMulticastAddresses()
 **********************************************************************************************************************/
/*! \brief      Assign default address values for dynamic multicast addresses
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VConfigureDynamicMulticastAddresses(uint8 IpCtrlIdx)
{
  uint8 TmpDefaultAddrIdx;

  IpV6_CtrlAddrIdxType MulticastAddrIdx = IPV6_CTRL_VAR(IpV6_DynamicMulticastAddressCount);

  while (0 < MulticastAddrIdx)
  {
    MulticastAddrIdx--;

    TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].DefaultAddrIdx;

    if (IPV6_DEFAULT_ADDR_IDX_INV > TmpDefaultAddrIdx)
    {
      /* Set dynamic multicast address to the default value */
      IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], IpV6_DefaultAddresses[TmpDefaultAddrIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */

      IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IpV6_DefaultAddresses[TmpDefaultAddrIdx], TRUE);

      TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].IpAddrId), TCPIP_IPADDR_STATE_ASSIGNED);
    }
    else
    {
      /* Dynamic address has no default value. Set address to unspecified address in order to indicate that no address is assigned */
      IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], IpV6_AddrUnspecified); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    }
  }
} /* End of IpV6_VConfigureDynamicMulticastAddresses() */

/**********************************************************************************************************************
 *  IpV6_VConfigureStaticMulticastAddresses()
 **********************************************************************************************************************/
/*! \brief      Assign default address values for static multicast addresses
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VConfigureStaticMulticastAddresses(uint8 IpCtrlIdx)
{
  IpV6_CtrlAddrIdxType MulticastAddrIdx;

  for (MulticastAddrIdx = IPV6_CTRL_VAR(IpV6_DynamicMulticastAddressCount); MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_MulticastAddressCount); MulticastAddrIdx++)
  {
    uint8 TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].DefaultAddrIdx;

    /* static multicast addresses must have a default value */

    IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IpV6_DefaultAddresses[TmpDefaultAddrIdx], TRUE);

    TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].IpAddrId), TCPIP_IPADDR_STATE_ASSIGNED);
  }
} /* End of IpV6_VConfigureStaticMulticastAddresses() */
#endif

/**********************************************************************************************************************
 *  IpV6_VCtrlStateStartup()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VCtrlStateStartup(uint8 IpCtrlIdx)
{
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  uint32                 PrivExtDesyncFactor;
#endif

  /* set 64bit interface identifier, generated from local MAC address */
  IpV6_VGetInterfaceIdentifier(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_InterfaceIdentifier)[0]);

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  if (IpV6_VCfgHasPrivExtConfig())
  {
    PrivExtDesyncFactor = IpV6_VGetRandomValue(0, IpV6_VCfgGetPrivExtMaxDesyncFactor());

    IPV6_PRIVEXT_VAR(PrivExtTempPreferredLifetime) = IpV6_VCfgGetPrivExtTempPreferredLifetime() - PrivExtDesyncFactor;

    /* [RFC4941 3.2.1. When Stable Storage Is Present / 1.] */

    /* | <64-Bit EUI-64 identifier [RFC2464]> | <64-Bit Random Value> | */
    IpV6_MemCpy(&IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[0], &IPV6_CTRL_VAR(IpV6_InterfaceIdentifier)[0], IPV6_INTERFACE_IDENTIFIER_LEN_BYTE); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    (void)esl_getBytesRNG(IPV6_INTERFACE_IDENTIFIER_LEN_BYTE, &IPV6_CTRL_VAR(IpV6_PrivExtInterfaceIdentifier)[IPV6_INTERFACE_IDENTIFIER_LEN_BYTE]);

    /* generate initial reandom interface identifier */
    (void)IpV6_VPrivExtGenerateTempIdentifier(IpCtrlIdx);
  }
#endif

#if (IPV6_ENABLE_MLD == STD_ON)
  if (IpV6_VCfgHasMldConfig())
  {
    IpV6_Mld_VSetActive(IpCtrlIdx, TRUE);
  }
#endif

  IPV6_CHANGE_STATE(IPV6_CTRL_STATE_LINK_READY);

  IpV6_Ndp_VStart(IpCtrlIdx);

} /* End of IpV6_VCtrlStateStartup */

/**********************************************************************************************************************
 *  IpV6_VCtrlStateOnline()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VCtrlStateOnline(uint8 IpCtrlIdx)
{
  /* Link-local address is configured */
  TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_BcAddrId)), TCPIP_IPADDR_STATE_ASSIGNED);

  /* Setup all configured unicast and multicast addresses that have default values */
#if ((IPV6_ENABLE_ADDR_NVM_STORAGE_SUPPORT == STD_ON) || (IPV6_DEFAULT_ADDRESS_COUNT > 0))
  IpV6_VConfigureSourceAddresses(IpCtrlIdx);
#endif
#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
  IpV6_VConfigureStaticMulticastAddresses(IpCtrlIdx);
  IpV6_VConfigureDynamicMulticastAddresses(IpCtrlIdx);
#endif

#if (IPV6_ENABLE_DHCP == STD_ON)
  if (IPV6_DHCPV6_MODE_AUTOMATIC == IpV6_VCfgGetDhcpMode())
  {
    if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IpV6_VCfgGetDhcpCtrlAddrIdx()].State)
    {
      (void)TcpIp_DhcpV6_StartAddressAssignment(IpCtrlIdx);
    }
  }
#endif

  /* notify EthSM that an IP address is available on controlletr */
  ETHSM_TCPIP_MODE_INDICATION(IpV6_CtrlIdxIp2EthIf[IpCtrlIdx], TRUE);
} /* End of IpV6_VCtrlStateOnline() */

/**********************************************************************************************************************
 *  IpV6_VCtrlStateShutdown()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_VCtrlStateShutdown(uint8 IpCtrlIdx)
{
  IpV6_CtrlAddrIdxType   CtrlAddrIdx;

  /* notify EthSM that no IP address is available on controlletr */
  ETHSM_TCPIP_MODE_INDICATION(IpV6_CtrlIdxIp2EthIf[IpCtrlIdx], FALSE);

  /* Remove all source/unicast address assignments */

  IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

  while (0 < CtrlAddrIdx)
  {
    CtrlAddrIdx--;

    if (IPV6_LADDR_STATE_INVALID != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      IpV6_VHandleSourceAddrAssignmentChange(IpCtrlIdx, CtrlAddrIdx, FALSE);
    }
  }

  IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* block used for limitation of variable scope */
  {
    uint8 MulticastAddrIdx;

    /* Remove all dynamic multicast address assignments */
    for (MulticastAddrIdx = 0; MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_DynamicMulticastAddressCount); MulticastAddrIdx++)
    {
      /* check if dynamic multicast address is configured. The value may be [::] if no address is configured */
      if (IPV6_ADDR_IS_MULTICAST(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx]))
      {
        IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], FALSE);

        TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].IpAddrId), TCPIP_IPADDR_STATE_UNASSIGNED);
      }
    }

#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
    /* Remove all static multicast address assignments */
    for (/* MulticastAddrIdx */; MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_MulticastAddressCount); MulticastAddrIdx++)
    {
      uint8 TmpDefaultAddrIdx =IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].DefaultAddrIdx;

      /* Statically configured multicast addresses are always assigned on startup and must be unassigned at shutdown */
      IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IpV6_DefaultAddresses[TmpDefaultAddrIdx], FALSE);

      TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].IpAddrId), TCPIP_IPADDR_STATE_UNASSIGNED);
    }
#endif
  }

  /* Remove all-nodes multicast address assignment */
  TcpIp_Cbk_VLocalIpAssignmentChg(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_BcAddrId)), TCPIP_IPADDR_STATE_UNASSIGNED);

  /* leave all-nodes multicast group */
  IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IpV6_AddrAllNodesLL, FALSE);

  IpV6_Icmp_Init(IpCtrlIdx);
  IpV6_Ndp_Init(IpCtrlIdx);
#if (IPV6_ENABLE_MLD == STD_ON)
  if (IpV6_VCfgHasMldConfig())
  {
    IpV6_Mld_VSetActive(IpCtrlIdx, FALSE);
  }
#endif

  IPV6_CHANGE_STATE(IPV6_CTRL_STATE_INIT);

  IPV6_CTRL_VAR(IpV6_LastBcAddrPtr) = NULL_PTR;

} /* End of IpV6_VCtrlStateShutdown */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* TIMESTAMP FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VTimeAddMs()
 **********************************************************************************************************************/
/*! \brief      Add milliseconds to timestamp
 *  \param[in]  TimestampPtr  pointer to the timestamp variable
 *  \param[in]  Milliseconds  number of milliseconds to add
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VTimeAddMs(
  P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_DATA) TimestampPtr,
  uint32 Milliseconds)
{
  TimestampPtr->Ms += Milliseconds;

  if (TimestampPtr->Ms >= 1000)
  {
    TimestampPtr->S += (TimestampPtr->Ms / 1000);
    TimestampPtr->Ms = (TimestampPtr->Ms % 1000);
  }
} /* End of IpV6_VTimeAddMs() */

/**********************************************************************************************************************
 *  IpV6_VSetLifetimeS()
 **********************************************************************************************************************/
/*! \brief      Set timestamp to a lifetime value
 *  \param[in]  TimestampPtr     pointer to the timestamp variable
 *  \param[in]  LifetimeS        lifetime value in seconds
 *  \param[in]  NextEventTimePtr pointer to a variable in which the timestamp value will be stored if it is less then
 *                               the current stored value
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VSetLifetimeS(
  P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_VAR) TimestampPtr,
  uint32 LifetimeS,
  P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_VAR) NextEventTimePtr)
{
  if ((IPV6_LIFETIME_UNLIMITED - IpV6_Time.S) > LifetimeS)
  {
    TimestampPtr->S  = IpV6_Time.S + LifetimeS;
    TimestampPtr->Ms = IpV6_Time.Ms;
  }
  else
  {
    TimestampPtr->S  = IPV6_LIFETIME_UNLIMITED;
    TimestampPtr->Ms = 0;
  }

  if ((NULL_PTR != NextEventTimePtr) && (IPV6_TIME_IS_LOWER(*TimestampPtr, *NextEventTimePtr)))
  {
    *NextEventTimePtr = *TimestampPtr;
  }
} /* End of IpV6_VSetLifetimeS() */

#if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VUpdateDefaultLinkMtu()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VUpdateDefaultLinkMtu(uint8 IpCtrlIdx, uint16 Mtu)
{
  if ((IPV6_MINIMUM_MTU <= Mtu) && (IpV6_VCfgGetDefaultLinkMtu() >= Mtu))
  {
    IpV6_ListIdxType DcEntryIdx;

    /* reconfigure MTU value [IPV6_MINIMUM_MTU...IPV6_LINK_MTU] (e.g. [1280...1500]) */
    IPV6_CTRL_VAR(IpV6_DefaultLinkMtu) = Mtu;

# if (IPV6_ENABLE_PATH_MTU == STD_ON)
    if (IpV6_VCfgIsPathMtuEnabled())
    {
      DcEntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount);

      while (0 < DcEntryIdx)
      {
        DcEntryIdx--;

        if (IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].Mtu > Mtu)
        {
          IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].Mtu = Mtu;
        }
      }
    }
# endif
  }
} /* End of IpV6_VUpdateDefaultLinkMtu() */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV6_InitMemory()
 **********************************************************************************************************************/
/*! \brief      This function has to be called before any other function of this module.
 *  \pre        Interrupts must be disabled
 *  \context    System Startup
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_InitMemory(void)
{

  IpV6_State = IPV6_STATE_UNINIT;

  return;
} /* End of IpV6_InitMemory() */

/**********************************************************************************************************************
 *  IpV6_Init()
 **********************************************************************************************************************/
/*! \brief      Initialization of the IpV6 module.
 *  \note       This function has to be called after IpV6_InitMemory() and before any other function of this module.
 *  \param[in]  ConfigPtr         Configuration structure for initializing the module
 *  \pre        Interrupts must be disabled
 *  \context    System Startup
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Init(P2CONST(IpV6_ConfigType, AUTOMATIC, IPV6_CONST) ConfigPtr)
{
#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV6_05.06 */
#else
  const uint8 IpCtrlIdx = 0; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
#endif

  TcpIp_SockIdxType SckDataIdx;

  /* module initialisation */
  IPV6_UNUSED_PARAMETER(ConfigPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  IPV6_TIME_SET(IpV6_Time, 0, 0);

  SckDataIdx = IPV6_MAX_NUM_SOCKETS;

  while (0 < SckDataIdx)
  {
    SckDataIdx--;
    IpV6_ResetSocket(SckDataIdx);
  }

#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IpCtrlIdx = IpV6_VCfgGetMaxNumCtrls();

  while (0 < IpCtrlIdx)
  {
    IpCtrlIdx--;
#endif

    IPV6_CTRL_VAR(IpV6_CtrlPreviousState) = IPV6_CTRL_STATE_UNINIT;
    IPV6_CTRL_VAR(IpV6_CtrlState)         = IPV6_CTRL_STATE_INIT;

    IPV6_CTRL_VAR(IpV6_LastBcAddrPtr) = NULL_PTR;

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    if (IpV6_VCfgHasPrivExtConfig())
    {
      IPV6_PRIVEXT_VAR(PrivExtNextIdGenTime) = IpV6_Time;

      /* initialize list descriptor of the dynamic lists for the temporary addresses */
      IpV6_DListInitDesc(&IPV6_PRIVEXT_VAR(TempSrcAddrListDesc), &IPV6_CTRL_VAR(IpV6_TempSrcAddrListNodes)[0], IpV6_VCfgGetPrivExtMaxTempAddrs());

      {
        /* initialize all dynamic lists in source address table */
        IpV6_CtrlAddrIdxType CtrlAddrIdx = IpV6_VCfgGetSourceAddressTableSize();

        while (0 < CtrlAddrIdx)
        {
          CtrlAddrIdx--;
          IpV6_DListInit(&IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].TempAddrs, &IPV6_PRIVEXT_VAR(TempSrcAddrListDesc));
        }
      }
    }
#endif

    IpV6_Icmp_Init(IpCtrlIdx);
    IpV6_Ndp_Init(IpCtrlIdx);

#if (IPV6_ENABLE_MLD == STD_ON)
    if (IpV6_VCfgHasMldConfig())
    {
      IpV6_Mld_Init(IpCtrlIdx);
    }
#endif

#if ((IPV6_ENABLE_RX_FRAGMENTATION == STD_ON) || (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON))
    if (IpV6_VCfgHasFragmentationConfig())
    {
      IpV6_VInitFragments(IpCtrlIdx);
    }
#endif

#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
    IPV6_CTRL_VAR(IpV6_IcmpNextHopLLAddrOverride) = FALSE;
#endif

#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  }
#endif

#if (IPV6_ENABLE_RX_DBG_COUNTERS == STD_ON)
  VStdMemClr(&IpV6_Dbg_RxCounters, sizeof(IpV6_Dbg_RxCountersType));
#endif

#if (IPV6_ENABLE_TX_DBG_COUNTERS == STD_ON)
  VStdMemClr(&IpV6_Dbg_TxCounters, sizeof(IpV6_Dbg_TxCountersType));
#endif

  IpV6_State = IPV6_STATE_INIT;

} /* End of IpV6_Init() */

/***********************************************************************************************************************
 *  IpV6_MainFunctionRx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV6_CODE) IpV6_MainFunctionRx(void)
{
#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
# if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV6_05.06 */
# else
  const uint8 IpCtrlIdx = 0; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
# endif

# if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  /* #10 Iterate over all configured IP instances. */
  for (IpCtrlIdx = 0; IpCtrlIdx < IpV6_VCfgGetMaxNumCtrls(); IpCtrlIdx++)
# endif
  {

    /* #20 Transmit pending IP fragments. */
    if (IpV6_VCfgHasFragmentationConfig())
    {
      IpV6_VRxFragments(IpCtrlIdx);
    }
  }
#endif
}  /* IpV6_MainFunctionRx() */

/***********************************************************************************************************************
 *  IpV6_MainFunctionTx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV6_CODE) IpV6_MainFunctionTx(void)
{
#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
# if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV6_05.06 */
# else
  const uint8 IpCtrlIdx = 0; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
# endif

# if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  /* #10 Iterate over all configured IP instances. */
  for (IpCtrlIdx = 0; IpCtrlIdx < IpV6_VCfgGetMaxNumCtrls(); IpCtrlIdx++)
# endif
  {

    /* #20 Transmit pending IP fragments. */
    if (IpV6_VCfgHasFragmentationConfig())
    {
      IpV6_VTxFragments(IpCtrlIdx);
    }
  }
#endif
}  /* IpV6_MainFunctionTx() */

/**********************************************************************************************************************
 *  IpV6_MainFunctionState()
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
 */
FUNC(void, IPV6_CODE) IpV6_MainFunctionState(void)
{
  /* #10 Check if module is initialized. */
  if (IPV6_IS_INITIALIZED == FALSE)
  {
    /* module is not initialized. Do nothing. */
  }
  else
  {
#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
    uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV6_05.06 */
#else
    const uint8 IpCtrlIdx = 0; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
#endif

    /* #20 Increment module time counter used for timeout handling. */
    IpV6_Time.Ms += IPV6_MAIN_FCT_CYCLE_TIME_MS;

    if (1000 <= IpV6_Time.Ms)
    {
      IpV6_Time.Ms -= 1000;
      IpV6_Time.S += 1;
    }

    /* #30 Iterate over all configured IP controller instances. */
#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
    IpCtrlIdx = IpV6_VCfgGetMaxNumCtrls();

    while (0 < IpCtrlIdx)
    {
      IpCtrlIdx--;
#endif

      /* #40 Handle IP controller state transitions. */
      if (IPV6_CTRL_VAR(IpV6_CtrlState) != IPV6_CTRL_VAR(IpV6_CtrlPreviousState))
      {
        /* controller state has changed */

        switch (IPV6_CTRL_VAR(IpV6_CtrlPreviousState))
        {
        case IPV6_CTRL_STATE_INIT:
          /* fall through */
        case IPV6_CTRL_STATE_LINK_READY:
          if (IPV6_CTRL_STATE_LLADDR_READY == IPV6_CTRL_VAR(IpV6_CtrlState))
          {
            /* IPV6_CTRL_STATE_INIT --> (IPV6_CTRL_STATE_LINK_READY -->) IPV6_CTRL_STATE_LLADDR_READY */
            IpV6_VCtrlStateOnline(IpCtrlIdx);
          }
          break;

          default:
            /* No deferred state transition necessary */
          break;
        }

        IPV6_CTRL_VAR(IpV6_CtrlPreviousState) = IPV6_CTRL_VAR(IpV6_CtrlState);
      }

      /* #50 Check if IP controller is in operational state. */
      if (IPV6_CTRL_STATE_LLADDR_DAD_FAIL == IPV6_CTRL_VAR(IpV6_CtrlState))
      {
        return;
      }

      IPV6_CTRL_VAR(IpV6_NextRouterProbeIdx) = 0;

      /* #60 Call MainFunctions of submodules ICMP, NDP, Privacy Extensions and MLD if ethernet link is established. */
      if (IPV6_CTRL_STATE_LINK_READY <= IPV6_CTRL_VAR(IpV6_CtrlState))
      {
        IpV6_Icmp_MainFunction(IpCtrlIdx);
        IpV6_Ndp_MainFunction(IpCtrlIdx);

#if (IPV6_ENABLE_MLD == STD_ON)
        if (IpV6_VCfgHasMldConfig())
        {
          IpV6_Mld_MainFunction(IpCtrlIdx);
        }
#endif

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
        if (IpV6_VCfgHasPrivExtConfig())
        {
          if (IPV6_TIME_EXPIRED(IPV6_PRIVEXT_VAR(PrivExtNextIdGenTime)))
          {
            /* [RFC4941 3.5. Regeneration of Randomized Interface Identifiers] */
            (void)IpV6_VPrivExtGenerateTempIdentifier(IpCtrlIdx);
            IPV6_PRIVEXT_VAR(PrivExtNextIdGenTime).S += (IPV6_PRIVEXT_VAR(PrivExtTempPreferredLifetime) - IpV6_VCfgGetPrivExtRegenAdvance());
          }
        }
#endif

#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
      }
#endif
    }

    /* #70 Call MainFunction of DHCPv6 submodule. */
#if (IPV6_ENABLE_DHCP == STD_ON)
    TcpIp_DhcpV6_MainFunction();
#endif
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV6_MainFunctionState() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_ProvideTxBuffer()
 **********************************************************************************************************************/
/*! \brief         Called by upper layer to get a transmit buffer.
 *  \param[in,out] AddrIdPtr           pointer to IP address identifier of the source address that shall be used.
 *                                     If *AddrIdPtr is not the ID of a local unicast address the value will be
 *                                     overwritten with the id of the automatically selected IPv6 address.
 *  \param[in]     SockIdx             index of the socket on which the data should be sent
 *  \param[out]    BufIdxPtr           index of the assigned buffer
 *  \param[in]     DstAddrPtr          IPv6 address of the destination node
 *  \param[out]    BufPtrPtr           pointer to the beginning of the packet payload
 *  \param[in,out] BufLenPtr           minimum desired buffer length.
 *                                     If the function succeeds the available buffer length (>= specified minimum)
 *                                     is returned
 *  \param[out]    TxReqDescrPtr       pointer to TX request descriptor (initialized by this function)
 *  \param[in]     ForceProvideBuffer  Provide a buffer even if NDP address resolution is pending
 *  \return        BUFREQ_OK           destination is reachable and buffer available.
 *                 BUFREQ_E_OVFL       requested buffer length cannot be provided.
 *                 BUFREQ_E_BUSY       currently no free buffer available, address resolution for destination has been
 *                                     triggered or has not finished yet. Try again later.
 *                 BUFREQ_E_NOT_OK     ProvideTxBuffer of lower layer failed, no source address available or invalid
 *                                     parameters.
 *  \context       interrupt and task level
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, IPV6_CODE) IpV6_ProvideTxBuffer(
    IPV6_P2V(IpV6_AddrIdType)                 AddrIdPtr,
    TcpIp_SockIdxType                         SockIdx,
    IPV6_P2C(IpV6_AddrType)                   DstAddrPtr,
    IPV6_P2V(uint8*)                          BufPtrPtr,
    IPV6_P2V(uint16)                          BufLenPtr,
    IPV6_P2V(TcpIp_IpTxRequestDescriptorType) TxReqDescrPtr,
    boolean                                   ForceProvideBuffer)
{
  uint8                                              IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType                               CtrlAddrIdx;

  Eth_PhysAddrType                                   NextHopLLAddr;
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_VAR_NOINIT) NextHopAddrPtr;
  IpV6_ListIdxType                                   DcEntryIdx;
  IpV6_ListIdxType                                   NcEntryIdx;
  BufReq_ReturnType                                  Result;
  boolean                                            NextHopReachable   = FALSE;
  boolean                                            DstAddrOnLink;
#if (IPV6_ENABLE_NDP_DAD == STD_ON)
  boolean                                            UseUnspecifiedAddr = FALSE;
#endif
#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
  boolean                                            SrcAddrOptimistic  = FALSE;
#endif

  /* Det checks */
  /* PRQA S 3109 10 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,                      IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_NOT_INITIALIZED, BUFREQ_E_NOT_OK);

  IpV6_CheckDetErrorReturnValue((NULL_PTR != AddrIdPtr),                  IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_POINTER,     BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_OR_ANY_ADDR_ID(*AddrIdPtr), IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_ADDR_ID,     BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SOCK_IDX(SockIdx),          IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_SOCK_IDX,    BUFREQ_E_NOT_OK);

  IpV6_CheckDetErrorReturnValue((NULL_PTR != TxReqDescrPtr),              IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_POINTER,     BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((NULL_PTR != DstAddrPtr),                 IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_POINTER,     BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((NULL_PTR != BufPtrPtr),                  IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_POINTER,     BUFREQ_E_NOT_OK);
  IpV6_CheckDetErrorReturnValue((NULL_PTR != BufLenPtr),                  IPV6_API_ID_PROVIDE_TX_BUFFER, IPV6_E_INV_POINTER,     BUFREQ_E_NOT_OK);
  /**/

  if (IPV6_ADDR_ID_ANY == (*AddrIdPtr))
  {

#if (IPV6_DEF_CTRL_IDX != IPV6_CTRL_IDX_INV)
    (*AddrIdPtr) = IpV6_BcAddrId[IPV6_DEF_CTRL_IDX];
#else
    /* No default controller configured. */
    return BUFREQ_E_NOT_OK;
#endif
  }

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   *AddrIdPtr);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, *AddrIdPtr);

  if (!IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* The IpAddrId references a multicast address.
     * Multicast addresses can not be used as source address.
     * Set CtrAddrIdx to the ANY addresses in order to perform automatic source address selection.
     */
    CtrlAddrIdx = IPV6_CTRL_ADDR_IDX_INV_OR_ANY;
  }

  IPV6_DBG_COUNTER_TX_INC(ProvTxBufTotalCalls);

#if (IPV6_ENABLE_DEST_ADDR_VALIDATION == STD_ON)
  if (FALSE == IpV6_IsValidDestinationAddress(IpCtrlIdx, DstAddrPtr))
  {
    /* Do not allow sending packets to invalid destination address! */
    IPV6_DBG_COUNTER_TX_INC(ProvTxBufFailInvDstAddr);
    return BUFREQ_E_NOT_OK;
  }
#endif

  /* In order to send the packet we need:
   *  - A source address that is suitable for the destination
   *    - The source address may be specified by the caller or will be determined automatically
   *  - A next hop address (this can be the destination itself if it is a neighbor or a default router)
   *  - The link-layer address of the next hop
   */

  IPV6_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY != CtrlAddrIdx)
  {
    /* Caller provided a source address, we have to respect this choice if the provided address
     * is suitable for sending the packet to the destination.
     */
    IpV6_LocAddrStateType SrcAddrState = IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State;

    switch (SrcAddrState)
    {
    case IPV6_LADDR_STATE_INVALID:
      Result = BUFREQ_E_NOT_OK;
      break;

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    case IPV6_LADDR_STATE_TENTATIVE:
      if (IPV6_CTRL_ADDR_IDX_LINK_LOCAL == CtrlAddrIdx)
      {
        /* send with unspecified address */
        UseUnspecifiedAddr = TRUE;
        Result = BUFREQ_OK;
      }
      else
      {
        /* do not allow transmit with unspecified address */
        Result = BUFREQ_E_NOT_OK;
      }
      break;

# if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
    case IPV6_LADDR_STATE_OPTIMISTIC:
      /* store information that source addres is optimistic. We must not not start link-layer address resolution from optimistic addresses. */
      SrcAddrOptimistic = TRUE;
      Result = BUFREQ_OK;
      break;
# endif
#endif

    default:
      Result = BUFREQ_OK;
    }

    if (BUFREQ_OK != Result)
    {
      /* the provided source address is not suitable for sending to the destination */

      IPV6_DBG_COUNTER_TX_INC(ProvTxBufFailInvSrcAddr);

      IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      return Result;
    }
  }

  /* Lookup entry in destination cache */

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
  /* Get index hint for destination cache entry */
  DcEntryIdx = IpV6_SocketData[SockIdx].DcIdxHint;
#endif

  if (E_OK == IpV6_Ndp_VDCLookup(IpCtrlIdx, DstAddrPtr, &DcEntryIdx))
  {
    /* Found Destination Cache entry with cached source and next hop addresses. */
    IPV6_DBG_COUNTER_TX_INC(ProvTxBufDcHits);

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
    /* store current index of destination cache entry as a hint for next lookup */
    IpV6_SocketData[SockIdx].DcIdxHint = DcEntryIdx;
#endif

    if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY == CtrlAddrIdx)
    {
      /* no explicit source address provided by caller */

      if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY != IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].SourceAddressIdx)
      {
        /* use cached source address */
        CtrlAddrIdx = IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].SourceAddressIdx;

#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
        SrcAddrOptimistic = (boolean)(IPV6_LADDR_STATE_OPTIMISTIC == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].SourceAddressIdx].State);
#endif
      }
      else
      {
        /* suitable source address will be determined below. */
      }
    }

#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
    DstAddrOnLink = IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].DestinationOnLink;
#endif

    /* use cached next hop address */
    NextHopAddrPtr = &IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress;
  }
  else
  {
    /* No corresponding Destination Cache entry has been found.
     * We have to select a matching source and next hop address.
     */

    /* Begin next hop determination */

    DstAddrOnLink = IpV6_VIsDestinationOnLink(IpCtrlIdx, DstAddrPtr);

    if (TRUE == DstAddrOnLink)
    {
      /* The destination is on-link (a neighbor), so we can sent the packet directly. */
      NextHopAddrPtr = DstAddrPtr;
    }
    else
    {
      /* The destination is considered to be off-link, so we have to sent the packet to one of the default routers. */
      NextHopAddrPtr = IpV6_VGetDefaultRouterAddr(IpCtrlIdx, DstAddrPtr, &NextHopReachable);

      if (NULL_PTR == NextHopAddrPtr)
      {
        /* No default routers known, we cannot sent the packet. */

        IPV6_DBG_COUNTER_TX_INC(ProvTxBufFailNoDefRouter);

        IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        return BUFREQ_E_NOT_OK;
      }
    }

    /* create new destination cache entry */
    IpV6_Ndp_VDCGetOrCreateEntry(IpCtrlIdx, DstAddrPtr, &DcEntryIdx);

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
    /* Optimization: store index of destination cache for this socket, so we can check this index first the next time ProvideTxBuffer is called */
    IpV6_SocketData[SockIdx].DcIdxHint = DcEntryIdx;
#endif

  }

  if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY == CtrlAddrIdx)
  {
    /* Source address was not provided by caller and there was no valid entry in the destination cache.
     * Start source address selection algorithm.
     */
    CtrlAddrIdx = IpV6_VSelectSourceAddress(IpCtrlIdx, DstAddrPtr);

    /* store selected source address index in destination cache.
     * The cached source address will be valid until one of the source addresses of this node change.
     * If no valid source address has been found SourceAddressIdx will be set to IPV6_CTRL_ADDR_IDX_INV
     * in order to indicate that noch valid source addres index is in cache.
     */

    IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].SourceAddressIdx = CtrlAddrIdx;

    if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY != CtrlAddrIdx)
    {
      /* valid source address has been found */
#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
      if (IPV6_LADDR_STATE_OPTIMISTIC == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
      {
        SrcAddrOptimistic = TRUE;
      }
#endif
    }
    else
    {
      /* no matching source address found */
      /* update next hop address in destination cache entry */
      IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress, *NextHopAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

      IPV6_DBG_COUNTER_TX_INC(ProvTxBufFailNoSrcAddr);

      IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      return BUFREQ_E_NOT_OK;
    }
  }

  (*AddrIdPtr) = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId;

  /* Source address and next hop determination completed. (CtrlAddrIdx and NextHopAddrPtr valid)*/

  /* Start link-layer address resolution for next hop address */

  if (FALSE == NextHopReachable)
  {
    if (IPV6_ADDR_IS_MULTICAST(*DstAddrPtr))
    {
      NextHopReachable = TRUE;
    }
    else
    {
      /* For non-multicast destinations we must resolve the link-layer address of the next hop neighbor.
       * If the link-layer address is not in the neighbor cache IpV6_VGetLinkLayerAddress() will create
       * an INCOMPLETE entry, trigger address resolution and return BUFREQ_E_BUSY.
       * If the source address is an optimistic address, triggering of address resolution will be inhibited.
       *
       * "A node MUST NOT use an Optimistic Address as the source address of a Neighbor Solicitation."
       * [RFC4429 3.2. Modifications to RFC 2461 Neighbor Discovery]
       */

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
      /* Get index hint for neighbor cache entry */
      NcEntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NcIdxHint;
#endif

#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
      if (TRUE == SrcAddrOptimistic)
      {
        /* check if we have the link-layer address of the next hop in the cache */
        if (E_OK == IpV6_Ndp_VNCLookup(IpCtrlIdx, NextHopAddrPtr, &NcEntryIdx))
        {
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
          /* Get index hint for neighbor cache entry */
          NcEntryIdx = IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NcIdxHint;
#endif
          if (IPV6_REACHABLE_STATE_INCOMPLETE != IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].State)
          {
            IPV6_LL_ADDR_COPY(&NextHopLLAddr[0], &IPV6_CTRL_VAR(IpV6_NeighborCache)[NcEntryIdx].LinkLayerAddress[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
            NextHopReachable = TRUE;
          }
        }

        if ((FALSE == NextHopReachable) && (TRUE == DstAddrOnLink))
        {
          IPV6_P2C(IpV6_AddrType) NextHopRouterAddrPtr;
          /* send the packet to a default router (if there is one) although the destination is on-link
           * since we must not do link-layer address resolution using an optimistic source address */

          NextHopRouterAddrPtr = IpV6_VGetDefaultRouterAddr(IpCtrlIdx, DstAddrPtr, &NextHopReachable);

          if (NULL_PTR != NextHopRouterAddrPtr)
          {
            NextHopAddrPtr = NextHopRouterAddrPtr;
          }
          else
          {
            IPV6_DBG_COUNTER_TX_INC(ProvTxBufNoDefRouterOptimistic);
          }
        }
      }
      else
#endif
      if(E_OK == IpV6_VResolveLinkLayerAddr(IpCtrlIdx, CtrlAddrIdx, NextHopAddrPtr, &NextHopLLAddr[0], &NcEntryIdx))
      {
        /* ok. link-layer address of next hop has been stored in NextHopLLAddr. */
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
        /* store current index of neighbor cache entry as a hint for next lookup */
        IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NcIdxHint = NcEntryIdx;
#endif
        NextHopReachable = TRUE;
      }
      else
      {
        /* link-layer address of next hop unknown. Will be handled below. */
      }
    }
  }

  /* update next hop address in destination cache entry */
  IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress, *NextHopAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
  IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].DestinationOnLink       = DstAddrOnLink;
#endif

  if ((FALSE == NextHopReachable) && (FALSE == ForceProvideBuffer))
  {
    /* link-layer address of NextHopAddr not cached in neighbor cache,
     * address resolution in progress...
     */
    IPV6_DBG_COUNTER_TX_INC(ProvTxBufBusyNextHopUnreachable);

    IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return BUFREQ_E_BUSY;
  }

  /* get tx buffer */

  /* we need additional space for the IPv6 header */
  (*BufLenPtr) += sizeof(IpV6_HdrType);

  {
#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
    uint16 PathMtu;

# if (IPV6_ENABLE_PATH_MTU == STD_ON)
    if (IpV6_VCfgIsPathMtuEnabled())
    {
      PathMtu = IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].Mtu;
    }
    else
# endif
# if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
    if (IpV6_VCfgIsLinkMtuReconfigurationEnabled())
    {
      PathMtu = IPV6_CTRL_VAR(IpV6_DefaultLinkMtu);
    }
    else
# endif
    {
      PathMtu = IpV6_VCfgGetDefaultLinkMtu();
    }

    if (IpV6_VCfgIsTxFragmentationEnabled() && /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
        ( ((*BufLenPtr) > PathMtu)
# if (IPV6_ENABLE_PATH_MTU == STD_ON)
          || (IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].MtuReported < PathMtu)
# endif
        )
       )
    {
      /* Requested payload length does not fit into the MTU of the path.
        * Send fragmented IPv6 packet if possible.
        */

      if ((*BufLenPtr) <= IpV6_VCfgGetTxFragmentBufferSize())
      {
        uint8 FragBufIdx;

        if (E_OK == IpV6_VGetFragmentTxBufferIdx(IpCtrlIdx, &FragBufIdx))
        {
          IPV6_CTRL_VAR(IpV6_NextTxFragmentId)++;
          IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].FragmentId    = IPV6_CTRL_VAR(IpV6_NextTxFragmentId);
          IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].UnfragPartLen = sizeof(IpV6_HdrType);
          IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].FragPartLen   = ((*BufLenPtr) - IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].UnfragPartLen);
          IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].MaxFragLen    = (uint16)(PathMtu - IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].UnfragPartLen - IPV6_FRAGMENT_EXT_HDR_LEN);
          IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].TxOfs         = 0;
          /* IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].NextHopPhysAddr  will be set in IpV6_Transmit() */

          (*BufPtrPtr) = IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[FragBufIdx].Buffer;
          (*BufLenPtr) = IpV6_VCfgGetTxFragmentBufferSize();
          TxReqDescrPtr->BufIdx   = FragBufIdx;
          TxReqDescrPtr->IsEthBuf = FALSE; /* indicates that we are not using an ethernet buffer for this packet */

          IPV6_DBG_COUNTER_TX_INC(ProvTxBufFragmentation);
          Result = BUFREQ_OK;
        }
        else
        {
          IPV6_DBG_COUNTER_TX_INC(ProvTxBufBusyNoFragBuf);
          Result = BUFREQ_E_BUSY; /* currently no free tx fragment buffer available */
        }
      }
      else
      {
        IPV6_DBG_COUNTER_TX_INC(ProvTxBufFailBufLenTooBig);
        Result = BUFREQ_E_NOT_OK; /* requested buffer length too big */
      }
    }
    else
#endif
    {
      /* try to obtain ethernet tx buffer */

      Result = TCPIP_LL_ProvideTxBuffer(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), IPV6_ETHER_TYPE_ID, IPV6_GET_SOCKET_ETHIF_FRAME_PRIO(SockIdx), &TxReqDescrPtr->BufIdx, (Eth_DataType**)BufPtrPtr, BufLenPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
      TxReqDescrPtr->IsEthBuf = TRUE;
#endif

      if (BUFREQ_OK == Result)
      {
        /* nothing to do in this case */
      }
      else if (BUFREQ_E_BUSY == Result)
      {
        IPV6_DBG_COUNTER_TX_INC(ProvTxBufEthIfBusy);
      }
      else
      {
        IPV6_DBG_COUNTER_TX_INC(ProvTxBufEthIfFail);
      }
    }
  }

  if (BUFREQ_OK == Result)
  {
    /* Buffer provided. Write IPv6 header directly into the buffer */

    P2VAR(IpV6_HdrType, AUTOMATIC, IPV6_APPL_VAR)     IpV6HdrPtr;

    IpV6HdrPtr = (IpV6_HdrType*)*BufPtrPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

    /* IpV6HdrPtr->HopLimit will be set in IpV6_Transmit() */
    /* IpV6HdrPtr->VersionTcFlNbo will be set in IpV6_Transmit() */

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
    if ((0 < IpV6_VCfgGetDadTransmits()) && (TRUE == UseUnspecifiedAddr)) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      IPV6_ADDR_COPY(IpV6HdrPtr->SrcAddr, IpV6_AddrUnspecified); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    }
    else
#endif
    {
      IPV6_ADDR_COPY(IpV6HdrPtr->SrcAddr, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    }

    IPV6_ADDR_COPY(IpV6HdrPtr->DstAddr, *DstAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

    /* Store CtrlIdx and BufIdx that belong to the socket until IpV6_Transmit is called.
     * This information is used by IpV6_Transmit(SockIdx, ...)
     */
    TxReqDescrPtr->IpAddrId      = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId;
    TxReqDescrPtr->CtrlIdx       = IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx);
    TxReqDescrPtr->EthPayloadPtr = (*BufPtrPtr);
    TxReqDescrPtr->EthPayloadLen = (*BufLenPtr);
    TxReqDescrPtr->IpPayloadOfs  = sizeof(IpV6_HdrType);
    TxReqDescrPtr->SockIdx       = SockIdx;

    /* ok, next hop mac address could be determined */
    /* move buffer pointer to beginning of IP payload and set the length */
    (*BufPtrPtr)  = &(*BufPtrPtr)[sizeof(IpV6_HdrType)];
    (*BufLenPtr) -= sizeof(IpV6_HdrType);

    IPV6_DBG_COUNTER_TX_INC(ProvTxBufSuccess);
  }

  IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_ProvideTxBuffer() */ /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_CancelTransmit()
 **********************************************************************************************************************/
 /*! \brief     Cancel transmit.
 *              This function may be called instead of IpV6_Transmit() in order to free a buffer provided by
 *              IpV6_ProvideTxBuffer().
 *  \param[in]  TxReqDescrPtr  pointer to TX request descriptor (initialized by IpV6_ProvideTxBuffer())
 *  \note       Cancel transmit is only possible after calling IpV6_ProvideTxBuffer() and before calling IpV6_Transmit()
 *  \context    interrupt and task level
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_CancelTransmit(
    IPV6_P2V(TcpIp_IpTxRequestDescriptorType) TxReqDescrPtr) /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
{
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  Eth_PhysAddrType NextHopLLAddr =
  {
    0, 0, 0, 0, 0, 0
  };

  /* Det checks */
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_INITIALIZED,             IPV6_API_ID_CANCEL_TRANSMIT, IPV6_E_NOT_INITIALIZED);
  IpV6_CheckDetErrorReturnVoid((NULL_PTR != TxReqDescrPtr),     IPV6_API_ID_CANCEL_TRANSMIT, IPV6_E_INV_POINTER);
  /**/

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, TxReqDescrPtr->IpAddrId);

  (void)TCPIP_LL_Transmit(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), TxReqDescrPtr->BufIdx, IPV6_ETHER_TYPE_ID, FALSE, 0, NextHopLLAddr);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_CancelTransmit() */

/**********************************************************************************************************************
 *  IpV6_Transmit()
 **********************************************************************************************************************/
/*! \brief      Transmit an IPv6 packet.
 *  \param[in]  SockIdx                  TcpIp socket index (used for optimization and tx confirmation)
 *  \param[in]  TxReqDescrPtr            pointer to TX request descriptor (initialized by IpV6_ProvideTxBuffer())
 *  \param[in]  NextHeader               NextHeader value (payload stored in buffer must directly start with this header type)
 *  \param[in]  PayloadLen               length of the payload including the header which has been written to the buffer
 *  \param[in]  TxConfirm                if set to TRUE TcpIp_Udp_TxConfirmation() will be called when the packet has been sent.
 *                                       (only valid for UDP packets)
 *  \param[in]  UlTxReqTabIdx            index of a tx request element that is passed back to the UDP in TcpIp_Udp_TxConfirmation()
 *  \param[in]  RetryOnPhysAddrMiss      FALSE: Ethernet buffer is released if physical address has not been resolved yet.
 *                                              Only Std_ReturnType compatible values (TCPIP_OK, TCPIP_E_NOT_OK) are returned.
 *                                       TRUE:  TCPIP_E_PHYS_ADDR_MISS is returned if physical address has not been resolved yet.
 *                                              The ethernet buffer is not released in this case.
 *  \return     TCPIP_OK                 operation completed successfully
 *  \return     TCPIP_E_NOT_OK           operation failed
 *  \return     TCPIP_E_PHYS_ADDR_MISS   operation failed because of an ARP cache miss
 *  \context    task level
 **********************************************************************************************************************/
FUNC(TcpIp_ReturnType, IPV6_CODE) IpV6_Transmit(
    TcpIp_SockIdxType                         SockIdx,
    IPV6_P2C(TcpIp_IpTxRequestDescriptorType) TxReqDescrPtr,  /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
    uint8                                     NextHeader,
    uint16                                    PayloadLen,
    boolean                                   TxConfirm,
    uint8                                     UlTxReqTabIdx,
    boolean                                   RetryOnPhysAddrMiss)
{
  uint8                                          IpCtrlIdx;   /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType                           CtrlAddrIdx;
  uint8                                          BufIdx;      /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  P2VAR(IpV6_HdrType, AUTOMATIC, IPV6_APPL_VAR) IpV6HdrPtr;
  Eth_PhysAddrType                               NextHopLLAddr;
  IpV6_ListIdxType                               DcEntryIdx;
  TcpIp_ReturnType                               Result;
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION != STD_ON)
  IpV6_ListIdxType                               NcEntryIdx;
#endif

  /* Det checks */
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,             IPV6_API_ID_TRANSMIT, IPV6_E_NOT_INITIALIZED, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SOCK_IDX(SockIdx), IPV6_API_ID_TRANSMIT, IPV6_E_INV_SOCK_IDX,    E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != TxReqDescrPtr,       IPV6_API_ID_TRANSMIT, IPV6_E_INV_POINTER,     E_NOT_OK);
  /**/

  BufIdx = TxReqDescrPtr->BufIdx;

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   TxReqDescrPtr->IpAddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, TxReqDescrPtr->IpAddrId);

  if (!IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* ERROR: The IpAddrId stored for the socket references a multicast address.
     * Multicast addresses can not be used as source address.
     */
    IpV6_DetReportError(IPV6_API_ID_TRANSMIT, IPV6_E_INV_ADDR_ID);
    return TCPIP_E_NOT_OK;
  }

  IPV6_DBG_COUNTER_TX_INC(TransmitCalls);

  IpV6HdrPtr = (IpV6_HdrType*)TxReqDescrPtr->EthPayloadPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IpV6HdrPtr->PayloadLenNbo = IPV6_HTONS(PayloadLen);
  IpV6HdrPtr->NextHdr       = NextHeader;
  IpV6HdrPtr->HopLimit      = IPV6_GET_SOCKET_HOP_LIMIT(SockIdx);

  IpV6HdrPtr->VersionTcFlNbo = IPV6_HEADER_VERSION_TC_FL_ZERO_NBO;

#if (IPV6_ENABLE_TRAFFIC_CLASS_API == STD_ON)
  if (0 != (IpV6_SocketData[SockIdx].Flags & IPV6_SOCKET_DATA_FLAG_TRAFFIC_CLASS))
  {
    /* use socket specific 'Traffic Class' value */
    IpV6HdrPtr->VersionTcFlNbo |= (IpV6_SocketData[SockIdx].IpV6HdrVersionTcFlNbo & IPV6_HEADER_MASK_TRAFFIC_CLASS_NBO);
  }
  else
#endif
  {
    /* use controller specific 'Traffic Class' value */
    IpV6HdrPtr->VersionTcFlNbo |= (IpV6_VCfgGetDefaultTrafficClassFlowLabelNbo()  & IPV6_HEADER_MASK_TRAFFIC_CLASS_NBO);
  }

#if (IPV6_ENABLE_TRAFFIC_CLASS_API == STD_ON)
  if (0 != (IpV6_SocketData[SockIdx].Flags & IPV6_SOCKET_DATA_FLAG_FLOW_LABEL))
  {
    /* use socket specific 'Flow Label' value */
    IpV6HdrPtr->VersionTcFlNbo |= (IpV6_SocketData[SockIdx].IpV6HdrVersionTcFlNbo & IPV6_HEADER_MASK_FLOW_LABEL_NBO);
  }
  else
#endif
  {
    /* use controller specific 'Flow Label' value */
    IpV6HdrPtr->VersionTcFlNbo |= (IpV6_VCfgGetDefaultTrafficClassFlowLabelNbo()  & IPV6_HEADER_MASK_FLOW_LABEL_NBO);
  }

  /* SrcAddr and DstAddr are already set by IpV6_ProvideTxBuffer() */

  /*
   * Next hop address should have been stored in destination cache by IPv6_ProvideTxBuffer().
   * Link-layer address of next hop should have been stored in neighbor cache since IPv6_ProvideTxBuffer()
   * returns BUFREQ_BUSY until address resolution for next hop has been completed.
   */

  IPV6_BEGIN_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
  Result = IpV6_Ndp_VDCLookup(IpCtrlIdx, &IpV6HdrPtr->DstAddr, &IpV6_SocketData[SockIdx].DcIdxHint);
  DcEntryIdx = IpV6_SocketData[SockIdx].DcIdxHint;
#else
  Result = IpV6_Ndp_VDCLookup(IpCtrlIdx, &IpV6HdrPtr->DstAddr, &DcEntryIdx);
#endif

  if (TCPIP_OK == Result)
  {
    /* Destination Cache entry found */

#if (IPV6_ENABLE_NDP_INV_NS == STD_ON)
    if (IpV6_VCfgIsNdpInvNsEnabled() && (IPV6_SOCK_IDX_NDP == SockIdx) && (TRUE == IPV6_CTRL_VAR(IpV6_IcmpNextHopLLAddrOverride))) /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */
    {
      /* For the ICMP socket, we allow overriding the next hop link-layer address.
       * This is neccessary only for sending Inverse Neighbor Discovery Solicitations.
       * "Although the destination IP address is the all-node multicast address,
       *  the message is sent only to the target node."
       *  [RFC3122 4.1 Sender Node Processing]
       */
      IPV6_LL_ADDR_COPY(&NextHopLLAddr[0], &IPV6_CTRL_VAR(IpV6_IcmpNextHopLLAddrOverrideValue)[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      IPV6_CTRL_VAR(IpV6_IcmpNextHopLLAddrOverride) = FALSE; /* reset flag */
    }
    else
#endif
    {
#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
      if (E_OK != IpV6_VResolveLinkLayerAddr(IpCtrlIdx, CtrlAddrIdx, &IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress, &NextHopLLAddr[0], &IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NcIdxHint))
#else
      if (E_OK != IpV6_VResolveLinkLayerAddr(IpCtrlIdx, CtrlAddrIdx, &IPV6_CTRL_VAR(IpV6_DestinationCache)[DcEntryIdx].NextHopAddress, &NextHopLLAddr[0], &NcEntryIdx))
#endif
      {
        Result = TCPIP_E_PHYS_ADDR_MISS;
      }
    }

  }

  IPV6_END_CRITICAL_SECTION_DC(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (TCPIP_OK == Result)
  {
   /* tx possible */

#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
    if (FALSE == TxReqDescrPtr->IsEthBuf)
    {
      /*lint -e(645) In case Result == E_OK NextHopLLAddr is always initialized. If Result != E_OK this statement will not be reached. */
      IPV6_LL_ADDR_COPY(IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].NextHopPhysAddr, NextHopLLAddr); /* PRQA S 3354,0310 */ /* MD_IpV6_3354,MD_IpV6_0310 */
      IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].Status = IPV6_FRAG_TX_STATUS_TRANSMIT;
    }
    else
#endif
    {
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      boolean CalculateProtocolChecksum;
#endif

      if (TRUE == TxConfirm)
      {
        /* Store which socket index belongs to the BufIdx of CtrlIdx.
         * This information is used in IpV6_TxConfirmation(CtrlIdx, BufIdx, ...).
         */
        IpV6_EthCtrlData[IpCtrlIdx].EthTxBufs[BufIdx].SocketIdx     = SockIdx;
        IpV6_EthCtrlData[IpCtrlIdx].EthTxBufs[BufIdx].UlTxReqTabIdx = UlTxReqTabIdx;
      }

#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      switch(NextHeader)
      {
# if (TCPIP_SUPPORT_TCP == STD_ON)
        case IPV6_HDR_ID_TCP:
          CalculateProtocolChecksum = (boolean)(FALSE == IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).Tcp);
          break;
# endif
        case IPV6_HDR_ID_UDP:
          CalculateProtocolChecksum = (boolean)(FALSE == IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).Udp);
          break;

        case IPV6_HDR_ID_ICMP:
          CalculateProtocolChecksum = (boolean)(FALSE == IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).Icmp);
          break;

        default:
          /* not supported protocol type */
          IpV6_DetReportInternalError(IPV6_E_NOT_SUPPORTED);
          CalculateProtocolChecksum = FALSE;
          break;
      }

      if (TRUE == CalculateProtocolChecksum)
#endif
      {
        uint32  ProtocolChecksum;
        P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) PayloadPtr = &TxReqDescrPtr->EthPayloadPtr[IPV6_HDR_LEN]; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
        IpBase_SockAddrIn6Type RemSockAddr; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
        IpBase_SockAddrIn6Type LocSockAddr; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

        RemSockAddr.sin6_family = IPBASE_AF_INET6;
        IPV6_ADDR_COPY(RemSockAddr.sin6_addr.addr[0], IpV6HdrPtr->DstAddr.addr[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        LocSockAddr.sin6_family = IPBASE_AF_INET6;
        IPV6_ADDR_COPY(LocSockAddr.sin6_addr.addr[0], IpV6HdrPtr->SrcAddr.addr[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */

        ProtocolChecksum = TcpIp_VCalcPseudoHdrAndChecksum((const IpBase_SockAddrType*)&RemSockAddr, (const IpBase_SockAddrType*)&LocSockAddr, PayloadPtr, PayloadLen, NextHeader);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

        switch (NextHeader)
        {
#if (IPV6_ENABLE_UL_TCP == STD_ON)
          case TCPIP_PROTOCOL_TCP:
            TcpIp_Tcp_StoreChecksumInHeader((uint16)ProtocolChecksum, PayloadPtr);
            break;
#endif
          case TCPIP_PROTOCOL_UDP:
            TcpIp_Udp_StoreChecksumInHeader((uint16)ProtocolChecksum, PayloadPtr);
            break;

          case IPV6_HDR_ID_ICMP:
            IpV6_Icmp_StoreChecksumInHeader((uint16)ProtocolChecksum, PayloadPtr);
            break;

          default:
            {
              /* unsupported protocol.
                * -> do nothing
                */
              IpV6_DetReportInternalError(IPV6_E_NOT_SUPPORTED);
            }
        }  /* switch(Protocol) */
      }

      Result = TCPIP_LL_Transmit(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), BufIdx, IPV6_ETHER_TYPE_ID, TxConfirm, sizeof(IpV6_HdrType) + PayloadLen, NextHopLLAddr);

      if (E_OK == Result)
      {
        IPV6_DBG_COUNTER_TX_INC(TransmitSuccess);
      }
    }
  }

  if ((TCPIP_E_NOT_OK == Result) || ((TCPIP_E_PHYS_ADDR_MISS == Result) && (FALSE == RetryOnPhysAddrMiss)))
  {
#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
    if (FALSE == TxReqDescrPtr->IsEthBuf)
    {
      /* release fragment tx buffer */
      IPV6_CTRL_VAR(IpV6_FragmentTxBufferDescriptors)[BufIdx].Status = IPV6_FRAG_TX_STATUS_UNUSED;
    }
    else
#endif
    {
      /* cancel transmit, calling TCPIP_LL_Transmit with zero length payload will release the buffer */
      (void)TCPIP_LL_Transmit(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), BufIdx, IPV6_ETHER_TYPE_ID, TxConfirm, 0, NextHopLLAddr);
    }

    IPV6_DBG_COUNTER_TX_INC(TransmitFailures);
    Result = TCPIP_E_NOT_OK;
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Transmit() */ /* PRQA S 6010, 6030, 6050, 6060  */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STPAR */

/**********************************************************************************************************************
 *  IpV6_GetSrcAddrPtr()
 **********************************************************************************************************************/
/*! \brief      Get the source address which has been stored in the IPv6 header by IpV6_ProvideTxBuffer()
 *  \param[in]  TxReqDescrPtr  pointer to TX request descriptor (initialized by IpV6_ProvideTxBuffer())
 *  \return     pointer to the source address in the IPv6 header
 *  \context    interrupt and task level
 **********************************************************************************************************************/
FUNC(P2VAR(IpV6_AddrType, AUTOMATIC, IPV6_APPL_VAR), IPV6_CODE) IpV6_GetSrcAddrPtr(
  IPV6_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr)
{
  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,        IPV6_API_ID_GET_SRC_ADDR, IPV6_E_NOT_INITIALIZED, (IpV6_AddrType*)NULL_PTR);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != IpTxReqDescPtr, IPV6_API_ID_GET_SRC_ADDR, IPV6_E_INV_SOCK_IDX,    (IpV6_AddrType*)NULL_PTR);
  /**/

  return &((IpV6_HdrType*)IpTxReqDescPtr->EthPayloadPtr)->SrcAddr; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetSrcAddrPtr() */

/**********************************************************************************************************************
 *  IpV6_GetDstAddrPtr()
 **********************************************************************************************************************/
/*! \brief      Get the destination address which has been stored in the IPv6 header by IpV6_ProvideTxBuffer()
 *  \param[in]  TxReqDescrPtr  pointer to TX request descriptor (initialized by IpV6_ProvideTxBuffer())
 *  \return     pointer to the destination address in the IPv6 header
 *  \context    interrupt and task level
 **********************************************************************************************************************/
FUNC(P2VAR(IpV6_AddrType, AUTOMATIC, IPV6_APPL_VAR), IPV6_CODE) IpV6_GetDstAddrPtr(
  IPV6_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr)
{
  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_INITIALIZED,        IPV6_API_ID_GET_DST_ADDR, IPV6_E_NOT_INITIALIZED, (IpV6_AddrType*)NULL_PTR);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != IpTxReqDescPtr, IPV6_API_ID_GET_DST_ADDR, IPV6_E_INV_SOCK_IDX,    (IpV6_AddrType*)NULL_PTR);
  /**/

  return &((IpV6_HdrType*)IpTxReqDescPtr->EthPayloadPtr)->DstAddr; /* PRQA S 0310 */ /* MD_IpV6_0310 */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetDstAddrPtr() */

/**********************************************************************************************************************
 *  IpV6_RxIndication()
 **********************************************************************************************************************/
/*! \brief      This function is called by the lower layer (e.g. EthIf) and signals receiption of a new ethernet packet
 *  \param[in]  CtrlIdx     index of the controller that received the frame
 *  \param[in]  FrameType   the frame type of the received frame (0x86DD for IPv6)
 *  \param[in]  IsBroadcast indicates that frame has been sent to broadcast address
 *  \param[in]  PhysAddrPtr physical address (source of the ethernet frame)
 *  \param[in]  DataPtr     pointer to the received payload starting with the IPv6 header
 *  \param[in]  LenByte     length of the received payload in bytes
 *  \context    interrupt
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_RxIndication(
  uint8         CtrlIdx,
  Eth_FrameType FrameType,
  boolean       IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr, /* PRQA S 3673 */ /* MD_IpV6_16.07 */
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte)
{
  uint8                  IpCtrlIdx;   /* ip controller index */ /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType   CtrlAddrIdx; /* address index in SourceAddressTable of ip controller */
  IpV6_AddrIdType        AddrId;      /* ip address identifier */

  uint16                 DataLen;     /* length of the IPv6 packet in bytes */
  uint16                 ThisHdrOfs;  /* offset of the current header relative to DataPtr */
  uint16                 PrevHdrOfs;  /* offset of the previous header relative to DataPtr */

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
  uint16                 NxtHdrFieldOfs; /* position of the Next Header field of the current header relative to EthDataPtr */
#endif

  uint8                  NextHdrValue;  /* holds next header type value */
  IPV6_P2V(IpV6_HdrType) IpV6HdrPtr;    /* pointer to main IPv6 header */
  uint16                 ThisHdrLen;    /* contains the length of the currently interpreted header */
  uint32                 VersionTcFl;

#if ((IPV6_VENABLE_HOP_BY_HOP_EXT_HDR_SKIPPING != STD_ON) || (IPV6_VENABLE_DESTINATION_OPT_EXT_HDR_SKIPPING != STD_ON))
  uint8                  OptType;       /* type of the currently processed TLV option (Hop-by-Hop Opt, DestOpt) */
  uint16                 OptDataLen;    /* option data length of the currently processed TLV option (Hop-by-Hop Opt, DestOpt) */
  uint16                 OptOfs;        /* offset to the currently processed TLV option relative to PayloadPtr */ /* PRQA S 0781 */ /* MD_IpV6_05.06 */
#endif

#if (IPV6_ENABLE_MLD == STD_ON)
  IpV6_DListIdxType      McAddrIdx   = IPV6_DLIST_END_IDX;
#endif
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  IpV6_DListIdxType      TempAddrIdx = IPV6_DLIST_END_IDX;
#endif

  IpV6_LocAddrStateType  SrcAddrState;

  /* generic socket address structures for passing packets to TCP and UDP */
  IpBase_SockAddrIn6Type SrcSockAddr;

#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
  boolean checksumVerified = TRUE;
#else
  boolean checksumVerified = FALSE;
#endif

  IPV6_UNUSED_PARAMETER(IsBroadcast);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV6_UNUSED_PARAMETER(FrameType);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_INITIALIZED,   IPV6_API_ID_RX_INDICATION, IPV6_E_NOT_INITIALIZED);
  IpV6_CheckDetErrorReturnVoid((NULL_PTR != DataPtr), IPV6_API_ID_RX_INDICATION, IPV6_E_INV_POINTER);
  /**/

  if (E_OK != IPV6_CTRLIDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
  {
    /* no IPv6 instance configured for this EthIf controller */
    return; /* silently discard packet */
  }

  if (IPV6_CTRL_STATE_LINK_READY > IPV6_CTRL_VAR(IpV6_CtrlState))
  {
    return; /* ignore received packets until transceiver link is active */
  }

  IPV6_DBG_COUNTER_RX_INC(RxTotal);

  /* discard packet if it doesn't contain a complete IPv6 header */
  if (IPV6_HDR_LEN > LenByte)
  {

    IPV6_DBG_COUNTER_RX_INC(DropInvHdr);
    return; /* silently ignore invalid packet */
  }

  /* PRQA S 0310 2 */ /* MD_IpV6_0310 */
  IpV6HdrPtr = (IpV6_HdrType*)DataPtr;

  PrevHdrOfs = 0;
  ThisHdrOfs = IPV6_HDR_LEN;

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
  NxtHdrFieldOfs = IPV6_HEADER_NXT_HDR_FIELD_OFFS; /* Position of next header field in IPv6 header */
#endif

  /* process main IPv6 header */

  VersionTcFl = IPV6_NTOHL(IpV6HdrPtr->VersionTcFlNbo);

  /* ignore packet if IPv6 version != 6 */
  if (IPV6_HEADER_VERSION != IPV6_HEADER_GET_VERSION(VersionTcFl))
  {

    IPV6_DBG_COUNTER_RX_INC(DropInvHdr);
    return; /* silently ignore invalid packet */
  }

  DataLen = IPV6_HDR_LEN + IPV6_NTOHS(IpV6HdrPtr->PayloadLenNbo);

  /* ignore packet if payload length exceeds ethernet payload length */
  if (DataLen > LenByte)
  {

    IPV6_DBG_COUNTER_RX_INC(DropInvLen);
    return; /* silently ignore invalid packet */
  }

  /*
   * We do not have to verify the hop limit.
   * This would only be neccessary if we have to forward the packet.
   * Conformance Test v6LC.1.1.8: Hop Limit Zero - End Node checks this
   */

  /* Compare source address of the received packet with our own local (source) addresses.
   * Since the unspecified address [::] is not allowed for TCP/UDP packets but for NDP packets, this check is done later. */
  if (TRUE == IpV6_VIsValidSourceAddress(IpCtrlIdx, &IpV6HdrPtr->SrcAddr, FALSE, &SrcAddrState, &CtrlAddrIdx))
  {
#if (IPV6_ENABLE_OPTIMISTIC_DAD == STD_ON)
    /* #-- In case the packet is from an address we currently use as optimistic: Accept the packet */
    if (IPV6_LADDR_STATE_OPTIMISTIC == SrcAddrState)
    {
      /* do not filter out packets sent from an address we currently use as optimistic.
       * Otherwise DAD would not detect NA sent in response to DAD NS.
       */
    }
    else
#endif
    {
      /* #-- Otherwise silently ignore the packet sent from an address that is assigned to this node. (packet is sent by this node) */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      IpV6_DListIdxType SatTempAddrIdx;
#endif
      IPV6_DBG_COUNTER_RX_INC(DropOwn);
      /* In this error case the SATLookup is repeated (It has already been done with result E_OK within the call to IpV6_VIsValidSourceAddress(allowUnspecAddr=FALSE), above! */
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
      (void)IpV6_VSATLookup(IpCtrlIdx, &IpV6HdrPtr->SrcAddr, &CtrlAddrIdx, &SatTempAddrIdx);
#else
      (void)IpV6_VSATLookup(IpCtrlIdx, &IpV6HdrPtr->SrcAddr, &CtrlAddrIdx);
#endif
      /* #-- In case it is not a multicast address, calculate the TcpIp LocalAddrId */
      if((CtrlAddrIdx != IPV6_CTRL_ADDR_IDX_INV_OR_ANY) && (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx)))
      {
        /* #-- Handle duplicate address. This handling includes PhysAddr comparison! */
        TcpIp_VDuplicateAddrDetected(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId), (uint8*)(&IpV6HdrPtr->SrcAddr), PhysAddrPtr);  /* PRQA S 0310 */ /* MD_IpV6_0310 */
      }
      return;
      /**/
    }
  }

  /* PRQA S 3415 3 */ /* MD_IpV6_3415 */
  if (IPV6_ADDR_IS_MULTICAST(IpV6HdrPtr->SrcAddr) ||
      IPV6_ADDR_IS_LOOPBACK(IpV6HdrPtr->SrcAddr)  ||
      IPV6_ADDR_EQUALS(IpV6HdrPtr->DstAddr, IpV6HdrPtr->SrcAddr))
  {
    IPV6_DBG_COUNTER_RX_INC(DropInvSrc);
    /* do not accept packets sent from multicast or loopback source address or packets with equal source and destination address */
    return;
  }

  /* discard all packets not addressed to us */
  if (E_OK != IpV6_VIngressFilter(IpCtrlIdx, &IpV6HdrPtr->DstAddr, &CtrlAddrIdx
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
    , &TempAddrIdx
#endif
#if (IPV6_ENABLE_MLD == STD_ON)
    , &McAddrIdx
#endif
    ))
  {
    IPV6_DBG_COUNTER_RX_INC(DropOtherNode);
    return; /* packet for other host */
  }

  if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY != CtrlAddrIdx)
  {
    if (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
    {
      AddrId      = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].IpAddrId;
    }
    else
    {
      uint8 MulticastAddrIdx = IPV6_CTRL_ADDR_IDX_TO_MULTICAST_ADDR_IDX(CtrlAddrIdx);

      AddrId      = IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].IpAddrId;
    }

    IPV6_CTRL_VAR(IpV6_LastBcAddrPtr) = NULL_PTR;
  }
  else
  {
    /* all-nodes multicast destination */
    AddrId      = IPV6_CTRL_VAR(IpV6_BcAddrId);
    IPV6_CTRL_VAR(IpV6_LastBcAddrPtr) = &IpV6HdrPtr->DstAddr;
    IPV6_DBG_COUNTER_RX_INC(RxMulticast);
  }

  /* read extension or uppler-layer headers */

  NextHdrValue = IpV6HdrPtr->NextHdr;

  /***********************************************
   *              NEXT HEADER LOOP               *
   ***********************************************/
  while (IPV6_HDR_ID_NO_NEXT_HDR != NextHdrValue)
  {
    /* ignore packet if end of payload is reached and we're expecting a next header */
    if (ThisHdrOfs >= DataLen)
    {
      IPV6_DBG_COUNTER_RX_INC(DropInvNxtHdr);
      return;
    }

    switch (NextHdrValue)
    {
     /***********************************************
      * Extension Header: Hop-by-Hop                *
      ***********************************************/
    case IPV6_EXT_HDR_ID_HOP_BY_HOP:

      NextHdrValue = DataPtr[ThisHdrOfs];
      ThisHdrLen = IPV6_CALC_EXT_HEADER_LENGTH(DataPtr[ThisHdrOfs+1]);

      /* this header (when present) MUST immediately follow the IPv6 header (see RFC2460 4. IPv6 Extension Headers) */
      if (IPV6_HDR_LEN != ThisHdrOfs)
      {
#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
        IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_NXT_HDR, DataPtr, DataLen, NxtHdrFieldOfs); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif

        IPV6_DBG_COUNTER_RX_INC(DropHdrOrder);
        return;
      }

      /* ignore packet if hop by hop header length exceeds remaining payload length */
      if ((ThisHdrOfs + ThisHdrLen) > DataLen)
      {

        IPV6_DBG_COUNTER_RX_INC(DropInvNxtHdrLen);
        return;
      }

#if (IPV6_VENABLE_HOP_BY_HOP_EXT_HDR_SKIPPING != STD_ON)

      /* process Type-Length-Value (TLV) encoded options */

      OptOfs = ThisHdrOfs + 2; /* first option starts after NextHeader and HdrExtLen bytes */

      /* option loop */
      while (OptOfs < (ThisHdrOfs + ThisHdrLen))
      {
        OptType = DataPtr[OptOfs]; /* first byte of TLV option indicates Type */

        if (OptType == IPV6_EXT_HDR_OPT_TYPE_PAD_1) /* Pad1 option */
        {
          /* Pad1 option consists only of the OptType (1 byte padding) */
          OptOfs++; /* next option starts directly after this one-byte option */
          continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
        }

        OptDataLen = DataPtr[OptOfs+1]; /* second byte of TLV option indicates DataLength in bytes */

        /* ignore packet if option length exceeds HopByHop header length */
        if ((OptOfs + IPV6_TLV_OPT_DATA_OFFS + OptDataLen) > (ThisHdrOfs + ThisHdrLen))
        {
          IPV6_DBG_COUNTER_RX_INC(DropInvOptLen);
          return;
        }

        switch (OptType)
        {
          /* The only defined options by RFC2460 4.3 are the Pad0 and PadN options.
           * Pad0 is handled above, since it's a special 1-byte option.
           */

        case IPV6_EXT_HDR_OPT_TYPE_PAD_N: /* PadN option */
          /* ignore padding data (2 + N byte padding) */
          break;

        default:
          if (E_OK != IpV6_VHandleUnknownTlvOption(AddrId, DataPtr, DataLen, OptOfs)) /* PRQA S 0310 */ /* MD_IpV6_0310 */
          {
            /* we have to discard the packet */
            IPV6_DBG_COUNTER_RX_INC(DropUknOpt);
            return;
          }
        }

        OptOfs += IPV6_TLV_OPT_DATA_OFFS + OptDataLen; /* skip to next option */
      }

#else

#endif /* (IPV6_VENABLE_HOP_BY_HOP_EXT_HDR_SKIPPING != STD_ON) */

      break;

    /***********************************************
     * Extension Header: Destination Options       *
     ***********************************************/
    case IPV6_EXT_HDR_ID_DST_OPT:

      NextHdrValue = DataPtr[ThisHdrOfs];
      ThisHdrLen = IPV6_CALC_EXT_HEADER_LENGTH(DataPtr[ThisHdrOfs+1]);

      /* ignore packet if destination options header length exceeds remaining payload length */
      if ((ThisHdrOfs + ThisHdrLen) > DataLen)
      {

        IPV6_DBG_COUNTER_RX_INC(DropInvNxtHdrLen);
        return;
      }

#if (IPV6_VENABLE_DESTINATION_OPT_EXT_HDR_SKIPPING != STD_ON)

      /* process TLV encoded options */

      OptOfs = ThisHdrOfs + 2; /* first option starts after NextHeader and HdrExtLen bytes */

      /* option loop */
      while (OptOfs < (ThisHdrOfs + ThisHdrLen))
      {
        OptType = DataPtr[OptOfs]; /* first byte of TLV option indicates Type */

        if (OptType == IPV6_EXT_HDR_OPT_TYPE_PAD_1) /* Pad1 option */
        {
          /* Pad1 option consists only of the OptType (1 byte padding) */

          OptOfs++; /* next option starts directly after this one-byte option */
          continue; /* PRQA S 0770 */ /* MD_IpV6_0770 */
        }

        OptDataLen = DataPtr[OptOfs+1]; /* second byte of TLV option indicates DataLength in bytes */

        /* ignore packet if option length exceeds DestinationOpt header length */
        if ((OptOfs + IPV6_TLV_OPT_DATA_OFFS + OptDataLen) > (ThisHdrOfs + ThisHdrLen))
        {

          IPV6_DBG_COUNTER_RX_INC(DropInvOptLen);
          return;
        }

        switch (OptType)
        {
          /* The only defined options by RFC2460 4.6 are the Pad0 and PadN options.
           * Pad0 is handled above, since it's a special 1-byte option.
           */

        case IPV6_EXT_HDR_OPT_TYPE_PAD_N: /* PadN option */
          /* ignore padding data (2 + N byte padding) */
          break;

        default:
          if (E_OK != IpV6_VHandleUnknownTlvOption(AddrId, DataPtr, DataLen, OptOfs)) /* PRQA S 0310 */ /* MD_IpV6_0310 */
          {
            /* we have to discard the packet */
            IPV6_DBG_COUNTER_RX_INC(DropUknOpt);
            return;
          }
        }

        OptOfs += IPV6_TLV_OPT_DATA_OFFS + OptDataLen; /* skip to next option */
      }

#else

#endif /* (IPV6_VENABLE_DESTINATION_OPT_EXT_HDR_SKIPPING != STD_ON) */

      break;

    /***********************************************
     * Extension Header: Routing                   *
     ***********************************************/
    case IPV6_EXT_HDR_ID_ROUTING:

      NextHdrValue = DataPtr[ThisHdrOfs];
      ThisHdrLen = IPV6_CALC_EXT_HEADER_LENGTH(DataPtr[ThisHdrOfs+1]);

      /* ignore packet if routing header length exceeds remaining payload length */
      if ((ThisHdrOfs + ThisHdrLen) > DataLen)
      {
        IPV6_DBG_COUNTER_RX_INC(DropInvNxtHdrLen);
        return;
      }

      /* [RFC2460 4.4.] */
      /* we do not have support for any Routing Type, so this field contains always an unrecognized type */

      if (0 == DataPtr[ThisHdrOfs+3]) /* Segments Left */
      {
        /* We have to ignore the routing header and proceed to the next header */
      }
      else /* Segments Left is non-zero */
      {
        /* We have to discard the packet and send an ICMP Parameter Problem message (Code 0) pointing to the unrecognized Routing Type */
#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
        IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_ERR_HDR_FIELD, DataPtr, DataLen, ThisHdrOfs + 2); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif

        NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR; /* stop processing */
      }

#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      if(IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).IpRouting == FALSE)
      {
        checksumVerified = FALSE;
      }
#endif

      break;

    /***********************************************
     * Extension Header: Fragment                  *
     ***********************************************/
    case IPV6_EXT_HDR_ID_FRAGMENT:

      (void)IpV6_VProcessFragmentHeader(AddrId, DataPtr, DataLen, ThisHdrOfs, PrevHdrOfs);

      /* Stop processing of fragmented packet here.
       * Other headers will be processed if all fragments haven been reassembled.
       */
      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR;
      ThisHdrLen   = IPV6_FRAGMENT_EXT_HDR_LEN; /* fixed length */

#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      /* in case of fragmentation the upper layer checksum never can be calculated */
      checksumVerified = FALSE;
#endif

      break;

    /***********************************************
     * Extension Header: AH                        *
     ***********************************************/
    case IPV6_EXT_HDR_ID_AUTH:

      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR; /* stop processing */
      ThisHdrLen = 0;
#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      checksumVerified = FALSE;  /* if IPSec is used no upper layer checksums can be calculated in HW */
#endif
      break;

    /***********************************************
     * Extension Header: ESP                       *
     ***********************************************/
    case IPV6_EXT_HDR_ID_ESP:

      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR; /* stop processing */
      ThisHdrLen = 0;
#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      checksumVerified = FALSE;  /* if IPSec is used no upper layer checksums can be calculated in HW */
#endif

      break;

    /***********************************************
     * Upper Layer Header: ICMP                    *
     ***********************************************/
    case IPV6_HDR_ID_ICMP:
#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      if(IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).Icmp == FALSE)
      {
        checksumVerified = FALSE;
      }
#endif

      IpV6_Icmp_RxIndication(AddrId, DataPtr, DataLen, PhysAddrPtr, ThisHdrOfs, checksumVerified);
      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR;
      ThisHdrLen = 0;
      break;

    /***********************************************
     * Upper Layer Header: TCP                     *
     ***********************************************/
#if (IPV6_ENABLE_UL_TCP == STD_ON)
    case IPV6_HDR_ID_TCP:
      if ((!IPV6_ADDR_IS_UNSPECIFIED(IpV6HdrPtr->SrcAddr)) && IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
      {
        SrcSockAddr.sin6_family = IPBASE_AF_INET6;
        IPV6_ADDR_COPY(SrcSockAddr.sin6_addr, IpV6HdrPtr->SrcAddr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
        if(IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).Tcp == FALSE)
        {
          checksumVerified = FALSE;
        }
#endif

        TcpIp_Tcp_RxIndication(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(AddrId), (IpBase_SockAddrType*)&SrcSockAddr, &DataPtr[ThisHdrOfs], (DataLen - ThisHdrOfs) /* PRQA S 0310 */ /* MD_IpV6_0310 */
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
          , checksumVerified
#endif
          );
      }
      else
      {
        /* TCP packets sent from unspecified address [::] or sent to a multicast address are invalid. */
        /* ignore packet */
      }
      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR;
      ThisHdrLen = 0;
      break;
#endif /* (IPV6_ENABLE_UL_TCP == STD_ON) */

    /***********************************************
     * Upper Layer Header: UDP                     *
     ***********************************************/
    case IPV6_HDR_ID_UDP:
      if (!IPV6_ADDR_IS_UNSPECIFIED(IpV6HdrPtr->SrcAddr))
      {
#if (IPV6_ENABLE_DHCP == STD_ON)
        P2CONST(TcpIp_Udp_HdrType, AUTOMATIC, TCPIP_APPL_DATA) UdpHdrPtr = (IPV6_P2C(TcpIp_Udp_HdrType))&DataPtr[ThisHdrOfs]; /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif

        SrcSockAddr.sin6_family = IPBASE_AF_INET6;
        IPV6_ADDR_COPY(SrcSockAddr.sin6_addr, IpV6HdrPtr->SrcAddr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

#if(TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
        if(IPV6_CTRL_VAR(TcpIp_IpV6_ChecksumOffloadingSupport).Udp == FALSE)
        {
          checksumVerified = FALSE;
        }
#endif

#if (IPV6_ENABLE_DHCP == STD_ON)
        if ((TCPIP_DHCPV6_SERVER_PORT == IPBASE_HTON16(UdpHdrPtr->SrcPort)) && (TCPIP_DHCPV6_CLIENT_PORT == IPBASE_HTON16(UdpHdrPtr->TgtPort)))
        {
          /* This is a packet from a DHCPv6 server for a DHCPv6 client. */
          IpBase_SockAddrIn6Type IpRxSockAddrLoc;

          IpRxSockAddrLoc.sin6_family   = IPBASE_AF_INET6;
          IPV6_ADDR_COPY(IpRxSockAddrLoc.sin6_addr, IpV6HdrPtr->DstAddr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
          IpRxSockAddrLoc.sin6_port     = 0;  /* undefined */

          TcpIp_DhcpV6_RxIndication(IpCtrlIdx, &DataPtr[ThisHdrOfs], DataLen - ThisHdrOfs, (IpBase_SockAddrType *)&SrcSockAddr, (IpBase_SockAddrType *)&IpRxSockAddrLoc /* PRQA S 0310 */ /* MD_IpV4_11.04 */
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
            , checksumVerified
#endif
            );
        }
        else
#endif
        {
          (void)TcpIp_Udp_RxIndication(IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(AddrId),
            (IPV6_P2V(IpBase_SockAddrType))&SrcSockAddr, /* PRQA S 0310 */ /* MD_IpV6_0310 */
            &DataPtr[ThisHdrOfs], DataLen - ThisHdrOfs
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
            , checksumVerified
#else
            , FALSE
#endif
            );
        }
      }
      else
      {
        /* UDP packets sent from unspecified address [::] are invalid. */
        /* ignore packet */
      }

      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR;
      ThisHdrLen = 0;
      break;

    /***********************************************
     *                NO NEXT HEADER               *
     ***********************************************/
    case IPV6_HDR_ID_NO_NEXT_HDR:
      /* packet doesn't contain any payload */
      ThisHdrLen = 0;
      break;

    /***********************************************
     *              UNSUPPORTED HEADER             *
     ***********************************************/
    default:
      /* Unknown header found. We should send an ICMP Parameter Problem message Code 1 according to RFC2460 4. */

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
      IpV6_Icmp_VTxErrParameterProblem(AddrId, IPV6_ICMP_MSG_PARAM_PROBLEM_CODE_UNKNOWN_NXT_HDR, DataPtr, DataLen, NxtHdrFieldOfs); /* PRQA S 0310 */ /* MD_IpV6_0310 */
#endif

      NextHdrValue = IPV6_HDR_ID_NO_NEXT_HDR;
      ThisHdrLen = 0;
    }

#if (IPV6_ENABLE_ICMP_TX_PARAM_PROBLEM == STD_ON)
    NxtHdrFieldOfs = ThisHdrOfs;
#endif

    /* move to next header */
    PrevHdrOfs = ThisHdrOfs;
    ThisHdrOfs += ThisHdrLen;
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_RxIndication() */ /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_TxConfirmation()
 **********************************************************************************************************************/
/*! \brief      This function is called by the lower layer (ethernet) to confirm data transmission
 *  \param[in]  CtrlIdx   index of the controller that transmitted the frame
 *  \param[in]  BufIdx    index of the tx buffer that has been transmitted
 *  \context    interrupt
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_TxConfirmation(
  uint8 CtrlIdx,
  uint8 BufIdx )
{
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  P2CONST(IpV6_EthTxBufDataType, AUTOMATIC, IPV6_VAR_NOINIT) EthTxBufDataPtr;

  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_INITIALIZED, IPV6_API_ID_TX_CONFIRMATION, IPV6_E_NOT_INITIALIZED);
  /**/

  if (E_OK != IPV6_CTRLIDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
  {
    /* no IPv6 instance configured for this EthIf controller */
    return; /* silently ignore notification */
  }

  EthTxBufDataPtr = &IpV6_EthCtrlData[IpCtrlIdx].EthTxBufs[BufIdx];

  TcpIp_Udp_TxConfirmation(EthTxBufDataPtr->SocketIdx, EthTxBufDataPtr->UlTxReqTabIdx);

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_TxConfirmation() */

/**********************************************************************************************************************
 *  IpV6_Cbk_TrcvLinkStateChg()
 **********************************************************************************************************************/
/*! \brief        Called by lower layer (e.g. EthIf) to indicate a change of the transceiver link state
 *  \param[in]    CtrlIdx         index of the ethernet controller
 *  \param[in]    TrcvLinkState   new state of the transceiver
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_Cbk_TrcvLinkStateChg(
  uint8                 CtrlIdx,
  EthTrcv_LinkStateType TrcvLinkState)
{
#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8                  IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
#else
  const uint8            IpCtrlIdx = 0;
#endif

  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_INITIALIZED,             IPV6_API_ID_CBK_TRCV_LINK_STATE_CHG, IPV6_E_NOT_INITIALIZED);
  /**/

#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
  if (E_OK != IPV6_CTRLIDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
#else
  if (CtrlIdx != IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx))
#endif
  {
    /* no IPv6 instance configured for this EthIf controller */
    return; /* silently ignore notification */
  }

  if (ETHTRCV_LINK_STATE_ACTIVE != TrcvLinkState)
  {
    /* LINK DOWN */

    IpV6_VCtrlStateShutdown(IpCtrlIdx);

#if (IPV6_ENABLE_TCPIP == STD_ON)
    TcpIp_Cbk_VTrcvLinkStateChg(IpCtrlIdx, IPBASE_AF_INET6, FALSE);
#endif
  }
  else
  {
    /* LINK UP */

    IpV6_VCtrlStateStartup(IpCtrlIdx);

#if (IPV6_ENABLE_TCPIP == STD_ON)
    TcpIp_Cbk_VTrcvLinkStateChg(IpCtrlIdx, IPBASE_AF_INET6, TRUE);
#endif
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_Cbk_TrcvLinkStateChg() */

/***********************************************************************************************************************
 *  IpV6_Ip_RequestIpAddrAssignment
 **********************************************************************************************************************/
/*! \brief        Initiate the local IP address assignment for the IP address specified by IpAddrId.
 *  \param[in]    IpAddrId            IP address index specifying the IP address for which an
 *                                    assignment shall be initiated.
 *  \param[in]    Type                type of IP address assignment which shall be initiated
 *  \param[in]    LocalIpAddrPtr      pointer to structure containing the IP address which shall be
 *                                    assigned to the EthIf controller indirectly specified via
 *                                    LocalAddrId. Note: This parameter is only used in case the
 *                                    parameters Type is set to TCPIP_IPADDR_ASSIGNMENT_STATIC.
 *  \param[in]    Netmask             Network mask of IPv4 address or address prefix of IPv6 address in CIDR Notation.
 *  \param[in]    DefaultRouterPtr    Pointer to struct where the IP address of the default router
 *                                    (gateway) is stored (struct member 'port' is not used and of
 *                                    arbitrary value) (IPv4 only)
 *  \return       E_OK                The request has been accepted
 *                E_NOT_OK            The request has not been accepted
 *  \context      task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) IpV6_Ip_RequestIpAddrAssignment(
    uint8 IpAddrId,
    TcpIp_IpAddrAssignmentType Type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr,
    uint8 Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr )
{
  Std_ReturnType RetVal;
  uint8                  IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  IPV6_UNUSED_PARAMETER(Netmask);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV6_UNUSED_PARAMETER(DefaultRouterPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   IpAddrId);

  if (IPV6_CTRL_VAR(IpV6_CtrlState) > IPV6_CTRL_STATE_INIT)
  {
    if(Type == TCPIP_IPADDR_ASSIGNMENT_STATIC)
    {
      uint32  PreferredLifetime = IPV6_LIFETIME_UNLIMITED; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
      uint32  ValidLifetime     = IPV6_LIFETIME_UNLIMITED; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
      boolean NoDad             = FALSE;

      RetVal = IpV6_SetAddress( IpAddrId, (IPV6_P2C(IpV6_AddrType))&(((IPV6_P2C(IpBase_SockAddrIn6Type))LocalIpAddrPtr)->sin6_addr), PreferredLifetime, ValidLifetime, NoDad);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    }
#if (IPV6_ENABLE_DHCP == STD_ON)
    else if(Type == TCPIP_IPADDR_ASSIGNMENT_DHCP)
    {
      /* trigger DHCPv6 address assignment */
      RetVal = TcpIp_DhcpV6_StartAddressAssignment(IpCtrlIdx);
    }
#endif
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    /* offline, no address assignment can be triggered */
    RetVal = E_NOT_OK;
  }

  return RetVal;
}

/**********************************************************************************************************************
 *  IpV6_SetAddress()
 **********************************************************************************************************************/
/*! \brief        Assign an IPv6 address to the node.
 *                If the address is already assigned to the node, only the lifetimes will be updated.
 *                Specifying an address with ValidLifetime of zero will remove the address from the node.
 *  \param[in]    AddrId            ip address identifier
 *  \param[in]    AddressPtr        Address to be assigned/removed (If this parameter is NULL_PTR, the configured default address will be set)
 *  \param[in]    PreferredLifetime Time in seconds this address may be used as preferred address
 *  \param[in]    ValidLifetime     Time in seconds this address is valid
 *  \param[in]    NoDad             Do not perform DAD for this address (address may be used immediately)
 *  \retval       E_OK              Address assigned, removed or lifetimes updated
 *  \retval       E_NOT_OK          Assignment failed (maximum number of source addresses reached or invalid parameters)
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_SetAddress(
  IpV6_AddrIdType AddrId,
  P2CONST(IpV6_AddrType, AUTOMATIC, TCPIP_APPL_DATA) AddressPtr,
  uint32  PreferredLifetime,
  uint32  ValidLifetime,
  boolean NoDad)
{
  Std_ReturnType         Result = E_OK;
  uint8                  IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType   CtrlAddrIdx;

  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_SET_ADDRESS, IPV6_E_INV_ADDR_ID, E_NOT_OK);
                                /* AddressPtr may be NULL_PTR */
  /**/

  if (ValidLifetime < PreferredLifetime)
  {
    /* ValidLifetime must not be less than PrefrerredLifetime */
    return E_NOT_OK;
  }

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY == CtrlAddrIdx)
  {
    /* AutoSelect/Broadcast addresses cannot be set */
    return E_NOT_OK;
  }

  if (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* Set Unicast address */

    if ((IPV6_ADDR_CFG_SRC_STATIC != IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].AddressAssignVariant) &&
        (IPV6_ADDR_CFG_SRC_DHCP   != IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].AddressAssignVariant))
    {
      /* only STATIC and DHCP addresses can be changed via this api */
      return E_NOT_OK;
    }

    if (NULL_PTR == AddressPtr)
    {
#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
      /* Set default address (if available) or unspecified address */
      uint8 TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].DefaultAddrIdx;

      if (IPV6_DEFAULT_ADDR_IDX_INV > TmpDefaultAddrIdx)
      {
        AddressPtr = &IpV6_DefaultAddresses[TmpDefaultAddrIdx];
      }
      else
#endif
      {
        AddressPtr = &IpV6_AddrUnspecified;
      }

      PreferredLifetime = IPV6_LIFETIME_UNLIMITED;
      ValidLifetime     = IPV6_LIFETIME_UNLIMITED;
    }

    IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (IPV6_LADDR_STATE_INVALID != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      if (TRUE == IpV6_CmpNetAddr(AddressPtr, &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address))
      {
        /* address has not changed, just update lifetimes... */

#if (IPV6_ENABLE_NDP_DAD == STD_ON)
        if (IPV6_LADDR_STATE_TENTATIVE == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
        {
          /* do not update next event time here, if Duplicate Address Detection is still in progress */
          IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].PreferredTime, PreferredLifetime, NULL_PTR);
          IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].ValidTime,     ValidLifetime,     NULL_PTR);
        }
        else
#endif
        {
          /* This address is already configured, so we just need to update the lifetimes */
          IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].PreferredTime, PreferredLifetime, &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].NextEventTime);
          IPV6_SET_LIFETIME_S(IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].ValidTime,     ValidLifetime,     &IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].NextEventTime);
        }

      }
      else
      {
        /* address changes. remove old address and configure new */
        IpV6_VHandleSourceAddrAssignmentChange(IpCtrlIdx, CtrlAddrIdx, FALSE);
        IpV6_VSATRemoveEntry(IpCtrlIdx, CtrlAddrIdx); /* changes state to IPV6_LADDR_STATE_INVALID */
      }
    }

    if (IPV6_LADDR_STATE_INVALID == IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      if (!IPV6_ADDR_IS_UNSPECIFIED(*AddressPtr))
      {
        /* add address */
        Result = IpV6_Ndp_VAddSourceAddress(AddrId, AddressPtr, PreferredLifetime, ValidLifetime, 0, (TRUE == NoDad) ? IPV6_SRC_ADDR_DAD_MODE_NONE : IPV6_SRC_ADDR_DAD_MODE_OPTIMISTIC);
      }

#if (IPV6_ENABLE_ADDR_NVM_STORAGE_SUPPORT == STD_ON)
      if (E_OK == Result)
      {
        uint16_least NvmDataOffset = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].TcpIpNvmDataOffset;

        if (TCPIP_NVM_BLOCK_SIZE > NvmDataOffset)
        {
          /* An NvM block has been configured for this AddrId. */
          uint8 NvmErrStatus = NVM_REQ_PENDING;
          (void)NvM_GetErrorStatus(TCPIP_NVM_BLOCK_ID, &NvmErrStatus);

          if(NVM_REQ_PENDING != NvmErrStatus)
          {
            IPV6_ADDR_COPY(*IPV6_NVM_GET_ADDR_PTR(NvmDataOffset), *AddressPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

            /* signal the NvM that the value of the permanent RAM block has been changed */
            (void)NvM_SetRamBlockStatus(TCPIP_NVM_BLOCK_ID, TRUE);
          }
          else
          {
            return E_NOT_OK;
          }
        }
      }
#endif
    }

    IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    /* Set Muticast address */
    uint8 MulticastAddrIdx = IPV6_CTRL_ADDR_IDX_TO_MULTICAST_ADDR_IDX(CtrlAddrIdx);

    if (NULL_PTR == AddressPtr)
    {
#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
      /* Set default address */
      uint8 TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].DefaultAddrIdx;

      if (IPV6_DEFAULT_ADDR_IDX_INV > TmpDefaultAddrIdx)
      {
        AddressPtr = &IpV6_DefaultAddresses[TmpDefaultAddrIdx];
      }
      else
#endif
      {
        AddressPtr = &IpV6_AddrUnspecified;
      }
    }

    if (IPV6_ADDR_IS_MULTICAST(*AddressPtr) || IPV6_ADDR_IS_UNSPECIFIED(*AddressPtr)) /* PRQA S 3415 */ /* MD_IpV6_3415 */
    {
      /* OK: A multicast or the unspecified address shall be assigned. */

      if (MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_DynamicMulticastAddressCount))
      {
        /* OK: Multicast Address is configured as runtime changeable */

        if (!IPV6_ADDR_EQUALS(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], *AddressPtr))
        {
          /* Change IP Address assignment */
          TcpIp_LocalAddrIdType TmpTcpIpAddrId = IPV6_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].IpAddrId);

          if (IPV6_ADDR_IS_MULTICAST(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx]))
          {
            /* There is curretly a different muticast assigned for this IpAddrId.
             * Notify upper-layer about removed assignment...
             */
            IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], FALSE);

            TcpIp_Cbk_VLocalIpAssignmentChg(TmpTcpIpAddrId, TCPIP_IPADDR_STATE_UNASSIGNED); /* PRQA S 0310 */ /* MD_IpV6_0310 */
          }

          /* Change IP address */
          IPV6_ADDR_COPY(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], *AddressPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */

          if (IPV6_ADDR_IS_MULTICAST(IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx]))
          {
            /* A new address has been assigned for this IpAddrId.
             * Notify upper-layer...
             */
            IPV6_VJOIN_MULTICAST_GROUP(IpCtrlIdx, &IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx], TRUE);

            TcpIp_Cbk_VLocalIpAssignmentChg(TmpTcpIpAddrId, TCPIP_IPADDR_STATE_ASSIGNED); /* PRQA S 0310 */ /* MD_IpV6_0310 */
          }
        }
        else
        {
          /* The value of *AddressPtr is already assigned to this IpAddrId. No action neccessary. */
          Result = E_OK;
        }
      }
      else
      {
        /* ERROR: The multicast address of this IpAddrId was not configured to be changeable during runtime. */
        Result = E_NOT_OK;
      }
    }
    else
    {
      /* ERROR: The value of *AddressPtr does not represent the unspecified [::] or a multicast address ff00::/8 */
      Result = E_NOT_OK;
    }
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_SetAddress() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV6_GetLocalAddress()
 **********************************************************************************************************************/
/*! \brief        Get address configured for address identifier.
 *  \param[in]    AddrId                 ip address identifier
 *  \param[out]   AddressPtr             Address configured for specified address identifier
 *  \param[out]   AddressPrefixBitLenPtr Length of the CIDR address prefix in bit. (parameter not used if set to NULL_PTR)
 *  \retval       E_OK                   Returned valid address
 *  \retval       E_NOT_OK               The address identifier is invalid or actually no address configured for this identifier
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_GetLocalAddress(
  IpV6_AddrIdType         AddrId,
  IPV6_P2V(IpV6_AddrType) AddressPtr,
  IPV6_P2V(uint8)         AddressPrefixBitLenPtr)
{
  Std_ReturnType         Result;
  uint8                  IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType   CtrlAddrIdx;
  uint8                  PrefixBitLen;

  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_GET_LOCAL_ADDRESS, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != AddressPtr,        IPV6_API_ID_GET_LOCAL_ADDRESS, IPV6_E_INV_POINTER, E_NOT_OK);
  /**/

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (IPV6_CTRL_ADDR_IDX_INV_OR_ANY == CtrlAddrIdx)
  {
    /* Return Last received all-nodes multicast address */
    if (NULL_PTR != IPV6_CTRL_VAR(IpV6_LastBcAddrPtr))
    {
      IPV6_ADDR_COPY(*AddressPtr, *IPV6_CTRL_VAR(IpV6_LastBcAddrPtr)); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      Result = E_OK;
    }
    else
    {
      /* no all-nodes muticast received on this controller so far */
      IPV6_ADDR_COPY(*AddressPtr, IpV6_AddrAllNodesLL); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      Result = E_OK;
    }

    PrefixBitLen = IPV6_MULTICAST_PREFIX_LEN_BIT;
  }
  else if (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* Return unicast address */
    IPV6_BEGIN_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (IPV6_LADDR_STATE_INVALID != IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].State)
    {
      IPV6_ADDR_COPY(*AddressPtr, IPV6_CTRL_VAR(IpV6_SourceAddressTable)[CtrlAddrIdx].Address); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      Result = E_OK;
    }
    else
    {
      /* There is currently no unicast address assigned to this AddrId */
      IPV6_ADDR_COPY(*AddressPtr, IpV6_AddrUnspecified); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      Result = E_OK;
    }

    /* Prefix of unicast address is always 64 bit. */
    PrefixBitLen = (IPV6_PREFIX_LEN_BYTE * IPV6_BITS_IN_BYTE);

    IPV6_END_CRITICAL_SECTION_SAT(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    /* Return multicast address */

    uint8 MulticastAddrIdx = IPV6_CTRL_ADDR_IDX_TO_MULTICAST_ADDR_IDX(CtrlAddrIdx);

    if (MulticastAddrIdx < IPV6_CTRL_VAR(IpV6_DynamicMulticastAddressCount))
    {
      /* Return dynamic (runtime changeable) multicast address */
      IPV6_ADDR_COPY(*AddressPtr, IPV6_CTRL_VAR(IpV6_DynamicMulticastAddresses)[MulticastAddrIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      PrefixBitLen = IPV6_MULTICAST_PREFIX_LEN_BIT;
      Result = E_OK;
    }
    else
    {
#if (IPV6_DEFAULT_ADDRESS_COUNT > 0)
      /* Return static multicast address */
      uint8 TmpDefaultAddrIdx = IPV6_CTRL_VAR(IpV6_MulticastAddressConfig)[MulticastAddrIdx].DefaultAddrIdx;

      IPV6_ADDR_COPY(*AddressPtr, IpV6_DefaultAddresses[TmpDefaultAddrIdx]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
      PrefixBitLen = IPV6_MULTICAST_PREFIX_LEN_BIT;
      Result = E_OK;
#else
      PrefixBitLen = 0;
      Result = E_NOT_OK;
#endif
    }
  }

  if (AddressPrefixBitLenPtr != NULL_PTR)
  {
    *AddressPrefixBitLenPtr = PrefixBitLen;
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetLocalAddress() */

/**********************************************************************************************************************
 *  IpV6_GetDefaultRouterAddress()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, IPV6_CODE) IpV6_GetDefaultRouterAddress(
  IpV6_AddrIdType              ipAddrId,
  IPV6_P2V(IpBase_AddrIn6Type) routerAddressPtr,
  IPV6_P2V(boolean)            routerIsReachablePtr)
{
  Std_ReturnType          retVal;
  IPV6_P2C(IpV6_AddrType) tmpRouterAddrPtr;

  /* Det checks */
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(ipAddrId),  IPV6_API_ID_GET_DEF_ROUTER_ADDRESS, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(routerAddressPtr     != NULL_PTR, IPV6_API_ID_GET_DEF_ROUTER_ADDRESS, IPV6_E_INV_POINTER, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(routerIsReachablePtr != NULL_PTR, IPV6_API_ID_GET_DEF_ROUTER_ADDRESS, IPV6_E_INV_POINTER, E_NOT_OK);
  /**/

  /* #10 Call internal service IpV6_VGetDefaultRouterAddr(). */

  IPV6_BEGIN_CRITICAL_SECTION_DRL();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  tmpRouterAddrPtr = IpV6_VGetDefaultRouterAddr(IPV6_ADDR_ID_2_CTRL_IDX(ipAddrId), NULL_PTR, routerIsReachablePtr);

  if (tmpRouterAddrPtr != NULL_PTR)
  {
    IPV6_ADDR_COPY(*routerAddressPtr, *tmpRouterAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    retVal = E_OK;
  }
  else
  {
    IPV6_ADDR_COPY(*routerAddressPtr, IpV6_AddrUnspecified); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    retVal = E_NOT_OK;
  }

  IPV6_END_CRITICAL_SECTION_DRL();   /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV6_GetDefaultRouterAddress() */

/**********************************************************************************************************************
 *  IpV6_SetTrafficClass()
 **********************************************************************************************************************/
/*! \brief        Set value of 'Traffic Class' field in IPv6 header for all packets sent over a specific socket.
 *                (override controller specific default)
 *  \param[in]    SocketIdx         TcpIp socket index
 *  \param[in]    TrafficClass      Value of the 'Traffic Class' field.
 *                                  (see [RFC2460 3. IPv6 Header Format, 7. Traffic Classes])
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_SetTrafficClass(
  TcpIp_SockIdxType SocketIdx,
  uint8             TrafficClass)
{
#if (IPV6_ENABLE_TRAFFIC_CLASS_API == STD_ON)
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_SOCK_IDX(SocketIdx), IPV6_API_ID_SET_TRAFFIC_CLASS, IPV6_E_INV_PARAM);
  /**/

  IpV6_SocketData[SocketIdx].IpV6HdrVersionTcFlNbo &= ~(IPV6_HEADER_MASK_TRAFFIC_CLASS_NBO);                                                     /* clear current 'Traffic Class' value */
  IpV6_SocketData[SocketIdx].IpV6HdrVersionTcFlNbo |=  (IPV6_HEADER_MASK_TRAFFIC_CLASS_NBO & IPV6_HTONL((((uint32)TrafficClass) << IPV6_HEADER_TC_OFFS))); /* set new 'Traffic Class' value */
  IpV6_SocketData[SocketIdx].Flags                 |= IPV6_SOCKET_DATA_FLAG_TRAFFIC_CLASS;                                                       /* set use 'Traffic Class' flag */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
#else
  IPV6_UNUSED_PARAMETER(SocketIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV6_UNUSED_PARAMETER(TrafficClass);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  IpV6_DetReportError(IPV6_API_ID_SET_TRAFFIC_CLASS, IPV6_E_NOT_SUPPORTED);
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_SetTrafficClass() */

/**********************************************************************************************************************
 *  IpV6_SetFlowLabel()
 **********************************************************************************************************************/
/*! \brief        Set value of 'Flow Label' field in IPv6 header for all packets sent over a specific socket.
 *                (override controller specific default)
 *  \param[in]    SocketIdx         TcpIp socket index
 *  \param[in]    FlowLabel         Value of the 'Flow Label' field. (the most 12 significant bits will be ignored)
 *                                  (see [RFC2460 3. IPv6 Header Format, 6. Flow Labels])
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_SetFlowLabel(
  TcpIp_SockIdxType SocketIdx,
  uint32            FlowLabel)
{
#if (IPV6_ENABLE_TRAFFIC_CLASS_API == STD_ON)
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_SOCK_IDX(SocketIdx), IPV6_API_ID_SET_FLOW_LABEL, IPV6_E_INV_PARAM);
  /**/

  IpV6_SocketData[SocketIdx].IpV6HdrVersionTcFlNbo &= ~(IPV6_HEADER_MASK_FLOW_LABEL_NBO);                         /* clear current 'Flow Label' value */
  IpV6_SocketData[SocketIdx].IpV6HdrVersionTcFlNbo |=  (IPV6_HEADER_MASK_FLOW_LABEL_NBO & IPV6_HTONL(FlowLabel)); /* set new 'Flow Label' value */
  IpV6_SocketData[SocketIdx].Flags                 |= IPV6_SOCKET_DATA_FLAG_FLOW_LABEL;                           /* set use 'Flow Label' flag */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
#else
  IPV6_UNUSED_PARAMETER(SocketIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV6_UNUSED_PARAMETER(FlowLabel);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  IpV6_DetReportError(IPV6_API_ID_SET_FLOW_LABEL, IPV6_E_NOT_SUPPORTED);
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_SetFlowLabel() */

/**********************************************************************************************************************
 *  IpV6_SetHopLimit()
 **********************************************************************************************************************/
/*! \brief        Set value of 'Hop Limit' field in IPv6 header for all packets sent over a specific socket.
 *                (override controller specific default)
 *  \param[in]    SocketIdx         TcpIp socket index
 *  \param[in]    HopLimit          'Hop Limit' value that shall be set in the IPv6 header of outgoing packets of the
 *                                  socket.
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_SetHopLimit(
  TcpIp_SockIdxType SocketIdx,
  uint8             HopLimit)
{
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_SOCK_IDX(SocketIdx), IPV6_API_ID_SET_HOP_LIMIT, IPV6_E_INV_PARAM);
  /**/

  IpV6_SocketData[SocketIdx].HopLimit  = HopLimit;
  IpV6_SocketData[SocketIdx].Flags    |= IPV6_SOCKET_DATA_FLAG_HOP_LIMIT;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_SetFlowLabel() */

/**********************************************************************************************************************
 *  IpV6_SetEthIfFramePrio()
 **********************************************************************************************************************/
/*! \brief        Set 'EthIf Frame Priority' for all packets sent over a specific socket.
 *                (override controller specific default)
 *  \param[in]    SocketIdx         TcpIp socket index
 *  \param[in]    FramePrio         VLAN frame priority value (0..7)
 *  \retval       E_OK              success
 *  \retval       E_NOT_OK          invalid priority value
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_SetEthIfFramePrio(
  TcpIp_SockIdxType SocketIdx,
  uint8             FramePrio)
{
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_SOCK_IDX(SocketIdx), IPV6_API_ID_SET_ETHIF_FRAME_PRIO, IPV6_E_INV_PARAM, E_NOT_OK);
  /**/

  if (FramePrio > IPV6_MAX_ETHIF_FRAME_PRIO)
  {
    return E_NOT_OK;
  }

  IpV6_SocketData[SocketIdx].EthIfFramePrio = FramePrio;
  IpV6_SocketData[SocketIdx].Flags         |= IPV6_SOCKET_DATA_FLAG_ETHIF_FRAME_PRIO;
  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_SetFlowLabel() */

/**********************************************************************************************************************
 *  IpV6_ResetSocket()
 **********************************************************************************************************************/
/*! \brief        Reset socket specific parameters like 'Hop Limit', 'Traffic Class', 'Flow Label' and
 *                'EthIf Frame Priority' to the controller specific defaults.
 *  \param[in]    SocketIdx         TcpIp socket index
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_ResetSocket(
  TcpIp_SockIdxType SocketIdx)
{
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_SOCK_IDX(SocketIdx), IPV6_API_ID_RESET_SOCKET, IPV6_E_INV_PARAM);
  /**/

#if (IPV6_ENABLE_CACHE_LOOKUP_OPTIMIZATION == STD_ON)
  IpV6_SocketData[SocketIdx].DcIdxHint             = 0;
#endif

  IpV6_SocketData[SocketIdx].EthIfFramePrio        = 0;
  IpV6_SocketData[SocketIdx].HopLimit              = 0;

#if (IPV6_ENABLE_TRAFFIC_CLASS_API == STD_ON)
  IpV6_SocketData[SocketIdx].IpV6HdrVersionTcFlNbo = IPV6_HEADER_VERSION_TC_FL_ZERO_NBO;
#endif

  IpV6_SocketData[SocketIdx].Flags                 = 0;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_SetFlowLabel() */

/**********************************************************************************************************************
 *  IpV6_IsValidDestinationAddress()
 **********************************************************************************************************************/
/*! \brief        Determine whether or not an IPv6 address is a valid destination address.
 *                Before calling IpV6_ProvideTxBuffer() the destination address should be checked for validity using
 *                this function.
 *                The following destination addresses are invalid:
 *                 - Multicast addresses with interface-local scope.
 *                 - The Loopback address [::1]
 *                 - The Unspecified (all-zeros) address [::]
 *                 - Any address assigned to this node
 *  \param[in]    IpCtrlIdx      ip controller index
 *  \param[in]    DstAddrPtr     Destination address to be checked.
 *  \return       TRUE           Address is a valid destination address
 *  \return       FALSE          Address is NOT a valid destination address
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_IsValidDestinationAddress(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr)
{
  IpV6_LocAddrStateType SrcAddrState;
  IpV6_CtrlAddrIdxType  CtrlAddrIdx;

  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DstAddrPtr, IPV6_API_ID_IS_VALID_DST_ADDR, IPV6_E_INV_POINTER, FALSE);
  /**/

  if (IPV6_ADDR_IS_MULTICAST(*DstAddrPtr) && (!IPV6_ADDR_MULTICAST_HAS_VALID_SCOPE(*DstAddrPtr)))
  {
    /* Do not allow sending to multicast addresses with invalid scope */
    return FALSE;
  }

  if (IPV6_ADDR_IS_LOOPBACK(*DstAddrPtr))
  {
    return FALSE;
  }

  if (IPV6_ADDR_IS_UNSPECIFIED(*DstAddrPtr))
  {
    return FALSE;
  }

  IPV6_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (TRUE == IpV6_VIsValidSourceAddress(IpCtrlIdx, DstAddrPtr, TRUE, &SrcAddrState, &CtrlAddrIdx))
  {
    /* Do not allow sending packets to outself! */
    IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    return FALSE;
  }

  IPV6_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return TRUE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_IsValidDestinationAddress() */

/**********************************************************************************************************************
 *  IpV6_GetLocalAddressCfgSrc()
 **********************************************************************************************************************/
/*! \brief        Determine the configuration source of an address assigned to this node.
 *  \param[in]    AddrId         ip address identifier
 *  \param[out]   CfgSrcPtr      Configuration source of the given address
 *  \return       TRUE           Address is a valid destination address
 *  \return       E_OK           Address assigned to this node. Configuration source has been stored in *CfcSrcPtr.
 *  \return       E_NOT_OK       Address is NOT assigned to this node. *CfcSrcPtr not modified.
 *  \context      interrupt and task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_GetLocalAddressCfgSrc(
  IpV6_AddrIdType         AddrId,
  IPV6_P2V(uint8)         CfgSrcPtr)
{
  uint8                 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_CtrlAddrIdxType  CtrlAddrIdx;

  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(AddrId), IPV6_API_ID_GET_SRC_ADDR_CFG_SRC, IPV6_E_INV_ADDR_ID, E_NOT_OK);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != CfgSrcPtr,         IPV6_API_ID_GET_SRC_ADDR_CFG_SRC, IPV6_E_INV_POINTER, E_NOT_OK);
  /**/

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
  IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (IPV6_CTRL_ADDR_IS_UNICAST(CtrlAddrIdx))
  {
    /* Return configuration source of unicast address */
    (*CfgSrcPtr) = IPV6_CTRL_VAR(IpV6_SourceAddressConfig)[CtrlAddrIdx].AddressAssignVariant;
  }
  else
  {
    /* Configuration source off All-Nodes and other multicast addresses is always STATIC */
    (*CfgSrcPtr) = IPV6_ADDR_CFG_SRC_STATIC;
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetLocalAddressCfgSrc() */

/**********************************************************************************************************************
 *  IpV6_GetCurrentHopLimit()
 **********************************************************************************************************************/
/*! \brief      Get current HopLimit value of IP controller.
 *              This may be the configured HopLimit or the dynamically set value by Router Advertisements
 *  \param[in]  IpCtrlIdx     ip controller index
 *  \return     HopLimit value
 **********************************************************************************************************************/
FUNC(uint8, IPV6_CODE) IpV6_GetCurrentHopLimit(uint8 IpCtrlIdx)
{
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_GET_CURRENT_HOP_LIMIT, IPV6_E_INV_CTRL_IDX, 0);
  /**/

  return IPV6_CTRL_VAR(IpV6_CurHopLimit);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetCurrentHopLimit() */

/**********************************************************************************************************************
 *  IpV6_GetPhysAddr()
 **********************************************************************************************************************/
/*! \brief      Get physical address of IP controller
 *  \param[in]  IpCtrlIdx     ip controller index
 *  \param[out] PhysAddrPtr   physical address
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_GetPhysAddr(uint8 IpCtrlIdx, IPV6_P2V(uint8) PhysAddrPtr)
{
  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_GET_PHYS_ADDR, IPV6_E_INV_CTRL_IDX);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != PhysAddrPtr,           IPV6_API_ID_GET_PHYS_ADDR, IPV6_E_INV_POINTER);
  /**/

  TCPIP_LL_GetPhysAddr(IPV6_CTRLIDX_IP_2_ETHIF(IpCtrlIdx), PhysAddrPtr);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetPhysAddr() */

/***********************************************************************************************************************
 *  IpV6_GetPhysAddrByIpAddrId
 **********************************************************************************************************************/
/*! \brief        Obtains the physical source address used by the EthIf controller implicitly specified via IpAddrId.
 *  \param[in]    IpAddrId           Local address identifier implicitely specifing the EthIf controller for which the
 *                                   physical address shall be obtained.
 *  \param[out]   PhysAddrPtr        Pointer to the memory where the physical source address (MAC address) in network
 *                                   byte order is stored
 *  \return       E_OK               The request was successful 
 *                E_NOT_OK           The request was not successful, e.g. no unique Ctrl specified via IpAddrId.
 *  \context      task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) IpV6_GetPhysAddrByIpAddrId(
    IpV6_AddrIdType IpAddrId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) PhysAddrPtr )
{
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  /* Get the IpControllerIndex */
  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, IpAddrId);

  /* Get the phys addr */
  IpV6_GetPhysAddr(IpCtrlIdx, PhysAddrPtr);

  return E_OK;
}

/***********************************************************************************************************************
 *  IpV6_GetRemotePhysAddr
 **********************************************************************************************************************/
/*! \brief        Lookup the physical address for a remote network address. 
 *  \description  TcpIp_GetRemotePhysAddr queries the IP/physical address translation table specified by CtrlIdx and
 *                returns the physical address related to the IP address specified by IpAddrPtr. In case no physical
 *                address can be retrieved and parameter initRes is TRUE, address resolution for the specified IP
 *                address is initiated on the local network.
 *  \param[in]    CtrlIdx             EthIf controller index to identify the related ARP/NDP table.
 *  \param[in]    IpAddrPtr           specifies the IP address for which the physical address shall be retrieved
 *  \param[in]    initRes             specifies if the address resolution shall be initiated (TRUE) or not (FALSE) in
 *                                    case the physical address related to the specified IP address is currently
 *                                    unknown.
 *  \param[out]   PhysAddrPtr         Pointer to the memory where the physical address (MAC address) related to the
 *                                    specified IP address is stored in network byte order.
 *  \return       TCPIP_E_OK               specified IP address resolved, physical address provided via PhysAddrPtr 
 *                TCPIP_E_PHYS_ADDR_MISS   physical address currently unknown (address resolution initiated if initRes set to TRUE)
 *  \context      task level
 **********************************************************************************************************************/
FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV6_GetRemotePhysAddr(
    uint8                                                        CtrlIdx,
    P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR)                      PhysAddrPtr,
    boolean                                                      initRes)
{
  TcpIp_ReturnType        RetVal;
  IPV6_P2C(IpV6_AddrType) DestAddrPtr = (IPV6_P2C(IpV6_AddrType))&(IpAddrPtr->addr[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  uint8                   IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  IpV6_ListIdxType        TempListIdx = 0xFF;  /* set to invalid value */
  boolean                 EntryFound;

  if (E_OK != IPV6_CTRLIDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
  {
    /* no IPv6 instance configured for this EthIf controller */
    return TCPIP_E_NOT_OK;
  }

  if (IPV6_ADDR_IS_MULTICAST(*DestAddrPtr))
  {
    /* map IPv6 multicast address to ethernet multicast address according to RFC 2464 section 7 */
    IpV6_VMulticastAddr2LinkLayerAddr(PhysAddrPtr, DestAddrPtr);
    return TCPIP_OK;
  }

  if (TRUE == initRes)
  {
    /* get or create entry in the Neighbor Cache */
    IpV6_Ndp_VNCGetOrCreateEntry(IpCtrlIdx, DestAddrPtr, &TempListIdx);
    EntryFound = TRUE;
  }
  else
  {
    /* lookup entry in Neighbor Cache */
    if(E_OK == IpV6_Ndp_VNCLookup(IpCtrlIdx, DestAddrPtr, &TempListIdx))
    {
      EntryFound = TRUE;
    }
    else
    {
      EntryFound = FALSE;
    }
  }

  if (EntryFound && (IPV6_REACHABLE_STATE_INCOMPLETE != IPV6_CTRL_VAR(IpV6_NeighborCache)[TempListIdx].State))
  {
    IPV6_LL_ADDR_COPY(PhysAddrPtr, &IPV6_CTRL_VAR(IpV6_NeighborCache)[TempListIdx].LinkLayerAddress[0]); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    RetVal = TCPIP_OK;
  }
  else
  {
    RetVal = TCPIP_E_PHYS_ADDR_MISS;
  }

  return RetVal;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

/**********************************************************************************************************************
 *  IpV6_GetAsBcAddrId()
 **********************************************************************************************************************/
/*! \brief      Get address id of AutoSelect/Broadcast address of controller
 *              (i.e.: AddrId of All-Nodes Link-Local address)
 *  \param[in]  IpCtrlIdx     ip controller index
 *  \return     address id of AutoSelect/Broadcast address
 **********************************************************************************************************************/
FUNC(IpV6_AddrIdType, IPV6_CODE) IpV6_GetAsBcAddrId(uint8 IpCtrlIdx)
{
  /* Det checks */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_CTRL_IDX(IpCtrlIdx), IPV6_API_ID_GET_AS_BC_ADDR_ID, IPV6_E_INV_CTRL_IDX, IPV6_ADDR_ID_INV);
  /**/

  return IPV6_CTRL_VAR(IpV6_BcAddrId);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_GetAsBcAddrId() */

/**********************************************************************************************************************
 *  IpV6_IsAddrIdAcceptable()
 **********************************************************************************************************************/
/*! \brief        This function checks whether the address identifier of a received packet is acceptable for a socket
 *                bound to a specific address identifier.
 *                The check is performed according to the acceptance filter given in [SWS_TCPIP_00106].
 *  \param[in]    BindIpAddrId           ip address identifier to ehich the socket has been bound
 *  \param[in]    RecvIpAddrId           ip address identifier of the received packet
 *  \param[out]   MatchType              specifies the type of the match (e.g. TCPIP_IF_MATCH, TCPIP_EXACT_MATCH)
 *  \return       TRUE                   packet is acceptable
 *  \return       FALSE                  packet is not acceptable
 *  \context      interrupt or task level
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_IsAddrIdAcceptable(IpV6_AddrIdType BindIpAddrId, IpV6_AddrIdType RecvIpAddrId, IPV6_P2V(uint8) MatchType)
{
  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_OR_ANY_ADDR_ID(BindIpAddrId), IPV6_API_ID_MATCH_IP_ADDR_IDS, IPV6_E_INV_ADDR_ID, FALSE);
  IpV6_CheckDetErrorReturnValue(IPV6_IS_VALID_ADDR_ID(RecvIpAddrId),        IPV6_API_ID_MATCH_IP_ADDR_IDS, IPV6_E_INV_ADDR_ID, FALSE);
  /**/

  if (IPV6_ADDR_ID_ANY == BindIpAddrId)
  {
    (*MatchType) = IPV6_ADDR_MATCH_EVER;
    return TRUE;
  }

  if (RecvIpAddrId == BindIpAddrId)
  {
    /* address matches filter address (exact match) */
    (*MatchType) = IPV6_ADDR_MATCH_EXACT;
    return TRUE;
  }

  if ((IPV6_ADDR_ID_2_ADDR_IDX(BindIpAddrId) == IPV6_CTRL_ADDR_IDX_INV_OR_ANY) || (IPV6_ADDR_ID_2_ADDR_IDX(RecvIpAddrId) == IPV6_CTRL_ADDR_IDX_INV_OR_ANY))
  {
    /* bind or received address is an AutoSelect_Multicast address */
    if (IPV6_ADDR_ID_2_CTRL_IDX(BindIpAddrId) == IPV6_ADDR_ID_2_CTRL_IDX(RecvIpAddrId))
    {
      /* both addresses are on same controller If-match */
      (*MatchType) = IPV6_ADDR_MATCH_IF;
      return TRUE;
    }
  }

  (*MatchType) = IPV6_ADDR_MATCH_NONE;
  return FALSE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_IsAddrIdAcceptable() */

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV6.c
 *********************************************************************************************************************/
