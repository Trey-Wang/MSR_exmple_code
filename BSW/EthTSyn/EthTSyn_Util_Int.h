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
 *         \file  EthTSyn_Util_Int.h
 *        \brief  EthTSyn Util internal header file
 *
 *      \details  Contains internal definitions and declarations used by EthTSyn
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

#if !defined(ETHTSYN_UTIL_INT_H)
# define ETHTSYN_UTIL_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_CfgAccess_Int.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if !defined (ETHTSYN_INLINE)
#  define ETHTSYN_INLINE  static INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* Provide empty definition of CANOE_WRITE_STRING Macros which are used for test purposes only */
# ifndef CANOE_WRITE_STRING
#  define CANOE_WRITE_STRING(Txt)
# endif
# ifndef CANOE_WRITE_STRING_1
#  define CANOE_WRITE_STRING_1(Txt, P1)
# endif
# ifndef CANOE_WRITE_STRING_2
#  define CANOE_WRITE_STRING_2(Txt, P1, P2)
# endif
# ifndef CANOE_WRITE_STRING_3
#  define CANOE_WRITE_STRING_3(Txt, P1, P2, P3)
# endif
# ifndef CANOE_WRITE_STRING_4
#  define CANOE_WRITE_STRING_4(Txt, P1, P2, P3, P4)
# endif

/* PRQA S 3412 MACRO_CODE */ /* MD_MSR_19.4 */
/* PRQA S 3453 FCT_MACRO */ /* MD_MSR_19.7 */
# define EthTSyn_Htons(HostShort16)                       IPBASE_HTON16(HostShort16)
# define EthTSyn_Htonl(HostLong32)                        IPBASE_HTON32(HostLong32)
# define EthTSyn_Ntohs(NetworkShort16)                    IPBASE_HTON16(NetworkShort16)
# define EthTSyn_Ntohl(NetworkLong32)                     IPBASE_HTON32(NetworkLong32)

# define ETHTSYN_PUT_UINT16(Buffer, Offset, Value)    (Buffer)[(Offset)+0] = (uint8)(((uint16_least)(Value)) >> 8u); \
                                                      (Buffer)[(Offset)+1] = (uint8)((uint16_least)(Value))

# define ETHTSYN_PUT_UINT32(Buffer, Offset, Value)    (Buffer)[(Offset)+0] = (uint8)(((uint32_least)(Value)) >> 24u); \
                                                      (Buffer)[(Offset)+1] = (uint8)(((uint32_least)(Value)) >> 16u); \
                                                      (Buffer)[(Offset)+2] = (uint8)(((uint32_least)(Value)) >>  8u); \
                                                      (Buffer)[(Offset)+3] = (uint8)((uint32_least)(Value))

# define ETHTSYN_PUT_3_BYTE(Buffer, Offset, Value)  \
  (Buffer)[(Offset)+0u] = (uint8)((((uint32_least)(Value)) >> 16u) & 0x000000FF); \
  (Buffer)[(Offset)+1u] = (uint8)((((uint32_least)(Value)) >> 8u) & 0x000000FF); \
  (Buffer)[(Offset)+2u] = (uint8)(((uint32_least)(Value)) & 0x000000FF)

# define ETHTSYN_GET_UINT16(Buffer, Offset, Variable)     IPBASE_GET_UINT16((Buffer), (Offset), (Variable))
# define ETHTSYN_GET_UINT32(Buffer, Offset, Variable)     IPBASE_GET_UINT32((Buffer), (Offset), (Variable))
# define ETHTSYN_GET_3_BYTE(Buffer, Offset, Variable)  ( (Variable) = (uint32)(((uint32)(0x00u) << 24u) | \
                                                                           ((uint32)((Buffer)[(Offset)+0u]) << 16u) | \
                                                                           ((uint32)((Buffer)[(Offset)+1u]) <<  8u) | \
                                                                           ((uint32)((Buffer)[(Offset)+2u]))) )

