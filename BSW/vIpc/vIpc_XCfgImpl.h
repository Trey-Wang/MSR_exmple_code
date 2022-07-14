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
/**       \file
 *        \brief  vIpc_XCfg src file
 *
 *      \details  Auxiliary src file containing abstractions to generated data
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK in file vIpc.c
 *********************************************************************************************************************/

#ifndef VIPC_XCFG_IMPL_H
# define VIPC_XCFG_IMPL_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vIpc_Types.h"

/* PRQA S 3219 EOF */ /* MD_MSR_14.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define VIPC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(vIpc_IsInitialized, VIPC_VAR_ZERO_INIT) vIpc_InitStatus;

#define VIPC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#define VIPC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * vIpc_XCfg_DevErrorDetect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_DevErrorDetect(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Det detection is enabled */
  return (boolean)(VIPC_DEV_ERROR_DETECT == STD_ON);
} /* vIpc_XCfg_DevErrorDetect */

/**********************************************************************************************************************
 * vIpc_XCfg_IsVIpcInitialized()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsVIpcInitialized(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if vIpc is initialized. */
  return (boolean)(vIpc_InitStatus == VIPC_INITIALIZED);
} /* vIpc_XCfg_IsVIpcInitialized */

/**********************************************************************************************************************
 * vIpc_XCfg_IsTxSduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsTxSduIdValid(PduIdType TxSduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Tx Connection Id is valid. */
  return (boolean)(TxSduId < vIpc_GetSizeOfTxConnections());
} /* vIpc_XCfg_IsTxSduIdValid */

/**********************************************************************************************************************
 * vIpc_XCfg_IsRxPduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsRxPduIdValid(PduIdType PduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Rx Pdu Id is valid. */
  return (boolean)(PduId < vIpc_GetSizeOfRxConnections());
} /* vIpc_XCfg_IsRxPduIdValid */

/**********************************************************************************************************************
 * vIpc_XCfg_IsTxPduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsTxPduIdValid(PduIdType PduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Tx Pdu Id is valid. */
  return (boolean)(PduId < vIpc_GetSizeOfTxChannel());
} /* vIpc_XCfg_IsTxPduIdValid */

/**********************************************************************************************************************
 * vIpc_XCfg_IsPduDataValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsPduDataValid(CONSTP2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Pdu Data is valid. */
  return (boolean)((PduInfoPtr != NULL_PTR) && (PduInfoPtr->SduDataPtr != NULL_PTR));
} /* vIpc_XCfg_IsPduDataValid */

/**********************************************************************************************************************
 * vIpc_XCfg_IsPduDataLenValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsPduDataLenValid(CONSTP2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Pdu Data length is valid. */
  return (boolean)((PduInfoPtr != NULL_PTR) && (PduInfoPtr->SduLength > 0));
} /* vIpc_XCfg_IsPduDataLenValid */

/**********************************************************************************************************************
 * vIpc_XCfg_IsBufSizePtrValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsBufSizePtrValid(CONSTP2CONST(PduLengthType, AUTOMATIC, VIPC_APPL_CONST) BufferSizePtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if Buffer Size Pointer is valid. */
  return (boolean)(BufferSizePtr != NULL_PTR);
} /* vIpc_XCfg_IsBufSizePtrValid */

/**********************************************************************************************************************
 * vIpc_XCfg_ReportError()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_XCfg_ReportError(vIpc_ServiceId ApiId, vIpc_ErrorCode ErrorId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  VIPC_DUMMY_STATEMENT(ApiId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  VIPC_DUMMY_STATEMENT(ErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* ----- Development Error Report ------------------------------------- */
  /* #10 Reports an DET error if one is occurred. */
#if (VIPC_DEV_ERROR_REPORT == STD_ON)
  if (ErrorId != VIPC_E_NO_ERROR)
  {
    (void) Det_ReportError(VIPC_MODULE_ID, VIPC_INSTANCE_ID_DET, ApiId, ErrorId);
    retVal = E_NOT_OK;
  }
