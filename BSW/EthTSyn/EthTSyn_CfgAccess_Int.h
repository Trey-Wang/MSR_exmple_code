/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  EthTSyn_CfgAccess_Int.h
 *        \brief  EthTSyn internal header file to access configuration data
 *
 *      \details  Contains all macros and function declarations used by the EthTSyn to access the configuration data
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(ETHTSYN_CFG_ACCESS_INT_H)
# define ETHTSYN_CFG_ACCESS_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types.h"
# include "EthTSyn_Lcfg.h"
# include "EthTSyn_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if !defined (ETHTSYN_INLINE)
#  define ETHTSYN_INLINE static INLINE
# endif

# define ETHTSYN_INV_TIME_DOMAIN_ID                    ((EthTSyn_DomainNumberType)(255))
# define ETHTSYN_INV_BUF_IDX                           ((EthTSyn_BufferIdxType)(255))

/* ----- Inline function availability ----- */
/* -- EthTSyn_CfgAccess_GetFollowUpDataIdListEntry() -- */
# if (ETHTSYN_FOLLOW_UP_DATA_ID_LIST_SIZE > 0u)
#  define ETHTSYN_INLINE_API_CFG_GET_FUP_DATA_ID_LIST_ENTRY  STD_ON
# else
#  define ETHTSYN_INLINE_API_CFG_GET_FUP_DATA_ID_LIST_ENTRY  STD_OFF
# endif /* EthTSyn_CfgAccess_GetFollowUpDataIdListEntry availability check */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FCT_MACRO */ /* MD_MSR_19.7 */
/* TimeDomain */
# define EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx)     (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].SlavePortCnt)
# define EthTSyn_CfgGetSlavePortIdxPtr(TimeDomainIdx)  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].SlavePortIdxPtr)
# define EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, Idx) \
  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].SlavePortIdxPtr[(Idx)])
# define EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx)    (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].MasterPortCnt)
# define EthTSyn_CfgGetMasterPortIdxPtr(TimeDomainIdx) (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].MasterPortIdxPtr)
# define EthTSyn_CfgGetMasterPortIdx(TimeDomainIdx, Idx) \
  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].MasterPortIdxPtr[(Idx)])
# define ETHTSYN_CFG_GET_SYNCHRONIZED_TIME_BASE_ID(TimeDomainIdx) \
  ((StbM_SynchronizedTimeBaseType)EthTSyn_TimeDomainConfig[(TimeDomainIdx)].SynchronizedTimeBaseId)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_COUNT(TimeDomainIdx) \
  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].OfsTimeDomainCnt)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_PTR(TimeDomainIdx) \
  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].OfsTimeDomainPtr)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_ID(TimeDomainIdx, OfsTimeDomainIdx) \
  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].OfsTimeDomainPtr[(OfsTimeDomainIdx)].OfsTimeDomainId)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_SYNCHRONIZED_TIME_BASE_ID(TimeDomainIdx, OfsTimeDomainIdx) \
  (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].OfsTimeDomainPtr[(OfsTimeDomainIdx)].SynchronizedTimeBaseId)

/* Port */
# define EthTSyn_CfgGetPortRole(PortIdx)               (EthTSyn_PortConfig[(PortIdx)].PortRolePtr->PortRole)
# define EthTSyn_CfgGetTimeDomainId(PortIdx)           (EthTSyn_PortConfig[(PortIdx)].TimeDomainId)
# define ETHTSYN_CFG_GET_PORT_NUMBER(PortIdx)          (EthTSyn_PortConfig[(PortIdx)].PortNumber)
# define ETHTSYN_CFG_GET_ETHTSYN_CTRL_IDX(PortIdx) \
  (EthTSyn_PortConfig[PortIdx].EthTSynCtrlIdx)
# define ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx) \
  (EthTSyn_CtrlConfig[EthTSyn_PortConfig[(PortIdx)].EthTSynCtrlIdx].EthIfCtrlIdx)
# define ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx) \
  (EthTSyn_CfgAccess_GetTimeDomainIdx(EthTSyn_CfgGetTimeDomainId((PortIdx))))
# define ETHTSYN_CFG_GET_SYNCHRONIZTED_TIME_BASE_ID_OF_PORT(PortIdx) \
  (ETHTSYN_CFG_GET_SYNCHRONIZED_TIME_BASE_ID(EthTSyn_CfgAccess_GetTimeDomainIdx(EthTSyn_CfgGetTimeDomainId((PortIdx)))))
