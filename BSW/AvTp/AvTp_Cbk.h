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
 *         File:  AvTp_Cbk.h
 *      Project:  AvTp
 *       Module:  Tp_Ieee1722AvTp
 *    Generator:  Tp_Ieee1722AvTp.jar
 *
 *  Description:  Call-back header of the Audio Video Bridging (AVB) Transport Protocol
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

#if !defined(AVTP_CBK_H)
#define AVTP_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "AvTp_Types.h"
#include "EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define AVTP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  AvTp_RxIndication
 *********************************************************************************************************************/
/**
  \brief          Handles processing of received AvTp frames.
  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
  \param[in]      FrameType    Ethertype of the frame
  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
  \param[in]      DataPtr      Pointer to the received data.
  \param[in]      LenByte      Byte count of the received frame.
  \return         none
  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_RxIndication(
  uint8 VCtrlIdx, 
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  AVTP_P2VAR(uint8) PhysAddrPtr,
  AVTP_P2VAR(uint8) DataPtr,
  uint16 LenByte);

/**********************************************************************************************************************
 *  AvTp_Cbk_TrcvLinkStateChg
 *********************************************************************************************************************/
/**
  \brief      Callback function that indicates a changed transceiver link state 
  \param[in]  VCtrlIdx       Index of virtual controller that link status changed
  \param[in]  TrcvLinkState  New link state of the transceiver
  \return     none
  \context    Function must be called from task level
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_Cbk_TrcvLinkStateChg(
  uint8 VCtrlIdx,
  AvTp_LinkStateType TrcvLinkState);

#define AVTP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* AVB_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: AvTp_Cbk.h
 *********************************************************************************************************************/
