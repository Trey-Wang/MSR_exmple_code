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
 *         File:  J1939Tp_Direct.c
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Implementation of the direct sub-module of the SAE J1939 Transport Layer module.
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

/**********************************************************************************************************************
 *  MISRA / PClint JUSTIFICATIONS
 *********************************************************************************************************************/
/* *INDENT-OFF* */

/* PRQA S 0779 EOF */ /* MD_MSR_5.1_777 */

/* *INDENT-ON* */


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#define J1939TP_DIRECT_C

#include "J1939Tp_Cfg.h"

#if (J1939TP_DIRECT_ENABLED == STD_ON)
# include "J1939Tp.h"
# include "J1939Tp_Int.h"
# include "J1939Tp_Direct.h"

/*! \trace SPEC-2167286 */
# include "PduR_J1939Tp.h"
/*! \trace SPEC-2167287 */
# include "CanIf.h"

# include "SchM_J1939Tp.h"


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Vendor specific BSW module version information */
# define J1939TP_MAJOR_SOURCE_VERSION        (1u)
# define J1939TP_MINOR_SOURCE_VERSION        (3u)
# define J1939TP_PATCH_SOURCE_VERSION        (2u)

/* Check the version of J1939Tp module header file */
# if ((J1939TP_SW_MAJOR_VERSION != J1939TP_MAJOR_SOURCE_VERSION) || (J1939TP_SW_MINOR_VERSION != J1939TP_MINOR_SOURCE_VERSION) || (J1939TP_SW_PATCH_VERSION != J1939TP_PATCH_SOURCE_VERSION))
#  error "Vendor specific version numbers of J1939Tp_Direct.c and J1939Tp.h are inconsistent"
# endif


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_DirectInternalTransmitTpDirect(PduIdType sduIdx);
# endif

# if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_DirectInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                     J1939Tp_RxSduRxSduIdxOfRxPduRxSduType * allocatedSduIdx,
                                                     J1939Tp_RxNSduStateType ** allocatedRxSduState);
# endif

# if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_DirectInternalRxIndication_ReceiveTpDirect(J1939Tp_RxSduRxSduIdxOfRxPduRxSduType
                                                                             sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                             J1939TP_APPL_DATA) info,
                                                                             PduLengthType pduLen, uint8 sa, uint8 da);
# endif

# define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */


/**********************************************************************************************************************
 *  J1939Tp_DirectInternalTransmitTpDirect
 *********************************************************************************************************************/
/*! \brief      This function transmits a direct frame.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function is called from J1939Tp_MainFunction().
 *********************************************************************************************************************/

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_DirectInternalTransmitTpDirect(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx = J1939Tp_GetTxPduDirectNPduIdxOfTxSdu(sduIdx);
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_DIRECT;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same direct N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      BufReq_ReturnType tpResult;

      pdu.SduDataPtr = data;
      pdu.SduLength = (PduLengthType) txSduState->Size;

      /*! \trace SPEC-2167315, SPEC-2167317 */
      tpResult = PduR_J1939TpCopyTxData(J1939Tp_GetSduIdOfTxSdu(sduIdx), &pdu, NULL_PTR, &txSduState->DataSize);
      if (tpResult == BUFREQ_OK)
      {
        Std_ReturnType result;
        uint8 pos;
        uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);

        for (pos = (uint8) txSduState->Size; pos < J1939TP_FRAME_LENGTH; pos++)
        {
          data[pos] = J1939TP_SNA_VALUE; /*!< \trace SPEC-2167353 */
        }

        data[J1939TP_NPDU_SA_POS] = txSduState->Sender;

        if (J1939Tp_GetPduFormatOfTxSdu(sduIdx) == J1939TP_PDU1_PDUFORMATOFTXSDU)
        {
          data[J1939TP_NPDU_DA_POS] = txSduState->Receiver;
        }
        else
        {
          data[J1939TP_NPDU_GE_POS] = (uint8) J1939Tp_PGN_GetPDUS(pgn);
        }

        data[J1939TP_NPDU_PDUF_POS] = (uint8) J1939Tp_PGN_GetPDUF(pgn);

        data[J1939TP_NPDU_DP_POS] = (uint8) J1939Tp_PGN_GetDP(pgn) | txSduState->Priority;

        pdu.SduLength = J1939TP_NPDU_LENGTH; /*!< \trace SPEC-2167352 */

        /* no protection required - all exits from pending and busy state are in the scope of the calling functions */
        txSduState->State = J1939TP_STATE_DIRECT_CONF;
        result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
        if (result != E_OK)
        {
          SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if (txSduState->State == J1939TP_STATE_DIRECT_CONF)
          {
            /* no protection required - no further actions related to the locked state */
            pduState->Locked = FALSE;
            /* protected against late Tx confirmation */
            txSduState->State = J1939TP_STATE_FINISH;
            SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

            /*! \trace SPEC-2167303 */
            PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

            /* no protection required - transition from uninterruptible state */
            txSduState->State = J1939TP_STATE_IDLE;
          }
          else
          {
            SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          }
        }
      }
      else
      {
        /* no protection required - no further actions related to the locked state */
        pduState->Locked = FALSE;

        if (tpResult == BUFREQ_E_BUSY)
        {
          /*! \trace SPEC-2167316 */
          /* no protection required - all exits from pending and busy state are in the scope of the calling functions */
          txSduState->State = J1939TP_STATE_DIRECT;
        }
        else
        {
          /*! \trace SPEC-2167320 */
          /* no protection required - all exits from pending and busy state are in the scope of the calling functions */
          txSduState->State = J1939TP_STATE_FINISH;

          /*! \trace SPEC-2167303 */
          PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_IDLE;
        }
      }
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* no protection required - all exits from busy state are in the scope of the calling functions */
    txSduState->State = J1939TP_STATE_DIRECT;
  }
} /* J1939Tp_InternalTransmitTpDirect */
# endif


