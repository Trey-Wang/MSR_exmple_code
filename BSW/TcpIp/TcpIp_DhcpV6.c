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
 *         \file  TcpIp_DhcpV6.c
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv6 - Client (DHCPv6 Client)
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

#define TCPIP_DHCPV6_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON) && (TCPIP_SUPPORT_DHCPV6 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp.h"
#include "TcpIp_Priv.h"
#include "TcpIp_DhcpV6.h"
#include "TcpIp_DhcpV6_Cbk.h"
#include "TcpIp_Lcfg.h"
#include "IpBase.h"
#include "vstdlib.h"

#include "TcpIp_IpV6_Priv.h"

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
# include "Dns.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if defined (STATIC)
#else
# define STATIC static
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
# define TCPIP_DHCPV6_VENABLE_CANOE_WRITE_STRING  STD_OFF
# define TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS STD_ON
#else
# define TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS STD_OFF
#endif

#define TCPIP_DHCPV6_MAX_TRANS_ID 0x00FFFFFFU
#define TCPIP_DHCPV6_INV_TRANS_ID 0xFF000000U

#define TCPIP_DHCPV6_USE_RAND_FUNCTION STD_ON

#define TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN            3U
#define TCPIP_DHCPV6_OPT_REQ_OPT_LIST_EXTRA_OPT_OFS  1U

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/*  */
#define TCPIP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
# if (TCPIP_DHCPV6_VENABLE_CANOE_WRITE_STRING == STD_ON)
STATIC sint8 VCanoeWriteStrBuffer[256];
# endif
#endif

#define TCPIP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*  */
#define TCPIP_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*  */
#define TCPIP_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*  */
#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
STATIC VAR(TcpIp_DhcpV6_IaAddrLeaseType, TCPIP_VAR_NOINIT) TcpIp_DhcpV6_IaAddrLeases[1];
#endif

#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
STATIC VAR(TcpIp_DhcpV6_OptIaAddrType,   TCPIP_VAR_NOINIT) TcpIp_DhcpV6_IaAddrBuf[1];
#endif

STATIC VAR(TcpIp_DhcpV6_TimeType, TCPIP_VAR_NOINIT) TcpIp_DhcpV6_Time;

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*  */
#define TCPIP_START_SEC_CONST_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC CONST(uint16, TCPIP_CONST) TcpIp_DhcpV6_OptionRequestList[TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN] = { TCPIP_DHCPV6_OPT_IA_NA, TCPIP_DHCPV6_OPT_DNS_SERVERS, TCPIP_DHCPV6_OPT_DOMAIN_LIST };

#define TCPIP_STOP_SEC_CONST_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*  */
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* link-local multicast address for all DHCPv6 server and relay agents */
/* PRQA S 0759 5 */ /* MD_IpV6_0759 */
CONST(IpBase_AddrIn6Type, TCPIP_CONST) TcpIp_DhcpV6_Addr_All_DHCP_Relay_Agents_and_Servers =
{
  {
    0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02
  }
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*  */
#define TCPIP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_VGetNextOption(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptLocationType, AUTOMATIC, TCPIP_APPL_VAR) OptPosPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptIaNa(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptIaNaType, AUTOMATIC, TCPIP_APPL_VAR) IaNaPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddRawOpt(
  uint8 IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  uint16 OptType,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptDataPtr,
  uint16 OptDataLen);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptStatusCode(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptStatusCodeType, AUTOMATIC, TCPIP_APPL_VAR) StatusCodePtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VCalcTimeout(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) TimePtr, boolean IsSolicit);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptIaAddr(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  P2VAR(TcpIp_DhcpV6_OptIaAddrType, AUTOMATIC, TCPIP_APPL_VAR) IaAddrPtr,
  uint8 IaAddrCount);
#else
  P2VAR(TcpIp_DhcpV6_OptIaAddrType, AUTOMATIC, TCPIP_APPL_VAR) IaAddrPtr);
#endif

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VRxReply(
  uint8                                      IpCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VRestartAddressAssignment(uint8 IpCtrlIdx, uint32 DelayMs);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptClientIdent(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptServerIdent(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptElapsedTime(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

#if (TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON)

# if (TCPIP_DHCPV6_TX_OPTION_CNT > 0)
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddUserOptions(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);
# endif

# if (TCPIP_DHCPV6_RX_OPTION_CNT > 0)
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VHandleUserOptions(
  uint8                                      MsgType,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen);
# endif

#endif

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_VCompareBytes(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) Data1Ptr,
  uint16 Data1Len,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) Data2Ptr,
  uint16 Data2Len);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VInitClientId(uint8 IpCtrlIdx);

#if (TCPIP_DHCPV6_USE_RAND_FUNCTION == STD_ON)
STATIC FUNC(sint32, TCPIP_CODE) TcpIp_DhcpV6_VGetRandomValue(sint32 MinValue, sint32 MaxValue);
#endif

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VTimeAddMs(
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_DATA) TimestampPtr,
  uint32 Milliseconds);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VSetLifetimeS(
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) TimestampPtr,
  uint32 LifetimeS,
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) NextEventTimePtr);

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VChangeState(uint8 NewState);
#endif

STATIC INLINE FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VWriteOptionCode(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) DataPtr,
  uint16                                  OptCode);

STATIC INLINE FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VWriteOptionLen(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) DataPtr,
  uint16                                  OptLen);

STATIC INLINE FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_VReadOptionCode(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr);

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_VCompareBytes(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) Data1Ptr,
  uint16 Data1Len,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) Data2Ptr,
  uint16 Data2Len);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VInitClientId(uint8 IpCtrlIdx);

#if (TCPIP_DHCPV6_USE_RAND_FUNCTION == STD_ON)
STATIC FUNC(sint32, TCPIP_CODE) TcpIp_DhcpV6_VGetRandomValue(sint32 MinValue, sint32 MaxValue);
#endif

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VTimeAddMs(
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_DATA) TimestampPtr,
  uint32 Milliseconds);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VSetLifetimeS(
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) TimestampPtr,
  uint32 LifetimeS,
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) NextEventTimePtr);

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VChangeState(uint8 NewState);
#endif

STATIC INLINE FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VWriteOptionCode(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) DataPtr,
  uint16                                  OptCode);

STATIC INLINE FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VWriteOptionLen(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) DataPtr,
  uint16                                  OptLen);

