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
 *         \file  TcpIp_Priv.c
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

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_PRIV_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_Priv.h"
#include "TcpIp.h"

#include "IpBase.h"

#if (TCPIP_SUPPORT_TCP == STD_ON)
#include "TcpIp_Tcp.h"
#endif

#include "TcpIp_Udp.h"
#include "TcpIp_XCfg.h"

#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
#include "TcpIp_DhcpV4.h"
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
#include "TcpIp_IpV4.h"
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# include "TcpIp_IpV6.h"
#endif

#if (TCPIP_SUPPORT_TLS == STD_ON)
# include "Tls_Cbk.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
VAR(uint8, TCPIP_VAR_NOINIT) TcpIp_ActiveState;

#if ( TCPIP_VENABLE_CANOE_WRITE_STRING == STD_ON )
VAR(sint8, TCPIP_VAR_NOINIT)                TcpIp_CanoeWriteStr[256];
#endif
  /* TCPIP_VENABLE_CANOE_WRITE_STRING */

#define TCPIP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (TCPIP_SUPPORT_TCP == STD_ON)
VAR(uint16, TCPIP_VAR_NOINIT)               TcpIp_Tcp_DynamicPortCount;
# endif
VAR(uint16, TCPIP_VAR_NOINIT)               TcpIp_Udp_DynamicPortCount;

#define TCPIP_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
CONST(IpBase_AddrIn6Type, TCPIP_CONST) TcpIp_Ipv6NullAddr =  /* PRQA S 0759 */ /* MD_TCPIP_18.4_0759 */
{
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  }
};
#endif

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_VMatchLocalIpAddrIdAndPort()
 **********************************************************************************************************************/
/*! \brief         Checks if an IPv4/IPv6 address matches the LocalAddrId to which the socket is bound.
 *  \details       -
 *  \param[in]     SockIdx:           Index of the socket.
 *  \param[in]     RxSockLocAddrPtr:  Pointer to the socket address.
 *  \param[in]     LocalAddrId        Local address id.
 *  \return        TRUE in case the LocalAddrId matches the locaAddrId of the bound socket, FALSE otherwise.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_VMatchLocalIpAddrIdAndPort(
    TcpIp_SockIdxType              SockIdx,
    TCPIP_P2C(IpBase_SockAddrType) RxSockLocAddrPtr,
    TcpIp_LocalAddrIdType          LocalAddrId);

#if (TCPIP_ENABLE_DLISTS == STD_ON)
/* Doubly Linked List (DList) Data Structure */

/* DList raw functions */

/**********************************************************************************************************************
 *  TcpIp_DListRawAddAfter()
 **********************************************************************************************************************/
/*! \brief Inserts a node into a node chain.
 *  \param[in]     NodeSetPtr  Storage array of the elements of the node chain.
 *  \param[in,out] FirstIdxPtr Index of the first element of the node chain.
 *  \param[in,out] LastIdxPtr  Index of the last element of the node chain.
 *  \param[in]     NewIdx      Index of the new element that shall be inserted into the node chain
 *  \param[in]     AfterIdx    Index of the predecessor of the newly inserted element. (must be in chain FirstIdx...LastIdx)
 *                             If AfterIdx is TCPIP_DLIST_END_IDX the new item will be inserted at the beginning.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddAfter(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              NewIdx,
  TcpIp_DListIdxType              AfterIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRawAddRangeAfter()
 **********************************************************************************************************************/
/*! \brief         Inserts a node chain into another node chain.
 *  \param[in]     NodeSetPtr    Storage array of the elements of the node chain.
 *  \param[in,out] FirstIdxPtr   Index of the first element of the node chain.
 *  \param[in,out] LastIdxPtr    Index of the last element of the node chain.
 *  \param[in]     NewStartIdx   Index of the first element of the node chain that shall be inserted
 *  \param[in]     NewEndIdx     Index of the last element of the node chain that shall be inserted
 *  \param[in]     AfterIdx      Index of the predecessor of the newly inserted node chain.
 *                               If AfterIdx is TCPIP_DLIST_END_IDX the chain will be inserted at the beginning.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddRangeAfter(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              NewStartIdx,
  TcpIp_DListIdxType              NewEndIdx,
  TcpIp_DListIdxType              AfterIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRawRemove()
 **********************************************************************************************************************/
/*! \brief         Removes a node from a node chain.
 *  \param[in]     NodeSetPtr    Storage array of the elements of the node chain.
 *  \param[in]     FirstIdxPtr   Index of the first element of the node chain.
 *  \param[in]     LastIdxPtr    Index of the last element of the node chain.
 *  \param[in]     RemIdx        Index of the element that shall be removed.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawRemove(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              RemIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRawRemoveRange()
 **********************************************************************************************************************/
/*! \brief         Removes a sub chain from a node chain.
 *  \param[in]     NodeSetPtr    Storage array of the elements of the node chain.
 *  \param[in,out] FirstIdxPtr   Index of the first element of the node chain.
 *  \param[in,out] LastIdxPtr    Index of the last element of the node chain.
 *  \param[in]     RemStartIdx   Index of the first element that shall be removed.
 *  \param[in]     RemEndIdx     Index of the last element that shall be removed.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DListRawRemoveRange(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              RemStartIdx,
  TcpIp_DListIdxType              RemEndIdx);

#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_VMatchLocalIpAddrIdAndPort
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
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_VMatchLocalIpAddrIdAndPort(
    TcpIp_SockIdxType              SockIdx,
    TCPIP_P2C(IpBase_SockAddrType) RxSockLocAddrPtr,
    TcpIp_LocalAddrIdType          LocalAddrId)
{
  boolean retVal = FALSE;

  /* #10 Check if socket is bound to the ANY controller/IP address. */
  if (TcpIp_SocketList[SockIdx].BindLocalAddrId == TCPIP_LOCALADDRID_ANY)
  {
    retVal = TRUE;
  }
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
  /* #20 Check if address specified by LocalAddrId is an IPv4 address. */
  else if (TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
  {
    /* Received packet via IpV4 */

    /* #30 Check if socket is bound to an IPv4 address. */
    if (TCPIP_LOCAL_ADDR_IS_AF_INET(TcpIp_SocketList[SockIdx].BindLocalAddrId))
    {
      /* #40 Ask IPv4 layer whether LocalAddrId of received packet matches the LocalAddrId to which the socket is bound. */
      uint8 matchType;

      /* Socket is bound to IpV4 address */
      IpV4_AddrIdType IpV4AddrId = TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(TcpIp_SocketList[SockIdx].BindLocalAddrId);

      /* MATCH? */
      retVal = IpV4_Ip_IsAddrIdAcceptable(IpV4AddrId, TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId), &matchType);
    }
    else
    {
      /* Socket is unbound or bound to other address family. -> This socket does not match. */
    }
  }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  /* #50 Check if address specified by LocalAddrId is an IPv6 address. */
  else if (TCPIP_COND_VALID_LOCALADDRID(LocalAddrId)) /* Just check for valid ID instead of INET6 because it would be redundant (produce MISRA warning) since INET is already checked above. */
  {
    /* #60 Check if socket is bound to an IPv6 address. */
    if (TCPIP_LOCAL_ADDR_IS_AF_INET6(TcpIp_SocketList[SockIdx].BindLocalAddrId))
    {
      /* #70 Ask IPv6 layer whether LocalAddrId of received packet matches the LocalAddrId to which the socket is bound. */
      uint8 matchType;

      IpV6_AddrIdType IpV6AddrId = TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(TcpIp_SocketList[SockIdx].BindLocalAddrId);

      /* MATCH? */
      retVal = IpV6_IsAddrIdAcceptable(IpV6AddrId, TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId), &matchType);
    }
    else
    {
      /* socket is unbound or bound to other address family. -> This socket does not match. */
    }
  }
