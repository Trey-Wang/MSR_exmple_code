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
 *         \file  EthTSyn_Types.h
 *        \brief  EthTSyn Types header
 *
 *      \details  Contains all type definitions
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

#if !defined(ETHTSYN_TYPES_H)
# define ETHTSYN_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "EthIf.h"
# include "EthTSyn_Cfg.h"
# include "StbM_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 8 */ /* MD_MSR_19.7 */
# define ETHTSYN_P2CONSTCFGROOT(Type)  P2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2CONSTCFG(Type)      P2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_CONSTP2CONSTCFG(Type) CONSTP2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2CONST(Type)         P2CONST(Type, AUTOMATIC, ETHTSYN_APPL_DATA)
# define ETHTSYN_CONSTP2CONST(Type)    CONSTP2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2VAR(Type)           P2VAR(Type, AUTOMATIC, ETHTSYN_APPL_VAR)
# define ETHTSYN_CONSTP2VAR(Type)      CONSTP2VAR(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2FUNC(RType, Type)   P2FUNC(RType, ETHTSYN_CODE, Type)

# define ETHTSYN_PHYS_ADDR_LEN         (6U)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8                       EthTSyn_PortIdxType;
typedef uint8_least                 EthTSyn_PortIdxIterType;
typedef uint8                       EthTSyn_CtrlIdxType;
typedef uint8_least                 EthTSyn_CtrlIdxIterType;
typedef uint8                       EthTSyn_PdelayIdxType;
typedef uint8_least                 EthTSyn_PdelayIdxIterType;
typedef uint8                       EthTSyn_TimeDomainIdxType;
typedef uint8_least                 EthTSyn_TimeDomainIdxIterType;
typedef uint16                      EthTSyn_PortNumberType;
typedef uint8                       EthTSyn_DomainNumberType;
# if (defined Rte_TypeDef_StbM_TimeStampType)
typedef StbM_SynchronizedTimeBaseType  EthTSyn_SynchronizedTimeBaseType;
# else
typedef uint16                      EthTSyn_SynchronizedTimeBaseType;
# endif
typedef uint8                       EthTSyn_BufferIdxType;
typedef uint8                       EthTSyn_ClockIdentityType[8]; /* Sync with EthTSyn_ClockIdentityPtrType */
typedef uint8                       EthTSyn_ClockIdentityPtrType;
typedef uint8                       EthTSyn_CorrectionFieldType[8];
typedef sint8                       EthTSyn_LogMessageIntervalType;
typedef sint64                      EthTSyn_TimediffType;
typedef uint64                      EthTSyn_UTimediffType;
typedef uint8                       EthTSyn_TimestampQualityType;
typedef StbM_TimeStampRawType       EthTSyn_TimestampRawType;
typedef uint32                      EthTSyn_PdelayType;

# if ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) )
typedef Eth_TimeStampType           EthTSyn_GlobalTimestampType;
typedef EthTSyn_GlobalTimestampType EthTSyn_IngressTimestampType;
# else
typedef EthTSyn_TimestampRawType    EthTSyn_IngressTimestampType;
/*! Timestamp struct definition */
typedef struct EthTSyn_GlobalTimestampTypeStruct
{
  uint32 seconds;
  uint16 secondsHi;
  uint32 nanoseconds;
} EthTSyn_GlobalTimestampType;
# endif /* ( (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) ) */

# if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
typedef Eth_FrameIdType EthTSyn_FrameIdType;
#  define ETHTSYN_INVALID_FRAME_ID ETH_INVALID_FRAME_ID
# else
typedef uint16 EthTSyn_FrameIdType;
# define ETHTSYN_INVALID_FRAME_ID 0
# endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */

# if (ETHTSYN_ETHIF_AR_VERSION_GREATER_EQUAL_4_2_2 == TRUE)
typedef ETHTSYN_P2CONST(uint8) EthTSyn_PhyAddrPtrType;
# else
typedef ETHTSYN_P2VAR(uint8)  EthTSyn_PhyAddrPtrType;
# endif /* (ETHTSYN_ETHIF_AR_VERSION_GREATER_EQUAL_4_2_2 == TRUE) */

/*! Egress timestamp handle struct definition */
typedef struct EthTSyn_EgressTimestampHandleTypeStruct
{
  EthTSyn_GlobalTimestampType  EgressTimestamp; /*<! HW-Timestamping only: The actual egress timestamp */
  EthTSyn_GlobalTimestampType  GlobalTimestamp; /*<! Sync msg only: The current time (StbM) at Tx */
  EthTSyn_GlobalTimestampType  SyncRefTimestamp; /*<! Sync msg and HW-Timestamping only: The Time (EthIf) at Tx */
  EthTSyn_TimestampRawType     RawTimestamp; /*<! SW-Timestamping only: The actual egress timestamp */
} EthTSyn_EgressTimestampHandleType;

/*! Time base update counter handle struct definition */
typedef struct EthTSyn_TimeBaseUpdateCounterHandlerTypeStruct
{
  boolean IsValueValid;  /* Set to true with the first call of StbM_GetTimeBaseUpdateCounter() */
  uint8   Value;
}EthTSyn_TimeBaseUpdateCounterHandlerType;

