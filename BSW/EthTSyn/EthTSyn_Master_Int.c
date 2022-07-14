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
/**        \file  EthTSyn_Master_Int.c
 *        \brief  EthTSyn Master source file - Internal Functions
 *
 *      \details  EthTSyn Master source file containing the EthTSyn Master implementation of the EthTSyn module.
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

#define ETHTSYN_MASTER_INT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Master_Int.h"
#include "EthTSyn_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
#include "EthTSyn_Util_Int.h"
#include "EthTSyn_Crc_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of Msn header file */
#if (  (ETHTSYN_SW_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_SW_MINOR_VERSION != (1u)) \
    || (ETHTSYN_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthTSyn_Master_Int.c and EthTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ETHTSYN_CFG_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_CFG_MINOR_VERSION != (1u)) )
# error "Version numbers of EthTSyn_Master_Int.c and EthTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* Wrap StbM-APIs (Ar4.3 -> Ar4.2) due to gradually implementation of new Ar3.4 APIs */ /*lint -e652 */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define StbM_GetOffset(timeBaseId, timeStamp, userData) (StbM_GetOffset((timeBaseId), (timeStamp)))
/*lint +e652 */

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
#define ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

VAR(EthTSyn_TimeBaseUpdateCounterHandlerType, ETHTSYN_VAR_NOINIT)
  EthTSyn_Master_TimeBaseUpdateCounterHandler[ETHTSYN_TIME_DOMAIN_COUNT];

#define ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define ETHTSYN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_Master_IsImmediateTimeSyncRequired
 *********************************************************************************************************************/
/*! \brief      Check if the an immediate time sync should be triggered for the given time domain
 *  \param[in]  TimeDomainIdx   Index of the TimeDomain
 *  \return     TRUE - Trigger immediate time sync
 *  \return     FALSE - Do not trigger immediate time sync
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Master_IsImmediateTimeSyncRequired(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_Master_IsGlobalTimeBaseStatusBit
 *********************************************************************************************************************/
/*! \brief      Check if the GLOBAL_TIME_BASE status bit of the StbM_TimeBaseStatus is set
 *  \param[in]  TimeDomainIdx   Index of the TimeDomain the status should be checked for
 *  \return     TRUE - GLOBAL_TIME_BASE status bit is set
 *  \return     FALSE - GLOBAL_TIME_BASE status bit is not set
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Master_IsGlobalTimeBaseStatusBit(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_Master_HasTimeBaseUpdateCounterChanged
 *********************************************************************************************************************/
/*! \brief      Check if the time base update counter has changed since last check for the given time domain
 *  \param[in]  TimeDomainIdx   Index of the TimeDomain
 *  \return     TRUE - Time base update counter has changed since last check
 *  \return     FALSE - Time base update counter has not changed since last check
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Master_HasTimeBaseUpdateCounterChanged(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_TxSyncMsg
 *********************************************************************************************************************/
/*! \brief      Transmits a Sync message on the given Port
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_TxSyncMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_GetTxBufferForSync
 *********************************************************************************************************************/
/*! \brief      Checks if Port is ready to transmit a Sync message and reserves an Ethernet TxBuffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtrPtr     Pointer to the Ethernet TxBuffer
 *  \param[out] BufIdxPtr       The index of the Ethernet TxBuffer
 *  \param[out] LenBytePtr      The length of the Ethernet TxBuffer
 *  \return     E_OK - Port is ready to transmit the Sync message and an Ethernet TxBuffer was locked successfully
 *  \return     E_NOT_OK - Port is not ready to transmit the Sync message or no Ethernet TxBuffer could be locked
 *  \note       Out parameters are (only) valid for return value of E_OK
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_GetTxBufferForSync(
                EthTSyn_PortIdxType   PortIdx,
  ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr,
  ETHTSYN_P2VAR(uint8)                BufIdxPtr,
  ETHTSYN_P2VAR(uint16)               LenBytePtr);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_EnableEgressTimestampForSync
 *********************************************************************************************************************/
/*! \brief      Enables egress timestamping for Sync message
 *  \param[in]  PortIdx      Index of the port that should transmit the message
 *  \param[in]  BufIdx       Index of the Ethernet TxBuffer
 *  \param[in]  MgmtInfoPtr  Pointer to the switch management information
 *  \return     E_OK - Successfully enabled egress timestamping
 *  \return     E_NOT_OK - Failed to enable egress timestamping
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_EnableEgressTimestampForSync(
                  EthTSyn_PortIdxType   PortIdx,
                  uint8                 BufIdx,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType)  MgmtInfoPtr);
# else
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_EnableEgressTimestampForSync(
  EthTSyn_PortIdxType  PortIdx,
  uint8                BufIdx);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_TxFollowUpMsg
 *********************************************************************************************************************/
/*! \brief      Transmits a FollowUp message on the given Port
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_TxFollowUpMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_IsFollowUpTxReady
 *********************************************************************************************************************/
/*! \brief      Checks if FollowUp message is ready for transmission on the given master Port
 *  \param[in]  PortIdx      Index of the port that should transmit the message
 *  \return     TRUE - FollowUp message is ready for transmission
 *  \return     FALSE - FollowUp message is not ready for transmission
 *  \context    EthTSyn_MasterTx_TxFollowUpMsg().
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_MasterTx_IsFollowUpTxReady(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_CopyFollowUpToTxBuffer
 *********************************************************************************************************************/
/*! \brief      Assemble FollowUp message for the given Port and store it in the given TxBuffer
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \param[out] TxBufPtr        The Assembled FollowUp message in network byte order
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_CopyFollowUpToTxBuffer(
                EthTSyn_PortIdxType  PortIdx,
  ETHTSYN_P2VAR(uint8)               TxBufPtr);

/**********************************************************************************************************************
 *  EthTSyn_Master_TxAnnounceMsg
 *********************************************************************************************************************/
/*! \brief      Transmits a Announce message
 *  \param[in]  PortIdx         Index of the port that should transmit the message
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Master_TxAnnounceMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpTlvs
 *********************************************************************************************************************/
/*! \brief          Append FollowUp AUTOSAR Tlvs
 *  \param[in]      PortIdx         Index of the port the FollowUp is transmitted on
 *  \param[in,out]  TxBufPtr  In:  The assembled FollowUp message in network byte order
 *                            Out: The appended FollowUp Ar Tlvs in network byte order
 *  \context        ANY
 *********************************************************************************************************************/
/* AR4.3 message format */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpTlvs(
                  EthTSyn_PortIdxType  PortIdx,
    ETHTSYN_P2VAR(uint8)               TxBufPtr);

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpTimeTlv
 *********************************************************************************************************************/
