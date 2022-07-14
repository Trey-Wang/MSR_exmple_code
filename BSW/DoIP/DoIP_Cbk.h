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
/**        \file  DoIP_Cbk.h
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Header file for callbacks of Diagnostic over IP
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if ( !defined(DOIP_CBK_H) )
# define DOIP_CBK_H

/**********************************************************************************************************************
* Include files
 *********************************************************************************************************************/

# include "DoIP_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define DOIP_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  DoIP_SoAdTpCopyTxData()
 *********************************************************************************************************************/
/*! \brief          Queries transmit data of a PDU
 *  \details        This function is called to acquire the transmit data of an I-PDU segment (N-PDU). Each call to this
 *                  function provides the next part of the TP PDU data. The size of the remaining data is written to
 *                  the position indicated by availableDataPtr.
 *  \param[in]      id                Identification of the transmitted TP PDU.
 *  \param[in]      info              Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                    (SduLength). An SduLength of 0 can be used to query the current amount of
 *                                    available data in the upper layer module. In this case, the SduDataPtr may be a
 *                                    NULL_PTR.
 *  \param[in]      retry             Retry is not supported by SoAd ( NULL_PTR )
 *  \param[out]     availableDataPtr  Indicates the remaining number of bytes that are available in the upper layer
 *                                    module's Tx buffer.
 *  \return         BUFREQ_OK         Data has been copied to the transmit buffer completely as requested.
 *  \return         BUFREQ_E_NOT_OK   Data has not been copied. Request failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyTxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  P2VAR(RetryInfoType, AUTOMATIC, DOIP_APPL_VAR) retry,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) availableDataPtr);

/**********************************************************************************************************************
 *  DoIP_SoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Transmission confirmation callback for TP
 *  \details        This function is called after the TP PDU has been transmitted on its network, the result indicates
 *                  whether the transmission was successful or not.
 *  \param[in]      id      Identification of the transmitted TP PDU.
 *  \param[in]      result  Result of the transmission of the TP PDU.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SoAdTpTxConfirmation(
  PduIdType id,
  Std_ReturnType result);

/**********************************************************************************************************************
 *  DoIP_SoAdTpCopyRxData()
 *********************************************************************************************************************/
/*! \brief          Called when SoAd has data to copy
 *  \details        This function is called to provide the received data of an TP PDU segment (N-PDU) to the upper
 *                  layer. Each call to this function provides the next part of the TP PDU data. The size of the
 *                  remaining data is written to the position indicated by bufferSizePtr.
 *  \param[in]      id                Identification of the received TP PDU.
 *  \param[in]      info              Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                                    (SduLength). An SduLength of 0 can be used to query the current amount of
 *                                    available buffer in the upper layer module. In this case, the SduDataPtr may be
 *                                    a NULL_PTR.
 *  \param[out]     bufferSizePtr     Available receive buffer after data has been copied.
 *  \return         BUFREQ_OK         Data copied successfully.
 *  \return         BUFREQ_E_NOT_OK   Data was not copied because an error occurred.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyRxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  DoIP_SoAdTpStartOfReception()
 *********************************************************************************************************************/
/*! \brief          Receive indication callback for TP
 *  \details        This function is called at the start of receiving an N-SDU. The N-SDU might be fragmented into
 *                  multiple N-PDUs or might consist of a single N-PDU.
 *  \param[in]      id                Identification of the TP PDU.
 *  \param[in]      info              Pointer to data to support first or single frames (not used by SoAd)
 *  \param[in]      TpSduLength       Total length of the N-SDU to be received.
 *  \param[out]     bufferSizePtr     Available receive buffer in the receiving module.
 *  \return         BUFREQ_OK         Connection has been accepted. bufferSizePtr indicates the available receive
 *                                    buffer; reception is continued. If no buffer of the requested size is
 *                                    available, a receive buffer size of 0 shall be indicated by bufferSizePtr.
 *  \return         BUFREQ_E_NOT_OK   Connection has been rejected; reception is aborted. bufferSizePtr remains
 *                                    unchanged.
 *  \return         BUFREQ_E_OVFL     No buffer of the required length can be provided; reception is aborted.
 *                                    bufferSizePtr remains unchanged.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpStartOfReception(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) info,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  DoIP_SoAdTpRxIndication()
 *********************************************************************************************************************/
/*! \brief          Receive indication callback for TP
 *  \details        Called after an TP PDU has been received via the TP API, the result indicates whether the
 *                  transmission was successful or not.
 *  \param[in]      id      Identification of the received TP PDU.
 *  \param[in]      result  Result of the reception.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SoAdTpRxIndication(
  PduIdType id,
  Std_ReturnType result);

/**********************************************************************************************************************
 *  DoIP_SoAdIfRxIndication()
 *********************************************************************************************************************/
/*! \brief          Receive indication callback for IF
 *  \details        Indication of a received IF PDU from a lower layer communication interface module.
 *  \param[in]      RxPduId     ID of the received IF PDU.
 *  \param[in]      PduInfoPtr  Contains the length (SduLength) of the received IF PDU and a pointer to a buffer
 *                              (SduDataPtr) containing the IF PDU.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different RxPduIds. Non reentrant for the same RxPduId.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SoAdIfRxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  DoIP_SoAdIfTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Transmission confirmation callback for IF
 *  \details        The lower layer communication interface module confirms the transmission of an IF PDU.
 *  \param[in]      TxPduId   ID of the IF PDU that has been transmitted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \note           No functionality is implemented.
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation(
  PduIdType TxPduId);

/**********************************************************************************************************************
 *  DoIP_SoConModeChg()
 *********************************************************************************************************************/
/*! \brief          Socket state change callback
 *  \details        Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 *  \param[in]      SoConId   Socket connection index specifying the socket connection with the mode change.
 *  \param[in]      Mode      New mode
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different SoConIds. Non reentrant for the same SoConId.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode);

/**********************************************************************************************************************
 *  DoIP_LocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*! \brief          IP address assignment change callback
 *  \details        This function gets called by the SoAd if an IP address assignment related to a socket connection
 *                  changes (i.e. new address assigned or assigned address becomes invalid).
 *  \param[in]      SoConId   Socket connection index specifying the socket connection where the IP address assigment
 *                            has changed.
 *  \param[in]      State     State of IP address assignment.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE - Reentrant for different SoConIds. Non reentrant for the same SoConId.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_LocalIpAddrAssignmentChg(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrStateType State);

/**********************************************************************************************************************
 *  DoIP_ActivationLineSwitch()
 *********************************************************************************************************************/
/*! \brief          Notifies DoIP on a switch of the DoIPActivationLine
 *  \details        This function is used to notify the DoIP on a switch of the DoIPActivationLine.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitch(void);

/**********************************************************************************************************************
 *  DoIP_ShutdownFinished()
 *********************************************************************************************************************/
/*! \brief          Indicates shutdown of SoAd
 *  \details        -
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, DOIP_CODE) DoIP_ShutdownFinished(void);

# define DOIP_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* !defined(DOIP_CBK_H) */

/**********************************************************************************************************************
 *  END OF FILE: DoIP_Cbk.h
 *********************************************************************************************************************/
