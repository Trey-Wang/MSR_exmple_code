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
/**        \file  EthTSyn_Pdelay_Int.c
 *        \brief  EthTSyn Path Delay source file - Internal Functions
 *
 *      \details  EthTSyn Path Delay source file containing the EthTSyn Path Delay implementation of the EthTSyn
 *                module.
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

#define ETHTSYN_PDELAY_INT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Pdelay_Int.h"
#include "EthTSyn_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
#include "EthTSyn_Util_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of Msn header file */
#if (  (ETHTSYN_SW_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_SW_MINOR_VERSION != (1u)) \
    || (ETHTSYN_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthTSyn_Pdelay_Int.c and EthTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ETHTSYN_CFG_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_CFG_MINOR_VERSION != (1u)) )
# error "Version numbers of EthTSyn_Pdelay_Int.c and EthTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (ETHTSYN_LOCAL)
# define ETHTSYN_LOCAL static
#endif

#if !defined (ETHTSYN_LOCAL_INLINE)
# define ETHTSYN_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define ETHTSYN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TxPdelayReqMsg
 *********************************************************************************************************************/
/*! \brief      Transmits a Pdelay Request message
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_TxPdelayReqMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_GetTxBufferForPdelayReq
 *********************************************************************************************************************/
/*! \brief      Checks if Port is ready to transmit a PdelayReq message and reserves an Ethernet TxBuffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtrPtr     Pointer to the Ethernet TxBuffer
 *  \param[out] BufIdxPtr       The index of the Ethernet TxBuffer
 *  \param[out] LenBytePtr      The length of the Ethernet TxBuffer
 *  \return     E_OK - Port is ready to transmit the PdelayReq message and an Ethernet TxBuffer was
 *                     locked successfully
 *  \return     E_NOT_OK - Port is not ready to transmit the PdelayReq message or no Ethernet TxBuffer
 *                         could be locked
 *  \note       Out parameters are (only) valid for return value of E_OK
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_GetTxBufferForPdelayReq(
                EthTSyn_PortIdxType   PortIdx,
  ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr,
  ETHTSYN_P2VAR(uint8)                BufIdxPtr,
  ETHTSYN_P2VAR(uint16)               LenBytePtr);

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_CopyPdelayReqToTxBuffer
 *********************************************************************************************************************/
/*! \brief      Copies a PdelayReq message to the given Tx buffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtr        The assembled PdelayReq message in network byte order
 *              the PdelayReq message.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayTx_CopyPdelayReqToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) TxBufPtr);

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq
 *********************************************************************************************************************/
/*! \brief      Enables egress timestamping for PdelayReq message
 *  \param[in]  PortIdx      Index of the port that should transmit the message
 *  \param[in]  BufIdx       Index of the Ethernet TxBuffer
 *  \param[in]  MgmtInfoPtr  Pointer to the switch management information
 *  \return     E_OK - Successfully enabled egress timestamping
 *  \return     E_NOT_OK - Failed to enable egress timestamping
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq(
                  EthTSyn_PortIdxType   PortIdx,
                  uint8                 BufIdx,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType)  MgmtInfoPtr);
# else
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq(
                  EthTSyn_PortIdxType   PortIdx,
                  uint8                 BufIdx);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TxPdelayRespMsg
 *********************************************************************************************************************/
/*! \brief      Transmits a PdelayResp message
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_TxPdelayRespMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_GetTxBufferForPdelayResp
 *********************************************************************************************************************/
/*! \brief      Checks if Port is ready to transmit a PdelayResp message and reserves an Ethernet TxBuffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtrPtr     Pointer to the Ethernet TxBuffer
 *  \param[out] BufIdxPtr       The index of the Ethernet TxBuffer
 *  \param[out] LenBytePtr      The length of the Ethernet TxBuffer
 *  \return     E_OK - Port is ready to transmit the PdelayResp message and an Ethernet TxBuffer was
 *                     locked successfully
 *  \return     E_NOT_OK - Port is not ready to transmit the PdelayResp message or no Ethernet TxBuffer
 *                         could be locked
 *  \note       Out parameters are (only) valid for return value of E_OK
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_GetTxBufferForPdelayResp(
                EthTSyn_PortIdxType   PortIdx,
  ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr,
  ETHTSYN_P2VAR(uint8)                BufIdxPtr,
  ETHTSYN_P2VAR(uint16)               LenBytePtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_CopyPdelayRespToTxBuffer
 *********************************************************************************************************************/
/*! \brief      Copies a PdelayResp message to the given Tx buffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtr        The assembled PdelayResp message in network byte order
 *              the PdelayResp message.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayTx_CopyPdelayRespToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) TxBufPtr);

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp
 *********************************************************************************************************************/
/*! \brief      Enables egress timestamping for PdelayResp message
 *  \param[in]  PortIdx      Index of the port that should transmit the message
 *  \param[in]  BufIdx       Index of the Ethernet TxBuffer
 *  \param[in]  MgmtInfoPtr  Pointer to the switch management information
 *  \return     E_OK - Successfully enabled egress timestamping
 *  \return     E_NOT_OK - Failed to enable egress timestamping
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp(
                  EthTSyn_PortIdxType   PortIdx,
                  uint8                 BufIdx,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType)  MgmtInfoPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */
# else
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp(
                  EthTSyn_PortIdxType   PortIdx,
                  uint8                 BufIdx); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TxPdelayRespFollowUpMsg
 *********************************************************************************************************************/
/*! \brief      Transmits a PdelayResp FollowUp message
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_TxPdelayRespFollowUpMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_GetTxBufferForPdelayRespFup
 *********************************************************************************************************************/
/*! \brief      Checks if Port is ready to transmit a PdelayResp FollowUp message and reserves an Ethernet TxBuffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtrPtr     Pointer to the Ethernet TxBuffer
 *  \param[out] BufIdxPtr       The index of the Ethernet TxBuffer
 *  \param[out] LenBytePtr      The length of the Ethernet TxBuffer
 *  \return     E_OK - Port is ready to transmit the PdelayResp FollowUp message and an Ethernet TxBuffer was
 *                     locked successfully
 *  \return     E_NOT_OK - Port is not ready to transmit the PdelayResp FollowUp message or no Ethernet TxBuffer
 *                         could be locked
 *  \note       Out parameters are (only) valid for return value of E_OK
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_GetTxBufferForPdelayRespFup(
                EthTSyn_PortIdxType   PortIdx,
  ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr,
  ETHTSYN_P2VAR(uint8)                BufIdxPtr,
  ETHTSYN_P2VAR(uint16)               LenBytePtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_CopyPdelayRespFupToTxBuffer
 *********************************************************************************************************************/
