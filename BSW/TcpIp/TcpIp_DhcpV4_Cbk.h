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
 *         \file  TcpIp_DhcpV4_Cbk.h
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv4 - Client (DHCPv4 Client) - Callback Header
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

#if !defined (TCPIP_DHCPV4_CBK_H)
# define TCPIP_DHCPV4_CBK_H

#include "TcpIp.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp.h"

#include "TcpIp_IpV4_Cfg.h"
#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  TcpIp_DhcpV4_RxIndication
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  IpCtrlIdx            controller index of the received message
 *  \param[in]  DataPtr              pointer to the received data
 *  \param[in]  DataLen              length of the received data
 *  \param[in]  SourcePtr            source network address and port
 *  \param[in]  DestinationPtr       destination network address and port
 *  \param[in]  ChecksumCalculated   indicates if the UDP checksum is already checked by lower layer
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_RxIndication(
    uint8 IpCtrlIdx,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 DataLen,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SourcePtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestinationPtr
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    , boolean ChecksumCalculated
#endif
        );

/***********************************************************************************************************************
 *  Dummy callback functions. Functions not implemented because DHCP does not use TCP
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_TcpAccepted(TcpIp_SockHndType SockHnd);
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_TcpConnected(TcpIp_SockHndType SockHnd);
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_TcpIpEvent(TcpIp_SockHndType SockHnd, IpBase_TcpIpEventType Event);

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
#endif  /* TCPIP_DHCPV4_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_DhcpV4_Cbk.h
 *********************************************************************************************************************/