# define ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx)  (EthTSyn_PortConfig[(PortIdx)].FramePriority)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_COUNT_OF_PORT(PortIdx) \
  (EthTSyn_TimeDomainConfig[ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT((PortIdx))].OfsTimeDomainCnt)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_POINTER_OF_PORT(PortIdx) \
  (EthTSyn_TimeDomainConfig[ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT((PortIdx))].OfsTimeDomainPtr)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_ID_OF_PORT(PortIdx, OfsTimeDomainIdx) \
  (ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_POINTER_OF_PORT((PortIdx))[OfsTimeDomainIdx].OfsTimeDomainId)
# define ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_SYNCHRONIZED_TIME_BASE_ID_OF_PORT(PortIdx, OfsTimeDomainIdx) \
  (ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_POINTER_OF_PORT((PortIdx))[OfsTimeDomainIdx].SynchronizedTimeBaseId)

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
#  define ETHTSYN_CFG_GET_SWITCH_IDX(EthTSynPortIdx)       (EthTSyn_PortConfig[(EthTSynPortIdx)].SwtInfo.SwitchIdx)
#  define ETHTSYN_CFG_GET_SWITCH_PORT_IDX(EthTSynPortIdx)  (EthTSyn_PortConfig[(EthTSynPortIdx)].SwtInfo.PortIdx)
# else
#  define ETHTSYN_CFG_GET_SWITCH_IDX(PortIdx)       (ETHTSYN_INVALID_ETHIF_SWITCH_IDX)
#  define ETHTSYN_CFG_GET_SWITCH_PORT_IDX(PortIdx)  (ETHTSYN_INVALID_SWITCH_PORT_IDX)
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

# define ETHTSYN_CFG_GET_FOLLOW_UP_MSG_LENGTH_TX(MasterPortIdx) \
  (EthTSyn_MasterPortConfigPtr((MasterPortIdx))->FollowUpMsgLength)

/* Pdelay */
# define ETHTSYN_CFG_GET_PDELAY_IDX(PortIdx)           (EthTSyn_PortConfig[(PortIdx)].PdelayIdx)
# define ETHTSYN_CFG_GET_P_DELAY_CFG(PortIdx) \
  (EthTSyn_PdelayConfig[ETHTSYN_CFG_GET_PDELAY_IDX((PortIdx))])
# define ETHTSYN_CFG_GET_P_DELAY_ACTIVE_PORT_IDX(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_CFG((PortIdx)).ActivePortIdx)
# define ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_CFG((PortIdx)).PdelayInitiatorCfgPtr)
# define ETHTSYN_CFG_GET_P_DELAY_RESPONDER_CFG_PTR(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_CFG((PortIdx)).PdelayResponderCfgPtr)

# define ETHTSYN_CFG_GET_P_DELAY_USE_STATIC(PortIdx) (ETHTSYN_CFG_GET_P_DELAY_CFG((PortIdx)).UseStaticPdelay)
# define ETHTSYN_CFG_GET_P_DELAY_INITIAL_NS(PortIdx) (ETHTSYN_CFG_GET_P_DELAY_CFG((PortIdx)).InitialPdelay)
# define ETHTSYN_CFG_GET_P_DELAY_RESP_RESP_FOLLOW_UP_TIMEOUT(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR((PortIdx))->PdelayRespRespFollowUpTimeout)

# define ETHTSYN_CFG_GET_ALWAYS_AS_CAPABLE(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_CFG((PortIdx)).AlwaysAsCapable)

/* Pdelay Initiator */
# define ETHTSYN_CFG_GET_P_DELAY_NEIGHBOR_DELAY_THRESHOLD_NS(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR((PortIdx))->PdelayNeighborDelayThreshold)
# define ETHTSYN_CFG_GET_P_DELAY_ALLOWED_LOST_RESPONSES(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR((PortIdx))->PdelayAllowedLostResponses)
# define ETHTSYN_CFG_GET_P_DELAY_REQ_SEND_INTERVAL_MF_CYCLES(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR(PortIdx)->PdelaySendInterval)
# define ETHTSYN_CFG_GET_P_DELAY_REQ_LOG_MESSAGE_INTERVAL(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR((PortIdx))->PdelayLogMessageInterval)
# define ETHTSYN_CFG_GET_P_DELAY_AVERAGE_WEIGHT(PortIdx) \
  (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR((PortIdx))->PdelayAverageWeight)

