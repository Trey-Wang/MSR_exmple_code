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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  EthTSyn.c
 *        \brief  EthTSyn implementation
 *
 *      \details  Contains the main implementation of the Time Sync over Ethernet (EthTSyn) AUTOSAR module
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

#define ETHTSYN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 0857 MACRO_LIMIT */ /* MD_MSR_1.1_857 */
#include "EthTSyn_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
#include "EthTSyn_Master_Int.h"
#include "EthTSyn_Slave_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
#include "EthTSyn_Pdelay_Int.h"
#include "EthTSyn_Util_Int.h"
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
# include "EthTSyn_SwtMgmt_Int.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if (   (ETHTSYN_SW_MAJOR_VERSION != 5u) \
     || (ETHTSYN_SW_MINOR_VERSION != 1u) \
     || (ETHTSYN_SW_PATCH_VERSION != 1u) )
# error "Vendor specific version numbers of EthTSyn.c and EthTSyn.h are inconsistent"
#endif

/* Check configurator version generated to EthTSyn_Cfg.h */
#if (   (ETHTSYN_CFG_MAJOR_VERSION != (0x05)) \
     || (ETHTSYN_CFG_MINOR_VERSION != (0x01)) )
# error "Version numbers of EthTSyn.c and EthTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if !defined (ETHTSYN_LOCAL)
# define ETHTSYN_LOCAL STATIC
#endif

#if !defined (ETHTSYN_LOCAL_INLINE)
# define ETHTSYN_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/* PRQA L: MACRO_LIMIT */
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
# define ETHTSYN_START_SEC_VAR_ZERO_INIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
# include "MemMap.h"

VAR(EthTSyn_StateType, ETHTSYN_VAR_ZERO_INIT) EthTSyn_State = ETHTSYN_STATE_UNINIT;

# define ETHTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
# include "MemMap.h"
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */

#define ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

VAR(EthTSyn_SystemType, ETHTSYN_VAR_NOINIT) EthTSyn_SystemState[ETHTSYN_TIME_DOMAIN_COUNT];

#define ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHTSYN_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  EthTSyn_SetLocalClockIdentity
 *********************************************************************************************************************/
/*! \brief      Sets the local clock identity of the EthTSyn controller.
 *  \param[in]  EthTSynCtrlIdx    Index of the EthTSyn controller the clock identity should be set
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SetLocalClockIdentity(
  EthTSyn_CtrlIdxType EthTSynCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_InitPort
 *********************************************************************************************************************/
/*! \brief      Initialization of EthTSyn Port
 *  \param[in]  PortIdx          Index of the Port that should be initialized
 *  \param[in]  TimeDomainIdx    Index of the corresponding TimeDomain
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitPort(
  EthTSyn_TimeDomainIdxType TimeDomainIdx,
  EthTSyn_PortIdxType       PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_InitTimeDomain
 *********************************************************************************************************************/
/*! \brief      Initialization of the given EthTSyn TimeDomain
 *  \param[in]  TimeDomainIdx    Index of the TimeDomain that should be initialized
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitTimeDomain(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_InitCtrl
 *********************************************************************************************************************/
/*! \brief      Initialization of the given EthTSyn controller
 *  \param[in]  EthTSynCtrlIdx  Index of the EthTSyn controller that should be initialized
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitCtrl(
  EthTSyn_CtrlIdxType EthTSynCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_InitPdelay
 *********************************************************************************************************************/
/*! \brief      Initialization of the given EthTSyn pdelay states
 *  \param[in]  PdelayIdx  Index of the EthTSyn pdelay states that should be initialized
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitPdelay(
  EthTSyn_PdelayIdxType PdelayIdx);

/**********************************************************************************************************************
 *  EthTSyn_ResetPortsAndPdelayOfCtrl
 *********************************************************************************************************************/
/*! \brief      Initialization all Ports of the given EthTSyn controller.
 *  \param[in]  EthTSynCtrlIdx  Index of the EthTSyn controller the Ports should be initialized for.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ResetPortsAndPdelayOfCtrl(
    EthTSyn_CtrlIdxType EthTSynCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_HandleEgressTimestamping
 *********************************************************************************************************************/
/*! \brief      Handles the egress timestamping for the given Port and message type.
 *  \param[in]  PortIdx  Index of the EthTSyn port
 *  \param[in]  MsgType  Type of the message
 *  \param[in]  BufIdx   Index of the Ethernet TxBuffer
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_HandleEgressTimestamping(
  EthTSyn_PortIdxType PortIdx,
  uint8               MsgType,
  uint8               BufIdx);

/**********************************************************************************************************************
 *  EthTSyn_EgressTimestamp_GetSyncRefTime
 *********************************************************************************************************************/
/*! \brief       If the transmitted message was a Sync message, a sync reference time will be retrieved
 *  \param[in]   PortIdx                    Index of the EthTSyn port the message was transmitted on
 *  \param[in]   MsgType                    The type of the transmitted message
 *  \param[out]  EgressTimestampHandlePtr   The parts of the egress timestamp handle relevant for sync messages
 *  \return      E_OK - Successfully retrieve sync reference time (or not a sync message)
 *  \return      E_NOT_OK - Failed to retrieve sync reference time
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_EgressTimestamp_GetSyncRefTime(
                EthTSyn_PortIdxType                 PortIdx,
                uint8                               MsgType,
  ETHTSYN_P2VAR(EthTSyn_EgressTimestampHandleType)  EgressTimestampHandlePtr);

/**********************************************************************************************************************
 *  EthTSyn_EgressTimestamp_GetTimestamp
 *********************************************************************************************************************/
/*! \brief       Get the actual egress timestamp
 *  \param[in]   PortIdx                    Index of the EthTSyn port the message was transmitted on
 *  \param[in]   MsgType                    The type of the transmitted message
 *  \param[in]   BufIdx                     Index of the Ethernet TxBuffer
 *  \param[out]  EgressTimestampHandlePtr   The actual egress timestamp in the egress timestamp handle
 *  \return      E_OK - Successfully retrieve the egress timestamp
 *  \return      E_NOT_OK - Failed to retrieve the egress timestamp
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_EgressTimestamp_GetTimestamp(
                EthTSyn_PortIdxType                 PortIdx,
                uint8                               MsgType,
                uint8                               BufIdx,
  ETHTSYN_P2VAR(EthTSyn_EgressTimestampHandleType)  EgressTimestampHandlePtr);

/**********************************************************************************************************************
 *  EthTSyn_EgressTimestamp_GetTimestamp
 *********************************************************************************************************************/
/*! \brief      Get the actual egress timestamp
 *  \param[in]  PortIdx                    Index of the EthTSyn port the message was transmitted on
 *  \param[in]  MsgType                    The type of the transmitted message
 *  \param[in]  EgressTimestampHandlePtr   The egress timestamp handle
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_EgressTimestamp_AssignTimestamp(
                  EthTSyn_PortIdxType                 PortIdx,
                  uint8                               MsgType,
  ETHTSYN_P2CONST(EthTSyn_EgressTimestampHandleType)  EgressTimestampHandlePtr);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_GetTxPortAndMsgType
 *********************************************************************************************************************/
