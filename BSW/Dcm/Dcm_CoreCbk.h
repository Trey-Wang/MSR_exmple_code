/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_CoreCbk.h
 *        \brief  Callback interface of DCM for other BSWs
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
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
#if !defined(DCM_CORECBK_H)
# define DCM_CORECBK_H
/* ----------------------------------------------
 ~&&&   Versions
---------------------------------------------- */

/*! Implementation version */
# define DCM_CORECBK_MAJOR_VERSION                                   8u
# define DCM_CORECBK_MINOR_VERSION                                   3u
# define DCM_CORECBK_PATCH_VERSION                                   1u
/* ----------------------------------------------
 ~&&&   Module API function declarations
---------------------------------------------- */
# define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h"                                                                                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
# if (DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED == STD_ON)
/* --- Com Manager API --------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  Dcm_ComM_NoComModeEntered()
 *********************************************************************************************************************/
/*! \brief          ComM call-back for entering no communication mode.
 *  \details        This call informs the DCM module about a ComM mode change to COMM_NO_COMMUNICATION.
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(void);

/**********************************************************************************************************************
 *  Dcm_ComM_SilentComModeEntered()
 *********************************************************************************************************************/
/*! \brief          ComM callback for entering silent communication mode.
 *  \details        This call informs the DCM module about a ComM mode change to COMM_SILENT_COMMUNICATION.
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(void);

/**********************************************************************************************************************
 *  Dcm_ComM_FullComModeEntered()
 *********************************************************************************************************************/
/*! \brief          ComM callback for entering full communication mode.
 *  \details        This call informs the DCM module about a ComM mode change to COMM_FULL_COMMUNICATION.
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(void);

/* --- PDU Router API ---------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  Dcm_ProvideRxBuffer()
 *********************************************************************************************************************/
/*! \brief          Request reception data manager.
 *  \details        Called at least once upon reception by a lower layer TP to request a buffer, where the received
 *                  data will be stored.
 *                  When called multiple times, the DCM expects that the previously provided buffer has been filled up
 *                  completely.
 *  \param[in]      DcmRxPduId         The RxPduId on the current request transmission
 *  \param[in]      TpSduLength        The required data length from the lower layer
 *  \param[out]     PduInfoPtr         Pointer to the request buffer pointer and buffer size
 *  \return         BUFREQ_OK          Buffer has been successfully provided
 *  \return         BUFREQ_E_OVFL      No buffer provided, available buffer is too small
 *  \return         BUFREQ_E_NOT_OK    No buffer provided, request failed
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE for different PduIds
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_ProvideRxBuffer(PduIdType DcmRxPduId
                                                     ,PduLengthType TpSduLength
                                                     ,P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) *PduInfoPtr);

/**********************************************************************************************************************
 *  Dcm_RxIndication()
 *********************************************************************************************************************/
/*! \brief          Request reception data manager.
 *  \details        This is called by the PduR to indicate the completion of a reception.
 *  \param[in]      DcmRxPduId    The RxPduId on the current request transmission
 *  \param[in]      Result        The reception result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RxIndication(PduIdType DcmRxPduId
                                     ,Dcm_NetArEnvNotifResultType Result);

/**********************************************************************************************************************
 *  Dcm_ProvideTxBuffer()
 *********************************************************************************************************************/
/*! \brief          Dispatches the response data copy processing.
 *  \details        Called by a lower layer TP to request a buffer with data to be transmitted.
 *  \param[in]      DcmTxPduId         The TxPduId on the current response transmission
 *  \param[in]      Length             The required data length from the lower layer (shall be 0)
 *  \param[out]     PduInfoPtr         Pointer to the chunk data pointer and current chunk length
 *  \return         BUFREQ_OK          Buffer has been successfully provided
 *  \return         BUFREQ_E_NOT_OK    No buffer provided, request failed
 *  \return         BUFREQ_E_BUSY      There is temporarily not enough data to be transmitted Retry later
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE for different PduIds
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_ProvideTxBuffer(PduIdType DcmTxPduId
                                                     ,P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) *PduInfoPtr
                                                     ,PduLengthType Length);

/**********************************************************************************************************************
 *  Dcm_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Transmission end notification.
 *  \details        This is called by the PduR to confirm an end of transmission.
 *  \param[in]      DcmTxPduId    The TxPduId on the current response transmission
 *  \param[in]      Result        Transmission result
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TxConfirmation(PduIdType DcmTxPduId
                                       ,NotifResultType Result);
# endif /* (DCM_BSW_ENV_ASR_VERSION_3XX_ENABLED == STD_ON) */
# if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED == STD_ON) || \
     (DCM_BSW_ENV_ASR_VERSION_401_ENABLED == STD_ON)