STATIC INLINE FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_VReadOptionCode(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VSetRetransParams(uint8 IpCtrlIdx, uint32 IRT, uint32 MRT, uint8 MRC, uint32 MRD);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VCalcTimeout(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) TimePtr, boolean IsSolicit);

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
STATIC FUNC(P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA), TCPIP_CODE) TcpIp_DhcpV6_VGetStatusMessage(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDbgPrintOptId(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen);
#endif

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VOptStackPushDataPos(P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VOptStackPopDataPos(
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_VAR) DataPosPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBeginMessage(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  uint8                                                  MsgType);

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_VEndMessage(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_DATA) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddRawOpt(
  uint8 IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  uint16 OptType,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptDataPtr,
  uint16 OptDataLen);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptClientIdent(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptServerIdent(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBeginOptIaNa(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr, uint32 IaId, uint32 T1, uint32 T2);
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VEndOptIaNa(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBeginOptIaAddr(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr, P2CONST(IpBase_AddrIn6Type, AUTOMATIC, TCPIP_APPL_DATA) AddrPtr, uint32 PreferredLifetime, uint32 ValidLifetime);
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VEndOptIaAddr(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptReqOpt(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  P2CONST(uint16, AUTOMATIC, TCPIP_APPL_DATA)            OptCodeArray,
  uint8                                                  OptCodeCount);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptElapsedTime(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildSolicit(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildInfoRequest(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRelease(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRenew(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRebind(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildDecline(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildConfirm(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRequest(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VTriggerTx(P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VTxMessage(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr);

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_VGetNextOption(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptLocationType, AUTOMATIC, TCPIP_APPL_VAR) OptPosPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptIaAddr(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  P2VAR(TcpIp_DhcpV6_OptIaAddrType, AUTOMATIC, TCPIP_APPL_VAR) IaAddrPtr,
  uint8 IaAddrCount);
#else
  P2VAR(TcpIp_DhcpV6_OptIaAddrType, AUTOMATIC, TCPIP_APPL_VAR) IaAddrPtr);
#endif

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptIaNa(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptIaNaType, AUTOMATIC, TCPIP_APPL_VAR) IaNaPtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptStatusCode(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptStatusCodeType, AUTOMATIC, TCPIP_APPL_VAR) StatusCodePtr);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptPreference(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) PreferenceValuePtr);

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptDnsServers(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptDnsServersType, AUTOMATIC, TCPIP_APPL_VAR) DnsServersOptPtr);
#endif

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptDomainList(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptDomainListType, AUTOMATIC, TCPIP_APPL_VAR) DomainListOptPtr);
#endif

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_RX_OPTION_CNT > 0))
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VHandleUserOptions(
  uint8                                      MsgType,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen);
#endif

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VRxAdvertise(
  uint8                                      IpCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VRxReply(
  uint8                                      IpCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen);

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VRestartAddressAssignment(uint8 IpCtrlIdx, uint32 DelayMs);

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
# if (defined(TCPIP_PROCESSOR_CANOEEMU) && (TCPIP_DHCPV6_VENABLE_CANOE_WRITE_STRING == STD_ON))
#  include "stdio.h"

#  define CANOE_DBG_MODULE_NAME "DHCPv6"

#  define CANOE_WRITE_STRING0(Txt) \
  CANoeAPI_WriteString("[" CANOE_DBG_MODULE_NAME "] " Txt);

/* PRQA S 1030,5013 3 */ /* MD_TcpIp_CANoeEmu */
#  define CANOE_WRITE_STRINGF(Txt, ...) \
   _snprintf((char*)VCanoeWriteStrBuffer, sizeof(VCanoeWriteStrBuffer), "[" CANOE_DBG_MODULE_NAME "] " Txt, __VA_ARGS__); \
  CANoeAPI_WriteString((const char *)(VCanoeWriteStrBuffer));

#  define CANOE_WRITE_STRING1(txt, p1)                     CANOE_WRITE_STRINGF(txt, p1)
#  define CANOE_WRITE_STRING2(txt, p1, p2)                 CANOE_WRITE_STRINGF(txt, p1, p2)
#  define CANOE_WRITE_STRING3(txt, p1, p2, p3)             CANOE_WRITE_STRINGF(txt, p1, p2, p3)
#  define CANOE_WRITE_STRING4(txt, p1, p2, p3, p4)         CANOE_WRITE_STRINGF(txt, p1, p2, p3, p4)
#  define CANOE_WRITE_STRING5(txt, p1, p2, p3, p4, p5)     CANOE_WRITE_STRINGF(txt, p1, p2, p3, p4, p5)
#  define CANOE_WRITE_STRING6(txt, p1, p2, p3, p4, p5, p6) CANOE_WRITE_STRINGF(txt, p1, p2, p3, p4, p5, p6)
# else
#  define CANOE_WRITE_STRING0(txt)
#  define CANOE_WRITE_STRING1(txt, p1)
#  define CANOE_WRITE_STRING2(txt, p1, p2)
#  define CANOE_WRITE_STRING3(txt, p1, p2, p3)
#  define CANOE_WRITE_STRING4(txt, p1, p2, p3, p4)
#  define CANOE_WRITE_STRING5(txt, p1, p2, p3, p4, p5)
#  define CANOE_WRITE_STRING6(txt, p1, p2, p3, p4, p5, p6)
# endif /* (defined(TCPIP_PROCESSOR_CANOEEMU) && (TCPIP_DHCPV6_VENABLE_CANOE_WRITE_STRING == STD_ON)) */
#endif

#define TCPIP_DHCPV6_GET_MSG_TYPE(MsgPtr) ((MsgPtr)->BufPtr[0])

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
#define TCPIP_DHCPV6_CHANGE_STATE(NEW_STATE) TcpIp_DhcpV6_VChangeState(IpCtrlIdx, TCPIP_DHCPV6_STATE_##NEW_STATE)
#else
/* PRQA S 0342 1 */ /* MD_IpV6_0342 */
#define TCPIP_DHCPV6_CHANGE_STATE(NEW_STATE) do { TCPIP_DHCPV6_STATUS_VAR(LastState) = TCPIP_DHCPV6_STATUS_VAR(State); TCPIP_DHCPV6_STATUS_VAR(State) = (TCPIP_DHCPV6_STATE_##NEW_STATE); } while (0)
#endif

/* PRQA S 0342, 0881 1 */ /* MD_IpV6_0342, MD_IpV6_0881 */
#define TCPIP_DHCPV6_INIT_RETRANS_PARAMS(MSG) (TcpIp_DhcpV6_VSetRetransParams(IpCtrlIdx, TCPIP_DHCPV6_##MSG##_TIMEOUT, TCPIP_DHCPV6_##MSG##_MAX_RT, TCPIP_DHCPV6_##MSG##_MAX_RC, TCPIP_DHCPV6_##MSG##_MAX_RD))

#define TCPIP_DHCPV6_GENERATE_TRANS_ID(VAR)   ((VAR) = ((((VAR) + (uint32)TcpIp_GetRandNoFct() + 1) & 0x00FFFFFF) + ((uint32)IpCtrlIdx << 16)))

#define TCPIP_DHCPV6_RAND_FACTOR(BASE_VALUE)  (((BASE_VALUE) / 10) - (2 * TCPIP_MAIN_FCT_PERIOD_MSEC))

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VCompareBytes()
 **********************************************************************************************************************/
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_VCompareBytes(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) Data1Ptr,
  uint16 Data1Len,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) Data2Ptr,
  uint16 Data2Len)
{
  uint16 IdIdx;

  if (Data1Len != Data2Len)
  {
    return FALSE;
  }

  IdIdx = Data1Len;

  while (0 < IdIdx)
  {
    IdIdx--;

    if (Data1Ptr[IdIdx] != Data2Ptr[IdIdx])
    {
      return FALSE;
    }
  }

  return TRUE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VCompareBytes() */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VInitClientId()
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VInitClientId(uint8 IpCtrlIdx)
{
  TCPIP_DHCPV6_STATUS_VAR(ClientDuid).TypeNbo   = IPV6_UINT8_HTONS(0x01U);
  TCPIP_DHCPV6_STATUS_VAR(ClientDuid).HwTypeNbo = IPV6_UINT8_HTONS(0x01U);
  TCPIP_DHCPV6_STATUS_VAR(ClientDuid).TimeNbo   = IPV6_HTONL(TcpIp_DhcpV6_VCfgGetDuidTime());
  /* get physical address for client DUID */

  IpV6_GetPhysAddr(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(ClientDuid).PhysAddr[0]);

} /* End of TcpIp_DhcpV6_VInitClientId() */

#if (TCPIP_DHCPV6_USE_RAND_FUNCTION == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VGetRandomValue()
 **********************************************************************************************************************/
/*! \brief      Get a random value between [MinValue...MaxValue]
 *  \param[in]  MinValue   minimum value
 *  \param[in]  MaxValue   maximum value
 *  \return     Random value between [MinValue...MaxValue]
 **********************************************************************************************************************/
STATIC FUNC(sint32, TCPIP_CODE) TcpIp_DhcpV6_VGetRandomValue(sint32 MinValue, sint32 MaxValue)
{
  uint32 ValueRange;
  uint32 RndValue;

  if (MinValue < MaxValue)
  {
    ValueRange = ((uint32)(MaxValue - MinValue)) + 1;

    if (ValueRange <= 0xFFFFU)
    {
      /* 16bit random number [0..0xFFFF] is sufficient */
      RndValue = (uint32)TcpIp_VIpGetRandNoFct();
    }
    else
    {
      /* create 32bit random number [0..0xFFFFFFFF] by combining two 16bit random numbers */
      RndValue = (uint32)(((uint32)TcpIp_VIpGetRandNoFct() << 16) | (uint32)TcpIp_VIpGetRandNoFct());
    }

    RndValue %= ValueRange;
  }
  else
  {
    RndValue = 0;
  }

  return MinValue + (sint32)RndValue;

} /* End of TcpIp_DhcpV6_VGetRandomValue() */
#endif

/* TIMESTAMP FUNCTIONS */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VTimeAddMs()
 **********************************************************************************************************************/
/*! \brief      Add milliseconds to timestamp
 *  \param[in]  TimestampPtr  pointer to the timestamp variable
 *  \param[in]  Milliseconds  number of milliseconds to add
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VTimeAddMs(
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_DATA) TimestampPtr,
  uint32 Milliseconds)
{
  TimestampPtr->Ms += Milliseconds;

  if (TimestampPtr->Ms >= 1000)
  {
    TimestampPtr->S += (TimestampPtr->Ms / 1000);
    TimestampPtr->Ms = (TimestampPtr->Ms % 1000);
  }
} /* End of TcpIp_DhcpV6_VTimeAddMs() */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VSetLifetimeS()
 **********************************************************************************************************************/
/*! \brief      Set timestamp to a lifetime value
 *  \param[in]  TimestampPtr     pointer to the timestamp variable
 *  \param[in]  LifetimeS        lifetime value in seconds
 *  \param[in]  NextEventTimePtr pointer to a variable in which the timestamp value will be stored if it is less then
 *                               the current stored value
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VSetLifetimeS(
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) TimestampPtr,
  uint32 LifetimeS,
  P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) NextEventTimePtr)
{
  if (TCPIP_DHCPV6_LIFETIME_UNLIMITED > LifetimeS)
  {
    TimestampPtr->S  = TcpIp_DhcpV6_Time.S + LifetimeS;
    TimestampPtr->Ms = TcpIp_DhcpV6_Time.Ms;
  }
  else
  {
    TimestampPtr->S  = TCPIP_DHCPV6_LIFETIME_UNLIMITED;
    TimestampPtr->Ms = 0;
  }

  if ((NULL_PTR != NextEventTimePtr) && (TCPIP_DHCPV6_TIME_IS_LOWER(*TimestampPtr, *NextEventTimePtr)))
  {
    *NextEventTimePtr = *TimestampPtr;
  }

} /* End of TcpIp_DhcpV6_VSetLifetimeS() */

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VChangeState(uint8 NewState)
{
  TcpIp_DhcpV6_LastState = TcpIp_DhcpV6_State;
  TcpIp_DhcpV6_State = NewState;

  CANOE_WRITE_STRING2("Changed state from 0x%02X to 0x%02X", TcpIp_DhcpV6_LastState, TcpIp_DhcpV6_State);

} /* End of TcpIp_DhcpV6_VChangeState() */
#endif

/* MISC FUNCTIONS */

STATIC INLINE FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VWriteOptionCode(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) DataPtr,
  uint16                                  OptCode)
{
  DataPtr[0] = (uint8)(OptCode >> 8);
  DataPtr[1] = (uint8)(OptCode);
}

STATIC INLINE FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VWriteOptionLen(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) DataPtr,
  uint16                                  OptLen)
{
  DataPtr[2] = (uint8)(OptLen >> 8);
  DataPtr[3] = (uint8)(OptLen);
}

STATIC INLINE FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_VReadOptionCode(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr)
{
  return (uint16)((uint16)DataPtr[0] << 8) | (DataPtr[1]);
}

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VSetRetransParams()
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VSetRetransParams(uint8 IpCtrlIdx, uint32 IRT, uint32 MRT, uint8 MRC, uint32 MRD)
{
  TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount = 0;

  TCPIP_DHCPV6_STATUS_VAR(RetransParams).InitRetransTimeout = IRT;
  TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransTime     = MRT;
  TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransCount    = MRC;

  if (MRD > 0)
  {
    TCPIP_DHCPV6_SET_EVENT_TIME_MS(TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout, MRD);
  }
  else
  {
    TCPIP_DHCPV6_SET_LIFETIME_S(TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout, TCPIP_DHCPV6_LIFETIME_UNLIMITED, (TcpIp_DhcpV6_TimeType*)NULL_PTR);
  }

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING4("Retrans Params: IRT: %dmsec, MRT: %dmsec, MRC: %d, MRD: %dmsec", IRT, MRT, MRC, MRD);
#endif
} /* End of TcpIp_DhcpV6_VSetRetransParams() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VCalcTimeout()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VCalcTimeout(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_TimeType, AUTOMATIC, TCPIP_APPL_VAR) TimePtr, boolean IsSolicit)
{
  uint32 RndOffset = 0;

  if (TCPIP_DHCPV6_TIME_EXPIRED(TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout))
  {
    return E_NOT_OK;
  }

  /* RT = [-(IRT/10)...(IRT/10)] */

  if ((TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransCount > 0) && (TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount > TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransCount))
  {
    /* maximum number of retransmission reached */
    return E_NOT_OK;
  }

  if (0 == TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount)
  {
    /* calculate first retransmission based on Initial Retransmission Timeout (IRT) */
    TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout = TCPIP_DHCPV6_STATUS_VAR(RetransParams).InitRetransTimeout;

    if (TcpIp_DhcpV6_VCfgIsRandomizeTimeoutsEnabled())
    {
      RndOffset = TCPIP_DHCPV6_RAND_FACTOR(TCPIP_DHCPV6_STATUS_VAR(RetransParams).InitRetransTimeout);

      if (TRUE == IsSolicit)
      {
        TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout += (uint32)TcpIp_DhcpV6_VGetRandomValue(0, (sint32)RndOffset);
      }
      else
      {
        sint32 RndValue = (sint32)TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout;

        RndValue += TcpIp_DhcpV6_VGetRandomValue(-(sint32)RndOffset, (sint32)RndOffset);

        TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout = (RndValue > 0) ? ((uint32)RndValue) : 0;
      }
    }
  }
  else
  {
    /* calculate next retransmission based on previous retransmission timeout */

    if (TcpIp_DhcpV6_VCfgIsRandomizeTimeoutsEnabled())
    {
      RndOffset = TCPIP_DHCPV6_RAND_FACTOR(TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout);
    }

    TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout <<= TCPIP_DHCPV6_LSHIFT_MUL_2;

    if (TcpIp_DhcpV6_VCfgIsRandomizeTimeoutsEnabled())
    {
      sint32 RndValue = (sint32)TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout;

      RndValue += TcpIp_DhcpV6_VGetRandomValue(-(sint32)RndOffset, (sint32)RndOffset);

      TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout = (RndValue > 0) ? ((uint32)RndValue) : 0;
    }
  }

  if ((0 < TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransTime) && (TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout > TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransTime))
  {
    /* limit Maximum Retransmission Time */

    if (TcpIp_DhcpV6_VCfgIsRandomizeTimeoutsEnabled())
    {
      RndOffset = TCPIP_DHCPV6_RAND_FACTOR(TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransTime);
    }

    TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout = TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransTime;

    if (TcpIp_DhcpV6_VCfgIsRandomizeTimeoutsEnabled())
    {
      sint32 RndValue = (sint32)TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout;

      RndValue += TcpIp_DhcpV6_VGetRandomValue(-(sint32)RndOffset, (sint32)RndOffset);

      TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout = (RndValue > 0) ? ((uint32)RndValue) : 0;
    }
  }

  *TimePtr = TcpIp_DhcpV6_Time;
  TcpIp_DhcpV6_VTimeAddMs(TimePtr, TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout);

  if (TCPIP_DHCPV6_TIME_IS_LOWER(TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout, *TimePtr))
  {
    /* maximum retransmission duration reached */
    *TimePtr = TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout;
  }

  TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount++;

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  {
    uint32 AbsTimeout = 0;
    if (TCPIP_DHCPV6_LIFETIME_UNLIMITED > TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout.S)
    {

      AbsTimeout = TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout.S - TcpIp_DhcpV6_Time.S;
    }

    CANOE_WRITE_STRING5("Calculated retransmission timeout: %d/%d ms, transmission: %d/%d, abs timeout: %d", TCPIP_DHCPV6_STATUS_VAR(RetransParams).Timeout, TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransTime, TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount, TCPIP_DHCPV6_STATUS_VAR(RetransParams).MaxRetransCount, AbsTimeout);
  }
#endif

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VCalcTimeout() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VGetStatusMessage()
 **********************************************************************************************************************/
STATIC FUNC(P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA), TCPIP_CODE) TcpIp_DhcpV6_VGetStatusMessage(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen)
{
  if (DataLen < sizeof(TcpIp_DhcpV6_StatusMsgBuffer))
  {
    TcpIp_DhcpV6_StatusMsgBufferLen = (uint8)(DataLen);
  }
  else
  {
    TcpIp_DhcpV6_StatusMsgBufferLen = (uint8)(sizeof(TcpIp_DhcpV6_StatusMsgBuffer) - 1);
  }

  VStdMemCpy(TcpIp_DhcpV6_StatusMsgBuffer, DataPtr, TcpIp_DhcpV6_StatusMsgBufferLen);

  TcpIp_DhcpV6_StatusMsgBuffer[TcpIp_DhcpV6_StatusMsgBufferLen] = 0;

  return TcpIp_DhcpV6_StatusMsgBuffer;
} /* End of TcpIp_DhcpV6_VGetStatusMessage() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDbgPrintOptId()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDbgPrintOptId(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen)
{
  uint16 DuIdType;
  uint16 DataPos = 0;
  P2CONST(TcpIp_DhcpV6_DuIdType1, AUTOMATIC, TCPIP_APPL_DATA) DuIdPtr;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  DuIdType = (DataPtr[DataPos] << 8) | (DataPtr[DataPos+1]);

  if (TCPIP_DHCPV6_DUID_TYPE_1 != DuIdType)
  {
    CANOE_WRITE_STRING1("Unknown DUID type %d", DuIdType);
    return E_NOT_OK;
  }

  DuIdPtr = (P2CONST(TcpIp_DhcpV6_DuIdType1, AUTOMATIC, TCPIP_APPL_DATA))&DataPtr[DataPos];

  CANOE_WRITE_STRING2("DUID Time: %d, HWAddr: %s", IPBASE_HTON32(DuIdPtr->TimeNbo), IpV6_VLLAddr2String(&DuIdPtr->PhysAddr));

  return E_OK;
} /* End of TcpIp_DhcpV6_VDbgPrintOptId() */
#endif

/* MESSAGE BUILDING FUNCTIONS */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VOptStackPushDataPos()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VOptStackPushDataPos(P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;
#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,     TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* push current option onto stack */
  if (MsgPtr->OptStackPos < TCPIP_DHCPV6_OPT_STACK_SIZE)
  {
    MsgPtr->OptStartPos[MsgPtr->OptStackPos] = MsgPtr->MsgBufPos;
    MsgPtr->OptStackPos++;
    Result = E_OK;
  }
  else
  {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
    CANOE_WRITE_STRING0("TcpIp_DhcpV6_VOptStackPushDataPos() Option Stack overflow!");
#endif
    Result = E_NOT_OK;
  }

  return Result;
} /* End of TcpIp_DhcpV6_VOptStackPushDataPos() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VOptStackPopDataPos()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VOptStackPopDataPos(
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_VAR) DataPosPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,     TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (0 < MsgPtr->OptStackPos)
  {
    MsgPtr->OptStackPos--;
    *DataPosPtr = MsgPtr->OptStartPos[MsgPtr->OptStackPos];
    Result = E_OK;
  }
  else
  {
    Result = E_NOT_OK;
  }

  return Result;
} /* End of TcpIp_DhcpV6_VOptStackPopDataPos() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBeginMessage()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBeginMessage(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  uint8                                                  MsgType)
{
#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,                         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr->MsgBufPtr,                 TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_MSG_HDR_LEN <= MsgPtr->MsgBufLen, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  MsgPtr->MsgBufPos = 0;
  MsgPtr->TxPending = FALSE;

  /* write message type code */
  MsgPtr->MsgBufPtr[0] = MsgType;

  MsgPtr->OptStackPos = 0;

  TCPIP_DHCPV6_GENERATE_TRANS_ID(TCPIP_DHCPV6_STATUS_VAR(CurrentTransId));

  /* write 24bit transaction id */
  MsgPtr->MsgBufPtr[1] = (uint8)(TCPIP_DHCPV6_STATUS_VAR(CurrentTransId) >> 16);
  MsgPtr->MsgBufPtr[2] = (uint8)(TCPIP_DHCPV6_STATUS_VAR(CurrentTransId) >> 8);
  MsgPtr->MsgBufPtr[3] = (uint8)(TCPIP_DHCPV6_STATUS_VAR(CurrentTransId));

  TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs) = 0;

  MsgPtr->MsgBufPos = 4;

  (void)TcpIp_DhcpV6_VAddOptClientIdent(IpCtrlIdx, MsgPtr); /* ignore return value, option should always fit into dhcp message */

  return E_OK;
} /* End of TcpIp_DhcpV6_VBeginMessage() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VEndMessage()
 **********************************************************************************************************************/
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV6_VEndMessage(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_DATA) MsgPtr)
{
  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_TX_OPTION_CNT > 0))
  (void)TcpIp_DhcpV6_VAddUserOptions(IpCtrlIdx, MsgPtr);
#endif

  return MsgPtr->MsgBufPos;
} /* End of TcpIp_DhcpV6_VEndMessage() */

/* OPTION ENCODING FUNCTIONS */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VAddRawOpt()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddRawOpt(
  uint8 IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  uint16 OptType,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptDataPtr,
  uint16 OptDataLen)
{
  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,     TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != OptDataPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < OptDataLen,         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  TcpIp_DhcpV6_VWriteOptionCode(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], OptType);
  if(OptType == TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN)
  {
    TcpIp_DhcpV6_VWriteOptionLen(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], (OptDataLen +1));
  }
  else
  {
    TcpIp_DhcpV6_VWriteOptionLen(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], OptDataLen);
  }

  MsgPtr->MsgBufPos += TCPIP_DHCPV6_OPT_HDR_LEN;

  if(OptType == TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN)
  {
    /* IPv6
        0 1 2 3 4 5 6 7
       +-+-+-+-+-+-+-+-+
       |  MBZ    |N|O|S|
       +-+-+-+-+-+-+-+-+
       S = 0   the server SHOULD NOT perform the A RR (FQDN-to-address) DNS updates
       O = 0   must be 0
       N = 1   the server SHOULD NOT perform any DNS updates
    */
    MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos] = 0x04;  /* set flag N */
    MsgPtr->MsgBufPos++;
  }

  VStdMemCpy(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], OptDataPtr, OptDataLen);

  MsgPtr->MsgBufPos += OptDataLen;

  return E_OK;
} /* End of TcpIp_DhcpV6_VAddRawOpt() */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VAddOptClientIdent()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptClientIdent(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  return TcpIp_DhcpV6_VAddRawOpt(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_OPT_CLIENTID, (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))&TCPIP_DHCPV6_STATUS_VAR(ClientDuid), TCPIP_DHCPV6_CLIENT_DUID_LEN); /* PRQA S 0310 */ /* MD_IpV6_0310 */
} /* End of TcpIp_DhcpV6_VAddOptClientIdent() */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6_VAddOptServerIdent()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptServerIdent(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  return TcpIp_DhcpV6_VAddRawOpt(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_OPT_SERVERID, (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))&TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerId[0], TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerIdLen); /* PRQA S 0310 */ /* MD_IpV6_0310 */
} /* End of TcpIp_DhcpV6_VAddOptServerIdent() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBeginOptIaNa()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBeginOptIaNa(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr, uint32 IaId, uint32 T1, uint32 T2)
{
  P2VAR(TcpIp_DhcpV6_OptIaNaHdrRawType, AUTOMATIC, TCPIP_APPL_VAR) OptIaNaHdrPtr;

  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,     TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (E_OK != TcpIp_DhcpV6_VOptStackPushDataPos(MsgPtr))
  {
    return E_NOT_OK;
  }

  TcpIp_DhcpV6_VWriteOptionCode(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], TCPIP_DHCPV6_OPT_IA_NA);
  /* option length currently unknown, since it depends on the options contined in this option */

  MsgPtr->MsgBufPos += TCPIP_DHCPV6_OPT_HDR_LEN;

  OptIaNaHdrPtr = (P2VAR(TcpIp_DhcpV6_OptIaNaHdrRawType, AUTOMATIC, TCPIP_APPL_VAR))&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  OptIaNaHdrPtr->IaIdNbo = IPBASE_HTON32(IaId);
  OptIaNaHdrPtr->T1Nbo   = IPBASE_HTON32(T1);
  OptIaNaHdrPtr->T2Nbo   = IPBASE_HTON32(T2);

  MsgPtr->MsgBufPos += sizeof(TcpIp_DhcpV6_OptIaNaHdrRawType);

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VBeginOptIaNa() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VEndOptIaNa()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VEndOptIaNa(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  uint16 TmpOptStartPos;

  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (E_OK != TcpIp_DhcpV6_VOptStackPopDataPos(MsgPtr, &TmpOptStartPos))
  {
    return E_NOT_OK;
  }

  if (TCPIP_DHCPV6_OPT_IA_NA != TcpIp_DhcpV6_VReadOptionCode(&MsgPtr->MsgBufPtr[TmpOptStartPos]))
  {
    return E_NOT_OK;
  }

  TcpIp_DhcpV6_VWriteOptionLen(&MsgPtr->MsgBufPtr[TmpOptStartPos], (uint16)(MsgPtr->MsgBufPos - TmpOptStartPos - TCPIP_DHCPV6_OPT_HDR_LEN));

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VEndOptIaNa() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBeginOptIaAddr()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBeginOptIaAddr(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr, P2CONST(IpBase_AddrIn6Type, AUTOMATIC, TCPIP_APPL_DATA) AddrPtr, uint32 PreferredLifetime, uint32 ValidLifetime)
{
  P2VAR(TcpIp_DhcpV6_OptIaAddrHdrRawType, AUTOMATIC, TCPIP_APPL_VAR) OptIaAddrHdrPtr;

  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,  TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* push current option onto stack */
  if (E_OK != TcpIp_DhcpV6_VOptStackPushDataPos(MsgPtr))
  {
    return E_NOT_OK;
  }

  TcpIp_DhcpV6_VWriteOptionCode(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], TCPIP_DHCPV6_OPT_IAADDR);
  /* option length currently unknown, since it depends on the options contined in this option */

  MsgPtr->MsgBufPos += TCPIP_DHCPV6_OPT_HDR_LEN;

  OptIaAddrHdrPtr = (P2VAR(TcpIp_DhcpV6_OptIaAddrHdrRawType, AUTOMATIC, TCPIP_APPL_VAR))&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  IPV6_ADDR_COPY(OptIaAddrHdrPtr->Addr, *AddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  OptIaAddrHdrPtr->PreferredLifetimeNbo = IPBASE_HTON32(PreferredLifetime);
  OptIaAddrHdrPtr->ValidLifetimeNbo     = IPBASE_HTON32(ValidLifetime);

  MsgPtr->MsgBufPos += sizeof(TcpIp_DhcpV6_OptIaAddrHdrRawType);

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VBeginOptIaAddr() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VEndOptIaAddr()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VEndOptIaAddr(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  uint16 TmpOptStartPos;

  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,  TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (E_OK != TcpIp_DhcpV6_VOptStackPopDataPos(MsgPtr, &TmpOptStartPos))
  {
    return E_NOT_OK;
  }

  if (TcpIp_DhcpV6_VReadOptionCode(&MsgPtr->MsgBufPtr[TmpOptStartPos]) != TCPIP_DHCPV6_OPT_IAADDR)
  {
    return E_NOT_OK;
  }

  TcpIp_DhcpV6_VWriteOptionLen(&MsgPtr->MsgBufPtr[TmpOptStartPos], (uint16)(MsgPtr->MsgBufPos - TmpOptStartPos - TCPIP_DHCPV6_OPT_HDR_LEN));

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VEndOptIaAddr() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VAddOptReqOpt()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptReqOpt(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr,
  P2CONST(uint16, AUTOMATIC, TCPIP_APPL_DATA)            OptCodeArray,
  uint8                                                  OptCodeCount)
{
  uint8 OptCodeIdx;

  IPV6_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,                                  TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(((0 == OptCodeCount) || (NULL_PTR != OptCodeArray)), TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  TcpIp_DhcpV6_VWriteOptionCode(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], TCPIP_DHCPV6_OPT_ORO);

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_REQ_OPTION_CNT > 0))
  TcpIp_DhcpV6_VWriteOptionLen(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], ((OptCodeCount + TCPIP_DHCPV6_REQ_OPTION_CNT) << TCPIP_DHCPV6_LSHIFT_MUL_2));
#else
  TcpIp_DhcpV6_VWriteOptionLen(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], (uint16)(OptCodeCount << TCPIP_DHCPV6_LSHIFT_MUL_2));
