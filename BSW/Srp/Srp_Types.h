/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Srp_Types.h
 *        \brief  SRP type definitions
 *
 *      \details  Type definitions of the Stream Registration Protocol (SRP)
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



#if !defined(SRP_TYPES_H)
# define SRP_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Srp_Cfg.h"
# include "Eth_Types.h"

/* PRQA S 0857 MACRO_LIMIT */ /* MD_MSR_1.1_857 */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 5 */ /* MD_MSR_19.7 */
# define SRP_P2CONSTCFGROOT(Type)  P2CONST(Type, AUTOMATIC, SRP_CONST)
# define SRP_P2CONSTCFG(Type)      P2CONST(Type, AUTOMATIC, SRP_CONST)
# define SRP_P2CONST(Type)         P2CONST(Type, AUTOMATIC, SRP_CONST)
# define SRP_P2VAR(Type)           P2VAR(Type, AUTOMATIC, SRP_VAR)
# define SRP_P2FUNC(RType, Type)   P2FUNC(RType, SRP_CODE, Type)

# define SRP_UINT_32_MAX           (0xFFFFFFFFu)
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*******************************************************DATA TYPES****************************************************/
/*! \brief      Srp VLAN Id Type */
typedef uint16  Srp_VLanIdType;
/*! \brief      Srp Unique Id Type */
typedef uint16  Srp_UniqueIdType;
/*! \brief      Srp Priority Type */
typedef uint8   Srp_PriorityType;
/*! \brief      Srp Port Index Type */
typedef uint8   Srp_PortIdxType;
/*! \brief      Srp Attribute Index Type */
typedef uint8   Srp_AttributeIdxType;
/*! \brief      Srp Accumulated Latency Type */
typedef uint32  Srp_AccumulatedLatencyType;
/*! \brief      MAC Address Type */
typedef uint8   Srp_MacAddressType[6];

/*! \brief      Srp State definitions */
typedef uint8  Srp_StateType;
# define SRP_STATE_UNINIT                                                      (0u)
# define SRP_STATE_INIT                                                        (1u)

/*! \brief      Reservation Failure Codes */
typedef uint8  Srp_ReservationFailureCodeType;
# define SRP_FAILURE_CODE_UNKNOWN                                              (0u)
# define SRP_FAILURE_CODE_INSUFFICIENT_BANDWITH                                (1u)
# define SRP_FAILURE_CODE_INSUFFICIENT_BRIDGE_RESOURCES                        (2u)
# define SRP_FAILURE_CODE_INSUFFICIENT_BANDWITH_FOR_TRAFFIC_CLASS              (3u)
# define SRP_FAILURE_CODE_STREAM_ID_IN_USE_BY_ANOTHER_TALKER                   (4u)
# define SRP_FAILURE_CODE_STREAM_DESTINATION_ADDRESS_ALREADY_IN_USE            (5u)
# define SRP_FAILURE_CODE_STREAM_PREEMPTED_BY_HIGHER_RANK                      (6u)
# define SRP_FAILURE_CODE_REPORTED_LATENCY_HAS_CHANGED                         (7u)
# define SRP_FAILURE_CODE_EGRESS_PORT_IS_NOT_AVB_CAPABLE                       (8u)
# define SRP_FAILURE_CODE_USE_A_DIFFERENT_DESTINATION_ADDRESS                  (9u)
# define SRP_FAILURE_CODE_OUT_OF_MSRP_RESOURCES                                (10u)
# define SRP_FAILURE_CODE_OUT_OF_MMRP_RESOURCES                                (11u)
# define SRP_FAILURE_CODE_CANNOT_STORE_DESTINATION_ADDRESS                     (12u)
# define SRP_FAILURE_CODE_REQUESTED_PRIORITY_IS_NOT_AN_SR_CLASS                (13u)
# define SRP_FAILURE_CODE_MAX_FRAME_SIZE_IS_TOO_LARGE_FOR_MEDIA                (14u)
# define SRP_FAILURE_CODE_MSRP_MAX_FAN_IN_PORTS_LIMIT_HAS_BEEN_REACHED         (15u)
# define SRP_FAILURE_CODE_CHANGES_IN_FIRST_VALUE_FOR_A_REGISTERED_STREAM_ID    (16u)
# define SRP_FAILURE_CODE_VLAN_IS_BLOCKED_ON_THIS_EGRESS_PORT                  (17u)
# define SRP_FAILURE_CODE_VLAN_TAGGING_IS_DIABLED_ON_THIS_EGRESS_PORT          (18u)
# define SRP_FAILURE_CODE_SR_CLASS_PRIORITY_MISMATCH                           (19u)

