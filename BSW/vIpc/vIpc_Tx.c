/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vIpc_Tx.c
 *        \brief  vIpc_Tx source file
 *
 *      \details  This file contains the functions required to transmit vIpc frames.
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

#define VIPC_TX_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vIpc_Tx.h"
#include "vIpc_PQ.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define VIPC_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VIPC_LOCAL CONST(uint32, VIPC_CONST) vIpc_CopyTxData_FirstCallMinSize = 8u;

#define VIPC_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define VIPC_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VIPC_LOCAL CONST(uint8, VIPC_CONST) vIpc_HeaderType2HeaderLen[4] =
{
  VIPC_MSG_HEADER_SIZE_SF,
  VIPC_MSG_HEADER_SIZE_FF,
  VIPC_MSG_HEADER_SIZE_CF,
  VIPC_MSG_HEADER_SIZE_LF
};  /* PRQA S 3218 */ /* MD_vIpc_8.7 */

#define VIPC_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_Tx_CalcTotalSduLength()
 *********************************************************************************************************************/
/*! \brief        Calculates the total length of requested Tx data.
 *  \details      Calculates the total length of requested Tx data (requested Tx data + all required vIpc header).
 *  \param[in]    TxConId       The connection ID for which the complete Tx length is calculated.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_Tx_CalcTotalSduLength(PduIdType TxConId);

/**********************************************************************************************************************
 * vIpc_Tx_InternalTransmit()
 *********************************************************************************************************************/
/*! \brief        Handles transmission request.
 *  \details      Enques the transmission request if not already queued and forwards transmission to lower layer if
 *                lower layer not busy.
 *  \param[in]    TxConId       The connection ID for which the transmission request is handled.
 *  \param[inout] PduInfoPtr    The pointer to the transmission data.
 *  \return       E_OK          Transmission request handling successfully.
 *  \return       E_NOT_OK      Transmission request handling failed.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_InternalTransmit(PduIdType TxConId,
                                                                           P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 * vIpc_Tx_SetNextMsgType()
 *********************************************************************************************************************/
/*! \brief        Copies the current vIpc header to the lower layer's transmit data.
 *  \details      Copies the current vIpc header (for SF, FF, CF or LF) to lower layer.
 *  \param[in]    TxConId       The connection ID for which the message type has to be changed.
 *  \param[in]    Event         The event that changes the next message type.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_SetNextMsgType(PduIdType TxConId, vIpc_MsgEventType Event);

/**********************************************************************************************************************
 * vIpc_Tx_SetNextMsgType_AfterCopyHeader()
 *********************************************************************************************************************/
/*! \brief        Copies the current vIpc header to the lower layer's transmit data.
 *  \details      Copies the current vIpc header (for SF, FF, CF or LF) to lower layer.
 *  \param[in]    TxConId       The connection ID for which the message type has to be changed.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_SetNextMsgType_AfterCopyHeader(PduIdType TxConId);

/**********************************************************************************************************************
 * vIpc_Tx_CopyHeader()
 *********************************************************************************************************************/
/*! \brief        Copies the current vIpc header to the lower layer's transmit data.
 *  \details      Copies the current vIpc header (for SF, FF, CF or LF) to lower layer.
 *  \param[in]    TxConId       The connection ID for which the header has to be copied.
 *  \param[inout] PduInfoPtr    The pointer to the transmission data.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          The PduInfoPtr points to first element for this segment.
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_CopyHeader(PduIdType TxConId,
                                                                     P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 * vIpc_Tx_GetRemTxDataOfCurrentSegment()
 *********************************************************************************************************************/
/*! \brief        Returns the remaining data of currently active segment to be transmitted.
 *  \details      -
 *  \param[in]    TxConId       The connection ID for which the remaining data is calculated.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_Tx_GetRemTxDataOfCurrentSegment(PduIdType TxConId);

/**********************************************************************************************************************
 * vIpc_Tx_IsPduInfoPtrValidForCopyTxData()
 *********************************************************************************************************************/