#endif
  return retVal;
} /* vIpc_XCfg_ReportError */

/**********************************************************************************************************************
 * vIpc_XCfg_IsLLBusy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsLLBusy(PduIdType PduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if the LL is busy. */
  return (boolean)(vIpc_XCfg_TxChan_GetIsActiveTxConAvailable(PduId) == TRUE);
} /* vIpc_XCfg_IsLLBusy */

/**********************************************************************************************************************
 * vIpc_XCfg_GetTxAddressOfTxConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_GetTxAddressOfTxConnection(PduIdType SduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the address of Tx connection. */
  return vIpc_XCfg_GetTxAddressOfTxConnections(SduId);
} /* vIpc_XCfg_GetTxAddressOfTxConnection */

/*
 *  Tx Connection Data Getter and Setter
 */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_GetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(vIpc_TxConnectionStateType, VIPC_CODE) vIpc_XCfg_TxCon_GetState(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the state of Tx connection. */
  return vIpc_TxConnectionInfo[TxConId].SduState;
} /* vIpc_XCfg_TxCon_GetState */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_SetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetState(PduIdType TxConId,
                                                                  vIpc_TxConnectionStateType NextState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the state of Tx connection. */
  vIpc_TxConnectionInfo[TxConId].SduState = NextState;
} /* vIpc_XCfg_TxCon_SetState */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_GetRemPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxCon_GetRemPayloadLength(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the remaining payload length of Tx connection. */
  return vIpc_TxConnectionInfo[TxConId].RemainingPayloadLength;
} /* vIpc_XCfg_TxCon_GetRemPayloadLength */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_SetRemPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetRemPayloadLength(PduIdType TxConId,
                                                                             PduLengthType NewLength)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the remaining payload length of Tx connection. */
  vIpc_TxConnectionInfo[TxConId].RemainingPayloadLength = NewLength;
} /* vIpc_XCfg_TxCon_SetRemPayloadLength */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_DecRemPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_DecRemPayloadLength(PduIdType TxConId,
                                                                             PduLengthType DecLength)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement the remaining payload length of Tx connection. */
  vIpc_TxConnectionInfo[TxConId].RemainingPayloadLength = vIpc_TxConnectionInfo[TxConId].RemainingPayloadLength - DecLength;
} /* vIpc_XCfg_TxCon_DecRemPayloadLength */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_GetRemCompleteLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxCon_GetRemCompleteLength(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the remaining complete data length of Tx connection. */
  return vIpc_TxConnectionInfo[TxConId].RemainingCompleteLength;
} /* vIpc_XCfg_TxCon_GetRemCompleteLength */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_SetRemCompleteLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetRemCompleteLength(PduIdType TxConId,
                                                                              PduLengthType NewLengthType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the remaining complete data length of Tx connection. */
  vIpc_TxConnectionInfo[TxConId].RemainingCompleteLength = NewLengthType;
} /* vIpc_XCfg_TxCon_SetRemCompleteLength */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_DecRemCompleteLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_DecRemCompleteLength(PduIdType TxConId,
                                                                              PduLengthType DecLengthType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement the remaining complete data length of Tx connection. */
  vIpc_TxConnectionInfo[TxConId].RemainingCompleteLength = vIpc_TxConnectionInfo[TxConId].RemainingCompleteLength
                                                           - DecLengthType;
} /* vIpc_XCfg_TxCon_DecRemCompleteLength */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_GetMessageType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(vIpc_MessageType, VIPC_CODE) vIpc_XCfg_TxCon_GetMessageType(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the message type of Tx connection. */
  return vIpc_TxConnectionInfo[TxConId].MessageType;
} /* vIpc_XCfg_TxCon_GetMessageType */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_SetMessageType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetMessageType(PduIdType TxConId,
                                                                       vIpc_MessageType NewMsgType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the message type of Tx connection. */
  vIpc_TxConnectionInfo[TxConId].MessageType = NewMsgType;
} /* vIpc_XCfg_TxCon_SetMessageType */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_GetCurrentSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_TxCon_GetCurrentSegment(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the current active segment of transmission of Tx channel. */
  return vIpc_TxConnectionInfo[TxConId].CurrentSegment;
} /* vIpc_XCfg_TxCon_GetCurrentSegment */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_SetCurrentSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetCurrentSegment(PduIdType TxConId,
                                                                          uint8 NextSegment)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the current active segment of transmission of Tx channel. */
  vIpc_TxConnectionInfo[TxConId].CurrentSegment = NextSegment;
} /* vIpc_XCfg_TxCon_SetCurrentSegment */

