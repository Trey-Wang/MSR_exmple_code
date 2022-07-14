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
 *         \file  TcpIp_Tcp.h
 *        \brief  Implementation of Transmission Control Protocol (TCP)
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_TCP_H)
# define TCPIP_TCP_H

/*
--- LIMITAIONS OF THIS IMPLEMENTATION ------------------------------------------
- (Backlog > 1) is not supported
- no precedence and security compartment support
- no OutOfBand data support
*/
/*************************************************************************************************/

/*
|0              |8              |16             |24             |
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Data |           |U|A|P|R|S|F|                               |
| Offset| Reserved  |R|C|S|S|Y|I|            Window             |
|       |           |G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum            |        Urgent Pointer         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Example TCP header format
*/

/* TCP User calls */
/*
   TcpIp_Tcp_Open()
   TcpIp_Tcp_Send()
   TcpIp_Tcp_Receive()
   TcpIp_Tcp_Close()
   TcpIp_Tcp_Status()
   TcpIp_Tcp_Abort()
*/
/*
Events that occur:
  User Calls:
    OPEN
    SEND
    RECEIVE
    CLOSE
    ABORT
    STATUS
  Arriving Segments:
    SEGMENT ARRIVES
  Timeouts:
    USER TIMEOUT
    RETRANSMISSION TIMEOUT
    TIME-WAIT TIMEOUT
*/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Priv_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* TCP ApiIds */
#define TCPIP_TCP_API_ID_INIT                        0xBCu
#define TCPIP_TCP_API_ID_MAIN_FUNCTION               0xBDu
#define TCPIP_TCP_API_ID_RX_INDICATION               0xBEu
#define TCPIP_TCP_API_ID_TRANSMIT                    0xBFu
#define TCPIP_TCP_API_ID_OPEN                        0xC0u
#define TCPIP_TCP_API_ID_CLOSE                       0xC1u
#define TCPIP_TCP_API_ID_SEND_ASR                    0xC2u
#define TCPIP_TCP_API_ID_RECEIVE                     0xC3u
#define TCPIP_TCP_API_ID_RECEIVED                    0xC4u
#define TCPIP_TCP_API_ID_TX_CHECK_AND_PREP_BUF       0xC5u
#define TCPIP_TCP_API_ID_DUPL_AND_CONNECT            0xC6u
#define TCPIP_TCP_API_ID_DIRECT_ACCEPT               0xC7u
#define TCPIP_TCP_API_ID_GET_SMALL_STATUS            0xC8u
#define TCPIP_TCP_API_ID_ABORT                       0xC9u
#define TCPIP_TCP_API_ID_SET_SOCK_OPT                0xCAu
#define TCPIP_TCP_API_ID_GET_SOCK_WRITABLE           0xCBu
#define TCPIP_TCP_API_ID_V_SEND_RESET                0xCCu
#define TCPIP_TCP_API_ID_GET_SOCK_IS_CONNECTED       0xCDu

/* TCP specific DET errors */
#define TCPIP_TCP_E_RST_QUEUE_OVERFLOW               0x10u

/* TCP state */
#define TCPIP_TCP_STATE_UNINIT                       0x00u
#define TCPIP_TCP_STATE_INITMEMORY                   0x01u
#define TCPIP_TCP_STATE_ENABLED                      0x02u

#define TCPIP_TCP_ACTIVE_OPEN                        TRUE
#define TCPIP_TCP_PASSIVE_OPEN                       FALSE

#define  TCPIP_TCP_SOCK_STATE_INVALID             0  /* must be '0' */
#define  TCPIP_TCP_SOCK_STATE_LISTEN              1
#define  TCPIP_TCP_SOCK_STATE_SYNSENT             2
#define  TCPIP_TCP_SOCK_STATE_SYNRCVD             3
#define  TCPIP_TCP_SOCK_STATE_ESTABLISHED         4
#define  TCPIP_TCP_SOCK_STATE_CLOSEWAIT           5
#define  TCPIP_TCP_SOCK_STATE_LASTACK             6
#define  TCPIP_TCP_SOCK_STATE_FINWAIT1            7
#define  TCPIP_TCP_SOCK_STATE_FINWAIT2            8
#define  TCPIP_TCP_SOCK_STATE_CLOSING             9
#define  TCPIP_TCP_SOCK_STATE_TIMEWAIT           10
#define  TCPIP_TCP_SOCK_STATE_CLOSED             11

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef void TcpIp_Tcp_ConfigType;

