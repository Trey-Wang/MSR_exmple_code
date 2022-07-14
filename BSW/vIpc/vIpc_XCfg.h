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
 *        \brief  vIpc_XCfg header file
 *
 *      \details  Auxiliary header file containing abstractions to generated data
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

#ifndef VIPC_XCFG_H
# define VIPC_XCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vIpc_Cfg.h"
#include "vIpc_Lcfg.h"
#include "vIpc_PBcfg.h"

#if (VIPC_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define VIPC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpc_XCfg_DevErrorDetect()
 *********************************************************************************************************************/
/*! \brief       Checks if DevErrorDetect is enabled or not.
 *  \details     -
 *  \return      FALSE          The DevErrorDetect is disabled.
 *               TRUE           The DevErrorDetect is enabled.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_DevErrorDetect(void);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsVIpcInitialized()
 *********************************************************************************************************************/
/*! \brief       Checks if vIpc is initialized for all channels.
 *  \details     -
 *  \return      FALSE          The vIpc is not initialized.
 *               TRUE           The vIpc is initialized.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsVIpcInitialized(void);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsTxSduIdValid()
 *********************************************************************************************************************/
/*! \brief       Checks if passed Tx Sdu Id is valid or not.
 *  \details     -
 *  \return      FALSE          The Id is invalid.
 *               TRUE           The Id is valid.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsTxSduIdValid(PduIdType TxSduId);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsRxPduIdValid()
 *********************************************************************************************************************/
/*! \brief       Checks if passed Rx Pdu Id is valid or not.
 *  \details     -
 *  \return      FALSE          The Id is invalid.
 *               TRUE           The Id is valid.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsRxPduIdValid(PduIdType PduId);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsTxPduIdValid()
 *********************************************************************************************************************/
/*! \brief       Checks if passed Tx Pdu Id is valid or not.
 *  \details     -
 *  \return      FALSE          The Id is invalid.
 *               TRUE           The Id is valid.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsTxPduIdValid(PduIdType PduId);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsPduDataValid()
 *********************************************************************************************************************/
/*! \brief       Checks if passed Pdu data is valid or not.
 *  \details     -
 *  \return      FALSE          The Pdu data is invalid.
 *               TRUE           The Pdu data is valid.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsPduDataValid(CONSTP2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsPduDataLenValid()
 *********************************************************************************************************************/
/*! \brief       Checks if passed Pdu data length is valid or not.
 *  \details     -
 *  \return      FALSE          The Pdu data length is invalid.
 *               TRUE           The Pdu data length is valid.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsPduDataLenValid(CONSTP2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsBufSizePtrValid()
 *********************************************************************************************************************/
/*! \brief       Checks if passed buffer size pointer is valid or not.
 *  \details     -
 *  \return      FALSE          The buffer size pointer is invalid.
 *               TRUE           The buffer size pointer is valid.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_CONST_FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsBufSizePtrValid(CONSTP2CONST(PduLengthType, AUTOMATIC, VIPC_APPL_CONST) BufferSizePtr);

/**********************************************************************************************************************
 *  vIpc_XCfg_ReportError()
 *********************************************************************************************************************/
/*! \brief       Helper function that will report an error if Det is enabled and an error ID is passed.
 *  \details     -
 *  \return      E_OK           No error reported.
 *               E_NOT_OK       The specified error was reported.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, VIPC_CODE) vIpc_XCfg_ReportError(vIpc_ServiceId ApiId, vIpc_ErrorCode ErrorId);

/**********************************************************************************************************************
 *  vIpc_XCfg_IsLLBusy()
 *********************************************************************************************************************/