/**********************************************************************************************************************
 * vIpc_XCfg_TxCon_IncCurrentSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_IncCurrentSegment(PduIdType TxConId,
                                                                           uint8 Increment)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Increment the current active segment of transmission of Tx channel. */
  vIpc_TxConnectionInfo[TxConId].CurrentSegment = (uint8)(vIpc_TxConnectionInfo[TxConId].CurrentSegment + Increment);
} /* vIpc_XCfg_TxCon_IncCurrentSegment */

/*
 *  Tx Channel Data Getter and Setter
 */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_GetActiveTxConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_TxChan_GetActiveTxConId(PduIdType TxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the active connection of Tx channel. */
  return vIpc_TxChannelInfo[TxPduId].ActiveTxConId;
} /* vIpc_XCfg_TxChan_GetActiveTxConId */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_SetActiveTxConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetActiveTxConId(PduIdType TxChanId, PduIdType NextActiveConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the active connection of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].ActiveTxConId = NextActiveConId;
} /* vIpc_XCfg_TxChan_SetActiveTxConId */

/**********************************************************************************************************************
 * vIpc_XCfg_GetTxChanIsActiveTxConAvailable()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_XCfg_TxChan_GetIsActiveTxConAvailable(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if there is an active connection of Tx channel. */
  return vIpc_TxChannelInfo[TxChanId].IsActiveTxConAvailable;
} /* vIpc_XCfg_GetTxChanIsActiveTxConAvailable */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_SetIsActiveTxConAvailable()
 *********************************************************************************************************************/
/*!
 * Todo: Change name, is 
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetIsActiveTxConAvailable(PduIdType TxChanId, boolean State)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets if there is an active connection of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].IsActiveTxConAvailable = State;
} /* vIpc_XCfg_TxChan_SetIsActiveTxConAvailable */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the already transmitted data of current segment of Tx channel. */
  return vIpc_TxChannelInfo[TxChanId].AlreadySentDataOfCurrSeg;
} /* vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg(PduIdType TxChanId,
                                                                                      PduLengthType NewAlreadySentData)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the already transmitted data of current segment of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].AlreadySentDataOfCurrSeg = NewAlreadySentData;
} /* vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_IncAlreadySentDataOfCurrSeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_IncAlreadySentDataOfCurrSeg(PduIdType TxChanId,
                                                                                      PduLengthType Increment)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Increments the already transmitted data of current segment of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].AlreadySentDataOfCurrSeg += (PduLengthType)Increment;
} /* vIpc_XCfg_TxChan_IncAlreadySentDataOfCurrSeg */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_GetRemainingSeparationCycles()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxChan_GetRemainingSeparationCycles(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the remaining separation cycles of Tx channel. */
  return vIpc_TxChannelInfo[TxChanId].RemainingSeparationCycles;
} /* vIpc_XCfg_TxChan_GetRemainingSeparationCycles */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_SetRemainingSeparationCycles()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetRemainingSeparationCycles(PduIdType TxChanId, uint16 NewValue)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the remaining separation cycles of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].RemainingSeparationCycles = NewValue;
} /* vIpc_XCfg_TxChan_SetRemainingSeparationCycles */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_InitRemainingSeparationCycles()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_InitRemainingSeparationCycles(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initializes the remaining separation cycles of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].RemainingSeparationCycles = vIpc_GetNumSepCyclesOfTxChannel(TxChanId);
} /* vIpc_XCfg_TxChan_InitRemainingSeparationCycles */

