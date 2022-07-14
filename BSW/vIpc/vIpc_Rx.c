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
/**        \file  vIpc_Rx.c
 *        \brief  vIpc_Rx source file
 *
 *      \details  This file contains the functions required to receive vIpc frames.
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

#define VIPC_RX_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vIpc_Rx.h"

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

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_Rx_Process_SF()
 *********************************************************************************************************************/
/*! \brief        Processes the start of reception for single frames (SF).
 *  \details      -
 *  \param[in]    RxPduId       The Rx channel on which the data was received.
 *  \param[inout] PduInfoPtr    The pointer to the received data.
 *  \param[in]    TpSduLength   The length of received data.
 *  \param[out]   BufferSizePtr The pointer to the available buffer.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      rx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_SF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 * vIpc_Rx_Process_FF()
 *********************************************************************************************************************/
/*! \brief        Processes the start of reception for first frames (FF).
 *  \details      -
 *  \param[in]    RxPduId       The Rx channel on which the data was received.
 *  \param[inout] PduInfoPtr    The pointer to the received data.
 *  \param[in]    TpSduLength   The length of received data.
 *  \param[out]   BufferSizePtr The pointer to the available buffer.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      rx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_FF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 * vIpc_Rx_Process_CF()
 *********************************************************************************************************************/
/*! \brief        Processes the start of reception for consecutive frames (CF).
 *  \details      -
 *  \param[in]    RxPduId       The Rx channel on which the data was received.
 *  \param[inout] PduInfoPtr    The pointer to the received data.
 *  \param[in]    TpSduLength   The length of received data.
 *  \param[out]   BufferSizePtr The pointer to the available buffer.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      rx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_CF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 * vIpc_Rx_Process_LF()
 *********************************************************************************************************************/