/* 48 bit seconds */
# define ETHTSYN_TIMESTAMP_MAX_SECONDS                                             (0xFFFFFFFFFFFFLL)
/* 10^9 nanoseconds per second */
# define ETHTSYN_TIMESTAMP_MAX_NANOSECONDS                                         (1000000000UL)
/* Timestamp size in Byte (48 bit seconds + 32 bit nanoseconds = 10 Byte) */
# define ETHTSYN_TIMESTAMP_SIZE_BYTE                                               (10u)
/* 63 bit max timediff in nanoseconds */
# define ETHTSYN_TIMEDIFF_MAX_NANOSECONDS                                          (0x7FFFFFFFFFFFFFFFLL)
# define ETHTSYN_TIMEDIFF_MAX_SECONDS                                              (ETHTSYN_TIMEDIFF_MAX_NANOSECONDS \
                                                                                  / ETHTSYN_TIMESTAMP_MAX_NANOSECONDS)
# define ETHTSYN_MAX_UINT_32                                                       (0xFFFFFFFFUL)
# define ETHTSYN_MAX_UINT_32_64BIT                                                 (0x00000000FFFFFFFFULL)
# define ETHTSYN_MAX_UINT_64                                                       (0xFFFFFFFFFFFFFFFFULL)
# define ETHTSYN_INVALID_ETHIF_CTRL_IDX                                            (0xFFU)
# define ETHTSYN_INVALID_ETHTSYN_CTRL_IDX                                          (0xFFU)
# define ETHTSYN_INVALID_PORT_IDX                                                  (0xFFU)
# define ETHTSYN_INVALID_SWITCH_PORT_IDX                                           (0xFFU)
# define ETHTSYN_INVALID_ETHIF_SWITCH_IDX                                          (0xFFU)
# define ETHTSYN_INVALID_TIME_DOMAIN_IDX                                           (0xFFU)
# define ETHTSYN_INVALID_PDELAY_IDX                                                (0xFFU)
# define ETHTSYN_INVALID_SYNC_SEND_INTERVAL                                        ((uint32)0xFFFFFFFFU)
# define ETHTSYN_INVALID_LOG_SYNC_SEND_INTERVAL                                    ((sint8)0x7F)

/* ETHTSYN State Definitions */
typedef uint8  EthTSyn_StateType;
# define ETHTSYN_STATE_UNINIT                                                      (0u)
# define ETHTSYN_STATE_INIT                                                        (1u)

/* ETHTSYN State-Machine State Definitions */
typedef uint8  EthTSyn_StateMachineStateType;
/* Sync Receive Sm States */
# define ETHTSYN_STATE_SYNC_RECEIVE_DISCARD                                        (0u)
# define ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP                          (1u)
# define ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC                               (2u)
# define ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC_INGRESS_TIMESTAMP             (4u)
# define ETHTSYN_STATE_SYNC_RECEIVE_SYNC_TIMEOUT                                   (5u)

/* Sync Send Sm States */
# define ETHTSYN_STATE_SYNC_SEND_INITIALIZING                                      (0u)
# define ETHTSYN_STATE_SYNC_SEND_WAITING_FOR_SYNC_SEND                             (1u)
# define ETHTSYN_STATE_SYNC_SEND_SEND_SYNC                                         (2u)
# define ETHTSYN_STATE_SYNC_SEND_SEND_IMMEDIATE_SYNC                               (3u)
# define ETHTSYN_STATE_SYNC_SEND_SEND_FOLLOW_UP                                    (4u)
# define ETHTSYN_STATE_SYNC_SEND_SEND_ANNOUNCE                                     (5u)
# define ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_PENDING                            (6u)
# define ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_SYNC                          (7u)
# define ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_WAIT_FOLLOW_UP                     (8u)
# define ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_SEND_FOLLOW_UP                     (9u)
# define ETHTSYN_STATE_SYNC_SEND_DISABLED                                          (255U)

/* Pdelay Req Sm States */
# define ETHTSYN_STATE_P_DELAY_REQ_NOT_ENABLED                                     (0u)
# define ETHTSYN_STATE_P_DELAY_REQ_INITIAL_SEND_P_DELAY_REQ                        (1u)
# define ETHTSYN_STATE_P_DELAY_REQ_SEND_P_DELAY_REQ                                (2u)
# define ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP                        (3u)
# define ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_INGRESS_TIMESTAMP      (4u)
# define ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_P_DELAY_RESP_FOLLOW_UP              (5u)
# define ETHTSYN_STATE_P_DELAY_REQ_RECEIVED_PDELAY_RESP_AND_PDELAY_RESP_FOLLOW_UP  (6u)
# define ETHTSYN_STATE_P_DELAY_REQ_WAITING_FOR_PDELAY_INTERVAL_COUNTER             (7u)