#endif

  MsgPtr->MsgBufPos += TCPIP_DHCPV6_OPT_HDR_LEN;

  for (OptCodeIdx = 0; OptCodeIdx < OptCodeCount; OptCodeIdx++)
  {
    MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos]   = (uint8)(OptCodeArray[OptCodeIdx] >> 8);
    MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos+1] = (uint8)(OptCodeArray[OptCodeIdx]);
    MsgPtr->MsgBufPos += 2;
  }

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_REQ_OPTION_CNT > 0))
  {
    uint8 RxOptIdx;
    for (RxOptIdx = 0; RxOptIdx < TCPIP_DHCPV6_REQ_OPTION_CNT; RxOptIdx++)
    {
      if ((TRUE == TcpIp_DhcpV6_ReqOptConstData[RxOptIdx].SendOnAllCtrls) || (MsgPtr->CtrlIdx == TcpIp_DhcpV6_ReqOptConstData[RxOptIdx].CtrlIdx))
      {
        MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos]   = (uint8)(TcpIp_DhcpV6_ReqOptConstData[RxOptIdx].OptId >> 8);
        MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos+1] = (uint8)(TcpIp_DhcpV6_ReqOptConstData[RxOptIdx].OptId);
        MsgPtr->MsgBufPos += 2;
      }
    }
  }
#endif

  return E_OK;
} /* End of TcpIp_DhcpV6_VAddOptReqOpt() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VAddOptElapsedTime()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddOptElapsedTime(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  P2VAR(TcpIp_DhcpV6_OptElapsedTimeRawType, AUTOMATIC, TCPIP_APPL_VAR) OptElTimeHdrPtr;
  uint32 ElapsedTime;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  TcpIp_DhcpV6_VWriteOptionCode(&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], TCPIP_DHCPV6_OPT_ELAPSED_TIME);
  TcpIp_DhcpV6_VWriteOptionLen( &MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos], TCPIP_DHCPV6_OPT_ELAPSED_TIME_LEN);

  MsgPtr->MsgBufPos += TCPIP_DHCPV6_OPT_HDR_LEN;

  OptElTimeHdrPtr = (P2VAR(TcpIp_DhcpV6_OptElapsedTimeRawType, AUTOMATIC, TCPIP_APPL_VAR))&MsgPtr->MsgBufPtr[MsgPtr->MsgBufPos]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  ElapsedTime = (TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs) / 10);

  if (ElapsedTime > 0x0000FFFF)
  {
    ElapsedTime = 0xFFFF;
  }

  OptElTimeHdrPtr->ElapsedTimeNbo = IPV6_HTONS((uint16)ElapsedTime);

  MsgPtr->MsgBufPos += sizeof(TcpIp_DhcpV6_OptElapsedTimeRawType);

  return E_OK;
} /* End of TcpIp_DhcpV6_VAddOptElapsedTime() */

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_TX_OPTION_CNT > 0))
/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VAddUserOptions()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VAddUserOptions(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  uint8 TxOptIdx;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  for (TxOptIdx = 0; TxOptIdx < TCPIP_DHCPV6_TX_OPTION_CNT; TxOptIdx++)
  {
    if (0 < IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][TxOptIdx].OptLen)
    {
      (void)TcpIp_DhcpV6_VAddRawOpt(IpCtrlIdx, MsgPtr, IpV6_DhcpV6_TxOptionId[TxOptIdx], IpV6_DhcpV6_TxOptionData[IpCtrlIdx][TxOptIdx], IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][TxOptIdx].OptLen);
    }
  }

  return E_OK;
} /* End of TcpIp_DhcpV6_VAddUserOptions() */
#endif