# if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_DirectInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                     J1939Tp_RxSduRxSduIdxOfRxPduRxSduType * allocatedSduIdx,
                                                     J1939Tp_RxNSduStateType ** allocatedRxSduState)
{
  J1939Tp_RxPduRxSduRxSduTableIndEndIdxOfRxPduType pduSduIdx;
  boolean allocated = FALSE;

  for (pduSduIdx = J1939Tp_GetRxPduRxSduRxSduTableIndStartIdxOfRxPdu(pduIdx);
       pduSduIdx < J1939Tp_GetRxPduRxSduRxSduTableIndEndIdxOfRxPdu(pduIdx); pduSduIdx++)
  {
    J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx = J1939Tp_GetRxSduRxSduIdxOfRxPduRxSdu(pduSduIdx);
    J1939Tp_RxChannelChannelIdxOfRxSduType rxChannelIdx = J1939Tp_GetRxChannelChannelIdxOfRxSdu(sduIdx);

#  if (J1939TP_RXSAOFRXCHANNEL == STD_ON)
    /*! \trace SPEC-2167348 */
    uint8 configuredSa = J1939Tp_GetRxSaOfRxChannel(rxChannelIdx);
    if ((configuredSa == J1939TP_NO_ADDRESS_CONFIGURED) || (configuredSa == sa))
#  endif
    {
#  if (J1939TP_RXDAOFRXCHANNEL == STD_ON)
      /*! \trace SPEC-2167348 */
      uint8 configuredDa = J1939Tp_GetRxDaOfRxChannel(rxChannelIdx);
      /* TODO: Check whether the following tests cannot be removed, because DA was already set to broadcast for PDU2 */
      if ((J1939Tp_GetPduFormatOfRxPdu(pduIdx) == J1939TP_PDU2_PDUFORMATOFRXPDU) ||
          ((J1939Tp_GetRxProtocolTypeOfRxChannel(rxChannelIdx) != J1939TP_PROTOCOL_CMDT_RXPROTOCOLTYPEOFRXCHANNEL) &&
#   if (J1939TP_EXISTS_PROTOCOL_ETP_RXPROTOCOLTYPEOFRXCHANNEL == STD_ON)
           (J1939Tp_GetRxProtocolTypeOfRxChannel(rxChannelIdx) != J1939TP_PROTOCOL_ETP_RXPROTOCOLTYPEOFRXCHANNEL) &&
#   endif
           (da == J1939TP_BROADCAST_ADDRESS)) ||
          ((J1939Tp_GetRxProtocolTypeOfRxChannel(rxChannelIdx) != J1939TP_PROTOCOL_BAM_RXPROTOCOLTYPEOFRXCHANNEL) &&
           ((configuredDa == J1939TP_NO_ADDRESS_CONFIGURED) || (configuredDa == da))))
#  endif
      {
        J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

        SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (rxSduState->State == J1939TP_STATE_IDLE)
        {
          /* protected against concurrent reception of the same direct N-PDU */ /* TODO: Check if this is necessary */
          rxSduState->State = J1939TP_STATE_DIRECT_BUSY;
          SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          allocated = TRUE;

          *allocatedSduIdx = sduIdx;
          *allocatedRxSduState = rxSduState;

          break; /* stop search for free N-SDU */
        }
        else
        {
          SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
      }
    }
  }

#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
  if (!allocated)
  {
    /* No free connection found for incoming direct frame */

    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_NO_CONNECTION);
  }
#  else
  J1939TP_DUMMY_STATEMENT(allocated); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#  endif

  return allocated;
}
# endif


