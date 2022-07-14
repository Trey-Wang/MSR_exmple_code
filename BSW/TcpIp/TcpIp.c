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
 *         \file  TcpIp.c
 *        \brief  Implementation of TcpIp Module
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */ /* [L] Number of macro definitions exceeds 1024 - program is non-conforming. */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */ /* [L] Maximum '#if...' nesting exceeds 8 levels - program is non-conforming. */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

#define TCPIP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp.h"
#include "TcpIp_Cbk.h"
#include "TcpIp_Priv.h"
#include "TcpIp_Lcfg.h"

#include "IpBase.h"

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# include "TcpIp_IpV4.h"
# include "TcpIp_IpV4_Cbk.h"   /* needed to forward the RxIndication */
# include "TcpIp_Arp.h"
# include "TcpIp_Arp_Cbk.h"    /* needed to forward the RxIndication */
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# include "TcpIp_IpV6.h"
# include "TcpIp_IpV6_Cbk.h"   /* needed to forward the RxIndication */
# include "TcpIp_IcmpV6.h"
# include "TcpIp_Ndp.h"
#endif

#include "TcpIp_Udp.h"

#if (TCPIP_SUPPORT_TCP == STD_ON)
# include "TcpIp_Tcp.h"
#endif

#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
# include "TcpIp_DhcpV4.h"
#endif

#if (TCPIP_SUPPORT_DHCPV6 == STD_ON)
# include "TcpIp_DhcpV6.h"
#endif

#if (TCPIP_DHCPV4SERVER_ENABLED == STD_ON)
# include "TcpIp_DhcpV4Server.h"
#endif

#if (TCPIP_SUPPORT_TLS == STD_ON)
# include "Tls.h"
# include "Tls_Cbk.h"         /* needed to forward the RxIndication */
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of TcpIp.h file */
#if (  (TCPIP_SW_MAJOR_VERSION != 6u) \
    || (TCPIP_SW_MINOR_VERSION != 4u) \
    || (TCPIP_SW_PATCH_VERSION != 0u) )
  #error "TcpIp.c: Source and Header file are inconsistent!"
#endif

/* Check configuration variant for building the library */
#if ( (defined V_EXTENDED_BUILD_LIB_CHECK) && (TCPIP_CONFIGURATION_VARIANT_PRECOMPILE == TCPIP_CONFIGURATION_VARIANT) )
  #error "TcpIp.c: Switch configuration variant to link-time or post-build for library build!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(TcpIp_TcpIpErrorType, TCPIP_VAR_NOINIT) TcpIp_LastSockError;

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(TcpIp_StateType, TCPIP_VAR_ZERO_INIT) TcpIp_State = TCPIP_STATE_UNINIT;

#define TCPIP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_VSocketAccessExclusive
 **********************************************************************************************************************/
/*! \brief          Checks if another TX request is currently processed on a socket.
 *                  If not, the socket will be locked for other TX requests.
 *  \param[in]      socketId              Socket identifier of the related local socket resource.
 *  \return         TRUE                  Socket has been locked and can be used for transmit.
 *  \return         FALSE                 Socket is already locked and must not be used for transmit.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(boolean, TCPIP_CODE) TcpIp_VSocketAccessExclusive(TcpIp_SocketIdType socketId);

/**********************************************************************************************************************
 *  TcpIp_UdpTransmitInternal()
 *********************************************************************************************************************/
/*! \brief          Transmits a UDP datagram over a UDP socket.
 *  \details        -
 *  \param[in]      socketId            Socket identifier of the related local UDP socket resource.
 *  \param[in]      dataPtr             Payload of the UDP packet.
 *  \param[in]      dataLen             Length of the UDP payload. [bytes]
 *  \param[in]      remoteSockAddrPtr   IP address and port of the remote host.
 *  \return         E_OK                Datagram successfully passed to lower layer for transmission.
 *  \return         E_NOT_OK            Datagram could not be sent for one of the following reasons:
 *                                      - Link layer address resolution in progress.
 *                                      - No free TX buffer form lower layer available.
 *                                      - No suitable IP source address available.
 *                                      - Other permanent error.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmitInternal(
    TcpIp_SocketIdType            socketId,
    TCPIP_P2C(uint8)              dataPtr,            /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint16                        dataLen,
    TCPIP_P2C(TcpIp_SockAddrType) remoteSockAddrPtr); /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */

/**********************************************************************************************************************
  TcpIp_VBindSocketImplicit()
**********************************************************************************************************************/
/*! \brief         Tries to bind a UDP socket to the local address if the socket is not bound already.
 *  \details       -
 *  \param[in]     socketId    Socket identifier of the related local UDP socket resource.
 *  \return        E_OK        Socket successfully bound or was already bound.
 *  \return        E_NOT_OK    Socket could not be bound.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VBindSocketImplicit(TcpIp_SocketIdType socketId);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VCloseTcpSocket
 **********************************************************************************************************************/
/*! \brief         Requests to close the given TCP socket connection and releases all related resources.
 *  \details       Performs the following actions for the socket specified by SocketId in case it is a TCP socket: 
 *                 (a) if the connection is active and 
 *                 (a1) Abort = FALSE: the connection is terminated after performing a regular connection termination handshake and releasing all related resources.
 *                 (a2) Abort = TRUE: connection will immediately be terminated by sending a RST segment and releasing all related resources.
 *                 (b) if the socket is in the Listen state, the Listen state is left immediately and related resources are released.
 *  \param[in]     SocketId Socket identifier of the related local TCP socket resource.
 *  \param[in]     Abort    TRUE: connection will immediately be terminated by sending a RST-Segment and releasing
 *                          all related resources. 
 *                          FALSE: connection will be terminated after performing a regular connection termination
 *                          handshake and releasing all related resources.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE for different socket owners.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VCloseTcpSocket(
    TcpIp_SocketIdType SocketId,
    boolean            Abort);
#endif /* TCPIP_SUPPORT_TCP */

/***********************************************************************************************************************
 *  TcpIp_BindInternal
 **********************************************************************************************************************/
/*! \brief          Binds a UDP or TCP socket to a local resource
 *  \details        Checks whether the socket may be bound, using the given parameters, and does the actual binding.
 *  \param[in]      SocketId            Refer to description of this parameter at function TcpIp_Bind().
 *  \param[in]      LocalAddrId         Refer to description of this parameter at function TcpIp_Bind().
 *  \param[in]      IpAddrId            IPvX controller specific address identifier representing the IPvX address that
 *                                      will be used as source address for transmitted packets.
 *  \param[in,out]  PortPtr             Refer to description of this parameter at function TcpIp_Bind().
 *  \param[out]     ErrorIdPtr          TCPIP_E_NO_ERROR    In case the binding was conducted
 *                                      TCPIP_E_ADDRINUSE   In case a socket with the requested protocol, port and
 *                                                          IpAddrId has already been requested, earlier.
 *  \return         E_OK                The request has been accepted
 *  \return         E_NOT_OK            The request has not been accepted (e.g. address in use)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket IDs.
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BindInternal(
    TcpIp_SocketIdType    SocketId,
    TcpIp_LocalAddrIdType LocalAddrId,
    TcpIp_LocalAddrIdType IpAddrId,
    TCPIP_P2V(uint16)     PortPtr,
    TCPIP_P2V(uint8)      ErrorIdPtr);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VTcpListenSockStateListen
 **********************************************************************************************************************/
/*! \brief          Handles the listen action for a socket that is in state 'listen', already.
 *  \details        -
 *  \param[in]      SocketId           Socket identifier of the related local socket resource.
 *  \param[in]      MaxChannels        Maximum number of new parallel connections established on this listen connection.
 *  \return         E_OK               The request has been accepted
 *  \return         E_NOT_OK           The request has not been accepted
 *  \pre            -
 *  \context        task level
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VTcpListenSockStateListen(
    TcpIp_SocketIdType SocketId,
    uint16             MaxChannels);
#endif

#if (STD_ON == TCPIP_DIAG_ACCESS_EXTENSIONS_SUPPORT)
/**********************************************************************************************************************
 *  TcpIp_DiagDataReadAccessInternal
 *********************************************************************************************************************/
/*! \brief          Reads miscellaneous TcpIp-related diagnostic data
 *  \details        The memory fragment DataPtr points to should be aligned properly regarding the expected returned
 *                  type / struct. Data is only written if RetValue is E_OK.
 *                  This is a Vector extension.
 *  \param[in]      SocketId       Socket identifier of the related local socket resource.
 *  \param[in]      DataID         Valid and checked data identifier
 *  \param[out]     DataPtr        Valid and checked pointer for diagnostic data
 *  \param[in,out]  DataLenPtr     Valid and checked pointer for maximum / actual length of diagnostic data in bytes
 *  \return         E_OK           diagnostic data has been written to the DataPtr and DataLenPtr parameter
 *  \return         E_NOT_OK       invalid parameter (data identifier not found, NULL_PTR parameter, invalid length)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DiagDataReadAccessInternal(
    TcpIp_SocketIdType   SocketId,
    TcpIp_DiagParamsType DataID,
    TCPIP_P2V(uint8)     DataPtr,
    TCPIP_P2V(uint16)    DataLenPtr);
#endif

/***********************************************************************************************************************
 *  TcpIp_VGetSocketForUser
 **********************************************************************************************************************/
/*! \brief          Requests allocation of a new socket (TCP or UDP).
 *  \details        Each accepted incoming TCP connection also allocates a socket resource. Server sockets are special,
 *                  because they do not need any RX and TX buffer and a lot less socket description parameters.
 *  \param[in]      Domain            Unchecked IP address family (IpV4 or IpV6).
 *                                    The parameter 'Domain' doesn't have to be checked since our socket concept always
 *                                    supports both IP versions and 'Domain' is simply taken over into the socket data.
 *  \param[in]      Protocol          Valid and checked socket protocol as sub-family of parameter type (TCP or UDP)
 *  \param[out]     SocketIdPtr       Valid and checked pointer to socket identifier representing the requested socket.
 *                                    This socket identifier must be provided for all further API calls which requires a SocketId.
 *                                    SocketIdPtr is only valid if return value is E_OK.
 *  \param[in]      UserIndex         Valid and checked identifier of the socket user (e.g. TLS or HTTP)
 *  \return         E_OK              The request has been accepted
 *  \return         E_NOT_OK          The request has not been accepted: no free socket
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetSocketForUser(
    TcpIp_DomainType              Domain,
    TcpIp_ProtocolType            Protocol,
    TCPIP_P2V(TcpIp_SocketIdType) SocketIdPtr,
    uint8                         UserIndex);

/***********************************************************************************************************************
 *  TcpIp_VProvideTxBufferTcp
 **********************************************************************************************************************/
/*! \brief          Requests a TX buffer for sending data (internal) using the TCP protocol.
 *  \details        Called by deprecated AUTOSAR 4.0.3. API TcpIp_ProvideTxBufferInt.
 *                  It will be removed together with this API in future revisions of this module.
 *  \param[out]     PbufPtr               pointer for the data
 *  \param[in]      Length                data length in bytes
 *  \param[in]      SockIdx               socket index (derived from SocketHnd of the caller TcpIp_ProvideTxBufferInt())
 *  \param[in]      PbufArrayIdx          position in array of the array of TcpIp_PbufType
 *  \return         BUFREQ_OK             buffer provided 
 *  \return         BUFREQ_E_NOT_OK       provide buffer failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket owners
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_VProvideTxBufferTcp(
    TCPIP_P2V(TcpIp_PbufType) *PbufPtr,
    uint32                     Length,
    TcpIp_SockIdxType          SockIdx,
    uint8                      PbufArrayIdx);

/***********************************************************************************************************************
 *  TcpIp_VProvideTxBufferUdp
 **********************************************************************************************************************/
/*! \brief          Requests a TX buffer for sending data (internal) using the UDP protocol.
 *  \details        Called by deprecated AUTOSAR 4.0.3. API TcpIp_ProvideTxBufferInt.
 *                  It will be removed together with this API in future revisions of this module.
 *  \param[in]      DestinationPtr        destination network address and port
 *  \param[out]     PbufPtr               pointer for the data
 *  \param[in]      Length                data length in bytes
 *  \param[in]      SockIdx               socket index (derived from SocketHnd of the caller TcpIp_ProvideTxBufferInt())
 *  \param[in]      PbufArrayIdx          position in array of the array of TcpIp_PbufType
 *  \return         BUFREQ_OK             buffer provided 
 *  \return         BUFREQ_E_NOT_OK       provide buffer failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket owners
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_VProvideTxBufferUdp(
    TCPIP_P2C(TcpIp_SockAddrType)  DestinationPtr,
    TCPIP_P2V(TcpIp_PbufType)     *PbufPtr,
    uint32                         Length,
    TcpIp_SockIdxType              SockIdx,
    uint8                          PbufArrayIdx);

/***********************************************************************************************************************
 *  TcpIp_VProvideTxBufferInt
 **********************************************************************************************************************/
/*! \brief          Requests a TX buffer for sending data (internal).
 *  \details        Called by deprecated AUTOSAR 4.0.3. API TcpIp_ProvideTxBufferInt.
 *                  Will be removed together with this API in future revisions of this module.
 *  \param[in]      SockIdx               socket index (derived from SocketHnd of the caller TcpIp_ProvideTxBufferInt())
 *  \param[in]      DestinationPtr        destination network address and port
 *  \param[out]     PbufPtr               pointer for the data
 *  \param[in]      Length                data length in bytes
 *  \return         BUFREQ_OK             buffer provided 
 *  \return         BUFREQ_E_NOT_OK       provide buffer failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket owners
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_VProvideTxBufferInt(
    TcpIp_SockHndType              SockIdx,
    TCPIP_P2C(TcpIp_SockAddrType)  DestinationPtr,
    TCPIP_P2V(TcpIp_PbufType)     *PbufPtr,
    uint32                         Length,
    uint8                          PbufArrayIdx);

#ifndef TCPIP_DECLARE_OWN_MAINFUNCTION
# if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
/* Refer to TcpIp.h for documentation and doxygen comments of the following functions: */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MainFunctionRx(void);
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MainFunctionState(void);
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MainFunctionTx(void);
# endif
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_InitMemory( void )
{
  /* #10 Set the TcpIp initialization state to 'not initialized'. */
  TcpIp_State = TCPIP_STATE_UNINIT;

#if (TCPIP_DHCPV4SERVER_ENABLED == STD_ON)
  /* #20 Call the InitMemory function of the submodule DhcpV4Server in case it is configured. */
  TcpIp_DhcpV4Server_InitMemory();
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
  /* #30 Call the InitMemory function of the submodule IpV4 in case it is configured. */
  IpV4_InitMemory();
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  /* #40 Call the InitMemory function of the submodule IpV6 in case it is configured. */
  IpV6_InitMemory();
#endif
}

/***********************************************************************************************************************
 *  TcpIp_Init
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 * */
FUNC(void, TCPIP_CODE) TcpIp_Init(
    TCPIP_P2C(TcpIp_ConfigType) ConfigPtr)  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  TCPIP_UNUSED_PARAMETER(ConfigPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  /* #10 Initialize the submodules (IPv4, IPv6, TCP, UDP), if configured. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
  IpV4_Init(NULL_PTR);
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
  IpV6_Init(NULL_PTR);
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
  TcpIp_Tcp_Init();
#endif

  TcpIp_Udp_Init();

  /* #20 Initialize socket-related data structures. */
  TcpIp_VInitSockets();

  /* #30 Initialize address-related data structures. */
  {
    uint8_least localAddrId;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

    for (localAddrId = 0; localAddrId < TCPIP_MAX_NUM_IP_ADDR_IDS; localAddrId++)
    {
      TcpIp_IpAddrAssignmentState[localAddrId] = TCPIP_IPADDR_STATE_UNASSIGNED;
    }
  }

  /* #40 Initialize the DHCPv4 client submodule, if configured. */
#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
  TcpIp_DhcpV4_Init();
#endif

  /* #50 Initialize the DHCPv6 client submodule, if configured. */
#if (TCPIP_SUPPORT_DHCPV6 == STD_ON)
  TcpIp_DhcpV6_Init(NULL_PTR);
#endif

  /* #60 Initialize the DHCPv4 server submodule, if configured. */
#if (TCPIP_DHCPV4SERVER_ENABLED == STD_ON)
  TcpIp_DhcpV4Server_Init(NULL_PTR);
#endif

  /* #70 Set module state to INIT and active state of the module to ONLINE. */
  TcpIp_ActiveState = TCPIP_ACTIVE_STATE_ONLINE;
  TcpIp_State       = TCPIP_STATE_INIT;
}

