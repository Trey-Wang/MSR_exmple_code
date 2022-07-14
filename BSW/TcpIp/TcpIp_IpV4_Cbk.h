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
 *         \file  TcpIp_IpV4_Cbk.h
 *        \brief  Implementation of Internet Protocol version 4 (IPv4) - Callback Header
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

#if !defined (IPV4_IP_CBK_H)
# define IPV4_IP_CBK_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Std_Types.h"
#include "TcpIp_Types.h"
#include "EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define IPV4_START_SEC_CODE
#include "MemMap.h"
/***********************************************************************************************************************
 *  IpV4_Ip_RxIndication
 **********************************************************************************************************************/
/*! \brief        Indication of a received IP message.
 *  \details      This callback function is called by the lower layer (EthIf) when a message arrives.
 *  \param[in]    CtrlIdx               controller index
 *  \param[in]    IpBufferPtr           pointer to the received data
 *  \param[in]    LenByte               length of received data in bytes
 *  \return       void
 *  \context      interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, IPV4_CODE) IpV4_Ip_RxIndication(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte);

/***********************************************************************************************************************
 *  IpV4_Ip_TxConfirmation
 **********************************************************************************************************************/
/*! \brief        Confirmation of a message transmission.
 *  \description  This callback function is called by lower layer (EthIf) if a message has been transmitted by the
 *                hardware.
 *  \param[in]    CtrlIdx               controller index
 *  \param[in]    BufIdx                buffer index
 *  \return       void
 *  \note
 *  \context      interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, IPV4_CODE) IpV4_Ip_TxConfirmation(
    uint8 CtrlIdx,
    uint8 BufIdx );

/***********************************************************************************************************************
 *  IpV4_Ip_Cbk_TrcvLinkStateChg
 **********************************************************************************************************************/
/*! \brief      This callback function is called by lower layer on physical link status change
 *  \param[in]  CtrlIdx               controller index
 *  \param[in]  TrcvLinkState         transceiver linke state
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern FUNC(void, IPV4_CODE) IpV4_Ip_Cbk_TrcvLinkStateChg(
    uint8 CtrlIdx,
    EthTrcv_LinkStateType TrcvLinkState );

#define IPV4_STOP_SEC_CODE
#include "MemMap.h"

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
#endif  /* IPV4_IP_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: Ip_Cbk.h
 *********************************************************************************************************************/
