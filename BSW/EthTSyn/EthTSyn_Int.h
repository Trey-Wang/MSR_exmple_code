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
 *         \file  EthTSyn_Int.h
 *        \brief  EthTSyn internal header file
 *
 *      \details  Contains internal definitions and declarations used by the EthTSyn only
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

#if !defined(ETHTSYN_INT_H)
# define ETHTSYN_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn.h"
# include "EthTSyn_Util_Int.h"
# include "SchM_EthTSyn.h"
# include "StbM.h"

# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif  /* ETHTSYN_DEV_ERROR_REPORT */

# include "IpBase.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if !defined (ETHTSYN_INLINE)
#  define ETHTSYN_INLINE static INLINE
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ETHTSYN General */
# define ETHTSYN_PROTOCOL_VERSION                             (0x02u)

/* ETHTSYN Header Length */
# define ETHTSYN_HDR_COM_LENGTH                               (34u)
# define ETHTSYN_HDR_COM_RESERVED_1_LENGTH                    (4u)
# define ETHTSYN_HDR_COM_CORRECTION_FIELD_LENGTH              (8u)
# define ETHTSYN_HDR_COM_TRANSPORT_SPECIFIC                   (0x10u)
# define ETHTSYN_HDR_COM_CONTROL_SYNC                         (0x00u)
# define ETHTSYN_HDR_COM_CONTROL_FOLLOW_UP                    (0x02u)
# define ETHTSYN_HDR_COM_CONTROL_ANNOUNCE                     (0x05u)
# define ETHTSYN_HDR_COM_CONTROL_PDELAY_REQ                   (0x05u)
# define ETHTSYN_HDR_COM_CONTROL_PDELAY_RESP                  (0x05u)
# define ETHTSYN_HDR_COM_CONTROL_PDELAY_RESP_FOLLOW_UP        (0x05u)

# define ETHTSYN_HDR_COM_LOG_MESSAGE_INTERVAL_DEFAULT         (0x7Fu)

/* ETHTSYN Header Flags */ 
# define ETHTSYN_FLAGS_TWO_STEP                               (0x0002u)
# define ETHTSYN_FLAGS_LEAP61                                 (0x0100u)
# define ETHTSYN_FLAGS_LEAP59                                 (0x0200u)
# define ETHTSYN_FLAGS_UTC_OFFSET_VALID                       (0x0400u)
# define ETHTSYN_FLAGS_ETHTSYN_TIMESCALE                      (0x0800u)
# define ETHTSYN_FLAGS_TIME_TRACEABLE                         (0x1000u)
# define ETHTSYN_FLAGS_FREQUENCY_TRACEABLE                    (0x2000u)

/* PRQA S 3453 8 */ /* MD_MSR_19.7 */
# define ETHTSYN_HDR_FLAGS_TWO_STEP(param)                    ((uint16)(param) & ETHTSYN_FLAGS_TWO_STEP)
# define ETHTSYN_HDR_FLAGS_LEAP61(param)                      ((uint16)(param) & ETHTSYN_FLAGS_LEAP61)
# define ETHTSYN_HDR_FLAGS_LEAP59(param)                      ((uint16)(param) & ETHTSYN_FLAGS_LEAP59)
# define ETHTSYN_HDR_FLAGS_ETHTSYN_TIMESCALE(param)           ((uint16)(param) & ETHTSYN_FLAGS_ETHTSYN_TIMESCALE)
# define ETHTSYN_HDR_FLAGS_UTC_OFFSET_VALID(param)            ((uint16)(param) & ETHTSYN_FLAGS_UTC_OFFSET_VALID)
# define ETHTSYN_HDR_FLAGS_TIME_TRACEABLE(param)              ((uint16)(param) & ETHTSYN_FLAGS_TIME_TRACEABLE)
# define ETHTSYN_HDR_FLAGS_FREQUENCY_TRACEABLE(param)         ((uint16)(param) & ETHTSYN_FLAGS_FREQUENCY_TRACEABLE)

/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
# define ETHTSYN_HDR_COM_MSG_TYPE(param)                      ((uint8)(param) & 0x0F)
# define ETHTSYN_HDR_COM_VERSION(param)                       ((uint8)(param) & 0x0F)

/* Message Pdelay values */
# define ETHTSYN_MSG_P_DELAY_PAYLOAD_LENGTH                   (20u)

