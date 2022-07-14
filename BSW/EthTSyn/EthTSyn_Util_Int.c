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
/**        \file  EthTSyn_Util_Int.c
 *        \brief  EthTSyn Util source file - Internal Functions
 *
 *      \details  EthTSyn Util source file containing the implementation of utilities used by different Sub-Modules of
 *                of the EthTSyn
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

#define ETHTSYN_UTIL_INT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Util_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
#include "EthTSyn_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of EthTSyn header file */
#if (  (ETHTSYN_SW_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_SW_MINOR_VERSION != (1u)) \
    || (ETHTSYN_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthTSyn_Util_Int.c and EthTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ETHTSYN_CFG_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_CFG_MINOR_VERSION != (1u)) )
# error "Version numbers of EthTSyn_Util_Int.c and EthTSyn_Cfg.h are inconsistent!"
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
 *  EthTSyn_UtilRx_GetExpectedMsgLengthWithMsgTypeCheck
 *********************************************************************************************************************/
/*! \brief      Get the expected message length and verify the message type of the given message
 *  \param[in]  ComMsgPtr                     Pointer to the message to be checked
 *  \param[out] ExpectedLengthPtr             The expected message length
 *  \param[out] IsExpectedLengthMinLengthPtr  TRUE:  The expected message length is just the minimal length
 *                                            FALSE: The expected message length has to match exactly
 *  \param[out] ErrorIdPtr                    If any check fails, the errorId will be stored here
 *  \return     E_OK - Message type valid and expected length successfully retrieved
 *  \return     E_NOT_OK - Message type invalid or failed to retrieve expected length
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_GetExpectedMsgLengthWithMsgTypeCheck(
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr,
    ETHTSYN_P2VAR(uint16)                    ExpectedLengthPtr,
    ETHTSYN_P2VAR(boolean)                   IsExpectedLengthMinLengthPtr,
    ETHTSYN_P2VAR(uint8)                     ErrorIdPtr);

/**********************************************************************************************************************
 *  EthTSyn_Util_AddTimediffToTimestamp
 *********************************************************************************************************************/
/*! \brief         Adds the given time difference to the given timestamp
 *  \param[in]     TimeDiffNs    The time difference in nanoseconds
 *  \param[in,out] TimestampPtr  In:   The timestamp to be incremented by the given time difference
 *                               Out:  The given Timestamp + the given time difference
 *  \return        E_OK - Successfully added the time difference to the timestamp
 *  \return        E_NOT_OK - Failed to added the time difference to the timestamp
 *  \context       ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_AddTimediffToTimestamp(
                  uint64                          TimeDiffNs,
    ETHTSYN_P2VAR(EthTSyn_InternalTimestampType)  TimestampPtr);

/**********************************************************************************************************************
 *  EthTSyn_Util_SubtractTimediffFromTimestamp
 *********************************************************************************************************************/
