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
 *         File:  J1939Tp_Etp.c
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Implementation of the ISO 11783 ETP sub-module of the SAE J1939 Transport Layer module.
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

#define J1939TP_ETP_C

#include "J1939Tp_Cfg.h"

#if (J1939TP_ETP_ENABLED == STD_ON)
# include "J1939Tp.h"
# include "J1939Tp_Int.h"
# include "J1939Tp_Etp.h"

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
#  error "Vendor specific version numbers of J1939Tp_Etp.c and J1939Tp.h are inconsistent"
# endif


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmRts(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmCts(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmDpo(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmEOMAck(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpAbortTx(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpAbortRx(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpDt(PduIdType sduIdx);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalTransmit_SearchConflictingTxChannel(PduIdType sduIdx,
                                                       J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx, uint8 sa,
                                                       uint8 da);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint8 da, uint32 pgn,
                                                  J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * allocatedSduIdx,
                                                  J1939Tp_RxNSduStateType ** allocatedRxSduState);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpCmRts(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                          sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                          J1939TP_APPL_DATA) info,
                                                                          uint8 sa, uint8 da, uint32 size,
                                                                          J1939Tp_RxNSduStateType * rxSduState);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalRxIndication_FindActiveTxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                    J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType * sduIdx,
                                                    J1939Tp_TxNSduStateType ** txSduState);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpCmCts(J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType
                                                                          sduIdx, CONSTP2CONST(PduInfoType, AUTOMATIC,
                                                                                               J1939TP_APPL_DATA) info,
                                                                          uint32 pgn,
                                                                          J1939Tp_TxNSduStateType * txSduState);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalRxIndication_FindActiveRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                    J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * sduIdx,
                                                    J1939Tp_RxNSduStateType ** rxSduState);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpCmDpo(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                          sduIdx, CONSTP2CONST(PduInfoType, AUTOMATIC,
                                                                                               J1939TP_APPL_DATA) info,
                                                                          uint32 pgn,
                                                                          J1939Tp_RxNSduStateType * rxSduState);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void
J1939Tp_EtpInternalRxIndication_ReceiveEtpCmEomack(J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType sduIdx,
                                                   CONSTP2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
                                                   uint32 pgn, J1939Tp_TxNSduStateType * txSduState);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpAbortRx(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                            sduIdx, uint32 pgn,
                                                                            J1939Tp_RxNSduStateType * rxSduState);
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpAbortTx(J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType
                                                                            sduIdx, uint32 pgn,
                                                                            J1939Tp_TxNSduStateType * txSduState);
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpDt(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                       sduIdx, CONSTP2CONST(PduInfoType, AUTOMATIC,
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
 *  J1939Tp_EtpInternalTransmitEtpCmRts
 *********************************************************************************************************************/
/*! \brief      This functions sends the ETP.CM_RTS message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmRts(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduTxEtpCmNPduIdxOfTxChannel(J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_TX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      Std_ReturnType result;

      data[J1939TP_ETPCM_CB] = J1939TP_ECB_RTS;
      data[J1939TP_ETPCM_NBT_LO] = J1939Tp_Make_NBT_LO(txSduState->Size);
      data[J1939TP_ETPCM_NBT_LOMD] = J1939Tp_Make_NBT_LOMD(txSduState->Size);
      data[J1939TP_ETPCM_NBT_HIMD] = J1939Tp_Make_NBT_HIMD(txSduState->Size);
      data[J1939TP_ETPCM_NBT_HI] = J1939Tp_Make_NBT_HI(txSduState->Size);
      data[J1939TP_ETPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
      data[J1939TP_ETPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
      data[J1939TP_ETPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
      data[J1939TP_NPDU_SA_POS] = txSduState->Sender;
      data[J1939TP_NPDU_DA_POS] = txSduState->Receiver;
      data[J1939TP_NPDU_PRI_POS] = txSduState->Priority;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939TP_NPDU_LENGTH;

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_ETP_RTS_CONF;
      result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
      if (result != E_OK)
      {
        SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (txSduState->State == J1939TP_STATE_ETP_RTS_CONF)
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

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_ETP_RTS;
  }
} /* J1939Tp_EtpInternalTransmitEtpCmRts */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmitEtpCmCts
 *********************************************************************************************************************/
/*! \brief      This functions sends the ETP.CM_CTS message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmCts(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduRxEtpFcNPduIdxOfRxChannel(J1939Tp_GetRxChannelChannelIdxOfRxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_RX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      /*! \trace SPEC-2167327, SPEC-2167331 */
      uint8 block = J1939Tp_GetRxPacketsPerBlockOfRxChannel(J1939Tp_GetRxChannelChannelIdxOfRxSdu(sduIdx));
      uint8 data[J1939TP_NPDU_LENGTH]; /* must stay at the same level as pdu to avoid false QAC warning 3217 */
      PduInfoType pdu;
      uint32 maxblock;
      uint16 blocksize;
      BufReq_ReturnType tpResult = BUFREQ_OK;

      maxblock = rxSduState->Packets - rxSduState->PacketOffset;
      if (block > maxblock)
      {
        block = (uint8) maxblock;
      }

      /*! \trace SPEC-2167327 */
      if (block == maxblock) /* last block */
      {
        blocksize = (uint16) (rxSduState->Size - (rxSduState->PacketOffset * J1939TP_ETPDT_DATA_SIZE));
      }
      else
      {
        blocksize = block * J1939TP_ETPDT_DATA_SIZE;
      }

      if (rxSduState->BufSize < blocksize)
      {
        pdu.SduLength = 0; /*!< \trace SPEC-2167328 */
        pdu.SduDataPtr = NULL_PTR;
        tpResult = PduR_J1939TpCopyRxData(J1939Tp_GetSduIdOfRxSdu(sduIdx), &pdu, &rxSduState->BufSize);
      }

      if (tpResult == BUFREQ_OK)
      {
        if ((rxSduState->BufSize >= blocksize) || (rxSduState->TimeOut <= 1))
        {
          uint32 pgn = (uint32) J1939Tp_GetPGNOfRxSdu(sduIdx);
          Std_ReturnType result;

          if (rxSduState->BufSize >= blocksize)
          {
            uint32 sequence = rxSduState->PacketOffset + 1;

            rxSduState->NextBlock = block;
            data[J1939TP_ETPCM_NOP] = block; /*!< \trace SPEC-2167327 */
            data[J1939TP_ETPCM_NPN_LO] = J1939Tp_Make_NPN_LO(sequence);
            data[J1939TP_ETPCM_NPN_MD] = J1939Tp_Make_NPN_MD(sequence);
            data[J1939TP_ETPCM_NPN_HI] = J1939Tp_Make_NPN_HI(sequence);
          }
          else
          {
            data[J1939TP_ETPCM_NOP] = J1939TP_CTS_WAIT; /*!< \trace SPEC-2167329 */
            data[J1939TP_ETPCM_NPN_LO] = J1939TP_SNA_VALUE;
            data[J1939TP_ETPCM_NPN_MD] = J1939TP_SNA_VALUE;
            data[J1939TP_ETPCM_NPN_HI] = J1939TP_SNA_VALUE;
          }

          data[J1939TP_ETPCM_CB] = J1939TP_ECB_CTS;
          data[J1939TP_ETPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
          data[J1939TP_ETPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
          data[J1939TP_ETPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
          data[J1939TP_NPDU_SA_POS] = rxSduState->Receiver;
          data[J1939TP_NPDU_DA_POS] = rxSduState->Sender;
          data[J1939TP_NPDU_PRI_POS] = rxSduState->Priority;

          pdu.SduDataPtr = data;
          pdu.SduLength = J1939TP_NPDU_LENGTH;

          if (rxSduState->BufSize >= blocksize)
          {
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_ETP_CTS_CONF;
          }
          else
          {
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_ETP_CTSWAIT_CONF;
          }
          result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
          if (result != E_OK)
          {
            SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            if ((rxSduState->State == J1939TP_STATE_ETP_CTS_CONF) ||
                (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT_CONF))
            {
              /* no protection required - no further actions related to the locked state */
              pduState->Locked = FALSE;
              /* protected against late Tx confirmation */
              rxSduState->State = J1939TP_STATE_FINISH;
              SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              /* InSession is active before this function is called */
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
        }
        else
        {
          /* no protection required - no further actions related to the locked state */
          pduState->Locked = FALSE;

          if (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT_BUSY)
          {
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_ETP_CTSWAIT;
          }
          else
          {
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_ETP_CTS;
          }
        }
      }
      else
      {
        /*! \trace SPEC-2167311 */
        /* no protection required - no further actions related to the locked state */
        pduState->Locked = FALSE;
        rxSduState->AbortReason = J1939TP_CAR_RESOURCES;
        rxSduState->TimeOut = J1939Tp_GetTime_Tr();
        /* no protection required - transition from uninterruptible state */
        rxSduState->State = J1939TP_STATE_ETP_ABORT_BUSY;

        J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
      }
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT_BUSY)
    {
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_ETP_CTSWAIT;
    }
    else
    {
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_ETP_CTS;
    }
  }
} /* J1939Tp_EtpInternalTransmitEtpCmCts */ /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmitEtpCmDpo
 *********************************************************************************************************************/
/*! \brief      This functions sends the ETP.CM_DPO message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmDpo(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduTxEtpCmNPduIdxOfTxChannel(J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_TX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      Std_ReturnType result;

      data[J1939TP_ETPCM_CB] = J1939TP_ECB_DPO;
      data[J1939TP_ETPCM_NPO] = txSduState->NextBlock;
      data[J1939TP_ETPCM_DPO_LO] = J1939Tp_Make_DPO_LO(txSduState->PacketOffset);
      data[J1939TP_ETPCM_DPO_MD] = J1939Tp_Make_DPO_MD(txSduState->PacketOffset);
      data[J1939TP_ETPCM_DPO_HI] = J1939Tp_Make_DPO_HI(txSduState->PacketOffset);
      data[J1939TP_ETPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
      data[J1939TP_ETPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
      data[J1939TP_ETPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
      data[J1939TP_NPDU_SA_POS] = txSduState->Sender;
      data[J1939TP_NPDU_DA_POS] = txSduState->Receiver;
      data[J1939TP_NPDU_PRI_POS] = txSduState->Priority;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939TP_NPDU_LENGTH; /*!< \trace SPEC-2167352 */

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_ETP_DPO_CONF;
      result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
      if (result != E_OK)
      {
        SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (txSduState->State == J1939TP_STATE_ETP_DPO_CONF)
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

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_ETP_DPO;
  }
} /* J1939Tp_EtpInternalTransmitEtpCmDpo */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmitEtpCmEOMAck
 *********************************************************************************************************************/
/*! \brief      This function sends the ETP.CM_EOMAck message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpCmEOMAck(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduRxEtpFcNPduIdxOfRxChannel(J1939Tp_GetRxChannelChannelIdxOfRxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_RX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint32 pgn = (uint32) J1939Tp_GetPGNOfRxSdu(sduIdx);
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      Std_ReturnType result;

      data[J1939TP_ETPCM_CB] = J1939TP_ECB_EOMACK;
      data[J1939TP_ETPCM_NBT_LO] = J1939Tp_Make_NBT_LO(rxSduState->Size);
      data[J1939TP_ETPCM_NBT_LOMD] = J1939Tp_Make_NBT_LOMD(rxSduState->Size);
      data[J1939TP_ETPCM_NBT_HIMD] = J1939Tp_Make_NBT_HIMD(rxSduState->Size);
      data[J1939TP_ETPCM_NBT_HI] = J1939Tp_Make_NBT_HI(rxSduState->Size);
      data[J1939TP_ETPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
      data[J1939TP_ETPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
      data[J1939TP_ETPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
      data[J1939TP_NPDU_SA_POS] = rxSduState->Receiver;
      data[J1939TP_NPDU_DA_POS] = rxSduState->Sender;
      data[J1939TP_NPDU_PRI_POS] = rxSduState->Priority;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939TP_NPDU_LENGTH;

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_ETP_EOMACK_CONF;
      result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
      if (result != E_OK)
      {
        SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (rxSduState->State == J1939TP_STATE_ETP_EOMACK_CONF)
        {
          /* no protection required - no further actions related to the locked state */
          pduState->Locked = FALSE;
          /* protected against late Tx confirmation */
          rxSduState->State = J1939TP_STATE_FINISH;
          SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          /* InSession is active before this function is called */
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
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* no protection required - transition from uninterruptible state */
    rxSduState->State = J1939TP_STATE_ETP_EOMACK;
  }
} /* J1939Tp_EtpInternalTransmitEtpCmEOMAck */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmitEtpAbortTx
 *********************************************************************************************************************/
/*! \brief      This function sends the ETP_ConnAbort message and terminates N-SDU transmission and related reception.
 *
 *  \param[in]  sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpAbortTx(PduIdType sduIdx)
{
  J1939Tp_TxChannelChannelIdxOfTxSduType channelIdx = J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx);
  J1939Tp_SizeOfTxPduType pduIdx = J1939Tp_GetTxPduTxEtpCmNPduIdxOfTxChannel(channelIdx);
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_TX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint32 pgn = (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx);
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      Std_ReturnType result;

      data[J1939TP_ETPCM_CB] = J1939TP_ECB_ABORT;
      data[J1939TP_ETPCM_CAR] = (uint8) txSduState->AbortReason;
      data[J1939TP_ETPCM_RES3] = J1939TP_SNA_VALUE;
      data[J1939TP_ETPCM_RES4] = J1939TP_SNA_VALUE;
      data[J1939TP_ETPCM_RES5] = J1939TP_SNA_VALUE;
      data[J1939TP_ETPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
      data[J1939TP_ETPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
      data[J1939TP_ETPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
      data[J1939TP_NPDU_SA_POS] = txSduState->Sender;
      data[J1939TP_NPDU_DA_POS] = txSduState->Receiver;
      data[J1939TP_NPDU_PRI_POS] = txSduState->Priority;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939TP_NPDU_LENGTH;

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_ETP_ABORT_CONF;
      result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
      if (result != E_OK)
      {
        SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (txSduState->State == J1939TP_STATE_ETP_ABORT_CONF)
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

#  if (J1939TP_ETP_RX_ENABLED == STD_ON)
      {
        J1939Tp_ComMChannelOfTxChannelType comMChannel = J1939Tp_GetComMChannelOfTxChannel(channelIdx);
        PduIdType pairedSduIdx;

        for (pairedSduIdx = 0; pairedSduIdx < J1939Tp_GetSizeOfRxSdu(); pairedSduIdx++)
        {
#   if (J1939TP_INVALIDHNDOFRXSDU == STD_ON)
          if (!J1939Tp_IsInvalidHndOfRxSdu(pairedSduIdx))
#   endif
          {
            J1939Tp_RxNSduStateType *pairedRxSduState =
              &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(pairedSduIdx));

            if ((J1939Tp_GetComMChannelOfRxChannel(J1939Tp_GetRxChannelChannelIdxOfRxSdu(pairedSduIdx)) == comMChannel)
                && ((pairedRxSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) &&
                (pairedRxSduState->Sender == txSduState->Receiver) && (pairedRxSduState->Receiver == txSduState->Sender)
                && (J1939Tp_GetPGNOfRxSdu(pairedSduIdx) == pgn))
            {
              SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              if ((pairedRxSduState->State == J1939TP_STATE_ETP_CTS) ||
                  (pairedRxSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
                  (pairedRxSduState->State == J1939TP_STATE_ETP_DPO) ||
                  (pairedRxSduState->State == J1939TP_STATE_ETP_DATA) ||
                  (pairedRxSduState->State == J1939TP_STATE_ETP_EOMACK) ||
                  (pairedRxSduState->State == J1939TP_STATE_ETP_ABORT))
              {
                /* protected against concurrent main function invocation or concurrent reception of ETP.ConnAbort or
                   ETP.DT message */
                pairedRxSduState->State = J1939TP_STATE_FINISH;
                SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                if (pairedRxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
                {
                  /*! \trace SPEC-2167305 */
                  PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
                }

                /* no protection required - transition from uninterruptible state */
                pairedRxSduState->State = J1939TP_STATE_IDLE;
              }
              else
              {
                SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                pairedRxSduState->Aborted = TRUE;
              }

              break;
            }
          }
        }
      }