/* Pdelay Resp Sm States */
# define ETHTSYN_STATE_P_DELAY_RESP_NOT_ENABLED                                    (0u)
# define ETHTSYN_STATE_P_DELAY_RESP_INITAIL_WAITING_FOR_PDELAY_REQ                 (1u)
# define ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_REQ                         (2u)
# define ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_REQ_INGRESS_TIMESTAMP       (3u)
# define ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP                              (4u)
# define ETHTSYN_STATE_P_DELAY_RESP_WAITING_FOR_PDELAY_RESP_EGRESS_TIMESTAMP       (5u)
# define ETHTSYN_STATE_P_DELAY_RESP_SEND_P_DELAY_RESP_FOLLOW_UP                    (6u)

/* ETHTSYN Port State Definitions */
typedef uint8  EthTSyn_SyncStateType;
# define ETHTSYN_SYNC                                      (0U)
# define ETHTSYN_UNSYNC                                    (1U)
# define ETHTSYN_UNCERTAIN                                 (2U)
# define ETHTSYN_NEVERSYNC                                 (3U)

/* ETHTSYN Port Role Definitions */
typedef uint8  EthTSyn_PortRoleType;
# define ETHTSYN_PORT_ROLE_SLAVE                           (0U)
# define ETHTSYN_PORT_ROLE_MASTER                          (1U)
# define ETHTSYN_PORT_ROLE_PASSIVE                         (2U)
# define ETHTSYN_PORT_ROLE_DISABLED                        (3U)

/* ETHTSYN Compare Definitions */
typedef sint8  EthTSyn_CompareType;
# define ETHTSYN_COMPARE_LESS                              (-1)
# define ETHTSYN_COMPARE_EQUAL                             (0)
# define ETHTSYN_COMPARE_GREATER                           (1)

/* ETHTSYN Transmission Mode Definitions */
typedef uint8 EthTSyn_TransmissionModeType;
# define ETHTSYN_TX_OFF                                    (0U)
# define ETHTSYN_TX_ON                                     (1U)

/* ETHTSYN Timestamp State Definitions */
typedef uint8 EthTSyn_TimestampStateType;
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_INVALID                         (0x00U)
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED                    (0x02U)
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED_NOT_AVAILABLE      (0x0AU)
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_NOT_REQUIRED_AVAILABLE          (0x0EU)
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_WAITING_FOR_TIMESTAMP  (0x03U)
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE          (0x0BU)
# define ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE              (0x0FU)
/* ETHTSYN Timestamp State Masks */
/*
 * +-------------------------------+
 * |R|R|R|R|Avail_V|Avail|Req_V|Req|
 * +-------------------------------+
 */
# define ETHTSYN_TIMESTAMP_STATE_REQUIRED_MASK                             (0x01U)
# define ETHTSYN_TIMESTAMP_STATE_REQUIRED_FLAG_VALID_MASK                  (0x02U)
# define ETHTSYN_TIMESTAMP_STATE_AVAILABLE_MASK                            (0x04U)
# define ETHTSYN_TIMESTAMP_STATE_AVAILABLE_FLAG_VALID_MASK                 (0x08U)

/* EthTSyn port type type */
typedef uint8 EthTSyn_PortTypeType;
/* Bit-Masks */
/*
 * +--------------------------------+
 * |R|R|R|R|R|Host|Switch|EndStation|
 * | | | | | |Port| Port |   Port   |
 * +--------------------------------+
 */
# define ETHTSYN_PORT_TYPE_INVALID                         (0x00U)
# define ETHTSYN_PORT_TYPE_END_STATION_PORT                (0x01U)
# define ETHTSYN_PORT_TYPE_SWITCH_PORT                     (0x02U)
# define ETHTSYN_PORT_TYPE_SWITCH_AND_END_STATION_PORT     (0x03U)
# define ETHTSYN_PORT_TYPE_SWITCH_HOST_PORT                (0x04U)

/*! EthTSyn Timestamp Format struct definition */
typedef struct EthTSyn_TimestampTypeStruct
{
  uint8 Seconds[6];
  uint8 NanoSeconds[4];
} EthTSyn_TimestampType;

/*! Internal timestamp struct definition (used for arithmetic) */
typedef struct EthTSyn_InternalTimestampTypeStruct
{
  uint32 nanoseconds;
  uint64 seconds;
} EthTSyn_InternalTimestampType;

/*! Port identity struct definition */
typedef struct EthTSyn_PortIdentityTypeStruct
{
  EthTSyn_ClockIdentityType ClockIdentity;
  EthTSyn_PortNumberType    PortNumber;
} EthTSyn_PortIdentityType;

/*! EthTSyn Clock Quality format struct definition */
typedef struct EthTSyn_ClockQualityTypeStruct
{
  uint8        ClockClass;
  uint8        ClockAccuracy;
  uint16       OffsetScaledLogVariance;
} EthTSyn_ClockQualityType;

/*! EthTSyn Common Message Header format struct definition */
typedef struct EthTSyn_CommonMsgHdrTypeStruct
{
  uint8                       TransSpecific_MessageType;
  uint8                       Reserved_VersionGPtp;
  uint16                      MessageLength; /* 4 byte aligned */
  EthTSyn_DomainNumberType    DomainNumber;
  uint8                       Reserved_0;
  uint16                      Flags; /* 4 byte aligned */
  EthTSyn_CorrectionFieldType CorrectionField;
  uint8                       Reserved_1[4]; /* 4 byte aligned */
  EthTSyn_ClockIdentityType   SourceClockIdentity;
  EthTSyn_PortNumberType      SourcePortNumber;
  uint16                      SequenceId;
  uint8                       Control;
  EthTSyn_LogMessageIntervalType LogMessageInterval;
} EthTSyn_CommonMsgHdrType;