/*! \brief      Get EthTSyn-Port and message type the TxConfirmation was called for
 *  \param[in]  EthTSynCtrlIdx  Pointer to the common message header
 *  \param[in]  TxBufIdx        Transmission buffer index
 *  \param[out] PortIdxPtr      Index of the Port the message was transmitted on
 *  \param[out] MsgTypePtr      Type of the message the TxConfirmation was called for
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_GetTxPortAndMsgType(
                EthTSyn_CtrlIdxType  EthTSynCtrlIdx,
                uint8                TxBufIdx,
  ETHTSYN_P2VAR(EthTSyn_PortIdxType) PortIdxPtr,
  ETHTSYN_P2VAR(uint8)               MsgTypePtr);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_IsEgressTimestampRequired
 *********************************************************************************************************************/
/*! \brief      Get EthTSyn-Port and message type the TxConfirmation was called for
 *  \param[in]  PortIdx  Index of the Port the message was transmitted on
 *  \param[in]  MsgType  Type of the message the TxConfirmation was called for
 *  \return     TRUE - Egress timestamp required
 *  \return     FALSE - Egress timestamp not required
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_TxConf_IsEgressTimestampRequired(
  EthTSyn_PortIdxType  PortIdx,
  uint8                MsgType);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProcessTxConfirmation
 *********************************************************************************************************************/
/*! \brief      Process the TxConfirmation, i.e. handle state machines
 *  \param[in]  PortIdx     Index of the Port the message was transmitted on
 *  \param[in]  MsgType     Type of the message the TxConfirmation was called for
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProcessTxConfirmation(
                EthTSyn_PortIdxType  PortIdx,
                uint8                MsgType);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProcessSyncTxConfirmation
 *********************************************************************************************************************/
/*! \brief      Process the TxConfirmation for a Sync message
 *  \param[in]  PortIdx     Index of the Port the Sync message was transmitted on
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProcessSyncTxConfirmation(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProcessFollowUpTxConfirmation
 *********************************************************************************************************************/
/*! \brief      Process the TxConfirmation for a FollowUp message
 *  \param[in]  PortIdx     Index of the Port the FollowUp message was transmitted on
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProcessFollowUpTxConfirmation(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_HandleSyncSentCallback
 *********************************************************************************************************************/
/*! \brief      Handle the optional callback for a transmitted Sync message
 *  \param[in]  PortIdx  Index of the Port the Sync message was transmitted on
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_HandleSyncSentCallback(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_HandleFollowUpSentCallback
 *********************************************************************************************************************/
/*! \brief      Handle the optional callback for a transmitted FollowUp message
 *  \param[in]  PortIdx  Index of the Port the FollowUp message was transmitted on
 *  \context    EthTSyn_TxConfirmation()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_HandleFollowUpSentCallback(
  EthTSyn_PortIdxType PortIdx);

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_RxInd_HandleMessageReception
 *********************************************************************************************************************/
/*! \brief      Performs basic message checks and gets the EthTSyn Port the message was received on
 *  \param[in]  EthIfCtrlIdx  Index of the EthIf Ctrl the message was received on
 *  \param[in]  LenByte       The received message length passed to EthTSyn_RxIndication() from EthIf
 *  \param[in]  DataPtr       Pointer to the Rx-Buffer passed to the EthTSyn_RxIndication from EthIf
 *  \param[out] ErrorIdPtr    If any error occurred which should be reported to the DET, the Error Id will be stored
 *                            here
 *  \context    EthTSyn_RxIndication()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RxInd_HandleMessageReception(
                  uint8   EthIfCtrlIdx,
                  uint16  LenByte,
  ETHTSYN_P2CONST(uint8)  DataPtr,
    ETHTSYN_P2VAR(uint8)  ErrorIdPtr);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_RxInd_PerformBasicMsgChecksAngGetPort
 *********************************************************************************************************************/
/*! \brief      Performs basic message checks and gets the EthTSyn Port the message was received on
 *  \param[in]  EthIfCtrlIdx  Index of the EthIf Ctrl the message was received on
 *  \param[in]  LenByte       The received message length passed to EthTSyn_RxIndication() from EthIf
 *  \param[in]  DataPtr       Pointer to the Rx-Buffer passed to the EthTSyn_RxIndication from EthIf
 *  \param[out] PortIdxPtr    Index of the EthTSyn Port the message was received on
 *  \param[out] ErrorIdPtr    If any error occurred which should be reported to the DET, the Error Id will be stored
 *                            here
 *  \return     E_OK - Basic message checks passed
 *  \return     E_NOT_OK - Basic message check not passed
 *  \context    EthTSyn_RxIndication()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_RxInd_PerformBasicMsgChecksAngGetPort(
                  uint8                 EthIfCtrlIdx,
                  uint16                LenByte,
  ETHTSYN_P2CONST(uint8)                DataPtr,
    ETHTSYN_P2VAR(EthTSyn_PortIdxType)  PortIdxPtr,
    ETHTSYN_P2VAR(uint8)                ErrorIdPtr);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_SetLocalClockIdentity
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SetLocalClockIdentity(EthTSyn_CtrlIdxType EthTSynCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Eth_PhysAddrType physAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the physical address for the EthIf controller of the passed EthTSyn controller. */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  EthIf_GetPhysAddr(EthTSyn_CtrlConfig[EthTSynCtrlIdx].EthIfCtrlIdx, &physAddr[0]);

  /* #20 Derive the clock identity from the physical address and store it. */
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[0] = physAddr[0];
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[1] = physAddr[1];
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[2] = physAddr[2];
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[3] = 0xFF;
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[4] = 0xFE;
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[5] = physAddr[3];
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[6] = physAddr[4];
  EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity[7] = physAddr[5];
} /* EthTSyn_SetLocalClockIdentity() */

/**********************************************************************************************************************
 *  EthTSyn_InitPort
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitPort(EthTSyn_TimeDomainIdxType TimeDomainIdx,
  EthTSyn_PortIdxType PortIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Set default port values. */
  /* #10 Check port role. */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_MASTER)
  {
    /* #100 Master Port --> Initialize Sync send state machine. */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);
    /* set SyncSendSm values */
    syncSendSmPtr->SyncTxBufIdx = ETHTSYN_INV_BUF_IDX;
    syncSendSmPtr->FollowUpTxBufIdx = ETHTSYN_INV_BUF_IDX;
    syncSendSmPtr->TxConfirmationPending = 0;

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
    /* End-Station */
    ETHTSYN_DUMMY_STATEMENT(TimeDomainIdx); /* PRQA S 3453, 3112, 3199 */ /* MD_MSR_19.7 */ /* MD_MSR_14.2 */
    syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_INITIALIZING;
    syncSendSmPtr->SyncTxBufPtr = NULL_PTR;
    syncSendSmPtr->FollowUpTxBufPtr = NULL_PTR;
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    syncSendSmPtr->TxIntervalCnt = ETHTSYN_CFG_GET_SYNC_TX_INTERVAL_CNT(PortIdx);
#else /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */
    /* Bridge */
    if(EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx) == 0)
    {
      if((EthTSyn_Util_GetPortType(PortIdx) & ETHTSYN_PORT_TYPE_END_STATION_PORT) != 0)
      {
        /* End-Station Port */
        syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_DISABLED;
      }
      else
      {
        /* Switch Port */
        syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_INITIALIZING;
        /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
        syncSendSmPtr->TxIntervalCnt = EthTSyn_MasterPortConfigPtr(PortIdx)->SyncSendInterval;
        syncSendSmPtr->LogMessageInterval = EthTSyn_MasterPortConfigPtr(PortIdx)->SyncLogMessageInterval;
      }
    }
    else
    {
      syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_PENDING;
    }
    syncSendSmPtr->SyncTxBufPtr = NULL_PTR;
    syncSendSmPtr->FollowUpTxBufPtr = NULL_PTR;
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */
  } /* END: If Master Port */
  else /* Slave Port */
  {
    /* #101 Slave-Port -> Initialize Sync receive state machine. */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    IpBase_Fill(ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx), 0, sizeof(EthTSyn_SyncReceiveSmType));
  }

} /* End of EthTSyn_InitPort() */