#  endif
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_ETP_ABORT;
  }
} /* J1939Tp_EtpInternalTransmitEtpAbortTx */ /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmitEtpAbortRx
 *********************************************************************************************************************/
/*! \brief      This functions sends the ETP_ConnAbort message and terminates N-SDU reception and related transmission.
 *
 *  \param[in]  sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpAbortRx(PduIdType sduIdx)
{
  J1939Tp_RxChannelChannelIdxOfRxSduType channelIdx = J1939Tp_GetRxChannelChannelIdxOfRxSdu(sduIdx);
  J1939Tp_SizeOfTxPduType pduIdx = J1939Tp_GetTxPduRxEtpFcNPduIdxOfRxChannel(channelIdx);
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_RX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.CM N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint32 pgn = (uint32) J1939Tp_GetPGNOfRxSdu(sduIdx);
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      Std_ReturnType result;

      data[J1939TP_ETPCM_CB] = J1939TP_ECB_ABORT;
      data[J1939TP_ETPCM_CAR] = (uint8) rxSduState->AbortReason;
      data[J1939TP_ETPCM_RES3] = J1939TP_SNA_VALUE;
      data[J1939TP_ETPCM_RES4] = J1939TP_SNA_VALUE;
      data[J1939TP_ETPCM_RES5] = J1939TP_SNA_VALUE;
      data[J1939TP_ETPCM_PGN_LO] = J1939Tp_Make_PGN_LO(pgn);
      data[J1939TP_ETPCM_PGN_MD] = J1939Tp_Make_PGN_MD(pgn);
      data[J1939TP_ETPCM_PGN_HI] = J1939Tp_Make_PGN_HI(pgn);
      data[J1939TP_NPDU_SA_POS] = rxSduState->Receiver;
      data[J1939TP_NPDU_DA_POS] = rxSduState->Sender;
      data[J1939TP_NPDU_PRI_POS] = rxSduState->Priority;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939TP_NPDU_LENGTH;

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_ETP_ABORT_CONF;
      result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
      if (result != E_OK)
      {
        SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (rxSduState->State == J1939TP_STATE_ETP_ABORT_CONF)
        {
          /* no protection required - no further actions related to the locked state */
          pduState->Locked = FALSE;
          /* protected against late Tx confirmation */
          rxSduState->State = J1939TP_STATE_FINISH;
          SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
          {
            /*! \trace SPEC-2167305 */
            PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
          }

          /* no protection required - transition from uninterruptible state */
          rxSduState->State = J1939TP_STATE_IDLE;
        }
        else
        {
          SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
      }