/**********************************************************************************************************************
 * vIpc_XCfg_TxChan_DecRemainingSeparationCycles()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_DecRemainingSeparationCycles(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrements the remaining separation cycles of Tx channel. */
  vIpc_TxChannelInfo[TxChanId].RemainingSeparationCycles--;
} /* vIpc_XCfg_TxChan_DecRemainingSeparationCycles */

/*
 *  Tx Queue Data Getter and Setter
 */
/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_GetConnectionId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQ_GetConnectionId(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the queue connection Id of Tx connection. */
  return vIpc_GetTxQueueElem(TxConId).Q_ConnectionId;
} /* vIpc_XCfg_TxQ_GetConnectionId */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_SetConnectionId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetConnectionId(PduIdType TxConId, uint32 NewConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the queue connection Id of Tx connection. */
  vIpc_GetTxQueueElem(TxConId).Q_ConnectionId = NewConId;
} /* vIpc_XCfg_TxQ_SetConnectionId */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_GetNextHigherIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQ_GetNextHigherIdx(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the queue index to next higher priority of Tx connection. */
  return vIpc_GetTxQueueElem(TxConId).Q_NextHigherIdx;
} /* vIpc_XCfg_TxQ_GetNextHigherIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_SetNextHigherIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetNextHigherIdx(PduIdType TxConId, uint32 NewHigherIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the queue index to next higher priority of Tx connection. */
  vIpc_GetTxQueueElem(TxConId).Q_NextHigherIdx = NewHigherIdx;
} /* vIpc_XCfg_TxQ_SetNextHigherIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_GetNextLowerIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQ_GetNextLowerIdx(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the queue index to next lower priority of Tx connection. */
  return vIpc_GetTxQueueElem(TxConId).Q_NextLowerIdx;
} /* vIpc_XCfg_TxQ_GetNextLowerIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_SetNextLowerIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetNextLowerIdx(PduIdType TxConId, uint32 NewLowerIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the queue index to next lower priority of Tx connection. */
  vIpc_GetTxQueueElem(TxConId).Q_NextLowerIdx = NewLowerIdx;
} /* vIpc_XCfg_TxQ_SetNextLowerIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_GetPrio()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(vIpc_PrioType, VIPC_CODE) vIpc_XCfg_TxQ_GetPrio(PduIdType TxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the queue priority of Tx connection. */
  return vIpc_GetTxQueueElem(TxConId).Q_Prio;
} /* vIpc_XCfg_TxQ_GetPrio */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_SetPrio()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetPrio(PduIdType TxConId, vIpc_PrioType newPrio)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the queue priority of Tx connection. */
  vIpc_GetTxQueueElem(TxConId).Q_Prio = newPrio;
} /* vIpc_XCfg_TxQ_SetPrio */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQ_InitData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_InitData(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 overallNumberOfTxCons = (uint32)vIpc_XCfg_GetSizeOfTxConnections();
  uint32 connectionIds;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initializes queue entries of all Tx connections. */
  for (connectionIds = 0; connectionIds < overallNumberOfTxCons; connectionIds++)
  {
    vIpc_GetTxQueueElem(connectionIds).Q_ConnectionId = connectionIds;
    vIpc_GetTxQueueElem(connectionIds).Q_NextLowerIdx = overallNumberOfTxCons;
    vIpc_GetTxQueueElem(connectionIds).Q_NextHigherIdx = overallNumberOfTxCons;
    vIpc_GetTxQueueElem(connectionIds).Q_Prio = 0;
  }
} /* vIpc_XCfg_TxQ_InitData */