/*! \brief      Srp Rank Type */
typedef uint8   Srp_RankType;
# define SRP_RANK_EMERGENCY        (0u)
# define SRP_RANK_NON_EMERGENCY    (1u)

/*! \brief      Talker Declaration Type */
typedef uint8  Srp_TalkerDeclarationType;
# define SRP_DECLARATION_TYPE_TALKER_ADVERTISE    (1u)
# define SRP_DECLARATION_TYPE_TALKER_FAILED       (2u)

/*! \brief      Listener Declaration Type */
typedef uint8  Srp_ListenerDeclarationType;
# define SRP_DECLARATION_TYPE_LISTENER_IGNORE           (0u)
# define SRP_DECLARATION_TYPE_LISTENER_ASKING_FAILED    (1u)
# define SRP_DECLARATION_TYPE_LISTENER_READY            (2u)
# define SRP_DECLARATION_TYPE_LISTENER_READY_FAILED     (3u)

#if (SRP_MMRP_SUPPORT == STD_ON)
/*! \brief      MMRP Attribute Type */
typedef uint8  Srp_MmrpAttributeType;
# define SRP_MMRP_SERVICE_REQUIREMENT_TYPE  (1u)
# define SRP_MMRP_MAC_TYPE                  (2u)

/*! \brief      MMMRP Service Requirement Attribute Type */
typedef uint8  Srp_MmrpServiceRequirementAttributeType;
# define SRP_MMRP_ALL_GROUPS                            (0u)
# define SRP_MMRP_ALL_UNREGISTERED_GROUPS               (1u)
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/*! \brief      Srp State Machine State Type */
typedef uint8  Srp_StateMachineStateType;
/*! \brief      Applicant State Machine States */
# define SRP_STATE_APPLICANT_VO                  (0u)  /* Very anxious Observer */
# define SRP_STATE_APPLICANT_VP                  (1u)  /* Very anxious Passive */
# define SRP_STATE_APPLICANT_VN                  (2u)  /* Very anxious New */
# define SRP_STATE_APPLICANT_AN                  (3u)  /* Anxious New */
# define SRP_STATE_APPLICANT_AA                  (4u)  /* Anxious Active */
# define SRP_STATE_APPLICANT_QA                  (5u)  /* Quiet Active */
# define SRP_STATE_APPLICANT_LA                  (6u)  /* Leaving Active */
# define SRP_STATE_APPLICANT_AO                  (7u)  /* Anxious Observer */
# define SRP_STATE_APPLICANT_QO                  (8u)  /* Quiet Observer */
# define SRP_STATE_APPLICANT_AP                  (9u)  /* Anxious Passive */
# define SRP_STATE_APPLICANT_QP                  (10u) /* Quiet Passive */
# define SRP_STATE_APPLICANT_LO                  (11u) /* Leaving Observer */
/*! \brief      Registrar State Machine States */
# define SRP_STATE_REGISTRAR_IN                  (0u) /* In */
# define SRP_STATE_REGISTRAR_LV                  (1u) /* Leaving */
# define SRP_STATE_REGISTRAR_MT                  (2u) /* Empty */
/*! \brief      LeaveAll State Machine States */
# define SRP_STATE_LEAVEL_ALL_ACTIVE             (0u) /* Active */
# define SRP_STATE_LEAVEL_ALL_PASSIVE            (1u) /* Passive */
/*! \brief      PeriodicTransmission State Machine States */
# define SRP_STATE_PERIODIC_TRANSMISSION_ACTIVE  (0u) /* Active */
# define SRP_STATE_PERIODIC_TRANSMISSION_PASSIVE (1u) /* Passive */