/******************************
 * MESSAGE ENCODING FUNCTIONS *
 ******************************/

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildSolicit()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildSolicit(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VTxSolicit()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_SOL);

  /* SHOULD add option request option [RFC3315 17.1.1. Creation of Solicit Messages] */
  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptReqOpt(IpCtrlIdx, MsgPtr, &TcpIp_DhcpV6_OptionRequestList[0], TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildSolicit() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildInfoRequest()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildInfoRequest(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VBuildInfoRequest()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_INF);

  /* MUST add option request option [RFC3315 18.1.5. Creation and Transmission of Information-request Messages] */
  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptReqOpt(IpCtrlIdx, MsgPtr, &TcpIp_DhcpV6_OptionRequestList[TCPIP_DHCPV6_OPT_REQ_OPT_LIST_EXTRA_OPT_OFS], TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN - TCPIP_DHCPV6_OPT_REQ_OPT_LIST_EXTRA_OPT_OFS);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildInfoRequest() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildRelease()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRelease(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VBuildRelease()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_REL);

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptServerIdent(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaAddr(IpCtrlIdx, MsgPtr, &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaAddr(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildRelease() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildRenew()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRenew(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VBuildRenew()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_REN);

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptServerIdent(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaAddr(IpCtrlIdx, MsgPtr, &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaAddr(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  /* MUST add option request option [RFC3315 18.1.3. Creation and Transmission of Renew Messages] */
  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptReqOpt(IpCtrlIdx, MsgPtr, &TcpIp_DhcpV6_OptionRequestList[0], TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildRenew() */ /* PRQA S 6010, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCAL */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildRebind()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRebind(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VBuildRebind()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_REB);

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaAddr(IpCtrlIdx, MsgPtr, &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaAddr(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  /* MUST add option request option [RFC3315 18.1.4. Creation and Transmission of Rebind Messages] */

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptReqOpt(IpCtrlIdx, MsgPtr, &TcpIp_DhcpV6_OptionRequestList[0], TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildRebind() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildDecline()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildDecline(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VBuildDecline()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_DEC);

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptServerIdent(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaAddr(IpCtrlIdx, MsgPtr, &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaAddr(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildDecline() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildConfirm()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildConfirm(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr,                         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr->MsgBufPtr,                 TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_MSG_HDR_LEN <= MsgPtr->MsgBufLen, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
#endif

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING0("TcpIp_DhcpV6_VBuildConfirm()");
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_CNF);

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaAddr(IpCtrlIdx, MsgPtr, &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaAddr(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildConfirm() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VBuildRequest()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VBuildRequest(
  uint8                                                  IpCtrlIdx,
  P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType Result;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  Result = TcpIp_DhcpV6_VBeginMessage(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_MSG_REQ);

  /* add server identifier */
  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptServerIdent(IpCtrlIdx, MsgPtr);
  }

  /* MUST add option request option [RFC3315 18.1.1. Creation and Transmission of Request Messages ] */
  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VAddOptReqOpt(IpCtrlIdx, MsgPtr, &TcpIp_DhcpV6_OptionRequestList[0], TCPIP_DHCPV6_OPT_REQ_OPT_LIST_LEN);
  }

  /* add IA_NA option */
  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VBeginOptIaNa(IpCtrlIdx, MsgPtr, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId, 0, 0);
  }

  if (E_OK == Result)
  {
    Result = TcpIp_DhcpV6_VEndOptIaNa(IpCtrlIdx, MsgPtr);
  }

  if (E_OK == Result)
  {
    (void)TcpIp_DhcpV6_VEndMessage(IpCtrlIdx, MsgPtr);
  }

  return Result;
} /* End of TcpIp_DhcpV6_VBuildRequest() */

/* DHCPv6 MESSATE TX */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VTriggerTx()
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VTriggerTx(P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  MsgPtr->TxPending = TRUE;
} /* End of TcpIp_DhcpV6_VTriggerTx() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VTxMessage()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VTxMessage(uint8 IpCtrlIdx, P2VAR(TcpIp_DhcpV6_MsgType, AUTOMATIC, TCPIP_APPL_VAR) MsgPtr)
{
  Std_ReturnType                   Result;
  uint16                           OldBufPos;
  uint8                           *BufPtr;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  TcpIp_IpTxRequestDescriptorType  IpTxReqDesc;
  IpV6_AddrIdType                  IpV6AddrId;
  uint16                           BufLenByte;
  uint16                           LenByte; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != MsgPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* store current message length */
  OldBufPos = MsgPtr->MsgBufPos;

  Result = TcpIp_DhcpV6_VAddOptElapsedTime(IpCtrlIdx, MsgPtr);

  if (E_OK == Result)
  {
    IpV6AddrId = IpV6_GetAsBcAddrId(IpCtrlIdx);

    LenByte = MsgPtr->MsgBufPos +  sizeof(TcpIp_Udp_HdrType);
    BufLenByte = LenByte;  /* Add UDP Header length */

    if(BUFREQ_OK == IpV6_ProvideTxBuffer(&IpV6AddrId, TcpIp_DhcpV6_VCfgGetSocketHandle(), &TcpIp_DhcpV6_Addr_All_DHCP_Relay_Agents_and_Servers, &BufPtr, &BufLenByte, &IpTxReqDesc, FALSE))
    {
      /* Build UDP Header */
      TcpIp_Udp_HdrType *UdpHdrPtr;
      UdpHdrPtr = (TcpIp_Udp_HdrType*)BufPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

      UdpHdrPtr->SrcPort  = IPV6_HTONS(TCPIP_DHCPV6_CLIENT_PORT);
      UdpHdrPtr->TgtPort  = IPV6_HTONS(TCPIP_DHCPV6_SERVER_PORT);
      UdpHdrPtr->LenByte  = IPV6_HTONS(LenByte);
      UdpHdrPtr->Checksum = 0;

      IpBase_Copy((IpBase_CopyDataType*)&BufPtr[sizeof(TcpIp_Udp_HdrType)], (IpBase_CopyDataType*)&MsgPtr->MsgBufPtr[0], MsgPtr->MsgBufPos); /* PRQA S 0310 */ /* MD_IpV6_0310 */

      /* UDP checksum is claculated by lower layer (IPv6 or in hardware) */

      if (E_OK == IpV6_Transmit(TcpIp_DhcpV6_VCfgGetSocketHandle(), &IpTxReqDesc, TCPIP_PROTOCOL_UDP, LenByte, FALSE, 0, FALSE))
      {
        Result = E_OK;
      }
    }

    /* remove elapsed time option from message buffer */
    MsgPtr->MsgBufPos = OldBufPos;
  }

  return Result;
} /* End of TcpIp_DhcpV6_VTxMessage() */

/* OPTION DECODING */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VGetNextOption()
 **********************************************************************************************************************/
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_VGetNextOption(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptLocationType, AUTOMATIC, TCPIP_APPL_VAR) OptPosPtr)
{
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,   TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != OptPosPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (TCPIP_DHCPV6_GET_OPT_OK != OptPosPtr->ErrCode)
  {
    /* previous opeartion failed */
    return FALSE;
  }

  OptPosPtr->OptOfs += OptPosPtr->OptLen;

  if (OptPosPtr->OptOfs == DataLen)
  {
    /* all options have been read */
    return FALSE;
  }

  if ((OptPosPtr->OptOfs + TCPIP_DHCPV6_OPT_HDR_LEN) > DataLen)
  {
    /* not enough space left in buffer for header of next option */
    OptPosPtr->ErrCode = TCPIP_DHCPV6_GET_OPT_INV_HDR_LENGTH;
    return FALSE;
  }

  OptPtr = &DataPtr[OptPosPtr->OptOfs];

  OptPosPtr->OptCode = (uint16)(((uint16)OptPtr[0] << 8) | ((uint16)OptPtr[1]));
  OptPosPtr->OptLen  = (uint16)(((uint16)OptPtr[2] << 8) | ((uint16)OptPtr[3]));

  OptPosPtr->OptOfs += TCPIP_DHCPV6_OPT_HDR_LEN;

  if ((OptPosPtr->OptOfs + OptPosPtr->OptLen) > DataLen)
  {
    /* option data length exceeds data buffer length */
    OptPosPtr->ErrCode = TCPIP_DHCPV6_GET_OPT_INV_LENGTH;
    return FALSE;
  }

  return TRUE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VGetNextOption() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDecodeOptIaAddr()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptIaAddr(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  P2VAR(TcpIp_DhcpV6_OptIaAddrType, AUTOMATIC, TCPIP_APPL_VAR) IaAddrPtr,
  uint8 IaAddrCount)
#else
  P2VAR(TcpIp_DhcpV6_OptIaAddrType, AUTOMATIC, TCPIP_APPL_VAR) IaAddrPtr)
#endif
{
  TcpIp_DhcpV6_OptLocationType OptLoc;
  P2CONST(TcpIp_DhcpV6_OptIaAddrHdrRawType, AUTOMATIC, TCPIP_APPL_DATA) OptIaAddrPtr;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,   TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,           TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != IaAddrPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* check minimum length of IAADDR option */
  if (TCPIP_DHCPV6_OPT_IAADDR_HDR_LEN > DataLen)
  {
    return E_NOT_OK;
  }

  OptIaAddrPtr = (P2CONST(TcpIp_DhcpV6_OptIaAddrHdrRawType, AUTOMATIC, TCPIP_APPL_DATA))&DataPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  OptLoc.OptOfs = sizeof(TcpIp_DhcpV6_OptIaAddrHdrRawType);
  OptLoc.OptLen = 0;
  OptLoc.ErrCode = TCPIP_DHCPV6_GET_OPT_OK;

#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  {
    boolean IaAddrIdxValid = FALSE;

    for (IaAddrIdx = 0; IaAddrIdx < IaAddrCount; IaAddrIdx++)
    {
      if (FALSE == IaAddrPtr[IaAddrIdx].Valid)
      {
        VStdMemCpy(&IaAddrPtr[IaAddrIdx].Addr, &OptIaAddrPtr->Addr.addr, sizeof(IpBase_AddrIn6Type));
        IaAddrPtr[IaAddrIdx].PreferredLifetime = IPV6_NTOHL(OptIaAddrPtr->PreferredLifetimeNbo);
        IaAddrPtr[IaAddrIdx].ValidLifetime     = IPV6_NTOHL(OptIaAddrPtr->ValidLifetimeNbo);

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
        CANOE_WRITE_STRING3("Opt IA_NA, Addr: %s, PreferredLifetime: %d, ValidLifetime: %d", IpV6_VNetAddr2String(&IaAddrPtr[IaAddrIdx].Addr.addr), IaAddrPtr[IaAddrIdx].PreferredLifetime, IaAddrPtr[IaAddrIdx].ValidLifetime);
#endif

        IaAddrIdxValid = TRUE;
        break;
      }
    }

    if (FALSE == IaAddrIdxValid)
    {
      return E_NOT_OK;
    }
  }
#else
  IPV6_ADDR_COPY(IaAddrPtr->Addr, OptIaAddrPtr->Addr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
  IaAddrPtr->PreferredLifetime = IPV6_NTOHL(OptIaAddrPtr->PreferredLifetimeNbo);
  IaAddrPtr->ValidLifetime     = IPV6_NTOHL(OptIaAddrPtr->ValidLifetimeNbo);

  if (IaAddrPtr->PreferredLifetime <= IaAddrPtr->ValidLifetime)
  {
    IaAddrPtr->Valid = TRUE;
  }
  else
  {
    IaAddrPtr->Valid = FALSE;
  }
#endif

  IaAddrPtr->StatusCode.StatusCode = TCPIP_DHCPV6_STATUS_CODE_SUCCESS;
  IaAddrPtr->StatusCode.StatusTextLen = 0;

  while (TRUE == TcpIp_DhcpV6_VGetNextOption(DataPtr, DataLen, &OptLoc))
  {
    switch (OptLoc.OptCode)
    {
    case TCPIP_DHCPV6_OPT_STATUS_CODE:
      if (E_OK == TcpIp_DhcpV6_VDecodeOptStatusCode(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &IaAddrPtr->StatusCode))
      {

      }
      break;

    default:
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING3("TcpIp_DhcpV6_VDecodeOptIaAddr() Skipping unknown option code %d, length %d at offset %d", OptLoc.OptCode, OptLoc.OptLen, OptLoc.OptOfs);
#endif
      break;
    }
  } /* option loop */

  if(TCPIP_DHCPV6_GET_OPT_OK != OptLoc.ErrCode)
  {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
    CANOE_WRITE_STRING0("TcpIp_DhcpV6_VRxReply() Option parsing error!");
#endif
    return E_NOT_OK;
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VDecodeOptIaAddr() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDecodeOptIaNa()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptIaNa(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptIaNaType, AUTOMATIC, TCPIP_APPL_VAR) IaNaPtr)
{
  TcpIp_DhcpV6_OptLocationType OptLoc;
  uint32 RecIaId;

  P2CONST(TcpIp_DhcpV6_OptIaNaHdrRawType, AUTOMATIC, TCPIP_APPL_DATA) OptIaNaRawPtr;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != IaNaPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* check minimum length of IA_NA option */
  if (TCPIP_DHCPV6_OPT_IA_NA_HDR_LEN > DataLen)
  {
    return E_NOT_OK;
  }

  OptIaNaRawPtr = (P2CONST(TcpIp_DhcpV6_OptIaNaHdrRawType, AUTOMATIC, TCPIP_APPL_DATA))&DataPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  RecIaId = IPBASE_HTON32(OptIaNaRawPtr->IaIdNbo);

  if (RecIaId != IaNaPtr->IaId)
  {
    return E_NOT_OK;
  }

  IaNaPtr->T1 = IPBASE_HTON32(OptIaNaRawPtr->T1Nbo);
  IaNaPtr->T2 = IPBASE_HTON32(OptIaNaRawPtr->T2Nbo);

  if ((IaNaPtr->T2 > 0) && (IaNaPtr->T1 > IaNaPtr->T2))
  {
    /* this option MUST be ignored. */
    IaNaPtr->Valid = FALSE;
    return E_NOT_OK;
  }

  IaNaPtr->Valid = TRUE;

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
  CANOE_WRITE_STRING3("Opt IA_NA, IAID: %08x, T1: %d, T2: %d", IaNaPtr->IaId, IaNaPtr->T1, IaNaPtr->T2);
#endif

  OptLoc.OptOfs = sizeof(TcpIp_DhcpV6_OptIaNaHdrRawType);
  OptLoc.OptLen = 0;
  OptLoc.ErrCode = TCPIP_DHCPV6_GET_OPT_OK;

  IaNaPtr->StatusCode.StatusCode = TCPIP_DHCPV6_STATUS_CODE_SUCCESS;
  IaNaPtr->StatusCode.StatusTextLen = 0;

#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  for(IaAddrIdx = 0; IaAddrIdx < IaNaPtr->IaAddrCount; IaAddrIdx++)
  {
    IaNaPtr->IaAddrPtr[IaAddrIdx].Valid = FALSE;
  }
#endif

  /* iterate over all options */
  while (TRUE == TcpIp_DhcpV6_VGetNextOption(DataPtr, DataLen, &OptLoc))
  {
    switch (OptLoc.OptCode)
    {
    case TCPIP_DHCPV6_OPT_IAADDR:
      (void)TcpIp_DhcpV6_VDecodeOptIaAddr(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &IaNaPtr->IaAddr);
      break;

    case TCPIP_DHCPV6_OPT_STATUS_CODE:
      if (E_OK == TcpIp_DhcpV6_VDecodeOptStatusCode(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &IaNaPtr->StatusCode))
      {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
        CANOE_WRITE_STRING2("IA_NA Status: %d - %s.", IaNaPtr->StatusCode.StatusCode, TcpIp_DhcpV6_VGetStatusMessage(IaNaPtr->StatusCode.StatusTextPtr, IaNaPtr->StatusCode.StatusTextLen));
#endif
      }
      break;

    default:
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING3("TcpIp_DhcpV6_VDecodeOptIaNa() Skipping unknown option code %d, length %d at offset %d", OptLoc.OptCode, OptLoc.OptLen, OptLoc.OptOfs);
#endif
      break;
    }
  } /* option loop */

  if(TCPIP_DHCPV6_GET_OPT_OK != OptLoc.ErrCode)
  {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
    CANOE_WRITE_STRING0("TcpIp_DhcpV6_VRxReply() Option parsing error!");
#endif
    return E_NOT_OK;
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VDecodeOptIaNa() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDecodeOptStatusCode()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptStatusCode(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptStatusCodeType, AUTOMATIC, TCPIP_APPL_VAR) StatusCodePtr)
{
  P2CONST(TcpIp_DhcpV6_OptStatusCodeHdrRawType, AUTOMATIC, TCPIP_APPL_DATA) OptStatusCodeHdrPtr;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,     TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION,   TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,             TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION,   TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != StatusCodePtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* check minimum length of status code option */
  if (TCPIP_DHCPV6_OPT_STATUS_CODE_HDR_LEN > DataLen)
  {
    return E_NOT_OK;
  }

  OptStatusCodeHdrPtr = (P2CONST(TcpIp_DhcpV6_OptStatusCodeHdrRawType, AUTOMATIC, TCPIP_APPL_DATA))&DataPtr[0]; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  StatusCodePtr->StatusCode = IPV6_HTONS(OptStatusCodeHdrPtr->StatusCodeNbo);
  StatusCodePtr->StatusTextPtr = &DataPtr[sizeof(TcpIp_DhcpV6_OptStatusCodeHdrRawType)];
  StatusCodePtr->StatusTextLen = (uint16)(DataLen - sizeof(TcpIp_DhcpV6_OptStatusCodeHdrRawType));

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VDecodeOptStatusCode() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDecodeOptPreference()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptPreference(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) PreferenceValuePtr)
{
#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,            TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,                    TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != PreferenceValuePtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* check minimum length of status code option */
  if (TCPIP_DHCPV6_OPT_PREFERENCE_LEN != DataLen)
  {
    return E_NOT_OK;
  }

  *PreferenceValuePtr = DataPtr[0];

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VDecodeOptPreference() */

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDecodeOptDnsServers()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptDnsServers(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptDnsServersType, AUTOMATIC, TCPIP_APPL_VAR) DnsServersOptPtr)
{
#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,          TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,                  TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DnsServersOptPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* check minimum length of dns servers option */
  if (0 != (DataLen & TCPIP_DHCPV6_MASK_MOD_16))
  {
    /* "Length of the list of DNS recursive name servers in octets;
     *  must be a multiple of 16"
     *  [RFC3646 3. DNS Recursive Name Server option]
     */

    DnsServersOptPtr->Valid = FALSE;
    return E_NOT_OK;
  }

  DnsServersOptPtr->Valid = TRUE;
  DnsServersOptPtr->Addrs = (IpBase_AddrIn6Type*)DataPtr;
  DnsServersOptPtr->AddrCount = (DataLen >> TCPIP_DHCPV6_RSHIFT_DIV_16);

  return E_OK;
} /* End of TcpIp_DhcpV6_VDecodeOptDnsServers() */
#endif

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VDecodeOptDomainList()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VDecodeOptDomainList(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 DataLen,
  P2VAR(TcpIp_DhcpV6_OptDomainListType, AUTOMATIC, TCPIP_APPL_VAR) DomainListOptPtr)
{
#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,          TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,                  TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DomainListOptPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  DomainListOptPtr->Valid = TRUE;
  DomainListOptPtr->List = DataPtr;
  DomainListOptPtr->ListLength = DataLen;

  return E_OK;
} /* End of TcpIp_DhcpV6_VDecodeOptDomainList() */
#endif

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_RX_OPTION_CNT > 0))
/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VHandleUserOptions()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VHandleUserOptions(
  uint8                                      MsgType,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen)
{
  TcpIp_DhcpV6_OptLocationType OptLoc;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr,          TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,                  TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_PARAM,   E_NOT_OK);
#endif

  OptLoc.OptOfs = 0;
  OptLoc.OptLen = 0;
  OptLoc.ErrCode = TCPIP_DHCPV6_GET_OPT_OK;

  while (TRUE == TcpIp_DhcpV6_VGetNextOption(DataPtr, DataLen, &OptLoc))
  {
    TcpIp_RxDhcpV6OptFcts(0, OptLoc.OptCode, OptLoc.OptLen, &DataPtr[OptLoc.OptOfs], MsgType);
  }

  return E_OK;
} /* End of TcpIp_DhcpV6_VHandleUserOptions() */
#endif

/* MESSAGE RECEPTION */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VRxAdvertise()
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VRxAdvertise(
  uint8                                      IpCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen)
{
  TcpIp_DhcpV6_OptLocationType OptLoc;
  boolean ClientDuidValid = FALSE;
  boolean ServerDuidValid = FALSE;
  uint16  ServerDuidOfs = 0;
  uint16  ServerDuidLen = 0;
  TcpIp_DhcpV6_OptStatusCodeType OptStCode;
  uint8 ServerPreference = 0;

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  /* assume status success if no status code option present */
  OptStCode.StatusCode = TCPIP_DHCPV6_STATUS_CODE_SUCCESS;
  OptStCode.StatusTextLen = 0;

  TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).Valid = FALSE;

  OptLoc.OptOfs = 0;
  OptLoc.OptLen = 0;
  OptLoc.ErrCode = TCPIP_DHCPV6_GET_OPT_OK;

  /* parse options */
  while (TRUE == TcpIp_DhcpV6_VGetNextOption(DataPtr, DataLen, &OptLoc))
  {
    switch (OptLoc.OptCode)
    {
    case TCPIP_DHCPV6_OPT_CLIENTID:
      /* check client identifier */
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      TcpIp_DhcpV6_VDbgPrintOptId(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen);
#endif

      if (TRUE == TcpIp_DhcpV6_VCompareBytes(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, (uint8*)&TCPIP_DHCPV6_STATUS_VAR(ClientDuid), TCPIP_DHCPV6_CLIENT_DUID_LEN)) /* PRQA S 0310 */ /* MD_IpV6_0310 */
      {
        /* valid client identifier */
        ClientDuidValid = TRUE;
      }
      else
      {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
        CANOE_WRITE_STRING0("TcpIp_DhcpV6_VRxAdvertise() invalid client identifier!");
#endif
      }
      break;

    case TCPIP_DHCPV6_OPT_SERVERID:
      if ((0 < OptLoc.OptLen) && (TCPIP_DHCPV6_DUID_MAX_LEN >= OptLoc.OptLen))
      {
        ServerDuidValid = TRUE;
        ServerDuidOfs = OptLoc.OptOfs;
        ServerDuidLen = OptLoc.OptLen;
      }
      else
      {
        TcpIp_DhcpV6_DetReportError(TCPIP_DHCPV6_API_ID_RX_INDICATION, TCPIP_DHCPV6_E_SERVER_ID_OVFL);
      }
      break;

    case TCPIP_DHCPV6_OPT_STATUS_CODE:

      if (E_OK != TcpIp_DhcpV6_VDecodeOptStatusCode(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &OptStCode))
      {
        /* invalid status code */
      }
      break;

    case TCPIP_DHCPV6_OPT_IA_NA:
      if (E_OK != TcpIp_DhcpV6_VDecodeOptIaNa(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf)))
      {
        /* invalid IA_NA option */
      }
      break;

    case TCPIP_DHCPV6_OPT_PREFERENCE:
      if (E_OK != TcpIp_DhcpV6_VDecodeOptPreference(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &ServerPreference))
      {
        /* invalid preference option */
      }
      break;

    default:
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING3("TcpIp_DhcpV6_VRxAdvertise() Skipping unknown option code %d, length %d at offset %d", OptLoc.ErrCode, OptLoc.OptLen, OptLoc.OptOfs);
#endif
      break;
    }
  } /* option loop */

  if(TCPIP_DHCPV6_GET_OPT_OK != OptLoc.ErrCode)
  {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
    CANOE_WRITE_STRING0("TcpIp_DhcpV6_VRxReply() Option parsing error!");
#endif
    return E_NOT_OK;
  }

  /* validate received advertisement */
  if ((TRUE == ClientDuidValid) && (TRUE == ServerDuidValid) && (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == OptStCode.StatusCode))
  {
    if ((TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).Valid) && (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).StatusCode.StatusCode))
    {
      if ((TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.Valid) && (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.StatusCode.StatusCode))
      {
        if ((FALSE == TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Valid) || (TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Preference < ServerPreference))
        {
          TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Valid = TRUE;
          TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Preference  = ServerPreference;
          TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerIdLen = (uint8)ServerDuidLen;
          VStdMemCpy(&TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerId[0], &DataPtr[ServerDuidOfs], TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerIdLen);
        }
      }
    }
  }

  return (TRUE == TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Valid) ? E_OK : E_NOT_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VRxAdvertise() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VRxReply()
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VRxReply(
  uint8                                      IpCtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                     DataLen)
{
  TcpIp_DhcpV6_OptLocationType OptLoc;
  boolean ValidClientId = FALSE;
  boolean ValidServerId = FALSE;
  TcpIp_DhcpV6_OptStatusCodeType OptStatusCode;
#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
  TcpIp_DhcpV6_OptDnsServersType OptDnsServers;
  TcpIp_DhcpV6_OptDomainListType OptDomainList;
#endif

#if (TCPIP_DHCPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != DataPtr, TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(0 < DataLen,         TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION, TCPIP_DHCPV6_E_INV_POINTER, E_NOT_OK);
#endif

  TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).Valid = FALSE;
  OptStatusCode.StatusCode = TCPIP_DHCPV6_STATUS_CODE_SUCCESS;
  OptStatusCode.StatusTextLen = 0;

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
  OptDnsServers.Valid = FALSE;
  OptDomainList.Valid = FALSE;
#endif

  OptLoc.OptOfs = 0;
  OptLoc.OptLen = 0;
  OptLoc.ErrCode = TCPIP_DHCPV6_GET_OPT_OK;

  while (TRUE == TcpIp_DhcpV6_VGetNextOption(DataPtr, DataLen, &OptLoc))
  {
    switch (OptLoc.OptCode)
    {
    case TCPIP_DHCPV6_OPT_CLIENTID:
      if (TRUE == TcpIp_DhcpV6_VCompareBytes(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, (uint8*)&TCPIP_DHCPV6_STATUS_VAR(ClientDuid), TCPIP_DHCPV6_CLIENT_DUID_LEN)) /* PRQA S 0310 */ /* MD_IpV6_0310 */
      {
        ValidClientId = TRUE;
      }
      break;

    case TCPIP_DHCPV6_OPT_SERVERID:
      if (TRUE == TcpIp_DhcpV6_VCompareBytes(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerId[0], TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerIdLen))
      {
        ValidServerId = TRUE;
      }
      break;

    case TCPIP_DHCPV6_OPT_STATUS_CODE:
      {
        (void)TcpIp_DhcpV6_VDecodeOptStatusCode(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &OptStatusCode);

#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING2("Reply Status Code: %d - %s.", OptStatusCode.StatusCode, TcpIp_DhcpV6_VGetStatusMessage(OptStatusCode.StatusTextPtr, OptStatusCode.StatusTextLen));
#endif
      }
      break;

    case TCPIP_DHCPV6_OPT_IA_NA:
      if (E_OK != TcpIp_DhcpV6_VDecodeOptIaNa(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf)))
      {
        /* invalid IA_NA option */
      }
      break;

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
    case TCPIP_DHCPV6_OPT_DNS_SERVERS:
      if (TcpIp_DhcpV6_VCfgIsRdnssOptEnabled())
      {
        if (E_OK != TcpIp_DhcpV6_VDecodeOptDnsServers(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &OptDnsServers))
        {
          /* invalid DNS_SERVERS option */
        }
      }
      break;

    case TCPIP_DHCPV6_OPT_DOMAIN_LIST:
      if (TcpIp_DhcpV6_VCfgIsDnsslOptEnabled())
      {
        if (E_OK != TcpIp_DhcpV6_VDecodeOptDomainList(&DataPtr[OptLoc.OptOfs], OptLoc.OptLen, &OptDomainList))
        {
          /* invalid DOMAIN_LIST option */
        }
      }
      break;
#endif

    default:
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING3("TcpIp_DhcpV6_VRxReply() Skipping unknown option code %d, length %d at offset %d", OptLoc.OptCode, OptLoc.OptLen, OptLoc.OptOfs);
#endif
      break;
    }
  } /* option loop */

  if(TCPIP_DHCPV6_GET_OPT_OK != OptLoc.ErrCode)
  {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
    CANOE_WRITE_STRING0("TcpIp_DhcpV6_VRxReply() Option parsing error!");
#endif
    return;
  }

  if ((FALSE == ValidClientId) || (FALSE == ValidServerId))
  {
    /* ignore reply messages with invalid client or server identifier */
    return;
  }

  switch (TCPIP_DHCPV6_STATUS_VAR(State))
  {
  case TCPIP_DHCPV6_STATE_TX_REL: /* Release */
    if (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == OptStatusCode.StatusCode)
    {
      /* Received reply in response to release message */
      TCPIP_DHCPV6_CHANGE_STATE(UNBOUND);
    }
    break;

  case TCPIP_DHCPV6_STATE_TX_DEC: /* Decline */
    if (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == OptStatusCode.StatusCode)
    {
      /* Received reply in response to decline message */
      TCPIP_DHCPV6_CHANGE_STATE(UNBOUND);
    }
  break;

  case TCPIP_DHCPV6_STATE_TX_REQ: /* Request */
  case TCPIP_DHCPV6_STATE_TX_CNF: /* Confirm */
  case TCPIP_DHCPV6_STATE_TX_REB: /* Rebind */
  case TCPIP_DHCPV6_STATE_TX_REN: /* Renew */

    /* Received reply in response to request, confirm, rebind or renew message */

    if (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == OptStatusCode.StatusCode)
    {
#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
      if (TcpIp_DhcpV6_VCfgIsRdnssOptEnabled())
      {
        if (TRUE == OptDnsServers.Valid)
        {
          Dns_UpdateIpV6Servers(OptDnsServers.Addrs, OptDnsServers.AddrCount, TCPIP_DHCPV6_LIFETIME_UNLIMITED, 0);
        }
      }

      if (TcpIp_DhcpV6_VCfgIsDnsslOptEnabled())
      {
        if (TRUE == OptDomainList.Valid)
        {
          /* TODO: */
        }
      }
#endif

      /* requesting */
      if (TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).Valid)
      {
        /* reply contains an IA_NA option */

        if (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).StatusCode.StatusCode)
        {
          /* IA_NA option has positive status code */

          TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid = TRUE;
          TCPIP_DHCPV6_STATUS_VAR(IaNaLease).T1Timeout = (TcpIp_DhcpV6_Time.S + TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).T1);
          TCPIP_DHCPV6_STATUS_VAR(IaNaLease).T2Timeout = (TcpIp_DhcpV6_Time.S + TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).T2);

          if ((TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.Valid) && (TCPIP_DHCPV6_STATUS_CODE_SUCCESS == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.StatusCode.StatusCode))
          {
            /* IA_NA option contains valid address */

#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_RX_OPTION_CNT > 0))
            TcpIp_DhcpV6_VHandleUserOptions(TCPIP_DHCPV6_MSG_REP, DataPtr, DataLen);
#endif

            /* update address lease information */

            IPV6_ADDR_COPY(TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.Addr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
            TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.PreferredUntil = (TcpIp_DhcpV6_Time.S + TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.PreferredLifetime);
            TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.ValidUntil     = (TcpIp_DhcpV6_Time.S + TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.ValidLifetime);

            if (E_OK == IpV6_SetAddress(TCPIP_DHCPV6_VAR(TcpIp_DhcpV6_IpAddrId), &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.PreferredLifetime, TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaAddr.ValidLifetime, FALSE))
            {
              TCPIP_DHCPV6_CHANGE_STATE(BOUND);

              /* wait until T1 has been expired */
              TCPIP_DHCPV6_TIME_SET(TCPIP_DHCPV6_STATUS_VAR(IdleTime), TCPIP_DHCPV6_STATUS_VAR(IaNaLease).T1Timeout, 0);
            }
            else
            {
              TCPIP_DHCPV6_CHANGE_STATE(FAIL);
            }
          }
        }
        else if (TCPIP_DHCPV6_STATUS_CODE_NOT_ON_LINK == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).StatusCode.StatusCode)
        {
          /* the requested address is not on-link and must not be used */

          TcpIp_DhcpV6_VRestartAddressAssignment(IpCtrlIdx, 0);
        }
        else if (TCPIP_DHCPV6_STATUS_CODE_NO_BINDING == TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).StatusCode.StatusCode)
        {
          /* the DHCP server has no valid binding. request new address. */

          (void)TcpIp_DhcpV6_VBuildRequest(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));
          TCPIP_DHCPV6_INIT_RETRANS_PARAMS(REQ);
          TCPIP_DHCPV6_CHANGE_STATE(TX_REQ);
          TCPIP_DHCPV6_SET_IDLE_TIME_S(0);
        }
        else
        {
          /* Unknown status code in IA_NA option. Ignore the reply */
        }
      }
      else
      {
        /* reply message does not contain matching IA_NA option. Ignore reply. */
      }
    }
    else if ((TCPIP_DHCPV6_STATUS_CODE_NO_BINDING == OptStatusCode.StatusCode) ||
             (TCPIP_DHCPV6_STATUS_CODE_NOT_ON_LINK == OptStatusCode.StatusCode))
    {
      /* reply message contains negative status code NO_BINDING or NOT_ON_LINK. Restart address assignment procedure. */

      TcpIp_DhcpV6_VRestartAddressAssignment(IpCtrlIdx, 0);
    }
    else
    {
      /* reply message contains negative status code. Do not handle reply. */
    }

    break;

  default:
    /* ignore reply */
    break;
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_VRxReply() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

STATIC FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_VRestartAddressAssignment(uint8 IpCtrlIdx, uint32 DelayMs)
{
  TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Valid = FALSE;
  TCPIP_DHCPV6_STATUS_VAR(AdvInfo).ServerIdLen = 0;

  if (TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid)
  {
    /* remove assigned address */
    (void)IpV6_SetAddress(TCPIP_DHCPV6_VAR(TcpIp_DhcpV6_IpAddrId), &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0, FALSE);

    TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid = FALSE;
  }

  TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId = 0;

#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLeasePtr   = &TcpIp_DhcpV6_IaAddrLeases[0];
  TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLeaseCount = 1;

  TcpIp_DhcpV6_IaAddrLeases[0].ValidUntil = 0;
  TcpIp_DhcpV6_IaAddrLeases[0].PreferredUntil = 0;
#endif

  TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).Valid = FALSE;
  TCPIP_DHCPV6_STATUS_VAR(IaNaOptBuf).IaId = TCPIP_DHCPV6_STATUS_VAR(IaNaLease).IaId;

#if (TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA == STD_ON)
  TcpIp_DhcpV6_IaNaBuf.IaAddrPtr = &TcpIp_DhcpV6_IaAddrBuf[0];
  TcpIp_DhcpV6_IaNaBuf.IaAddrCount = 1;
#endif

  (void)TcpIp_DhcpV6_VBuildSolicit(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));
  TCPIP_DHCPV6_INIT_RETRANS_PARAMS(SOL);
  TCPIP_DHCPV6_CHANGE_STATE(TX_SOL);
  TCPIP_DHCPV6_SET_IDLE_TIME_MS(DelayMs);
}

