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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  EthIf_Cbk.h
 *        \brief  Ethernet Interface callback header file
 *
 *      \details  Component to abstract the underlying Ethernet hardware for the upper layers to provide a generic
 *                interface to be used for controlling and communicate with the help of the Ethernet hardware
 *                provided by an ECU (Ethernet controller, Ethernet transceiver, Ethernet switch).
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

#if !defined (ETHIF_CBK_H)
# define ETHIF_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Types.h"
# include "EthIf_Switch_Cbk.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthIf_RxIndication
 *********************************************************************************************************************/
/*! \brief      Notifies the EthIf about a received Ethernet frame
 *  \details    Functions takes the given Ethernet frame data, analysis the Ethernet header for VLAN and EtherType
 *              information and decides whether to drop the frame or pass it to a known EthIf user.
 *  \param[in]  CtrlIdx        Ethernet controller index
 *  \param[in]  FrameType      EtherType the Ethernet frame is related to
 *  \param[in]  IsBroadcast    Broadcast indication:
 *                             FALSE - frame isn't a broadcast frame
 *                             TRUE - frame is a broadcast frame
 *  \param[in]  PhysAddrPtr    Source MAC address of the Ethernet frame
 *  \param[out] DataPtr        Location of the Ethernet frame payload (no VLAN tag considered)
 *  \param[in]  LenByte        Length of the Ethernet frame payload (no VLAN tag considered)
 *  \pre         Module has been initialized
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111172
 *  \trace SPEC-2393651
 *  \trace SPEC-2393655
 *  \trace SPEC-2393933
 *  \trace SPEC-2393658
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_RxIndication(
        uint8                                      CtrlIdx,
        Eth_FrameType                              FrameType,
        boolean                                    IsBroadcast,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA)  DataPtr,
        uint16                                     LenByte);

/**********************************************************************************************************************
 *  EthIf_TxConfirmation
 *********************************************************************************************************************/
/*! \brief      Notifies the EthIf about the transmission of a Ethernet frame
 *  \details    Function handles the confirmation of an Ethernet frame transmission and passes it to the respective
 *              EthIf user.
 *  \param[in]  CtrlIdx        Ethernet controller index
 *  \param[in]  BufIdx         Index of the buffer the transmission is confirmed for
 *  \pre         Module has been initialized
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111171
 *  \trace SPEC-2393659
 *  \trace SPEC-2393664
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_TxConfirmation(
  uint8  CtrlIdx,
  uint8  BufIdx);

# define ETHIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Cbk.h
 *********************************************************************************************************************/