/* --- Com Manager API --------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  Dcm_ComM_NoComModeEntered()
 *********************************************************************************************************************/
/*! \brief          ComM call-back for entering no communication mode.
 *  \details        This call informs the DCM module about a ComM mode change to COMM_NO_COMMUNICATION.
 *  \param[in]      networkId    The communication channel on which this state is now active
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(NetworkHandleType networkId);

/**********************************************************************************************************************
 *  Dcm_ComM_SilentComModeEntered()
 *********************************************************************************************************************/
/*! \brief          ComM callback for entering silent communication mode.
 *  \details        This call informs the DCM module about a ComM mode change to COMM_SILENT_COMMUNICATION.
 *  \param[in]      networkId    The communication channel on which this state is now active
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(NetworkHandleType networkId);

/**********************************************************************************************************************
 *  Dcm_ComM_FullComModeEntered()
 *********************************************************************************************************************/
/*! \brief          ComM callback for entering full communication mode.
 *  \details        This call informs the DCM module about a ComM mode change to COMM_FULL_COMMUNICATION.
 *  \param[in]      networkId    The communication channel on which this state is now active
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(NetworkHandleType networkId);

/* --- CANtp API --------------------------------------------------------------------------------------------------- */
#  if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnRequestDetection()
 *********************************************************************************************************************/
/*! \brief          Notification about a DCM related connection activation.
 *  \details        This function will be called by the transport layer for each DCM related connection activation
 *                  (ECU's own or external). In case that a foreign ECU is addressed any ongoing service processing
 *                  from the same connection will be canceled.
 *  \param[in]      canTpRxPduId       The CANtp RxPDUID of the request
 *  \param[in]      tpAddrExtension    The address extension used on this request
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if((DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)) && (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_OnRequestDetection(PduIdType canTpRxPduId
                                           ,uint8 tpAddrExtension);
#  endif

/* --- PDU Router API ---------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  Dcm_StartOfReception()
 *********************************************************************************************************************/
/*! \brief          Start of new request reception.
 *  \details        Called once to initialize the reception of a diagnostic request.
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[in]      tpSduLength        The total length of the TP message
 *  \param[out]     bufferSizePtr      Available buffer size
 *  \return         BUFREQ_OK          A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK    No free buffer is available - ignore request
 *  \return         BUFREQ_E_OVFL      The request message is too long for the configured buffer
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception(PduIdType rxPduId
#  if (DCM_PDUR_API_412_ENABLED == STD_ON)
                                                      ,P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
#  endif
                                                      ,PduLengthType tpSduLength
                                                      ,P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
 *  Dcm_CopyRxData()
 *********************************************************************************************************************/
/*! \brief          Copy Rx data of current frame
 *  \details        Called once upon reception of each segment. Within this call, the received data is copied from the
 *                  receive TP buffer to the DCM receive buffer.
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[out]     bufferSizePtr      Available buffer size
 *  \return         BUFREQ_OK          A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK    No free buffer is available - ignore request
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED == STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED == STD_ON)
 *  \pre            The API might only be called with an SduLength greater 0 if the RxBufferSizePtr returned by the
 *                  previous API call indicates sufficient receive buffer (SduLength <= RxBufferSizePtr).
 *                  The function must only be called if the connection has been accepted by an initial call to
 *                  Dcm_StartOfReception.
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData(PduIdType rxPduId
                                                ,P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
                                                ,P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
 *  Dcm_TpRxIndication()
 *********************************************************************************************************************/
