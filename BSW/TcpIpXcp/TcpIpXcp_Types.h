/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  TcpIpXcp_Types.h
 *    Component:  TcpIpXcp Types Header
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implementation of XCP over TcpIp based on Vector AUTOSAR Stack
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Herkommer             Hr            Vector Informatik GmbH
 *  Oliver Reineke                Ore           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2008-10-09  Hr                    First Version
 *  02.00.00  2013-06-04  Hr      ESCAN00059277 Adapt component to SoAd
 *                        Hr      ESCAN00069959 Support ASR4 environment
 *  02.00.01  2013-11-15  Hr      ESCAN00071975 Missing XcpOnEth header in response packet
 *  02.00.02  2014-02-25  Ore     ESCAN00073922 Compiler error: Symbol "TCPIPXCP_EXCLUSIVE_AREA_0" unknown
 *                        Hr      ESCAN00074272 SetPduMode feature leads to continuous transmission
 *  02.00.03  2014-05-09  Hr      ESCAN00075589 Improve robustness of use case "if transmission of XCP on Ethernet frame fails"
 *  02.00.04  2014-07-07  Hr      ESCAN00076800 Rx buffer not aligned to 32Bit
 *                        Hr      ESCAN00076953 Xcp connection terminated in case of Alive Supervision Timeout
 *  02.01.00  2015-02-16  Hr      ESCAN00079998 Missing memory abstraction in header
 *            2015-04-23  Hr      ESCAN00080791 Support Resume Mode
 *  02.02.00  2015-05-05  Hr      ESCAN00082845 Support TxConfirmation timeout timer
 *  02.02.01  2015-11-12  Hr      ESCAN00086397 FEAT-1357: SafeXCP
 *                        Hr      ESCAN00085638 Compiler warning: Modifiable lvalue
 *  03.00.00  2016-08-31  Hr      ESCAN00091918 FEAT-1980: Add Multi Client / Multi Connection support
 *                        Hr      ESCAN00091954 Safe deactivation does not consider TcpIpXcp_Init
 *                        Hr      ESCAN00091955 Safe deactivation shall be performed before DET checks or anything
 *  03.00.01  2017-04-19  Hr      ESCAN00094223 DET error due to incorrect channel id conversion
 *********************************************************************************************************************/

#if !defined (TCPIPXCP_TYPES_H)
#define TCPIPXCP_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"
#include "SoAd.h"

/**********************************************************************************************************************
 *  Type definitions
 *********************************************************************************************************************/
#define TCPIPXCP_PROTOCOL_UDP 0
#define TCPIPXCP_PROTOCOL_TCP 1

typedef uint16 TcpIpXcp_TimeoutTimerType;

typedef enum
{
  TCPIPXCP_SET_OFFLINE = 0,
  TCPIPXCP_SET_ONLINE
} TcpIpXcp_PduSetModeType;

typedef struct
{
  PduIdType RxPduId;
  PduIdType SoAdTxPduId;
  PduIdType TxConfPduId;
  SoAd_SoConIdType SoConId;
} TcpIpXcp_XcpPduType;

typedef struct
{
  P2CONST(TcpIpXcp_XcpPduType, TYPEDEF, TCPIPXCP_PBCFG) TcpIpXcp_XcpPduPtr;
} TcpIpXcp_ConfigType;

#endif
  /* TCPIPXCP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: TcpIpXcp_Types.h
 *********************************************************************************************************************/
