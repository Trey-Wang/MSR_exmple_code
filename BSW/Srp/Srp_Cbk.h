/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Srp_Cbk.h
 *        \brief  SRP Call-back header file
 *
 *      \details  Call-back header file of the Stream Registration Protocol (SRP)
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



#ifndef SRP_CBK_H
# define SRP_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Srp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SRP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  Srp_Mmrp_RxIndication
 *********************************************************************************************************************/
/**
 *  \brief          Handles processing of received MMRP frames.
 *  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
 *  \param[in]      FrameType    Ethertype of the frame
 *  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
 *  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
 *  \param[in]      DataPtr      Pointer to the received data.
 *  \param[in]      LenByte      Byte count of the received frame.
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
#if (SRP_MMRP_SUPPORT == STD_ON)
FUNC(void, SRP_CODE) Srp_Mmrp_RxIndication(
  uint8 VCtrlIdx, 
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  SRP_P2VAR(uint8) PhysAddrPtr,
  SRP_P2VAR(uint8) DataPtr,
  uint16 LenByte);
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */


/**********************************************************************************************************************
 *  Srp_Mvrp_RxIndication
 *********************************************************************************************************************/
/**
 *  \brief          Handles processing of received MVRP frames.
 *  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
 *  \param[in]      FrameType    Ethertype of the frame
 *  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
 *  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
 *  \param[in]      DataPtr      Pointer to the received data.
 *  \param[in]      LenByte      Byte count of the received frame.
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_Mvrp_RxIndication(
  uint8 VCtrlIdx, 
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  SRP_P2VAR(uint8) PhysAddrPtr,
  SRP_P2VAR(uint8) DataPtr,
  uint16 LenByte);

/**********************************************************************************************************************
 *  Srp_Msrp_RxIndication
 *********************************************************************************************************************/
/**
 *  \brief          Handles processing of received MSRP frames.
 *  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
 *  \param[in]      FrameType    Ethertype of the frame
 *  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
 *  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
 *  \param[in]      DataPtr      Pointer to the received data.
 *  \param[in]      LenByte      Byte count of the received frame.
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_Msrp_RxIndication(
  uint8 VCtrlIdx, 
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  SRP_P2VAR(uint8) PhysAddrPtr,
  SRP_P2VAR(uint8) DataPtr,
  uint16 LenByte);

/**********************************************************************************************************************
 *  Srp_Cbk_TrcvLinkStateChg
 *********************************************************************************************************************/
/**
  \brief        Callback function that notifies a changed state of the transceiver link
  \param[in]    CtrlIdx         Index of the controller that changed its state
  \param[in]    TrcvLinkState   New link state of the transceiver
  \return       none
  \context      This function can be called in task context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_Cbk_TrcvLinkStateChg(
  uint8 CtrlIdx,
  Srp_LinkStateType TrcvLinkState);

# define SRP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */


#endif /* SRP_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: Srp_Cbk.h
 *********************************************************************************************************************/
