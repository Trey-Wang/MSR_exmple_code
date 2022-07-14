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
/**        \file  SoAd_Cbk.h
 *        \brief  Socket Adaptor callback header file
 *
 *      \details  Vector static callback header file for AUTOSAR Socket Adaptor module. This header file contains
 *                the callback declarations of the module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/


#if !defined(SOAD_CBK_H)
# define SOAD_CBK_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd_Types.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SOAD_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  SoAd_RxIndication()
 *********************************************************************************************************************/
/*! \brief        Receives data from sockets.
 *  \details      -
 *  \param[in]    SocketId        Socket identifier.
 *  \param[in]    RemoteAddrPtr   Pointer to remote address.
 *  \param[in]    BufPtr          Pointer to buffer of received data.
 *  \param[in]    Length          Length of received data.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RxIndication(
  SoAd_SocketIdType SocketId,
  SoAd_SockAddrType* RemoteAddrPtr,
  uint8* BufPtr,
  uint16 Length);

/**********************************************************************************************************************
 *  SoAd_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer.
 *  \details        -
 *  \param[in]      SocketId                Socket identifier.
 *  \param[in]      BufPtr                  Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLength|BufLengthPtr  Pointer to length|Length of provided transmission buffer.
 *  \return         BUFREQ_OK               Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK         Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData(
  SoAd_SocketIdType SocketId,
  uint8* BufPtr,
# if ( SOAD_VTX_DYN_LEN_ENABLED == STD_ON )
  uint16* BufLengthPtr
# else
  uint16 BufLength
# endif /* SOAD_VTX_DYN_LEN_ENABLED == STD_ON */
         );

/**********************************************************************************************************************
 *  SoAd_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Confirms transmission of data.
 *  \details        -
 *  \param[in]      SocketId                Socket identifier.
 *  \param[in]      Length                  Length of confirmed data.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxConfirmation(
  SoAd_SocketIdType SocketId,
  uint16 Length);

/**********************************************************************************************************************
 *  SoAd_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*! \brief          Receives local IP address assignment state changes.
 *  \details        -
 *  \param[in]      IpAddrId    IP address identifier.
 *  \param[in]      State       State of IP address assignment.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different IP address identifier, FALSE for same IP address identifier.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_LocalIpAddrAssignmentChg(
  SoAd_LocalAddrIdType IpAddrId,
  SoAd_IpAddrStateType State);

/**********************************************************************************************************************
 *  SoAd_TcpAccepted()
 *********************************************************************************************************************/
/*! \brief        Accepts TCP connections on a listen socket.
 *  \details      -
 *  \param[in]    SocketId            Listen socket identifier.
 *  \param[in]    SocketIdConnected   Connected socket identifier.
 *  \param[in]    RemoteAddrPtr       Pointer to remote addres.
 *  \return       E_OK                Connection was accepted.
 *  \return       E_NOT_OK            Connection was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier..
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpAccepted(
  SoAd_SocketIdType SocketId,
  SoAd_SocketIdType SocketIdConnected,
  SoAd_SockAddrType* RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_TcpConnected()
 *********************************************************************************************************************/
/*! \brief        Handles TCP connections which have been initiated locally and are now successfully connected.
 *  \details      -
 *  \param[in]    SocketId    Socket identifier.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TcpConnected(
  SoAd_SocketIdType SocketId);

/**********************************************************************************************************************
 *  SoAd_TcpIpEvent()
 *********************************************************************************************************************/
/*! \brief        Handles events on sockets.
 *  \details      -
 *  \param[in]    SocketId    Socket identifier.
 *  \param[in]    Event       Event type. [TCPIP_TCP_RESET, TCPIP_TCP_CLOSED, TCPIP_TCP_FIN_RECEIVED, TCPIP_UDP_CLOSED]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TcpIpEvent(
  SoAd_SocketIdType SocketId,
  SoAd_EventType Event);

# define SOAD_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* !defined(SOAD_CBK_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Cbk.h
 *********************************************************************************************************************/