/*! \brief      Copies a PdelayResp FollowUp message to the given Tx buffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtr        The assembled PdelayResp FollowUp message in network byte order
 *              the PdelayResp FollowUp message.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayTx_CopyPdelayRespFupToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) TxBufPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_CheckForPdelayRespTimeout
 *********************************************************************************************************************/
/*! \brief      Checks if a PdelayResp or PdelayRespFollowUp timeout occurred and handles the timeout
 *  \param[in]  PortIdx         Index of the used port
 *  \return     TRUE - Transmission of new PdelayReq should be triggered
 *  \return     FALSE - No transmission of new PdelayReq required
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Pdelay_CheckForPdelayRespTimeout(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TriggerPdelayCalculation
 *********************************************************************************************************************/
/*! \brief      Trigger the calculation of a new Pdelay.
 *  \details    Check if a new Pdelay value can be computed. If so trigger the calculation of the new Pdelay.
 *  \param[in]  PortIdx         Index of the port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_TriggerPdelayCalculation(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ComputePdelay
 *********************************************************************************************************************/
/*! \brief      Calculation of path delay time for the given Port
 *  \param[in]  PortIdx         Index of the used port
 *  \return     E_OK - Calculation was successful
 *  \return     E_NOT_OK - Calculation was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_ComputePdelay(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdelayRx_IsReadyToReceivePdelayResp
 *********************************************************************************************************************/
/*! \brief      Checks if a Pdelay response message can currently be received on the given Port without respect to the
 *              Pdelay request state machine state
 *  \param[in]  PortIdx   Index of the port
 *  \return     TRUE - Ready to receive Pdelay response message
 *  \return     FALSE - Not ready to receive Pdelay response message
 *  \context    ANY
 *********************************************************************************************************************/
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdelayRx_IsReadyToReceivePdelayResp(
  EthTSyn_PortIdxType PortIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_PdelayRx_ReceivePdelayResp
 *********************************************************************************************************************/
/*! \brief      Checks the state of the Pdelay request state machine and performs required action for the reception
 *              of the Pdelay response message
 *  \param[in]  PortIdx     Index of the Port the Pdelay request was received on
 *  \param[in]  ComMsgPtr   Pointer to the received Pdelay response message
 *  \context    ANY
 *********************************************************************************************************************/
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayRx_ReceivePdelayResp(
                  EthTSyn_PortIdxType        PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_PdelayRx_IsReadyToReceivePdelayRespFup
 *********************************************************************************************************************/
/*! \brief      Checks if a Pdelay response follow up message can currently be received on the given Port
 *  \param[in]  PortIdx   Index of the port
 *  \return     TRUE - Ready to receive Pdelay response follow up message
 *  \return     FALSE - Not ready to receive Pdelay response follow up message
 *  \context    ANY
 *********************************************************************************************************************/
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdelayRx_IsReadyToReceivePdelayRespFup(
  EthTSyn_PortIdxType PortIdx); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TxPdelayReqMsg
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_TxPdelayReqMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(uint8) txBufU8Ptr;
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);
  uint8 bufIdx;
  Std_ReturnType retVal = E_NOT_OK;
  uint16 txBufferSize = ETHTSYN_MSG_P_DELAY_REQ_LENGTH;
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthSwt_MgmtInfoType mgmtInfo;
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) mgmtInfoPtr = NULL_PTR;
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is ready for transmission and get an Ethernet TxBuffer. */
  if(EthTSyn_PdelayTx_GetTxBufferForPdelayReq(PortIdx, &txBufU8Ptr, &bufIdx, &txBufferSize) == E_OK)
  {
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #20 Reload timeout interval and increment SequenceId. */
    pDelayReqSmPtr->PdelayReq.TxIntervalCnt = ETHTSYN_CFG_GET_P_DELAY_REQ_SEND_INTERVAL_MF_CYCLES(PortIdx);
    pDelayReqSmPtr->PdelayReq.SequenceId++;

    /* #30 Copy PdelayReq to Ethernet TxBuffer. */
    EthTSyn_PdelayTx_CopyPdelayReqToTxBuffer(PortIdx, txBufU8Ptr);

    pDelayReqSmPtr->TxBufIdx = bufIdx;

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
#  if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    pDelayReqSmPtr->TxBufPtr = txBufU8Ptr;
#  endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    /* #40 Get switch management info and frame id if applicable. */
    if(EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId(PortIdx, txBufferSize, txBufU8Ptr, &mgmtInfo) == E_OK)
    {
#  if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
#   if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
      pDelayReqSmPtr->FrameId = mgmtInfo.FrameId;
#   endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
#  endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */

      mgmtInfoPtr = &mgmtInfo;
    }
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    pDelayReqSmPtr->PdelayReq.EgressTimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #50 Try to enable egress timestamp. */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    retVal = EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq(PortIdx, bufIdx, mgmtInfoPtr);
# else
    retVal = EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq(PortIdx, bufIdx);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    if(retVal == E_OK)
    {
      /* #500 Successfully enabled egress timestamp -> trigger transmission. */
      txBufferSize = ETHTSYN_MSG_P_DELAY_REQ_LENGTH;
    }
    else
    {
      /* #501 Failed to enable egress timestamp -> release message buffer and retry transmission later. */
      txBufferSize = 0;
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    mgmtInfoPtr = NULL_PTR;
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    }

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, txBufferSize, TRUE, mgmtInfoPtr);
# else
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, txBufferSize, TRUE);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    if(txBufferSize == 0)
    {
      retVal = E_NOT_OK;
    }

    if(retVal == E_OK)
    {
      pDelayReqSmPtr->TxConfirmationPending++;
    }
  }

  return retVal;
} /* EthTSyn_Pdelay_TxPdelayReqMsg() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_GetTxBufferForPdelayReq
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_GetTxBufferForPdelayReq(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr, ETHTSYN_P2VAR(uint8) BufIdxPtr,
  ETHTSYN_P2VAR(uint16) LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  *LenBytePtr = ETHTSYN_MSG_P_DELAY_REQ_LENGTH;
  /* #10 Check if Port is ready for transmission. */
  if(EthTSyn_IsTxReady(PortIdx) == TRUE)
  {
    /* #20 Verify that Pdelay Initiator is present for the given Port. */
    if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
    {
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      /* #30 Get Ethernet TxBuffer. */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
        ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), BufIdxPtr, (ETHTSYN_P2VAR(Eth_DataType*)) TxBufPtrPtr,
        LenBytePtr) == BUFREQ_OK)
      {
        retVal = E_OK;
      }
    }
  }
  return retVal;
} /* EthTSyn_PdelayTx_GetTxBufferForPdelayReq() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_CopyPdelayReqToTxBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayTx_CopyPdelayReqToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PDelayReqMsgType) pDelayReqPtr;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comMsgPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  comMsgPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))TxBufPtr;
  pDelayReqPtr = (ETHTSYN_P2VAR(EthTSyn_PDelayReqMsgType))&TxBufPtr[ETHTSYN_HDR_COM_LENGTH];

  /* #10 Set common header message attributes. */
  EthTSyn_UtilTx_PrepareCommonMsgHdr(comMsgPtr, ETHTSYN_MSG_TYPE_PDELAY_REQ, PortIdx);

  /* #20 Set specific message type attributes. */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))pDelayReqPtr->Reserved_0, 0,
      ETHTSYN_MSG_P_DELAY_REQ_RESERVED_0_LENGTH);
  IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))pDelayReqPtr->Reserved_1, 0,
      ETHTSYN_MSG_P_DELAY_REQ_RESERVED_1_LENGTH);

} /* EthTSyn_PdelayTx_CopyPdelayReqToTxBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq(
  EthTSyn_PortIdxType PortIdx, uint8 BufIdx, ETHTSYN_P2CONST(EthSwt_MgmtInfoType)  MgmtInfoPtr)
# else
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq(
  EthTSyn_PortIdxType PortIdx, uint8 BufIdx)
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) )
  retVal = E_OK;
  ETHTSYN_DUMMY_STATEMENT(BufIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) ) */