/*! \brief          Indication of a new request reception accomplishment
 *  \details        This is called by the PduR to indicate the completion of a reception.
 *  \param[in]      rxPduId     The RxPDUID of the request
 *                  E_OK        The complete N-PDU has been received and is stored in the receive buffer
 *                  E_NOT_OK    The N_PDU has not been received properly, DCM should prepare for a new reception
 *  \param[in]      result      Request reception status
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TpRxIndication(PduIdType rxPduId
                                       ,Dcm_NetArEnvNotifResultType result);

/**********************************************************************************************************************
 *  Dcm_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copy Tx data for the current frame.
 *  \details        At invocation of Dcm_CopyTxData the DCM module copies the requested transmit data with ID PduId
 *                  from its internal transmit buffer to the location specified by the PduInfoPtr.
 *                  The function Dcm_CopyTxData also calculates and sets the TxDataCntPtr to the amount of remaining
 *                  bytes for the transmission of this data.
 *                  If RetryInfoPtr is NULL_PTR or if TpDataState is equal to TP_DATACONF, the DCM shall always copy
 *                  the next fragment of data to the SduDataPtr.
 *                  No TpDataState other than TP_DATACONF is supported by the current DCM implementation.
 *  \param[in]      txPduId             Identifies the DCM data to be sent. This information is used to derive the PCI
 *                                      information within the transport protocol. The value has to be same as in the
 *                                      according service call PduR_DcmTransmit().
 *  \param[in]      info                Pointer to a PduInfoType, which indicates the number of bytes to be copied
 *                                      (SduLength) and the location where the data have to be copied to (SduDataPtr).
 *                                      An SduLength of 0 is possible in order to poll the available transmit data
 *                                      count. In this case no data are to be copied and SduDataPtr might be invalid.
 *  \param[in]      retry               If the transmitted TP I-PDU does not support the retry feature a NULL_PTR
 *                                      can be provided. This indicates that the copied transmit data can be removed
 *                                      from the buffer after it has been copied.
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call.
 *  \return         BUFREQ_OK           A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK     No free buffer is available - ignore request
 *  \return         BUFREQ_E_BUSY       No free buffer at this time - try later again
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE for different PduIds
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData(PduIdType txPduId
                                                ,P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
                                                ,P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) retry
                                                ,P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr);

/**********************************************************************************************************************
 *  Dcm_TpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Confirmation of finished response transmission
 *  \details        This is called by the PduR to confirm an end of transmission.
 *  \param[in]      txPduId    The confirmation TxPDUID of the response
 *  \param[in]      result     Request reception status
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON)
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(PduIdType txPduId
                                         ,Dcm_NetArEnvNotifResultType result);

/**********************************************************************************************************************
 *  Dcm_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Confirmation for a specific TxPduId interface channel.
 *  \details        This is called by the PduR to confirm an end of transmission.
 *  \param[in]      txPduId    The confirmation TxPDUID of the response
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if ( (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED = STD_ON) ||
 *                  (DCM_BSW_ENV_ASR_VERSION_401_ENABLED = STD_ON) )
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TxConfirmation(PduIdType txPduId);
# endif /* (DCM_BSW_ENV_ASR_VERSION_4XX_ENABLED == STD_ON) || \
 (DCM_BSW_ENV_ASR_VERSION_401_ENABLED == STD_ON) */
# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)                                                                                                         /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_TriggerTransmit()
 *********************************************************************************************************************/
/*! \brief          Called by Fr and Lin interfaces in configuration specific cases.
 *  \details        -
 *  \param[in]      txPduId    The confirmation TxPDUID of the response
 *  \param[in]      info       Message context (data and length of the portion)
 *  \return         E_OK          All data were copied
 *  \return         E_NOT_OK      An error occurred, no data copied
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_NET_PERIODIC_TX_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerTransmit(PduIdType txPduId
                                                  ,P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info);
# endif
# define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h"                                                                                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#endif /* !defined(DCM_CORECBK_H) */
/* ********************************************************************************************************************
 * END OF FILE: Dcm_CoreCbk.h
 * ******************************************************************************************************************** */