/* Message PdelayReq values */
# define ETHTSYN_MSG_P_DELAY_REQ_LENGTH                       (ETHTSYN_MSG_P_DELAY_PAYLOAD_LENGTH \
                                                              + ETHTSYN_HDR_COM_LENGTH)
# define ETHTSYN_MSG_P_DELAY_REQ_RESERVED_0_LENGTH            (10u)
# define ETHTSYN_MSG_P_DELAY_REQ_RESERVED_1_LENGTH            (10u)

/* Message PdelayResp values */
# define ETHTSYN_MSG_P_DELAY_RESP_LENGTH                      (ETHTSYN_MSG_P_DELAY_PAYLOAD_LENGTH \
                                                              + ETHTSYN_HDR_COM_LENGTH)

/* Message PdelayRespFollowUp values */
# define ETHTSYN_MSG_P_DELAY_RESP_FOLLOW_UP_LENGTH            (ETHTSYN_MSG_P_DELAY_PAYLOAD_LENGTH \
                                                              + ETHTSYN_HDR_COM_LENGTH)

/* Message FollowUp values */
/* FollowUp message lengths -> EthTSyn_Cfg.h */
/* Precise Origin Timestamp length (10) + complete FollowUp IEEE Tlv length (32) */
# define ETHTSYN_FOLLOW_UP_AR_TLV_OFFSET                             (42u)
# define ETHTSYN_FOLLOW_UP_TLV_TYPE                                  (0x03u)
# define ETHTSYN_FOLLOW_UP_TLV_LENGTH_FIELD                          (28u)
# define ETHTSYN_FOLLOW_UP_ORGANISATION_ID                           (0x000080C2ul)
# define ETHTSYN_FOLLOW_UP_ORGANISATION_SUBTYPE                      (0x00000001ul)
# define ETHTSYN_MSG_FOLLOW_UP_PRECISE_ORIGIN_TS_SECONDS_LENGTH      (6u)
# define ETHTSYN_MSG_FOLLOW_UP_PRECISE_ORIGIN_TS_NANOSECONDS_LENGTH  (4u)
# define ETHTSYN_MSG_FOLLOW_UP_ORGANISATION_ID_LENGTH                (3u)
# define ETHTSYN_MSG_FOLLOW_UP_ORGANISATION_SUB_TYPE_LENGTH          (3u)
# define ETHTSYN_MSG_FOLLOW_UP_LAST_GM_PHASE_CHANGE_LENGTH           (12u)
# define ETHTSYN_MSG_FOLLOW_UP_SCALED_LAST_GM_FREQ_CHANGE_LENGTH     (4u)
# define ETHTSYN_MSG_FOLLOW_UP_CORRECTION_FIELD_MULTIPLIER           (65536u)
# define ETHTSYN_MSG_FOLLOW_UP_CORRECTION_FIELD_SHIFT                (16u)
# define ETHTSYN_MSG_FOLLOW_UP_CORRECTION_FIELD_LENGTH               (8u)

/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define ETHTSYN_MSG_FOLLOW_UP_CORRECTION_FIELD_TO_NS(CorrectionField)  ((uint64)((CorrectionField) >> 16u))
# define ETHTSYN_MSG_FOLLOW_UP_NS_TO_CORRECTION_FIELD(Nanoseconds)      ((uint64)((Nanoseconds) << 16u))

/* Message Sync values */
# define ETHTSYN_MSG_SYNC_PAYLOAD_LENGTH                      (10u)
# define ETHTSYN_MSG_SYNC_LENGTH                              (ETHTSYN_MSG_SYNC_PAYLOAD_LENGTH \
                                                              + ETHTSYN_HDR_COM_LENGTH)
# define ETHTSYN_MSG_SYNC_RESERVED_LENGTH                     (10u)

/* ETHTSYN Announce values */
# define ETHTSYN_MSG_ANNOUNCE_PAYLOAD_LENGTH                  (42u)
# define ETHTSYN_MSG_ANNOUNCE_LENGTH                          (ETHTSYN_MSG_ANNOUNCE_PAYLOAD_LENGTH \
                                                              + ETHTSYN_HDR_COM_LENGTH)
# define ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH    (8u)
# define ETHTSYN_ANNOUNCE_PRIORITY_1_NETWORK_INFRASTRUCTURE   (246u)
# define ETHTSYN_ANNOUNCE_PRIORITY_1_OTHER_SYSTEM             (248u)
# define ETHTSYN_ANNOUNCE_PRIORITY_1_NOT_GM_CAPABLE           (255u)