# define ETHTSYN_GET_FOLLOW_UP_CORRECTION_VALUE_NS_FROM_MSG(CorrectionFieldNbo)  ( (uint64)( \
  ((uint64)(CorrectionFieldNbo)[0] << 40U) | \
  ((uint64)(CorrectionFieldNbo)[1] << 32U) | \
  ((uint64)(CorrectionFieldNbo)[2] << 24U) | \
  ((uint64)(CorrectionFieldNbo)[3] << 16U) | \
  ((uint64)(CorrectionFieldNbo)[4] <<  8U) | \
  ((uint64)(CorrectionFieldNbo)[5] <<  0U)) )

# define EthTSyn_CopyFollowUpCorrectionField(FollowUpCorrectionValueNs, FollowUpCorrectionField)  \
  (FollowUpCorrectionField)[0] = (uint8)((FollowUpCorrectionValueNs) >> 40U); \
  (FollowUpCorrectionField)[1] = (uint8)((FollowUpCorrectionValueNs) >> 32U); \
  (FollowUpCorrectionField)[2] = (uint8)((FollowUpCorrectionValueNs) >> 24U); \
  (FollowUpCorrectionField)[3] = (uint8)((FollowUpCorrectionValueNs) >> 16U); \
  (FollowUpCorrectionField)[4] = (uint8)((FollowUpCorrectionValueNs) >>  8U); \
  (FollowUpCorrectionField)[5] = (uint8)((FollowUpCorrectionValueNs) >>  0U); \
  (FollowUpCorrectionField)[6] = 0; \
  (FollowUpCorrectionField)[7] = 0;

# define ETHTSYN_COPY_STBM_TS_TO_GLOBAL_TS(StbmTimestamp, GlobalTimestamp) \
  (GlobalTimestamp).secondsHi   = (StbmTimestamp).secondsHi; \
  (GlobalTimestamp).seconds     = (StbmTimestamp).seconds;   \
  (GlobalTimestamp).nanoseconds = (StbmTimestamp).nanoseconds;

# define ETHTSYN_COPY_GLOBAL_TS_TO_STBM_TS_WO_TBS(GlobalTimestamp, StbmTimestamp) \
  ETHTSYN_COPY_GLOBAL_TS_TO_STBM_TS_W_TBS((GlobalTimestamp), (StbmTimestamp), (0u))

# define ETHTSYN_COPY_GLOBAL_TS_TO_STBM_TS_W_TBS(GlobalTimestamp, StbmTimestamp, TimeBaseStatus) \
  (StbmTimestamp).timeBaseStatus = (TimeBaseStatus); \
  (StbmTimestamp).secondsHi      = (GlobalTimestamp).secondsHi; \
  (StbmTimestamp).seconds        = (GlobalTimestamp).seconds;   \
  (StbmTimestamp).nanoseconds    = (GlobalTimestamp).nanoseconds;

# define ETHTSYN_TIME_BASE_STATUS_TO_AR_SUB_TLV_STATUS(StbmTimeBaseStatus, ArSubTlvStatus) \
  ((ArSubTlvStatus) = (uint8) \
  (((StbmTimeBaseStatus) & ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK) >> ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT))

# define ETHTSYN_AR_SUB_TLV_STATUS_TO_TIME_BASE_STATUS(ArSubTlvStatus, StbmTimeBaseStatus) \
  ((StbmTimeBaseStatus) = (uint8) \
  (((ArSubTlvStatus) << ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT) & ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK))

# ifndef ETHTSYN_ENTER_CRITICAL_SECTION_0
#  define ETHTSYN_ENTER_CRITICAL_SECTION_0()   SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0()
# endif /* ETHTSYN_ENTER_CRITICAL_SECTION_0 */
# ifndef ETHTSYN_LEAVE_CRITICAL_SECTION_0
#  define ETHTSYN_LEAVE_CRITICAL_SECTION_0()   SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0()
# endif /* ETHTSYN_LEAVE_CRITICAL_SECTION_0 */
# ifndef ETHTSYN_CHECK_CRITICAL_SECTION_0
/* Macro provided by TestApp used to check if a Critical section is opened (e.g. when calling a foreign module) */
#  define ETHTSYN_CHECK_CRITICAL_SECTION_0()
# endif /* ETHTSYN_CHECK_CRITICAL_SECTION_0 */