/* Slave-Port */
# define EthTSyn_CfgGetEnableAnnounce(SlavePortIdx)    (EthTSyn_SlavePortConfigPtr((SlavePortIdx))->EnableAnnounce)
# define EthTSyn_CfgGetEnableSourcePortIdentityCheck(SlavePortIdx) \
   (EthTSyn_SlavePortConfigPtr((SlavePortIdx))->EnableSrcPortIdentityCheck)
# define EthTSyn_CfgGetMasterPortIdentity(SlavePortIdx) \
   (EthTSyn_SlavePortConfigPtr((SlavePortIdx))->MasterPortIdentity)

/* Master-Port */
# define ETHTSYN_CFG_GET_SYNC_TX_INTERVAL_CNT(MasterPortIdx) \
  (EthTSyn_MasterPortConfigPtr((MasterPortIdx))->SyncSendInterval)

/* Configuration access Macros only available in Bridge configuration */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/* TimeDomain */
# define EthTSyn_CfgGetSyncTimeout(TimeDomainIdx)         (EthTSyn_TimeDomainConfig[(TimeDomainIdx)].SyncTimeout)
# define EthTSyn_CfgGetKeepSrcPortIdentity(TimeDomainIdx) (EthTSyn_TimeDomainConfig[ \
                                                            (TimeDomainIdx)].KeepSrcPortIdentity)
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

# define EthTSyn_MasterPortConfigPtr(PortIdx) \
   ((ETHTSYN_P2CONSTCFG(EthTSyn_MasterPortConfigType))&EthTSyn_PortConfig[(PortIdx)].PortRolePtr->PortRole)
# define EthTSyn_SlavePortConfigPtr(PortIdx) \
   ((ETHTSYN_P2CONSTCFG(EthTSyn_SlavePortConfigType))&EthTSyn_PortConfig[(PortIdx)].PortRolePtr->PortRole)

/* Tx FollowUp Ar Sub-Tlv Used */
# define ETHTSYN_CFG_GET_AR_SUB_TLV_TIME_USED(PortIdx)       (EthTSyn_MasterPortConfigPtr((PortIdx))->TimeSubTlv)
# define ETHTSYN_CFG_GET_AR_SUB_TLV_STATUS_USED(PortIdx)     (EthTSyn_MasterPortConfigPtr((PortIdx))->StatusSubTlv)
# define ETHTSYN_CFG_GET_AR_SUB_TLV_USER_DATA_USED(PortIdx)  (EthTSyn_MasterPortConfigPtr((PortIdx))->UserDataSubTlv)
# define ETHTSYN_CFG_GET_AR_SUB_TLV_OFS_USED(PortIdx)        (EthTSyn_MasterPortConfigPtr((PortIdx))->OfsSubTlv)

# define ETHTSYN_CFG_GET_TX_CRC_SECURED(PortIdx)             (EthTSyn_MasterPortConfigPtr((PortIdx))->TxCrcSecured)
# define ETHTSYN_CFG_GET_TX_CRC_SECURED_FLAGS(PortIdx) \
  (EthTSyn_MasterPortConfigPtr((PortIdx))->CrcTimeFlagsTxSecured)

/* Rx FollowUp Crc */
# define ETHTSYN_CFG_GET_RX_CRC_VALIDATED(PortIdx)       (EthTSyn_SlavePortConfigPtr((PortIdx))->RxCrcValidated)
# define ETHTSYN_CFG_GET_RX_CRC_VALIDATED_FLAGS(PortIdx) (EthTSyn_SlavePortConfigPtr((PortIdx))->CrcFlagsRxValidated)

/* PRQA L: FCT_MACRO */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetTimeDomainIdx
 *********************************************************************************************************************/
/*! \brief      Get the TimeDomain index of the given TimeDomainNumber
 *  \param[in]  TimeDomainNumber:      Number of the TimeDomain the index should be retrieved for
 *  \return     ETHTSYN_INVALID_TIME_DOMAIN_IDX - If no corresponding TimeDomain was found
 *  \return     other values - Time domain index
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(EthTSyn_TimeDomainIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetTimeDomainIdx(
  EthTSyn_DomainNumberType TimeDomainNumber);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetTimeDomainIdx
 *********************************************************************************************************************/
