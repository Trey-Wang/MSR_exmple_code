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
 *         File:  vCanCcGbt_Cbk.h
 *       Module:  vCanCcGbt
 *
 *  Description:  Callback header of the vCanCcGbt module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (VCANCCGBT_CBK_H)
# define VCANCCGBT_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "vCanCcGbt_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*============================================= VCANCCGBT_START_SEC_CODE ============================================*/
# define VCANCCGBT_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  vCanCcGbt_RxIndication()
 *********************************************************************************************************************/
/*! \brief        Indicates the reception of an N-PDU.
 *  \details      Indication of a received PDU from a lower layer communication interface module.
 *  \param[in]    RxPduId         ID of the received N-PDU.
 *  \param[in]    PduInfoPtr      Contains the length (SduLength) of the received N-PDU and a pointer to a buffer
 *                                (SduDataPtr) containing the N-PDU and MetaData.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different RxPduIds.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_RxIndication(PduIdType rxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) pduInfoPtr);

/***********************************************************************************************************************
 *  vCanCcGbt_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms the successful transmission of an N-PDU.
 *  \details      The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 *  \param[in]    txPduId         ID of the N-PDU that has been transmitted.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different RxPduIds.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_TxConfirmation(PduIdType txPduId);

/***********************************************************************************************************************
 *  vCanCcGbt_CopyTxData()
 *********************************************************************************************************************/
/*! \brief        This function indicats the request for new data for a TP transmission.
 *  \details      -
 *  \param[in]    id                PDU id
 *  \param[in]    info              Contains the length of the next data block that should be send and a pointer to the
                                    buffer.
 *  \param[in]    retry
 *  \param[in]    availableDataPtr
 *  \return       BUFREQ_OK
                  BUFREQ_E_NOT_OK
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, VCANCCGBT_CODE) vCanCcGbt_CopyTxData(PduIdType id,
                                                             P2VAR(PduInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) info,
                                                             P2VAR(RetryInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) retry,
                                                             P2VAR(PduLengthType, AUTOMATIC,
                                                                   VCANCCGBT_APPL_DATA) availableDataPtr);

/***********************************************************************************************************************
 *  vCanCcGbt_TpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms the successful transmission of an TP PDU.
 *  \details      -
 *  \param[in]    txPduId   ID of the PDU that has been transmitted.
 *  \param[in]    result    Result of the transmssion.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_TpTxConfirmation(PduIdType txPduId, Std_ReturnType result);

/**********************************************************************************************************************
 *  vCanCcGbt_StartOfReception
 *********************************************************************************************************************/
/*! \brief        This function is called at the start of receiving a TP PDU.
 *  \details      -
 *  \param[in]    pduId           Identification of the N-SDU.
 *  \param[in]    info            Pointer to a PduInfoType structure containing the MetaData and MetaDataLength.
 *  \param[in]    tpSduLength     Total length of the N-SDU to be received.
 *  \param[in]    bufferSizePtr   Available receive buffer in the receiving module. This parameter will be used to
 *                                compute the block size in the transport protocol module.
 *  \return       BUFREQ_OK:      Connection has been accepted. bufferSizePtr indicates the available receive buffer;
 *                                reception is continued. If no buffer of the requested size is available, a receive
 *                                buffer size of 0 shall be indicated by bufferSizePtr.
 *                BUFREQ_E_NOT_OK: Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
 *                BUFREQ_E_OVFL:  No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 *                                remains unchanged.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, VCANCCGBT_CODE) vCanCcGbt_StartOfReception(PduIdType pduId,
                                                                   P2VAR(PduInfoType, AUTOMATIC,
                                                                         VCANCCGBT_APPL_DATA) info,
                                                                   PduLengthType tpSduLength, P2VAR(PduLengthType,
                                                                                                    AUTOMATIC,
                                                                                                    VCANCCGBT_APPL_DATA)
                                                                   bufferSizePtr);

/**********************************************************************************************************************
 *  vCanCcGbt_CopyRxData
 *********************************************************************************************************************/
/*! \brief        This function is called to provide the received data of an N-SDU segment (N-PDU) to the upper layer.
 *  \details      Each call to this function provides the next part of the N-SDU data. The size of the remaining data is
 *                written to the position indicated by bufferSizePtr.
 *  \param[in]    pduId           Identification of the received N-SDU.
 *  \param[in]    info            Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                                (SduLength).
 *  \param[in]    bufferSizePtr   Available receive buffer after data has been copied.
 *  \return       BUFREQ_OK:      Data copied successfully.
 *                BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, VCANCCGBT_CODE) vCanCcGbt_CopyRxData(PduIdType pduId,
                                                             P2VAR(PduInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) info,
                                                             P2VAR(PduLengthType, AUTOMATIC,
                                                                   VCANCCGBT_APPL_DATA) availableDataPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_TpRxIndication
 *********************************************************************************************************************/
/*! \brief        This function is called after a N-SDU has been received via the TP API.
 *  \details      The result indicates whether the reception was successful or not.
 *  \param[in]    pduId         Identification of the received N-SDU.
 *  \param[in]    result          Result of the reception.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_TpRxIndication(PduIdType pduId, Std_ReturnType result);

# define VCANCCGBT_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================= VCANCCGBT_STOP_SEC_CODE =============================================*/

#endif /* VCANCCGBT_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: vCanCcGbt_Cbk.h
 *********************************************************************************************************************/