#  if (J1939TP_ETP_TX_ENABLED == STD_ON)
      {
        J1939Tp_ComMChannelOfRxChannelType comMChannel = J1939Tp_GetComMChannelOfRxChannel(channelIdx);
        PduIdType pairedSduIdx;

        for (pairedSduIdx = 0; pairedSduIdx < J1939Tp_GetSizeOfTxSdu(); pairedSduIdx++)
        {
#   if (J1939TP_INVALIDHNDOFTXSDU == STD_ON)
          if (!J1939Tp_IsInvalidHndOfTxSdu(pairedSduIdx))
#   endif
          {
            J1939Tp_TxNSduStateType *pairedTxSduState =
              &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(pairedSduIdx));

            if ((J1939Tp_GetComMChannelOfTxChannel(J1939Tp_GetTxChannelChannelIdxOfTxSdu(pairedSduIdx)) == comMChannel)
                && ((pairedTxSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) &&
                (pairedTxSduState->Sender == rxSduState->Receiver) && (pairedTxSduState->Receiver == rxSduState->Sender)
                && (J1939Tp_GetPGNOfTxSdu(pairedSduIdx) == pgn))
            {
              SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              if ((pairedTxSduState->State == J1939TP_STATE_ETP_RTS) ||
                  (pairedTxSduState->State == J1939TP_STATE_ETP_CTS) ||
                  (pairedTxSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
                  (pairedTxSduState->State == J1939TP_STATE_ETP_ABORT) ||
                  (pairedTxSduState->State == J1939TP_STATE_ETP_DPO) ||
                  (pairedTxSduState->State == J1939TP_STATE_ETP_DATA))
              {
                /* protected against concurrent main function invocation or concurrent reception of ETP.ConnAbort
                   message */
                pairedTxSduState->State = J1939TP_STATE_FINISH;
                SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                /*! \trace SPEC-2167303 */
                PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

                /* no protection required - transition from uninterruptible state */
                pairedTxSduState->State = J1939TP_STATE_IDLE;
              }
              else
              {
                SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                pairedTxSduState->AbortReason = J1939TP_CAR_NO_ABORT;
                pairedTxSduState->Aborted = TRUE;
              }

              break;
            }
          }
        }
      }
#  endif
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* no protection required - transition from uninterruptible state */
    rxSduState->State = J1939TP_STATE_ETP_ABORT;
  }
} /* J1939Tp_EtpInternalTransmitEtpAbortRx */ /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmitEtpDt
 *********************************************************************************************************************/
/*! \brief      This function sends the ETP.DT message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
static void J1939Tp_EtpInternalTransmitEtpDt(PduIdType sduIdx)
{
  J1939Tp_SizeOfTxPduType pduIdx =
    J1939Tp_GetTxPduTxEtpDtNPduIdxOfTxChannel(J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx));
  J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(pduIdx));
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->CurrentNSduType = J1939TP_NSDU_ETP_TX;
    pduState->CurrentNSduId = sduIdx;
    pduState->ConfTimeout = J1939Tp_GetTxConfTimeout();
    /* protected against concurrent access to the same ETP.DT N-PDU */
    pduState->Locked = TRUE;
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    {
      uint32 sequence = txSduState->PacketOffset + txSduState->Sequence;
      uint8 data[J1939TP_NPDU_LENGTH];
      PduInfoType pdu;
      BufReq_ReturnType tpResult;

      pdu.SduDataPtr = &data[J1939TP_ETPDT_DATA];
      pdu.SduLength = J1939TP_ETPDT_DATA_SIZE;

      if (sequence == txSduState->Packets)
      {
        pdu.SduLength = (PduLengthType) (txSduState->Size - ((sequence - 1) * J1939TP_ETPDT_DATA_SIZE));
      }

      /*! \trace SPEC-2167315, SPEC-2167317 */
      tpResult = PduR_J1939TpCopyTxData(J1939Tp_GetSduIdOfTxSdu(sduIdx), &pdu, NULL_PTR, &txSduState->DataSize);
      if (tpResult == BUFREQ_OK)
      {
        uint8 pos;
        Std_ReturnType result;

        data[J1939TP_ETPDT_SEQ] = txSduState->Sequence;

        for (pos = J1939TP_ETPDT_DATA + (uint8) pdu.SduLength; pos < J1939TP_FRAME_LENGTH; pos++)
        {
          data[pos] = J1939TP_SNA_VALUE; /*!< \trace SPEC-2167353 */
        }

        data[J1939TP_NPDU_SA_POS] = txSduState->Sender;
        data[J1939TP_NPDU_DA_POS] = txSduState->Receiver;
        data[J1939TP_NPDU_PRI_POS] = txSduState->Priority;

        pdu.SduDataPtr = data;
        pdu.SduLength = J1939TP_NPDU_LENGTH; /*!< \trace SPEC-2167352 */

        /* no protection required - transition from uninterruptible state */
        txSduState->State = J1939TP_STATE_ETP_DATA_CONF;
        result = CanIf_Transmit(J1939Tp_GetPduIdOfTxPdu(pduIdx), &pdu);
        if (result != E_OK)
        {
          SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          if (txSduState->State == J1939TP_STATE_ETP_DATA_CONF)
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
          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_ETP_DATA;
        }
        else
        {
          /*! \trace SPEC-2167320 */
          txSduState->AbortReason = J1939TP_CAR_RESOURCES;

          J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
        }
      }
    }
  }
  else
  {
    SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_ETP_DATA;
  }
} /* J1939Tp_EtpInternalTransmitEtpDt */ /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpInternalTransmit_SearchConflictingTxChannel
 *********************************************************************************************************************/
