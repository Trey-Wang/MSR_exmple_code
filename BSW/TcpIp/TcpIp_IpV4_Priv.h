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
 *         \file  TcpIp_IpV4_Priv.h
 *        \brief  Implementation of Internet Protocol version 4 (IPv4) - Internal Functions
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

#if !defined (IPV4_PRIV_H)
# define IPV4_PRIV_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_IpV4_Types.h"
#include "TcpIp_IpV4_Cfg.h"
#include "TcpIp_IpV4_Lcfg.h"

#if (IPV4_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif
#include "TcpIp_Cbk.h"
#include "SchM_TcpIp.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define IPV4_INV_SOCK_IDX                  255u

# define IPV4_DUMMY_ASM_NOP

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

#define IPV4_VENTER_CRITICAL_SECTION() SchM_Enter_TcpIp_TCPIP_EXCLUSIVE_AREA_0()
#define IPV4_VLEAVE_CRITICAL_SECTION() SchM_Exit_TcpIp_TCPIP_EXCLUSIVE_AREA_0()

/* ------------------------------------------------- */
/* ------------------------------------------------- */
/* Development Error Tracer */
/* PRQA S 3458 22 */ /* MD_MSR_19.4 */
#if ( IPV4_DEV_ERROR_REPORT == STD_ON )
# define IpV4_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )           { if(!(CONDITION)) {    \
    (void)Det_ReportError( IPV4_MODULE_ID, IPV4_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return; } }
# define IpV4_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )  { if(!(CONDITION)) {    \
    (void)Det_ReportError( IPV4_MODULE_ID, IPV4_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); } }
# define IpV4_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )           { if(!(CONDITION)) {    \
    (void)Det_ReportError( IPV4_MODULE_ID, IPV4_VINSTANCE_ID, (API_ID), (ERROR_CODE)); } }
# define IpV4_ReportDetError( API_ID, ERROR_CODE ) \
    (void)Det_ReportError( IPV4_MODULE_ID, IPV4_VINSTANCE_ID, (API_ID), (ERROR_CODE) )
#else
# define IpV4_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )
# define IpV4_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
# define IpV4_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )
# define IpV4_ReportDetError(                      API_ID, ERROR_CODE )
#endif
  /* IPV4_DEV_ERROR_REPORT */

#if (TCPIP_USE_DUMMY_STATEMENT == STD_ON)
# define IPV4_UNUSED_PARAMETER(Parameter) TCPIP_DUMMY_STATEMENT(Parameter)
#else
# define IPV4_UNUSED_PARAMETER(Parameter)
#endif

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# define IPV4_UNUSED_IPCTRLIDX() 
# define IPV4_IPCTRLIDX IpCtrlIdx
#else
# define IPV4_UNUSED_IPCTRLIDX() IPV4_UNUSED_PARAMETER(IpCtrlIdx)
# define IPV4_IPCTRLIDX 0
#endif

#define IPV4_SET_BIT_U8(VAR, BIT)  IPV4_SET_FLAG_U8((VAR), (1U << (BIT)))
#define IPV4_CLR_BIT_U8(VAR, BIT)  IPV4_CLR_FLAG_U8((VAR), (1U << (BIT)))
#define IPV4_CHK_BIT_U8(VAR, BIT)  IPV4_CHK_FLAG_U8((VAR), (1U << (BIT)))

#define IPV4_SET_FLAG_U8(VAR, FLAG)  ((VAR) |= (uint8)  (FLAG))
#define IPV4_CLR_FLAG_U8(VAR, FLAG)  ((VAR) &= (uint8)((~(uint8_least)(FLAG)) & 0xFFu))
#define IPV4_CHK_FLAG_U8(VAR, FLAG)  (((uint8)((VAR) &  (FLAG))) != 0U)

#define IPV4_HDR_FIELD_CREATE_VER_IHL(HDR_LEN_BYTE)   ((((uint8)IPV4_IP_HEADER_VERSION_V4) << 4) | ((HDR_LEN_BYTE) >> 2))
#define IPV4_HDR_FIELD_GET_VERSION(IP_HDR_PTR)        (uint8)(((IP_HDR_PTR)->Ver_Ihl & 0xF0u) >> 4)
#define IPV4_HDR_FIELD_GET_IHL_BYTE(IP_HDR_PTR)       (uint8)(((IP_HDR_PTR)->Ver_Ihl & 0x0FU) << 2)
#define IPV4_HDR_FIELD_GET_TOTAL_LEN(IP_HDR_PTR)      IPV4_NTOHS((IP_HDR_PTR)->TotLen)