/*! \brief       Checks if the LL of the Tx channel is currently busy.
 *  \details     -
 *  \return      FALSE           The LL is idle.
 *               TRUE            The LL is busy.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_XCfg_IsLLBusy(PduIdType PduId);


/*
 *  Tx Connection Data Getter and Setter
 */

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_GetState()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified Tx connection.
 *  \details     -
 *  \return      The current connection state of the specified Tx connection.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(vIpc_TxConnectionStateType, VIPC_CODE) vIpc_XCfg_TxCon_GetState(PduIdType TxConId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_SetState()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified Tx connection to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetState(PduIdType TxConId,
                                                                  vIpc_TxConnectionStateType NextState);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_GetRemPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Returns the remaining payload length to be transmitted of the specified Tx connection.
 *  \details     -
 *  \return      The current remaining payload length of the specified Tx connection.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxCon_GetRemPayloadLength(PduIdType TxConId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_SetRemPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Sets the remaining payload length of the specified Tx connection to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetRemPayloadLength(PduIdType TxConId,
                                                                             PduLengthType NewLength);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_DecRemPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Decrements the remaining payload length of the specified Tx connection.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_DecRemPayloadLength(PduIdType TxConId,
                                                                             PduLengthType DecLength);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_GetRemCompleteLength()
 *********************************************************************************************************************/
/*! \brief       Returns the remaining number of bytes to be transmitted.
 *  \details     -
 *  \return      The remaining number of bytes to be transmitted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxCon_GetRemCompleteLength(PduIdType TxConId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_SetRemCompleteLength()
 *********************************************************************************************************************/
/*! \brief       Sets the remaining number of bytes to be transmitted.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetRemCompleteLength(PduIdType TxConId,
                                                                              PduLengthType NewLengthType);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_DecRemCompleteLength()
 *********************************************************************************************************************/
/*! \brief       Decrements the remaining number of bytes to be transmitted.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_DecRemCompleteLength(PduIdType TxConId,
                                                                              PduLengthType DecLengthType);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_GetMessageType()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(vIpc_MessageType, VIPC_CODE) vIpc_XCfg_TxCon_GetMessageType(PduIdType TxConId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_SetMessageType()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetMessageType(PduIdType TxConId,
                                                                        vIpc_MessageType NewMsgType);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_GetCurrentSegment()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_TxCon_GetCurrentSegment(PduIdType TxConId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_SetCurrentSegment()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_SetCurrentSegment(PduIdType TxConId,
                                                                           uint8 NextSegment);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxCon_IncCurrentSegment()
 *********************************************************************************************************************/
/*! \brief       Increments the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxCon_IncCurrentSegment(PduIdType TxConId,
                                                                           uint8 Increment);


/*
 *  Tx Channel Data Getter and Setter
 */

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_GetActiveTxConId()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_TxChan_GetActiveTxConId(PduIdType TxChanId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_SetActiveTxConId()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetActiveTxConId(PduIdType TxChanId, PduIdType NextActiveConId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_GetIsActiveTxConAvailable()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_XCfg_TxChan_GetIsActiveTxConAvailable(PduIdType TxChanId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_SetIsActiveTxConAvailable()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetIsActiveTxConAvailable(PduIdType TxChanId, boolean State);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxChan_GetAlreadySentDataOfCurrSeg(PduIdType TxChanId);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg(PduIdType TxChanId,
                                                                                      PduLengthType NewAlreadySentData);

/**********************************************************************************************************************
 *  vIpc_XCfg_TxChan_IncAlreadySentDataOfCurrSeg()
 *********************************************************************************************************************/
/*! \brief       Increments the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_IncAlreadySentDataOfCurrSeg(PduIdType TxChanId,
                                                                                      PduLengthType Increment);


LOCAL_INLINE FUNC(PduLengthType, VIPC_CODE) vIpc_XCfg_TxChan_GetRemainingSeparationCycles(PduIdType TxChanId);


LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_SetRemainingSeparationCycles(PduIdType TxChanId, uint16 NewValue);


LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_InitRemainingSeparationCycles(PduIdType TxChanId);


LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxChan_DecRemainingSeparationCycles(PduIdType TxChanId);



/*
 *  Tx Queue Data Getter and Setter
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQ_GetConnectionId(PduIdType TxConId);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetConnectionId(PduIdType TxConId, uint32 NewConId);
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQ_GetNextHigherIdx(PduIdType TxConId);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetNextHigherIdx(PduIdType TxConId, uint32 NewHigherIdx);
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQ_GetNextLowerIdx(PduIdType TxConId);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetNextLowerIdx(PduIdType TxConId, uint32 NewLowerIdx);
LOCAL_INLINE FUNC(vIpc_PrioType, VIPC_CODE) vIpc_XCfg_TxQ_GetPrio(PduIdType TxConId);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_SetPrio(PduIdType TxConId, vIpc_PrioType newPrio);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQ_InitData(void);



/*
 *  Tx Queue Info Data Getter and Setter
 */
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQI_GetHighPrioIdx(PduIdType TxChanId);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQI_SetHighPrioIdx(PduIdType TxChanId, uint32 NewHighPrioIdx);
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_TxQI_GetLowPrioIdx(PduIdType TxChanId);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQI_SetLowPrioIdx(PduIdType TxChanId, uint32 NewLowPrioIdx);
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_TxQI_InitHighAndLowPrioIdx(PduIdType TxChanId);



/*
 *  Rx Connection Data Getter and Setter
 */

/**********************************************************************************************************************
 *  vIpc_XCfg_GetRxCon_State()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(vIpc_RxSduStateType, VIPC_CODE) vIpc_XCfg_GetRxCon_State(PduIdType RxSduId);

/**********************************************************************************************************************
 *  vIpc_XCfg_SetRxCon_State()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxCon_State(PduIdType RxSduId,
                                                                  vIpc_RxSduStateType NextState);

/**********************************************************************************************************************
 *  vIpc_XCfg_GetRxPduTpCount()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetRxPduTpCount(PduIdType RxPduId);

/**********************************************************************************************************************
 *  vIpc_XCfg_SetRxPduTpCount()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection state of the specified SDU to the new value.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxPduTpCount(PduIdType RxPduId,
                                                                   PduIdType NewCount);

/**********************************************************************************************************************
 *  vIpc_XCfg_GetRxPduConnIdx()
 *********************************************************************************************************************/
/*! \brief       Returns the connection idx of a Rx channel.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetRxPduConnId(PduIdType RxPduId);

/**********************************************************************************************************************
 *  vIpc_XCfg_SetRxPduConnIdx()
 *********************************************************************************************************************/
/*! \brief       Sets the current connection idx of a Rx connection.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxPduConnId(PduIdType RxPduId,
                                                                  PduIdType NewConnId);


/**********************************************************************************************************************
 *  vIpc_XCfg_GetPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Get the payload length of a Rx channel.
 *  \details     -
 *  \return      The payload length of the Rx channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_GetRxPayloadLength(PduIdType RxPduId);


/**********************************************************************************************************************
 *  vIpc_XCfg_SetPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Set the payload length of a Rx channel.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxPayloadLength(PduIdType RxPduId,
                                                                      uint32 length);


/**********************************************************************************************************************
 *  vIpc_XCfg_GetRxRemainingPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Get the remaining payload length of a Rx channel.
 *  \details     -
 *  \return      The remaining payload length of a Rx channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint32, VIPC_CODE) vIpc_XCfg_GetRxRemainingPayloadLength(PduIdType RxPduId);


/**********************************************************************************************************************
 *  vIpc_XCfg_SetRxRemainingPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Set the remaining payload length of a Rx channel.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetRxRemainingPayloadLength(PduIdType RxPduId,
                                                                               uint32 length);


/**********************************************************************************************************************
 *  vIpc_XCfg_GetBufferSize()
 *********************************************************************************************************************/
/*! \brief       Get the sequence counter of a Rx channel.
 *  \details     -
 *  \return      The sequence counter of a Rx channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_GetSeqCtr(PduIdType RxPduId);


/**********************************************************************************************************************
 *  vIpc_XCfg_SetRemainingPayloadLength()
 *********************************************************************************************************************/
/*! \brief       Set the remaining buffer length of a channel.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetSeqCtr(PduIdType RxPduId,
                                                             uint8 length);


/**********************************************************************************************************************
 *  vIpc_XCfg_GetBufferSize()
 *********************************************************************************************************************/
/*! \brief       Get the frame type of a Rx channel.
 *  \details     -
 *  \return      The frame type of a Rx channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_GetFrameType(PduIdType RxPduId);


/**********************************************************************************************************************
 *  vIpc_XCfg_SetFrameType()
 *********************************************************************************************************************/
/*! \brief       Set the frame type of a Rx channel.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_XCfg_SetFrameType(PduIdType RxPduId,
                                                                uint8 type);

/*
 *  ComStackLib abstraction Rx Channels
 */

/**********************************************************************************************************************
 *  vIpc_XCfg_GetAddressLengthOfRxChannel()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_AddressLengthOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetAddressLengthOfRxChannel(PduIdType Index);

/**********************************************************************************************************************
 *  vIpc_XCfg_GetAddressStartIdxOfRxChannel()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_AddressStartIdxOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetAddressStartIdxOfRxChannel(PduIdType Index);

/**********************************************************************************************************************
 *  vIpc_XCfg_GetMaxLengthOfRxChannel()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfRxChannel(PduIdType Index);


/*
 *  ComStackLib abstraction Rx Connections
 */

/**********************************************************************************************************************
 *  vIpc_XCfg_GetMaxLengthOfRxConnections()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfRxConnectionsType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfRxConnections(PduIdType Index);

/**********************************************************************************************************************
 *  vIpc_XCfg_GetRxAddressOfRxConnections()
 *********************************************************************************************************************/
/*! \brief       Returns the current connection state of the specified SDU.
 *  \details     -
 *  \return      The current connection state of the specified SDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_PURE_FUNC(vIpc_RxAddressOfRxConnectionsType, VIPC_CODE) vIpc_XCfg_GetRxAddressOfRxConnections(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_UpperLayerIdOfRxConnectionsType, VIPC_CODE) vIpc_XCfg_GetUpperLayerIdOfRxConnections(PduIdType Index);


/*
 *  ComStackLib abstraction Rx Function Pointer Tables
 */

LOCAL_INLINE VIPC_PURE_FUNC(vIpc_CopyRxDataFctPtrType, VIPC_CODE) vIpc_XCfg_GetCopyRxDataOfRxUlCallbacks(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_RxIndicationFctPtrType, VIPC_CODE) vIpc_XCfg_GetRxIndicationOfRxUlCallbacks(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_StartOfReceptionFctPtrType, VIPC_CODE) vIpc_XCfg_GetStartOfReceptionOfRxUlCallbacks(PduIdType Index);


/*
 *  ComStackLib abstraction Rx Data Size
 */

LOCAL_INLINE VIPC_PURE_FUNC(vIpc_RxConnectionsIdxOfAddressType, VIPC_CODE) vIpc_XCfg_GetRxConnectionsIdxOfAddress(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_AddressEndIdxOfRxChannelType, VIPC_CODE) vIpc_XCfg_GetAddressEndIdxOfRxChannel(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfAddress(void);


LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfRxChannel(void);


LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfRxConnections(void);


LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfRxUlCallbacks(void);


/*
 *  ComStackLib abstraction Tx Function Pointer Tables
 */

LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TransmitFctPtrType, VIPC_CODE) vIpc_XCfg_GetTransmitOfTransmitLLFunctionTable(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TxConfirmationFctPtrType, VIPC_CODE) vIpc_XCfg_GetTxConfirmationOfTxUlCallbacks(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_CopyTxDataFctPtrType, VIPC_CODE) vIpc_XCfg_GetCopyTxDataOfTxUlCallbacks(PduIdType Index);


/*
 *  ComStackLib abstraction Tx Connections
 */

/**********************************************************************************************************************
 *  vIpc_XCfg_GetTxAddressOfTxConnection()
 *********************************************************************************************************************/
/*! \brief       Returns the address to identify the corresponding SDU ID on Rx side.
 *  \details     -
 *  \param[in]   SduId         The connection Id.
 *  \return      The target address to identify the SDU on Rx.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint8, VIPC_CODE) vIpc_XCfg_GetTxAddressOfTxConnection(PduIdType SduId);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_UpperLayerIdOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetUpperLayerIdOfTxConnections(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TxChannelIdxOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetTxChannelIdxOfTxConnections(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_TxAddressOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetTxAddressOfTxConnections(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_PriorityOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetPriorityOfTxConnections(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfTxConnectionsType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfTxConnections(PduIdType Index);


/*
 *  ComStackLib abstraction Tx Channels
 */

LOCAL_INLINE VIPC_PURE_FUNC(vIpc_NumSepCyclesOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetNumSepCyclesOfTxChannel(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_NumConnectionsOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetNumConnectionsOfTxChannel(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_MaxLengthOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetMaxLengthOfTxChannel(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_LowerLayerIdOfTxChannelType, VIPC_CODE) vIpc_XCfg_GetLowerLayerIdOfTxChannel(PduIdType Index);


LOCAL_INLINE VIPC_PURE_FUNC(vIpc_Algorithm, VIPC_CODE) vIpc_XCfg_GetAlgorithmOfTxChannel(PduIdType Index);


/*
 *  ComStackLib abstraction Tx Data Size
 */

LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTransmitLLFunctionTable(void);

LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTxChannel(void);

LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTxConnections(void);

/**********************************************************************************************************************
 *  vIpc_XCfg_GetSizeOfTxUlCallbacks()
 *********************************************************************************************************************/
/*! \brief       Returns the number of UL callbacks.
 *  \details     -
 *  \return      The number of UL callbacks.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LOCAL_INLINE VIPC_PURE_FUNC(PduIdType, VIPC_CODE) vIpc_XCfg_GetSizeOfTxUlCallbacks(void);


#define VIPC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#ifndef VIPC_UNIT_TEST
# include "vIpc_XCfgImpl.h"
#endif

#endif /* VIPC_XCFG_H */
