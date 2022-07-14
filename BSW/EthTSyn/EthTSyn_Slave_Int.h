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
 *         \file  EthTSyn_Slave_Int.h
 *        \brief  EthTSyn Slave internal header file
 *
 *      \details  Contains internal definitions and declarations used by EthTSyn (Slave role) only
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

#if !defined(ETHTSYN_SLAVE_INT_H)
# define ETHTSYN_SLAVE_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if !defined(ETHTSYN_INLINE)
#  define ETHTSYN_INLINE static INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

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
 *  EthTSyn_Slave_ProcessSmSyncReceive
 *********************************************************************************************************************/
/*! \brief      Handles the processing of the Sync Receive state-machine for the given TimeDomain
 *  \param[in]  PortIdx    Index of the Port which state-machine should be processed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_ProcessSmSyncReceive(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Slave_ProcessSmAnnounceReceive
 *********************************************************************************************************************/
/*! \brief      Processing of the Announce receive state-machine for the given Port
 *  \param[in]  PortIdx    Index of the port that should be processed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_ProcessSmAnnounceReceive(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Slave_HandleCyclicTasks
 *********************************************************************************************************************/
/*! \brief      Handles the cyclic tasks of each Slave-Port of the given TimeDomain
 *  \param[in]  TimeDomainIdx    Index of the TimeDomain
 *  \context    EthTSyn_MainFunction()
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_HandleCyclicTasks(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcessReceivedSyncMsg
 *********************************************************************************************************************/
/*! \brief      Processing of a received Sync message
 *  \param[in]  ComMsgPtr                  Pointer to the common message header
 *  \param[in]  PortIdx                    Index of the Port the message was received on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcessReceivedSyncMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                       EthTSyn_PortIdxType       PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg
 *********************************************************************************************************************/
/*! \brief      Processing of a received FollowUp message
 *  \param[in]  ComMsgPtr                  Pointer to the common message header
 *  \param[in]  PortIdx                    Index of the Port the message was received on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                       EthTSyn_PortIdxType       PortIdx); /* PRQA S 0777 */ /* MD_MSR_5.1_777 */

/**********************************************************************************************************************
 *  EthTSyn_Slave_ProcessReceivedAnnounceMsg
 *********************************************************************************************************************/
/*! \brief      Processing of a received Announce message
 *  \param[in]  ComMsgPtr       Pointer to the common message header
 *  \param[in]  PortIdx         Index of the Port the message was received on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_ProcessReceivedAnnounceMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                       EthTSyn_PortIdxType       PortIdx);

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_EndStation_Slave_ProcessReceivedSync
 *********************************************************************************************************************/
/*! \brief      Processing of a received Sync message on an End-Station port
 *  \param[in]  PortIdx          Index of the port the message was received on
 *  \param[in]  ComMsgHdrPtr     Pointer to the common PTP message header
 *  \param[in]  RxFrameInfoPtr   Pointer to the Rx info of the received message
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_EndStation_Slave_ProcessReceivedSync(
                  EthTSyn_PortIdxType              PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)        ComMsgHdrPtr,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_RxFrameInfoType) RxFrameInfoPtr);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL INLINE FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_SetMasterPriorityVector
 *********************************************************************************************************************/
/*! \brief      Sets the EthTSyn system state of the master priority vector for the given TimeDomain
 *  \param[in]  AnnounceMsgPtr    Pointer to the Announce message where the master priority vector is stored
 *  \param[in]  TimeDomainIdx     Index of the TimeDomain the master priority should be set for
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SetMasterPriorityVector(
  ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType)  AnnounceMsgPtr,
                  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_ResetMasterPriorityVector
 *********************************************************************************************************************/
/*! \brief      Deletes the EthTSyn system state of the master priority vector for the given TimeDomain
 *  \param[in]  TimeDomainIdx  Index of the TimeDomain the master priority should be reseted for
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ResetMasterPriorityVector(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  GLOBAL INLINE FUNCTIONS
 *********************************************************************************************************************/

# if !defined(ETHTSYN_UNIT_TEST) /* COV_ETHTSYN_TESTSUITE_INSTRUMENTATION */
/**********************************************************************************************************************
 *  EthTSyn_SetMasterPriorityVector
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SetMasterPriorityVector(
  ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType) AnnounceMsgPtr, EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  CANOE_WRITE_STRING("EthTSyn_SetMasterPriorityVector: Set new Grandmaster");
  EthTSyn_SetClockIdentity( EthTSyn_SystemState[TimeDomainIdx].GmPriority.PortIdentity.ClockIdentity,
    AnnounceMsgPtr->ClockIdentity);
  EthTSyn_SystemState[TimeDomainIdx].GmPresent = TRUE;
} /* EthTSyn_SetMasterPriorityVector() */

/**********************************************************************************************************************
 *  EthTSyn_ResetMasterPriorityVector
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ResetMasterPriorityVector(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  IpBase_Fill( (ETHTSYN_P2VAR(IpBase_CopyDataType))
    EthTSyn_SystemState[TimeDomainIdx].GmPriority.PortIdentity.ClockIdentity, 0, sizeof(EthTSyn_ClockIdentityType));
  EthTSyn_SystemState[TimeDomainIdx].GmPresent = FALSE;
} /* EthTSyn_ResetMasterPriorityVector() */
# endif /* !defined(ETHTSYN_UNIT_TEST) */

# define ETHTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* ETHTSYN_SLAVE_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Slave_Int.h
 *********************************************************************************************************************/