/**********************************************************************************************************************
 *  J1939Tp_DirectInternalRxIndication_ReceiveTpDirect
 *********************************************************************************************************************/
/*! \brief      This function reports a direct frame to the upper layer.
 *
 *  \param[in]  sduIdx
 *  \param[in]  info
 *  \param[in]  pduLen
 *  \param[in]  sa
 *  \param[in]  da
 *  \context    The function is called from J1939Tp_MainFunction().
 *********************************************************************************************************************/

# if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_DirectInternalRxIndication_ReceiveTpDirect(J1939Tp_RxSduRxSduIdxOfRxPduRxSduType
                                                                             sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                             J1939TP_APPL_DATA) info,
                                                                             PduLengthType pduLen, uint8 sa, uint8 da)
{
  uint8 metaData[J1939TP_MDL_SIZE];
  BufReq_ReturnType tpResult;
  PduInfoType addrInfo;
  PduLengthType bufSize;

  addrInfo.SduLength = J1939Tp_GetMetaDataLengthOfRxSdu(sduIdx);
  addrInfo.SduDataPtr = metaData;

  metaData[J1939TP_MDL_SA_POS] = sa;
  metaData[J1939TP_MDL_DA_POS] = da;
  metaData[J1939TP_MDL_PDUF_POS] = J1939TP_SNA_VALUE;
  metaData[J1939TP_MDL_PRI_POS] = J1939Tp_MDL_Get_PRI(info->SduDataPtr[pduLen + J1939TP_MDL_PRI_POS]);

  tpResult = PduR_J1939TpStartOfReception(J1939Tp_GetSduIdOfRxSdu(sduIdx), &addrInfo, pduLen, &bufSize);
  if (tpResult == BUFREQ_OK)
  {
    Std_ReturnType result = E_NOT_OK;

    if (bufSize >= pduLen)
    {
      PduInfoType pdu;

      pdu.SduDataPtr = info->SduDataPtr;
      pdu.SduLength = pduLen;

      tpResult = PduR_J1939TpCopyRxData(J1939Tp_GetSduIdOfRxSdu(sduIdx), &pdu, &bufSize);
      if (tpResult == BUFREQ_OK)
      {
        result = E_OK;
      }
    }

    PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), result);
  }
}
# endif

# define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/


/**********************************************************************************************************************
 *  INTERNAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  J1939Tp_DirectTxMain
 *********************************************************************************************************************/
/*! \brief      Main function of the J1939Tp_DirectTxMain. It is called by J1939Tp_MainFunction.
 *
 *  \param[in]  PduIdType sduIdx
 *  \param[out] void
 *  \return     0 - error
 *  \return     1 - success, inOut is not NULL
 *  \context    Function could be called from interrupt level or from task level
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_DirectTxMain(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  if (txSduState->State == J1939TP_STATE_DIRECT)
  {
    if (txSduState->TimeOut == 0)
    {
      /* no protection required - all exits from pending state are in the scope of this function */
      txSduState->State = J1939TP_STATE_FINISH;

      J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, J1939TP_E_TIMEOUT_TR);

      /*! \trace SPEC-2167303 */
      PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      --txSduState->TimeOut;

      /*! \trace SPEC-2167316 */
      J1939Tp_DirectInternalTransmitTpDirect(sduIdx);
    }
  }
} /* J1939Tp_DirectTxMain */
# endif


/**********************************************************************************************************************
 *  J1939Tp_DirectTransmit
 *********************************************************************************************************************/