/*! \brief         Append FollowUp AUTOSAR Time TLV
 *  \param[in]     PortIdx         Index of the port the FollowUp is transmitted on
 *  \param[in]     DataId          DataId used for Crc calculation
 *  \param[in]     ComMsgPtr       Pointer to the common message header of the FollowUp message
 *  \param[out]    BufferPtr       Pointer to the Tx buffer where the Tlv should be appended
 *  \param[in,out] OffsetPtr       In:  Offset in the given buffer where to store the Tlv
 *                                 Out: New offset which has to be used for the next Tlv
 *  \context      ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpTimeTlv(
                    EthTSyn_PortIdxType        PortIdx,
                    uint8                      DataId,
    ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr,
      ETHTSYN_P2VAR(uint8)                     BufferPtr,
      ETHTSYN_P2VAR(uint16)                    OffsetPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpStatusTlv
 *********************************************************************************************************************/
/*! \brief         Append FollowUp AUTOSAR Status TLV
 *  \param[in]     PortIdx         Index of the port the FollowUp is transmitted on
 *  \param[in]     DataId          DataId used for Crc calculation
 *  \param[out]    BufferPtr       Pointer to the Tx buffer where the Tlv should be appended
 *  \param[in,out] OffsetPtr       In:  Offset in the given buffer where to store the Tlv
 *                                 Out: New offset which has to be used for the next Tlv
 *  \context      ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpStatusTlv(
                  EthTSyn_PortIdxType  PortIdx,
                  uint8                DataId,
    ETHTSYN_P2VAR(uint8)               BufferPtr,
    ETHTSYN_P2VAR(uint16)              OffsetPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpUserDataTlv
 *********************************************************************************************************************/
/*! \brief         Append FollowUp AUTOSAR UserData TLV
 *  \param[in]     PortIdx         Index of the port the FollowUp is transmitted on
 *  \param[in]     DataId          DataId used for Crc calculation
 *  \param[out]    BufferPtr       Pointer to the Tx buffer where the Tlv should be appended
 *  \param[in,out]  OffsetPtr       In:  Offset in the given buffer where to store the Tlv
 *                                 Out: New offset which has to be used for the next Tlv
 *  \context       ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpUserDataTlv(
                  EthTSyn_PortIdxType  PortIdx,
                  uint8                DataId,
    ETHTSYN_P2VAR(uint8)               BufferPtr,
    ETHTSYN_P2VAR(uint16)              OffsetPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpOfsTlvs
 *********************************************************************************************************************/
/*! \brief         Append FollowUp AUTOSAR Ofs TLVs
 *  \param[in]     PortIdx         Index of the port the FollowUp is transmitted on
 *  \param[in]     DataId          DataId used for Crc calculation
 *  \param[out]    BufferPtr       Pointer to the Tx buffer where the Tlv should be appended
 *  \param[in,out]  OffsetPtr       In:  Offset in the given buffer where to store the Tlv
 *                                 Out: New offset which has to be used for the next Tlv
 *  \context      ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpOfsTlvs(
                  EthTSyn_PortIdxType  PortIdx,
                  uint8                DataId,
    ETHTSYN_P2VAR(uint8)               BufferPtr,
    ETHTSYN_P2VAR(uint16)              OffsetPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_Master_HandleCyclicMasterPortTasks
 *********************************************************************************************************************/
/*! \brief      Handles the cyclic tasks for each of the given Master-Ports
 *  \param[in]  MasterPortIdxPtr    Pointer to the master port index list
 *  \param[in]  MasterPortCnt       Amount of master ports
 *  \param[in]  ImmediateTimeSync   Indicating if immediate time sync should be triggered
 *  \context    EthTSyn_MainFunction().
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Master_HandleCyclicMasterPortTasks(
  ETHTSYN_P2CONST(EthTSyn_PortIdxType)  MasterPortIdxPtr,
                  uint8                 MasterPortCnt,
                  boolean               ImmediateTimeSync);

/**********************************************************************************************************************
 *  EthTSyn_Master_SyncSendSm_SendSync
 *********************************************************************************************************************/
/*! \brief      Handles the Sync message transmission of the sync send state machine (triggered by
 *              EthTSyn_Master_ProcessSmSyncSend() )
 *  \param[in]  MasterPortIdx    Index of the master port, the sync message should be transmitted on
 *  \param[in]  IsImmediateSync  Indicating if the sync is an immediate sync
 *  \context    EthTSyn_Master_ProcessSmSyncSend().
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Master_SyncSendSm_SendSync(
  EthTSyn_PortIdxType  MasterPortIdx,
  boolean              IsImmediateSync);

/**********************************************************************************************************************
 *  EthTSyn_Master_SyncSendSm_SendFup
 *********************************************************************************************************************/
/*! \brief      Handles the FollowUp message transmission of the sync send state machine (triggered by
 *              EthTSyn_Master_ProcessSmSyncSend() )
 *  \param[in]  MasterPortIdx    Index of the master port, the follow up message should be transmitted on
 *  \context    EthTSyn_Master_ProcessSmSyncSend().
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Master_SyncSendSm_SendFup(
  EthTSyn_PortIdxType MasterPortIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Master_IsImmediateTimeSyncRequired
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Master_IsImmediateTimeSyncRequired(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean triggerImmediateTimeSync = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if immediate time sync is activated for the time domain. */
  if(EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForTimeDomain(TimeDomainIdx) == TRUE)
  {
    /* #20 Check global time base status bit. */
    if(EthTSyn_Master_IsGlobalTimeBaseStatusBit(TimeDomainIdx) == TRUE)
    {
      /* #30 Check if time base update counter has changed. */
      triggerImmediateTimeSync = EthTSyn_Master_HasTimeBaseUpdateCounterChanged(TimeDomainIdx);
    }
  }

  return triggerImmediateTimeSync;
} /* EthTSyn_Master_IsImmediateTimeSyncRequired() */

/**********************************************************************************************************************
 *  EthTSyn_Master_IsGlobalTimeBaseStatusBit
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Master_IsGlobalTimeBaseStatusBit(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isGlobalTimeBase = FALSE;
  StbM_TimeStampType timestamp;
  StbM_UserDataType userData;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the time base status from Stbm. */
  if(StbM_GetCurrentTime(ETHTSYN_CFG_GET_SYNCHRONIZED_TIME_BASE_ID(TimeDomainIdx), &timestamp, &userData) == E_OK)
  {
    /* #100 Check if the Global Time Base bit is set. */
    if((timestamp.timeBaseStatus & ETHTSYN_TIME_BASE_STATUS_GLOBAL_TIME_BASE_MASK) != 0)
    {
      isGlobalTimeBase = TRUE;
    }
  }

  return isGlobalTimeBase;
} /* EthTSyn_Master_IsGlobalTimeBaseStatusBit() */