/* CHECK CONDITIONS: */

/* IPV4_E_NOT_INITIALIZED */
#define IPV4_COND_INIT                     ( IPV4_STATE_INIT == IpV4_State )
/* IPV4_E_INV_CTRL_IDX */ /* currently only one controller is supported */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# define IPV4_COND_VALID_CTRLIDX(IpCtrlIdx)   ((IpCtrlIdx) < IpV4_VCfgGetMaxNumCtrls())
#else
# define IPV4_COND_VALID_CTRLIDX(IpCtrlIdx)   (0 == (IpCtrlIdx))
#endif

/* IPV4_E_INV_POINTER */
#define IPV4_COND_VALID_PTR(Pointer)       ( NULL_PTR != (Pointer) )
/* IPV4_E_INV_PARAM */
#define IPV4_COND_VALID_IPADDR(IpAddr)     ( (0 != (IpAddr)) )
#define IPV4_COND_MIN_SIZE(Value, Ref)     ( (Value) >= (Ref) )
#define IPV4_COND_PBUF_LEN(PbufPtr)        ( (NULL_PTR != (PbufPtr)->payload) && \
                                              (0 != (PbufPtr)->totLen) && \
                                              ((PbufPtr)->totLen >= (PbufPtr)->len) )

#define IPV4_COND_VALID_IPADDRID(IpAddrId)        ( (IpAddrId) < IpV4_VCfgGetMaxNumIpAddrs() )
#define IPV4_COND_VALID_OR_ANY_IPADDRID(IpAddrId) ( IPV4_COND_VALID_IPADDRID(IpAddrId) || (IPV4_ADDR_ID_ANY == (IpAddrId)) )

/* implementation limit: max message length is 64kB */
#define IPV4_COND_LEN_MAX_UINT16(Len)      ( (uint32)(Len) <= (uint32)0xFFFF )

/* BSD API checks: */
#define IPV4_COND_PROT_UDP(Prot)           ( IPV4_SOCK_PROT_UDP == (Prot) )
#define IPV4_COND_PROT2(Prot)              ( (IPV4_SOCK_PROT_UDP == (Prot)) || \
                                             (IPV4_SOCK_PROT_TCP == (Prot)) )

/* Byte-Order conversion macros */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  /* Host byte-order is big endian (network byte-order) */
# define IPV4_NTOHS(UINT16_VALUE) (UINT16_VALUE)
# define IPV4_HTONS(UINT16_VALUE) (UINT16_VALUE)
# define IPV4_NTOHL(UINT32_VALUE) (UINT32_VALUE)
# define IPV4_HTONL(UINT32_VALUE) (UINT32_VALUE)

# define IPV4_UINT8_HTONS(UINT8_VALUE) (UINT8_VALUE)
# define IPV4_UINT16_HTONL(UINT16_VALUE) (UINT16_VALUE)

# define IPV4_GET_UINT16_HBO(Buffer, Offset, Variable)    ( (Variable) = (uint16)(((uint16)((Buffer)[(Offset)+0]) << 8) | \
                                                                                    ((uint16)((Buffer)[(Offset)+1]))) )

# define IPV4_GET_UINT32_HBO(Buffer, Offset, Variable)    ( (Variable) = ((uint32)((Buffer)[(Offset)+0]) << 24) | \
                                                                           ((uint32)((Buffer)[(Offset)+1]) << 16) | \
                                                                           ((uint32)((Buffer)[(Offset)+2]) <<  8) | \
                                                                           ((uint32)((Buffer)[(Offset)+3])) )
#else
  /* Host byte-order is little endian */
# define IPV4_NTOHS(UINT16_VALUE) ((uint16)( (((UINT16_VALUE) & 0xFF00u) >> 8) | (((UINT16_VALUE) & 0x00FFu) << 8) ))
# define IPV4_HTONS(UINT16_VALUE) IPV4_NTOHS(UINT16_VALUE)
# define IPV4_NTOHL(UINT32_VALUE) ((uint32)( (uint32)(((UINT32_VALUE) & 0xFF000000u) >> 24) | \
                                             (uint32)(((UINT32_VALUE) & 0x00FF0000u) >>  8) | \
                                             (uint32)(((UINT32_VALUE) & 0x0000FF00u) <<  8) | \
                                             (uint32)(((UINT32_VALUE) & 0x000000FFu) << 24) ))
