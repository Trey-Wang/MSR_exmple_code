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
/**        \file  EthTSyn_SwtMgmt.c
 *        \brief  EthTSyn Switch Management source file
 *
 *      \details  EthTSyn Switch Management source file containing the EthTSyn Switch Management implementation of the
 *                EthTSyn module.
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

#define ETHTSYN_SWTMGMT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_SwtMgmt_Int.h"
#include "EthTSyn_SwtMgmt_Cbk.h"

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
# include "EthTSyn_Int.h" /* PRQA S 0883, 3219 */ /* MD_EthTSyn_19.15, MD_MSR_14.1_3219 */
# include "EthTSyn_Util_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
# include "EthTSyn_Slave_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
# include "EthTSyn_Master_Int.h"
# include "EthTSyn_Pdelay_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of EthTSyn header file */
# if (  (ETHTSYN_SW_MAJOR_VERSION != (5u)) \
     || (ETHTSYN_SW_MINOR_VERSION != (1u)) \
     || (ETHTSYN_SW_PATCH_VERSION != (1u)) )
#  error "Vendor specific version numbers of EthTSyn_SwtMgmt.c and EthTSyn.h are inconsistent"
# endif

/* Check the version of the configuration header file */
# if (  (ETHTSYN_CFG_MAJOR_VERSION != (5u)) \
     || (ETHTSYN_CFG_MINOR_VERSION != (1u)) )
#  error "Version numbers of EthTSyn_SwtMgmt.c and EthTSyn_Cfg.h are inconsistent!"
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
/* PRQA S 3453 3 */ /* MD_MSR_19.7 */
#  define ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MSG_BUFFER(MsgBufferPtr)  ((MsgBufferPtr)->SwtMgmtInfo.FrameId)
#  define ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr)    ((MgmtInfoPtr)->FrameId)
# else
#  define ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MSG_BUFFER(MsgBufferPtr)  ETHTSYN_INVALID_FRAME_ID
#  define ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr)    ETHTSYN_INVALID_FRAME_ID
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
# if ( (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) && (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) )
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#  define ETHTSYN_SWT_MGMT_FRAME_ID_FROM_SM(SmPtr)                 ((SmPtr)->FrameId)
# else
#  define ETHTSYN_SWT_MGMT_FRAME_ID_FROM_SM(SmPtr)                 ETHTSYN_INVALID_FRAME_ID
#endif /* ( (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) && (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) ) */

/* PRQA S 3453 8 */ /* MD_MSR_19.7 */
# define ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_REQUIRED_VALID(TimestampState) \
  (((TimestampState) & ETHTSYN_TIMESTAMP_STATE_REQUIRED_FLAG_VALID_MASK) != 0)
# define ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_REQUIRED(TimestampState) \
  (((TimestampState) & ETHTSYN_TIMESTAMP_STATE_REQUIRED_MASK) != 0)
# define ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE_VALID(TimestampState) \
  (((TimestampState) & ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK) != 0)
# define ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE(TimestampState) \
  (((TimestampState) & ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK) != 0)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if !defined (ETHTSYN_LOCAL)
#  define ETHTSYN_LOCAL static
# endif

# if !defined (ETHTSYN_LOCAL_INLINE)
#  define ETHTSYN_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

ETHTSYN_LOCAL VAR(EthTSyn_SwtMgmt_MsgBufferType, ETHTSYN_VAR_NOINIT)EthTSyn_SwtMgmt_MsgBuffer[
                                                                          ETHTSYN_SWT_MGMT_MSG_BUFFER_COUNT];

# define ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define ETHTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_StoreSwitchEgressTimestamp
 *********************************************************************************************************************/
/*! \brief      Checks if the Timestamp belongs to an received or transmitted message, stores it and triggers possible
 *              further processing
 *  \param[in]  EthIfCtrlIdx   Index of the Ethernet controller within the context of the Ethernet Interface
 *  \param[in]  DataPtr        Buffer Pointer to map the Timestamp indication to the received/transmitted frame
 *  \param[in]  MgmtInfoPtr    Management information
 *  \param[in]  TimeStampPtr   Current timestamp
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_StoreSwitchEgressTimestamp(
                  uint8                EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)               DataPtr,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr,
  ETHTSYN_P2CONST(Eth_TimeStampType)   TimeStampPtr);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_StoreSwitchIngressTimestamp
 *********************************************************************************************************************/
/*! \brief      Checks if the Timestamp belongs to an received or transmitted message, stores it and triggers possible
 *              further processing
 *  \param[in]  EthIfCtrlIdx   Index of the Ethernet controller within the context of the Ethernet Interface
 *  \param[in]  DataPtr        Buffer Pointer to map the Timestamp indication to the received/transmitted frame
 *  \param[in]  MgmtInfoPtr    Management information
 *  \param[in]  TimeStampPtr   Current timestamp
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_StoreSwitchIngressTimestamp(
                  uint8                EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)               DataPtr,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr,
  ETHTSYN_P2CONST(Eth_TimeStampType)   TimeStampPtr);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_StoreSwitchMgmtInfo
 *********************************************************************************************************************/
/*! \brief      Gets the EthTSyn message buffer which belongs to the given Switch management information and stores the
 *              information and triggers the processing of the message buffer if applicable
 *  \param[in]  EthIfCtrlIdx   Index of the Ethernet controller within the context of the Ethernet Interface
 *  \param[in]  DataPtr        Buffer Pointer to map the Switch management info to the received frame
 *  \param[in]  MgmtInfoPtr    Management information
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_StoreSwitchMgmtInfo(
                  uint8                EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)               DataPtr,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ProcessMsgBuffer
 *********************************************************************************************************************/
/*! \brief          Processing of a received message where all required information are present
 *  \param[in,out]  MsgBufferPtr  In:  Pointer to the message buffer to be processed
 *                                Out: Adopted message buffer state/information
 *  \context        ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ProcessMsgBuffer(
  ETHTSYN_CONSTP2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_IsMsgBfferReadyForProcessing
 *********************************************************************************************************************/
/*! \brief      Checks if the given message buffer is ready for processing
 *  \param[in]  MsgBufferPtr   Pointer to the message buffer to be checked
 *  \return     TRUE - Message buffer is ready for processing
 *  \return     FALSE - Message buffer is not ready for processing
 *  \context    EthTSyn_SwtMgmtRx_ProcessMsgBuffer()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_IsMsgBfferReadyForProcessing(
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetPortForMsgBuffer
 *********************************************************************************************************************/
/*! \brief      Retrieves the EthTSyn Port which matches the information of the buffered message
 *  \param[in]  MsgBufferPtr   Pointer to the message buffer
 *  \return     ETHTSYN_INVALID_PORT_IDX - no matching port was found
 *  \return     other values - the matching EthTSyn Port index
 *  \context    EthTSyn_SwtMgmtRx_ProcessMsgBuffer()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_GetPortForMsgBuffer(
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ProcessReceivedMsg
 *********************************************************************************************************************/
/*! \brief          Handles the processing of a received message (switch management)
 *  \param[in]      PortIdx        Index of the port the message was received on
 *  \param[in,out]  MsgBufferPtr  In:  Pointer to the message buffer
 *                                Out: Modified state of the message buffer and message type dependent values
 *  \context        EthTSyn_SwtMgmtRx_ProcessMsgBuffer()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ProcessReceivedMsg(
                EthTSyn_PortIdxType             PortIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType)  MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ForwardSyncMsgToEndStation
 *********************************************************************************************************************/
/*! \brief      Handles the forwarding of a received Sync message to the End-Station implementation
 *  \param[in]  TimeDomainIdx  Index of the time domain the message was received on
 *  \param[in]  MsgBufferPtr   Pointer to the message buffer
 *  \context    EthTSyn_SwtMgmtRx_ProcessMsgBuffer()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ForwardSyncMsgToEndStation(
                  EthTSyn_TimeDomainIdxType       TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MsgBufferType)  MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ForwardFollowUpMsgToEndStation
 *********************************************************************************************************************/
/*! \brief          Handles the forwarding of a received FollowUp message to the End-Station implementation
 *  \param[in]      PortIdx        Index of the port the message was received on
 *  \param[in]      TimeDomainIdx  Index of the time domain the message was received on
 *  \param[in,out]  MsgBufferPtr  In:  Pointer to the message buffer
 *                                Out: Adopted correction field
 *  \context        EthTSyn_SwtMgmtRx_ProcessMsgBuffer()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ForwardFollowUpMsgToEndStation(
                EthTSyn_PortIdxType             PortIdx,
                EthTSyn_TimeDomainIdxType       TimeDomainIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType)  MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_TriggerStateMachine
 *********************************************************************************************************************/
/*! \brief      Trigger the processing of a specified state machine
 *  \param[in]  StateMachine    Identification of the state machine to be processed
 *  \param[in]  PortIdx         Index of the EthTSyn port the state machine should be processed for.
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_TriggerStateMachine(
  EthTSyn_SwtMgmt_StateMachineIndentificationType StateMachine,
  EthTSyn_PortIdxType                             PortIdx);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedPdelayReq
 *********************************************************************************************************************/
/*! \brief      Processing of a received Pdelay_Req message
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]  MsgBufferPtr:  Pointer to the message buffer to be processed
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedPdelayReq(
                       EthTSyn_PortIdxType            PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedPdelayResp
 *********************************************************************************************************************/
/*! \brief      Processing of a received Pdelay_Resp message
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]  MsgBufferPtr:  Pointer to the message buffer to be processed
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedPdelayResp(
                       EthTSyn_PortIdxType            PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedPdelayRespFollowUp
 *********************************************************************************************************************/
/*! \brief      Processing of a received Pdelay_Resp_FollowUp message
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]  MsgBufferPtr:  Pointer to the message buffer to be processed
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedPdelayRespFollowUp(
                       EthTSyn_PortIdxType            PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedSync
 **********************************************************************************************************************/
/*! \brief      Processing of a received Sync message
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]  TimeDomainIdx: Index of the TimeDomain the message was received on
 *  \param[in]  MsgBufferPtr:  Pointer to the message buffer to be processed
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedSync(
                  EthTSyn_PortIdxType            PortIdx,
                  EthTSyn_TimeDomainIdxType      TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_CheckSyncMsgReception
 **********************************************************************************************************************/
/*! \brief      Checks if a sync message should be received according to the Port and Sync source port identity
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]  TimeDomainIdx: Index of the TimeDomain the message was received on
 *  \param[in]  ComMsgPtr:     Pointer to the common message header of the sync message
 *  \return     TRUE - Receive message
 *  \return     FALSE - Drop message
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_CheckSyncMsgReception(
                  EthTSyn_PortIdxType        PortIdx,
                  EthTSyn_TimeDomainIdxType  TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_IsPortReadyToReceiveSync
 **********************************************************************************************************************/
/*! \brief      Checks if the given port is ready to receive a sync message
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \return     TRUE - Port is ready to receive sync message
 *  \return     FALSE - Port is not ready to receive sync message
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_IsPortReadyToReceiveSync(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedFollowUp
 *********************************************************************************************************************/
/*! \brief          Processing of a received FollowUp message
 *  \param[in]      PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]      TimeDomainIdx: Index of the TimeDomain the message was received on
 *  \param[in,out]  MsgBufferPtr:  In:  Pointer to the message buffer to be processed
 *                                 Out: Adopted follow up correction field
 *  \context        ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedFollowUp(
                EthTSyn_PortIdxType            PortIdx,
                EthTSyn_TimeDomainIdxType      TimeDomainIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedAnnounce
 *********************************************************************************************************************/
/*! \brief      Processing of a received Announce message
 *  \param[in]  PortIdx:       Index of the EthTSyn Port the message was received on
 *  \param[in]  MsgBufferPtr:  Pointer to the message buffer to be processed
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedAnnounce(
                       EthTSyn_PortIdxType            PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ReleaseMsgBuffer
 *********************************************************************************************************************/
/*! \brief          Release the specified message buffer
 *  \details        This functions releases the specified message buffer
 *  \param[in,out]  MsgBufferPtr:  In:  Pointer to the message buffer element to be released
 *                                 Out: Information like state set within the message buffer struct
 *  \context        ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ReleaseMsgBuffer(
  ETHTSYN_CONSTP2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_PerformBasicMsgChecksAngGetMsgBuffer
 *********************************************************************************************************************/
/*! \brief       Perform basic message checks and get a free message buffer, prepare and lock it
 *  \param[in]   LenByte          The received message length passed to EthTSyn_RxIndication() from EthIf
 *  \param[in]   DataPtr          Pointer to the Rx-Buffer passed to the EthTSyn_RxIndication from EthIf
 *  \param[out]  MsgBufferPtrPtr  Pointer to the free message buffer element
 *  \param[out]  ErrorIdPtr       If any error occurred which should be reported to the DET, the Error Id will be stored
 *  \return      E_OK - Message checks passed and message buffer available
 *  \return      E_NOT_OK - Message checks failed or no message buffer available
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_PerformBasicMsgChecksAngGetMsgBuffer(
                  uint16                          LenByte,
  ETHTSYN_P2CONST(uint8)                          DataPtr,
    ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType*) MsgBufferPtrPtr,
    ETHTSYN_P2VAR(uint8)                          ErrorIdPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_HandleIngressTimestamp
 *********************************************************************************************************************/
