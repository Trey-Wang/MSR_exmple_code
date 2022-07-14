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
 *         File:  Tls_Cbk.h
 *      Project:  OnBoardCharger
 *       Module:  Transport Layer Security
 *    Generator:  SysSercice_Tls.dll
 *
 *  Description:  callback header
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Tls module. >> Tls.h
 *********************************************************************************************************************/

#if !defined (TLS_CBK_H)
# define TLS_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Tls_Types.h"
#include "TcpIp_Types.h"
#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TLS_START_SEC_CODE
#include "MemMap.h"

/***********************************************************************************************************************
 *  Tls_RxIndication
 **********************************************************************************************************************/
/*! \brief         Receive Indication Callback
 *  \details       This function receives incoming data from TCP and stores this information in a storage descriptor.
 *  \param[in]     SocketId            Socket id
 *  \param[in]     DataPtr             Pointer to the received data
 *  \param[in]     DataLen             Length of the received data
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TLS_CODE) Tls_RxIndication(
  Tls_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
  uint16 DataLen );

/***********************************************************************************************************************
 *  Tls_TcpConnected
 **********************************************************************************************************************/
/*! \brief         Tls callback, remote side of this socket accepted a connection request.
 *  \details       The TCP connection is established, now the TLS handshake is started by sending a client_hello.
 *  \param[in]     SocketId        Socket on which a TLS connection is established
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-121566, CREQ-121567
 **********************************************************************************************************************/
FUNC(void, TLS_CODE) Tls_TcpConnected(
  TcpIp_SockHndType SocketId );

/***********************************************************************************************************************
 *  Tls_TcpIpEvent
 **********************************************************************************************************************/
/*! \brief         TcpIp event callback, TCP issued an event.
 *  \details       Process the TLS states depending on the receivedn TcpIp event.
 *  \param[in]     SocketId       Socket on which the indicated event occured
 *  \param[in]     Event          Event that occured in TcpIp
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(void, TLS_CODE) Tls_TcpIpEvent(
  TcpIp_SockHndType SocketId,
  IpBase_TcpIpEventType Event );

#define TLS_STOP_SEC_CODE
#include "MemMap.h"
#endif 
  /* TLS_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: Tls_Cbk.h
 *********************************************************************************************************************/