/*! \brief      This function sends the TP.DT message.
 *
 *  \param[in]  PduIdType sduIdx
 *  \param[in]  J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx
 *  \param[in]  uint8 sa
 *  \param[in]  uint8 da
 *  \context    The function can be called on interrupt and task level.
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalTransmit_SearchConflictingTxChannel(PduIdType sduIdx,
                                                       J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx, uint8 sa,
                                                       uint8 da)
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

          if (((otherTxSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) &&
              (otherTxSduState->Sender == sa) && (otherTxSduState->Receiver == da))
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


# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalRxIndication_AllocateRxChannel(PduIdType pduIdx, uint8 sa, uint8 da, uint32 pgn,
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
    if (J1939Tp_GetRxProtocolTypeOfRxChannel(rxChannelIdx) != J1939TP_PROTOCOL_BAM_RXPROTOCOLTYPEOFRXCHANNEL)
#  endif
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
          J1939Tp_RxChannelRxNSduRxNSduTableIndEndIdxOfRxChannelType rxChannelSduIdx;

          for (rxChannelSduIdx = J1939Tp_GetRxChannelRxNSduRxNSduTableIndStartIdxOfRxChannel(rxChannelIdx);
               rxChannelSduIdx < J1939Tp_GetRxChannelRxNSduRxNSduTableIndEndIdxOfRxChannel(rxChannelIdx);
               rxChannelSduIdx++)
          {
            J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx =
              J1939Tp_GetRxSduRxNSduIdxOfRxChannelRxNSdu(rxChannelSduIdx);
            J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

            if (((rxSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) && (rxSduState->Sender == sa) &&
                (rxSduState->Receiver == da))
            {
              J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_UNTIMELY_RTS);

              SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              if ((rxSduState->State == J1939TP_STATE_ETP_CTS) || (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
                  (rxSduState->State == J1939TP_STATE_ETP_DPO) || (rxSduState->State == J1939TP_STATE_ETP_DATA) ||
                  (rxSduState->State == J1939TP_STATE_ETP_EOMACK) || (rxSduState->State == J1939TP_STATE_ETP_ABORT))
              {
                /* protected against concurrent timeout and concurrent reception of ETP.DT message */
                rxSduState->State = J1939TP_STATE_FINISH;
                SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  /*! \trace SPEC-2167305 */
                  PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
                }

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
                rxSduState->InSession = FALSE;
                /* protected against concurrent reception of ETP.CM_RTS message */ /* TODO: Check if this is necessary */
                rxSduState->State = J1939TP_STATE_ETP_RTS_BUSY;
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
  }

#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
  if (found && (!allocated))
  {
    /* No free connection found for incoming RTS */

    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_NO_CONNECTION);
  }
#  else
  J1939TP_DUMMY_STATEMENT(found); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#  endif

  return allocated;
} /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
# endif


# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpCmRts(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                          sduIdx, P2CONST(PduInfoType, AUTOMATIC,
                                                                                          J1939TP_APPL_DATA) info,
                                                                          uint8 sa, uint8 da, uint32 size,
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
    PduInfoType addrInfo;
    BufReq_ReturnType tpResult;

    rxSduState->Size = size;
    rxSduState->Packets = J1939Tp_Get_EtpPackets(size);
    rxSduState->Sender = sa;
    rxSduState->Receiver = da;
    rxSduState->Priority = J1939Tp_MDL_Get_PRI(info->SduDataPtr[J1939TP_NPDU_PRI_POS]);

    addrInfo.SduLength = J1939Tp_GetMetaDataLengthOfRxSdu(sduIdx);
    addrInfo.SduDataPtr = &(info->SduDataPtr[J1939TP_NPDU_MDL_POS]);

    tpResult = PduR_J1939TpStartOfReception(J1939Tp_GetSduIdOfRxSdu(sduIdx), &addrInfo, (PduLengthType) size,
                                            &rxSduState->BufSize);
    if (tpResult == BUFREQ_OK)
    {
      rxSduState->InSession = TRUE;
      rxSduState->PacketOffset = J1939TP_PACKET_OFFSET_INITIAL;
      rxSduState->Sequence = J1939TP_RX_SEQUENCE_INITIAL;
      rxSduState->TimeOut = J1939Tp_GetTime_Tr();

      J1939Tp_EtpInternalTransmitEtpCmCts(sduIdx);
    }
    else
    {
      /*! \trace SPEC-2167310, SPEC-2167311 */
      rxSduState->AbortReason = J1939TP_CAR_RESOURCES;
      rxSduState->TimeOut = J1939Tp_GetTime_Tr();

      J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
    }
  }
} /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif


# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalRxIndication_FindActiveTxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                    J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType * sduIdx,
                                                    J1939Tp_TxNSduStateType ** txSduState)
{
  J1939Tp_RxPduTxChannelTxChannelTableIndEndIdxOfRxPduType rxPduTxChannelIdx;
  boolean found = FALSE;

  /* Search for all Tx channels that use the reported PDU. It is not necessary to search for all Tx channels with the
     same ComM channel, because there can only be one Rx PDU per ComM channel (ensured by CanIf). */
  for (rxPduTxChannelIdx = J1939Tp_GetRxPduTxChannelTxChannelTableIndStartIdxOfRxPdu(pduIdx);
       (rxPduTxChannelIdx < J1939Tp_GetRxPduTxChannelTxChannelTableIndEndIdxOfRxPdu(pduIdx)) && (!found);
       rxPduTxChannelIdx++)
  {
    J1939Tp_TxChannelTxChannelIdxOfRxPduTxChannelType txChannelIdx =
      J1939Tp_GetTxChannelTxChannelIdxOfRxPduTxChannel(rxPduTxChannelIdx);
    J1939Tp_TxChannelTxNSduTxNSduTableIndEndIdxOfTxChannelType txChannelSduIdx;

    for (txChannelSduIdx = J1939Tp_GetTxChannelTxNSduTxNSduTableIndStartIdxOfTxChannel(txChannelIdx);
         txChannelSduIdx < J1939Tp_GetTxChannelTxNSduTxNSduTableIndEndIdxOfTxChannel(txChannelIdx); txChannelSduIdx++)
    {
      *sduIdx = J1939Tp_GetTxSduTxNSduIdxOfTxChannelTxNSdu(txChannelSduIdx);
      *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(*sduIdx));

      if ((((*txSduState)->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) && ((*txSduState)->Receiver == sa) &&
          ((*txSduState)->Sender == da))
      {
        found = TRUE;

        break;
      }
    }
  }

  return found;
}
# endif


# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpCmCts(J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType
                                                                          sduIdx, CONSTP2CONST(PduInfoType, AUTOMATIC,
                                                                                               J1939TP_APPL_DATA) info,
                                                                          uint32 pgn,
                                                                          J1939Tp_TxNSduStateType * txSduState)
{
  if (pgn == (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx))
  {
    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_CTS) || (txSduState->State == J1939TP_STATE_ETP_CTSWAIT))
    {
      /* protected against concurrent main function invocation and concurrent Tx abort */
      txSduState->State = J1939TP_STATE_ETP_CTS_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      {
        uint8 block = info->SduDataPtr[J1939TP_ETPCM_NOP];
        uint32 nextPacket =
          J1939Tp_Get_NPN(info->SduDataPtr[J1939TP_ETPCM_NPN_LO], info->SduDataPtr[J1939TP_ETPCM_NPN_MD],
                          info->SduDataPtr[J1939TP_ETPCM_NPN_HI]);

        /*! \trace SPEC-2167333 */
        if (block == J1939TP_CTS_WAIT)
        {
#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
          if (nextPacket != J1939TP_ETPCM_NPN_UNUSED)
          {
            J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_NPN);
          }
#  endif

          txSduState->TimeOut = J1939Tp_GetTime_T4();
          /* no protection required - transition from uninterruptible state */
          txSduState->State = J1939TP_STATE_ETP_CTSWAIT;
        }
        else
        {
          uint32 sequence = txSduState->PacketOffset + txSduState->Sequence;
          if (nextPacket == sequence)
          {
            uint32 maxblock = txSduState->Packets - (sequence - 1);

            if (block > maxblock)
            {
              J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_NOP);

              txSduState->AbortReason = J1939TP_CAR_ECTS_PACKETS;
              txSduState->TimeOut = J1939Tp_GetTime_Tr();

              J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
            }
            else
            {
              txSduState->NextBlock = block;

              txSduState->TimeOut = J1939Tp_GetTime_Tr();

              J1939Tp_EtpInternalTransmitEtpCmDpo(sduIdx);
            }
          }
          else
          {
            /*! \trace SPEC-2167334, SPEC-2167336 */
            J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_NPN);

            /* Abort with reason SNA (no reason specified for ETP.CTS requesting a sequence number that cannot be
               delivered) */
            txSduState->AbortReason = J1939TP_CAR_OTHER;
            txSduState->TimeOut = J1939Tp_GetTime_Tr();

            J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
          }
        }
      }
    }
    else
    {
      /* ETP.CM_CTS shall be ignored if received unexpectedly */

      if (txSduState->State == J1939TP_STATE_ETP_DATA)
      {
        /* protected against concurrent main function invocation and concurrent Tx abort */
        txSduState->State = J1939TP_STATE_ETP_CTS_BUSY;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_UNEXPECTED_ECTS);

        /* Abort with reason 4 (CTS messages received when data transfer is in progress) */
        txSduState->AbortReason = J1939TP_CAR_UNTIMELY_CTS; /*!< \trace SPEC-2167355 */
        txSduState->TimeOut = J1939Tp_GetTime_Tr();

        J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
      }
      else
      {
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_IGNORED_ECTS);
      }
    }
  }
  else
  {
    /* ETP.CM_CTS received with unexpected PGN, send TP.Conn_Abort */

    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_PGN);

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_RTS) || (txSduState->State == J1939TP_STATE_ETP_CTS) ||
        (txSduState->State == J1939TP_STATE_ETP_CTSWAIT) || (txSduState->State == J1939TP_STATE_ETP_ABORT) ||
        (txSduState->State == J1939TP_STATE_ETP_DPO) || (txSduState->State == J1939TP_STATE_ETP_DATA))
    {
      /* protected against concurrent main function invocation and concurrent Tx abort */
      txSduState->State = J1939TP_STATE_ETP_CTS_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Abort with reason 14 (Unexpected ECTS PGN) */
      txSduState->AbortReason = J1939TP_CAR_ECTS_PGN;
      txSduState->TimeOut = J1939Tp_GetTime_Tr();

      J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Abort with reason 14 (Unexpected ECTS PGN) */
      txSduState->AbortReason = J1939TP_CAR_ECTS_PGN;
      txSduState->Aborted = TRUE;
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif


# if (J1939TP_ETP_RX_ENABLED == STD_ON)
/*! \trace SPEC-2167348 */
J1939TP_LOCAL_INLINE boolean
J1939Tp_EtpInternalRxIndication_FindActiveRxChannel(PduIdType pduIdx, uint8 sa, uint8 da,
                                                    J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType * sduIdx,
                                                    J1939Tp_RxNSduStateType ** rxSduState)
{
  J1939Tp_RxPduRxChannelRxChannelTableIndEndIdxOfRxPduType rxPduRxChannelIdx;
  boolean found = FALSE;

  /* Search for all Rx channels that use the reported PDU. It is not necessary to search for all Rx channels with the
     same ComM channel, because there can only be one Rx PDU per ComM channel (ensured by CanIf). */
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

      if ((((*rxSduState)->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) && ((*rxSduState)->Sender == sa) &&
          ((*rxSduState)->Receiver == da))
      {
        found = TRUE;

        break;
      }
    }
  }

  return found;
}
# endif


# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpCmDpo(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                          sduIdx, CONSTP2CONST(PduInfoType, AUTOMATIC,
                                                                                               J1939TP_APPL_DATA) info,
                                                                          uint32 pgn,
                                                                          J1939Tp_RxNSduStateType * rxSduState)
{
  SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (rxSduState->State == J1939TP_STATE_ETP_DPO)
  {
    /* protected against concurrent timeout and concurrent reception of TP.CM message */
    rxSduState->State = J1939TP_STATE_ETP_DPO_BUSY;
    SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (pgn == (uint32) J1939Tp_GetPGNOfRxSdu(sduIdx))
    {
      uint32 nextPacket =
        J1939Tp_Get_DPO(info->SduDataPtr[J1939TP_ETPCM_DPO_LO], info->SduDataPtr[J1939TP_ETPCM_DPO_MD],
                        info->SduDataPtr[J1939TP_ETPCM_DPO_HI]);
      if (nextPacket == rxSduState->PacketOffset)
      {
        uint8 npo = info->SduDataPtr[J1939TP_ETPCM_NPO];

#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
        if (npo != rxSduState->NextBlock)
        {
          J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_NPO);
        }
#  endif

        if (npo > rxSduState->NextBlock)
        {
          rxSduState->AbortReason = J1939TP_CAR_DPO_PACKETS;
          rxSduState->TimeOut = J1939Tp_GetTime_Tr();

          J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
        }
        else
        {
          if (npo < rxSduState->NextBlock)
          {
            rxSduState->NextBlock = npo;
          }

          rxSduState->PacketOffset = nextPacket;
          rxSduState->Sequence = J1939TP_RX_SEQUENCE_INITIAL;

          rxSduState->TimeOut = J1939Tp_GetTime_T1();
          /* no protection required - transition from uninterruptible state */
          rxSduState->State = J1939TP_STATE_ETP_DATA;
        }
      }
      else
      {
        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_DPO);

        rxSduState->AbortReason = J1939TP_CAR_DPO_OFFSET;
        rxSduState->TimeOut = J1939Tp_GetTime_Tr();

        J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
      }
    }
    else
    {
      J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_UNEXPECTED_DPO);

      rxSduState->AbortReason = J1939TP_CAR_DPO_PGN;
      rxSduState->TimeOut = J1939Tp_GetTime_Tr();

      J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
    }
  }
  else
  {
    SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif


# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void
J1939Tp_EtpInternalRxIndication_ReceiveEtpCmEomack(J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType sduIdx,
                                                   CONSTP2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
                                                   uint32 pgn, J1939Tp_TxNSduStateType * txSduState)
{
  if (pgn == (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx))
  {
    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_CTS) || (txSduState->State == J1939TP_STATE_ETP_CTSWAIT))
    {
      /* protected against concurrent main function invocation and concurrent Tx abort */
      txSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
      if (txSduState->Size != J1939Tp_Get_NBT(info->SduDataPtr[J1939TP_ETPCM_NBT_LO],
                                              info->SduDataPtr[J1939TP_ETPCM_NBT_LOMD],
                                              info->SduDataPtr[J1939TP_ETPCM_NBT_HIMD],
                                              info->SduDataPtr[J1939TP_ETPCM_NBT_HI]))
      {
        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_ANBT);
      }
#  endif

      /*! \trace SPEC-2167302 */
      PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_OK);

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* ETP.CM_EndOfMsgAck shall be ignored if received unexpectedly */

      J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_IGNORED_EOMACK);
    }
  }

#  if (J1939TP_DEV_ERROR_REPORT == STD_OFF)
  J1939TP_DUMMY_STATEMENT_CONST(info); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#  endif
}
# endif


# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpAbortRx(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                            sduIdx, uint32 pgn,
                                                                            J1939Tp_RxNSduStateType * rxSduState)
{
  if (pgn == (uint32) J1939Tp_GetPGNOfRxSdu(sduIdx))
  {
    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((rxSduState->State == J1939TP_STATE_ETP_CTS) || (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
        (rxSduState->State == J1939TP_STATE_ETP_DPO) || (rxSduState->State == J1939TP_STATE_ETP_DATA) ||
        (rxSduState->State == J1939TP_STATE_ETP_EOMACK) || (rxSduState->State == J1939TP_STATE_ETP_ABORT))
    {
      /* protected against concurrent main function invocation, concurrent Tx abort, and concurrent reception of ETP.DT
         message */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
      }

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      rxSduState->Aborted = TRUE;
    }
  }
  else
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_IGNORED_ABORT);
  }
}
# endif


# if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpAbortTx(J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType
                                                                            sduIdx, uint32 pgn,
                                                                            J1939Tp_TxNSduStateType * txSduState)
{
  if (pgn == (uint32) J1939Tp_GetPGNOfTxSdu(sduIdx))
  {
    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_RTS) || (txSduState->State == J1939TP_STATE_ETP_CTS) ||
        (txSduState->State == J1939TP_STATE_ETP_CTSWAIT) || (txSduState->State == J1939TP_STATE_ETP_ABORT) ||
        (txSduState->State == J1939TP_STATE_ETP_DPO) || (txSduState->State == J1939TP_STATE_ETP_DATA))
    {
      /* protected against concurrent main function invocation and concurrent Tx abort */
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

      txSduState->AbortReason = J1939TP_CAR_NO_ABORT;
      txSduState->Aborted = TRUE;
    }
  }
  else
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_IGNORED_ABORT);
  }
}
# endif