# ifndef ETHTSYN_CALL_DET_REPORT_RUNTIME_ERROR
#  define ETHTSYN_CALL_DET_REPORT_RUNTIME_ERROR()
# endif /* ETHTSYN_CALL_DET_REPORT_RUNTIME_ERROR */

/* State Machine access Macros */
# define ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)    (ETHTSYN_CFG_GET_P_DELAY_INITIATOR_CFG_PTR((PortIdx))->PdelayReqSmPtr)
# define ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)   (ETHTSYN_CFG_GET_P_DELAY_RESPONDER_CFG_PTR((PortIdx))->PdelayRespSmPtr)

# define ETHTSYN_SYNC_RECEIVE_SM_PTR(SlavePortIdx) ((((ETHTSYN_P2CONSTCFG(EthTSyn_SlavePortConfigType)) \
    EthTSyn_PortConfig[(SlavePortIdx)].PortRolePtr)->SyncReceiveSmPtr))
# define ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(PortIdx)     (EthTSyn_SlavePortConfigPtr((PortIdx))->AnnounceReceiveSmPtr)
# define ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)            (EthTSyn_MasterPortConfigPtr((PortIdx))->SyncSendSmPtr)

/* ----- "State" access Macros ----- */
/* TimeDomain */
# define ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) (EthTSyn_TimeDomainState[(TimeDomainIdx)].SlavePortIdx)

/* Port */
# define ETHTSYN_STATE_GET_LINK_STATE(PortIdx) \
  (EthTSyn_CtrlState[ETHTSYN_CFG_GET_ETHTSYN_CTRL_IDX((PortIdx))].TrcvLinkState)
# define ETHTSYN_STATE_GET_TX_MODE(PortIdx) \
  (EthTSyn_CtrlState[ETHTSYN_CFG_GET_ETHTSYN_CTRL_IDX((PortIdx))].TransmissionMode)

/* Controller */
# define ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx) \
  (EthTSyn_CtrlState[ETHTSYN_CFG_GET_ETHTSYN_CTRL_IDX((PortIdx))].ClockIdentity)

/* PRQA L: MACRO_CODE */
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
 *  EthTSyn_Util_GetFrameId
 *********************************************************************************************************************/
/*! \brief      Retrieves the frame identifier contained in the Ethernet frame buffer
 *  \param[in]  DataPtr     Pointer to the Ethernet frame buffer
 *  \param[in]  PayloadLen  Length of the Ethernet frame payload
 *  \return     Frame identifier contained in the Ethernet frame buffer
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(EthTSyn_FrameIdType, ETHIF_CODE) EthTSyn_Util_GetFrameId(
  ETHTSYN_P2CONST(uint8)  DataPtr,
                  uint16  PayloadLen);

/**********************************************************************************************************************
 *  EthTSyn_UtilTx_PrepareCommonMsgHdr
 *********************************************************************************************************************/
/*! \brief      Initializes the common message header
 *  \param[in]  ComMsgHdrPtr    Pointer to the common message header to be initialized
 *  \param[in]  MsgType         The type of the message the header should be prepared for
 *  \param[in]  PortIdx         Index of the EthTSyn Port the message will be transmitted on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_UtilTx_PrepareCommonMsgHdr(
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr,
                uint8                     MsgType,
                EthTSyn_PortIdxType       PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_UtilTx_HtonTimestamp
 *********************************************************************************************************************/
/*! \brief       Sets a local time representation to EthTSyn timestamp of EthTSyn messages
 *  \param[out]  GlobalTimestampPtr    Pointer to the EthTSyn timestamp value (destination)
 *  \param[in]   LocalTimestampPtr     Pointer to the local timestamp value (source)
 *  \context     ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_UtilTx_HtonTimestamp(
    ETHTSYN_P2VAR(EthTSyn_TimestampType)       GlobalTimestampPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) LocalTimestampPtr);

/**********************************************************************************************************************
 *  EthTSyn_Util_TimestampPlusTimediff
 *********************************************************************************************************************/
