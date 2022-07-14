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
 *         \file  TcpIp_Udp.c
 *        \brief  Implementation of User Datagram Protocol (UDP)
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

#define TCPIP_UDP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_Cfg.h"
#include "TcpIp_Udp.h"
#include "TcpIp_Lcfg.h"
#include "TcpIp_Udp_Cbk.h"
#include "TcpIp_Priv.h"
#include "TcpIp_Cbk.h"
#include "IpBase.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# include "TcpIp_IpV4.h"
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# include "TcpIp_IpV6.h"
#endif
#include "IpBase_Copy.h"
#include "IpBase_Sock.h"

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (IPV4_ADDR_ID_ANY != TCPIP_IP_ADDR_ID_ANY)
#  error "Error: IPV4_ADDR_ID_ANY != TCPIP_IP_ADDR_ID_ANY"
# endif
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (IPV6_ADDR_ID_ANY != TCPIP_IP_ADDR_ID_ANY)
#  error "Error: IPV6_ADDR_ID_ANY != TCPIP_IP_ADDR_ID_ANY"
# endif
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

#define TCPIP_UDP_EMPTY_PORT     0x0000u
#define TCPIP_UDP_EMPTY_CHKSUM   0x0000u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */
#define TCPIP_UDP_INC_TXQUEUE_WRITE_POS()        TcpIp_Udp_TxQueueWritePos = (uint8)(TcpIp_Udp_TxQueueWritePos +1u) & (uint8)((TcpIp_Udp_VCfgGetMaxTxQueueSize())-1u)

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
# define TCPIP_UDP_RETRY_QUEUE_IDX_INV            TCPIP_DLIST_END_IDX
#else
# define TCPIP_UDP_RETRY_QUEUE_IDX_INV            0
#endif

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
# define TcpIp_Udp_VIsTxConfirmationEnabled(UDP_SOCKET_ID)  (TcpIp_Udp_Socket_AssignedTxReqListIndex[(UDP_SOCKET_ID)] != 0xFF)
#else
# define TcpIp_Udp_VIsTxConfirmationEnabled(UDP_SOCKET_ID)  (FALSE)
#endif

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( defined TCPIP_ENABLE_INT_DEBUG_COUNTERS )
STATIC VAR(uint32, TCPIP_VAR_NOINIT) TcpIp_Udp_RxIndCounter;
STATIC VAR(uint32, TCPIP_VAR_NOINIT) TcpIp_Udp_TransmitCounter;
#endif  /* ( defined TCPIP_ENABLE_INT_DEBUG_COUNTERS ) */

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  TcpIp_Udp_VTransmit()
**********************************************************************************************************************/
/*! \brief      Transmits or queues the transmission of a UDP datagram.
 *  \details    -
 *  \param[in]  udpSocketId     Index of the UDP socket.
 *  \param[in]  ipTxReqDescPtr  IP TX descriptor that was provided by IpVX_ProvideTxBuffer().
 *  \param[in]  ipPayloadBufPtr Buffer where the UDP header and payload will be written.
 *  \param[in]  udpPayloadLen   Length of the UDP payload. [bytes]
 *  \param[in]  destSockAddrPtr Destination IP address and port.
 *  \param[in]  txReqIdx        TX request index that is passed to the IP layer and will be returned in TX confirmation.
 *  \return     E_OK            UDP datagram transmitted or transmit queued.
 *  \return     E_NOT_OK        Transmit not possible.
 *  \note       The payload of the UDP packet must be written in the provided IP buffer before.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        -
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_VTransmit(
  TcpIp_SocketIdType                         udpSocketId,
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) ipTxReqDescPtr,
  TCPIP_P2V(uint8)                           ipPayloadBufPtr,
  uint16                                     udpPayloadLen,
  TCPIP_P2C(IpBase_SockAddrType)             destSockAddrPtr,
  uint8                                      txReqIdx);

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Udp_VTxReqListHandleOverflow
 **********************************************************************************************************************/
/*! \brief      Verifies that there is sufficient space in the TX request table of a socket.
 *  \details    -
 *  \param[in]  socketId     Index of the UDP socket.
 *  \return     E_OK         UDP datagram transmitted or transmit queued.
 *  \return     E_NOT_OK     Transmit not possible.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        -
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_VTxReqListHandleOverflow(TcpIp_SocketIdType socketId);
#endif

#if (TCPIP_UDP_CHECK_RX_CHECKSUM == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Udp_VValidateChecksum
 **********************************************************************************************************************/
/*! \brief      Validates the checksum of a received UDP datagram.
 *  \details    -
 *  \param[in]  ReceivedChecksum  Received checksum value that was stored inside the UDP header.
 *  \param[in]  RxSockLocAddrPtr  Destination IP address of the received UDP packet.
 *  \param[in]  RxSockRemAddrPtr  Source IP address of the received UDP packet.
 *  \param[in]  IpPayloadPtr      Payload of the received IP packet (starting with the UDP header).
 *  \param[in]  IpPayloadLen      Length of the IP payload in bytes.
 *  \return     E_OK              Checksum of received UDP datagram matches ReceivedChecksum.
 *  \return     E_NOT_OK          Checksum of received UDP datagram does not match ReceivedChecksum.
 *  \context    ANY
 *  \reentrant  TRUE
 *  \pre        -
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_VValidateChecksum(
    uint16                         ReceivedChecksum,
    TCPIP_P2C(IpBase_SockAddrType) RxSockLocAddrPtr,
    TCPIP_P2C(IpBase_SockAddrType) RxSockRemAddrPtr,
    TCPIP_P2C(uint8)               IpPayloadPtr,
    uint16                         IpPayloadLen);
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  TcpIp_Udp_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration of the User Datagram
 *              Protocol and may be used to initialize the data structures.
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \note       No configuration variant support implemented, so only TCPIP_CONFIG_VARIANT_1 (compile time) is available.
 *  \context    initialization
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_Init( void )
{
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  uint8 TxReqListIdx;
#endif

#if ( defined TCPIP_ENABLE_INT_DEBUG_COUNTERS )
  /* Debug variables: */
  TcpIp_Udp_RxIndCounter = 0;
  TcpIp_Udp_TransmitCounter = 0;