/* PUBLIC FUNCTIONS */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_IsActive()
 **********************************************************************************************************************/
/*! \brief      Checks if the DhcpV6 is currently trying to acquire a valid address lease or already has one.
 *  \retval     TRUE     DHCPv6 address assignment in progress
 *  \retval     FALSE    DHCPv6 is inactive
 *  \context    task level
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_IsActive(uint8 IpCtrlIdx)
{
  if ((TCPIP_DHCPV6_STATE_INIT == TCPIP_DHCPV6_STATUS_VAR(State)) || (TCPIP_DHCPV6_STATE_FAIL == TCPIP_DHCPV6_STATUS_VAR(State)))
  {
    return FALSE;
  }

  return TRUE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_IsActive() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_StartAddressAssignment()
 **********************************************************************************************************************/
/*! \brief      Manually start DHCPv6 address assignment procedure.
 *              This function will be called by IpV6 automatically if configured.
 *  \retval     E_OK     DHCPv6 address assignment in progress
 *  \retval     E_NOT_OK Could not start DHCPv6 address assignment
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_StartAddressAssignment(uint8 IpCtrlIdx)
{
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_IS_INITIALIZED(), TCPIP_DHCPV6_API_ID_START, TCPIP_DHCPV6_E_NOT_INITIALIZED, E_NOT_OK);

  TcpIp_DhcpV6_VInitClientId(IpCtrlIdx);

  /* delay sending of first solicit message */
  if (TcpIp_DhcpV6_VCfgGetSolicitDelayMin() == TcpIp_DhcpV6_VCfgGetSolicitDelayMax())
  {
    TcpIp_DhcpV6_VRestartAddressAssignment(IpCtrlIdx, TcpIp_DhcpV6_VCfgGetSolicitDelayMin());
  }
  else
  {
    TcpIp_DhcpV6_VRestartAddressAssignment(IpCtrlIdx, (uint32)TcpIp_DhcpV6_VGetRandomValue((sint32)TcpIp_DhcpV6_VCfgGetSolicitDelayMin(), (sint32)TcpIp_DhcpV6_VCfgGetSolicitDelayMax()));
  }

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_StartAddressAssignment() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_StartAddressAssignmentForAddr()
 **********************************************************************************************************************/