/***********************************************************************************************************************
 *  TcpIp_Shutdown
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 * */
FUNC(void, TCPIP_CODE) TcpIp_Shutdown( void )
{
  /* #10 Set the modules' 'active state' to OFFLINE. */
  TcpIp_ActiveState = TCPIP_ACTIVE_STATE_OFFLINE;
} /* TcpIp_Shutdown() */

/***********************************************************************************************************************
 *  TcpIp_RxIndication
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
FUNC(void, TCPIP_CODE) TcpIp_RxIndication(
    uint8            CtrlIdx,
    Eth_FrameType    FrameType,
    boolean          IsBroadcast,
    TCPIP_P2V(uint8) PhysAddrPtr,
    TCPIP_P2V(uint8) DataPtr,
    uint16           LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check of parameter 'CtrlIdx' is done by the RxIndication functions of the sub-modules! */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Call sub-module-specific RxIndication functions depending on the FrameType. */
    switch(FrameType)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    /* #30 In case IpV4 is configured, call the IpV4-specific RxIndication function for FrameType IpV4. */
    case TCPIP_ETH_FRAME_TYPE_IPV4:
    {
      IpV4_Ip_RxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_x */
      break;
    }

    /* #40 In case IpV4 is configured, call the IpV4/ARP-specific RxIndication function for FrameType IpV4/ARP. */
    case TCPIP_ETH_FRAME_TYPE_ARP:
    {
      IpV4_Arp_RxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_x */
      break;
    }
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    /* #50 In case IpV6 is configured, call the IpV6-specific RxIndication function for FrameType IpV6. */
    case TCPIP_ETH_FRAME_TYPE_IPV6:
    {
      IpV6_RxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_x */
      break;
    }
#endif
    default:
      /* TcpIp called for unsupported FrameType. -> ignore this message */
      errorId = TCPIP_E_INV_ARG;
      break;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_RXINDICATION, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* TcpIp_RxIndication() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_GetSocketForUser
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetSocketForUser(
    TcpIp_DomainType              Domain,
    TcpIp_ProtocolType            Protocol,
    TCPIP_P2V(TcpIp_SocketIdType) SocketIdPtr,
    uint8                         UserIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'Domain' */
  else if (!TCPIP_COND_VALID_DOMAIN(Domain))
  {
    errorId = TCPIP_E_AFNOSUPPORT;
  }
  /* Parameter 'Protocol' is checked below. */
  /* Check parameter 'SocketIdPtr' */
  else if (SocketIdPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'UserIndex' */
  else if (UserIndex >= TCPIP_UNDEFINED_SOCK_OWNER_IDX)
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  else
  /* ----- Implementation ----------------------------------------------- */
#endif
  /* #20 Check the protocol to be supported. */
  if(    (TCPIP_IPPROTO_UDP == Protocol)
# if (TCPIP_SUPPORT_TCP == STD_ON)
      || (TCPIP_IPPROTO_TCP == Protocol)
# endif
    )
  {
    /* #30 Forward the request to the internal helper function. */
    retVal = TcpIp_VGetSocketForUser(Domain, Protocol, SocketIdPtr, UserIndex);
  }
  else
  {
    /* MISRA: The protocol is not supported - leave retVal at value E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void) Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_SOCKET, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetSocketForUser() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_TCP_STMIF */

/***********************************************************************************************************************
 *  TcpIp_VGetSocketForUser
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetSocketForUser(
    TcpIp_DomainType              Domain,
    TcpIp_ProtocolType            Protocol,
    TCPIP_P2V(TcpIp_SocketIdType) SocketIdPtr,
    uint8                         UserIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  TcpIp_SocketIdType socketId;
  TcpIp_SocketIdType socketIdStart;
  TcpIp_SocketIdType socketIdEnd;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine relevant sockets based on 'Protocol'. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (Protocol == TCPIP_IPPROTO_TCP)
  {
    socketIdStart = TcpIp_VCfgGetMaxNumUdpSocks();
    socketIdEnd   = TcpIp_VCfgGetMaxNumSocks();
  }
  else
#else
  TCPIP_UNUSED_PARAMETER(Protocol);
#endif
  {
    socketIdStart = 0;
    socketIdEnd   = TcpIp_VCfgGetMaxNumUdpSocks();
  }

  /* #20 Iterate over relevant sockets looking for a 'free' socket which is currently not assigned to a user. */
  for(socketId = socketIdStart; socketId < socketIdEnd; socketId++)
  {
    if (TcpIp_SockUserIdx[socketId] == TCPIP_UNDEFINED_SOCK_OWNER_IDX)
    {
      break;
    }
  }

  /* #30 In case a free socket has been found, allocate the socket for this user. */
  if (socketId < socketIdEnd)
  {
    TcpIp_VPreconfigSocket(socketId);
    TcpIp_SockListenActive[socketId].ConnStat = TCPIP_SOCK_CONN_ALLOCATED;

    ((IpBase_SockAddrType*)(&TcpIp_SocketList[socketId].LocSock))->sa_family = Domain;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

    /* Initialize TCP socket parameters, if socket is a TCP socket. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
    if (Protocol == TCPIP_IPPROTO_TCP)
    {
      TcpIp_Tcp_InitSock(socketId);
    }
#endif

    *SocketIdPtr = socketId;
    TcpIp_SockUserIdx[socketId] = UserIndex;

    retVal = E_OK;
  }
  /* else: No free socket was found */

  return retVal;
} /* TcpIp_VGetSocketForUser() */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_TcpListen
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpListen(
    TcpIp_SocketIdType SocketId,
    uint16             MaxChannels)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'MaxChannels' */
  else if (MaxChannels >= TcpIp_VCfgGetMaxNumTcpSocks())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 In case the socket is in listen state, already: Handle the request in that state */
    if (TcpIp_SockListenActive[SocketId].ConnStat == TCPIP_SOCK_CONN_LISTEN_ENABLED)
    {
      retVal = TcpIp_VTcpListenSockStateListen(SocketId, MaxChannels);
    }
    else
    {
      /* #30 In case the socket is not yet in listen state: Bring the socket into that state */
      if (TcpIp_SockListenActive[SocketId].ConnStat == TCPIP_SOCK_CONN_ALLOCATED)
      {
        /* TCP: Passive open: ( TcpIp_VTcpListenSockStateAllocated ) */
        TcpIp_TcpIdxType      TcpIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
        TcpIp_SockAddrInXType SockAddrRem;  /* remote socket */

        /* #40 Set remote address to 'undefined' */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
        (void)IpBase_DelSockAddr( ((IpBase_SockAddrType *)&SockAddrRem), IPBASE_AF_INET6 );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#else
        (void)IpBase_DelSockAddr( ((IpBase_SockAddrType *)&SockAddrRem), IPBASE_AF_INET );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#endif

        /* #50 Try to open the TCP socket in passive mode and inform the caller about the result */
        TcpIdx = TcpIp_Tcp_Open(SocketId, (IpBase_SockAddrType*)&SockAddrRem, TCPIP_TCP_PASSIVE_OPEN);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        if (TcpIdx != TCPIP_TCP_INV_IDX)
        {
          TcpIp_SockListenActive[SocketId].ConnStat = TCPIP_SOCK_CONN_LISTEN_ENABLED;
          TcpIp_SocketList[SocketId].IsTcpListenSocket = TRUE;
          TcpIp_SocketList[SocketId].TcpMaxNumListenSockets = MaxChannels;  /* set limit */
          retVal = E_OK;
        }
        /* else: Open failed: Leave retVal at value E_NOT_OK */
      }
      /* else: Socket is in an invalid state - leave retVal at E_NOT_OK. */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_LISTEN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_Listen() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_VTcpListenSockStateListen
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
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VTcpListenSockStateListen(
    TcpIp_SocketIdType SocketId,
    uint16             MaxChannels )
{
  Std_ReturnType retVal = E_OK;

  /* #10 In case the given MaxChannels value has changed, check if more than MaxChannels sockets have been derived
         from the current socket and set the TcpIp-internal limit to the new MaxChannels value */
  if(MaxChannels != TcpIp_SocketList[SocketId].TcpMaxNumListenSockets)
  {
    /* #20 In case MaxChannels is smaller than the TcpIp-internal limit iterate all sockets and count those that are related to the given SocketId */
    if(MaxChannels < TcpIp_SocketList[SocketId].TcpMaxNumListenSockets)
    {
      /* Check if more than MaxChannels sockets have been derived from the current socket */
      TcpIp_SocketIdType SockCnt = 0;
      TcpIp_SocketIdType LocSockId;
      for(LocSockId = TcpIp_VCfgGetMaxNumUdpSocks(); LocSockId < TcpIp_VCfgGetMaxNumSocks(); LocSockId++)
      {
        if(TcpIp_SocketList[LocSockId].TcpMasterListenSockId == SocketId)
        {
          SockCnt++;
        }
      }
      /* #30 In case the number of allocated sockets related to the given SocketId has increased: Set the TcpIp-internal limit to the new value */
      if(MaxChannels >= SockCnt)
      {
        TcpIp_SocketList[SocketId].TcpMaxNumListenSockets = MaxChannels;
      }
      /* #40 Indicated an error in case the given MaxChannels value is larger than the counted number of related sockets */
      else
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* #50 In case MaxChannels is bigger than the currently configured value, simply set the TcpIp-internal limit to the new value */
      TcpIp_SocketList[SocketId].TcpMaxNumListenSockets = MaxChannels;
    }
  }
  /* else: Leave retVal at value E_OK */
  return retVal;
} /* TcpIp_VTcpListenSockStateListen() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH , MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_TcpConnect
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpConnect(
    TcpIp_SocketIdType            SocketId,
    TCPIP_P2C(TcpIp_SockAddrType) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'RemoteAddrPtr' */
  else if (RemoteAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'RemoteAddrPtr' */
  else if(TcpIp_VNetAddrIsUnicast((const IpBase_SockAddrType*)RemoteAddrPtr) == FALSE)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  /* ----- Implementation ----------------------------------------------- */
  /* #20 Connect the socket, in case the socket is properly bound. */
  if(TCPIP_SOCK_CONN_ALLOCATED == TcpIp_SockListenActive[SocketId].ConnStat)
  {
    /* #30 Assign a random port number in case the socket (identified by SocketId) has not yet been assigned a valid port number. */
    if(0 == ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SocketId].LocSock))->sin_port)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    {
      uint16 randomPort = 0;
      uint8 localSocketId;
      uint8 socketIdMin = TcpIp_VCfgGetMaxNumUdpSocks();
      uint8 socketIdMax = TcpIp_VCfgGetMaxNumSocks() - 1;

      /* #40 Iterate all sockets to find a unique random local port. */
      while(randomPort == 0)
      {
        /* #50 Determine a random port number using the dynamic port range defined by IANA: minimum value is 0xC000, max 0xFFFF. */
        randomPort = TcpIp_GetRandNoFct();
        if(IPBASE_HTON16(randomPort) == ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SocketId].LocSock))->sin_port)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        {
          randomPort += 1;
        }
        if(randomPort < 0xC000u)
        {
          /* Use the dynamic port range defined by IANA. Min value is 0xC000, max 0xFFFF. */
          randomPort |= (uint16)0xC000u;
        }
        ((IpBase_SockAddrInType *)(&TcpIp_SocketList[SocketId].LocSock))->sin_port = IPBASE_HTON16(randomPort);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

        /* #60 Check that the random port number is not used on any other TCP socket. */
        for(localSocketId = socketIdMin; localSocketId <= socketIdMax; localSocketId++)
        {
          /* Check if port of the socket is the same as current new random value or is '0' */
          if( (localSocketId != SocketId) &&  (((TcpIp_SockAddrInetType*)(&TcpIp_SocketList[localSocketId].LocSock))->port == randomPort) )  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
          {
            /* In case a socket with the same local port already exists, set randomPort to an invalid value in order to stay in the enclosing while-loop */
            randomPort = 0;
            break;
          }
        }
      }
    }
    else
    {
      /* Leave retVal at value E_NOT_OK. */
    }

    /* #70 Actually open the TCP connection using the given SocketId. */
    if( TCPIP_TCP_INV_IDX != TcpIp_Tcp_Open(SocketId, (const IpBase_SockAddrType *)RemoteAddrPtr, TCPIP_TCP_ACTIVE_OPEN))  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    {
      TcpIp_SockListenActive[SocketId].ConnStat = TCPIP_SOCK_CONN_CONNECTED;
      retVal = E_OK;
    }
    /* else: Leave retVal at value E_NOT_OK. */
  }
  else
  {
    /* Leave retVal at value E_NOT_OK. */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_CONNECT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_TcpTransmit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpTransmit(
    TcpIp_SocketIdType SocketId,
    TCPIP_P2V(uint8)   DataPtr,  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint32             AvailableLength,
    boolean            ForceRetrieve)  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (TCPIP_COND_VALID_TCPSOCKIDX(SocketId) == FALSE)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'AvailableLength' */
  else if (AvailableLength > 0xFFFFu)
  {
    /* This implementation is limited to 64k per transmit. */
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    BufReq_ReturnType  bufReqRetVal;
    IpBase_PbufType   *pbufPtr;

    TCPIP_UNUSED_PARAMETER(ForceRetrieve);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

    /* This implementation only accepts a transmit request if the whole data indicated by 'AvailableLength' can be stored into the TCP tx buffer. 
     * Therefore the value of 'ForceRetrieve' == FALSE is ignored.
     */

    /* #20 Try to obtain buffer segment from TCP TX ring-buffer. */
    bufReqRetVal = TcpIp_ProvideTxBuffer(SocketId, (TCPIP_P2C(TcpIp_SockAddrType))NULL_PTR, &pbufPtr, AvailableLength);

    if (bufReqRetVal == BUFREQ_OK)
    {
      /* TCP TxBuffer is available. */
      /* #30 If indirect data Provision is used (DataPtr == NULL_PTR). */
      if (NULL_PTR == DataPtr)
      {
        uint16 copyTxDataLen = pbufPtr->len;

        /* #40 Call <Up>_CopyTxData() of upper-layer. */
        bufReqRetVal = TcpIp_CopyTxDataFunctions(TcpIp_SockUserIdx[SocketId], SocketId, pbufPtr->payload, &copyTxDataLen);

        if (bufReqRetVal == BUFREQ_OK)
        {
          if (copyTxDataLen < pbufPtr->len)
          {
            /* Upper layer provided less data than requested. */
            pbufPtr->len    = copyTxDataLen;
            pbufPtr->totLen = copyTxDataLen;
          }
          else if (pbufPtr->len < pbufPtr->totLen)
          {
            /* Another chuck of data is required due to a buffer wrap around in the TCP ring buffer. */
            copyTxDataLen = pbufPtr[1].len;
            /* #50 Call <Up>_CopyTxData() of upper-layer again for second TCP buffer segment, if wrap-around in TCP ring-buffer occurred. */
            /* assumption: TCP uses a ring buffer and therefore a maximum of two buffer fragments can occur in the PBuf. Otherwise a loop would be needed here. */
            bufReqRetVal = TcpIp_CopyTxDataFunctions(TcpIp_SockUserIdx[SocketId], SocketId, (&pbufPtr[1])->payload, &copyTxDataLen);

            if ((bufReqRetVal == BUFREQ_OK) && (copyTxDataLen < pbufPtr[1].len))
            {
              /* Upper layer provided less data than requested. */
              pbufPtr[1].len  = copyTxDataLen;
              pbufPtr->totLen = pbufPtr->len + copyTxDataLen;
            }
          }
          else
          {
            /* User provided expected length of data. */
          }
        }
      }
      else
      {
        /* #60 Copy data from DataPtr into TCP ring-buffer if data was provided directly. */
        /* copy data to the buffer */
        /* data is already provided in this function call, so send in the same way as in ASR 4.0.x */
        /* copy */
        (void)IpBase_CopyString2PbufAt(DataPtr, (uint16)AvailableLength, pbufPtr, 0);
      }

      /* #70 Trigger transmission of data or free TX buffer if an error occurred. */
      if (bufReqRetVal == BUFREQ_OK)
      {
        retVal = TcpIp_TransmitTo(SocketId, pbufPtr, NULL_PTR);  /* destination is ignored since it is already set during connection setup */
      }
      else
      {
        /* CopyTxData returned negative result. Cancel transmit. */
        pbufPtr->totLen = 0;
        (void)TcpIp_TransmitTo(SocketId, pbufPtr, NULL_PTR);  /* destination is ignored since it is already set during connection setup */
        /* Do not change retVal. Return E_NOT_OK. */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_TRANSMIT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
}  /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif  /* (TCPIP_SUPPORT_TCP == STD_ON) */ 

/***********************************************************************************************************************
 *  TcpIp_VBindSocketImplicit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VBindSocketImplicit(TcpIp_SocketIdType socketId)
{
  boolean socketIsBound;

  /* #10 Try to bind the socket to the local address, if socket is not already bound. */
  if (((TCPIP_P2C(IpBase_SockAddrInType))&TcpIp_SocketList[socketId].LocSock)->sin_port > 0)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    socketIsBound = TRUE;
  }
  /* #20 In case the socket is unbound: Try to bind it implicitly to a dynamic port. */
  /* \trace SPEC-50982 */
  else if (TcpIp_Bind(socketId, TcpIp_SocketList[socketId].BindLocalAddrId, &(((TCPIP_P2V(IpBase_SockAddrInType))&TcpIp_SocketList[socketId].LocSock)->sin_port)) == E_OK)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    socketIsBound = TRUE;
  }
  else
  {
    socketIsBound = FALSE;
  }

  return socketIsBound;
}