/**********************************************************************************************************************
 *  EthTSyn_Master_HasTimeBaseUpdateCounterChanged
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Master_HasTimeBaseUpdateCounterChanged(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean updateCounterHasChanged = FALSE;
  uint8 updateCounter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get current time base update counter from StbM. */
  updateCounter = StbM_GetTimeBaseUpdateCounter(ETHTSYN_CFG_GET_SYNCHRONIZED_TIME_BASE_ID(TimeDomainIdx));

  /* #20 Check if time base update counter has been read before. */
  if(EthTSyn_Master_TimeBaseUpdateCounterHandler[TimeDomainIdx].IsValueValid == TRUE)
  {
    /* #200 Check if value has changed. */
    if(updateCounter != EthTSyn_Master_TimeBaseUpdateCounterHandler[TimeDomainIdx].Value)
    {
      EthTSyn_Master_TimeBaseUpdateCounterHandler[TimeDomainIdx].Value = updateCounter;
      updateCounterHasChanged = TRUE;
    }
  }
  else
  {
    /* #201 Store initial value. */
    EthTSyn_Master_TimeBaseUpdateCounterHandler[TimeDomainIdx].Value = updateCounter;
    EthTSyn_Master_TimeBaseUpdateCounterHandler[TimeDomainIdx].IsValueValid = TRUE;
  }

  return updateCounterHasChanged;
}

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_TxSyncMsg
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_TxSyncMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(uint8) txBufU8Ptr;
  ETHTSYN_P2VAR(EthTSyn_SyncMsgType) syncPtr;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;
  uint8 bufIdx;
  Std_ReturnType retVal = E_NOT_OK;
  uint16 txBufferSize;
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthSwt_MgmtInfoType mgmtInfo;
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) mgmtInfoPtr = NULL_PTR;
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Try to get Ethernet TxBuffer for the sync message. */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(EthTSyn_MasterTx_GetTxBufferForSync(PortIdx, &txBufU8Ptr, &bufIdx, &txBufferSize) == E_OK)
  {
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Init syncSendSmPtr */
    syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* Reset Timestamp States */
    syncSendSmPtr->FollowUpPreciseOriginTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
#if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    syncSendSmPtr->SyncIngressTimestampStateHostPort = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
#endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

    /* Set message pointers */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    comMsgPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))txBufU8Ptr;
    syncPtr = (ETHTSYN_P2VAR(EthTSyn_SyncMsgType))&txBufU8Ptr[ETHTSYN_HDR_COM_LENGTH];

    /* #20 Set common header message attributes. */
    EthTSyn_UtilTx_PrepareCommonMsgHdr(comMsgPtr, ETHTSYN_MSG_TYPE_SYNC, PortIdx);

    /* #30 Set message specific attributes. */
    IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))syncPtr->Reserved, 0u, sizeof(syncPtr->Reserved));

    syncSendSmPtr->SyncTxBufIdx = bufIdx;

    /* store buffer pointer for call-back in TxConfirmation */
    syncSendSmPtr->SyncTxBufPtr = txBufU8Ptr;

    /* #40 Get switch management information and frame id if applicable. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    if(EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId(PortIdx, txBufferSize, txBufU8Ptr, &mgmtInfo) == E_OK)
    {
      mgmtInfoPtr = &mgmtInfo;
# if ( (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) )
      syncSendSmPtr->FrameId = mgmtInfo.FrameId;
# endif /* ( (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) ) */
    }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_SEND_FOLLOW_UP;
    syncSendSmPtr->FollowUpPreciseOriginTimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #50 Enable egress timestamping. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    retVal = EthTSyn_MasterTx_EnableEgressTimestampForSync(PortIdx, bufIdx, mgmtInfoPtr);
#else
    retVal = EthTSyn_MasterTx_EnableEgressTimestampForSync(PortIdx, bufIdx);
#endif /*  */

    if(retVal == E_OK)
    {
      txBufferSize = ETHTSYN_MSG_SYNC_LENGTH;
    }
    else
    {
      txBufferSize = 0;
    }

    /* #60 Trigger transmission. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, txBufferSize, TRUE, mgmtInfoPtr);
#else
    retVal = EthTSyn_Transmit(PortIdx, bufIdx, txBufferSize, TRUE);
#endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

    if(txBufferSize == 0u)
    {
      retVal = E_NOT_OK;
    }

  }

  return retVal;
} /* EthTSyn_MasterTx_TxSyncMsg() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_GetTxBufferForSync
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_GetTxBufferForSync(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(ETHTSYN_P2VAR(uint8)) TxBufPtrPtr, ETHTSYN_P2VAR(uint8) BufIdxPtr,
  ETHTSYN_P2VAR(uint16) LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  *LenBytePtr = ETHTSYN_MSG_SYNC_LENGTH;
  /* #10 Check if port is ready for transmission. */
  if(EthTSyn_IsTxReady(PortIdx) == TRUE)
  {
    /* #20 Get Ethernet TxBuffer. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
        ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), BufIdxPtr, (ETHTSYN_P2VAR(Eth_DataType*))TxBufPtrPtr,
        LenBytePtr) == BUFREQ_OK)
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_MasterTx_GetTxBufferForSync() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_EnableEgressTimestampForSync
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_EnableEgressTimestampForSync(
  EthTSyn_PortIdxType PortIdx, uint8 BufIdx, ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr)
# else
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_EnableEgressTimestampForSync(
  EthTSyn_PortIdxType PortIdx, uint8 BufIdx)
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);
#endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
#if ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) )
  ETHTSYN_DUMMY_STATEMENT(MgmtInfoPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) ) */

  /* ----- Implementation ----------------------------------------------- */
# if ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) )
  retVal = E_OK;
  ETHTSYN_DUMMY_STATEMENT(BufIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) ) */

#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  retVal = EthIf_EnableEgressTimestamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx);
  if(retVal == E_OK)
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
  {
#if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    if(MgmtInfoPtr != NULL_PTR)
    {
      syncSendSmPtr->SyncEgressTimestampStateSwitchPort =
          ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
      syncSendSmPtr->SyncIngressTimestampStateHostPort =
          ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      retVal = EthIf_SwitchEnableEgressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx, MgmtInfoPtr);
    }
#else
    /* Nothing to do here. */
#endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
  }

  return retVal;
} /* EthTSyn_MasterTx_EnableEgressTimestampForSync() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_TxFollowUpMsg
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
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_MasterTx_TxFollowUpMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);
  ETHTSYN_P2VAR(uint8) txBufU8Ptr;
  uint8 bufIdx;
  Std_ReturnType retVal = E_NOT_OK;
  uint16 txBufferSize = ETHTSYN_CFG_GET_FOLLOW_UP_MSG_LENGTH_TX(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthSwt_MgmtInfoType mgmtInfo;
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) mgmtInfoPtr = NULL_PTR;
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if FollowUp is ready for transmission on the given Port. */
  if(EthTSyn_MasterTx_IsFollowUpTxReady(PortIdx) == TRUE)
  {
    /* #20 Get Ethernet transmission buffer. */ /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
      ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), &bufIdx, (ETHTSYN_P2VAR(Eth_DataType*)) &txBufU8Ptr,
      &txBufferSize) == BUFREQ_OK)
    {
      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #30 Copy follow up message to Ethernet TxBuffer. */
      EthTSyn_MasterTx_CopyFollowUpToTxBuffer(PortIdx, txBufU8Ptr);

      /* Store bufIdx and bufferPtr for call-back in tx-confirmation */
      syncSendSmPtr->FollowUpTxBufIdx = bufIdx;
      syncSendSmPtr->FollowUpTxBufPtr = txBufU8Ptr;

      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 Append FollowUp Ar TLVs if applicable. */
      EthTSyn_MasterTx_AppendArFollowUpTlvs(PortIdx, txBufU8Ptr);

      /* #50 Get switch management information and frame id if applicable. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      if(EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId(PortIdx, txBufferSize, txBufU8Ptr, &mgmtInfo) == E_OK)
      {
        mgmtInfoPtr = &mgmtInfo;
      }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

      /* #60 Trigger transmission. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      retVal = EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_CFG_GET_FOLLOW_UP_MSG_LENGTH_TX(PortIdx), TRUE, mgmtInfoPtr);
#else
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      retVal = EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_CFG_GET_FOLLOW_UP_MSG_LENGTH_TX(PortIdx), TRUE);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    }
  }

  return retVal;
} /* EthTSyn_MasterTx_TxFollowUpMsg() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_IsFollowUpTxReady
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_MasterTx_IsFollowUpTxReady(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTxReady = FALSE;
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is ready for transmission */
  if(EthTSyn_IsTxReady(PortIdx) == TRUE)
  {
    /* #20 Check if Precise Origin Timestamp is ready for processing */
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if(EthTSyn_IsTimestampReadyToProcess(syncSendSmPtr->FollowUpPreciseOriginTimestampState) == TRUE)
    {
#if ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) )
      /* #30 Check if Switch Timestamps are available already */ /* PRQA S 3415 2 */ /* MD_EthTSyn_12.4 */
      if((EthTSyn_IsTimestampReadyToProcess(syncSendSmPtr->SyncEgressTimestampStateSwitchPort) == TRUE) &&
        (EthTSyn_IsTimestampReadyToProcess(syncSendSmPtr->SyncIngressTimestampStateHostPort) == TRUE))
      {
        isTxReady = TRUE;
      }
#else
      isTxReady = TRUE;
#endif /* ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) ) */
    }
    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  return isTxReady;
} /* EthTSyn_MasterTx_IsFollowUpTxReady() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_CopyFollowUpToTxBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_CopyFollowUpToTxBuffer(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2VAR(uint8) TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comMsgPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))TxBufPtr;
  ETHTSYN_P2VAR(EthTSyn_FollowUpMsgType) followUpPtr =
      (ETHTSYN_P2VAR(EthTSyn_FollowUpMsgType))&TxBufPtr[ETHTSYN_HDR_COM_LENGTH];
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);
  uint64 unsignedFollowUpCorrectionValueNs;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the common message header. */
  EthTSyn_UtilTx_PrepareCommonMsgHdr(comMsgPtr, ETHTSYN_MSG_TYPE_FOLLOW_UP, PortIdx);

  /* #20 Set FollowUp message specific values. */
  /* Set specific message type attributes to zero */
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))followUpPtr, 0, sizeof(EthTSyn_FollowUpMsgType));

  /* Set IEEE Tlv values. */ /*lint -e572 */
  followUpPtr->TlvType = EthTSyn_Htons(ETHTSYN_FOLLOW_UP_TLV_TYPE);
  followUpPtr->LengthField = EthTSyn_Htons(ETHTSYN_FOLLOW_UP_TLV_LENGTH_FIELD);
  followUpPtr->OrganisationId[0] = (uint8)(ETHTSYN_FOLLOW_UP_ORGANISATION_ID >> 16U);
  followUpPtr->OrganisationId[1] = (uint8)(ETHTSYN_FOLLOW_UP_ORGANISATION_ID >> 8U);
  followUpPtr->OrganisationId[2] = (uint8)((ETHTSYN_FOLLOW_UP_ORGANISATION_ID >> 0U) & 0xFF);
  followUpPtr->OrganisationSubType[0] = (uint8)(ETHTSYN_FOLLOW_UP_ORGANISATION_SUBTYPE >> 16U);
  followUpPtr->OrganisationSubType[1] = (uint8)(ETHTSYN_FOLLOW_UP_ORGANISATION_SUBTYPE >> 8U);
  followUpPtr->OrganisationSubType[2] = (uint8)(ETHTSYN_FOLLOW_UP_ORGANISATION_SUBTYPE >> 0U); /*lint +e572 */

  /* Set Precise origin timestamp. */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  EthTSyn_UtilTx_HtonTimestamp((ETHTSYN_P2VAR(EthTSyn_TimestampType))followUpPtr->PreciseOriginTimestampSeconds,
      &syncSendSmPtr->FollowUpPreciseOriginTimestamp);

  /* #30 Compute follow up correction field. */
#if ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) )
  {
    EthTSyn_TimediffType switchResidenceTime;

    unsignedFollowUpCorrectionValueNs = ETHTSYN_SWT_UPLINK_TO_TX_RESIDENCE_TIME_NS;
    /* calculate FollowUp correction value. */
    if(EthTSyn_Util_TimestampMinusTimestamp(&syncSendSmPtr->SyncEgressTimestampSwitchPort,
        &syncSendSmPtr->SyncIngressTimestampHostPort, &switchResidenceTime) == E_OK)
    {
      if(switchResidenceTime > 0)
      {
        unsignedFollowUpCorrectionValueNs = (uint64)switchResidenceTime;
      }
    }
  }
#else
  unsignedFollowUpCorrectionValueNs = 0;