/*! \brief          Handles the ingress timestamping of a received frame
 *  \param[in]      EthIfCtrlIdx  Index of the EthIf controller the message was received on
 *  \param[in]      DataPtr       Pointer to the received message
 *  \param[in,out]  MsgBufferPtr  In:  Pointer to the used message buffer
 *                                Out: Timestamp informations set within the message buffer struct
 *  \context        ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_HandleIngressTimestamp(
                  uint8                           EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)                          DataPtr,
    ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType)  MsgBufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProvideMsgBuffer
 *********************************************************************************************************************/
/*! \brief       Get a free message buffer, prepare and lock it
 *  \param[out]  MsgBufferPtrPtr:    Pointer to the free message buffer element
 *  \return      E_OK - Free message buffer available
 *  \return      E_NOT_OK - No message buffer available
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProvideMsgBuffer(
    ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) *MsgBufferPtrPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetMappedMsgBuffer
 *********************************************************************************************************************/
/*! \brief       Get the message buffer mapped to the given FrameId and Rx-Buffer Address
 *  \param[in]   CtrlIdx:            Index of the EthIf controller the frame was received on
 *  \param[in]   RxBufAddr:          Address of the corresponding Rx-Buffer
 *  \param[in]   FrameId:            The Frame ID of the frame the message buffer is requested for
 *  \param[out]  MsgBufferPtrPtr:    Pointer to the mapped message buffer element
 *  \return      E_OK - Message buffer available
 *  \return      E_NOT_OK - No message buffer mapped
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetMappedMsgBuffer(
                    uint8                           CtrlIdx,
    ETHTSYN_P2CONST(uint8)                          RxBufAddr,
                    EthTSyn_FrameIdType             FrameId,
      ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) *MsgBufferPtrPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetEndStationSlavePortIdx
 *********************************************************************************************************************/
/*! \brief      Get the End-Station Slave Port Index of the given TimeDomain
 *  \param[in]  TimeDomainIdx:      Index of the TimeDomain the slave port should be retrieved for
 *  \return     ETHTSYN_INVALID_PORT_IDX - no end station slave port found
 *  \return     other values - the corresponding slave port index
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetEndStationSlavePortIdx(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetSwitchSlavePortIdx
 *********************************************************************************************************************/
/*! \brief      Get the Switch Slave Port Index of the given TimeDomain
 *  \param[in]  TimeDomainIdx:      Index of the TimeDomain the slave port should be retrieved for
 *  \return     ETHTSYN_INVALID_PORT_IDX - no switch slave port found
 *  \return     other values - the corresponding slave port index
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetSwitchSlavePortIdx(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetIngressTimestamp
 *********************************************************************************************************************/
/*! \brief      Handles the ingress timestamping
 *  \param[in]  EthIfCtrlIdx         Ethernet interface controller index
 *  \param[in]  DataPtr              Pointer to the Rx-Buffer
 *  \param[out] IngressTimestampPtr  The Ingress timestamp
 *  \return     E_OK - Succeeded
 *  \return     E_NOT_OK - Failed
 *  \context    EthTSyn_RxIndication()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetIngressTimestamp(
                  uint8                         EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)                        DataPtr,
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType)  IngressTimestampPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetMappedStateMachine
 *********************************************************************************************************************/
/*! \brief       Get the message buffer mapped to the given FrameId and Rx-Buffer Address
 *  \param[in]   CtrlIdx:          Index of the corresponding EthIf controller
 *  \param[in]   TxBufPtr:        Address of the corresponding Tx-Buffer
 *  \param[in]   FrameId:          The Frame ID of the frame the state machine is requested for
 *  \param[out]  PortIdxPtr:       Pointer to the mapped message buffer element
 *  \return      ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM - no corresponding state machin found
 *  \return      other values - the mapped state machine indication value
 *  \context     ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SwtMgmt_StateMachineIndentificationType, ETHTSYN_CODE)
  EthTSyn_SwtMgmt_GetMappedStateMachine(
                           uint8                      CtrlIdx,
      ETHTSYN_CONSTP2CONST(uint8)                     TxBufPtr,
                           EthTSyn_FrameIdType        FrameId,
             ETHTSYN_P2VAR(EthTSyn_PortIdxType)       PortIdxPtr);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetStateMachineTxTimestampInfo
 *********************************************************************************************************************/
/*! \brief       Get the message buffer mapped to the given FrameId and Rx-Buffer Address
 *  \param[in]   StateMachine:         The State machine 'type' for which the timestamp info is required
 *  \param[in]   PortIdx:              The Port index of the state machine
 *  \param[in]   TimestampDir:         Direction of the Timestamp (ingress/egress)
 *  \param[out]  TimestampPtrPtr:      Pointer to the corresponding Timestamp
 *  \param[out]  TimestampStatePtrPtr: Pointer to the corresponding Timestamp state
 *  \return      E_OK - Successfully retrieved the Timestamp info
 *               E_NOT_OK - Failed to retrieve the Timestamp info
 *  \context     ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetStateMachineTxTimestampInfo(
                  EthTSyn_SwtMgmt_StateMachineIndentificationType  StateMachine,
                  EthTSyn_PortIdxType                              PortIdx,
                  EthTSyn_SwtMgmt_TimestampDirectionType           TimestampDir,
    ETHTSYN_P2VAR(ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType))      TimestampPtrPtr,
    ETHTSYN_P2VAR(ETHTSYN_P2VAR(EthTSyn_TimestampStateType))       TimestampStatePtrPtr);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_TxSyncMsg
 *********************************************************************************************************************/
/*! \brief       Transmission of a Sync message on the given Port
 *  \param[in]   PortIdx:    Index of the Port the Sync message should be transmitted on
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_TxSyncMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_CopySyncMsgFromSiteSyncSyncSmToTxBuffer
 *********************************************************************************************************************/
/*! \brief       Copies the Sync message of the SiteSyncSync state machine to the Ethernet Tx-Buffer and adepts the
 *               Source-Port-Identity (if applicable)
 *  \param[in]   PortIdx:    Index of the Port the Sync message should be transmitted on
 *  \param[out]  BufferPtr:  Pointer to the Ethernet Tx-Buffer. The Sync message will be copied here
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CopySyncMsgFromSiteSyncSyncSmToTxBuffer(
                EthTSyn_PortIdxType  PortIdx,
  ETHTSYN_P2VAR(uint8)               BufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_TxFollowUpMsg
 *********************************************************************************************************************/
/*! \brief       Transmission of a FollowUp message on the given Port
 *  \param[in]   PortIdx:    Index of the Port the FollowUp message should be transmitted on
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_TxFollowUpMsg(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_CopyFollowUpMsgFromSiteSyncSyncSmToTxBuffer
 *********************************************************************************************************************/
/*! \brief       Copies the FollowUp message of the SiteSyncSync state machine to the Ethernet Tx-Buffer and adepts the
 *               Source-Port-Identity (if applicable)
 *  \param[in]   PortIdx:    Index of the Port the FollowUp message should be transmitted on
 *  \param[out]  BufferPtr:  Pointer to the Ethernet Tx-Buffer. The FollowUp message will be copied here
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CopyFollowUpMsgFromSiteSyncSyncSmToTxBuffer(
                EthTSyn_PortIdxType  PortIdx,
  ETHTSYN_P2VAR(uint8)               BufferPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ComputeAndSetFollowUpCorrectionField
 *********************************************************************************************************************/
/*! \brief      Calculate the FollowUp correction field value and set it in the corresponding FollowUp message
 *  \param[in]  PortIdx:    Index of the Port the FollowUp message should be transmitted on
 *  \param[in]  ComMsgPtr:  Pointer to the header of the FollowUp message
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ComputeAndSetFollowUpCorrectionField(
                  EthTSyn_PortIdxType        PortIdx,
    ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType)  ComMsgPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_HandleSendSyncStateOfSiteSyncSyncSm
 *********************************************************************************************************************/
/*! \brief      Handles the state ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_SYNC of the SiteSyncSync state machine
 *  \param[in]  TimeDomainIdx:    Index of the TimeDomain the SiteSyncSync state machine is processed for
 *  \context    EthTSyn_SwtMgmt_ProcessSmSiteSyncSync()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_HandleSendSyncStateOfSiteSyncSyncSm(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_HandleSendFollowUpStateOfSiteSyncSyncSm
 *********************************************************************************************************************/
/*! \brief      Handles the state ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP of the SiteSyncSync state machine
 *  \param[in]  TimeDomainIdx:    Index of the TimeDomain the SiteSyncSync state machine is processed for
 *  \context    EthTSyn_SwtMgmt_ProcessSmSiteSyncSync()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_HandleSendFollowUpStateOfSiteSyncSyncSm(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_HandleReceivedFupAfterTimeoutStateOfSiteSyncSyncSm
 *********************************************************************************************************************/