/*! \brief      Srp Link State Type */
typedef uint8  Srp_LinkStateType;
# define SRP_LINKE_STATE_DOWN                    (0u)
# define SRP_LINKE_STATE_ACTIVE                  (1u)

/*! \brief      Srp MRP Event Type */
typedef uint8  Srp_MrpEventType;
# define SRP_MRP_EVENT_R_NEW                     (0u)  /* rNew!: Receive New message */
# define SRP_MRP_EVENT_R_JOIN_IN                 (1u)  /* rJoinIn!: Receive JoinIn message */
# define SRP_MRP_EVENT_R_IN                      (2u)  /* rIn!: Receive In message */
# define SRP_MRP_EVENT_R_JOIN_MT                 (3u)  /* rJoinMt!: Receive JoinEmpty message */
# define SRP_MRP_EVENT_R_MT                      (4u)  /* rMt!: Receive Empty message */
# define SRP_MRP_EVENT_R_LV                      (5u)  /* rLv!: Receive Leave message */
# define SRP_MRP_EVENT_R_LA                      (6u) /* rLA!: Receive a LeaveAll message */

# define SRP_MRP_EVENT_BEGIN                     (7u)  /* Begin!: Initialize state machine */
# define SRP_MRP_EVENT_NEW                       (8u)  /* New!: A new declaration */
# define SRP_MRP_EVENT_JOIN                      (9u)  /* Join!: Declaration without signaling new registration */
# define SRP_MRP_EVENT_LV                        (10u)  /* Lv!: Withdraw a declaration */

# define SRP_MRP_EVENT_RE_DECLARE                (11u) /* Re-Declare: Port role changes from Designated to Root or 
                                                          Alternate Port */
# define SRP_MRP_EVENT_PERIODIC                  (12u) /* periodic!: A periodic transmission event occurs */
# define SRP_MRP_EVENT_TX                        (13u) /* tx!: Transmission opportunity without a LeaveAll */
# define SRP_MRP_EVENT_TX_LA                     (14u) /* txLA!: Transmission opportunity with a LeaveAll */
# define SRP_MRP_EVENT_TX_LAF                    (15u) /* txLAF!: Transmission opportunity with a LeaveAll, 
                                                          and with no room (Full) */
# define SRP_MRP_EVENT_FLUSH                     (16u) /* Flush!: Port role changes from Root or Alternate Port to
                                                          Designated Port */
# define SRP_MRP_EVENT_LEAVE_TIMER               (17u) /* leavetimer!: LeaveTimer has expired */
# define SRP_MRP_EVENT_LEAVE_ALL_TIMER           (18u) /* leavealltimer!: LeaveAllTimer has expired */
# define SRP_MRP_EVENT_PERIODIC_TIMER            (19u) /* periodictimer!: PeriodicTimer has expired */

/*! \brief      Srp Leave All Type */
typedef uint8  Srp_LeaveAllType;
# define SRP_LEAVE_ALL_EVENT_NULL                (0x00u) /* No LeaveAll event */
# define SRP_LEAVE_ALL_EVENT                     (0x01u) /* LeaveAll event */

/*! \brief      Srp Attribute Event Type */
typedef uint8  Srp_AttributeEventType;
# define SRP_ATTRIBUTE_EVENT_NEW                 (0u) /* New */
# define SRP_ATTRIBUTE_EVENT_JOIN_IN             (1u) /* Join In */
# define SRP_ATTRIBUTE_EVENT_IN                  (2u) /* In */
# define SRP_ATTRIBUTE_EVENT_JOIN_MT             (3u) /* Join Empty */
# define SRP_ATTRIBUTE_EVENT_MT                  (4u) /* Empty */
# define SRP_ATTRIBUTE_EVENT_LV                  (5u) /* Leave */

