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
 *         \file  TcpIp_Udp.h
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

#if !defined (TCPIP_UDP_H)
# define TCPIP_UDP_H

/*
--- LIMITAIONS OF THIS IMPLEMENTATION ------------------------------------------
- no data handling (rx ant tx) bigger than 64kB possible
- TcpIp_Udp_RecvFrom() ignores "flags"
*/

/*
|0              |8              |16             |24             |
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |           Checksum            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
UDP header format
*/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Priv_Types.h"
#include "IpBase_Types.h"
#include "TcpIp_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* UDP ApiIds */
#define TCPIP_UDP_API_ID_INIT                        0xD0u
#define TCPIP_UDP_API_ID_RX_INDICATION               0xD1u
#define TCPIP_UDP_API_ID_TX_CONFIRMATION             0xD3u
#define TCPIP_UDP_API_ID_MAIN_FUNCTION               0xD4u
#define TCPIP_UDP_API_ID_RECV_FROM                   0xD5u
#define TCPIP_UDP_API_ID_GET_BUF_FILL_LEVEL          0xD6u
#define TCPIP_UDP_API_ID_TRANSMIT_ASR                0xD7u
#define TCPIP_UDP_API_ID_CLOSE_SOCKET                0xD8u
#define TCPIP_UDP_API_ID_REQUEST_TX_BUFFER           0xD9u

#define TCPIP_UDP_API_ID_RST_P1                      0xDAu

/* UDP specific DET errors */
/* none */

#define TCPIP_UDP_HDR_LEN_BYTE        8u

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef void TcpIp_Udp_ConfigType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_Udp_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration of the User Datagram
 *              Protocol and may be used to initialize the data structures.
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \context    initialization
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_Init( void );

/***********************************************************************************************************************
 *  TcpIp_Udp_CloseSocket
 **********************************************************************************************************************/
/*! \brief      This API call closes the UDP socket and resets all relevant internal variables.
 *  \param[in]  SockIdx             socket index
 *  \note
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_CloseSocket(
    TcpIp_SocketIdType SockIdx );

/***********************************************************************************************************************
 *  TcpIp_Udp_RequestTxBuffer
 **********************************************************************************************************************/
/*! \brief         Provides a UDP TX buffer.
 *  \details       This function calls the ProvideTxBuffer service of the IP layer (IPv4 or IPv6) based on the destination
 *                 address and returns a buffer for the UDP payload.
 *  \param[in]     socketId              UDP socket index.
 *  \param[in]     destinationPtr        Pointer to the destination socket address.
 *  \param[out]    bufPtr                Pointer to the (first) UDP payload buffer.
 *  \param[in,out] bufLenPtr             In:  Required buffer size. [bytes]
 *                                       Out: Actual size of the provided buffer fragment. [bytes] (see notes).
 *  \return        BUFREQ_OK             Buffer provided.
 *  \return        BUFREQ_E_NOT_OK       Buffer could not be provided.
 *  \return        BUFREQ_E_BUSY         IP layer is currently not able to provide buffer. Try again later.
 *  \note                                If IPv4 is used this function may provide more than one buffer if the requested
 *                                       payload size is too large to fit into one IP packet. In that case only the first
 *                                       buffer fragment is returned by this function. The additional buffer fragments
 *                                       can be obtained via TcpIp_Udp_FragmentRequestTxBuffer().
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Udp_RequestTxBuffer(
    TcpIp_SocketIdType              socketId,
    TCPIP_P2C(IpBase_SockAddrType)  destinationPtr,
    TCPIP_P2V(uint8)               *bufPtr,
    TCPIP_P2V(uint16)               bufLenPtr);

/***********************************************************************************************************************
 *  TcpIp_Udp_FragmentRequestTxBuffer
 **********************************************************************************************************************/
/*! \brief       Provides an additional UDP TX buffer fragment.
 *  \details     TcpIp_Udp_RequestTxBuffer() may return a buffer that is smaller than the requested length.
 *               In that case additional buffer fragments can be obtained via this function.
 *  \param[in]   udpSocketId            Index of the UDP socket.
 *  \param[in]   addrFamily             Address Family of destination used at previous TcpIp_Udp_RequestTxBuffer() call.
 *  \param[out]  bufPtrPtr              Pointer to the next UDP buffer fragment.
 *  \param[out]  bufLenPtr              Length of the next UDP buffer fragment in bytes.

 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         TcpIp_Udp_RequestTxBuffer() must be called first.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_FragmentRequestTxBuffer(
  TcpIp_SocketIdType  udpSocketId,
  IpBase_FamilyType   addrFamily,
  TCPIP_P2V(uint8)   *bufPtrPtr,
  TCPIP_P2V(uint16)   bufLenPtr);

/***********************************************************************************************************************
 *  TcpIp_Udp_Transmit
 **********************************************************************************************************************/
/*! \brief       Transmits a UDP packet.
 *  \details     -
 *  \param[in]   udpSocketId      Index of the UDP socket.
 *  \param[in]   udpPayloadLen    Length of the UDP payload. [bytes]
 *  \param[in]   destSockAddrPtr  Destination IP address and port of the UDP packet.
 *  \return      E_OK             Transmit request accepted.
 *  \return      E_NOT_OK         Transmit failed.
 *  \note        The payload of the UDP packet must be written in the provided IP buffer before.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \pre         -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_Transmit(
  TcpIp_SocketIdType             udpSocketId,
  uint16                         udpPayloadLen,
  TCPIP_P2C(IpBase_SockAddrType) destSockAddrPtr);

/***********************************************************************************************************************
 *  TcpIp_Udp_TransmitCancel
 **********************************************************************************************************************/
/*! \brief        Cancels transmit of a UDP datagram.
 *  \details      -
 *  \param[in]    udpSocketId      Index of the UDP socket.
 *  \param[in]    destSockAddrPtr  Destination IP address and port of the UDP packet.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \pre          TcpIp_Udp_ProvideTxBuffer() must have been called before.
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_TransmitCancel(
  TcpIp_SocketIdType             udpSocketId,
  TCPIP_P2C(IpBase_SockAddrType) destSockAddrPtr);

/***********************************************************************************************************************
 *  TcpIp_Udp_MainFunctionRx
 **********************************************************************************************************************/
/*! \brief      Main function for RX path. Also handles Tx confirmations.
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_MainFunctionRx(void);
/***********************************************************************************************************************
 *  TcpIp_Udp_MainFunctionTx
 **********************************************************************************************************************/
/*! \brief      Main function for TX path. Transmits queued packets.
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_MainFunctionTx(void);

/***********************************************************************************************************************
 *  TcpIp_Udp_ResetSocket
 **********************************************************************************************************************/
/*! \brief       Reset UDP socket and notify upper layer.
 *  \param[in]   socketId          TcpIp socket Identifier.
 *  \context     task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_ResetSocket(TcpIp_SocketIdType socketId);

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
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValuePtr );

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Udp_SendRetryQueueIfPossible
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Udp_SendRetryQueueIfPossible(
    TcpIp_SocketIdType udpSocketId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr);  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */

/***********************************************************************************************************************
 *  TcpIp_Udp_CancelRetriesForDestination
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Udp_CancelRetriesForDestination(
  uint8                         CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrType) RemoteAddrPtr);  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
#endif

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIP_UDP_H */
/**********************************************************************************************************************
 *  END OF FILE: Udp.h
 *********************************************************************************************************************/
