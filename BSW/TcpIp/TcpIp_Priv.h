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
 *         \file  TcpIp_Priv.h
 *        \brief  Implementation of TcpIp Module - Internal Functions
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined TCPIP_PRIV_H  /* PRQA S 0883 */ /* MD_TCPIP_19.15_0883 */
# define TCPIP_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Priv_Types.h"

#include "TcpIp_Cfg.h"
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif
#include "TcpIp_Lcfg.h"
#include "SchM_TcpIp.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define TCPIP_SOCK_CONN_NONE                0u
#define TCPIP_SOCK_CONN_ALLOCATED           1u
#define TCPIP_SOCK_CONN_LISTEN_ENABLED      2u
#if(TCPIP_SUPPORT_TCP == STD_ON)
# define TCPIP_SOCK_CONN_CONNECTED          3u
#endif

#define TCPIP_DUMMY_ASM_NOP    /* empty define */

#define TCPIP_ACTIVE_STATE_OFFLINE          0u  /* module is offline / not initialized */
#define TCPIP_ACTIVE_STATE_IN_SHUTDOWN      1u  /* module is shutting down, all sockets are about to beeing closed */
#define TCPIP_ACTIVE_STATE_ONLINE           2u  /* module is initialized and ready for communication (if an IP address is available) */

/* Ethernet frame types */
#define TCPIP_ETH_FRAME_TYPE_IPV4           0x0800U
#define TCPIP_ETH_FRAME_TYPE_ARP            0x0806U
#define TCPIP_ETH_FRAME_TYPE_IPV6           0x86DDU

/* Private constants of the TCPIP */
#define TCPIP_TX_CONF_LEN_MAX               (0xFFFFU)

#ifdef TCPIP_PROCESSOR_CANOEEMU
  #define TCPIP_VENABLE_CANOE_WRITE_STRING            STD_OFF
#else
  #define TCPIP_VENABLE_CANOE_WRITE_STRING            STD_OFF
#endif

#define TCPIP_API_ID_V_INTERNAL_FUNCTION              0xFFu

#define TCPIP_DHCPV4_MULTIPLE_CONFIG_SETS STD_ON
#define TCPIP_DHCPV4_MULTIPLE_INSTANCES STD_ON

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

#ifdef TCPIP_PROCESSOR_CANOEEMU
#if (TCPIP_VENABLE_CANOE_WRITE_STRING == STD_ON)
  #include "stdio.h"
  #define CANOE_WRITE_STRING(Txt)                 CANoeAPI_WriteString((Txt));
  #define CANOE_WRITE_STRING_1(Txt, P1)           \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1); CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
  #define CANOE_WRITE_STRING_2(Txt, P1, P2)       \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1, P2); \
    CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
  #define CANOE_WRITE_STRING_3(Txt, P1, P2, P3)   \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1, P2, P3); \
    CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
  #define CANOE_WRITE_STRING_4(Txt, P1, P2, P3, P4)   \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1, P2, P3, P4); \
    CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
#else
  #define CANOE_WRITE_STRING(txt)
  #define CANOE_WRITE_STRING_1(txt, p1)
  #define CANOE_WRITE_STRING_2(txt, p1, p2)
  #define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
  #define CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)
#endif
  /* X_VENABLE_CANOE_WRITE_STRING */
#else
  #define CANOE_WRITE_STRING(txt)
  #define CANOE_WRITE_STRING_1(txt, p1)
  #define CANOE_WRITE_STRING_2(txt, p1, p2)
  #define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
  #define CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)
#endif

#define TCPIP_VENTER_CRITICAL_SECTION() SchM_Enter_TcpIp_TCPIP_EXCLUSIVE_AREA_0()
#define TCPIP_VLEAVE_CRITICAL_SECTION() SchM_Exit_TcpIp_TCPIP_EXCLUSIVE_AREA_0()

#define TCPIP_SOCKHND2SOCKIDX(SockHnd)  ((TcpIp_SockIdxType)(SockHnd))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_SOCKHND2TCPIDX(SockHnd)   ((TcpIp_TcpIdxType)((TcpIp_SockIdxType)(SockHnd) - ((TcpIp_SockIdxType)(TcpIp_VCfgGetMaxNumUdpSocks()))))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_SOCKIDX2TCPIDX(SockIdx)   ((TcpIp_TcpIdxType)((SockIdx) - (TcpIp_VCfgGetMaxNumUdpSocks())))  /* PRQA S 3453 */ /* MD_MSR_19.7 */  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */

#define TCPIP_SOCKIDX2SOCKHND(SockIdx)  ((TcpIp_SockHndType)(SockIdx))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_TCPIDX2SOCKHND(TcpIdx)    ((TcpIp_SockHndType)((TcpIdx) + (TcpIp_VCfgGetMaxNumUdpSocks())))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_TCPIDX2SOCKIDX(TcpIdx)    ((TcpIp_SockIdxType)((TcpIdx) + (TcpIp_VCfgGetMaxNumUdpSocks())))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_COND_VALID_TCPSOCKIDX(SockIdx)  ( (TcpIp_VCfgGetMaxNumSocks() > (SockIdx)) && \
                                              (TcpIp_VCfgGetMaxNumUdpSocks() <= (SockIdx)) )  /* TCPIP_E_INV_SOCK_ID */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_COND_VALID_UDPSOCKIDX(SockIdx)  (TcpIp_VCfgGetMaxNumUdpSocks() > (SockIdx))  /* TCPIP_E_INV_SOCK_ID */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_COND_INIT                     ( TCPIP_STATE_INIT == TcpIp_State )  /* TCPIP_E_NOTINIT */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_SOCKET_ID_TO_TCP_IDX(SOCKET_ID)  ((SOCKET_ID)  - TcpIp_VCfgGetMaxNumUdpSocks())
#define TCPIP_TCP_IDX_TO_SOCKET_ID(TCP_IDX)    ((TCP_IDX)    + TcpIp_VCfgGetMaxNumUdpSocks())
#define TCPIP_SOCKET_ID_IS_TCP(SOCKET_ID)      ((SOCKET_ID) >= TcpIp_VCfgGetMaxNumUdpSocks()) /*! Checks if a valid SocketId references a TCP socket. (Use TCPIP_COND_VALID_SOCKET_ID() in order to check if the socket ID is valid at all.) */

#define TCPIP_SOCKET_ID_TO_UDP_IDX(SOCKET_ID)   (SOCKET_ID)
#define TCPIP_UDP_IDX_TO_SOCKET_ID(UDP_IDX)     (UDP_IDX)
#define TCPIP_SOCKET_ID_IS_UDP(SOCKET_ID)      ((SOCKET_ID) <  TcpIp_VCfgGetMaxNumUdpSocks())  /*! Checks if a valid SocketId references a UDP socket.  */