/*! \brief      Handles the state ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_FUP_AFTER_TIMEOUT of the SiteSyncSync
 *              state machine
 *  \param[in]  TimeDomainIdx:    Index of the TimeDomain the SiteSyncSync state machine is processed for
 *  \context    EthTSyn_SwtMgmt_ProcessSmSiteSyncSync()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_HandleReceivedFupAfterTimeoutStateOfSiteSyncSyncSm(
  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_StoreSwitchEgressTimestamp
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
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_StoreSwitchEgressTimestamp(uint8 EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr,
  ETHTSYN_P2CONST(Eth_TimeStampType) TimeStampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) msgBufferPtr = NULL_PTR;
  ETHTSYN_P2VAR(Eth_TimeStampType) egressTsPtr = NULL_PTR;
  ETHTSYN_P2VAR(EthTSyn_TimestampStateType) egressTsStatePtr = NULL_PTR;
  EthTSyn_SwtMgmt_StateMachineIndentificationType stateMachine = ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM;
  EthTSyn_PortIdxType portIdx = ETHTSYN_INVALID_PORT_IDX;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the timestamp is given for received or transmitted message. */
  if(EthTSyn_SwtMgmt_GetMappedMsgBuffer(
      EthIfCtrlIdx, DataPtr, ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr), &msgBufferPtr) == E_OK)
  {
    if(msgBufferPtr != NULL_PTR)
    {
      egressTsPtr = &msgBufferPtr->EgressTimestampHostPortMgmt.Timestamp;
      egressTsStatePtr = &msgBufferPtr->EgressTimestampHostPortMgmt.TimestampState;
    }
  }
  else
  {
    /* Try to get mapped state machine (Tx) */
    stateMachine = EthTSyn_SwtMgmt_GetMappedStateMachine(
        EthIfCtrlIdx, DataPtr, ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr), &portIdx);
    if(stateMachine != ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM)
    {
      (void)EthTSyn_SwtMgmt_GetStateMachineTxTimestampInfo(stateMachine, portIdx,
          ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_EGRESS, &egressTsPtr, &egressTsStatePtr);
    }
  }

  /* #20 Check if either Rx or Tx message/state machine is available. */
  if( (egressTsPtr != NULL_PTR) && (egressTsStatePtr != NULL_PTR) )
  {
    /* #200 Check if egress timestamp is available. */
    if(TimeStampPtr == NULL_PTR)
    {
      /* #2000 Timestamp is not available -> mark TimeStamp as not available. */
      *egressTsStatePtr |= ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK;
      *egressTsStatePtr &= (EthTSyn_TimestampStateType)~(ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK);
    }
    else
    {
      /* #2001 Timestamp available -> Copy timestamp and mark it as available. */
      *egressTsStatePtr |= (ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK |
          ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK);
      *egressTsPtr = *TimeStampPtr;
    }
  }

  /* #30 Trigger processing of message buffer (received message) or state machine (transmitted message). */
  if(msgBufferPtr != NULL_PTR)
  {
    EthTSyn_SwtMgmtRx_ProcessMsgBuffer(msgBufferPtr);
  }
  else if( (stateMachine  != ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM) &&
      (portIdx       != ETHTSYN_INVALID_PORT_IDX)  )
  {
    EthTSyn_SwtMgmt_TriggerStateMachine(stateMachine, portIdx);
  }
  else
  {
    /* Nothing to do here (MISRA) */
  }
} /* EthTSyn_SwtMgmt_StoreSwitchEgressTimestamp() */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_StoreSwitchIngressTimestamp
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
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_StoreSwitchIngressTimestamp(uint8 EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr,
  ETHTSYN_P2CONST(Eth_TimeStampType) TimeStampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) msgBufferPtr = NULL_PTR;
  ETHTSYN_P2VAR(Eth_TimeStampType) ingressTsPtr = NULL_PTR;
  ETHTSYN_P2VAR(EthTSyn_TimestampStateType) ingressTsStatePtr = NULL_PTR;
  EthTSyn_SwtMgmt_StateMachineIndentificationType stateMachine = ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM;
  EthTSyn_PortIdxType portIdx = ETHTSYN_INVALID_PORT_IDX;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the timestamp is given for received or transmitted message. */
  if(EthTSyn_SwtMgmt_GetMappedMsgBuffer(
      EthIfCtrlIdx, DataPtr, ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr), &msgBufferPtr) == E_OK)
  {
    if(msgBufferPtr != NULL_PTR)
    {
      ingressTsPtr = &msgBufferPtr->IngressTimestampMgmt.Timestamp;
      ingressTsStatePtr = &msgBufferPtr->IngressTimestampMgmt.TimestampState;
    }
  }
  else
  {
    /* Initialize message buffer ptr */
    msgBufferPtr = NULL_PTR;
    /* Try to get mapped state machine (Tx) */
    stateMachine = EthTSyn_SwtMgmt_GetMappedStateMachine(
        EthIfCtrlIdx, DataPtr, ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr), &portIdx);
    if(stateMachine != ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM)
    {
      (void)EthTSyn_SwtMgmt_GetStateMachineTxTimestampInfo(stateMachine, portIdx,
          ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_INGRESS, &ingressTsPtr, &ingressTsStatePtr);
    }
  }

  /* #20 Check if either Rx or Tx message/state machine is available. */
  if( (ingressTsPtr != NULL_PTR) && (ingressTsStatePtr != NULL_PTR) )
  {
    /* #200 Check if egress timestamp is available. */
    if(TimeStampPtr == NULL_PTR)
    {
      /* #2000 Timestamp is not available -> mark TimeStamp as not available */
      *ingressTsStatePtr |= ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK;
      *ingressTsStatePtr &= (EthTSyn_TimestampStateType)~(ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK);
    }
    else
    {
      /* #2001 Timestamp available -> copy timestamp and mark it as available. */
      *ingressTsStatePtr |= (ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK |
          ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK);
      *ingressTsPtr = *TimeStampPtr;
    }
  }

  /* #30 Trigger processing of message buffer (received message) or state machine (transmitted message). */
  if(msgBufferPtr != NULL_PTR)
  {
    EthTSyn_SwtMgmtRx_ProcessMsgBuffer(msgBufferPtr);
  }
  else if( (stateMachine  != ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM) &&
      (portIdx       != ETHTSYN_INVALID_PORT_IDX)  )
  {
    EthTSyn_SwtMgmt_TriggerStateMachine(stateMachine, portIdx);
  }
  else
  {
    /* Nothing to do here (MISRA) */
  }

} /* EthTSyn_SwtMgmt_StoreSwitchIngressTimestamp() */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_StoreSwitchMgmtInfo
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_StoreSwitchMgmtInfo(uint8 EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) msgBufferPtr = NULL_PTR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to get the corresponding message buffer. */
  if(EthTSyn_SwtMgmt_GetMappedMsgBuffer(
      EthIfCtrlIdx, DataPtr, ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MGMT_INFO(MgmtInfoPtr), &msgBufferPtr) == E_OK)
  {
    if(msgBufferPtr != NULL_PTR)
    {
      /* #20 Verify the passed management info. */
      if( (MgmtInfoPtr->SwitchIdx == ETHTSYN_INVALID_ETHIF_SWITCH_IDX) ||
          (MgmtInfoPtr->PortIdx == ETHTSYN_INVALID_SWITCH_PORT_IDX) )
      {
        /* #200 Passed management info invalid -> release message buffer and drop message. */
        EthTSyn_SwtMgmt_ReleaseMsgBuffer(msgBufferPtr);
      }
      else
      {
        /* #201 Passed management info valid -> store switch management information in the message buffer and process
         * the message buffer. */
        msgBufferPtr->SwtMgmtInfo.SwitchIdx = MgmtInfoPtr->SwitchIdx;
        msgBufferPtr->SwtMgmtInfo.PortIdx   = MgmtInfoPtr->PortIdx;

        /* Set message buffer state to port info available. */
        msgBufferPtr->MsgBufferState = ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_PORT_INFO_AVAILABLE;

        EthTSyn_SwtMgmtRx_ProcessMsgBuffer(msgBufferPtr);
      }
    }
  }
} /* EthTSyn_SwtMgmtRx_StoreSwitchMgmtInfo() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ProcessMsgBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ProcessMsgBuffer(
    ETHTSYN_CONSTP2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /*
  * TimeDomainId is known from message
  * TimeDomainIdx can be retrieved using the TimeDomainId
  * The EthTSyn-Port can be found by iterating all Ports of the TimeDomain(Idx)
  * The EthTSyn-Port has to match the CtrlIdx and the SwtPortIdx
  * Within a TimeDomain only one Port for the pair of EthIf-Ctrl and Swt-Port can exist
  */

  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType portIdx;

  /* ----- Implementation --------------------------------------------- */
  /* #10 Check if message is ready for processing. */
  if(EthTSyn_SwtMgmtRx_IsMsgBfferReadyForProcessing(MsgBufferPtr) == TRUE)
  {
    /* #20 Message is ready for processing. Get matching EthTSyn Port. */
    MsgBufferPtr->MsgBufferState = ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_PROCESSING;
    /* #30 Get corresponding EthTSyn Port */
    portIdx = EthTSyn_SwtMgmtRx_GetPortForMsgBuffer(MsgBufferPtr);

    if(portIdx < ETHTSYN_PORT_COUNT)
    {
      /* #40 Valid port found --> process message */
      EthTSyn_SwtMgmtRx_ProcessReceivedMsg(portIdx, MsgBufferPtr);
    }

    /* #50 Release the message buffer. */
    EthTSyn_SwtMgmt_ReleaseMsgBuffer(MsgBufferPtr);
  }
} /* EthTSyn_SwtMgmtRx_ProcessMsgBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_IsMsgBfferReadyForProcessing
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_IsMsgBfferReadyForProcessing(
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isReady = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if timestamp indications were already called. */
  /* PRQA S 3415 3 */ /* MD_EthTSyn_12.4 */
  if( (EthTSyn_IsTimestampReadyToProcess(MsgBufferPtr->IngressTimestampMgmt.TimestampState) == TRUE)
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
      && (EthTSyn_IsTimestampReadyToProcess(MsgBufferPtr->EgressTimestampHostPortMgmt.TimestampState) == TRUE)
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
  )
  {
    /* #20 Check if Port info is available. */
    if(MsgBufferPtr->MsgBufferState == ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_PORT_INFO_AVAILABLE)
    {
      /* #30 Ready to process message. */
      isReady = TRUE;
    }
  }

  return isReady;
} /* EthTSyn_SwtMgmtRx_IsMsgBfferReadyForProcessing() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetPortForMsgBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_GetPortForMsgBuffer(
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType portIdx = ETHTSYN_INVALID_PORT_IDX;
  EthTSyn_TimeDomainIdxType timeDomainIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get and verify the time domain index of the time domain of the message. */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  timeDomainIdx = EthTSyn_CfgAccess_GetTimeDomainIdx(MsgBufferPtr->Msg.MsgHdr.DomainNumber);

  if(timeDomainIdx != ETHTSYN_INVALID_TIME_DOMAIN_IDX)
  {
    /* #20 Check if the message was received on the slave port of the time domain (if present). */
    EthTSyn_PortIdxType slavePortIdx = EthTSyn_SwtMgmt_GetSwitchSlavePortIdx(timeDomainIdx);
    if( (slavePortIdx != ETHTSYN_INVALID_PORT_IDX) &&
        (ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(slavePortIdx) == MsgBufferPtr->CtrlIdx) &&
        (EthTSyn_PortConfig[slavePortIdx].SwtInfo.SwitchIdx ==
            MsgBufferPtr->SwtMgmtInfo.SwitchIdx) &&
            (EthTSyn_PortConfig[slavePortIdx].SwtInfo.PortIdx ==
                MsgBufferPtr->SwtMgmtInfo.PortIdx) )
    {
      portIdx = slavePortIdx;
    }
    else
    {
      /* #30 Check if the message was received on any of the master port of the time domain. */
      EthTSyn_PortIdxIterType masterPortCnt;
      for(masterPortCnt = 0; masterPortCnt < EthTSyn_CfgGetMasterPortCnt(timeDomainIdx); masterPortCnt++)
      {
        if( (ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(EthTSyn_CfgGetMasterPortIdx(timeDomainIdx, masterPortCnt)) ==
            MsgBufferPtr->CtrlIdx) &&
            (EthTSyn_PortConfig[EthTSyn_CfgGetMasterPortIdx(timeDomainIdx, masterPortCnt)].SwtInfo.SwitchIdx ==
                MsgBufferPtr->SwtMgmtInfo.SwitchIdx) &&
                (EthTSyn_PortConfig[EthTSyn_CfgGetMasterPortIdx(timeDomainIdx, masterPortCnt)].SwtInfo.PortIdx ==
                    MsgBufferPtr->SwtMgmtInfo.PortIdx) )
        {
          /* port found */
          portIdx = EthTSyn_CfgGetMasterPortIdx(timeDomainIdx, masterPortCnt);
          break;
        }
      }
    }
  }

  return portIdx;
} /* EthTSyn_SwtMgmtRx_GetPortForMsgBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetPortForMsgBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ProcessReceivedMsg(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx;

  /* ----- Implementation ----------------------------------------------- */
  timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);

  switch(ETHTSYN_HDR_COM_MSG_TYPE(MsgBufferPtr->Msg.MsgHdr.TransSpecific_MessageType))
  {
  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
    /* Process Pdelay_Req message. */
    EthTSyn_SwtMgmt_ProcessReceivedPdelayReq(PortIdx, MsgBufferPtr);
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
    EthTSyn_SwtMgmt_ProcessReceivedPdelayResp(PortIdx, MsgBufferPtr);
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_RES_FOLLOW_UP:
    EthTSyn_SwtMgmt_ProcessReceivedPdelayRespFollowUp(PortIdx, MsgBufferPtr);
    break;

  case ETHTSYN_MSG_TYPE_SYNC:
    EthTSyn_SwtMgmt_ProcessReceivedSync(PortIdx, timeDomainIdx, MsgBufferPtr);
    break;

  case ETHTSYN_MSG_TYPE_FOLLOW_UP:
    EthTSyn_SwtMgmt_ProcessReceivedFollowUp(PortIdx, timeDomainIdx, MsgBufferPtr);
    break;

  case ETHTSYN_MSG_TYPE_ANNOUNCE:
    EthTSyn_SwtMgmt_ProcessReceivedAnnounce(PortIdx, MsgBufferPtr);
    break;

  default:
    /* This should never happen (MISRA) */
    break;
  } /* END: switch(msgType) */
} /* EthTSyn_SwtMgmtRx_ProcessReceivedMsg() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ForwardSyncMsgToEndStation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ForwardSyncMsgToEndStation(
  EthTSyn_TimeDomainIdxType TimeDomainIdx, ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmt_RxFrameInfoType rxFrameInfo;
  EthTSyn_PortIdxType endStationSlavePortIdx = EthTSyn_SwtMgmt_GetEndStationSlavePortIdx(TimeDomainIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the message has to be forwarded to the EthTSyn End-Station. */
  if(endStationSlavePortIdx != ETHTSYN_INVALID_PORT_IDX)
  {
    ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) ingressTimestampPtr = NULL_PTR;
#  if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    if(MsgBufferPtr->IngressTimestampHostCtrlMgmt.TimestampState ==
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE)
    {
      ingressTimestampPtr = (ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType))
                                  &MsgBufferPtr->IngressTimestampHostCtrlMgmt.Timestamp;
    }
#  else
    if(MsgBufferPtr->IngressTimestampMgmt.TimestampState ==
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE)
    {
      ingressTimestampPtr = (ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType))
                                  &MsgBufferPtr->IngressTimestampMgmt.Timestamp;
    }
#  endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    if(ingressTimestampPtr != NULL_PTR)
    {
      /* #20 Pass the message to the End-Station. */
      rxFrameInfo.IngressTimestamp = *ingressTimestampPtr;
      EthTSyn_EndStation_Slave_ProcessReceivedSync(endStationSlavePortIdx, &MsgBufferPtr->Msg.MsgHdr,
          &rxFrameInfo);
    }
  }
} /* EthTSyn_SwtMgmtRx_ForwardSyncMsgToEndStation() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ForwardFollowUpMsgToEndStation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ForwardFollowUpMsgToEndStation(
  EthTSyn_PortIdxType PortIdx, EthTSyn_TimeDomainIdxType TimeDomainIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr) /* PRQA S 3673 */ /* MD_EthTSyn_16.7b */
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType endStationSlavePortIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the message has to be forwarded to the EthTSyn End-Station. */
  endStationSlavePortIdx = EthTSyn_SwtMgmt_GetEndStationSlavePortIdx(TimeDomainIdx);
  if(endStationSlavePortIdx < ETHTSYN_PORT_COUNT)
  {
    /* #20 Adopt the follow up correction field by adding the switch residence time. */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    EthTSyn_TimediffType followUpCorrectionFieldNs = (EthTSyn_TimediffType)
        ETHTSYN_GET_FOLLOW_UP_CORRECTION_VALUE_NS_FROM_MSG(MsgBufferPtr->Msg.MsgHdr.CorrectionField);

    /* PRQA S 3415, 0310 4 */ /* MD_EthTSyn_12.4, MD_EthTSyn_11.4 */
    if((EthTSyn_IsTimestampReadyToProcess(
        ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx)->Sync.IngressTimestampState) == TRUE) &&
        (EthTSyn_IsTimestampReadyToProcess(
            ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx)->Sync.EgressTimestampHostPortState)))
    {
      EthTSyn_TimediffType residenceTime;
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      if(EthTSyn_Util_TimestampMinusTimestamp(
          &ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx)->Sync.EgressTimestampHostPort,
          &ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx)->Sync.IngressTimestamp, &residenceTime) == E_OK)
      { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        followUpCorrectionFieldNs += residenceTime;
      }
    }
    else
    {
      /* Use default residence Time */
      followUpCorrectionFieldNs += (EthTSyn_TimediffType)ETHTSYN_SWT_RX_TO_UPLINK_RESIDENCE_TIME_NS;
    }
    /* PRQA S 0310, 3109 2 */ /* MD_EthTSyn_11.4 */ /* MD_MSR_14_3 */
    EthTSyn_CopyFollowUpCorrectionField((uint64)followUpCorrectionFieldNs,
        MsgBufferPtr->Msg.MsgHdr.CorrectionField);