# define IPV4_HTONL(UINT32_VALUE) IPV4_NTOHL(UINT32_VALUE)

# define IPV4_UINT8_HTONS(UINT8_VALUE)   ((uint16)(  ((uint16)(UINT8_VALUE) << 8)))
# define IPV4_UINT16_HTONL(UINT16_VALUE) ((uint32)( (((uint32)(UINT16_VALUE) & 0x0000FF00u) <<  8) | \
                                                    (((uint32)(UINT16_VALUE) & 0x000000FFu) << 24) ))

# define IPV4_GET_UINT16_HBO(Buffer, Offset, Variable)    ( (Variable) = (uint16)(((uint16)((Buffer)[(Offset)+1]) << 8) | \
                                                                                  ((uint16)((Buffer)[(Offset)+0]))) )

# define IPV4_GET_UINT32_HBO(Buffer, Offset, Variable)    ( (Variable) = ((uint32)((Buffer)[(Offset)+3]) << 24) | \
                                                                         ((uint32)((Buffer)[(Offset)+2]) << 16) | \
                                                                         ((uint32)((Buffer)[(Offset)+1]) <<  8) | \
                                                                         ((uint32)((Buffer)[(Offset)+0])) )

#endif

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# define IPV4_CTRL_VAR(VAR) (VAR)[IpCtrlIdx]
# define IPV4_CTRL_LL_VAR(VAR) (IpV4_Ip_LlAddrState[IpCtrlIdx]->VAR) /* PRQA S 3410 */ /* MD_IpV4_19.10 */
# define IPV4_CTRL_VAR_X(VAR, CTRL_IDX) (VAR)[(CTRL_IDX)]
#else
# define IPV4_CTRL_VAR(VAR) (VAR)[0]
# define IPV4_CTRL_LL_VAR(VAR) (IpV4_Ip_LlAddrState[0]->VAR)         /* PRQA S 3410 */ /* MD_IpV4_19.10 */
# define IPV4_CTRL_VAR_X(VAR, CTRL_IDX) (VAR)[0]
#endif

# define IPV4_ADDR_ID_2_CTRL_IDX(ADDR_ID)  IpV4_Ip_AddrId2CtrlAddrMap[(ADDR_ID)].IpCtrlIdx
# define IPV4_ADDR_ID_2_ADDR_IDX(ADDR_ID)  IpV4_Ip_AddrId2CtrlAddrMap[(ADDR_ID)].IpCtrlAddrIdx

#define IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IP_CTRL_IDX,   ADDR_ID) ((IP_CTRL_IDX)   = IPV4_ADDR_ID_2_CTRL_IDX(ADDR_ID))
#define IPV4_GET_ADDR_IDX_FROM_ADDR_ID(CTRL_ADDR_IDX, ADDR_ID) ((CTRL_ADDR_IDX) = IPV4_ADDR_ID_2_ADDR_IDX(ADDR_ID))

/* currently only one unicast address per controller is supported which has Idx == 0 on every controller */
#define IPV4_UNICAST_ADDR_COUNT    1U
#define IPV4_CTRL_ADDR_IDX_FIRST 0U

#define IPV4_CTRL_ADDR_IDX_IS_UNICAST(CTRL_ADDR_IDX)                        ( (CTRL_ADDR_IDX)      <  IPV4_UNICAST_ADDR_COUNT)

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
# define IPV4_CTRL_ADDR_IDX_IS_MULTICAST(CTRL_ADDR_IDX)                      (((CTRL_ADDR_IDX)      >= IPV4_UNICAST_ADDR_COUNT))
# define IPV4_CTRL_ADDR_IDX_2_MULTICAST_ADDR_IDX(CTRL_ADDR_IDX)      ((uint8)((CTRL_ADDR_IDX)      -  IPV4_UNICAST_ADDR_COUNT))
# define IPV4_CTRL_MULTICAST_ADDR_IDX_2_ADDR_IDX(MULTICAST_ADDR_IDX) ((uint8)((MULTICAST_ADDR_IDX) +  IPV4_UNICAST_ADDR_COUNT))
#endif