#define TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LOCAL_ADDR_ID)       (LOCAL_ADDR_ID)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LOCAL_ADDR_ID)      (TcpIp_LocalAddrIdType)((LOCAL_ADDR_ID) - (TcpIp_LocalAddrIdType)TCPIP_MAX_NUM_IPV4_ADDR_IDS)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LOCAL_ADDR_ID)      ((TcpIp_LocalAddrIdType)(TCPIP_LOCAL_ADDR_IS_AF_INET(LOCAL_ADDR_ID) ? TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LOCAL_ADDR_ID) : TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LOCAL_ADDR_ID)))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_LOCAL_ADDR_IS_AF_INET(LOCAL_ADDR_ID)        ((LOCAL_ADDR_ID) <  TCPIP_MAX_NUM_IPV4_ADDR_IDS)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_LOCAL_ADDR_IS_AF_INET6(LOCAL_ADDR_ID)      (((LOCAL_ADDR_ID) >= TCPIP_MAX_NUM_IPV4_ADDR_IDS) && TCPIP_COND_VALID_LOCALADDRID(LOCAL_ADDR_ID))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_GET_LOCAL_ADDR_FAMILY(LOCAL_ADDR_ID)       (TCPIP_LOCAL_ADDR_IS_AF_INET(LOCAL_ADDR_ID) ? IPBASE_AF_INET : IPBASE_AF_INET6)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_REQUEST_NEXT_DYNAMIC_PORT                  ((uint16)0)
#define TCPIP_GET_NEXT_DYNAMIC_PORT(COUNTER_VAR)         ((uint16)((COUNTER_VAR)++ | 0xC000u))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* TcpIp TxBuf */
#define TcpIp_IncTxBufIdx(SockIdx, Idx, Num)  ((Idx) = (((Idx) +(Num)) % (TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx)))))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TcpIp_DecTxBufIdx(SockIdx, Idx, Num)  (Idx) = (((Idx) + (TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx)))) - (Num)) % (TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx)))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if !defined(TCPIP_VENABLE_INTERNAL_DET_CHECKS)
# define TCPIP_VENABLE_INTERNAL_DET_CHECKS STD_OFF
#endif

#if (TCPIP_ENABLE_DLISTS == STD_ON)

# define TCPIP_DLIST_GET_NEXT(NodeIdx, DList) ((DList).DescPtr->NodePtr[(NodeIdx)].NextIdx)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define TCPIP_DLIST_GET_PREV(NodeIdx, DList) ((DList).DescPtr->NodePtr[(NodeIdx)].PrevIdx)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define TCPIP_DLIST_SET_NEXT(NodeIdx, DList) ((NodeIdx) = TCPIP_DLIST_GET_NEXT(NodeIdx, DList))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define TCPIP_DLIST_SET_PREV(NodeIdx, DList) ((NodeIdx) = TCPIP_DLIST_GET_PREV(NodeIdx, DList))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define TCPIP_DLIST_IS_EMPTY(DList)           (TCPIP_DLIST_END_IDX == (DList).FirstIdx)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define TCPIP_DLIST_END_IDX 0xFF

#endif

/* TCP */

/* RAM array access macros: */

/* TxRstQueue */
#define TcpIp_Tcp_IncTxRstQueueIdx(Idx)  ((Idx) = (uint8)(((Idx) +1) % (TcpIp_Tcp_VCfgGetTxRstQueueSize())))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* RxBuffers */
#define TcpIp_Tcp_GetRxBuffersElemPtr(TcpIdx, Idx)  (&(TcpIp_Tcp_RxBuffer[TcpIp_Tcp_Socket_AssignedRxBufferIndex[TcpIdx]])[Idx])  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* RxPreBufArray */
#define TcpIp_Tcp_GetRxPreBufArrayElemPtr(TcpIdx, Idx)  (&(TcpIp_Tcp_RxPreBufArray[ ((TcpIdx) * (TcpIp_Tcp_VCfgGetMaxNumOooSegments())) + (Idx)]))  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TcpIp_GetTxBuffersElemPtr(SockIdx, Idx)    (&(TcpIp_Tcp_TxBuffer[TcpIp_Tcp_Socket_AssignedTxBufferIndex[TCPIP_SOCKIDX2TCPIDX(SockIdx)]])[Idx])  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Development Error Tracer */
#if ( TCPIP_DEV_ERROR_REPORT == STD_ON )
# define TcpIp_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )           { if(!(CONDITION)) {    \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return; } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TcpIp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )  { if(!(CONDITION)) {    \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TcpIp_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )           { if(!(CONDITION)) {    \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TcpIp_CallDetReportError( API_ID, ERROR_CODE )  { \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
#else
# define TcpIp_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )
# define TcpIp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
# define TcpIp_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )
# define TcpIp_CallDetReportError(                  API_ID, ERROR_CODE )
#endif  /* TCPIP_DEV_ERROR_REPORT */

/* CHECK CONDITIONS to be used in ASSERT and DET macros: */