# else
    ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    /* #30 Pass the message to the end station implementation. */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg((ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))&MsgBufferPtr->Msg,
        endStationSlavePortIdx);
  }
} /* EthTSyn_SwtMgmtRx_ForwardFollowUpMsgToEndStation() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_TriggerStateMachine
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_TriggerStateMachine(
    EthTSyn_SwtMgmt_StateMachineIndentificationType StateMachine, EthTSyn_PortIdxType PortIdx)
{
  switch(StateMachine)
  {
    case ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_RESP_SM:
      EthTSyn_Pdelay_ProcessSmPdelayResp(PortIdx);
      break;

    case ETHTSYN_STATE_MACHINE_IDENTIFICATION_SYNC_SEND_SM:
      EthTSyn_Master_ProcessSmSyncSend(PortIdx);
      break;

    default:
      /* Nothing to do here */
      break;
  }
} /* EthTSyn_SwtMgmt_TriggerStateMachine() */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedPdelayReq
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedPdelayReq(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is pdelay responder. */
  if(EthTSyn_CfgAccess_IsPdelayResponder(PortIdx) == TRUE)
  {
    ETHTSYN_P2VAR(EthTSyn_PdelayRespSmType) pDelayRespSmPtr = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx);
    /* #20 Check if Pdelay_Req is expected */
    if( (pDelayRespSmPtr->State == ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_REQ) ||
        (pDelayRespSmPtr->State == ETHTSYN_STATE_P_DELAY_RESP_INITAIL_WAITING_FOR_PDELAY_REQ) )
    {
      /* #30 Check if ingress timestamp is available */
      if(ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE(
          MsgBufferPtr->IngressTimestampMgmt.TimestampState))
      {
        /* #40 Process message. */
        ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /* set PdelayResp procedure as in progress to avoid clock change settings in this phase */
        pDelayRespSmPtr->InProgress = TRUE;

        /* store message attributes */
        EthTSyn_SetClockIdentity(pDelayRespSmPtr->PdelayReq.SourcePortIdentity.ClockIdentity,
            MsgBufferPtr->Msg.MsgHdr.SourceClockIdentity);

        pDelayRespSmPtr->PdelayReq.SourcePortIdentity.PortNumber =
            (uint16)EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SourcePortNumber);

        pDelayRespSmPtr->PdelayReq.SequenceId =
            (uint16)EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId);

        pDelayRespSmPtr->PdelayReq.IngressTimestamp =
            MsgBufferPtr->IngressTimestampMgmt.Timestamp;

        pDelayRespSmPtr->PdelayReq.IngressTimestampState =
            ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;

        /* trigger transmission of PdelayResp message */
        pDelayRespSmPtr->State = ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP;

        ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        EthTSyn_Pdelay_ProcessSmPdelayResp(PortIdx);
      }
    }
  }
} /* EthTSyn_SwtMgmt_ProcessReceivedPdelayReq() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedPdelayResp
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedPdelayResp(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONST(EthTSyn_PdelayRespMsgType) pDelayRespMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is Pdelay initiator. */
  if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
  {
    pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);

    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    pDelayRespMsgPtr = (ETHTSYN_P2CONST(EthTSyn_PdelayRespMsgType))
          &MsgBufferPtr->Msg.Payload[0];

    /* #20 Check Pdelay request state machine state. */
    switch(pDelayReqSmPtr->State)
    {
    case ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP:
      /* #200 State: Waiting for Pdelay response -> Check message attributes. */
      /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
      if((EthTSyn_PortIdentityIsEqualLocalPort(
          PortIdx,
          (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))pDelayRespMsgPtr->RequestingClockIdentity) == TRUE) &&
          (EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId) == pDelayReqSmPtr->PdelayReq.SequenceId))
      {
        /* #2000 Check if Pdelay request ingress timestamp is available and process message if available. */
        if(ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE(MsgBufferPtr->IngressTimestampMgmt.TimestampState))
        {
          pDelayReqSmPtr->PdelayResp.IngressTimestamp = MsgBufferPtr->IngressTimestampMgmt.Timestamp;
          pDelayReqSmPtr->PdelayResp.IngressTimestampState =
              ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;

          pDelayReqSmPtr->PdelayResp.SequenceId = (uint16)EthTSyn_Ntohs(
              MsgBufferPtr->Msg.MsgHdr.SequenceId);
          EthTSyn_SetClockIdentity(pDelayReqSmPtr->PdelayResp.SourcePortIdentity.ClockIdentity,
              MsgBufferPtr->Msg.MsgHdr.SourceClockIdentity);
          pDelayReqSmPtr->PdelayResp.SourcePortIdentity.PortNumber =
              (uint16)EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SourcePortNumber);
          /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
          EthTSyn_NtohTimestamp(&pDelayReqSmPtr->PdelayResp.RequestReceiptTimestamp,
              (ETHTSYN_P2VAR(EthTSyn_TimestampType))pDelayRespMsgPtr->RequestReceiptTimestampSeconds);

          pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP;
        }
      }
      else
      {
        /* #2001 PdelayResp message has incorrect attributes -> Reset */
        /* check for lost responses limit */
        if(pDelayReqSmPtr->LostResponses <
            ETHTSYN_CFG_GET_P_DELAY_ALLOWED_LOST_RESPONSES(PortIdx))
        {
          pDelayReqSmPtr->LostResponses++;
        }
        else /* lost responses limit is exceeded */
        {
          EthTSyn_Util_SetAsCapable(PortIdx, ETHTSYN_CFG_GET_ALWAYS_AS_CAPABLE(PortIdx));
        }

        pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_SEND_P_DELAY_REQ;
      }
      break;

    case ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP:
      /* #201 State: Waiting for Pdelay response follow up -> check for double PdelayResp message. */
      if(pDelayReqSmPtr->PdelayReq.SequenceId == EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId))
      {
        /* #2010 Received PdelayResp message with same SequenceId twice -> Reset */
        /* check for lost responses limit */
        if(pDelayReqSmPtr->LostResponses < ETHTSYN_CFG_GET_P_DELAY_ALLOWED_LOST_RESPONSES(PortIdx))
        {
          pDelayReqSmPtr->LostResponses++;
        }
        else /* lost responses limit is exceeded */
        {
          EthTSyn_Util_SetAsCapable(PortIdx, ETHTSYN_CFG_GET_ALWAYS_AS_CAPABLE(PortIdx));
        }

        pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_SEND_P_DELAY_REQ;
      }
      break;

    default:
      /* Nothing to do here (MISRA). */
      break;
    } /* END: switch(pDelayReqSmPtr->State) */

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  } /* END: if(IsPdelayInitiator) */
} /* EthTSyn_SwtMgmt_ProcessReceivedPdelayResp() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedPdelayRespFollowUp
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedPdelayRespFollowUp(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONST(EthTSyn_PDelayResFollowUpMsgType) pDelayRespFollowUpMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_PdelayReqSmType) pDelayReqSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is Pdelay initiator. */
  if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
  {
    pDelayReqSmPtr = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx);
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    pDelayRespFollowUpMsgPtr = (ETHTSYN_P2CONST(EthTSyn_PDelayResFollowUpMsgType))
          &MsgBufferPtr->Msg.Payload[0];

    /* #20 Check Pdelay request state machine state. */
    if(pDelayReqSmPtr->State == ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP)
    {
      /* #30 Verify message attributes. */
      /* PRQA S 3415, 0310 7 */ /* MD_EthTSyn_12.4, MD_EthTSyn_11.4 */
      if((EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId) == pDelayReqSmPtr->PdelayReq.SequenceId ) &&
          (EthTSyn_PortIdentityIsEqual(
              (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))&pDelayReqSmPtr->PdelayResp.SourcePortIdentity,
              (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))MsgBufferPtr->Msg.MsgHdr.SourceClockIdentity) == TRUE) &&
          (EthTSyn_PortIdentityIsEqualLocalPort(
              PortIdx,
              (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))pDelayRespFollowUpMsgPtr->RequestingClockIdentity) == TRUE))
      {
        /* #40 Receive message and handle state machine. */
        /* store massage attributes */
        /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
        EthTSyn_NtohTimestamp(&pDelayReqSmPtr->PdelayRespFollowUp.ResponseOriginTimestamp,
            (ETHTSYN_P2VAR(EthTSyn_TimestampType))pDelayRespFollowUpMsgPtr->ResponseOriginTimestampSeconds);

        /* reset LostResponses counter */
        pDelayReqSmPtr->LostResponses = 0;

        /* check if the PdelayResp was not from own system */
        /* PRQA S 3415 3 */ /* MD_EthTSyn_12.4 */
        if(EthTSyn_CompareClockIdentity(MsgBufferPtr->Msg.MsgHdr.SourceClockIdentity,
            ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx)) != ETHTSYN_COMPARE_EQUAL)
        {
          /* valid PdelayResp and PdelayRespFollowUp were received -> path delay calculation in MainFunction context
        is possible, after PdelayReq message was transmitted successfully (indicated by TxConfirmationPending
        flag)*/
          pDelayReqSmPtr->State = ETHTSYN_STATE_P_DELAY_REQ_RECEIVED_PDELAY_RESP_AND_PDELAY_RESP_FOLLOW_UP;
        }
      }
    }

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  } /* END: if(IsPdelayInitiator) */
} /* EthTSyn_SwtMgmt_ProcessReceivedPdelayResp() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedSync
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedSync(EthTSyn_PortIdxType PortIdx,
  EthTSyn_TimeDomainIdxType TimeDomainIdx, ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if sync message can be received on given Port.  */
  if(EthTSyn_SwtMgmtRx_CheckSyncMsgReception(PortIdx, TimeDomainIdx,
      (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))&MsgBufferPtr->Msg.MsgHdr) == TRUE)
  {
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx);

    /* #20 Check if ingress timestamp is available. */
    if(ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE(MsgBufferPtr->IngressTimestampMgmt.TimestampState))
    {
      /* #30 Store required message attributes and handle sync receive state machine. */
      /* Store the timestamp */
      syncReceiveSmPtr->Sync.IngressTimestamp = MsgBufferPtr->IngressTimestampMgmt.Timestamp;
      syncReceiveSmPtr->Sync.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;

# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
      if(ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE(
          MsgBufferPtr->EgressTimestampHostPortMgmt.TimestampState))
      {
        syncReceiveSmPtr->Sync.EgressTimestampHostPort = MsgBufferPtr->EgressTimestampHostPortMgmt.Timestamp;
        syncReceiveSmPtr->Sync.EgressTimestampHostPortState =
            ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
      }
      else
      {
        syncReceiveSmPtr->Sync.EgressTimestampHostPortState =
            ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;
      }
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    }

    /* Store Sync message attributes */
    syncReceiveSmPtr->Sync.LogMessageInterval = MsgBufferPtr->Msg.MsgHdr.LogMessageInterval;

    EthTSyn_SetClockIdentity(syncReceiveSmPtr->Sync.SourcePortIdentity.ClockIdentity,
        MsgBufferPtr->Msg.MsgHdr.SourceClockIdentity);

    syncReceiveSmPtr->Sync.SourcePortIdentity.PortNumber =
        (uint16)EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SourcePortNumber);

    syncReceiveSmPtr->Sync.SequenceId =
        (uint16)EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId);

    /* change state to waiting for FollowUp */
    syncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP;
    /* set FollowUp receive timeout time */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    syncReceiveSmPtr->FollowUp.FollowUpRxTimeoutCnt =
        EthTSyn_SlavePortConfigPtr(PortIdx)->FollowUpAllowedRxDelay;

    /* rest sync rx timeout cnt */
    syncReceiveSmPtr->Sync.SyncRxTimeoutCnt = EthTSyn_CfgGetSyncTimeout(TimeDomainIdx);

    /* #40 Handle SiteSyncSync state machine. */
    switch(EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State)
    {
    case ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_FUP_AFTER_TIMEOUT:
    case ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING:
    case ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP:
      /* Copy Message to SiteSyncSync Sync Message Buffer */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))&EthTSyn_SiteSyncSyncSm(TimeDomainIdx).Msg.MsgHdr,
          (ETHTSYN_P2CONST(IpBase_CopyDataType))
          &MsgBufferPtr->Msg.MsgHdr, ETHTSYN_HDR_COM_LENGTH);

      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))EthTSyn_SiteSyncSyncSm(TimeDomainIdx).Msg.Payload,
          (ETHTSYN_P2CONST(IpBase_CopyDataType))MsgBufferPtr->Msg.Payload,
          ETHTSYN_SWT_MGMT_MSG_BUFFER_PAYLOAD_SIZE);

      /* Reset TxConfirmationPending of the SiteSyncSync state machine */
      EthTSyn_SiteSyncSyncSm(TimeDomainIdx).TxConfirmationPending =
          EthTSyn_TimeDomainConfig[TimeDomainIdx].MasterPortCnt;

      /* Change SiteSyncSyncSm state to 'SendSync' */
      EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_SYNC;

      /* Process Sm SiteSyncSync */
      EthTSyn_SwtMgmt_ProcessSmSiteSyncSync(TimeDomainIdx);

      break;

    case ETHTSYN_STATE_SITE_SYNC_SYNC_SYNC_TIMEOUT:
      EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_SYNC_AFTER_TIMEOUT;
      break;

    default:
      /* nothing to do */
      break;
    }

    /* #50 Forward Sync message to End-Station if applicable. */
    EthTSyn_SwtMgmtRx_ForwardSyncMsgToEndStation(TimeDomainIdx, MsgBufferPtr);
  }

} /* EthTSyn_SwtMgmt_ProcessReceivedSync() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_CheckSyncMsgReception
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_CheckSyncMsgReception(EthTSyn_PortIdxType PortIdx,
    EthTSyn_TimeDomainIdxType TimeDomainIdx, ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean receiveMessage = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is ready to receive a sync message. */
  if(EthTSyn_SwtMgmtRx_IsPortReadyToReceiveSync(PortIdx) == TRUE)
  {
    /* #20 Perform source port identity check if required. */
    /* Check if source port identity check is enabled */
    if(EthTSyn_CfgGetEnableSourcePortIdentityCheck(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    {
      /* Verify source port identity */
      if( EthTSyn_CheckSourcePortIdentity(ComMsgPtr, PortIdx) == TRUE )
      {
        /* source port identity is invalid --> discard message */
        receiveMessage = TRUE;
      }
    }
    /* #30 Perform announce check if required. */
    /* Check if Announce is enabled */
    else if(EthTSyn_SlavePortConfigPtr(PortIdx)->EnableAnnounce == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    {
      /* Validate System State */
      if( EthTSyn_CheckAnnounce(ComMsgPtr, TimeDomainIdx) == TRUE )
      {
        /* Check failed --> Stop processing of Sync-Message */
        receiveMessage = TRUE;
      }
    }
    else
    {
      receiveMessage = TRUE;
    }
  }

  return receiveMessage;
} /* EthTSyn_SwtMgmtRx_CheckSyncMsgReception() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_IsPortReadyToReceiveSync
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_IsPortReadyToReceiveSync(
  EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isReady = FALSE;
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is slave port */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_SLAVE)
  {
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx);
    /* #20 Check if port is AsCapable. */
    if(EthTSyn_Util_GetAsCapable(PortIdx) == TRUE)
    {
      /* #30 Check state of sync receive state machine. */
      if((syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_DISCARD ) ||
          (syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC ) ||
          (syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_SYNC_TIMEOUT))
      {
        isReady = TRUE;
      }
    }
  }

  return isReady;
} /* EthTSyn_SwtMgmtRx_IsPortReadyToReceiveSync() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedFollowUp
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedFollowUp(
  EthTSyn_PortIdxType PortIdx, EthTSyn_TimeDomainIdxType TimeDomainIdx,
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;
  boolean receiveMessage = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that port is slave port. */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_SLAVE)
  {
    syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #20 Verify sync receive state machine state and matching sequence id (sync and follow up). */
    if((syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP) &&
        (EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId) == syncReceiveSmPtr->Sync.SequenceId) )
    {

      /* #30 Check for matching source port identity of Sync and FollowUp. */
      /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
      if( (EthTSyn_CompareClockIdentity(MsgBufferPtr->Msg.MsgHdr.SourceClockIdentity,
          syncReceiveSmPtr->Sync.SourcePortIdentity.ClockIdentity) == ETHTSYN_COMPARE_EQUAL) &&
          (EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SourcePortNumber) ==
              syncReceiveSmPtr->Sync.SourcePortIdentity.PortNumber) )
      {
        /* Received FollowUp from same node as the former Sync --> receive message */
        receiveMessage = TRUE;
      }

      if(receiveMessage == TRUE)
      {
        /* #40 Store message attributes. */
        syncReceiveSmPtr->FollowUp.SequenceId = /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
            (uint16)EthTSyn_Ntohs(MsgBufferPtr->Msg.MsgHdr.SequenceId);

        /* #50 Handle SiteSyncSycnSm. */
        switch(EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State)
        {
        case ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING:
        case ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_SYNC:
          /* Copy FollowUp message to SiteSyncSync state machine */
          /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
          IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))&EthTSyn_SiteSyncSyncSm(TimeDomainIdx).Msg.MsgHdr,
              (ETHTSYN_P2CONST(IpBase_CopyDataType))
              &MsgBufferPtr->Msg.MsgHdr, ETHTSYN_HDR_COM_LENGTH);

          /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
          IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))EthTSyn_SiteSyncSyncSm(TimeDomainIdx).Msg.Payload,
              (ETHTSYN_P2CONST(IpBase_CopyDataType))MsgBufferPtr->Msg.Payload,
              ETHTSYN_SWT_MGMT_MSG_BUFFER_PAYLOAD_SIZE);

          /* Reset TxConfirmationPending of SiteSyncSync state machine. */
          EthTSyn_SiteSyncSyncSm(TimeDomainIdx).TxConfirmationPending =
              EthTSyn_TimeDomainConfig[TimeDomainIdx].MasterPortCnt;

          /* Change state of SiteSyncSyncSm and Process the SM */
          EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP;

          EthTSyn_SwtMgmt_ProcessSmSiteSyncSync(TimeDomainIdx);
          break;

        case ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_SYNC_AFTER_TIMEOUT:
          EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_FUP_AFTER_TIMEOUT;

          EthTSyn_SwtMgmt_ProcessSmSiteSyncSync(TimeDomainIdx);
          break;

        default:
          /* nothing to do */
          break;
        }

        syncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC;

        /* #60 Forward FollowUp to End-Station if applicable. */
        EthTSyn_SwtMgmtRx_ForwardFollowUpMsgToEndStation(PortIdx, TimeDomainIdx,
            (ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType))MsgBufferPtr);
      }
    }
  } /* END: if(IsSlavePort) */

} /* EthTSyn_SwtMgmt_ProcessReceivedFollowUp() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessReceivedAnnounce
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessReceivedAnnounce(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType) announceMsgPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is ready to receive announce message. */
  if(EthTSyn_UtilRx_IsPortReadyToReceiveAnnounce(PortIdx) == TRUE)
  {
    /* #20 Check for valid Announce message. */
    if(EthTSyn_UtilRx_QualifyAnnounceMsg(PortIdx, &MsgBufferPtr->Msg.MsgHdr) == E_OK)
    {
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      announceMsgPtr = (ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType))&MsgBufferPtr->Msg.Payload[0];
      CANOE_WRITE_STRING("EthTSyn_RxIndication: Received Announce");
      /* reset announce timeout counter */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(PortIdx)->RxTimeoutCnt =
          EthTSyn_SlavePortConfigPtr(PortIdx)->AnnounceAllowedRxDelay;
      /* check if no Grandmaster is available */
      if(EthTSyn_SystemState[ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx)].GmPresent == FALSE)
      {
        EthTSyn_SetMasterPriorityVector(announceMsgPtr, ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx));
      }
    }
  }
} /* EthTSyn_SwtMgmt_ProcessReceivedAnnounce() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ReleaseMsgBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ReleaseMsgBuffer(
    ETHTSYN_CONSTP2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Release the message buffer */
  MsgBufferPtr->MsgBufferState = ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_UNUSED;
  /* #20 Reset the timestamp states */
  MsgBufferPtr->IngressTimestampMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  MsgBufferPtr->EgressTimestampHostPortMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID;