/*! \brief        Returns if the PDU info pointer is valid.
 *  \details      -
 *  \param[in]    PduInfoPtr    The pointer to be checked for validity.
 *  \return       FALSE         The pointer is invalid.
 *                TRUE          The pointer is valid.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_Tx_IsPduInfoPtrValidForCopyTxData(
    CONSTP2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 * vIpc_Tx_ResetConnection()
 *********************************************************************************************************************/
/*! \brief        Resets the Tx connection
 *  \details      -
 *  \param[in]    PduIdType     The connection to be reset
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_Tx_ResetConnection(PduIdType conId);

/**********************************************************************************************************************
 * vIpc_Tx_ConfirmTransmission()
 *********************************************************************************************************************/
/*! \brief        Confirms the transmission to the upper layer
 *  \details      -
 *  \param[in]    PduIdType       The pointer to be checked for validity.
 *  \param[in]    Std_ReturnType  Determines a successful or unsuccessful transmission
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      tx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_Tx_ConfirmTransmission(PduIdType conId, Std_ReturnType result);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_Tx_CalcTotalSduLength()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_Tx_CalcTotalSduLength(PduIdType TxConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType pduId         = vIpc_XCfg_GetTxChannelIdxOfTxConnections(TxConId);
  PduLengthType maxChnLen = vIpc_XCfg_GetMaxLengthOfTxChannel(pduId);
  PduLengthType completeLength;
  vIpc_MsgEventType msgEvent;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Given the requested length and the SF header fit in a single frame. */
  /* #15 Then the message can be transmitted unsegmented. */
  if ((vIpc_XCfg_TxCon_GetRemPayloadLength(TxConId) + VIPC_MSG_HEADER_SIZE_SF)
      <= maxChnLen) /* TODO: Check if < or <= is correct! */
  {
    completeLength =
      (PduLengthType)(vIpc_XCfg_TxCon_GetRemPayloadLength(TxConId) + (PduLengthType) VIPC_MSG_HEADER_SIZE_SF);
    msgEvent = VIPC_MSG_EVENT_START_SF;
  }
  /* #20 Otherwise, calculate the complete length: */
  else
  {
    PduLengthType tempPduLen = vIpc_XCfg_TxCon_GetRemPayloadLength(TxConId) - (maxChnLen - VIPC_MSG_HEADER_SIZE_FF);

    /* #30 Given that the Tx data fits in first frame and last frame. */
    /* #35 Then no consecutive frame is required; calculate complete length. */
    if (tempPduLen <= (maxChnLen - VIPC_MSG_HEADER_SIZE_LF))
    {
      completeLength = (PduLengthType)(maxChnLen + tempPduLen + VIPC_MSG_HEADER_SIZE_LF);
    }
    /* #40 Otherwise, calculate number of consecutive frames and length of last frame to calculate the complete length. */
    else
    {
      PduLengthType numberOfConsecFrames = tempPduLen / (maxChnLen - VIPC_MSG_HEADER_SIZE_CF);
      PduLengthType lengthOfLastFrame    = tempPduLen % (maxChnLen - VIPC_MSG_HEADER_SIZE_CF);

      if (lengthOfLastFrame > 0)
      {
        lengthOfLastFrame += VIPC_MSG_HEADER_SIZE_LF;
      }

      completeLength = (((PduLengthType)(numberOfConsecFrames + 1 /* For First Frame */)) * maxChnLen) + lengthOfLastFrame;
    }

    msgEvent = VIPC_MSG_EVENT_START_FF;
  }

  vIpc_XCfg_TxCon_SetRemCompleteLength(TxConId, completeLength);
  vIpc_Tx_SetNextMsgType(TxConId, msgEvent);
} /* vIpc_Tx_CalcTotalSduLength */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_Tx_InternalTransmit()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_InternalTransmit(PduIdType TxConId,
                                                                           P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the new transmission request is for an idle connection. */
  if (vIpc_XCfg_TxCon_GetState(TxConId) == VIPC_TX_CON_STATE_IDLE)
  {
    PduIdType chanId = vIpc_XCfg_GetTxChannelIdxOfTxConnections(TxConId);

    vIpc_XCfg_TxCon_SetRemPayloadLength(TxConId, PduInfoPtr->SduLength);
    vIpc_XCfg_TxCon_SetCurrentSegment(TxConId, 0);

    /* #20 Then calculate the full length of data including vIpc headers. */
    vIpc_Tx_CalcTotalSduLength(TxConId);

    /* #30 Enqueue the requested transmission with SDU ID. */
    vIpc_PQ_Insert(TxConId);
    vIpc_XCfg_TxCon_SetState(TxConId, VIPC_TX_CON_STATE_QUEUED);

    /* #40 If the lower layer is not busy, trigger transmission on lower layer. */
    if (vIpc_XCfg_IsLLBusy(chanId) == FALSE)
    {
      vIpc_Tx_Process(chanId);
    }

    retVal = E_OK;
  }
  /* #50 Otherwise, indicate error. */

  return retVal;
} /* vIpc_Tx_InternalTransmit */