/*! EthTSyn Announce Message Format struct definition */
typedef struct EThTSyn_AnnounceMsgTypeStruct
{
  uint8                     Reserved_0[10];
  sint16                    CurrentUtcOffset;
  uint8                     Reserved_1;
  /* Begin systemIdentity for BMCA evaluation */
  uint8                     GrandmasterPriority_1;
  EthTSyn_ClockQualityType  GrandmasterClockQuality;
  uint8                     GrandmasterPriority_2;
  EthTSyn_ClockIdentityType GrandmasterIdentity;
  /* End systemIdentity for BMCA evaluation */
  /* no uint16 datatype of StepsRemoved possible due to padding / alignment issue */
  uint8                     StepsRemoved[2];
  uint8                     TimeSource;
  uint16                    TlvType;
  uint16                    LengthField;
  EthTSyn_ClockIdentityType ClockIdentity;
} EthTSyn_AnnounceMsgType;

/*! EthTSyn Signaling Message Format struct definition */
typedef struct EthTSyn_SignalingMsgTypeStruct
{
  EthTSyn_PortIdentityType TargetPortIdentity;
  uint16                   TlvType;
  uint16                   LengthField;
  uint8                    OrganisationId[3];
  uint8                    OrganisationSubType[3];
  sint8                    LinkDelayInt;
  sint8                    TimeSyncInt;
  sint8                    AnnounceInt;
  uint8                    Flags;
  uint16                   Reserved;
} EthTSyn_SignalingMsgType;

/*! EthTSyn Sync Message Format struct definition */
typedef struct EthTSyn_SyncMsgTypeStruct
{
  uint8 Reserved[10];
} EthTSyn_SyncMsgType;

/*! EthTSyn Follow Up Message Format struct definition */
typedef struct EthTSyn_FollowUpMsgTypeStruct
{
  uint8        PreciseOriginTimestampSeconds[6];
  uint8        PreciseOriginTimestampNanoSeconds[4];
  uint16       TlvType;
  uint16       LengthField;
  uint8        OrganisationId[3];
  uint8        OrganisationSubType[3];
  uint32       CumulativeScaledRateOffset;
  uint16       GmTimeBaseIndicator;
  uint8        lastGmPhaseChange[12];
  uint8        ScaledLastGmFreqChange[4];
} EthTSyn_FollowUpMsgType;

# define ETHTSYN_AR_TLV_ORGANIZATION_ID                  (0x1A75FBu)
# define ETHTSYN_AR_TLV_ORGANIZATION_SUB_TYPE            (0x605676u)

#  define ETHTSYN_AR_TLV_TYPE                            (0x0003u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED           (0x28u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED         (0x50u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED     (0x51u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED      (0x60u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED  (0x61u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED            (0x44u)
#  define ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED        (0x34u)

#  define ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE            (6u) /* Does not include the Sub-Tlvs length */
#  define ETHTSYN_AR_SUB_TLV_TIME_LENGTH_FIELD_VALUE          (3u)
#  define ETHTSYN_AR_SUB_TLV_STATUS_LENGTH_FIELD_VALUE        (2u)
#  define ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH_FIELD_VALUE     (5u)
#  define ETHTSYN_AR_SUB_TLV_OFS_LENGTH_FIELD_VALUE           (17u)

/* The actual AR Tlv header length is the value of its length field (6) + the type (2) and length field itself (2) */
#  define ETHTSYN_AR_TLV_HEADER_LENGTH                  (10u)
/* The actual length of an Ar SubTlv is the value of its length field + the type (1) and the length field itself (1) */
#  define ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH        (2u)
#  define ETHTSYN_AR_SUB_TLV_TIME_LENGTH                (5u)
#  define ETHTSYN_AR_SUB_TLV_STATUS_LENGTH              (4u)
#  define ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH           (7u)
#  define ETHTSYN_AR_SUB_TLV_OFS_LENGTH                 (19u)

/*! EthTSyn Follow_Up AR TLV Header struct definition */
typedef struct EthTSyn_ArTlvHeaderTypeStruct
{
  uint8  TlvType[2];
  uint8  LengthField[2];
  uint8  OrganisationId[3];
  uint8  OrganisationSubType[3];
} EthTSyn_ArTlvHeaderType;

/*! EthTSyn Follow_Up AR Sub TLV abstract header struct definition.
 *  It is intended to cast to the corresponding AR Sub Tlv type */
typedef struct EthTSyn_ArSubTlvAbstractHeaderTypeStruct
{
  uint8   Type;
  uint8   Length;
} EthTSyn_ArSubTlvAbstractHeaderType;