/***********************************************************************************************************************
 *  TcpIp_UdpTransmit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmit(
    TcpIp_SocketIdType            SocketId,
    TCPIP_P2V(uint8)              DataPtr,        /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    TCPIP_P2V(TcpIp_SockAddrType) RemoteAddrPtr,  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint16                        TotalLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType  retVal  = TCPIP_E_NOT_OK;
  uint8             errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (TCPIP_COND_VALID_UDPSOCKIDX(SocketId) == FALSE)
  {
    /* Passed socket ID does not represent a UDP socket. */
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'RemoteAddrPtr' */
  else if (RemoteAddrPtr == NULL_PTR)
  {
    /* RemoteAddrPtr not set. */
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (TCPIP_COND_VALID_ADDR_FAM(RemoteAddrPtr) == FALSE)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  {
    /* RemoteAddrPtr has invalid address family. */
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
    /* #20 Try to send queued UDP packets that are waiting for link layer address resolution. */
    (void)TcpIp_Udp_SendRetryQueueIfPossible(SocketId, RemoteAddrPtr);
#endif

    if (TcpIp_ActiveState != TCPIP_ACTIVE_STATE_ONLINE)
    {
      /* Reject Transmit because TcpIp module is OFFLINE. */
    }
    else if (TcpIp_SockListenActive[SocketId].ConnStat == TCPIP_SOCK_CONN_NONE)
    {
      /* Reject Transmit because socket is not open. */
    }
    else
    {
      /* #30 Check if UDP socket is bound. If not, try to bind the socket to a dynamic port and the specified remote address. */
      if (TcpIp_VBindSocketImplicit(SocketId) == TRUE)
      {
        /* #40 Obtain semaphore for the socket. */
        if (TcpIp_VSocketAccessExclusive(SocketId) == TRUE)
        {
          /* #50 Call UdpTransmitInternal function. */
          retVal = TcpIp_UdpTransmitInternal(SocketId, DataPtr, TotalLength, RemoteAddrPtr);

          /* #60 Release semaphore for the socket. */
          TcpIp_TxBufRequested[SocketId] = FALSE;
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_UDP_TRANSMIT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_UdpTransmit() */  /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STMIF, MD_MSR_STCAL, MD_MSR_STIMF */

/***********************************************************************************************************************
 *  TcpIp_UdpTransmitInternal
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
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmitInternal(
    TcpIp_SocketIdType            socketId,
    TCPIP_P2C(uint8)              dataPtr,            /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint16                        dataLen,
    TCPIP_P2C(TcpIp_SockAddrType) remoteSockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = TCPIP_E_NOT_OK;
  uint16            udpPayloadBufferLen = dataLen;
  BufReq_ReturnType udpBufReqRetVal;
  TCPIP_P2V(uint8)  udpPayloadPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request TX buffer for UDP packet from lower layer.
   *     Multiple buffer segments may be provided by the IP if fragmentation is required. */
  /* If TotalLength exceeds the payload that fits into one IP packet TcpIp_Udp_RequestTxBuffer() may only provide a buffer for the first payload fragment.
   * Additional buffers for the other payload fragments are obtained by TcpIp_Udp_FragmentRequestTxBuffer() later.
   */
  udpBufReqRetVal = TcpIp_Udp_RequestTxBuffer(socketId, (TCPIP_P2C(IpBase_SockAddrType))remoteSockAddrPtr, &udpPayloadPtr, &udpPayloadBufferLen);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

  if (udpBufReqRetVal == BUFREQ_OK)
  {
    boolean copyDataSuccessful = TRUE;
    uint16  dataPos = 0;

    /* a buffer could be provided successfully */
    TcpIp_TxRequestedLen[socketId] = dataLen;

    /* #20 Iterate over all provided UDP payload buffer fragments and copy upper layer data. */
    while (dataLen > 0)
    {
      /* Provided buffer may be larger than required. */
      if (dataLen < udpPayloadBufferLen)
      {
        udpPayloadBufferLen = dataLen;
      }

      /* #30 Copy data from upper layer into TX buffer, if data was provided. */
      if (dataPtr != NULL_PTR)
      {
        /* directly copy provided data from user into provided UDP buffer */
        IpBase_Copy((TCPIP_P2V(IpBase_CopyDataType))udpPayloadPtr, (TCPIP_P2C(IpBase_SockAddrType))&dataPtr[dataPos], udpPayloadBufferLen);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        dataPos += udpPayloadBufferLen;
        dataLen -= udpPayloadBufferLen;
      }
      /* #40 ELSE: Trigger upper layer to copy TX data into buffer by calling CopyTxData. */
      else
      {
        uint16 udpTxDataLen = udpPayloadBufferLen;

        /* call CopyTxData function of user */
        if (TcpIp_CopyTxDataFunctions(TcpIp_SockUserIdx[socketId], socketId, udpPayloadPtr, &udpTxDataLen) == BUFREQ_OK)
        {
          if (udpTxDataLen < udpPayloadBufferLen)
          {
            /* Upper layer provided less data than requested. Adapt requested length and do not copy more data. */
            TcpIp_TxRequestedLen[socketId] -= (dataLen - udpTxDataLen);
            dataLen = 0;
          }
          else
          {
            dataLen -= udpPayloadBufferLen;
          }
        }
        else
        {
          /* ERROR: CopyTxData of upper layer failed. Transmit will be aborted. */
          copyDataSuccessful = FALSE;
          dataLen = 0;
        }
      }

      /* #50 Request next TX buffer segment from lower layer if required. */
      if (dataLen > 0)
      {
        /* Request next UDP payload buffer fragment */
        if (E_OK != TcpIp_Udp_FragmentRequestTxBuffer(socketId, remoteSockAddrPtr->domain, &udpPayloadPtr, &udpPayloadBufferLen))
        {
          /* ERROR: This should not happen. If TcpIp_Udp_RequestTxBuffer() returns BUFREQ_OK there should be sufficient buffer space */
          TcpIp_CallDetReportError(TCPIP_API_ID_UDP_TRANSMIT, TCPIP_E_NOBUFS)
        }
      }
    }

    /* #60 Transmit UDP datagram if payload was successfully copied into TX buffer.
     *     Otherwise: Abort transmission. */
    if (copyDataSuccessful == TRUE)
    {
      TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (TcpIp_Udp_Transmit(socketId, (uint16)TcpIp_TxRequestedLen[socketId], (TCPIP_P2C(IpBase_SockAddrType))remoteSockAddrPtr) == E_OK)  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      {
        retVal = TCPIP_OK;
      }

      TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    else
    {
      TcpIp_Udp_TransmitCancel(socketId, (TCPIP_P2C(IpBase_SockAddrType))remoteSockAddrPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    }
  }
  else
  {
    /* No Ethernet buffer available or link-layer address resolution pending and no free element in UDP retry queue. */
    retVal = TCPIP_E_NOT_OK;
  }

  return retVal;
} /* TcpIp_UdpTransmitInternal() */  /* PRQA S 6010, 6030, 6080, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF, MD_MSR_STCAL */

/***********************************************************************************************************************
 *  TcpIp_IcmpV6Transmit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IcmpV6Transmit(
    TcpIp_LocalAddrIdType          LocalIpAddrId,
    TCPIP_P2C(TcpIp_SockAddrType)  RemoteAddrPtr,
    uint8                          HopLimit,
    uint8                          Type,
    uint8                          Code,
    uint16                         DataLength,
    TCPIP_P2C(uint8)               DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (TCPIP_LOCAL_ADDR_IS_AF_INET6(LocalIpAddrId) == FALSE)
  {
    /* LocalIpAddrId invalid. */
    errorId = TCPIP_E_INV_ARG;
  }
  else if (RemoteAddrPtr == NULL_PTR)
  {
    /* RemoteAddrPtr not set. */
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (RemoteAddrPtr->domain != TCPIP_AF_INET6)
  {
    /* RemoteAddr address family invalid. */
    errorId = TCPIP_E_INV_ARG;
  }
  else if (DataPtr == NULL_PTR)
  {
    /* DataPtr not set. */
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    IpV6_AddrIdType                   ipV6AddrId         = TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalIpAddrId);
    TCPIP_P2C(IpBase_SockAddrIn6Type) remoteAddrInet6Ptr = (TCPIP_P2C(IpBase_SockAddrIn6Type))RemoteAddrPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

    /* #20 Forward call to ICMPv6 submodule. */
    if (IpV6_Icmp_TxRawMessage(&ipV6AddrId, &remoteAddrInet6Ptr->sin6_addr, HopLimit, Type, Code, DataLength, DataPtr) == BUFREQ_OK)
    {
      retVal = E_OK;
    }
#else
    TCPIP_UNUSED_PARAMETER(LocalIpAddrId);
    TCPIP_UNUSED_PARAMETER(RemoteAddrPtr);
    TCPIP_UNUSED_PARAMETER(HopLimit);
    TCPIP_UNUSED_PARAMETER(Type);
    TCPIP_UNUSED_PARAMETER(Code);
    TCPIP_UNUSED_PARAMETER(DataLength);
    TCPIP_UNUSED_PARAMETER(DataPtr);
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_ICMPV6_TRANSMIT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_IcmpV6Transmit() */  /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STIMF */

/***********************************************************************************************************************
 *  TcpIp_ProvideTxBuffer
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_ProvideTxBuffer(
    TcpIp_SockHndType              SockHnd,
    TCPIP_P2C(TcpIp_SockAddrType)  DestinationPtr,
    TCPIP_P2V(TcpIp_PbufType)     *PbufPtr,
    uint32                         Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SockHnd' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SockHnd))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'PbufPtr' */
  else if (PbufPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check that the TcpIp module is in state 'online'. */
    if(TcpIp_ActiveState == TCPIP_ACTIVE_STATE_ONLINE)
    {
#if (TCPIP_SUPPORT_TLS == STD_ON)
      /* #30 Determine the protocol type in case the socket is dedicated to the TLS plugin. */
      TcpIp_SockIdxType SockIdx = TCPIP_SOCKHND2SOCKIDX(SockHnd);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
      uint8 Prot = TCPIP_SOCK_PROT_UDP;  /* Assume UDP by default. */ /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

      if(SockIdx >= TcpIp_VCfgGetMaxNumUdpSocks())
      {
        Prot = TCPIP_SOCK_PROT_TCP;
      }
      /* else: Leave value of 'Prot' at its default value TCPIP_SOCK_PROT_UDP */
#endif

      CANOE_WRITE_STRING_2("TcpIp_ProvideTxBuffer: ProvideTxBuffer-function will be called with SockHnd: %d and Length: %d)", SockHnd, Length)
      /* #40 Forward the request to the internal buffer provision routine or to the TLS plugin depending on which of these the socket is dedicated to. */
#if (TCPIP_SUPPORT_TLS == STD_ON)
      if( (Prot == TCPIP_SOCK_PROT_TCP) && (TcpIp_Tcp_UseTls[TCPIP_SOCKIDX2TCPIDX(SockIdx)] == TRUE) )
      {
        /* forward request to TLS */
        retVal = Tls_ProvideTxBuffer(SockIdx, PbufPtr, Length);
      }
      else
#endif
      {
        retVal = TcpIp_ProvideTxBufferInt(SockHnd, DestinationPtr, PbufPtr, Length);
      }
    }
    /* else: In case the module is not online, no buffer shall be provided: Leave retVal at value BUFREQ_E_NOT_OK. */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_PROVIDE_TX_BUFFER, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_ProvideTxBuffer() */

/***********************************************************************************************************************
 *  TcpIp_ProvideTxBufferInt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_ProvideTxBufferInt(
    TcpIp_SockHndType              SockHnd,
    TCPIP_P2C(TcpIp_SockAddrType)  DestinationPtr,
    TCPIP_P2V(TcpIp_PbufType)     *PbufPtr,
    uint32                         Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SockHnd' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SockHnd))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'PbufPtr' */
  else if (PbufPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'Length' */
  else if (!TCPIP_COND_LEN_MAX_UINT16(Length))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SockIdxType SockIdx = TCPIP_SOCKHND2SOCKIDX(SockHnd);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
    uint8 PbufArrayIdx = (uint8)(2 * SockIdx);

    retVal = TcpIp_VProvideTxBufferInt(SockIdx, DestinationPtr, PbufPtr, Length, PbufArrayIdx);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_PROVIDE_TX_BUFFER_INT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_ProvideTxBufferInt() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/***********************************************************************************************************************
 *  TcpIp_VProvideTxBufferInt
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_VProvideTxBufferInt(
    TcpIp_SockHndType              SockIdx,
    TCPIP_P2C(TcpIp_SockAddrType)  DestinationPtr,
    TCPIP_P2V(TcpIp_PbufType)     *PbufPtr,
    uint32                         Length,
    uint8                          PbufArrayIdx)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  /* #10 Check the given length value for consistency AND the given socket for being 'open'. */
  if(    ((SockIdx < TcpIp_VCfgGetMaxNumUdpSocks()) || (Length > 0))
#if (TCPIP_SUPPORT_TCP == STD_ON)
      && ((SockIdx < TcpIp_VCfgGetMaxNumUdpSocks()) || (Length <= (uint32)(TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx))) ))  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
#endif
      && (TcpIp_SockListenActive[SockIdx].ConnStat != TCPIP_SOCK_CONN_NONE)
    )
  {
    /* #20 Check whether the buffer for this socket has already been requested. */
    TCPIP_VENTER_CRITICAL_SECTION(); /* lock interrupts */ /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if(TcpIp_TxBufRequested[SockIdx] == FALSE)
    {
      TcpIp_TxBufRequested[SockIdx] = TRUE;
      retVal = BUFREQ_OK;
    }
    /* else: buffer is already locked by a previous call of ProvideTxBuffer(): Leave retVal at value BUFREQ_E_BUSY. */
    TCPIP_VLEAVE_CRITICAL_SECTION(); /* unlock interrupts */ /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #30 In case the buffer had not yet been requested, before: Forward the request to the subcomponent of the particular protocol, UDP or TCP. */
    if(retVal == BUFREQ_OK)
    {
      CANOE_WRITE_STRING_2("TcpIp_ProvideTxBuffer: ProvideTxBuffer function will be called with SockHnd: %d and Length: %d)", SockHnd, Length)
      if(SockIdx >= TcpIp_VCfgGetMaxNumUdpSocks())
      {
        /* Protocol is TCP */
#if (TCPIP_SUPPORT_TCP == STD_ON)
        retVal = TcpIp_VProvideTxBufferTcp(PbufPtr, Length, SockIdx, PbufArrayIdx);
#endif
      }
      else
      {
        /* Protocol is UDP */
        retVal = TcpIp_VProvideTxBufferUdp(DestinationPtr, PbufPtr, Length, SockIdx, PbufArrayIdx);
      }
    }
    /* else: The buffer has already been requested, before. */
  }
  /* else: One of the following conditions was true:
  1. Providing a buffer of size '0' makes no sense for TCP.
  2. Requested Length value is bigger than total available TxBuffer.
  3. The given socket is not in state 'open'.
  --> Leave retVal at value BUFREQ_E_NOT_OK */
  return retVal;
}

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VProvideTxBufferTcp
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
 */
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_VProvideTxBufferTcp(
    TCPIP_P2V(TcpIp_PbufType) *PbufPtr,
    uint32                     Length,
    TcpIp_SockIdxType          SockIdx,
    uint8                      PbufArrayIdx)
{
  BufReq_ReturnType retVal;

  /* #10 Check that the desired amount of data fits into the remaining space in the ring buffer. */
  if((uint32)(TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx)) - TcpIp_TxBufFillLevel[SockIdx]) >= Length)  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
  {
    /* Data fits into ring buffer - TCP use case */
    /* #20 Check at the TCP subcomponent, if TX data is available and the socket state is accordingly. */
    if(E_OK != TcpIp_Tcp_TxCheckAndPrepBuf(TCPIP_SOCKIDX2TCPIDX(SockIdx)))  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
    {
      /* #30 In case no TX data is available at the TcpIp subcomponent reset the tx-buffer-request-flag and reject the request. */
      TcpIp_TxBufRequested[SockIdx] = FALSE;  /* reset request to enable further requests */
      retVal = BUFREQ_E_NOT_OK;
    }
    else
    {
      /* #40 In case a TX-buffer is available at the TCP, calculate the remaining length from write postion to end of buffer. */
      /* provide buffer using Pbuf struct */
      uint32 RemBufLenByte = TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx)) - TcpIp_TxBufWritePos[SockIdx];  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
      *PbufPtr      = &TcpIp_TxPbufArray[PbufArrayIdx];

      TcpIp_TxPbufArray[PbufArrayIdx].totLen  = (uint16)Length;
      TcpIp_TxPbufArray[PbufArrayIdx].payload = TcpIp_GetTxBuffersElemPtr(SockIdx, TcpIp_TxBufWritePos[SockIdx]);  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
      /* #50 Handle the wrap-around of the ring-buffer that has possibly happened. */
      if(RemBufLenByte < Length)
      {
        /* ring buffer wrap-around has to be handled */
        TcpIp_TxPbufArray[PbufArrayIdx].len = (uint16)RemBufLenByte;

        TcpIp_TxPbufArray[PbufArrayIdx + 1].totLen  = (uint16)Length;
        TcpIp_TxPbufArray[PbufArrayIdx + 1].payload = TcpIp_GetTxBuffersElemPtr(SockIdx, 0);  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
        TcpIp_TxPbufArray[PbufArrayIdx + 1].len     = (uint16)Length - (uint16)RemBufLenByte;

        TcpIp_TxBufWritePos[SockIdx] = (uint16)Length - RemBufLenByte;
      }
      else
      {
        /* no ring buffer wrap-around, so all memory is in one block */
        TcpIp_TxPbufArray[PbufArrayIdx].len = (uint16)Length;

        TcpIp_TxBufWritePos[SockIdx] += (uint16)Length;
        if(TcpIp_VCfgGetTxBufSizeByte(TCPIP_SOCKIDX2TCPIDX(SockIdx)) == TcpIp_TxBufWritePos[SockIdx])  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
        {
          TcpIp_TxBufWritePos[SockIdx] = 0;
        }
      }
      /* #60 Update the TX-buffer admin data and indicate that the request has been accepted. */
      TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
      TcpIp_TxRequestedLen[SockIdx] = Length;
      TcpIp_TxBufFillLevel[SockIdx] += (uint16)Length;

      TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
      retVal = BUFREQ_OK;
    }
  }
  else
  /* #70 Indicate 'busy', in case the desired amount of data does not fit into the remaining space of the ring buffer. */
  {
    /* currently the remaining TxBuffer size is too small for this request */
    TcpIp_TxBufRequested[SockIdx] = FALSE;  /* reset length to enable further requests */

    retVal = BUFREQ_E_BUSY;
  }
  return retVal;
} /* TcpIp_VProvideTxBufferTcp */
#endif
/* #if (TCPIP_SUPPORT_TCP == STD_ON) */

/***********************************************************************************************************************
 *  TcpIp_VProvideTxBufferUdp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_VProvideTxBufferUdp(
    TCPIP_P2C(TcpIp_SockAddrType)  DestinationPtr,
    TCPIP_P2V(TcpIp_PbufType)     *PbufPtr,
    uint32                         Length,
    TcpIp_SockIdxType              SockIdx,
    uint8                          PbufArrayIdx)
{
  /* #10 Pass the UDP-related TX-buffer-request to the UDP sub-component. */
  /* UDP: check TX request handling array */
  uint16            UdpPayloadLen = (uint16)Length;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint8             *UdpPayloadPtr;
  BufReq_ReturnType retVal = TcpIp_Udp_RequestTxBuffer(SockIdx, (const IpBase_SockAddrType*)DestinationPtr, &UdpPayloadPtr, &UdpPayloadLen);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */

  /* #20 Update the admin-data depending on the result of the provide-tx-buffer-call. */
  if(retVal != BUFREQ_OK)
  {
    TcpIp_TxBufRequested[SockIdx] = FALSE;  /* unlock socket to enable further requests */
  }
  else
  {
    /* A buffer was provided successfully. */
    *PbufPtr = &TcpIp_TxPbufArray[PbufArrayIdx];
    TcpIp_TxPbufArray[PbufArrayIdx].payload = UdpPayloadPtr;
    TcpIp_TxPbufArray[PbufArrayIdx].len     = (uint16)Length;
    TcpIp_TxPbufArray[PbufArrayIdx].totLen  = Length;
    TcpIp_TxBufRequested[SockIdx] = TRUE;  /* lock socket to prevent further requests */
    TcpIp_TxRequestedLen[SockIdx] = Length;
  }
  return retVal;
} /* TcpIp_VProvideTxBufferUdp */