typedef struct
{
  uint8  SockState;       /* TCP socket state */
  uint16 TxBufBytesRem;   /* TxBuf bytes remaining */
  uint16 RxBufFillLevel;  /* RxBuf received bytes available */
} TcpIp_Tcp_SmallStatusType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  TcpIp_Tcp_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration of the User Datagram
 *              Protocol and may be used to initialize the data structures.
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \context    initialization
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_Tcp_Init( void );

/***********************************************************************************************************************
 *  TcpIp_Tcp_PrepareShutdown
 **********************************************************************************************************************/
/*! \brief      Prepare shutdown of TCP. Close "listen" socket, preconfig socket to "closed" instead of "listen".
 *  \param[in]  TcpIdx                tcp socket index
 *  \return     TRUE                  shutdown prepared
 *  \return     FALSE                 shutdownd request failed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_Tcp_PrepareShutdown( TcpIp_TcpIdxType TcpIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_CbkPathMtuChg
 **********************************************************************************************************************/
/*! \brief      The Path-MTU has changed for a certain destination address
 *  \param[in]  SockAddrPtr           net address that changed its state
 *  \param[in]  PathMtuSize           new size of the Path-MTU (transport layer part)
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_CbkPathMtuChg(
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SockAddrPtr,
  uint16 PathMtuSize );

/***********************************************************************************************************************
 *  TcpIp_Tcp_Received
 **********************************************************************************************************************/
/*! \brief      mark part of buffer as read
 *  \param[in]  TcpIdx                tcp socket index
 *  \param[in]  DataLenByte           length of received data in bytes
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Received(TcpIp_TcpIdxType TcpIdx,
    uint32 DataLenByte );

/***********************************************************************************************************************
 *  TcpIp_Tcp_TxCheckAndPrepBuf
 **********************************************************************************************************************/
/*! \brief      - check if enough buffer and storage struct elements are available
 *              - prepare element in TxReqArray
 *  \param[in]  TcpIdx                tcp socket index
 *  \param[in]  DataLenByte           length of received data in bytes
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_TxCheckAndPrepBuf(
    TcpIp_TcpIdxType TcpIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_InitSock
 **********************************************************************************************************************/
/*! \brief      Init a single socket on TCP level.
 *  \param[in]  SocketIdx             socket index
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_InitSock(
    TcpIp_SocketIdType SocketIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_SendAsr
 **********************************************************************************************************************/
/*! \brief      send data that was provided earlier
 *  \param[in]  TcpIdx                tcp socket index
 *  \param[in]  TxLen                 length of the data that shall be transmitted
 *  \return     E_OK                  transmission requested
 *  \return     E_NOT_OK              transmission request failed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_SendAsr(
    TcpIp_TcpIdxType TcpIdx,
    uint32 TxLen );

/***********************************************************************************************************************
 *  TcpIp_Tcp_GetSockIsConnected
 **********************************************************************************************************************/
/*! \brief       Get information if socket is connected and not yet closed.
 *  \param[in]   TcpIdx                 socket index
 *  \return      TRUE                   socket is connected 
 *               FALSE                  socket is not connected
 *  \context     task level
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_Tcp_GetSockIsConnected(
    TcpIp_TcpIdxType TcpIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_MainFunctionRx
 **********************************************************************************************************************/
/*! \brief      Main function RX
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_MainFunctionRx(void);

/***********************************************************************************************************************
 *  TcpIp_Tcp_MainFunctionTx
 **********************************************************************************************************************/
/*! \brief      Main function TX
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_MainFunctionTx(void);

/***********************************************************************************************************************
 *  TcpIp_Tcp_MainFunctionState
 **********************************************************************************************************************/
/*! \brief      Main function State
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_MainFunctionState(void);

/***********************************************************************************************************************
 *  TcpIp_Tcp_DuplicateSocket
 **********************************************************************************************************************/
/*! \brief      duplicate parameters of listen socket and return a new (acceptable) socket
 *  \param[in]  ListenTcpIdx          listen socket index
 *  \return                           TcpIdx of the new socket or TCPIP_TCP_INV_IDX if socket could not be duplicated
 *  \context    task level
 **********************************************************************************************************************/
