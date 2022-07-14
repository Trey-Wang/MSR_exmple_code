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
 *         File:  J1939Tp_Bam.c
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Implementation of the BAM sub-module of the SAE J1939 Transport Layer module.
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

#define J1939TP_BAM_C

#include "J1939Tp_Cfg.h"

#if (J1939TP_BAM_ENABLED == STD_ON)
# include "J1939Tp.h"
# include "J1939Tp_Int.h"
# include "J1939Tp_Bam.h"

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
#  error "Vendor specific version numbers of J1939Tp_Bam.c and J1939Tp.h are inconsistent"
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalTransmitTpCmBam(PduIdType sduIdx);
# endif

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalTransmitTpDt(PduIdType sduIdx);
# endif

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_BamInternalTransmit_SearchConflictingTxChannel(PduIdType sduIdx,
                                                       J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx, uint8 sa);
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalRxIndication_AllocateRxChannel()
 *********************************************************************************************************************/
/*! \brief       Processes BAM reception
 *  \details     Processes the BAM reception of an N-PDU from the CanIf.
 *  \param[in]   pduIdx       Rx channel
 *  \param[in]   info         Pointer to J1939Tp N-PDU structure.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      J1939TP_BAM_RX_ENABLED
 *********************************************************************************************************************/
# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_BamInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint32 pgn,
                                                  J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * allocatedSduIdx,
                                                  J1939Tp_RxNSduStateType ** allocatedRxSduState);
# endif

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalRxIndication_ReceiveTpCmBam(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                         sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                         J1939TP_APPL_DATA) info,
                                                                         uint8 sa, uint16 size,
                                                                         J1939Tp_RxNSduStateType * rxSduState);
# endif

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_BamInternalRxIndication_FindActiveRxChannel(PduIdType pduIdx, uint8 sa,
                                                    J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * sduIdx,
                                                    J1939Tp_RxNSduStateType ** rxSduState);
# endif

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalRxIndication_ReceiveTpDt(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                      sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                      J1939TP_APPL_DATA) info,
                                                                      J1939Tp_RxNSduStateType * rxSduState);
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
 *  J1939Tp_BamInternalTransmitTpCmBam
 *********************************************************************************************************************/
