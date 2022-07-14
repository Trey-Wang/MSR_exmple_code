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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Etm_Cbk.h
 *        \brief  Etm header file
 *
 *      \details  Implementation of the Etm (Ethernet Testability Module) module. This module implements the
 *                AUTOSAR Testability Protocol and Service Primitives protocol for interaction with external testers
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined(ETM_CBK_H)
# define ETM_CBK_H

/**********************************************************************************************************************
 *  INCLUDE
 *********************************************************************************************************************/
# include "TcpIp_Types.h"
# include "Eth_GeneralTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Etm_RxIndication()
 **********************************************************************************************************************/
/*! \brief        The TCP/IP stack calls this function after the reception of data on a socket.
 *  \param[in]    SocketId        Socket identifier of the related local socket resource.
 *  \param[in]    RemoteAddrPtr   Pointer to memory containing IP address and port of the remote host which sent the data.
 *  \param[in]    BufPtr          Pointer to the received data.
 *  \param[in]    BufLength       Data length of the received TCP segment or UDP datagram.
 *  \return       none
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Reentrant for different SocketIds. Non reentrant for the same SocketId.
 *  \synchronous  Synchronous.
 *  \warning      The frame buffer has to be released later.
 **********************************************************************************************************************/
FUNC(void, ETM_CODE) Etm_RxIndication(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, ETM_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETM_APPL_DATA) BufPtr,
  uint16 BufLength);

/***********************************************************************************************************************
 *  Etm_EthIf_RxIndication()
 **********************************************************************************************************************/
/*! \brief        Indicates the reception of an Ethernet frame.
 *  \param[in]    CtrlIdx       Index of the Ethernet controller within the context of the Ethernet Interface.
 *  \param[in]    FrameType     Frame type of received Ethernet frame.
 *  \param[in]    IsBroadcast   Parameter to indicate a broadcast frame.
 *  \param[in]    PhysAddrPtr   Pointer to Physical source address (MAC address in network byte order) of received Ethernet frame.
 *  \param[in]    DataPtr       Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided).
 *  \param[in]    LenByte       Length of received data.
 *  \return       none
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Don't care.
 *  \synchronous  --
 **********************************************************************************************************************/
FUNC(void, ETM_CODE) Etm_EthIf_RxIndication(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETM_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETM_APPL_DATA) DataPtr,
  uint16 LenByte);

/***********************************************************************************************************************
 *  Etm_TcpAccepted()
 **********************************************************************************************************************/
/*! \brief        This function gets called if the stack put a socket into the listen mode before (as server) and a peer
                  connected to it (as client).
 *  \details      The TCP/IP stack calls this function after a socket was set into the listen state with TcpIp_TcpListen()
                  and a TCP connection is requested by the peer. 
 *  \param[in]    SocketId            Socket identifier of the related local socket resource which has been used at TcpIp_Bind().
 *  \param[in]    SocketIdConnected   Socket identifier of the local socket resource used for the established connection.
 *  \param[in]    RemoteAddrPtr       IP address and port of the remote host.
 *  \return       E_OK                Accepts the established connection.
 *  \return       E_NOT_OK            Refuses the established connection, TcpIp stack shall close the connection.
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Non Reentrant.
 *  \synchronous  Synchronous.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETM_CODE) Etm_TcpAccepted(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, ETM_APPL_DATA) RemoteAddrPtr);

/***********************************************************************************************************************
 *  Etm_TcpPreAccepted()
 **********************************************************************************************************************/
