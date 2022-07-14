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
 *         \file  TcpIp_Tcp_Cbk.h
 *        \brief  Implementation of Transmission Control Protocol (TCP) - Callback Header
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_TCP_CBK_H)
# define TCPIP_TCP_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Priv_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_Tcp_RxIndication
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  LocalAddrId           local address identifier
 *  \param[in]  RxSockRemAddrPtr      pointer to the socket
 *  \param[in]  DataPtr               pointer to the received data
 *  \param[in]  LenByte               length of received data in bytes
 *  \param[in]  ChecksumCalculated    indicates if the TCP checksum is already checked by lower layer
 *  \note       The parameter 'ChecksumCalculated' is only used if the lower layer (hardware) is capable of checking
 *                the received TCP checksum.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_RxIndication(
    TcpIp_LocalAddrIdType LocalAddrId,
    P2VAR(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RxSockRemAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 LenByte
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    , boolean ChecksumCalculated
#endif
    );

/***********************************************************************************************************************
 *  TcpIp_Tcp_StoreChecksumInHeader
 **********************************************************************************************************************/
/*! \brief       Stores a calculated TCP checksum value at the correct position into the TCP header
 *  \param[in]   Checksum              Finalized checksum value.
 *  \param[in]   TcpHdrPtr             Pointer the location of the TCP header.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_StoreChecksumInHeader(
    uint16           Checksum,
    TCPIP_P2V(uint8) TcpHdrPtr);

#if (TCPIP_SUPPORT_TLS == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsRxIndication
 **********************************************************************************************************************/
/*! \brief      Forwards the RxIndication from TLS to the socket owner
 *  \param[in]  SocketId              socket id
 *  \param[in]  DataPtr               pointer to the received data
 *  \param[in]  DataLen               length of received data in bytes
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsRxIndication(
    TcpIp_SocketIdType                                    SocketId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA)              DataPtr,
    uint16                                                DataLen);

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsTxConfirmation
 **********************************************************************************************************************/
/*! \brief      Forwards the TxConfirmation from TLS to the socket owner
 *  \param[in]  SocketId              socket id
 *  \param[in]  DataLen               length of transmitted data in bytes
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsTxConfirmation(
    TcpIp_SocketIdType                                    SocketId,
    uint16                                                DataLen);

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsConnected
 **********************************************************************************************************************/
/*! \brief      Forwards the 'connected' event from TLS to the socket owner
 *  \param[in]  SocketId              socket id
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsConnected(
    TcpIp_SocketIdType SocketId );

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsEvent
 **********************************************************************************************************************/
/*! \brief      Forwards the event from TLS to the socket owner
 *  \param[in]  SocketId              socket id
 *  \param[in]  Event                 event identifier
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsEvent(
    TcpIp_SocketIdType SocketId,
    TcpIp_EventType Event );

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsValidationResult
 **********************************************************************************************************************/
/*! \brief           Forwards the event from TLS to the socket owner
 *  \param[in]       SocketId              socket id
 *  \param[in,out]   ValidationResultPtr   Result of the chain validation, can be overruled by the socket owner
 *  \param[in]       CertChainPtr          Pointer to the certificate chain
 *  \param[in]       CertChainLen          Length of the certificate chain
 *  \context         TASK
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsValidationResult(
    TcpIp_SocketIdType  SocketId,
    TCPIP_P2V(uint8)    ValidationResultPtr,
    TCPIP_P2C(uint8)    CertChainPtr,
    uint32              CertChainLen );

/***********************************************************************************************************************
 *  TcpIp_Tcp_TlsReceived
 **********************************************************************************************************************/
/*! \brief      mark part of buffer as read
 *  \param[in]  SocketId              socket index
 *  \param[in]  DataLenByte           length of received data in bytes
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tcp_TlsReceived(
    TcpIp_SocketIdType SocketId,
    uint32 DataLenByte );

#endif

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIP_TCP_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Tcp_Cbk.h
 *********************************************************************************************************************/