/**********************************************************************************************************************
 *  EthTSyn_InitTimeDomain
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitTimeDomain(EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* PRQA S 0781 1 */ /* MD_EthTSyn_5.6 */
  EthTSyn_PortIdxType masterPortCnt = EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx);
  EthTSyn_PortIdxType slavePortCnt = EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx);

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Reset time base update counter valid flag. */
  EthTSyn_Master_TimeBaseUpdateCounterHandler[TimeDomainIdx].IsValueValid = FALSE;

  /* #20 Initialize all master ports. */
  while(masterPortCnt > 0)
  {
    masterPortCnt--;
    EthTSyn_InitPort(TimeDomainIdx, EthTSyn_CfgGetMasterPortIdx(TimeDomainIdx, masterPortCnt));
  }

  /* #30 Initialize all slave ports. */
  while(slavePortCnt > 0)
  {
    EthTSyn_PortIdxType slavePortIdx;

    slavePortCnt--;
    slavePortIdx =  EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt);
    EthTSyn_InitPort(TimeDomainIdx, slavePortIdx);

    /* set SyncReceiveSm values */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_SYNC_RECEIVE_SM_PTR(slavePortIdx)->State = ETHTSYN_STATE_SYNC_RECEIVE_DISCARD;
  }

  /* #40 Reset master priority. */
  EthTSyn_ResetMasterPriorityVector(TimeDomainIdx);

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  /* #50 Init Site sync sync state machine if applicable. */
  slavePortCnt = EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx);
  if(slavePortCnt == 0)
  {
    EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_DISABLED;
  }
  else
  {
    EthTSyn_SiteSyncSyncSm(TimeDomainIdx).TxConfirmationPending = 0;
    EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING;

    /* #60 Set sync receive state machine values required when switch management is enabled. */
    ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) = ETHTSYN_INVALID_PORT_IDX;
    while(slavePortCnt > 0)
    {
      slavePortCnt--;
      if( (EthTSyn_Util_GetPortType(EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt)) &
          ETHTSYN_PORT_TYPE_SWITCH_PORT) != 0 )
      {
        /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
        ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) = EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt);
        /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
        ETHTSYN_SYNC_RECEIVE_SM_PTR(ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx))->Sync.SyncRxTimeoutCnt =
            EthTSyn_CfgGetSyncTimeout(TimeDomainIdx);
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
        /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
        ETHTSYN_SYNC_RECEIVE_SM_PTR(
            ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx))->Sync.EgressTimestampHostPortState =
                ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
        break;
      }
    }
  }
#else
  /* End-Station */
  /* Nothing to do here */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

} /* EthTSyn_InitTimeDomain() */

/***********************************************************************************************************************
 * EthTSyn_InitCtrl
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitCtrl(EthTSyn_CtrlIdxType EthTSynCtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_CtrlState[EthTSynCtrlIdx].TransmissionMode = ETHTSYN_TX_ON;
  IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))EthTSyn_CtrlState[EthTSynCtrlIdx].ClockIdentity, 0,
    sizeof(EthTSyn_ClockIdentityType));
} /* EthTSyn_InitCtrl() */

/***********************************************************************************************************************
 * EthTSyn_InitPdelay
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_InitPdelay(EthTSyn_PdelayIdxType PdelayIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize Pdelay Request state machine if required. */
  if(EthTSyn_PdelayConfig[PdelayIdx].PdelayInitiatorCfgPtr != NULL_PTR)
  {
    ETHTSYN_P2CONST(EthTSyn_PdelayInitiatorCfgType) pDelayInitiatorPtr =
      EthTSyn_PdelayConfig[PdelayIdx].PdelayInitiatorCfgPtr;

    if(pDelayInitiatorPtr->PdelayReqSmPtr != NULL_PTR)
    {
      ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr = pDelayInitiatorPtr->PdelayReqSmPtr;

      pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_NOT_ENABLED;
      pDelayReqSmPtr->PdelayReq.TxIntervalCnt = pDelayInitiatorPtr->PdelaySendInterval;
      pDelayReqSmPtr->TxBufIdx = ETHTSYN_INV_BUF_IDX;
      pDelayReqSmPtr->TxConfirmationPending = 0u;
      pDelayReqSmPtr->Pdelay = EthTSyn_PdelayConfig[PdelayIdx].InitialPdelay;
      pDelayReqSmPtr->LostResponses = 0u;
      pDelayReqSmPtr->AsCapable = EthTSyn_PdelayConfig[PdelayIdx].AlwaysAsCapable;
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
      pDelayReqSmPtr->TxBufPtr = NULL_PTR;
#  if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
      pDelayReqSmPtr->FrameId = ETHTSYN_INVALID_FRAME_ID;
#  endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    }
  }

  /* #20 Initialize Pdelay Response state machine if required. */
  if(EthTSyn_PdelayConfig[PdelayIdx].PdelayResponderCfgPtr != NULL_PTR)
  {
    if(EthTSyn_PdelayConfig[PdelayIdx].PdelayResponderCfgPtr->PdelayRespSmPtr != NULL_PTR)
    {
      ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr =
        EthTSyn_PdelayConfig[PdelayIdx].PdelayResponderCfgPtr->PdelayRespSmPtr;

      pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_NOT_ENABLED;
      pDelayRespSmPtr->TxBufIdx = ETHTSYN_INV_BUF_IDX;
      pDelayRespSmPtr->TxConfirmationPending = 0u;
      pDelayRespSmPtr->InProgress = FALSE;
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
      pDelayRespSmPtr->TxBufPtr = NULL_PTR;
#  if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
      pDelayRespSmPtr->FrameId = ETHTSYN_INVALID_FRAME_ID;
#  endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    }
  }
} /* EthTSyn_InitPdelay() */

/***********************************************************************************************************************
 * EthTSyn_ResetPortsAndPdelayOfCtrl
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ResetPortsAndPdelayOfCtrl(EthTSyn_CtrlIdxType EthTSynCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxIterType portIter;
  EthTSyn_PortIdxType portIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all EthTSyn ports of the given EthTSyn controller. */
  for(portIter = 0; portIter < EthTSyn_CtrlConfig[EthTSynCtrlIdx].EthTSynPortCount; portIter++)
  {
    portIdx = EthTSyn_CtrlConfig[EthTSynCtrlIdx].EthTSynPortIdxPtr[portIter];

    /* #100 Init the current EthTSyn port. */
    EthTSyn_InitPort(ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(portIdx), portIdx);

    /* #101 Init the Pdelay if current port is an active Pdelay port. */
    if(portIdx == ETHTSYN_CFG_GET_P_DELAY_ACTIVE_PORT_IDX(portIdx))
    {
      EthTSyn_InitPdelay(ETHTSYN_CFG_GET_PDELAY_IDX(portIdx));
    }
  }
} /* EthTSyn_ResetPortsAndPdelayOfCtrl() */