/*! \brief         Subtracts the given time difference to the given timestamp
 *  \param[in]     TimeDiffNs    The time difference in nanoseconds
 *  \param[in,out] TimestampPtr  In:   The timestamp to be decremented by the given time difference
 *                              Out:  The given Timestamp - the given time difference
 *  \return        E_OK - Successfully subtracted the time difference from the timestamp
 *                 E_NOT_OK - Failed to subtract the time difference from the timestamp
 *  \context       ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_SubtractTimediffFromTimestamp(
                  uint64                          TimeDiffNs,
    ETHTSYN_P2VAR(EthTSyn_InternalTimestampType)  TimestampPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

 /**********************************************************************************************************************
  *  EthTSyn_UtilRx_GetExpectedMsgLengthWithMsgTypeCheck
  *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
 ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_GetExpectedMsgLengthWithMsgTypeCheck(
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, ETHTSYN_P2VAR(uint16) ExpectedLengthPtr,
  ETHTSYN_P2VAR(boolean) IsExpectedLengthMinLengthPtr, ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
   /* ----- Local Variables ---------------------------------------------- */
   Std_ReturnType retVal = E_OK;

   /* ----- Implementation ----------------------------------------------- */
   *ExpectedLengthPtr = 0;
   *IsExpectedLengthMinLengthPtr = FALSE;

   /* #10 Check the message type. */
   switch(ETHTSYN_HDR_COM_MSG_TYPE(ComMsgPtr->TransSpecific_MessageType))
   {
   /* #100 Set the expected length for valid message types. */
   case ETHTSYN_MSG_TYPE_SYNC:
     *ExpectedLengthPtr = ETHTSYN_MSG_SYNC_LENGTH;
     break;

   case ETHTSYN_MSG_TYPE_FOLLOW_UP:
     *ExpectedLengthPtr = ETHTSYN_MSG_FOLLOW_UP_RX_MIN_LENGTH;
     *IsExpectedLengthMinLengthPtr = TRUE;
     break;

   case ETHTSYN_MSG_TYPE_PDELAY_REQ:
     *ExpectedLengthPtr = ETHTSYN_MSG_P_DELAY_REQ_LENGTH;
     break;

   case ETHTSYN_MSG_TYPE_PDELAY_RESP:
     *ExpectedLengthPtr = ETHTSYN_MSG_P_DELAY_RESP_LENGTH;
     break;

   case ETHTSYN_MSG_TYPE_PDELAY_RES_FOLLOW_UP:
     *ExpectedLengthPtr = ETHTSYN_MSG_P_DELAY_RESP_FOLLOW_UP_LENGTH;
     break;

   case ETHTSYN_MSG_TYPE_ANNOUNCE:
     *ExpectedLengthPtr = ETHTSYN_MSG_ANNOUNCE_LENGTH;
     break;

   default:
     /* #101 Invalid message type. Set the errorId and return with E_NOT_OK. */
     *ErrorIdPtr = ETHTSYN_E_INV_MSG_TYPE;
     retVal = E_NOT_OK;
     break;
   }

   return retVal;
} /* EthTSyn_UtilRx_GetExpectedMsgLengthWithMsgTypeCheck() */