# define ETHTSYN_ANNOUNCE_PRIORITY_2_DEFAULT                  (248u)
# define ETHTSYN_ANNOUNCE_CLOCK_CLASS_UNKNOWN                 (248u)
# define ETHTSYN_ANNOUNCE_CLOCK_CLASS_NOT_GM_CAPABLE          (255u)
# define ETHTSYN_ANNOUNCE_CLOCK_ACCURACY_UNKNOWN              (254u)
# define ETHTSYN_ANNOUNCE_OFFSET_SCALED_LOG_UNKNOWN           (0x4100u)
# define ETHTSYN_ANNOUNCE_TIME_SOURCE_INTERNAL_OSCILLATOR     (0xA0u)
# define ETHTSYN_ANNOUNCE_TLV_TYPE                            (0x08u)
# define ETHTSYN_ANNOUNCE_TLV_LENGTH_FIELD                    (0x08u)

/* ETHTSYN Message Types */
# define ETHTSYN_MSG_TYPE_ANNOUNCE                            (0x0Bu)
# define ETHTSYN_MSG_TYPE_SIGNALING                           (0x0Cu)
# define ETHTSYN_MSG_TYPE_SYNC                                (0x00u)
# define ETHTSYN_MSG_TYPE_PDELAY_REQ                          (0x02u)
# define ETHTSYN_MSG_TYPE_PDELAY_RESP                         (0x03u)
# define ETHTSYN_MSG_TYPE_FOLLOW_UP                           (0x08u)
# define ETHTSYN_MSG_TYPE_PDELAY_RES_FOLLOW_UP                (0x0Au)
# define ETHTSYN_MSG_TYPE_INVALID                             (0xFFu)

/* TimeBaseStatus Masks */
# define ETHTSYN_TIME_BASE_STATUS_TIMEOUT_MASK                (0x01u)
# define ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK             (0x04u)
# define ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT            (2u)
# define ETHTSYN_TIME_BASE_STATUS_GLOBAL_TIME_BASE_MASK       (0x08u)
# define ETHTSYN_TIME_BASE_STATUS_TIMELEAP_FUTURE_MASK        (0x10u)
# define ETHTSYN_TIME_BASE_STATUS_TIMELEAP_PAST_MASK          (0x20u)

/* ETHTSYN Ar (Sub)-Tlv values */
# define ETHTSYN_AR_SUB_TLV_OFS_TIME_SEC_SECONDS_OFFSET       (2u)

# define ETHTSYN_UINT16_MAX_VALUE                             (65535u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#if ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) && (ETHTSYN_WRAP_ETH_IF_TIME_APIS == STD_ON) )
/* PRQA S 3453 5 */ /* MD_MSR_19.7 */
/* Macros required to adopt AR4.3 EthIf APIs to the EthIf APIs of the Vector EthIf */
# define EthIf_GetEgressTimeStamp(CtrlIdx, BufIdx, TimeQualPtr, TimeStampPtr) \
  (EthIf_GetEgressTimestamp((CtrlIdx), (BufIdx), (TimeStampPtr), (TimeQualPtr)))
# define EthIf_GetIngressTimeStamp(CtrlIdx, DataPtr, TimeQualPtr, TimeStampPtr) \
  (EthIf_GetIngressTimestamp((CtrlIdx), (DataPtr), (TimeStampPtr), (TimeQualPtr)))
#endif /* ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) && (ETHTSYN_WRAP_ETH_IF_TIME_APIS == STD_ON) ) */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#  define ETHTSYN_START_SEC_VAR_ZERO_INIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#  include "MemMap.h"

extern VAR(EthTSyn_StateType, ETHTSYN_VAR_ZERO_INIT) EthTSyn_State;

#  define ETHTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#  include "MemMap.h"
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */

# define ETHTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
# include "MemMap.h"

extern VAR(EthTSyn_SystemType, ETHTSYN_VAR_NOINIT) EthTSyn_SystemState[ETHTSYN_TIME_DOMAIN_COUNT];

# define ETHTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
# include "MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  EthTSyn_CheckAnnounce
 *********************************************************************************************************************/
