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
/**        \file  Sd_Types.h
 *        \brief  Service Discovery header file
 *
 *      \details  Types definitions of Service Discovery (SD)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(SD_TYPES_H)
# define SD_TYPES_H

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA S 0777, 0779 EOF */ /* MD_MSR_5.1 */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "ComStack_Types.h"
# include "SoAd.h"
# include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 /* PRQA S 0850, 3453 2 */ /* MD_MSR_19.8, MD_MSR_19.7 */
# define SD_P2CONST(Type)    P2CONST(Type, AUTOMATIC, SD_APPL_DATA)
# define SD_P2VAR(Type)      P2VAR  (Type, AUTOMATIC, SD_APPL_VAR)

# define SD_HEADER_PROTOCOL_VERSION                           (0x01u)
# define SD_HEADER_INTERFACE_VERSION                          (0x01u)
# define SD_HEADER_MESSAGE_TYPE                               (0x02u)
# define SD_HEADER_RETURN_CODE                                (0x00u)
# define SD_FLAGS_REBOOT_MASK                                 (0x80u)
# define SD_FLAGS_UNICAST_MASK                                (0x40u)
# define SD_TCPIP_PORT_ANY                                    (0x0000u)

# define SD_SERVICE_ID_OTHERSERV                              (0xFFFEu)

# define SD_INV_DEM_ID                                        (0u)
# define SD_INV_TIMER_VALUE_16BIT                             (0xFFFFu)
# define SD_INV_TIMER_VALUE_32BIT                             (0xFFFFFFFFu)
# define SD_INV_OPTION_INDEX                                  (0xFFu)
# define SD_INV_OPTION_LENGTH                                 (0xFFFFu)

/* Entry Array wildcards */
# define SD_ENTRY_WILDCARD_SERVICE_ID                         (0xFFFFu)       /* Only allowed for FindService entries */
# define SD_ENTRY_WILDCARD_INSTANCE_ID                        (0xFFFFu)       /* Only allowed for FindService entries */
# define SD_ENTRY_WILDCARD_MAJOR_VERSION                      (0xFFu)         /* Only allowed for type 1 entries */
# define SD_ENTRY_WILDCARD_MINOR_VERSION                      (0xFFFFFFFFu)   /* Only allowed for type 1 entries */

/*
 * ----- SD Header -------------------------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |           Request ID (Client ID / Session ID) [32 bit]        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | Protocol Ver. | Interface Ver.| Message Type  |  Return Code  |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |    Flags     |                    Reserved                    |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |              Length of Entries Array [32 bit]                 |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                                                               |
 *  |                        Entries Array                          |
 *  |                                                          ...  |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |              Length of Options Array [32 bit]                 |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                                                               |
 *  |                        Options Array                          |
 *  |                                                          ...  |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 */

# define SD_HEADER_LENGTH                                     (12u)
# define SD_HEADER_CLIENTID_OFFSET                            (0u)
# define SD_HEADER_CLIENTID_SIZE                              (2u)
# define SD_HEADER_SESSIONID_OFFSET                           (2u)
# define SD_HEADER_SESSIONID_SIZE                             (2u)
# define SD_HEADER_PROTOCOLVERSION_OFFSET                     (4u)
# define SD_HEADER_PROTOCOLVERSION_SIZE                       (1u)
# define SD_HEADER_INTERFACEVERSION_OFFSET                    (5u)
# define SD_HEADER_INTERFACEVERSION_SIZE                      (1u)
# define SD_HEADER_MESSAGETYPE_OFFSET                         (6u)
# define SD_HEADER_MESSAGETYPE_SIZE                           (1u)
# define SD_HEADER_RETURNCODE_OFFSET                          (7u)
# define SD_HEADER_RETURNCODE_SIZE                            (1u)
# define SD_HEADER_FLAGS_OFFSET                               (8u)
# define SD_HEADER_FLAGS_SIZE                                 (1u)
# define SD_HEADER_RESERVED_OFFSET                            (9u)
# define SD_HEADER_RESERVED_SIZE                              (3u)
# define SD_HEADER_LENGTHOFENTRIESARRAY_SIZE                  (4u)
# define SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE                  (4u)