/*! EthTSyn Follow_Up AR Sub TLV Time Secured struct definition */
typedef struct EthTSyn_ArSubTlvTimeSecuredTypeStruct
{
  uint8   Type;
  uint8   Length;
  uint8   CrcTimeFlags;
  uint8   CrcTime0;
  uint8   CrcTime1;
} EthTSyn_ArSubTlvTimeSecuredType;

/* EthTSyn Follow_Up AR Sub TLV Status Secured/Not Secured */
#  define ETHTSYN_AR_SUB_TLV_STATUS_SGW_MASK                (0x01u)
#  define ETHTSYN_AR_SUB_TLV_STATUS_SGW_POS                 (0u)

#  define ETHTSYN_AR_SUB_TLV_STATUS_SGW_SYNC_TO_GTM         (0x00u)
#  define ETHTSYN_AR_SUB_TLV_STATUS_SGW_SYNC_TO_SUB_DOMAIN  (0x01u)

/*! EthTSyn Follow_Up AR Sub TLV Status Secured/Not Secured struct definition */
typedef struct EthTSyn_ArSubTlvStatusTypeStruct
{
  uint8   Type;
  uint8   Length;
  uint8   Status;
  uint8   CrcStatus; /*!< Only applicable for StatusSecured, Reserved otherwise */
} EthTSyn_ArSubTlvStatusType;

/*! EthTSyn Follow_Up AR Sub TLV UserData Secured/Not Secured struct definition */
typedef struct EthTSyn_ArSubTlvUserDataTypeStruct
{
  uint8   Type;
  uint8   Length;
  uint8   UserDataLength;
  uint8   UserByte0;
  uint8   UserByte1;
  uint8   UserByte2;
  uint8   CrcUserData; /*!< Only applicable for UserDataSecured, Reserved otherwise */
} EthTSyn_ArSubTlvUserDataType;

/*! EthTSyn Follow_Up AR Sub TLV OFS Secured/Not Secured struct definition */
typedef struct EthTSyn_ArSubTlvOfsTypeStruct
{
  uint8   Type;
  uint8   Length;
  uint8   OfsTimeDomain;
  uint8   OfsTimeSec[6];
  uint8   OfsTimeNSec[4];
  uint8   Status;
  uint8   UserDataLength;
  uint8   UserByte0;
  uint8   UserByte1;
  uint8   UserByte2;
  uint8   CrcOfs; /*!< Only applicable for OfsSecured, Reserved otherwise */
} EthTSyn_ArSubTlvOfsType;

/*! EthTSyn Pdelay Request Message Format struct definition */
typedef struct EthTSyn_PDelayReqMsgTypeStruct
{
  uint8 Reserved_0[10];
  uint8 Reserved_1[10];
} EthTSyn_PDelayReqMsgType;

/*! EthTSyn Pdelay Response Message Format struct definition */
typedef struct EthTSyn_PdelayRespMsgTypeStruct
{
  uint8                     RequestReceiptTimestampSeconds[6];
  uint8                     RequestReceiptTimestampNanoSeconds[4];
  EthTSyn_ClockIdentityType RequestingClockIdentity;
  EthTSyn_PortNumberType    RequestingPortNumber;
} EthTSyn_PdelayRespMsgType;

/*! EthTSyn Pdelay Response Follow Up Message Format struct definition */
typedef struct EthTSyn_PDelayResFollowUpMsgTypeStruct
{
  uint8                     ResponseOriginTimestampSeconds[6];
  uint8                     ResponseOriginTimestampNanoSeconds[4];
  EthTSyn_ClockIdentityType RequestingClockIdentity;
  EthTSyn_PortNumberType    RequestingPortNumber;
} EthTSyn_PDelayResFollowUpMsgType;

/* Tx Call-backs */
typedef ETHTSYN_P2FUNC(void, EthTSyn_SyncSentCbkPtrType)     (EthTSyn_PortIdxType,
                                                        ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType),
                                                        ETHTSYN_P2CONST(EthTSyn_SyncMsgType) );

typedef ETHTSYN_P2FUNC(void, EthTSyn_FollowUpSentCbkPtrType) (EthTSyn_PortIdxType,
                                                        ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType),
                                                        ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType) );

/*! EthTSyn Pdelay Request state-machine struct definition */
typedef struct EthTSyn_PdelayReqSmReqTypeStruct
{
  uint16                      SequenceId;
  uint32                      TxIntervalCnt; /*!< Decremented in each Mf. Tx Pdelay Request when counter reaches '0' */
  EthTSyn_TimestampStateType  EgressTimestampState;
  /*!< For SW-Timestamping, only 'raw' timestamps are used.
   * The raw timestamp will be stored in the 'nanoseconds' field of the global timestamp */
  EthTSyn_GlobalTimestampType EgressTimestamp;
} EthTSyn_PdelayReqSmReqType;

/*! EthTSyn Pdelay Response Request state-machine struct definition */
typedef struct EthTSyn_PdelayRespSmReqTypeStruct
{
  uint16                      SequenceId;
  EthTSyn_PortIdentityType    SourcePortIdentity; /*!< Source Port Identity of the requesting port */
  EthTSyn_TimestampStateType  IngressTimestampState;
  /*!< For SW-Timestamping, only 'raw' timestamps are used.
   * The raw timestamp will be stored in the 'nanoseconds' field of the global timestamp */
  EthTSyn_GlobalTimestampType IngressTimestamp;
} EthTSyn_PdelayRespSmReqType;