/*! \brief      Srp Ether Type Type */
typedef uint16  Srp_EtherTypeType;
# define SRP_MMRP_ETHER_TYPE                     (0x88F6u)
# define SRP_MVRP_ETHER_TYPE                     (0x88F5u)
# define SRP_MSRP_ETHER_TYPE                     (0x22EAu)

/*! \brief      Srp MSRP Attribute Type */
typedef uint8  Srp_MsrpAttributeType;
# define SRP_MSRP_TALKER_ADVERTISE               (1u)
# define SRP_MSRP_TALKER_FAILED                  (2u)
# define SRP_MSRP_LISTENER                       (3u)
# define SRP_MSRP_DOMAIN                         (4u)

/*! \brief      Srp SR class Type */
typedef uint8  Srp_SrClassType;
# define SRP_SR_CLASS_A                          (0u)
# define SRP_SR_CLASS_B                          (1u)
# define SRP_INV_SR_CLASS                        (255u)

/* PRQA L: MACRO_LIMIT */
/*******************************************************END DATA TYPES************************************************/

/*******************************************************STRUCTURES****************************************************/

/*! \brief      Stream ID Type */
typedef struct
{
  Srp_MacAddressType  SourceAddress;
  Srp_UniqueIdType    UniqueId;
}Srp_StreamIdType;

/*! \brief      Failure Information Type */
typedef struct
{
  uint64                          BridgeID;
  Srp_ReservationFailureCodeType  ReservationFailuredCode;
}Srp_FailureInformationType;

/*! \brief      TSpec Type */
typedef struct
{
  uint16                         MaxFrameSize;
  uint16                         MaxIntervalFrames;
}Srp_TSpecType;

/*! \brief      Data Frame Parameters Type */
typedef struct
{
  Srp_MacAddressType             DestinationAddress;
  Srp_VLanIdType                 VLanIdentifier;
}Srp_DataFrameParametersType;

#if (SRP_MMRP_SUPPORT == STD_ON)
/*! \brief      MMRP Attribute Value Type */
typedef struct
{
  Srp_MmrpAttributeType                    AttributeType;
  Srp_MacAddressType                       MacAddress;
  Srp_MmrpServiceRequirementAttributeType  ServiceReq;
}Srp_MmrpAttributeValueType;
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/*! \brief      MSRP Stream Info Type */
typedef struct
{
  Srp_StreamIdType               StreamId;
  Srp_DataFrameParametersType    DataFrameParameters;
  Srp_TSpecType                  TSpec;
  Srp_PriorityType               Priority;
  Srp_RankType                   Rank;
  Srp_AccumulatedLatencyType     AccumulatedLatency;
  Srp_FailureInformationType     FailureInformation;
}Srp_MsrpStreamInfoType;

/*! \brief      MSRP Register Stream Info Type */
typedef struct
{
  Srp_StreamIdType               StreamId;
  Srp_DataFrameParametersType    DataFrameParameters;
  Srp_TSpecType                  TSpec;
  Srp_PriorityType               Priority;
  Srp_RankType                   Rank;
  Srp_AccumulatedLatencyType     AccumulatedLatency;
}Srp_MsrpRegisterStreamInfoType;

/*! \brief      MSRP Domain Info Type */
typedef struct
{
  uint8                          SrClassId;
  uint8                          SrClassPriority;
  uint16                         SrClassVid;
}Srp_MsrpDomainInfoType;

/*! \brief      MSRP Attribute Value Type */
typedef struct
{
  Srp_MsrpAttributeType          AttributeType;
  Srp_MsrpStreamInfoType         StreamInfo;
  Srp_MsrpDomainInfoType         DomainInfo;
  Srp_ListenerDeclarationType    ListenerDeclaration;
  uint32                         ReservedRxBandwidth;
}Srp_MsrpAttributeValueType;


