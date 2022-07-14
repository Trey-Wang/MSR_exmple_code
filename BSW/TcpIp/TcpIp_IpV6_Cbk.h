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
 *         \file  TcpIp_IpV6_Cbk.h
 *        \brief  Implementation of Internet Protocol version 6 (IPv6) - Callback Header
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

#if !defined(IPV6_CBK_H)
#define IPV6_CBK_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Std_Types.h"
#include "EthIf.h"

#include "TcpIp_IpV6_Types.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV6_RxIndication()
 **********************************************************************************************************************/
/*! \brief      This function is called by the lower layer (e.g. EthIf) and signals receiption of a new ethernet packet
 *  \param[in]  CtrlIdx     index of the controller that received the frame
 *  \param[in]  FrameType   the frame type of the received frame (0x86DD for IPv6)
 *  \param[in]  IsBroadcast indicates that frame has been sent to broadcast address
 *  \param[in]  PhysAddrPtr physical address (source of the ethernet frame)
 *  \param[in]  DataPtr  pointer to the received payload starting with the IPv6 header
 *  \param[in]  LenByte     length of the received payload in bytes
 *  \context    interrupt
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) IpV6_RxIndication(
  uint8         CtrlIdx,
  Eth_FrameType FrameType,
  boolean       IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte);

/**********************************************************************************************************************
 *  IpV6_TxConfirmation()
 **********************************************************************************************************************/
/*! \brief      This function is called by the lower layer (ethernet) to confirm data transmission
 *  \param[in]  CtrlIdx   index of the controller that trsmitted the frame
 *  \param[in]  BufIdx    index of the tx buffer that has been transmitted
 *  \context    interrupt
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) IpV6_TxConfirmation(
  VAR(uint8, AUTOMATIC) CtrlIdx,
  VAR(uint8, AUTOMATIC) BufIdx);

/**********************************************************************************************************************
 *  IpV6_Cbk_TrcvLinkStateChg()
 **********************************************************************************************************************/
/*! \brief        Called by lower layer (e.g. EthIf) to indicate a change of the transceiver link state
 *  \param[in]    CtrlIdx         index of the ethernet controller
 *  \param[in]    TrcvLinkState   new state of the transceiver
 **********************************************************************************************************************/
extern FUNC(void, IPV6_CODE) IpV6_Cbk_TrcvLinkStateChg(
  uint8                 CtrlIdx,
  EthTrcv_LinkStateType TrcvLinkState);

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* PRQA L:ASR_MEMMAP */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif  /* IPV6_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: IpV6_Cbk.h
 *********************************************************************************************************************/