#endif /* ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) ) */

  /* #40 Set follow up correction field. */ /* PRQA S 3109 1 */ /* MD_MSR_14_3 */
  EthTSyn_CopyFollowUpCorrectionField(unsignedFollowUpCorrectionValueNs, comMsgPtr->CorrectionField);

} /* EthTSyn_MasterTx_CopyFollowUpToTxBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_Master_TxAnnounceMsg
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
ETHTSYN_LOCAL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Master_TxAnnounceMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(uint8) txBufU8Ptr;
  ETHTSYN_P2VAR(EthTSyn_AnnounceMsgType) announcePtr;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comMsgPtr;
  uint8 bufIdx;
  Std_ReturnType retVal = E_NOT_OK;
  uint16 txBufferSize = ETHTSYN_MSG_ANNOUNCE_LENGTH;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is ready for transmission. */
  if(EthTSyn_IsTxReady(PortIdx) == TRUE)
  {
    /* #20 Try to get an transmission buffer. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
      ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), &bufIdx, (ETHTSYN_P2VAR(Eth_DataType*)) &txBufU8Ptr,
      &txBufferSize) == BUFREQ_OK)
    {
      /* #30 Copy announce message to the transmission buffer */
      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      /* set message pointers */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      comMsgPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))txBufU8Ptr;
      announcePtr = (ETHTSYN_P2VAR(EthTSyn_AnnounceMsgType))&txBufU8Ptr[ETHTSYN_HDR_COM_LENGTH];
    
      /* set common header message attributes */
      comMsgPtr->TransSpecific_MessageType = ETHTSYN_HDR_COM_TRANSPORT_SPECIFIC | ETHTSYN_MSG_TYPE_ANNOUNCE;
      comMsgPtr->Reserved_VersionGPtp = ETHTSYN_PROTOCOL_VERSION;

      comMsgPtr->MessageLength = EthTSyn_Htons((uint16)ETHTSYN_MSG_ANNOUNCE_LENGTH); /*lint !e572 */
      comMsgPtr->DomainNumber = EthTSyn_PortConfig[PortIdx].TimeDomainId;
    
      comMsgPtr->Reserved_0 = 0;
      comMsgPtr->Flags = 0;
    
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))comMsgPtr->CorrectionField, 0,
        sizeof(EthTSyn_CorrectionFieldType));
    
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))comMsgPtr->Reserved_1, 0, sizeof(comMsgPtr->Reserved_1));
    
      EthTSyn_SetClockIdentity(comMsgPtr->SourceClockIdentity, ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx));
      comMsgPtr->SourcePortNumber = (uint16)EthTSyn_Htons(ETHTSYN_CFG_GET_PORT_NUMBER(PortIdx));
    
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      comMsgPtr->SequenceId = (uint16)EthTSyn_Htons(ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SequenceId);
      comMsgPtr->Control = ETHTSYN_HDR_COM_CONTROL_ANNOUNCE;
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      comMsgPtr->LogMessageInterval = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->LogMessageInterval;
    
      /* set specific message type attributes */
    
      /* set first reserved field to zero */
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))announcePtr->Reserved_0, 0, sizeof(announcePtr->Reserved_0));
      
      announcePtr->CurrentUtcOffset = 0;
    
      /* set second reserved field to zero */
      announcePtr->Reserved_1 = 0;
    
      announcePtr->GrandmasterPriority_1 = ETHTSYN_ANNOUNCE_PRIORITY_1_NETWORK_INFRASTRUCTURE;
      announcePtr->GrandmasterClockQuality.ClockAccuracy = ETHTSYN_ANNOUNCE_CLOCK_ACCURACY_UNKNOWN;
      announcePtr->GrandmasterClockQuality.ClockClass = ETHTSYN_ANNOUNCE_CLOCK_CLASS_UNKNOWN;
      announcePtr->GrandmasterClockQuality.OffsetScaledLogVariance = 
        (uint16)EthTSyn_Htons(ETHTSYN_ANNOUNCE_OFFSET_SCALED_LOG_UNKNOWN);
      announcePtr->GrandmasterPriority_2 = ETHTSYN_ANNOUNCE_PRIORITY_2_DEFAULT;
    
      EthTSyn_SetClockIdentity(announcePtr->GrandmasterIdentity, ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx));
      
      announcePtr->StepsRemoved[0] = 0;
      announcePtr->StepsRemoved[1] = 0;
    
      announcePtr->TimeSource = ETHTSYN_ANNOUNCE_TIME_SOURCE_INTERNAL_OSCILLATOR;
      /*lint -e572 */
      announcePtr->TlvType = EthTSyn_Ntohs((uint16)ETHTSYN_ANNOUNCE_TLV_TYPE);
      announcePtr->LengthField = EthTSyn_Ntohs((uint16)ETHTSYN_ANNOUNCE_TLV_LENGTH_FIELD); /*lint +e572 */
    
      /* set Tlv Clock identity */
      EthTSyn_SetClockIdentity(announcePtr->ClockIdentity, ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx));
      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 Trigger transmission. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      retVal = EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_MSG_ANNOUNCE_LENGTH, FALSE, NULL_PTR);