#define TCPIP_COND_VALID_SOCKET_ID(SocketId)   ((TcpIp_SocketIdType)(TcpIp_VCfgGetMaxNumSocks()) > (SocketId))  /* TCPIP_E_INV_SOCK_ID */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_COND_MIN_SIZE(Value, Ref)     ( (Value) >= (Ref) )                     /* TCPIP_E_INV_ARG */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_COND_PBUF_LEN(PbufPtr)        ( (NULL_PTR != (PbufPtr)->payload) && \
                                              (0 != (PbufPtr)->totLen) && \
                                              ((PbufPtr)->totLen >= (PbufPtr)->len) )      /* TCPIP_E_INV_ARG */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_COND_VALID_LOCALADDRID(LocalAddrId)        ( TcpIp_VCfgGetMaxLocalAddrId() >= (LocalAddrId) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TCPIP_COND_VALID_OR_ANY_LOCALADDRID(LocalAddrId) ( (TCPIP_LOCALADDRID_ANY == (LocalAddrId)) || TCPIP_COND_VALID_LOCALADDRID(LocalAddrId) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_IPADDR_ASSIGNMENT_MAX_VALUE  TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
/* IPV4 and IPV6 are enabled */
#define TCPIP_COND_VALID_ADDR_FAM(SockAddrPtr)   ( (((const IpBase_SockAddrType*)(SockAddrPtr))->sa_family == IPBASE_AF_INET6) || \
                                                   (((const IpBase_SockAddrType*)(SockAddrPtr))->sa_family == IPBASE_AF_INET) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# else
/* only IPV4 is enabled */
#define TCPIP_COND_VALID_ADDR_FAM(SockAddrPtr)   ( ((const IpBase_SockAddrType*)(SockAddrPtr))->sa_family == IPBASE_AF_INET )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif
#else
/* only IPV6 is enabled */
#define TCPIP_COND_VALID_ADDR_FAM(SockAddrPtr)   ( ((const IpBase_SockAddrType*)(SockAddrPtr))->sa_family == IPBASE_AF_INET6 )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

/* implementation limit: max message length is 64kB */
#define TCPIP_COND_LEN_MAX_UINT16(Len)      ( (uint32)(Len) <= (uint32)0xFFFF )        /* TCPIP_E_INV_ARG */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TCPIP_COND_VALID_DOMAIN(Domain)      ( (IPBASE_AF_INET == (Domain)) || \
                                               (IPBASE_AF_INET6 == (Domain)))          /* TCPIP_E_INV_ARG */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#if (TCPIP_SUPPORT_TCP == STD_ON)
#define TCPIP_COND_VALID_PROT(Prot)          ( (TCPIP_SOCK_PROT_UDP == (Prot)) || \
                                               (TCPIP_SOCK_PROT_TCP == (Prot)) )       /* TCPIP_E_INV_ARG */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TCPIP_COND_VALID_PROT(Prot)          ( TCPIP_SOCK_PROT_UDP == (Prot) )         /* TCPIP_E_INV_ARG */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#define TCPIP_TCP_COND_VALID_TCPIDX(TcpIdx)   ( (TCPIP_TCP_INV_IDX != (TcpIdx)) && \
                                          (TcpIp_VCfgGetMaxNumTcpSocks() > (TcpIdx)) )  /* TCPIP_E_INV_SOCK_ID */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if (TCPIP_USE_DUMMY_STATEMENT == STD_ON)
# define TCPIP_UNUSED_PARAMETER(Parameter) TCPIP_DUMMY_STATEMENT(Parameter)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define TCPIP_UNUSED_PARAMETER(Parameter)
#endif

#define TCPIP_ASSERT(CONDITION)                TCPIP_ASSERT_MSG(CONDITION, NULL_PTR)
#define TCPIP_ASSERT_RET(CONDITION)            TCPIP_ASSERT_MSG_RET(CONDITION, NULL_PTR)
#define TCPIP_ASSERT_RETVAL(CONDITION, RETVAL) TCPIP_ASSERT_MSG_RETVAL(CONDITION, NULL_PTR, RETVAL)
#define TCPIP_ASSERT_UNREACHABLE()             TCPIP_ASSERT_UNREACHABLE_MSG("This statement must not be reached!")
#if defined(TCPIP_ASSERT_HANDLER)
# define TCPIP_ASSERT_MSG_RETVAL(CONDITION, MSG, RETVAL) (void)TCPIP_ASSERT_HANDLER(CONDITION, MSG); if (!(CONDITION)) return (RETVAL)
# define TCPIP_ASSERT_MSG_RET(CONDITION, MSG)            (void)TCPIP_ASSERT_HANDLER(CONDITION, MSG); if (!(CONDITION)) return
# define TCPIP_ASSERT_MSG(CONDITION, MSG)                (void)TCPIP_ASSERT_HANDLER(CONDITION, MSG)
# define TCPIP_ASSERT_UNREACHABLE_MSG(MSG)               (void)TCPIP_ASSERT_HANDLER(FALSE, MSG)
#else
# define TCPIP_ASSERT_MSG_RETVAL(CONDITION, MSG, RETVAL)
# define TCPIP_ASSERT_MSG_RET(CONDITION, MSG)
# define TCPIP_ASSERT_MSG(CONDITION, MSG)     
# define TCPIP_ASSERT_UNREACHABLE_MSG(MSG)
#endif

#define TCPIP_CYCLES_PER_SECOND             (1000 / TCPIP_MAIN_FCT_PERIOD_MSEC)
#define TCPIP_MAX_SEC_IN_UINT32_CYCLE_VAR   (0xFFFFFFFFu / TCPIP_CYCLES_PER_SECOND)

/* ------------------------------------------------- */
/* ------------------------------------------------- */
/* Configuration access (depending on configuration variant)*/
#define TcpIp_VCfgGetMaxNumUdpSocks()       (TCPIP_MAX_NUM_UDP_SOCKETS)
#define TcpIp_VCfgGetMaxNumTcpSocks()       (TCPIP_MAX_NUM_TCP_SOCKETS)
#define TcpIp_VCfgGetMaxNumSocks()          (TCPIP_MAX_NUM_SOCKETS)
#define TcpIp_VCfgGetMainFctPeriodMsec()    (TCPIP_MAIN_FCT_PERIOD_MSEC)
#define TcpIp_VCfgGetMaxLocalAddrId()       (TCPIP_MAX_LOCAL_ADDR_ID)

#define TcpIp_VCfgGetSockMode()             (TcpIp_Cfg_SockMode)
#define TcpIp_VCfgGetUserIdx()              (TcpIp_SockUserIdx)  /* (TcpIp_CfgUserIdx) */
#define TcpIp_VCfgGetTxBufSizeByte(TcpIdx)        (TcpIp_Tcp_TxBufferSize[TcpIp_Tcp_Socket_AssignedTxBufferIndex[TcpIdx]])  /* (TcpIp_TxBufSizeByte) */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIp_VCfgGetTxBufSizeOffset()      (TcpIp_TxBufSizeOffset)

#define TcpIp_VCfgGetNumSocketOwners()      (TCPIP_NUM_SOCKET_OWNERS)

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
#define TcpIp_VCfgGetUdpTxReqListNum()      (TCPIP_UDP_TX_REQ_LIST_NUM)
#endif

#define TcpIp_VCfgGetLocalIpAssignmentChgFuncCount() (TCPIP_LOCAL_IP_ADDR_ASSIGNMENT_CHG_CBK_CNT)
#define TcpIp_VCfgGetPhysAddrTableChgFuncCount()     (TCPIP_PHYS_ADDR_TABLE_CHG_FUNCTION_CNT)

/* DHCP */
/* Configuration access (depending on configuration variant)*/

#define TcpIp_DhcpV4_VCfgGetSockHnd()                       (IPV4_DHCP_TX_SOCKET_IDX)