/**********************************************************************************************************************
 * vIpc_Tx_SetNextMsgType()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_SetNextMsgType(PduIdType TxConId, vIpc_MsgEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal           = E_OK;
  vIpc_MessageType newMsgType     = VIPC_MSG_SF;
  vIpc_MessageType currentMsgType = vIpc_XCfg_TxCon_GetMessageType(TxConId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the message type has to be reset, set it to SF. */
  if (Event == VIPC_MSG_EVENT_RESET)
  {
    /* Next message type is already set to SF. */
  }
  /* #20 Otherwise determine new msg type based on current msg type and event: */
  else
  {
    switch (currentMsgType)
    {
      /* #30 If the current message type is SF:
       *       If the event start SF occurs, the message type remains.
       *       Otherwise if the event start FF occurs, set the message type to FF.
       *       Otherwise, reset the message type to SF and indicate an error. */
      case (VIPC_MSG_SF):
        if (Event == VIPC_MSG_EVENT_START_SF)
        {
          /* It is already set to SF. */
        }
        else if (Event == VIPC_MSG_EVENT_START_FF)
        {
          newMsgType = VIPC_MSG_FF;
        }
        else
        {
          retVal = E_NOT_OK;
        }
        break;

      /* #40 If the current message type is FF or CF:
       *       If the event start CF occurs, set the message type to CF.
       *       Otherwise if the event start LF occurs, set the message type to LF.
       *       Otherwise, reset the message type to SF and indicate an error. */
      case (VIPC_MSG_FF):
      case (VIPC_MSG_CF):
        if (Event == VIPC_MSG_EVENT_START_CF)
        {
          newMsgType = VIPC_MSG_CF;
        }
        else if (Event == VIPC_MSG_EVENT_START_LF)
        {
          newMsgType = VIPC_MSG_LF;
        }
        else
        {
          retVal = E_NOT_OK;
        }
        break;

      /* #50 If the current message type is LF or invalid:
       *       Reset the message type to SF and indicate an error. */
      default:
        retVal = E_NOT_OK;
        break;
    }
  }

  vIpc_XCfg_TxCon_SetMessageType(TxConId, newMsgType);

  return retVal;
} /* vIpc_Tx_SetNextMsgType */

/**********************************************************************************************************************
 * vIpc_Tx_SetNextMsgType_AfterCopyHeader()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_SetNextMsgType_AfterCopyHeader(PduIdType TxConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal           = E_OK;
  PduIdType chanId                = vIpc_XCfg_GetTxChannelIdxOfTxConnections(TxConId);
  vIpc_MsgEventType eventMsg      = VIPC_MSG_EVENT_RESET;
  vIpc_MessageType currentMsgType = vIpc_XCfg_TxCon_GetMessageType(TxConId);

  /* ----- Implementation ----------------------------------------------- */
  switch (currentMsgType)
  {
    /* #10 If current message is a SF or a LF, reset next message type to SF. */
    case (VIPC_MSG_SF):
    case (VIPC_MSG_LF):
      break;

    /* #20 Otherwise, if this is a FF or CF:
     *       And the remaining Tx data does not fit in a LF, set type of next message to CF.
     *       Otherwise, set type of next message to LF. */
    case (VIPC_MSG_FF):
    case (VIPC_MSG_CF):
      if (vIpc_XCfg_TxCon_GetRemCompleteLength(TxConId)
          > (PduLengthType)(2 * vIpc_XCfg_GetMaxLengthOfTxChannel(chanId)))
      {
        eventMsg = VIPC_MSG_EVENT_START_CF;
      }
      else
      {
        eventMsg = VIPC_MSG_EVENT_START_LF;
      }
      break;

    /* #40 Otherwise, indicate failure and reset message type to SF. */
    default:
      retVal = E_NOT_OK;
      break;
  }

  vIpc_Tx_SetNextMsgType(TxConId, eventMsg);

  return retVal;
} /* vIpc_Tx_SetNextMsgType_AfterCopyHeader */