/*******************************************************STATE MACHINE Types*******************************************/
/*! \brief      Registrar State Machine Type */
typedef struct
{
  Srp_StateMachineStateType      State;
  uint32                         LeaveTimer;
  boolean                        LeaveTimerRunning;
}Srp_RegistrarStateMachineType;

/*! \brief      Applicant State Machine Type */
typedef struct
{
  Srp_StateMachineStateType                   State;
  SRP_P2CONST(Srp_RegistrarStateMachineType)  RegistrarSmPtr;
  boolean                                     SmIsValid;
  boolean                                     RequestTx;
}Srp_ApplicantStateMachineType;

/*! \brief      LeaveAll State Machine Type */
typedef struct
{
  Srp_StateMachineStateType      State;
  boolean                        LeaveAllTimerRunning;
  uint32                         LeaveAllTimer;
}Srp_LeaveAllStateMachineType;

/*! \brief      PeriodicTransmission State Machine Type */
typedef struct
{
  Srp_StateMachineStateType      State;
  boolean                        PeriodicTimerRunning;
  uint32                         PeriodicTimer;
}Srp_PeridodicTransmissionStateMachineType;
/*******************************************************END STATE MACHINE Types***************************************/

/*******************************************************PARTICIPANT TYPES*********************************************/
typedef uint8  Srp_MrpApplicationType;
# define SRP_MMRP_APPLICATION                (0u)
# define SRP_MVRP_APPLICATION                (1u)
# define SRP_MSRP_APPLICATION                (2u)

typedef struct
{
  Srp_MrpApplicationType                     MrpApplication;
  uint32                                     JoinTimer;
  uint8                                      BufIdx;
  SRP_P2VAR(uint8)                           BufPtr;
  uint16                                     LenByte;
  uint16                                     UsedLenByte;
  Srp_LeaveAllStateMachineType               LeaveAllSM;
  Srp_PeridodicTransmissionStateMachineType  PeriodicTransmissionSM;
}Srp_MrpParticipantType; /* It is intended to cast this Type to Srp_MXrpParticipantType (X: M||V||S). Therefore it is
                          mandatory that the first struct members of the 'Sub-Types' are equal (incl. the Order)  */

#if (SRP_MMRP_SUPPORT == STD_ON)
/*! \brief      MMRP Participant Type */
typedef struct
{
  Srp_MrpApplicationType                     MrpApplication;
  uint32                                     JoinTimer;
  uint8                                      BufIdx;
  SRP_P2VAR(uint8)                           BufPtr;
  uint16                                     LenByte;
  uint16                                     UsedLenByte;
  Srp_LeaveAllStateMachineType               LeaveAllSM;
  Srp_PeridodicTransmissionStateMachineType  PeriodicTransmissionSM;
  Srp_ApplicantStateMachineType              ApplicantSM[SRP_SUPPORTED_MMRP_ATTRIBUTES];
  Srp_RegistrarStateMachineType              RegistrarSM[SRP_SUPPORTED_MMRP_ATTRIBUTES];
  Srp_MmrpAttributeValueType                 Attribute[SRP_SUPPORTED_MMRP_ATTRIBUTES];
  boolean                                    AttributeIsValid[SRP_SUPPORTED_MMRP_ATTRIBUTES];
}Srp_MmrpParticipantType;
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/*! \brief      MVRP Participant Type */
typedef struct
{
  Srp_MrpApplicationType                     MrpApplication;
  uint32                                     JoinTimer;
  uint8                                      BufIdx;
  SRP_P2VAR(uint8)                           BufPtr;
  uint16                                     LenByte;
  uint16                                     UsedLenByte;
  Srp_LeaveAllStateMachineType               LeaveAllSM;
  Srp_PeridodicTransmissionStateMachineType  PeriodicTransmissionSM;
  Srp_ApplicantStateMachineType              ApplicantSM[SRP_SUPPORTED_MVRP_ATTRIBUTES];
  Srp_RegistrarStateMachineType              RegistrarSM[SRP_SUPPORTED_MVRP_ATTRIBUTES];
  Srp_VLanIdType                             VlanId[SRP_SUPPORTED_MVRP_ATTRIBUTES];
  boolean                                    AttributeIsValid[SRP_SUPPORTED_MVRP_ATTRIBUTES];
}Srp_MvrpParticipantType;