/* Static header fields */
# define SD_HEADER_CLIENTID                                   (0u) /* Statically set to 0 */
# define SD_HEADER_PROTOCOLVERSION                            (1u) /* Statically set to 1 */
# define SD_HEADER_INTERFACEVERSION                           (1u) /* Statically set to 1 */
# define SD_HEADER_MESSAGETYPE                                (2u) /* Statically set to 2 */
# define SD_HEADER_RETURNCODE                                 (0u) /* Statically set to 0 */
# define SD_HEADER_RESERVED                                   (0u) /* Statically set to 0 */

/*
 * ----- Entry Format -------------------------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |   Index 1st   |   Index 2nd   | #of 1 | #of 2 |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |         Service ID            |         Instance ID           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | Major Version |                 TTL                           |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * ---
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                         Minor Version                         |   Entry Format Type 1
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * OR
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |        Reserved       |Counter|        Eventgroup ID          |   Entry Format Type 2
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 */

# define SD_ENTRY_SIZE                                        (16u)
# define SD_ENTRY_TYPE_OFFSET                                 (0u)
# define SD_ENTRY_TYPE_SIZE                                   (1u)
# define SD_ENTRY_INDEX1_OFFSET                               (1u)
# define SD_ENTRY_INDEX1_SIZE                                 (1u)
# define SD_ENTRY_INDEX2_OFFSET                               (2u)
# define SD_ENTRY_INDEX2_SIZE                                 (1u)
# define SD_ENTRY_NROFOPTS_OFFSET                             (3u)
# define SD_ENTRY_NROFOPTS_SIZE                               (1u)
# define SD_ENTRY_SERVICEID_OFFSET                            (4u)
# define SD_ENTRY_SERVICEID_SIZE                              (3u)
# define SD_ENTRY_INSTANCEID_OFFSET                           (6u)
# define SD_ENTRY_INSTANCEID_SIZE                             (3u)
# define SD_ENTRY_MAJORVERSION_OFFSET                         (8u)
# define SD_ENTRY_MAJORVERSION_SIZE                           (1u)
# define SD_ENTRY_TTL_OFFSET                                  (9u)
# define SD_ENTRY_TTL_SIZE                                    (3u)
/* Entry Format Type 1 */
# define SD_ENTRY1_MINORVERSION_OFFSET                        (12u)
# define SD_ENTRY1_MINORVERSION_SIZE                          (4u)
/* Entry Format Type 2 */
# define SD_ENTRY2_RESERVEDCOUNTER_OFFSET                     (12u)
# define SD_ENTRY2_RESERVEDCOUNTER_SIZE                       (2u)
# define SD_ENTRY2_EVENTGROUPID_OFFSET                        (14u)
# define SD_ENTRY2_EVENTGROUPID_SIZE                          (2u)

/*
 * ----- Option -------------------------------------------------------------------------------------------------------
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |            Length             |     Type      |    Reserved   |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * ---
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |              Zero-terminated configuration string             |   Configuration Option
 *  |                ([len]id=value[len]id=value[0])                |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * OR
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                      IPv4 Address [32bit]                     |   IPv4 Endpoint Option
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    Reserved   |    L4-Proto   |         Port Number           |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * OR
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *  |                                                               |   IPv6 Endpoint Option
 *  |                      IPv6 Address [128bit]                    |
 *  |                                                               |
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    Reserved   |    L4-Proto   |         Port Number           |
 *  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 */