/*! EthTSyn Pdelay Response state-machine struct definition */
typedef struct EthTSyn_PdelayRespSmRespTypeStruct
{
  uint16                      SequenceId;
  EthTSyn_TimestampStateType  EgressTimestampState;
  /*!< For SW-Timestamping, only 'raw' timestamps are used.
   * The raw timestamp will be stored in the 'nanoseconds' field of the global timestamp */
  EthTSyn_GlobalTimestampType EgressTimestamp;
} EthTSyn_PdelayRespSmRespType;

/*! EthTSyn Pdelay Request Response state-machine struct definition */
typedef struct EthTSyn_PdelayReqSmRespTypeStruct
{
  uint16 SequenceId;
  EthTSyn_PortIdentityType SourcePortIdentity; /*!< Source Port Identity of the responding port  */
  EthTSyn_TimestampStateType  IngressTimestampState;
  /*!< For SW-Timestamping, only 'raw' timestamps are used.
   * The raw timestamp will be stored in the 'nanoseconds' field of the global timestamp */
  EthTSyn_GlobalTimestampType IngressTimestamp;
  EthTSyn_GlobalTimestampType RequestReceiptTimestamp;
} EthTSyn_PdelayReqSmRespType;

/*! EthTSyn Pdelay Request FollowUp state-machine struct definition */
typedef struct EthTSyn_PdelayReqSmRespFollowUpTypeStruct
{
  EthTSyn_TimestampStateType  ResponseOriginTimestampState;
  EthTSyn_GlobalTimestampType ResponseOriginTimestamp;
} EthTSyn_PdelayReqSmRespFollowUpType;

/*! EthTSyn Pdelay Request state-machine struct definition */
typedef struct EthTSyn_PdelayReqSmTypeStruct
{
  EthTSyn_StateMachineStateType       State;
  uint16                              LostResponses;
  boolean                             AsCapable;
  EthTSyn_PdelayType                  Pdelay;
  EthTSyn_PdelayReqSmReqType          PdelayReq;
  EthTSyn_PdelayReqSmRespType         PdelayResp;
  EthTSyn_PdelayReqSmRespFollowUpType PdelayRespFollowUp;
  sint8                               TxConfirmationPending;
  /*!< Tx == +1, TxConf == -1 --> == 0: Transmit + TxConfirmation occurred */
  EthTSyn_BufferIdxType               TxBufIdx;
  uint32                              PdelayRespRespFollowUpTimeout;
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  ETHTSYN_P2CONST(uint8)              TxBufPtr;
#  if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
  EthTSyn_FrameIdType                 FrameId;
#  endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
} EthTSyn_PdelayReqSmType;

/*! EthTSyn Pdelay Response state-machine struct definition */
typedef struct EthTSyn_PdelayRespSmTypeStruct
{
  boolean                       InProgress;
  EthTSyn_StateMachineStateType State;
  EthTSyn_PdelayRespSmReqType   PdelayReq;
  EthTSyn_PdelayRespSmRespType  PdelayResp;
  sint8                         TxConfirmationPending;
  /*!< Tx == +1, TxConf == -1 --> == 0: Transmit + TxConfirmation occurred */
  EthTSyn_BufferIdxType         TxBufIdx;
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
#  if (ETHTSYN_FRAME_ID_SUPPORT == STD_ON)
  EthTSyn_FrameIdType           FrameId;
#  endif /* (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) */
#  if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  ETHTSYN_P2CONST(uint8)        TxBufPtr;
#  endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
} EthTSyn_PdelayRespSmType;

/*! EthTSyn Sync Receive state-machine Sync message struct definition */
typedef struct EthTSyn_SyncReceiveSmSyncTypeStruct
{
  uint16                         SequenceId;
  EthTSyn_TimestampStateType     IngressTimestampState;
  /*!< For SW-Timestamping, only 'raw' timestamps are used.
   * The raw timestamp will be stored in the 'nanoseconds' field of the global timestamp */
  EthTSyn_GlobalTimestampType    IngressTimestamp;
  EthTSyn_PortIdentityType       SourcePortIdentity;
  EthTSyn_LogMessageIntervalType LogMessageInterval;
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  uint32                         SyncRxTimeoutCnt;
#  if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  EthTSyn_TimestampStateType     EgressTimestampHostPortState;
  EthTSyn_GlobalTimestampType    EgressTimestampHostPort;
#  endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
} EthTSyn_SyncReceiveSmSyncType;

/*! EthTSyn Sync Receive state-machine FollowUp message struct definition */
typedef struct EthTSyn_SyncReceiveSmFollowUpTypeStruct
{
  uint16                      SequenceId;
  uint32                      FollowUpRxTimeoutCnt;
} EthTSyn_SyncReceiveSmFollowUpType;

