/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Tp_Cbk.h
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Callback header of the SAE J1939 Transport Layer module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (J1939TP_CBK_H)
# define J1939TP_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Tp_Cfg.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  J1939Tp_RxIndication()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of an N-PDU.
 *  \details      Indication of a received PDU from a lower layer communication interface module.
 *  \param[in]    RxPduId         ID of the received N-PDU.
 *  \param[in]    PduInfoPtr      Contains the length (SduLength) of the received N-PDU and a pointer to a buffer
 *                                (SduDataPtr) containing the N-PDU and MetaData.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different RxPduIds.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_RxIndication(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) PduInfoPtr);

/***********************************************************************************************************************
 *  J1939Tp_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms the successful transmission of an N-PDU.
 *  \details      The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 *  \param[in]    TxPduId         ID of the N-PDU that has been transmitted.
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different RxPduIds.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_TxConfirmation(PduIdType TxPduId);

# define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/

#endif /* !defined (J1939TP_CBK_H) */

/**********************************************************************************************************************
 *  END OF FILE: J1939Tp_Cbk.h
 *********************************************************************************************************************/