#endif
  /* TCPIP_DEV_ERROR_DETECT == STD_ON */

  TcpIp_Udp_DynamicPortCount = 0;

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  for(TxReqListIdx = 0; TcpIp_VCfgGetUdpTxReqListNum() > TxReqListIdx; TxReqListIdx++)
  {
    TcpIp_Udp_TxReqListFillNum[TxReqListIdx]         = 0;
    TcpIp_Udp_TxReqListWritePos[TxReqListIdx]        = 0;
    TcpIp_Udp_TxReqListReadPos[TxReqListIdx]         = 0;
    TcpIp_Udp_TxReqList_IsUsedBySocket[TxReqListIdx] = TCPIP_INV_SOCK_IDX;
  }
#endif

  for(SockIdx = 0; ((TcpIp_SockIdxType)TcpIp_VCfgGetMaxNumUdpSocks()) > SockIdx; SockIdx++)
  {
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
    TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx] = 0xFF;  /* invalid index */
#endif
  }

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
  TcpIp_DListInitDesc(&TcpIp_Udp_TxRetrQueuePoolDesc, &TcpIp_Udp_TxRetrQueueElementChain[0], TCPIP_UDP_TX_RETRY_QUEUE_NUM_TOTAL);
  for(SockIdx = 0; ((TcpIp_SockIdxType)TcpIp_VCfgGetMaxNumUdpSocks()) > SockIdx; SockIdx++)
  {
    TcpIp_DListInit(&TcpIp_Udp_TxRetrQueueForSock[SockIdx], &TcpIp_Udp_TxRetrQueuePoolDesc);
    TcpIp_Udp_TxRetrQueueMaxNum[SockIdx] = TCPIP_UDP_TX_RETRY_QUEUE_NUM_TOTAL;  /* init with size TCPIP_UDP_TX_RETRY_QUEUE_NUM_TOTAL, can be changed or disables using SetSocketOption */
  }
#endif

  return;
}

/***********************************************************************************************************************
 *  TcpIp_Udp_CloseSocket
 **********************************************************************************************************************/
/*! \brief      This API call closes the UDP socket and resets all relevant internal variables.
 *  \param[in]  SockIdx             socket index
 *  \note
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_CloseSocket(
    TcpIp_SocketIdType SockIdx )
{
  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_INIT,                      TCPIP_UDP_API_ID_CLOSE_SOCKET, TCPIP_E_NOTINIT )
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_VALID_UDPSOCKIDX(SockIdx), TCPIP_UDP_API_ID_CLOSE_SOCKET, TCPIP_E_INV_SOCK_ID )
  /* ------------------------------------------------- */

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  {
    if(TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx] != 0xFF)  /* TxConfirmation is enabled */
    {
      uint8 TxReqListIdx;

      /* reset variables */
      TxReqListIdx = TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx];

      TcpIp_Udp_TxReqListFillNum[TxReqListIdx]         = 0;
      TcpIp_Udp_TxReqListWritePos[TxReqListIdx]        = 0;
      TcpIp_Udp_TxReqListReadPos[TxReqListIdx]         = 0;
      TcpIp_Udp_TxReqList_IsUsedBySocket[TxReqListIdx] = TCPIP_INV_SOCK_IDX;

      TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx] = 0xFF;  /* invalid index */
    }
    else
    {
      /* nothing to do */
    }
  }
#endif

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)

  if(!TCPIP_DLIST_IS_EMPTY(TcpIp_Udp_TxRetrQueueForSock[SockIdx]))
  {
    /* cancel transmit of all retry elements for this socket (release ethernet buffers) */
    TcpIp_DListIdxType ListIdx = TcpIp_Udp_TxRetrQueueForSock[SockIdx].FirstIdx;

    while (TCPIP_DLIST_END_IDX != ListIdx)
    {
      (void)TcpIp_VIpTransmitCancel(&TcpIp_Udp_TxRetrQueueElements[ListIdx].IpTxRequestDesc, ((TCPIP_P2C(TcpIp_SockAddrType))&TcpIp_Udp_TxRetrQueueElements[ListIdx].Destination)->domain, 0, TCPIP_PROTOCOL_UDP);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

      TCPIP_DLIST_SET_NEXT(ListIdx, TcpIp_Udp_TxRetrQueueForSock[SockIdx]);
    }
  }

  TcpIp_DListClear(&TcpIp_Udp_TxRetrQueueForSock[SockIdx]);
  TcpIp_Udp_TxRetrQueueMaxNum[SockIdx] = TCPIP_UDP_TX_RETRY_QUEUE_NUM_TOTAL;  /* init with size TCPIP_UDP_TX_RETRY_QUEUE_NUM_TOTAL, can be changed or disables using SetSocketOption */