/***********************************************************************************************************************
 * EthTSyn_HandleEgressTimestamping
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_HandleEgressTimestamping(EthTSyn_PortIdxType PortIdx,
  uint8 MsgType, uint8 BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_EgressTimestampHandleType egressTimestampHandle;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the sync reference time if applicable. */
  if(EthTSyn_EgressTimestamp_GetSyncRefTime(PortIdx, MsgType, &egressTimestampHandle) == E_OK)
  {
    /* #20 Get the actual egress timestamp. */
    if(EthTSyn_EgressTimestamp_GetTimestamp(PortIdx, MsgType, BufIdx, &egressTimestampHandle) == E_OK)
    {
      /* #30 Assign the timestamp values to the correct state machine. */
      EthTSyn_EgressTimestamp_AssignTimestamp(PortIdx, MsgType, &egressTimestampHandle);
    }
  }

} /* EthTSyn_HandleEgressTimestamping() */

/**********************************************************************************************************************
 *  EthTSyn_EgressTimestamp_GetSyncRefTime
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_EgressTimestamp_GetSyncRefTime(
  EthTSyn_PortIdxType PortIdx, uint8 MsgType, ETHTSYN_P2VAR(EthTSyn_EgressTimestampHandleType) EgressTimestampHandlePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  StbM_TimeStampType timeStampGlobalStbm;
  StbM_UserDataType userData;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimeStampQualType timeStampQual;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if message is sync message. */
  if(MsgType == ETHTSYN_MSG_TYPE_SYNC)
  {
    /* #20 Get the current global time from the StbM. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    if(StbM_GetCurrentTime(ETHTSYN_CFG_GET_SYNCHRONIZTED_TIME_BASE_ID_OF_PORT(PortIdx), &timeStampGlobalStbm,
        &userData) == E_OK)
    {
      ETHTSYN_COPY_STBM_TS_TO_GLOBAL_TS(timeStampGlobalStbm, EgressTimestampHandlePtr->GlobalTimestamp)
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
      /* HW-Timestamping */
      /* #30 If hardware timestamping is enabled, get the current HW time. */
      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      if(EthIf_GetCurrentTime(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), &timeStampQual,
          &EgressTimestampHandlePtr->SyncRefTimestamp) == E_OK)
      {
        if(timeStampQual == ETH_VALID)
        {
          retVal = E_OK;
        }
      }
#else
      /* SW-Timestamping */
      retVal = E_OK;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

    }
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_EgressTimestamp_GetSyncRefTime() */

/**********************************************************************************************************************
 *  EthTSyn_EgressTimestamp_GetTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_EgressTimestamp_GetTimestamp(
  EthTSyn_PortIdxType PortIdx, uint8 MsgType, uint8 BufIdx,
  ETHTSYN_P2VAR(EthTSyn_EgressTimestampHandleType) EgressTimestampHandlePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimeStampQualType timeStampQual;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If hardware timestamping is enabled, get the egress timestamp from the hardware. */
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  /* HW-Timestamping */
  ETHTSYN_DUMMY_STATEMENT(MsgType); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  retVal = E_NOT_OK;
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(EthIf_GetEgressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), BufIdx, &timeStampQual,
      &EgressTimestampHandlePtr->EgressTimestamp) == E_OK)
  {
    if(timeStampQual == ETH_VALID)
    {
      retVal = E_OK;
    }
  }
#else
  /* SW-Timestamping */
  /* #20 If hardware timestamping is disabled (i.e. SW timestamping is used) get the current raw time from the StbM */
  ETHTSYN_DUMMY_STATEMENT(BufIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  if((MsgType == ETHTSYN_MSG_TYPE_PDELAY_REQ) || (MsgType == ETHTSYN_MSG_TYPE_PDELAY_RESP))
  {
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    retVal = StbM_GetCurrentTimeRaw(&EgressTimestampHandlePtr->RawTimestamp);
  }
  else
  {
    retVal = E_OK;
  }
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  return retVal;
} /* EthTSyn_EgressTimestamp_GetTimestamp() */