/*! \brief          Calculates the sum of a timestamps and a time difference
 *  \param[in,out]  DestTimestampPtr       Pointer to timestamp where the first summand is stored and the
 *                                       sum is written
 *  \param[in]      TimediffPtr            Pointer the value where the time difference is stored
 *  \return         E_OK - Sum of added timestamps is valid
 *  \return         E_NOT_OK - Sum of added timestamps is invalid
 *  \context        ANY
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_TimestampPlusTimediff(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) DestTimestampPtr,
  ETHTSYN_P2CONST(EthTSyn_TimediffType)        TimediffPtr);

/**********************************************************************************************************************
 *  EthTSyn_Util_TimestampMinusTimestamp
 *********************************************************************************************************************/
/*! \brief         Calculates the time difference between Timestamp1 and Timestamp2 (diff = Timestamp1 - Timestamp2)
 *  \param[in]     Timestamp1Ptr          Pointer to timestamp of the first timestamp
 *  \param[in]     Timestamp2Ptr          Pointer to timestamp of the second timestamp
 *  \param[out]    TimediffPtr            Pointer to time difference between Timestamp1 and Timestamp2
 *  \return        E_OK - Timediff is valid
 *  \return        E_NOT_OK - Timediff is invalid
 *  \context       ANY
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_TimestampMinusTimestamp(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Timestamp1Ptr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Timestamp2Ptr,
    ETHTSYN_P2VAR(EthTSyn_TimediffType)        TimediffPtr);

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_CheckMessageGeneral
 *********************************************************************************************************************/
/*! \brief      Perform general message checks
 *  \param[in]  MsgPtr           Pointer to the message to be checked
 *  \param[in]  LenByte          Length of the message in Byte
 *  \param[out] ErrorIdPtr       If the a general message check fails, the corresponding errorId will be stored here
 *  \return     E_OK - Passed message check
 *  \return     E_NOT_OK - Message check failed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_CheckMessageGeneral(
    ETHTSYN_P2CONST(uint8)                     MsgPtr,
                    uint16                     LenByte,
      ETHTSYN_P2VAR(uint8)                     ErrorIdPtr);

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_CheckMessageTypeAndLength
 *********************************************************************************************************************/
/*! \brief      Checks if the frame length and the message length of the common message header matches the message type
 *              Checks if the message type is valid and if the frame length and message length of the common header
 *              matches the message type
 *  \param[in]  ComMsgHdrPtr    Pointer to the common message header to be checked
 *  \param[in]  LenByte         Length of the message in Byte
 *  \param[out] ErrorIdPtr      If the a message check fails, the corresponding errorId will be stored here
 *  \return     E_OK - Passed message check
 *  \return     E_NOT_OK - Message check failed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_CheckMessageTypeAndLength(
    ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr,
                    uint16                    LenByte,
      ETHTSYN_P2VAR(uint8)                    ErrorIdPtr);

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_QualifyAnnounceMsg
 *********************************************************************************************************************/
/*! \brief      Validation of Announce message
 *  \param[in]  PortIdx         Index of EthTSyn port that received the Announce message
 *  \param[in]  ComMsgPtr       Pointer to the EthTSyn common header of the Announce message
 *  \return     E_OK - Announce message is valid
 *  \return     E_NOT_OK - Announce message is not valid
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_QualifyAnnounceMsg(
                  EthTSyn_PortIdxType       PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr);

/**********************************************************************************************************************
 *  GLOBAL INLINE FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Util_IsTimestampAvailable
 *********************************************************************************************************************/
/*! \brief      Check if the Timestamp of the given Timestamp state is available.
 *  \param[in]  TimestampState:    State of the Timestamp to check
 *  \return     TRUE - Timestamp is available
 *  \return     FALSE - Timestamp is not available
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Util_IsTimestampAvailable(
  EthTSyn_TimestampStateType TimestampState);

/**********************************************************************************************************************
 *  EthTSyn_Util_GetPortType
 *********************************************************************************************************************/