/*! \brief      This functions transmits a TP.CM_BAM frame.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function is called from J1939Tp_MainFunction().
 *********************************************************************************************************************/

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalTransmitTpCmBam(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduTxCmNPduIdxOfTxChannel(J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_BAM;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same TP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));
      uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      Std_ReturnType result;

      data[J1939TP_TPCM_CB] = J1939TP_CB_BAM;
      data[J1939TP_TPCM_TMS_LO] = J1939Tp_Make_TMS_LO(txSduState->Size);
      data[J1939TP_TPCM_TMS_HI] = J1939Tp_Make_TMS_HI(txSduState->Size);
      data[J1939TP_TPCM_TNOP] = (uint8) txSduState->Packets;
      data[J1939TP_TPCM_RES5] = J1939TP_SNA_VALUE;
      data[J1939TP_TPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
      data[J1939TP_TPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
      data[J1939TP_TPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
      data[J1939TP_NPDU_SA_POS] = txSduState->Sender;
      data[J1939TP_NPDU_DA_POS] = J1939TP_BROADCAST_ADDRESS;
      data[J1939TP_NPDU_PRI_POS] = txSduState->Priority;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939TP_NPDU_LENGTH;

      /* no protection required - all exits from BAM state are in the scope of the Tx main function */
      txSduState->State = J1939TP_STATE_BAM_BAM_CONF;
      result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
      if (result != E_OK)
      {
        SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (txSduState->State == J1939TP_STATE_BAM_BAM_CONF)
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
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* J1939Tp_BamInternalTransmitTpCmBam */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalTransmitTpDt
 *********************************************************************************************************************/
/*! \brief      This function transmits a TP.DT frame
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function is called from J1939Tp_MainFunction().
 *********************************************************************************************************************/

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalTransmitTpDt(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduTxDtNPduIdxOfTxChannel(J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_BAM;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same TP.DT N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      BufReq_ReturnType tpResult;

      pdu.SduDataPtr = &data[J1939TP_TPDT_DATA];
      pdu.SduLength = J1939TP_TPDT_DATA_SIZE;

      if (txSduState->Sequence == txSduState->Packets)
      {
        pdu.SduLength = (PduLengthType) ((txSduState->Size) - (((txSduState->Sequence) - 1) * J1939TP_TPDT_DATA_SIZE));
      }

      /*! \trace SPEC-2167315, SPEC-2167318 */
      tpResult = PduR_J1939TpCopyTxData(J1939Tp_GetSduIdOfTxSdu(sduIdx), &pdu, NULL_PTR, &txSduState->DataSize);
      if (tpResult == BUFREQ_OK)
      {
        uint8 pos;
        Std_ReturnType result;

        data[J1939TP_TPDT_SEQ] = txSduState->Sequence;

        for (pos = J1939TP_TPDT_DATA + (uint8) pdu.SduLength; pos < J1939TP_FRAME_LENGTH; pos++)
        {
          data[pos] = J1939TP_SNA_VALUE; /*!< \trace SPEC-2167353 */
        }

        data[J1939TP_NPDU_SA_POS] = txSduState->Sender;
        data[J1939TP_NPDU_DA_POS] = J1939TP_BROADCAST_ADDRESS;
        data[J1939TP_NPDU_PRI_POS] = txSduState->Priority;

        pdu.SduDataPtr = data;
        pdu.SduLength = J1939TP_NPDU_LENGTH; /*!< \trace SPEC-2167352 */

        /* no protection required - all exits from data state are in the scope of the Tx main function */
        txSduState->State = J1939TP_STATE_BAM_DATA_CONF;
        result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
        if (result != E_OK)
        {
          SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if (txSduState->State == J1939TP_STATE_BAM_DATA_CONF)
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
        /*! \trace SPEC-2167316 */
        /* no protection required - no further actions related to the locked state */
        pduState->Locked = FALSE;

        if (tpResult != BUFREQ_E_BUSY)
        {
          /*! \trace SPEC-2167320 */
          /* no protection required - all exits from data state are in the scope of the Tx main function */
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
  }
} /* J1939Tp_BamInternalTransmitTpDt */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalTransmit_SearchConflictingTxChannel
 *********************************************************************************************************************/
/*! \brief      This function reports a direct frame to the upper layer.
 *
 *  \param[in]  sduIdx
 *  \param[in]  txChannelIdx
 *  \param[in]  sa
 *  \context    The function is called from J1939Tp_Transmit().
 *********************************************************************************************************************/

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_BamInternalTransmit_SearchConflictingTxChannel(PduIdType sduIdx,
                                                       J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx, uint8 sa)
{
  J1939Tp_ComMChannelOfTxChannelType comMChannel = J1939Tp_GetComMChannelOfTxChannel(txChannelIdx);
  J1939Tp_SizeOfTxChannelType otherTxChannelIdx;
  boolean found = FALSE;

  for (otherTxChannelIdx = 0; (otherTxChannelIdx < J1939Tp_GetSizeOfTxChannel()) && (!found); otherTxChannelIdx++)
  {
    if (J1939Tp_GetComMChannelOfTxChannel(otherTxChannelIdx) == comMChannel)
    {
      J1939Tp_TxChannelTxNSduTxNSduTableIndEndIdxOfTxChannelType sduIndIdx;

      for (sduIndIdx = J1939Tp_GetTxChannelTxNSduTxNSduTableIndStartIdxOfTxChannel(otherTxChannelIdx);
           sduIndIdx < J1939Tp_GetTxChannelTxNSduTxNSduTableIndEndIdxOfTxChannel(otherTxChannelIdx); sduIndIdx++)
      {
        J1939Tp_TxChannelTxNSduTxNSduTableIndType otherSduIdx =
          J1939Tp_GetTxSduTxNSduIdxOfTxChannelTxNSdu(J1939Tp_GetTxChannelTxNSduTxNSduTableInd(sduIndIdx));

        if (otherSduIdx != sduIdx)
        {
          J1939Tp_TxNSduStateType *otherTxSduState =
            &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(otherSduIdx));

          if (((otherTxSduState->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE) &&
              (otherTxSduState->Sender == sa))
          {
            found = TRUE;

            break;
          }
        }
      }
    }
  }

  return found;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalRxIndication_AllocateRxChannel
 *********************************************************************************************************************/
/*! \brief      This function reports a direct frame to the upper layer.
 *
 *  \param[in]  pduIdx
 *  \param[in]  sa
 *  \param[in]  pgn
 *  \param[out] allocatedSduIdx
 *  \param[out] allocatedRxSduState
 *  \context    The function is called from J1939Tp_MainFunction().
 *********************************************************************************************************************/

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_BamInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint32 pgn,
                                                  J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * allocatedSduIdx,
                                                  J1939Tp_RxNSduStateType ** allocatedRxSduState)
{
  J1939Tp_RxPduRxChannelRxChannelTableIndEndIdxOfRxPduType rxPduRxChannelIdx;
  boolean found = FALSE;
  boolean allocated = FALSE;

  /*
   * Search for all Rx channels that use the reported PDU. It is not necessary to search for all Rx channels with the
   * same ComM channel, because there can only be one Rx PDU per ComM channel (ensured by CanIf).
   */
  for (rxPduRxChannelIdx = J1939Tp_GetRxPduRxChannelRxChannelTableIndStartIdxOfRxPdu(pduIdx);
       rxPduRxChannelIdx < J1939Tp_GetRxPduRxChannelRxChannelTableIndEndIdxOfRxPdu(pduIdx); rxPduRxChannelIdx++)
  {
    J1939Tp_RxChannelRxChannelIdxOfRxPduRxChannelType rxChannelIdx =
      J1939Tp_GetRxChannelRxChannelIdxOfRxPduRxChannel(rxPduRxChannelIdx);

#  if (J1939TP_RXPROTOCOLTYPEOFRXCHANNEL == STD_ON)
    if (J1939Tp_GetRxProtocolTypeOfRxChannel(rxChannelIdx) != J1939TP_PROTOCOL_CMDT_RXPROTOCOLTYPEOFRXCHANNEL)
#  endif
    {
#  if (J1939TP_RXSAOFRXCHANNEL == STD_ON)
      /*! \trace SPEC-2167348 */
      uint8 configuredSa = J1939Tp_GetRxSaOfRxChannel(rxChannelIdx);
      if ((configuredSa == J1939TP_NO_ADDRESS_CONFIGURED) || (configuredSa == sa))
#  endif
      {
        J1939Tp_RxChannelRxNSduRxNSduTableIndEndIdxOfRxChannelType rxChannelSduIdx;

        for (rxChannelSduIdx = J1939Tp_GetRxChannelRxNSduRxNSduTableIndStartIdxOfRxChannel(rxChannelIdx);
             rxChannelSduIdx < J1939Tp_GetRxChannelRxNSduRxNSduTableIndEndIdxOfRxChannel(rxChannelIdx);
             rxChannelSduIdx++)
        {
          J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx =
            J1939Tp_GetRxSduRxNSduIdxOfRxChannelRxNSdu(rxChannelSduIdx);
          J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

          if (((rxSduState->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE) && (rxSduState->Sender == sa))
          {
            J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_UNTIMELY_BAM);

            SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            if (rxSduState->State == J1939TP_STATE_BAM_DATA)
            {
              /* protected against concurrent timeout and concurrect reception of TP.DT message */
              rxSduState->State = J1939TP_STATE_FINISH;
              SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              /*! \trace SPEC-2167305 */
              PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

              /* no protection required - transition from uninterruptible state */
              rxSduState->State = J1939TP_STATE_IDLE;
            }
            else
            {
              SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              rxSduState->Aborted = TRUE;
            }
          }

          /*! \trace SPEC-2167293, SPEC-2167348 */
          if ((!allocated) && (pgn == (uint32) J1939Tp_GetPGNOfRxSdu(sduIdx)))
          {
            found = TRUE;

            SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            if (rxSduState->State == J1939TP_STATE_IDLE)
            {
              rxSduState->Aborted = FALSE;
              /* protected against concurrent reception of TP.CM message */ /* TODO: Check if this is necessary */
              rxSduState->State = J1939TP_STATE_BAM_BAM_BUSY;
              SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              allocated = TRUE;

              *allocatedSduIdx = sduIdx;
              *allocatedRxSduState = rxSduState;
            }
            else
            {
              SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            }
          }
        }
      }
    }
  }

#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
  if (found && (!allocated))
  {
    /* No free connection found for incoming BAM connection */

    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_NO_CONNECTION);
  }
#  else
  J1939TP_DUMMY_STATEMENT(found); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#  endif

  return allocated;
} /* J1939Tp_BamInternalRxIndication_AllocateRxChannel */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalRxIndication_ReceiveTpCmBam
 *********************************************************************************************************************/
/*! \brief      This function reports a direct frame to the upper layer.
 *
 *  \param[in]  sduIdx
 *  \param[in]  info
 *  \param[in]  sa
 *  \param[in]  size
 *  \param[in]  rxSduState
 *  \context    The function is called from J1939Tp_MainFunction().
 *********************************************************************************************************************/

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalRxIndication_ReceiveTpCmBam(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                         sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                         J1939TP_APPL_DATA) info,
                                                                         uint8 sa, uint16 size,
                                                                         J1939Tp_RxNSduStateType * rxSduState)
{
  uint8_least packets = (uint8_least) J1939Tp_Get_Packets(size);
  boolean invalidSize;

#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
  if (packets != info->SduDataPtr[J1939TP_TPCM_TNOP])
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_TNOP);
  }
#  endif

#  if (J1939TP_ISVARIABLEOFRXSDU == STD_ON)
  if (J1939Tp_IsIsVariableOfRxSdu(sduIdx))
  {
    invalidSize = (boolean) (size > J1939Tp_GetPduLengthOfRxSdu(sduIdx));
  }
  else
  {
    invalidSize = (boolean) (size != J1939Tp_GetPduLengthOfRxSdu(sduIdx));
  }
#  else
  invalidSize = (boolean) (size != J1939Tp_GetPduLengthOfRxSdu(sduIdx));
#  endif

  if (invalidSize) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SIZE);

    /* no protection required - transition from uninterruptible state */
    rxSduState->State = J1939TP_STATE_IDLE;
  }
  else
  {
    PduInfoType addrInfo;
    BufReq_ReturnType tpResult;

    addrInfo.SduLength = J1939Tp_GetMetaDataLengthOfRxSdu(sduIdx);
    addrInfo.SduDataPtr = &(info->SduDataPtr[J1939TP_NPDU_MDL_POS]);

    tpResult = PduR_J1939TpStartOfReception(J1939Tp_GetSduIdOfRxSdu(sduIdx), &addrInfo, (PduLengthType) size,
                                            &(rxSduState->BufSize));
    if (tpResult == BUFREQ_OK)
    {
      rxSduState->Size = size;
      rxSduState->Packets = packets;
      rxSduState->Sender = sa;
      rxSduState->Receiver = J1939TP_BROADCAST_ADDRESS;
      rxSduState->Sequence = J1939TP_RX_SEQUENCE_INITIAL;

      rxSduState->TimeOut = J1939Tp_GetTime_T1(); /*!< \trace SPEC-2167345 */
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_BAM_DATA;
    }
    else
    {
      /*! \trace SPEC-2167310, SPEC-2167311 */
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
  }
}
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalRxIndication_FindActiveRxChannel
 *********************************************************************************************************************/
/*! \brief      This function reports a direct frame to the upper layer.
 *
 *  \param[in]  pduIdx
 *  \param[in]  sa
 *  \param[out] sduIdx
 *  \param[out] rxSduState
 *  \context    The function is called from J1939Tp_RxIndication().
 *  \trace      SPEC-2167348
 *********************************************************************************************************************/

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_BamInternalRxIndication_FindActiveRxChannel(PduIdType pduIdx, uint8 sa,
                                                    J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * sduIdx,
                                                    J1939Tp_RxNSduStateType ** rxSduState)
{
  J1939Tp_RxPduRxChannelRxChannelTableIndEndIdxOfRxPduType rxPduRxChannelIdx;
  boolean found = FALSE;

  for (rxPduRxChannelIdx = J1939Tp_GetRxPduRxChannelRxChannelTableIndStartIdxOfRxPdu(pduIdx);
       (rxPduRxChannelIdx < J1939Tp_GetRxPduRxChannelRxChannelTableIndEndIdxOfRxPdu(pduIdx)) && (!found);
       rxPduRxChannelIdx++)
  {
    J1939Tp_RxChannelRxChannelIdxOfRxPduRxChannelType rxChannelIdx =
      J1939Tp_GetRxChannelRxChannelIdxOfRxPduRxChannel(rxPduRxChannelIdx);
    J1939Tp_RxChannelRxNSduRxNSduTableIndEndIdxOfRxChannelType rxChannelSduIdx;

    for (rxChannelSduIdx = J1939Tp_GetRxChannelRxNSduRxNSduTableIndStartIdxOfRxChannel(rxChannelIdx);
         rxChannelSduIdx < J1939Tp_GetRxChannelRxNSduRxNSduTableIndEndIdxOfRxChannel(rxChannelIdx); rxChannelSduIdx++)
    {
      *sduIdx = J1939Tp_GetRxSduRxNSduIdxOfRxChannelRxNSdu(rxChannelSduIdx);
      *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(*sduIdx));

      if ((((*rxSduState)->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE) && ((*rxSduState)->Sender == sa))
      {
        found = TRUE;

        break;
      }
    }
  }

  return found;
}
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamInternalRxIndication_ReceiveTpDt
 *********************************************************************************************************************/
/*! \brief      This function reports a direct frame to the upper layer.
 *
 *  \param[in]  sduIdx
 *  \param[in]  info
 *  \param[in]  rxSduState
 *  \context    The function is called from J1939Tp_RxIndication().
 *********************************************************************************************************************/

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_BamInternalRxIndication_ReceiveTpDt(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                      sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                      J1939TP_APPL_DATA) info,
                                                                      J1939Tp_RxNSduStateType * rxSduState)
{
  SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (rxSduState->State == J1939TP_STATE_BAM_DATA)
  {
    /* protected against concurrent timeout and concurrent reception of TP.CM message */
    rxSduState->State = J1939TP_STATE_BAM_DATA_BUSY;
    SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (rxSduState->Aborted) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
    {
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_FINISH;

      /*! \trace SPEC-2167305 */
      PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      rxSduState->Sequence++;

      if (rxSduState->Sequence == info->SduDataPtr[J1939TP_TPDT_SEQ])
      {
        PduInfoType pdu;
        BufReq_ReturnType tpResult;

        pdu.SduDataPtr = &info->SduDataPtr[J1939TP_TPDT_DATA];
        pdu.SduLength = J1939TP_TPDT_DATA_SIZE;

        if (rxSduState->Sequence == rxSduState->Packets)
        {
          pdu.SduLength = (PduLengthType) ((rxSduState->Size) - ((rxSduState->Sequence - 1) * J1939TP_TPDT_DATA_SIZE));
        }

        tpResult = PduR_J1939TpCopyRxData(J1939Tp_GetSduIdOfRxSdu(sduIdx), &pdu, &rxSduState->BufSize);
        if (tpResult == BUFREQ_OK)
        {
          if (rxSduState->Sequence == rxSduState->Packets)
          {
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_FINISH;

            /*! \trace SPEC-2167304 */
            PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_OK);

            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_IDLE;
          }
          else
          {
            rxSduState->TimeOut = J1939Tp_GetTime_T1(); /*!< \trace SPEC-2167345 */
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_BAM_DATA;
          }
        }
        else
        {
          /*! \trace SPEC-2167311 */
          /* no protection required - transition from uninterruptible state */
          rxSduState->State = J1939TP_STATE_FINISH;

          /*! \trace SPEC-2167305 */
          PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

          /* no protection required - transition from uninterruptible state */
          rxSduState->State = J1939TP_STATE_IDLE;
        }
      }
      else
      {
        /*! \trace SPEC-2167347 */
        /* no protection required - transition from uninterruptible state */
        rxSduState->State = J1939TP_STATE_FINISH;

        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SN);

        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

        /* no protection required - transition from uninterruptible state */
        rxSduState->State = J1939TP_STATE_IDLE;
      }
    }
  }
  else
  {
    SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* TP.DT shall be ignored if received unexpectedly */
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/

/**********************************************************************************************************************
 *  INTERNAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  J1939Tp_BamTxMain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamTxMain(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  if (txSduState->TimeOut == 0)
  { 
    /*! \trace SPEC-2167345 */
    J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, J1939TP_E_TIMEOUT_TR);

    if ((txSduState->State == J1939TP_STATE_BAM_BAM) || (txSduState->State == J1939TP_STATE_BAM_DATA))
    {
      /* no protection required - all exits from BAM and data states are in the scope of this function */
      txSduState->State = J1939TP_STATE_FINISH;

      /*! \trace SPEC-2167303 */
      PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_IDLE;
    }
  }
  else
  {
    --txSduState->TimeOut;

    switch (txSduState->State)
    {
    case J1939TP_STATE_BAM_BAM:
      J1939Tp_BamInternalTransmitTpCmBam(sduIdx);
      break;

    case J1939TP_STATE_BAM_DATA:
      if (txSduState->TimeOut <= (J1939Tp_GetTime_Tr() - J1939Tp_GetTime_Bam())) /*!< \trace SPEC-2167345 */
      {
        /*! \trace SPEC-2167316 */
        J1939Tp_BamInternalTransmitTpDt(sduIdx);
      }
      break;

    default: /* COV_J1939TP_SWITCHCASE_DEFAULT */
      break;
    }
  }
} /* J1939Tp_BamTxMain */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamRxMain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */

# if (J1939TP_BAM_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamRxMain(PduIdType sduIdx)
{
  J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

  if (rxSduState->Aborted) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
  {
    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (rxSduState->State == J1939TP_STATE_BAM_DATA)
    {
      /* protected against concurrent reception of TP.DT message */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /*! \trace SPEC-2167305 */
      PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else if (rxSduState->TimeOut == 0)
  { 
    /*! \trace SPEC-2167345 */
    J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, J1939TP_E_TIMEOUT_T1);

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (rxSduState->State == J1939TP_STATE_BAM_DATA)
    {
      /* protected against concurrent reception of TP.DT message */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /*! \trace SPEC-2167305 */
      PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    --rxSduState->TimeOut;
  }
} /* J1939Tp_BamRxMain */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_BamTransmit(PduIdType sduIdx,
                                                       P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info)
{
  Std_ReturnType result = E_NOT_OK;

  if (info->SduLength > J1939TP_MAX_LENGTH) /* PRQA S 3356, 3359 */ /* MD_J1939Tp_13.7, MD_J1939Tp_13.7 */
  { /* PRQA S 3201 */ /* MD_J1939Tp_14.1_3201 */
    /*! \trace SPEC-2167289 */
    J1939Tp_ReportDevError(J1939TP_SID_TRANSMIT, J1939TP_E_INVALID_LENGTH);
  }
  else
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (txSduState->State == J1939TP_STATE_IDLE)
    {
      /* protected against a reentrant call for the same N-SDU */ /* TODO: Check if this is necessary */
      txSduState->State = J1939TP_STATE_BAM_BAM_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      {
        J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx = J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx);
        uint8 sa;
        boolean found;

        /*! \trace SPEC-2167312 */
        if (J1939Tp_GetTxSaOfTxChannel(txChannelIdx) == J1939TP_NO_ADDRESS_CONFIGURED)
        {
          sa = info->SduDataPtr[J1939TP_MDL_SA_POS];
        }
        else
        {
          sa = J1939Tp_GetTxSaOfTxChannel(txChannelIdx);
        }

        found = J1939Tp_BamInternalTransmit_SearchConflictingTxChannel(sduIdx, txChannelIdx, sa);

        /*! \trace SPEC-2167293 */
        if (!found)
        {
          J1939Tp_MetaDataLengthOfTxSduType mdl = J1939Tp_GetMetaDataLengthOfTxSdu(sduIdx);
          uint8 pri;

          if (mdl > J1939TP_MDL_PRI_POS)
          {
            pri = J1939Tp_MDL_Get_PRI(info->SduDataPtr[J1939TP_MDL_PRI_POS]);
          }
          else
          {
            pri = J1939TP_DEFAULT_PRIORITY;
          }

          txSduState->Size = info->SduLength;
          txSduState->Packets = J1939Tp_Get_Packets(info->SduLength);
          txSduState->Sender = sa;
          txSduState->Receiver = J1939TP_BROADCAST_ADDRESS;
          txSduState->Priority = pri;
          txSduState->Sequence = J1939TP_TX_SEQUENCE_INITIAL;
          txSduState->TimeOut = J1939Tp_GetTime_Tr(); /*!< \trace SPEC-2167345 */
          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_BAM_BAM;

          result = E_OK;
        }
        else
        {
          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_IDLE;
        }
      }
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  return result;
} /* End of J1939Tp_BamTransmit */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */

# if (J1939TP_BAM_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamTxConfirmation(PduIdType sduIdx, J1939Tp_PduTypeOfTxPduType pduType)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  if (pduType == J1939TP_TPCM_PDUTYPEOFTXPDU)
  {
    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (txSduState->State == J1939TP_STATE_BAM_BAM_CONF)
    {
      /* advance state to send first TP.DT frame */
      txSduState->TimeOut = J1939Tp_GetTime_Tr(); /*!< \trace SPEC-2167345 */
      /* protected against concurrent transmission failure and early Tx confirmation timeout */
      txSduState->State = J1939TP_STATE_BAM_DATA;
    }
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else /* pduType == J1939TP_TPDT_PDUTYPEOFTXPDU */
  {
    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (txSduState->State == J1939TP_STATE_BAM_DATA_CONF)
    {
      if (txSduState->Sequence == txSduState->Packets)
      {
        /* protected against concurrent transmission failure and early Tx confirmation timeout */
        txSduState->State = J1939TP_STATE_FINISH;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /*! \trace SPEC-2167302 */
        PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_OK);

        /* no protection required - transition from uninterruptible state */
        txSduState->State = J1939TP_STATE_IDLE;
      }
      else
      {
        /* advance state to send next TP.DT frame */
        txSduState->Sequence++;
        txSduState->TimeOut = J1939Tp_GetTime_Tr(); /*!< \trace SPEC-2167345 */
        /* protected against concurrent transmission failure and early Tx confirmation timeout */
        txSduState->State = J1939TP_STATE_BAM_DATA;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
} /* J1939Tp_BamTxConfirmation */
# endif

/**********************************************************************************************************************
 *  J1939Tp_BamRxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 */
# if (J1939TP_BAM_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamRxIndication(PduIdType pduIdx,
                                                 P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
                                                 J1939Tp_PduTypeOfRxPduType pduType, uint8 sa, uint8 da)
{
#  if (J1939TP_CANIF_DLCCHECK == STD_OFF)
  if (info->SduLength == J1939TP_NPDU_LENGTH)
#  endif
  {
    if (da != J1939TP_BROADCAST_ADDRESS)
    {
      /*! \trace SPEC-2167310 */
#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
      if (info->SduDataPtr[J1939TP_TPCM_CB] == J1939TP_CB_BAM)
      {
        /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_CONTROL_BYTE);
      }
#  endif
    }
    else
    {
      if (pduType == J1939TP_TPCM_PDUTYPEOFRXPDU)
      {
        if (info->SduDataPtr[J1939TP_TPCM_CB] == J1939TP_CB_BAM)
        {
          uint32 pgn = J1939Tp_Get_PGN(info->SduDataPtr[J1939TP_TPCM_PGN_LO], info->SduDataPtr[J1939TP_TPCM_PGN_MD],
                                       info->SduDataPtr[J1939TP_TPCM_PGN_HI]);

          if (!J1939Tp_PGN_Valid(pgn))
          {
            /*! \trace SPEC-2167310 */
            J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_PGN);
          }
          else
          {
            uint16 size = J1939Tp_Get_TMS(info->SduDataPtr[J1939TP_TPCM_TMS_LO], info->SduDataPtr[J1939TP_TPCM_TMS_HI]);

            if (size > J1939TP_MAX_LENGTH)
            {
              J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_TMS);
            }
            else
            {
              J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
              J1939Tp_RxNSduStateType *rxSduState;

              /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
              if (J1939Tp_BamInternalRxIndication_AllocateRxChannel(pduIdx, sa, pgn, &sduIdx, &rxSduState))
              {
                J1939Tp_BamInternalRxIndication_ReceiveTpCmBam(sduIdx, info, sa, size, rxSduState);
              }
            }
          }
        }
        else
        {
          J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_CONTROL_BYTE);
        }
      }
      else /* if (pduType == J1939TP_TPDT_PDUTYPEOFRXPDU) */
      {
        J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
        J1939Tp_RxNSduStateType *rxSduState;

        /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
        if (J1939Tp_BamInternalRxIndication_FindActiveRxChannel(pduIdx, sa, &sduIdx, &rxSduState))
        {
          J1939Tp_BamInternalRxIndication_ReceiveTpDt(sduIdx, info, rxSduState);
        }
      }
    }
  }
#  if ((J1939TP_CANIF_DLCCHECK == STD_OFF) && (J1939TP_RUNTIME_ERROR_REPORT == STD_ON))
  else
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_DLC);
  }
#  endif
} /* J1939Tp_BamRxIndication */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

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

 MD_J1939Tp_13.7
      Reason:     Depending on the configured PduLengthType, the check is always false.
      Risk:       When PduLengthType is uint8, depending on compiler optimizations dead code might be included.
      Prevention: Set PduLengthType to uin16 or uint32, or enable appropriate compiler optimizations.

 MD_J1939Tp_14.1_3201
      Reason:     Depending on the configured PduLengthType, this code is never executed.
      Risk:       When PduLengthType is uint8, depending on compiler optimizations dead code might be included.
      Prevention: Set PduLengthType to uin16 or uint32, or enable appropriate compiler optimizations.

 MD_J1939Tp_3199
      Reason:     Dummy statement, used to avoid compiler warnings.
      Risk:       None
      Prevention: N/A
*/

/* COV_JUSTIFICATION_BEGIN

\ID COV_J1939TP_SWITCHCASE_DEFAULT
  \ACCEPT X
  \REASON Switch case has a default part for MISRA checks which is not reachable.

COV_JUSTIFICATION_END */