#endif
  else
  {
    /* invalid or unsupported address family! */
    TcpIp_CallDetReportError(TCPIP_API_ID_RX_SOCK_IDENT, TCPIP_E_AFNOSUPPORT)
  }

  /* #80 If IP address matches to bound IP address return TRUE if port is equal to the bound port of the socket. */
  if (retVal == TRUE)
  {
    retVal = IpBase_SockPortIsEqual((TCPIP_P2C(IpBase_SockAddrType))&TcpIp_SocketList[SockIdx].LocSock, RxSockLocAddrPtr); /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_VRxUdpSockIdxIdent
 **********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
FUNC(TcpIp_SockIdxType, TCPIP_CODE) TcpIp_VRxUdpSockIdxIdent(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockLocAddrPtr,
    TcpIp_LocalAddrIdType                                LocalAddrId)
{
  TcpIp_SockIdxType udpSockIdx;
  TcpIp_SockIdxType udpSockIdxMatch = TCPIP_INV_SOCK_IDX;

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT_RETVAL((RxSockLocAddrPtr != NULL_PTR), udpSockIdxMatch);
  TCPIP_ASSERT(TCPIP_COND_VALID_LOCALADDRID(LocalAddrId));

  /* #10 Iterate over all UDP sockets. */

  for(udpSockIdx = 0; udpSockIdx < TcpIp_VCfgGetMaxNumUdpSocks(); udpSockIdx++)
  {
    /* #20 Check if UDP socket is bound. */
    if (TcpIp_SockListenActive[udpSockIdx].ConnStat == TCPIP_SOCK_CONN_LISTEN_ENABLED)
    {
      /* #30 Check if socket is bound to a LocalAddrId and a port that match the specified IP address and port. */
      if (TcpIp_VMatchLocalIpAddrIdAndPort(udpSockIdx, RxSockLocAddrPtr, LocalAddrId) == TRUE)
      {
        udpSockIdxMatch = udpSockIdx;
        break;
      }
    }
  }

  return udpSockIdxMatch;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VRxTcpSockIdxIdent
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
FUNC(TcpIp_SockIdxType, TCPIP_CODE) TcpIp_VRxTcpSockIdxIdent(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockLocAddrPtr,
    TcpIp_LocalAddrIdType                                LocalAddrId)
{
  TcpIp_SockIdxType tcpSockIdx;
  TcpIp_SockIdxType tcpSockIdxMatch = TCPIP_INV_SOCK_IDX;

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT_RETVAL((RxSockRemAddrPtr != NULL_PTR), tcpSockIdxMatch);
  TCPIP_ASSERT_RETVAL((RxSockLocAddrPtr != NULL_PTR), tcpSockIdxMatch);
  TCPIP_ASSERT(TCPIP_COND_VALID_LOCALADDRID(LocalAddrId));

  /* #10 Iterate over all TCP sockets and match IP address and Port information. */
  for(tcpSockIdx = TcpIp_VCfgGetMaxNumUdpSocks(); tcpSockIdx < TcpIp_VCfgGetMaxNumSocks(); tcpSockIdx++)
  {
    if (TcpIp_SockListenActive[tcpSockIdx].ConnStat == TCPIP_SOCK_CONN_CONNECTED)
    {
      /* #20 Check if socket is a connected TCP socket and (Remote IP, Remote Port, Local IP, Local Port) match exactly. */
      if (TcpIp_VSockAddrIpAndPortIsEqual((TCPIP_P2C(IpBase_SockAddrType))&TcpIp_SocketList[tcpSockIdx].LocSock, RxSockLocAddrPtr) == TRUE) /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* Local IP address and Port match. */
        if (TcpIp_VSockAddrIpAndPortIsEqual((TCPIP_P2C(IpBase_SockAddrType))&TcpIp_SocketList[tcpSockIdx].RemSock, RxSockRemAddrPtr) == TRUE) /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        {
          /* Local and Remote IP address and Port match. */
          tcpSockIdxMatch = tcpSockIdx;
          break;
        }
      }
    }
    else if (TcpIp_SockListenActive[tcpSockIdx].ConnStat == TCPIP_SOCK_CONN_LISTEN_ENABLED)
    {
      /* #30 If TCP socket is in state bound match IP address. */
      if (TcpIp_VMatchLocalIpAddrIdAndPort(tcpSockIdx, RxSockLocAddrPtr, LocalAddrId) == TRUE)
      {
        tcpSockIdxMatch = tcpSockIdx;
        /* Listen socket matches but continue search because there may be an established socket for this connection. */
      }
    }
    else
    {
      /* This socket does not match. */
    }
  }

  return tcpSockIdxMatch;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */
#endif

/***********************************************************************************************************************
 *  TcpIp_SetLastSockError
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SetLastSockError(
    uint16 Error )
{
  /* #10 Store the given error code 'Error' in the error variable that can be read out by the application. */
  TcpIp_LastSockError = Error;
}

/***********************************************************************************************************************
 *  TcpIp_VInitSockets
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VInitSockets(void)
{
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  TcpIp_LastSockError = 0;
  /* #10 Iterate all configured sockets and call the internal initialization function for each socket. */
  for (SockIdx = 0; SockIdx < (TcpIp_VCfgGetMaxNumSocks()); SockIdx++)
  {
    TcpIp_VInitSocket(SockIdx);
  }
}

/***********************************************************************************************************************
 *  TcpIp_VInitSocket
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VInitSocket(
    TcpIp_SockIdxType SockIdx )
{
  TcpIp_VDelSockAddrMapping(SockIdx);
  TcpIp_VInitTxSocket(SockIdx);
  TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_NONE;
  TcpIp_SockUserIdx[SockIdx]               = TCPIP_UNDEFINED_SOCK_OWNER_IDX;
  TcpIp_VPreconfigSocket(SockIdx);
}

/***********************************************************************************************************************
 *  TcpIp_VInitTxSocket
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VInitTxSocket(
    TcpIp_SockIdxType SockIdx )
{
  TcpIp_TxBufFillLevel[SockIdx] = 0;
  TcpIp_TxBufWritePos[SockIdx]  = 0;
  TcpIp_TxBufRequested[SockIdx] = FALSE;

  /* init Pbuf element */
  TcpIp_TxPbufArray[SockIdx << 1].totLen = 0;
}

/***********************************************************************************************************************
 *  TcpIp_VDelSockAddrMapping
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
FUNC(void, TCPIP_CODE) TcpIp_VDelSockAddrMapping(
    TcpIp_SockIdxType SockIdx )
{
  /* #10 Check the given socket index to be valid. */
  if (TCPIP_COND_VALID_SOCKET_ID(SockIdx))
  {
    /* #20 Clear the socket admin data and the socket user index of the socket identified by the given socked index. */
    ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SockIdx].RemSock))->sin_family = IPBASE_AF_UNSPEC;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SockIdx].LocSock))->sin_family = IPBASE_AF_UNSPEC;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    /* #30 Clear the IpV6 related socket admin data of the socket identified by the given socked index. */
    (void)IpBase_CopyIpV6Addr( &((IpBase_SockAddrIn6Type *)(&TcpIp_SocketList[SockIdx].RemSock))->sin6_addr, &TcpIp_Ipv6NullAddr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    (void)IpBase_CopyIpV6Addr( &((IpBase_SockAddrIn6Type *)(&TcpIp_SocketList[SockIdx].LocSock))->sin6_addr, &TcpIp_Ipv6NullAddr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#else
    /* #40 Clear the IpV4 related socket admin data of the socket identified by the given socked index. */
    ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SockIdx].RemSock))->sin_addr   = TCPIP_INADDR_ANY;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SockIdx].LocSock))->sin_addr   = TCPIP_INADDR_ANY;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#endif
    ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SockIdx].RemSock))->sin_port   = 0;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SockIdx].LocSock))->sin_port   = 0;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

    TcpIp_SockUserIdx[SockIdx] = TCPIP_UNDEFINED_SOCK_OWNER_IDX;

    TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_NONE;

    TcpIp_SocketList[SockIdx].IsTcpListenSocket      = FALSE;
    TcpIp_SocketList[SockIdx].TcpMaxNumListenSockets = 0;
    TcpIp_SocketList[SockIdx].TcpMasterListenSockId  = TCPIP_INV_SOCKET_ID;  /* invalid */

    /* Reset TX IpAddrId */
    /* Socket has ANY binding. This means use any IP on any controller for transmission */
    TcpIp_SocketList[SockIdx].TxIpAddrId      = TCPIP_IPVX_ADDR_ID_ANY; /* 0xFF */
    TcpIp_SocketList[SockIdx].BindLocalAddrId = TCPIP_LOCALADDRID_ANY;  /* 0xFE */

    /* #50 Call the service functions of the submodules IpV4 and IpV6 in order to reset the socket, there. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    IpV4_Ip_ResetSocket(SockIdx);
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    IpV6_ResetSocket(SockIdx);
#endif
  }
  else
  {
#if ( TCPIP_DEV_ERROR_REPORT == STD_ON )
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_V_DEL_SOCK_ADDR_MAPPING, TCPIP_E_INV_SOCK_ID); /* PRQA S 3458 */ /* MD_MSR_19.4 */
#endif
  }
} /* TcpIp_VDelSockAddrMapping() */