# define SD_OPTION_LENGTH_OFFSET                              (0u)
# define SD_OPTION_LENGTH_SIZE                                (2u)
# define SD_OPTION_TYPE_OFFSET                                (2u)
# define SD_OPTION_TYPE_SIZE                                  (1u)
# define SD_OPTION_RESERVED_OFFSET                            (3u)
# define SD_OPTION_RESERVED_SIZE                              (1u)
# define SD_OPTION_NOT_COVERED_FIELDS                         (SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE)

/* Configuration Option */
# define SD_OPTION_CONFIG_FIXED_HEADER_SIZE                   (4u)
# define SD_OPTION_CONFIG_STRING_OFFSET                       (4u)

/* IPv4 Endpoint Option */
# define SD_OPTION_IPV4_COVERED_LENGTH                        (0x09u)
# define SD_OPTION_IPV4_ADDRESS_OFFSET                        (4u)
# define SD_OPTION_IPV4_ADDRESS_SIZE                          (4u)
# define SD_OPTION_IPV4_RESERVED_OFFSET                       (8u)
# define SD_OPTION_IPV4_RESERVED_SIZE                         (1u)
# define SD_OPTION_IPV4_L4PROTO_OFFSET                        (9u)
# define SD_OPTION_IPV4_L4PROTO_SIZE                          (1u)
# define SD_OPTION_IPV4_PORTNUMBER_OFFSET                     (10u)
# define SD_OPTION_IPV4_PORTNUMBER_SIZE                       (2u)
# define SD_OPTION_IPV4_SIZE                                  (12u)

/* IPv6 Endpoint Option */
# define SD_OPTION_IPV6_COVERED_LENGTH                        (0x15u)
# define SD_OPTION_IPV6_ADDRESS_OFFSET                        (4u)
# define SD_OPTION_IPV6_ADDRESS_SIZE                          (16u)
# define SD_OPTION_IPV6_RESERVED_OFFSET                       (20u)
# define SD_OPTION_IPV6_RESERVED_SIZE                         (1u)
# define SD_OPTION_IPV6_L4PROTO_OFFSET                        (21u)
# define SD_OPTION_IPV6_L4PROTO_SIZE                          (1u)
# define SD_OPTION_IPV6_PORTNUMBER_OFFSET                     (22u)
# define SD_OPTION_IPV6_PORTNUMBER_SIZE                       (2u)
# define SD_OPTION_IPV6_SIZE                                  (24u)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8  Sd_StateType;
typedef uint16 Sd_Dem_EventIdType;
typedef uint16 Sd_FamilyType;

/* This type defines the fields of an SD message. */
typedef struct
{
  uint32                                                      Reserved;
  uint16                                                      ClientID;
  uint16                                                      SessionID;
  uint8                                                       ProtocolVersion;
  uint8                                                       InterfaceVersion;
  uint8                                                       MessageType;
  uint8                                                       ReturnCode;
  uint8                                                       Flags;
} Sd_HeaderType;

/* This type defines the format of all time and timer parameter. */
typedef struct
{
  uint32                                                      Seconds;
  uint16                                                      Milliseconds;
} Sd_ExtendedTimeType;

/* This type defines a container which can save IPv4 as well as IPv6 socket addresses. */
typedef struct
{
  Sd_FamilyType     family;
  SoAd_PortType     port;
  uint32            data[4];
} Sd_SockAddrContainerType;

typedef P2CONST(uint8, SD_APPL_DATA, AUTOMATIC)               Sd_ConfigOptionStringType;

/* This type defines the structure containing attached endpoint options and a reference to the configuration option. */
typedef struct
{
  SD_P2VAR(Sd_ConfigOptionStringType)                         ConfigOptionPtrArrayPtr;
  Sd_SockAddrContainerType                                    EndpointAddrUdp;
  Sd_SockAddrContainerType                                    EndpointAddrTcp;
} Sd_AttachedOptionsType;

/* This type defines the internal states of a instance. */
typedef enum
{
  SD_INSTANCE_DOWN                                            = SOAD_IPADDR_STATE_UNASSIGNED,
  SD_INSTANCE_UP_AND_CONFIGURED                               = SOAD_IPADDR_STATE_ASSIGNED
} Sd_ServiceInstanceStateType;