/*! \brief      Get the TimeDomain index of the given Port
 *  \param[in]  PortIdx:  EthTSyn port index
 *  \return     ETHTSYN_INVALID_TIME_DOMAIN_IDX - If no corresponding TimeDomain was found
 *  \return     other values - Time domain index
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(EthTSyn_TimeDomainIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetTimeDomainIdxOfPort(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetFollowUpDataIdListEntry
 *********************************************************************************************************************/
/*! \brief      Get the value of the FollowUp Data Id list for the given FollowUp Sequence Id
 *  \param[in]  TimeDomainIdx:   Index of the TimeDomain the DataId is requested for
 *  \param[in]  SequenceId:      The SequenceId
 *  \return     The value of the FollowUp Data Id list for the given Sequence Id
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_INLINE_API_CFG_GET_FUP_DATA_ID_LIST_ENTRY == STD_ON)
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CfgAccess_GetFollowUpDataIdListEntry(
  EthTSyn_TimeDomainIdxType TimeDomainIdx,
  uint16                    SequenceId);
# endif /* (ETHTSYN_INLINE_API_CFG_GET_FUP_DATA_ID_LIST_ENTRY == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsArTlvTxRequired
 *********************************************************************************************************************/
/*! \brief      Check if an Ar Tlv has to be appended to the FollowUp message transmitted on the given Port
 *  \param[in]  PortIdx:   Index of the Port the FollowUp message is transmitted on
 *  \return     TRUE - Ar Tlv required
 *  \return     FALSE - Ar Tlv not required
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsArTlvTxRequired(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetPdelayIndex
 *********************************************************************************************************************/
/*! \brief      Get the Pdelay index for the given port
 *  \param[in]  PortIdx:   Index of the Port
 *  \return     ETHTSYN_P_DELAY_CONFIG_COUNT - no Pdelay index found (e.g. invalid port index passed)
 *  \return     other values - the Pdelay index of the given port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(EthTSyn_PdelayIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetPdelayIndex(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetPdelayInitiatorCfgPtr
 *********************************************************************************************************************/
/*! \brief      Get the Pdelay initiator config pointer for the given port.
 *  \param[in]  PortIdx:   Index of the Port
 *  \return     NULL_PTR - no Pdelay initiator found
 *  \return     other values - pointer to the Pdelay initiator configuration of the given port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(ETHTSYN_P2CONSTCFG(EthTSyn_PdelayInitiatorCfgType), ETHTSYN_CODE)
  EthTSyn_CfgAccess_GetPdelayInitiatorCfgPtr(
      EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetPdelayResponderCfgPtr
 *********************************************************************************************************************/
/*! \brief      Get the Pdelay responder config pointer for the given port.
 *  \param[in]  PortIdx:   Index of the Port
 *  \return     NULL_PTR - no Pdelay responder found
 *  \return     other values - pointer to the Pdelay responder configuration of the given port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(ETHTSYN_P2CONSTCFG(EthTSyn_PdelayResponderCfgType), ETHTSYN_CODE)
  EthTSyn_CfgAccess_GetPdelayResponderCfgPtr(
      EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsPdelayInitiator
 *********************************************************************************************************************/
/*! \brief      Check if the given Port is a Pdelay initiator
 *  \param[in]  PortIdx:   Index of the Port
 *  \return     TRUE - Port is Pdelay initiator
 *  \return     FALSE - Port is no Pdelay initiator
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsPdelayInitiator(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsPdelayResponder
 *********************************************************************************************************************/
/*! \brief      Check if the given Port is a Pdelay responder
 *  \param[in]  PortIdx:   Index of the Port
 *  \return     TRUE - Port is Pdelay responder
 *  \return     FALSE - Port is no Pdelay responder
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsPdelayResponder(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsActivePdelayPort
 *********************************************************************************************************************/
/*! \brief      Check if the given Port is the active Pdelay Port
 *  \param[in]  PortIdx:   Index of the Port
 *  \return     TRUE - Port is the active Pdelay Port
 *  \return     FALSE - Port is not the active Pdelay Port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsActivePdelayPort(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForMasterPort
 *********************************************************************************************************************/
/*! \brief      Check if immediate time sync is active for the given master port
 *  \param[in]  MasterPortIdx:   Index of the master port
 *  \return     TRUE - Immediate TimeSync is activated for the given master port
 *  \return     FALSE - Immediate TimeSync is not activated for the given master port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForMasterPort(
  EthTSyn_PortIdxType MasterPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForTimeDomain
 *********************************************************************************************************************/