#if (TCPIP_DHCPV4_MULTIPLE_CONFIG_SETS == STD_ON)
# define TcpIp_DhcpV4_VCfgGetInitWaitMinMsec()              (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->InitWaitMin)
# define TcpIp_DhcpV4_VCfgGetInitWaitMaxMsec()              (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->InitWaitMax)
# define TcpIp_DhcpV4_VCfgGetDiscoverMaxNum()               (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->DiscoverMaxNum)
# define TcpIp_DhcpV4_VCfgGetRequestMaxNum()                (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->RequestMaxNum)
# define TcpIp_DhcpV4_VCfgGetDiscoverMinIntervalSec()       (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->DiscoverIntervalMin)
# define TcpIp_DhcpV4_VCfgGetDiscoverMaxIntervalSec()       (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->DiscoverIntervalMax)
# define TcpIp_DhcpV4_VCfgGetRequestIntervalSec()           (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->RequestInterval)
# define TcpIp_DhcpV4_VCfgGetRenewMinIntervalSec()          (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->RenewIntervalMin)
# define TcpIp_DhcpV4_VCfgGetRebindMinIntervalSec()         (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->RebindIntervalMin)
# define TcpIp_DhcpV4_VCfgGetHostNameLenMax()               (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->HostNameLenMax)
# define TcpIp_DhcpV4_VCfgGetRestartOnFail()                (TcpIp_DhcpV4_ConfigSets[IpCtrlIdx]->RestartOnFail)
#else
# define TcpIp_DhcpV4_VCfgGetInitWaitMinMsec()              (TCPIP_DHCPV4_INIT_WAIT_MIN_MSEC)
# define TcpIp_DhcpV4_VCfgGetInitWaitMaxMsec()              (TCPIP_DHCPV4_INIT_WAIT_MAX_MSEC)
# define TcpIp_DhcpV4_VCfgGetDiscoverMaxNum()               (TCPIP_DHCPV4_DISCOVER_MAX_NUM)
# define TcpIp_DhcpV4_VCfgGetRequestMaxNum()                (TCPIP_DHCPV4_REQUEST_MAX_NUM)
# define TcpIp_DhcpV4_VCfgGetDiscoverMinIntervalSec()       (TCPIP_DHCPV4_DISCOVER_MIN_INTERVAL_SEC)
# define TcpIp_DhcpV4_VCfgGetDiscoverMaxIntervalSec()       (TCPIP_DHCPV4_DISCOVER_MAX_INTERVAL_SEC)
# define TcpIp_DhcpV4_VCfgGetRequestIntervalSec()           (TCPIP_DHCPV4_REQUEST_INTERVAL_SEC)
# define TcpIp_DhcpV4_VCfgGetRenewMinIntervalSec()          (TCPIP_DHCPV4_RENEW_MIN_INTERVAL_SEC)
# define TcpIp_DhcpV4_VCfgGetRebindMinIntervalSec()         (TCPIP_DHCPV4_REBIND_MIN_INTERVAL_SEC)
# define TcpIp_DhcpV4_VCfgGetHostNameLenMax()               (TCPIP_DHCPV4_HOSTNAME_LEN_MAX)
# define TcpIp_DhcpV4_VCfgGetRestartOnFail()                (TCPIP_DHCPV4_RESTART_ON_FAIL)
#endif

/* TCP */
/* Configuration access (depending on configuration variant): */
#define TcpIp_Tcp_VCfgGetSocketIdxRstTx()               (TCPIP_TCP_SOCKET_IDX_RST_TX)
#define TcpIp_Tcp_VCfgGetTxRetrQueueSize()              (TCPIP_TCP_TX_RETR_QUEUE_SIZE)
#define TcpIp_Tcp_VCfgGetTxRstQueueSize()               (TCPIP_TCP_TX_RST_QUEUE_SIZE)
#define TcpIp_Tcp_VCfgGetMaxWindowSize(RxBufIdx)        (TcpIp_Tcp_RxBufferSize[RxBufIdx])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIp_Tcp_VCfgGetMaxTxMsgSizeByte()             (TCPIP_TCP_MAX_TX_MSG_SIZE_BYTE)   /* EthPayload (1500) - IPv4Header (20) - TcpHeader (20) */
#define TcpIp_Tcp_VCfgGetMaxRxMsgSizeByte()             (TCPIP_TCP_MAX_RX_MSG_SIZE_BYTE)
#define TcpIp_Tcp_VCfgGetTxRetryIntMinCycles()          (TCPIP_TCP_TX_RETRY_INT_MIN_CYCLES)  /* retry interval */
#define TcpIp_Tcp_VCfgGetTxRetryIntMaxCycles()          (TCPIP_TCP_TX_RETRY_INT_MAX_CYCLES)  /* retry interval */
#define TcpIp_Tcp_VCfgGet2MslTimeoutCycles()            (TCPIP_TCP_2MSL_TIMEOUT_CYCLES)
#define TcpIp_Tcp_VCfgGetKeepAliveTimeoutDefCycles()    (TCPIP_TCP_KEEP_ALIVE_TIMEOUT_DEF_CYCLES)
#define TcpIp_Tcp_VCfgGetKeepAliveMaxProbesDef()        (TCPIP_TCP_KEEP_ALIVE_MAX_PROBE_DEF)
#define TcpIp_Tcp_VCfgGetKeepAliveIntervalDefCycles()   (TCPIP_TCP_KEEP_ALIVE_INTERVAL_DEF_CYCLES)
#define TcpIp_Tcp_VCfgGetNagleTimeoutCycles()           (TCPIP_TCP_NAGLE_TIMEOUT_CYCLES)
#define TcpIp_Tcp_VCfgGetUserTimeoutMinCycles()         (TCPIP_TCP_USER_TIMEOUT_MIN_CYCLES)
#define TcpIp_Tcp_VCfgGetUserTimeoutMaxCycles()         (TCPIP_TCP_USER_TIMEOUT_MAX_CYCLES)
#define TcpIp_Tcp_VCfgGetUserTimeoutDefCycles()         (TCPIP_TCP_USER_TIMEOUT_DEF_CYCLES)
#define TcpIp_Tcp_VCfgGetOooAvgNumSeg()                 (TCPIP_TCP_OOO_AVG_NUM_SEG_PER_SOCK)

/* UDP */
/* Configuration access (depending on configuration variant)*/

#define TcpIp_Udp_VCfgGetTxReqListSize(ListIdx)         (TcpIp_Udp_TxReqListSize[ListIdx])  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ----- */
#define TCPIP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(uint8, TCPIP_VAR_NOINIT) TcpIp_ActiveState;

#if ( TCPIP_VENABLE_CANOE_WRITE_STRING == STD_ON )
extern VAR(sint8, TCPIP_VAR_NOINIT)                TcpIp_CanoeWriteStr[];
#endif
  /* TCPIP_VENABLE_CANOE_WRITE_STRING */