/**********************************************************************************************************************
 *  EthTSyn_EgressTimestamp_AssignTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_EgressTimestamp_AssignTimestamp(EthTSyn_PortIdxType PortIdx,
  uint8 MsgType, ETHTSYN_P2CONST(EthTSyn_EgressTimestampHandleType) EgressTimestampHandlePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_GlobalTimestampType timeStampGlobal;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  EthTSyn_TimediffType timeDiffEgressAndRefSync = 0;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  timeStampGlobal = EgressTimestampHandlePtr->GlobalTimestamp;

  /* #10 Check the message type and assign the timestamp to the corresponding state machine. */
  switch(MsgType)
  {
  case ETHTSYN_MSG_TYPE_SYNC:
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpPreciseOriginTimestampState |=
      ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    /* HW-Timestamping */
    if(EthTSyn_Util_TimestampMinusTimestamp(&EgressTimestampHandlePtr->EgressTimestamp,
        &EgressTimestampHandlePtr->SyncRefTimestamp, &timeDiffEgressAndRefSync) == E_OK)
    {
      if(EthTSyn_Util_TimestampPlusTimediff(&timeStampGlobal, &timeDiffEgressAndRefSync) == E_OK)
      {
        /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
        ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpPreciseOriginTimestampState |=
          ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK;
      }
    }
#else
    /* SW-Timestamping */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpPreciseOriginTimestampState |= ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpPreciseOriginTimestamp = timeStampGlobal;
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
    ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.EgressTimestampState =
      ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    /* HW-Timestamping */
    ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.EgressTimestamp = EgressTimestampHandlePtr->EgressTimestamp;
#else
    /* SW-Timestamping */
    ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.EgressTimestamp.nanoseconds = EgressTimestampHandlePtr->RawTimestamp;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
    ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayResp.EgressTimestampState =
      ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    /* HW-Timestamping */
    ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayResp.EgressTimestamp = EgressTimestampHandlePtr->EgressTimestamp;
#else
    /* SW-Timestamping */
    ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayResp.EgressTimestamp.nanoseconds =
        EgressTimestampHandlePtr->RawTimestamp;
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
    break;

  default:
    /* This should never happen. Nothing to do here (MISRA) */
    break;
  }
} /* EthTSyn_EgressTimestamp_AssignTimestamp() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_GetTxPortAndMsgType
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_GetTxPortAndMsgType(EthTSyn_CtrlIdxType EthTSynCtrlIdx,
  uint8  TxBufIdx, ETHTSYN_P2VAR(EthTSyn_PortIdxType) PortIdxPtr, ETHTSYN_P2VAR(uint8) MsgTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxIterType portIter;
  EthTSyn_PortIdxType portIdx;
  uint8 msgType = ETHTSYN_MSG_TYPE_INVALID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize out parameters. */
  *PortIdxPtr = ETHTSYN_INVALID_PORT_IDX;
  *MsgTypePtr = ETHTSYN_MSG_TYPE_INVALID;

  /* #20 Iterate over all Ports of the EthTSyn controller. */
  for(portIter = 0; portIter < EthTSyn_CtrlConfig[EthTSynCtrlIdx].EthTSynPortCount; portIter++)
  {
    portIdx = EthTSyn_CtrlConfig[EthTSynCtrlIdx].EthTSynPortIdxPtr[portIter];

    /* #200 Check if any message was transmitted on the current port using the given Tx-Buffer. */
    if(EthTSyn_CfgAccess_IsPdelayInitiator(portIdx) == TRUE)
    {
      if(ETHTSYN_P_DELAY_REQ_SM_PTR(portIdx)->TxBufIdx == TxBufIdx)
      {
        /* Port and message type found */
        msgType = ETHTSYN_MSG_TYPE_PDELAY_REQ;
      }
    }

    if(EthTSyn_CfgAccess_IsPdelayResponder(portIdx) == TRUE)
    {
      if(ETHTSYN_P_DELAY_RESP_SM_PTR(portIdx)->TxBufIdx == TxBufIdx)
      {
        msgType = ETHTSYN_MSG_TYPE_PDELAY_RESP;
      }
    }

    if(EthTSyn_CfgGetPortRole(portIdx) == ETHTSYN_PORT_ROLE_MASTER)
    {
      if(ETHTSYN_SYNC_SEND_SM_PTR(portIdx)->SyncTxBufIdx == TxBufIdx) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        msgType = ETHTSYN_MSG_TYPE_SYNC;
      }
      else if(ETHTSYN_SYNC_SEND_SM_PTR(portIdx)->FollowUpTxBufIdx == TxBufIdx) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        msgType = ETHTSYN_MSG_TYPE_FOLLOW_UP;
      }
      else
      {
        /* Nothing to do here (MISRA). */
      }
    }

    if(msgType != ETHTSYN_MSG_TYPE_INVALID)
    {
      /* #30 Port and message type found. Set out parameters and stop iteration of Ports. */
      *PortIdxPtr = portIdx;
      *MsgTypePtr = msgType;
      break;
    }
  } /* END: Iteration of Ports for the given Controller */
} /* EthTSyn_TxConf_GetTxPortAndMsgType() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_IsEgressTimestampRequired
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_TxConf_IsEgressTimestampRequired(EthTSyn_PortIdxType PortIdx,
  uint8 MsgType)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean egressTimestampRequired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check message type. */
  switch(MsgType)
  {
  case ETHTSYN_MSG_TYPE_SYNC:
    /* #20 Sync message. Check if egress timestamp is required. */ /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    if((ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpPreciseOriginTimestampState &
      ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP) != 0)
    {
      egressTimestampRequired = TRUE;
    }
    break;

#if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF)
  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
    /* #30 Pdelay_Req || Pdelay_Resp. Egress timestamp required (no Switch Timestamping). */
    egressTimestampRequired = TRUE;
    break;
#endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) */

  default:
    /* Nothing to do here (MISRA). */
    break;
  }

  return egressTimestampRequired;
} /* EthTSyn_TxConf_IsEgressTimestampRequired() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProcessTxConfirmation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProcessTxConfirmation(EthTSyn_PortIdxType PortIdx,
    uint8 MsgType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Perform required actions for the message type. */
  switch(MsgType)
  {
  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
    ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->TxBufIdx = ETHTSYN_INV_BUF_IDX;
    ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->TxConfirmationPending--;
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
    ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->TxBufIdx = ETHTSYN_INV_BUF_IDX;
    ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->TxConfirmationPending--;
    break;

  case ETHTSYN_MSG_TYPE_SYNC:
    EthTSyn_TxConf_ProcessSyncTxConfirmation(PortIdx);
    break;

  case ETHTSYN_MSG_TYPE_FOLLOW_UP:
    EthTSyn_TxConf_ProcessFollowUpTxConfirmation(PortIdx);
    break;

  default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
    /* Nothing to do here (MISRA) */
    break;
  }
} /* EthTSyn_TxConf_ProcessTxConfirmation() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProcessSyncTxConfirmation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProcessSyncTxConfirmation(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  if(ETHTSYN_STATE_GET_SLAVE_PORT_IDX(timeDomainIdx) == ETHTSYN_INVALID_PORT_IDX)
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  {
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->TxConfirmationPending--; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #10 Handle Sync Sent Callback. */
    EthTSyn_TxConf_HandleSyncSentCallback(PortIdx);
  }
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  else
  {
    /* #20 Handle site sync sync state machine if applicable. */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->State = ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_WAIT_FOLLOW_UP;
    if(EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending > 0)
    {
      EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending--;
      if(EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending == 0)
      {
        EthTSyn_SiteSyncSyncSm(timeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING;
      }
    }
  }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncTxBufIdx = ETHTSYN_INV_BUF_IDX; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
} /* EthTSyn_TxConf_ProcessSyncTxConfirmation() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProcessFollowUpTxConfirmation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProcessFollowUpTxConfirmation(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  if(ETHTSYN_STATE_GET_SLAVE_PORT_IDX(timeDomainIdx) == ETHTSYN_INVALID_PORT_IDX)
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  {
    /* #10 Handle FollowUp sent callback. */
    EthTSyn_TxConf_HandleFollowUpSentCallback(PortIdx);
  }
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  else
  {
    /* #20 Handle site sync sync state machine if applicable. */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->State = ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_PENDING;
    if(EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending > 0)
    {
      EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending--;
      if(EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending == 0)
      {
        EthTSyn_SiteSyncSyncSm(timeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING;
      }
    }
  }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpTxBufIdx = ETHTSYN_INV_BUF_IDX;
} /* EthTSyn_TxConf_ProcessFollowUpTxConfirmation() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_HandleSyncSentCallback
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_HandleSyncSentCallback(EthTSyn_PortIdxType PortIdx)
{
  /* #10 Check if Sync Sent Callback is configured. */
  if(EthTSyn_MasterPortConfigPtr(PortIdx)->SyncSentCbkPtr != NULL_PTR) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    /* #100 Call Sync Sent Callback. */
    /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgPtr =
      (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncTxBufPtr;
    ETHTSYN_P2CONST(EthTSyn_SyncMsgType) syncPtr =
      (ETHTSYN_P2CONST(EthTSyn_SyncMsgType))&comMsgPtr[ETHTSYN_HDR_COM_LENGTH];

    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    EthTSyn_MasterPortConfigPtr(PortIdx)->SyncSentCbkPtr(PortIdx, comMsgPtr, syncPtr);

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncTxBufPtr = NULL_PTR; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */
  }
} /* EthTSyn_TxConf_HandleSyncSentCallback() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_HandleFollowUpSentCallback
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_HandleFollowUpSentCallback(EthTSyn_PortIdxType PortIdx)
{
  /* #10 Check if FollowUp Sent Callback is configured. */
  if(EthTSyn_MasterPortConfigPtr(PortIdx)->FollowUpSentCbkPtr != NULL_PTR) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    /* #100 Call FollowUp Sent Callback. */
    /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgPtr =
      (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpTxBufPtr;
    ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType) followUpPtr =
      (ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType))&comMsgPtr[ETHTSYN_HDR_COM_LENGTH];
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    EthTSyn_MasterPortConfigPtr(PortIdx)->FollowUpSentCbkPtr(PortIdx, comMsgPtr, followUpPtr);

    ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->FollowUpTxBufPtr = NULL_PTR; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  }
} /* EthTSyn_TxConf_HandleFollowUpSentCallback */