/**********************************************************************************************************************
 * vIpc_Tx_CopyHeader()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_Tx_CopyHeader(PduIdType TxConId,
                                                                     P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal           = E_OK;
  PduIdType chanId                = vIpc_XCfg_GetTxChannelIdxOfTxConnections(TxConId);
  vIpc_MessageType currentMsgType = vIpc_XCfg_TxCon_GetMessageType(TxConId);
  uint8 address                   = vIpc_XCfg_GetTxAddressOfTxConnections(TxConId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy address and message type to lower layer's transmit buffer. */
  vIpc_Msg_SetAddress(PduInfoPtr, address);
  vIpc_Msg_SetType(PduInfoPtr, currentMsgType);

  switch (currentMsgType)
  {
    /* #20 If this is a SF or a LF, copy no more data and indicate success. */
    case (VIPC_MSG_SF):
    case (VIPC_MSG_LF):
      break;

    /* #30 Otherwise, if this is the first frame:
     *       Copy the overall payload length of this SDU to lower layer's transmit buffer. */
    case (VIPC_MSG_FF):
      vIpc_Msg_SetLength(PduInfoPtr, vIpc_XCfg_TxCon_GetRemPayloadLength(TxConId));
      break;

    /* #40 Otherwise, if this is a consecutive frame:
      *       Copy sequence counter to lower layer's transmit buffer. */
    case (VIPC_MSG_CF):
      vIpc_Msg_SetSeqCtr(PduInfoPtr, vIpc_XCfg_TxCon_GetCurrentSegment(TxConId));

      if (vIpc_XCfg_TxCon_GetRemCompleteLength(TxConId)
          > (PduLengthType)(2 * vIpc_XCfg_GetMaxLengthOfTxChannel(chanId)))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
      {
        vIpc_XCfg_TxCon_IncCurrentSegment(TxConId, 1);
      }
      break;

    /* #50 Otherwise, indicate failure. */
    default:
      retVal = E_NOT_OK;
      break;
  }

  return retVal;
} /* vIpc_Tx_CopyHeader */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_Tx_GetRemTxDataOfCurrentSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_Tx_GetRemTxDataOfCurrentSegment(PduIdType TxConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType remTxDataOfSegment;
  PduLengthType remCompleteLength  = vIpc_XCfg_TxCon_GetRemCompleteLength(TxConId);
  PduIdType chanId                 = vIpc_XCfg_GetTxChannelIdxOfTxConnections(TxConId);
  PduLengthType maxLengthOfChannel = vIpc_XCfg_GetMaxLengthOfTxChannel(chanId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Given that a whole segment has already been sent */
  if (vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg(chanId) >= maxLengthOfChannel)
  {
    /* #11 Then the remaining data is zero */
    remTxDataOfSegment = 0;
  }
  /* #20 Or given that the remaining length of the connection is at least as large as the max size of a channel */
  else if (remCompleteLength >= maxLengthOfChannel)
  {
    /* #21 Then the remaining data in the segment is the size of the channel without the already sent data */
    remTxDataOfSegment = maxLengthOfChannel - vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg(chanId);
  }
  else
  {
    /* #30 Else the remaining segment length is equal to the remaining complete length */
    remTxDataOfSegment = remCompleteLength;
  }

  return remTxDataOfSegment;
} /* vIpc_Tx_GetRemTxDataOfCurrentSegment */

/**********************************************************************************************************************
 * vIpc_Tx_IsPduInfoPtrValidForCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_Tx_IsPduInfoPtrValidForCopyTxData(
    CONSTP2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the PDU pointer is no NULL pointer:
   *       If the LL request number of available transmit data, PDU pointer is valid.
   *       Otherwise, if the data pointer points to valid data, PDU pointer is valid.
   *     Otherwise, the PDU pointer is invalid. */
  if (PduInfoPtr != NULL_PTR)
  {
    if (PduInfoPtr->SduLength == 0)
    {
      retVal = TRUE;
    }
    else if (PduInfoPtr->SduDataPtr != NULL_PTR)
    {
      retVal = TRUE;
    }
    else
    {
      retVal = FALSE;
    }
  }

  return retVal;
} /* vIpc_Tx_IsPduInfoPtrValidForCopyTxData */