#define TCPIP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ----- */
#define TCPIP_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (TCPIP_SUPPORT_TCP == STD_ON)
extern VAR(uint16, TCPIP_VAR_NOINIT)               TcpIp_Tcp_DynamicPortCount;
# endif
extern VAR(uint16, TCPIP_VAR_NOINIT)               TcpIp_Udp_DynamicPortCount;

#define TCPIP_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ----- */
#define TCPIP_START_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
extern CONST(IpBase_AddrIn6Type, TCPIP_CONST) TcpIp_Ipv6NullAddr;  /* PRQA S 0759 */ /* MD_TCPIP_18.4_0759 */
#endif

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if !defined (STATIC)
# define STATIC static
#endif

#if !defined (TCPIP_LOCAL)
# define TCPIP_LOCAL static
#endif

#if !defined (TCPIP_LOCAL_INLINE)
# define TCPIP_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * MISCELLANEOUS SERVICE FUNCTIONS - START
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_VPreconfigSocket
 **********************************************************************************************************************/
/*! \brief          Pre-configures a (TCP or UDP) socket
 *  \details        Called in the context of the initialization of the TcpIp.
 *  \param[in]      SockIdx               socket index
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE)  TcpIp_VPreconfigSocket(TcpIp_SockIdxType SockIdx);

/***********************************************************************************************************************
 *  TcpIp_VRxUdpSockIdxIdent
 **********************************************************************************************************************/
/*! \brief          Identifies an incoming UDP socket.
 *  \param[in]      RxSockLocAddrPtr:   Local address of the received packet.
 *  \param[in]      LocalAddrId:        Local address id.
 *  \return         Socket index of the given socket address in case the UDP socket was identified.
 *  \return         TCPIP_INV_SOCK_IDX in case the UDP socket could NOT be identified.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(TcpIp_SockIdxType, TCPIP_CODE) TcpIp_VRxUdpSockIdxIdent(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockLocAddrPtr,
    TcpIp_LocalAddrIdType                                LocalAddrId);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VRxTcpSockIdxIdent
 **********************************************************************************************************************/
/*! \brief          Identifies an incoming TCP socket.
 *  \param[in]      RxSockRemAddrPtr:   Remote address of the received packet.
 *  \param[in]      RxSockLocAddrPtr:   Local address of the received packet.
 *  \param[in]      LocalAddrId:        Local address id.
 *  \return         Socket index of the given socket address in case the TCP socket was identified.
 *  \return         TCPIP_INV_SOCK_IDX in case the TCP socket could NOT be identified.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(TcpIp_SockIdxType, TCPIP_CODE) TcpIp_VRxTcpSockIdxIdent(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockLocAddrPtr,
    TcpIp_LocalAddrIdType                                LocalAddrId);
#endif

/***********************************************************************************************************************
 *  TcpIp_SetLastSockError
 **********************************************************************************************************************/
/*! \brief         Sets the last socket error.
 *  \details       -
 *  \param[in]     Error                  error code
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SetLastSockError(
  uint16 Error );

/***********************************************************************************************************************
 *  TcpIp_VInitSockets
 **********************************************************************************************************************/
/*! \brief         Initializes all sockets.
 *  \details       -
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE)TcpIp_VInitSockets(void);

/***********************************************************************************************************************
 *  TcpIp_VInitSocket
 **********************************************************************************************************************/
/*! \brief         Initializes the admin data of one single socket.
 *  \details       Called in the context of the initialization of the TcpIp.
 *  \param[in]     SockIdx                socket index
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VInitSocket(TcpIp_SockIdxType SockIdx);

/***********************************************************************************************************************
 *  TcpIp_VInitTxSocket
 **********************************************************************************************************************/
/*! \brief         Inits some variables of a tx socket
 *  \details       Called in the context of the initialization of the TcpIp.
 *  \param[in]     SockIdx                socket index
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VInitTxSocket(TcpIp_SockIdxType SockIdx);

/***********************************************************************************************************************
 *  TcpIp_VDelSockAddrMapping
 **********************************************************************************************************************/
/*! \brief         Deletes a socket address mapping.
 *  \details       Called in the context of the initialization of the TcpIp.
 *  \param[in]     SockIdx                socket index
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VDelSockAddrMapping(TcpIp_SockIdxType SockIdx);

/***********************************************************************************************************************
 *  TcpIp_VSockIpAddrIsEqual
 **********************************************************************************************************************/
/*! \brief         Checks if the IP addresses of two given sockets are equal.
 *  \details       -
 *  \param[inout]  SockAPtr             target socket address
 *  \param[in]     SockBPtr             source socket address
 *  \return        TRUE                 IP addr is equal
 *  \return        FALSE                IP addr is not equal
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VSockIpAddrIsEqual(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockBPtr );

/***********************************************************************************************************************
 *  TcpIp_VSockAddrIpAndPortIsEqual
 **********************************************************************************************************************/
/*! \brief         Checks if IP addresses and ports of two given sockets are equal.
 *  \details       -
 *  \param[inout]  SockAPtr             target socket address
 *  \param[in]     SockBPtr             source socket address
 *  \return        TRUE                 IP addr and port are equal
 *  \return        FALSE                IP addr and port are not equal
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VSockAddrIpAndPortIsEqual(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockBPtr );

/***********************************************************************************************************************
 *  TcpIp_VNetAddrIsUnicast
 **********************************************************************************************************************/
/*! \brief         Checks if the given SockAddr is a unicast address.
 *  \details       -
 *  \param[in]     SockAddrPtr      Socket address containing the IP address
 *  \return        TRUE             The socket address contains a unicast IP address
 *  \return        FALSE            The socket address does not contain a unicast IP address
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VNetAddrIsUnicast(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAddrPtr );

/***********************************************************************************************************************
 *  TcpIp_VCalcPseudoHdrAndChecksum
 **********************************************************************************************************************/
/*! \brief         Calculates IP pseudo header and payload checksum and returns a partial or finalized result.
 *  \details       -
 *  \param[in]     RxSockRemAddrPtr      pointer to remote address value
 *  \param[in]     RxSockLocAddrPtr      pointer to local address value
 *  \param[in]     DataPtr               pointer to payload data
 *  \param[in]     DataLen               length payload data in bytes
 *  \param[in]     Protocol              protocol identifier
 *  \return        DataPtr != NULL_PTR: Checksum value finalized to uint16 range.
 *  \return        DataPtr == NULL_PTR: Partial uint32 checksum value that can be passed to IpBase_TcpIpChecksumAdd()
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(uint32, TCPIP_CODE) TcpIp_VCalcPseudoHdrAndChecksum(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockLocAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) DataLen,
    CONST(uint8, AUTOMATIC) Protocol );

/**********************************************************************************************************************
 * TcpIp_CmpLLAddr
 **********************************************************************************************************************/