/*! \brief      Trigger the address assignment via DHCP
 *  \param[in]  IpAddrId    Address ID for which the address assignment shall be triggered
 *  \retval     E_OK        triggered succeeded
 *  \retval     E_NOT_OK    trigger failed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_StartAddressAssignmentForAddr(uint8 IpAddrId)
{
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   IpAddrId);
  return TcpIp_DhcpV6_StartAddressAssignment(IpCtrlIdx);
}

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_RequestInformation()
 **********************************************************************************************************************/
/*! \brief      Send Information Request message to DHCPv6 servers in order to receive other configuration options
 *              like DNS servers without performig DHCPv6 address assignment.
 *  \retval     E_OK     Sending of Information Request message has been triggered
 *  \retval     E_NOT_OK Sending of Information Request message not possible
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_RequestInformation(uint8 IpCtrlIdx)
{
  Std_ReturnType Result;

  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_IS_INITIALIZED(), TCPIP_DHCPV6_API_ID_RELEASE, TCPIP_DHCPV6_E_NOT_INITIALIZED, E_NOT_OK);

  TcpIp_DhcpV6_VInitClientId(IpCtrlIdx);

  Result = TcpIp_DhcpV6_VBuildInfoRequest(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));

  if (E_OK == Result)
  {
    TCPIP_DHCPV6_INIT_RETRANS_PARAMS(INF);
    TCPIP_DHCPV6_CHANGE_STATE(TX_INF);
    TcpIp_DhcpV6_VTriggerTx(&TCPIP_DHCPV6_STATUS_VAR(Msg));

    /* delay sending of first confirm message */
    if (TcpIp_DhcpV6_VCfgGetInfoRequestDelayMin() == TcpIp_DhcpV6_VCfgGetInfoRequestDelayMax())
    {
      TCPIP_DHCPV6_SET_IDLE_TIME_MS(TcpIp_DhcpV6_VCfgGetInfoRequestDelayMin());
    }
    else
    {
      TCPIP_DHCPV6_SET_IDLE_TIME_MS((uint32)TcpIp_DhcpV6_VGetRandomValue((sint32)TcpIp_DhcpV6_VCfgGetInfoRequestDelayMin(), (sint32)TcpIp_DhcpV6_VCfgGetInfoRequestDelayMax()));
    }
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_RequestInformation() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Decline()
 **********************************************************************************************************************/