/**********************************************************************************************************************
 *  EthTSyn_Util_AddTimediffToTimestamp
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
  */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_AddTimediffToTimestamp(uint64 TimeDiffNs,
  ETHTSYN_P2VAR(EthTSyn_InternalTimestampType) TimestampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_UTimediffType DiffSeconds;
  uint32 DiffNanoseconds;

  /* ----- Implementation ----------------------------------------------- */
  DiffSeconds = (EthTSyn_UTimediffType)(TimeDiffNs / ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);
  DiffNanoseconds = (uint32)(TimeDiffNs % ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);

  /* #10 Overflow check of rounded up seconds -> Check nanoseconds overflow not required */
  if((EthTSyn_UTimediffType)((TimestampPtr->seconds + 1) + DiffSeconds) <=
      (EthTSyn_UTimediffType)ETHTSYN_TIMESTAMP_MAX_SECONDS)
  {
    /* #20 Add nanoseconds of both summands */
    TimestampPtr->nanoseconds += (uint32)DiffNanoseconds;

    /* #30 Add seconds of overflow nanoseconds */
    TimestampPtr->seconds += (uint64)((uint64)TimestampPtr->nanoseconds /
        (uint64)ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);

    /* #40 Truncate overflow nanoseconds */
    TimestampPtr->nanoseconds = TimestampPtr->nanoseconds % ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;

    /* #50 Add seconds */
    TimestampPtr->seconds += (EthTSyn_UTimediffType)DiffSeconds;

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_Util_AddTimediffToTimestamp() */

/**********************************************************************************************************************
 *  EthTSyn_Util_SubtractTimediffFromTimestamp
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_SubtractTimediffFromTimestamp(uint64 TimeDiffNs,
  ETHTSYN_P2VAR(EthTSyn_InternalTimestampType) TimestampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_UTimediffType DiffSeconds;
  uint32 DiffNanoseconds;

  /* ----- Implementation ----------------------------------------------- */
  DiffSeconds = (EthTSyn_UTimediffType)(TimeDiffNs / ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);
  DiffNanoseconds = (uint32)(TimeDiffNs % ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);

  /* #10 Check that result is not negative */
  if(DiffSeconds <= TimestampPtr->seconds)
  {
    /* #20 Check for Nanoseconds underflow */
    if(DiffNanoseconds <= TimestampPtr->nanoseconds)
    {
      /* #200 Calculate timestamp and mark resulat as valid */
      TimestampPtr->seconds -= DiffSeconds;
      TimestampPtr->nanoseconds -= DiffNanoseconds;

      retVal = E_OK;
    }
    else
    {
      /* #30 Handle Underflow. Destination seconds must be greater than difference seconds */
      if(TimestampPtr->seconds > DiffSeconds)
      {
        /* #300 Calculate timestamp and mark result as valid */
        TimestampPtr->seconds--;
        TimestampPtr->nanoseconds =
            (uint32)(((ETHTSYN_TIMESTAMP_MAX_NANOSECONDS + TimestampPtr->nanoseconds) - DiffNanoseconds));
        TimestampPtr->seconds -= DiffSeconds;

        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_Util_SubtractTimediffFromTimestamp() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Util_GetFrameId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(EthTSyn_FrameIdType, ETHIF_CODE) EthTSyn_Util_GetFrameId(ETHTSYN_P2CONST(uint8) DataPtr,
  uint16 PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(uint8) frameIdPtr;
  EthTSyn_FrameIdType frameId = ETHTSYN_INVALID_FRAME_ID;
  uint8_least frameIdByteIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 1 */ /* Byte access needed to avoid mis-aligned access on specific platforms. */
  frameIdPtr = (ETHTSYN_P2VAR(uint8))&frameId;
  for( frameIdByteIdx = 0; frameIdByteIdx < sizeof(EthTSyn_FrameIdType); frameIdByteIdx++ )
  {
    frameIdPtr[frameIdByteIdx] = DataPtr[PayloadLen + frameIdByteIdx];
  }

  return frameId;
} /* EthTSyn_Util_GetFrameId() */

/**********************************************************************************************************************
 *  EthTSyn_UtilTx_PrepareCommonMsgHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_UtilTx_PrepareCommonMsgHdr(
  ETHTSYN_P2VAR(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr, uint8 MsgType, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 msgLength = 0;
  uint16 sequenceId = 0;
  uint8 comHdrControl = 0;
  sint8 logMessageInterval = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get message specific values of the common message header from Message type. */
  switch(MsgType)
  {
  case ETHTSYN_MSG_TYPE_SYNC:
    msgLength = ETHTSYN_MSG_SYNC_LENGTH;
    sequenceId = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SequenceId; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    comHdrControl = ETHTSYN_HDR_COM_CONTROL_SYNC;
    logMessageInterval = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->LogMessageInterval; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    break;

  case ETHTSYN_MSG_TYPE_FOLLOW_UP:
    msgLength = ETHTSYN_CFG_GET_FOLLOW_UP_MSG_LENGTH_TX(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    sequenceId = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SequenceId; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    comHdrControl = ETHTSYN_HDR_COM_CONTROL_FOLLOW_UP;
    logMessageInterval = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->LogMessageInterval; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_REQ:
    msgLength = ETHTSYN_MSG_P_DELAY_REQ_LENGTH;
    sequenceId = ETHTSYN_P_DELAY_REQ_SM_PTR(PortIdx)->PdelayReq.SequenceId;
    comHdrControl = ETHTSYN_HDR_COM_CONTROL_PDELAY_REQ;
    logMessageInterval = ETHTSYN_CFG_GET_P_DELAY_REQ_LOG_MESSAGE_INTERVAL(PortIdx);
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_RESP:
    msgLength = ETHTSYN_MSG_P_DELAY_RESP_LENGTH;
    sequenceId = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayReq.SequenceId;
    comHdrControl = ETHTSYN_HDR_COM_CONTROL_PDELAY_RESP;
    logMessageInterval = ETHTSYN_HDR_COM_LOG_MESSAGE_INTERVAL_DEFAULT;
    break;

  case ETHTSYN_MSG_TYPE_PDELAY_RES_FOLLOW_UP:
    msgLength = ETHTSYN_MSG_P_DELAY_RESP_FOLLOW_UP_LENGTH;
    sequenceId = ETHTSYN_P_DELAY_RESP_SM_PTR(PortIdx)->PdelayReq.SequenceId;
    comHdrControl = ETHTSYN_HDR_COM_CONTROL_PDELAY_RESP_FOLLOW_UP;
    logMessageInterval = ETHTSYN_HDR_COM_LOG_MESSAGE_INTERVAL_DEFAULT;
    break;

  case ETHTSYN_MSG_TYPE_ANNOUNCE:
    msgLength = ETHTSYN_MSG_ANNOUNCE_LENGTH;
    sequenceId = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->SequenceId; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    comHdrControl = ETHTSYN_HDR_COM_CONTROL_ANNOUNCE;
    logMessageInterval = ETHTSYN_SYNC_SEND_SM_PTR(PortIdx)->LogMessageInterval; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    break;

  default:
    /* This should never happen. Nothing to do (MISRA) */
    break;
  }

  /* #20 Set common message header attributes. */
  ComMsgHdrPtr->TransSpecific_MessageType = (uint8)(ETHTSYN_HDR_COM_TRANSPORT_SPECIFIC | MsgType);
  ComMsgHdrPtr->Reserved_VersionGPtp = ETHTSYN_PROTOCOL_VERSION;

  ComMsgHdrPtr->MessageLength = (uint16)EthTSyn_Htons(msgLength);
  ComMsgHdrPtr->DomainNumber = EthTSyn_PortConfig[PortIdx].TimeDomainId;

  ComMsgHdrPtr->Reserved_0 = 0;
  ComMsgHdrPtr->Flags = EthTSyn_Htons(ETHTSYN_FLAGS_TWO_STEP);  /*lint !e572 */

  if(MsgType != ETHTSYN_MSG_TYPE_FOLLOW_UP)
  {
    /* Correction field only used for FollowUp messages */
    IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))ComMsgHdrPtr->CorrectionField, 0,
      sizeof(EthTSyn_CorrectionFieldType));
  }

  IpBase_Fill((ETHTSYN_P2VAR(IpBase_CopyDataType))ComMsgHdrPtr->Reserved_1, 0, ETHTSYN_HDR_COM_RESERVED_1_LENGTH);
  
  EthTSyn_SetClockIdentity(ComMsgHdrPtr->SourceClockIdentity, ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx));
  ComMsgHdrPtr->SourcePortNumber = (uint16)EthTSyn_Htons(ETHTSYN_CFG_GET_PORT_NUMBER(PortIdx));

  ComMsgHdrPtr->SequenceId = (uint16)EthTSyn_Htons(sequenceId);
  ComMsgHdrPtr->Control = comHdrControl;
  ComMsgHdrPtr->LogMessageInterval = logMessageInterval;

} /* EthTSyn_UtilTx_PrepareCommonMsgHdr() */

/***********************************************************************************************************************
 *  EthTSyn_UtilTx_HtonTimestamp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_UtilTx_HtonTimestamp(ETHTSYN_P2VAR(EthTSyn_TimestampType) GlobalTimestampPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) LocalTimestampPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  GlobalTimestampPtr->NanoSeconds[0] = (uint8)(LocalTimestampPtr->nanoseconds >> 24U);
  GlobalTimestampPtr->NanoSeconds[1] = (uint8)(LocalTimestampPtr->nanoseconds >> 16U);
  GlobalTimestampPtr->NanoSeconds[2] = (uint8)(LocalTimestampPtr->nanoseconds >> 8U);
  GlobalTimestampPtr->NanoSeconds[3] = (uint8)(LocalTimestampPtr->nanoseconds >> 0U);

  GlobalTimestampPtr->Seconds[0] = (uint8)(LocalTimestampPtr->secondsHi >> 8U);
  GlobalTimestampPtr->Seconds[1] = (uint8)(LocalTimestampPtr->secondsHi >> 0U);

  GlobalTimestampPtr->Seconds[2] = (uint8)(LocalTimestampPtr->seconds >> 24U);
  GlobalTimestampPtr->Seconds[3] = (uint8)(LocalTimestampPtr->seconds >> 16U);
  GlobalTimestampPtr->Seconds[4] = (uint8)(LocalTimestampPtr->seconds >> 8U);
  GlobalTimestampPtr->Seconds[5] = (uint8)(LocalTimestampPtr->seconds >> 0U);

} /* EthTSyn_UtilTx_HtonTimestamp() */

/**********************************************************************************************************************
 *  EthTSyn_Util_TimestampPlusTimediff
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_TimestampPlusTimediff(
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) DestTimestampPtr,
  ETHTSYN_P2CONST(EthTSyn_TimediffType) TimediffPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_InternalTimestampType internalTimestamp;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that DestTimestampPtr and TimediffPtr are valid. */
  if((DestTimestampPtr != NULL_PTR) && (TimediffPtr != NULL_PTR))
  {
    /* #20 Convert Global to internal Timestamp. */
    internalTimestamp.nanoseconds = DestTimestampPtr->nanoseconds;
    internalTimestamp.seconds     = DestTimestampPtr->seconds;
    internalTimestamp.seconds    |= ((uint64)DestTimestampPtr->secondsHi << 32u);

    /* #30 Check for positive value of time difference. */
    if(*TimediffPtr > 0)
    {
      /* #300 Positive Timediff. Add Timediff to timestamp. */
      EthTSyn_UTimediffType TimediffU64Ns = (EthTSyn_UTimediffType)(*TimediffPtr);
      retVal = EthTSyn_Util_AddTimediffToTimestamp(TimediffU64Ns, &internalTimestamp);
    }
    else if(*TimediffPtr < 0)
    {
      /* #301 Negative Timediff. Subtract Timediff from timestamp. */
      EthTSyn_UTimediffType TimediffU64Ns = (EthTSyn_UTimediffType)(-(*TimediffPtr));
      retVal = EthTSyn_Util_SubtractTimediffFromTimestamp(TimediffU64Ns, &internalTimestamp);
    }
    else
    {
      /* #302 Timediff is zero -> add nothing. */
      retVal = E_OK;
    }

    /* The seconds field of the internal timestamp has a value range from 0 - (2^48-1) */
    /* #40 Check if result is valid. */
    if(retVal == E_OK)
    {
      /* #400 Convert Internal to global Timestamp. */
      DestTimestampPtr->nanoseconds = internalTimestamp.nanoseconds;
      DestTimestampPtr->seconds     = (uint32)internalTimestamp.seconds;
      DestTimestampPtr->secondsHi   = (uint16)(internalTimestamp.seconds >> 32U);
    }
  }

  return retVal;
} /* EthTSyn_Util_TimestampPlusTimediff() */