/* This type defines the type of endpoint options. */
typedef enum
{
  SD_ENDPOINT_TYPE_INVALID                                    = 0x00,
  SD_ENDPOINT_TYPE_TCP                                        = 0x06,
  SD_ENDPOINT_TYPE_UDP                                        = 0x11
} Sd_EndpointType;

/* This type defines the internal run state of the module. */
typedef enum
{
  SD_RUN_MODE_INITIAL_RUN                                     = 0u,
  SD_RUN_MODE_RUNNING                                         = 1u,
  SD_RUN_MODE_NEW_IP_ASSIGNED                                 = 2u
} Sd_RunModeType;

/* This type defines the match types for comparison of socket addresses. */
typedef enum
{
  SD_MATCH_FALSE                                              = 0u,
  SD_MATCH_ANY                                                = 2u,
  SD_MATCH_TRUE                                               = 3u
} Sd_MatchType;

/* This type defines the set of all match criteria for local and remote addresses. */
typedef struct
{
  Sd_MatchType                                                LocalIpMatchType;
  Sd_MatchType                                                LocalPortMatchType;
  Sd_MatchType                                                RemoteIpMatchType;
  Sd_MatchType                                                RemotePortMatchType;
} Sd_LocalRemoteMatchTypeType;

/* This type defines the modes of a socket connection. */
typedef enum
{
  SD_SOCON_MODE_ONLINE                                        = 0u,  /* SOAD_SOCON_ONLINE          0x00u */
  SD_SOCON_MODE_RECONNECT                                     = 1u,  /* SOAD_SOCON_RECONNECT       0x01u */
  SD_SOCON_MODE_OFFLINE                                       = 2u,  /* SOAD_SOCON_OFFLINE         0x02u */
  SD_SOCON_MODE_LOST_CONNECTION                               = 3u,
  SD_SOCON_MODE_INVALID                                       = 10u
} Sd_SoConModeType;

/* This type defines the possible entry types. */
typedef enum
{
  SD_ENTRY_NONE                                               = 0xAAu,
  SD_ENTRY_FIND_SERVICE                                       = 0x00u,  /* Entry format type 1 - Services */
  SD_ENTRY_OFFER_SERVICE                                      = 0x01u,  /* Entry format type 1 - Services */
  SD_ENTRY_STOP_OFFER_SERVICE                                 = 0x81u,  /* Entry format type 1 - Services        Additional type to differentiate SendEntries */
  SD_ENTRY_SUBSCRIBE_EVENTGROUP                               = 0x06u,  /* Entry format type 2 - Eventgroups */
  SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP                          = 0x86u,  /* Entry format type 2 - Eventgroups     Additional type to differentiate SendEntries */
  SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK                           = 0x07u,  /* Entry format type 2 - Eventgroups */
  SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK                          = 0x87u,  /* Entry format type 2 - Eventgroups     Additional type to differentiate SendEntries */
  SD_ENTRY_TYPE_MASK                                          = 0x7Fu
} Sd_EntryType;

/* This type defines the possible option types. */
typedef enum
{
  SD_OPTION_NONE                                              = 0xAAu,
  SD_OPTION_CONFIGURATION                                     = 0x01u,
  SD_OPTION_IPV4_ENDPOINT                                     = 0x04u,
  SD_OPTION_IPV6_ENDPOINT                                     = 0x06u,
  SD_OPTION_IPV4_MULTICAST                                    = 0x14u,
  SD_OPTION_IPV6_MULTICAST                                    = 0x16u,
  SD_OPTION_IPV4_SD_ENDPOINT                                  = 0x24u,
  SD_OPTION_IPV6_SD_ENDPOINT                                  = 0x26u,
  SD_OPTION_RESERVED                                          = 0x00u,
  SD_OPTION_MULTICAST_MASK                                    = 0x10u
} Sd_OptionType;