#endif
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Udp_RxIndication
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
FUNC(TcpIp_Udp_ReturnType, TCPIP_CODE) TcpIp_Udp_RxIndication(
    TcpIp_LocalAddrIdType          LocalAddrId,
    TCPIP_P2V(IpBase_SockAddrType) RxSockRemAddrPtr,
    TCPIP_P2V(uint8)               IpPayloadPtr,
    uint16                         IpPayloadLen,
    boolean                        ChecksumValidated)
{
  TcpIp_Udp_ReturnType retVal = TCPIP_UDP_E_NOT_OK;

  TCPIP_ASSERT(TCPIP_COND_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_LOCALADDRID(LocalAddrId));
  TCPIP_ASSERT(RxSockRemAddrPtr != NULL_PTR);
  TCPIP_ASSERT(IpPayloadPtr     != NULL_PTR);

#if ( defined TCPIP_ENABLE_INT_DEBUG_COUNTERS )
  /* Debug variable: */
  TcpIp_Udp_RxIndCounter++;
#endif
  /* TCPIP_DEV_ERROR_DETECT == STD_ON */

  /* #10 Ignore packet if the UDP header is incomplete. */
  if (IpPayloadLen >= TCPIP_UDP_HDR_LEN_BYTE)
  {
    TCPIP_P2V(TcpIp_Udp_HdrType) udpHdrPtr     = (TCPIP_P2V(TcpIp_Udp_HdrType))IpPayloadPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
    uint16                       udpHdrLenByte = IPBASE_HTON16(udpHdrPtr->LenByte);

    /* #20 Ignore datagram if length in UDP header is invalid. */
    if ((udpHdrLenByte >= TCPIP_UDP_HDR_LEN_BYTE) && (udpHdrLenByte <= IpPayloadLen))
    {
      TcpIp_SockAddrInXType  rxSockLocAddr;

      /* #30 Retrieve source IP address of the UDP datagram from IP layer. */
      if (TcpIp_GetIpAddr(LocalAddrId, (TCPIP_P2V(TcpIp_SockAddrType))&rxSockLocAddr, NULL_PTR, NULL_PTR) == E_OK)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* #40 Validate checksum of UDP header if not already done by lower layer. */
        if (ChecksumValidated == FALSE)
        {
          if (TcpIp_Udp_VValidateChecksum(udpHdrPtr->Checksum, (TCPIP_P2C(IpBase_SockAddrType))&rxSockLocAddr, RxSockRemAddrPtr, IpPayloadPtr, udpHdrLenByte) == E_OK)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
          {
            ChecksumValidated = TRUE;
          }
        }

        if (ChecksumValidated == TRUE)
        {
          TcpIp_SockIdxType sockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
          if(IPBASE_AF_INET == RxSockRemAddrPtr->sa_family)
# endif
          {
            ((IpBase_SockAddrInType*)&rxSockLocAddr)->sin_port   = udpHdrPtr->TgtPort;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
            ((IpBase_SockAddrInType*)RxSockRemAddrPtr)->sin_port = udpHdrPtr->SrcPort;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
          }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
          else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
          {
            ((IpBase_SockAddrIn6Type*)&rxSockLocAddr)->sin6_port   = udpHdrPtr->TgtPort;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
            ((IpBase_SockAddrIn6Type*)RxSockRemAddrPtr)->sin6_port = udpHdrPtr->SrcPort;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
          }
#endif

          /* #50 Pass the packet to the corresponding socket owner or ignore packet if no matching socket is found. */
          sockIdx = TcpIp_VRxUdpSockIdxIdent((TCPIP_P2C(IpBase_SockAddrType))&rxSockLocAddr, LocalAddrId);  /* Identify socket */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */

          if (TCPIP_INV_SOCK_IDX != sockIdx)
          {
            uint16 udpPayloadLen = (uint16)(IpPayloadLen - TCPIP_UDP_HDR_LEN_BYTE);
            /* ASR-API */
            /* forward message to upper layer (socket) */

            IpBase_PbufType DataDesc;
            DataDesc.payload = (uint8*) (&(udpHdrPtr[1]));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
            DataDesc.len     = udpPayloadLen;
            DataDesc.totLen  = udpPayloadLen;

            TcpIp_VSockRxIndicationAsr42x( sockIdx, RxSockRemAddrPtr, &DataDesc );
            retVal = TCPIP_UDP_OK;
          }
          else
          {
            /* there is no corresponding socket found -> drop message, or send ICMP message */
            retVal = TCPIP_UDP_E_NO_MATCHING_SOCKET;
          }
        }
      }
    }
  }

  return retVal;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Udp_VTxReqListHandleOverflow
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_VTxReqListHandleOverflow(TcpIp_SocketIdType socketId)
{
  Std_ReturnType retVal = E_OK;

  /* #10 Check if TX confirmation is enabled for the socket. */
  if (TcpIp_Udp_Socket_AssignedTxReqListIndex[socketId] != 0xFF)
  {
    uint8 TxReqListIdx = TcpIp_Udp_Socket_AssignedTxReqListIndex[socketId];

    /* #20 Check and if there is enough space in TX request list. */
    if (TcpIp_Udp_TxReqListFillNum[TxReqListIdx] == TcpIp_Udp_VCfgGetTxReqListSize(TxReqListIdx))
    {
      /* TcpIp_Udp_TxReqList is full */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
#endif

/***********************************************************************************************************************
 *  TcpIp_Udp_RequestTxBuffer
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Udp_RequestTxBuffer(
    TcpIp_SocketIdType              socketId,
    TCPIP_P2C(IpBase_SockAddrType)  destinationPtr,
    TCPIP_P2V(uint8)               *bufPtr,
    TCPIP_P2V(uint16)               bufLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType bufRetValue;
  boolean           forceProvidePuffer;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_UDPSOCKIDX(socketId));
  TCPIP_ASSERT_RETVAL(destinationPtr != NULL_PTR, BUFREQ_E_NOT_OK);
  TCPIP_ASSERT(bufPtr    != NULL_PTR);
  TCPIP_ASSERT(bufLenPtr != NULL_PTR);

  /* #10 Check if TX confirmation is enabled for the socket and if there is enough space in TX request list. */
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  if (TcpIp_Udp_VTxReqListHandleOverflow(socketId) == E_OK)
#endif
  {
    /* there is a valid free request table element available */
    /* -> check if IP can provide a buffer / has already resolved the address */
    uint16           ipBufferLen = (*bufLenPtr) + TCPIP_UDP_HDR_LEN_BYTE;
    TCPIP_P2V(uint8) ipBufferPtr = NULL_PTR;

    /* #20 Check if TX request list queuing is enabled. */
#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
    if (TcpIp_Udp_TxRetrQueueMaxNum[socketId] != 0)
    {
      forceProvidePuffer = TRUE;
    }
    else
#endif
    {
      forceProvidePuffer = FALSE;
    }

    /* #30 Get TX buffer from IP layer. */
    switch (destinationPtr->sa_family)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    case IPBASE_AF_INET:
      {
        IpV4_AddrIdType ipAddrId = TcpIp_SocketList[socketId].TxIpAddrId;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

        bufRetValue = IpV4_Ip_ProvideTxBuffer(&ipAddrId, socketId, (TCPIP_P2C(IpBase_SockAddrInType))destinationPtr,  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
                        &ipBufferPtr, &ipBufferLen,
                        &TcpIp_Udp_IpTxRequestDescriptor[socketId], forceProvidePuffer);

        break;
      }
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    case IPBASE_AF_INET6:
      {
        IpV6_AddrIdType ipAddrId = TcpIp_SocketList[socketId].TxIpAddrId;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

        bufRetValue = IpV6_ProvideTxBuffer(&ipAddrId, socketId, (TCPIP_P2C(IpV6_AddrType))&(((TCPIP_P2C(IpBase_SockAddrIn6Type))destinationPtr)->sin6_addr),  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
                        &ipBufferPtr, &ipBufferLen,
                        &TcpIp_Udp_IpTxRequestDescriptor[socketId], forceProvidePuffer);

        break;
      }
#endif

    default:
      {
        /* invalid address family */
        bufRetValue = BUFREQ_E_NOT_OK;
      }
    }

    /* #40 Update output parameters and socket variables. */
    if (ipBufferPtr != NULL_PTR)
    {
      (*bufPtr)     = &ipBufferPtr[TCPIP_UDP_HDR_LEN_BYTE];
      (*bufLenPtr)  = (uint16)(ipBufferLen - TCPIP_UDP_HDR_LEN_BYTE);

      TcpIp_Udp_TxReqIpBufPtr[socketId] = ipBufferPtr;
    }
  }
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  else
  {
    bufRetValue = BUFREQ_E_BUSY;
  }
#endif

  return bufRetValue;
}  /* TcpIp_Udp_RequestTxBuffer */  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  TcpIp_Udp_FragmentRequestTxBuffer
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_FragmentRequestTxBuffer(
  TcpIp_SocketIdType  udpSocketId,
  IpBase_FamilyType   addrFamily,
  TCPIP_P2V(uint8)   *bufPtrPtr,
  TCPIP_P2V(uint16)   bufLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
  if (addrFamily == IPBASE_AF_INET)
  {
    retVal = IpV4_Ip_FragmentProvideTxBuffer(&TcpIp_Udp_IpTxRequestDescriptor[udpSocketId], bufPtrPtr, bufLenPtr);
  }
  else
#endif
  {
    TCPIP_UNUSED_PARAMETER(udpSocketId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    TCPIP_UNUSED_PARAMETER(bufPtrPtr);   /* PRQA S 3112 */ /* MD_MSR_14.2 */
    TCPIP_UNUSED_PARAMETER(bufLenPtr);   /* PRQA S 3112 */ /* MD_MSR_14.2 */

    /* Only IPv4 does provide multiple buffer fragments. */
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Udp_FragmentRequestTxBuffer() */

/***********************************************************************************************************************
 *  TcpIp_Udp_TransmitCancel
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Udp_TransmitCancel(
  TcpIp_SocketIdType             udpSocketId,
  TCPIP_P2C(IpBase_SockAddrType) destSockAddrPtr)
{
  TCPIP_ASSERT_RET(destSockAddrPtr != NULL_PTR);

  TcpIp_VIpTransmitCancel(&TcpIp_Udp_IpTxRequestDescriptor[udpSocketId], destSockAddrPtr->sa_family, 0, TCPIP_PROTOCOL_UDP);
}

/***********************************************************************************************************************
 *  TcpIp_Udp_Transmit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_Transmit(
  TcpIp_SocketIdType             udpSocketId,
  uint16                         udpPayloadLen,
  TCPIP_P2C(IpBase_SockAddrType) destSockAddrPtr)
{
  Std_ReturnType retVal;
  uint8 txReqIdx;

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  TCPIP_P2V(TcpIp_Udp_TxReqEleType) txReqListElePtr;
  uint8                             txReqListIdx;
#endif

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(TCPIP_COND_VALID_UDPSOCKIDX(udpSocketId));

#if ( defined TCPIP_ENABLE_INT_DEBUG_COUNTERS )
  TcpIp_Udp_TransmitCounter++;  /* for debugging purposes only */
#endif

  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Prepare entry in TX request list, if TxConfirmation is configured. */
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  txReqListIdx = TcpIp_Udp_Socket_AssignedTxReqListIndex[udpSocketId];

  if (txReqListIdx != 0xFF)  /* TxConfirmation is enabled */
  {
    /* take free TcpIp_Udp_TxReqList element. */
    txReqIdx = TcpIp_Udp_TxReqListWritePos[txReqListIdx];
    txReqListElePtr = &((TcpIp_Udp_TxReqList[txReqListIdx])[txReqIdx]);

    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* fill TxReqList element */
    txReqListElePtr->TxDataLenByte = udpPayloadLen;
    txReqListElePtr->Transmitted   = FALSE;
  }
  else
  {
    txReqIdx = 0;
    txReqListElePtr = NULL_PTR;
      
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
#else
  txReqIdx = 0;
#endif

  /* #20 Send UDP message. */
  retVal = TcpIp_Udp_VTransmit(udpSocketId, &TcpIp_Udp_IpTxRequestDescriptor[udpSocketId], TcpIp_Udp_TxReqIpBufPtr[udpSocketId], udpPayloadLen, destSockAddrPtr, txReqIdx);

  /* #30 Update entry in TX request list, if TxConfirmation is configured. */
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  if (txReqListIdx != 0xFF)
  {
    if (retVal == E_OK)
    {
      TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* update list variables */
      TcpIp_Udp_TxReqListWritePos[txReqListIdx]++;

      if (TcpIp_Udp_VCfgGetTxReqListSize(txReqListIdx) == TcpIp_Udp_TxReqListWritePos[txReqListIdx])
      {
        TcpIp_Udp_TxReqListWritePos[txReqListIdx] = 0;
      }

      TcpIp_Udp_TxReqListFillNum[txReqListIdx]++;

      TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    else
    {
      TCPIP_ASSERT_RETVAL(txReqListElePtr != NULL_PTR, E_NOT_OK);

      /* empty TxReqList element */
      txReqListElePtr->TxDataLenByte = 0;  /* PRQA S 0505 */ /* MD_TCPIP_UDP_1.2_0505_a */
    }
  }
#else
  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif

  return retVal;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  TcpIp_Udp_TxConfirmation
 **********************************************************************************************************************/
/*! \brief      Transmission Confirmation Callback
 *  \param[in]  SockIdx               socket index
 *  \param[in]  TxReqTabIdx           tx request table index
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_TxConfirmation(
    TcpIp_SockIdxType SockIdx,
    uint8 TxReqTabIdx )
{
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  uint8 TxReqListIdx;
#endif

  /* ------------------------------------------------- */
  /* DET */
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_INIT, TCPIP_UDP_API_ID_TX_CONFIRMATION, TCPIP_E_NOTINIT )
  TcpIp_CheckDetErrorReturnVoid( TCPIP_COND_VALID_UDPSOCKIDX(SockIdx), TCPIP_UDP_API_ID_TX_CONFIRMATION, TCPIP_E_INV_SOCK_ID )
  /* ------------------------------------------------- */

#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  TxReqListIdx = TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx];

  if(TxReqListIdx != 0xFF)  /* TxConfirmation is enabled */
  {
    uint8 LocTxReqTabIdx;  /* socket specific TxReqList index */

    LocTxReqTabIdx = TxReqTabIdx;

    if(TcpIp_Udp_TxReqListReadPos[TxReqListIdx] == LocTxReqTabIdx)
    {
      /* this is the first element in the TxReqList */

      /* get values from TcpIp_Udp_TxReqTab, and confirm tx */
      TcpIp_VSockTxConfirmation(SockIdx, (TcpIp_Udp_TxReqList[TxReqListIdx])[TxReqTabIdx].TxDataLenByte);
      (TcpIp_Udp_TxReqList[TxReqListIdx])[TxReqTabIdx].TxDataLenByte = 0;

      TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* update list variables. */
      TcpIp_Udp_TxReqListReadPos[TxReqListIdx]++;
      if(TcpIp_Udp_TxReqListReadPos[TxReqListIdx] == TcpIp_Udp_VCfgGetTxReqListSize(TxReqListIdx))
      {
        TcpIp_Udp_TxReqListReadPos[TxReqListIdx] = 0;
      }
      TcpIp_Udp_TxReqListFillNum[TxReqListIdx]--;

      TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    else
    {
      /* mark element for later confirmation */
      (TcpIp_Udp_TxReqList[TxReqListIdx])[TxReqTabIdx].Transmitted = TRUE;
    }
  }
#else
  TCPIP_UNUSED_PARAMETER(SockIdx);      /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(TxReqTabIdx);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Udp_MainFunctionRx
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
FUNC(void, TCPIP_CODE) TcpIp_Udp_MainFunctionRx(void)
{
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
  /* #10 Iterate over all UDP sockets. */
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  for(SockIdx = 0; SockIdx < TcpIp_VCfgGetMaxNumUdpSocks(); SockIdx++)
  {
    /* #20 Check if TxReqList is assigned to the socket (Socket uses optional TxConfirmation feature). */
    uint8 TxReqListIdx = TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx];

    if(TxReqListIdx != 0xFF)  /* TxConfirmation is enabled */
    {
      /* #30 Iterate over entries in the TxReqList. */
      while(0 != TcpIp_Udp_TxReqListFillNum[TxReqListIdx])
      {
        uint8   FillNum;
        uint16  LocalTxReqTabIdx;
        uint16  TxConfLen;
        boolean Transmitted;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
        boolean IssueTxConf = FALSE;

        TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /* #40 Remove entry from the ReqList, if transmission of request has been confirmend. */
        FillNum = TcpIp_Udp_TxReqListFillNum[TxReqListIdx];
        if(0 != FillNum)
        {
          LocalTxReqTabIdx =  TcpIp_Udp_TxReqListReadPos[TxReqListIdx];
          Transmitted      = (TcpIp_Udp_TxReqList[TxReqListIdx])[LocalTxReqTabIdx].Transmitted;
          TxConfLen        = (TcpIp_Udp_TxReqList[TxReqListIdx])[LocalTxReqTabIdx].TxDataLenByte;

          if(TRUE == Transmitted)
          {
            IssueTxConf = TRUE;

            /* inc ReadPos */
            TcpIp_Udp_TxReqListReadPos[TxReqListIdx]++;
            if(TcpIp_Udp_TxReqListReadPos[TxReqListIdx] == TcpIp_Udp_VCfgGetTxReqListSize(TxReqListIdx))
            {
              /* wraparound */
              TcpIp_Udp_TxReqListReadPos[TxReqListIdx] = 0;
            }
            /* decrease FillNum */
            TcpIp_Udp_TxReqListFillNum[TxReqListIdx]--;
          }
        }
        else
        {
          /* no list element available */
          TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
          break;  /* leave the while-loop */
        }

        TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

        if(IssueTxConf == TRUE)
        {
          /* confirm tx */
          /* #50 Forward TxConfirmation to TcpIpSocketOwner. */
          TcpIp_VSockTxConfirmation(SockIdx, TxConfLen);
        }
        else
        {
          /* no pending element at the start of the list */
          break;  /* PRQA S 0771 */ /* MD_TCPIP_UDP_14.6_0771_a */
        }
      }
    }
  }
#endif
} /* TcpIp_Udp_MainFunctionRx() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * TcpIp_Udp_MainFunctionTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Udp_MainFunctionTx(void)
{
#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
  /* #10 Try to send UDP packets that have been queued because link-layer address resolution was not completed. */
  TcpIp_SockIdxType SockIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  for(SockIdx = 0; SockIdx < TcpIp_VCfgGetMaxNumUdpSocks(); SockIdx++)
  {
    if(0 != TcpIp_Udp_TxRetrQueueForSock[SockIdx].ElementCount)
    {
      (void)TcpIp_Udp_SendRetryQueueIfPossible(SockIdx, NULL_PTR /* any address */);
    }
  }
#endif
} /* TcpIp_Udp_MainFunctionTx() */

/***********************************************************************************************************************
 *  TcpIp_Udp_ResetSocket
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_ResetSocket(TcpIp_SocketIdType socketId)
{
  TcpIp_Udp_CloseSocket(socketId);  /* reset tx confirmation depending elements */

  TcpIp_TcpIpEventFunctions(TcpIp_SockUserIdx[socketId], socketId, TCPIP_UDP_CLOSED);  /* trigger 'CLOSED' event */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_Udp_SetSockOpt
 **********************************************************************************************************************/
/*! \brief       set UDP socket option
 *  \param[in]   SockIdx                socket index
 *  \param[in]   ParameterId            option name
 *  \param[in]   ParameterValuePtr      option parameter pointer
 *  \return      E_OK                   socket option change
 *  \return      E_NOT_OK               socket option change request failed
 *  \context     task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_SetSockOpt(
    TcpIp_SocketIdType SockIdx,
    TcpIp_ParamIdType ParameterId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValuePtr )
{
  Std_ReturnType RetValue = E_NOT_OK;

  if(TcpIp_VCfgGetMaxNumUdpSocks() > SockIdx)  /* only for UDP sockets */
  {
    switch(ParameterId)
    {
#if (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON)
    case TCPIP_PARAMID_V_UDP_TXREQLISTSIZE:
      {
        uint8 RequestedListSize = *ParameterValuePtr;
        uint8 TxReqListIdx;

        /* choose a matching rx buffer and assign it to the socket / TCP connection */
        for(TxReqListIdx = 0; TxReqListIdx < TcpIp_VCfgGetUdpTxReqListNum(); TxReqListIdx++)
        {
          if((TcpIp_Udp_TxReqList_IsUsedBySocket[TxReqListIdx] == TCPIP_INV_SOCK_IDX) && (RequestedListSize == TcpIp_Udp_TxReqListSize[TxReqListIdx]))
          {
            /* found a free and matching TxReqList */
            TcpIp_Udp_TxReqList_IsUsedBySocket[TxReqListIdx] = SockIdx;
            TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx] = TxReqListIdx;
            RetValue = E_OK;
            break;  /* stop the for-loop */
          }
        }
        break;
      }
#endif  /* (TCPIP_UDP_TXCONFIRMATION_ENABLED == STD_ON) */

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
    case TCPIP_PARAMID_V_UDP_TXRETRYLISTSIZE:
      {
        /* set the maximum number of Eth tx buffers used for buffering messages in case the remote address is not yet resolved */
        uint8 RetryListSizeMax = *ParameterValuePtr;
        if(TCPIP_UDP_TX_RETRY_QUEUE_NUM_TOTAL >= RetryListSizeMax)
        {
          TcpIp_Udp_TxRetrQueueMaxNum[SockIdx] = RetryListSizeMax;
          RetValue = E_OK;
        }
        break;
      }
#endif

    default:
      {
        /* error, do nothing */
        TCPIP_UNUSED_PARAMETER(ParameterValuePtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
        break;
      }
    }
  }

  return RetValue;
}

/**********************************************************************************************************************
 *  TcpIp_Udp_VTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_VTransmit(
  TcpIp_SocketIdType                         udpSocketId,
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) ipTxReqDescPtr,
  TCPIP_P2V(uint8)                           ipPayloadBufPtr,
  uint16                                     udpPayloadLen,
  TCPIP_P2C(IpBase_SockAddrType)             destSockAddrPtr,
  uint8                                      txReqIdx)
{
  TCPIP_P2V(TcpIp_Udp_HdrType) udpHdrPtr;
  uint16              ipTransmitLength = udpPayloadLen + TCPIP_UDP_HDR_LEN_BYTE;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  Std_ReturnType      retVal;
  TcpIp_ReturnType    transmitRetVal;
  
  TCPIP_ASSERT_RETVAL(destSockAddrPtr != NULL_PTR, E_NOT_OK);

  /* #10 Write UDP header. */
  udpHdrPtr = (TCPIP_P2V(TcpIp_Udp_HdrType))ipPayloadBufPtr;                                            /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
  udpHdrPtr->SrcPort  = ((IpBase_SockAddrInType *)&(TcpIp_SocketList[udpSocketId].LocSock))->sin_port;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  udpHdrPtr->TgtPort  = ((TCPIP_P2C(IpBase_SockAddrInType))destSockAddrPtr)->sin_port;                  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  udpHdrPtr->LenByte  = IPBASE_HTON16(ipTransmitLength);
  udpHdrPtr->Checksum = TCPIP_UDP_EMPTY_CHKSUM;  /* checksum will be calculated by lower layer. */

  /* #20 Trigger transmission of UDP datagram via IP layer. */
  transmitRetVal = TcpIp_VIpTransmit(ipTxReqDescPtr, ipTransmitLength, TCPIP_PROTOCOL_UDP, (boolean)TcpIp_Udp_VIsTxConfirmationEnabled(udpSocketId), txReqIdx, TRUE, destSockAddrPtr);

  if (transmitRetVal == TCPIP_OK)
  {
    retVal = E_OK;
  }
  else if (transmitRetVal == TCPIP_E_PHYS_ADDR_MISS)
  {
    /* #30 Queue transmission of packet if IP layer could not send the packet due to physical address cache miss, if configured.
     *     Otherwise: Abort transmission. */
#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
    /* add request to the retry list if possible */
    if ((TcpIp_Udp_TxRetrQueueMaxNum[udpSocketId] > 0) && (TcpIp_Udp_TxRetrQueueForSock[udpSocketId].ElementCount < TcpIp_Udp_TxRetrQueueMaxNum[udpSocketId]))
    {
      /* for this socket the retry queue is enabled an not full */
      TcpIp_DListIdxType                RetryQueueEleIdx;  /* index of the new retry queue element */
      TcpIp_Udp_RetryQueueElementType  *RetryQueueElePtr;  /* pointer to the new retry queue element */

      if (TcpIp_DListAddElement(&TcpIp_Udp_TxRetrQueueForSock[udpSocketId], TcpIp_Udp_TxRetrQueueForSock[udpSocketId].LastIdx, &RetryQueueEleIdx) == E_OK)
      {
        /* a new element could be added */
        RetryQueueElePtr = &TcpIp_Udp_TxRetrQueueElements[RetryQueueEleIdx];

        /* fill retry queue element */
        (void)IpBase_CopySockAddr((IpBase_SockAddrType*)&(RetryQueueElePtr->Destination), (TCPIP_P2C(IpBase_SockAddrType))destSockAddrPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        RetryQueueElePtr->UdpPayloadLen   = udpPayloadLen;
        RetryQueueElePtr->UdpTxReqTabIdx  = txReqIdx;
        RetryQueueElePtr->IpTxRequestDesc = *ipTxReqDescPtr;

        /* accept this first send attempt */
        retVal = E_OK;
      }
      else
      {
        /* no new element could be added (probably no element was available) */
        TcpIp_VIpTransmitCancel(&TcpIp_Udp_IpTxRequestDescriptor[udpSocketId], destSockAddrPtr->sa_family, txReqIdx, TCPIP_PROTOCOL_UDP);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* the transmit has to be canceled because there is no queue element to store the request */
      TcpIp_VIpTransmitCancel(&TcpIp_Udp_IpTxRequestDescriptor[udpSocketId], destSockAddrPtr->sa_family, txReqIdx, TCPIP_PROTOCOL_UDP);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      retVal = E_NOT_OK;
    }
#else
    /* This should never happen since the address availability is already tested in ProvideBuffer */
    retVal = E_NOT_OK;
#endif
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}  /* TcpIp_Udp_VTransmit */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/***********************************************************************************************************************
 *  TcpIp_Udp_StoreChecksumInHeader
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_StoreChecksumInHeader(
    uint16           Checksum,
    TCPIP_P2V(uint8) UdpHdrPtr)
{
  TcpIp_Udp_HdrType *HdrPtr = (TcpIp_Udp_HdrType*)UdpHdrPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */

  if(0 == Checksum)
  {
    HdrPtr->Checksum = 0xFFFF;
  }
  else
  {
    HdrPtr->Checksum = Checksum;
  }
} /* TcpIp_Udp_StoreChecksumInHeader() */

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Udp_SendRetryQueueIfPossible
 **********************************************************************************************************************/
/*! \brief         Transmit stored messages if possible
 *  \param[in]     SocketId           socket index
 *  \param[in]     RemoteAddrPtr      IP address and port of the remote host to transmit to.
 *  \return        E_OK               OK
 *                 E_NOT_OK           Something went wrong
 *  \note
 *  \context       TcpIp_UdpTransmit
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_SendRetryQueueIfPossible(
    TcpIp_SocketIdType udpSocketId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr)  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
{
  if (TcpIp_Udp_TxRetrQueueForSock[udpSocketId].ElementCount > 0)
  {
    /* try to send the stored elements first if they have the same destination and try to send only those ones */
    TcpIp_DListIdxType ListIdx      = TcpIp_Udp_TxRetrQueueForSock[udpSocketId].FirstIdx;
    TcpIp_DListIdxType NextListIdx;
    uint8 TotElementCount           = TcpIp_Udp_TxRetrQueueForSock[udpSocketId].ElementCount;
    uint8 CurrentElementCounter;

    for (CurrentElementCounter = 0; CurrentElementCounter < TotElementCount; CurrentElementCounter++)
    {
      if( (RemoteAddrPtr == NULL_PTR) /* any address */ ||  /* PRQA S 3415 */ /* MD_TCPIP_12.4_3415 */
          (TRUE == IpBase_SockIpAddrIsEqual((IpBase_SockAddrType*)RemoteAddrPtr, (IpBase_SockAddrType*)&TcpIp_Udp_TxRetrQueueElements[ListIdx].Destination)) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        /* the element has the same destination address as the current send request, or all elemts have to be sent */
        TCPIP_P2V(TcpIp_Udp_RetryQueueElementType) retryQueueElePtr = &TcpIp_Udp_TxRetrQueueElements[ListIdx];

        NextListIdx = TcpIp_Udp_TxRetrQueueElementChain[ListIdx].NextIdx;

        if (TcpIp_VIpTransmit(&retryQueueElePtr->IpTxRequestDesc, retryQueueElePtr->UdpPayloadLen + TCPIP_UDP_HDR_LEN_BYTE, TCPIP_PROTOCOL_UDP, (boolean)TcpIp_Udp_VIsTxConfirmationEnabled(udpSocketId), retryQueueElePtr->UdpTxReqTabIdx, TRUE, (TCPIP_P2C(IpBase_SockAddrType))&retryQueueElePtr->Destination) == TCPIP_OK)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        {
          /* element could be sent, so remove it from retry list */
          TcpIp_DListRemoveElement(&TcpIp_Udp_TxRetrQueueForSock[udpSocketId], ListIdx);
        }

        ListIdx = NextListIdx;
      }
      else
      {
        /* get the next list element */
        ListIdx = TcpIp_Udp_TxRetrQueueElementChain[ListIdx].NextIdx;
      }
    }
  }
  return E_OK;
}

/***********************************************************************************************************************
 *  TcpIp_Udp_CancelRetriesForDestination
 **********************************************************************************************************************/
/*! \brief         Cancel all pending retries for a specific destination
 *  \param[in]     CtrlIdx            EthIf controller index
 *  \param[in]     RemoteAddrPtr      IP address of the remote host
 *  \context
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_CancelRetriesForDestination(
  uint8                         CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrType) RemoteAddrPtr)  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
{
  TcpIp_SockIdxType UdpSockIdx;

  for(UdpSockIdx = 0; UdpSockIdx < TcpIp_VCfgGetMaxNumUdpSocks(); UdpSockIdx++)
  {
    if(!TCPIP_DLIST_IS_EMPTY(TcpIp_Udp_TxRetrQueueForSock[UdpSockIdx]))
    {
      /* cancel transmit of all retry elements for this socket that should be sent to the unreachable destination */
      TcpIp_DListIdxType ListIdx = TcpIp_Udp_TxRetrQueueForSock[UdpSockIdx].FirstIdx;

      while (TCPIP_DLIST_END_IDX != ListIdx)
      {
        TcpIp_DListIdxType NextListIdx = TCPIP_DLIST_GET_NEXT(ListIdx, TcpIp_Udp_TxRetrQueueForSock[UdpSockIdx]);

        if (TcpIp_Udp_TxRetrQueueElements[ListIdx].IpTxRequestDesc.CtrlIdx == CtrlIdx)
        {
          if (TRUE == TcpIp_VSockIpAddrIsEqual((TCPIP_P2C(IpBase_SockAddrType))&TcpIp_Udp_TxRetrQueueElements[ListIdx].Destination, (TCPIP_P2C(IpBase_SockAddrType))RemoteAddrPtr))  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
          {
            (void)TcpIp_VIpTransmitCancel(&TcpIp_Udp_TxRetrQueueElements[ListIdx].IpTxRequestDesc, RemoteAddrPtr->domain, 0, TCPIP_PROTOCOL_UDP);
            TcpIp_DListRemoveElement(&TcpIp_Udp_TxRetrQueueForSock[UdpSockIdx], ListIdx);
          }
        }

        ListIdx = NextListIdx;
      }
    }
  }
} /* TcpIp_Udp_CancelRetriesForDestination() */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (TCPIP_UDP_CHECK_RX_CHECKSUM == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Udp_VValidateChecksum
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_VValidateChecksum(
    uint16                         ReceivedChecksum,
    TCPIP_P2C(IpBase_SockAddrType) RxSockLocAddrPtr,
    TCPIP_P2C(IpBase_SockAddrType) RxSockRemAddrPtr,
    TCPIP_P2C(uint8)               IpPayloadPtr,
    uint16                         IpPayloadLen)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* #10 Verify that checksum of received packet is zero, if packet has a checksum. */
  if (ReceivedChecksum != 0)
  {
    if (TcpIp_VCalcPseudoHdrAndChecksum(RxSockRemAddrPtr, RxSockLocAddrPtr, IpPayloadPtr, IpPayloadLen, TCPIP_SOCK_PROT_UDP) == 0)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    {
      /* checksum is valid -> OK */
      retVal = E_OK;
    }
  }
  else
  {
    /* #20 Otherwise accept packet only if it is not an IPv6 packet. (Checksum is mandatory in case of IPv6). */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if (RxSockRemAddrPtr->sa_family != IPBASE_AF_INET6)
# endif
    {
      /* for IPv4 the checksum is optional. */
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
   MD_TCPIP_UDP_1.2_0505_a:
      Reason:     'ReqListElePtr' is always properly initialized if (TcpIp_Udp_Socket_AssignedTxReqListIndex[SockIdx] != 0xFF)
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TCPIP_UDP_14.6_0771_a:
      Reason:     The break statement is used multiple (two) times in a while struct to stop the function
                  TcpIp_Udp_MainFunction from issuing further TxConfirmations This seems to be the most efficient way
                  to do that.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.

*/

/**********************************************************************************************************************
 *  END OF FILE: Udp.c
 *********************************************************************************************************************/