/***********************************************************************************************************************
 *  TcpIp_VSockIpAddrIsEqual
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VSockIpAddrIsEqual(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockBPtr )
{
  boolean retVal = FALSE;

  TCPIP_ASSERT_RETVAL((SockAPtr != NULL_PTR), retVal);
  TCPIP_ASSERT_RETVAL((SockBPtr != NULL_PTR), retVal);

  /* #10 Ensure that the addresses are comparable, i.e. their address familiy is the same. */
  if(SockAPtr->sa_family == SockBPtr->sa_family)
  {
    if(IPBASE_AF_INET == SockAPtr->sa_family)
    {
      /* #20 In case the addresses are IPv4, compare the 32-bit value of the addresses. */
      retVal = (boolean) ( ((const IpBase_SockAddrInType*)SockAPtr)->sin_addr == ((const IpBase_SockAddrInType*)SockBPtr)->sin_addr );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    }
    else if (IPBASE_AF_INET6 == SockAPtr->sa_family)
    {
      /* #30 In case the addresses are IPv6, compare the IpV6 addresses byte by byte. */
      uint16 i = sizeof(IpBase_AddrIn6Type);

      retVal = TRUE; /* Assume, that all bytes of the two IpV6 addresses are the same */
      while(i != 0)
      {
        i--;
        if( ((const uint8*)&((const IpBase_SockAddrIn6Type*)SockAPtr)->sin6_addr)[i] !=  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_compare */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
            ((const uint8*)&((const IpBase_SockAddrIn6Type*)SockBPtr)->sin6_addr)[i] )   /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_compare */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        {
          /* Set the retVal to FALSE, again, and exit the loop, as soon as the first byte differs */
          retVal = FALSE;
          break;
        }
      }
    }
    else
    {
      /* Error, no valid sa_family: Leave retVal at value FALSE. */
    }
  }
  /* else: Address families not the same: Leave retVal at value FALSE. */
  return retVal;
} /* TcpIp_VSockIpAddrIsEqual() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_VSockAddrIpAndPortIsEqual
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VSockAddrIpAndPortIsEqual(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockBPtr )
{
  boolean match = FALSE;

  TCPIP_ASSERT_RETVAL((SockAPtr != NULL_PTR), match);
  TCPIP_ASSERT_RETVAL((SockBPtr != NULL_PTR), match);

  /* #10 Check the socket families to be equal (basic precondition for comparison). */
  if(SockAPtr->sa_family == SockBPtr->sa_family)
  {
    /* #20 Depending on the address family, compare address and port of the given sockets. */
    switch (SockAPtr->sa_family)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    case IPBASE_AF_INET:
      {
        TCPIP_P2C(IpBase_SockAddrInType) SockAddrInAPtr = (TCPIP_P2C(IpBase_SockAddrInType))SockAPtr; /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        TCPIP_P2C(IpBase_SockAddrInType) SockAddrInBPtr = (TCPIP_P2C(IpBase_SockAddrInType))SockBPtr; /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */

        match = (boolean)(    (SockAddrInAPtr->sin_addr == SockAddrInBPtr->sin_addr)
                           && (SockAddrInAPtr->sin_port == SockAddrInBPtr->sin_port)
                         );
      }
      break;
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    case IPBASE_AF_INET6:
      {
        TCPIP_P2C(IpBase_SockAddrIn6Type) SockAddrInAPtr = (TCPIP_P2C(IpBase_SockAddrIn6Type))SockAPtr; /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        TCPIP_P2C(IpBase_SockAddrIn6Type) SockAddrInBPtr = (TCPIP_P2C(IpBase_SockAddrIn6Type))SockBPtr; /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
      
        match = (boolean)(    (SockAddrInAPtr->sin6_addr.addr32[0] == SockAddrInBPtr->sin6_addr.addr32[0])
                           && (SockAddrInAPtr->sin6_addr.addr32[1] == SockAddrInBPtr->sin6_addr.addr32[1])
                           && (SockAddrInAPtr->sin6_addr.addr32[2] == SockAddrInBPtr->sin6_addr.addr32[2])
                           && (SockAddrInAPtr->sin6_addr.addr32[3] == SockAddrInBPtr->sin6_addr.addr32[3])
                           && (SockAddrInAPtr->sin6_port == SockAddrInBPtr->sin6_port)
                         );
      }
      break;
#endif
    default:
      /* No match: Leave match at value FALSE. */
      break;
    }
  }

  return match;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */

/***********************************************************************************************************************
 *  TcpIp_VNetAddrIsUnicast
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VNetAddrIsUnicast(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAddrPtr )
{
  boolean retVal;
  /* #10 Determine the domain of the socket address. */
  switch(SockAddrPtr->sa_family)
  {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
  case IPBASE_AF_INET:
    {
      /* #20 For an IPv4 socket address check the address to be valid, non-multicast and non-broadcast. */
      /* socket address struct is always filled in network byte order */
      TcpIp_NetAddrType SockNetAddr = (((const IpBase_SockAddrInType*)SockAddrPtr)->sin_addr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */

      if( (0 != SockNetAddr) /* invalid global '0' address */ &&
          (!IPV4_ADDR_IS_MULTICAST(SockNetAddr)) /* multicast address */ &&
          (SockNetAddr != TCPIP_INADDR_BROADCAST) /* global broadcast */
          /* subnet broadcasts can not be detected here */ )
      {
        retVal = TRUE;
      }
      else
      {
        retVal = FALSE;
      }
      break;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  case IPBASE_AF_INET6:
    {
      /* #30 For an IPv6 socket address check the first byte of the address (in network byte order) to be non-0xff. */
      /* Read the first by of the address (network byte order). */
      P2CONST(uint8, AUTOMATIC, TCPIP_APPL_VAR) firstBytePtr = (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_VAR)) (&(((P2CONST(IpBase_SockAddrIn6Type, AUTOMATIC, TCPIP_APPL_VAR))SockAddrPtr)->sin6_addr));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_compare */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
      if ((*firstBytePtr  != 0xFF))
      {
        retVal = TRUE;
      }
      else
      {
        retVal = FALSE;
      }
      break;
    }
#endif
  default:
    {
      retVal = FALSE;  /* no valid domain type */
      break;
    }
  }

  return retVal;
} /* TcpIp_VNetAddrIsUnicast() */

/***********************************************************************************************************************
 *  TcpIp_VCalcPseudoHdrAndChecksum
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_VCalcPseudoHdrAndChecksum(
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockLocAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) DataLen,
    CONST(uint8, AUTOMATIC) Protocol )
{
  uint32 Checksum; /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  if(IPBASE_AF_INET == RxSockRemAddrPtr->sa_family)
#endif
  {
    /* IpV4 */
    TcpIp_PseudoHdrBlockV4Type PseudoHdr;
    TcpIp_PseudoHdrV4Type *PseudoHdrPtr = (TcpIp_PseudoHdrV4Type*)&PseudoHdr[0];  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

    PseudoHdrPtr->RemAddr  = ((const IpBase_SockAddrInType*)RxSockRemAddrPtr)->sin_addr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    PseudoHdrPtr->LocAddr  = ((const IpBase_SockAddrInType*)RxSockLocAddrPtr)->sin_addr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    PseudoHdrPtr->Null     = 0;
    PseudoHdrPtr->Protocol = Protocol;
    PseudoHdrPtr->LenByte  = IPBASE_HTON16(DataLen);
    Checksum = IpBase_TcpIpChecksumAdd((TCPIP_P2V(uint8))PseudoHdrPtr, (sizeof(PseudoHdr)), 0, FALSE);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    if (DataPtr != NULL_PTR)
    {
      /* Payload provided. Add payload checksum and finalize result. */
      Checksum = IpBase_TcpIpChecksumAdd(DataPtr, DataLen, Checksum, TRUE);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    }
  }
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  else
#endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  {
    /* IpV6 */
    TcpIp_PseudoHdrBlockV6Type PseudoHdr;
    TcpIp_PseudoHdrV6Type *PseudoHdrPtr = (TcpIp_PseudoHdrV6Type*)&PseudoHdr[0];  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

    (void)IpBase_CopyIpV6Addr(&PseudoHdrPtr->RemAddr, &((const IpBase_SockAddrIn6Type*)RxSockRemAddrPtr)->sin6_addr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    (void)IpBase_CopyIpV6Addr(&PseudoHdrPtr->LocAddr, &((const IpBase_SockAddrIn6Type*)RxSockLocAddrPtr)->sin6_addr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    PseudoHdrPtr->Null     = 0;
    PseudoHdrPtr->Protocol = Protocol;
    PseudoHdrPtr->LenByte  = IPBASE_HTON16(DataLen);
    Checksum = IpBase_TcpIpChecksumAdd((TCPIP_P2V(uint8))PseudoHdrPtr, (sizeof(PseudoHdr)), 0, FALSE);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    if (DataPtr != NULL_PTR)
    {
      /* Payload provided. Add payload checksum and finalize result. */
      Checksum = IpBase_TcpIpChecksumAdd(DataPtr, DataLen, Checksum, TRUE);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    }
  }
#endif

  return Checksum;
}