/*! \brief      Decline an IPv6 address that has been assigned via DHCPv6
 *  \param[in]  AddrPtr  Declined IPv6 address
 *  \retval     E_OK     Decline procedure has been triggered
 *  \retval     E_NOT_OK There is no valid address lease that can be declined
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_Decline(uint8 IpCtrlIdx, P2CONST(IpBase_AddrIn6Type, AUTOMATIC, IPV6_APPL_DATA) AddrPtr)
{
  Std_ReturnType Result;

  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_IS_INITIALIZED(), TCPIP_DHCPV6_API_ID_START, TCPIP_DHCPV6_E_NOT_INITIALIZED, E_NOT_OK);
  TcpIp_DhcpV6_CheckDetErrorReturnValue(NULL_PTR != AddrPtr,           TCPIP_DHCPV6_API_ID_START, TCPIP_DHCPV6_E_INV_POINTER,     E_NOT_OK);

  if ((TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid) && (TRUE == IpV6_CmpNetAddr(AddrPtr, &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr))) /* PRQA S 3415 */ /* MD_IpV6_3415 */
  {
    Result = TcpIp_DhcpV6_VBuildDecline(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));

    if (E_OK == Result)
    {
      TCPIP_DHCPV6_INIT_RETRANS_PARAMS(DEC);
      TCPIP_DHCPV6_CHANGE_STATE(TX_DEC);
      TCPIP_DHCPV6_SET_IDLE_TIME_S(0);
    }
  }
  else
  {
    Result = FALSE;
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_Decline() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Confirm()
 **********************************************************************************************************************/
/*! \brief      Trigger confirm of active DHCPv6 address leases.
 *              If confirm fails address assignment procedure will be restarted automatically.
 *  \retval     E_OK     Confirm procedure has been triggered
 *  \retval     E_NOT_OK There is no valid address lease that can be confirmed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_Confirm(uint8 IpCtrlIdx)
{
  Std_ReturnType Result;

  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_IS_INITIALIZED(), TCPIP_DHCPV6_API_ID_START, TCPIP_DHCPV6_E_NOT_INITIALIZED, E_NOT_OK);

  if (TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid)
  {
    Result = TcpIp_DhcpV6_VBuildConfirm(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));

    if (E_OK == Result)
    {
      TCPIP_DHCPV6_INIT_RETRANS_PARAMS(CNF);
      TCPIP_DHCPV6_CHANGE_STATE(TX_CNF);

      /* delay sending of first confirm message */
      if (TcpIp_DhcpV6_VCfgGetConfirmDelayMin() == TcpIp_DhcpV6_VCfgGetConfirmDelayMax())
      {
        TCPIP_DHCPV6_SET_IDLE_TIME_MS(TcpIp_DhcpV6_VCfgGetConfirmDelayMin());
      }
      else
      {
        TCPIP_DHCPV6_SET_IDLE_TIME_MS((uint32)TcpIp_DhcpV6_VGetRandomValue((sint32)TcpIp_DhcpV6_VCfgGetConfirmDelayMin(), (sint32)TcpIp_DhcpV6_VCfgGetConfirmDelayMax()));
      }
    }
  }
  else
  {
    Result = FALSE;
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_Confirm() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_ReleaseAddress()
 **********************************************************************************************************************/
/*! \brief      Release DHCPv6 address assignment.
 *              This function will call IpV6_SetAddress() with zero lifetime values for any address configured via
 *              DHCPv6, so IpV6 will stop using these addresses.
 *  \retval     E_OK     Release procedure has been triggered
 *  \retval     E_NOT_OK There is no valid address lease that can be released
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_ReleaseAddress(uint8 IpCtrlIdx)
{
  Std_ReturnType Result;

  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnValue(TCPIP_DHCPV6_IS_INITIALIZED(), TCPIP_DHCPV6_API_ID_RELEASE, TCPIP_DHCPV6_E_NOT_INITIALIZED, E_NOT_OK);

  if (TRUE == TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid)
  {
    Result = TcpIp_DhcpV6_VBuildRelease(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));

    if (E_OK == Result)
    {
      (void)IpV6_SetAddress(TCPIP_DHCPV6_VAR(TcpIp_DhcpV6_IpAddrId), &TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.Addr, 0, 0, FALSE);

      TCPIP_DHCPV6_INIT_RETRANS_PARAMS(REL);
      TCPIP_DHCPV6_CHANGE_STATE(TX_REL);
      TCPIP_DHCPV6_SET_IDLE_TIME_S(0);
    }
  }
  else
  {
    Result = FALSE;
  }

  return Result;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_ReleaseAddress() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Reset()
 **********************************************************************************************************************/
/*! \brief      Reset DHCP.
 *              DHCP has to be triggered via TcpIp_DhcpV6_StartAddressAssignment() after calling this function.
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_Reset(uint8 IpCtrlIdx)
{
  TCPIP_DHCPV6_STATUS_VAR(CurrentTransId) = TCPIP_DHCPV6_INV_TRANS_ID;

  TCPIP_DHCPV6_STATUS_VAR(IaNaLease).Valid = FALSE;

  TCPIP_DHCPV6_SET_IDLE_TIME_MS(1000);
  TCPIP_DHCPV6_CHANGE_STATE(INIT);

  TCPIP_DHCPV6_STATUS_VAR(Msg).TxPending = FALSE;
  TCPIP_DHCPV6_STATUS_VAR(Msg).MsgBufPtr = TCPIP_DHCPV6_VAR(TcpIp_DhcpV6_MsgTxBuffer);
  TCPIP_DHCPV6_STATUS_VAR(Msg).MsgBufLen = TcpIp_DhcpV6_VCfgGetMsgTxBufferSize();

/* clear all tx options */
#if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON)  && (TCPIP_DHCPV6_TX_OPTION_CNT > 0))
  {
    TcpIp_DhcpV6_OptSelectorType TxOptIdx = TCPIP_DHCPV6_TX_OPTION_CNT;

    while (0 < TxOptIdx)
    {
      TxOptIdx--;
      IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][TxOptIdx].OptLen = 0;
    }
  }
#endif
} /* End of TcpIp_DhcpV6_Reset() */

#if (TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON)
# if (TCPIP_DHCPV6_TX_OPTION_CNT > 0)
/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VGetTxOption
 **********************************************************************************************************************/
/*! \brief      Get user option data for outgoing DHCPv6 messages. 
 *  \description  By this API service the TCP/IP stack reads the DHCP option data identified by parameter option.
 *  \param[in]  IpAddrId       IP address identifier representing the local IP address and EthIf controller for which
*                              the DHCP option shall be read.
 *  \param[in]  OptId          DHCP option, e.g. Host Name
 *  \param[in]  OptLenPtr      length of DHCP option data
 *  \param[in]  OptPtr         Pointer to memory containing DHCP option data
 *  \param[out] MsgTypesPtr    Define for which DHCP message types the option are sent
 *  \return     E_OK           no error occured. 
 *              E_NOT_OK       DHCP option data could not be read.
 *  \note       Currently only the option 'TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN' is supported.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VGetTxOption(
    uint8 IpAddrId,
    uint16 OptId,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) OptLenPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) MsgTypesPtr )  /* out parameter */
{
  TcpIp_DhcpV6_OptSelectorType OptSelector;
  Std_ReturnType RetValue;
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  /* Currently the implementation is limited to one option that is always sent, but the API is already prepared to do a more generic handling */
  *MsgTypesPtr = 0;

#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_OFF)
  IPV6_UNUSED_PARAMETER(IpAddrId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  /* check for valid DHCP option */
  for(OptSelector = 0; OptSelector < TCPIP_DHCPV6_TX_OPTION_CNT; OptSelector++)
  {
    if(IpV6_DhcpV6_TxOptionId[OptSelector] == OptId)
    {
      break;
    }
  }

  if(OptSelector < TCPIP_DHCPV6_TX_OPTION_CNT)
  {
    /* currently only one single option (FQDN-Option) is supported */
    if(OptId == TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN)
    {
      IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, IpAddrId);

      if (*OptLenPtr >= IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][OptSelector].OptLen)
      {
        /* copy length and host name */
        *OptLenPtr = IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][OptSelector].OptLen;
        if (*OptLenPtr > 0)
        {
          IpBase_Copy(OptPtr, IpV6_DhcpV6_TxOptionData[IpCtrlIdx][OptSelector], *OptLenPtr);
        }
        RetValue = E_OK;
      }
      else
      {
        /* error, provided buffer is too short */
        RetValue = E_NOT_OK;
      }
    }
    else
    {
      /* error, not supported DHCPoption requested */
      RetValue = E_NOT_OK;
    }
  }
  else
  {
    /* error, no matching supported DHCP option found */
    RetValue = E_NOT_OK;
  }

  return RetValue;
} /* End of TcpIp_DhcpV6_VGetTxOption() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VSetTxOption
 **********************************************************************************************************************/
/*! \brief      Set user option data for outgoing DHCPv6 messages. 
 *  \description  By this API service the TCP/IP stack writes the DHCP option data identified by parameter option.
 *  \param[in]  IpAddrId       IP address identifier representing the local IP address and EthIf controller for which
*                              the DHCP option shall be written.
 *  \param[in]  OptId          DHCP option, e.g. Host Name
 *  \param[in]  OptLen         length of DHCP option data
 *  \param[in]  OptPtr         Pointer to memory containing DHCP option data
 *  \param[in]  MsgTypes       Define for which DHCP message types the option shall be sent
 *  \return     E_OK           no error occured. 
 *              E_NOT_OK       DHCP option data could not be written.
 *  \note       Currently only the option 'TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN' is supported.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VSetTxOption(
    uint8 IpAddrId,
    uint16 OptId,
    uint16 OptLen,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr,
    uint16 MsgTypes)
{
  TcpIp_DhcpV6_OptSelectorType OptSelector;
  uint8          IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV6_05.06 */
  Std_ReturnType RetValue;

  /* check for valid DHCP option */
  for(OptSelector = 0; OptSelector < TCPIP_DHCPV6_TX_OPTION_CNT; OptSelector++)
  {
    if(IpV6_DhcpV6_TxOptionId[OptSelector] == OptId)
    {
      break;
    }
  }

  if(OptSelector < TCPIP_DHCPV6_TX_OPTION_CNT)
  {
    /* currently only one single option is supported */
    if(OptId == TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN)
    {
      /* An old API for setting a host name was TcpIp_DhcpV6_SetHostName(). */
#if (IPV6_MULTI_CONTROLLER_SUPPORT == STD_ON)
      IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, IpAddrId);
#else
      IpCtrlIdx = 0;
#endif

      if (OptLen <= IpV6_DhcpV6_TxOptionLenMax[IpCtrlIdx][OptSelector])
      {
        /* copy length, flags and host name */
        IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][OptSelector].OptLen   = OptLen;
        IpV6_DhcpV6_TxOptionInfo[IpCtrlIdx][OptSelector].MsgFlags = MsgTypes;
        IpBase_Copy(IpV6_DhcpV6_TxOptionData[IpCtrlIdx][OptSelector], OptPtr, OptLen);
        RetValue = E_OK;
      }
      else
      {
        /* error, fqdn option data is too long */
        RetValue = E_NOT_OK;
      }
    }
    else
    {
      /* error, not supported option */
      RetValue = E_NOT_OK;
    }
  }
  else
  {
    RetValue = E_NOT_OK;
  }

  return RetValue;
} /* End of TcpIp_DhcpV6_VSetTxOption() */
# endif
#endif

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_InitMemory()
 **********************************************************************************************************************/