/*
 *  Tx Queue Info Data Getter and Setter
 */
/**********************************************************************************************************************
 * vIpc_XCfg_TxQI_GetHighPrioIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQI_GetHighPrioIdx(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the high priority pointer of Tx channel. */
  return vIpc_QueueInfo[TxChanId].QI_HighPrioIdx;
} /* vIpc_XCfg_TxQI_GetHighPrioIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQI_GetHighPrioIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQI_SetHighPrioIdx(PduIdType TxChanId, uint32 NewHighPrioIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the high priority pointer of Tx channel. */
  vIpc_QueueInfo[TxChanId].QI_HighPrioIdx = NewHighPrioIdx;
} /* vIpc_XCfg_TxQI_GetHighPrioIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQI_GetLowPrioIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQI_GetLowPrioIdx(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the low priority pointer of Tx channel. */
  return vIpc_QueueInfo[TxChanId].QI_LowPrioIdx;
} /* vIpc_XCfg_TxQI_GetLowPrioIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQI_SetLowPrioIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQI_SetLowPrioIdx(PduIdType TxChanId, uint32 NewLowPrioIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the low priority pointer of Tx channel. */
  vIpc_QueueInfo[TxChanId].QI_LowPrioIdx = NewLowPrioIdx;
} /* vIpc_XCfg_TxQI_SetLowPrioIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_TxQI_SetLowPrioIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQI_InitHighAndLowPrioIdx(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initializes the high and low queue pointer of all Tx channels. */
  uint32 initIdx = (vIpc_ConAddressType)vIpc_XCfg_GetSizeOfTxConnections();
  vIpc_QueueInfo[TxChanId].QI_HighPrioIdx = initIdx;
  vIpc_QueueInfo[TxChanId].QI_LowPrioIdx = initIdx;
} /* vIpc_XCfg_TxQI_SetLowPrioIdx */

/*
 *  Rx Data Getter and Setter
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRxPduTpCount()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetRxPduTpCount(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the TP count of Rx channel. */
  return vIpc_RxChannelInfo[RxPduId].vIpc_RxPduTpCount;
} /* vIpc_XCfg_GetRxPduTpCount */

/**********************************************************************************************************************
 * vIpc_XCfg_SetRxPduTpCount()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxPduTpCount(PduIdType RxPduId,
                                                                   PduIdType NewCount)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the TP count of Rx channel. */
  vIpc_RxChannelInfo[RxPduId].vIpc_RxPduTpCount = NewCount;
} /* vIpc_XCfg_SetRxPduTpCount */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRxPduConnIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetRxPduConnId(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the active connection Id of Rx channel. */
  return vIpc_RxChannelInfo[RxPduId].vIpc_RxPduConnId;
} /* vIpc_XCfg_GetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_SetRxPduConnIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxPduConnId(PduIdType RxPduId,
                                                                   PduIdType NewConnId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the active connection Id of Rx channel. */
  vIpc_RxChannelInfo[RxPduId].vIpc_RxPduConnId = NewConnId;
} /* vIpc_XCfg_SetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_GetPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_GetRxPayloadLength(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the payload length of Rx channel. */
  return vIpc_RxConnectionInfo[RxPduId].PayloadLength;
} /* vIpc_XCfg_GetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_SetPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxPayloadLength(PduIdType RxPduId,
                                                                      uint32 length)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the payload length of Rx channel. */
  vIpc_RxConnectionInfo[RxPduId].PayloadLength = length;
} /* vIpc_XCfg_SetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRemainingPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_GetRxRemainingPayloadLength(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the remaining payload length of Rx channel. */
  return vIpc_RxConnectionInfo[RxPduId].RemainingPayloadLength;
} /* vIpc_XCfg_GetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_SetRemainingPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxRemainingPayloadLength(PduIdType RxPduId,
                                                                               uint32 length)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the remaining payload length of Rx channel. */
  vIpc_RxConnectionInfo[RxPduId].RemainingPayloadLength = length;
} /* vIpc_XCfg_SetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRxCon_State()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(vIpc_RxSduStateType, VIPC_CODE) vIpc_XCfg_GetRxCon_State(PduIdType RxSduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the state of Rx connection. */
  return vIpc_RxConnectionInfo[RxSduId].SduState;
} /* vIpc_XCfg_GetRxCon_State */