/*! \brief      Check if immediate time sync is active for the given time domain
 *  \param[in]  TimeDomainIdx:   Index of the TimeDomain
 *  \return     TRUE - Immediate TimeSync is activated for the given TimeDomain
 *  \return     FALSE - Immediate TimeSync is not activated for the given TimeDomain
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForTimeDomain(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetMasterPortCount
 *********************************************************************************************************************/
/*! \brief      Get the amount of configured master ports for the given time domain
 *  \param[in]  TimeDomainIdx:   Index of the time domain
 *  \return     Amount of master ports for the given time domain
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CfgAccess_GetMasterPortCount(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsGmCapable
 *********************************************************************************************************************/
/*! \brief      Check if node is GrandMaster capable
 *  \return     TRUE - Node is GrandMaster capable
 *  \return     FALSE - Node is not GrandMaster capable
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsGmCapable(void);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsMessageComplianceEnabled
 *********************************************************************************************************************/
/*! \brief      Check if message compliance is enabled
 *  \return     TRUE - message compliance is enabled
 *  \return     FALSE - message compliance is disabled
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsMessageComplianceEnabled(void);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsCrcEnabled
 *********************************************************************************************************************/
/*! \brief      Check if Crc is enabled
 *  \return     TRUE - Crc is enabled
 *  \return     FALSE - Crce is disabled
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsCrcEnabled(void);

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl
 *********************************************************************************************************************/
/*! \brief      Get the EthTSyn controller corresponding to the given EthIf controller
 *  \param[in]  EthIfCtrlIdx  EthIf controller index.
 *  \return     ETHTSYN_CTRL_COUNT - no matching EthTSyn controller found
 *  \return     other values - index of the corresponding EthTSyn controller
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(EthTSyn_CtrlIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(
  uint8 EthIfCtrlIdx);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# ifndef ETHTSYN_UNIT_TEST /* COV_ETHTSYN_TESTSUITE_INSTRUMENTATION */