/* This type defines the values used for the option runs which map options to entries. */
typedef struct
{
  uint8                                                       Index1st;
  uint8                                                       Index2nd;
  uint8                                                       Nr1stOptions;
  uint8                                                       Nr2ndOptions;
} Sd_EntryToOptionMapType;

/* This type defines the control structure for the option array. */
typedef struct
{
  uint16                                                      ArrayStartIdx;
  uint16                                                      ArrayEndIdx;
  uint8                                                       FreeOptionIndex;
  uint8                                                       DeduplicatedOptionIndex;
} Sd_OptionArrayInfoType;

/* This type defines the protocol type of the option. */
typedef enum
{
  SD_L4PROTOCOL_NONE                                          = 0x00u,
  SD_L4PROTOCOL_TCP                                           = 0x06u,
  SD_L4PROTOCOL_UDP                                           = 0x11u
} Sd_L4ProtocolType;

/* This type defines the types of SubscribeEventgroup entries. */
typedef enum
{
  SD_SERVER_EVENT_EVENTGROUP_NONE                             = 0x00u,
  SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE                        = 0x10u,
  SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI                  = 0x11u,
  SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE                   = 0x20u,
  SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE_MULTI             = 0x21u
} Sd_ServerEventgroupEventType;

/* This type defines the types of the internal server state machine. */
typedef enum
{
  SD_SERVER_STATE_NONE                                        = 0u,
  SD_SERVER_STATE_NOT_READY                                   = 1u,
  SD_SERVER_STATE_READY_INITIAL_WAIT                          = 2u,
  SD_SERVER_STATE_READY_REPETITION                            = 3u,
  SD_SERVER_STATE_READY_MAIN_PHASE                            = 4u
} Sd_ServerStateMachineType;

/* This type defines the types of the internal event handler state machine. */
typedef enum
{
  SD_EVENT_HANDLER_STATE_NONE                                 = 0u,
  SD_EVENT_HANDLER_STATE_SERVICE_DOWN                         = 1u,
  SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED                       = 2u,
  SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION                 = 3u,
  SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION               = 4u
} Sd_EventHandlerStateMachineType;

/* This type defines the state of a remote client. */
typedef enum
{
  SD_EVENT_HANDLER_CLIENT_STATE_INVALID                       = 0u,
  SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED                  = 1u,
  SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED                    = 2u
} Sd_EventHandlerClientStateType;

/* This type defines the state of a pending trigger transmit request. */
typedef enum
{
  SD_TRIGGER_TRANSMIT_STATE_INVALID                           = 0u,
  SD_TRIGGER_TRANSMIT_STATE_NEXT_CYCLE                        = 1u,
  SD_TRIGGER_TRANSMIT_STATE_NOW                               = 2u
} Sd_TriggerTransmitStateType;

/* This type defines the type of OfferService entries. */
typedef enum
{
  SD_CLIENT_EVENT_SERVICE_NONE                                = 0x00u,
  SD_CLIENT_EVENT_SERVICE_STOP_OFFER                          = 0x10u,
  SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_MULTI              = 0x11u,
  SD_CLIENT_EVENT_SERVICE_STOP_OFFER_OFFER_UNI                = 0x12u,
  SD_CLIENT_EVENT_SERVICE_OFFER                               = 0x20u,
  SD_CLIENT_EVENT_SERVICE_OFFER_MULTI                         = 0x21u,
  SD_CLIENT_EVENT_SERVICE_OFFER_UNI                           = 0x22u,
  SD_CLIENT_EVENT_TYPE_MASK                                   = 0xF0u
} Sd_ClientServiceEventType;

/* This type defines the type of SubscribeEventgroupAcknowledge entries. */
typedef enum
{
  SD_CLIENT_EVENT_EVENTGROUP_NONE                             = 0u,
  SD_CLIENT_EVENT_EVENTGROUP_ACK                              = 3u,
  SD_CLIENT_EVENT_EVENTGROUP_NACK                             = 4u
} Sd_ClientEventgroupEventType;