# endif
  /* #30 Reset the EthIf Ctrl and MgmtInfo */
  MsgBufferPtr->CtrlIdx = ETHTSYN_INVALID_ETHIF_CTRL_IDX;
  MsgBufferPtr->SwtMgmtInfo.SwitchIdx = ETHTSYN_INVALID_ETHIF_SWITCH_IDX;
  MsgBufferPtr->SwtMgmtInfo.PortIdx = ETHTSYN_INVALID_SWITCH_PORT_IDX;

  /* #40 Reset Rx buffer pointer */
  MsgBufferPtr->RxBufAddr = NULL_PTR;

  ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* EthTSyn_SwtMgmt_ReleaseMsgBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_PerformBasicMsgChecksAngGetMsgBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_PerformBasicMsgChecksAngGetMsgBuffer(
  uint16 LenByte, ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType*) MsgBufferPtrPtr,
  ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgHdrPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to get a free message buffer. */
  if(EthTSyn_SwtMgmt_ProvideMsgBuffer(MsgBufferPtrPtr) == E_OK)
  {
    /* #20 Message buffer available and locked. Perform general message checks. */
    if(EthTSyn_UtilRx_CheckMessageGeneral((ETHTSYN_P2CONST(uint8))DataPtr, LenByte, ErrorIdPtr) == E_OK)
    {
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      comMsgHdrPtr = (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))&DataPtr[0];
      /* #30 General message checks passed. Check message type and length. */
      if(EthTSyn_UtilRx_CheckMessageTypeAndLength(comMsgHdrPtr, LenByte, ErrorIdPtr) == E_OK)
      {
        /* #40 Message type and length check passed. Ensure that message buffer is sufficient for the message. */
        if(ETHTSYN_SWT_MGMT_MSG_BUFFER_PAYLOAD_SIZE >= (LenByte - ETHTSYN_HDR_COM_LENGTH))
        {
          retVal = E_OK;
        }
        else
        {
          /* insufficient buffer --> drop message */
          *ErrorIdPtr = ETHTSYN_SWT_MGMT_E_MSG_BUFFER_PAYLOAD_OVERFLOW;
        }
      }
    }

    /* #50 Release message buffer if any message check failed. */
    if(retVal == E_NOT_OK)
    {
      EthTSyn_SwtMgmt_ReleaseMsgBuffer(*MsgBufferPtrPtr);
    }
  }
  else
  {
    /* No message buffer available --> drop message */
    *ErrorIdPtr = ETHTSYN_SWT_MGMT_E_MSG_BUFFER_OVERFLOW;
  }

  return retVal;
} /* EthTSyn_SwtMgmtRx_PerformBasicMsgChecksAngGetMsgBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_HandleIngressTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_HandleIngressTimestamp(uint8 EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) MsgBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgHdrPtr = (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))&DataPtr[0];
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  EthTSyn_TimeDomainIdxType timeDomainIdx = EthTSyn_CfgAccess_GetTimeDomainIdx(comMsgHdrPtr->DomainNumber);
# endif /* # (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  /* #10 Switch Timestamping enabled. Mark timestamp states according to message type necessity. */
  switch(ETHTSYN_HDR_COM_MSG_TYPE(comMsgHdrPtr->TransSpecific_MessageType))
  {
  case ETHTSYN_MSG_TYPE_SYNC:
    MsgBufferPtr->IngressTimestampMgmt.TimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
    MsgBufferPtr->EgressTimestampHostPortMgmt.TimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
    /* #100 Sync message. Get timestamp of Host Controller if required. */
    if(EthTSyn_SwtMgmt_GetEndStationSlavePortIdx(timeDomainIdx) != ETHTSYN_INVALID_PORT_IDX)
    {
      if(EthTSyn_SwtMgmt_GetIngressTimestamp(EthIfCtrlIdx, DataPtr,
          &MsgBufferPtr->IngressTimestampHostCtrlMgmt.Timestamp) == E_OK)
      {
        MsgBufferPtr->IngressTimestampHostCtrlMgmt.TimestampState =
            ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
      }
      else
      {
        MsgBufferPtr->IngressTimestampHostCtrlMgmt.TimestampState =
            ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;
      }
    }
    break;
  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
    MsgBufferPtr->IngressTimestampMgmt.TimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
    MsgBufferPtr->EgressTimestampHostPortMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED;
    break;
  default:
    MsgBufferPtr->IngressTimestampMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED;
    MsgBufferPtr->EgressTimestampHostPortMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED;
    break;
  }