#define IPV4_GET_SOCKET_TIME_TO_LIVE(SockIdx)        ((0 != (IPV4_IP_TRANSFER_BLOCK_FLAG_TIME_TO_LIVE     & IpV4_Ip_TransferBlock[SockIdx].Flags)) ? IpV4_Ip_TransferBlock[SockIdx].TimeToLive     : IpV4_Ip_VCfgGetDefaultTimeToLive())
#define IPV4_GET_SOCKET_TYPE_OF_SERVICE(SockIdx)     ((0 != (IPV4_IP_TRANSFER_BLOCK_FLAG_TYPE_OF_SERVICE  & IpV4_Ip_TransferBlock[SockIdx].Flags)) ? IpV4_Ip_TransferBlock[SockIdx].TypeOfService  : IpV4_Ip_VCfgGetDefaultTypeOfService())
#define IPV4_GET_SOCKET_ETHIF_FRAME_PRIO(SockIdx)    ((0 != (IPV4_IP_TRANSFER_BLOCK_FLAG_ETHIF_FRAME_PRIO & IpV4_Ip_TransferBlock[SockIdx].Flags)) ? IpV4_Ip_TransferBlock[SockIdx].EthIfFramePrio : IpV4_Ip_VCfgGetDefaultEthIfFramePrio())
#define IPV4_GET_SOCKET_FORCE_ZERO_SRC_ADDR(SockIdx)  (0 != (IPV4_IP_TRANSFER_BLOCK_FLAG_ZERO_SRC_ADDR    & IpV4_Ip_TransferBlock[SockIdx].Flags))

#define IPV4_LL_ADDR_IS_EQUAL(ADDR_A, ADDR_B) ( ((ADDR_A)[5] == (ADDR_B)[5]) && \
                                                ((ADDR_A)[4] == (ADDR_B)[4]) && \
                                                ((ADDR_A)[3] == (ADDR_B)[3]) && \
                                                ((ADDR_A)[2] == (ADDR_B)[2]) && \
                                                ((ADDR_A)[1] == (ADDR_B)[1]) && \
                                                ((ADDR_A)[0] == (ADDR_B)[0]) )

#define IPV4_LL_ADDR_IS_GROUP(ADDR)           (0 != ((ADDR)[0] & 0x01))
#define IPV4_LL_ADDR_IS_EMPTY(ADDR)           ( (0x00 == (ADDR)[5]) && \
                                                (0x00 == (ADDR)[4]) && \
                                                (0x00 == (ADDR)[3]) && \
                                                (0x00 == (ADDR)[2]) && \
                                                (0x00 == (ADDR)[1]) && \
                                                (0x00 == (ADDR)[0]) )
#define IPV4_LL_ADDR_IS_VALID(ADDR)           ( !( IPV4_LL_ADDR_IS_GROUP((ADDR)) || IPV4_LL_ADDR_IS_EMPTY((ADDR)) ) )

#define IPV4_UINT32_TIMESTAMP_IS_OLDER(TS_A, TS_B) (((TS_B) - (TS_A) - 1) < 0x80000000U)

#define IpV4_VCfgGetMaxNumCtrls()          (IPV4_MAX_NUM_CONTROLLERS)
#define IpV4_VCfgGetMaxNumSocks()          (IPV4_MAX_NUM_SOCKETS)
#define IpV4_VCfgGetMainFctPeriodMsec()    (IPV4_MAIN_FCT_PERIOD_MSEC)
#define IpV4_VCfgGetMaxNumIpAddrs()        (IPV4_MAX_NUM_ADDR_ID)

#define IpV4_Ip_VCfgGetAddrAssignmentPrio(TYPE)      ((uint8)((IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentConfig).AssignmentPrioByType >> (2*(TYPE))) & 0x03u))
#define IpV4_Ip_VCfgGetAddrAssignmentCfgByPrio(PRIO)          (IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentConfig).AssignmentCfgByPrio[(PRIO)-1])
#define IpV4_Ip_VCfgIsAddrAssignmentEnabled(TYPE)             (IpV4_Ip_VCfgGetAddrAssignmentPrio(TYPE) != 0)
#define IpV4_Ip_VCfgGetAddrAssignmentMethodByCfg(CFG)         ((uint8)((CFG) & IPV4_IP_ADDR_ASSIGNMENT_MASK_METHOD))