# if ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) )
  ETHTSYN_DUMMY_STATEMENT(MgmtInfoPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /*( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) ) */

# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  if(MgmtInfoPtr != NULL_PTR)
  {
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    retVal = EthIf_SwitchEnableEgressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx, MgmtInfoPtr);
  }
  else
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
  {
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    retVal = EthIf_EnableEgressTimestamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx);
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
  }

  return retVal;
} /* EthTSyn_PdelayTx_EnableEgressTimestampForPdelayReq() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TxPdelayRespMsg
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
ETHTSYN_LOCAL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_TxPdelayRespMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);
  ETHTSYN_P2VAR(uint8) txBufU8Ptr;
  uint8 bufIdx;
  uint16 txBufferSize;
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthSwt_MgmtInfoType mgmtInfo;
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) mgmtInfoPtr = NULL_PTR;
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is ready for transmission and get an Ethernet TxBuffer. */
  if(EthTSyn_PdelayTx_GetTxBufferForPdelayResp(PortIdx, &txBufU8Ptr, &bufIdx, &txBufferSize) == E_OK)
  {
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #20 Copy the Pdelay response to the Ethernet TxBuffer. */
    EthTSyn_PdelayTx_CopyPdelayRespToTxBuffer(PortIdx, txBufU8Ptr);

    /* #30 Store buffer index for TxConfirmation. */
    pDelayRespSmPtr->TxBufIdx = bufIdx;

# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    /* #40 Store Tx buffer pointer if required for later switch timestamp callouts. */
    pDelayRespSmPtr->TxBufPtr = txBufU8Ptr;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

    /* #50 Get switch management info and frame id if applicable. */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    if(EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId(PortIdx, txBufferSize, txBufU8Ptr, &mgmtInfo) == E_OK)
    {
      mgmtInfoPtr = &mgmtInfo;
#  if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
    /* #500 Store frame id if applicable. */
      pDelayRespSmPtr->FrameId = mgmtInfo.FrameId;
#  endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
    }
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    pDelayRespSmPtr->PdelayResp.EgressTimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
    pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_RESP_EGRESS_TIMESTAMP;

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #60 Try to enable egress timestamp. */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    retVal = EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp(PortIdx, bufIdx, mgmtInfoPtr);
# else
    retVal = EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp(PortIdx, bufIdx);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

    if(retVal == E_OK)
    {
      /* #600 Successfully enabled egress timestamp -> trigger transmission. */
      txBufferSize = ETHTSYN_MSG_P_DELAY_RESP_LENGTH;
    }
    else
    {
      /* #601 Failed to enable egress timestamp -> release message buffer and retry transmission later. */
      txBufferSize = 0;
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      mgmtInfoPtr = NULL_PTR;
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    }

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, txBufferSize, TRUE, mgmtInfoPtr);
# else
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, txBufferSize, TRUE);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    if(txBufferSize == 0)
    {
      retVal = E_NOT_OK;
    }

  }

  return retVal;
} /* EthTSyn_Pdelay_TxPdelayRespMsg() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_GetTxBufferForPdelayResp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_GetTxBufferForPdelayResp(
  EthTSyn_PortIdxType  PortIdx, ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr, ETHTSYN_P2VAR(uint8) BufIdxPtr,
  ETHTSYN_P2VAR(uint16) LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  *LenBytePtr = ETHTSYN_MSG_P_DELAY_RESP_LENGTH;
  /* #10 Verify that port is ready for transmission. */
  if(EthTSyn_IsTxReady(PortIdx) == TRUE)
  {
    /* #20 Verify that Pdelay request ingress timestamp is ready for processing. */
    if(EthTSyn_IsTimestampReadyToProcess(pDelayRespSmPtr->PdelayReq.IngressTimestampState) == TRUE)
    {
      /* #30 Get Ethernet transmission buffer from EthIf. */
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      if(EthIf_ProvideTxBuffer(
          ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE, ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx),
          BufIdxPtr, (ETHTSYN_P2VAR(Eth_DataType*))TxBufPtrPtr, LenBytePtr) == BUFREQ_OK)
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_PdelayTx_GetTxBufferForPdelayResp() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_CopyPdelayRespToTxBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayTx_CopyPdelayRespToTxBuffer( EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2VAR(uint8) TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayRespMsgType) pDelayRespMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* Set message pointers */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  comMsgPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))TxBufPtr;
  pDelayRespMsgPtr = (ETHTSYN_P2VAR(EthTSyn_PdelayRespMsgType))&TxBufPtr[ETHTSYN_HDR_COM_LENGTH];

  /* #10 Initialize common message header. */
  EthTSyn_UtilTx_PrepareCommonMsgHdr(comMsgPtr, ETHTSYN_MSG_TYPE_PDELAY_RESP, PortIdx);

  /* #20 Set message specific attributes. */
  EthTSyn_SetClockIdentity(pDelayRespMsgPtr->RequestingClockIdentity,
      pDelayRespSmPtr->PdelayReq.SourcePortIdentity.ClockIdentity);

  pDelayRespMsgPtr->RequestingPortNumber =
      (uint16)EthTSyn_Htons(pDelayRespSmPtr->PdelayReq.SourcePortIdentity.PortNumber);

  /* #30 Check if Pdelay request ingress timestamp is available. */
  if(EthTSyn_Util_IsTimestampAvailable(pDelayRespSmPtr->PdelayReq.IngressTimestampState) == TRUE)
  {
    /* #300 Pdelay request ingress timestamp available -> set request received timestamp. */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    EthTSyn_UtilTx_HtonTimestamp(
        (ETHTSYN_P2VAR(EthTSyn_TimestampType))pDelayRespMsgPtr->RequestReceiptTimestampSeconds,
        &pDelayRespSmPtr->PdelayReq.IngressTimestamp);
  }
  else
  {
    /* #301 Pdelay request ingress timestamp not available -> set request received timestamp to '0'. */
    IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))pDelayRespMsgPtr->RequestReceiptTimestampSeconds, 0u,
        ETHTSYN_TIMESTAMP_SIZE_BYTE);
  }
}

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp(
  EthTSyn_PortIdxType PortIdx, uint8 BufIdx, ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr)