FUNC(TcpIp_TcpIdxType, TCPIP_CODE) TcpIp_Tcp_DuplicateSocket(
    TcpIp_TcpIdxType ListenTcpIdx);

/***********************************************************************************************************************
 *  TcpIp_Tcp_AcceptSocket
 **********************************************************************************************************************/
/*! \brief      accept a connection on a socket duplicated by TcpIp_Tcp_DuplicateSocket()
 *  \param[in]  ListenTcpIdx         Index of the listen socket
 *  \param[in]  AcceptTcpIdx         Index of the duplicated/accept socket
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_AcceptSocket(
    TcpIp_TcpIdxType ListenTcpIdx,
    TcpIp_TcpIdxType AcceptTcpIdx);

/***********************************************************************************************************************
 *  TcpIp_Tcp_Open
 **********************************************************************************************************************/
/*! \brief      active or passive open of a connection
 *  \param[in]  SockIdx                socket index
 *  \param[in]  SockAddr               socket address
 *  \param[in]  ActiveOpen             active or passive open of a connection
 *  \context    task level
 *  \note       Format: OPEN (local port, foreign socket, active/passive [, timeout] [, precedence]
 *              [, security/compartment] [, options]) -> local connection name
 **********************************************************************************************************************/
FUNC(TcpIp_TcpIdxType, TCPIP_CODE) TcpIp_Tcp_Open(
    TcpIp_SockIdxType SockIdx,
    P2CONST(IpBase_SockAddrType, TCPIP_APPL_DATA, TCPIP_CONST) SockAddrPtr,
    boolean ActiveOpen
    /* , uint8 Precedence,
    uint8 Security */ );

/***********************************************************************************************************************
 *  TcpIp_Tcp_Close
 **********************************************************************************************************************/
/*! \brief       has to close the TCP connection gracefully, and then signal success to SOCK
 *  \param[in]   TcpIdx                 socket index
 *  \context     task level
 *  \note        Format: CLOSE (local connection name)
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Close(
    TcpIp_TcpIdxType TcpIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_Abort
 **********************************************************************************************************************/
/*! \brief       hard cancelation of connection
 *  \param[in]   TcpIdx                 socket index
 *  \context     task level
 *  \note        Format: ABORT (local connection name)
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_Abort(
    TcpIp_TcpIdxType TcpIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_ResetSocket
 **********************************************************************************************************************/
/*! \brief       Reset socket and inform upper layer.
 *  \param[in]   socketId          TcpIp socket Identifier.
 *  \context     task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_ResetSocket(TcpIp_SocketIdType socketId);

/***********************************************************************************************************************
 *  TcpIp_Tcp_VReleaseAsignedBuffers
 **********************************************************************************************************************/
/*! \brief       Release the rx and tx buffers assigned to the TCP socket
 *  \param[in]   TcpIdx                 TCP index
 *  \context     task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_VReleaseAsignedBuffers(
    TcpIp_TcpIdxType TcpIdx );

/***********************************************************************************************************************
 *  TcpIp_Tcp_SetSockOpt
 **********************************************************************************************************************/
/*! \brief       set TCP socket option
 *  \param[in]   TcpIdx                 socket index
 *  \param[in]   OptName                option name
 *  \param[in]   OptVal                 option parameter
 *  \return      E_OK                   socket option change
 *  \return      E_NOT_OK               socket option change request failed
 *  \context     task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_SetSockOpt(
    TcpIp_TcpIdxType TcpIdx,
    uint8 OptName,
    uint32 OptVal );

/***********************************************************************************************************************
 *  Stack internal FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_Tcp_VAddrResTimeout
 **********************************************************************************************************************/
/*! \brief      Address resolution timout Callback
 *  \param[in]  DestAddrPtr           Pointer to destination address
 *  \context    interrupt or task level
 *  \note       The address resolution for the given address timed out on the given controller, after the address
 *              resolution request was accepted earlier.
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_VAddrResTimeout(
  P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestAddrPtr );

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIP_TCP_H */
/**********************************************************************************************************************
 *  END OF FILE: Tcp.h
 *********************************************************************************************************************/