/*! \brief      This function creates the direct frame according to its structure and is called by J1939TpUp_Transmit.
 *
 *  \param[in]  PduIdType sduIdx,
 *  \param[in]  P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info
 *  \param[out] Std_ReturnType
 *  \return     0 - error
 *  \return     1 - success, inOut is not NULL
 *  \context    Function could be called from interrupt level or from task level
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_DirectTransmit(PduIdType sduIdx,
                                                          P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));
  Std_ReturnType result = E_NOT_OK;

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (txSduState->State == J1939TP_STATE_IDLE)
  {
    /* protected against a reentrant call for the same N-SDU */ /* TODO: Check if this is necessary */
    txSduState->State = J1939TP_STATE_DIRECT_BUSY;
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      J1939Tp_MetaDataLengthOfTxSduType mdl = J1939Tp_GetMetaDataLengthOfTxSdu(sduIdx);
      J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx = J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx);
      uint8 pri, da, sa;

      /*! \trace SPEC-2167312 */
      if (J1939Tp_GetTxSaOfTxChannel(txChannelIdx) == J1939TP_NO_ADDRESS_CONFIGURED)
      {
        sa = info->SduDataPtr[J1939TP_MDL_SA_POS];
      }
      else
      {
        sa = J1939Tp_GetTxSaOfTxChannel(txChannelIdx);
      }

      /*! \trace SPEC-2167312 */
      if ((J1939Tp_GetTxProtocolTypeOfTxChannel(txChannelIdx) == J1939TP_PROTOCOL_BAM_TXPROTOCOLTYPEOFTXCHANNEL) ||
          ((J1939Tp_GetTxProtocolTypeOfTxChannel(txChannelIdx) == J1939TP_PROTOCOL_UNUSED_TXPROTOCOLTYPEOFTXCHANNEL) &&
           (info->SduDataPtr[J1939TP_MDL_DA_POS] == J1939TP_BROADCAST_ADDRESS)))
      {
        da = J1939TP_BROADCAST_ADDRESS;
      }
      else if (J1939Tp_GetTxDaOfTxChannel(txChannelIdx) == J1939TP_NO_ADDRESS_CONFIGURED)
      {
        da = info->SduDataPtr[J1939TP_MDL_DA_POS];
      }
      else
      {
        da = J1939Tp_GetTxDaOfTxChannel(txChannelIdx);
      }
      
      if (mdl > J1939TP_MDL_PRI_POS)
      {
        pri = J1939Tp_MDL_Get_PRI(info->SduDataPtr[J1939TP_MDL_PRI_POS]);
      }
      else
      {
        pri = J1939TP_DEFAULT_PRIORITY;
      }

      txSduState->Size = info->SduLength;
      txSduState->Sender = sa;
      txSduState->Receiver = da;
      txSduState->Priority = pri;
      txSduState->TimeOut = J1939Tp_GetTime_Tr();

      J1939Tp_DirectInternalTransmitTpDirect(sduIdx);

      result = E_OK;
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  return result;
} /* J1939Tp_DirectTransmit */
# endif


/**********************************************************************************************************************
 *  J1939Tp_DirectTxConfirmation
 *********************************************************************************************************************/
/*! \brief      This function is called by the J1939Tp_TxConfirmation after successful
 *              transmission of a J1939Tp Tx N-PDU.
 *
 *  \param[in]  PduIdType sduIdx,
 *  \param[in]  J1939Tp_NPduType pduType
 *  \param[out] void
 *  \return     0 - error
 *  \return     1 - success, inOut is not NULL
 *  \context    Function could be called from interrupt level or from task level
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_DirectTxConfirmation(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (txSduState->State == J1939TP_STATE_DIRECT_CONF)
  {
    /* protected against concurrent transmission failure and early Tx confirmation timeout */
    txSduState->State = J1939TP_STATE_FINISH;
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /*! \trace SPEC-2167302 */
    PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_OK);

    txSduState->State = J1939TP_STATE_IDLE;
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* J1939Tp_DirectTxConfirmation */
# endif


/**********************************************************************************************************************
 *  J1939Tp_DirectRxIndication
 *********************************************************************************************************************/
/*! \brief      This function is called by the J1939Tp_RxIndication after successful reception of a J1939Tp Rx N-PDU.
 *
 *  \param[in]  PduIdType pduIdx,
 *  \param[in]  P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
 *  \param[in]  J1939Tp_NPduType pduType
 *  \param[out] void
 *  \return     0 - error
 *  \return     1 - success, inOut is not NULL
 *  \context    Function could be called from interrupt level or from task level
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_DirectRxIndication(PduIdType pduIdx,
                                                    P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info, uint8 sa)
{
  PduLengthType pduLen = info->SduLength - J1939TP_MDL_SIZE;
  uint8 da = info->SduDataPtr[pduLen + J1939TP_MDL_DA_POS];

  if (J1939Tp_GetPduFormatOfRxPdu(pduIdx) == J1939TP_PDU2_PDUFORMATOFRXPDU)
  {
    da = J1939TP_BROADCAST_ADDRESS;
  }

  if (!J1939Tp_DA_Valid(da))
  {
    /*! \trace SPEC-2167310 */
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_DA);
  }
  else
  {
    J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx;
    J1939Tp_RxNSduStateType *rxSduState;

    /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
    if (J1939Tp_DirectInternalRxIndication_AllocateRxChannel(pduIdx, sa, da, &sduIdx, &rxSduState))
    {
      J1939Tp_DirectInternalRxIndication_ReceiveTpDirect(sduIdx, info, pduLen, sa, da);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
  }
} /* J1939Tp_DirectRxIndication */
# endif


/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */


# define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/
#endif


/* Module specific MISRA deviations:

 MD_J1939Tp_13.2
      Reason:     No deviation: MISRA rule 13.2 explicitly allows to use Boolean checks when it is ensured that the tested variable is only used as a Boolean value.
      Risk:       None
      Prevention: N/A

 MD_J1939Tp_3199
      Reason:     Dummy statement, used to avoid compiler warnings.
      Risk:       None
      Prevention: N/A
*/