/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetTimeDomainIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(EthTSyn_TimeDomainIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetTimeDomainIdx(
  EthTSyn_DomainNumberType TimeDomainNumber)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_INVALID_TIME_DOMAIN_IDX;
  EthTSyn_TimeDomainIdxIterType timeDomainCounter;

  /* ----- Implementation ----------------------------------------------- */
  for(timeDomainCounter = 0; timeDomainCounter < ETHTSYN_TIME_DOMAIN_COUNT; timeDomainCounter++)
  {
    if(EthTSyn_TimeDomainConfig[timeDomainCounter].TimeDomainId == TimeDomainNumber)
    {
      timeDomainIdx = (EthTSyn_TimeDomainIdxType)timeDomainCounter;
      break;
    }
  }

  return timeDomainIdx;
} /* EthTSyn_CfgAccess_GetTimeDomainIdx() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetTimeDomainIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(EthTSyn_TimeDomainIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetTimeDomainIdxOfPort(
  EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_INVALID_TIME_DOMAIN_IDX;

  /* ----- Implementation ----------------------------------------------- */
  if(PortIdx < ETHTSYN_PORT_COUNT)
  {
    timeDomainIdx = EthTSyn_CfgAccess_GetTimeDomainIdx(EthTSyn_PortConfig[PortIdx].TimeDomainId);
  }

  return timeDomainIdx;
} /* EthTSyn_CfgAccess_GetTimeDomainIdxOfPort() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetFollowUpDataIdListEntry
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
#  if (ETHTSYN_INLINE_API_CFG_GET_FUP_DATA_ID_LIST_ENTRY == STD_ON)
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CfgAccess_GetFollowUpDataIdListEntry(
  EthTSyn_TimeDomainIdxType TimeDomainIdx, uint16 SequenceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 dataListIdx = (uint8)(SequenceId % ETHTSYN_FOLLOW_UP_DATA_ID_LIST_SIZE);
  uint8 dataListEntry = 0;

  /* ----- Implementation ----------------------------------------------- */
  if(EthTSyn_TimeDomainConfig[TimeDomainIdx].FollowUpDataIdListPtr != NULL_PTR)
  {
    dataListEntry = EthTSyn_TimeDomainConfig[TimeDomainIdx].FollowUpDataIdListPtr[dataListIdx];
  }
  return dataListEntry;
} /* EthTSyn_CfgAccess_GetFollowUpDataIdListEntry() */
#  endif /* (ETHTSYN_INLINE_API_CFG_GET_FUP_DATA_ID_LIST_ENTRY == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsArTlvTxRequired
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsArTlvTxRequired(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean arTlvRequired = FALSE;
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2CONST(EthTSyn_MasterPortConfigType) masterPortCfgPtr = EthTSyn_MasterPortConfigPtr(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  if((masterPortCfgPtr->TimeSubTlv == TRUE) ||
      (masterPortCfgPtr->StatusSubTlv == TRUE) ||
      (masterPortCfgPtr->UserDataSubTlv == TRUE) ||
      (masterPortCfgPtr->OfsSubTlv == TRUE))
  {
    arTlvRequired = TRUE;
  }

  return arTlvRequired;
} /* EthTSyn_CfgAccess_IsArTlvTxRequired() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetPdelayIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(EthTSyn_PdelayIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetPdelayIndex(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdelayIdxType pDelayIdx = ETHTSYN_P_DELAY_CONFIG_COUNT;

  /* ----- Implementation ----------------------------------------------- */
  if(PortIdx < ETHTSYN_PORT_COUNT)
  {
    pDelayIdx = EthTSyn_PortConfig[PortIdx].PdelayIdx;
  }

  return pDelayIdx;
} /* EthTSyn_CfgAccess_GetPdelayIndex() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetPdelayInitiatorCfgPtr
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(ETHTSYN_P2CONSTCFG(EthTSyn_PdelayInitiatorCfgType), ETHTSYN_CODE)
  EthTSyn_CfgAccess_GetPdelayInitiatorCfgPtr(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONSTCFG(EthTSyn_PdelayInitiatorCfgType) pDelayInitiatorCfgPtr = NULL_PTR;
  EthTSyn_PdelayIdxType pDelayIdx;

  /* ----- Implementation ----------------------------------------------- */
  pDelayIdx = EthTSyn_CfgAccess_GetPdelayIndex(PortIdx);
  if(pDelayIdx < ETHTSYN_P_DELAY_CONFIG_COUNT)
  {
    pDelayInitiatorCfgPtr = EthTSyn_PdelayConfig[pDelayIdx].PdelayInitiatorCfgPtr;
  }

  return pDelayInitiatorCfgPtr;
} /* EthTSyn_CfgAccess_GetPdelayInitiatorCfgPtr() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetPdelayResponderCfgPtr
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(ETHTSYN_P2CONSTCFG(EthTSyn_PdelayResponderCfgType), ETHTSYN_CODE)
  EthTSyn_CfgAccess_GetPdelayResponderCfgPtr(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONSTCFG(EthTSyn_PdelayResponderCfgType) pDelayResponderCfgPtr = NULL_PTR;
  EthTSyn_PdelayIdxType pDelayIdx;

  /* ----- Implementation ----------------------------------------------- */
  pDelayIdx = EthTSyn_CfgAccess_GetPdelayIndex(PortIdx);
  if(pDelayIdx < ETHTSYN_P_DELAY_CONFIG_COUNT)
  {
    pDelayResponderCfgPtr = EthTSyn_PdelayConfig[pDelayIdx].PdelayResponderCfgPtr;
  }

  return pDelayResponderCfgPtr;
} /* EthTSyn_CfgAccess_GetPdelayResponderCfgPtr() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsPdelayInitiator
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsPdelayInitiator(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isPdelayInitiator = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Check for Valid Pdelay initiator config pointer for the given Port */
  if(EthTSyn_CfgAccess_GetPdelayInitiatorCfgPtr(PortIdx) != NULL_PTR)
  {
    isPdelayInitiator = TRUE;
  }

  return isPdelayInitiator;
} /* EthTSyn_CfgAccess_IsPdelayInitiator() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsPdelayResponder
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsPdelayResponder(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isPdelayResponder = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Check for Valid Pdelay responder config pointer for the given Port */
  if(EthTSyn_CfgAccess_GetPdelayResponderCfgPtr(PortIdx) != NULL_PTR)
  {
    isPdelayResponder = TRUE;
  }

  return isPdelayResponder;
} /* EthTSyn_CfgAccess_IsPdelayResponder() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsActivePdelayPort
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsActivePdelayPort(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isActivePdelayPort = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_P_DELAY_CONFIG_COUNT <= 1u)
  ETHTSYN_DUMMY_STATEMENT(PortIdx);
#  else
  if(ETHTSYN_CFG_GET_PDELAY_IDX(PortIdx) < ETHTSYN_P_DELAY_CONFIG_COUNT)
#  endif /* (ETHTSYN_P_DELAY_CONFIG_COUNT <= 1u) */
  {
    if(ETHTSYN_CFG_GET_P_DELAY_CFG(PortIdx).ActivePortIdx == PortIdx)
    {
      isActivePdelayPort = TRUE;
    }
  }

  return isActivePdelayPort;
} /* EthTSyn_CfgAccess_IsActivePdelayPort() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForMasterPort
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForMasterPort(
  EthTSyn_PortIdxType MasterPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isImmediateTimeSyncActive = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if(MasterPortIdx < ETHTSYN_PORT_COUNT)
  {
    if(EthTSyn_CfgGetPortRole(MasterPortIdx) == ETHTSYN_PORT_ROLE_MASTER)
    {
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      isImmediateTimeSyncActive = EthTSyn_MasterPortConfigPtr(MasterPortIdx)->ImmediateTimeSync;
    }
  }

  return isImmediateTimeSyncActive;
} /* EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForMasterPort() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForTimeDomain
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForTimeDomain(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isImmediateTimeSyncActive = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if(TimeDomainIdx < ETHTSYN_TIME_DOMAIN_COUNT)
  {
    isImmediateTimeSyncActive = EthTSyn_TimeDomainConfig[TimeDomainIdx].ImmediateTimeSync;
  }

  return isImmediateTimeSyncActive;
} /* EthTSyn_CfgAccess_IsImmediateTimeSyncActiveForTimeDomain() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_GetMasterPortCount
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_CfgAccess_GetMasterPortCount(EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 masterPortCnt = 0;

  /* ----- Implementation ----------------------------------------------- */
  if(TimeDomainIdx < ETHTSYN_TIME_DOMAIN_COUNT)
  {
    masterPortCnt = EthTSyn_TimeDomainConfig[TimeDomainIdx].MasterPortCnt;
  }

  return masterPortCnt;
} /* EthTSyn_CfgAccess_GetMasterPortCount() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsGmCapable
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsGmCapable(void)
{
  /* ----- Implementation ----------------------------------------------- */
  return (boolean)(ETHTSYN_SWT_MGMT_IS_GM_CAPABLE == STD_ON);
} /* EthTSyn_CfgAccess_IsGmCapable() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsMessageComplianceEnabled
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsMessageComplianceEnabled(void)
{
  /* ----- Implementation ----------------------------------------------- */
  return (boolean)(ETHTSYN_MESSAGE_COMPLIANCE == STD_ON);
} /* EthTSyn_CfgAccess_IsMessageComplianceEnabled() */