# else
  /* #20 No Switch Timestamping. Get the Ingress Timestamp (host controller) if necessary */
  switch(ETHTSYN_HDR_COM_MSG_TYPE(comMsgHdrPtr->TransSpecific_MessageType))
  {
  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
  case ETHTSYN_MSG_TYPE_SYNC:
  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
    if(EthTSyn_SwtMgmt_GetIngressTimestamp(EthIfCtrlIdx, DataPtr,
        &(MsgBufferPtr->IngressTimestampMgmt.Timestamp)) == E_OK)
    {
      /* Successfully retrieved the IngressTimestamp --> Store it */
      MsgBufferPtr->IngressTimestampMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
    }
    else
    {
      /* Failed to retrieve the IngressTimestamp */
      MsgBufferPtr->IngressTimestampMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;
    }
    break;
  default:
    /* No Ingress Timestamp needed */
    MsgBufferPtr->IngressTimestampMgmt.TimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED_NOT_AVAILABLE;
    break;
  }
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

} /* EthTSyn_SwtMgmtRx_HandleIngressTimestamp() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProvideMsgBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProvideMsgBuffer(
    ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) *MsgBufferPtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least index;

  /* ----- Implementation ----------------------------------------------- */
  *MsgBufferPtrPtr = NULL_PTR;

  ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* #10 Iterate through all message buffers */
  for(index = 0; index < ETHTSYN_SWT_MGMT_MSG_BUFFER_COUNT; index++)
  {
    /* #1000 If message buffer is free */
    if(EthTSyn_SwtMgmt_MsgBuffer[index].MsgBufferState == ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_UNUSED)
    {
      /* #100000 Lock the message buffer */
      EthTSyn_SwtMgmt_MsgBuffer[index].MsgBufferState = ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_INVALID;

      /* 100010 Provide the MsgBuffer Pointer */
      *MsgBufferPtrPtr = &EthTSyn_SwtMgmt_MsgBuffer[index];
      retVal = E_OK;
      break;
    }
  }
  ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;
} /* EthTSyn_SwtMgmt_ProvideMsgBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetMappedMsgBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetMappedMsgBuffer(uint8 CtrlIdx,
    ETHTSYN_P2CONST(uint8) RxBufAddr, EthTSyn_FrameIdType FrameId,
    ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) *MsgBufferPtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least index;

  /* ----- Implementation ----------------------------------------------- */
  for(index = 0; index < ETHTSYN_SWT_MGMT_MSG_BUFFER_COUNT; index++)
  {
    if( (EthTSyn_SwtMgmt_MsgBuffer[index].MsgBufferState == ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_VALID) ||
        (EthTSyn_SwtMgmt_MsgBuffer[index].MsgBufferState == ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_PORT_INFO_AVAILABLE) )
    {
      if( (EthTSyn_SwtMgmt_MsgBuffer[index].CtrlIdx == CtrlIdx) &&
          (EthTSyn_SwtMgmt_MsgBuffer[index].RxBufAddr == RxBufAddr) &&
          (ETHTSYN_SWT_MGMT_FRAME_ID_FROM_MSG_BUFFER(&EthTSyn_SwtMgmt_MsgBuffer[index]) == FrameId) )
      {
        retVal = E_OK;
        *MsgBufferPtrPtr = &EthTSyn_SwtMgmt_MsgBuffer[index];
        break;
      }
    }
  }

  return retVal;
} /* EthTSyn_SwtMgmt_GetMappedMsgBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetEndStationSlavePortIdx
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetEndStationSlavePortIdx(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
/* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType slavePortIdx = ETHTSYN_INVALID_PORT_IDX;
  EthTSyn_PortIdxType slavePortCnt;

  /* ----- Implementation ----------------------------------------------- */
  if(TimeDomainIdx < ETHTSYN_TIME_DOMAIN_COUNT)
  {
    slavePortCnt = EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx);

    if(slavePortCnt > 0)
    {
      while(slavePortCnt > 0)
      {
        slavePortCnt--;
        if((EthTSyn_Util_GetPortType(EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt)) &
          ETHTSYN_PORT_TYPE_END_STATION_PORT) != 0)
        {
          slavePortIdx = EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt);
          break;
        }
      }
    }
  }
  return slavePortIdx;
} /* EthTSyn_SwtMgmt_GetEndStationSlavePortIdx() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetSwitchSlavePortIdx
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetSwitchSlavePortIdx(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
/* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType slavePortIdx = ETHTSYN_INVALID_PORT_IDX;
  EthTSyn_PortIdxType slavePortCnt = EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx);

  /* ----- Implementation ----------------------------------------------- */
  while(slavePortCnt > 0)
  {
    slavePortCnt--;
    if((EthTSyn_Util_GetPortType(EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt)) &
      ETHTSYN_PORT_TYPE_SWITCH_PORT) != 0)
    {
      slavePortIdx = EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt);
      break;
    }
  }

  return slavePortIdx;
} /* EthTSyn_SwtMgmt_GetSwitchSlavePortIdx() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetIngressTimestamp
 *********************************************************************************************************************/
/* PRQA S 3673 2 */ /* MD_EthTSyn_16.7c */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetIngressTimestamp(uint8 EthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr, ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) IngressTimestampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_GlobalTimestampType ingressTimestamp;
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimestampQualityType timestampQual = ETH_INVALID;
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  /* HW-Timestamping */ /* PRQA S 0310, 0311 2 */ /* MD_EthTSyn_11.4, MD_EthTSyn_11.5 */
  if(EthIf_GetIngressTimeStamp(EthIfCtrlIdx, (ETHTSYN_P2VAR(Eth_DataType))DataPtr, &timestampQual,
    &ingressTimestamp) == E_OK)
  {
    if(timestampQual == ETH_VALID)
    {
      *IngressTimestampPtr = ingressTimestamp;
      retVal = E_OK;
    }
  }
# else
  /* SW-Timestamping */
  ETHTSYN_DUMMY_STATEMENT(EthIfCtrlIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(DataPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(StbM_GetCurrentTimeRaw(&ingressTimestamp.nanoseconds) == E_OK)
  {
    IngressTimestampPtr->secondsHi = 0;
    IngressTimestampPtr->seconds = 0;
    IngressTimestampPtr->nanoseconds = ingressTimestamp.nanoseconds;
    retVal = E_OK;
  }
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  return retVal;
} /* EthTSyn_SwtMgmt_GetIngressTimestamping() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetMappedStateMachine
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SwtMgmt_StateMachineIndentificationType, ETHTSYN_CODE)
  EthTSyn_SwtMgmt_GetMappedStateMachine(uint8 CtrlIdx, ETHTSYN_CONSTP2CONST(uint8) TxBufPtr,
  EthTSyn_FrameIdType FrameId, ETHTSYN_P2VAR(EthTSyn_PortIdxType) PortIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmt_StateMachineIndentificationType stateMachine = ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM;
  EthTSyn_PortIdxType portIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate through all relevant state machines (i.e. SyncSend, Pdelay_Req, Pdelay_Resp). */
  for(portIdx = 0; portIdx < ETHTSYN_PORT_COUNT; portIdx++)
  {
    if(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(portIdx) == CtrlIdx)
    {
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      if((EthTSyn_CfgGetPortRole(portIdx) == ETHTSYN_PORT_ROLE_MASTER) &&
        (ETHTSYN_SYNC_SEND_SM_PTR(portIdx)->SyncTxBufPtr == TxBufPtr) &&
          (ETHTSYN_SWT_MGMT_FRAME_ID_FROM_SM(ETHTSYN_SYNC_SEND_SM_PTR(portIdx)) == FrameId))
      {
        /* SyncSend state machine */
        stateMachine = ETHTSYN_STATE_MACHINE_IDENTIFICATION_SYNC_SEND_SM;
      }
      if((EthTSyn_CfgAccess_IsPdelayInitiator(portIdx) == TRUE) &&
        (ETHTSYN_P_DELAY_REQ_SM_PTR(portIdx)->TxBufPtr == TxBufPtr) &&
        (ETHTSYN_SWT_MGMT_FRAME_ID_FROM_SM(ETHTSYN_P_DELAY_REQ_SM_PTR(portIdx)) == FrameId))
      {
        /* Pdelay request state machine */
        stateMachine = ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_REQ_SM;
      }
      if((EthTSyn_CfgAccess_IsPdelayResponder(portIdx) == TRUE) &&
        (ETHTSYN_P_DELAY_RESP_SM_PTR(portIdx)->TxBufPtr == TxBufPtr) &&
        (ETHTSYN_SWT_MGMT_FRAME_ID_FROM_SM(ETHTSYN_P_DELAY_RESP_SM_PTR(portIdx)) == FrameId))
      {
        /* Pdelay response state machine */
        stateMachine = ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_RESP_SM;
      }

      if(stateMachine != ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM)
      {
        /* #20 State Machine found --> stop iteration and get PortIdx. */
        *PortIdxPtr = portIdx;
        break;
      }
    }
  }

  return stateMachine;
} /* EthTSyn_SwtMgmt_GetMappedStateMachine */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_GetStateMachineTxTimestampInfo
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
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmt_GetStateMachineTxTimestampInfo(
    EthTSyn_SwtMgmt_StateMachineIndentificationType StateMachine,
    EthTSyn_PortIdxType PortIdx, EthTSyn_SwtMgmt_TimestampDirectionType TimestampDir,
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) *TimestampPtrPtr,
    ETHTSYN_P2VAR(EthTSyn_TimestampStateType) *TimestampStatePtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check state machine. */
  switch(StateMachine)
  {
    case ETHTSYN_STATE_MACHINE_IDENTIFICATION_SYNC_SEND_SM:
      /* #100 Sync send state machine -> check timestamp direction. */
      if(TimestampDir == ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_INGRESS)
      {
        /* #1000 Ingress timestamp -> get sync ingress timestamp host port information. */
        /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
        *TimestampPtrPtr = &ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncIngressTimestampHostPort;
        *TimestampStatePtrPtr = &ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncIngressTimestampStateHostPort;
        if(EthTSyn_IsTimestampReadyToProcess(
            ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncEgressTimestampStateSwitchPort) == TRUE)
        {
          ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncTxBufPtr = NULL_PTR; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
        }
        retVal = E_OK;
      }
      if(TimestampDir == ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_EGRESS)
      {
        /* #1001 Egress timestamp -> get sync egress timestamp switch port information. */
        /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
        *TimestampPtrPtr = &ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncEgressTimestampSwitchPort;
        *TimestampStatePtrPtr = &ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncEgressTimestampStateSwitchPort;
        if(EthTSyn_IsTimestampReadyToProcess(
          ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncIngressTimestampStateHostPort) == TRUE)
        {
          ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SyncTxBufPtr = NULL_PTR; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
        }
        retVal = E_OK;
      }
      break;

    case ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_REQ_SM:
      /* #101 Pdelay request state machine -> check timestamp direction. */
      if(TimestampDir == ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_EGRESS)
      {
        /* #1010 Egress timestamp -> get Pdelay request egress timestamp information. */
        *TimestampPtrPtr = &ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.EgressTimestamp;
        *TimestampStatePtrPtr = &ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.EgressTimestampState;
        ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->TxBufPtr = NULL_PTR;
        retVal = E_OK;
      }
      break;

    case ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_RESP_SM:
      /* #101 Pdelay response state machine -> check timestamp direction. */
      if(TimestampDir == ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_EGRESS)
      {
        /* #1010 Egress timestamp -> get Pdelay response egress timestamp information. */
        *TimestampPtrPtr = &ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayResp.EgressTimestamp;
        *TimestampStatePtrPtr = &ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayResp.EgressTimestampState;
        ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->TxBufPtr = NULL_PTR;
        retVal = E_OK;
      }
      break;

    default:
      /* nothing to do here */
      break;
  }

  return retVal;
} /* EthTSyn_SwtMgmt_GetStateMachineTimestampInfo() */
# endif /* ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_TxSyncMsg
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_TxSyncMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(uint8) u8BufPtr;
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);
  uint16 txBufferSize = ETHTSYN_MSG_SYNC_LENGTH;
  uint8 bufIdx;
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);
  EthSwt_MgmtInfoType mgmtInfo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get transmission buffer. */
  /* PRQA S 0310 4 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
      ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), &bufIdx, (ETHTSYN_P2VAR(Eth_DataType*))&u8BufPtr,
      &txBufferSize) == BUFREQ_OK)
  {
    /* #20 Copy the sync message to TxBuffer. */
    EthTSyn_SwtMgmtTx_CopySyncMsgFromSiteSyncSyncSmToTxBuffer(PortIdx, u8BufPtr);

    /* #30 Get and set required information for later processing (transmission and TxConfirmation) */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    syncSendSmPtr->SyncEgressTimestampStateSwitchPort =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
    syncSendSmPtr->SyncIngressTimestampStateHostPort =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
# else
    syncSendSmPtr->FollowUpPreciseOriginTimestampState =
        ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

    /* Get Switch management Info. */
    mgmtInfo.SwitchIdx = EthTSyn_PortConfig[PortIdx].SwtInfo.SwitchIdx;
    mgmtInfo.PortIdx = EthTSyn_PortConfig[PortIdx].SwtInfo.PortIdx;

    syncSendSmPtr->SyncTxBufIdx = bufIdx;
    syncSendSmPtr->SyncTxBufPtr = u8BufPtr;

# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
    /* Save Frame Id */
    mgmtInfo.FrameId = EthTSyn_Util_GetFrameId(u8BufPtr, txBufferSize);
    syncSendSmPtr->FrameId = mgmtInfo.FrameId;
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)*/

    /* #40 Enable egress timestamping if required. */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    (void)EthIf_SwitchEnableEgressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), bufIdx, &mgmtInfo);