/**********************************************************************************************************************
 * vIpc_XCfg_SetRxCon_State()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxCon_State(PduIdType RxSduId,
                                                                  vIpc_RxSduStateType NextState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the state of Rx connection. */
  vIpc_RxConnectionInfo[RxSduId].SduState = NextState;
} /* vIpc_XCfg_SetTxCon_State */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSeqCtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_GetSeqCtr(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the sequence counter of Rx channel. */
  return vIpc_RxConnectionInfo[RxPduId].SeqCtr;
} /* vIpc_XCfg_GetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_SetSeqCtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetSeqCtr(PduIdType RxPduId,
                                                                 uint8 length)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Sets the sequence counter of Rx channel. */
  vIpc_RxConnectionInfo[RxPduId].SeqCtr = length;
} /* vIpc_XCfg_SetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSeqCtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_GetFrameType(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the frame type of Rx channel. */
  return vIpc_RxConnectionInfo[RxPduId].FrameType;
} /* vIpc_XCfg_GetRxPduConnIdx */

/**********************************************************************************************************************
 * vIpc_XCfg_SetSeqCtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetFrameType(PduIdType RxPduId,
                                                                 uint8 type)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the frame type of Rx channel. */
  vIpc_RxConnectionInfo[RxPduId].FrameType = type;
} /* vIpc_XCfg_SetRxPduConnIdx */

/*
 *  ComStackLib abstraction Rx Channels
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetAddressLengthOfRxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_AddressLengthOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetAddressLengthOfRxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns address length of Rx channel. */
  return vIpc_GetAddressLengthOfRxChannel(Index);
} /* vIpc_XCfg_GetAddressLengthOfRxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetAddressLengthOfRxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_AddressStartIdxOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetAddressStartIdxOfRxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns address start index of Rx channel. */
  return vIpc_GetAddressStartIdxOfRxChannel(Index);
} /* vIpc_XCfg_GetAddressStartIdxOfRxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetMaxLengthOfRxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfRxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns maximum PDU length of Rx channel. */
  return vIpc_GetMaxLengthOfRxChannel(Index);
} /* vIpc_XCfg_GetMaxLengthOfRxChannel */

/*
 *  ComStackLib abstraction Rx Connections
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetMaxLengthOfRxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfRxConnectionsType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfRxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns maximum PDU length of Rx connection. */
  return vIpc_GetMaxLengthOfRxConnections(Index);
} /* vIpc_XCfg_GetMaxLengthOfRxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRxAddressOfRxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_RxAddressOfRxConnectionsType, VIPC_CODE) vIpc_XCfg_GetRxAddressOfRxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the address of Rx connection. */
  return vIpc_GetRxAddressOfRxConnections(Index);
} /* vIpc_XCfg_GetRxAddressOfRxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetUpperLayerIdOfRxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_UpperLayerIdOfRxConnectionsType, VIPC_CODE) vIpc_XCfg_GetUpperLayerIdOfRxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the UL Id of Rx connection. */
  return vIpc_GetUpperLayerIdOfRxConnections(Index);
} /* vIpc_XCfg_GetUpperLayerIdOfRxConnections */