/*! \brief      Perform the announce check.
 *  \details    Checks if the Source-Port-Identity of the given Sync/FollowUp message (ComMsgPtr) is equal to the one
 *              received with a former announce message (stored in SystemState)
 *  \param[in]  ComMsgPtr      Pointer to the Common Message Header of the message the check should be performed for
 *  \param[in]  TimeDomainIdx  Index of the TimeDomain
 *  \return     TRUE - Announce check passed
 *  \return     FALSE - Announce check failed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CheckAnnounce(
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                  EthTSyn_TimeDomainIdxType TimeDomainIdx);

/**********************************************************************************************************************
 *  EthTSyn_CheckSourcePortIdentity
 *********************************************************************************************************************/
/*! \brief      Perform source port identity check.
 *  \details    Checks if the  Source-Port-Identity of the given Sync/FollowUp message (ComMsgPtr) is equal to the one
 *              configured for the given port
 *  \param[in]  ComMsgPtr    Pointer to the Common Message Header of the message the check should be performed for
 *  \param[in]  SlavePortIdx Index of the Slave-Port the message was received on
 *  \return     TRUE - Source port identity check passed
 *              FALSE - Source port identity check failed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_CheckSourcePortIdentity(
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                  EthTSyn_PortIdxType       SlavePortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Transmit
 *********************************************************************************************************************/
/*! \brief      Transmits a EthTSyn frame on the given Port
 *  \param[in]  PortIdx           Index of the port that is used for transmission
 *  \param[in]  BufIdx            Index of the transmit buffer that holds the data
 *  \param[in]  LenByte           Byte count of the data that should be transmitted
 *  \param[in]  TxConfirmation    Determines if Tx confirmation is needed
 *  \param[in]  MgmtInfoPointer   Pointer to the Switch management info. NULL_PTR for non Switch-Ports
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \context    ANY
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Transmit(
                  EthTSyn_PortIdxType  PortIdx,
                  uint8                BufIdx,
                  uint16               LenByte,
                  boolean              TxConfirmation,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr);
# else
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Transmit(
  EthTSyn_PortIdxType PortIdx,
  uint8               BufIdx,
  uint16              LenByte,
  boolean             TxConfirmation);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL INLINE FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_SetClockIdentity
 *********************************************************************************************************************/
/*! \brief       Sets a clock identity to the given value
 *  \param[out]  TargetPtr    Pointer to the clock identity that should be set
 *  \param[in]   SourcePtr    Pointer to the clock identity that should be used as source value
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SetClockIdentity(
                  EthTSyn_ClockIdentityType  TargetPtr,
  ETHTSYN_P2CONST(uint8)                     SourcePtr );

/**********************************************************************************************************************
 *  EthTSyn_CompareClockIdentity
 *********************************************************************************************************************/
/*! \brief      Compares the value of the given clock identities.
 *  \param[in]  Value                      Clock identity that should be compared.
 *  \param[in]  Reference                  Reference Clock identity that is used for comparison.
 *  \return     ETHTSYN_COMPARE_LESS - Value clock identity is less than Reference clock identity
 *  \return     ETHTSYN_COMPARE_EQUAL - Clock identities are equal
 *  \return     ETHTSYN_COMPARE_GREATER - Value clock identity is greater that Reference clock identity
 *  \note       no side-effects
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(EthTSyn_CompareType, ETHTSYN_CODE) EthTSyn_CompareClockIdentity(
  ETHTSYN_P2CONST(EthTSyn_ClockIdentityPtrType) Value,
  ETHTSYN_P2CONST(EthTSyn_ClockIdentityPtrType) Reference);

/**********************************************************************************************************************
 *  EthTSyn_NtohTimestamp
 *********************************************************************************************************************/
/*! \brief       Converts an received Timestamp from Network-Byte-Order to Host-Byte-Order and into the locally used
                 TimestampType
 *  \param[out]  LocalTimestampPtr     Pointer to the local timestamp value in Host-Byte-Order (destination)
 *  \param[in]   GlobalTimestampPtr    Pointer to the EthTSyn timestamp value in Network-Byte-Order (source)
 *  \context     ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_NtohTimestamp(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) LocalTimestampPtr,
  ETHTSYN_P2CONST(EthTSyn_TimestampType)       GlobalTimestampPtr );

/**********************************************************************************************************************
 *  EthTSyn_IsTimestampReadyToProcess
 *********************************************************************************************************************/
