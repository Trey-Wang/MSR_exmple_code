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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  CanTSyn_Cbk.h
 *        \brief  Vector AUTOSAR CanTSyn Callback header file
 *
 *      \details  This header file contains the prototypes of callback functions of the Vector AUTOSAR module CanTSyn.

 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(CANTSYN_CBK_H)
# define CANTSYN_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "CanTSyn_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CANTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CanTSyn_RxIndication()
 *********************************************************************************************************************/
/*! \brief       Indicate a received I-PDU
 *  \details     This function indicates a received I-PDU from a lower layer communication interface module.
 *  \param[in]   RxPduId                 ID of the received I-PDU.
 *  \param[in]   PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a buffer (SduDataPtr) containing the I-PDU.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE Reentrant for different PduIds. Non reentrant for the same PduId.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CANTSYN_CODE) CanTSyn_RxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief       Confirm a message transmission
 *  \details     The lower layer communication interface module confirms the transmission of an I-PDU.
 *  \param[in]   TxPduId                 ID of the I-PDU that has been transmitted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE Reentrant for different PduIds. Non reentrant for the same PduId.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CANTSYN_CODE) CanTSyn_TxConfirmation(PduIdType TxPduId); /* PRQA S 0850 */ /* MD_MSR_19.8 */

# define CANTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* CANTSYN_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: CanTSyn_Cbk.h
 *********************************************************************************************************************/