/**********************************************************************************************************************
 *  EthTSyn_RxInd_HandleMessageReception
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RxInd_HandleMessageReception(uint8 EthIfCtrlIdx, uint16 LenByte,
  ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
  EthTSyn_PortIdxType portIdx = ETHTSYN_INVALID_PORT_IDX;
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgPtr = (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))DataPtr;
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Perform basic message checks and get EthTSyn PortIdx. */
  if(EthTSyn_RxInd_PerformBasicMsgChecksAngGetPort(EthIfCtrlIdx, LenByte, DataPtr, &portIdx, ErrorIdPtr) == E_OK)
  {
    /* Port found -> continue processing of the message */
    /* #20 Get message type and process the message if applicable. */
    switch(ETHTSYN_HDR_COM_MSG_TYPE(comMsgPtr->TransSpecific_MessageType))
    {
    case ETHTSYN_MSG_TYPE_PDELAY_REQ:
      EthTSyn_Pdelay_ProcessReceivedPdelayReqMsg(comMsgPtr, portIdx);
      break;

    case ETHTSYN_MSG_TYPE_PDELAY_RESP:
      EthTSyn_Pdelay_ProcessReceivedPdelayRespMsg(comMsgPtr, portIdx);
      break;

    case ETHTSYN_MSG_TYPE_PDELAY_RES_FOLLOW_UP:
      EthTSyn_Pdelay_ProcessReceivedRespFollowUpMsg(comMsgPtr, portIdx);
      break;

    case ETHTSYN_MSG_TYPE_SYNC:
      EthTSyn_SlaveRx_ProcessReceivedSyncMsg(comMsgPtr, portIdx);
      break;

    case ETHTSYN_MSG_TYPE_FOLLOW_UP:
      EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg(comMsgPtr, portIdx);
      break;

    case ETHTSYN_MSG_TYPE_ANNOUNCE:
      EthTSyn_Slave_ProcessReceivedAnnounceMsg(comMsgPtr, portIdx);
      break;

    default:
      /* Nothing to do here (MISRA) */
      break;
    }
  }
} /* EthTSyn_RxInd_HandleMessageReception() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_RxInd_PerformBasicMsgChecksAngGetPort
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_RxInd_PerformBasicMsgChecksAngGetPort(
  uint8 EthIfCtrlIdx, uint16 LenByte, ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2VAR(EthTSyn_PortIdxType) PortIdxPtr,
  ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Perform general message checks. */
  if(EthTSyn_UtilRx_CheckMessageGeneral(DataPtr, LenByte, ErrorIdPtr) == E_OK)
  {
    comMsgPtr = (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))DataPtr; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    /* #20 General message checks passed. Check message type and length. */
    if(EthTSyn_UtilRx_CheckMessageTypeAndLength(comMsgPtr, LenByte, ErrorIdPtr) == E_OK)
    {
      EthTSyn_PortIdxType portCounter;
      /* #30 Valid message type and length. Get the EthTSyn Port. */
      for(portCounter = 0; portCounter < ETHTSYN_PORT_COUNT; portCounter++)
      {
        if((ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(portCounter) == EthIfCtrlIdx) &&
          (EthTSyn_CfgGetTimeDomainId(portCounter) == comMsgPtr->DomainNumber))
        {
          /* #300 EthTSyn Port found. */
          *PortIdxPtr = portCounter;
          retVal = E_OK;
          break;
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_RxInd_PerformBasicMsgChecksAngGetPort() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_CheckAnnounce
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CheckAnnounce(ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Verify the System State. */
  /* PRQA S 3415 2 */ /* MD_EthTSyn_12.4 */
  if( (EthTSyn_SystemState[TimeDomainIdx].GmPresent == TRUE) &&
    (EthTSyn_CompareClockIdentity(ComMsgPtr->SourceClockIdentity,
    EthTSyn_SystemState[TimeDomainIdx].GmPriority.PortIdentity.ClockIdentity) == ETHTSYN_COMPARE_EQUAL) )
  {
    retVal = TRUE;
  }

  return retVal;
} /* EthTSyn_CheckAnnounce() */

/**********************************************************************************************************************
 *  EthTSyn_CheckSourcePortIdentity
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CheckSourcePortIdentity(
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType SlavePortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify source port identity. */
  /* PRQA S 3415, 0310 3 */ /* MD_EthTSyn_12.4 */ /* MD_EthTSyn_11.4 */
  if( (EthTSyn_Ntohs(ComMsgPtr->SourcePortNumber) == EthTSyn_CfgGetMasterPortIdentity(SlavePortIdx).PortNumber) &&
      (EthTSyn_CompareClockIdentity(ComMsgPtr->SourceClockIdentity,
         EthTSyn_CfgGetMasterPortIdentity(SlavePortIdx).ClockIdentity) == ETHTSYN_COMPARE_EQUAL) )
  {
    retVal = TRUE;
  }

  return retVal;
} /* EthTSyn_CheckSourcePortIdentity() */

/**********************************************************************************************************************
 *  EthTSyn_Transmit
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
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Transmit(EthTSyn_PortIdxType PortIdx, uint8 BufIdx, uint16 LenByte,
  boolean TxConfirmation, ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr)
#else
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Transmit(EthTSyn_PortIdxType PortIdx, uint8 BufIdx,
  uint16 LenByte, boolean TxConfirmation)
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrlIdx = ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx); /* PRQA S 0781 */ /* MD_EthTSyn_5.6 */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  /* #10 Set the switch egress port for the message to be transmitted if applicable. */
  if(MgmtInfoPtr != NULL_PTR)
  {
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    (void)EthIf_SetSwitchMgmtInfo(ctrlIdx, BufIdx, MgmtInfoPtr);
  }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* #20 Trigger the transmission. */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(EthIf_Transmit(ctrlIdx, BufIdx, ETHTSYN_FRAME_TYPE, TxConfirmation, LenByte,
    (ETHTSYN_P2VAR(uint8))EthTSyn_DestinationMac) == E_OK)
  {
    /* #200 Transmission successfull -> return with E_OK. */
    retVal = E_OK;
  }
  else
  {
    /* #201 Transmission failed -> release Ethernet TxBuffer and return with E_NOT_OK. */
    errorId = ETHTSYN_E_TX_FAILED;
    /* Release TxBuffer */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    (void)EthIf_Transmit(ctrlIdx, BufIdx, ETHTSYN_FRAME_TYPE, TxConfirmation, 0,
      (ETHTSYN_P2VAR(uint8))EthTSyn_DestinationMac);
  }

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TRANSMIT, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
} /* EthTSyn_Transmit() */

