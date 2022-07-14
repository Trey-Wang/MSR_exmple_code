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
 *         \file  TcpIp_Udp_Cbk.h
 *        \brief  Implementation of User Datagram Protocol (UDP) - Callback Header
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_UDP_CBK_H)
# define TCPIP_UDP_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Types.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8 TcpIp_Udp_ReturnType;

#define TCPIP_UDP_OK                    0
#define TCPIP_UDP_E_NOT_OK              1
#define TCPIP_UDP_E_NO_MATCHING_SOCKET  2

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_Udp_RxIndication
 **********************************************************************************************************************/
/*! \brief         Processes a received UDP datagram.
 *  \details       -
 *  \param[in]     LocalAddrId           IP address identifier of the local address that matches the destination IP 
 *                                       address of the received packet.
 *  \param[in]     RxSockRemAddrPtr      Remote (source) address of the received packet.
 *  \param[in]     IpPayloadPtr          Payload of the received IP packet (starting with the UDP header).
 *  \param[in]     IpPayloadLen          Length of the payload in bytes.
 *  \param[in]     ChecksumValidated     Indicates if the UDP checksum was already checked by lower layer.
 *  \return        TCPIP_UDP_OK                    UDP packet was passed to upper layer.
 *  \return        TCPIP_UDP_E_NO_MATCHING_SOCKET  Packet ignored, because no matching UDP socket was found.
 *  \return        TCPIP_UDP_E_NOT_OK              Packet ignored for other reason.
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(TcpIp_Udp_ReturnType, TCPIP_CODE) TcpIp_Udp_RxIndication(
    TcpIp_LocalAddrIdType          LocalAddrId,
    TCPIP_P2V(IpBase_SockAddrType) RxSockRemAddrPtr,
    TCPIP_P2V(uint8)               IpPayloadPtr,
    uint16                         IpPayloadLen,
    boolean                        ChecksumValidated);

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
    uint8 TxReqTabIdx );

/***********************************************************************************************************************
 *  TcpIp_Udp_StoreChecksumInHeader
 **********************************************************************************************************************/
/*! \brief       Stores a calculated UDP checksum value at the correct position into the UDP header.
 *  \param[in]   Checksum              Finalized checksum value.
 *  \param[in]   UdpHdrPtr             Pointer the location of the UDP header.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/

FUNC(void, TCPIP_CODE) TcpIp_Udp_StoreChecksumInHeader(
    uint16           Checksum,
    TCPIP_P2V(uint8) UdpHdrPtr);

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIP_UDP_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Udp_Cbk.h
 *********************************************************************************************************************/