# else
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    (void)EthIf_EnableEgressTimestamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), bufIdx);
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

    /* #50 Trigger transmission. */
    if(EthTSyn_Transmit(PortIdx, bufIdx, ETHTSYN_MSG_SYNC_LENGTH, TRUE, &mgmtInfo) != E_OK)
    {
      /* EthTSyn_SiteSyncSyncSm(TimeDomainIdx).TxConfirmationPending is set to the amount of configured
       * MasterPorts for the TimeDomain at the reception of a Sync message. Since this code part can only
       * be reached after the reception of a Sync message no underflow is possible */
      EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending--;
      /* Transmission failed */
      syncSendSmPtr->SyncTxBufIdx = ETHTSYN_INV_BUF_IDX;
      syncSendSmPtr->SyncTxBufPtr = NULL_PTR;
# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
      syncSendSmPtr->FrameId = ETHTSYN_INVALID_FRAME_ID;
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
    }
  }

} /* EthTSyn_SwtMgmtTx_TxSyncMsg() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTX_CopySyncMsgFromSiteSyncSyncSmToTxBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CopySyncMsgFromSiteSyncSyncSmToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) BufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncMsgType) syncMsgPtr;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comHdrPtr;
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  comHdrPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))BufferPtr;
  syncMsgPtr = (ETHTSYN_P2VAR(EthTSyn_SyncMsgType))&BufferPtr[ETHTSYN_HDR_COM_LENGTH];

  /* #10 Copy common Hdr to TxBuffer. */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))comHdrPtr,
      (ETHTSYN_P2CONST(IpBase_CopyDataType))&EthTSyn_SiteSyncSyncSm(timeDomainIdx).Msg.MsgHdr,
      ETHTSYN_HDR_COM_LENGTH);

  /* #20 Manipulate SourcePortIdentity fields within the common Hdr if required. */
  if(EthTSyn_CfgGetKeepSrcPortIdentity(timeDomainIdx) != TRUE)
  {
    /* Use 'Own' source port identity */
    EthTSyn_SetClockIdentity(comHdrPtr->SourceClockIdentity,
        ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx));

    comHdrPtr->SourcePortNumber = (uint16)EthTSyn_Htons(ETHTSYN_CFG_GET_PORT_NUMBER(PortIdx));
  }

  /* #30 Copy Sync message values to TxBuffer. */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))syncMsgPtr,
      (ETHTSYN_P2CONST(IpBase_CopyDataType))EthTSyn_SiteSyncSyncSm(timeDomainIdx).Msg.Payload,
      ETHTSYN_MSG_SYNC_PAYLOAD_LENGTH);

} /* EthTSyn_SwtMgmtTX_CopySyncMsgFromSiteSyncSyncSmToTxBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_TxFollowUpMsg
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_TxFollowUpMsg(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);
  uint8 bufIdx;
  ETHTSYN_P2VAR(uint8) u8BufPtr;
  uint16 txBufferSize = (uint16)EthTSyn_Ntohs(EthTSyn_SiteSyncSyncSm(timeDomainIdx).Msg.MsgHdr.MessageLength);
  uint16 txLen = txBufferSize;
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);
  ETHTSYN_P2CONST(EthTSyn_TimestampStateType) syncEgressTimestampStatePtr;

  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  syncEgressTimestampStatePtr = &syncSendSmPtr->SyncEgressTimestampStateSwitchPort;
# else
  syncEgressTimestampStatePtr = &syncSendSmPtr->FollowUpPreciseOriginTimestampState;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

  /* #10 Check if Sync egress timestamp is ready for procession. */
  if(EthTSyn_IsTimestampReadyToProcess(*syncEgressTimestampStatePtr) == TRUE)
  {
    /* #20 Get transmission buffer. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    if(EthIf_ProvideTxBuffer(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), ETHTSYN_FRAME_TYPE,
        ETHTSYN_CFG_GET_FRAME_PRIORITY(PortIdx), &bufIdx, (ETHTSYN_P2VAR(Eth_DataType*))&u8BufPtr,
        &txBufferSize) == BUFREQ_OK)
    {
      /* ----- Variables ----------------------------------- */
      EthSwt_MgmtInfoType mgmtInfo;

      /* #30 Copy FollowUp message to TxBuffer. */
      EthTSyn_SwtMgmtTx_CopyFollowUpMsgFromSiteSyncSyncSmToTxBuffer(PortIdx, u8BufPtr);

      /* #40 Store information required for later processing in transmit and TxConfirmation. */
      syncSendSmPtr->FollowUpTxBufIdx = bufIdx;
      syncSendSmPtr->FollowUpTxBufPtr = u8BufPtr;
# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
      mgmtInfo.FrameId = EthTSyn_Util_GetFrameId(u8BufPtr, txBufferSize);
      syncSendSmPtr->FrameId = mgmtInfo.FrameId;
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
      /* Set Switch management Info */
      mgmtInfo.SwitchIdx = EthTSyn_PortConfig[PortIdx].SwtInfo.SwitchIdx;
      mgmtInfo.PortIdx = EthTSyn_PortConfig[PortIdx].SwtInfo.PortIdx;

      /* #50 Trigger transmission. */
      if(EthTSyn_Transmit(PortIdx, bufIdx, txLen, TRUE, &mgmtInfo) != E_OK)
      {
        /* EthTSyn_SiteSyncSyncSm(TimeDomainIdx).TxConfirmationPending is set to the amount of configured
         * MasterPorts for the TimeDomain at the reception of a FollowUp message. Since this code part can only
         * be reached after the reception of a FollowUp message no underflow is possible */
        EthTSyn_SiteSyncSyncSm(timeDomainIdx).TxConfirmationPending--;
        syncSendSmPtr->FollowUpTxBufIdx = ETHTSYN_INV_BUF_IDX;
        syncSendSmPtr->FollowUpTxBufPtr = NULL_PTR;
# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
        syncSendSmPtr->FrameId = ETHTSYN_INVALID_FRAME_ID;
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
      }
    }
  }
} /* EthTSyn_SwtMgmtTx_TxFollowUpMsg() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTX_CopyFollowUpMsgFromSiteSyncSyncSmToTxBuffer
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CopyFollowUpMsgFromSiteSyncSyncSmToTxBuffer(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(uint8) BufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);
  uint16 txLen;
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) comHdrPtr;
  ETHTSYN_P2VAR(EthTSyn_FollowUpMsgType) followUpMsgPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  comHdrPtr = (ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType))BufferPtr;
  followUpMsgPtr = (ETHTSYN_P2VAR(EthTSyn_FollowUpMsgType))&BufferPtr[ETHTSYN_HDR_COM_LENGTH];
  txLen = (uint16)EthTSyn_Ntohs(EthTSyn_SiteSyncSyncSm(timeDomainIdx).Msg.MsgHdr.MessageLength);

  /* #10 Copy Common Hdr to TxBuffer. */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))comHdrPtr,
      (ETHTSYN_P2CONST(IpBase_CopyDataType))&EthTSyn_SiteSyncSyncSm(timeDomainIdx).Msg.MsgHdr,
      ETHTSYN_HDR_COM_LENGTH);

  /* #20 Copy FollwUp to TxBuffer. */
  /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
  IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))followUpMsgPtr,
      (ETHTSYN_P2CONST(IpBase_CopyDataType))EthTSyn_SiteSyncSyncSm(timeDomainIdx).Msg.Payload,
      (txLen - ETHTSYN_HDR_COM_LENGTH));

  /* #30 Manipulate the SourcePortIdentity if required. */
  if(EthTSyn_CfgGetKeepSrcPortIdentity(timeDomainIdx) != TRUE)
  {
    /* Use 'Own' source port identity */
    EthTSyn_SetClockIdentity(comHdrPtr->SourceClockIdentity,
        ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx));

    comHdrPtr->SourcePortNumber = (uint16)EthTSyn_Htons(EthTSyn_PortConfig[PortIdx].PortNumber);
  }
  /* #50 Compute and set the FollowUp correction field. */
  EthTSyn_SwtMgmtTx_ComputeAndSetFollowUpCorrectionField(PortIdx, comHdrPtr);

} /* EthTSyn_SwtMgmtTX_CopyFollowUpMsgFromSiteSyncSyncSmToTxBuffer() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ComputeAndSetFollowUpCorrectionField
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
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ComputeAndSetFollowUpCorrectionField(
    EthTSyn_PortIdxType PortIdx, ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) ComMsgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  ETHTSYN_P2CONST(EthTSyn_TimestampStateType) syncEgressTimestampStatePtr;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType)  syncEgressTimestampPtr;
  EthTSyn_TimediffType followUpCorrectionFieldNs = 0;
  EthTSyn_PortIdxType slavePortIdx = EthTSyn_SwtMgmt_GetSwitchSlavePortIdx(
      ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx));
  /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  if(slavePortIdx < ETHTSYN_PORT_COUNT)
  {
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    syncEgressTimestampStatePtr = &syncSendSmPtr->SyncEgressTimestampStateSwitchPort;
    syncEgressTimestampPtr      = &syncSendSmPtr->SyncEgressTimestampSwitchPort;
# else
    syncEgressTimestampPtr      = &syncSendSmPtr->FollowUpPreciseOriginTimestamp;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */

# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
    /* #10 Switch timestamping. Check if sync egress timestamp is available. */
    if(ETHTSYN_SWT_MGMT_COND_TIMESTAMP_STATE_TS_AVAILABLE(*syncEgressTimestampStatePtr))
    {
      /* #100 Sync egress timestamp available. Compute switch residence time of the sync message. */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      if(EthTSyn_Util_TimestampMinusTimestamp(syncEgressTimestampPtr,
          &ETHTSYN_SYNC_RECEIVE_SM_PTR(slavePortIdx)->Sync.IngressTimestamp, &followUpCorrectionFieldNs) != E_OK)
      {
        /* #1000 Failed to compute switch residence time. Use default value. */
        followUpCorrectionFieldNs = (EthTSyn_TimediffType)
                        (ETHTSYN_SWT_RX_TO_UPLINK_RESIDENCE_TIME_NS + ETHTSYN_SWT_UPLINK_TO_TX_RESIDENCE_TIME_NS);
      }
    }
    else
    {
      /* #101 Sync egress timestamp not available. Use default value for residence time. */
      followUpCorrectionFieldNs = (EthTSyn_TimediffType)
                      (ETHTSYN_SWT_RX_TO_UPLINK_RESIDENCE_TIME_NS + ETHTSYN_SWT_UPLINK_TO_TX_RESIDENCE_TIME_NS);
    }