#else
      retVal = EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_MSG_ANNOUNCE_LENGTH, FALSE);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    }
  } /* END: Check if Port is enabled */
  return retVal;
} /* EthTSyn_Master_TxAnnounceMsg() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpTlvs
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
/* AR4.3 message format */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpTlvs(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2VAR(uint8) TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 arTlvHdrLengthField = ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE; /* Add length sum of Sub-Tlvs later */
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgPtr = (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))TxBufPtr;
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_ArTlvHeaderType) arTlvHdrPtr =
    (ETHTSYN_P2VAR(EthTSyn_ArTlvHeaderType))&TxBufPtr[ETHTSYN_HDR_COM_LENGTH + ETHTSYN_FOLLOW_UP_AR_TLV_OFFSET];
  /* Set bufferPtr to OrganisationId because the Length starts from this field */
  ETHTSYN_P2VAR(uint8) bufferPtr = ((ETHTSYN_P2VAR(uint8))arTlvHdrPtr->OrganisationId);
  uint8 dataId = EthTSyn_CfgAccess_GetFollowUpDataIdListEntry(ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx),
    (uint16)EthTSyn_Ntohs(comMsgPtr->SequenceId));

  /* ----- Implementation ----------------------------------------------- */
  if(EthTSyn_CfgAccess_IsArTlvTxRequired(PortIdx) == TRUE)
  {
    /* #10 Append the Time Secured Sub-Tlv if applicable */
    EthTSyn_MasterTx_AppendArFollowUpTimeTlv(PortIdx, dataId, comMsgPtr, bufferPtr, &arTlvHdrLengthField);

    /* #20 Append the Status (Not) Secured Sub-Tlv if applicable */
    EthTSyn_MasterTx_AppendArFollowUpStatusTlv(PortIdx, dataId, bufferPtr, &arTlvHdrLengthField);

    /* #30 Append the User Data (Not) Secured Sub-Tlv if applicable */
    EthTSyn_MasterTx_AppendArFollowUpUserDataTlv(PortIdx, dataId, bufferPtr, &arTlvHdrLengthField);

    /* #40 Append the Ofs (Not) Secured Sub-Tlv if applicable */
    EthTSyn_MasterTx_AppendArFollowUpOfsTlvs(PortIdx, dataId, bufferPtr, &arTlvHdrLengthField);

    /* #50 Init ArTlvHdr */
    /* Set Tlv Type */
    ETHTSYN_PUT_UINT16(arTlvHdrPtr->TlvType, 0, ETHTSYN_AR_TLV_TYPE); /*lint !e572 */

    /* Set length field */
    ETHTSYN_PUT_UINT16(arTlvHdrPtr->LengthField, 0u, arTlvHdrLengthField);

    /* Set Organization Id and SubType */
    ETHTSYN_PUT_3_BYTE(arTlvHdrPtr->OrganisationId, 0, ETHTSYN_AR_TLV_ORGANIZATION_ID);
    ETHTSYN_PUT_3_BYTE(arTlvHdrPtr->OrganisationSubType, 0, ETHTSYN_AR_TLV_ORGANIZATION_SUB_TYPE);
  }

} /* EthTSyn_MasterTx_AppendArFollowUpTlvs() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpTimeTlv
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpTimeTlv(EthTSyn_PortIdxType PortIdx,
  uint8 DataId, ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, ETHTSYN_P2VAR(uint8) BufferPtr,
  ETHTSYN_P2VAR(uint16) OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_ArSubTlvTimeSecuredType) timeSecuredSubTlvPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Time Tlv is required. */
  if(ETHTSYN_CFG_GET_AR_SUB_TLV_TIME_USED(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    /* Get the Time Secured Sub-Tlv pointer */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    timeSecuredSubTlvPtr = (ETHTSYN_P2VAR(EthTSyn_ArSubTlvTimeSecuredType))&BufferPtr[*OffsetPtr];

    /* #20 Increase offset for further Sub-Tlv. */
    (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_TIME_LENGTH;

    /* #30 Append Time Secured Tlv. */
    /* Set Type and length */
    timeSecuredSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED;
    timeSecuredSubTlvPtr->Length = ETHTSYN_AR_SUB_TLV_TIME_LENGTH_FIELD_VALUE;

    /* Set CRC_Time_Flags */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    timeSecuredSubTlvPtr->CrcTimeFlags = ETHTSYN_CFG_GET_TX_CRC_SECURED_FLAGS(PortIdx);

    /* Calculate and set CRC_Time_0/1 */
    timeSecuredSubTlvPtr->CrcTime0 = EthTSyn_Crc_ComputeTime0Crc(DataId, timeSecuredSubTlvPtr->CrcTimeFlags,
      ComMsgPtr);
    timeSecuredSubTlvPtr->CrcTime1 = EthTSyn_Crc_ComputeTime1Crc(DataId, timeSecuredSubTlvPtr->CrcTimeFlags,
      ComMsgPtr);
  }
} /* EthTSyn_MasterTx_AppendArFollowUpTimeTlv() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpStatusTlv
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpStatusTlv(EthTSyn_PortIdxType PortIdx,
  uint8 DataId, ETHTSYN_P2VAR(uint8) BufferPtr, ETHTSYN_P2VAR(uint16) OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_ArSubTlvStatusType) statusSubTlvPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Status Tlv is required. */
  if(ETHTSYN_CFG_GET_AR_SUB_TLV_STATUS_USED(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    StbM_TimeStampType tempStbmTimeStamp;
    StbM_UserDataType tempUserData;

    /* Get the Status (Not) Secured Sub-Tlv pointer */ /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    statusSubTlvPtr = (ETHTSYN_P2VAR(EthTSyn_ArSubTlvStatusType))&BufferPtr[*OffsetPtr];

    /* #20 Increase offset for further Sub-Tlv. */
    (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_STATUS_LENGTH;

    /* Set length */
    statusSubTlvPtr->Length = ETHTSYN_AR_SUB_TLV_STATUS_LENGTH_FIELD_VALUE;

    /* #30 Get the synchronized time base status. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    if(StbM_GetCurrentTime(ETHTSYN_CFG_GET_SYNCHRONIZTED_TIME_BASE_ID_OF_PORT(PortIdx), &tempStbmTimeStamp,
      &tempUserData) == E_OK)
    {
      ETHTSYN_TIME_BASE_STATUS_TO_AR_SUB_TLV_STATUS(tempStbmTimeStamp.timeBaseStatus, statusSubTlvPtr->Status);
    }
    else
    {
      statusSubTlvPtr->Status = 0u;
    }

    /* #40 Append Status Tlv. */
    /* Write Type Secured/NotSecured and Crc (if applicable) */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    if(ETHTSYN_CFG_GET_TX_CRC_SECURED(PortIdx) == ETHTSYN_CRC_TX_SECURED_SUPPORTED)
    {
      /* Secured */
      statusSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED;
      statusSubTlvPtr->CrcStatus = EthTSyn_Crc_ComputeStatusCrc(DataId, statusSubTlvPtr);
    }
    else
    {
      /* Not Secured */
      statusSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED;
      statusSubTlvPtr->CrcStatus = 0u;
    }
  }
} /* EthTSyn_MasterTx_AppendArFollowUpStatusTlv() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpUserDataTlv
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpUserDataTlv(EthTSyn_PortIdxType PortIdx,
  uint8 DataId, ETHTSYN_P2VAR(uint8) BufferPtr, ETHTSYN_P2VAR(uint16) OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_ArSubTlvUserDataType) userDataSubTlvPtr;
  StbM_UserDataType userData;
  StbM_TimeStampType timeStamp;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize userData */
  userData.userDataLength = 0;
  userData.userByte0 = 0;
  userData.userByte1 = 0;
  userData.userByte2 = 0;
  
  /* #10 Check if UserData Tlv is required. */
  if(ETHTSYN_CFG_GET_AR_SUB_TLV_USER_DATA_USED(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    /* Get the UserData (Not) Secured Sub-Tlv pointer */ /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    userDataSubTlvPtr = (ETHTSYN_P2VAR(EthTSyn_ArSubTlvUserDataType))&BufferPtr[*OffsetPtr];

    /* #20 Increase offset for further Sub-Tlv. */
    (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH;

    /* Set length field */
    userDataSubTlvPtr->Length = ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH_FIELD_VALUE;

    /* #30 Get the synchronized time base UserData. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    if(StbM_GetCurrentTime(ETHTSYN_CFG_GET_SYNCHRONIZTED_TIME_BASE_ID_OF_PORT(PortIdx), &timeStamp, &userData) != E_OK)
    {
      /* #300 Failed to retrieve user data -> set to '0'. */
      userData.userDataLength = 0;
      userData.userByte0 = 0;
      userData.userByte1 = 0;
      userData.userByte2 = 0;
    }

    /* #40 Append UserData Tlv. */
    /* Write UserData */
    userDataSubTlvPtr->UserDataLength = userData.userDataLength;
    userDataSubTlvPtr->UserByte0 = userData.userByte0;
    userDataSubTlvPtr->UserByte1 = userData.userByte1;
    userDataSubTlvPtr->UserByte2 = userData.userByte2;

    /* Write Type Secured/NotSecured and Crc (if applicable) */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    if(ETHTSYN_CFG_GET_TX_CRC_SECURED(PortIdx) == ETHTSYN_CRC_TX_SECURED_SUPPORTED)
    {
      /* Secured */
      userDataSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED;
      userDataSubTlvPtr->CrcUserData = EthTSyn_Crc_ComputeUserDataCrc(DataId, userDataSubTlvPtr);
    }
    else
    {
      /* Not Secured */
      userDataSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED;
      userDataSubTlvPtr->CrcUserData = 0u;
    }
  }
} /* EthTSyn_MasterTx_AppendArFollowUpUserDataTlv() */

/**********************************************************************************************************************
 *  EthTSyn_MasterTx_AppendArFollowUpOfsTlvs
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_MasterTx_AppendArFollowUpOfsTlvs(EthTSyn_PortIdxType PortIdx,
  uint8 DataId, ETHTSYN_P2VAR(uint8) BufferPtr, ETHTSYN_P2VAR(uint16) OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_ArSubTlvOfsType) ofsSubTlvPtr;
  uint8_least offsetTimeDomainIter;
  StbM_UserDataType userData;
  StbM_TimeStampType timeStamp;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize UserData and Timestamp */
  userData.userDataLength = 0;
  userData.userByte0 = 0;
  userData.userByte1 = 0;
  userData.userByte2 = 0;

  timeStamp.timeBaseStatus = 0;
  timeStamp.secondsHi = 0;
  timeStamp.seconds = 0;
  timeStamp.nanoseconds = 0;

  /* #10 Check if Ofs Tlv is required */
  if(ETHTSYN_CFG_GET_AR_SUB_TLV_OFS_USED(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    /* Iterate the offset TimeDomains for the given Port and append an Ofs Sub-Tlv for each of them */
    for(offsetTimeDomainIter = 0u; offsetTimeDomainIter < ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_COUNT_OF_PORT(PortIdx);
      offsetTimeDomainIter++)
    {
      uint8 ofsTimeDomainId = ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_ID_OF_PORT(PortIdx, offsetTimeDomainIter);
      StbM_SynchronizedTimeBaseType syncTimeBaseIdOfs =
        ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_SYNCHRONIZED_TIME_BASE_ID_OF_PORT(PortIdx, offsetTimeDomainIter);

      /* Get Ofs (Not) Secured Sub-Tlv Pointer */ /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      ofsSubTlvPtr = (ETHTSYN_P2VAR(EthTSyn_ArSubTlvOfsType))&BufferPtr[*OffsetPtr];

      /* #20 Increase offset for further Sub-Tlv */
      (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_OFS_LENGTH;

      /* Set length field */
      ofsSubTlvPtr->Length = ETHTSYN_AR_SUB_TLV_OFS_LENGTH_FIELD_VALUE;

      /* Set offset time domain */
      ofsSubTlvPtr->OfsTimeDomain = ofsTimeDomainId;

      /* #30 Get Offset and user data */
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      if(StbM_GetOffset(syncTimeBaseIdOfs, &timeStamp, &userData) != E_OK)
      {
        /* #300 Failed to retrieve the Offset and user data -> set to '0' */
        userData.userDataLength = 0;
        userData.userByte0 = 0;
        userData.userByte1 = 0;
        userData.userByte2 = 0;

        timeStamp.timeBaseStatus = 0;
        timeStamp.secondsHi = 0;
        timeStamp.seconds = 0;
        timeStamp.nanoseconds = 0;
      }

      /* #40 Append Ofs Tlv */
      /* Write Offset */
      ETHTSYN_PUT_UINT16(ofsSubTlvPtr->OfsTimeSec, 0u, timeStamp.secondsHi);
      ETHTSYN_PUT_UINT32(ofsSubTlvPtr->OfsTimeSec, ETHTSYN_AR_SUB_TLV_OFS_TIME_SEC_SECONDS_OFFSET, timeStamp.seconds);
      ETHTSYN_PUT_UINT32(ofsSubTlvPtr->OfsTimeNSec, 0u, timeStamp.nanoseconds);

      /* Write Status */
      ETHTSYN_TIME_BASE_STATUS_TO_AR_SUB_TLV_STATUS(timeStamp.timeBaseStatus, ofsSubTlvPtr->Status);

      /* Write UserData */
      ofsSubTlvPtr->UserDataLength = userData.userDataLength;
      ofsSubTlvPtr->UserByte0 = userData.userByte0;
      ofsSubTlvPtr->UserByte1 = userData.userByte1;
      ofsSubTlvPtr->UserByte2 = userData.userByte2;

      /* Write Type Secured/NotSecured and Crc (if applicable) */
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      if(ETHTSYN_CFG_GET_TX_CRC_SECURED(PortIdx) == ETHTSYN_CRC_TX_SECURED_SUPPORTED)
      {
        /* Secured */
        ofsSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED;
        ofsSubTlvPtr->CrcOfs = EthTSyn_Crc_ComputeOfsCrc(DataId, ofsSubTlvPtr);
      }
      else
      {
        /* Not Secured */
        ofsSubTlvPtr->Type = ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED;
        ofsSubTlvPtr->CrcOfs = 0u;
      }
    } /* END: Iteration of all Offset TimeDomains for the given Port */
  }
} /* EthTSyn_MasterTx_AppendArFollowUpOfsTlvs() */

/**********************************************************************************************************************
 *  EthTSyn_Master_HandleCyclicMasterPortTasks
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Master_HandleCyclicMasterPortTasks(
  ETHTSYN_P2CONST(EthTSyn_PortIdxType) MasterPortIdxPtr, uint8 MasterPortCnt, boolean ImmediateTimeSync)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least masterPortIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all master ports. */
  for(masterPortIter = 0; masterPortIter < MasterPortCnt; masterPortIter++)
  {
    EthTSyn_PortIdxType masterPortIdx = MasterPortIdxPtr[masterPortIter];
    /* #20 Check if Port is Tx ready */
    if(EthTSyn_IsTxReady(masterPortIdx) == TRUE)
    {
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(masterPortIdx);
      /* #30 Handle timer/counter of the Port */
      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if(syncSendSmPtr->TxIntervalCnt > 0)
      {
        syncSendSmPtr->TxIntervalCnt--;
      }

      /* #40 Handle immediate time sync. */
      /* PRQA S 3415 2 */ /* MD_EthTSyn_12.4 */
      if((ImmediateTimeSync == TRUE) &&
          (EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForMasterPort(masterPortIdx) == TRUE))
      {
        syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_SEND_IMMEDIATE_SYNC;
      }
      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      /* #50 Process SyncSend state-machine */
      EthTSyn_Master_ProcessSmSyncSend(masterPortIdx);
    }
  } /* END: Iteration of MasterPorts */
} /* EthTSyn_Master_HandleCyclicMasterPortTasks() */

/**********************************************************************************************************************
 *  EthTSyn_Master_SyncSendSm_SendSync
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Master_SyncSendSm_SendSync(EthTSyn_PortIdxType MasterPortIdx,
  boolean IsImmediateSync)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;
  EthTSyn_StateMachineStateType syncSendSmStateTxFailed;
  uint32 txIntervalCntTxSuccess;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Sync message is an immediate sync and prepare the required values accordingly. */
  if(IsImmediateSync == TRUE)
  {
    syncSendSmStateTxFailed = ETHTSYN_STATE_SYNC_SEND_SEND_IMMEDIATE_SYNC;
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    txIntervalCntTxSuccess = EthTSyn_MasterPortConfigPtr(MasterPortIdx)->CyclicMsgResumeCounter;
  }
  else
  {
    syncSendSmStateTxFailed = ETHTSYN_STATE_SYNC_SEND_SEND_SYNC;
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    txIntervalCntTxSuccess = EthTSyn_MasterPortConfigPtr(MasterPortIdx)->SyncSendInterval;
  }

  /* #20 Verify that port is AsCapable. */
  if(EthTSyn_Util_GetAsCapable(MasterPortIdx) == TRUE)
  {
    syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(MasterPortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #30 Trigger transmission of Sync message. */
    if(EthTSyn_MasterTx_TxSyncMsg(MasterPortIdx) == E_OK)
    {
      /* #300 Transmission successful: Recharge tx interval counter. */
      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      syncSendSmPtr->TxConfirmationPending++;
      syncSendSmPtr->TxIntervalCnt = txIntervalCntTxSuccess;
      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    else
    {
      /* #301 Transmission failed: Retry transmission later by setting the state of the SyncSendSm. */
      syncSendSmPtr->State = syncSendSmStateTxFailed;
    }
  }
} /* EthTSyn_Master_SyncSendSm_SendSync() */

/**********************************************************************************************************************
 *  EthTSyn_Master_SyncSendSm_SendFup
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Master_SyncSendSm_SendFup(EthTSyn_PortIdxType MasterPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that port is AsCapable. */
  if(EthTSyn_Util_GetAsCapable(MasterPortIdx) == TRUE)
  {
    /* #20 Trigger transmission of follow up message. */
    if(EthTSyn_MasterTx_TxFollowUpMsg(MasterPortIdx) == E_OK)
    {
      /* #30 Transmission successful -> Handle sync send state machine. */
      syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(MasterPortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      syncSendSmPtr->SequenceId++;
      syncSendSmPtr->FollowUpPreciseOriginTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
      syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_WAITING_FOR_SYNC_SEND;

#if ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) )
      syncSendSmPtr->SyncEgressTimestampStateSwitchPort = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
      syncSendSmPtr->SyncIngressTimestampStateHostPort = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
#endif /* ( (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) ) */

      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 Check if announce should be transmitted on the given master port. */
      if(EthTSyn_MasterPortConfigPtr(MasterPortIdx)->TxAnnounceMsg == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        /* #400 Trigger transmission of announce message. */
        if(EthTSyn_Master_TxAnnounceMsg(MasterPortIdx) != E_OK)
        {
        }
      }
    }
  }
} /* EthTSyn_Master_SyncSendSm_SendFup() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Master_ProcessSmSyncSend
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
FUNC(void, ETHTSYN_CODE) EthTSyn_Master_ProcessSmSyncSend(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that port is a master port. */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_MASTER)
  {
    syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    /* #20 Check state of the sync send state machine. */
    switch(syncSendSmPtr->State)
    {
    case ETHTSYN_STATE_SYNC_SEND_INITIALIZING:
      /* #200 State: Initializing -> Init the state machine. */
      syncSendSmPtr->SequenceId = 0;
      syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_WAITING_FOR_SYNC_SEND;
      break;

    case ETHTSYN_STATE_SYNC_SEND_WAITING_FOR_SYNC_SEND:
      /* #201 State: Waiting for Sync send -> Check sync tx interval counter. */
      if(syncSendSmPtr->TxIntervalCnt == 0)
      {
        EthTSyn_Master_SyncSendSm_SendSync(PortIdx, FALSE);
      } /* END: if(EthTSyn_SyncSendSm(PortIdx).TxIntervalCnt == 0) */
      break;

    case ETHTSYN_STATE_SYNC_SEND_SEND_SYNC:
      /* #202 State: Send Sync -> Transmit sync message. */
      EthTSyn_Master_SyncSendSm_SendSync(PortIdx, FALSE);
      break;

    case ETHTSYN_STATE_SYNC_SEND_SEND_IMMEDIATE_SYNC:
      /* #203 State: Send Immediate Sync -> Transmit sync message. */
      EthTSyn_Master_SyncSendSm_SendSync(PortIdx, TRUE);
      break;

    case ETHTSYN_STATE_SYNC_SEND_SEND_FOLLOW_UP:
      /* #204 State: Send FollowUp -> Transmit follow up message. */
      /* Check if TxConfirmation for sent Sync message occurred */
      if(syncSendSmPtr->TxConfirmationPending == 0)
      {
        EthTSyn_Master_SyncSendSm_SendFup(PortIdx);
      }
      break;
    default:
      /* Nothing to do here (MISRA). */;
      break;
    } /* END: Switch(SyncSendSm State) */
  } /* END: If MasterPort */

} /* EthTSyn_Master_ProcessSmSyncSend() */

/**********************************************************************************************************************
 *  EthTSyn_Master_HandleCyclicTasks
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Master_HandleCyclicTasks(EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean triggerImmediateTimeSync;

  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  /* #10 Check that TimeDomain is not a Bridge TimeDomain */
  if(ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) == ETHTSYN_INVALID_PORT_IDX)
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  {
    /* #20 Iterate all MasterPorts of the given TimeDomain */
    if(EthTSyn_CfgGetMasterPortIdxPtr(TimeDomainIdx) != NULL_PTR)
    {
      /* #30 Check if immediate time sync is required. */
      triggerImmediateTimeSync = EthTSyn_Master_IsImmediateTimeSyncRequired(TimeDomainIdx);

      EthTSyn_Master_HandleCyclicMasterPortTasks(EthTSyn_CfgGetMasterPortIdxPtr(TimeDomainIdx),
          EthTSyn_CfgAccess_GetMasterPortCount(TimeDomainIdx), triggerImmediateTimeSync);
    } /* END: If MasterPorts are present */
  }

} /* EthTSyn_Master_HandleCyclicTasks() */

#define ETHTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 * see file EthTSyn.c
*/

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Master_Int.c
 *********************************************************************************************************************/