/*! \brief      MSRP Participant Type */
typedef struct
{
  Srp_MrpApplicationType                     MrpApplication;
  uint32                                     JoinTimer;
  uint8                                      BufIdx;
  SRP_P2VAR(uint8)                           BufPtr;
  uint16                                     LenByte;
  uint16                                     UsedLenByte;
  Srp_LeaveAllStateMachineType               LeaveAllSM;
  Srp_PeridodicTransmissionStateMachineType  PeriodicTransmissionSM;
  Srp_ApplicantStateMachineType              ApplicantSM[SRP_SUPPORTED_MSRP_ATTRIBUTES];
  Srp_RegistrarStateMachineType              RegistrarSM[SRP_SUPPORTED_MSRP_ATTRIBUTES];
  Srp_MsrpAttributeValueType                 Attribute[SRP_SUPPORTED_MSRP_ATTRIBUTES];
  boolean                                    AttributeIsValid[SRP_SUPPORTED_MSRP_ATTRIBUTES];
}Srp_MsrpParticipantType;
/*******************************************************END PARTICIPANT TYPES*****************************************/

/*******************************************************APPLICATION CALLBACK TYPES************************************/

/*! \brief      MSRP Attribute Info Type */
typedef struct
{
  Srp_MsrpAttributeType                      AttributeType;
  Srp_PortIdxType                            PortIdx;
  SRP_P2CONST(Srp_MsrpStreamInfoType)        StreamInfoPtr;
}Srp_MsrpAttributeInfoType;

/*! \brief      MSRP Register Stream Cbk Type */
typedef SRP_P2FUNC(void, Srp_RegisterStreamCbkPtrType)  (SRP_P2CONST(Srp_MsrpAttributeInfoType));

/*! \brief      MSRP Register Attach Cbk Type */
typedef SRP_P2FUNC(void, Srp_RegisterAttachCbkPtrType)   (SRP_P2CONST(Srp_MsrpAttributeInfoType));

/*! \brief      MSRP De-register Stream Cbk Type */
typedef SRP_P2FUNC(void, Srp_DeregisterStreamCbkPtrType)   (SRP_P2CONST(Srp_MsrpAttributeInfoType));

/*! \brief      MSRP De-register Attach Cbk Type */
typedef SRP_P2FUNC(void, Srp_DeregisterAttachCbkPtrType)   (SRP_P2CONST(Srp_MsrpAttributeInfoType));

/*******************************************************END APPLICATION CALLBACK TYPES********************************/

/*******************************************************MESSAGE TYPES*************************************************/
/*! \brief      Srp Common Header Type */
typedef struct
{
  uint8                          ProtocolVersion;
}Srp_CommonHeaderType;

/*! \brief      Srp Common Message Type */
typedef struct
{
  uint8                          AttributeType;
  uint8                          AttributeLength;
}Srp_CommonMsgType;


#if (SRP_MMRP_SUPPORT == STD_ON)
/*! \brief      MMRP MAC Message Type */
typedef struct
{
  uint16                          VectorHeader;
  Srp_MacAddressType              FirstValue;
  uint8                           Vector[SRP_EVENTS_CNT];
  uint16                          EndMark;
}Srp_MmrpMacMsgType;