/*! \brief      Initializes global variables
 *  \pre        This function has to be called before any other function of this module
 *  \context    System Startup
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_InitMemory(void)
{
  uint8 IpCtrlIdx = TCPIP_MAX_NUM_IPV6_CTRLS; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  while (0 < IpCtrlIdx)
  {
    IpCtrlIdx--;

    if (TcpIp_DhcpV6_VCfgIsDhcpEnabled())
    {
      TCPIP_DHCPV6_STATUS_VAR(State)     = TCPIP_DHCPV6_STATE_UNINIT;
      TCPIP_DHCPV6_STATUS_VAR(LastState) = TCPIP_DHCPV6_STATE_UNINIT;
    }
  }
} /* End of TcpIp_DhcpV6_InitMemory() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Init()
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration  of the DHCP module and may
 *              be used to initialize the data structures.
 *  \pre        TcpIp_VInitSockets()
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \context    initialization
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_Init(
    P2CONST(TcpIp_DhcpV6_ConfigType, TCPIP_PBCFG, TCPIP_CONST) CfgPtr )
{
  uint8 IpCtrlIdx = TCPIP_MAX_NUM_IPV6_CTRLS; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  IPV6_UNUSED_PARAMETER(CfgPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  TCPIP_DHCPV6_TIME_SET(TcpIp_DhcpV6_Time, 0, 0);

  while (0 < IpCtrlIdx)
  {
    IpCtrlIdx--;

    if (TcpIp_DhcpV6_VCfgIsDhcpEnabled())
    {
      TcpIp_DhcpV6_Reset(IpCtrlIdx);
    }
  }

} /* End of TcpIp_DhcpV6_Init() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_MainFunction
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
 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_MainFunction(void)
{
  /* #10 Iterate over all IPv6 controllers that have a local address with DHCPv6 address assignment method. */
  uint8 IpCtrlIdx = TCPIP_MAX_NUM_IPV6_CTRLS; /* PRQA S 0781 */ /* MD_IpV6_05.06 */

  while (0 < IpCtrlIdx)
  {
    IpCtrlIdx--;

    if (TcpIp_DhcpV6_VCfgIsDhcpEnabled() && TCPIP_DHCPV6_IS_INITIALIZED())
    {
      TCPIP_DHCPV6_TIME_ADD_MS(TcpIp_DhcpV6_Time, TCPIP_MAIN_FCT_PERIOD_MSEC);

      /* #20 Calculate elapsed time since start of DHCPv6 address assignment. */
      if (0xFFFFFFFFU > TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs))
      {
        if ((0xFFFFFFFFU - TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs)) > TCPIP_MAIN_FCT_PERIOD_MSEC)
        {
          TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs) += TCPIP_MAIN_FCT_PERIOD_MSEC;
        }
        else
        {
          TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs) = 0xFFFFFFFFU;
        }
      }

      /* #30 Transmit pending DHCPv6 message. */
      if (TRUE == TCPIP_DHCPV6_STATUS_VAR(Msg).TxPending)
      {
        if (1 == TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount)
        {
          TCPIP_DHCPV6_STATUS_VAR(ElapsedTimeMs) = 0;
        }

        /* try to transmit pending message */
        if (E_OK == TcpIp_DhcpV6_VTxMessage(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg)))
        {
          /* message transmitted successfully */
          TCPIP_DHCPV6_STATUS_VAR(Msg).TxPending = FALSE;
        }
      }

      /* #40 Check if action is required for the DCHPv6 instance depending on current state: */ 
      if (TCPIP_DHCPV6_TIME_EXPIRED(TCPIP_DHCPV6_STATUS_VAR(IdleTime)))
      {
        switch (TCPIP_DHCPV6_STATUS_VAR(State))
        {
        case TCPIP_DHCPV6_STATE_TX_REN: /* Renew */
          /* #50 [RENEW] Send RENEW message or switch to REBIND if number of retransmissions has been reached. */
          if (E_OK == TcpIp_DhcpV6_VCalcTimeout(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(IdleTime), FALSE))
          {
            /* (re)transmit renew message */
            TcpIp_DhcpV6_VTriggerTx(&TCPIP_DHCPV6_STATUS_VAR(Msg));
          }
          else
          {
            /* received no reply to renew message, switch to sending rebind messages */
            if (E_OK == TcpIp_DhcpV6_VBuildRebind(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg)))
            {
              TCPIP_DHCPV6_INIT_RETRANS_PARAMS(REB);

              /* send renew messages until valid lifetime has been expired */
              TCPIP_DHCPV6_TIME_SET(TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout, TCPIP_DHCPV6_STATUS_VAR(IaNaLease).AddrLease.ValidUntil, 0);

              TCPIP_DHCPV6_CHANGE_STATE(TX_REB);
            }
            else
            {
              TCPIP_DHCPV6_CHANGE_STATE(FAIL);
            }
          }
          break;

        case TCPIP_DHCPV6_STATE_TX_SOL: /* Solicit */
          /* [SOLICIT] Move to state REQUEST if valid ADVERTISE message has been received from DHCPv6 server. */
          if (TRUE == TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Valid)
          {
            /* received valid advertisement */
            (void)TcpIp_DhcpV6_VBuildRequest(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));
            TCPIP_DHCPV6_INIT_RETRANS_PARAMS(REQ);
            TCPIP_DHCPV6_CHANGE_STATE(TX_REQ);
          }
          /* #70 ELSE: Retransmit SOLICIT message if number of retransmits has not been reached yet. */
          else if (E_OK == TcpIp_DhcpV6_VCalcTimeout(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(IdleTime), TRUE))
          {
            /* (re)transmit solicit message */
            TcpIp_DhcpV6_VTriggerTx(&TCPIP_DHCPV6_STATUS_VAR(Msg));
          }
          else
          {
            TCPIP_DHCPV6_CHANGE_STATE(FAIL);
          }
          break;

        case TCPIP_DHCPV6_STATE_TX_CNF: /* Confirm */
          /* #80 [CONFIRM] (Re-)Transmit CONFIRM message. */
          if (E_OK == TcpIp_DhcpV6_VCalcTimeout(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(IdleTime), FALSE))
          {
            /* (re)transmit message */
            TcpIp_DhcpV6_VTriggerTx(&TCPIP_DHCPV6_STATUS_VAR(Msg));
          }
          else
          {
            TCPIP_DHCPV6_CHANGE_STATE(FAIL);
          }
          break;

        case TCPIP_DHCPV6_STATE_TX_REQ: /* Request */
        case TCPIP_DHCPV6_STATE_TX_DEC: /* Decline */
        case TCPIP_DHCPV6_STATE_TX_REB: /* Rebind */
          /* #90 [REQUEST,DECLINE,REBIND] (Re-)Transmit message or restart DHCPv6 address assignment if number of retransmissions has been reached. */
          if (E_OK == TcpIp_DhcpV6_VCalcTimeout(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(IdleTime), FALSE))
          {
            /* (re)transmit message */
            TcpIp_DhcpV6_VTriggerTx(&TCPIP_DHCPV6_STATUS_VAR(Msg));
          }
          else
          {
            /* restart */
            if (E_OK == TcpIp_DhcpV6_StartAddressAssignment(IpCtrlIdx))
            {
              TCPIP_DHCPV6_CHANGE_STATE(FAIL);
            }
          }

          break;

        case TCPIP_DHCPV6_STATE_TX_REL: /* Release */
          /* #100 [RELEASE] (Re-)Transmit Release message. */
          if (E_OK == TcpIp_DhcpV6_VCalcTimeout(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(IdleTime), FALSE))
          {
            /* (re)transmit message */
            TcpIp_DhcpV6_VTriggerTx(&TCPIP_DHCPV6_STATUS_VAR(Msg));
          }
          else
          {
            TCPIP_DHCPV6_CHANGE_STATE(FAIL);
          }

          break;

        case TCPIP_DHCPV6_STATE_BOUND: /* Bound */
          /* #110 [BOUND] Schedule idle time until first Renew message must be sent and switch to state RENEW. */
          (void)TcpIp_DhcpV6_VBuildRenew(IpCtrlIdx, &TCPIP_DHCPV6_STATUS_VAR(Msg));
          TCPIP_DHCPV6_INIT_RETRANS_PARAMS(REN);

          /* send renew messages until T2 is reached */
          TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout.S = TCPIP_DHCPV6_STATUS_VAR(IaNaLease).T2Timeout;
          TCPIP_DHCPV6_STATUS_VAR(RetransParams).AbsRetransTimeout.Ms = 0;

          TCPIP_DHCPV6_CHANGE_STATE(TX_REN);
          break;

        default:
             /**/
          break;
        }
      }
      else
      {
        /* do nothing else until idle time has expired. e.g. message rx timeout has expired. */
      }
    }
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_MainFunction() */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_RxIndication
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  IpCtrlIdx          ip controller index
 *  \param[in]  DataPtr            pointer to the received data
 *  \param[in]  DataLen            length of the received data in bytes
 *  \param[in]  SourcePtr          source address pointer
 *  \param[in]  DestinationPtr     destination address pointer
 *  \param[in]  ChecksumCalculated    indicates if the UDP checksum is already checked by lower layer
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_RxIndication(
    uint8 IpCtrlIdx,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 DataLen,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SourcePtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestinationPtr
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    , boolean ChecksumCalculated
#endif
    )
{
  uint32 TransId;
  uint8 MsgType;
  uint16 PayloadPos;
  P2CONST(TcpIp_Udp_HdrType, AUTOMATIC, TCPIP_APPL_DATA) UdpHdrPtr;

  if ((!TcpIp_DhcpV6_VCfgIsDhcpEnabled()) || (!TCPIP_DHCPV6_IS_INITIALIZED()))
  {
    return;
  }

  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_DhcpV6_CheckDetErrorReturnVoid(TCPIP_DHCPV6_IS_INITIALIZED(), TCPIP_DHCPV6_API_ID_RX_INDICATION, TCPIP_DHCPV6_E_NOT_INITIALIZED);
  TcpIp_DhcpV6_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,           TCPIP_DHCPV6_API_ID_RX_INDICATION, TCPIP_DHCPV6_E_INV_POINTER);
  TcpIp_DhcpV6_CheckDetErrorReturnVoid(NULL_PTR != SourcePtr,         TCPIP_DHCPV6_API_ID_RX_INDICATION, TCPIP_DHCPV6_E_INV_POINTER);

  UdpHdrPtr = (P2CONST(TcpIp_Udp_HdrType, AUTOMATIC, TCPIP_APPL_DATA))DataPtr; /* PRQA S 0310 */ /* MD_IpV6_0310 */

  if ((TCPIP_DHCPV6_SERVER_PORT != IPBASE_HTON16(UdpHdrPtr->SrcPort)) || (TCPIP_DHCPV6_CLIENT_PORT != IPBASE_HTON16(UdpHdrPtr->TgtPort)))
  {
    return;
  }

  if (IPBASE_HTON16(UdpHdrPtr->LenByte) > DataLen)
  {
    return;
  }

#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
  if(FALSE == ChecksumCalculated)
#endif
  {
    TcpIp_PseudoHdrV6Type PseudoHdr;
    uint32 ChecksumTmp;

    IPV6_ADDR_COPY(PseudoHdr.RemAddr, ((const IpBase_SockAddrIn6Type*)SourcePtr)->sin6_addr);      /* PRQA S 0310 */ /* MD_IpV6_0310 */
    IPV6_ADDR_COPY(PseudoHdr.LocAddr, ((const IpBase_SockAddrIn6Type*)DestinationPtr)->sin6_addr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
    PseudoHdr.Null     = 0;
    PseudoHdr.Protocol = TCPIP_SOCK_PROT_UDP;
    PseudoHdr.LenByte  = IPV6_HTONS(DataLen);

    ChecksumTmp = IpBase_TcpIpChecksumAdd((P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))&PseudoHdr, (uint32)(sizeof(PseudoHdr)),           0, FALSE);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    ChecksumTmp = IpBase_TcpIpChecksumAdd((P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))UdpHdrPtr,                      DataLen, ChecksumTmp,  TRUE);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

    if (0 != (uint16)ChecksumTmp)
    {
      return;
    }
  }

  /* skip UDP header */
  DataPtr = &DataPtr[sizeof(TcpIp_Udp_HdrType)];
  DataLen = IPV6_HTONS(UdpHdrPtr->LenByte) - 8;

  MsgType = DataPtr[0];
  TransId = ((uint32)DataPtr[1] << 16) | ((uint32)DataPtr[2] << 8) | ((uint32)DataPtr[3]);

  if (TransId != TCPIP_DHCPV6_STATUS_VAR(CurrentTransId))
  {
    /* drop message with unexpected transaction id */
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
    CANOE_WRITE_STRING2("TcpIp_DhcpV6_RxIndication() discarding message. Expected transaction id %d but received %d.", TcpIp_DhcpV6_CurrentTransId, TransId);
#endif
    return;
  }

  PayloadPos = TCPIP_DHCPV6_MSG_HDR_LEN;

  switch (MsgType)
  {
  case TCPIP_DHCPV6_MSG_ADV:

    if (TCPIP_DHCPV6_STATE_TX_SOL == TCPIP_DHCPV6_STATUS_VAR(State))
    {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
        CANOE_WRITE_STRING0("TcpIp_DhcpV6_RxIndication() RxAdvertise");
#endif

      if (E_OK == TcpIp_DhcpV6_VRxAdvertise(IpCtrlIdx, &DataPtr[PayloadPos], (DataLen - PayloadPos)))
      {
        if (TcpIp_DhcpV6_VCfgIsUseFirstValidAdvEnabled())
        {
          TCPIP_DHCPV6_SET_IDLE_TIME_S(0);
        }
        else if ((TCPIP_DHCPV6_MAX_PREFERENCE == TCPIP_DHCPV6_STATUS_VAR(AdvInfo).Preference) || (1 < TCPIP_DHCPV6_STATUS_VAR(RetransParams).TxCount))
        {
          /* if we have received a valid advertise with a maximum preference value we continue immediately */
          TCPIP_DHCPV6_SET_IDLE_TIME_S(0);
        }
        else
        {
          /* wait for more advertisements */
        }
      }
    }
    else
    {
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING0("TcpIp_DhcpV6_RxIndication() RxAdvertise in WRONG STATE");
#endif
    }

    break;

  case TCPIP_DHCPV6_MSG_REP:

    /* accept replys only if we are expecting it */
    if ((TCPIP_DHCPV6_STATE_TX_SOL <= TCPIP_DHCPV6_STATUS_VAR(State)) && (TCPIP_DHCPV6_STATE_TX_REL >= TCPIP_DHCPV6_STATUS_VAR(State)))
    {
      TcpIp_DhcpV6_VRxReply(IpCtrlIdx, &DataPtr[PayloadPos], (DataLen - PayloadPos));
    }
    break;

  case TCPIP_DHCPV6_MSG_REC:
    /* TODO: not implemented yet */
    break;

    default:
#if defined(TCPIP_DHCPV6_V_CANOE_DEBUG)
      CANOE_WRITE_STRING1("Unkown Message Type: %d", MsgType);
#endif
      break;
  }

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of TcpIp_DhcpV6_RxIndication() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) && (TCPIP_SUPPORT_DHCPV6 == STD_ON) */

/* Justification for module-specific MISRA deviations:

    MD_IpV6_0342: MISRA Rule 19.13 / 0342 K&R compilers do not support the ISO glue operator '##'.
      Reason:     For efficient code the use of macros and the operator '##' is necessary.
      Risk:       The used compiler is not ISO compatible. No functional risk.
      Prevention: AUTOSAR is based on ISO compatible compilers.

    MD_IpV6_0881: MISRA Rule 19.12 / 0881 [U] Order of evaluation of multiple ## operators is undefined.
      Reason:     For efficient code the use of macros and the operator '##' is necessary.
      Risk:       Evaluation order of ## operators not relevent in this context. Incorrect evaluation of ## operator would lead to compile error. No functional risk.
      Prevention: Code is tested with multiple compilers.

*/

/**********************************************************************************************************************
 *  END OF FILE: TcpIp_DhcpV6.c
 *********************************************************************************************************************/
