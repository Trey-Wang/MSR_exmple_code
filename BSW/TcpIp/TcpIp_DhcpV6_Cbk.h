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
 *         \file  TcpIp_DhcpV6_Cbk.h
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv6 - Client (DHCPv6 Client) - Callback Header
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

#if !defined (TCPIP_DHCPV6_CBK_H)
# define TCPIP_DHCPV6_CBK_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp.h"

#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_RxIndication
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  IpCtrlIdx          ip controller index
 *  \param[in]  DataPtr            pointer to the received data
 *  \param[in]  DataLen            length of the received data in bytes
 *  \param[in]  SourcePtr          source address pointer
 *  \param[in]  DestinationPtr     destination address pointer
 *  \param[in]  ChecksumCalculated    indicates if the UDP checksum is already checked by lower layer
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_RxIndication(
    uint8                                                    IpCtrlIdx,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA)                 DataPtr,
    uint16                                                   DataLen,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SourcePtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestinationPtr
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    , boolean ChecksumCalculated
#endif
    );

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_TxConfirmation
 **********************************************************************************************************************/
/*! \brief      Transmission Confirmation Callback
 *  \param[in]  SockHnd            socket handle
 *  \param[in]  Length             length in bytes
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_TxConfirmation(
    TcpIp_SockHndType SockHnd,
    uint32 Length );

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Cbk_LocalIpAssignmentChg
 **********************************************************************************************************************/
/*! \brief      Callback for change of network address assignment notification
 *  \param[in]  IpAddrId           ip address identifier
 *  \param[in]  State              assignment state
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_Cbk_LocalIpAssignmentChg(
    IpV6_AddrIdType       IpAddrId,
    TcpIp_IpAddrStateType State);

/***********************************************************************************************************************
 *  Dummy callback functions. Functions not implemented because DHCP does not use TCP
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_TcpAccepted(TcpIp_SockHndType SockHnd);
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_TcpConnected(TcpIp_SockHndType SockHnd);
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_TcpIpEvent(TcpIp_SockHndType SockHnd, IpBase_TcpIpEventType Event);

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif  /* TCPIP_DHCPV4_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_DhcpV6_Cbk.h
 *********************************************************************************************************************/