/***********************************************************************************************************************
 *  TcpIp_VDuplicateAddrDetected
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VDuplicateAddrDetected(
  TcpIp_LocalAddrIdType   LocalAddrId,
  TCPIP_P2C(uint8)        SrcAddrPtr,
  TCPIP_P2C(uint8)        RemotePhysAddrPtr)
{
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  Eth_PhysAddrType LocalPhysAddr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  /* #10 Verify that RemotePhysAddrPtr does not match the physical address of this node. */
  if (TcpIp_GetPhysAddr(LocalAddrId, &LocalPhysAddr[0]) == E_OK)
  {
    if (TcpIp_CmpLLAddr(RemotePhysAddrPtr, LocalPhysAddr) == FALSE)
    {
      /* #20 Verify that LocalAddrId references an IPv6 address. */
      if (TCPIP_LOCAL_ADDR_IS_AF_INET6(LocalAddrId))
      {
        /* #20 Verify that LocalAddrId references an IPv6 address. */
        uint8_least cbkIdx;
        TcpIp_SockAddrInXType sockAddr;

        IPV6_ADDR_COPY(sockAddr.sin6_addr, *SrcAddrPtr); /* PRQA S 0310 */ /* MD_IpV6_0310 */
        sockAddr.sin6_port = TCPIP_PORT_ANY;
        sockAddr.sin6_family = IPBASE_AF_INET6;

        /* #30 Inform all registered upper layers about a DADCONFLICT using the associated callback. */
        for(cbkIdx = 0; cbkIdx < TcpIp_CfgDuplicateAddrDetectionHandlingCbkCount(); cbkIdx++)
        {
          TcpIp_CfgDuplicateAddrDetectionHandlingCbk((uint8)cbkIdx)(LocalAddrId, (TcpIp_SockAddrType*)(&sockAddr), &LocalPhysAddr[0], RemotePhysAddrPtr);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
        }
      }
    }
  }
#else
  TCPIP_UNUSED_PARAMETER(LocalAddrId);
  TCPIP_UNUSED_PARAMETER(SrcAddrPtr);
  TCPIP_UNUSED_PARAMETER(RemotePhysAddrPtr);
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_VDuplicateAddrDetected() */

 /***********************************************************************************************************************
 *  TcpIp_VPhysAddrTableChg
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VPhysAddrTableChg(
  uint8                         CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,
  TCPIP_P2V(uint8)              PhysAddrPtr,  /* PRQA S 3206, 3673 */ /* MD_TCPIP_3206, MD_TCPIP_16.7_3673 */
  boolean                       Valid)
{
#if (TCPIP_PHYS_ADDR_TABLE_CHG_FUNCTION_CNT > 0)
  uint8_least cbkIdx;
#endif

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
  /* #10 In case UDP queue for address cache misses is configured, clear pending UDP retry elements for unresolved destinations. */
  if (FALSE == Valid)
  {
    TcpIp_Udp_CancelRetriesForDestination(CtrlIdx, IpAddrPtr);
  }
#endif

#if (TCPIP_PHYS_ADDR_TABLE_CHG_FUNCTION_CNT > 0)
  /* #20 Inform upper layers about the change in the physical address resolution table. */
  for (cbkIdx = 0; cbkIdx < TcpIp_VCfgGetPhysAddrTableChgFuncCount(); cbkIdx++)
  {
    TcpIp_PhysAddrTableChgCallbacks[cbkIdx](CtrlIdx, IpAddrPtr, PhysAddrPtr, Valid);
  }
#else
  TCPIP_UNUSED_PARAMETER(PhysAddrPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
# if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_OFF)
  TCPIP_UNUSED_PARAMETER(CtrlIdx);      /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(IpAddrPtr);    /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(Valid);        /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif 
#endif

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_VPhysAddrTableChg() */

/***********************************************************************************************************************
 *  TcpIp_RxIndicationFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_RxIndicationFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    TCPIP_P2V(TcpIp_SockAddrType) RemoteSockAddrPtr,
    TCPIP_P2V(uint8)              DataPtr, /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint16                        DataLenByte)
{
  TCPIP_ASSERT(UserIndex < TcpIp_VCfgGetNumSocketOwners());
  TCPIP_ASSERT(SocketId  < TcpIp_VCfgGetMaxNumSocks());
  TCPIP_ASSERT(RemoteSockAddrPtr != NULL_PTR);
  TCPIP_ASSERT(DataPtr           != NULL_PTR);

  /* #10 Check the index of the socket user. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 In case the index is valid, inform upper layers about the finalization of the reception on the given socket. */
    if (TcpIp_SocketOwnerConfig[UserIndex].RxIndicationFuncPtr != NULL_PTR)
    {
      TcpIp_SocketOwnerConfig[UserIndex].RxIndicationFuncPtr(SocketId, RemoteSockAddrPtr, DataPtr, DataLenByte);
    }
    else
    {
      /* #30 Issue a DET error in case the mandatory callback to do that is not configured. */
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_RX_INDICATION, TCPIP_E_CBK_REQUIRED)
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_RX_INDICATION, TCPIP_E_INV_ARG)
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_TxConfirmationFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TxConfirmationFunctions(
    uint8              UserIndex,
    TcpIp_SocketIdType SocketId,
    uint16             DataLenByte)
{
  TCPIP_ASSERT(UserIndex < TcpIp_VCfgGetNumSocketOwners());
  TCPIP_ASSERT(SocketId  < TcpIp_VCfgGetMaxNumSocks());

  /* #10 Check if UserIndex is valid. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 Inform the socket user about the amount of transmitted data, if callback is configured. */
    if (TcpIp_SocketOwnerConfig[UserIndex].TxConfirmationFuncPtr != NULL_PTR)
    {
      TcpIp_SocketOwnerConfig[UserIndex].TxConfirmationFuncPtr(SocketId, DataLenByte);
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TX_CONFIRMATION, TCPIP_E_INV_ARG)
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_TcpAcceptedFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpAcceptedFunctions(
    uint8                         UserIndex,
    TcpIp_SocketIdType            SocketId,
    TcpIp_SocketIdType            SocketIdConnected,
    TCPIP_P2V(TcpIp_SockAddrType) RemoteSockAddrPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  TCPIP_ASSERT(UserIndex         < TcpIp_VCfgGetNumSocketOwners());
  TCPIP_ASSERT(SocketId          < TcpIp_VCfgGetMaxNumSocks());
  TCPIP_ASSERT(SocketIdConnected < TcpIp_VCfgGetMaxNumSocks());
  TCPIP_ASSERT(RemoteSockAddrPtr != NULL_PTR);

  /* #10 Check the index of the socket user. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 In case the index is valid and a callout is configured for that user, inform the socket user about the acceptance of the socket. */
    if (TcpIp_SocketOwnerConfig[UserIndex].TcpAcceptedFuncPtr != NULL_PTR)
    {
      retVal = TcpIp_SocketOwnerConfig[UserIndex].TcpAcceptedFuncPtr(SocketId, SocketIdConnected, RemoteSockAddrPtr);
    }
    else
    {
      /* #30 Otherwise issue a DET error. */
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_ACCEPTED, TCPIP_E_CBK_REQUIRED)
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_ACCEPTED, TCPIP_E_INV_ARG)
  }

  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_TcpConnectedFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TcpConnectedFunctions(
    uint8              UserIndex,
    TcpIp_SocketIdType SocketId)
{
  TCPIP_ASSERT(UserIndex < TcpIp_VCfgGetNumSocketOwners());
  TCPIP_ASSERT(SocketId  < TcpIp_VCfgGetMaxNumSocks());

  /* #10 Check the index of the socket user. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 In case a callout is configured for that user, inform the socket user about the connection of the socket. */
    if (TcpIp_SocketOwnerConfig[UserIndex].TcpConnectedFuncPtr != NULL_PTR)
    {
      TcpIp_SocketOwnerConfig[UserIndex].TcpConnectedFuncPtr(SocketId);
    }
    else
    {
      /* #30 Otherwise issue a DET error. */
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_CONNECTED, TCPIP_E_CBK_REQUIRED)
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_CONNECTED, TCPIP_E_INV_ARG)
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_TcpIpEventFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TcpIpEventFunctions(
    uint8                 UserIndex,
    TcpIp_SocketIdType    SocketId,
    IpBase_TcpIpEventType EventType)
{
  TCPIP_ASSERT(UserIndex < TcpIp_VCfgGetNumSocketOwners());
  TCPIP_ASSERT(SocketId  < TcpIp_VCfgGetMaxNumSocks());

  /* #10 Check the index of the socket user. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 In case a callout is configured for that user, inform the socket user about the TCP event related to that socket. */
    if (TcpIp_SocketOwnerConfig[UserIndex].TcpIpEventFuncPtr != NULL_PTR)
    {
      TcpIp_SocketOwnerConfig[UserIndex].TcpIpEventFuncPtr(SocketId, EventType);
    }
    else
    {
      /* #30 Otherwise issue a DET error. */
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_IP_EVENT, TCPIP_E_CBK_REQUIRED)
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_IP_EVENT, TCPIP_E_INV_ARG)
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_TlsValidationResultFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TlsValidationResultFunctions(
    uint8               UserIndex,
    TcpIp_SocketIdType  SocketId,
    TCPIP_P2V(uint8)    ValidationResultPtr,
    TCPIP_P2C(uint8)    CertChainPtr,
    uint32              CertChainLen )
{
  /* #10 Check the index of the socket user. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 Call TlsValidation callback of socket owner, if configured. */
    if (TcpIp_SocketOwnerConfig[UserIndex].TlsValidationResultFuncPtr != NULL_PTR)
    {
      TcpIp_SocketOwnerConfig[UserIndex].TlsValidationResultFuncPtr(SocketId, ValidationResultPtr, CertChainPtr, CertChainLen);
    }
    /* #30 Otherwise: Do nothing. */
    else
    {
      /* OK: Callback is optional. */
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TLS_VALIDIATION_RESULT, TCPIP_E_INV_ARG)
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_CopyTxDataFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_CopyTxDataFunctions(
    uint8               UserIndex,
    TcpIp_SocketIdType  SocketId,
    TCPIP_P2V(uint8)    BufferPtr,    /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    TCPIP_P2V(uint16)   BufferLenBytePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  TCPIP_ASSERT(UserIndex < TcpIp_VCfgGetNumSocketOwners());
  TCPIP_ASSERT(SocketId  < TcpIp_VCfgGetMaxNumSocks());
  TCPIP_ASSERT(BufferPtr        != NULL_PTR);
  TCPIP_ASSERT(BufferLenBytePtr != NULL_PTR);

  /* #10 Check the index of the socket user. */
  if (UserIndex < TcpIp_VCfgGetNumSocketOwners())
  {
    /* #20 Call extended CopyTxData callback of socket owner, if configured. */
    if (TcpIp_SocketOwnerConfig[UserIndex].CopyTxDataDynFuncPtr != NULL_PTR)
    {
      retVal = TcpIp_SocketOwnerConfig[UserIndex].CopyTxDataDynFuncPtr(SocketId, BufferPtr, BufferLenBytePtr);
    }
    /* #30 Otherwise: Call AUTOSAR CopyTxData callback of socket owner, if configured. */
    else if (TcpIp_SocketOwnerConfig[UserIndex].CopyTxDataFuncPtr != NULL_PTR)
    {
      retVal = TcpIp_SocketOwnerConfig[UserIndex].CopyTxDataFuncPtr(SocketId, BufferPtr, *BufferLenBytePtr);
    }
    /* #40 Otherwise: Report an error to the Det. */
    else
    {
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_COPY_TX_DATA, TCPIP_E_CBK_REQUIRED)
    }
  }
  else
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_COPY_TX_DATA, TCPIP_E_INV_ARG)
  }

  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_LocalIpAssignmentChgFunctions
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_LocalIpAssignmentChgFunctions(
    TcpIp_LocalAddrIdType LocalAddrId,   /* PRQA S 3206 */ /* MD_TCPIP_3206 */
    TcpIp_IpAddrStateType AddrState)  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