/*! \brief      Check if the Timestamp of the given Timestamp state is ready to be processed.
 *  \param[in]  TimestampState:    State of the Timestamp to check
 *  \return     TRUE - Timestamp is ready to be processed
 *  \return     FALSE - Timestamp is not ready to be processed
 *  \note       If Timestamp is actually available has to be checked separately (EthTSyn_Util_IsTimestampAvailable)
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_IsTimestampReadyToProcess(
  EthTSyn_TimestampStateType TimestampState);

/**********************************************************************************************************************
 *  EthTSyn_PortIdentityIsEqual
 *********************************************************************************************************************/
/*! \brief      Checks if the two given Port Identities are equal or not.
 *  \param[in]  PortIdentity1     Port Identity 1
 *  \param[in]  PortIdentity2Nbo  Port Identity 2 in Network byte order
 *  \return     TRUE - The Port Identities are equal
 *  \return     FALSE - The Port Identities are not equal
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PortIdentityIsEqual(
  ETHTSYN_CONSTP2CONST(EthTSyn_PortIdentityType) PortIdentity1,
  ETHTSYN_CONSTP2CONST(EthTSyn_PortIdentityType) PortIdentity2Nbo);

/**********************************************************************************************************************
 *  EthTSyn_PortIdentityIsEqualLocalPort
 *********************************************************************************************************************/
/*! \brief      Checks if the given port identity equals the port identity of the given port
 *  \param[in]  PortIdx           Port index
 *  \param[in]  PortIdentityNbo   Port Identity in Network byte order
 *  \return     TRUE - The Port Identities are equal
 *  \return     FALSE - The Port Identities are not equal
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PortIdentityIsEqualLocalPort(
                       EthTSyn_PortIdxType       PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_PortIdentityType) PortIdentityNbo);

/**********************************************************************************************************************
 *  EthTSyn_IsTxReady
 *********************************************************************************************************************/
/*! \brief      Checks if transmission on the given Port is possible
 *  \param[in]  PortIdx     Port index
 *  \return     TRUE  - Port is ready for transmission
 *  \return     FALSE - Port is not ready for transmission
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_IsTxReady(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# if !defined(ETHTSYN_UNIT_TEST) /* COV_ETHTSYN_TESTSUITE_INSTRUMENTATION */
/**********************************************************************************************************************
 *  EthTSyn_SetClockIdentity
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SetClockIdentity(EthTSyn_ClockIdentityType TargetPtr,
  ETHTSYN_P2CONST(uint8) SourcePtr )
{
  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  IpBase_Copy( (ETHTSYN_P2VAR(IpBase_CopyDataType)) TargetPtr,
    (ETHTSYN_P2CONST(IpBase_CopyDataType)) SourcePtr, sizeof(EthTSyn_ClockIdentityType));
} /* EthTSyn_SetClockIdentity() */

/**********************************************************************************************************************
 *  EthTSyn_CompareClockIdentity
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(EthTSyn_CompareType, ETHTSYN_CODE) EthTSyn_CompareClockIdentity(
  ETHTSYN_P2CONST(EthTSyn_ClockIdentityPtrType) Value, ETHTSYN_P2CONST(EthTSyn_ClockIdentityPtrType) Reference)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least Idx; /* PRQA S 0781 */ /* MD_EthTSyn_5.6 */
  EthTSyn_CompareType retVal = ETHTSYN_COMPARE_EQUAL;

  /* ----- Implementation ----------------------------------------------- */
  for(Idx = 0; Idx < 8; Idx++)
  {
    if(Value[Idx] > Reference[Idx])
    {
      retVal = ETHTSYN_COMPARE_GREATER;
      break;
    }
    else if(Value[Idx] < Reference[Idx])
    {
      retVal = ETHTSYN_COMPARE_LESS;
      break; /* PRQA S 0771 */ /* MD_EthTSyn_14.6 */
    }
    else
    {
      /* values are equal -> do nothing */
    }
  }
  return retVal;
} /* EthTSyn_CompareClockIdentity() */

