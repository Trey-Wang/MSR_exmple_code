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
 *         File:  J1939Tp_Fpp.c
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Implementation of the NMEA 2000 fast packet sub-module of the SAE J1939 Transport Layer module.
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

#define J1939TP_FPP_C

#include "J1939Tp_Cfg.h"

#if (J1939TP_FPP_ENABLED == STD_ON)
# include "J1939Tp.h"
# include "J1939Tp_Int.h"
# include "J1939Tp_Fpp.h"

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
#  error "Vendor specific version numbers of J1939Tp_Fpp.c and J1939Tp.h are inconsistent"
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalTransmitFf(PduIdType sduIdx);
# endif

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalTransmitAf(PduIdType sduIdx);
# endif

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_FppInternalTransmit_SearchConflictingTxChannel(PduIdType sduIdx,
                                                       J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx, uint8 sa,
                                                       uint8 da);
# endif

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_FppInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                  J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * allocatedSduIdx,
                                                  J1939Tp_RxNSduStateType ** allocatedRxSduState);
# endif

/**********************************************************************************************************************
*  J1939Tp_FppInternalRxIndication_ReceiveFf()
*********************************************************************************************************************/
/*! \brief       Example of a global function
*  \details     Example of a global function. The description of this function is always visible in
*               the doxygen documentation.
*  \param[in]   Channel                 Input parameter, must be in range of <?>.
*  \param[out]  Value                   Output parameter reference, must not be NULL_PTR.
*  \return      E_NOT_OK - function has been called with invalid parameters
*  \return      E_OK - success
*  \pre         Module is initialized.
*  \context     TASK
*  \reentrant   FALSE
*  \synchronous TRUE
*  \note        Requirement Specification example function.
*  \trace       SPEC-123, SPEC-456
*  \trace       CREQ-123
*********************************************************************************************************************/
# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalRxIndication_ReceiveFf(J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx,
                                                                    P2CONST(PduInfoType, AUTOMATIC,
                                                                            J1939TP_APPL_DATA) info,
                                                                    PduLengthType pduLen, uint8 sa, uint8 da,
                                                                    PduLengthType size,
                                                                    J1939Tp_RxNSduStateType * rxSduState);
# endif

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_FppInternalRxIndication_FindActiveRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                    J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * sduIdx,
                                                    J1939Tp_RxNSduStateType ** rxSduState);
# endif