#if (TcpIp_VCfgGetLocalIpAssignmentChgFuncCount() > 0)
  uint8_least cbkIdx;

  TCPIP_ASSERT(LocalAddrId <= TcpIp_VCfgGetMaxLocalAddrId());

  for (cbkIdx = 0; cbkIdx < TcpIp_VCfgGetLocalIpAssignmentChgFuncCount(); cbkIdx++)
  {
    TcpIp_LocalIpAddrAssignmentCallbacks[cbkIdx](LocalAddrId, AddrState);
  }
#else
  TCPIP_UNUSED_PARAMETER(LocalAddrId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  TCPIP_UNUSED_PARAMETER(AddrState); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_VIpTransmit(
    TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr,
    uint16                                     IpPayloadLen,
    uint8                                      IpProtocol,
    boolean                                    ReqTxConfirmation,
    uint8                                      UlTxReqIdx,
    boolean                                    RetryOnPhysAddrMiss,
    TCPIP_P2C(IpBase_SockAddrType)             DestSockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retVal;

  /* ----- Development Error Checks ------------------------------------- */
  TCPIP_ASSERT(DestSockAddrPtr != NULL_PTR);
  TCPIP_ASSERT((DestSockAddrPtr->sa_family == IPBASE_AF_INET) || (DestSockAddrPtr->sa_family == IPBASE_AF_INET6));

#if (TCPIP_SUPPORT_IPV4 != TCPIP_SUPPORT_IPV6)
  TCPIP_UNUSED_PARAMETER(DestSockAddrPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
  if (DestSockAddrPtr->sa_family == IPBASE_AF_INET)
# endif
  {
    retVal = IpV4_Ip_Transmit(IpTxReqDescPtr, IpPayloadLen, IpProtocol, ReqTxConfirmation, UlTxReqIdx, RetryOnPhysAddrMiss);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
  }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
  else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  {
    retVal = IpV6_Transmit(IpTxReqDescPtr->SockIdx, IpTxReqDescPtr, IpProtocol, IpPayloadLen, ReqTxConfirmation, UlTxReqIdx, RetryOnPhysAddrMiss);
  }
#endif

  return retVal;
}  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VSockTcpStateChg
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VSockTcpStateChg(
  TcpIp_SockIdxType SockIdx,
  uint8 SoUserIdx,
  TcpIp_StateType State,
  boolean SocketUsesTls )  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT_RET(TCPIP_COND_VALID_TCPSOCKIDX(SockIdx));

#if (TCPIP_SUPPORT_TLS == STD_OFF)
  TCPIP_UNUSED_PARAMETER(SocketUsesTls);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  /* #10 Check the owner of the socket to be valid. */
  if(SoUserIdx < TCPIP_UNDEFINED_SOCK_OWNER_IDX)
  {
    /* #20 Set the new socket state and perform the actions related to the state-transition, depending on the given state. */
    switch(State)
    {
    case TCPIP_TCP_STATE_CONN_ESTAB:
      TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_CONNECTED;
      break;
    case TCPIP_TCP_STATE_CONNECTED:
      TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_CONNECTED;
#if (TCPIP_SUPPORT_TLS == STD_ON)
      if(SocketUsesTls == TRUE)
      {
        Tls_TcpConnected(SockIdx);
      }
      else
#endif
      {
        TcpIp_TcpConnectedFunctions(SoUserIdx, SockIdx);
      }
      break;
    case TCPIP_TCP_STATE_CLOSED:
      TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_NONE;
#if (TCPIP_SUPPORT_TLS == STD_ON)
      if(SocketUsesTls == TRUE)
      {
        Tls_TcpIpEvent(SockIdx, IPBASE_TCP_EVENT_CLOSED);
        TcpIp_TcpIpEventFunctions(SoUserIdx, SockIdx, IPBASE_TCP_EVENT_CLOSED);
      }
      else
#endif
      {
        TcpIp_TcpIpEventFunctions(SoUserIdx, SockIdx, IPBASE_TCP_EVENT_CLOSED);
      }
      break;
    case TCPIP_TCP_STATE_RST_REC:
      TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_NONE;
#if (TCPIP_SUPPORT_TLS == STD_ON)
      if(SocketUsesTls == TRUE)
      {
        Tls_TcpIpEvent(SockIdx, IPBASE_TCP_EVENT_RESET);
      }
#endif
      TcpIp_TcpIpEventFunctions(SoUserIdx, SockIdx, IPBASE_TCP_EVENT_RESET);
      break;
    case TCPIP_TCP_STATE_CONN_REQ_REC:
      /* always accept the incoming connection, do not forward callback to upper layers */
      break;
    case TCPIP_TCP_STATE_FIN_REC:
      /* remote side starts to close the connection */
#if (TCPIP_SUPPORT_TLS == STD_ON)
      if(SocketUsesTls == TRUE)
      {
        Tls_TcpIpEvent(SockIdx, IPBASE_TCP_EVENT_FIN_RECEIVED);
      }
#endif
      TcpIp_TcpIpEventFunctions(SoUserIdx, SockIdx, IPBASE_TCP_EVENT_FIN_RECEIVED);
      break;
    case TCPIP_TCP_STATE_CONN_REFUSED:
      TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_NONE;
#if (TCPIP_SUPPORT_TLS == STD_ON)
      if(SocketUsesTls == TRUE)
      {
        Tls_TcpIpEvent(SockIdx, IPBASE_TCP_EVENT_RESET);
      }
#endif
      TcpIp_TcpIpEventFunctions(SoUserIdx, SockIdx, IPBASE_TCP_EVENT_RESET);
      break;
    case TCPIP_TCP_STATE_CONN_CLOSING:
      /* No action to be done */
      break;
    default:
      /* should not happen - all other states are invalid */
      break;
    } /* switch */
  } /* else: invalid user index */
}
#endif /* (TCPIP_SUPPORT_TCP == STD_ON) */

/***********************************************************************************************************************
 *  TcpIp_VSockRxIndicationAsr42x
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VSockRxIndicationAsr42x(
    TcpIp_SockIdxType SockIdx,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_CONST) RxSockAddrPtr,
    P2VAR(IpBase_PbufType, AUTOMATIC, TCPIP_APPL_DATA) DataDescPtr )
{
  TcpIp_SockUserIdxType  SoUserIdx;

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_SOCKET_ID(SockIdx));
  TCPIP_ASSERT_RET((RxSockAddrPtr != NULL_PTR));
  TCPIP_ASSERT_RET((DataDescPtr   != NULL_PTR));

  /* #10 Look up the socket user based on the given socket index. */
  SoUserIdx = TcpIp_SockUserIdx[SockIdx];

  /* #20 In case the socket user is valid, call the configured RxIndication function. */
  if (SoUserIdx < TCPIP_UNDEFINED_SOCK_OWNER_IDX)
  {
    TcpIp_RxIndicationFunctions(SoUserIdx, (TcpIp_SocketIdType)SockIdx, (TcpIp_SockAddrType*)RxSockAddrPtr, DataDescPtr->payload, DataDescPtr->len);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 0311 */ /* MD_TCPIP_11.5_0311_ASR */

    /* #30 Call RxIndication again if the data comes in two parts due to a wrap-around inside the ring-buffer. */
    if(DataDescPtr->len != DataDescPtr->totLen)
    {
      TcpIp_PbufType *LocPbufPtr = &DataDescPtr[1];
      TcpIp_RxIndicationFunctions(SoUserIdx, (TcpIp_SocketIdType)SockIdx, (TcpIp_SockAddrType*)RxSockAddrPtr, LocPbufPtr->payload, LocPbufPtr->len);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 0311 */ /* MD_TCPIP_11.5_0311_ASR */
    }
  } 
  /* consumed length will be ACKed by TcpIp_TcpReceived() later */
} /* TcpIp_VSockRxIndicationAsr42x() */