/**********************************************************************************************************************
 *  EthTSyn_RxIndication
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
/* PRQA S 3673 2 */ /* MD_EthTSyn_16.7c */
FUNC(void, ETHTSYN_CODE) EthTSyn_RxIndication(uint8 CtrlIdx, Eth_FrameType FrameType, boolean IsBroadcast,
  EthTSyn_PhyAddrPtrType PhysAddrPtr, ETHTSYN_P2VAR(uint8) DataPtr, uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if (EthTSyn_State == ETHTSYN_STATE_UNINIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if ((DataPtr == NULL_PTR) || (PhysAddrPtr == NULL_PTR))
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else if (FrameType != ETHTSYN_FRAME_TYPE)
  {
    errorId = ETHTSYN_E_INV_FRAME_TYPE;
  }
  else if (EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(CtrlIdx) >= ETHTSYN_CTRL_COUNT)
  {
    errorId = ETHTSYN_E_CTRL_IDX;
  }
  else if(EthTSyn_CtrlState[EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(CtrlIdx)].TrcvLinkState ==
      ETHTRCV_LINK_STATE_DOWN)
  {
    errorId = ETHTSYN_E_TRCV_DOWN;
  }
  else
#endif
  {
# if (ETHTSYN_DEV_ERROR_DETECT == STD_OFF)
    ETHTSYN_DUMMY_STATEMENT(FrameType); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    ETHTSYN_DUMMY_STATEMENT(PhysAddrPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_OFF) */
    ETHTSYN_DUMMY_STATEMENT(IsBroadcast); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    /* #20 Switch management enabled. Pass the received Frame to the Switch management. */
    EthTSyn_SwtMgmtRx_HandleMessageReception(CtrlIdx, DataPtr, LenByte);
#else
    /* #30 Switch management disabled. Handle message reception. */
    EthTSyn_RxInd_HandleMessageReception(CtrlIdx, LenByte, DataPtr, &errorId);
#endif /* ETHTSYN_SWT_MGMT_SUPPORT */
  }

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_RX_INDICATION, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

} /* EthTSyn_RxIndication() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */

#if (ETHTSYN_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_GetVersionInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_GetVersionInfo(ETHTSYN_P2VAR(Std_VersionInfoType) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check VersionInfoPtr for NULL pointer. note: no uninit check is performed. */
  if(VersionInfoPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Set version info with corresponding macros from component header. */
    VersionInfoPtr->vendorID = (uint16)ETHTSYN_VENDOR_ID;
    VersionInfoPtr->moduleID = (uint16)ETHTSYN_MODULE_ID;
    VersionInfoPtr->sw_major_version = (uint8)ETHTSYN_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = (uint8)ETHTSYN_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = (uint8)ETHTSYN_SW_PATCH_VERSION;
  }

# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_GET_VERSION_INFO, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
} /* EthTSyn_GetVersionInfo() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (ETHTSYN_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_MainFunction
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
FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of component. */
  if(EthTSyn_State != ETHTSYN_STATE_INIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Iterate all TimeDomains. */
    for(timeDomainIdx = 0; timeDomainIdx < ETHTSYN_TIME_DOMAIN_COUNT; timeDomainIdx++)
    {
      /* #200 Handle cyclic tasks for Master-Ports. */
      EthTSyn_Master_HandleCyclicTasks(timeDomainIdx);

      /* #201 Handle cyclic tasks for Slave-Ports. */
      EthTSyn_Slave_HandleCyclicTasks(timeDomainIdx);

      /* #202 Process site sync sync state machine if applicable. */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      if(EthTSyn_CfgGetSlavePortCnt(timeDomainIdx) > 0)
      {
        /* SiteSyncSync state machine */
        EthTSyn_SwtMgmt_ProcessSmSiteSyncSync(timeDomainIdx);
      }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    } /* END: Iteration of TimeDomains */

#if (ETHTSYN_P_DELAY_CONFIG_COUNT > 0u)
    /* #30 Handle cyclic task for Pdelay. */
    EthTSyn_Pdelay_HandleCyclicTasks();
#endif /* (ETHTSYN_P_DELAY_CONFIG_COUNT > 0u) */

  }

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_MAIN_FUNCTION, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

} /* EthTSyn_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_InitMemory
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE)  EthTSyn_InitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_CtrlIdxType ethTSynCtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set module state to unitialized. */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  EthTSyn_State = ETHTSYN_STATE_UNINIT;
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */

  /* #20 Initialize local data. */
  for(ethTSynCtrlIdx = 0; ethTSynCtrlIdx < ETHTSYN_CTRL_COUNT; ethTSynCtrlIdx++)
  {
    EthTSyn_CtrlState[ethTSynCtrlIdx].TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
  }
}

/**********************************************************************************************************************
 *  EthTSyn_Init
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
FUNC(void, ETHTSYN_CODE) EthTSyn_Init(ETHTSYN_P2CONSTCFGROOT(EthTSyn_ConfigType) CfgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx;
  EthTSyn_CtrlIdxType ethTSynCtrlIdx;
  EthTSyn_PdelayIdxType pDelayIdx;

  /* ----- Implementation ----------------------------------------------- */
  /*lint -e438 */
  ETHTSYN_DUMMY_STATEMENT(CfgPtr); /* PRQA S 3453, 3112, 3199 */ /* MD_MSR_19.7 */ /* MD_MSR_14.2 */

  /* #10 Initialize all TimeDomains. */
  for(timeDomainIdx = 0; timeDomainIdx < ETHTSYN_TIME_DOMAIN_COUNT; timeDomainIdx++)
  {
    EthTSyn_InitTimeDomain(timeDomainIdx);
  }
  /* #20 Initialize all EthTSyn controllers. */
  for(ethTSynCtrlIdx = 0u; ethTSynCtrlIdx < ETHTSYN_CTRL_COUNT; ethTSynCtrlIdx++)
  {
    EthTSyn_InitCtrl(ethTSynCtrlIdx);
  }
  /* #30 Initialize all Pdelay handlers. */
  for(pDelayIdx = 0; pDelayIdx < ETHTSYN_P_DELAY_CONFIG_COUNT; pDelayIdx++)
  {
    EthTSyn_InitPdelay(pDelayIdx);
  }

  /* #40 Set default values as zero. */
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))EthTSyn_SystemState, 0, sizeof(EthTSyn_SystemState));

  /* #50 Initialize switch management if applicable. */
#if(ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthTSyn_SwtMgmt_Init();
#endif

  /* #60 Set module state to initialized. */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  EthTSyn_State = ETHTSYN_STATE_INIT;
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */

} /* EthTSyn_Init() */ /*lint +e438 */