/**********************************************************************************************************************
*  J1939Tp_FppInternalRxIndication_ReceiveAf()
*********************************************************************************************************************/
/*! \brief       Example of a global function
*  \details     Example of a global function. The description of this function is always visible in
*               the doxygen documentation.
*  \param[in]   Channel                 Input parameter, must be in range of <?>.
*  \param[out]  Value                   Output parameter reference, must not be NULL_PTR.
*  \return      E_NOT_OK - function has been called with invalid parameters
*  \return      E_OK - success
*  \pre         Module is initialized.
*  \context     TASK
*  \reentrant   FALSE
*  \synchronous TRUE
*  \note        Requirement Specification example function.
*  \trace       SPEC-123, SPEC-456
*  \trace       CREQ-123
*********************************************************************************************************************/
# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalRxIndication_ReceiveAf(J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx,
                                                                    P2CONST(PduInfoType, AUTOMATIC,
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
 *  J1939Tp_FppInternalTransmitFf
 *********************************************************************************************************************/
/*! \brief      This functions sends the first frame of a fast packet message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalTransmitFf(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx = J1939Tp_GetTxPduDirectNPduIdxOfTxSdu(sduIdx);
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_FPP;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      BufReq_ReturnType tpResult;

      /* Let SduDataPtr point to the start of payload area */
      pdu.SduDataPtr = &data[J1939TP_FF_DATA];
      /* Set the size of the payload area of an FF */
      pdu.SduLength = J1939TP_FF_DATA_SIZE;

      /* single frame? */
      if (txSduState->Packets == J1939TP_FPP_FC_FF)
      {
        pdu.SduLength = (PduLengthType) txSduState->Size;
      }

      /*! \trace SPEC-2167315, SPEC-2167317 */
      tpResult = PduR_J1939TpCopyTxData(J1939Tp_GetSduIdOfTxSdu(sduIdx), &pdu, NULL_PTR, &txSduState->DataSize);
      if (tpResult == BUFREQ_OK)
      {
        uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);
        uint8 pos;
        Std_ReturnType result;

        /* First two bytes are used for the sequence counter and the amount of bytes of the NSdu to be transmitted */
        data[J1939TP_FF_SC] = J1939Tp_Make_SC(txSduState->NextBlock);
        data[J1939TP_FF_TNDB] = (uint8) txSduState->Size;

        /* fill up unused bytes with invalid value */
        for (pos = J1939TP_FF_DATA + (uint8) pdu.SduLength; pos < J1939TP_FRAME_LENGTH; pos++)
        {
          data[pos] = J1939TP_SNA_VALUE; /*!< \trace SPEC-2167353 */
        }

        /* MetaData */
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

        pdu.SduDataPtr = data;
        pdu.SduLength = J1939TP_NPDU_LENGTH; /*!< \trace SPEC-2167352 */

        /* no protection required - transition from uninterruptible state */
        txSduState->State = J1939TP_STATE_FPP_FF_CONF;
        result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
        if (result != E_OK)
        {
          SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if (txSduState->State == J1939TP_STATE_FPP_FF_CONF)
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
      else /* tpResult != BUFREQ_OK */
      {
        /* no protection required - no further actions related to the locked state */
        pduState->Locked = FALSE;

        if (tpResult == BUFREQ_E_BUSY)
        {
          /*! \trace SPEC-2167316 */
          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_FPP_FF;
        }
        else
        {
          /*! \trace SPEC-2167320 */
          /* no protection required - transition from uninterruptible state */
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

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_FPP_FF;
  }
} /* J1939Tp_FppInternalTransmitFf */
# endif

/**********************************************************************************************************************
 *  J1939Tp_FppInternalTransmitAf
 *********************************************************************************************************************/
/*! \brief      This functions sends a followup frame of a fast packet message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalTransmitAf(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx = J1939Tp_GetTxPduDirectNPduIdxOfTxSdu(sduIdx);
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_FPP;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      BufReq_ReturnType tpResult;

      /* Let SduDataPtr point to the start of payload area */
      pdu.SduDataPtr = &data[J1939TP_AF_DATA];
      /* Set the size of the payload area of an AF */
      pdu.SduLength = J1939TP_AF_DATA_SIZE;

      /* last frame? */
      if (txSduState->Sequence == txSduState->Packets)
      {
        pdu.SduLength = (PduLengthType) (txSduState->Size -
                                         (((txSduState->Sequence - 1) * J1939TP_AF_DATA_SIZE) + J1939TP_FF_DATA_SIZE));
      }

      /*! \trace SPEC-2167315, SPEC-2167317 */
      tpResult = PduR_J1939TpCopyTxData(J1939Tp_GetSduIdOfTxSdu(sduIdx), &pdu, NULL_PTR, &txSduState->DataSize);
      if (tpResult == BUFREQ_OK)
      {
        uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);
        uint8 pos;
        Std_ReturnType result;

        /* First byte is used for the sequence counter and the frame counter */
        data[J1939TP_AF_SCFC] = J1939Tp_Make_SCFC(txSduState->NextBlock, txSduState->Sequence);

        /* fill up unused bytes with invalid value */
        for (pos = J1939TP_AF_DATA + (uint8) pdu.SduLength; pos < J1939TP_FRAME_LENGTH; pos++)
        {
          data[pos] = J1939TP_SNA_VALUE; /*!< \trace SPEC-2167353 */
        }

        /* MetaData */
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

        pdu.SduDataPtr = data;
        pdu.SduLength = J1939TP_NPDU_LENGTH; /*!< \trace SPEC-2167352 */

        /* no protection required - transition from uninterruptible state */
        txSduState->State = J1939TP_STATE_FPP_AF_CONF;
        result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
        if (result != E_OK)
        {
          SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if (txSduState->State == J1939TP_STATE_FPP_AF_CONF)
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
      else /* tpResult != BUFREQ_OK */
      {
        /* no protection required - no further actions related to the locked state */
        pduState->Locked = FALSE;

        if (tpResult == BUFREQ_E_BUSY)
        {
          /*! \trace SPEC-2167316 */
          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_FPP_AF;
        }
        else
        {
          /*! \trace SPEC-2167320 */
          /* no protection required - transition from uninterruptible state */
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

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_FPP_AF;
  }
} /* J1939Tp_FppInternalTransmitAf */
# endif

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_FppInternalTransmit_SearchConflictingTxChannel(PduIdType sduIdx,
                                                       J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx, uint8 sa,
                                                       uint8 da)
{
  J1939Tp_ComMChannelOfTxChannelType comMChannel = J1939Tp_GetComMChannelOfTxChannel(txChannelIdx);
  J1939Tp_PGNOfTxSduType pgn = J1939Tp_GetPGNOfTxSdu(sduIdx);
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
        J1939Tp_PGNOfTxSduType otherPgn = J1939Tp_GetPGNOfTxSdu(otherSduIdx);

        if (otherSduIdx != sduIdx)
        {
          J1939Tp_TxNSduStateType *otherTxSduState =
            &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(otherSduIdx));

          if (((otherTxSduState->State & J1939TP_STATEMASK_FPP) != J1939TP_STATE_IDLE) &&
              (otherTxSduState->Sender == sa) && (otherTxSduState->Receiver == da) && (otherPgn == pgn))
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
* J1939Tp_FppInternalRxIndication_AllocateRxChannel()
*********************************************************************************************************************/
# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_FppInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                  J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * allocatedSduIdx,
                                                  J1939Tp_RxNSduStateType ** allocatedRxSduState)
{
  J1939Tp_RxPduRxSduRxSduTableIndEndIdxOfRxPduType pduSduIdx;
  boolean found = FALSE;
  boolean allocated = FALSE;

  for (pduSduIdx = J1939Tp_GetRxPduRxSduRxSduTableIndStartIdxOfRxPdu(pduIdx);
       pduSduIdx < J1939Tp_GetRxPduRxSduRxSduTableIndEndIdxOfRxPdu(pduIdx); pduSduIdx++)
  {
    J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx = J1939Tp_GetRxSduRxSduIdxOfRxPduRxSdu(pduSduIdx);
    J1939Tp_RxChannelChannelIdxOfRxSduType rxChannelIdx = J1939Tp_GetRxChannelChannelIdxOfRxSdu(sduIdx);

    if (J1939Tp_GetRxProtocolTypeOfRxChannel(rxChannelIdx) == J1939TP_PROTOCOL_FPP_RXPROTOCOLTYPEOFRXCHANNEL)
    {
#  if (J1939TP_RXSAOFRXCHANNEL == STD_ON)
      /*! \trace SPEC-2167348 */
      uint8 configuredSa = J1939Tp_GetRxSaOfRxChannel(rxChannelIdx);
      if ((configuredSa == J1939TP_NO_ADDRESS_CONFIGURED) || (configuredSa == sa))
#  endif
      {
#  if (J1939TP_RXDAOFRXCHANNEL == STD_ON)
        /*! \trace SPEC-2167348 */
        uint8 configuredDa = J1939Tp_GetRxDaOfRxChannel(rxChannelIdx);
        if ((configuredDa == J1939TP_NO_ADDRESS_CONFIGURED) || (configuredDa == da))
#  endif
        {
          J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

          if ((rxSduState->State != J1939TP_STATE_IDLE) && (rxSduState->Sender == sa) && (rxSduState->Receiver == da))
          {
            J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_UNTIMELY_FF);

            SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            if (rxSduState->State == J1939TP_STATE_FPP_AF)
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

          /*! \trace SPEC-2167293 */
          if (!allocated)
          {
            found = TRUE;

            SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            if (rxSduState->State == J1939TP_STATE_IDLE)
            {
              rxSduState->Aborted = FALSE;
              /* protected against concurrent reception of the same FPP N-PDU */ /* TODO: Check if this is necessary */
              rxSduState->State = J1939TP_STATE_FPP_FF_BUSY;
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
    /* No free connection found for incoming FPP connection */

    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_NO_CONNECTION);
  }
#  else
  J1939TP_DUMMY_STATEMENT(found); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#  endif

  return allocated;
} /* J1939Tp_FppInternalRxIndication_AllocateRxChannel() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

/**********************************************************************************************************************
* J1939Tp_FppInternalRxIndication_ReceiveFf()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
*/

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalRxIndication_ReceiveFf(J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx,
                                                                    P2CONST(PduInfoType, AUTOMATIC,
                                                                            J1939TP_APPL_DATA) info,
                                                                    PduLengthType pduLen, uint8 sa, uint8 da,
                                                                    PduLengthType size,
                                                                    J1939Tp_RxNSduStateType * rxSduState)
{
  boolean invalidSize;

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
    if (rxSduState->NextBlock != (uint8) J1939Tp_Get_SC(info->SduDataPtr[J1939TP_FF_SC]))
    {
      uint8 metaData[J1939TP_MDL_SIZE];
      PduInfoType addrInfo;
      BufReq_ReturnType tpResult;

      metaData[J1939TP_MDL_SA_POS] = sa;
      metaData[J1939TP_MDL_DA_POS] = da;
      metaData[J1939TP_MDL_PDUF_POS] = J1939TP_SNA_VALUE;
      metaData[J1939TP_MDL_PRI_POS] = J1939Tp_MDL_Get_PRI(info->SduDataPtr[pduLen + J1939TP_MDL_PRI_POS]);

      addrInfo.SduLength = J1939Tp_GetMetaDataLengthOfRxSdu(sduIdx);
      addrInfo.SduDataPtr = metaData;

      tpResult = PduR_J1939TpStartOfReception(J1939Tp_GetSduIdOfRxSdu(sduIdx), &addrInfo, size, &rxSduState->BufSize);
      if (tpResult == BUFREQ_OK)
      {
        uint8_least afCount = (uint8_least) J1939Tp_Get_Fpp_Packets(size);
        PduInfoType pdu;

        pdu.SduDataPtr = &info->SduDataPtr[J1939TP_FF_DATA];
        pdu.SduLength = (uint8) J1939TP_FF_DATA_SIZE;

        /* single packet? */
        if (afCount == J1939TP_FPP_FC_FF)
        {
          pdu.SduLength = (PduLengthType) size;
        }

        tpResult = PduR_J1939TpCopyRxData(J1939Tp_GetSduIdOfRxSdu(sduIdx), &pdu, &rxSduState->BufSize);
        if (tpResult == BUFREQ_OK)
        {
          /* single packet? */
          if (afCount == J1939TP_FPP_FC_FF)
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
            rxSduState->Size = size;
            rxSduState->Packets = afCount;
            rxSduState->Sender = sa;
            rxSduState->Receiver = da;
            rxSduState->Sequence = J1939TP_FPP_FC_FF;
            rxSduState->NextBlock = (uint8) J1939Tp_Get_SC(info->SduDataPtr[J1939TP_FF_SC]);

            rxSduState->TimeOut = J1939Tp_GetTime_T1();
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_FPP_AF;
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
        /*! \trace SPEC-2167310, SPEC-2167311 */
        /* no protection required - transition from uninterruptible state */
        rxSduState->State = J1939TP_STATE_IDLE;
      }
    }
    else
    {
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_FINISH;

      J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SC);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
  }
} /* J1939Tp_FppInternalRxIndication_ReceiveFf */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
# endif

/**********************************************************************************************************************
* J1939Tp_FppInternalRxIndication_FindActiveRxChannel()
*********************************************************************************************************************/
# if (J1939TP_FPP_RX_ENABLED == STD_ON)
/*! \trace SPEC-2167348 */
J1939TP_LOCAL_INLINE boolean
J1939Tp_FppInternalRxIndication_FindActiveRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                    J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * sduIdx,
                                                    J1939Tp_RxNSduStateType ** rxSduState)
{
  J1939Tp_RxPduRxSduRxSduTableIndEndIdxOfRxPduType pduSduIdx;
  boolean found = FALSE;

  for (pduSduIdx = J1939Tp_GetRxPduRxSduRxSduTableIndStartIdxOfRxPdu(pduIdx);
       pduSduIdx < J1939Tp_GetRxPduRxSduRxSduTableIndEndIdxOfRxPdu(pduIdx); pduSduIdx++)
  {
    *sduIdx = J1939Tp_GetRxSduRxSduIdxOfRxPduRxSdu(pduSduIdx);
    *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(*sduIdx));

    if ((((*rxSduState)->State & J1939TP_STATEMASK_FPP) != J1939TP_STATE_IDLE) && ((*rxSduState)->Sender == sa) &&
        ((*rxSduState)->Receiver == da))
    {
      found = TRUE;

      break; /* stop search for free N-SDU */
    }
  }

  return found;
} /* J1939Tp_FppInternalRxIndication_FindActiveRxChannel */
# endif

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_FppInternalRxIndication_ReceiveAf(J1939Tp_RxSduRxSduIdxOfRxPduRxSduType sduIdx,
                                                                    P2CONST(PduInfoType, AUTOMATIC,
                                                                            J1939TP_APPL_DATA) info,
                                                                    J1939Tp_RxNSduStateType * rxSduState)
{
  SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (rxSduState->State == J1939TP_STATE_FPP_AF)
  {
    /* protected against concurrent timeout */
    rxSduState->State = J1939TP_STATE_FPP_AF_BUSY;
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

      if (J1939Tp_Get_SC(info->SduDataPtr[J1939TP_AF_SCFC]) == rxSduState->NextBlock)
      {
        if (J1939Tp_Get_FC(info->SduDataPtr[J1939TP_AF_SCFC]) == rxSduState->Sequence)
        {
          PduInfoType pdu;
          BufReq_ReturnType tpResult;

          pdu.SduDataPtr = &info->SduDataPtr[J1939TP_AF_DATA];
          pdu.SduLength = J1939TP_AF_DATA_SIZE;

          /* last packet? */
          if (rxSduState->Sequence == rxSduState->Packets)
          {
            pdu.SduLength = (PduLengthType) (rxSduState->Size -
                                             (((rxSduState->Sequence - 1) * J1939TP_AF_DATA_SIZE) +
                                              J1939TP_FF_DATA_SIZE));
          }

          tpResult = PduR_J1939TpCopyRxData(J1939Tp_GetSduIdOfRxSdu(sduIdx), &pdu, &rxSduState->BufSize);
          if (tpResult == BUFREQ_OK)
          {
            /* last packet? */
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
              rxSduState->TimeOut = J1939Tp_GetTime_T1();
              /* no protection required - transition from uninterruptible state */
              rxSduState->State = J1939TP_STATE_FPP_AF;
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
          /* no protection required - transition from uninterruptible state */
          rxSduState->State = J1939TP_STATE_FINISH;

          J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_FC);

          /*! \trace SPEC-2167305 */
          PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

          /* no protection required - transition from uninterruptible state */
          rxSduState->State = J1939TP_STATE_IDLE;
        }
      }
      else
      {
        /* no protection required - transition from uninterruptible state */
        rxSduState->State = J1939TP_STATE_FINISH;

        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SC);

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

    /* AF shall be ignored if received unexpectedly */
  }
} /* J1939Tp_FppInternalRxIndication_ReceiveAf */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
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
 *  J1939Tp_FppTxMain()
 *********************************************************************************************************************/

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_FppTxMain(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  if (txSduState->TimeOut == 0)
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, J1939TP_E_TIMEOUT_FP);

    if ((txSduState->State == J1939TP_STATE_FPP_FF) || (txSduState->State == J1939TP_STATE_FPP_AF))
    {
      /* no protection required - all exits from FF and AF states are in the scope of this function */
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
    case J1939TP_STATE_FPP_FF:
      /* no protection required - all exits from FF and AF states are in the scope of this function */
      txSduState->State = J1939TP_STATE_FPP_FF_BUSY;

      /*! \trace SPEC-2167316 */
      J1939Tp_FppInternalTransmitFf(sduIdx);
      break;

    case J1939TP_STATE_FPP_AF:
      /* no protection required - all exits from FF and AF states are in the scope of this function */
      txSduState->State = J1939TP_STATE_FPP_AF_BUSY;

      /*! \trace SPEC-2167316 */
      J1939Tp_FppInternalTransmitAf(sduIdx);
      break;

    default:
      break;
    }
  }
} /* J1939Tp_FppTxMain */
# endif

/**********************************************************************************************************************
 *  J1939Tp_FppRxMain
 *********************************************************************************************************************/
/*! \brief      Main function of the J1939Tp_FppRxMain. It is called by J1939Tp_MainFunction.
 *
 *  \param[in]  PduIdType sduIdx
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_FppRxMain(PduIdType sduIdx)
{
  J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

  if (rxSduState->Aborted) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
  {
    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (rxSduState->State == J1939TP_STATE_FPP_AF)
    {
      /* protected against concurrent reception of FF and AF message */
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
    J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, J1939TP_E_TIMEOUT_T1);

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (rxSduState->State == J1939TP_STATE_FPP_AF)
    {
      /* no protection required - all exits from FF and AF states are in the scope of this function */
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
} /* J1939Tp_FppRxMain */
# endif

/**********************************************************************************************************************
 *  J1939Tp_FppTransmit()
 *********************************************************************************************************************/

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_FppTransmit(PduIdType sduIdx,
                                                       P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info)
{
  Std_ReturnType result = E_NOT_OK;

  if (info->SduLength > J1939TP_FPP_MAX_LENGTH)
  {
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
      txSduState->State = J1939TP_STATE_FPP_FF_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      {
        J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx = J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx);
        uint8 sa, da;
        boolean found;

        if (J1939Tp_GetTxSaOfTxChannel(txChannelIdx) == J1939TP_NO_ADDRESS_CONFIGURED)
        {
          sa = info->SduDataPtr[J1939TP_MDL_SA_POS];
        }
        else
        {
          sa = J1939Tp_GetTxSaOfTxChannel(txChannelIdx);
        }

        if (J1939Tp_GetTxDaOfTxChannel(txChannelIdx) == J1939TP_NO_ADDRESS_CONFIGURED)
        {
          da = info->SduDataPtr[J1939TP_MDL_DA_POS];
        }
        else
        {
          da = J1939Tp_GetTxDaOfTxChannel(txChannelIdx);
        }

        found = J1939Tp_FppInternalTransmit_SearchConflictingTxChannel(sduIdx, txChannelIdx, sa, da);

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
          /* Packets includes just the amount of AFs */
          txSduState->Packets = J1939Tp_Get_Fpp_Packets(info->SduLength);
          txSduState->Sender = sa;
          txSduState->Receiver = da;
          txSduState->Priority = pri;
          txSduState->Sequence = J1939TP_FPP_FC_FF;
          txSduState->TimeOut = J1939Tp_GetTime_Fp();

          txSduState->NextBlock++;
          if (txSduState->NextBlock > J1939TP_FPP_SC_MAX)
          {
            txSduState->NextBlock = J1939TP_FPP_SC_INITIAL;
          }

          J1939Tp_FppInternalTransmitFf(sduIdx);

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
} /* J1939Tp_FppTransmit */
# endif

/**********************************************************************************************************************
 *  J1939Tp_FppTxConfirmation()
 *********************************************************************************************************************/

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_FppTxConfirmation(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if ((txSduState->State == J1939TP_STATE_FPP_FF_CONF) || (txSduState->State == J1939TP_STATE_FPP_AF_CONF))
  {
    /* last packet? */
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
      /* protected against concurrent transmission failure and early Tx confirmation timeout */
      txSduState->State = J1939TP_STATE_FPP_AF_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* prepare transmission of next AF */
      txSduState->Sequence++;
      txSduState->TimeOut = J1939Tp_GetTime_Fp();

      J1939Tp_FppInternalTransmitAf(sduIdx);
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* J1939Tp_FppTxConfirmation */
# endif

/**********************************************************************************************************************
 *  J1939Tp_FppRxIndication()
 *********************************************************************************************************************/

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_FppRxIndication(PduIdType pduIdx,
                                                 P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info, uint8 sa)
{
#  if (J1939TP_CANIF_DLCCHECK == STD_OFF)
  if (info->SduLength == J1939TP_NPDU_LENGTH)
#  endif
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
      /* #-- Check if this is the first frame of an FPP connection. */
      if (J1939Tp_Get_FC(info->SduDataPtr[J1939TP_FF_SC]) == J1939TP_FPP_FC_FF)
      {
        PduLengthType size = info->SduDataPtr[J1939TP_FF_TNDB];
        
        /* #-- The frame size should not exceed the maximum of 223 Bytes. */
        if (size > J1939TP_FPP_MAX_LENGTH)
        {
          /* #-- Otherwise report runtime error J1939TP_E_INVALID_SIZE. */
          J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SIZE);
        }
        else
        {
          J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
          J1939Tp_RxNSduStateType *rxSduState;

          /* #-- Allocate a free channel and start the reception of the FF frame. */
          /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
          if (J1939Tp_FppInternalRxIndication_AllocateRxChannel(pduIdx, sa, da, &sduIdx, &rxSduState))
          {
            J1939Tp_FppInternalRxIndication_ReceiveFf(sduIdx, info, pduLen, sa, da, size, rxSduState);
          }
        }
      }
      else
      {
        J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
        J1939Tp_RxNSduStateType *rxSduState;

        /* #-- Find the active connection and start reception of an AF frame. */
        /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
        if (J1939Tp_FppInternalRxIndication_FindActiveRxChannel(pduIdx, sa, da, &sduIdx, &rxSduState))
        {
          J1939Tp_FppInternalRxIndication_ReceiveAf(sduIdx, info, rxSduState);
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
} /* J1939Tp_FppRxIndication */
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

 MD_J1939Tp_3199
      Reason:     Dummy statement, used to avoid compiler warnings.
      Risk:       None
      Prevention: N/A
*/