/***********************************************************************************************************************
 *  TcpIp_VSockTxConfirmation
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VSockTxConfirmation(
    TcpIp_SockIdxType SockIdx,
    uint32 DataLenByte )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_SOCKET_ID(SockIdx));
  /* #10 Check that the socket has a valid user. */
  if(TcpIp_SockUserIdx[SockIdx] < TCPIP_UNDEFINED_SOCK_OWNER_IDX)
  {
    /* #20 Confirm the overall length in chunks of at maximum 0xffff bytes. */
    if(DataLenByte <= TCPIP_TX_CONF_LEN_MAX)
    {
      /* forward confirmation to UL: */
      TcpIp_TxConfirmationFunctions(TcpIp_SockUserIdx[SockIdx], (TcpIp_SockHndType)(SockIdx), (uint16)DataLenByte);
    }
    else
    {
      uint16 txConfLen;
      uint32 remainingTxConfLen = DataLenByte;
      while(remainingTxConfLen != 0)
      {
        /* forward confirmation to UL: */
        if(remainingTxConfLen > TCPIP_TX_CONF_LEN_MAX)
        {
          txConfLen = (uint16)TCPIP_TX_CONF_LEN_MAX;
        }
        else
        {
          txConfLen = (uint16)remainingTxConfLen;
        }
        TcpIp_TxConfirmationFunctions(TcpIp_SockUserIdx[SockIdx], (TcpIp_SockHndType)(SockIdx), txConfLen);
        remainingTxConfLen -= txConfLen;
      }
    }

    /* #30 Update the internal buffer fill level according to the number of bytes confirmed. */
    /* decrease buffer fill level: */
    if(0 != DataLenByte)
    {
      TcpIp_TxBufFillLevel[SockIdx] -= (uint16)DataLenByte;
    }
    else
    {
      /* negative TxConfirmation */
      TcpIp_TxBufFillLevel[SockIdx] = 0;
      CANOE_WRITE_STRING("TcpIp_TxConfirmation: negative TxConfirmation")
    }
  }
} /* TcpIp_VSockTxConfirmation() */

/***********************************************************************************************************************
 *  TcpIp_Cbk_VLocalIpAssignmentChg
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VLocalIpAssignmentChg(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrStateType State)
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_LOCALADDRID(LocalAddrId));

  /* #10 Store the new state of the given local address id. */
  TcpIp_IpAddrAssignmentState[LocalAddrId] = State;

  /* #20 Reset the affected IP controller in case the new state is 'unassigned'. */
  if(TCPIP_IPADDR_STATE_UNASSIGNED == State)
  {
    TcpIp_VRstCtrl(LocalAddrId);
  }

  TcpIp_LocalIpAssignmentChgFunctions(LocalAddrId, State); /*lint !e522 */ /* Function may have no side-effects depending on configuration. */
} /* TcpIp_Cbk_VLocalIpAssignmentChg() */

/***********************************************************************************************************************
 *  TcpIp_Cbk_VTrcvLinkStateChg
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VTrcvLinkStateChg(
  uint8 IpCtrlIdx,                 /* PRQA S 3206 */ /* MD_TCPIP_3206 */
  IpBase_FamilyType IpCtrlFamily,  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
  boolean Assigned )               /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);

  TCPIP_UNUSED_PARAMETER(IpCtrlIdx);     /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(IpCtrlFamily);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(Assigned);      /* PRQA S 3112 */ /* MD_MSR_14.2 */

  /* #10 Reset all sockets that are currently using the given controller. */

} /* TcpIp_Cbk_VTrcvLinkStateChg() */

/***********************************************************************************************************************
 *  TcpIp_Cbk_VPathMtuChg
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VPathMtuChg(
  uint8 CtrlIdx,  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAddrPtr,
  uint16 PathMtuSize )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT_RET((SockAddrPtr != NULL_PTR));

  TCPIP_UNUSED_PARAMETER(CtrlIdx);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  /* #10 Forward path-MTU callback to TCP subcomponent in case it is configured. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  TcpIp_Tcp_CbkPathMtuChg(SockAddrPtr, PathMtuSize);
#else
  TCPIP_UNUSED_PARAMETER(SockAddrPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(PathMtuSize);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  /* #20 Forward path-MTU callback to UDP subcomponent in case it is configured. */
} /* TcpIp_Cbk_VPathMtuChg() */

/***********************************************************************************************************************
 *  TcpIp_Cbk_VAddrResTimeout
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VAddrResTimeout(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestAddrPtr )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_LOCALADDRID(LocalAddrId));  /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_ASSERT_RET((DestAddrPtr != NULL_PTR));

  TCPIP_UNUSED_PARAMETER(LocalAddrId);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  CANOE_WRITE_STRING_2("TcpIp_Cbk_VAddrResTimeout: function called, LocalAddrId = %d, AddrType = %d", LocalAddrId, DestAddrPtr->sa_family);
  /* #10 Forward the timeout event to the TCP subcomponent in case it is configured. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  /* check all sockets in TCP */
  TcpIp_Tcp_VAddrResTimeout(DestAddrPtr);
#endif
  /* #20 Forwarding the timeout event to the UDP subcomponent is not required because UDP is 'fire and forget'. */
  /* check all sockets in UDP */
  /* nothing to be done */
  /* There is no stored UDP TxRequest if no ARP resolution was available (and therefore no IP buffer was provided). So
     this function should never be called after all. */
}

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VSockTcpStateChgAccepted
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VSockTcpStateChgAccepted(
  TcpIp_SockIdxType SockIdx,
  uint8 SoUserIdx )
{
  Std_ReturnType retVal = E_NOT_OK;

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_SOCKET_ID(SockIdx));

  /* #10 Check the validity of the user of the socket. */
  if(SoUserIdx < TCPIP_UNDEFINED_SOCK_OWNER_IDX)
  {
    /* #20 Accept the new TCP connection on the derived socket identified by the given sock idx. */
    /* 'SockIdx' is the new derived socket that is used for the TCP connection (not the listen socket!) */
    uint8 ListenSockIdx = TcpIp_SocketList[SockIdx].TcpMasterListenSockId;
    TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_CONNECTED;
    /* #30 Inform the upper layers about the new, accepted, TCP connection. */
    retVal = TcpIp_TcpAcceptedFunctions(SoUserIdx, ListenSockIdx, TCPIP_SOCKIDX2SOCKHND(SockIdx), (TcpIp_SockAddrType*)&(TcpIp_SocketList[SockIdx].RemSock)); /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  }
  /* else: Invalid user index. Leave retVal at value E_NOT_OK. */

  return retVal;
} /* TcpIp_VSockTcpStateChgAccepted() */
#endif
/* (STD_ON == TCPIP_SUPPORT_TCP) */

/***********************************************************************************************************************
 *  TcpIp_VIpGetRandNoFct
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, TCPIP_CODE) TcpIp_VIpGetRandNoFct(void)
{
  /* #10 Call the random number function configured in the generated data. */
  return TcpIp_GetRandNoFct();
} /* TcpIp_VIpGetRandNoFct() */