# else
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp(
  EthTSyn_PortIdxType PortIdx, uint8 BufIdx)
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
# if ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) )
  retVal = E_OK;
  ETHTSYN_DUMMY_STATEMENT(BufIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) ) */

# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  if(MgmtInfoPtr != NULL_PTR)
  {
    /* #10 Enable switch egress timestamping for a switch port. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    retVal = EthIf_SwitchEnableEgressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx, MgmtInfoPtr);
  }
  else
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
  {
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    /* #20 Try to enable the egress timestamping for an end-station port. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    retVal = EthIf_EnableEgressTimestamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx);
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
  }

  return retVal;
} /* EthTSyn_PdelayTx_EnableEgressTimestampForPdelayResp() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TxPdelayRespFollowUpMsg
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
ETHTSYN_LOCAL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_TxPdelayRespFollowUpMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2VAR(uint8) txBufU8Ptr;
  uint8 bufIdx;
  uint16 txBufferSize;
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthSwt_MgmtInfoType mgmtInfo;
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) mgmtInfoPtr = NULL_PTR;
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check if Port is ready for transmission and get an Ethernet TxBuffer. */
  if(EthTSyn_PdelayTx_GetTxBufferForPdelayRespFup(PortIdx, &txBufU8Ptr, &bufIdx, &txBufferSize) == E_OK)
  {
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #20 Copy PdelayResp FollowUp message to TxBuffer. */
    EthTSyn_PdelayTx_CopyPdelayRespFupToTxBuffer(PortIdx, txBufU8Ptr);

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    /* #30 Get Switch management info if required. */
    if(EthTSyn_Util_IsSwitchPort(PortIdx) == TRUE)
    {
      mgmtInfo.SwitchIdx = EthTSyn_PortConfig[PortIdx].SwtInfo.SwitchIdx;
      mgmtInfo.PortIdx = EthTSyn_PortConfig[PortIdx].SwtInfo.PortIdx;
# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
      /* #300 Get frame id if required. */
      mgmtInfo.FrameId = EthTSyn_Util_GetFrameId(txBufU8Ptr, txBufferSize);
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */

      mgmtInfoPtr = &mgmtInfo;
    }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #40 Trigger transmission. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_MSG_P_DELAY_RESP_FOLLOW_UP_LENGTH, FALSE, mgmtInfoPtr);
#else
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_MSG_P_DELAY_RESP_FOLLOW_UP_LENGTH, FALSE);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  }

  return retVal;
} /* EthTSyn_Pdelay_TxPdelayRespFollowUpMsg() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_GetTxBufferForPdelayRespFup
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdelayTx_GetTxBufferForPdelayRespFup(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr, ETHTSYN_P2VAR(uint8) BufIdxPtr,
  ETHTSYN_P2VAR(uint16) LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  *LenBytePtr = ETHTSYN_MSG_P_DELAY_RESP_FOLLOW_UP_LENGTH;
  /* #10 Check if Port is ready for transmission. */
  if(EthTSyn_IsTxReady(PortIdx) == TRUE)
  {
    /* #20 Check if egress timestamp of PdelayResp is ready for processing. */
    if(EthTSyn_IsTimestampReadyToProcess(pDelayRespSmPtr->PdelayResp.EgressTimestampState) == TRUE)
    {
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      /* #30 Get Ethernet transmission buffer. */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
          ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), BufIdxPtr, (ETHTSYN_P2VAR(Eth_DataType*)) TxBufPtrPtr,
          LenBytePtr) == BUFREQ_OK)
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_PdelayTx_GetTxBufferForPdelayRespFup() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayTx_CopyPdelayRespFupToTxBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayTx_CopyPdelayRespFupToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PDelayResFollowUpMsgType) pDelayRespFollowUpPtr;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF)
  EthTSyn_GlobalTimestampType pDelayRespEgressTimestamp;
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* Set message pointers. */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  comMsgPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))TxBufPtr;
  pDelayRespFollowUpPtr = (ETHTSYN_P2VAR(EthTSyn_PDelayResFollowUpMsgType))&TxBufPtr[ETHTSYN_HDR_COM_LENGTH];

  /* #10 Set common header message attributes. */
  EthTSyn_UtilTx_PrepareCommonMsgHdr(comMsgPtr, ETHTSYN_MSG_TYPE_PDELAY_RES_FOLLOW_UP, PortIdx);

  /* #20 Set specific message type attributes. */
  EthTSyn_SetClockIdentity(pDelayRespFollowUpPtr->RequestingClockIdentity,
      pDelayRespSmPtr->PdelayReq.SourcePortIdentity.ClockIdentity);

  pDelayRespFollowUpPtr->RequestingPortNumber =
      (uint16)EthTSyn_Htons(pDelayRespSmPtr->PdelayReq.SourcePortIdentity.PortNumber);

  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  /* HW-Timestamping */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  EthTSyn_UtilTx_HtonTimestamp((ETHTSYN_P2VAR(EthTSyn_TimestampType))
      pDelayRespFollowUpPtr->ResponseOriginTimestampSeconds,
      &pDelayRespSmPtr->PdelayResp.EgressTimestamp);