#define IpV4_Ip_VCfgGetAddrTcpIpAddrId(IpCtrlAddrIdx) (IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[(IpCtrlAddrIdx)].TcpIpAddrId)
#define IpV4_Ip_VCfgGetMulticastAddrCount() (IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrCount))

#define IpV4_Ip_VCfgGetDefaultEthIfFramePrio() (IPV4_CTRL_VAR(IpV4_Ip_DefaultEthIfFramePrio))
#define IpV4_Ip_VCfgGetDefaultTypeOfService()  (IPV4_CTRL_VAR(IpV4_Ip_DefaultTypeOfService))
#define IpV4_Ip_VCfgGetDefaultTimeToLive()     (IPV4_CTRL_VAR(IpV4_Ip_DefaultTimeToLive))

#define IpV4_Ip_VCfgHasFragmentationConfig()     (NULL_PTR != IPV4_CTRL_VAR(IpV4_Fragmentation_Config))
#define IpV4_Ip_VCfgGetReassemblyBufCnt()        (IPV4_CTRL_VAR(IpV4_Fragmentation_Config)->ReassemblyBufferCount)
#define IpV4_Ip_VCfgGetReassemblyBufDescCnt()    (IPV4_CTRL_VAR(IpV4_Fragmentation_Config)->ReassemblyBufferCount)
#define IpV4_Ip_VCfgGetReassemblyBufSize()       (IPV4_CTRL_VAR(IpV4_Fragmentation_Config)->MaxPacketSize)
#define IpV4_Ip_VCfgGetReassemblyTimeoutMs()     (IPV4_CTRL_VAR(IpV4_Fragmentation_Config)->ReassemblyTimeoutMs)
#define IpV4_Ip_VCfgIsReassemblyInOrderEnabled() (IPV4_CTRL_VAR(IpV4_Fragmentation_Config)->InOrderReassembly)
#define IpV4_Ip_VCfgGetMaxTxMsgSize()             IPV4_CTRL_VAR(IpV4_Ip_MaxTxMsgSize)

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
# define IpV4_Ip_VCfgIsReceiveAllMulticastsEnabled() (TRUE == IPV4_CTRL_VAR(IpV4_Ip_ReceiveAllMulticasts))
#endif

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#define IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#define IPV4_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ARP */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

/* CHECK CONDITIONS: */
#define IPV4_ARP_COND_VALID_SOCKIDX(SockIdx)   ( IpV4_VCfgGetMaxNumSocks() > (SockIdx) )  /* IPV4_E_INV_SOCK_IDX */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# define IpV4_Arp_VCfgGetMaxNumAddrRes()           IpV4_Arp_Config[IpCtrlIdx]->ArpTableSize
# define IpV4_Arp_VCfgGetElementTimeoutSec()       IpV4_Arp_Config[IpCtrlIdx]->ArpEntryTimeout
# define IpV4_Arp_VCfgGetRetryIntervalSec()        IpV4_Arp_Config[IpCtrlIdx]->ArpRetryInterval
# define IpV4_Arp_VCfgGetRetryTimeSec()            IpV4_Arp_Config[IpCtrlIdx]->ArpRetryTime
# define Ipv4_Arp_VCfgGetStaticTableSize()         IpV4_Arp_StaticTableSize[IpCtrlIdx]
#else
# define IpV4_Arp_VCfgGetMaxNumAddrRes()           IpV4_Arp_Config[0]->ArpTableSize
# define IpV4_Arp_VCfgGetElementTimeoutSec()       IpV4_Arp_Config[0]->ArpEntryTimeout
# define IpV4_Arp_VCfgGetRetryIntervalSec()        IpV4_Arp_Config[0]->ArpRetryInterval
# define IpV4_Arp_VCfgGetRetryTimeSec()            IpV4_Arp_Config[0]->ArpRetryTime
# define Ipv4_Arp_VCfgGetStaticTableSize()         IpV4_Arp_StaticTableSize[0]
#endif

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */

/* ICMP */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

/* Configuration access (depending on configuration variant)*/