# if (J1939TP_ETP_RX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_EtpInternalRxIndication_ReceiveEtpDt(J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType
                                                                       sduIdx, CONSTP2CONST(PduInfoType, AUTOMATIC,
                                                                                            J1939TP_APPL_DATA) info,
                                                                       J1939Tp_RxNSduStateType * rxSduState)
{
  SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (rxSduState->State == J1939TP_STATE_ETP_DATA)
  {
    /* protected against concurrent timeout and concurrent reception of ETP.CM message */
    rxSduState->State = J1939TP_STATE_ETP_DATA_BUSY;
    SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (rxSduState->Aborted) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
    {
      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_FINISH;

      /* InSession is active before the current state is reached */
      /*! \trace SPEC-2167305 */
      PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      rxSduState->Sequence++;

      if (rxSduState->Sequence == info->SduDataPtr[J1939TP_ETPDT_SEQ])
      {
        uint32 sequence = rxSduState->PacketOffset + rxSduState->Sequence;
        PduInfoType pdu;
        BufReq_ReturnType tpResult;

        pdu.SduDataPtr = &info->SduDataPtr[J1939TP_ETPDT_DATA];
        pdu.SduLength = J1939TP_ETPDT_DATA_SIZE;

        if (sequence == rxSduState->Packets)
        {
          pdu.SduLength = (PduLengthType) (rxSduState->Size - ((sequence - 1) * J1939TP_ETPDT_DATA_SIZE));
        }

        tpResult = PduR_J1939TpCopyRxData(J1939Tp_GetSduIdOfRxSdu(sduIdx), &pdu, &rxSduState->BufSize);
        if (tpResult == BUFREQ_OK)
        {
          if (sequence == rxSduState->Packets)
          {
            rxSduState->TimeOut = J1939Tp_GetTime_Tr();

            J1939Tp_EtpInternalTransmitEtpCmEOMAck(sduIdx);
          }
          else if (rxSduState->Sequence == rxSduState->NextBlock)
          {
            rxSduState->PacketOffset = sequence;
            rxSduState->Sequence = J1939TP_RX_SEQUENCE_INITIAL;
            rxSduState->TimeOut = J1939Tp_GetTime_Tr();

            J1939Tp_EtpInternalTransmitEtpCmCts(sduIdx);
          }
          else
          {
            rxSduState->TimeOut = J1939Tp_GetTime_T1();
            /* no protection required - transition from uninterruptible state */
            rxSduState->State = J1939TP_STATE_ETP_DATA;
          }
        }
        else
        {
          /*! \trace SPEC-2167311 */
          rxSduState->AbortReason = J1939TP_CAR_RESOURCES;
          rxSduState->TimeOut = J1939Tp_GetTime_Tr();

          J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
        }
      }
      else
      {
        /*! \trace SPEC-2167338, SPEC-2167339 */
        J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SN);

        /* TODO: Abort with reason 8 if sequence number is repeated */
        rxSduState->AbortReason = J1939TP_CAR_BAD_SEQ;
        rxSduState->TimeOut = J1939Tp_GetTime_Tr();

        J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
      }
    }
  }
  else
  {
    SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* ETP.DT shall be ignored if received unexpectedly */
  }
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
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
 *  J1939Tp_EtpTxMain
 *********************************************************************************************************************/
/*! \brief      Main function of the J1939Tp_EtpTxMain. It is called by J1939Tp_MainFunction.
 *
 *  \param[in]  PduIdType sduIdx
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_EtpTxMain(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  if (txSduState->Aborted) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
  {
    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_RTS) || (txSduState->State == J1939TP_STATE_ETP_CTS) ||
        (txSduState->State == J1939TP_STATE_ETP_CTSWAIT) || (txSduState->State == J1939TP_STATE_ETP_ABORT) ||
        (txSduState->State == J1939TP_STATE_ETP_DPO) || (txSduState->State == J1939TP_STATE_ETP_DATA))
    {
      if (txSduState->AbortReason == J1939TP_CAR_NO_ABORT)
      {
        /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
        txSduState->State = J1939TP_STATE_FINISH;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /*! \trace SPEC-2167303 */
        PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

        /* no protection required - transition from uninterruptible state */
        txSduState->State = J1939TP_STATE_IDLE;
      }
      else
      {
        txSduState->State = J1939TP_STATE_ETP_ABORT_BUSY;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        txSduState->TimeOut = J1939Tp_GetTime_Tr();

        J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
      }
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else if (txSduState->TimeOut == 0)
  {
#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
    uint8 errorId = J1939TP_E_NO_ERROR;

    switch (txSduState->State)
    {
    case J1939TP_STATE_ETP_CTS:
      errorId = J1939TP_E_TIMEOUT_T3;
      break;

    case J1939TP_STATE_ETP_CTSWAIT:
      errorId = J1939TP_E_TIMEOUT_T4;
      break;

    case J1939TP_STATE_ETP_RTS:
    case J1939TP_STATE_ETP_ABORT:
    case J1939TP_STATE_ETP_DPO:
    case J1939TP_STATE_ETP_DATA:
      errorId = J1939TP_E_TIMEOUT_TR;
      break;

    default: /* MISRA */ /* COV_J1939TP_ETP_SWITCHCASE_DEFAULT */
      break;
    }

    J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, errorId);
#  endif

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_RTS) || (txSduState->State == J1939TP_STATE_ETP_ABORT))
    {
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
      txSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /*! \trace SPEC-2167303 */
      PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_IDLE;
    }
    else if ((txSduState->State == J1939TP_STATE_ETP_CTS) || (txSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
             (txSduState->State == J1939TP_STATE_ETP_DPO) || (txSduState->State == J1939TP_STATE_ETP_DATA))
    {
      /*! \trace SPEC-2167341 */
      txSduState->AbortReason = J1939TP_CAR_TIMEOUT;
      txSduState->TimeOut = J1939Tp_GetTime_Tr();
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
      txSduState->State = J1939TP_STATE_ETP_ABORT_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    --txSduState->TimeOut;

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    switch (txSduState->State)
    {
    case J1939TP_STATE_ETP_RTS:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
      txSduState->State = J1939TP_STATE_ETP_RTS_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpCmRts(sduIdx);
      break;

    case J1939TP_STATE_ETP_ABORT:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
      txSduState->State = J1939TP_STATE_ETP_ABORT_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpAbortTx(sduIdx);
      break;

    case J1939TP_STATE_ETP_DPO:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
      txSduState->State = J1939TP_STATE_ETP_DPO_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpCmDpo(sduIdx);
      break;

    case J1939TP_STATE_ETP_DATA:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort */
      txSduState->State = J1939TP_STATE_ETP_DATA_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /*! \trace SPEC-2167316 */
      J1939Tp_EtpInternalTransmitEtpDt(sduIdx);
      break;

    default:
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      break;
    }
  }
} /* J1939Tp_EtpTxMain */ /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpRxMain
 *********************************************************************************************************************/
/*! \brief      Main function of the J1939Tp_EtpRxMain. It is called by J1939Tp_MainFunction.
 *
 *  \param[in]  PduIdType sduIdx
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_EtpRxMain(PduIdType sduIdx)
{
  J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

  if (rxSduState->Aborted) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
  {
    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((rxSduState->State == J1939TP_STATE_ETP_CTS) || (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
        (rxSduState->State == J1939TP_STATE_ETP_DPO) || (rxSduState->State == J1939TP_STATE_ETP_DATA) ||
        (rxSduState->State == J1939TP_STATE_ETP_EOMACK) || (rxSduState->State == J1939TP_STATE_ETP_ABORT))
    {
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort or ETP.DT message */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
      }

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
#  if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
    uint8 errorId = J1939TP_E_NO_ERROR;

    switch (rxSduState->State)
    {
    case J1939TP_STATE_ETP_CTS:
    case J1939TP_STATE_ETP_EOMACK:
    case J1939TP_STATE_ETP_ABORT:
      errorId = J1939TP_E_TIMEOUT_TR;
      break;

    case J1939TP_STATE_ETP_CTSWAIT:
      errorId = J1939TP_E_TIMEOUT_TH;
      break;

    case J1939TP_STATE_ETP_DPO:
      errorId = J1939TP_E_TIMEOUT_T2;
      break;

    case J1939TP_STATE_ETP_DATA:
      errorId = J1939TP_E_TIMEOUT_T1;
      break;

    default: /* MISRA */ /* COV_J1939TP_ETP_SWITCHCASE_DEFAULT */
      break;
    }

    J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, errorId);