/*
 *  ComStackLib abstraction Rx Function Pointer Tables
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_CopyRxDataFctPtrType, VIPC_CODE) vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the CopyRxData callback of Rx connection. */
  return vIpc_GetCopyRxDataOfRxUlCallbacks(Index);
} /* vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRxIndicationOfRxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_RxIndicationFctPtrType, VIPC_CODE) vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the RxIndication callback of Rx connection. */
  return vIpc_GetRxIndicationOfRxUlCallbacks(Index);
} /* vIpc_XCfg_GetRxIndicationOfRxUlCallbacks */

/**********************************************************************************************************************
 * vIpc_XCfg_GetStartOfReceptionOfRxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_StartOfReceptionFctPtrType, VIPC_CODE) vIpc_XCfg_GetStartOfReceptionOfRxUlCallbacks(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the StartOfReception callback of Rx connection. */
  return vIpc_GetStartOfReceptionOfRxUlCallbacks(Index);
} /* vIpc_XCfg_GetStartOfReceptionOfRxUlCallbacks */

/*
 *  ComStackLib abstraction Rx Data Size
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetRxConnectionsIdxOfAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_RxConnectionsIdxOfAddressType, VIPC_CODE) vIpc_XCfg_GetRxConnectionsIdxOfAddress(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the Rx connection Id of address. */
  return vIpc_GetRxConnectionsIdxOfAddress(Index);
} /* vIpc_XCfg_GetRxConnectionsIdxOfAddress */

/**********************************************************************************************************************
 * vIpc_XCfg_GetAddressEndIdxOfRxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_AddressEndIdxOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetAddressEndIdxOfRxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns address end index of Rx channel. */
  return vIpc_GetAddressEndIdxOfRxChannel(Index);
} /* vIpc_XCfg_GetAddressEndIdxOfRxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfAddress(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Rx addresses. */
  return (PduIdType)vIpc_GetSizeOfAddress();
} /* vIpc_XCfg_GetSizeOfAddress */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfRxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfRxChannel(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Rx channels. */
  return (PduIdType)vIpc_GetSizeOfRxChannel();
} /* vIpc_XCfg_GetSizeOfRxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfRxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfRxConnections(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Rx connections. */
  return (PduIdType)vIpc_GetSizeOfRxConnections();
} /* vIpc_XCfg_GetSizeOfRxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfRxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfRxUlCallbacks(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Rx UL callbacks. */
  return (PduIdType)vIpc_GetSizeOfRxUlCallbacks();
} /* vIpc_XCfg_GetSizeOfRxUlCallbacks */

/*
 *  ComStackLib abstraction Tx Function Pointer Tables
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetTransmitOfTransmitLLFunctionTable()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TransmitFctPtrType, VIPC_CODE) vIpc_XCfg_GetTransmitOfTransmitLLFunctionTable(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns Transmit function of LL with Tx channel Id. */
  return vIpc_GetTransmitOfTransmitLLFunctionTable(Index);
} /* vIpc_XCfg_GetTransmitOfTransmitLLFunctionTable */

/**********************************************************************************************************************
 * vIpc_XCfg_GetTxConfirmationOfTxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TxConfirmationFctPtrType, VIPC_CODE) vIpc_XCfg_GetTxConfirmationOfTxUlCallbacks(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the TxConfirmation callback of Tx connection. */
  return vIpc_GetTxConfirmationOfTxUlCallbacks(Index);
} /* vIpc_XCfg_GetTxConfirmationOfTxUlCallbacks */

/**********************************************************************************************************************
 * vIpc_XCfg_GetCopyTxDataOfTxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_CopyTxDataFctPtrType, VIPC_CODE) vIpc_XCfg_GetCopyTxDataOfTxUlCallbacks(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the CopyTxData callback of Tx connection. */
  return vIpc_GetCopyTxDataOfTxUlCallbacks(Index);
} /* vIpc_XCfg_GetCopyTxDataOfTxUlCallbacks */

/*
 *  ComStackLib abstraction Tx Connections
 */