/*! \brief      MMRP Service Requirement Message Type */
typedef struct
{
  uint16                                      VectorHeader;
  Srp_MmrpServiceRequirementAttributeType     FirstValue;
  uint8                                       Vector[SRP_EVENTS_CNT];
  uint16                                      EndMark;
}Srp_MmrpServiceReqMsgType;
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/*! \brief      MVRP Message Type */
typedef struct
{
  uint16                         VectorHeader;
  Srp_VLanIdType                 FirstValue;
  uint8                          Vector[SRP_EVENTS_CNT];
  uint16                         EndMark;
}Srp_MvrpMsgType;

/*! \brief      MSRP Message Type */
typedef struct
{
  uint16                         AttributeListLength;
}Srp_MsrpMsgType;

/*! \brief      MSRP Talker Advertise Message Type */
typedef struct
{
  uint16                         VectorHeader;
  /* FirstValue */
  Srp_StreamIdType               StreamId;
  Srp_DataFrameParametersType    DataFrameParameters;
  Srp_TSpecType                  TSpec;
  uint8                          PriorityAndRank; /* 3Bits DataFramePriority, 1Bit Rank, 4Bits Reserved */
  uint32                         AccumulatedLatency;
  /* End FirstValue */
  uint8                          Vector[SRP_EVENTS_CNT];
  uint16                         EndMark;
}Srp_MsrpTalkerAdMsgType;

/*! \brief      MSRP Talker Failed Message Type */
typedef struct
{
  uint16                         VectorHeader;
  /* FirstValue */
  Srp_StreamIdType               StreamId;
  Srp_DataFrameParametersType    DataFrameParameters;
  Srp_TSpecType                  TSpec;
  uint8                          PriorityAndRank; /* 3Bits DataFramePriority, 1Bit Rank, 4Bits Reserved */
  uint32                         AccumulatedLatency;
  Srp_FailureInformationType     FailureInformation;
  /* End FirstValue */
  uint8                          Vector[SRP_EVENTS_CNT];
  uint16                         EndMark;
}Srp_MsrpTalkerFailedMsgType;

/*! \brief      MSRP Listener Message Type */
typedef struct
{
  uint16                         VectorHeader;
  /* FirstValue */
  Srp_StreamIdType               StreamId;
  /* End FirstValue */
  uint8                          Vector[SRP_EVENTS_CNT*2];
  uint16                         EndMark;
}Srp_MsrpListenerMsgType;

/*! \brief      MSRP Domain Message Type */
typedef struct
{
  uint16                         VectorHeader;
  /* FirstValue */
  uint8                          SRclassId;
  uint8                          SRclassPriority;
  uint16                         SRclassVid;
  /* End FirstValue */
  uint8                          Vector[SRP_EVENTS_CNT];
  uint16                         EndMark;
}Srp_MsrpDomainMsgType;

/*******************************************************END MESSAGE TYPES*********************************************/

/*! \brief      Srp Port State Type */
typedef struct
{
# if (SRP_MMRP_SUPPORT == STD_ON)
  Srp_MmrpParticipantType        MmrpParticipant;
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */
  Srp_MvrpParticipantType        MvrpParticipant;
  Srp_MsrpParticipantType        MsrpParticipant;
# if (SRP_TYPE == SRP_TYPE_STATIC)
  uint32                         UsedBandwithTx;
# endif /* (SRP_TYPE == SRP_TYPE_STATIC) */
  uint32                         UsedBandwithRx;
  boolean                        Enabled;
}Srp_PortStateType;
/*******************************************************END STRUCTURES************************************************/

/*******************************************************CONFIG TYPES**************************************************/
/*! \brief      Srp Config Type */
#if (SRP_CONFIGURATION_VARIANT == SRP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
typedef struct
{
  uint8                                Dummy;
} Srp_VPostBuildConfigType;
typedef Srp_VPostBuildConfigType   Srp_ConfigType;
#else
typedef uint8                      Srp_ConfigType;
#endif /* ( SRP_CONFIGURATION_VARIANT == SRP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) */
/*******************************************************END CONFIG TYPES**********************************************/

#endif /* SRP_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Srp_Types.h
 *********************************************************************************************************************/