/**********************************************************************************************************************
 *  EthTSyn_Util_TimestampMinusTimestamp
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Util_TimestampMinusTimestamp(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Timestamp1Ptr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Timestamp2Ptr,
  ETHTSYN_P2VAR(EthTSyn_TimediffType) TimediffPtr)
 {
   /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_TimediffType diffSeconds;
  EthTSyn_TimediffType diffNanoseconds;
  EthTSyn_UTimediffType diffSecondsU64;
  EthTSyn_InternalTimestampType internalTimestamp1;
  EthTSyn_InternalTimestampType internalTimestamp2;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert Global to internal Timestamp. */
  internalTimestamp1.nanoseconds = Timestamp1Ptr->nanoseconds;
  internalTimestamp1.seconds     = Timestamp1Ptr->seconds;
  internalTimestamp1.seconds    |= ((uint64)Timestamp1Ptr->secondsHi << 32U);

  internalTimestamp2.nanoseconds = Timestamp2Ptr->nanoseconds;
  internalTimestamp2.seconds     = Timestamp2Ptr->seconds;
  internalTimestamp2.seconds    |= ((uint64)Timestamp2Ptr->secondsHi << 32U);

  /* #20 Check if difference in seconds is suitable for nanoseconds difference value.
   * No overflow is expected due Seconds field of EthTSyn_TimestampType should not exceed 2^48. */
  diffSeconds = (EthTSyn_TimediffType)(internalTimestamp1.seconds - internalTimestamp2.seconds);

  /* check if time difference is less zero */
  if(diffSeconds < 0)
  {
    diffSecondsU64 = (EthTSyn_UTimediffType)(-diffSeconds);
  }
  /* time difference is greater zero */
  else
  {
    diffSecondsU64 = (EthTSyn_UTimediffType)diffSeconds;
  }

  /* #30 Round up difference in seconds due to handle possible nanoseconds overflow in further calculation. */
  diffSecondsU64++;
  /* #40 Check if time difference in seconds is suitable for nanoseconds difference value. */
  if((EthTSyn_UTimediffType)diffSecondsU64 < (EthTSyn_UTimediffType)ETHTSYN_TIMEDIFF_MAX_SECONDS)
  {
    /* #50 Calculate time difference. */
    /* calculate correct nanoseconds time difference */
    diffNanoseconds = (EthTSyn_TimediffType)
            ((EthTSyn_TimediffType)internalTimestamp1.nanoseconds - (EthTSyn_TimediffType)internalTimestamp2.nanoseconds);

    /* set seconds value */
    *TimediffPtr = diffSeconds * ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;

    /* add nanoseconds value */
    *TimediffPtr = *TimediffPtr + diffNanoseconds;

    retVal = E_OK;
  }
  /* timediff value exceeds max nanoseconds value */
  else
  {
    *TimediffPtr = 0;
  }

  return retVal;
} /* EthTSyn_Util_TimestampMinusTimestamp() */

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_CheckMessageGeneral
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_CheckMessageGeneral(ETHTSYN_P2CONST(uint8) MsgPtr,
    uint16 LenByte, ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) comMsgHdrPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Message length sanity check */
  if(LenByte < ETHTSYN_HDR_COM_LENGTH)
  {
    /* Invalid message length, message is smaller than common header --> drop message */
    /* Note: This should never happen since the common header length is 34 while the min Eth Frame Payload size is 46 */
    *ErrorIdPtr = ETHTSYN_E_INV_MSG_LENGTH;
  }
  else
  {
    /* Get the common message hdr */
    comMsgHdrPtr = (ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType))&MsgPtr[0]; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #20 Check header message length vs. LenByte */
    if((uint16)EthTSyn_Ntohs(comMsgHdrPtr->MessageLength) > LenByte)
    {
      *ErrorIdPtr = ETHTSYN_E_INV_MSG_LENGTH;
    }
    /* #30 Check EthTSyn Protocol version */
    else if(ETHTSYN_HDR_COM_VERSION(comMsgHdrPtr->Reserved_VersionGPtp) != ETHTSYN_PROTOCOL_VERSION)
    {
      /* Invalid EthTSyn protocol version --> drop message */
      *ErrorIdPtr = ETHTSYN_E_INV_PROTOCOL_VERSION;
    }
    else
    {
      /* #40 Check TimeDomainId */
      if(EthTSyn_CfgAccess_GetTimeDomainIdx(comMsgHdrPtr->DomainNumber) < ETHTSYN_TIME_DOMAIN_COUNT)
      {
        retVal = E_OK;
      }
      else
      {
        *ErrorIdPtr = ETHTSYN_E_INV_DOMAIN_NUMBER;
      }
    }
  }

  return retVal;
} /* EthTSyn_UtilRx_CheckMessageGeneral() */

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_CheckMessageTypeAndLength
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_CheckMessageTypeAndLength(
    ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr, uint16 LenByte, ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint16 expectedMsgLength = 0;
  boolean expectedMsgLengthIsMinLength = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to get the expected message length and validate message type. */
  retVal = EthTSyn_UtilRx_GetExpectedMsgLengthWithMsgTypeCheck(ComMsgHdrPtr, &expectedMsgLength,
      &expectedMsgLengthIsMinLength, ErrorIdPtr);

  if((retVal == E_OK) && (expectedMsgLength > 0))
  {
    /* #20 Verify message length. */
    uint16 msgLengthHeader = (uint16)EthTSyn_Ntohs(ComMsgHdrPtr->MessageLength);
    if(expectedMsgLengthIsMinLength == TRUE)
    {
      if( (LenByte < expectedMsgLength) || (msgLengthHeader < expectedMsgLength) )
      {
        *ErrorIdPtr = ETHTSYN_E_INV_MSG_LENGTH;
        retVal = E_NOT_OK;
      }
    }
    else
    {
      if( (LenByte < expectedMsgLength) || (msgLengthHeader != expectedMsgLength) )
      {
        *ErrorIdPtr = ETHTSYN_E_INV_MSG_LENGTH;
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_UtilRx_CheckMessageTypeAndLength() */

/**********************************************************************************************************************
 *  EthTSyn_UtilRx_QualifyAnnounceMsg
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_UtilRx_QualifyAnnounceMsg(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType) announceMsgPtr;
  ETHTSYN_P2CONST(EthTSyn_ClockIdentityType) pathSequenceClockIdentityPtr;
  uint16 pathSequenceTlvCnt;
  uint16 assumedLenByte;
  uint16 stepsRemovedCnt;
  uint16 lenByte = (uint16)EthTSyn_Ntohs(ComMsgPtr->MessageLength);

  /* ----- Implementation ----------------------------------------------- */
  /* set message pointers */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  announceMsgPtr = (ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType))
      &((ETHTSYN_P2CONST(uint8)) ComMsgPtr)[ETHTSYN_HDR_COM_LENGTH];

  /* #10 Get attributes of announce message. */
  ETHTSYN_GET_UINT16(announceMsgPtr->StepsRemoved, 0, stepsRemovedCnt);

  /* #20 Sanity check of announce message length field. */
  if((EthTSyn_Ntohs(announceMsgPtr->LengthField) % ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH) == 0)
  {
    pathSequenceTlvCnt = (uint16)(EthTSyn_Ntohs(announceMsgPtr->LengthField) /
        ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH);

    /* #30 Calculate assumed length including the tlv path sequence. Subtract the
     * ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH because the length of one element is already
     * included in the ETHTSYN_MSG_ANNOUNCE_LENGTH. */
    assumedLenByte = (uint16)(ETHTSYN_MSG_ANNOUNCE_LENGTH + ( EthTSyn_Ntohs(announceMsgPtr->LengthField) -
        ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH ));

    /* #40 Check that received Announce message is not from own system and that own system is not included in the path
     * sequence. */
    /* PRQA S 3415, 0310 6 */ /* MD_EthTSyn_12.4 */ /* MD_EthTSyn_11.4 */
    if((EthTSyn_CompareClockIdentity(ComMsgPtr->SourceClockIdentity,
        ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx)) != ETHTSYN_COMPARE_EQUAL) &&
        (EthTSyn_CompareClockIdentity(announceMsgPtr->ClockIdentity,
            ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx)) != ETHTSYN_COMPARE_EQUAL) &&
            (lenByte == assumedLenByte) &&
            (stepsRemovedCnt <= EthTSyn_SlavePortConfigPtr(PortIdx)->MaxStepsRemoved))
    {
      uint16_least i;
      retVal = E_OK;

      /* set message pointer to the clock identity of the path sequence */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      pathSequenceClockIdentityPtr = (ETHTSYN_P2CONST(EthTSyn_ClockIdentityType))announceMsgPtr->ClockIdentity;
      for(i = 0; i < pathSequenceTlvCnt; i++)
      {
        if(EthTSyn_CompareClockIdentity(pathSequenceClockIdentityPtr[i],
            ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx)) == ETHTSYN_COMPARE_EQUAL)
        {
          retVal = E_NOT_OK;
          break;
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_UtilRx_QualifyAnnounceMsg() */

#define ETHTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 * see file EthTSyn.c
*/

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Util_Int.c
 *********************************************************************************************************************/