/***********************************************************************************************************************
 *  TcpIp_VIpTransmitCancel
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VIpTransmitCancel(
    TCPIP_P2V(TcpIp_IpTxRequestDescriptorType) IpTxReqDescrPtr,
    TcpIp_DomainType                           Domain,
    uint8                                      UlTxReqTabIdx,
    uint8                                      IpProtocol)
{
  /* TxConfirmation is always set to 'FALSE' */

#if (TCPIP_SUPPORT_IPV4 != TCPIP_SUPPORT_IPV6)
  TCPIP_UNUSED_PARAMETER(Domain);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_OFF)
  TCPIP_UNUSED_PARAMETER(IpProtocol);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  /* #10 Forward call to particular IpV4 or IpV6 subcomponent, based on the domain. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
  if(IPBASE_AF_INET == Domain)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
# endif
  {
    /* IpV4 destination address is set to '0' since it is not used in IpV4 for this usecase */
    (void)IpV4_Ip_Transmit(IpTxReqDescrPtr, 0, IpProtocol, FALSE, UlTxReqTabIdx, FALSE);
  }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
  else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  {
    TCPIP_UNUSED_PARAMETER(UlTxReqTabIdx);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

    IpV6_CancelTransmit(IpTxReqDescrPtr);
  }
#endif
} /* TcpIp_VIpTransmitCancel() */

/***********************************************************************************************************************
 *  TcpIp_VRstCtrl
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VRstCtrl(TcpIp_LocalAddrIdType LocalAddrId)
{
  TcpIp_SockIdxType sockIdx;

#if (TCPIP_SUPPORT_TCP == STD_ON)
  TcpIp_SockAddrInXType sockAddr;

  (void)TcpIp_GetIpAddr(LocalAddrId, (TCPIP_P2V(TcpIp_SockAddrType))&sockAddr, NULL_PTR, NULL_PTR);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#endif
  /* #10 Check all sockets that are bound to the given local address id whether they are in state 'listen' or 'connected'. */
  for (sockIdx = 0; sockIdx < TcpIp_VCfgGetMaxNumSocks(); sockIdx++)
  {
    boolean initSocket = FALSE;
    /* #20 In case the socket is in the desired state, reset the socket, according to its associated protocol, TCP or UDP. */
    switch (TcpIp_SockListenActive[sockIdx].ConnStat)
    {
    case TCPIP_SOCK_CONN_LISTEN_ENABLED:
      /* Reset TCP/UDP listen socket if it is bound to the LocalAddrId. */
      if (TcpIp_SocketList[sockIdx].BindLocalAddrId == LocalAddrId)
      {
#if (TCPIP_SUPPORT_TCP == STD_ON)
        if (sockIdx >= TcpIp_VCfgGetMaxNumUdpSocks())
        {
          TcpIp_Tcp_ResetSocket(sockIdx);
        }
        else
#endif
        {
          TcpIp_Udp_ResetSocket(sockIdx);
        }

        initSocket = TRUE;
      }
      break;

#if (TCPIP_SUPPORT_TCP == STD_ON)
    case TCPIP_SOCK_CONN_CONNECTED:
      if (TcpIp_VSockIpAddrIsEqual((const IpBase_SockAddrType*)(&TcpIp_SocketList[sockIdx].LocSock), (const IpBase_SockAddrType*)&sockAddr) == TRUE)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* TCP Socket. */
        TcpIp_Tcp_ResetSocket(sockIdx);
        initSocket = TRUE;
      }
      break;
#endif

    default:
      /* Socket not bound or established. -> Nothing to do. */
      break;
    }
    /* #30 In case the socket was reset, initialize its admin data. */
    if (initSocket == TRUE)
    {
      TcpIp_VInitSocket(sockIdx);
    }
  }
} /* TcpIp_VRstCtrl() */

#if (TCPIP_ENABLE_DLISTS == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_DListRawAddAfter()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddAfter(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              NewIdx,
  TcpIp_DListIdxType              AfterIdx)
{
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != NodeSetPtr,  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != FirstIdxPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != LastIdxPtr,  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  NodeSetPtr[NewIdx].PrevIdx = AfterIdx;

  /* #10 Insert the new node at the desired position, in case the index to insert the new node after is not the last element. */
  if (TCPIP_DLIST_END_IDX != AfterIdx)
  {
    /* insert after existing node */
    NodeSetPtr[NewIdx].NextIdx = NodeSetPtr[AfterIdx].NextIdx;
    NodeSetPtr[AfterIdx].NextIdx = NewIdx;
  }
  else
  {
    /* #20 Otherwise insert the new node as the first element. */
    NodeSetPtr[NewIdx].NextIdx = (*FirstIdxPtr);
    (*FirstIdxPtr) = NewIdx;
  }

  /* #30 Complete the linkage of the new node, depending on the node being the last node or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[NewIdx].NextIdx)
  {
    /* new node is not last node */
    NodeSetPtr[NodeSetPtr[NewIdx].NextIdx].PrevIdx = NewIdx;
  }
  else
  {
    /* new node is last node */
    (*LastIdxPtr) = NewIdx;
  }

} /* TcpIp_DListRawAddAfter() */

/**********************************************************************************************************************
 *  TcpIp_DListRawAddRangeAfter()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddRangeAfter(
  TCPIP_P2V(TcpIp_DListNodeType) NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  LastIdxPtr,
  TcpIp_DListIdxType             NewStartIdx,
  TcpIp_DListIdxType             NewEndIdx,
  TcpIp_DListIdxType             AfterIdx)
{
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != NodeSetPtr,  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != FirstIdxPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != LastIdxPtr,  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);

  if (E_OK != TcpIp_VDbgDListCheckChain(NodeSetPtr, NewStartIdx, NewEndIdx, (TCPIP_P2V(uint8))NULL_PTR))
  {
    TcpIp_DetReportError(TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_PARAM);
    return;
  }
#endif

  NodeSetPtr[NewStartIdx].PrevIdx = AfterIdx;

  /* #10 Insert the new node-range at the desired position, in case the index to insert the new node-range after is not the last element. */
  if (TCPIP_DLIST_END_IDX != AfterIdx)
  {
    /* insert after existing node */
    NodeSetPtr[NewEndIdx].NextIdx = NodeSetPtr[AfterIdx].NextIdx;
    NodeSetPtr[AfterIdx].NextIdx  = NewStartIdx;
  }
  else
  {
    /* #20 Otherwise insert the new node-range at the beginning. */
    /* insert as first node */
    NodeSetPtr[NewEndIdx].NextIdx = (*FirstIdxPtr);
    (*FirstIdxPtr) = NewStartIdx;
  }

  /* #30 Complete the linkage of the new node-range, depending on the node-range being at the end or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[NewEndIdx].NextIdx)
  {
    /* new node is not last node */
    NodeSetPtr[NodeSetPtr[NewEndIdx].NextIdx].PrevIdx = NewEndIdx;
  }
  else
  {
    /* new node is last node */
    (*LastIdxPtr) = NewEndIdx;
  }

} /* TcpIp_DListRawAddRangeAfter() */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

/**********************************************************************************************************************
 *  TcpIp_DListRawRemove()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawRemove(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType             RemIdx)
{
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != NodeSetPtr,               TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != FirstIdxPtr,              TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != LastIdxPtr,               TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(TCPIP_DLIST_END_IDX != (*FirstIdxPtr), TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(TCPIP_DLIST_END_IDX != (*LastIdxPtr),  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(TCPIP_DLIST_END_IDX != RemIdx,         TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  /* #10 Remove the node to be removed from the its position, in case the index to remove the node is not the last element. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemIdx].PrevIdx)
  {
    /* node is not first node */
    NodeSetPtr[NodeSetPtr[RemIdx].PrevIdx].NextIdx = NodeSetPtr[RemIdx].NextIdx;
  }
  else
  {
    /* #20 Otherwise remove the desired node at the beginning. */
    /* remove first node */
    (*FirstIdxPtr) = NodeSetPtr[RemIdx].NextIdx;
  }

  /* #30 Re-link the remaining nodes, depending on whether the deleted node was placed at the end or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemIdx].NextIdx)
  {
    /* node is not last node */
    NodeSetPtr[NodeSetPtr[RemIdx].NextIdx].PrevIdx = NodeSetPtr[RemIdx].PrevIdx;
  }
  else
  {
    /* remove last node */
    (*LastIdxPtr) = NodeSetPtr[RemIdx].PrevIdx;
  }

  NodeSetPtr[RemIdx].PrevIdx = TCPIP_DLIST_END_IDX;
  NodeSetPtr[RemIdx].NextIdx = TCPIP_DLIST_END_IDX;

} /* TcpIp_DListRawRemove() */