/*! \brief          This method compares two Link Layer addresses (Ethernet physical addresses).
 *  \details        -
 *  \param[in]      LLAddr1Ptr      pointer to first physical layer address to compare.
 *  \param[in]      LLAddr2Ptr      pointer to second physical layer address to compare.
 *  \return         TRUE in case the addresses are equal, FALSE otherwise.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_CmpLLAddr(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) LLAddr1Ptr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) LLAddr2Ptr);

/***********************************************************************************************************************
 *  TcpIp_VIpTransmit
 **********************************************************************************************************************/
/*! \brief          Wraps the call to transmit function of IP layer. (IPV4/IPv6)
 *  \details        -
 *  \param[in]      IpTxReqDescrPtr         Pointer to TX request descriptor that was used at IpV4_Ip_ProvideTxBuffer() call.
 *  \param[in]      IpPayloadLen            Number of bytes to transmit.
 *                                          This implementations allows two special cases:
 *                                          1: The value of ipPayloadLen may be larger than requested by IpV4_Ip_ProvideTxBuffer() if
 *                                             there was enough space left in the last provided buffer fragment.
 *                                          2: The value of ipPayloadLen may be less than requested by IpV4_ProvideTxBuffer().
 *                                             Even if there were multiple buffers provided, only the required number of fragments will be sent.
 *                                             Previously provided but unused buffers will be released by this function.
 *  \param[in]      IpProtocol              IP protocol value. (TCP, UDP, ICMP)
 *  \param[in]      ReqTxConfirmation       Indicates whether TX confirmation is required from lower layer (EthIf).
 *  \param[in]      UlTxReqIdx              Upper layer TX request identifier.
 *  \param[in]      RetryOnPhysAddrMiss     Do not release ethernet buffer if physical address could not be resolved.
 *  \param[in]      DestSockAddrPtr         Destination IP address.
 *  \return         TCPIP_OK                Transmission triggered.
 *  \return         TCPIP_E_NOT_OK          Transmit failed.
 *  \return         TCPIP_E_PHYS_ADDR_MISS  Transmit failed due to ARP cache miss.
 *  \pre            A previous call of IpVx_ProvideTxBuffer() must have been successful.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_VIpTransmit(
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr,
  uint16                                     IpPayloadLen,
  uint8                                      IpProtocol,
  boolean                                    ReqTxConfirmation,
  uint8                                      UlTxReqIdx,
  boolean                                    RetryOnPhysAddrMiss,
  TCPIP_P2C(IpBase_SockAddrType)             DestSockAddrPtr);

/***********************************************************************************************************************
 *  TcpIp_VIpTransmitCancel
 **********************************************************************************************************************/
/*! \brief          Cancels the transmit after a preceding call to ProvideTxBuffer.
 *  \details        -
 *  \param[in,out]  IpTxReqDescrPtr       IP TX request descriptor.
 *  \param[in]      Domain                IP address family/domain. (INET, INET6)
 *  \param[in]      UlTxReqTabIdx         TX request index.
 *  \param[in]      IpProtocol            IP protocol (UDP, TCP, ICMP).
 *  \pre            A previous call of IpVx_ProvideTxBuffer() must have been successful.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VIpTransmitCancel(
    TCPIP_P2V(TcpIp_IpTxRequestDescriptorType) IpTxReqDescrPtr,
    TcpIp_DomainType                           Domain,
    uint8                                      UlTxReqTabIdx,
    uint8                                      IpProtocol);

/***********************************************************************************************************************
 *  TcpIp_VRstCtrl
 **********************************************************************************************************************/
/*! \brief         Resets some variables for one controller
 *  \details       Called in the context of transceiver link loss, or IP address expiration.
 *  \param[in]     LocalAddrId      IP address identifier of the address that triggered the reset.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VRstCtrl(TcpIp_LocalAddrIdType LocalAddrId);

/**********************************************************************************************************************
 * MISCELLANEOUS SERVICE FUNCTIONS - STOP
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * FUNCTIONS CONDUCTING CALLOUTS TO UPPER LAYERS - START
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_VPhysAddrTableChg()
 **********************************************************************************************************************/