/**********************************************************************************************************************
 *  EthTSyn_CfgAccess_IsCrcEnabled
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_CfgAccess_IsCrcEnabled(void)
{
  /* ----- Implementation ----------------------------------------------- */
  return (boolean)(EthTSyn_Crc_CalculateCrc8H2FFctPtr != NULL_PTR);
} /* EthTSyn_CfgAccess_IsCrcEnabled() */

/***********************************************************************************************************************
 * EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(EthTSyn_CtrlIdxType, ETHTSYN_CODE) EthTSyn_CfgAccess_GetEthTSynCtrlFromEthIfCtrl(uint8 EthIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_CtrlIdxType ethTSynCtrlIdx = ETHTSYN_CTRL_COUNT;
  EthTSyn_CtrlIdxType ethTSynCtrlIter;

  /* ----- Implementation ----------------------------------------------- */
  for(ethTSynCtrlIter = 0; ethTSynCtrlIter < ETHTSYN_CTRL_COUNT; ethTSynCtrlIter++)
  {
    if(EthTSyn_CtrlConfig[ethTSynCtrlIter].EthIfCtrlIdx == EthIfCtrlIdx)
    {
      ethTSynCtrlIdx = ethTSynCtrlIter;
      break;
    }
  }

  return ethTSynCtrlIdx;
} /* EthTSyn_GetEthTSynCtrlFromEthIfCtrl */
# endif /* ETHTSYN_UNIT_TEST */

# define ETHTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* ETHTSYN_CFG_ACCESS_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_CfgAccess_Int.h
 *********************************************************************************************************************/