#define IpV4_Icmp_VCfgGetEchoReplyPendLenMax()  (IPV4_ICMP_ECHO_REPLY_PEND_LEN_MAX)
#define IpV4_Icmp_VCfgGetTxSockIdx()            (IPV4_ICMP_TX_SOCKET_IDX)

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */

/* IP */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* IP LL - states */
#define IPV4_IP_LL_STATE_INACTIVE          0x0
#define IPV4_IP_LL_STATE_INIT              0x1
#define IPV4_IP_LL_STATE_WAIT              0x2
#define IPV4_IP_LL_STATE_PROBE             0x3
#define IPV4_IP_LL_STATE_CONFLICT          0x4
#define IPV4_IP_LL_STATE_ANNOUNCE          0x5
#define IPV4_IP_LL_STATE_READY             0x6
/* IP LL Probe - substates */
#define IPV4_IP_LL_PROBE_STATE_CHOOSE      0x1
#define IPV4_IP_LL_PROBE_STATE_PROBE       0x2
#define IPV4_IP_LL_PROBE_STATE_WAIT        0x3
/* IP LL Announce - substates */
#define IPV4_IP_LL_ANN_STATE_PROBE         0x1
#define IPV4_IP_LL_ANN_STATE_WAIT          0x2

#define IPV4_IP_TRANSFER_BLOCK_FLAG_TIME_TO_LIVE      1u
#define IPV4_IP_TRANSFER_BLOCK_FLAG_TYPE_OF_SERVICE   2u
#define IPV4_IP_TRANSFER_BLOCK_FLAG_ETHIF_FRAME_PRIO  4u
#define IPV4_IP_TRANSFER_BLOCK_FLAG_PHYS_DEST_ADDR    8u
#define IPV4_IP_TRANSFER_BLOCK_FLAG_ZERO_SRC_ADDR    16u

#define IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED         0u
#define IPV4_IP_REASSEMBLY_BUF_STATUS_IN_PROGRESS    1u
#define IPV4_IP_REASSEMBLY_BUF_STATUS_FAILED         2u
#define IPV4_IP_REASSEMBLY_BUF_STATUS_COMPLETE       3u

#define IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS  0xFFFFu

#define IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS          2u

#define IPV4_IP_REASSEMBLY_BUF_IDX_INV            0xFFu
#define IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV       0xFFu

#define IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE       0u /* Zero is used because it is not neccessary to store the IP header in the reassembly buffer. */

#define IPV4_IP_REASSEMBLY_FLAG_FIRST_FRAG_RECEIVED  1u
#define IPV4_IP_REASSEMBLY_FLAG_LAST_FRAG_RECEIVED   2u

#define IPV4_IP_TX_BUF_IDX_INV                    0xFFu

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

/* CHECK CONDITIONS: */
#define IPV4_IP_COND_VALID_SOCKIDX(SockIdx)   ( IpV4_Ip_VCfgGetMaxNumSocks() > (SockIdx) )  /* IPV4_E_INV_SOCK_IDX */

#define IPV4_P2V(TYPE) P2VAR(TYPE,   AUTOMATIC, IPV4_APPL_VAR)
#define IPV4_P2C(TYPE) P2CONST(TYPE, AUTOMATIC, IPV4_APPL_DATA)

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# define IpV4_Ip_VCfgGetMtuTx()                       (IpV4_Ip_MtuTx[IpCtrlIdx])
# define IpV4_Ip_VCfgGetMaxNumSocks()                 (IPV4_IP_MAX_NUM_SOCKETS)
#else
# define IpV4_Ip_VCfgGetMtuTx()                       (IpV4_Ip_MtuTx[0])
# define IpV4_Ip_VCfgGetMaxNumSocks()                 (IPV4_IP_MAX_NUM_SOCKETS)
#endif

#define IpV4_Ip_VCfgGetDefaultCtrlIdx()              (IPV4_IP_DEFAULT_CTRL_IDX)
#define IpV4_Ip_VCfgGetMaxTimeAddrReqSec()           (IPV4_IP_MAX_TIME_ADDR_REQ_SEC)