/*! \brief         Called by the IpV4 or IpV6 in case of a change in the ARP/NDP table related to the
 *                 controller specified by CtrlIdx.
 *  \details       -
 *  \param[in]     CtrlIdx     EthIf controller index of the related ARP/NDP table.
 *  \param[in]     IpAddrPtr   specifies the IP address of the changed ARP/NDP table entry
 *  \param[in]     PhysAddrPtr specifies the physical address of the changed ARP/NDP table entry
 *  \param[in]     Valid       specifies if the ARP/NDP table entry is added or changed (TRUE) or has been removed (FALSE)
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VPhysAddrTableChg(
  uint8                         CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,
  TCPIP_P2V(uint8)              PhysAddrPtr,
  boolean                       Valid);

/**********************************************************************************************************************
 *  TcpIp_VDuplicateAddrDetected
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about an IPv4/IPv6 address conflict.
 *  \details       Does NOT trigger the named DEM event.
 *  \param[in]     LocalAddrId: IP address Identifier, representing an IP address specified in the TcpIp module configuration.
 *  \param[in]     SrcAddrPtr: Pointer to the memory where the affected source IP address is stored in network byte order.
 *  \param[in]     RemotePhysAddrPtr: Pointer to the memory where the remote physical address (MAC address) related to the specified IP address is stored in network byte order.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         DSGN-TcpIpDuplicateAddrDetection
 *********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_VDuplicateAddrDetected(
  TcpIp_LocalAddrIdType LocalAddrId,
  TCPIP_P2C(uint8)      SrcAddrPtr,
  TCPIP_P2C(uint8)      RemotePhysAddrPtr);

/**********************************************************************************************************************
 *  TcpIp_RxIndicationFunctions
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about the finalization of a reception of data.
 *  \details       -
 *  \param[in]     UserIndex         Identifier of the socket user (e.g. TLS or HTTP)
 *  \param[in]     SocketId          Id of the socket.
 *  \param[in]     RemoteSockAddrPtr Address info of the socket connected newly.
 *  \param[in]     DataPtr           Payload of the data received.
 *  \param[in]     DataLenByte       Payload length of the data received.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         DSGN-TcpIp
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_RxIndicationFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    TCPIP_P2V(TcpIp_SockAddrType) RemoteSockAddrPtr,
    TCPIP_P2V(uint8)              DataPtr, /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint16                        DataLenByte);

/**********************************************************************************************************************
 *  TcpIp_TxConfirmationFunctions
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about .
 *  \details       -
 *  \param[in]     userIndex         Identifier of the socket user (e.g. TLS or HTTP)
 *  \param[in]     socketId          Id of the socket
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TxConfirmationFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    uint16                        DataLenByte);

/**********************************************************************************************************************
 *  TcpIp_TcpAcceptedFunctions
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about a new socket connection on a listen socket and asks them for acceptance.
 *  \details       -
 *  \param[in]     UserIndex         Identifier of the socket user (e.g. TLS or HTTP)
 *  \param[in]     SocketId          Id of the listen socket that a connect has taken place on.
 *  \param[in]     SocketIdConnected Id of the socket connected newly.
 *  \param[in]     RemoteSockAddrPtr Address info of the socket connected newly.
 *  \return        E_OK: Upper layer has accepted the new connection.
 *  \return        E_NOT_OK: Upper layer has rejected the new connection.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpAcceptedFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    TcpIp_SocketIdType            SocketIdConnected,
    TCPIP_P2V(TcpIp_SockAddrType) RemoteSockAddrPtr);

/**********************************************************************************************************************
 *  TcpIp_TcpConnectedFunctions
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about the "connect" event on a socket.
 *  \details       -
 *  \param[in]     UserIndex         Identifier of the socket user (e.g. TLS or HTTP)
 *  \param[in]     SocketId
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TcpConnectedFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId);

/**********************************************************************************************************************
 *  TcpIp_TcpIpEventFunctions
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about TcpIp events.
 *  \details       -
 *  \param[in]     userIndex         Identifier of the socket user (e.g. TLS or HTTP)
 *  \param[in]     socketId          Id of the socket
 *  \param[in]     eventType         Status event that occurred in the connection related to the given socket.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
 FUNC(void, TCPIP_CODE) TcpIp_TcpIpEventFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    IpBase_TcpIpEventType         EventType);

/**********************************************************************************************************************
 *  TcpIp_TlsValidationResultFunctions
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about Tls validation results.
 *  \details       -
 *  \param[in]     UserIndex            Identifier of the socket user (e.g. SoAd or Scc)
 *  \param[in]     SocketId             Id of the socket
 *  \param[in,out] ValidationResultPtr  Result of the certificate validation in Tls
 *  \param[in]     CertChainPtr         Pointer to the received certificate chain
 *  \param[in]     CertChainLen         Length of the received certificate chain
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TlsValidationResultFunctions(
    uint8               UserIndex,
    TcpIp_SocketIdType  SocketId,
    TCPIP_P2V(uint8)    ValidationResultPtr,
    TCPIP_P2C(uint8)    CertChainPtr,
    uint32              CertChainLen );

/**********************************************************************************************************************
 *  TcpIp_CopyTxDataFunctions
 *********************************************************************************************************************/
/*! \brief         -
 *  \details       -
 *  \param[in]     UserIndex         Identifier of the socket user (e.g. TLS or HTTP)
 *  \param[in]     SocketId          Id of the socket
 *  \param[in]     -
 *  \return        -
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         DSGN-TcpIp
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_CopyTxDataFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    TCPIP_P2V(uint8)              BufferPtr,    /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    TCPIP_P2V(uint16)             BufferLenBytePtr);

/**********************************************************************************************************************
 *  TcpIp_LocalIpAssignmentChgFunctions
 *********************************************************************************************************************/
/*! \brief         Calls the configured IP address assignment change callbacks of the upper layers.
 *  \details       -
 *  \param[in]     LocalAddrId      Local IP address identifier.
 *  \param[in]     AddrState        Assignment state of the IP address.
 *  \return        -
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         DSGN-TcpIp
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_LocalIpAssignmentChgFunctions(
    TcpIp_LocalAddrIdType         LocalAddrId,
    TcpIp_IpAddrStateType         AddrState);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VSockTcpStateChg
 **********************************************************************************************************************/
/*! \brief         Conducts the "TCP socket state change" event callback to upper layers.
 *  \details       The callback is conducted for the given socket index and socket user.
 *  \param[in]     SockIdx               socket index
 *  \param[in]     SoUserIdx             socket user
 *  \param[in]     State                 new socket state
 *  \param[in]     SocketUsesTls         indicates wether the socket uses TLS
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VSockTcpStateChg(
  TcpIp_SockIdxType SockIdx,
  uint8 SoUserIdx,
  TcpIp_StateType State,
  boolean SocketUsesTls );
#endif

/***********************************************************************************************************************
 *  TcpIp_VSockRxIndicationAsr42x
 **********************************************************************************************************************/
/*! \brief         Conducts the receive indication callback to upper layers.
 *  \details       -
 *  \param[in]     SockIdx               socket index
 *  \param[in]     RxSockPtr             pointer to socket
 *  \param[in]     DataDescPtr           pointer to received data
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VSockRxIndicationAsr42x(
    TcpIp_SockIdxType SockIdx,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockAddrPtr,
    P2VAR(IpBase_PbufType, AUTOMATIC, TCPIP_APPL_DATA) DataDescPtr );

/***********************************************************************************************************************
 *  TcpIp_VSockTxConfirmation
 **********************************************************************************************************************/
/*! \brief         tx confirmation callback for socket
 *  \details       -
 *  \param[in]     SockIdx               socket index
 *  \param[in]     DataLenByte           length of confirmed data in bytes
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VSockTxConfirmation(
    TcpIp_SockIdxType SockIdx,
    uint32 DataLenByte );

/***********************************************************************************************************************
 *  TcpIp_Cbk_VLocalIpAssignmentChg
 **********************************************************************************************************************/
/*! \brief         Conducts the IP state change event callback to upper layers.
 *  \details       The callback is conducted to for the given local address id.
 *  \param[in]     LocalAddrId           local address identifier
 *  \param[in]     State                 new state of address (ASSIGNED, UNASSIGNED, ONHOLD)
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VLocalIpAssignmentChg(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrStateType State );

/***********************************************************************************************************************
 *  TcpIp_Cbk_VTrcvLinkStateChg
 **********************************************************************************************************************/
/*! \brief          Conducts the transceiver state change event callback to upper layers.
 *  \details        The callback is conducted for the given IP controller / transceiver index.
 *  \param[in]      IpCtrlIdx             IP controller / transceiver index
 *  \param[in]      IpCtrlFamily          Family of the IP controller (AF_INET or AF_INET6)
 *  \param[in]      Assigned              assignment flag
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VTrcvLinkStateChg(
  uint8 IpCtrlIdx,
  IpBase_FamilyType IpCtrlFamily,
  boolean Assigned );

/***********************************************************************************************************************
 *  TcpIp_Cbk_VPathMtuChg 
 **********************************************************************************************************************/