/*! \brief      Get the Port type of the given Port
 *  \param[in]  PortIdx:      Index of the Port
 *  \return     ETHTSYN_PORT_TYPE_END_STATION_PORT - End-Station Port
 *  \return     ETHTSYN_PORT_TYPE_SWITCH_PORT - Switch-Port
 *  \return     ETHTSYN_PORT_TYPE_SWITCH_HOST_PORT - Host-Port of the Switch
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_INLINE FUNC(EthTSyn_PortTypeType, ETHTSYN_CODE) EthTSyn_Util_GetPortType(
  EthTSyn_PortIdxType PortIdx);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Util_IsSwitchPort
 *********************************************************************************************************************/
/*! \brief      Check if the given Port is a Switch-Port
 *  \param[in]  PortIdx:  Index of the Port
 *  \return     TRUE - Port is a Switch-Port
 *  \return     FALSE - Port is not a Switch-Port
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Util_IsSwitchPort(
  EthTSyn_PortIdxType PortIdx);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId
 *********************************************************************************************************************/
/*! \brief      If the given Port is a Switch port, the corresponding switch management information will be returned,
 *              include the frame id if applicable.
 *  \param[in]  PortIdx:      Index of the Port
 *  \param[in]  LenByte:      LenByte of the corresponding TxBuffer
 *  \param[in]  TxBufPtr:     Pointer to the TxBuffer
 *  \param[out] MgmtInfoPtr:  The retrieved switch management information including the frame id if applicable
 *  \return     E_OK - Port is a Switch-Port and switch management information was retrieved.
 *  \return     E_NOT_OK - Port is not a Switch-Port
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId(
                  EthTSyn_PortIdxType  PortIdx,
                  uint16               LenByte,
  ETHTSYN_P2CONST(uint8)               TxBufPtr,
    ETHTSYN_P2VAR(EthSwt_MgmtInfoType) MgmtInfoPtr);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_IsPortReadyToReceiveAnnounce
 *********************************************************************************************************************/
/*! \brief      Checks if the given Port is ready to receive an announce message (including check of PortRole == SLAVE)
 *  \param[in]  PortIdx         Index of EthTSyn port that shall receive the announce message
 *  \return     TRUE - Port is ready to receive the announce message
 *  \return     FALSE - Port is not ready to receive the announce message
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_UtilRx_IsPortReadyToReceiveAnnounce(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Util_GetAsCapable
 *********************************************************************************************************************/
/*! \brief      Check if the given Port is AsCapable
 *  \param[in]  PortIdx  Index of the Port
 *  \return     TRUE - Port is AsCapable
 *  \return     FALSE - Port is not AsCapable
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Util_GetAsCapable(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Util_SetAsCapable
 *********************************************************************************************************************/
/*! \brief      Set AsCapable of the given Port
 *  \param[in]  PortIdx    Index of the Port
 *  \param[in]  AsCapable  The new value for AsCapable of the given Port
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Util_SetAsCapable(
  EthTSyn_PortIdxType PortIdx,
  boolean             AsCapable);

/**********************************************************************************************************************
 *  GLOBAL INLINE FUNCTIONS
 *********************************************************************************************************************/
# if !defined(ETHTSYN_UNIT_TEST) /* COV_ETHTSYN_TESTSUITE_INSTRUMENTATION */
/**********************************************************************************************************************
 *  EthTSyn_Util_IsTimestampAvailable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Util_IsTimestampAvailable(
  EthTSyn_TimestampStateType TimestampState)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean tsAvailable = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if((TimestampState & ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK) != 0)
  {
    if((TimestampState & ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK) != 0)
    {
      tsAvailable = TRUE;
    }
  }

  return tsAvailable;
} /* EthTSyn_Util_IsTimestampAvailable() */