# else
    /* #20 No Switch timestamping. Compute SW residence time of the sync message. */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    if(EthTSyn_Util_TimestampMinusTimestamp(syncEgressTimestampPtr,
        &ETHTSYN_SYNC_RECEIVE_SM_PTR(slavePortIdx)->Sync.IngressTimestamp, &followUpCorrectionFieldNs) == E_OK)
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
    {
      if(followUpCorrectionFieldNs >= 0)
      {
        uint64 unsignedFupCorrectionField = (uint64)followUpCorrectionFieldNs;
        EthTSyn_PdelayType pDelay = 0;
        /* Add the Path Delay 'of the Slave Port' to the Residence Time */
        /*
                      +----------+      Sync         +--------------------+ Sync
                      |  Master  +------------------>+S||    Bridge    ||M+--------->
                      +----------+t1               t2+--------------------+t3

                                 +  Pdelay (t2-t1)   + Residence (t3-t2)  +
                                 +<----------------->-<------------------>+
                                 +                   +                    +
         */
        /* #30 Add Pdelay to the correction field. */
        if(EthTSyn_Pdelay_GetCurrentPdelay(slavePortIdx, &pDelay) == E_OK)
        { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          unsignedFupCorrectionField += pDelay;
        }
        /* #40 Add the former FollowUpCorrectionValue */
        /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
        unsignedFupCorrectionField +=
            ETHTSYN_GET_FOLLOW_UP_CORRECTION_VALUE_NS_FROM_MSG(ComMsgPtr->CorrectionField);

        /* #50 Set the computed value of the correction field. */
        /* Multiply FollwUpCorrectionField by 2^16 (as specified by IEEE802.1AS and copy it to the TxBuffer */
        EthTSyn_CopyFollowUpCorrectionField(unsignedFupCorrectionField, ComMsgPtr->CorrectionField)

      }
    }
  }
} /* EthTSyn_SwtMgmtTx_ComputeAndSetFollowUpCorrectionField() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_HandleSendSyncStateOfSiteSyncSyncSm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_HandleSendSyncStateOfSiteSyncSyncSm(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxIterType masterPortCnt;
  EthTSyn_PortIdxType masterPortIdx;
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all master Ports. */
  for(masterPortCnt = 0; masterPortCnt < EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx); masterPortCnt++)
  {
    masterPortIdx = EthTSyn_CfgGetMasterPortIdx(TimeDomainIdx, masterPortCnt);
    syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(masterPortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #20 Check if Sync send state machine of the master port is in a state where the sync message should be send. */
    if( (syncSendSmPtr->State == ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_PENDING) ||
        (syncSendSmPtr->State == ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_FOLLOW_UP) ||
        (syncSendSmPtr->State == ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_SYNC) )
    {
      /* #30 Trigger transmission of the Sync message. */
      syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_SYNC;
      EthTSyn_SwtMgmtTx_TxSyncMsg(masterPortIdx);
    }
  } /* END: Iteration of master ports */
} /* EthTSyn_SwtMgmt_HandleSendSyncStateOfSiteSyncSyncSm() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_HandleSendFollowUpStateOfSiteSyncSyncSm
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_HandleSendFollowUpStateOfSiteSyncSyncSm(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType slavePortIdx;
  EthTSyn_PortIdxType masterPortIdx;
  EthTSyn_PortIdxIterType masterPortCnt;
  ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  slavePortIdx = EthTSyn_SwtMgmt_GetSwitchSlavePortIdx(TimeDomainIdx);

  if(slavePortIdx < ETHTSYN_PORT_COUNT)
  {
    /* #10 Check that sync ingress timestamp is ready for processing. */
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    if(EthTSyn_IsTimestampReadyToProcess(ETHTSYN_SYNC_RECEIVE_SM_PTR(slavePortIdx)->Sync.IngressTimestampState) ==
        TRUE)
    {
      /* #20 Iterate over all master Ports. */
      for(masterPortCnt = 0; masterPortCnt < EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx); masterPortCnt++)
      {

        masterPortIdx = EthTSyn_TimeDomainConfig[TimeDomainIdx].MasterPortIdxPtr[masterPortCnt];
        syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(masterPortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

        /* #30 Check if Sync send state machine of the master port is in a state where the FollowUp should be send. */
        if( (syncSendSmPtr->State == ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_WAIT_FOLLOW_UP) ||
            (syncSendSmPtr->State == ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_FOLLOW_UP) )
        {
          /* #40 Trigger transmission of the FollowUp message. */
          syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_FOLLOW_UP;
          EthTSyn_SwtMgmtTx_TxFollowUpMsg(masterPortIdx);
        }
      }
    }
  }
} /* EthTSyn_SwtMgmt_HandleSendFollowUpStateOfSiteSyncSyncSm() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_HandleReceivedFupAfterTimeoutStateOfSiteSyncSyncSm
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_HandleReceivedFupAfterTimeoutStateOfSiteSyncSyncSm(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxType slavePortIdx;
  EthTSyn_PortIdxIterType masterPortCnt;

  /* ----- Implementation ----------------------------------------------- */
  slavePortIdx = EthTSyn_SwtMgmt_GetSwitchSlavePortIdx(TimeDomainIdx);
  /* #10 Check if bridge acted as grand master. */
  if(ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) != slavePortIdx)
  {
    /* #20 Bridge acted as grand master after sync-timeout --> "enable" the slave port again and start 'normal'
     *     bridge operation again. */
    ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) = slavePortIdx; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #30 Iterate over all master Ports. */
    for(masterPortCnt = 0; masterPortCnt < EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx); masterPortCnt++)
    {
      /* #40 Ensure that no Sync/FollowUp cycle is ongoing. */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      if(ETHTSYN_SYNC_SEND_SM_PTR(EthTSyn_CfgGetMasterPortIdx(TimeDomainIdx, masterPortCnt))->State !=
          ETHTSYN_STATE_SYNC_SEND_SEND_FOLLOW_UP)
      {
        /* #50 Enable 'normal' bridge operation for the master Port. */
        /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
        ETHTSYN_SYNC_SEND_SM_PTR(EthTSyn_CfgGetMasterPortIdx(TimeDomainIdx, masterPortCnt))->State =
            ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_PENDING;
      }
    }
  }
} /* EthTSyn_SwtMgmt_HandleReceivedFupAfterTimeoutStateOfSiteSyncSyncSm() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least u8Index;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Initialize message buffers */
  for(u8Index = 0; u8Index < ETHTSYN_SWT_MGMT_MSG_BUFFER_COUNT; u8Index++)
  {
    IpBase_Fill(&EthTSyn_SwtMgmt_MsgBuffer[u8Index], 0, sizeof(EthTSyn_SwtMgmt_MsgBufferType));
  }

  /* #20 Initialize site sync sync state machine */
  for(u8Index = 0; u8Index < ETHTSYN_TIME_DOMAIN_COUNT; u8Index++)
  {
    /* #2000 Initialize site sync sync state machine state */
    if(EthTSyn_CfgGetSlavePortCnt(u8Index) <= 0)
    {
      EthTSyn_SiteSyncSyncSm(u8Index).State = ETHTSYN_STATE_SITE_SYNC_SYNC_DISABLED;
      ETHTSYN_STATE_GET_SLAVE_PORT_IDX(u8Index) = ETHTSYN_INVALID_PORT_IDX;
    }
    else
    {
      ETHTSYN_STATE_GET_SLAVE_PORT_IDX(u8Index) =
        EthTSyn_SwtMgmt_GetSwitchSlavePortIdx((EthTSyn_TimeDomainIdxType)u8Index);
      EthTSyn_SiteSyncSyncSm(u8Index).TxConfirmationPending = 0;
      EthTSyn_SiteSyncSyncSm(u8Index).State = ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING;
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_SYNC_RECEIVE_SM_PTR(ETHTSYN_STATE_GET_SLAVE_PORT_IDX(u8Index))->Sync.SyncRxTimeoutCnt =
        EthTSyn_CfgGetSyncTimeout(u8Index);
    }
  }

} /* EthTSyn_SwtMgmt_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_ProcessSmSiteSyncSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_ProcessSmSiteSyncSync(EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Implementation --------------------------------------------- */
  /* #10 Check state of the SiteSyncSync state machine. */
  switch(EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State)
  {
  case ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_SYNC:
    /* #100 Handle Send Sync state. */
    EthTSyn_SwtMgmt_HandleSendSyncStateOfSiteSyncSyncSm(TimeDomainIdx);
    break; /* END: ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_SYNC */

  case ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP:
    /* #101 Handle Send FollowUp state. */
    EthTSyn_SwtMgmt_HandleSendFollowUpStateOfSiteSyncSyncSm(TimeDomainIdx);
    break; /* END: ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP */

  case ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_FUP_AFTER_TIMEOUT:
    /* #102 Handle Received FollowUp after timeout state. */
    EthTSyn_SwtMgmt_HandleReceivedFupAfterTimeoutStateOfSiteSyncSyncSm(TimeDomainIdx);
    break; /* END: ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_FUP_AFTER_TIMEOUT */

  default:
    /* nothing to do */
    break;
  }

} /* EthTSyn_ProcessSmSitySyncSync() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_HandleMessageReception
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_HandleMessageReception(uint8 CtrlIdx, ETHTSYN_P2CONST(uint8) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SwtMgmt_MsgBufferType) msgBufferPtr;
  EthTSyn_PortIdxIterType portCnt;
  boolean foundPortForEthIfCtrl = FALSE;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if at least one (Switch-)Port is assigned to the Ctrl. */
  for(portCnt = 0; portCnt < ETHTSYN_PORT_COUNT; portCnt++)
  {
    if(EthTSyn_Util_IsSwitchPort(portCnt) == TRUE)
    {
      if(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(portCnt) == CtrlIdx)
      {
        foundPortForEthIfCtrl = TRUE;
        break;
      }
    }
  }

  if(foundPortForEthIfCtrl == TRUE)
  {
    /* At least one port is configured for the EthIf controller and the time domain --> receive the message */
    /* #20 Perform basic message checks and get a free message buffer. */
    if(EthTSyn_SwtMgmtRx_PerformBasicMsgChecksAngGetMsgBuffer(LenByte, DataPtr, &msgBufferPtr, &errorId) == E_OK)
    {
      /* #30 Map message buffer to the EthIf Ctrl and Frame */
# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      msgBufferPtr->SwtMgmtInfo.FrameId = EthTSyn_Util_GetFrameId(DataPtr, LenByte);
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
      msgBufferPtr->CtrlIdx = CtrlIdx;
      msgBufferPtr->RxBufAddr = DataPtr;

      /* #40 Handle Timestamping */
      EthTSyn_SwtMgmtRx_HandleIngressTimestamp(CtrlIdx, DataPtr, msgBufferPtr);

      /* #50 Copy the received message to the message buffer for later processing. */
      /* Copy the Message Header */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))
        &msgBufferPtr->Msg.MsgHdr,
        (ETHTSYN_P2CONST(IpBase_CopyDataType))DataPtr, ETHTSYN_HDR_COM_LENGTH);

      /* Copy the Message (Payload) */
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      IpBase_Copy((ETHTSYN_P2VAR(IpBase_CopyDataType))
        msgBufferPtr->Msg.Payload,
        (ETHTSYN_P2CONST(IpBase_CopyDataType))&DataPtr[ETHTSYN_HDR_COM_LENGTH],
        (EthTSyn_Ntohs(msgBufferPtr->Msg.MsgHdr.MessageLength) - ETHTSYN_HDR_COM_LENGTH));

      msgBufferPtr->MsgBufferState =
        ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_VALID;
    }
  }

# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID,
        ETHTSYN_SWT_MGMT_SID_SWITCH_MGMT_RX_HANDLE_MSG_RECEPTION, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

} /* EthTSyn_SwtMgmtRx_HandleMessageReception() */

/**********************************************************************************************************************
 *  EthTSyn_SwitchMgmtInfoIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwitchMgmtInfoIndication(uint8 CtrlIdx, ETHTSYN_P2CONST(uint8) DataPtr,
    ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check input parameters. */
  if(EthTSyn_State == ETHTSYN_STATE_UNINIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if(DataPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else if(MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else
# endif /* ETHTSYN_DEV_ERROR_DETECT */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Store Switch management information and trigger processing of message buffer if applicable. */
    EthTSyn_SwtMgmtRx_StoreSwitchMgmtInfo(CtrlIdx, DataPtr, MgmtInfoPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SWT_MGMT_SID_SWITCH_MGMT_INFO_INDICATION,
      errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* ETHTSYN_DEV_ERROR_REPORT */
} /* EthTSyn_SwitchMgmtInfoIndication() */

/**********************************************************************************************************************
 *  EthTSyn_SwitchEgressTimeStampIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwitchEgressTimeStampIndication(uint8 CtrlIdx, ETHTSYN_P2CONST(uint8) DataPtr,
    ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr, ETHTSYN_P2CONST(Eth_TimeStampType) timeStampPtr)
{
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters */
#  if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  if(EthTSyn_State == ETHTSYN_STATE_UNINIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if(DataPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else if(MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else
#  endif /* ETHTSYN_DEV_ERROR_DETECT */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Store the switch egress timestamp and trigger further processing if applicable. */
    EthTSyn_SwtMgmt_StoreSwitchEgressTimestamp(CtrlIdx, DataPtr, MgmtInfoPtr, timeStampPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
#  if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID,
        ETHTSYN_SWT_MGMT_SID_SWITCH_EGRESS_TIMESTAMP_INDICATION, errorId);
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(errorId);
#  endif /* ETHTSYN_DEV_ERROR_REPORT */
# else
/*(ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) */
  /* PRQA S 3112 4 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(CtrlIdx);
  ETHTSYN_DUMMY_STATEMENT(DataPtr);
  ETHTSYN_DUMMY_STATEMENT(MgmtInfoPtr);
  ETHTSYN_DUMMY_STATEMENT(timeStampPtr);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
} /* EthTSyn_SwitchEgressTimeStampIndication() */

/**********************************************************************************************************************
 *  EthTSyn_SwitchIngressTimeStampIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwitchIngressTimeStampIndication(uint8 CtrlIdx, ETHTSYN_P2CONST(uint8) DataPtr,
    ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr, ETHTSYN_P2CONST(Eth_TimeStampType) timeStampPtr)
{
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters */
#  if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  if(EthTSyn_State == ETHTSYN_STATE_UNINIT)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if(DataPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else if(MgmtInfoPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else
#  endif /* ETHTSYN_DEV_ERROR_DETECT */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Store the switch ingress timestamp and trigger further processing if applicable. */
    EthTSyn_SwtMgmt_StoreSwitchIngressTimestamp(CtrlIdx, DataPtr, MgmtInfoPtr, timeStampPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
#  if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID,
      ETHTSYN_SWT_MGMT_SID_SWITCH_INGRESS_TIMESTAMP_INDICATION, errorId);
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(errorId);
#  endif /* ETHTSYN_DEV_ERROR_REPORT */
# else
  /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_OFF) */
  /* PRQA S 3112 4 */ /* MD_MSR_14.2 */
  ETHTSYN_DUMMY_STATEMENT(CtrlIdx);
  ETHTSYN_DUMMY_STATEMENT(DataPtr);
  ETHTSYN_DUMMY_STATEMENT(MgmtInfoPtr);
  ETHTSYN_DUMMY_STATEMENT(timeStampPtr);
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
} /* EthTSyn_SwitchIngressTimeStampIndication() */
#define ETHTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHTSYN_SWT_MGMT_SUPPORT */

/* Justification for module-specific MISRA deviations:
 * see file EthTSyn.c
*/

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwtMgmt.c
 *********************************************************************************************************************/