/**********************************************************************************************************************
 *  EthTSyn_NtohTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_NtohTimestamp(
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) LocalTimestampPtr,
  ETHTSYN_P2CONST(EthTSyn_TimestampType) GlobalTimestampPtr )
{
  /* ----- Implementation ----------------------------------------------- */
  LocalTimestampPtr->nanoseconds = (uint32)(((uint32) GlobalTimestampPtr->NanoSeconds[0] << 24u) |
                                            ((uint32) GlobalTimestampPtr->NanoSeconds[1] << 16u) |
                                            ((uint32) GlobalTimestampPtr->NanoSeconds[2] <<  8u) |
                                            ((uint32) GlobalTimestampPtr->NanoSeconds[3] <<  0u));

  LocalTimestampPtr->secondsHi =   (uint16)(((uint16) GlobalTimestampPtr->Seconds[0] << 8u) |
                                            ((uint16) GlobalTimestampPtr->Seconds[1] << 0u));

  LocalTimestampPtr->seconds =     (uint32)(((uint32) GlobalTimestampPtr->Seconds[2] << 24u) |
                                            ((uint32) GlobalTimestampPtr->Seconds[3] << 16u) |
                                            ((uint32) GlobalTimestampPtr->Seconds[4] <<  8u) |
                                            ((uint32) GlobalTimestampPtr->Seconds[5] <<  0u));

} /* EthTSyn_NtohTimestamp() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  EthTSyn_IsTimestampReadyToProcess
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_IsTimestampReadyToProcess(
  EthTSyn_TimestampStateType TimestampState)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean timestampReadyToProcess = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if((TimestampState & ETHTSYN_TIMESTAMP_STATE_REQUIRED_FLAG_VALID_MASK) != 0)
  {
    if((TimestampState & ETHTSYN_TIMESTAMP_STATE_REQUIRED_MASK) == 0)
    {
      /* Timestamp not required --> processing possible regardless of the availability */
      timestampReadyToProcess = TRUE;
    }
    else
    {
      /* Timestamp required. Check availability */
      if((TimestampState & ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK) != 0)
      {
        timestampReadyToProcess = TRUE;
      }
    }
  }

  return timestampReadyToProcess;
} /* EthTSyn_IsTimestampReadyToProcess() */

/**********************************************************************************************************************
 *  EthTSyn_PortIdentityIsEqual
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PortIdentityIsEqual(
  ETHTSYN_CONSTP2CONST(EthTSyn_PortIdentityType) PortIdentity1,
  ETHTSYN_CONSTP2CONST(EthTSyn_PortIdentityType) PortIdentity2Nbo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isEqual = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if(PortIdentity1->PortNumber == (EthTSyn_PortNumberType)EthTSyn_Ntohs(PortIdentity2Nbo->PortNumber))
  {
    if(EthTSyn_CompareClockIdentity(PortIdentity1->ClockIdentity, PortIdentity2Nbo->ClockIdentity) ==
      ETHTSYN_COMPARE_EQUAL)
    {
      isEqual = TRUE;
    }
  }

  return isEqual;
} /* EthTSyn_PortIdentityIsEqual() */

/**********************************************************************************************************************
 *  EthTSyn_PortIdentityIsEqualLocalPort
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PortIdentityIsEqualLocalPort(EthTSyn_PortIdxType PortIdx,
  ETHTSYN_CONSTP2CONST(EthTSyn_PortIdentityType) PortIdentityNbo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isEqual = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if(ETHTSYN_CFG_GET_PORT_NUMBER(PortIdx) == (EthTSyn_PortNumberType)EthTSyn_Ntohs(PortIdentityNbo->PortNumber))
  {
    if(EthTSyn_CompareClockIdentity(ETHTSYN_STATE_GET_LOCAL_CLOCK_IDENTITY(PortIdx),
      PortIdentityNbo->ClockIdentity) == ETHTSYN_COMPARE_EQUAL)
    {
      isEqual = TRUE;
    }
  }

  return isEqual;
} /* EthTSyn_PortIdentityIsEqualLocalPort() */

/**********************************************************************************************************************
 *  EthTSyn_IsTxReady
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_IsTxReady(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean txReady = FALSE;
  const EthTSyn_CtrlIdxType ethTSynCtrlIdx = EthTSyn_PortConfig[PortIdx].EthTSynCtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* Check EthTSyn Ctrl State */
  if((EthTSyn_CtrlState[ethTSynCtrlIdx].TransmissionMode == ETHTSYN_TX_ON) &&
    (EthTSyn_CtrlState[ethTSynCtrlIdx].TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE))
  {
    txReady = TRUE;
  }

  return txReady;
} /* EthTSyn_IsTxReady() */

# endif /* ETHTSYN_UNIT_TEST */

# define ETHTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHTSYN_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Int.h
 *********************************************************************************************************************/
