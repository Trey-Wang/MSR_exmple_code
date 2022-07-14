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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*! \file  J1939Rm_Cbk.h
 *  \brief SAE J1939 Request Manager (J1939Rm)
 *
 *  \details Callback header of the SAE J1939 Request Manager.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (J1939RM_CBK_H)
# define J1939RM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "J1939Rm_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*============================================== J1939RM_START_SEC_CODE =============================================*/
# define J1939RM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  J1939Rm_RxIndication
 *********************************************************************************************************************/
/*! \brief       Indicates the reception of a J1939Rm PDU from the PduR.
 *  \details     Function only performs the parameter check and calls the internal RxIndication function.
 *  \param[in]   RxPduId         ID of the received I-PDU.
 *  \param[in]   PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 *                               (SduDataPtr) containing the I-PDU and MetaData.
 *  \pre         J1939Rm_RxIndication is not currently executed with the same RxPduId.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Rm_RxIndication(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, J1939RM_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  J1939Rm_TxConfirmation
 *********************************************************************************************************************/
/*! \brief       Confirms the successful transmission of a J1939Rm PDU by the PduR.
 *  \details     Function only performs the parameter check and calls the internal TxConfirmation function.
 *  \param[in]   TxPduId         ID of the I-PDU that has been transmitted.
 *  \pre         J1939Rm_TxConfirmation is not currently executed with the same TxPduId.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Rm_TxConfirmation(PduIdType TxPduId);

/**********************************************************************************************************************
 *  J1939Rm_ComRxIpduCallout
 *********************************************************************************************************************/
/*! \brief       Indicates reception of a requested PDU by COM.
 *  \details     -
 *  \param[in]   PduId           ID of the received I-PDU.
 *  \param[in]   PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to the data of the
 *                               I-PDU and MetaData (SduDataPtr).
 *  \return      TRUE:           I-PDU will be processed normal,
 *  \return      FALSE:          I-PDU will not be processed any further
 *  \pre         J1939Rm_ComRxIpduCallout is not currently executed with the same PduId.
 *  \context     TASK|ISR
 *  \reentrant   TRUE, for different PDU Ids
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, J1939RM_CODE) J1939Rm_ComRxIpduCallout(PduIdType PduId,
                                                     P2VAR(PduInfoType, AUTOMATIC, J1939RM_APPL_DATA) PduInfoPtr);

# define J1939RM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939RM_STOP_SEC_CODE ==============================================*/

#endif /* J1939RM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Rm_Cbk.h
 *********************************************************************************************************************/
