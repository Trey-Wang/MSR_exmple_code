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
/*!        \file  FrTSyn_Cbk.h
 *        \brief  Vector AUTOSAR FrTSyn Callback header file
 *
 *      \details  This header file contains the prototypes of callback functions of the Vector AUTOSAR module FrTSyn.

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

#if !defined(FRTSYN_CBK_H)
# define FRTSYN_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "FrTSyn_Cfg.h"

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

# define FRTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (FRTSYN_SLAVE_COUNT)
/**********************************************************************************************************************
 *  FrTSyn_RxIndication()
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
FUNC(void, FRTSYN_CODE) FrTSyn_RxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# else
# define FrTSyn_RxIndication(RxPduId, PduInfoPtr) (E_NOT_OK) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif

# if (FRTSYN_MASTER_COUNT)
/**********************************************************************************************************************
 *  FrTSyn_TriggerTransmit()
 *********************************************************************************************************************/
/*! \brief       Trigger a message transmission
 *  \details     The upper layer module (called module) shall check whether the available data fits into the buffer size reported by PduInfoPtr->SduLength.
 *               If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied data in
 *               PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 *  \param[in]     TxPduId              ID of the SDU that is requested to be transmitted.
 *  \param[in,out] PduInfoPtr           Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied, and the available buffer size
 *                                      in SduLength. On return, the service will indicate the length of the copied SDU data in SduLength.
 *  \return      E_OK     - SDU has been copied and SduLength indicates the number of copied bytes.
 *  \return      E_NOT_OK - No SDU data has been copied. PduInfoPtr must not be used since it may contain a NULL pointer or point to invalid data.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE Reentrant for different PduIds. Non reentrant for the same PduId.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TriggerTransmit(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) PduInfoPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */
# else
# define FrTSyn_TriggerTransmit(TxPduId, PduInfoPtr) (E_NOT_OK) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif

# define FRTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FRTSYN_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: FrTSyn_Cbk.h
 *********************************************************************************************************************/