/* This type defines the types of the internal client state machine. */
typedef enum
{
  SD_CLIENT_STATE_NONE                                        = 0u,
  SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN                      = 1u,
  SD_CLIENT_STATE_NOT_REQUESTED_SEEN                          = 2u,
  SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY                     = 3u,
  SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT                      = 4u,
  SD_CLIENT_STATE_SEARCHING_REPETITION                        = 5u,
  SD_CLIENT_STATE_SERVICE_READY                               = 6u,
  SD_CLIENT_STATE_STOPPED                                     = 7u
} Sd_ClientStateMachineType;

/* This type defines the types of the internal consumed eventgroup state machine. */
/* PRQA S 0779 7 */ /* MD_MSR_5.1_779 */
typedef enum
{
  SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED                   = 1u,
  SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT              = 2u,
  SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER  = 3u,
  SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RETRY        = 4u,
  SD_CONSUMED_EVENTGROUP_STATE_SUBSCRIBED                     = 5u
} Sd_ConsumedEventGroupStateMachineType;

/* This type defines the Server states that are reported to the SD using the expected API Sd_ServerServiceSetState. */
typedef enum
{
  SD_SERVER_SERVICE_DOWN                                      = 0u,
  SD_SERVER_SERVICE_AVAILABLE                                 = 1u
} Sd_ServerServiceSetStateType;

/* This type defines the Client states that are reported to the BswM using the expected API Sd_ClientServiceSetState. */
typedef enum
{
  SD_CLIENT_SERVICE_RELEASED                                  = 0u,
  SD_CLIENT_SERVICE_REQUESTED                                 = 1u
} Sd_ClientServiceSetStateType;

/* This type defines the subscription policy by consumed EventGroup for the Client Service. */
typedef enum
{
  SD_CONSUMED_EVENTGROUP_RELEASED                             = 0u,
  SD_CONSUMED_EVENTGROUP_REQUESTED                            = 1u
} Sd_ConsumedEventGroupSetStateType;

/* This type defines the modes to indicate the current mode request of a Client Service. */
typedef enum
{
  SD_CLIENT_SERVICE_DOWN                                      = 0u,
  SD_CLIENT_SERVICE_AVAILABLE                                 = 1u
} Sd_ClientServiceCurrentStateType;

/* This type defines the subscription policy by consumed EventGroup for the Client Service. */
typedef enum
{
  SD_CONSUMED_EVENTGROUP_DOWN                                 = 0u,
  SD_CONSUMED_EVENTGROUP_AVAILABLE                            = 1u
} Sd_ConsumedEventGroupCurrentStateType;

/* This type defines the subscription policy by EventHandler for the Server Service. */
typedef enum
{
  SD_EVENT_HANDLER_RELEASED                                   = 0u,
  SD_EVENT_HANDLER_REQUESTED                                  = 1u
} Sd_EventHandlerCurrentStateType;

/**********************************************************************************************************************
 *  GLOBAL CAPABILITY RECORD MACROS AND TYPES
 *********************************************************************************************************************/
/* This type defines the capability record function API defined by AUTOSAR. */
typedef P2FUNC(boolean, SD_CODE, Sd_CapabilityRecordCalloutFunctionApiType)  ( \
          PduIdType pduID, \
          uint8 type, \
          uint16 serviceID, \
          uint16 instanceID, \
          uint8 majorVersion, \
          uint32 minorVersion, \
          CONSTP2CONST(Sd_ConfigOptionStringType, AUTOMATIC, SD_APPL_DATA) receivedConfigOptionPtrArray, \
          CONSTP2CONST(Sd_ConfigOptionStringType, AUTOMATIC, SD_APPL_DATA) configuredConfigOptionPtrArray);

#endif /* SD_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Sd_Types.h
 *********************************************************************************************************************/