/**********************************************************************************************************************
 *  EthTSyn_TrcvLinkStateChg
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
FUNC(void, ETHTSYN_CODE) EthTSyn_TrcvLinkStateChg(uint8 CtrlIdx, EthTrcv_LinkStateType TrcvLinkState)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_CtrlIdxType ethTSynCtrlIdx = ETHTSYN_CTRL_COUNT;
  EthTSyn_CtrlIdxType ethTSynCtrlIter;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component. */
  if(EthTSyn_State != ETHTSYN_STATE_INIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Get EthTSyn Controller */
    for(ethTSynCtrlIter = 0; ethTSynCtrlIter < ETHTSYN_CTRL_COUNT; ethTSynCtrlIter++)
    {
      if(EthTSyn_CtrlConfig[ethTSynCtrlIter].EthIfCtrlIdx == CtrlIdx)
      {
        /* EthTSyn Controller found */
        ethTSynCtrlIdx = ethTSynCtrlIter;
        break;
      }
    }

    if(ethTSynCtrlIdx < ETHTSYN_CTRL_COUNT)
    {
      /* #30 Check the passed transceiver link state. */
      switch(TrcvLinkState)
      {
      case ETHTRCV_LINK_STATE_DOWN:
        /* #300 Handle transceiver link state down. */
        if(EthTSyn_CtrlState[ethTSynCtrlIdx].TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
        {
          /* Trcv Link state changed */
          EthTSyn_CtrlState[ethTSynCtrlIdx].TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;

          /* remove multi-cast frame reception */
          ETHTSYN_CHECK_CRITICAL_SECTION_0();
          (void)EthIf_UpdatePhysAddrFilter(CtrlIdx, EthTSyn_DestinationMac, ETH_REMOVE_FROM_FILTER);
          /* set default values as zero */
          /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
          IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))EthTSyn_SystemState, 0, sizeof(EthTSyn_SystemType));
          EthTSyn_ResetPortsAndPdelayOfCtrl(ethTSynCtrlIdx);

          CANOE_WRITE_STRING_1("[Link] Link Down EthTSynCtrlIdx: %u", ethTSynCtrlIdx);
        }
        break;

      case ETHTRCV_LINK_STATE_ACTIVE:
        /* #301 Handle transceiver link state up. */
        if(EthTSyn_CtrlState[ethTSynCtrlIdx].TrcvLinkState == ETHTRCV_LINK_STATE_DOWN)
        {
          /* Trcv Link state changed */
          EthTSyn_CtrlState[ethTSynCtrlIdx].TrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;

          /* Init default values */
          EthTSyn_InitCtrl(ethTSynCtrlIdx);
          EthTSyn_SetLocalClockIdentity(ethTSynCtrlIdx);
          /* add multicast frame reception */
          ETHTSYN_CHECK_CRITICAL_SECTION_0();
          (void)EthIf_UpdatePhysAddrFilter(CtrlIdx, EthTSyn_DestinationMac, ETH_ADD_TO_FILTER);

          CANOE_WRITE_STRING_1("[Link] Link Up EthTSynCtrlIdx: %u", ethTSynCtrlIdx);
        }
        break;

      default:
        /* Nothing to do here (MISRA) */
        break;
      }
    }
  }

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default error if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TRCV_LINK_STATE_CHG, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_TrcvLinkStateChg() */

/**********************************************************************************************************************
 *  EthTSyn_TxConfirmation
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
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConfirmation(uint8 CtrlIdx, uint8 BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;
  EthTSyn_CtrlIdxType ethtsynCtrlIdx;
  EthTSyn_PortIdxType portIdx = ETHTSYN_INVALID_PORT_IDX;
  uint8 msgType = ETHTSYN_MSG_TYPE_INVALID;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(EthTSyn_State != ETHTSYN_STATE_INIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if(EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(CtrlIdx) >= ETHTSYN_CTRL_COUNT)
  {
    errorId = ETHTSYN_E_CTRL_IDX;
  }
  else
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Get the corresponding EthTSyn controller */
    ethtsynCtrlIdx = EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(CtrlIdx);

    /* #30 Get the EthTSyn Port and Message type */
    EthTSyn_TxConf_GetTxPortAndMsgType(ethtsynCtrlIdx, BufIdx, &portIdx, &msgType);

    if((portIdx < ETHTSYN_PORT_COUNT) && (msgType != ETHTSYN_MSG_TYPE_INVALID))
    {
      /* #40 Port and message type found. Check if egress timestamp is required */
      if(EthTSyn_TxConf_IsEgressTimestampRequired(portIdx, msgType) == TRUE)
      {
        /* #400 Handle egress timestamping */
        EthTSyn_HandleEgressTimestamping(portIdx, msgType, BufIdx);
      }

      /* #50 Process TxConfirmation */
      EthTSyn_TxConf_ProcessTxConfirmation(portIdx, msgType);
    }
  }

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TX_CONFIRMATION, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

} /* EthTSyn_TxConfirmation() */

/**********************************************************************************************************************
 *  EthTSyn_SetTransmissionMode
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SetTransmissionMode(uint8 CtrlIdx, EthTSyn_TransmissionModeType Mode)
{
  /* ------ Local Variables -----------------------------------------------*/
  EthTSyn_CtrlIdxType ethtsynCtrlIdx;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if(EthTSyn_State != ETHTSYN_STATE_INIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if((Mode != ETHTSYN_TX_ON) && (Mode != ETHTSYN_TX_OFF))
  {
    errorId = ETHTSYN_E_PARAM;
  }
  else if(EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(CtrlIdx) >= ETHTSYN_CTRL_COUNT)
  {
    errorId = ETHTSYN_E_CTRL_IDX;
  }
  else
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Set the transmission mode of the corresponding EthTSyn controller. */
    ethtsynCtrlIdx = EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(CtrlIdx);

    if(ethtsynCtrlIdx < ETHTSYN_CTRL_COUNT)
    {
      EthTSyn_CtrlState[ethtsynCtrlIdx].TransmissionMode = Mode;
    }
  }

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SET_TRANSMISSION_MODE, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

} /* EthTSyn_SetTransmissionMode() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#define ETHTSYN_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/* module specific MISRA deviations:
  MD_EthTSyn_11.4:
      Reason:     Casting to different object pointer type
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_EthTSyn_11.5:
      Reason:     Called API expects a P2VAR.
      Risk:       No risk. The object addressed by the pointer parameter is not modified within the called API.
                  The parameters is specified as 'in' parameter by AUTOSAR.
      Prevention: Covered by code review.
  MD_EthTSyn_16.7a:
      Reason:     The parameter is not modified and so the pointer could be of type 'pointer to const'
      Risk:       No risk. The object addressed by the pointer parameter is of type 'pointer to const'
      Prevention: Covered by code review.
  MD_EthTSyn_16.7b:
      Reason:     The parameter is not modified and so the pointer could be of type 'pointer to const'
      Risk:       No risk. The modification of the parameter depends on preprocessor define
      Prevention: Covered by code review
  MD_EthTSyn_16.7c:
      Reason:     The parameter is of type P2VAR due to AUTOSAR specification
      Risk:       No risk.
      Prevention: Covered by code review
  MD_EthTSyn_5.6:
      Reason:     Reuse of variable name improves readability
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_EthTSyn_12.4:
      Reason:     The right hand operand of the && operator is only evaluated if the left hand operand is "true"
      Risk:       No risk. No assignments are done in conditional statements
      Prevention: Covered by code review.
  MD_EthTSyn_14.1_3219:
      Reason:     Some inline functions used in different source files are defined in a header file. These functions
                  are not necessarily used in every source file however.
      Risk:       Some superfluous function are present which might lead to a small resource overhead.
      Prevention: Covered by code review.
  MD_EthTSyn_14.6:
      Reason:     Performance enhancement
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_EthTSyn_15.2:
      Reason:     Fall through case to reuse code parts common among different cases.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_EthTSyn_19.15:
      Reason:     Wrong MISRA Warning. Standard Include Guard is present.
      Risk:       No risk.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn.c
 *********************************************************************************************************************/