/*! EthTSyn Sync Receive state-machine struct definition */
typedef struct EthTSyn_SyncReceiveSmTypeStruct
{
  EthTSyn_StateMachineStateType     State;
  EthTSyn_SyncReceiveSmSyncType     Sync;
  EthTSyn_SyncReceiveSmFollowUpType FollowUp;
} EthTSyn_SyncReceiveSmType;

/*! EthTSyn Announce Received state-machine struct definition */
typedef struct EthTSyn_AnnounceReceiveSmTypeStruct
{
  uint32 RxTimeoutCnt;
} EthTSyn_AnnounceReceiveSmType;

/*! EthTSyn Sync Send state-machine struct definition */
typedef struct EthTSyn_SyncSendSmTypeStruct
{
  EthTSyn_StateMachineStateType  State;
  EthTSyn_TimestampStateType     FollowUpPreciseOriginTimestampState;
  EthTSyn_GlobalTimestampType    FollowUpPreciseOriginTimestamp; /*!< Sync Egress Timestamp */
  EthTSyn_BufferIdxType          SyncTxBufIdx;
  EthTSyn_BufferIdxType          FollowUpTxBufIdx;
  sint8                          TxConfirmationPending;
  /*!< Tx == +1, TxConf == -1 --> == 0: Transmit + TxConfirmation occurred */
  uint16                         SequenceId;
  uint32                         TxIntervalCnt; /*!< Decrement with each Mf. Tx Sync message when '0' is reached */
  EthTSyn_LogMessageIntervalType LogMessageInterval;
  ETHTSYN_P2CONST(uint8)         SyncTxBufPtr;
  ETHTSYN_P2CONST(uint8)         FollowUpTxBufPtr;
# if ( (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) )
  EthTSyn_FrameIdType            FrameId;
# endif /* ( (ETHTSYN_FRAME_ID_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) ) */
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  EthTSyn_GlobalTimestampType    SyncIngressTimestampHostPort;
  EthTSyn_TimestampStateType     SyncIngressTimestampStateHostPort;
  EthTSyn_GlobalTimestampType    SyncEgressTimestampSwitchPort;
  EthTSyn_TimestampStateType     SyncEgressTimestampStateSwitchPort;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
} EthTSyn_SyncSendSmType;

/*! EthTSyn General Port Role struct definition.
 * This type is used as an AbstractPortConfigType. It is intended to cast this type to either
 * EthTSyn_MasterPortConfigType or EthTSyn_SlavePortConfigType */
typedef struct EthTSyn_GeneralPortRoleTypeStruct
{
  EthTSyn_PortRoleType                      PortRole;
} EthTSyn_GeneralPortRoleType;

/*! EthTSyn Ctrl State struct definition */
typedef struct EthTSyn_CtrlStateTypeStruct
{
  EthTSyn_TransmissionModeType TransmissionMode;
  EthTrcv_LinkStateType        TrcvLinkState;
  EthTSyn_ClockIdentityType    ClockIdentity;
} EthTSyn_CtrlStateType;

/*! EthTSyn Grand Master Priority struct definition */
typedef struct EthTSyn_GmPriorityTypeStruct
{
  EthTSyn_PortIdentityType PortIdentity; /*!< Port Identity of the Grand Master */
} EthTSyn_GmPriorityType;

/*! EthTSyn System State struct definition */
typedef struct EthTSyn_SystemTypeStruct
{
  EthTSyn_GmPriorityType GmPriority; /*!< The Priority of the valid Grand Master */
  boolean GmPresent;
} EthTSyn_SystemType;

/*! EthTSyn Config struct definition */
# if ( ETHTSYN_CONFIGURATION_VARIANT == ETHTSYN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
typedef struct EthTSyn_VPostBuildConfigTypeStruct
{
  uint8                                Dummy;
} EthTSyn_ConfigType;
# else
typedef uint8                          EthTSyn_ConfigType;
# endif /* ( ETHTSYN_CONFIGURATION_VARIANT == ETHTSYN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) */

/* Crc function pointer */
typedef ETHTSYN_P2FUNC(uint8, EthTSyn_Crc_CalculateCrc8H2FFctPtrType)
    (ETHTSYN_P2CONST(uint8), uint32, uint8, boolean);

/***********************************************************************************************************************
 * ETHTSYN SWITCH MANAGEMENT TYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_INVALID_IDX                     (0xFFU)
# define ETHTSYN_SWT_MGMT_TIMESTAMP_QUEUE_INVALID_IDX                (0xFFU)

/* Site Sync Sync Sm States */
# define ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING                        (0U)
# define ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_SYNC                      (1U)
# define ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP                 (2U)
# define ETHTSYN_STATE_SITE_SYNC_SYNC_SYNC_TIMEOUT                   (3U)
# define ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_SYNC_AFTER_TIMEOUT    (4U)
# define ETHTSYN_STATE_SITE_SYNC_SYNC_RECEIVED_FUP_AFTER_TIMEOUT     (5U)
# define ETHTSYN_STATE_SITE_SYNC_SYNC_DISABLED                       (255U)