/* LL addr cfg */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# define IpV4_Ip_VCfgGetProbeWaitMinMsec()            (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->ProbeWaitMinMs)
# define IpV4_Ip_VCfgGetProbeWaitMaxMsec()            (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->ProbeWaitMaxMs)
# define IpV4_Ip_VCfgGetProbeNum()                    (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->ProbeNum)
# define IpV4_Ip_VCfgGetProbeMinMsec()                (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->ProbeMinMs)
# define IpV4_Ip_VCfgGetProbeMaxMsec()                (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->ProbeMaxMs)
# define IpV4_Ip_VCfgGetAnnounceWaitMsec()            (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->AnnounceWaitMs)
# define IpV4_Ip_VCfgGetAnnounceNum()                 (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->AnnounceNum)
# define IpV4_Ip_VCfgGetAnnounceIntervalMsec()        (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->AnnounceIntervalMs)
# define IpV4_Ip_VCfgGetMaxConflicts()                (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->MaxConflicts)
# define IpV4_Ip_VCfgGetRateLimitIntervalMsec()       (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->RateLimitIntervalMs)
# define IpV4_Ip_VCfgGetDefendIntervalMsec()          (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->DefendIntervalMs)
# define IpV4_Ip_VCfgGetLinkLocalAddrCalloutFunc()    (IpV4_Ip_LlAddrConfigs[IpCtrlIdx]->AddrCalloutFunc)
#else
# define IpV4_Ip_VCfgGetProbeWaitMinMsec()            (IpV4_Ip_LlAddrConfigs[0]->ProbeWaitMinMs)
# define IpV4_Ip_VCfgGetProbeWaitMaxMsec()            (IpV4_Ip_LlAddrConfigs[0]->ProbeWaitMaxMs)
# define IpV4_Ip_VCfgGetProbeNum()                    (IpV4_Ip_LlAddrConfigs[0]->ProbeNum)
# define IpV4_Ip_VCfgGetProbeMinMsec()                (IpV4_Ip_LlAddrConfigs[0]->ProbeMinMs)
# define IpV4_Ip_VCfgGetProbeMaxMsec()                (IpV4_Ip_LlAddrConfigs[0]->ProbeMaxMs)
# define IpV4_Ip_VCfgGetAnnounceWaitMsec()            (IpV4_Ip_LlAddrConfigs[0]->AnnounceWaitMs)
# define IpV4_Ip_VCfgGetAnnounceNum()                 (IpV4_Ip_LlAddrConfigs[0]->AnnounceNum)
# define IpV4_Ip_VCfgGetAnnounceIntervalMsec()        (IpV4_Ip_LlAddrConfigs[0]->AnnounceIntervalMs)
# define IpV4_Ip_VCfgGetMaxConflicts()                (IpV4_Ip_LlAddrConfigs[0]->MaxConflicts)
# define IpV4_Ip_VCfgGetRateLimitIntervalMsec()       (IpV4_Ip_LlAddrConfigs[0]->RateLimitIntervalMs)
# define IpV4_Ip_VCfgGetDefendIntervalMsec()          (IpV4_Ip_LlAddrConfigs[0]->DefendIntervalMs)
# define IpV4_Ip_VCfgGetLinkLocalAddrCalloutFunc()    (IpV4_Ip_LlAddrConfigs[0]->AddrCalloutFunc)
#endif

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
# if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
#  define IpV4_Ip_VCfgGetNvmAddrVar()                 (IpV4_Ip_Nvm[IpCtrlIdx])
# endif
#else
# if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
#  define IpV4_Ip_VCfgGetFixAddrConfig()              (IpV4_Ip_Nvm[0])
# endif
#endif

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7    */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  IpV4_VAssignPhysAddr
 **********************************************************************************************************************/
/*! \brief      This API assigns a physical address to a pointer
 *  \param[in]  PhysAddrDstPtr        destination pointer for the physical address
 *  \param[in]  PhysAddrSrcPtr        source pointer to the physical address
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_VAssignPhysAddr(
    P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrDstPtr,
    P2CONST(uint8, IPV4_APPL_DATA, IPV4_CONST) PhysAddrSrcPtr );

/***********************************************************************************************************************
 *  IpV4_Ip_VIpAddrAssignmentReadyNotification
 **********************************************************************************************************************/
/*! 
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_VIpAddrAssignmentReadyNotification(
  uint8   IpCtrlIdx, 
  uint8   addrAssignmentMethod,
  boolean addrAssigned);

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
#endif  /* IPV4_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Priv.h
 *********************************************************************************************************************/