/**********************************************************************************************************************
 * vIpc_Tx_ResetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_Tx_ResetConnection(PduIdType conId)
{
  vIpc_PQ_Remove(conId);
  vIpc_Tx_SetNextMsgType(conId, VIPC_MSG_EVENT_RESET);
  vIpc_XCfg_TxCon_SetState(conId, VIPC_TX_CON_STATE_IDLE);
  vIpc_XCfg_TxCon_SetCurrentSegment(conId, 0);
  vIpc_XCfg_TxCon_SetRemCompleteLength(conId, (PduLengthType) 0);
  vIpc_XCfg_TxCon_SetRemPayloadLength(conId, 0);
}

/**********************************************************************************************************************
 * vIpc_Tx_ConfirmTransmission()
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_Tx_ConfirmTransmission(PduIdType conId, Std_ReturnType Result)
{
  vIpc_XCfg_GetTxConfirmationOfTxUlCallbacks(conId)(vIpc_XCfg_GetUpperLayerIdOfTxConnections(conId), Result);
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_Tx_Process()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_Tx_Process(PduIdType TxChannel)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType pdu;
  PduIdType conId;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the queued connection with the highest priority. */
  retVal = vIpc_PQ_Peek(TxChannel, &conId);

  /* #20 If there was a queued transmission request. */
  if (retVal == E_OK)
  {
    pdu.SduDataPtr = NULL_PTR;

    /* #30 Set new connection active. */
    vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg(TxChannel, 0);
    vIpc_XCfg_TxChan_SetActiveTxConId(TxChannel, conId);
    vIpc_XCfg_TxChan_SetIsActiveTxConAvailable(TxChannel, TRUE);

    /* #40 If the SDU length fits in a single frame, request sending this data. */
    if (vIpc_XCfg_TxCon_GetRemCompleteLength(conId) <= vIpc_XCfg_GetMaxLengthOfTxChannel(TxChannel))
    {
      pdu.SduLength = vIpc_XCfg_TxCon_GetRemCompleteLength(conId);
    }
    /* #50 Otherwise:
     *       Set the transmission length to the max. length of lower layer.
     *       Trigger transmission and re-enqueue the connection transmission request. */
    else
    {
      pdu.SduLength = vIpc_XCfg_GetMaxLengthOfTxChannel(TxChannel);
    }

    if (pdu.SduLength == 0)
    {
      retVal = E_NOT_OK;
    }
    else
    {
      vIpc_XCfg_TxCon_SetState(conId, VIPC_TX_CON_STATE_ACTIVE_TX);
      retVal = vIpc_XCfg_GetTransmitOfTransmitLLFunctionTable(TxChannel)(
        vIpc_XCfg_GetLowerLayerIdOfTxChannel(TxChannel), &pdu);
    }

    if (retVal == E_NOT_OK)
    {
      /* TODO: Test this! */
      vIpc_XCfg_TxChan_SetIsActiveTxConAvailable(TxChannel, FALSE);
      vIpc_XCfg_TxCon_SetState(conId, VIPC_TX_CON_STATE_QUEUED);
    }
  }
} /* vIpc_Tx_Process */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, VIPC_CODE) vIpc_Transmit(PduIdType TxSduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  vIpc_ErrorCode errorId = VIPC_E_NO_ERROR;
  CONST(boolean, VIPC_CONST) isDetEnabled = vIpc_XCfg_DevErrorDetect();

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 If vIpc is not initialized or an invalid parameter is given, report an error. */
  if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_NOT_INITIALIZED;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsTxSduIdValid(TxSduId) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_SDU_ID;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsPduDataValid(PduInfoPtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_POINTER;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsPduDataLenValid(PduInfoPtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_CONFIG;
  }
  /* #20 Otherwise, forward the transmission request. */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = vIpc_Tx_InternalTransmit(TxSduId, PduInfoPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
  retVal |= vIpc_XCfg_ReportError(VIPC_SID_TRANSMIT, errorId);

  return retVal;
} /* vIpc_Transmit */

/**********************************************************************************************************************
 * vIpc_CopyTxData()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_CopyTxData(PduIdType TxPduId,
                                                   P2VAR(PduInfoType, AUTOMATIC, VIPC_APPL_VAR) PduInfoPtr,
                                                   P2VAR(RetryInfoType, AUTOMATIC, VIPC_APPL_VAR) RetryPtr,
                                                   P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) AvailableDataPtr) /* PRQA S 3673 */ /* MD_MSR_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  vIpc_ErrorCode errorId   = VIPC_E_NO_ERROR;
  boolean isDetEnabled     = vIpc_XCfg_DevErrorDetect();

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 If vIpc is not initialized or an invalid parameter is given, report an error. */
  if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_NOT_INITIALIZED;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsTxPduIdValid(TxPduId) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_PDU_ID;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_Tx_IsPduInfoPtrValidForCopyTxData(PduInfoPtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_POINTER;
  }
  /* #20 Otherwise determine correct parameters and perform copy operation if applicable: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If there is an active connection for transmission: */
    if (vIpc_XCfg_TxChan_GetIsActiveTxConAvailable(TxPduId) == TRUE)
    {
      PduIdType conId = vIpc_XCfg_TxChan_GetActiveTxConId(TxPduId);

      /* #40 If the LL indicates currently no buffer available:
       *       Forward CopyTxData to UL and indicate LL how much data can be copied. */
      if (PduInfoPtr->SduLength == 0)
      {
        retVal = vIpc_XCfg_GetCopyTxDataOfTxUlCallbacks(conId)(vIpc_XCfg_GetUpperLayerIdOfTxConnections(conId),
                                                               PduInfoPtr,
                                                               RetryPtr,
                                                               AvailableDataPtr);

        AvailableDataPtr[0] = vIpc_Tx_GetRemTxDataOfCurrentSegment(conId);
      }
      /* #50 Otherwise continue checks: */
      else
      {
        PduLengthType alreadySentData = vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg(TxPduId);

        /* #60 If there is not enough Tx buffer provided by the lower layer for the header, indicate an error. */
        if ((alreadySentData == 0) && (PduInfoPtr->SduLength < vIpc_CopyTxData_FirstCallMinSize))
        {
          errorId = VIPC_E_INV_CONFIG;
        }
        /* #70 Otherwise forward call to perform copy operation: */
        else
        {
          PduInfoType locPduInfoPtr;
          PduLengthType headerLength = 0u;

          /* #80 If there is a new segment to be sent, firstly reserve the place for the vIpc header. */
          if (alreadySentData == 0)
          {
            headerLength = vIpc_HeaderType2HeaderLen[(uint8)(vIpc_XCfg_TxCon_GetMessageType(conId))];
          }
          locPduInfoPtr.SduDataPtr = &PduInfoPtr->SduDataPtr[headerLength];
          locPduInfoPtr.SduLength  = PduInfoPtr->SduLength - headerLength;

          /* #90 Forward the remaining Tx buffer space for copying data by upper layer. */
          retVal = vIpc_XCfg_GetCopyTxDataOfTxUlCallbacks(conId)(vIpc_XCfg_GetUpperLayerIdOfTxConnections(conId),
                                                                 &locPduInfoPtr,
                                                                 RetryPtr,
                                                                 AvailableDataPtr);

          /* #100 If the upper layer succeeded:
           *        If there is a new segment to be sent, firstly copy vIpc header.
           *        Calculate remaining data to be copied.
           *        If complete data is copied, set the connection state to VIPC_TX_CON_STATE_WAIT_FOR_TX_CONF. */
          if (retVal == BUFREQ_OK)
          {
            PduLengthType remainingData;

            if (alreadySentData == 0)
            {
              (void) vIpc_Tx_CopyHeader(conId, PduInfoPtr);
              (void) vIpc_Tx_SetNextMsgType_AfterCopyHeader(conId);
            }

            vIpc_XCfg_TxChan_IncAlreadySentDataOfCurrSeg(TxPduId, PduInfoPtr->SduLength);
            vIpc_XCfg_TxCon_DecRemCompleteLength(conId, PduInfoPtr->SduLength);
            vIpc_XCfg_TxCon_DecRemPayloadLength(conId, locPduInfoPtr.SduLength);

            remainingData       = vIpc_Tx_GetRemTxDataOfCurrentSegment(conId);
            AvailableDataPtr[0] = remainingData;

            if (remainingData == 0)
            {
              vIpc_XCfg_TxCon_SetState(conId, VIPC_TX_CON_STATE_WAIT_FOR_TX_CONF);
            }
          }
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  (void) vIpc_XCfg_ReportError(VIPC_SID_COPYTXDATA, errorId);

  VIPC_DUMMY_STATEMENT(RetryPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return retVal;
} /* vIpc_CopyTxData */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */  /* PRQA 6080 1 */ /* MD_MSR_STMIF */ /* PRQA 6030 1 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 * vIpc_TxConfirmation()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_TxConfirmation(PduIdType TxPduId, Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  vIpc_ErrorCode errorId = VIPC_E_NO_ERROR;
  boolean isDetEnabled   = vIpc_XCfg_DevErrorDetect();
  PduIdType conId        = vIpc_XCfg_TxChan_GetActiveTxConId(TxPduId);

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Given Development Errors are enabled 
   *       And vIpc is not initialized
   *       Or an invalid parameter is given
   *         Then report an error. */
  if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_NOT_INITIALIZED;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsTxPduIdValid(TxPduId) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_PDU_ID;
  }
  else
  {
    /* #20 Otherwise determine correct parameters and perform TxConfirmation if applicable: */

    /* ----- Implementation ----------------------------------------------- */
    vIpc_XCfg_TxChan_InitRemainingSeparationCycles(TxPduId);
    vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg(TxPduId, 0);
    vIpc_XCfg_TxChan_SetActiveTxConId(TxPduId, 0);
    vIpc_XCfg_TxChan_SetIsActiveTxConAvailable(TxPduId, FALSE);

    /* #30 Check if we expected a Tx Confirmation */
    Result |= (Std_ReturnType)(vIpc_XCfg_TxCon_GetState(conId) != VIPC_TX_CON_STATE_WAIT_FOR_TX_CONF);

    /* #40 Given the lower layer successfully transmitted a segment
           And there is still data to be sent */
    if ((Result == E_OK) && (vIpc_XCfg_TxCon_GetRemPayloadLength(conId) > 0))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
    {
      /* #50 Then re-schedule this connection. */
      vIpc_XCfg_TxCon_SetState(conId, VIPC_TX_CON_STATE_QUEUED);
    }
    else
    {
      /* #60 Otherwise, reset connection data and forward the TxConfirmation to indicate the UL that the
       *     transmission has terminated. */
      vIpc_Tx_ResetConnection(conId);

      vIpc_Tx_ConfirmTransmission(conId, Result);
    }

    /* #70 Given the channel is allowed to transmit immediately */
    if (vIpc_XCfg_GetNumSepCyclesOfTxChannel(TxPduId) == 0)
    {
      /* #71 Then trigger the next transmission */
      vIpc_Tx_Process(TxPduId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  (void) vIpc_XCfg_ReportError(VIPC_SID_TXCONFIRMATION, errorId);
} /* vIpc_TxConfirmation */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  END OF FILE: vIpc_Tx.c
 *********************************************************************************************************************/