/**********************************************************************************************************************
 * vIpc_XCfg_GetUpperLayerIdOfTxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_UpperLayerIdOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetUpperLayerIdOfTxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the UL Id of Tx connection. */
  return vIpc_GetUpperLayerIdOfTxConnections(Index);
} /* vIpc_XCfg_GetUpperLayerIdOfTxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetTxChannelIdxOfTxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TxChannelIdxOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetTxChannelIdxOfTxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns Tx channel index to which the Tx connection is assigned. */
  return vIpc_GetTxChannelIdxOfTxConnections(Index);
} /* vIpc_XCfg_GetTxChannelIdxOfTxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetTxAddressOfTxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TxAddressOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetTxAddressOfTxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the address of Tx connection. */
  return vIpc_GetTxAddressOfTxConnections(Index);
} /* vIpc_XCfg_GetTxAddressOfTxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetPriorityOfTxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_PriorityOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetPriorityOfTxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the priority of Tx connection. */
  return vIpc_GetPriorityOfTxConnections(Index);
} /* vIpc_XCfg_GetPriorityOfTxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetMaxLengthOfTxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfTxConnections(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the maximum PDU length of Tx connection. */
  return vIpc_GetMaxLengthOfTxConnections(Index);
} /* vIpc_XCfg_GetMaxLengthOfTxConnections */

/*
 *  ComStackLib abstraction Tx Channels
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetNumSepCyclesOfTxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_NumSepCyclesOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetNumSepCyclesOfTxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the number of separation cycles of Tx channel. */
  return vIpc_GetNumSepCyclesOfTxChannel(Index);
} /* vIpc_XCfg_GetNumSepCyclesOfTxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetNumConnectionsOfTxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_NumConnectionsOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetNumConnectionsOfTxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of connections assigned to Tx channel. */
  return vIpc_GetNumConnectionsOfTxChannel(Index);
} /* vIpc_XCfg_GetNumConnectionsOfTxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetMaxLengthOfTxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfTxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns maximum PDU length of Tx channel. */
  return vIpc_GetMaxLengthOfTxChannel(Index);
} /* vIpc_XCfg_GetMaxLengthOfTxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetLowerLayerIdOfTxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_LowerLayerIdOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetLowerLayerIdOfTxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the LL Id of Tx channel. */
  return vIpc_GetLowerLayerIdOfTxChannel(Index);
} /* vIpc_XCfg_GetLowerLayerIdOfTxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetAlgorithmOfTxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_Algorithm, VIPC_CODE) vIpc_XCfg_GetAlgorithmOfTxChannel(PduIdType Index)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns the queue algorithm of Tx channel. */
  return vIpc_GetAlgorithmOfTxChannel(Index);
} /* vIpc_XCfg_GetAlgorithmOfTxChannel */

/*
 *  ComStackLib abstraction Tx Data Size
 */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfTransmitLLFunctionTable()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTransmitLLFunctionTable(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Transmit functions. */
  return (PduIdType)vIpc_GetSizeOfTransmitLLFunctionTable();
} /* vIpc_XCfg_GetSizeOfTransmitLLFunctionTable */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfTxChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTxChannel(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Tx channels. */
  return (PduIdType)vIpc_GetSizeOfTxChannel();
} /* vIpc_XCfg_GetSizeOfTxChannel */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfTxConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTxConnections(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of Tx connections. */
  return (PduIdType)vIpc_GetSizeOfTxConnections();
} /* vIpc_XCfg_GetSizeOfTxConnections */

/**********************************************************************************************************************
 * vIpc_XCfg_GetSizeOfTxUlCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTxUlCallbacks(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns number of UL callbacks. */
  return (PduIdType)vIpc_GetSizeOfTxUlCallbacks();
} /* vIpc_XCfg_GetSizeOfTxUlCallbacks */

#define VIPC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: vIpc_XCfgImpl.h
 *********************************************************************************************************************/

#endif /* VIPC_XCFG_IMPL_H */
