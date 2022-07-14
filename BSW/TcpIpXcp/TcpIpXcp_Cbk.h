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
 *         File:  TcpIpXcp_Cbk.h
 *    Component:  TcpIpXcp Callback Header
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Callback header of the XCP on TcpIp Transport Layer
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
#if !defined(__TCPIPXCP_CBK_H__)
#define __TCPIPXCP_CBK_H__

/**************************************************************************************************
* Include files
**************************************************************************************************/
#include "ComStack_Types.h"            /* Include standard types */
#include "TcpIpXcp_Cfg.h"
#if ( TCPIPXCP_MODE_CHG_API == STD_ON )
#include "SoAd.h"
#endif

/**************************************************************************************************
* Prototypes of callback functions
**************************************************************************************************/

#define TCPIPXCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern FUNC(void, TCPIPXCP_CODE) Xcp_SoAdRxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, TCPIPXCP_APPL_DATA) PduInfoPtr);
extern FUNC(void, TCPIPXCP_CODE) Xcp_SoAdTxConfirmation(PduIdType TxPduId);

#if ( TCPIPXCP_MODE_CHG_API == STD_ON )
extern FUNC(void, TCPIPXCP_CODE) Xcp_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode);
#endif

#define TCPIPXCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* __TCPIPXCP_CBK_H__ */