#  endif

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (rxSduState->State == J1939TP_STATE_ETP_ABORT)
    {
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort message */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
      }

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else if ((rxSduState->State == J1939TP_STATE_ETP_CTS) || (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT) ||
             (rxSduState->State == J1939TP_STATE_ETP_DPO) || (rxSduState->State == J1939TP_STATE_ETP_DATA) ||
             (rxSduState->State == J1939TP_STATE_ETP_EOMACK))
    {
      /*! \trace SPEC-2167342 */
      rxSduState->AbortReason = J1939TP_CAR_TIMEOUT;
      rxSduState->TimeOut = J1939Tp_GetTime_Tr();
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort or ETP.DT message */
      rxSduState->State = J1939TP_STATE_ETP_ABORT_BUSY;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
    }
    else
    {
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  else
  {
    --rxSduState->TimeOut;

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    switch (rxSduState->State)
    {
    case J1939TP_STATE_ETP_CTS:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort message */
      rxSduState->State = J1939TP_STATE_ETP_CTS_BUSY;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpCmCts(sduIdx);
      break;

    case J1939TP_STATE_ETP_CTSWAIT:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort message */
      rxSduState->State = J1939TP_STATE_ETP_CTSWAIT_BUSY;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpCmCts(sduIdx);
      break;

    case J1939TP_STATE_ETP_EOMACK:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort message */
      rxSduState->State = J1939TP_STATE_ETP_EOMACK_BUSY;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpCmEOMAck(sduIdx);
      break;

    case J1939TP_STATE_ETP_ABORT:
      /* protected against concurrent Tx abort and concurrent reception of ETP.ConnAbort message */
      rxSduState->State = J1939TP_STATE_ETP_ABORT_BUSY;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_EtpInternalTransmitEtpAbortRx(sduIdx);
      break;

    default:
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      break;
    }
  }
} /* J1939Tp_EtpRxMain */ /* PRQA S 6030 */ /* MD_MSR_STCYC */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpTransmit
 *********************************************************************************************************************/