/*! \brief        Processes the start of reception for last frames (LF).
 *  \details      -
 *  \param[in]    RxPduId       The Rx channel on which the data was received.
 *  \param[inout] PduInfoPtr    The pointer to the received data.
 *  \param[in]    TpSduLength   The length of received data.
 *  \param[out]   BufferSizePtr The pointer to the available buffer.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \ingroup      rx
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_LF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr);

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
};

#define VIPC_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * vIpc_Rx_Process_SF()
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
 */
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_SF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType ULInfoPtr;
  PduLengthType bufSize;
  PduIdType ulPduId;
  PduIdType sduId;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 headerLen;

  /* ----- Implementation ----------------------------------------------- */
  headerLen = vIpc_HeaderType2HeaderLen[VIPC_MSG_SF];
  /* #10 Received address is in valid range, store intermediate sdu id for later use in CopyRxData and RxIndication. */
  sduId = (PduIdType)((PduIdType) vIpc_XCfg_GetAddressStartIdxOfRxChannel(RxPduId)
                      + vIpc_Msg_GetAddress(PduInfoPtr));
  ulPduId = vIpc_XCfg_GetUpperLayerIdOfRxConnections(sduId);
  vIpc_XCfg_SetRxPduConnId(RxPduId, sduId);

  /* #20 First check if we are in the correct state */
  if (vIpc_XCfg_GetRxCon_State(sduId) != VIPC_RX_SDU_STATE_IDLE)
  {
    /* #30 We received an incomplete sequence. Give RxIndication with E_NOT_OK to close it. */
    vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(sduId)(ulPduId, E_NOT_OK);
    vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_IDLE);
  }

  /* #40 Forward start of reception minus vIpc header information. */
  ULInfoPtr.SduDataPtr = &PduInfoPtr->SduDataPtr[headerLen];
  ULInfoPtr.SduLength  = PduInfoPtr->SduLength - headerLen;
  bufSize              = 0;

  /* #50 Memorize TP API params for later use in CopyRxData and RxIndication. */
  vIpc_XCfg_SetSeqCtr(sduId, 0u);
  vIpc_XCfg_SetRxPduTpCount(RxPduId, 0u);
  vIpc_XCfg_SetRxPayloadLength(sduId, ULInfoPtr.SduLength);
  vIpc_XCfg_SetRxRemainingPayloadLength(sduId, ULInfoPtr.SduLength);

  retVal = vIpc_XCfg_GetStartOfReceptionOfRxUlCallbacks(sduId)(ulPduId, &ULInfoPtr, TpSduLength - headerLen, &bufSize);
  /* #60 Multiple headerLen might be required. This can not be calculated here as the number of segments is missing.
   *     A single header will probably sufficient to satisfy the lower layer.
   *     If the upper layer returned a size of 0 we do not add the vIpc header information, but return the zero unmodified.
   */
  if (bufSize > 0)
  {
    *BufferSizePtr = bufSize + headerLen;
  }
  else
  {
    *BufferSizePtr = 0u;
  }

  if (retVal == BUFREQ_OK)
  {
    vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_WAIT_FOR_RX_IND);
    vIpc_XCfg_SetFrameType(sduId, VIPC_MSG_SF);
  }
  return (retVal);
} /* vIpc_Rx_Process_SF */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_Rx_Process_FF()
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
 */
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_FF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType ULInfoPtr;
  PduLengthType bufSize;
  PduIdType ulPduId;
  PduIdType sduId;
  BufReq_ReturnType retVal;
  uint8 headerLen;

  /* ----- Implementation ----------------------------------------------- */
  headerLen = vIpc_HeaderType2HeaderLen[VIPC_MSG_FF];
  /* #10 Received address is in valid range, store intermediate sdu id for later use in CopyRxData and RxIndication. */
  sduId = (PduIdType)((PduIdType) vIpc_XCfg_GetAddressStartIdxOfRxChannel(RxPduId)
                      + vIpc_Msg_GetAddress(PduInfoPtr));
  ulPduId = vIpc_XCfg_GetUpperLayerIdOfRxConnections(sduId);
  vIpc_XCfg_SetRxPduConnId(RxPduId, sduId);

  /* #20 First check if we are in the correct state. */
  if (vIpc_XCfg_GetRxCon_State(sduId) != VIPC_RX_SDU_STATE_IDLE)
  {
    /* #30 We received an incomplete sequence. Give RxIndication with E_NOT_OK to close it. */
    vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(sduId)(ulPduId, E_NOT_OK);
    vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_IDLE);
  }
  /* #40 Forward start of reception minus vIpc header information. */
  ULInfoPtr.SduDataPtr = &PduInfoPtr->SduDataPtr[headerLen];
  ULInfoPtr.SduLength  = PduInfoPtr->SduLength - headerLen;
  bufSize              = 0;

  /* #50 Memorize TP API params for later use in CopyRxData and RxIndication. */
  vIpc_XCfg_SetSeqCtr(sduId, 0u);
  vIpc_XCfg_SetRxPduTpCount(RxPduId, 0u);

  {
    uint32 length = vIpc_Msg_GetLength(PduInfoPtr);
    vIpc_XCfg_SetRxPayloadLength(sduId, length);
    vIpc_XCfg_SetRxRemainingPayloadLength(sduId, length);
  }

  retVal = vIpc_XCfg_GetStartOfReceptionOfRxUlCallbacks(sduId)(ulPduId, &ULInfoPtr, TpSduLength - headerLen, &bufSize);
  /* #60 Multiple headerLen might be required. This can not be calculated here as the number of segments is missing.
   *     A single header will probably sufficient to satisfy the lower layer.
   *     If the upper layer returned a size of 0 we do not add the vIpc header information, but return the zero unmodified.
   */
  if (bufSize > 0)
  {
    *BufferSizePtr = bufSize + headerLen;
  }
  else
  {
    *BufferSizePtr = 0u;
  }

  if (retVal == BUFREQ_OK)
  {
    vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_WAIT_FOR_CF);
    vIpc_XCfg_SetFrameType(sduId, VIPC_MSG_FF);
  }
  return (retVal);
} /* vIpc_Rx_Process_FF */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_Rx_Process_CF()
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
VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_CF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bufSize;
  PduIdType ulPduId;
  PduIdType sduId;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 headerLen;

  /* ----- Implementation ----------------------------------------------- */
  VIPC_DUMMY_STATEMENT(TpSduLength);
  headerLen = vIpc_HeaderType2HeaderLen[VIPC_MSG_CF];
  /* #10 Received address is in valid range, store intermediate sdu id for later use in CopyRxData and RxIndication. */
  sduId = (PduIdType)((PduIdType) vIpc_XCfg_GetAddressStartIdxOfRxChannel(RxPduId)
                      + vIpc_Msg_GetAddress(PduInfoPtr));
  ulPduId = vIpc_XCfg_GetUpperLayerIdOfRxConnections(sduId);
  vIpc_XCfg_SetRxPduConnId(RxPduId, sduId);

  /* #20 Are we in the correct state to receive a CF. */
  if (vIpc_XCfg_GetRxCon_State(sduId) == VIPC_RX_SDU_STATE_WAIT_FOR_CF)
  {
    /* #30 Is the sequence counter correct? */
    if (vIpc_XCfg_GetSeqCtr(sduId) == vIpc_Msg_GetSeqCtr(PduInfoPtr))
    {
      PduInfoType ULInfoPtr = {NULL_PTR, 0};

      /* #40 Yes, the sequence is correct, prepare for CopyRxData. */
      vIpc_XCfg_SetSeqCtr(sduId, (vIpc_XCfg_GetSeqCtr(sduId) + 1u) & VIPC_MSG_SEQCTR_MASK);
      vIpc_XCfg_SetRxPduTpCount(RxPduId, 0u);
      bufSize = 0u;
      (void)vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks(vIpc_XCfg_GetRxPduConnId(RxPduId))
                                                   (ulPduId,
                                                    &ULInfoPtr,
                                                    &bufSize);
      *BufferSizePtr = bufSize + headerLen;
      vIpc_XCfg_SetFrameType(sduId, VIPC_MSG_CF);
    }
    else
    {
      /* #50 We received a wrong sequence. Give RxIndication with E_NOT_OK to close it. */
      vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(sduId)(ulPduId, E_NOT_OK);
      vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_IDLE);
      vIpc_XCfg_SetRxRemainingPayloadLength(sduId, 0u);
      *BufferSizePtr = 0;
    }
    retVal = BUFREQ_OK;
  }
  else
  {
    if (vIpc_XCfg_GetRxCon_State(sduId) != VIPC_RX_SDU_STATE_IDLE)
    {
      /* #60 We received an incomplete sequence. Give RxIndication with E_NOT_OK to close it. */
      vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(sduId)(ulPduId, E_NOT_OK);
      vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_IDLE);
      vIpc_XCfg_SetRxRemainingPayloadLength(sduId, 0u);
    }
    *BufferSizePtr = 0;
  }

  return (retVal);
} /* vIpc_Rx_Process_CF */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_Rx_Process_LF()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 VIPC_LOCAL_INLINE FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_Rx_Process_LF(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                              PduLengthType TpSduLength,
                                              P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType bufSize;
  PduIdType ulPduId;
  PduIdType sduId;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 headerLen;

  /* ----- Implementation ----------------------------------------------- */
  VIPC_DUMMY_STATEMENT(TpSduLength);
  headerLen = vIpc_HeaderType2HeaderLen[VIPC_MSG_LF];
  /* #10 Received address is in valid range, store intermediate sdu id for later use in CopyRxData and RxIndication. */
  sduId = (PduIdType)((PduIdType) vIpc_XCfg_GetAddressStartIdxOfRxChannel(RxPduId)
                      + vIpc_Msg_GetAddress(PduInfoPtr));
  ulPduId = vIpc_XCfg_GetUpperLayerIdOfRxConnections(sduId);
  vIpc_XCfg_SetRxPduConnId(RxPduId, sduId);

  vIpc_XCfg_SetRxPduTpCount(RxPduId, 0u);
  /* #20 Are we in the correct state to process the LF? */
  if (vIpc_XCfg_GetRxCon_State(sduId) == VIPC_RX_SDU_STATE_WAIT_FOR_CF)
  {
    PduInfoType ULInfoPtr;

    vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_WAIT_FOR_RX_IND);

    /* #30 Request pending buffer size and forward it to lower layer */
    ULInfoPtr.SduDataPtr = NULL_PTR;
    ULInfoPtr.SduLength  = 0u;
    bufSize              = 0u;
    (void) vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks(vIpc_XCfg_GetRxPduConnId(RxPduId))(ulPduId, &ULInfoPtr, &bufSize);
    *BufferSizePtr = bufSize + headerLen;
    vIpc_XCfg_SetFrameType(sduId, VIPC_MSG_LF);
    retVal = BUFREQ_OK;
  }
  else
  {
    if (vIpc_XCfg_GetRxCon_State(sduId) != VIPC_RX_SDU_STATE_IDLE)
    {
      /* #40 We received an incomplete sequence. Give RxIndication with E_NOT_OK to close it. */
      vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(sduId)(ulPduId, E_NOT_OK);
      vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_IDLE);
    }
    *BufferSizePtr = 0;
  }

  return (retVal);
} /* vIpc_Rx_Process_LF */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_RxIndication()
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
FUNC(void, VIPC_CODE) vIpc_RxIndication(PduIdType RxPduId, Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  vIpc_ErrorCode errorId = VIPC_E_NO_ERROR;
  boolean isDetEnabled   = vIpc_XCfg_DevErrorDetect();

  /* #10 If vIpc is not initialized or an invalid parameter is given, report an error. */
  if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_NOT_INITIALIZED;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsRxPduIdValid(RxPduId) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_PDU_ID;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 When no errors have been detected, process the incoming Pdu. */
    PduIdType sduId   = vIpc_XCfg_GetRxPduConnId(RxPduId);
    PduIdType ulPduId = vIpc_XCfg_GetUpperLayerIdOfRxConnections(sduId);

    /* #30 If we are in the correct state we forward the RxIndication. */
    if (vIpc_XCfg_GetRxCon_State(sduId) == VIPC_RX_SDU_STATE_WAIT_FOR_RX_IND)
    {
      /* #40 Did we receive all data bytes or went something missing? */
      if (vIpc_XCfg_GetRxRemainingPayloadLength(sduId) > 0u)
      {
        /* #50 We received an incomplete sequence. Give RxIndication with E_NOT_OK to close it. */
        Result = E_NOT_OK;
      }

      vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(sduId)(ulPduId, Result);
      vIpc_XCfg_SetRxCon_State(sduId, VIPC_RX_SDU_STATE_IDLE);
    }
  }

  (void) vIpc_XCfg_ReportError(VIPC_SID_RXINDICATION, errorId);
} /* vIpc_RxIndication */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_CopyRxData()
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
 */
FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_CopyRxData(PduIdType RxPduId,
                                                   P2VAR(PduInfoType, AUTOMATIC, VIPC_APPL_VAR) PduInfoPtr,
                                                   P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  vIpc_ErrorCode errorId   = VIPC_E_NO_ERROR;
  boolean isDetEnabled     = vIpc_XCfg_DevErrorDetect();

  /* #10 If vIpc is not initialized or an invalid parameter is given, report an error. */
  if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_NOT_INITIALIZED;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsRxPduIdValid(RxPduId) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_PDU_ID;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsBufSizePtrValid(BufferSizePtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 When no errors have been detected, process the incoming Pdu. */
    PduIdType sduId = vIpc_XCfg_GetRxPduConnId(RxPduId);

    /* #30 If we are in the correct state, we process and forward the call to the upper layer. */
    if (vIpc_XCfg_GetRxCon_State(sduId) != VIPC_RX_SDU_STATE_IDLE)
    {
      PduIdType ulPduId = vIpc_XCfg_GetUpperLayerIdOfRxConnections(sduId);

      /* #40 Query buffer size information use case. */
      if (PduInfoPtr->SduLength == 0u)
      {
        PduLengthType bufSize;
        retVal = vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks(sduId)(ulPduId, PduInfoPtr, &bufSize);
        /* #50 If the upper layer returned a size of 0 we do not add the vIpc header information, but return the zero unmodified. */
        if (bufSize > 0)
        {
          *BufferSizePtr = bufSize + vIpc_HeaderType2HeaderLen[vIpc_XCfg_GetFrameType(sduId)];
        }
        else
        {
          *BufferSizePtr = 0u;
        }
      }
      else
      {
        PduInfoType ULInfoPtr;
        PduLengthType bufSize;
        PduLengthType remLen = PduInfoPtr->SduLength;

        /* #60 Regular use case. Is this the beginning of a segment or are we copying a fragment? */
        if (vIpc_XCfg_GetRxPduTpCount(RxPduId) == 0u)
        {
          vIpc_MessageType frameType = vIpc_Msg_GetType(PduInfoPtr);
          uint8 headerLen            = vIpc_HeaderType2HeaderLen[frameType];

          vIpc_XCfg_SetRxPduTpCount(RxPduId, 1u);

          /* #70 Copy data minus the vIpc header information. */
          ULInfoPtr.SduDataPtr = &PduInfoPtr->SduDataPtr[headerLen];
          ULInfoPtr.SduLength  = PduInfoPtr->SduLength - headerLen;
          remLen -= headerLen;
        }
        else
        {
          /* #80 Copy full header as we don't have a vIpc header we have to subtract. */
          ULInfoPtr = *PduInfoPtr;
        }

        retVal         = vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks(sduId)(ulPduId, &ULInfoPtr, &bufSize);
        *BufferSizePtr = bufSize;
        vIpc_XCfg_SetRxRemainingPayloadLength(sduId, (uint32)(vIpc_XCfg_GetRxRemainingPayloadLength(sduId) - remLen));
      }
    }
    else
    {
      /* #90 We return BUFREQ_E_NOT_OK here as the transmission is idle, i.e. no StartOfReception. */
      *BufferSizePtr = 0u;
    }
  }

  (void) vIpc_XCfg_ReportError(VIPC_SID_COPYRXDATA, errorId);

  return retVal;
} /* vIpc_CopyRxData */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */  /* PRQA 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * vIpc_StartOfReception()
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
FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_StartOfReception(PduIdType RxPduId,
                                                         P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                                         PduLengthType TpSduLength,
                                                         P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  vIpc_ErrorCode errorId   = VIPC_E_NO_ERROR;
  boolean isDetEnabled     = vIpc_XCfg_DevErrorDetect();

  /* #10 If vIpc is not initialized or an invalid parameter is given, report an error. */
  if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_NOT_INITIALIZED;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsRxPduIdValid(RxPduId) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_PDU_ID;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsPduDataValid(PduInfoPtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_POINTER;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsPduDataLenValid(PduInfoPtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_CONFIG;
  }
  else if ((isDetEnabled == TRUE) && (vIpc_XCfg_IsBufSizePtrValid(BufferSizePtr) == FALSE))  /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorId = VIPC_E_INV_POINTER;
  }
  else if ((isDetEnabled == TRUE) && (PduInfoPtr->SduLength < 2u))
  {
    errorId = VIPC_E_INV_CONFIG;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Given no errors have been detected, process the incoming Pdu. */
    uint8 vIpcAddress = vIpc_Msg_GetAddress(PduInfoPtr);

    /* #30 Is received address valid for this channel. */
    if (vIpcAddress < vIpc_XCfg_GetAddressLengthOfRxChannel(RxPduId))
    {
      uint8 frameType   = vIpc_Msg_GetType(PduInfoPtr);

      switch (frameType)
      {
        case VIPC_MSG_SF:
          /* #40 Process Single Frame. */
          retVal = vIpc_Rx_Process_SF(RxPduId, PduInfoPtr, TpSduLength, BufferSizePtr);
          break;
        case VIPC_MSG_FF:
          /* #50 Process First Frame. */
          retVal = vIpc_Rx_Process_FF(RxPduId, PduInfoPtr, TpSduLength, BufferSizePtr);
          break;
        case VIPC_MSG_CF:
          /* #60 Process Continuous Frame. */
          retVal = vIpc_Rx_Process_CF(RxPduId, PduInfoPtr, TpSduLength, BufferSizePtr);
          break;
        case VIPC_MSG_LF:
          /* #70 Process Last Frame. */
          retVal = vIpc_Rx_Process_LF(RxPduId, PduInfoPtr, TpSduLength, BufferSizePtr);
          break;
        default:
          /* Concluding else Frame Format, do nothing. */
          break;
      }
    }
  }

  (void) vIpc_XCfg_ReportError(VIPC_SID_STARTOFRECEPTION, errorId);

  return retVal;
} /* vIpc_StartOfReception */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */  /* PRQA 6080 1 */ /* MD_MSR_STMIF */ /* PRQA 6030 1 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  END OF FILE: vIpc_Rx.c
 *********************************************************************************************************************/
