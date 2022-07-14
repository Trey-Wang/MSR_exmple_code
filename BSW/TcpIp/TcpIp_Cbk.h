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
 *         \file  TcpIp_Cbk.h
 *        \brief  Implementation of TcpIp Module - Callback Header
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_CBK_H)
# define TCPIP_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Cfg.h"
#include "Eth_GeneralTypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_RxIndication
 **********************************************************************************************************************/
/*! \brief         Handles the indication and the data of a received frame.
 *  \details       This function will simply forward the incoming RxIndication to the corresponding RxIndications in the
 *                 modules IpV4 and IpV6.
 *  \param[in]     CtrlIdx       Index of the EthIf controller.
 *  \param[in]     FrameType     frame type of received Ethernet frame
 *  \param[in]     IsBroadcast   parameter to indicate a broadcast frame
 *  \param[in]     PhysAddrPtr   pointer to Physical source address (MAC address in network byte order) of received Ethernet frame
 *  \param[in]     DataPtr       Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided).
 *  \param[in]     LenByte       Length of received data.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-116254
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_RxIndication(
    uint8 CtrlIdx,
    Eth_FrameType FrameType,
    boolean IsBroadcast,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 LenByte );

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIP_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Cbk.h
 *********************************************************************************************************************/