/**********************************************************************************************************************
 *  TcpIp_DListRawRemoveRange()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DListRawRemoveRange(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType             RemStartIdx,
  TcpIp_DListIdxType             RemEndIdx)
{
  TcpIp_DListIdxType Idx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint8 ChainLength = 1;

#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnValue(NULL_PTR != NodeSetPtr,  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER, 0);
  TcpIp_CheckDetErrorReturnValue(NULL_PTR != FirstIdxPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER, 0);
  TcpIp_CheckDetErrorReturnValue(NULL_PTR != LastIdxPtr,  TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER, 0);
#endif

#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  if (E_OK != TcpIp_VDbgDListCheckChain(NodeSetPtr, RemStartIdx, RemEndIdx, (TCPIP_P2V(uint8))NULL_PTR))
  {
    TcpIp_DetReportError(TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_PARAM);
    return 0;
  }
#endif

  /* count number of elements in chain */
  Idx = RemStartIdx;

  while (RemEndIdx != Idx)
  {
    ChainLength++;
    Idx = NodeSetPtr[Idx].NextIdx;
  }

  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemStartIdx].PrevIdx)
  {
    /* node is not first node */
    NodeSetPtr[NodeSetPtr[RemStartIdx].PrevIdx].NextIdx = NodeSetPtr[RemEndIdx].NextIdx;
  }
  else
  {
    /* remove first node */
    (*FirstIdxPtr) = NodeSetPtr[RemEndIdx].NextIdx;
  }

  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemEndIdx].NextIdx)
  {
    /* node is not last node */
    NodeSetPtr[NodeSetPtr[RemEndIdx].NextIdx].PrevIdx = NodeSetPtr[RemStartIdx].PrevIdx;
  }
  else
  {
    /* remove last node */
    (*LastIdxPtr) = NodeSetPtr[RemStartIdx].PrevIdx;
  }

  NodeSetPtr[RemStartIdx].PrevIdx = TCPIP_DLIST_END_IDX;
  NodeSetPtr[RemEndIdx].NextIdx   = TCPIP_DLIST_END_IDX;

  return ChainLength;
} /* TcpIp_DListRawRemoveRange() */

/**********************************************************************************************************************
 *  TcpIp_DListInitDesc()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListInitDesc(
  TCPIP_P2V(TcpIp_DListDescType) DescPtr,
  TCPIP_P2V(TcpIp_DListNodeType) NodePtr,
  uint8                        NodeCount)
{
  uint8 DataIdx = NodeCount;
  DescPtr->NodePtr = NodePtr;

#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != DescPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != NodePtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  if (0 < NodeCount)
  {
    DescPtr->FirstFreeIdx = 0;
    DescPtr->LastFreeIdx  = (TcpIp_DListIdxType)(NodeCount - 1);

    while (0 < DataIdx)
    {
      DataIdx--;

      if (DescPtr->FirstFreeIdx < DataIdx)
      {
        NodePtr[DataIdx].PrevIdx = (TcpIp_DListIdxType)(DataIdx - 1);
      }
      else
      {
        NodePtr[DataIdx].PrevIdx = TCPIP_DLIST_END_IDX;
      }

      if (DescPtr->LastFreeIdx > DataIdx)
      {
        NodePtr[DataIdx].NextIdx = (TcpIp_DListIdxType)(DataIdx + 1);
      }
      else
      {
        NodePtr[DataIdx].NextIdx = TCPIP_DLIST_END_IDX;
      }
    }
  }
  else
  {
    DescPtr->FirstFreeIdx = TCPIP_DLIST_END_IDX;
    DescPtr->LastFreeIdx  = TCPIP_DLIST_END_IDX;
  }

  DescPtr->ElementCount = NodeCount;

} /* TcpIp_DListInitDesc() */

/**********************************************************************************************************************
 *  TcpIp_DListInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListInit(
  TCPIP_P2V(TcpIp_DListType)     ListPtr,
  TCPIP_P2V(TcpIp_DListDescType) DescPtr)
{
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != ListPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != DescPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  ListPtr->DescPtr      = DescPtr;
  ListPtr->FirstIdx     = TCPIP_DLIST_END_IDX;
  ListPtr->LastIdx      = TCPIP_DLIST_END_IDX;
  ListPtr->ElementCount = 0;

} /* TcpIp_DListInit() */

/**********************************************************************************************************************
 *  TcpIp_DListAddElement()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DListAddElement(
  TCPIP_P2V(TcpIp_DListType)    DListPtr,
  TcpIp_DListIdxType           AfterIdx,
  TCPIP_P2V(TcpIp_DListIdxType) NewIdxPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  TCPIP_ASSERT(NULL_PTR != DListPtr);

  if (TCPIP_DLIST_END_IDX == DListPtr->DescPtr->FirstFreeIdx)
  {
    /* no free elements */
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
    TcpIp_DetReportError(TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_DLIST_BUFFER_EMPTY);
#endif
    /* leave retVal on value E_NOT_OK */
  }
  else
  {
    /* get first element from free list */
    TcpIp_DListIdxType NewIdx = DListPtr->DescPtr->FirstFreeIdx;
    TcpIp_DListRawRemove(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, NewIdx);
    DListPtr->DescPtr->ElementCount--;

    /* add new element to list */
    TcpIp_DListRawAddAfter(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, NewIdx, AfterIdx);
    DListPtr->ElementCount++;
    (*NewIdxPtr) = NewIdx;

    retVal = E_OK;
  }
  return retVal;
} /* TcpIp_DListAddElement() */

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElementRange()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElementRange(
  TCPIP_P2V(TcpIp_DListType) DListPtr,
  TcpIp_DListIdxType        RemStartIdx,
  TcpIp_DListIdxType        RemEndIdx)
{
  uint8 ChainCount;

#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != DListPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  /* remove element from list */
  ChainCount = TcpIp_DListRawRemoveRange(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, RemStartIdx, RemEndIdx);
  DListPtr->ElementCount -= ChainCount;

  /* add removed element to beginning of free list */
  TcpIp_DListRawAddRangeAfter(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, RemStartIdx, RemEndIdx, DListPtr->DescPtr->FirstFreeIdx);
  DListPtr->DescPtr->ElementCount += ChainCount;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_DListRemoveElementRange() */

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElement()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElement(
  TCPIP_P2V(TcpIp_DListType) DListPtr,
  TcpIp_DListIdxType        RemIdx)
{
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != DListPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  /* remove element from list */
  TcpIp_DListRawRemove(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, RemIdx);
  DListPtr->ElementCount--;

  /* add removed element to beginning of free list */
  TcpIp_DListRawAddAfter(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, RemIdx, DListPtr->DescPtr->FirstFreeIdx);
  DListPtr->DescPtr->ElementCount++;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_DListRemoveElement() */

/**********************************************************************************************************************
 *  TcpIp_DListClear()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListClear(
  TCPIP_P2V(TcpIp_DListType) DListPtr)
{
#if (TCPIP_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  TcpIp_CheckDetErrorReturnVoid(NULL_PTR != DListPtr, TCPIP_API_ID_V_INTERNAL_FUNCTION, TCPIP_E_INV_POINTER);
#endif

  if (!TCPIP_DLIST_IS_EMPTY(*DListPtr))
  {
    TcpIp_DListRemoveElementRange(DListPtr, DListPtr->FirstIdx, DListPtr->LastIdx);
  }

} /* TcpIp_DListClear() */

#endif

/**********************************************************************************************************************
 *  TcpIp_CmpLLAddr()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_CmpLLAddr(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) LLAddr1Ptr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) LLAddr2Ptr)
{
  /* #10 Assume that the 2 link layer addresses are identical. */
  boolean retVal = TRUE;
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  TCPIP_ASSERT(LLAddr1Ptr != NULL_PTR);
  TCPIP_ASSERT(LLAddr2Ptr != NULL_PTR);

  /* #20 In case at least 1 byte of the 6 bytes of the link layer address byte arrays is NOT identical: Set the return value to FALSE. */
  if ((LLAddr1Ptr[5] != LLAddr2Ptr[5]) ||
      (LLAddr1Ptr[4] != LLAddr2Ptr[4]) ||
      (LLAddr1Ptr[3] != LLAddr2Ptr[3]) ||
      (LLAddr1Ptr[2] != LLAddr2Ptr[2]) ||
      (LLAddr1Ptr[1] != LLAddr2Ptr[1]) ||
      (LLAddr1Ptr[0] != LLAddr2Ptr[0]))
  {
    retVal = FALSE;
  }

  return retVal;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* TcpIp_CmpLLAddr() */

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:

   module global deviations -> see file TcpIp.c

   TCPIP_PRIV-specific deviations:
   (none)
*/

/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Priv.c
 *********************************************************************************************************************/