#else
  /* SW-Timestamping */
  pDelayRespEgressTimestamp.secondsHi = 0u;
  pDelayRespEgressTimestamp.seconds = 0u;
  pDelayRespEgressTimestamp.nanoseconds = pDelayRespSmPtr->PdelayResp.EgressTimestamp.nanoseconds;
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  EthTSyn_UtilTx_HtonTimestamp((ETHTSYN_P2VAR(EthTSyn_TimestampType))
      pDelayRespFollowUpPtr->ResponseOriginTimestampSeconds,
      &pDelayRespEgressTimestamp);
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

} /* EthTSyn_PdelayTx_CopyPdelayRespFupToTxBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_CheckForPdelayRespTimeout
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Pdelay_CheckForPdelayRespTimeout(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean txPdelayReq = FALSE;
  boolean lostResponse = FALSE;
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Pdelay request transmission interval elapsed. */
  ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (pDelayReqSmPtr->PdelayReq.TxIntervalCnt == 0u)
  {
    /* #100 Interval elapsed -> mark lost response and transmission. */
    txPdelayReq = TRUE;
    lostResponse = TRUE;
  }
  else
  {
    /* #101 Interval not elapsed -> Check for Pdelay response timeout. */
    if(ETHTSYN_CFG_GET_P_DELAY_RESP_RESP_FOLLOW_UP_TIMEOUT(PortIdx) > 0)
    {
      if(pDelayReqSmPtr->PdelayRespRespFollowUpTimeout == 0u)
      {
        lostResponse = TRUE;
      }
    }
  }

  /* #20 Check for lost response. */
  if(lostResponse == TRUE)
  {
    /* #200 Lost response -> reset PdelayReqSm state to WAITING_FOR_PDELAY_INTERVAL_COUNTER */
    pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_PDELAY_INTERVAL_COUNTER;

    /* #201 Check for lost response limit. */
    if(pDelayReqSmPtr->LostResponses < ETHTSYN_CFG_GET_P_DELAY_ALLOWED_LOST_RESPONSES(PortIdx))
    {
      /* #2010 Lost response limit not exceeded -> Increment lost response counter. */
      pDelayReqSmPtr->LostResponses++;
    }
    else
    {
      /* #2011 Lost response limit exceeded -> Set AsCapable to FALSE if AlwaysAsCapable is FALSE for the Port. */
      pDelayReqSmPtr->AsCapable = ETHTSYN_CFG_GET_ALWAYS_AS_CAPABLE(PortIdx);
    }
  }

  ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return txPdelayReq;
} /* EthTSyn_Pdelay_CheckForPdelayRespTimeout() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_TriggerPdelayCalculation
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_TriggerPdelayCalculation(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);
  EthTSyn_PortIdxType activePortIdx = ETHTSYN_CFG_GET_P_DELAY_ACTIVE_PORT_IDX(PortIdx);
  boolean newAsCapable = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that TxConfirmation for sent Pdelay request occurred. */
  if(pDelayReqSmPtr->TxConfirmationPending == 0)
  {
    /* #20 Verify that Pdelay request egress timestamp is ready for processing. */
    if(EthTSyn_IsTimestampReadyToProcess(pDelayReqSmPtr->PdelayReq.EgressTimestampState) == TRUE)
    {
      /* #30 All values of Pdelay request sequence are available --> Compute new Pdelay, set AsCapable and
       * change the state of the PdelayReqSm to WAITING_FOR_PDELAY_INTERVAL_COUNTER. */
      if (ETHTSYN_CFG_GET_P_DELAY_USE_STATIC(PortIdx) == FALSE)
      {
        newAsCapable = (boolean)(EthTSyn_Pdelay_ComputePdelay(activePortIdx) == E_OK);
      }

      EthTSyn_Util_SetAsCapable(PortIdx, newAsCapable);
      pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_PDELAY_INTERVAL_COUNTER;
    }
  }
} /* EthTSyn_Pdelay_TriggerPdelayCalculation() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ComputePdelay
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_ComputePdelay(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr;
  EthTSyn_TimediffType localResidenceTime;
  EthTSyn_TimediffType remoteResidenceTime = 0;
  EthTSyn_TimediffType pDelay;  /* PRQA S 0781 */ /* MD_EthTSyn_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  /* computes the mean propagation delay on the link (see also IEEE802.1AS-2011 Figure 11-1) */
  /* t1: Pdelay_Req egress timestamp (initiator), t2: Pdelay_Req ingress timestamp (responder) */
  /* t3: Pdelay_Resp egress timestamp (responder), t4: Pdelay_Resp ingress timestamp (initiator) */
  /*      (t4 - t1) - (t3 - t2)                      */
  /* D = ___________________________                 */
  /*              2                                  */

  pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);

  /* #10 Compute local residence time. */
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  /* HW-Timestamping */
  localResidenceTime = 0;
  if(EthTSyn_Util_TimestampMinusTimestamp(&ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayResp.IngressTimestamp,
      &ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.EgressTimestamp, &localResidenceTime) == E_OK)
# else
    /* SW-Timestamping: Only 'raw' Timestamps are used for PdelayReq egress and PdelayResp ingress. These are stored
     * in the 'nanoseconds' field of the Timestamp */
    localResidenceTime = (EthTSyn_TimediffType)
    ((EthTSyn_TimediffType)pDelayReqSmPtr->PdelayResp.IngressTimestamp.nanoseconds -
        (EthTSyn_TimediffType)pDelayReqSmPtr->PdelayReq.EgressTimestamp.nanoseconds);
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
  {
    /* #20 Successfully computed local residence time -> compute remote residence time. */
    if(EthTSyn_Util_TimestampMinusTimestamp(&pDelayReqSmPtr->PdelayRespFollowUp.ResponseOriginTimestamp,
        &pDelayReqSmPtr->PdelayResp.RequestReceiptTimestamp, &remoteResidenceTime) == E_OK)
    {
      /* #30 Successfully computed remote residence time --> calculate current path delay. */
      pDelay = (localResidenceTime - remoteResidenceTime) / 2u;
      /* #40 Perform sanity and threshold check of path delay. */
      if((pDelay >= 0u) &&
          (pDelay < (EthTSyn_TimediffType)ETHTSYN_CFG_GET_P_DELAY_NEIGHBOR_DELAY_THRESHOLD_NS(PortIdx)))
      {
        /* #50 Valid Pdelay -> low pass filtering with weighted previous value for averaging. */
        pDelayReqSmPtr->Pdelay = (EthTSyn_PdelayType)
                  (((ETHTSYN_CFG_GET_P_DELAY_AVERAGE_WEIGHT(PortIdx) - 1) *
                      pDelayReqSmPtr->Pdelay) + (EthTSyn_PdelayType)pDelay) /
                      (ETHTSYN_CFG_GET_P_DELAY_AVERAGE_WEIGHT(PortIdx));
        retVal = E_OK;
      }
    }
  }

  return retVal;
}/* EthTSyn_Pdelay_ComputePdelay() */