/**********************************************************************************************************************
 *  EthTSyn_Util_GetPortType
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_INLINE FUNC(EthTSyn_PortTypeType, ETHTSYN_CODE) EthTSyn_Util_GetPortType(
    EthTSyn_PortIdxType PortIdx)
{

  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortTypeType portType = ETHTSYN_PORT_TYPE_END_STATION_PORT;

  /* ----- Implementation ----------------------------------------------- */
  if ( (EthTSyn_PortConfig[PortIdx].SwtInfo.SwitchIdx != ETHTSYN_INVALID_ETHIF_SWITCH_IDX) &&
       (EthTSyn_PortConfig[PortIdx].SwtInfo.PortIdx != ETHTSYN_INVALID_SWITCH_PORT_IDX) )
  {
    portType = ETHTSYN_PORT_TYPE_SWITCH_PORT;
  }

  return portType;
} /* EthTSyn_Util_GetPortType() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Util_IsSwitchPort
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Util_IsSwitchPort(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isSwitchPort = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if(EthTSyn_Util_GetPortType(PortIdx) == ETHTSYN_PORT_TYPE_SWITCH_PORT)
  {
    isSwitchPort = TRUE;
  }

  return isSwitchPort;
} /* EthTSyn_Util_IsSwitchPort() */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId(EthTSyn_PortIdxType PortIdx,
  uint16 LenByte, ETHTSYN_P2CONST(uint8) TxBufPtr, ETHTSYN_P2VAR(EthSwt_MgmtInfoType) MgmtInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(EthTSyn_Util_IsSwitchPort(PortIdx) == TRUE)
  {
    MgmtInfoPtr->PortIdx = ETHTSYN_CFG_GET_SWITCH_PORT_IDX(PortIdx);
    MgmtInfoPtr->SwitchIdx = ETHTSYN_CFG_GET_SWITCH_IDX(PortIdx);
#   if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
    MgmtInfoPtr->FrameId = EthTSyn_Util_GetFrameId(TxBufPtr, LenByte);
#   else
    ETHTSYN_DUMMY_STATEMENT(LenByte);
    ETHTSYN_DUMMY_STATEMENT(TxBufPtr);
#   endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_UtilTx_GetSwtMgmtInfoAndFrameId() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_IsPortReadyToReceiveAnnounce
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_UtilRx_IsPortReadyToReceiveAnnounce(
  EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isReady = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the given Port is a Slave Port. */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_SLAVE)
  {
    /* #20 Check if Port is AsCapable. */
    if(EthTSyn_Util_GetAsCapable(PortIdx) == TRUE)
    {
      /* #30 Check if Announce is enabled for the Port. */
      if(EthTSyn_CfgGetEnableAnnounce(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        isReady = TRUE;
      }
    }
  }

  return isReady;
} /* EthTSyn_SlaveRx_IsPortReadyToReceiveAnnounce() */

/**********************************************************************************************************************
 *  EthTSyn_Util_GetAsCapable
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Util_GetAsCapable(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean asCapable = TRUE; /* Init as TRUE for better readability */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is pdelay initiator because AsCapble can only be FALSE when Port is pdelay initiator. */
  if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
  {
    /* #20 Port is Pdelay initiator -> Check AsCapable status. */
    asCapable = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->AsCapable;
  }

  return asCapable;
} /* EthTSyn_Util_GetAsCapable() */

/**********************************************************************************************************************
 *  EthTSyn_Util_SetAsCapable
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Util_SetAsCapable(EthTSyn_PortIdxType PortIdx, boolean AsCapable)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Port is pdelay initiator because AsCapble can only change for Pdelay initiators. */
  if(EthTSyn_CfgAccess_IsPdelayInitiator(PortIdx) == TRUE)
  {
    /* #20 Verify that Port is not always AsCapable. */
    if(ETHTSYN_CFG_GET_ALWAYS_AS_CAPABLE(PortIdx) == FALSE)
    {
      /* #30 Only set AsCapble for Ports which are not always AsCapable. */
      ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->AsCapable = AsCapable;
    }
  }
} /* EthTSyn_Util_SetAsCapable() */

# endif /* !defined(ETHTSYN_UNIT_TEST) */
# define ETHTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* ETHTSYN_UTIL_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Util_Int.h
 *********************************************************************************************************************/
