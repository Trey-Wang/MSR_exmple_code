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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Dcm_Cbk.h
 *    Component:  MICROSAR Diagnostic Communication Manager for SAE J1939
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  callback definition header file for J1939 DCM
 *  
 *********************************************************************************************************************/

#if !defined (J1939DCM_CBK_H)
#define J1939DCM_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "J1939Dcm_Cfg.h"
#include "J1939Dcm_Types.h"
#include "J1939Rm_Types.h"

/**********************************************************************************************************************
 *  CALLBACK FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* --- callbacks from J1939 RM --------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
 * J1939Dcm_RequestIndication()
 **********************************************************************************************************************/
/*! \brief      Indicates reception of a Requested DM.
 *  \details    Updates DM queue upon receiving a request. Triggers NACK whenever conditions are not suitable 
 *              to accept a new request.
 *  \param[in]  node          Node by which the request was received
 *  \param[in]  channel       Channel on which the request was received
 *  \param[in]  requestedPgn  PGN of the requested DM
 *  \param[in]  extIdInfo     Extended identifier bytes
 *  \param[in]  sourceAddress Address of the node that sent the Requested DM
 *  \param[in]  destAddress   Address of this node or 0xFF for broadcast
 *  \param[in]  priority      Priority of the Request PG
 *  \context    ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        -
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_RequestIndication( uint8 node,
                                                      NetworkHandleType channel,
                                                      uint32 requestedPgn,
                                                      P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939DCM_APPL_DATA) extIdInfo,
                                                      uint8 sourceAddress,
                                                      uint8 destAddress,
                                                      uint8 priority ); /* SWS_J1939Dcm_00101 */

/* --- callbacks from PduR ------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************
 * J1939Dcm_RxIndication()
 **********************************************************************************************************************/
/*! \brief      Indicates a reception of I-PDU from a lower layer communication interface module.
 *  \details    Extracts the message contents of DM13, DM14, and DM17 for further processing.
 *  \param[in]  RxPduId                 ID of the received I-PDU
 *  \param[in]  PduInfoPtr->SduLength   Length of the received I-PDU
 *              PduInfoPtr->SduDataPtr  pointer to buffer containing the I-PDU
 *  \context    ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        -
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_RxIndication( PduIdType RxPduId,
                                                 P2CONST(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) PduInfoPtr ); /* SWS_J1939Dcm_00128 */

/**********************************************************************************************************************
 * J1939Dcm_StartOfReception()
 **********************************************************************************************************************/
/*! \brief       Indicates start of an I-PDU reception.
 *  \details     Triggers the application to start receiving new message coming from PduR.
 *  \param[in]   id                ID of the received I-PDU
 *  \param[in]   info->SduLength   Length of the received SF / FF payload                       (not yet supported)
 *               info->SduDataPtr  pointer to buffer containing the FF / SF payload + meta data  (not yet supported)
 *  \param[in]   TpSduLength       Total length of the I-PDU to be received
 *  \param[out]  bufferSizePtr     Available receive buffer
 *  \return      BUFREQ_OK         Application was successfully triggered to receive data from PduR
 *  \return      BUFREQ_E_NOT_OK   Failed to trigger the application to receive data from PduR
 *  \context     ISR1|ISR2
 *  \reentrant   TRUE
 *  \pre         -
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939DCM_CODE) J1939Dcm_StartOfReception( PduIdType id,
                                                                  P2VAR(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) info,
                                                                  PduLengthType TpSduLength,
                                                                  P2VAR(PduLengthType, AUTOMATIC, J1939DCM_APPL_DATA) bufferSizePtr ); /* SWS_J1939Dcm_00102 */

/**********************************************************************************************************************
 * J1939Dcm_CopyRxData()
 **********************************************************************************************************************/