/*! \brief        This function is similar to 'Etm_TcpAccepted'.
 *  \details      This function is similar to 'Etm_TcpAccepted'. This function is called right after receiving a
                  'SYN' on a TCP listen socket and reports the listen socket ID and the ID of the socket that will
                  accept the connection request. These sockets are the same ones that are reported in the offical call
                  'Etm_TcpAccepted' later on. The reported socket IDs may only be used for analysis purposes,
                  the accepting socket may not be used in any way before it is fully connected (reported by call of
                  'Etm_TcpAccepted'). This function is needed if the socket IDs are needed in the TCP states
                  SYN-RECV or SYN-SEND.
 *  \param[in]    SocketId            Socket identifier of the related local socket resource which has been used at TcpIp_Bind().
 *  \param[in]    SocketIdConnected   Socket identifier of the local socket resource used for the established connection.
 *  \param[in]    RemoteAddrPtr       IP address and port of the remote host.
 *  \return       E_OK                Return always E_OK.
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Non Reentrant.
 *  \synchronous  Synchronous.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETM_CODE) Etm_TcpPreAccepted(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, ETM_APPL_DATA) RemoteAddrPtr);

/***********************************************************************************************************************
 *  Etm_TcpConnected()
 **********************************************************************************************************************/
/*! \brief        This function gets called if the stack initiated a TCP connection before (as client) and the peer
                  (the server) acknowledged the connection set up.
 *  \details      The TCP/IP stack calls this function after a socket was requested to connect with TcpIp_TcpConnect()
                  and a TCP connection is confirmed by the peer. The parameter value of SocketId equals the SocketId
                  value of the preceeding TcpIp_TcpConnect() call.
 *  \param[in]    SocketId    Socket identifier of the related local socket resource.
 *  \return       none
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Non Reentrant.
 *  \synchronous  Synchronous.
 **********************************************************************************************************************/
FUNC(void, ETM_CODE) Etm_TcpConnected(
  TcpIp_SocketIdType SocketId);

/***********************************************************************************************************************
 *  Etm_TcpIpEvent()
 **********************************************************************************************************************/
/*! \brief        This function gets called if the stack encounters a condition described by the values in TcpIpEvent.
 *  \param[in]    SocketId    Socket identifier of the related local socket resource.
 *  \param[in]    Event       This parameter contains a description of the event just encountered.
 *  \return       none
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Non Reentrant.
 *  \synchronous  Synchronous.
 **********************************************************************************************************************/
FUNC(void, ETM_CODE) Etm_TcpIpEvent(
  TcpIp_SocketIdType SocketId,
  IpBase_TcpIpEventType Event);

/***********************************************************************************************************************
 *  Etm_LocalIpAddrAssignmentChg()
 **********************************************************************************************************************/
/*! \brief        This function gets called by the TCP/IP stack if an IP address assignment changes
                  (i.e. new address assigned or assigned address becomes invalid).
 *  \param[in]    LocalAddrId   IP address Identifier, representing an IP address specified in the TcpIp module
                                configuraiton (e.g. static IPv4 address on EthIf controller 0).
 *  \param[in]    State         State of IP address assignment.
 *  \return       none
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Non Reentrant.
 *  \synchronous  Synchronous.
 **********************************************************************************************************************/
FUNC(void, ETM_CODE) Etm_LocalIpAddrAssignmentChg(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State);

/***********************************************************************************************************************
 *  Etm_CopyTxData()
 **********************************************************************************************************************/
/*! \brief        This function requests to copy data for transmission to the buffer indicated. This call is triggered
                  by TcpIp_Transmit(). Note: The call to Etm_CopyTxData() may happen in the context of TcpIp_Transmit().
 *  \param[in]    SocketId          Socket identifier of the related local socket resource.
 *  \param[in]    BufPtr            Pointer to buffer for transmission data.
 *  \param[in]    BufLength         Length of provided data buffer.
 *  \return       BUFREQ_OK         Data has been copied to the transmit buffer completely as requested.
 *  \return       BUFREQ_E_NOT_OK   Data has not been copied. Request failed. (No further action for TcpIp required.
                                    Later the upper layer might either close the socket or retry the transmit request)
 *  \pre          Module is initialized.
 *  \context      INTERRUPT
 *  \reentrant    Non Reentrant.
 *  \synchronous  Synchronous.
 **********************************************************************************************************************/
BufReq_ReturnType Etm_CopyTxData(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, ETM_APPL_DATA) BufPtr,
  uint16 BufLength
);

# define ETM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: Etm_Cbk.h
 *********************************************************************************************************************/