/**********************************************************************************************************************
 *  EthTSyn_PdelayRx_IsReadyToReceivePdelayResp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdelayRx_IsReadyToReceivePdelayResp(
  EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isReady = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is an active Pdelay port. */
  if(EthTSyn_CfgAccess_IsActivePdelayPort(PortIdx) == TRUE)
  {
    /* #20 Check if Pdelay initiator is activated. */
    if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
    {
      isReady = TRUE;
    }
  }

  return isReady;
} /* EthTSyn_PdelayRx_IsReadyToReceivePdelayResp() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_PdelayRx_ReceivePdelayResp
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
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayRx_ReceivePdelayResp(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);
  ETHTSYN_P2CONST(EthTSyn_PdelayRespMsgType) pDelayRespMsgPtr;
  boolean lostResponse = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check Pdelay request state machine state. */
  if(pDelayReqSmPtr->State == ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP)
  {
    /* #100 Waiting for Pdelay response -> continue message processing. */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    pDelayRespMsgPtr = (ETHTSYN_P2CONST(EthTSyn_PdelayRespMsgType))
                          &((ETHTSYN_P2CONST(uint8))ComMsgPtr)[ETHTSYN_HDR_COM_LENGTH];

    /* #101 Check for matching Sequence Id and requesting source port identity. */
    /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
    if((EthTSyn_PortIdentityIsEqualLocalPort(PortIdx,
        (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))pDelayRespMsgPtr->RequestingClockIdentity) == TRUE) &&
        ((uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId) == pDelayReqSmPtr->PdelayReq.SequenceId))
    {
      /* #1010 Store message attributes. */
      pDelayReqSmPtr->PdelayResp.SequenceId = (uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId);
      EthTSyn_SetClockIdentity(pDelayReqSmPtr->PdelayResp.SourcePortIdentity.ClockIdentity,
          ComMsgPtr->SourceClockIdentity);
      pDelayReqSmPtr->PdelayResp.SourcePortIdentity.PortNumber = (EthTSyn_PortNumberType)
                            EthTSyn_Ntohs(ComMsgPtr->SourcePortNumber);
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      EthTSyn_NtohTimestamp(&pDelayReqSmPtr->PdelayResp.RequestReceiptTimestamp,
          (ETHTSYN_P2VAR(EthTSyn_TimestampType))pDelayRespMsgPtr->RequestReceiptTimestampSeconds);

      /* #1011 Change state of the pdelay request state machine. */
      pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP;
    }
    else
    {
      lostResponse = TRUE;
    }
  }
  /* #20 Check for double Pdelay response */
  else if(pDelayReqSmPtr->State == ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP)
  {
    if(pDelayReqSmPtr->PdelayReq.SequenceId == (uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId))
    {
      /* #200 Received Pdelay response with same sequence id twice -> lost response. */
      lostResponse = TRUE;
    }
  }
  else
  {
    /* Nothing to do here (MISRA) */
  }

  /* #30 Check for lost response. */
  if(lostResponse == TRUE)
  {
    /* #300 Invalid message attributes -> treat as lost response. */
    if(pDelayReqSmPtr->LostResponses < ETHTSYN_CFG_GET_P_DELAY_ALLOWED_LOST_RESPONSES(PortIdx))
    {
      pDelayReqSmPtr->LostResponses++;
    }
    else
    {
      /* Lost response limit exceeded -> Mark as not AsCapable if port is not set to 'AlwaysAsCapable'. */
      EthTSyn_Util_SetAsCapable(PortIdx, ETHTSYN_CFG_GET_ALWAYS_AS_CAPABLE(PortIdx));
    }

    /* #301 Trigger re-transmission of the pdelay request */
    pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_SEND_P_DELAY_REQ;
  }
} /* EthTSyn_PdelayRx_ReceivePdelayResp() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_PdelayRx_IsReadyToReceivePdelayRespFup
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdelayRx_IsReadyToReceivePdelayRespFup(
  EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isReady = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is an active Pdelay port. */
  if(EthTSyn_CfgAccess_IsActivePdelayPort(PortIdx) == TRUE)
  {
    /* #20 Check if Pdelay initiator is activated. */
    if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
    {
      /* #30 Pdelay initiator is enabled -> check State of Pdelay Request state machine. */
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      if(ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->State == ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP)
      {
        isReady = TRUE;
      }
    }
  }

  return isReady;
} /* EthTSyn_PdelayRx_IsReadyToReceivePdelayRespFup() */
#endif /* #if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessSmPdelayReq
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
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessSmPdelayReq(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);
  EthTSyn_PortIdxType activePortIdx = ETHTSYN_CFG_GET_P_DELAY_ACTIVE_PORT_IDX(PortIdx);
  boolean txPdelayReq = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check Pdelay request state machine state. */
  switch(pDelayReqSmPtr->State)
  {
  case ETHTSYN_STATE_P_DELAY_REQ_NOT_ENABLED:
    /* #100 State: Not enabled -> Enable state machine. */
    if(ETHTSYN_STATE_GET_LINK_STATE(activePortIdx) == ETHTRCV_LINK_STATE_ACTIVE)
    {
      pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_INITIAL_SEND_P_DELAY_REQ;
    }
    break;

  case ETHTSYN_STATE_P_DELAY_REQ_INITIAL_SEND_P_DELAY_REQ:
  case ETHTSYN_STATE_P_DELAY_REQ_SEND_P_DELAY_REQ:
    /* #101 State: Send Pdelay request -> mark transmission of Pdelay request. */
    txPdelayReq = TRUE;
    break;

  case ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP:
  case ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP:
    /* #102 State: Witing for Pdelay response/response follow up -> Check and handle PdelayResp or
     * PdelayRespFollowUp timeout. */
    txPdelayReq = EthTSyn_Pdelay_CheckForPdelayRespTimeout(PortIdx);
    break;

  case ETHTSYN_STATE_P_DELAY_REQ_RECEIVED_PDELAY_RESP_AND_PDELAY_RESP_FOLLOW_UP:
    /* #103 State: Received Pdelay response and response follow up -> trigger Pdelay calculation. */
    EthTSyn_Pdelay_TriggerPdelayCalculation(PortIdx);
    break;

  case ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_PDELAY_INTERVAL_COUNTER:
    /* #104 State: Waiting for Pdelay interval counter -> Handle Pdelay interval counter. */
    if(pDelayReqSmPtr->PdelayReq.TxIntervalCnt == 0u)
    {
      txPdelayReq = TRUE;
    }
    break;

  default:
    /* Nothing to do here (MISRA). */
    break;
  } /* END: switch(pDelayReqSmPtr->State) */

  /* #20 Check if Pdelay request should be transmitted. */
  if(txPdelayReq == TRUE)
  {
    /* #200 Yes -> transmit Pdelay request. */
    if(EthTSyn_Pdelay_TxPdelayReqMsg(activePortIdx) == E_OK)
    {
      pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP;
      pDelayReqSmPtr->PdelayRespRespFollowUpTimeout =
        ETHTSYN_CFG_GET_P_DELAY_RESP_RESP_FOLLOW_UP_TIMEOUT(activePortIdx);
    }
    else
    {
      pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_SEND_P_DELAY_REQ;
    }
  }

} /* EthTSyn_Pdelay_ProcessSmPdelayReq() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessSmPdelayResp
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
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessSmPdelayResp(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);
  EthTSyn_PortIdxType activePortIdx = ETHTSYN_CFG_GET_P_DELAY_ACTIVE_PORT_IDX(PortIdx);
# if (ETHTSYN_P_DELAY_CONFIG_COUNT <= 1u)
  ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* (ETHTSYN_P_DELAY_CONFIG_COUNT <= 1u) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check Pdelay response state machine state. */
  switch(pDelayRespSmPtr->State)
  {
  case ETHTSYN_STATE_P_DELAY_RESP_NOT_ENABLED:
    /* #100 State: Not enabled -> enable Pdelay response state machine. */
    if(ETHTSYN_STATE_GET_LINK_STATE(activePortIdx) == ETHTRCV_LINK_STATE_ACTIVE)
    {
      pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_INITAIL_WAITING_FOR_PDELAY_REQ;
    }
    break;

  case ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP:
    /* #101 State: Send Pdelay response -> transmit Pdelay response. */
    if(EthTSyn_Pdelay_TxPdelayRespMsg(activePortIdx) == E_OK)
    {
      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      pDelayRespSmPtr->TxConfirmationPending++;
      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    break;

  case ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_RESP_EGRESS_TIMESTAMP:
    /* #102 State: Waiting for Pdelay response egress timestamp -> Check if TxConfirmation for send Pdelay response
     * occurred */
    if(pDelayRespSmPtr->TxConfirmationPending == 0)
    {
      /* #1020 TxConfirmation occurred -> Transmit Pdelay response follow up. */
      if(EthTSyn_Pdelay_TxPdelayRespFollowUpMsg(activePortIdx) == E_OK)
      {
        pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_REQ;
      }
      else
      {
        pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP_FOLLOW_UP;
      }
    }
    break;

  case ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP_FOLLOW_UP:
    /* #103 State: Send Pdelay response follow up -> transmit Pdelay response follow up. */
    if(EthTSyn_Pdelay_TxPdelayRespFollowUpMsg(activePortIdx) == E_OK)
    {
      pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_REQ;
    }
    break;

  default:
    /* This should never happen (MISRA) */
    break;
  }

} /* EthTSyn_Pdelay_ProcessSmPdelayResp() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_HandleCyclicTasks
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
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_HandleCyclicTasks(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdelayIdxType pdelayIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all Pdelay configurations */
  for(pdelayIdx = 0; pdelayIdx < ETHTSYN_P_DELAY_CONFIG_COUNT; pdelayIdx++)
  {
    /* #20 Check if Pdelay initiator is activated for the current Pdelay configuration. */
    if(EthTSyn_PdelayConfig[pdelayIdx].PdelayInitiatorCfgPtr != NULL_PTR)
    {
      ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pdelayReqSmPtr =
        EthTSyn_PdelayConfig[pdelayIdx].PdelayInitiatorCfgPtr->PdelayReqSmPtr;
      /* #200 Verify that active port of the current Pdelay configuration is ready for transmission. */
      if(EthTSyn_IsTxReady(EthTSyn_PdelayConfig[pdelayIdx].ActivePortIdx) == TRUE)
      {
        /* #2000 Handle timers and timeouts of Pdelay request state machine. */
        ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if(pdelayReqSmPtr->PdelayReq.TxIntervalCnt > 0u)
        {
          pdelayReqSmPtr->PdelayReq.TxIntervalCnt--;
        }
        if(pdelayReqSmPtr->PdelayRespRespFollowUpTimeout > 0u)
        {
          pdelayReqSmPtr->PdelayRespRespFollowUpTimeout--;
        }
        ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
      /* #201 Process Pdelay request state machine. */
      EthTSyn_Pdelay_ProcessSmPdelayReq(EthTSyn_PdelayConfig[pdelayIdx].ActivePortIdx);
    }

    /* #30 Check of Pdelay responder is activated for the current Pdelay configuration. */
    if(EthTSyn_PdelayConfig[pdelayIdx].PdelayResponderCfgPtr != NULL_PTR)
    {
      /* #300 Process Pdelay response state machine. */
      EthTSyn_Pdelay_ProcessSmPdelayResp(EthTSyn_PdelayConfig[pdelayIdx].ActivePortIdx);
    }
  } /* END: Iteration Pdelay Config */
} /* EthTSyn_Pdelay_HandleCyclicTasks() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_GetCurrentPdelay
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_GetCurrentPdelay(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2VAR(EthTSyn_PdelayType) PdelayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for valid port index. */
  if(PortIdx < ETHTSYN_PORT_COUNT)
  {
    /* #20 Check for valid Pdelay index. */
    if(EthTSyn_CfgAccess_GetPdelayIndex(PortIdx) >= ETHTSYN_P_DELAY_CONFIG_COUNT)
    {
      *PdelayPtr = 0;
    }
    else
    {
      /* #30 Check if static Pdelay should be used. */
      if(ETHTSYN_CFG_GET_P_DELAY_USE_STATIC(PortIdx) == TRUE)
      {
        /* #300 Static Pdelay -> return the static value. */
        *PdelayPtr = ETHTSYN_CFG_GET_P_DELAY_INITIAL_NS(PortIdx);
        retVal = E_OK;
      }
      else
      {
        /* #301 No static Pdelay -> check if Pdelay initiator is activated for the port and return the corresponding
         * Pdelay. */
        if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
        {
          *PdelayPtr = ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR(PortIdx)->PdelayReqSmPtr->Pdelay;
          retVal = E_OK;
        }
        else
        {
          *PdelayPtr = 0;
          retVal = E_OK;
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_Pdelay_GetCurrentPdelay() */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessReceivedPdelayReqMsg
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
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessReceivedPdelayReqMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr;
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimestampQualityType    timestampQual;
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that Pdelay request was received on an active Pdelay port. */
  if(PortIdx == ETHTSYN_CFG_GET_P_DELAY_CFG(PortIdx).ActivePortIdx)
  {
    /* #20 Only process the Pdelay Request message if it was received on the EthTSyn Port which is used for Pdelay. */
    if(EthTSyn_CfgAccess_IsPdelayResponder(PortIdx) == TRUE)
    {
      /* #30 Pdelay responder is enabled -> continue message processing. */
      pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);
      
      /* #40 Get the ingress timestamp. */
      pDelayRespSmPtr->PdelayReq.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      /* PRQA S 0310, 0311 2 */ /* MD_EthTSyn_11.4, MD_EthTSyn_11.5 */
      if(EthIf_GetIngressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), (ETHTSYN_P2VAR(Eth_DataType))ComMsgPtr,
        &timestampQual, &pDelayRespSmPtr->PdelayReq.IngressTimestamp) == E_OK)
      {
        if(timestampQual == ETH_VALID)
        {
          pDelayRespSmPtr->PdelayReq.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
        }
      }
# else
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      if(StbM_GetCurrentTimeRaw(&pDelayRespSmPtr->PdelayReq.IngressTimestamp.nanoseconds) == E_OK)
      {
        pDelayRespSmPtr->PdelayReq.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
      }
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      /* #50 Only proceed if ingress timestamp is available. */
      if(pDelayRespSmPtr->PdelayReq.IngressTimestampState == ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE)
      {
        /* #60 Check Pdelay response state machine state. */
        if((pDelayRespSmPtr->State == ETHTSYN_STATE_P_DELAY_RESP_INITAIL_WAITING_FOR_PDELAY_REQ) ||
          (pDelayRespSmPtr->State == ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_REQ) ||
          (pDelayRespSmPtr->State == ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP_FOLLOW_UP))
        {
          /* #70 Store message attributes */

          /* Store Requesting source port identity */
          EthTSyn_SetClockIdentity(pDelayRespSmPtr->PdelayReq.SourcePortIdentity.ClockIdentity,
            ComMsgPtr->SourceClockIdentity);
          pDelayRespSmPtr->PdelayReq.SourcePortIdentity.PortNumber = (EthTSyn_PortNumberType)
            EthTSyn_Ntohs(ComMsgPtr->SourcePortNumber);

          /* Store sequence id */
          pDelayRespSmPtr->PdelayReq.SequenceId = (uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId);

          /* Change Pdelay response state machine state */
          pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP;
        }
      }
      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  
} /* EthTSyn_Pdelay_ProcessReceivedPdelayReqMsg() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessReceivedPdelayRespMsg
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
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessReceivedPdelayRespMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr;
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimestampQualityType timestampQual = ETH_INVALID;
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is ready to received Pdelay response. */
  if(EthTSyn_PdelayRx_IsReadyToReceivePdelayResp(PortIdx) == TRUE)
  {
    /* #20 Pdelay initiator is enabled -> continue message processing. */
    pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);

    /* #30 Get Ingress timestamp. */
    pDelayReqSmPtr->PdelayResp.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    /* HW-Timestamping */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    /* PRQA S 0310, 0311 2 */ /* MD_EthTSyn_11.4, MD_EthTSyn_11.5 */
    if(EthIf_GetIngressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), (ETHTSYN_P2VAR(Eth_DataType))ComMsgPtr,
        &timestampQual, &pDelayReqSmPtr->PdelayResp.IngressTimestamp) == E_OK)
    {
      if(timestampQual == ETH_VALID)
      {
        pDelayReqSmPtr->PdelayResp.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
      }
    }