/***********************************************************************************************************************
 *  TcpIp_TransmitTo
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TransmitTo(
    TcpIp_SockHndType             SockHnd,
    TCPIP_P2C(TcpIp_PbufType)     PbufPtr,
    TCPIP_P2C(TcpIp_SockAddrType) DestinationPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SockHnd' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SockHnd))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'PbufPtr' */
  else if (PbufPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SockIdxType SockIdx = TCPIP_SOCKHND2SOCKIDX(SockHnd);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

    /* #20 Check that the socket is 'open' (i.e. operating an active connection). */
    if(TcpIp_SockListenActive[SockIdx].ConnStat != TCPIP_SOCK_CONN_NONE)
    {
      /* #30 Accept the request in case the length is okay or it is a TCP socket that TLS is enabled for. (In this case, size is checked by TLS.) */
      if(    (TcpIp_TxRequestedLen[SockIdx] >= PbufPtr->totLen)
#if ((TCPIP_SUPPORT_TCP == STD_ON) && (TCPIP_SUPPORT_TLS == STD_ON))
             /* In case it is a TCP socket and TLS is enabled for this socket: Size has to be checked in TLS, nothing to do here */
          || ((SockIdx >= TcpIp_VCfgGetMaxNumUdpSocks()) && (TcpIp_Tcp_UseTls[TCPIP_SOCKIDX2TCPIDX(SockIdx)] == TRUE))
#endif
        )
      {
#if ((TCPIP_SUPPORT_TCP == STD_ON) && (TCPIP_SUPPORT_TLS == STD_ON))
        /* #40 Dispatch between the TCP- and TLS-related transmit-call, in case both, TCP and TLS are configured. */
        if(SockIdx >= TcpIp_VCfgGetMaxNumUdpSocks())
        {
          /* This is a TCP socket */
          if(TcpIp_Tcp_UseTls[TCPIP_SOCKIDX2TCPIDX(SockIdx)] == TRUE)
          {
            /* forward request to TLS */
            retVal = Tls_TransmitTo(SockIdx, PbufPtr);
          }
          else
          {
            /* forward request to TcpIp internal function */
            retVal = TcpIp_TransmitToInt(SockHnd, PbufPtr, DestinationPtr);
          }
        }
        else
#endif
        {
          /* #50 In all other cases it is a 'non-TLS' TCP or UDP socket and the request is forwarded to the TcpIp internal helper function. */
          retVal = TcpIp_TransmitToInt(SockHnd, PbufPtr, DestinationPtr);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TRANSMIT_TO, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_TransmitTo() */ /* PRQA S 6080 */ /* MD_TCP_STMIF */

/***********************************************************************************************************************
 *  TcpIp_TransmitToInt
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TransmitToInt(
    TcpIp_SockHndType             SockHnd,
    TCPIP_P2C(TcpIp_PbufType)     PbufPtr,
    TCPIP_P2C(TcpIp_SockAddrType) DestinationPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SockHnd' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SockHnd))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'PbufPtr' */
  else if (PbufPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SockIdxType SockIdx = TCPIP_SOCKHND2SOCKIDX(SockHnd);  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
    TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #20 Check whether the affected socket is an UDP socket. */
    if(TCPIP_COND_VALID_UDPSOCKIDX(SockIdx))
    {
      /* #30 In case the TX data has been provided fully and exactly (by length), pass the transmit request to the UDP subcomponent, otherwise cancel the request at the UDP subcomponent. */
      /* UDP: */
      if (PbufPtr->totLen == PbufPtr->len)
      {
        retVal = TcpIp_Udp_Transmit(SockIdx, PbufPtr->len, (const IpBase_SockAddrType*)DestinationPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
      }
      else
      {
        TcpIp_Udp_TransmitCancel(SockIdx, (const IpBase_SockAddrType*)DestinationPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        retVal = E_OK;
      }
      /* #40 In any case, tree the buffer request lock after the processing of the transmit request. */
      TcpIp_TxBufRequested[SockIdx] = FALSE;
    }
    else
    {
#if (TCPIP_SUPPORT_TCP == STD_ON)
      /* #50 In case it is no UDP socket the affected socket is a TCP socket. */
      /* TCP: */
      /* ignore destination */
      /* #60 Pass the transmit request to the TCP subcomponent in case there still is data left to transmit. */
      if(0 != PbufPtr->totLen)
      {
        if(TcpIp_TxRequestedLen[SockIdx] != PbufPtr->totLen)
        {
          /* transmitted length is smaller than the requested length -> decrease the buffer fill level */
          uint32 LenDiff = TcpIp_TxRequestedLen[SockIdx] - PbufPtr->totLen;
          TcpIp_DecTxBufIdx(SockIdx, TcpIp_TxBufWritePos[SockIdx], LenDiff);
          TcpIp_TxBufFillLevel[SockIdx] -= LenDiff;
        }
        retVal = TcpIp_Tcp_SendAsr(TCPIP_SOCKIDX2TCPIDX(SockIdx), PbufPtr->totLen);
      }
      else
      {
        /* nothing to be sent, DataLen is '0' */
        /* cancel / free the provided tx buffer */
        TcpIp_DecTxBufIdx(SockIdx, TcpIp_TxBufWritePos[SockIdx], TcpIp_TxRequestedLen[SockIdx]);
        TcpIp_TxBufFillLevel[SockIdx] -= TcpIp_TxRequestedLen[SockIdx];
        retVal = E_OK;
      }
      /* #70 In any case, tree the buffer request lock after the processing of the transmit request. */
      TcpIp_TxBufRequested[SockIdx] = FALSE;
#else
      retVal = E_NOT_OK; 
#endif
    }
    TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TRANSMIT_TO_INT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_TransmitToInt() */ /* PRQA S 6080, 6010 */ /* MD_TCP_STMIF, MD_MSR_STPTH */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_TcpReceived
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpReceived(
    TcpIp_SocketIdType SocketId,
    uint32             Length )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'Length' */
  else if (!TCPIP_COND_LEN_MAX_UINT16(Length))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check this TCP socket to be properly requested by a socket user. */
    if(TcpIp_SockUserIdx[SocketId] != TCPIP_UNDEFINED_SOCK_OWNER_IDX)
    {
      /* #30 Dispatch the reception to TCP or TLS according to the sockets configuration. */ 
      if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
      {
# if (TCPIP_SUPPORT_TLS == STD_ON)
        if(TcpIp_Tcp_UseTls[TCPIP_SOCKHND2TCPIDX(SocketId)] == TRUE)
        {
          /* TLS received */
          Tls_Received(SocketId, Length);
        }
        else
# endif
        {
          /* TCP received */
          TcpIp_Tcp_Received(TCPIP_SOCKIDX2TCPIDX(SocketId), Length);
        }
        retVal = E_OK;
      }
      /* else: Invalid socket id: Leave retVal at value E_NOT_OK. */
    }
    /* else: socket is not requested by a user: Leave retVal at value E_NOT_OK. */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_RECEIVED, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* TcpIp_TcpReceived() */ /* PRQA S 6080 */ /* MD_TCP_STMIF */
# endif

/***********************************************************************************************************************
 *  TcpIp_Close
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Close(
    TcpIp_SocketIdType SocketId,
    boolean            Abort )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else
#endif
  /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  /* #20 Check parameter 'SocketId' for the TCP use case. */
  if(TCPIP_COND_VALID_TCPSOCKIDX(SocketId))
  {
    /* #30 In case the SocketId is valid, indicate E_OK. In case the sockets connection status is active and valid, close the socket. */
    retVal = E_OK;
    if(TcpIp_SockListenActive[SocketId].ConnStat != TCPIP_SOCK_CONN_NONE)
    {
      /* TCP */
      TcpIp_VCloseTcpSocket(SocketId, Abort);
    }
    /* else: Socket is closed, already */
  }
  else
#endif  /* (TCPIP_SUPPORT_TCP == STD_ON) */
  /* #40 Check parameter 'SocketId' for the UDP use case. */
  if(TCPIP_COND_VALID_UDPSOCKIDX(SocketId))
  {
    if (TcpIp_SockListenActive[SocketId].ConnStat != TCPIP_SOCK_CONN_NONE)
    {
      /* UDP */
      /* #50 Temporarily store the userIdx of the socket in order to inform upper layer and then actually delete the socket. */
      uint8 tempUserIdx                         = TcpIp_SockUserIdx[SocketId];
      TcpIp_SockUserIdx[SocketId]               = TCPIP_UNDEFINED_SOCK_OWNER_IDX;
      TcpIp_SockListenActive[SocketId].ConnStat = TCPIP_SOCK_CONN_NONE;

      TcpIp_VDelSockAddrMapping(SocketId);
      TcpIp_Udp_CloseSocket(SocketId);
      TcpIp_TcpIpEventFunctions(tempUserIdx, SocketId, TCPIP_UDP_CLOSED);  /* trigger 'CLOSED' event */

      retVal = E_OK;
    }
    /* else: Socket is closed, already */
  }
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  else
  /* #60 In case the socketId is neither valid for TCP nor valid for UDP: Optionally indicate a DET error. */
  {
    errorId = TCPIP_E_INV_ARG;
  }
#endif

#if (TCPIP_SUPPORT_TCP == STD_OFF)
  TCPIP_UNUSED_PARAMETER(Abort);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_CLOSE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_Close() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_VCloseTcpSocket
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
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VCloseTcpSocket(
    TcpIp_SocketIdType SocketId,
    boolean            Abort )
{
  /* #10 Temporarily store userIdx and listenSocket-flag of the socket in order to inform upper layer. Then actually delete the socket. */
  boolean tempIsTcpListenSocket = TcpIp_SocketList[SocketId].IsTcpListenSocket;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint8 tempUserIdx = TcpIp_SockUserIdx[SocketId];

  if(Abort == FALSE)
  {
    /* #20 Trigger the closing of the socket at the TCP. */
    TcpIp_Tcp_Close(TCPIP_SOCKHND2TCPIDX(SocketId));
  }
  else
  {
    /* #30 Trigger the fast closing (abort) of the socket at the TCP and update the admin data to actually delete the socket. */
    TcpIp_Tcp_Abort(TCPIP_SOCKHND2TCPIDX(SocketId));

    /* delete socket */
    TcpIp_SockUserIdx[SocketId] = TCPIP_UNDEFINED_SOCK_OWNER_IDX;
    TcpIp_SockListenActive[SocketId].ConnStat = TCPIP_SOCK_CONN_NONE;
  }

  /* #40 If the socket was a listen socket, delete potential references to this master socket in all sockets derived from this listen socket. */
  if(tempIsTcpListenSocket == TRUE)
  {
    /* #50 Delete the references to this master socket from all sockets. */
    uint8 TempSockIdx;
    for(TempSockIdx = TCPIP_MAX_NUM_UDP_SOCKETS; TempSockIdx < TCPIP_MAX_NUM_SOCKETS; TempSockIdx++)
    {
      if(TcpIp_SocketList[TempSockIdx].TcpMasterListenSockId == SocketId)
      {
        /* delete reference to the master socket */
        TcpIp_SocketList[TempSockIdx].TcpMasterListenSockId = TCPIP_INV_SOCKET_ID;  /* set to invalid index */
      }
    }
  }

  /* #60 Check, whether to inform the upper layers about the closing of the socket in case of slow closing but non-connected socket. */
  if( (Abort == FALSE) && (TcpIp_SockListenActive[SocketId].ConnStat != TCPIP_SOCK_CONN_CONNECTED) )
  {
    /* delete socket */
    TcpIp_SockUserIdx[SocketId] = TCPIP_UNDEFINED_SOCK_OWNER_IDX;
    TcpIp_SockListenActive[SocketId].ConnStat = TCPIP_SOCK_CONN_NONE;
    Abort = TRUE; /* Set Abort to TRUE in order to trigger the information of the upper layers. */
  }

  /* #70 Actually inform upper layers about the closing of the socket, if required. */
  if(TRUE == Abort)
  {
#if (TCPIP_SUPPORT_TLS == STD_ON)
    if(TcpIp_Tcp_UseTls[TCPIP_SOCKHND2TCPIDX(SocketId)] == TRUE)
    {
      Tls_TcpIpEvent(SocketId, TCPIP_TCP_CLOSED);  /* trigger 'CLOSED' event */
    }
    else
#endif
    {
      TcpIp_TcpIpEventFunctions(tempUserIdx, SocketId, TCPIP_TCP_CLOSED);  /* trigger 'CLOSED' event */
    }
  }
} /* TcpIp_VCloseTcpSocket() */
#endif /* TCPIP_SUPPORT_TCP */

/***********************************************************************************************************************
 *  TcpIp_Bind
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Bind(
    TcpIp_SocketIdType    SocketId,
    TcpIp_LocalAddrIdType LocalAddrId,
    TCPIP_P2V(uint16)     PortPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_COND_VALID_OR_ANY_LOCALADDRID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'PortPtr' */
  else if (PortPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check state of the address that is affected by the bind-request: */
  else if ( (LocalAddrId != TCPIP_LOCALADDRID_ANY) && (TCPIP_IPADDR_STATE_ASSIGNED != TcpIp_IpAddrAssignmentState[LocalAddrId]) )
  {
    /* reject the bind request if no valid address is available for the LocalAddrId */
    errorId = TCPIP_E_ADDRNOTAVAIL;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_LocalAddrIdType ipAddrId;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
#if ( TCPIP_DEV_ERROR_DETECT == STD_OFF )
    retVal = E_OK;
#endif
    /* #20 Set IpAddrId that shall be used for transmission. */
    /* This Id may be updated by (IpV4_Ip|IpV6)_ProvideTxBuffer().
     * E.g. If ipAddrId is an AsBc address the IP will return the id of the auto-selected source address.
     */
    /* #30 In case the socket has Unicast or AutoSelect binding, this means use concrete or any IP on concrete controller for transmission. */
    if (TCPIP_COND_VALID_LOCALADDRID(LocalAddrId))
    {
      ipAddrId = TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalAddrId);
    }
    else
#if ( TCPIP_DEV_ERROR_DETECT == STD_OFF )
    /* #40 In case the socket has ANY binding: Use any IP on any controller for transmission. */
    if (TCPIP_LOCALADDRID_ANY == LocalAddrId)  /* PRQA S 3355, 3358 */ /* MD_TCPIP_13.7_3355_3358 */
#endif
    {
      ipAddrId = TCPIP_IP_ADDR_ID_ANY;
    }
    /* The following check is disabled when DET is STD_OFF because it is DET-checked, above, already! */
#if ( TCPIP_DEV_ERROR_DETECT == STD_OFF )
    /* #50 In case the localAddrId is invalid: Unbind socket / ipAddrId shall not be invalid -> set ipAddrId to ANY. */
    else if(TCPIP_LOCALADDRID_INV == LocalAddrId)  /* PRQA S 3201 */ /* MS_MSR_14.1 */
    {
      ipAddrId = TCPIP_IP_ADDR_ID_ANY;
    }
    else
    {
      retVal = E_NOT_OK;
    }
#endif
    /* #60 In case all parameter checks have passed: Actually conduct the binding. */
#if ( TCPIP_DEV_ERROR_DETECT == STD_OFF )
    if (retVal == E_OK)
#endif
    {
      retVal = TcpIp_BindInternal(SocketId, LocalAddrId, ipAddrId, PortPtr, &errorId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_BIND, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_Bind() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_BindInternal
 **********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BindInternal(
    TcpIp_SocketIdType    SocketId,
    TcpIp_LocalAddrIdType LocalAddrId,
    TcpIp_LocalAddrIdType IpAddrId,
    TCPIP_P2V(uint16)     PortPtr,
    TCPIP_P2V(uint8)      ErrorIdPtr)
{
  /* [SWS_TCPIP_00146]:  TcpIp_Bind() shall check if there is another socket already bound to the same port, protocol
      and local address and if that is the case refuse the request and return E_NOT_OK. If development error detection
      is enabled, the service TcpIp_Bind() shall also raise the development error code TCPIP_E_ADDRINUSE. () \trace SPEC-66326 */
  Std_ReturnType retVal = E_OK;
  uint8 socketIdMin;
  uint8 socketIdMax;
  uint8 localSocketId;
  uint16 portNbo;
  
  *ErrorIdPtr = TCPIP_E_NO_ERROR;
  /* #10 Depending on the protocol of the socketId (UDP or TCP) set iteration borders for the subsequent address check. */
  /* #20 Issue a protocol-specific dynamic port number if desired by the caller. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (SocketId < TcpIp_VCfgGetMaxNumUdpSocks())
#endif
  {
    /* UDP */
    socketIdMin = 0;
    socketIdMax = TcpIp_VCfgGetMaxNumUdpSocks() - 1;

    if (*PortPtr == TCPIP_REQUEST_NEXT_DYNAMIC_PORT)
    {
      *PortPtr = TCPIP_GET_NEXT_DYNAMIC_PORT(TcpIp_Udp_DynamicPortCount);
    }
  }
#if (TCPIP_SUPPORT_TCP == STD_ON)
  else
  {
    /* TCP */
    socketIdMin = TcpIp_VCfgGetMaxNumUdpSocks();
    socketIdMax = TcpIp_VCfgGetMaxNumSocks() - 1;

    if (*PortPtr == TCPIP_REQUEST_NEXT_DYNAMIC_PORT)
    {
      *PortPtr = TCPIP_GET_NEXT_DYNAMIC_PORT(TcpIp_Tcp_DynamicPortCount);
    }
  }
#endif
  /* #30 For all OTHER socketIds of the current protocol (UDP or TCP) assert, that port and local address are not the same. */
  portNbo = IPBASE_HTON16(*PortPtr);
  
  for(localSocketId = socketIdMin; localSocketId <= socketIdMax; localSocketId++)
  {
    /* Check if port and local address are the same as for the new request */
    if (TcpIp_SockListenActive[localSocketId].ConnStat != TCPIP_SOCK_CONN_NONE)
    {
      if(    (localSocketId != SocketId)
          && (portNbo != TCPIP_REQUEST_NEXT_DYNAMIC_PORT)
          && (TcpIp_SocketList[localSocketId].BindLocalAddrId == LocalAddrId)
          && (((TcpIp_SockAddrInetType*)(&TcpIp_SocketList[localSocketId].LocSock))->port == portNbo) /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        )
      {
        /* Error: A socket with the requested protocol, port and IpAddrId has already been requested, before. */
        *ErrorIdPtr = TCPIP_E_ADDRINUSE;
        retVal = E_NOT_OK;
        break;
      }
    }
  }

  /* #40 In case no other socket has the same port and address as the current socket: Do the actual binding. */
  if(retVal == E_OK)
  {
    ((IpBase_SockAddrInType*)(&TcpIp_SocketList[SocketId].LocSock))->sin_port = portNbo;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    TcpIp_SocketList[SocketId].TxIpAddrId      = IpAddrId;
    TcpIp_SocketList[SocketId].BindLocalAddrId = LocalAddrId;

    if (TCPIP_COND_VALID_UDPSOCKIDX(SocketId))
    {
      /* Bound UDP socket is always handled as listening socket. */
      TcpIp_SockListenActive[SocketId].ConnStat  = TCPIP_SOCK_CONN_LISTEN_ENABLED;
    }
  }

  return retVal;
} /* TcpIp_BindInternal() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/***********************************************************************************************************************
 *  TcpIp_RequestIpAddrAssignment
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpAddrAssignment(
    TcpIp_LocalAddrIdType         LocalAddrId,
    TcpIp_IpAddrAssignmentType    Type,
    TCPIP_P2V(TcpIp_SockAddrType) LocalIpAddrPtr,  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    uint8                         Netmask,
    TCPIP_P2V(TcpIp_SockAddrType) DefaultRouterPtr )  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check input parameters. */
  else if (LocalAddrId > TcpIp_VCfgGetMaxLocalAddrId())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if ((Type > TCPIP_IPADDR_ASSIGNMENT_MAX_VALUE) && (Type != TCPIP_IPADDR_ASSIGNMENT_ANY))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Values of 'LocalIpAddrPtr', 'Netmask' and 'DefaultRouterPtr' are checked below. */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 In case the given IP address is not static OR a local address has been provided, forward the request to the related internal service function. */
    if ((Type != TCPIP_IPADDR_ASSIGNMENT_STATIC) || (LocalIpAddrPtr != NULL_PTR))
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
      /* #30 Dispatch the request to the affected IP subcomponent depending on the address type being IpV4 or IpV6. */
      if(TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
# endif
      {
        /* #40 In case the LocalAddrId is an IpV4 address id, pass the provided optional address information localIpAddr and defaultRouterAddr to the IpV4 subcomponent. */
        if (Netmask <= 32)
        {
          uint8 ipAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId);
          IpBase_AddrInType localIpAddr       = IPV4_ADDR_ANY;
          IpBase_AddrInType defaultRouterAddr = IPV4_ADDR_ANY;

          if (LocalIpAddrPtr != NULL_PTR)
          {
            localIpAddr = ((TCPIP_P2C(IpBase_SockAddrInType))LocalIpAddrPtr)->sin_addr;          /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
          }

          if (DefaultRouterPtr != NULL_PTR)
          {
            defaultRouterAddr = ((TCPIP_P2C(IpBase_SockAddrInType))DefaultRouterPtr)->sin_addr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
          }

          retVal = IpV4_Ip_RequestIpAddrAssignment(ipAddrId, Type, localIpAddr, Netmask, defaultRouterAddr);
        }
        else
        {
          /* ERROR: Netmask is invalid. */
          errorId = TCPIP_E_INV_ARG;
        }
      }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
      else
# endif
      {
        if (Netmask <= 128)
        {
          /* #50 In case the LocalAddrId is an IpV6 address id, pass all provided address information to the IpV6 subcomponent. */
          uint8 ipAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId);
          retVal = IpV6_Ip_RequestIpAddrAssignment(ipAddrId, Type, LocalIpAddrPtr, Netmask, DefaultRouterPtr);
        }
        else
        {
          /* ERROR: Netmask is invalid. */
          errorId = TCPIP_E_INV_ARG;
        }
      }
#endif
    }
    else
    {
      /* ERROR: LocalIpAddrPtr must not be NULL_PTR if assignment type is STATIC. */
      errorId = TCPIP_E_INV_ARG;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
}  /* TcpIp_RequestIpAddrAssignment() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_ReleaseIpAddrAssignment
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseIpAddrAssignment(
    TcpIp_LocalAddrIdType LocalAddrId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalAddrId' */
  else if (LocalAddrId > TcpIp_VCfgGetMaxLocalAddrId())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Call TcpIp_ReleaseSpecificIpAddrAssignment with all assignment methods. */
    retVal = TcpIp_ReleaseSpecificIpAddrAssignment(LocalAddrId, TCPIP_IPADDR_ASSIGNMENT_ANY);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_REL_IP_ADDR_ASSIGN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
}  /* TcpIp_ReleaseIpAddrAssignment() */

/***********************************************************************************************************************
 *  TcpIp_ReleaseSpecificIpAddrAssignment
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseSpecificIpAddrAssignment (
    TcpIp_LocalAddrIdType      LocalAddrId,
    TcpIp_IpAddrAssignmentType AssignmentType)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'localAddrId' */
  else if (LocalAddrId > TcpIp_VCfgGetMaxLocalAddrId())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'assignmentType' */
  else if ((AssignmentType > TCPIP_IPADDR_ASSIGNMENT_MAX_VALUE) && (AssignmentType != TCPIP_IPADDR_ASSIGNMENT_ANY))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the related subcomponent, depending on the protocol of the given local address id. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
  if(TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
# endif
    {
      /* LocalAddrId is an IpV4 address id */
      retVal = IpV4_Ip_ReleaseIpAddrAssignment(TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId), AssignmentType);
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* LocalAddrId is an IpV6 address id */
      uint32  PreferredLifetime = 0;
      uint32  ValidLifetime     = 0;
      boolean NoDad             = FALSE;

      retVal = IpV6_SetAddress(TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId), NULL_PTR, PreferredLifetime, ValidLifetime, NoDad);
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_REL_IP_ADDR_ASSIGN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_ReleaseSpecificIpAddrAssignment() */

/***********************************************************************************************************************
 *  TcpIp_GetPhysAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetPhysAddr(
    TcpIp_LocalAddrIdType LocalAddrId,
    TCPIP_P2V(uint8)      PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LocalAddrId' */
  else if (TCPIP_COND_VALID_LOCALADDRID(LocalAddrId) == FALSE)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Depending on the address-type of the given LocalAddrId, forward the request to the IpV4 or IpV6 subcomponent. */
    uint8 IpVxAddrId;
    /* Forward request to IP */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if(TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
# endif
    {
      /* IpV4 */
      IpVxAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId);
      IpV4_Ip_GetPhysAddr((IpV4_AddrIdType)IpVxAddrId, PhysAddrPtr);
      retVal = E_OK;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* IpV6 */
      IpVxAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId);
      retVal = IpV6_GetPhysAddrByIpAddrId((IpV6_AddrIdType)IpVxAddrId, PhysAddrPtr);
    }
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_PHYS_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetPhysAddr() */

/***********************************************************************************************************************
 *  TcpIp_GetRemotePhysAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_GetRemotePhysAddr(
    uint8                         CtrlIdx,
    TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
    TCPIP_P2V(uint8)              PhysAddrPtr,
    boolean                       initRes)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'IpAddrPtr' */
  else if (IpAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the IP subcomponent related to the domain of the given IpAddrPtr. */
    switch(IpAddrPtr->domain)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    case TCPIP_AF_INET:
      {
        /* IpV4 */
        retVal = IpV4_Ip_GetRemotePhysAddr( CtrlIdx, ((TCPIP_P2C(TcpIp_SockAddrInetType))IpAddrPtr)->addr[0], PhysAddrPtr, initRes ); /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        break;
      }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    case TCPIP_AF_INET6:
      {
        /* IpV6 */
        retVal = IpV6_GetRemotePhysAddr( CtrlIdx, (TCPIP_P2C(TcpIp_SockAddrInet6Type))IpAddrPtr, PhysAddrPtr, initRes );   /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        break;
      }
#endif
    default:
      {
        /* error, invalid domain type */
        retVal = TCPIP_E_NOT_OK;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_REMOTE_PHYS_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetRemotePhysAddr() */

/***********************************************************************************************************************
 *  TcpIp_GetArpCacheEntries
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
 /**********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetArpCacheEntries(
    uint8                              CtrlIdx,
    TCPIP_P2V(uint32)                  NumberOfElementsPtr,
    TCPIP_P2V(TcpIp_ArpCacheEntryType) EntryListPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NumberOfElementsPtr' */
  else if (NumberOfElementsPtr == NULL_PTR)
  {
     errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'EntryListPtr' */
  /* Check parameter 'NumberOfElementsPtr' */
  else if ((*NumberOfElementsPtr > 0) && (EntryListPtr == NULL_PTR))
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */  
    /* #20 Forward call to IpV4 submodule. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)   
    retVal = IpV4_Arp_GetCacheEntries(CtrlIdx, NumberOfElementsPtr, EntryListPtr);
#else
    TCPIP_UNUSED_PARAMETER(CtrlIdx);
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_ARP_CACHE_ENTRIES, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); 
#endif
  return retVal;
} /* TcpIp_GetArpCacheEntries() */

/***********************************************************************************************************************
 *  TcpIp_GetCtrlIdx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetCtrlIdx(
    TcpIp_LocalAddrIdType LocalAddrId,
    TCPIP_P2V(uint8)      CtrlIdxPtr)  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'CtrlIdxPtr' */
  else if (CtrlIdxPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LocalAddrId' */
  else if (TCPIP_COND_VALID_LOCALADDRID(LocalAddrId) == FALSE)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Depending on the protocol of the local address id pass the request to the affected submodule IpV4 or IpV6. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if(TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
# endif
    {
      /* IpV4 */
      (*CtrlIdxPtr) = IpV4_CtrlIdxIp2EthIf[IpV4_Ip_AddrId2CtrlAddrMap[TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId)].IpCtrlIdx];
      retVal = E_OK;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* IpV6 */
      (*CtrlIdxPtr) = IpV6_CtrlIdxIp2EthIf[IpV6_AddrId2CtrlAddrMap[TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId)].IpCtrlIdx];
      retVal = E_OK;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_CTRL_IDX, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetCtrlIdx() */

/***********************************************************************************************************************
 *  TcpIp_MainFunctionRx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MainFunctionRx(void)
#else
FUNC(void, TCPIP_CODE) TcpIp_MainFunctionRx(void)
#endif
{
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_OFF)
  /* #10 Check initialization and active state of the component. */
  if ((TcpIp_State == TCPIP_STATE_UNINIT) || (TcpIp_ActiveState == TCPIP_ACTIVE_STATE_OFFLINE))
  {
    /* module is not initialized or offline, so do nothing here but just return */
  }
  else
#endif
  {
    /* IpV4 submodule does not have RX MainFunction. */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    /* #20 Call RX MainFunction of IpV6 submodule. */
    IpV6_MainFunctionRx();
#endif

    /* #30 Call RX MainFunction of UDP submodule. */
    TcpIp_Udp_MainFunctionRx();

#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #40 Call RX MainFunction of TCP submodule. */
    TcpIp_Tcp_MainFunctionRx();
#endif
  }
} /* TcpIp_MainFunctionRx() */

/***********************************************************************************************************************
 *  TcpIp_MainFunctionState
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
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MainFunctionState(void)
#else
FUNC(void, TCPIP_CODE) TcpIp_MainFunctionState(void)
#endif
{
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_OFF)
  /* #10 Check initialization and active state of the component. */
  if ((TcpIp_State == TCPIP_STATE_UNINIT) || (TcpIp_ActiveState == TCPIP_ACTIVE_STATE_OFFLINE))
  {
    /* module is not initialized or offline, so do nothing here but just return */
  }
  else
#endif
  {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    /* #20 Call MainFunction of IpV4 submodule. */
    IpV4_MainFunction();
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    /* #30 Call State MainFunction of IpV6 submodule. */
    IpV6_MainFunctionState();
#endif

#if (TCPIP_DHCPV4SERVER_ENABLED == STD_ON)
    /* #40 Call MainFunction of DHCPv4 Server submodule. */
    TcpIp_DhcpV4Server_MainFunction();
#endif

    /* UDP submodule only has RX and TX MainFunctions. */

#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #50 Call State MainFunction of TCP submodule. */
    TcpIp_Tcp_MainFunctionState();
#endif
  }
} /* TcpIp_MainFunctionState() */

/***********************************************************************************************************************
 *  TcpIp_MainFunctionTx
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MainFunctionTx(void)
#else
FUNC(void, TCPIP_CODE) TcpIp_MainFunctionTx(void)
#endif
{
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_OFF)
  /* #10 Check initialization and active state of the component. */
  if ((TcpIp_State == TCPIP_STATE_UNINIT) || (TcpIp_ActiveState == TCPIP_ACTIVE_STATE_OFFLINE))
  {
    /* module is not initialized or offline, so do nothing here but just return */
  }
  else
#endif
  {
    /* IpV4 submodule does not have a TX MainFunction. */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    /* #20 Call TX MainFunction of IpV6 submodule. */
    IpV6_MainFunctionTx();
#endif

    /* #30 Call TX MainFunction of UDP submodule. */
    TcpIp_Udp_MainFunctionTx();

#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #40 Call TX MainFunction of TCP submodule. */
    TcpIp_Tcp_MainFunctionTx();
#endif
  }
} /* TcpIp_MainFunctionTx() */

#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_MainFunction(void)
{
  /* #10 Check initialization and active state of the component. */
  if ((TcpIp_State == TCPIP_STATE_UNINIT) || (TcpIp_ActiveState == TCPIP_ACTIVE_STATE_OFFLINE))
  {
    /* module is not initialized or offline, so do nothing here but just return */
  }
  else
  {
    /* #20 Call RX MainFunction. */
    TcpIp_MainFunctionRx();

    /* #30 Call State MainFunction. */
    TcpIp_MainFunctionState();

    /* #40 Call TX MainFunction. */
    TcpIp_MainFunctionTx();
  }
} /* TcpIp_MainFunction() */
#endif

/***********************************************************************************************************************
 *  TcpIp_ChangeParameter
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ChangeParameter(
    TcpIp_SocketIdType SocketId,
    TcpIp_ParamIdType  ParameterId,
    TCPIP_P2V(uint8)   ParameterValue)  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* #20 ParameterId and ParameterValue are not checked for DET because they are properly handled by the switch-case-statements, below! */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 SockIdx = SocketId;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

    switch(ParameterId)
    {
    /* #30 Pass all TCP-related parameter change requests to the related function of the TCP submodule in case it is enabled. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
    case TCPIP_PARAMID_TCP_NAGLE:                /* fall through. */
    case TCPIP_PARAMID_V_TCP_UTOENABLED:         /* fall through. */
    case TCPIP_PARAMID_V_TCP_UTOCHANGEABLE:      /* fall through. */
    case TCPIP_PARAMID_TCP_KEEPALIVE:
      {
        uint8 TcpOptParameterValue = *ParameterValue;
        retVal = TcpIp_Tcp_SetSockOpt(TCPIP_SOCKHND2TCPIDX(SockIdx), ParameterId, TcpOptParameterValue);
        break;
      }
    case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX: /* fall through. */
    case TCPIP_PARAMID_V_TCP_UTOUPDATE:
      {
        uint16 TcpOptParameterValue = *((uint16*)(ParameterValue));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_parameter */  /* PRQA S 3305 */ /* MD_TCPIP_3305_param */
        retVal = TcpIp_Tcp_SetSockOpt(TCPIP_SOCKHND2TCPIDX(SockIdx), ParameterId, TcpOptParameterValue);
        break;
      }
    case TCPIP_PARAMID_TCP_RXWND_MAX:            /* fall through. */
    case TCPIP_PARAMID_V_TCP_TXBUFSIZE:          /* fall through. */
    case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:   /* fall through. */
    case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:
      {
        uint32 TcpOptParameterValue = *((uint32*)(ParameterValue));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_parameter */  /* PRQA S 3305 */ /* MD_TCPIP_3305_param */
        /* choose a matching buffer from the pool, and assign it to the socket */
        retVal = TcpIp_Tcp_SetSockOpt(TCPIP_SOCKHND2TCPIDX(SockIdx), ParameterId, TcpOptParameterValue);
        break;
      }
# if (TCPIP_SUPPORT_TLS == STD_ON)
    /* #40 Pass all TLS-related parameter change requests to the related function of the TLS module in case it is enabled. */
    case TCPIP_PARAMID_V_USE_TLS:
      {
        TcpIp_CheckDetErrorReturnValue( TCPIP_COND_VALID_TCPSOCKIDX(SocketId), TCPIP_API_ID_CHANGE_PARAM, TCPIP_E_INV_ARG, E_NOT_OK )

        retVal = Tls_GetSocket(SocketId);  /* get a free connection map element */
        if(retVal == E_OK)
        {
          TcpIp_Tcp_UseTls[TCPIP_SOCKHND2TCPIDX(SockIdx)] = TRUE;
        }
        break;
      }
    case TCPIP_PARAMID_V_TLS_TXBUFSIZE:
    case TCPIP_PARAMID_V_TLS_RXBUFSIZE:
    case TCPIP_PARAMID_V_TLS_SELECT_OCSP_REQEST:
      {
        retVal = Tls_ChangeParameter(SocketId, ParameterId, ParameterValue);  /* set TLS buffer sizes */
        break;
      }
    case TCPIP_PARAMID_V_TLS_HEARTBEAT_MODE:
    case TCPIP_PARAMID_V_TLS_HEARTBEAT_PERIOD_S:
      {
        retVal = Tls_ChangeParameter(SocketId, ParameterId, ParameterValue);  /* forward change request to TLS */
        break;
      }
# endif  /* (TCPIP_SUPPORT_TLS == STD_ON) */
#endif /* (TCPIP_SUPPORT_TCP == STD_ON) */
    /* #50 Pass all UDP-related parameter change requests to the related function of the UDP submodule. */
    case TCPIP_PARAMID_V_UDP_TXREQLISTSIZE:
      {
        /* choose a matching buffer from the pool, and assign it to the socket */
        retVal = TcpIp_Udp_SetSockOpt(SockIdx, ParameterId, ParameterValue);
        break;
      }
#if (TCPIP_UDP_TX_QUEUE_ADDR_MISS_ENABLED == STD_ON)
    case TCPIP_PARAMID_V_UDP_TXRETRYLISTSIZE:
      {
        /* set the maximum number of ethernet TX buffers used for buffering messages in case the remote address is not yet resolved */
        retVal = TcpIp_Udp_SetSockOpt(SockIdx, ParameterId, ParameterValue);
        break;
      }
#endif
    /* #60 Pass all IP-related parameter change requests to the related function of the IP submodule in case it is enabled. */
    case TCPIP_PARAMID_FRAMEPRIO:
      {
        /* uint8 FramePrio value, forward to IpV4 and IpV6 */
        uint8 TempValue = *ParameterValue;
        retVal = E_OK;
#if(TCPIP_SUPPORT_IPV4 == STD_ON)
        if(E_NOT_OK == IpV4_Ip_SetEthIfFramePrio(SocketId, TempValue))
        {
          retVal = E_NOT_OK;
        }
#endif
#if(TCPIP_SUPPORT_IPV6 == STD_ON)
        if(E_NOT_OK == IpV6_SetEthIfFramePrio(SocketId, TempValue))
        {
          retVal = E_NOT_OK;
        }
#endif
        break;
      }
    case TCPIP_PARAMID_TTL:
      {
        /* forward to IpV4 and IpV6 */
        uint8 TempValue = *ParameterValue;
#if(TCPIP_SUPPORT_IPV4 == STD_ON)
        IpV4_Ip_SetTimeToLive(SocketId, TempValue);
#endif
#if(TCPIP_SUPPORT_IPV6 == STD_ON)
        IpV6_SetHopLimit(SocketId, TempValue);
#endif
        retVal = E_OK;
        break;
      }
#if(TCPIP_SUPPORT_IPV4 == STD_ON)
    case TCPIP_PARAMID_V_IPV4_TYPE_OF_SERVICE:
      {
        /* uint8 TypeOfService value, forward to IpV4 */
        uint8 TempValue = *ParameterValue;
        IpV4_Ip_SetTypeOfService(SocketId, TempValue);
        retVal = E_OK;
        break;
      }
#endif
/* else: Leave retVal at value E_NOT_OK */
#if(TCPIP_SUPPORT_IPV6 == STD_ON)
    case TCPIP_PARAMID_V_IPV6_FLOW_LABEL:
      {
        /* uint32 FlowLabel value, forward to IpV6 */
        uint32 TempValue = *((uint32*)(ParameterValue));  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_parameter */  /* PRQA S 3305 */ /* MD_TCPIP_3305_param */
        IpV6_SetFlowLabel(SocketId, TempValue);
        retVal = E_OK;
        break;
      }
    case TCPIP_PARAMID_V_IPV6_TRAFFIC_CLASS:
      {
        /* uint8 Traffic Class value, forward to IpV6 */
        uint8 TempValue = *ParameterValue;
        IpV6_SetTrafficClass(SocketId, TempValue);
        retVal = E_OK;
        break;
      }
#endif
/* else: Leave retVal at value E_NOT_OK */
      case TCPIP_PARAMID_V_PHYS_DEST_ADDR:
      /* This parameter is currently only supported for IpV4. */
#if(TCPIP_SUPPORT_IPV4 == STD_ON)
        IpV4_Ip_SetPhysDestAddr(SocketId, ParameterValue);
        retVal = E_OK;
#endif
/* else: Leave retVal at value E_NOT_OK */
        break;
    default:
        /* Leave retVal at value E_NOT_OK */
        break;
    }
    /* switch() */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_CHANGE_PARAM, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_ChangeParameter() */ /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */

#if (TCPIP_SUPPORT_TLS == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tls_SetClientCertInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_SetClientCertInfo(
    TcpIp_SocketIdType SocketId,
    TCPIP_P2V(uint8)   CertPtr,
    TCPIP_P2V(uint8)   KeyPtr,
    uint16             CertLen,
    uint16             KeyLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the TLS plugin in case the socket is configured for TLS. */
    if(TcpIp_Tcp_UseTls[TCPIP_SOCKHND2TCPIDX(SocketId)] == TRUE)
    {
      retVal = Tls_SetClientCertInfo( SocketId, CertPtr, KeyPtr, CertLen, KeyLen );
    }
    /* else: TLS is not enabled for this socket: Leave retVal at E_NOT_OK */
  }
  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_CLIENT_CERT_INFO, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_Tls_SetClientCertInfo() */

/***********************************************************************************************************************
 *  TcpIp_Tls_GetNvmBlockIdForUsedRootCert
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_GetNvmBlockIdForUsedRootCert(
    TcpIp_SocketIdType         SocketId,
    TCPIP_P2V(NvM_BlockIdType) RootCertBlockIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'RootCertBlockIdPtr' */
  else if (RootCertBlockIdPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the TLS plugin. */
    retVal = Tls_GetNvmBlockIdForUsedRootCert( SocketId, RootCertBlockIdPtr );
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_NVM_BLOCK_FOR_USED_ROOT_CERT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_Tls_GetNvmBlockIdForUsedRootCert() */

/***********************************************************************************************************************
 *  TcpIp_Tls_RootCertWasModified
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_RootCertWasModified(
    NvM_BlockIdType NvmBlockId )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the TLS plugin. */
    retVal = Tls_RootCertWasModified( NvmBlockId );
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_ROOT_CERT_WAS_MODIFIED, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}
#endif  /* (TCPIP_SUPPORT_TLS == STD_ON) */

#if (TCPIP_SUPPORT_IPV4 == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_GetLocNetAddr                                                                                    [DEPRECATED]
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetLocNetAddr(
  TcpIp_LocalAddrIdType        LocalAddrId,
  TCPIP_P2V(TcpIp_NetAddrType) NetAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NetAddrPtr' */
  else if (NetAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check first criterion related to parameter 'LocalAddrId' */
  else if (!TCPIP_COND_VALID_LOCALADDRID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check first criterion related to parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the related function of the IpV4 submodule. */
    retVal = IpV4_Ip_GetLocNetAddr(TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId), NetAddrPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_LOC_NET_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetLocNetAddr() */

/***********************************************************************************************************************
 *  TcpIp_GetLocNetMask                                                                                    [DEPRECATED]
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetLocNetMask(
    TcpIp_LocalAddrIdType        LocalAddrId,
    TCPIP_P2V(TcpIp_NetAddrType) NetMaskPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NetMaskPtr' */
  else if (NetMaskPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check first criterion related to parameter 'LocalAddrId' */
  else if (!TCPIP_COND_VALID_LOCALADDRID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check first criterion related to parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward the request to the related function of the IpV4 submodule. */
    retVal = IpV4_Ip_GetLocNetMask(TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId), NetMaskPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_LOC_NET_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}  /* TcpIp_GetLocNetMask() */
#endif

/***********************************************************************************************************************
 *  TcpIp_GetIpAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIpAddr(
  TcpIp_LocalAddrIdType         LocalAddrId,
  TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,
  TCPIP_P2V(uint8)              NetmaskPtr,
  TCPIP_P2V(TcpIp_SockAddrType) DefaultRouterPtr )  /* PRQA S 3673 */ /* MD_TCPIP_16.7_3673 */  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else
#endif
  /* Check parameter 'LocalAddrId' */
  if (!TCPIP_COND_VALID_LOCALADDRID(LocalAddrId))
  {
    /* invalid or any address id: Leave retVal at value E_NOT_OK */
    errorId = TCPIP_E_INV_ARG;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check address family. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if(TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))
#endif
    {
      /* IPBASE_AF_INET */
      IpV4_AddrIdType ipAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId);

      /* #30 Write IpV4 address if requested. */
      if (IpAddrPtr != NULL_PTR)
      {
        TCPIP_P2V(IpBase_SockAddrInType) IpAddrInPtr = (TCPIP_P2V(IpBase_SockAddrInType))IpAddrPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        IpAddrInPtr->sin_family = IPBASE_AF_INET;
        retVal = IpV4_Ip_GetLocNetAddr(ipAddrId, &IpAddrInPtr->sin_addr);  /* PRQA S 3689 */ /* MD_TCPIP_21.1_3689_arrayusage */
      }
      else
      {
        retVal = E_OK;
      }

      /* #40 Write IpV4 netmask if requested. */
      if ((retVal == E_OK) && (NetmaskPtr != NULL_PTR))
      {
        IpBase_AddrInType netmask;
        retVal = IpV4_Ip_GetLocNetMask(ipAddrId, &netmask);

        if (retVal == E_OK)
        {
          /* convert network mask to CIDR format */
          uint8 shiftCnt = 0u;
          uint32 netmaskShifted = IPBASE_HTON32(netmask);
          while((shiftCnt < 32u) && ((netmaskShifted & 0x01) == 0u))
          {
            netmaskShifted = netmaskShifted >> 1;
            shiftCnt++;
          }
          *NetmaskPtr = (uint8)(32u - shiftCnt);
        }
      }

      /* #50 Write IpV4 default router address if requested. */
      if ((retVal == E_OK) && (DefaultRouterPtr != NULL_PTR))
      {
        TCPIP_P2V(IpBase_SockAddrInType) sockAddrAddrInPtr = (TCPIP_P2V(IpBase_SockAddrInType))DefaultRouterPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        sockAddrAddrInPtr->sin_family = IPBASE_AF_INET;
        /* Ignore return value of IpV4_Ip_GetDefaultRouterNetAddr() because TcpIp_GetIpAddr() should not return an error if there is no default router. */
        IpV4_Ip_GetDefaultRouterNetAddr(ipAddrId, &sockAddrAddrInPtr->sin_addr);
      }
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
#endif
    {
      /* IPBASE_AF_INET6 */
      /* #60 Write IpV6 address if requested. */
      if (IpAddrPtr != NULL_PTR)
      {
        TCPIP_P2V(IpBase_SockAddrIn6Type) sockAddrAddrIn6Ptr = (TCPIP_P2V(IpBase_SockAddrIn6Type))IpAddrPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        sockAddrAddrIn6Ptr->sin6_family = IPBASE_AF_INET6;
        retVal = IpV6_GetLocalAddress(TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId), &sockAddrAddrIn6Ptr->sin6_addr, NetmaskPtr);  /* PRQA S 291 */ /* MD_TCPIP_21.1_3689_arrayusage */
      }
      else
      {
        retVal = E_OK;
      }

      /* #70 Write IpV6 default router address if requested. */
      if ((retVal == E_OK) && (DefaultRouterPtr != NULL_PTR))
      {
        boolean routerIsReachable;
        TCPIP_P2V(IpBase_SockAddrIn6Type) sockAddrAddrIn6Ptr = (TCPIP_P2V(IpBase_SockAddrIn6Type))DefaultRouterPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
        sockAddrAddrIn6Ptr->sin6_family = IPBASE_AF_INET6;
        /* Ignore return value of IpV6_GetDefaultRouterAddress() because TcpIp_GetIpAddr() should not return an error if there is no default router. */
        (void)IpV6_GetDefaultRouterAddress(TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId), &sockAddrAddrIn6Ptr->sin6_addr, &routerIsReachable);  /* PRQA S 3382 */ /* MD_TCPIP_21.1_3382 */
      }
      /* else: No further action requested: Leave retVal on its current value */
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_IP_ADDR, errorId);
  }
#else
  TCPIP_UNUSED_PARAMETER(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetIpAddr() */  /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_GetRemNetAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetRemNetAddr(
  TcpIp_SocketIdType             SocketId,
  TCPIP_P2V(TcpIp_SockAddrType) *SockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check Parameter 'SocketId'. */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  /* Check parameter 'SockAddrPtr' */
  else if (SockAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #20 In case it is a TCP socket AND the socket is connected: Store the remote address. */
    if (TCPIP_SOCKET_ID_IS_TCP(SocketId))
    {
      if (TcpIp_Tcp_GetSockIsConnected(TCPIP_SOCKET_ID_TO_TCP_IDX(SocketId)) == TRUE)
      {
        *SockAddrPtr = (TcpIp_SockAddrType*)&TcpIp_SocketList[SocketId].RemSock;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        retVal = E_OK;
      }
      /* else: TCP socket is not connected: Leave retVal at value E_NOT_OK. */
    }
    /* else: No remote socket address is stored for UDP sockets: Leave retVal at value E_NOT_OK. */
#else
    TCPIP_UNUSED_PARAMETER(SocketId);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
    TCPIP_UNUSED_PARAMETER(SockAddrPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

    /* #30 Reject the request in case only UDP is enabled because in this case no remote addresses are stored. */
    retVal = E_NOT_OK;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_REM_NET_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetRemNetAddr() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (TCPIP_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_GetVersionInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_GetVersionInfo(
    TCPIP_P2V(Std_VersionInfoType) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'versioninfo' */
  else if (versioninfo == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Write the version info into the struct pointed to by the given pointer. */
    versioninfo->vendorID         = TCPIP_VENDOR_ID;
    versioninfo->moduleID         = TCPIP_MODULE_ID;
    versioninfo->sw_major_version = TCPIP_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = TCPIP_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = TCPIP_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_VERSION_INFO, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* TcpIp_GetVersionInfo() */ /* PRQA S 6080 */ /* MD_TCP_STMIF */
#endif
  /* TCPIP_VERSION_INFO_API */

#if TCPIP_SUPPORT_DHCPV4 == STD_ON
# if ((TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV4_TX_OPTION_CNT > 0))  /* PRQA S 3332 */ /* MD_TCPIP_19.11_3332 */
/***********************************************************************************************************************
 *  TcpIp_DhcpReadOption
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpReadOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint8                 Option,
    TCPIP_P2V(uint8)      DataLength,
    TCPIP_P2V(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IS_AF_INET(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 MsgFlags;
    /* Socket has Unicast or AutoSelect binding. This means use concrete or any IP on concrete controller for transmission */
    uint8 IpAddrId = TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalIpAddrId);
    /* #20 Forward the request to the DHCPv4 subcomponent. */
    retVal = TcpIp_DhcpV4_VGetTxOption(IpAddrId, Option, DataLength, DataPtr, &MsgFlags);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_READ_OPT, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* TcpIp_DhcpReadOption() */ /* PRQA S 6080 */ /* MD_TCP_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpWriteOption
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpWriteOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint8                  Option,
    uint8                  DataLength,
    TCPIP_P2C(uint8)       DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IS_AF_INET(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Socket has Unicast or AutoSelect binding. This means use concrete or any IP on concrete controller for transmission */
    uint8 IpAddrId = TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalIpAddrId);
    /* #20 Forward the request to the DHCPv4 subcomponent. */
    retVal = TcpIp_DhcpV4_VSetTxOption(IpAddrId, Option, DataLength, DataPtr, TCPIP_DHCPV4_MSG_FLAG_ALLWAYS);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_WRITE_OPT, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* TcpIp_DhcpWriteOption() */
# endif
#endif
  /* TCPIP_SUPPORT_DHCPV4 == STD_ON */

#if (TCPIP_SUPPORT_DHCPV6 == STD_ON)
# if ((TCPIP_DHCPV6_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV6_TX_OPTION_CNT > 0))
/***********************************************************************************************************************
 *  TcpIp_DhcpV6ReadOption
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6ReadOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint16                Option,
    TCPIP_P2V(uint16)     DataLength,
    TCPIP_P2V(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IS_AF_INET6(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 MsgFlags;
    /* Socket has Unicast or AutoSelect binding. This means use concrete or any IP on concrete controller for transmission */
    uint8 IpAddrId = TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalIpAddrId);
    /* #20 Forward the request to the DHCPv6 subcomponent. */
    retVal = TcpIp_DhcpV6_VGetTxOption(IpAddrId, Option, DataLength, DataPtr, &MsgFlags);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_V6_READ_OPT, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* TcpIp_DhcpV6ReadOption() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6WriteOption
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6WriteOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint16                Option,
    uint16                DataLength,
    TCPIP_P2C(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IS_AF_INET6(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Socket has Unicast or AutoSelect binding. This means use concrete or any IP on concrete controller for transmission */
    uint8 IpAddrId = TCPIP_LOCAL_ADDR_ID_2_IPVX_ID(LocalIpAddrId);
    /* #20 Forward the request to the DHCPv6 subcomponent. */
    retVal = TcpIp_DhcpV6_VSetTxOption(IpAddrId, Option, DataLength, DataPtr, TCPIP_DHCPV6_MSG_FLAG_ALLWAYS);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_WRITE_OPT, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* TcpIp_DhcpV6WriteOption() */
# endif
#endif
  /* TCPIP_SUPPORT_DHCPV6 == STD_ON */

/***********************************************************************************************************************
 *  TcpIp_GetDhcpTimeoutInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_GetDhcpTimeoutInfo(
    uint8 CtrlIdx,  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
    uint8 IpVer )   /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  boolean retVal = FALSE;
#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
  /* #10 Check the given IP version to be IpV4. */
  if(IpVer == TCPIP_IP_PROT_VER_4)
  {
    /* #20 Call the internal service function of the DHCPv4 submodule in order to retrieve the timeout status. */
    /* The following function returns the timeout status, directly, i.e.: FALSE: No DHCP timeout occurred, TRUE: A DHCP timeout occurred. */
    retVal = TcpIp_DhcpV4_GetDhcpTimeoutInfo(CtrlIdx);
  }
  /* else: This case is currently not supported: Leave retVal at value FALSE */
#else
  /* DHCP is not activated, so no timeout can occur: Leave retVal at value FALSE. */
  TCPIP_UNUSED_PARAMETER(IpVer);    /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TCPIP_UNUSED_PARAMETER(CtrlIdx);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  return retVal;
  /* TCPIP_SUPPORT_DHCPV4 == STD_ON */
} /* TcpIp_GetDhcpTimeoutInfo() */

/***********************************************************************************************************************
 *  TcpIp_GetIpAddrCfgSrc
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIpAddrCfgSrc(
    TcpIp_LocalAddrIdType  LocalAddrId,
    TCPIP_P2V(uint8)       CfgSrcPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_COND_VALID_LOCALADDRID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'CfgSrcPtr' */
  else if (CfgSrcPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check address family. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if (TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId))  /* PRQA S 3689 */ /* MD_TCPIP_21.1_3689_arrayusage */
#endif
    {
      uint8 ipAddrAssignmentMethod = IpV4_Ip_GetActiveAddrAssignmentMethod(TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId));  /* PRQA S 3689 */ /* MD_TCPIP_21.1_3689_arrayusage */

      /* #30 Map IpV4 address configuration source to TcpIp configuration source. */
      switch (ipAddrAssignmentMethod)
      {
      case IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC:
        (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_FIX;
        break;

      case IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP:
        (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_DHCP;
        break;

      case IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL:
        (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_LL;
        break;

      default:
        (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_NONE;
        break;
      }

      retVal = E_OK;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
#endif
    {
      uint8 IpV6CfgSrc;
      retVal = IpV6_GetLocalAddressCfgSrc(TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId), &IpV6CfgSrc);  /* PRQA S 3689 */ /* MD_TCPIP_21.1_3689_arrayusage */

      if (retVal == E_OK)
      {
        /* #40 Map IpV6 address configuration source to TcpIp configuration source. */
        switch (IpV6CfgSrc)
        {
        case IPV6_ADDR_CFG_SRC_LL:
          (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_LL;
          break;

        case IPV6_ADDR_CFG_SRC_STATIC:
          (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_FIX;
          break;

        case IPV6_ADDR_CFG_SRC_RA:
          (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_RA;
          break;

        case IPV6_ADDR_CFG_SRC_DHCP:
          (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_DHCP;
          break;

#if defined(IPV6_ADDR_CFG_SRC_PRIV_EXT)
        case IPV6_ADDR_CFG_SRC_PRIV_EXT:
          (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_PRIV_EXT;
          break;
#endif

        default:
          (*CfgSrcPtr) = TCPIP_IP_CFG_SRC_NONE;
          break;
        }
      }
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_IP_ADDR_CFG_SRC, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetIpAddrCfgSrc() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_RequestComMode
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestComMode(
    uint8            CtrlIdx,  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
    TcpIp_StateType  State)  /* PRQA S 3206 */ /* MD_TCPIP_3206 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check of parameter 'CtrlIdx' is done by the Rx-Indication functions of the sub-modules! */
  /* Check parameter 'State' */
  else if (State > TCPIP_ACTIVE_STATE_ONLINE)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Currently this function has no functionality, thus simply return E_OK. */
    TCPIP_UNUSED_PARAMETER(CtrlIdx);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_REQUEST_COM_MODE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_RequestComMode() */ /* PRQA S 6080 */ /* MD_TCP_STMIF */

/***********************************************************************************************************************
 *  TcpIp_GetLocSockAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetLocSockAddr(
    TcpIp_SocketIdType            SocketId,
    TCPIP_P2V(TcpIp_SockAddrType) SockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  /* Check parameter 'SockPtr' */
  else if (SockPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 In case all parameters are valid, copy the desired ones. */
    IpBase_SockAddrType *LocSockPtr = (IpBase_SockAddrType*)(&TcpIp_SocketList[SocketId].LocSock);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    retVal = IpBase_CopySockAddr((IpBase_SockAddrType*)SockPtr, LocSockPtr);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_LOCAL_SOCK_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetLocSockAddr() */

#if (STD_ON == TCPIP_DIAG_ACCESS_EXTENSIONS_SUPPORT)
/**********************************************************************************************************************
 *  TcpIp_DiagDataReadAccess
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DiagDataReadAccess(
    TcpIp_SocketIdType   SocketId,
    TcpIp_DiagParamsType DataID,
    TCPIP_P2V(uint8)     DataPtr,
    TCPIP_P2V(uint16)    DataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'DataID' */
  else if (DataID >= TCPIP_DP_INVALID)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DataLenPtr' */
  else if (DataLenPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  /* Check parameter 'SockHnd' */
  if (!TCPIP_COND_VALID_SOCKET_ID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 In case all parameters are valid, call the internal service function that conducts the desired action. */
    retVal = TcpIp_DiagDataReadAccessInternal(SocketId, DataID, DataPtr, DataLenPtr);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_DIAG_DATA_READ_ACCESS, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* TcpIp_DiagDataReadAccess() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_TCP_STMIF */

/**********************************************************************************************************************
 *  TcpIp_DiagDataReadAccessInternal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DiagDataReadAccessInternal(
    TcpIp_SocketIdType   SocketId,
    TcpIp_DiagParamsType DataID,
    TCPIP_P2V(uint8)     DataPtr,
    TCPIP_P2V(uint16)    DataLenPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  /* #10 Evaluate the DataID the caller desires to read. */
  switch(DataID)
  {
  case TCPIP_DP_SOCKET_LOCAL_PORT:
    /* ----------------------------------------------------------------------------------------- */
    /* #20 Read the current local port number for a given socket. */
    if(*DataLenPtr >= 2)
    {
      *((IpBase_PortType*)DataPtr) = IPBASE_HTON16(((IpBase_SockAddrInType*)(&TcpIp_SocketList[SocketId].LocSock))->sin_port);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_parameter */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */  /* PRQA S 3305 */ /* MD_TCPIP_3305 */
      *DataLenPtr = 2;
      retVal = E_OK;
    }
    break;
# if ((TCPIP_SUPPORT_TCP == STD_ON) && (TCPIP_TCP_DIAG_READ_ACK_SEQ_NUM_ENABLED == STD_ON))
  case TCPIP_DP_SOCKET_TCP_CURRENT_ACK_NUM:
    /* ----------------------------------------------------------------------------------------- */
    /* #30 Read the TCP acknowledgment number that would be used for the next segment for sending */
    if((*DataLenPtr >= 4) && (SocketId >= TcpIp_VCfgGetMaxNumUdpSocks()))  /* PRQA S 3355 */ /* MD_TCPIP_13.7_3355_3358 */
    {
      TcpIp_TcpIdxType TcpId = (TcpIp_TcpIdxType)((TcpIp_TcpIdxType)SocketId - (TcpIp_TcpIdxType)TcpIp_VCfgGetMaxNumUdpSocks());
      *((uint32*)DataPtr) = TcpIp_Tcp_RcvNxt[TcpId];  /* direct access to a TCP variable */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_parameter */  /* PRQA S 3305 */ /* MD_TCPIP_3305_param */
      retVal = E_OK;
    }
    break;
  case TCPIP_DP_SOCKET_TCP_NEXT_SEQ_NUM:
    /* ----------------------------------------------------------------------------------------- */
    /* #40 Read the TCP sequence number that would be used for the next data segment for sending. */
    if((*DataLenPtr >= 4) && (SocketId >= TcpIp_VCfgGetMaxNumUdpSocks()))  /* PRQA S 3355, 3358 */ /* MD_TCPIP_13.7_3355_3358 */
    {
      TcpIp_TcpIdxType TcpId = (TcpIp_TcpIdxType)((TcpIp_TcpIdxType)SocketId - (TcpIp_TcpIdxType)TcpIp_VCfgGetMaxNumUdpSocks());
      *((uint32*)DataPtr) = TcpIp_Tcp_TxNextSendSeqNo[TcpId];  /* direct access to a TCP variable */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_parameter */  /* PRQA S 3305 */ /* MD_TCPIP_3305_struct */
      retVal = E_OK;
    }
    break;
# endif  /* ((TCPIP_SUPPORT_TCP == STD_ON) && (TCPIP_TCP_DIAG_READ_ACK_SEQ_NUM_ENABLED == STD_ON)) */
  default:  /* PRQA S 2018 */ /* MS_MSR_14.1 */
    /* do nothing */
    break;
  }
  return retVal;
}
#endif  /* (STD_ON == TCPIP_SUPPORT_DIAG_ACCESS_EXTENSIONS) */

/***********************************************************************************************************************
 *  TcpIp_ClearARCache
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ClearARCache(
    TcpIp_LocalAddrIdType LocalAddrId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (LocalAddrId > TcpIp_VCfgGetMaxLocalAddrId())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Trigger cache clearing */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
    if(TCPIP_LOCAL_ADDR_IS_AF_INET(LocalAddrId)) /* PRQA S 3355, 3358 */ /* MD_TCPIP_13.7_3355_3358 */
    {
      /* IpV4 */
      IpV4_AddrIdType IpAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV4_ID(LocalAddrId); /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
      IpV4_Arp_ClearCache(IpAddrId);
      retVal = E_OK;
    }
    else
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    if(TCPIP_LOCAL_ADDR_IS_AF_INET6(LocalAddrId)) /* PRQA S 3355, 3358 */ /* MD_TCPIP_13.7_3355_3358 */
    {
      /* IpV6 */
      IpV6_AddrIdType IpAddrId = TCPIP_LOCAL_ADDR_ID_2_IPV6_ID(LocalAddrId); /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
      retVal = IpV6_Ndp_ClearCache(IpAddrId);
    }
    else
#endif
    { /* PRQA S 3201 */ /* MS_MSR_14.1 */
      /* nothing to do (required by MISRA) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_CLEAR_AR_CACHE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_ClearARCache() */

/***********************************************************************************************************************
 *  TcpIp_GetNdpCacheEntries
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetNdpCacheEntries(
    uint8                              CtrlIdx,
    TCPIP_P2V(uint32)                  NumberOfElementsPtr,
    TCPIP_P2V(TcpIp_NdpCacheEntryType) EntryListPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  /* Check parameter 'TcpIp_State' */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NumberOfElementsPtr' */
  else if (NumberOfElementsPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'NumberOfElementsPtr' */
  /* Check parameter 'EntryListPtr' */
  else if ((*NumberOfElementsPtr > 0) && (EntryListPtr == NULL_PTR))
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Return cache entries if IpV6 is supported. */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    retVal = IpV6_Ndp_GetCacheEntries(CtrlIdx, NumberOfElementsPtr, EntryListPtr);
#else
    TCPIP_UNUSED_PARAMETER(CtrlIdx);
#endif /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_NDP_CACHE_ENTRIES, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* TcpIp_GetNdpCacheEntries() */

/***********************************************************************************************************************
 *  INTERNAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_VPreconfigSocket
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VPreconfigSocket(TcpIp_SockIdxType SockIdx)
{
  /* #10 Check state of the module to be 'online'. */
  if(TCPIP_ACTIVE_STATE_ONLINE == TcpIp_ActiveState)
  {
    /* #20 Initialize the given socket's admin data for both IP protocols. */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
    (void)IpBase_DelSockAddr( ((IpBase_SockAddrType *)(&TcpIp_SocketList[SockIdx].RemSock)), IPBASE_AF_INET6 );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    (void)IpBase_DelSockAddr( ((IpBase_SockAddrType *)(&TcpIp_SocketList[SockIdx].LocSock)), IPBASE_AF_INET6 );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#else
    (void)IpBase_DelSockAddr( ((IpBase_SockAddrType *)(&TcpIp_SocketList[SockIdx].RemSock)), IPBASE_AF_INET );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    (void)IpBase_DelSockAddr( ((IpBase_SockAddrType *)(&TcpIp_SocketList[SockIdx].LocSock)), IPBASE_AF_INET );  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
#endif
    /* #30 Initialize the given socket's user and connection status. */
    TcpIp_SockUserIdx[SockIdx]               = TCPIP_UNDEFINED_SOCK_OWNER_IDX;
    TcpIp_SockListenActive[SockIdx].ConnStat = TCPIP_SOCK_CONN_NONE;
  }
  /* else: Do not prepare socket if OFFLINE or IN_SHUTDOWN */
} /* TcpIp_VPreconfigSocket() */

/***********************************************************************************************************************
 *  TcpIp_VSocketAccessExclusive()
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(boolean, TCPIP_CODE) TcpIp_VSocketAccessExclusive(TcpIp_SocketIdType socketId)
{
  boolean retVal;
  /* #10 Enter exclusive area. */
  /* check if buffer is already requested */
  TCPIP_VENTER_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */  /* lock interrupts */
  
  /* #20 Check lock variable of the socket. */
  if (TcpIp_TxBufRequested[socketId] == TRUE)
  {
    /* buffer is already locked by a previous call of ProvideTxBuffer() */
    retVal = FALSE;
  }
  else
  {
    TcpIp_TxBufRequested[socketId] = TRUE;
    retVal = TRUE;
  }
  /* #30 Exit exclusive area. */
  TCPIP_VLEAVE_CRITICAL_SECTION();  /* PRQA S 3109 */ /* MD_MSR_14.3 */  /* unlock interrupts */
  
  return retVal;
} /* TcpIp_VSocketAccessExclusive() */

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*
   MICROSAR global MISRA deviations:
   - see file 'WI_MISRA_Compliance_MSR.pdf'

   module specific MISRA deviations:
   MD_TCPIP_14.5_0770: A continue statement has been used.
     Reason:     Avoiding continue statement here would decrease the code readability.
     Risk:       None.
     Prevention: Covered by code review.
   MD_TCPIP_1.1_0857:
     Reason:     Number of macro definitions is huge but improves readability. (rule 0857)
     Risk:       Some compilers might have a problem with too many macro definitions.
     Prevention: Use only compilers supporting more than 1024 macro definitions.
   MD_MSR_5.1_777_779:
     Reason:     A generated set of buffers uses the same name for all buffers and just appends a numbering suffix
     Risk:       None, since these buffers are not used manually but only used as elements in a generated array of
                 buffer pointers.
     Prevention: None.
   MD_TCPIP_5.6_0781:
     Reason:     Readability is increased by using well known names.
     Risk:       Elements can be confused with each other.
     Prevention: Covered by code review.
   MD_TCPIP_6.3_5013:
     Reason:     Usage of basic types is defined in SWS_SoAd
     Risk:       problem with different platforms
     Prevention: not possible
   MD_TCPIP_8.12_3684:
     Reason:     The arrays in file TcpIp_PBcfg.h and TcpIp_Lcfg.h are declared as extern without size specification
                 since their size depend on the configuration (e.g. in GENy).
     Risk:       There is no risk because these arrays are only used by TcpIp.
     Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_sock:
     Reason:     IpBase_SockAddrType includes address family to differ between IpV4 and IpV6. The corresponding
                 structs also start with the family field so casting is possible.
     Risk:       Only casting from IpBase_SockAddrInType or IpBase_SockAddrIn6Type to IpBase_SockAddrType is allowed.
     Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_copy:
     Reason:     The copy function uses a different base type as the local element.
     Risk:       The copy function might cause access problems on some platforms if elements are not aligned properly.
     Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_compare:
     Reason:     The compare function uses a different base type as the local element.
     Risk:       The compare function might cause access problems on some platforms if elements are not aligned properly.
     Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_struct:
     Reason:     Struct-mapping on a byte array element (e.g. a stream)
     Risk:       This might cause access problems on some platforms if elements are not aligned properly.
     Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_parameter:
     Reason:     An API uses a uint8 array to forward data to the function, and depending on another parameter (e.g. ParameterId) the data is interpreted in different ways.
     Risk:       This might cause access problems on some platforms if the data pointer is not set properly.
     Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_x:
     Reason:     others
     Risk:       not analyzed
     Prevention: Covered by code review.
   MD_TCPIP_11.5_0311_ASR:
     Reason:     AUTOSAR does not specify a const qualifier, so the const attribute has to be removed by a type cast
     Risk:       A user might try to access and change the struct, but this would not be detected during compile time
     Prevention: Covered by code review.
   MD_TCPIP_12.4_3415:
     Reason:     A concatenation of conditions may break without checking the later conditions if a previous one
                 already failed.
     Risk:       The concatenation of conditions might have a different effect as expected.
     Prevention: Covered by code review.
   MD_TCPIP_13.7_3355_3358:
     Reason:     This condition is always true only in case Det is enabled. To protect code against stack corruption
                 in case Det is disabled this if condition is added.
     Risk:       No risk.
     Prevention: Covered by code review.
   MD_TCPIP_13.7_3356_3359:
     Reason:     This condition is always false only in case Det is enabled. To protect code against stack corruption
                 in case Det is disabled this if condition is added.
     Risk:       No risk.
     Prevention: Covered by code review.
   MD_TCPIP_18.4_0759:
     Reason:     Unions shall not be used. Here a union is used to enable efficient access to the address data.
     Risk:       using unions has to be done very carefully since data might be misinterpreted.
     Prevention: Covered by code review.
   MD_TCPIP_19.11_3332:
     Reason:     A concurrent chain of preprocessor-checks connected with 'AND' can be stopped when a check fails, the
                 following checks can be omitted
     Risk:       Checks could be combined in a different way as expected
     Prevention: Covered by code review.
   MD_TCPIP_19.15_0883:
     Reason:     QA-C MISRA checker does not recognize the protection against multiple header include correctly.
     Risk:       No Risk.
     Prevention: Covered by code review.
   MD_TCPIP_21.1_3382:
     Reason:     The socket index of a TCP socket has always a higher or equal value than the number of UDP socket,
                 since the consecutive null-based list of sockets starts with UDP sockets followed by TCP sockets
     Risk:       The macro TCPIP_SOCKIDX2TCPIDX would cause an index wrap-around if it is used for UDP sockets, but the
                 macro is only meant to be used for TCP sockets (as the macro's name implies)
     Prevention: Covered by code review.
   MD_TCPIP_3199:
     Reason:     Dummy statement to avoid compiler warnings.
     Risk:       There is no risk as such statements have no effect on the code.
     Prevention: Covered by code review.
   MD_TCPIP_3205:
      Reason:     The defined type is only used in some configurations.
      Risk:       There is no risk.
      Prevention: Covered by code review.
   MD_TCPIP_3206:
      Reason:     API parameter is only used in some configurations.
      Risk:       There is no risk.
      Prevention: Covered by code review.
   MD_TCPIP_3305:
     Reason:     The different socket address types do all base on the same structure, the alignment should always be
                 the same so that mapping from one type to an other one should not be a problem.
     Risk:       Only casting between different variants of the socket type is allowed.
     Prevention: Covered by code review.
   MD_TCPIP_3305_param:
     Reason:     The API transfers parameters of different types but points to them using an uint8 pointer and a
                 parameter ID. The parameter type is specified for each parameter ID, so no false alignments should
                 occur.
     Risk:       Parameters could be casted to the wrong type accidently.
     Prevention: Covered by code review.
   MD_TCPIP_3305_struct:
     Reason:     TcpIp protocol headers are mapped to data arrays. The arrays should usually be aligned correctly.
     Risk:       Arrays could be misaligned accidently.
     Prevention: Covered by code review.
  MD_TCPIP_3198:
      Reason:     The parameter is used after this manipulation only in some configurations.
      Risk:       There is no risk.
      Prevention: Covered by code review.

   TCPIP-specific deviations:
   MD_TCPIP_16.7_3673:
     Reason:     Pointer could be const, but API has to be AUTOSAR-conform
     Risk:       Function could try to change the element the pointer points to, but this should not be allowed.
     Prevention: Covered by code review.
   MD_TCPIP_21.1_3689_arrayusage:
     Reason:     The array access cannot be out of bounds because only the defined enumerations for the address family are used
     Risk:       n/a
     Prevention: reviews and tests ensure the valid usage
*/

/**********************************************************************************************************************
 *  END OF FILE: TcpIp.c
 *********************************************************************************************************************/