/*! \brief       Provides the received data of an I-PDU segment to the J1939DCM.
 *  \details     Triggers the application to Copy data from the PduR buffer.
 *  \param[in]   id                ID of the received I-PDU
 *  \param[in]   info->SduLength   Length of the data to be copied
 *               info->SduDataPtr  pointer to the data to be copied
 *  \param[out]  bufferSizePtr     Available receive buffer after the data have been copied
 *  \return      BUFREQ_OK         J1939Dcm copied the Data successfully in PduR buffer
 *  \return      BUFREQ_E_NOT_OK   1939Dcm failed to copy the data in the PduR buffer
 *  \context     ISR1|ISR2
 *  \reentrant   TRUE
 *  \pre         Reception must be in progress (start of reception was triggered)
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939DCM_CODE) J1939Dcm_CopyRxData( PduIdType id,
                                                            P2CONST(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) info,
                                                            P2VAR(PduLengthType, AUTOMATIC, J1939DCM_APPL_DATA) bufferSizePtr ); /* SWS_J1939Dcm_00103 */

FUNC(void, J1939DCM_CODE) J1939Dcm_TpRxIndication( PduIdType id,
                                                   Std_ReturnType result ); /* SWS_J1939Dcm_00104 */

/**********************************************************************************************************************
 * J1939Dcm_CopyTxData()
 **********************************************************************************************************************/
/*! \brief       Provides the transmit data of an I-PDU segment to the J1939Tp.
 *  \details     Copies data from J1939Dcm transmit buffer to a buffer provided by PduR.
 *  \param[in]   id                ID of the I-PDU to be transmitted
 *  \param[in]   info->SduLength   Length of the data to be copied
 *               info->SduDataPtr  pointer to the data to be copied
 *  \param[in]   retry             parameter to request a retransmit in case of transmission problems
 *  \param[out]  availableDataPtr  Available transmit buffer size after the data have been copied
 *  \return      BUFREQ_OK         Application copied the data successfully from PduR buffer
 *               BUFREQ_E_NOT_OK   Application failed to copy the data from PduR buffer
 *  \context     ISR1|ISR2
 *  \reentrant   FALSE
 *  \pre         Transmission must be in progress
 
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939DCM_CODE) J1939Dcm_CopyTxData( PduIdType id,
                                                            P2VAR(PduInfoType, AUTOMATIC, J1939DCM_APPL_DATA) info,
                                                            P2VAR(RetryInfoType, AUTOMATIC, J1939DCM_APPL_DATA) retry,
                                                            P2VAR(PduLengthType, AUTOMATIC, J1939DCM_APPL_DATA) availableDataPtr ); /* SWS_J1939Dcm_00105 */

/**********************************************************************************************************************
 * J1939Dcm_TpTxConfirmation()
 **********************************************************************************************************************/
/*! \brief      Triggers transmission finalization after an I-PDU has been transmitted via the TP API.
 *  \details    Initiates transmission finalization on the node level only when the awaited confirmations for all the
 *              referenced channels by this node arrive.
 *  \param[in]  id        ID of the transmitted I-PDU
 *              result    Result of the reception.
 *  \context    ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        Transmission must have been started
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_TpTxConfirmation( PduIdType id,
                                                     Std_ReturnType result ); /* SWS_J1939Dcm_00106 */

/**********************************************************************************************************************
 * J1939Dcm_TxConfirmation()
 **********************************************************************************************************************/
/*! \brief      Triggers transmission finalization after an I-PDU has been transmitted via the IF API.
 *  \details    Initiates transmission finalization for non periodic messages of a length 8 bytes.  
 *  \param[in]  id        ID of the transmitted I-PDU
 *  \context    ISR1|ISR2
 *  \reentrant  TRUE
 *  \pre        Transmission must have been started
 **********************************************************************************************************************/
FUNC(void, J1939DCM_CODE) J1939Dcm_TxConfirmation( PduIdType id ); /* RfC 61706 */


/* --- callbacks from DEM -------------------------------------------------------------------------------------------*/
FUNC(Std_ReturnType, J1939DCM_CODE) J1939Dcm_DemTriggerOnDTCStatus( uint32 DTC,
                                                                    uint8 DTCStatusOld,
                                                                    uint8 DTCStatusNew ) ;  /* SWS_J1939Dcm_00122 + RfC60880 */

/* AUTOSAR defines two different names for the same interface! */
#define J1939Dcm_TriggerOnDTCStatusChange J1939Dcm_DemTriggerOnDTCStatus  /* SWS_J1939Dcm_00122 without RfC60880 */

#endif  /* J1939DCM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Dcm_Cbk.h
 *********************************************************************************************************************/
