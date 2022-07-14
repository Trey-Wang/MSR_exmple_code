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
 *         File:  EthFw_Cbk.h
 *       Module:  SysService_FwEth
 *    Generator:  SysService_FwEth.jar (DaVinci Configurator Pro)
 *
 *  Description:  Callback header of Firewall for Ethernet (ETHFW)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(ETHFW_CBK_H)
#define ETHFW_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "EthFw_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHFW_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthFw_IsFrameRxAllowed()
 **********************************************************************************************************************/
/*! \brief        Checks if the message matches the configured filter rules and shall be received.
 *  \param[in]    CtrlIdx         Index of the Ethernet controller.
 *  \param[in]    FrameHdrPtr     Structure containing Ethernet header information
 *  \param[in]    PayloadPtr      Pointer to payload which was received and shall be forwarded
 *  \param[in]    PayloadLen      Payload length
 *  \return       TRUE  - Forward message
 *  \return       FALSE - Discard message
 *  \pre          Module has been initialized
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-120299
 **********************************************************************************************************************/
FUNC(boolean, ETHFW_CODE) EthFw_IsFrameRxAllowed(
  uint8 CtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) FrameHdrPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
  uint16 PayloadLen);

/**********************************************************************************************************************
 *  EthFw_IsFrameTxAllowed()
 **********************************************************************************************************************/
/*! \brief        Checks if the message matches the configured filter rules and shall be transmitted.
 *  \param[in]    CtrlIdx         Index of the Ethernet controller.
 *  \param[in]    FrameHdrPtr     Structure containing Ethernet header information
 *  \param[in]    PayloadPtr      Pointer to payload which shall be transmitted
 *  \param[in]    PayloadLen      Payload length
 *  \return       TRUE  - Transmit message
 *  \return       FALSE - Discard message
 *  \pre          Module has been initialized
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-120300
 **********************************************************************************************************************/
FUNC(boolean, ETHFW_CODE) EthFw_IsFrameTxAllowed(
  uint8 CtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) FrameHdrPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
  uint16 PayloadLen);

#define ETHFW_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* ETHFW_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthFw_Cbk.h
 *********************************************************************************************************************/