/* Message Buffer States */
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_UNUSED                    (0U)
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_INVALID               (1U)
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_VALID                 (2U)
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_PORT_INFO_AVAILABLE       (3U)
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_MSG_PROCESSING            (4U)
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_STATE_DROP_MESSAGE              (5U)

/* State machine identification types */
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_REQ_SM             (0U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_P_DELAY_RESP_SM            (1U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_PORT_SYNC_SYNC_RECEIVE_SM  (2U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_SYNC_RECEIVE_SM            (3U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_SYNC_SEND_SM               (4U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_SITE_SYNC_SYNC_SM          (5U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_ANNOUNCE_RECEIVE_SM        (6U)
# define ETHTSYN_STATE_MACHINE_IDENTIFICATION_INVLID_SM                  (255U)

/* Timestamp direction type values */
# define ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_INGRESS          (0U)
# define ETHTSYN_SWT_MGMT_TIMESTAMP_DIRECTION_EGRESS           (1U)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 EthTSyn_SwtMgmt_MsgBufferIdxType;

typedef uint8 EthTSyn_SwtMgmt_MsgBufferStateType;

typedef uint8 EthTSyn_SwtMgmt_TsQueueIdxType;

typedef uint8 EthTSyn_SwtMgmt_StateMachineIndentificationType;

typedef uint8 EthTSyn_SwtMgmt_TimestampDirectionType;

/*! EthTSyn Switch Management Info struct definition */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
typedef EthSwt_MgmtInfoType  EthTSyn_SwtMgmtInfoType;
# else
typedef struct EthTSyn_SwtMgmtInfoTypeStruct
{
  uint8                SwitchIdx; /*!< EthIf switch index */
  uint8                PortIdx;   /*!< EthSwt port index */
  EthTSyn_FrameIdType  FrameId;   /*!< Frame ID to identify the frame */
} EthTSyn_SwtMgmtInfoType;
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

#ifndef ETHTSYN_SWT_MGMT_MSG_BUFFER_PAYLOAD_SIZE
/* Dummy size. SwtMgmt is disabled and type is not used. */
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_PAYLOAD_SIZE (1u)
#endif

/*! EthTSyn Generic message struct definition (used for message buffer) */
typedef struct EthTSyn_SwtMgmt_GenericMsgTypeStruct
{
  EthTSyn_CommonMsgHdrType   MsgHdr;
  uint8                      Payload[ETHTSYN_SWT_MGMT_MSG_BUFFER_PAYLOAD_SIZE];
}EthTSyn_SwtMgmt_GenericMsgType;

/*! EthTSyn Timestamp management struct definition */
typedef struct EthTSyn_SwtMgmt_TimeStampMgmtTypeStruct
{
  EthTSyn_TimestampStateType         TimestampState;
  EthTSyn_GlobalTimestampType        Timestamp;
}EthTSyn_SwtMgmt_TimeStampMgmtType;

/*! EthTSyn message buffer struct definition */
typedef struct EthTSyn_SwtMgmt_MsgBufferTypeStruct
{
  EthTSyn_SwtMgmt_MsgBufferStateType MsgBufferState;
  uint8                              CtrlIdx; /*!< EthIf Ctrl index of the received message */
  ETHTSYN_P2CONST(uint8)             RxBufAddr; /*!< RxBufPtr used to assign MgmtInfo/Ts indications to the message */
  EthTSyn_SwtMgmtInfoType            SwtMgmtInfo; /*!< Indicating the Switch Port the message was received on and the
                                                       Frame Id of the message */
  EthTSyn_SwtMgmt_TimeStampMgmtType  IngressTimestampMgmt;
# if (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON)
  EthTSyn_SwtMgmt_TimeStampMgmtType  EgressTimestampHostPortMgmt;
  EthTSyn_SwtMgmt_TimeStampMgmtType  IngressTimestampHostCtrlMgmt;
# else
  EthTSyn_TimestampRawType          IngressTimestampRaw;
# endif /* (ETHTSYN_SWT_MGMT_SWT_TIMESTAMP_SUPPORT == STD_ON) */
  EthTSyn_SwtMgmt_GenericMsgType     Msg;
}EthTSyn_SwtMgmt_MsgBufferType;

/*! EthTSyn Site Sync Sync state-machine struct definition */
typedef struct EthTSyn_SiteSyncSyncSmTypeStruct
{
  EthTSyn_StateMachineStateType            State;
  uint16                                   TxConfirmationPending;
  EthTSyn_SwtMgmt_GenericMsgType           Msg;
}EthTSyn_SiteSyncSyncSmType;

/*! EthTSyn TimeDomain struct definition */
typedef struct EthTSyn_TimeDomainTypeStruct
{
  EthTSyn_SiteSyncSyncSmType                SiteSyncSyncSm;
  EthTSyn_PortIdxType                       SlavePortIdx;
}EthTSyn_TimeDomainType;

/*! EthTSyn Rx Frame Info struct definition */
typedef struct EthTSyn_SwtMgmt_RxFrameInfoTypeStruct
{
  EthTSyn_GlobalTimestampType IngressTimestamp;
}EthTSyn_SwtMgmt_RxFrameInfoType;

#endif /* ETHTSYN_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Types.h
 *********************************************************************************************************************/