/*! \brief          Conducts the "path MTU has changed" event callout to upper layers.
 *  \details        The callback is conducted for the given the given ethernet ctrl index and destination address.
 *  \param[in]      CtrlIdx               controller index
 *  \param[in]      SockAddrPtr           net address that changed its state
 *  \param[in]      PathMtuSize           new size of the Path-MTU (transport layer part)
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VPathMtuChg(
  uint8 CtrlIdx,
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAddrPtr,
  uint16 PathMtuSize );

/***********************************************************************************************************************
 *  TcpIp_Cbk_VAddrResTimeout
 **********************************************************************************************************************/
/*! \brief          Conducts the "address resolution has timed out" callback to upper layers.
 *  \details        The callback is conducted for the given local address id, after the address resolution request
 *                  has been accepted, earlier.
 *  \param[in]      LocalAddrId           local address identifier
 *  \param[in]      DestAddrPtr           Pointer to destination address
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VAddrResTimeout(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestAddrPtr );

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VSockTcpStateChgAccepted
 **********************************************************************************************************************/
/*! \brief          Conducts the "TCP state change accepted" event callback to upper layers.
 *  \details        This function is derived from TcpIp_VSockTcpStateChg but implements a the following special handling:
 *                  1. It provides a return value from the called upper layer function.
 *                  2. It checks the given SoUserIdx.
 *                  3. It looks up the ListenSockIdx using the runtime-data-array TcpIp_SocketList at index SockIdx.
 *  \param[in]      SockIdx               socket index
 *  \param[in]      SoUserIdx             socket user
 *  \return         E_OK                  The user accepted the accepted connection
 *                  E_NOT_OK              The user refused the accepted connection
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VSockTcpStateChgAccepted(
  TcpIp_SockIdxType SockIdx,
  uint8 SoUserIdx );
#endif
/* (STD_ON == TCPIP_SUPPORT_TCP) */

/***********************************************************************************************************************
 *  TcpIp_VIpGetRandNoFct
 **********************************************************************************************************************/
/*! \brief          Provides a random number. Planned caller is a "lower layer", i.e. IPv4, IPv6.
 *  \details        -
 *  \return         RandNo              random number
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(uint16, TCPIP_CODE) TcpIp_VIpGetRandNoFct(void);

/**********************************************************************************************************************
 * FUNCTIONS CONDUCTING CALLOUTS TO UPPER LAYERS - STOP
 **********************************************************************************************************************/

#if (TCPIP_ENABLE_DLISTS == STD_ON)
/**********************************************************************************************************************
 * PUBLIC FUNCTIONS OF THE DYNAMIC LIST FUNCTIONALITY ("DList") - START
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_DListInitDesc()
 **********************************************************************************************************************/
/*! \brief          Initializes a Dlist descriptor. All nodes from the node storage array will be linked and inserted
 *                  into the free list. This descriptor can be used by multiple DList instances. See TcpIp_DListInit().
 *  \param[in]      DescPtr       Descriptor to be initialized.
 *  \param[in]      NodePtr       Node storage array
 *  \param[in]      NodeCount     Number of nodes in node storage array
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListInitDesc(
  TCPIP_P2V(TcpIp_DListDescType)  DescPtr,
  TCPIP_P2V(TcpIp_DListNodeType)  NodePtr,
  uint8                         NodeCount);

/**********************************************************************************************************************
 *  TcpIp_DListInit()
 **********************************************************************************************************************/
/*! \brief          Initializes a DList instance. After initialization the list is empty.
 *                  New elements can be inserted by TcpIp_DListAddElement().
 *  \param[in]      ListPtr       DList instnace
 *  \param[in]      DescPtr       DList descriptor that will by used by this instance.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListInit(
  TCPIP_P2V(TcpIp_DListType)      ListPtr,
  TCPIP_P2V(TcpIp_DListDescType)  DescPtr);

/**********************************************************************************************************************
 *  TcpIp_DListAddElement()
 **********************************************************************************************************************/
/*! \brief      Adds an element to a DList instnace by moving it from the free list.
 *  \param[in]  DListPtr   Handle to the list into that the new element should be inserted.
 *  \param[in]  AfterIdx   Index of a list item that shall be the predecessor of the new item.
 *                         If the index is TCPIP_DLIST_END_IDX the new item will be inserted at the beginning of the list.
 *  \param[out] NewIdxPtr  Index of the inserted item. Only valid if result is E_OK.
 *  \retval     E_OK       success
 *  \retval     E_NOT_OK   No elements in free list. (NewIdxPtr not modified)
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DListAddElement(
  TCPIP_P2V(TcpIp_DListType)      DListPtr,
  TcpIp_DListIdxType             AfterIdx,
  TCPIP_P2V(TcpIp_DListIdxType)   NewIdxPtr);

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElement()
 **********************************************************************************************************************/
/*! \brief      Removes a single element from a DList instance and adds it to the free list of the descriptor
 *  \param[in]  DListPtr  Handle to the DList instnace containing the element to be removed.
 *  \param[in]  RemIdx    Index of the element that shall be removed.
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElement(
  TCPIP_P2V(TcpIp_DListType)      DListPtr,
  TcpIp_DListIdxType              RemIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElementRange()
 **********************************************************************************************************************/
/*! \brief      Removes a chain of linked elements from the list and adds them to the free list of the descriptor.
 *  \param[in]  DListPtr    Handle to the DList instnace containing the elements to be removed.
 *  \param[in]  RemStartIdx Index of the first item of the element chain that shall be removed.
 *  \param[in]  RemEndIdx   Index of the lest item of the element chain that shall be removed.
 *  \note       RemStartIdx and RemEndIdx may be equal but for performance reasons TcpIp_DListRemoveElement() should be
 *              used for removing single elements.
 *  \warning    For performance reasons this function does not check if the elements to be removed are correctly linked
 *              when TCPIP_VENABLE_INTERNAL_DET_CHECKS are not enabled!
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElementRange(
  TCPIP_P2V(TcpIp_DListType)      DListPtr,
  TcpIp_DListIdxType              RemStartIdx,
  TcpIp_DListIdxType              RemEndIdx);

/**********************************************************************************************************************
 *  TcpIp_DListClear()
 **********************************************************************************************************************/
/*! \brief      Clears a DList instance by moving all elements to the free list of the descriptor.
 *  \param[in]  DListPtr  Handle to the DList instance
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListClear(
  TCPIP_P2V(TcpIp_DListType)      DListPtr);

/**********************************************************************************************************************
 * PUBLIC FUNCTIONS OF THE DYNAMIC LIST FUNCTIONALITY ("DList") - STOP
 **********************************************************************************************************************/
#endif

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIP_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Priv.h
 *********************************************************************************************************************/