/*! \brief      This function creates the J1939 frame according to its structure and calls the
 *              J1939Tp_EtpInternalTransmitEtpCmRts function.
 *
 *  \note       TODO
 *********************************************************************************************************************/

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_EtpTransmit(PduIdType sduIdx,
                                                       P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info)
{
  Std_ReturnType result = E_NOT_OK;

  if (info->SduLength > J1939TP_ETP_MAX_LENGTH) /* PRQA S 3356, 3359 */ /* MD_J1939Tp_13.7_Etp, MD_J1939Tp_13.7_Etp */
  { /* PRQA S 3201 */ /* MD_J1939Tp_14.1_3201_Etp */
    /*! \trace SPEC-2167289 */
    J1939Tp_ReportDevError(J1939TP_SID_TRANSMIT, J1939TP_E_INVALID_LENGTH);
  }
  else
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (txSduState->State == J1939TP_STATE_IDLE)
    {
      txSduState->Aborted = FALSE;
      /* protected against a reentrant call for the same N-SDU */ /* TODO: Check if this is necessary */
      txSduState->State = J1939TP_STATE_ETP_RTS_BUSY;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      {
        J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx = J1939Tp_GetTxChannelChannelIdxOfTxSdu(sduIdx);
        uint8 da, sa;
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

        found = J1939Tp_EtpInternalTransmit_SearchConflictingTxChannel(sduIdx, txChannelIdx, sa, da);

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
          txSduState->Packets = J1939Tp_Get_EtpPackets(info->SduLength);
          txSduState->Sender = sa;
          txSduState->Receiver = da;
          txSduState->Priority = pri;
          txSduState->PacketOffset = J1939TP_PACKET_OFFSET_INITIAL;
          txSduState->Sequence = J1939TP_TX_SEQUENCE_INITIAL;
          txSduState->TimeOut = J1939Tp_GetTime_Tr();

          J1939Tp_EtpInternalTransmitEtpCmRts(sduIdx);

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
} /* J1939Tp_EtpTransmit */
# endif


/**********************************************************************************************************************
 *  J1939Tp_EtpTxConfirmation
 *********************************************************************************************************************/
/*! \brief      This function sends the Tx confirmation.
 *
 *  \note       TODO
 *********************************************************************************************************************/

FUNC(void, J1939TP_CODE) J1939Tp_EtpTxConfirmation(PduIdType sduIdx, J1939Tp_PduTypeOfTxPduType pduType,
                                                   J1939Tp_NSduType sduType)
{
# if ((J1939TP_ETP_TX_ENABLED == STD_ON) && (J1939TP_ETP_RX_ENABLED == STD_ON))
  if (sduType == J1939TP_NSDU_ETP_TX)
# endif
# if (J1939TP_ETP_TX_ENABLED == STD_ON)
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    if (pduType == J1939TP_ETPCM_PDUTYPEOFTXPDU)
    {
      SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      switch (txSduState->State)
      {
      case J1939TP_STATE_ETP_RTS_CONF:
        txSduState->TimeOut = J1939Tp_GetTime_T3();
        /* protected against concurrent transmission failure and early Tx confirmation timeout */
        txSduState->State = J1939TP_STATE_ETP_CTS;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        break;

      case J1939TP_STATE_ETP_DPO_CONF:
        txSduState->TimeOut = J1939Tp_GetTime_Tr();
        /* protected against concurrent transmission failure and early Tx confirmation timeout */
        txSduState->State = J1939TP_STATE_ETP_DATA_BUSY;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        J1939Tp_EtpInternalTransmitEtpDt(sduIdx);
        break;

      case J1939TP_STATE_ETP_ABORT_CONF:
        /* protected against concurrent transmission failure and early Tx confirmation timeout */
        txSduState->State = J1939TP_STATE_FINISH;
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /*! \trace SPEC-2167303 */
        PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

        /* no protection required - transition from uninterruptible state */
        txSduState->State = J1939TP_STATE_IDLE;
        break;

      default:
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        break;
      }
    }
    else
    {
      SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if (txSduState->State == J1939TP_STATE_ETP_DATA_CONF)
      {
        if (txSduState->Sequence == txSduState->NextBlock)
        {
          txSduState->PacketOffset += txSduState->Sequence;
          txSduState->Sequence = J1939TP_TX_SEQUENCE_INITIAL;

          txSduState->TimeOut = J1939Tp_GetTime_T3();
          /* protected against concurrent transmission failure and early Tx confirmation timeout */
          txSduState->State = J1939TP_STATE_ETP_CTS;
          SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
        else
        {
          txSduState->Sequence++;
          txSduState->TimeOut = J1939Tp_GetTime_Tr();
          /* protected against concurrent transmission failure and early Tx confirmation timeout */
          txSduState->State = J1939TP_STATE_ETP_DATA_BUSY;
          SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          J1939Tp_EtpInternalTransmitEtpDt(sduIdx);
        }
      }
      else
      {
        SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
  }
# endif
# if ((J1939TP_ETP_TX_ENABLED == STD_ON) && (J1939TP_ETP_RX_ENABLED == STD_ON))
  else
# endif
# if (J1939TP_ETP_RX_ENABLED == STD_ON)
  {
    J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    switch (rxSduState->State)
    {
    case J1939TP_STATE_ETP_CTSWAIT_CONF:
      rxSduState->TimeOut = J1939Tp_GetTime_Th();
      /* protected against concurrent transmission failure and early Tx confirmation timeout */
      rxSduState->State = J1939TP_STATE_ETP_CTSWAIT;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      break;

    case J1939TP_STATE_ETP_CTS_CONF:
      rxSduState->TimeOut = J1939Tp_GetTime_T2();
      /* protected against concurrent transmission failure and early Tx confirmation timeout */
      rxSduState->State = J1939TP_STATE_ETP_DPO;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      break;

    case J1939TP_STATE_ETP_EOMACK_CONF:
      /* protected against early Tx confirmation timeout */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* InSession is active before the current state is reached */
      /*! \trace SPEC-2167304 */
      PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_OK);

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
      break;

    case J1939TP_STATE_ETP_ABORT_CONF:
      /* protected against concurrent transmission failure and early Tx confirmation timeout */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
      }

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
      break;

    default:
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      break;
    }
  }
# endif

  J1939TP_DUMMY_STATEMENT(pduType); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
  J1939TP_DUMMY_STATEMENT(sduType); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
} /* J1939Tp_EtpTxConfirmation */ /* PRQA S 6030 */ /* MD_MSR_STCYC */


/**********************************************************************************************************************
 *  J1939Tp_EtpRxIndication
 *********************************************************************************************************************/
/*! \brief      This functions sends the Rx indication.
 *
 *  \param[in]  const PduIdType pduIdx,
 *  \param[in]  P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
 *  \param[in]  const J1939Tp_NPduType pduType
 *  \context    Function could be called from interrupt level or from task level
 *  \note       TODO
 *********************************************************************************************************************/

FUNC(void, J1939TP_CODE) J1939Tp_EtpRxIndication(PduIdType pduIdx,
                                                 P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
                                                 J1939Tp_PduTypeOfRxPduType pduType, uint8 sa)
{
# if (J1939TP_CANIF_DLCCHECK == STD_OFF)
  if (info->SduLength == J1939TP_NPDU_LENGTH)
# endif
  {
    uint8 da = info->SduDataPtr[J1939TP_NPDU_DA_POS];

    if ((!J1939Tp_DA_Valid(da)) || (da == J1939TP_BROADCAST_ADDRESS))
    {
      /*! \trace SPEC-2167310 */
      J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_DA);
    }
    else
    {
      if (pduType == J1939TP_ETPCM_PDUTYPEOFRXPDU)
      {
        uint32 pgn = J1939Tp_Get_PGN(info->SduDataPtr[J1939TP_ETPCM_PGN_LO], info->SduDataPtr[J1939TP_ETPCM_PGN_MD],
                                     info->SduDataPtr[J1939TP_ETPCM_PGN_HI]);

        if (!J1939Tp_PGN_Valid(pgn))
        {
          /*! \trace SPEC-2167310 */
          J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_PGN);
        }
        else
        {
          J1939Tp_EtpControlByteType cb = (J1939Tp_EtpControlByteType) info->SduDataPtr[J1939TP_ETPCM_CB];

          switch (cb)
          {
          case J1939TP_ECB_RTS:
# if (J1939TP_ETP_RX_ENABLED == STD_ON)
          {
            uint32 size = J1939Tp_Get_NBT(info->SduDataPtr[J1939TP_ETPCM_NBT_LO],
                                          info->SduDataPtr[J1939TP_ETPCM_NBT_LOMD],
                                          info->SduDataPtr[J1939TP_ETPCM_NBT_HIMD],
                                          info->SduDataPtr[J1939TP_ETPCM_NBT_HI]);


            if (size > J1939TP_ETP_MAX_LENGTH)
            {
              J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_NBT);
            }
            else
            {
              J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
              J1939Tp_RxNSduStateType *rxSduState;

              /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
              if (J1939Tp_EtpInternalRxIndication_AllocateRxChannel(pduIdx, sa, da, pgn, &sduIdx, &rxSduState))
              {
                J1939Tp_EtpInternalRxIndication_ReceiveEtpCmRts(sduIdx, info, sa, da, size, rxSduState);
              }
            }
          }
# endif
            break;

          case J1939TP_ECB_CTS:
# if (J1939TP_ETP_TX_ENABLED == STD_ON)
          {
            J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType sduIdx;
            J1939Tp_TxNSduStateType *txSduState;

            /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
            if (J1939Tp_EtpInternalRxIndication_FindActiveTxChannel(pduIdx, sa, da, &sduIdx, &txSduState))
            {
              J1939Tp_EtpInternalRxIndication_ReceiveEtpCmCts(sduIdx, info, pgn, txSduState);
            }
          }
# endif
            break;

          case J1939TP_ECB_DPO:
# if (J1939TP_ETP_RX_ENABLED == STD_ON)
          {
            J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
            J1939Tp_RxNSduStateType *rxSduState;

            /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
            if (J1939Tp_EtpInternalRxIndication_FindActiveRxChannel(pduIdx, sa, da, &sduIdx, &rxSduState))
            {
              J1939Tp_EtpInternalRxIndication_ReceiveEtpCmDpo(sduIdx, info, pgn, rxSduState);
            }
          }
# endif
            break;

          case J1939TP_ECB_EOMACK:
# if (J1939TP_ETP_TX_ENABLED == STD_ON)
          {
            J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType sduIdx;
            J1939Tp_TxNSduStateType *txSduState;

            /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
            if (J1939Tp_EtpInternalRxIndication_FindActiveTxChannel(pduIdx, sa, da, &sduIdx, &txSduState))
            {
              J1939Tp_EtpInternalRxIndication_ReceiveEtpCmEomack(sduIdx, info, pgn, txSduState);
            }
          }
# endif
            break;

          case J1939TP_ECB_ABORT:
# if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
            if (!J1939Tp_CAR_ETP_Valid(info->SduDataPtr[J1939TP_ETPCM_CAR]))
            {
              J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_CAR);
            }
# endif

# if (J1939TP_ETP_RX_ENABLED == STD_ON)
            {
              J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
              J1939Tp_RxNSduStateType *rxSduState;

              /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
              if (J1939Tp_EtpInternalRxIndication_FindActiveRxChannel(pduIdx, sa, da, &sduIdx, &rxSduState))
              {
                J1939Tp_EtpInternalRxIndication_ReceiveEtpAbortRx(sduIdx, pgn, rxSduState);
              }
            }
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
            {
              J1939Tp_TxSduTxNSduIdxOfTxChannelTxNSduType sduIdx;
              J1939Tp_TxNSduStateType *txSduState;

              /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
              if (J1939Tp_EtpInternalRxIndication_FindActiveTxChannel(pduIdx, sa, da, &sduIdx, &txSduState))
              {
                J1939Tp_EtpInternalRxIndication_ReceiveEtpAbortTx(sduIdx, pgn, txSduState);
              }
            }
# endif

            /* TODO: Report J1939TP_E_IGNORED_ABORT also if the abort refers to a configured but inactive connection */
            break;

          default:
            J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_CONTROL_BYTE);
            break;
          }
        }
      }
# if (J1939TP_ETP_RX_ENABLED == STD_ON)
      else /* pduType == J1939TP_ETPDT_PDUTYPEOFRXPDU */
      {
        J1939Tp_RxSduRxNSduIdxOfRxChannelRxNSduType sduIdx;
        J1939Tp_RxNSduStateType *rxSduState;

        /* PRQA S 3344 1 */ /* MD_J1939Tp_13.2 */
        if (J1939Tp_EtpInternalRxIndication_FindActiveRxChannel(pduIdx, sa, da, &sduIdx, &rxSduState))
        {
          J1939Tp_EtpInternalRxIndication_ReceiveEtpDt(sduIdx, info, rxSduState);
        }
      }
# endif
    }
  }
# if ((J1939TP_CANIF_DLCCHECK == STD_OFF) && (J1939TP_RUNTIME_ERROR_REPORT == STD_ON))
  else
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_DLC);
  }
# endif
} /* End of J1939Tp_EtpRxIndication() */ /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

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

 MD_J1939Tp_13.7_Etp
      Reason:     Depending on the configured PduLengthType, the check is always false.
      Risk:       When PduLengthType is uint8 or uint16, depending on compiler optimizations dead code might be included.
      Prevention: Set PduLengthType to uint32, or enable appropriate compiler optimizations.

 MD_J1939Tp_14.1_3201_Etp
      Reason:     Depending on the configured PduLengthType, this code is never executed.
      Risk:       When PduLengthType is uint8 or uint16, depending on compiler optimizations dead code might be included.
      Prevention: Set PduLengthType to uint32, or enable appropriate compiler optimizations.

 MD_J1939Tp_3199
      Reason:     Dummy statement, used to avoid compiler warnings.
      Risk:       None
      Prevention: N/A
*/

/* COV_JUSTIFICATION_BEGIN

\ID COV_J1939TP_ETP_SWITCHCASE_DEFAULT
  \ACCEPT X
  \REASON Switch case has a default part for MISRA checks which is not reachable.

COV_JUSTIFICATION_END */