# else
    /* SW-Timestamping */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    if(StbM_GetCurrentTimeRaw(&pDelayReqSmPtr->PdelayResp.IngressTimestamp.nanoseconds) == E_OK)
    {
      pDelayReqSmPtr->PdelayResp.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
    }
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #40 Check if ingress timestamp is available. */
    if(pDelayReqSmPtr->PdelayResp.IngressTimestampState == ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE)
    {
      /* #400 Timestamp available -> receive Pdelay response. */
      EthTSyn_PdelayRx_ReceivePdelayResp(PortIdx, ComMsgPtr);
    }
    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  }
} /* EthTSyn_Pdelay_ProcessReceivedPdelayRespMsg() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessReceivedRespFollowUpMsg
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
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessReceivedRespFollowUpMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr;
  ETHTSYN_P2CONST(EthTSyn_PDelayResFollowUpMsgType) pDelayRespFollowUpMsgPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is ready to receive a Pdelay response follow up message. */
  if(EthTSyn_PdelayRx_IsReadyToReceivePdelayRespFup(PortIdx) == TRUE)
  {
    /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
    pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);
    pDelayRespFollowUpMsgPtr = (ETHTSYN_P2CONST(EthTSyn_PDelayResFollowUpMsgType))
                        &((ETHTSYN_P2CONST(uint8))ComMsgPtr)[ETHTSYN_HDR_COM_LENGTH];

    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #20 Check message attributes (Sequence Id, Matching Port Identity for PdelayResp and PdelayRespFollowUp,
     * Matching Requesting Port Identity) */
    /* Check Sequnce Id */
    if((uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId) == pDelayReqSmPtr->PdelayReq.SequenceId)
    {
      /* Check Source Port Identity */
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      if(EthTSyn_PortIdentityIsEqual(
          (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))&pDelayReqSmPtr->PdelayResp.SourcePortIdentity,
          (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))ComMsgPtr->SourceClockIdentity) == TRUE)
      {
        /* Check Requesting Port Identity */
        /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
        if(EthTSyn_PortIdentityIsEqualLocalPort(PortIdx,
            (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))pDelayRespFollowUpMsgPtr->RequestingClockIdentity) ==
                TRUE)
        {
          /* #30 Store message attributes, reset lost responses and transit Pdelay request state machine. */
          /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
          EthTSyn_NtohTimestamp(&pDelayReqSmPtr->PdelayRespFollowUp.ResponseOriginTimestamp,
              (ETHTSYN_P2VAR(EthTSyn_TimestampType))pDelayRespFollowUpMsgPtr->ResponseOriginTimestampSeconds);

          /* Reset lost responses. */
          pDelayReqSmPtr->LostResponses = 0u;

          /* valid PdelayResp and PdelayRespFollowUp were received -> path delay calculation in MainFunction
           * context is possible, after PdelayReq message was transmitted successfully (indicated by
           * TxConfirmationPending flag)*/

          pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_RECEIVED_PDELAY_RESP_AND_PDELAY_RESP_FOLLOW_UP;
        }
      }
    }
    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

} /* EthTSyn_Pdelay_ProcessReceivedRespFollowUpMsg() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

#define ETHTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 * see file EthTSyn.c
*/

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Pdelay_Int.c
 *********************************************************************************************************************/
