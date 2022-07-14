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
/**        \file  Dlt.c
 *        \brief  Dlt source file
 *
 *      \details  Source file of the AUTOSAR Diagnostic Log and Trace, according to:
 *                AUTOSAR Diagnostic Log and Trace, AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

 /*********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define DLT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dlt.h"
#include "Rte_Dlt.h"

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# include "DltCom.h"
#endif /* (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#if (DLT_DEV_ERROR_REPORT == STD_ON)
# include "Det.h" /* Include Development Error Tracer */
#endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */

#if defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# include "Xcp.h"
#endif /* (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) */

#if (DLT_USE_VERBOSE_MODE == STD_ON)
# include "VStdLib.h"            /* for VStdMemCpy */
#endif /* (DLT_USE_VERBOSE_MODE == STD_ON) */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/*lint -e522 */ /* Suppress ID522 because all functions without side-effect have to have no side-effect */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
/* Dlt500 */
#if((DLT_SW_MAJOR_VERSION != (0x01u)) || (DLT_SW_MINOR_VERSION != (0x07u)) || (DLT_SW_PATCH_VERSION != (0x02u)))
# error "Vendor specific version numbers of Dlt.c and Dlt.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#define Dlt_GetCurrentTime() DLT_GET_TIME_MEASUREMENT_FCT()

# if defined (DLT_IMPLEMENT_TIMESTAMP)
#define DLT_MIN_MESSAGE_LENGTH                  0x19UL
# else /* (DLT_IMPLEMENT_TIMESTAMP) */
#define DLT_MIN_MESSAGE_LENGTH                  0x15UL
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */

#define DLT_NUMBER_OF_SERVICES                  0x1Fu

#define kDltStateMachineStates                  DLT_STATE_NUMBER_OF_STATES
#define kDltStateMachineEvents                  0x7u

/* Header Type Masks */
#define Dlt_StandardHeaderType_UEH_Mask         0x01u /* Mask for Use Extended Header */
#define Dlt_StandardHeaderType_MSBF_Mask        0x02u /* Mask for Most Significant Byte First - if set => Little Endian, else Big Endian */
#define Dlt_StandardHeaderType_WEID_Mask        0x04u /* Mask for With ECU ID */
#define Dlt_StandardHeaderType_WSID_Mask        0x08u /* Mask for With Session ID */
#define Dlt_StandardHeaderType_WTMS_Mask        0x10u /* Mask for With Timestamp */
#define Dlt_StandardHeaderType_SWV_Mask         0xE0u /* Mask for Software Version */

#define Dlt_ExtendedHeaderType_VERB_Mask        0x01u /* Mask for Verbose Mode */
#define Dlt_ExtendedHeaderType_MSTP_Mask        0x0Eu /* Mask for Message Type */
#define Dlt_ExtendedHeaderType_MTIN_Mask        0xF0u /* Mask for Message Type Info */

#define Dlt_MessageTypeTypeMask                 0x07u /* Mask for Dlt_MessageOptionsType->Dlt_MessageTypeType: 
                                                       * DLT_TYPE_LOG = 0, DLT_TYPE_APP_TRACE = 1, 
                                                       * DLT_TYPE_NW_TRACE = 2, DLT_TYPE_CONTROL = 3, */

/* Control message pre-config */
#define Dlt_ControlMessageHeaderType_MSBF       0x37u /* Header type  - 0b001 1 0 1 1 1 - Version 3 | WTMS 1 | SID 0 | WEID 1 | MSBF 1 | UEH 1 */
#define Dlt_ControlMessageHeaderType_LSBF       0x35u /* Header type  - 0b001 1 0 1 0 1 - Version 3 | WTMS 1 | SID 0 | WEID 1 | MSBF 0 | UEH 1 */
#define Dlt_ControlMessageMessageInfo           0x26u /* Message Info - 0b0010 011 0    - Response | Control | Non-Verbose */

#if !defined (DLT_TIMEOUT)
#define DLT_TIMEOUT                             0x200u /* Timeout limit - 1024*MainFunctionCycle */
#endif

#define DLT_DEM_APPLID_DESC_LENGTH              0x0Au
#define DLT_DEM_CONTEXTID_DESC_LENGTH           0x05u

#define DLT_DET_APPLID_DESC_LENGTH              0x0Au
#define DLT_DET_CONTEXTID_DESC_LENGTH           0x04u

/* Dlt_OptionType */
#define DLT_OPT_LOGLEVEL                        0x00u /*!< Option type value: set to log. */
#define DLT_OPT_TRACESTATUS                     0x01u /*!< Option type value: set to trace. */
#define DLT_OPT_VERBOSEMODE                     0x02u /*!< Option type value: set to verbose. */
#define DLT_OPT_BOOLEAN                         0x03u /*!< Option type value: set to a boolean attribute. */

/* Use this statement for Payload, but not for Service ID */
# ifndef Dlt_IsPayloadInBigEndian
#define Dlt_IsPayloadInBigEndian() ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_MSBF_Mask) == Dlt_StandardHeaderType_MSBF_Mask)
# endif

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 1503, 1504, 1505, 1514 EOF */ /* MD_MSR_14.1 */ /* MD_MSR_8.10 */ /* MD_MSR_8.10 */ /* MD_Dlt_1514 */

/* PRQA S 3453 FUNCTIONLIKEMACRO_C */ /*  MD_MSR_19.7 */

#if defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if !defined (Dlt_TransmitData)
#define Dlt_TransmitData(event, data, dataLength)  Xcp_Event(event)
# endif /* !defined (Dlt_TransmitData) */
#endif /* defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if !defined (Dlt_TransmitData)
#define Dlt_TransmitData(event, data, dataLength)  Dlt_TxBuf_WriteToBuf(event, data, dataLength)
# endif /* !defined (Dlt_TransmitData) */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/* PRQA L:FUNCTIONLIKEMACRO_C */ /* MD_MSR_19.7 */

/*********************************************************************************************************************/
/* General definition of checking macros */
/* PRQA S 0857 17 */ /* MD_MSR_1.1_857 */
/* PRQA S 3458 16 */ /* MD_MSR_19.4 */
# define Dlt_CheckErrorReturnVoid(cond, api, err)        \
{ if(!(cond)){                                           \
    Dlt_CallDetReportError((api), (err));                \
    return;                                              \
}}

# define Dlt_CheckErrorReturnValue(cond, api, err, ret)  \
{ if(!(cond)){                                           \
    Dlt_CallDetReportError((api), (err));                \
    return(ret);                                         \
}}

# define Dlt_CheckErrorContinue(cond, api, err)          \
{ if(!(cond)){                                           \
    Dlt_CallDetReportError((api), (err));                \
}}

/*********************************************************************************************************************/
/* Macros for DET abstraction */

/* Dlt444, Dlt439 */
/* PRQA S 3453 10 */ /*  MD_MSR_19.7 */
#if(DLT_DEV_ERROR_DETECT == STD_ON)
# define Dlt_CheckDetErrorReturnVoid(cond, api, err)       Dlt_CheckErrorReturnVoid((cond), (api), (err))
# define Dlt_CheckDetErrorReturnValue(cond, api, err, ret) Dlt_CheckErrorReturnValue((cond), (api), (err), (ret))
# define Dlt_CheckDetErrorContinue(cond, api, err)         Dlt_CheckErrorContinue((cond), (api), (err))

# if (DLT_DEV_ERROR_REPORT == STD_ON)
# define Dlt_CallDetReportError(api, err)                  (void)Det_ReportError(DLT_MODULE_ID, 0, (api), (err)) /* ESCAN00087203 */
# else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
# define Dlt_CallDetReportError(api, err)
# endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

#else
# define Dlt_CheckDetErrorReturnVoid(cond, api, err)
# define Dlt_CheckDetErrorReturnValue(cond, api, err, ret) 
# define Dlt_CheckDetErrorContinue(cond, api, err) 
# define Dlt_CallDetReportError(api, err) 
#endif

/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
#  define DLT_BEGIN_CRITICAL_SECTION_0() SchM_Enter_Dlt_DLT_EXCLUSIVE_AREA_0()
#  define DLT_END_CRITICAL_SECTION_0()   SchM_Exit_Dlt_DLT_EXCLUSIVE_AREA_0()

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
typedef enum
{
  kDltEventInitDlt                 = 0u, /*!< Sub state: init. */
  kDltEventMessageReceived         = 1u, /*!< Sub state: at least one message is received. */
  kDltEventOneBufferHasContent     = 2u, /*!< Sub state: at least one buffer has content. */
  kDltEventTransmitCallSuccessful  = 3u, /*!< Sub state: transmission request was accepted. */
  kDltEventTransmitCallRejected    = 4u, /*!< Sub state: transmission request was rejected. */
  kDltEventSendingSucceeded        = 5u, /*!< Sub state: transmission was successfully finished. */
  kDltEventTimeout                 = 6u  /*!< Sub state: transmission was aborded due to a timeout of SoAd. */
} Dlt_EventType;                         /*!< Event type of sub state machine. */

typedef struct
{
  uint8  Buffer[DLT_MESSAGE_BUFFER_SIZE]; /*!< Member of message buffer: the buffer. */
  uint32 Write;                           /*!< Member of message buffer: the write pointer. */
  uint32 Read;                            /*!< Member of message buffer: the read pointer. */
} Dlt_BufferType;                         /*!< Message buffer type. */

typedef struct
{
  uint8  Buffer[DLT_CONTROL_BUFFER_SIZE]; /*!< Member of control buffer: the buffer. */
  uint32 Write;                           /*!< Member of control buffer: the write pointer. */
  uint32 Read;                            /*!< Member of control buffer: the read pointer. */
} Dlt_ControlBufferType;                  /*!< Control buffer type. */

typedef struct
{
  Dlt_MessageTypeType      message_type;  /*!< Member of filter info: the message type. */
  Dlt_SessionIDType        session_id;    /*!< Member of filter info: the session ID. */
  Dlt_MessageLogInfoType   log_info;      /*!< Member of filter info: if it is a log message, contains required data. */
  Dlt_MessageTraceInfoType trace_info;    /*!< Member of filter info: if it is a trace message, contains required data. */
} Dlt_FilterInfoType;                     /*!< Filter info type. */

typedef uint8 Dlt_OptionType; /*!< Option type: distinction between log, trace, verbose and boolean. */

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/* ====================================================================================================================
 * COMLAYER AUTOSAR
 * ====================================================================================================================
 */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

#define DLT_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA S 3218 40 */ /* MD_Dlt_3218 */
/* DEM application ID */
DLT_LOCAL CONST(uint32, DLT_CONST) Dlt_Dem_ApplId = 0x44454D00; /* DEM  */

/* DEM application ID description */
DLT_LOCAL CONST(uint8, DLT_CONST)  Dlt_Dem_ApplId_Desc[DLT_DEM_APPLID_DESC_LENGTH] = 
{
  /* D,    E,    M,     ,    E,    v,    e,    n,    t, NULL */
  0x44, 0x45, 0x4D, 0x20, 0x45, 0x76, 0x65, 0x6E, 0x74, 0x00 /* DEM Event */
};

/* DEM context ID */
DLT_LOCAL CONST(uint32, DLT_CONST) Dlt_Dem_ContextId = 0x53544430; /* STD0  */

/* DEM context ID description */
DLT_LOCAL CONST(uint8, DLT_CONST)  Dlt_Dem_ContextId_Desc[DLT_DEM_CONTEXTID_DESC_LENGTH] = 
{
  /* S,    T,    D,    0, NULL */
  0x53, 0x54, 0x44, 0x30, 0x00 /* STD0 */
};

/* DET application ID */
DLT_LOCAL CONST(uint32, DLT_CONST) Dlt_Det_ApplId = 0x44455400; /* DET  */

/* DET application ID description */
DLT_LOCAL CONST(uint8, DLT_CONST)  Dlt_Det_ApplId_Desc[DLT_DET_APPLID_DESC_LENGTH] = 
{
  /* D,    E,    T,     ,    E,    v,    e,    n,    t, NULL */
  0x44, 0x45, 0x54, 0x20, 0x45, 0x76, 0x65, 0x6E, 0x74, 0x00 /* DET Event */
};

/* DET context ID */
DLT_LOCAL CONST(uint32, DLT_CONST) Dlt_Det_ContextId = 0x53544400; /* STD  */

/* DET context ID description */
DLT_LOCAL CONST(uint8, DLT_CONST)  Dlt_Det_ContextId_Desc[DLT_DET_CONTEXTID_DESC_LENGTH] = 
{
  /* S,    T,    D, NULL */
  0x53, 0x54, 0x44, 0x00 /* STD */
};

#define DLT_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DLT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

DLT_LOCAL VAR(Dlt_BufferType, DLT_VAR_ZERO_INIT)      Dlt_SendBuffer = {0};
DLT_LOCAL VAR(Dlt_StateType, DLT_VAR_ZERO_INIT)       Dlt_CurrentState = (Dlt_StateType)DLT_STATE_UNINIT;
DLT_LOCAL VAR(uint8, DLT_VAR_ZERO_INIT)               Dlt_MessageCounter = 0;
DLT_LOCAL VAR(uint8, DLT_VAR_ZERO_INIT)               Dlt_MessageBufferOverflowHappened = (uint8)FALSE;

DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_ZERO_INIT) Dlt_GlobalState = DLT_GLOBAL_STATE_UNINIT;
DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_ZERO_INIT) Dlt_GlobalStateNew = DLT_GLOBAL_STATE_UNINIT;

#define DLT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DLT_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA S 0686 49 */ /* MD_Dlt_0686 */
DLT_LOCAL VAR(Dlt_LogInfoType, DLT_VAR_INIT)         Dlt_Context = { 2 /* Count Application IDs */,
                                                                     {
                                                                       { /* DEM */
                                                                         0x44454D00 /* DEM Application ID */,
                                                                         1 /* Count Context IDs */,
                                                                         {
                                                                           {
                                                                             0x53544430 /* DEM Context ID - STD0 */,
                                                                             (sint8)DLT_DEFAULT_MAX_LOG_LEVEL /* log level */,
                                                                             (sint8)DLT_DEFAULT_TRACE_STATUS /* trace status */,
                                                                             (uint8)DLT_DEM_CONTEXTID_DESC_LENGTH /* DEM Context ID description length */,
                                                                             Dlt_Dem_ContextId_Desc /* DEM Context ID description */,
                                                                             {
                                                                               (Dlt_SessionIDType)0x36, /* Dem module ID */
                                                                               (uint8)DLT_USE_VERBOSE_MODE,
                                                                               NULL_PTR,
                                                                               NULL_PTR,
                                                                               NULL_PTR
                                                                             }
                                                                           }
                                                                         },
                                                                         4,
                                                                         Dlt_Dem_ApplId_Desc /* DEM Application ID description */
                                                                       },
                                                                       { /* DET */
                                                                         0x44455400 /* DET Application ID */,
                                                                         1 /* Count Context IDs */,
                                                                         {
                                                                           {
                                                                             0x53544400 /* DET Context ID - STD */,
                                                                             (sint8)DLT_DEFAULT_MAX_LOG_LEVEL /* log level */,
                                                                             (sint8)DLT_DEFAULT_TRACE_STATUS /* trace status */,
                                                                             (uint8)DLT_DET_CONTEXTID_DESC_LENGTH /* DEM Context ID description length */,
                                                                             Dlt_Det_ContextId_Desc /* DEM Context ID description */,
                                                                             {
                                                                               (Dlt_SessionIDType)0x0F /* Det module ID */,
                                                                               (uint8)DLT_USE_VERBOSE_MODE,
                                                                               NULL_PTR,
                                                                               NULL_PTR,
                                                                               NULL_PTR
                                                                             }
                                                                           }
                                                                         },
                                                                         4,
                                                                         Dlt_Dem_ApplId_Desc /* DET Application ID description */
                                                                       }
                                                                     }
                                                                   };

/* Variables for Dlt runtime configuration */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
VAR(uint8, DLT_VAR_INIT)                             Dlt_FilterMessages = (uint8)DLT_FILTER_MESSAGES;
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
VAR(uint8, DLT_VAR_INIT)                             Dlt_HeaderUseTimestamp = (uint8)DLT_HEADER_USE_TIMESTAMP;
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */
# if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
VAR(uint8, DLT_VAR_INIT)                             Dlt_HeaderUseExtendedHeader = (uint8)DLT_HEADER_USE_EXTENDED_HEADER;
# endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
VAR(uint8, DLT_VAR_INIT)                             Dlt_HeaderUseVerboseMode = (uint8)DLT_USE_VERBOSE_MODE;
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */

VAR(uint8, DLT_VAR_INIT)                             Dlt_HeaderUseEcuId = (uint8)DLT_HEADER_USE_ECU_ID;
VAR(uint8, DLT_VAR_INIT)                             Dlt_HeaderUseSessionId = (uint8)DLT_HEADER_USE_SESSION_ID;
VAR(uint8, DLT_VAR_INIT)                             Dlt_DefaultTraceStatus = (uint8)DLT_DEFAULT_TRACE_STATUS; /* Tells if trace messages shall be forwarded by Dlt */
VAR(Dlt_MessageLogLevelType, DLT_VAR_INIT)           Dlt_DefaultMaxLogLevel = (Dlt_MessageLogLevelType)DLT_DEFAULT_MAX_LOG_LEVEL; /* Tells if log messages shall be forwarded by Dlt */

DLT_LOCAL VAR(uint32, DLT_VAR_INIT)                  Dlt_NumberOfRegisteredContextIds = 2;

#define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DLT_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

DLT_LOCAL VAR(Dlt_ControlBufferType, DLT_VAR_NOINIT) Dlt_ControlSendBuffer;
DLT_LOCAL VAR(uint8, DLT_VAR_NOINIT)                 Dlt_ControlSendMessage[DLT_MAX_MESSAGE_LENGTH];
DLT_LOCAL VAR(Dlt_ControlBufferType, DLT_VAR_NOINIT) Dlt_ReceiveBuffer;
DLT_LOCAL VAR(uint8, DLT_VAR_NOINIT)                 Dlt_ReceiveMessage[DLT_MAX_MESSAGE_LENGTH];

# if (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE)
DLT_LOCAL VAR(uint8, DLT_VAR_NOINIT)                 Dlt_SendData[DLT_MESSAGE_BUFFER_SIZE];
# else /* (DLT_MESSAGE_BUFFER_SIZE < DLT_CONTROL_BUFFER_SIZE) */
DLT_LOCAL VAR(uint8, DLT_VAR_NOINIT)                 Dlt_SendData[DLT_CONTROL_BUFFER_SIZE];
# endif /* (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE) */

DLT_LOCAL VAR(uint8,  DLT_VAR_NOINIT)                Dlt_TransmitMessage[DLT_MAX_MESSAGE_LENGTH];
DLT_LOCAL VAR(uint8,  DLT_VAR_NOINIT)                Dlt_MessageTransmitted;
DLT_LOCAL VAR(uint8,  DLT_VAR_NOINIT)                Dlt_SendingOrderAccepted;
DLT_LOCAL VAR(uint8,  DLT_VAR_NOINIT)                Dlt_RxIndicationOccured;
DLT_LOCAL VAR(uint16, DLT_VAR_NOINIT)                Dlt_Timeout;
                                              
DLT_LOCAL VAR(uint8,  DLT_VAR_NOINIT)                Dlt_CopyRxDataCounter;
DLT_LOCAL VAR(uint16, DLT_VAR_NOINIT)                Dlt_CopyRxDataLength;

VAR(PduInfoType, DLT_VAR_NOINIT)                     Dlt_SendPdu;

VAR(Dlt_SoConModeType, DLT_VAR_NOINIT)               Dlt_SoConMode;
DLT_LOCAL VAR(Dlt_SoConModeType, DLT_VAR_NOINIT)     Dlt_SoConLastMode;

#define DLT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#else /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/* ====================================================================================================================
 * COMLAYER XCP
 * ====================================================================================================================
 */

#define DLT_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_ZERO_INIT) Dlt_GlobalState = DLT_GLOBAL_STATE_UNINIT;
DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_ZERO_INIT) Dlt_GlobalStateNew = DLT_GLOBAL_STATE_UNINIT;

#define DLT_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DLT_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Variable is loaded with the last DET error code */
VAR(uint32, DLT_VAR_NOINIT)                          Dlt_DetErrorCode;

#define DLT_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DLT_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Variable is loaded with DEM event id and status code. */
VAR(Dlt_DemEventStatusType, DLT_VAR_NOINIT)          Dlt_DemEventStatus; /*lint -e830 */ /* Suppress ID830 */ /*lint -e552 */ /* Suppress ID552 */

/* Variable is loaded with message ID of non-verbose message . */
VAR(Dlt_MessageIDType, DLT_VAR_NOINIT)               Dlt_NonVerboseMessageId;

# if (DLT_USE_VERBOSE_MODE == STD_ON)
/* Buffer is loaded with payload of verbose message. */
VAR(uint8, DLT_VAR_NOINIT)                           Dlt_VerboseMessageData[DLT_MAX_MESSAGE_LENGTH];
# endif /* (DLT_USE_VERBOSE_MODE == STD_ON) */

#define DLT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
#define DLT_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
CONST(uint8, DLT_CONST) Dlt_StandardHeaderType_SoftwareVersion = 0x01;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

CONST(uint8, DLT_CONST) Dlt_ModuleTable[256] = 
{
/*  0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F           */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x0Au, 0x0Bu, 0x0Cu, 0x0Du, 0x00u, 0x00u,  /* 0x */ /* 0x0A = ECUM, 0x0B = FIM, 0x0C = COMM, 0x0D = WDGM */
  0x00u, 0x00u, 0x00u, 0x00u, 0x14u, 0x15u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Au, 0x00u, 0x00u, 0x1Du, 0x00u, 0x1Fu,  /* 1x */ /* 0x14 = NVM, 0x15 = FEE, 0x1A = XCP, 0x1D = NM, 0x1F = CANNM */
  0x20u, 0x00u, 0x00u, 0x23u, 0x24u, 0x25u, 0x00u, 0x00u, 0x28u, 0x00u, 0x2Au, 0x2Bu, 0x00u, 0x00u, 0x00u, 0x00u,  /* 2x */ /* 0x20 = FRNM, 0x23 = CANTP, 0x24 = FRTP, 0x25 = J1939TP, 0x28 = EA, 0x2A = BSWM, 0x2B = WDGIF */
  0x00u, 0x00u, 0x32u, 0x33u, 0x34u, 0x35u, 0x36u, 0x00u, 0x38u, 0x00u, 0x00u, 0x00u, 0x3Cu, 0x3Du, 0x3Eu, 0x00u,  /* 3x */ /* 0x32 = COM, 0x33 = PDUR, 0x34 = IPDUM, 0x35 = DCM, 0x36 = DEM, 0x38 depends on InstanceId (SOAD = 0x00, TCPIP = 0x01, IPV4 = 0x02, IPV6 = 0x04), 0x3C = CANIF, 0x3D = FRIF, 0x3E = LINIF */
  0x00u, 0x41u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x49u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* 4x */ /* 0x41 = ETHIF, 0x49 = ETHTRCV */
  0x50u, 0x51u, 0x52u, 0x53u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x5Cu, 0x00u, 0x00u, 0x00u,  /* 5x */ /* 0x50 = CAN, 0x51 = FR, 0x52 = LIN, 0x53 = SPI, 0x5C = FLS */
  0x00u, 0x00u, 0x00u, 0x00u, 0x64u, 0x65u, 0x66u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* 6x */ /* 0x64 = GPT, 0x65 = MCU, 0x66 = WDG */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x78u, 0x79u, 0x7Au, 0x7Bu, 0x00u, 0x00u, 0x00u, 0x00u,  /* 7x */ /* 0x78 = DIO, 0x79 = PWM, 0x7A = ICU, 0x7B = ADC */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x8Cu, 0x8Du, 0x8Eu, 0x8Fu,  /* 8x */ /* 0x8C = CANSM, 0x8D = LINSM, 0x8E = FRSM , 0x8F = ETHSM */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* 9x */ 
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xAEu, 0x00u,  /* Ax */ /* 0x(1)AE = ETH */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Bx */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Cx */
  0x00u, 0x00u, 0x00u, 0xD3u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Dx */ /* 0xD3 = FRXCP */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Ex */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu   /* Fx */ /* 0xFF : depends on InstanceId - 0x00: CANXCP, 0x65: RCS, 0x66: HTTP, 0x67: TLS, 0x68: Crypto, 0x69: DNS, 0x6A: JSON, 0x6B..0x6D: EXI, 0x6E: IPBase, 0x6F: SCC */
};

#define DLT_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define DLT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ####################################################################################################################
 *   HH      HH  EEEEEEEEEE  LL          PPPPPPPP    EEEEEEEEEE  RRRRRRRR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HHHHHHHHHH  EEEEEEEE    LL          PPPPPPPP    EEEEEEEE    RRRRRRRR
 *   HH      HH  EE          LL          PP          EE          RR    RR
 *   HH      HH  EE          LL          PP          EE          RR      RR
 *   HH      HH  EEEEEEEEEE  LLLLLLLLLL  PP          EEEEEEEEEE  RR      RR
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_Hlp_CopyArrayToLongLittleEndian()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from array to variable.
 * \details     Copies 4 byte from source array to variable with least significant byte first.
 * \param[out]  StartIndex            Start index of source array where the data is copied from.
 * \param[out]  DestData              Data destination.
 * \param[in]   SourceData            Data source.
 * \return      -
 * \pre         The source array must have at least the size of 4.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongLittleEndian(P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex,
                                                                          P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
                                                                          P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData);

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/**********************************************************************************************************************
  Dlt_Hlp_CopyArrayToLongBigEndian()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from array to variable.
 * \details     Copies 4 byte from source array to variable with most significant byte first.
 * \param[out]  StartIndex            Start index of source array where the data is copied from.
 * \param[out]  DestData              Data destination.
 * \param[in]   SourceData            Data source.
 * \return      -
 * \pre         The source array must have at least the size of 4.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongBigEndian(P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex,
                                                                       P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
                                                                       P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData);

# if (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN)
#  define Dlt_Hlp_CopyLongToArray Dlt_Hlp_CopyLongToArrayLittleEndian
#  define Dlt_Hlp_CopyIntToArray  Dlt_Hlp_CopyIntToArrayLittleEndian
# else /* (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN) */
#  define Dlt_Hlp_CopyLongToArray Dlt_Hlp_CopyLongToArrayBigEndian
#  define Dlt_Hlp_CopyIntToArray  Dlt_Hlp_CopyIntToArrayBigEndian
# endif /* (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN) */

/**********************************************************************************************************************
  Dlt_Hlp_CopyLongToArrayLittleEndian()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from variable to array.
 * \details     Copies 4 byte variable to destination array with least significant byte first.
 * \param[in]   SourceData            Data source.
 * \param[out]  DestData              Data destination.
 * \param[out]  index                 Start index of destination array where the data is copied to.
 * \return      -
 * \pre         The destination array must have at least the size of 4.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayLittleEndian(uint32 SourceData,
                                                                          P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                          P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

# if (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN)
/**********************************************************************************************************************
  Dlt_Hlp_CopyIntToArrayLittleEndian()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from variable to array.
 * \details     Copies 2 byte variable to destination array with least significant byte first.
 * \param[in]   SourceData            Data source.
 * \param[out]  DestData              Data destination.
 * \param[out]  index                 Start index of destination array where the data is copied to.
 * \return      -
 * \pre         The destination array must have at least the size of 2.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayLittleEndian(uint16 SourceData,
                                                                         P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                         P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

# endif /* (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN) */
/**********************************************************************************************************************
  Dlt_Hlp_CopyLongToArrayBigEndian()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from variable to array.
 * \details     Copies 4 byte variable to destination array with most significant byte first.
 * \param[in]   SourceData            Data source.
 * \param[out]  DestData              Data destination.
 * \param[out]  index                 Start index of destination array where the data is copied to.
 * \return      -
 * \pre         The destination array must have at least the size of 4.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayBigEndian(uint32 SourceData,
                                                                       P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                       P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

/**********************************************************************************************************************
  Dlt_Hlp_CopyIntToArrayBigEndian()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from variable to array.
 * \details     Copies 2 byte variable to destination array with most significant byte first.
 * \param[in]   SourceData            Data source.
 * \param[out]  DestData              Data destination.
 * \param[out]  index                 Start index of destination array where the data is copied to.
 * \return      -
 * \pre         The destination array must have at least the size of 2.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayBigEndian(uint16 SourceData,
                                                                      P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                      P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

/**********************************************************************************************************************
  Dlt_Hlp_CopyArrayToArray()
**********************************************************************************************************************/
/*!
 *
 * \brief       Copies data from one array to another.
 * \details     Copies data byte-wise from source to destination.
 * \param[out]  DestData              Data destination.
 * \param[in]   SourceData            Data source.
 * \param[in]   NumberOfBytes         The number of bytes to be copied.
 * \return      -
 * \pre         The source and destination arrays must have at least the size of NumberOfBytes.
 * \pre         Only reentrant if destination and source pointer content are not affected by second call while 
 *              interrupted.
 * \context     TASK|ISR
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToArray(P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
                                                               P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
                                                               uint8 NumberOfBytes);

/**********************************************************************************************************************
  Dlt_Hlp_GetApplicationIDIndex()
**********************************************************************************************************************/
/*!
 *
 * \brief       Returns the index of requested application ID.
 * \details     Searches in the context table for the application ID. Return the index of the application ID.
 * \param[in]   ApplicationID         The application ID.
 * \param[out]  ApplicationIDIndex    Index of application ID.
 * \return      DLT_OK                The search for the index of application ID was successful.
 * \return      DLT_NOT_OK            The application ID is not registered.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetApplicationIDIndex(uint32 ApplicationID,
                                                                     P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ApplicationIDIndex);

/**********************************************************************************************************************
  Dlt_Hlp_GetContextIDIndex()
**********************************************************************************************************************/
/*!
 *
 * \brief       Returns the index of requested context ID.
 * \details     Searches in the context table for the couple of application ID and context ID. Return the index of the
 *              context ID.
 * \param[in]   ApplIdIndx            Index of the application ID.
 * \param[in]   ContextID             The context ID.
 * \param[out]  ContextIDIndex        Index of context ID.
 * \return      DLT_OK                The search for the index of context ID was successful.
 * \return      DLT_NOT_OK            The context ID is not registered.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetContextIDIndex(uint16 ApplIdIndx,
                                                                 uint32 ContextID,
                                                                 P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex);

/**********************************************************************************************************************
  Dlt_Hlp_SetOptionInOneContext()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets new state of an option to one context.
 * \details     Sets new state of an option to one couple of application ID and context ID. Passes the new
 *              state to SWC if supported by SWC.
 * \param[in]   Option                The option that has to be changed. It can take following values:
 *                                     - DLT_OPT_LOGLEVEL
 *                                     - DLT_OPT_TRACESTATUS
 *                                     - DLT_OPT_VERBOSEMODE
 * \param[in]   NewState              The state the option has to be changed to.
 * \param[in]   ApplicationIdx        Index of application ID.
 * \param[in]   ContextIdx            Index of context ID.
 * \return      DLT_OK                The state change of option was successful.
 * \return      DLT_NOT_OK            The option to be changed is invalid.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOptionInOneContext(Dlt_OptionType Option,
                                                                     uint8 NewState,
                                                                     uint16 ApplicationIdx,
                                                                     uint16 ContextIdx);

/**********************************************************************************************************************
  Dlt_Hlp_SetOption()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets new state of an option.
 * \details     Sets new state of an option of
 *               - either one couple of application ID and context ID
 *               - all context IDs of one application ID or
 *               - all application ID and context ID.
 * \param[in]   ApplicationID         The application ID which option has to be changed.
 * \param[in]   ContextID             The context ID which option has to be changed.
 * \param[in]   Option                The option that has to be changed. It can take following values:
 *                                     - DLT_OPT_LOGLEVEL
 *                                     - DLT_OPT_TRACESTATUS
 *                                     - DLT_OPT_VERBOSEMODE
 * \param[in]   NewStatus             The state the option has to be changed to.
 * \return      DLT_OK                The state change of option was successful.
 * \return      DLT_NOT_OK            The state change of option has failed at least for one context.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOption(uint32 ApplicationID,
                                                         uint32 ContextID,
														 Dlt_OptionType Option,
														 uint8 NewStatus);

/**********************************************************************************************************************
  Dlt_Hlp_GetIndexDependingOnStandardHeader()
**********************************************************************************************************************/
/*!
 *
 * \brief       Modifies position of received message pointer depending on configuration in standard header.
 * \details     For each set flag ("WEID", "WTMS" and "WSID") of standard header the pointer position is moved.
 * \param[out]  Index                 Pointer to received message. Modified depending configuration in standard header.
 * \return      -
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_GetIndexDependingOnStandardHeader(P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) Index);

/**********************************************************************************************************************
  Dlt_Hlp_IsContextRegistered()
**********************************************************************************************************************/
/*!
 *
 * \brief       Checks whether a context is already registered.
 * \details     Checks whether the specified triple of application ID, context ID and session ID is already registered
 *              in the context table.
 * \param[in]   ApplId                The application ID to be searched for in context table.
 * \param[in]   ContId                The context ID to be searched for in context table.
 * \param[out]  AppIDIndex            Index of application ID.
 * \param[out]  ContextIDIndex        Index of context ID.
 * \param[out]  ApplAlreadyReg        Indicates if application ID is already registered.
 * \return      TRUE                  Indicates that context is already registered.
 * \return      FALSE                 Indicates that context is not registered yet.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_IsContextRegistered(Dlt_ApplicationIDType ApplId,
                                                                     Dlt_ContextIDType ContId,
                                                                     P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
                                                                     P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
                                                                     P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg
                                                                    );

/**********************************************************************************************************************
  Dlt_Hlp_CheckSessionId()
**********************************************************************************************************************/
/*!
 *
 * \brief       Checks whether the session ID fits to registered.
 * \details     Checks whether the specified triple of application ID, context ID and session ID is already registered
 *              in the context table.
 * \param[in]   SessId                The session ID to be searched for in context table.
 * \param[in]   ApplId                The application ID to be searched for in context table.
 * \param[in]   ContId                The context ID to be searched for in context table.
 * \param[out]  AppIDIndex            Index of application ID.
 * \param[out]  ContextIDIndex        Index of context ID.
 * \param[out]  ApplAlreadyReg        Indicates if application ID is already registered.
 * \return      TRUE                  Indicates that context is already registered.
 * \return      FALSE                 Indicates that context is not registered yet.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_CheckSessionId(Dlt_SessionIDType SessId,
                                                                Dlt_ApplicationIDType ApplId,
                                                                Dlt_ContextIDType ContId,
                                                                P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
                                                                P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
                                                                P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg
                                                               );

/**********************************************************************************************************************
  Dlt_Hlp_GetLengthOfReceivedMsg()
**********************************************************************************************************************/
/*!
 *
 * \brief       Returns the length of the received message.
 * \details     Reads byte 2 and 3 if first 4 bytes of new message are received. With these bytes the length of the
 *              received message is specified. Returns this length.
 * \param[in]   BufferIndex           Index of receive buffer.
 * \return      The length of the received message.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetLengthOfReceivedMsg(uint32 BufferIndex);

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        MM      MM    SSSSSS      GGGGGG
 *       TT        XX  XX          MMMM  MMMM  SS      SS  GG      GG
 *       TT        XX  XX          MM  MM  MM  SS          GG
 *       TT          XX      ====  MM  MM  MM    SSSSSS    GG    GGGG
 *       TT        XX  XX          MM      MM          SS  GG      GG
 *       TT        XX  XX          MM      MM  SS      SS  GG      GG
 *       TT      XX      XX        MM      MM    SSSSSS      GGGGGG G
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_TxMsg_Transmit()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds a DLT frame and puts it to send buffer.
 * \details     Verifies validity of passed data. Builds complete DLT frame for log and trace messages. Writes frame
 *              to send buffer.
 * \param[in]   FilterInfo            Filter options.
 * \param[in]   PayloadLength         Length of payload in bytes.
 * \param[in]   Payload               User data defined by application.
 * \return      DLT_E_OK                 The message was successfully written to send buffer.
 * \return      DLT_E_MSG_TOO_LARGE      The message (standard header + extended header + payload) is too long.
 * \return      DLT_E_UNKNOWN_SESSION_ID The context is not registered (application ID + context ID + session ID).
 * \return      DLT_E_IF_NOT_AVAILABLE   The send buffer is overflown.
 * \return      DLT_E_ERROR_UNKNOWN      Passed data is invalid.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_Transmit(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo,
                                                            uint16 PayloadLength,
                                                            P2CONST(uint8, AUTOMATIC, DLT_CONST) Payload);

/**********************************************************************************************************************
  Dlt_TxMsg_IsMessageValid()
**********************************************************************************************************************/
/*!
 *
 * \brief       Checks validity of data for log and trace messages.
 * \details     Checks whether the context is registered, message is illegally in verbose mode, message log level is
 *              in valid range and trace status is enabled.
 * \param[in]   FilterInfo            Filter options.
 * \param[out]  AppIdIndex            Index of application ID.
 * \param[out]  ContextIdIndex        Index of context ID.
 * \param[in]   MessageOptions        Specifies message type (log, trace, verbose).
 * \return      DLT_E_OK                 Data is valid.
 * \return      DLT_E_UNKNOWN_SESSION_ID The context is not registered yet.
 * \return      DLT_E_ERROR_UNKNOWN      Passed data is invalid.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_IsMessageValid(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo,
                                                                         P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIdIndex, 
                                                                         P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIdIndex,
                                                                         Dlt_MessageOptionsType MessageOptions);

/**********************************************************************************************************************
  Dlt_TxMsg_SetStandardHeader()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets standard header in send message.
 * \details     Sets the active flags in header type and the according bytes in standard header of the send message.
 * \param[in]   FilterInfo            Filter options.
 * \param[out]  Data                  The send message.
 * \param[out]  DataIndex             Index of send message where standard header ends.
 * \return      -
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxMsg_SetStandardHeader(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo, 
                                                                  P2VAR(uint8, AUTOMATIC, DLT_CONST) Data, 
                                                                  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex);

#if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
/**********************************************************************************************************************
  Dlt_TxMsg_SetExtendedHeader()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets extended header in send message.
 * \details     Sets Message Info depending on specified message type (in FilterInfo). Adds number of arguments, the
 *              application ID and context ID to send message.
 * \param[in]   FilterInfo            Filter options.
 * \param[out]  Data                  The send message.
 * \param[out]  DataIndex             Index of send message where standard header ends.
 * \param[in]   AppIdIndex            Index of the application ID.
 * \param[in]   ConIdIndex            Index of the context ID.
 * \return      DLT_E_OK              The setting of extended header was successful.
 * \return      DLT_E_ERROR_UNKNOWN   The message type was invalid.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_SetExtendedHeader(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo,
                                                                            P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
                                                                            P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex,
                                                                            uint16 AppIdIndex,
                                                                            uint16 ConIdIndex);
#endif /* defined (DLT_IMPLEMENT_EXTENDED_HEADER) */

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        BBBBBBBB    UU      UU  FFFFFFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT          XX      ====  BBBBBBBB    UU      UU    FFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT      XX      XX        BBBBBBBB      UUUUUU        FF
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_TxBuf_WriteToBuf()
**********************************************************************************************************************/
/*!
 *
 * \brief       Writes log/trace message to send buffer.
 * \details     Writes log/trace message to send buffer and detects send buffer overflow.
 * \param[in]   DltEvent              Filter options.
 * \param[out]  Data                  The send message.
 * \param[out]  DataLength            Index of send message where standard header ends.
 * \return      DLT_E_OK               The setting of extended header was successful.
 * \return      DLT_E_IF_NOT_AVAILABLE The send buffer is overflown.
 * \return      DLT_E_ERROR_UNKNOWN    The message type was invalid.
 * \pre         -
 * \context     TASK|ISR
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxBuf_WriteToBuf(uint8 DltEvent, 
                                                              P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) Data, 
                                                              uint16 DataLength);

/**********************************************************************************************************************
  Dlt_TxBuf_WriteToCtrlBuf()
**********************************************************************************************************************/
/*!
 *
 * \brief       Writes control message to control buffer.
 * \details     Writes control message to control buffer, stops if buffer overflow occurs.
 * \param[in]   Length                Number of bytes to be written.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteToCtrlBuf(uint16 Length);

/**********************************************************************************************************************
  Dlt_TxBuf_PrepareRespInCtrlBuf()
**********************************************************************************************************************/
/*!
 *
 * \brief       Prepares the response control frame.
 * \details     Writes static data, configuration depending data and received message depending data to response
 *              message.
 * \param[in]   Length                Length of complete control message.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_PrepareRespInCtrlBuf(uint16 Length);

/**********************************************************************************************************************
  Dlt_TxBuf_CalculateMessageLength()
**********************************************************************************************************************/
/*!
 *
 * \brief       Calculates the number of bytes to be sent.
 * \details     Calculates the number of bytes to be sent and limit it to maximum allowed message size. Used for
 *              log/trace and control messages.
 * \param[in]   ReadIndex             Length of complete control message.
 * \param[in]   WriteIndex            Length of complete control message.
 * \param[in]   bufferSize            Length of complete control message.
 * \return      Number of byte to be transmitted.
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint32, DLT_CODE) Dlt_TxBuf_CalculateMessageLength(uint32 ReadIndex,
									 uint32 WriteIndex,
									 uint32 bufferSize);

/**********************************************************************************************************************
  Dlt_TxBuf_SetReadIndex()
**********************************************************************************************************************/
/*!
 *
 * \brief       Updates the read pointer of control buffer or send buffer.
 * \details     Updates the read pointer of control buffer or send buffer, depending on the specified buffer.
 * \param[in]   setControlBuffer      If true, the control buffer is modified, otherwise the send buffer is modified.
 * \param[in]   bufferIdx             New position for read pointer.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_SetReadIndex(boolean setControlBuffer, uint32 bufferIdx);

/**********************************************************************************************************************
  Dlt_TxBuf_GetEcuIdAndService()
**********************************************************************************************************************/
/*!
 *
 * \brief       Reads out the ECU ID and service ID from received message.
 * \details     Reads out the ECU ID and service ID from received message and returns them.
 * \param[out]  EcuId                 The ECU ID specified in received message.
 * \param[out]  ServiceId             The session ID specified in received message.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_GetEcuIdAndService(P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) EcuId, 
                                                                   P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) ServiceId);

/**********************************************************************************************************************
  Dlt_TxBuf_WriteBufToTxMsg()
**********************************************************************************************************************/
/*!
 *
 * \brief       Reads out the ECU ID and service ID from received message.
 * \details     Reads out the ECU ID and service ID from received message and returns them.
 * \param[out]  EcuId                 The ECU ID specified in received message.
 * \param[out]  ServiceId             The session ID specified in received message.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteBufToTxMsg(P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceBuffer, 
                                                                uint32 ReadIndex, 
                                                                uint32 WriteIndex, 
                                                                boolean IsControlBuffer);

/* ####################################################################################################################
 *     SSSSSS    MM      MM
 *   SS      SS  MMMM  MMMM
 *   SS          MM  MM  MM
 *     SSSSSS    MM  MM  MM
 *           SS  MM      MM
 *   SS      SS  MM      MM
 *     SSSSSS    MM      MM
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_Sm_HandleSendingTransitions()
**********************************************************************************************************************/
/*!
 *
 * \brief       Triggers DLT state changes after sending.
 * \details     Changes DLT state from DLT_STATE_SENDING to DLT_STATE_READY_TO_SEND if sending succeeded.
 *              Changes DLT state from DLT_STATE_SENDING to DLT_STATE_WAIT_FOR_RECEP if timeout occured (sending
 *              failed multiple times).
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_HandleSendingTransitions(void);

/**********************************************************************************************************************
  Dlt_Sm_StateTransition()
**********************************************************************************************************************/
/*!
 *
 * \brief       Manages state transitions.
 * \details     Changes DLT states depending on previous state and triggering event. Calls transition function.
 * \param[in]   transitionEvent       The transition triggering event.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_StateTransition(const Dlt_EventType transitionEvent);

/**********************************************************************************************************************
  Dlt_Sm_DummyFunction()
**********************************************************************************************************************/
/*!
 *
 * \brief       Dummy function.
 * \details     Function to fill function pointer matrix.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_DummyFunction(void);

/**********************************************************************************************************************
  Dlt_Sm_SendingDecision()
**********************************************************************************************************************/
/*!
 *
 * \brief       Decides which buffer content is sent.
 * \details     If available it interprets received message and creates response message. If response message exists,
 *              it is sent. Otherwise, if log or trace message is available, send this message.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendingDecision(void);

/**********************************************************************************************************************
  Dlt_Sm_BuildResponseMessage()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response messages.
 * \details     Builds for each received message in receive buffer a response message and puts it in control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_BuildResponseMessage(void);

/**********************************************************************************************************************
  Dlt_Sm_ClearBuffer()
**********************************************************************************************************************/
/*!
 *
 * \brief       Clears all buffers.
 * \details     Clears all buffer contents and their read and write pointers.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_ClearBuffer(void);

/**********************************************************************************************************************
  Dlt_Sm_SendMsgBufferOverflow()
**********************************************************************************************************************/
/*!
 *
 * \brief       Creates a response control message of service MessageBufferOverflow.
 * \details     Creates a response control message of service MessageBufferOverflow and writes it to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendMsgBufferOverflow(void);

/**********************************************************************************************************************
  Dlt_Sm_GlobalStateTransition()
**********************************************************************************************************************/
/*!
 *
 * \brief       Changes global Dlt state if requested.
 * \details     Sets new global Dlt state if requested via Dlt_SetState. If transition from DLT_GLOBAL_STATE_ONLINE to
 *              DLT_GLOBAL_STATE_OFFLINE, clear content of all buffers.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_GlobalStateTransition(void);

/* ####################################################################################################################
 *     SSSSSS    EEEEEEEEEE  RRRRRRRR    VV      VV    IIIIII      CCCCCC    EEEEEEEEEE
 *   SS      SS  EE          RR      RR  VV      VV      II      CC      CC  EE
 *   SS          EE          RR      RR  VV      VV      II      CC          EE
 *     SSSSSS    EEEEEEEE    RRRRRRRR     VV    VV       II      CC          EEEEEEEE
 *           SS  EE          RR    RR      VV  VV        II      CC          EE
 *   SS      SS  EE          RR      RR     VVVV         II      CC      CC  EE
 *     SSSSSS    EEEEEEEEEE  RR      RR      VV        IIIIII      CCCCCC    EEEEEEEEEE
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_Srv_ParamInValidRange()
**********************************************************************************************************************/
/*!
 *
 * \brief       .
 * \details     .
 * \param[in]   IsGetter              Specifies whether it is a service to request data or setting data.
 * \param[in]   NumOfArgs             Number of arguments in payload.
 * \param[in]   RespLen               Length of complete response message.
 * \param[out]  Var                   The requested data or the variable to be changed (depends on isGetter).
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Srv_ParamInValidRange(Dlt_OptionType Opt, sint8 Value);

/**********************************************************************************************************************
  Dlt_Srv_FinalizeRespToCtrlBuf()
**********************************************************************************************************************/
/*!
 *
 * \brief       Completes response message and writes it to control buffer.
 * \details     Uses the software version specified in received message.
 * \param[in]   Option                Specifies the attribute to be requested for change or return value.
 * \param[in]   IsGetter              Specifies whether it is a service to request data or setting data.
 * \param[in]   NumOfArgs             Number of arguments in payload.
 * \param[in]   RespLen               Length of complete response message.
 * \param[out]  Var                   The requested data or the variable to be changed (depends on isGetter).
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_FinalizeRespToCtrlBuf(Dlt_OptionType Option,
                                                                    boolean IsGetter, 
                                                                    uint8 NumOfArgs, 
                                                                    uint16 RespLen, 
                                                                    P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Var);

/**********************************************************************************************************************
  Dlt_Srv_ServiceNotSupported()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response control message due to request of a not supported service.
 * \details     Builds response message to send DLT_NOT_SUPPORTED, because the requested service is not supported.
 *              Writes the message to control buffer.
 * \param[in]   Length                Length of response message.
 * \param[in]   NumberOfArgs          Number of arguments in payload.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_ServiceNotSupported(uint16 Length, uint8 NumberOfArgs);

/**********************************************************************************************************************
  Dlt_Srv_DummyService()
**********************************************************************************************************************/
/*!
 *
 * \brief       Dummy service.
 * \details     Called for requested services that are invalid. Builds response control message with DLT_NOT_SUPPORTED
 *              and writes message to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_DummyService(void); /* 0x0000 */

/**********************************************************************************************************************
  Dlt_Srv_SetStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets the log level or trace status of one registered context.
 * \details     Sets the log level or trace status of one registered context. If new state is -1, the default value is
 *              assigned to contexts log level or trace status. Response message indicates if value was set
 *              successfully.
 * \param[in]   Option                Is set to DLT_OPT_LOGLEVEL or DLT_OPT_TRACESTATUS.
 * \param[in]   DefaultValue          Default value of log level or trace status.
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetStatus(Dlt_OptionType Option, sint8 DefaultValue);

/**********************************************************************************************************************
  Dlt_Srv_SetTraceStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets the trace status of one registered context.
 * \details     If new state is -1, the default value is assigned to contexts trace status. Response message indicates
 *              if value was set successfully.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTraceStatus(void); /* 0x0002 */

/**********************************************************************************************************************
  Dlt_Srv_SetLogLevel()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets the log level of one registered context.
 * \details     If new state is -1, the default value is assigned to contexts log level. Response message indicates
 *              if value was set successfully.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogLevel(void); /* 0x0001 */

/**********************************************************************************************************************
  Dlt_Srv_GetLogInfo()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service GetLogInfo is not supported.
 * \details     Builds response message to indicate that service GetLogInfo is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogInfo(void); /* 0x0003 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_SetVerboseMode()
**********************************************************************************************************************/
/*!
 *
 * \brief       Sets the verbose mode of all specified and registered contexts.
 * \details     Sets the verbose mode of all specified and registered contexts. Response message indicates
 *              if value was set successfully.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetVerboseMode(void); /* 0x0009 */

/**********************************************************************************************************************
  Dlt_Srv_GetVerboseModeStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Returns the verbose mode of one registered contexts or the global flag.
 * \details     Builds response message with requested value. If the triple application ID, context ID and session ID
 *              are not registered, respond global flag Dlt_HeaderUseVerboseMode.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetVerboseModeStatus(void); /* 0x0019 */

/**********************************************************************************************************************
  Dlt_Srv_SetDefaultLogLevel()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines the global log level threshold for sending log messages.
 * \details     Sets the global default log level Dlt_DefaultMaxLogLevel. Builds response message and writes it to
 *              control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultLogLevel(void); /* 0x0011 */

/**********************************************************************************************************************
  Dlt_Srv_GetDefaultLogLevel()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response control message containing global default log level.
 * \details     Builds response control message containing global default log level Dlt_DefaultMaxLogLevel and writes
 *              it to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultLogLevel(void); /* 0x0004 */

/**********************************************************************************************************************
  Dlt_Srv_SetDefaultTraceStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines if sending trace messages is globally enabled/disabled.
 * \details     Sets/clears the global default trace status Dlt_DefaultTraceStatus. Builds response message and writes it to
 *              control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultTraceStatus(void); /* 0x0012 */

/**********************************************************************************************************************
  Dlt_Srv_GetDefaultLogLevel()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global default trace status.
 * \details     Builds response control message containing global default trace status Dlt_DefaultTraceStatus and writes
 *              it to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultTraceStatus(void); /* 0x0015 */

/**********************************************************************************************************************
  Dlt_Srv_SetUseECUID()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines if ECU ID has to be send within log and trace messages.
 * \details     Sets/clears the global flag Dlt_HeaderUseEcuId. Builds response message and writes it to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseECUID(void); /* 0x000D */

/**********************************************************************************************************************
  Dlt_Srv_GetUseECUID()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global flag status of Dlt_HeaderUseEcuId.
 * \details     Builds response message containing the global flag status of Dlt_HeaderUseEcuId. Writes the message to
 *              control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseECUID(void); /* 0x001B */

/**********************************************************************************************************************
  Dlt_Srv_SetUseSessionID()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines if session ID has to be send within log and trace messages.
 * \details     Sets/clears the global flag Dlt_HeaderUseSessionId. Builds response message and writes it to control
 *              buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseSessionID(void); /* 0x000E */

/**********************************************************************************************************************
  Dlt_Srv_GetUseSessionID()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global flag status of Dlt_HeaderUseSessionId.
 * \details     Builds response message containing the global flag status of Dlt_HeaderUseSessionId. Writes the message
 *              to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseSessionID(void); /* 0x001C */

/**********************************************************************************************************************
  Dlt_Srv_UseTimestamp()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines if timestamp has to be send within log and trace messages.
 * \details     Sets/clears the global flag Dlt_HeaderUseTimestamp. Builds response message and writes it to control
 *              buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseTimestamp(void); /* 0x000F */

/**********************************************************************************************************************
  Dlt_Srv_GetUseTimestamp()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global flag status of Dlt_HeaderUseTimestamp.
 * \details     Builds response message containing the global flag status of Dlt_HeaderUseTimestamp. Writes the message
 *              to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseTimestamp(void); /* 0x001D */

/**********************************************************************************************************************
  Dlt_Srv_SetMessageFilterering()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines if log and trace messages are filtered within DLT.
 * \details     Sets/clears the global flag Dlt_FilterMessages. Builds response message and writes it to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetMessageFilterering(void); /* 0x000A */

/**********************************************************************************************************************
  Dlt_Srv_GetMessageFiltereringStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global flag status of Dlt_FilterMessages.
 * \details     Builds response message containing the global flag status of Dlt_FilterMessages. Writes the message to
 *              control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetMessageFiltereringStatus(void); /* 0x001A */

/**********************************************************************************************************************
  Dlt_Srv_SetMessageFilterering()
**********************************************************************************************************************/
/*!
 *
 * \brief       Defines if log and trace messages have to use extended header.
 * \details     Sets/clears the global flag Dlt_HeaderUseExtendedHeader. Builds response message and writes it to
 *              control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseExtendedHeader(void); /* 0x0010 */

/**********************************************************************************************************************
  Dlt_Srv_GetUseExtendedHeader()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global flag status of Dlt_HeaderUseExtendedHeader.
 * \details     Builds response message containing the global flag status of Dlt_HeaderUseExtendedHeader. Writes the
 *              message to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseExtendedHeader(void); /* 0x001E */

/**********************************************************************************************************************
  Dlt_Srv_SetTimingPackets()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service SetTimingPackets is not supported.
 * \details     Builds response message to indicate that service SetTimingPackets is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTimingPackets(void); /* 0x000B - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetUseExtendedHeader()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the current time within ECU.
 * \details     Builds response message containing just standard and extended header because the timestamp is included
 *              in standard header. Writes the message to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLocalTime(void); /* 0x000C */

/**********************************************************************************************************************
  Dlt_Srv_SetComInterfaceStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service SetComInterfaceStatus is not supported.
 * \details     Builds response message to indicate that service SetComInterfaceStatus is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceStatus(void); /* 0x0007 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetComInterfaceStatus()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service GetComInterfaceStatus is not supported.
 * \details     Builds response message to indicate that service GetComInterfaceStatus is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceStatus(void); /* 0x0016 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_SetComInterfaceMaxBandwidth()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service SetComInterfaceMaxBandwidth is not supported.
 * \details     Builds response message to indicate that service SetComInterfaceMaxBandwidth is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceMaxBandwidth(void); /* 0x0008 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetComInterfaceMaxBandwidth()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service GetComInterfaceMaxBandwidth is not supported.
 * \details     Builds response message to indicate that service GetComInterfaceMaxBandwidth is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceMaxBandwidth(void); /* 0x0018 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetComInterfaceNames()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service GetComInterfaceNames is not supported.
 * \details     Builds response message to indicate that service GetComInterfaceNames is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceNames(void); /* 0x0017 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_StoreConfig()
**********************************************************************************************************************/
/*!
 *
 * \brief       Builds response message to indicate that service StoreConfig is not supported.
 * \details     Builds response message to indicate that service StoreConfig is not supported.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_StoreConfig(void); /* 0x0005 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_ResetToFactoryDefault()
**********************************************************************************************************************/
/*!
 *
 * \brief       Resets all configurations done in runtime.
 * \details     Resets global flags and complete context table (all registered contexts remain, but get initial
 *              configuration). Builds response message and copies it to control message.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_ResetToFactoryDefault(void); /* 0x0006 */

/**********************************************************************************************************************
  Dlt_Srv_GetSoftwareVersion()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the software version of DLT.
 * \details     Builds response control message containing the software version of DLT within the payload. The endianness
 *              depends on received message. The response message is written to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetSoftwareVersion(void); /* 0x0013 */

/**********************************************************************************************************************
  Dlt_Srv_GetSoftwareVersion()
**********************************************************************************************************************/
/*!
 *
 * \brief       Requests the global flag status of Dlt_MessageBufferOverflowHappened.
 * \details     Builds response control message containing the status of global flag Dlt_MessageBufferOverflowHappened.
 *              The response message is written to control buffer.
 * \param[in]   -
 * \return      -
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_MessageBufferOverflow(void); /* 0x0014 */
#endif /* #if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/* ####################################################################################################################
 *   HH      HH  EEEEEEEEEE  LL          PPPPPPPP    EEEEEEEEEE  RRRRRRRR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HHHHHHHHHH  EEEEEEEE    LL          PPPPPPPP    EEEEEEEE    RRRRRRRR
 *   HH      HH  EE          LL          PP          EE          RR    RR
 *   HH      HH  EE          LL          PP          EE          RR      RR
 *   HH      HH  EEEEEEEEEE  LLLLLLLLLL  PP          EEEEEEEEEE  RR      RR
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToLongLittleEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongLittleEndian(P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex,
                                                                          P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
                                                                          P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy source data from little endian array to destination 32bit variable. */
  (*DestData)  = (uint32)((uint32)(SourceData[StartIndex[0] + 3]) << 24);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 2]) << 16);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 1]) << 8);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0]]));

  /* #-- Set array index to new postion. */
  *StartIndex = StartIndex[0] + 4;
} /* Dlt_Hlp_CopyArrayToLongLittleEndian */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToLongBigEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongBigEndian(P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex,
                                                                       P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
                                                                       P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceData)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy source data from big endian array to destination 32bit variable. */
  (*DestData)  = (uint32)((uint32)(SourceData[StartIndex[0]]) << 24);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 1]) << 16);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 2]) << 8);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 3]));

  /* #-- Set array index to new position. */
  *StartIndex = StartIndex[0] + 4;
} /* Dlt_Hlp_CopyArrayToLongBigEndian */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyLongToArrayLittleEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayLittleEndian(uint32 SourceData,
                                                                          P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                          P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy source data from 32bit variable to destination little endian array.  */
  DestData[0] = (uint8)(SourceData);
  DestData[1] = (uint8)(SourceData >>  8);
  DestData[2] = (uint8)(SourceData >> 16);
  DestData[3] = (uint8)(SourceData >> 24);

  /* #-- Set array index to new postion. */
  *index = index[0] + 4;
} /* Dlt_Hlp_CopyLongToArrayLittleEndian */

# if (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN)

/**********************************************************************************************************************
 * Dlt_Hlp_CopyIntToArrayLittleEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayLittleEndian(uint16 SourceData,
                                                                         P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                         P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy source data from 16bit variable to destination little endian array.  */
  DestData[0] = (uint8)(SourceData);
  DestData[1] = (uint8)(SourceData >>  8);

  /* #-- Set array index to new postion. */
  *index = index[0] + 2;
} /* Dlt_Hlp_CopyIntToArrayLittleEndian */
# endif /* (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN) */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyLongToArrayBigEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayBigEndian(uint32 SourceData,
                                                                       P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                       P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy source data from 32bit variable to destination big endian array.  */
  DestData[0] = (uint8)(SourceData >> 24);
  DestData[1] = (uint8)(SourceData >> 16);
  DestData[2] = (uint8)(SourceData >>  8);
  DestData[3] = (uint8)(SourceData);

  /* #-- Set array index to new postion. */
  *index = index[0] + 4;
} /* Dlt_Hlp_CopyLongToArrayBigEndian */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyIntToArrayBigEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayBigEndian(uint16 SourceData,
                                                                      P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, 
                                                                      P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy source data from 16bit variable to destination big endian array.  */
  DestData[0] = (uint8)(SourceData >>  8);
  DestData[1] = (uint8)(SourceData);

  /* #-- Set array index to new postion. */
  *index = index[0] + 2;
} /* Dlt_Hlp_CopyIntToArrayBigEndian */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToArray
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToArray(P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData, P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData, uint8 NumberOfBytes)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 i;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Iterate over source and copy content to destination. */
  for (i = 0; i < NumberOfBytes; i++)
  {
    DestData[i] = SourceData[i];
  }
} /* Dlt_Hlp_CopyArrayToArray */

/**********************************************************************************************************************
 * Dlt_Hlp_SetOptionInOneContext
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOptionInOneContext(Dlt_OptionType Option, uint8 NewState, uint16 ApplicationIdx, uint16 ContextIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 retVal = (uint8)DLT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If option is set to log level, trace status or verbose mode.
   *       Change state of specified option in one couple of application ID and context ID in context table.
   *       If call back function is available, change the state also in SWC.
   */
  switch(Option)
  {
  case (DLT_OPT_LOGLEVEL):
    Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].log_level = (sint8)NewState;
    if (Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.Dlt_SetLogLevel_ != NULL_PTR)
    {
      retVal = (uint8)Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.Dlt_SetLogLevel_(Dlt_Context.app_ids[ApplicationIdx].app_id,
                                                                                         Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].context_id,
                                                                                         (Dlt_MessageLogLevelType)NewState);
    }
    break;

  case (DLT_OPT_TRACESTATUS):
    Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].trace_status = (sint8)NewState;
    if (Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.Dlt_SetTraceStatus_ != NULL_PTR)
    {
      retVal = (uint8)Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.Dlt_SetTraceStatus_(Dlt_Context.app_ids[ApplicationIdx].app_id,
                                                                                         Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].context_id,
                                                                                         (boolean)NewState);
    }
    break;

  case (DLT_OPT_VERBOSEMODE):
    Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.verbose_mode = NewState;
    if (Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.Dlt_SetVerboseMode_ != NULL_PTR)
    {
      retVal = (uint8)Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].cbk_info.Dlt_SetVerboseMode_(Dlt_Context.app_ids[ApplicationIdx].app_id,
                                                                                         Dlt_Context.app_ids[ApplicationIdx].context_id_info[ContextIdx].context_id,
                                                                                         (boolean)NewState);
    }
    break;

  /* #-- Otherwise:
   *       The specified option is invalid.
   */
  default:
    retVal = (uint8)DLT_NOT_OK;
    break;
  }

  if (retVal != DLT_OK)
  {
    retVal = DLT_NOT_OK;
  }

  return retVal;
} /* Dlt_Hlp_SetOptionInOneContext */

/**********************************************************************************************************************
 * Dlt_Hlp_GetApplicationIDIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetApplicationIDIndex(uint32 ApplicationID, P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ApplicationIDIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 retVal = (uint8)DLT_NOT_OK;
  uint16 i;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Iterate over all registered application IDs:
   *       If the application ID is registered, stop searching and return its index.
   */
  for (i = 0; (i < Dlt_Context.count_app_ids) && (retVal == (uint8)DLT_NOT_OK); i++)
  {
    if (Dlt_Context.app_ids[i].app_id == ApplicationID)
    {
      *ApplicationIDIndex = i;
      retVal = (uint8)DLT_OK;
    }
  }

  return retVal;
} /* Dlt_Hlp_GetApplicationIDIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_GetContextIDIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetContextIDIndex(uint16 ApplIdIndx, uint32 ContextID, P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 retVal = (uint8)DLT_NOT_OK;
  uint16 i;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Iterate over all registered context IDs:
   *       If the context ID is registered, stop searching and return its index.
   */
  for (i = 0; (i < Dlt_Context.app_ids[ApplIdIndx].count_context_ids) && (retVal == (uint8)DLT_NOT_OK); i++)
  {
    if (Dlt_Context.app_ids[ApplIdIndx].context_id_info[i].context_id == ContextID)
    {
      *ContextIDIndex = i;
      retVal = (uint8)DLT_OK;
    }
  }

  return retVal;
} /* Dlt_Hlp_GetContextIDIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_SetOption
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOption(uint32 ApplicationID, uint32 ContextID, Dlt_OptionType Option, uint8 NewStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 locStatus = (uint8)DLT_OK;
  uint16 applIDIndex = 0;
  uint16 contextIDIndex = 0;
  uint16 i;
  uint16 j;

  /* ----- Implementation ------------------------------------------------- */
  /* If no application ID is specified, the option change applies to all application IDs.
   * Otherwise, if no context ID is specified, the option change applies to all registered contexts of one application ID.
   * Otherwise, the option change applies to exactly one context.
   * For the given application and context IDs:
   * 	set the option to new state.
   */
  if (ApplicationID == 0)
  {
#if defined (DLT_IMPLEMENT_VERBOSE_MODE)
    if (Option == (Dlt_OptionType)DLT_OPT_VERBOSEMODE)
    {
      Dlt_HeaderUseVerboseMode = NewStatus;
    }
#endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */

    for (i = 0; (i < Dlt_Context.count_app_ids); i++)
    {
      for (j = 0; (j < Dlt_Context.app_ids[i].count_context_ids); j++)
      {
        locStatus |= Dlt_Hlp_SetOptionInOneContext(Option, NewStatus, i, j);
      }
    }
  }
  else if (ContextID == 0)
  {
    locStatus = Dlt_Hlp_GetApplicationIDIndex(ApplicationID, &applIDIndex);
    if (locStatus == (uint8)DLT_OK)
    {
      for (i = 0; (i < Dlt_Context.app_ids[applIDIndex].count_context_ids); i++)
      {
        locStatus |= Dlt_Hlp_SetOptionInOneContext(Option, NewStatus, applIDIndex, i);
      }      
    }
  }
  else
  {
    locStatus = Dlt_Hlp_GetApplicationIDIndex(ApplicationID, &applIDIndex);
    if (locStatus == (uint8)DLT_OK)
    {
      locStatus = Dlt_Hlp_GetContextIDIndex(applIDIndex, ContextID, &contextIDIndex);
    }
    if (locStatus == (uint8)DLT_OK)
    {
      locStatus = Dlt_Hlp_SetOptionInOneContext(Option, NewStatus, applIDIndex, contextIDIndex);
    }
  }

  return locStatus;
} /* Dlt_Hlp_SetOption */

/**********************************************************************************************************************
 * Dlt_Hlp_GetIndexDependingOnStandardHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_GetIndexDependingOnStandardHeader(P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) Index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Increase position of received message pointer for following flags set in the received message.
   *     The flags are "With Ecu ID", "With TiMeStamp" and "With Session ID".
   */
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
  {
    *Index = Index[0] + 4;
  }
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WTMS_Mask) == Dlt_StandardHeaderType_WTMS_Mask)
  {
    *Index = Index[0] + 4;
  }
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WSID_Mask) == Dlt_StandardHeaderType_WSID_Mask)
  {
    *Index = Index[0] + 4;
  }
} /* Dlt_Hlp_GetIndexDependingOnStandardHeader */

/**********************************************************************************************************************
 * Dlt_Hlp_IsContextRegistered
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_IsContextRegistered(Dlt_ApplicationIDType ApplId,
                                                                     Dlt_ContextIDType ContId,
                                                                     P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
                                                                     P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
                                                                     P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isContextRegistered = (boolean)FALSE;
  uint8 status;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Look for application ID in context table. */
  status = Dlt_Hlp_GetApplicationIDIndex(ApplId, AppIDIndex);

  /* #-- If the application ID and context ID are already registered and the specified session ID belongs to them:
   *       The context is already registered.
   */
  if (status == (uint8)DLT_OK)
  {
    *ApplAlreadyReg = TRUE;
    status = Dlt_Hlp_GetContextIDIndex(*AppIDIndex, ContId, ContextIDIndex);

    if (status == (uint8)DLT_OK)
    {
      isContextRegistered = (boolean)TRUE;
    }
  }

  return isContextRegistered;
} /* Dlt_Hlp_IsContextRegistered */

/**********************************************************************************************************************
 * Dlt_Hlp_CheckSessionId
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_CheckSessionId(Dlt_SessionIDType SessId,
                                                                Dlt_ApplicationIDType ApplId,
                                                                Dlt_ContextIDType ContId,
                                                                P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
                                                                P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
                                                                P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isSessionIdRegistered = (boolean)FALSE;
  boolean isContextRegistered;

  /* #-- Look for Application ID and Context ID in context table. */
  isContextRegistered = Dlt_Hlp_IsContextRegistered(ApplId, ContId, AppIDIndex, ContextIDIndex, ApplAlreadyReg);

  /* #-- If the specified Session ID belong to the Application ID and the Context ID, the Session is already registered. */
  if ((isContextRegistered == (uint8)TRUE) &&
    (Dlt_Context.app_ids[*AppIDIndex].context_id_info[*ContextIDIndex].cbk_info.session_id == SessId))
  {
    isSessionIdRegistered = (boolean)TRUE;
  }

  return isSessionIdRegistered;
  
  /* PRQA S 6060 1 */ /* MD_MSR_STPAR */
} /* Dlt_Hlp_CheckSessionId */

/**********************************************************************************************************************
 * Dlt_Hlp_GetLengthOfReceivedMsg
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetLengthOfReceivedMsg(uint32 BufferIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 lengthOfReceivedMsg;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If the receive buffer contains the first 4 byte of new message, get the complete message length. */
  if (Dlt_CopyRxDataLength == 4)
  {
    /* #-- Indicate that the received message length was successfully read. */
    Dlt_CopyRxDataCounter++;

    /* #-- Get the message length depending on position of message in receive buffer. */
    if (BufferIndex == 0)
    {
      lengthOfReceivedMsg  =  (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[DLT_CONTROL_BUFFER_SIZE - 2] << 8);
      lengthOfReceivedMsg |=  (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[DLT_CONTROL_BUFFER_SIZE - 1]);
    }
    else if (BufferIndex == 1)
    {
      lengthOfReceivedMsg  =  (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[DLT_CONTROL_BUFFER_SIZE - 1] << 8);
      lengthOfReceivedMsg |=  (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[0]);
    }
    else
    {
      lengthOfReceivedMsg  =  (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[BufferIndex - 2] << 8);
      lengthOfReceivedMsg |=  (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[BufferIndex - 1]);
    }

    /* #-- Request rest of received message */
    Dlt_CopyRxDataLength = lengthOfReceivedMsg - 4;
    lengthOfReceivedMsg = Dlt_CopyRxDataLength;
  }
  /* #-- Otherwise, request the missing bytes to get the first 4 bytes. */
  else
  {
    lengthOfReceivedMsg = 4 - Dlt_CopyRxDataLength;
  }

  return lengthOfReceivedMsg;
} /* Dlt_Hlp_GetLengthOfReceivedMsg */

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        MM      MM    SSSSSS      GGGGGG
 *       TT        XX  XX          MMMM  MMMM  SS      SS  GG      GG
 *       TT        XX  XX          MM  MM  MM  SS          GG
 *       TT          XX      ====  MM  MM  MM    SSSSSS    GG    GGGG
 *       TT        XX  XX          MM      MM          SS  GG      GG
 *       TT        XX  XX          MM      MM  SS      SS  GG      GG
 *       TT      XX      XX        MM      MM    SSSSSS      GGGGGGGG
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_TxMsg_Transmit
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_Transmit(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo,
                                                            uint16 PayloadLength,
                                                            P2CONST(uint8, AUTOMATIC, DLT_CONST) Payload)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal;
  uint32  i;
  uint16  appIdIndex = 0;
  uint16  conIdIndex = 0;
  uint16  locLength = 4; /* At least the shortest standard header has to be transmitted (4Byte) */
  Dlt_MessageOptionsType messageOptions;
  
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Check if transmit of log message or trace message is requested and get according message option. */
  if (FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    messageOptions = (Dlt_MessageOptionsType)FilterInfo->log_info.options;
  }
  else
  {
    messageOptions = (Dlt_MessageOptionsType)FilterInfo->trace_info.options;
  }
  /* #-- If message is vaild build frame. */
  retVal = Dlt_TxMsg_IsMessageValid(FilterInfo, &appIdIndex, &conIdIndex, messageOptions);

  if (retVal == (Dlt_ReturnType)DLT_E_OK)
  {
    /* #-- Initialize temp data buffer. */
    for (i = 0; i < (uint32)DLT_MAX_MESSAGE_LENGTH; i++)
    {
      Dlt_TransmitMessage[i] = 0;
    }

    /* #-- Set flags of Standard Header. */
    Dlt_TxMsg_SetStandardHeader(FilterInfo, &Dlt_TransmitMessage[0], &locLength);

    /* #-- If the extended header has to be used for this message:
	 *       Add the extended header to the frame and set verbose mode flag in message if active.
	 */
# if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
    if (Dlt_HeaderUseExtendedHeader == (uint8)TRUE)
    {
      Dlt_TransmitMessage[0] |= (uint8)Dlt_StandardHeaderType_UEH_Mask;

      if ((messageOptions & DLT_VERBOSE_MSG) == DLT_VERBOSE_MSG)
      {
        Dlt_TransmitMessage[locLength] |= (uint8)Dlt_ExtendedHeaderType_VERB_Mask;
      }

      retVal = Dlt_TxMsg_SetExtendedHeader(FilterInfo, &Dlt_TransmitMessage[0], &locLength, appIdIndex, conIdIndex);
    }
# endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
    Dlt_TransmitMessage[0] |= (uint8)(Dlt_StandardHeaderType_SoftwareVersion << 5);

    /* #-- If header length and payload exceed max message length, return error invalid message length. */
    if ((PayloadLength + locLength) > DLT_MAX_MESSAGE_LENGTH)
    {
      retVal = (Dlt_ReturnType)DLT_E_MSG_TOO_LARGE;
    }
    
    /* #-- If the message is valid, set the all left data in frame and copy it to send buffer. */
    if (retVal == (Dlt_ReturnType)DLT_E_OK)
    {
      for (i = 0; i < PayloadLength; i++)
      {
        Dlt_TransmitMessage[locLength + i] = Payload[i]; /*lint -e661 */ /* Suppress ID661 because out-of-bounds access is checked previously */
      }
    
      Dlt_TransmitMessage[1] = Dlt_MessageCounter;
      Dlt_MessageCounter++;

      locLength = locLength + PayloadLength;
      Dlt_TransmitMessage[2] = (uint8)(locLength >> 8);
      Dlt_TransmitMessage[3] = (uint8)(locLength);

      retVal = Dlt_TransmitData(DLT_VERBOSE_MSG_EVENT, Dlt_TransmitMessage, locLength);
    }
  }
  
  return retVal;
  /* PRQA S 6010 4 */ /* MD_MSR_STPTH */
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */
  /* PRQA S 6060 2 */ /* MD_MSR_STPAR */
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Dlt_TxMsg_Transmit() */

/**********************************************************************************************************************
 * Dlt_TxMsg_IsMessageValid
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_IsMessageValid(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo,
                                                                         P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIdIndex, 
                                                                         P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIdIndex,
                                                                         Dlt_MessageOptionsType MessageOptions)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retValue;
  Dlt_SessionIDType SessionId = FilterInfo->session_id;
  Dlt_ApplicationIDType ApplicationlId;
  Dlt_ContextIDType ContextId;
  boolean applAlreadyReg = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If the context is not registered, reject message. */
  if (FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    ApplicationlId = FilterInfo->log_info.app_id;
    ContextId = FilterInfo->log_info.context_id;
  }
  else
  {
    ApplicationlId = FilterInfo->trace_info.app_id;
    ContextId = FilterInfo->trace_info.context_id;
  }

  if (Dlt_Hlp_CheckSessionId(SessionId, ApplicationlId, ContextId, AppIdIndex, ContextIdIndex, &applAlreadyReg) == (uint8)FALSE)
  {
      retValue = (Dlt_ReturnType)DLT_E_UNKNOWN_SESSION_ID;
  }
  /* #-- Otherwise, if verbose mode is requested but not supported, reject message. */
  else if (((MessageOptions & DLT_VERBOSE_MSG) != (boolean)FALSE) 
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
        && ((Dlt_HeaderUseVerboseMode    == (uint8)FALSE)
        ||  (Dlt_HeaderUseExtendedHeader == (uint8)FALSE))
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */
          )
  {
    retValue = (Dlt_ReturnType)DLT_E_NOT_IN_VERBOSE_MODE;
  }
  /* #-- Otherwise, if the message type is log, check the log specific options. */
  else if (FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    /* #-- If the message option is not set to log, reject message. */
    if ((MessageOptions & (uint8)Dlt_MessageTypeTypeMask) != (uint8)DLT_TYPE_LOG)
    {
      retValue = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
    }
    /* #-- Otherwise, if filtering is enabled and the log level of the message has lower priority than minimal allowed, reject message.  */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
    /* Reject the message if the log level is higher than the allowed log level, reject message. */
    else if (((Dlt_FilterMessages == (uint8)TRUE) &&
              (Dlt_Context.app_ids[*AppIdIndex].context_id_info[*ContextIdIndex].log_level > (sint8)Dlt_DefaultMaxLogLevel)) ||
              (Dlt_Context.app_ids[*AppIdIndex].context_id_info[*ContextIdIndex].log_level == 0))
    {
      retValue = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
    }
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
    /* #-- Otherwise, the message is valid. */
    else
    {
      retValue = (Dlt_ReturnType)DLT_E_OK;
    }
  }
  /* #-- Otherwise, if the message type is trace, check the trace specific options. */
  else if ((FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_APP_TRACE) ||
           (FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_NW_TRACE)
          )
  {
    /* #-- If the message option is not set to application trace or network trace, reject message.  */
    if (((MessageOptions & (uint8)Dlt_MessageTypeTypeMask) != (uint8)DLT_TYPE_APP_TRACE) &&
        ((MessageOptions & (uint8)Dlt_MessageTypeTypeMask) != (uint8)DLT_TYPE_NW_TRACE)
       )
    {
      retValue = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
    }
    /* #-- Otherwise, if filtering is enabled and the trace status of message is set to off, reject message. */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
    /* Reject the message if the log level is higher than the allowed log level. */
    else if ((Dlt_FilterMessages == (uint8)TRUE) &&
             (Dlt_Context.app_ids[*AppIdIndex].context_id_info[*ContextIdIndex].trace_status == (sint8)DLT_OFF)
            )
    {
      retValue = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
    }
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
    /* #-- Otherwise, the message is valid. */
    else
    {
      retValue = (Dlt_ReturnType)DLT_E_OK;
    }
  }
  /* #-- Otherwise, the message is invalid. */
  else
  {
    retValue = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
  }

  return retValue;
} /* Dlt_TxMsg_IsMessageValid() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Dlt_TxMsg_SetStandardHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxMsg_SetStandardHeader(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo, 
                                                                  P2VAR(uint8, AUTOMATIC, DLT_CONST) Data, 
                                                                  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
  uint32 ecuID = (uint32)DLT_ECU_ID;
# endif /* (DLT_HEADER_USE_ECU_ID == STD_ON) */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
  uint32 timestamp = 0;
  /* #-- If Dlt global state is DLT_GLOBAL_STATE_ONLINE, the used time source is initialized and returns valid value. > SPEC-42734 */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    timestamp = Dlt_GetCurrentTime();
  }
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */

  /* ----- Implementation ------------------------------------------------- */
  /* Standard Header */
  /* #-- If byte order is set to big endian, set the flag "most significant byte first" in the message */
# if (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_BIGENDIAN)
  Data[0] |= (uint8)Dlt_StandardHeaderType_MSBF_Mask;
# endif /* (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_BIGENDIAN) */

  /* #-- If ecu ID transmission is enabled, set the according flag in message and the add the ecu ID */
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
  if (Dlt_HeaderUseEcuId == (uint8)TRUE)
  { /* Add With ECU ID flag to standard header */
    Data[0] |= (uint8)Dlt_StandardHeaderType_WEID_Mask;
    /* Add ECU ID */
    Dlt_Hlp_CopyLongToArrayBigEndian(ecuID, &Data[(*DataIndex)], DataIndex);
  }
# endif /* (DLT_HEADER_USE_ECU_ID == STD_ON) */
  /* #-- If the session ID is globally enabled, set the according flag in message and add the session ID */
  if (Dlt_HeaderUseSessionId == (uint8)TRUE)
  { /* Add With Session ID flag to standard header */
    Data[0] |= (uint8)Dlt_StandardHeaderType_WSID_Mask;
    /* Add session ID */
    Dlt_Hlp_CopyLongToArrayBigEndian(FilterInfo->session_id, &Data[(*DataIndex)], DataIndex);
  }
  /* #-- If timestamp is enabled, set the according flag in message and add the timestamp */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
  if (Dlt_HeaderUseTimestamp == (uint8)TRUE)
  { /* Add With Timestamp flag to standard header */
    Data[0] |= (uint8)Dlt_StandardHeaderType_WTMS_Mask;
    /* Add timestamp */
    Dlt_Hlp_CopyLongToArrayBigEndian(timestamp, &Data[(*DataIndex)], DataIndex);
  }
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */
} /* Dlt_TxMsg_SetStandardHeader */

# if defined (DLT_IMPLEMENT_EXTENDED_HEADER)

/**********************************************************************************************************************
 * Dlt_TxMsg_SetExtendedHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_SetExtendedHeader(P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_CONST) FilterInfo,
                                                                              P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
                                                                              P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex,
                                                                              uint16 AppIdIndex,
                                                                              uint16 ConIdIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retValue = (Dlt_ReturnType)DLT_E_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If the message type is log, set the log specific options in message */
  if (FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    Data[*DataIndex]      |= (uint8)(DLT_TYPE_LOG << 1);
    Data[*DataIndex]      |= (uint8)(((uint8)Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].log_level) << 4);
    Data[(*DataIndex) + 1] = (uint8)(FilterInfo->log_info.arg_count); /* Option: is used as Number Of Arguments */
  }
  /* #-- Otherwise, if the message type is application trace or network trace, set the trace specific options in message */
  else if ((FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_APP_TRACE) ||
           (FilterInfo->message_type == (Dlt_MessageTypeType)DLT_TYPE_NW_TRACE)
          )
  {
    Data[*DataIndex]      |= (uint8)(FilterInfo->message_type << 1);
    Data[*DataIndex]      |= (uint8)(FilterInfo->trace_info.trace_info << 4); /* Option: is used as Dlt_MessageTraceType (DLT_TRACE_VARIABLE|DLT_TRACE_FUNCTION_IN|...) */
    Data[(*DataIndex) + 1] = (uint8)(0); /* Number of arguments = 0, no info in Dlt_SendTraceMessage */
  }
  /* #-- Otherwise, the message is invalid. */
  else
  {
    retValue = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
  }

  *DataIndex = (*DataIndex) + 2;

  /* #-- Add application ID and context ID to frame. */
  Dlt_Hlp_CopyLongToArrayBigEndian((uint32)Dlt_Context.app_ids[AppIdIndex].app_id, &Data[*DataIndex], DataIndex);
  
  Dlt_Hlp_CopyLongToArrayBigEndian((uint32)Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].context_id, &Data[*DataIndex], DataIndex);

  return retValue;
} /* Dlt_TxMsg_SetExtendedHeader */
# endif /* defined (DLT_IMPLEMENT_EXTENDED_HEADER) */

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        BBBBBBBB    UU      UU  FFFFFFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT          XX      ====  BBBBBBBB    UU      UU    FFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT      XX      XX        BBBBBBBB      UUUUUU        FF
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_TxBuf_WriteToBuf
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxBuf_WriteToBuf(uint8 DltEvent, P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) Data, uint16 DataLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = DLT_E_ERROR_UNKNOWN;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If the message is triggered by application: */
  if ((DltEvent == (uint8)DLT_VERBOSE_MSG_EVENT) ||
      (DltEvent == (uint8)DLT_NON_VERBOSE_MSG_EVENT)
     )
  {
    uint32 i;
    uint32 bufferIndex = Dlt_SendBuffer.Write;
    
    /* #-- Copy message to send buffer.
     *     If send buffer is full:
     *       Set flag for message buffer overflow.
     *       Create response control message for message buffer overflow and copy it to control buffer.
     */
    for (i = 0; (i < (uint32)DataLength) && (Dlt_MessageBufferOverflowHappened == (uint8)FALSE); i++)
    {
      Dlt_SendBuffer.Buffer[bufferIndex] = Data[i];
      bufferIndex++;
      if (bufferIndex >= (uint32)DLT_MESSAGE_BUFFER_SIZE)
      {
        bufferIndex = 0;
      }
      if (bufferIndex == Dlt_SendBuffer.Read)
      {
        Dlt_MessageBufferOverflowHappened = (uint8)TRUE;
        if (Dlt_CurrentState != (Dlt_StateType)DLT_STATE_UNINIT)
        {
          Dlt_Sm_SendMsgBufferOverflow();
        }
      }
    }

    /* #-- If overflow occured, return error ID.
     *     Otherwise, update write pointer position of send buffer and return OK.
     */
    if (Dlt_MessageBufferOverflowHappened == (uint8)TRUE)
    {
      errorId = DLT_E_MSG_LOOSE;
      retVal = (Dlt_ReturnType)DLT_E_IF_NOT_AVAILABLE;
    }
    else
    {
      Dlt_SendBuffer.Write = bufferIndex;
      retVal = (Dlt_ReturnType)DLT_E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_SENDLOGMESSAGE, errorId);
  }
#else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */ 
#endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_TxBuf_WriteToBuf */

/**********************************************************************************************************************
 * Dlt_TxBuf_WriteToCtrlBuf
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteToCtrlBuf(uint16 Length)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i;
  uint32 bufferIndex = Dlt_ControlSendBuffer.Write;
  uint8  bufferOverflowHappened = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Copy response control message to control buffer until completed or buffer overflow.
   */
  for (i = 0; (i < Length) && (bufferOverflowHappened == FALSE); i++)
  {
    Dlt_ControlSendBuffer.Buffer[bufferIndex] = Dlt_ControlSendMessage[i];
    bufferIndex++;
    if (bufferIndex >= (uint32)DLT_CONTROL_BUFFER_SIZE)
    {
      bufferIndex = 0;
    }
    if (bufferIndex == Dlt_ControlSendBuffer.Read)
    {
      bufferOverflowHappened = (uint8)TRUE;
    }
  }

  /* #-- if copy process succeeded, update write pointer position of control buffer. */
  if (bufferOverflowHappened == (uint8)FALSE)
  {
    Dlt_ControlSendBuffer.Write = bufferIndex;
  }
} /* Dlt_TxBuf_WriteToCtrlBuf */

/**********************************************************************************************************************
 * Dlt_TxBuf_PrepareRespInCtrlBuf
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_PrepareRespInCtrlBuf(uint16 Length)
{ /* Fill in fixed data */
  /* ----- Local Variables ------------------------------------------------ */
  uint16 index = 2;
  uint32 ecuID = (uint32)DLT_ECU_ID;
  uint32 timestamp;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- if timestamp is enabled, get the current time */
#if defined (DLT_IMPLEMENT_TIMESTAMP)
  timestamp = Dlt_GetCurrentTime();
#else /* defined (DLT_IMPLEMENT_TIMESTAMP) */
  timestamp = 0;
#endif /* defined (DLT_IMPLEMENT_TIMESTAMP) */
  
  /* #-- if received control message is in big endian
   *       send response also in big endian
   *     otherwise
   *       send response in little endian
   */
  if (Dlt_IsPayloadInBigEndian())
  {
    Dlt_ControlSendMessage[ 0] = Dlt_ControlMessageHeaderType_MSBF; /* Header type - 0b001 1 0 1 1 1 - Version 3 | WTMS 1 | SID 0 | WEID 1 | MSBF 1 | UEH 1 */
  }
  else
  {
    Dlt_ControlSendMessage[ 0] = Dlt_ControlMessageHeaderType_LSBF; /* Header type - 0b001 1 0 1 0 1 - Version 3 | WTMS 1 | SID 0 | WEID 1 | MSBF 0 | UEH 1 */
  }

  /* #-- set current message counter value to response message */
  Dlt_ControlSendMessage[ 1] = Dlt_MessageCounter;

  /* #-- set length of message, ECU ID and timestamp in response message */
  Dlt_Hlp_CopyIntToArrayBigEndian(Length, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(ecuID, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(timestamp, &Dlt_ControlSendMessage[index], &index);

  /* #-- calculate the index of the  */
  index = 6;
  Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);

  /* #-- Set message info to
   *       response message
   *       control message
   *       non-verbose mode
   */
  Dlt_ControlSendMessage[12] = Dlt_ControlMessageMessageInfo; /* Response | Control | Non-Verbose */
  /* #-- Copy application ID, context ID and service ID from received message to response message */
  Dlt_Hlp_CopyArrayToArray(&Dlt_ControlSendMessage[14], &Dlt_ReceiveMessage[index], 12);

  Dlt_MessageCounter++;
} /* Dlt_TxBuf_PrepareRespInCtrlBuf */

/**********************************************************************************************************************
 * Dlt_TxBuf_CalculateMessageLength
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint32, DLT_CODE) Dlt_TxBuf_CalculateMessageLength(uint32 ReadIndex,
                                                                         uint32 WriteIndex,
                                                                         uint32 bufferSize)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 locLength;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Calculate number of bytes to be send. */
  if (ReadIndex <= WriteIndex)
  {
    locLength = WriteIndex - ReadIndex;
  }
  else
  {
    locLength = ((uint32)bufferSize - ReadIndex) + WriteIndex;
  }

  /* #-- Number of bytes to be sent is limited to max message length AND size of uint16. */
#if (DLT_TCPIP_MAX_TX_BUFFER > 0xFFFF)
  if (locLength > (uint32)0xFFFF)
  {
    locLength = (uint32)0xFFFF;
  }
#else /* (DLT_TCPIP_MAX_TX_BUFFER <= 0xFFFF) */
  if (locLength > (uint32)DLT_TCPIP_MAX_TX_BUFFER)
  {
    locLength = (uint32)DLT_TCPIP_MAX_TX_BUFFER - 1;
  }
#endif /* (DLT_TCPIP_MAX_TX_BUFFER > 0xFFFF) */

  return locLength;
} /* Dlt_TxBuf_CalculateMessageLength */

/**********************************************************************************************************************
 * Dlt_TxBuf_SetReadIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_SetReadIndex(boolean setControlBuffer, uint32 bufferIdx)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Update read pointer of control buffer or send buffer. Decided by caller. */
  if (setControlBuffer == (boolean)TRUE)
  {
    Dlt_ControlSendBuffer.Read = bufferIdx;
  }
  else
  {
    Dlt_SendBuffer.Read = bufferIdx;
  }
} /* Dlt_TxBuf_SetReadIndex */

/**********************************************************************************************************************
 * Dlt_TxBuf_GetEcuIdAndService
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_GetEcuIdAndService(P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) EcuId, 
                                                                   P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) ServiceId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 index = 14; /* = HTYP(1Byte) + MCNT(1Byte) + LEN(2Byte) + MSIN(1Byte) + NOAR(1Byte) + APID(4Byte) + CTID(4Byte) */

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Get index of Service ID from receive buffer. */
  Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);

  /* #-- Get Service ID from received message, depending on endianness. */
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_MSBF_Mask) == Dlt_StandardHeaderType_MSBF_Mask) /*  Do not use Dlt_IsPayloadInBigEndian for Service ID */
  { /* Message in big endian */
    Dlt_Hlp_CopyArrayToLongBigEndian(&index, ServiceId, &Dlt_ReceiveMessage[0]);
  }
  else
  { /* Message in little endian */
    Dlt_Hlp_CopyArrayToLongLittleEndian(&index, ServiceId, &Dlt_ReceiveMessage[0]);
  }

  /* #-- Get ECU ID from received message (header is always in big endian). */
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
  {
    index = 4;
    Dlt_Hlp_CopyArrayToLongBigEndian(&index, EcuId, &Dlt_ReceiveMessage[0]);
  }
} /* Dlt_TxBuf_GetEcuIdAndService */

/**********************************************************************************************************************
 * Dlt_TxBuf_WriteBufToTxMsg
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteBufToTxMsg(P2CONST(uint8, AUTOMATIC, DLT_CONST) SourceBuffer, uint32 ReadIndex, uint32 WriteIndex, boolean IsControlBuffer)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 locLength;
  uint32 bufferIndex;
  uint32 i;
  uint32 bufferSize;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Load buffer size, depending on which buffer is specified in IsControlBuffer. */
  if (IsControlBuffer == (boolean)TRUE)
  {
    bufferSize = (uint32)DLT_CONTROL_BUFFER_SIZE;
  }
  else
  {
    bufferSize = (uint32)DLT_MESSAGE_BUFFER_SIZE;
  }

  /* #-- Get the number of bytes to send. */
  locLength = Dlt_TxBuf_CalculateMessageLength(ReadIndex, WriteIndex, bufferSize);

  bufferIndex = ReadIndex;
  /* #-- Depending on chosen buffer either content of control buffer or content of send buffer is copied to tx message. */
  for (i = 0; i < locLength; i++)
  {
    Dlt_SendData[i] = SourceBuffer[bufferIndex];
    bufferIndex++;
    if (bufferIndex >= bufferSize)
    {
      bufferIndex = 0;
    }
  }
  Dlt_SendPdu.SduLength  = (uint16)locLength;
  Dlt_SendPdu.SduDataPtr = Dlt_SendData;

  /* #-- If the socket connection is online, send the tx message. */
  if (Dlt_SoConMode == (uint8)DLT_SOCON_ONLINE)
  {
    Dlt_SendingOrderAccepted = DltCom_Transmit(0, &Dlt_SendPdu);
  }

  /* #-- If the sending was successful:
   *       Depending on the specified buffer, the read pointer of control buffer or send buffer is set to new position.
   */
  if (Dlt_SendingOrderAccepted == DLT_OK)
  {
    Dlt_TxBuf_SetReadIndex(IsControlBuffer, bufferIndex);
  }
} /* Dlt_TxBuf_WriteBufToTxMsg */

/* ####################################################################################################################
 *     SSSSSS    MM      MM
 *   SS      SS  MMMM  MMMM
 *   SS          MM  MM  MM
 *     SSSSSS    MM  MM  MM
 *           SS  MM      MM
 *   SS      SS  MM      MM
 *     SSSSSS    MM      MM
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_Sm_HandleSendingTransitions
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_HandleSendingTransitions(void)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If the message was successfully transmitted:
   *       Initialize control variables for successful transmission and timeout.
   *       Transition from state DLT_STATE_SENDING to DLT_STATE_READY_TO_SEND
   */
  if (Dlt_MessageTransmitted == TRUE)
  {
    Dlt_MessageTransmitted = FALSE;
    Dlt_Timeout = 0;
    Dlt_Sm_StateTransition(kDltEventSendingSucceeded);
  }
  /* #-- Otherwise:
   *       Increase timeout counter.
   *       If timeout occurs:
   *         Initialize control variables for timeout and Rx indication.
   *         Transition from state DLT_STATE_SENDING to DLT_STATE_WAIT_FOR_RECEP
   */
  else
  {
    Dlt_Timeout++;
    if (Dlt_Timeout >= DLT_TIMEOUT)
    {
      Dlt_Timeout = 0;
      Dlt_RxIndicationOccured = 0;
      Dlt_Sm_StateTransition(kDltEventTimeout);
    }
  }
} /* Dlt_Sm_HandleSendingTransitions */

/**********************************************************************************************************************
 * Dlt_Sm_StateTransition
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_StateTransition(const Dlt_EventType transitionEvent)
{
  /* ----- Local Variables ------------------------------------------------ */
  DLT_LOCAL Dlt_StateType const kDltStateMatrix[kDltStateMachineStates][kDltStateMachineEvents] =
  {
                       /*   kDltEventInitDlt, 
                        *                             kDltEventMessageReceived, 
                        *                                                      kDltEventOneBufferHasContent, 
                        *                                                                                kDltEventTransmitCallSuccessful, 
                        *                                                                                                          kDltEventTransmitCallRejected, 
                        *                                                                                                                                    kDltEventSendingSucceeded, 
                        *                                                                                                                                                              kDltEventTimeout */
  /* unint */             { DLT_STATE_WAIT_FOR_RECEP, DLT_STATE_UNINIT,        DLT_STATE_UNINIT,         DLT_STATE_UNINIT,         DLT_STATE_UNINIT,         DLT_STATE_UNINIT,         DLT_STATE_UNINIT         },
  /* wait for sending */  { DLT_STATE_WAIT_FOR_RECEP, DLT_STATE_READY_TO_SEND, DLT_STATE_WAIT_FOR_RECEP, DLT_STATE_WAIT_FOR_RECEP, DLT_STATE_WAIT_FOR_RECEP, DLT_STATE_WAIT_FOR_RECEP, DLT_STATE_WAIT_FOR_RECEP },
  /* ready for sending */ { DLT_STATE_READY_TO_SEND,  DLT_STATE_READY_TO_SEND, DLT_STATE_TRY_SENDING,    DLT_STATE_READY_TO_SEND,  DLT_STATE_READY_TO_SEND,  DLT_STATE_READY_TO_SEND,  DLT_STATE_READY_TO_SEND  },
  /* try sending */       { DLT_STATE_TRY_SENDING,    DLT_STATE_TRY_SENDING,   DLT_STATE_TRY_SENDING,    DLT_STATE_SENDING,        DLT_STATE_READY_TO_SEND,  DLT_STATE_TRY_SENDING,    DLT_STATE_TRY_SENDING    },
  /* sending */           { DLT_STATE_SENDING,        DLT_STATE_SENDING,       DLT_STATE_SENDING,        DLT_STATE_SENDING,        DLT_STATE_SENDING,        DLT_STATE_READY_TO_SEND,  DLT_STATE_WAIT_FOR_RECEP }
  };

  DLT_LOCAL void (* const kDltStateFctMatrix[kDltStateMachineStates][kDltStateMachineStates])(void) =
  {
    /* | last state */
    /* v  */    /* -> new state */ /* uninit,               wait for sending,     ready for sending,    try sending,            sending, */
    /* unint */                     { Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction,   Dlt_Sm_DummyFunction },
    /* wait for sending */          { Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction,   Dlt_Sm_DummyFunction },
    /* ready for sending */         { Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_SendingDecision, Dlt_Sm_DummyFunction },
    /* try sending */               { Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction,   Dlt_Sm_DummyFunction },
    /* sending */                   { Dlt_Sm_DummyFunction, Dlt_Sm_ClearBuffer,   Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction,   Dlt_Sm_DummyFunction }
  };

  Dlt_StateType lastState = Dlt_CurrentState;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Set new state depending on occurred event and previous state. Call transition function. */
  Dlt_CurrentState = kDltStateMatrix[lastState][transitionEvent];
  kDltStateFctMatrix[lastState][Dlt_CurrentState]();
} /* Dlt_Sm_StateTransition */

/**********************************************************************************************************************
 * Dlt_Sm_DummyFunction
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_DummyFunction(void)
{
  /* Do nothing */
} /* Dlt_Sm_DummyFunction */

/**********************************************************************************************************************
 * Dlt_Sm_SendingDecision
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendingDecision(void)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If a complete message was received:
   *       Create according response frame and write it to control buffer. */
  if ((Dlt_ReceiveBuffer.Read != Dlt_ReceiveBuffer.Write) &&
      (Dlt_RxIndicationOccured > 0))
  {
    Dlt_Sm_BuildResponseMessage();
    Dlt_RxIndicationOccured = 0;
  }

  /* #-- If control buffer or send buffer has content, send one buffer content. Control messages have higher priority. */
  if (Dlt_ControlSendBuffer.Read != Dlt_ControlSendBuffer.Write)
  {
    Dlt_TxBuf_WriteBufToTxMsg(Dlt_ControlSendBuffer.Buffer, Dlt_ControlSendBuffer.Read, Dlt_ControlSendBuffer.Write, (boolean)TRUE);
  }
  /* ... No:  Check if Log or Trace Message in Dlt_SendBuffer */
  else if (Dlt_SendBuffer.Read != Dlt_SendBuffer.Write)
  {
    Dlt_TxBuf_WriteBufToTxMsg(Dlt_SendBuffer.Buffer, Dlt_SendBuffer.Read, Dlt_SendBuffer.Write, (boolean)FALSE);
  }
  else
  {
  }
  
  /* #-- If send buffer has no content AND buffer overflow flag was set:
   *       Reset message buffer overflow flag. */
  if (Dlt_SendBuffer.Read == Dlt_SendBuffer.Write)
  {
    if (Dlt_MessageBufferOverflowHappened == (uint8)TRUE)
    {
      Dlt_MessageBufferOverflowHappened = (uint8)FALSE;
    }
  }
} /* Dlt_Sm_SendingDecision() */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 * Dlt_Sm_BuildResponseMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_BuildResponseMessage(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  DLT_LOCAL void (* const dltServiceFctArray[DLT_NUMBER_OF_SERVICES]) (void) =
  {
    Dlt_Srv_DummyService /* 0x0000 */,                                Dlt_Srv_SetLogLevel /* 0x0001 */,                           Dlt_Srv_SetTraceStatus /* 0x0002 */, 
    Dlt_Srv_GetLogInfo /* 0x0003 */,                                  Dlt_Srv_GetDefaultLogLevel /* 0x0004 */,                    Dlt_Srv_StoreConfig /* 0x0005 - Not supported */, 
    Dlt_Srv_ResetToFactoryDefault /* 0x0006 - Not supported */,       Dlt_Srv_SetComInterfaceStatus /* 0x0007 - Not supported */, Dlt_Srv_SetComInterfaceMaxBandwidth /* 0x0008 - Not supported */, 
    Dlt_Srv_SetVerboseMode /* 0x0009 */,                              Dlt_Srv_SetMessageFilterering /* 0x000A */,                 Dlt_Srv_SetTimingPackets /* 0x000B - Not supported */,
    Dlt_Srv_GetLocalTime /* 0x000C */,                                Dlt_Srv_SetUseECUID /* 0x000D */,                           Dlt_Srv_SetUseSessionID /* 0x000E */, 
    Dlt_Srv_UseTimestamp /* 0x000F */,                                Dlt_Srv_UseExtendedHeader /* 0x0010 */,                     Dlt_Srv_SetDefaultLogLevel /* 0x0011 */, 
    Dlt_Srv_SetDefaultTraceStatus /* 0x0012 */,                       Dlt_Srv_GetSoftwareVersion /* 0x0013 */,                    Dlt_Srv_MessageBufferOverflow /* 0x0014 */, 
    Dlt_Srv_GetDefaultTraceStatus /* 0x0015 */,                       Dlt_Srv_GetComInterfaceStatus /* 0x0016 - Not supported */, Dlt_Srv_GetComInterfaceNames /* 0x0017 - Not supported */, 
    Dlt_Srv_GetComInterfaceMaxBandwidth /* 0x0018 - Not supported */, Dlt_Srv_GetVerboseModeStatus /* 0x0019 */,                  Dlt_Srv_GetMessageFiltereringStatus /* 0x001A */, 
    Dlt_Srv_GetUseECUID /* 0x001B */,                                 Dlt_Srv_GetUseSessionID /* 0x001C */,                       Dlt_Srv_GetUseTimestamp /* 0x001D */, 
    Dlt_Srv_GetUseExtendedHeader /* 0x001E */
  };

  uint32 i;
  uint32 bufferIndex;
  uint32 serviceID;
  uint32 ecuID = 0;
  uint16 locLength;
  uint8  avoidDeadLock = 0;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Iterate over receive buffer content until empty: */
  while ((Dlt_ReceiveBuffer.Read != Dlt_ReceiveBuffer.Write) &&
         (Dlt_RxIndicationOccured > 0))
  {
    locLength = (uint16)DLT_MAX_MESSAGE_LENGTH;
    bufferIndex = Dlt_ReceiveBuffer.Read;
	
    /* #-- Iterate over receive buffer to extract one receive message: */
    for (i = 0; (i < (uint32)DLT_MAX_MESSAGE_LENGTH) && (i < (uint32)locLength); i++)
    {
      Dlt_ReceiveMessage[i] = Dlt_ReceiveBuffer.Buffer[bufferIndex];
      /* Get real message length to stop copy if message ends */
      if (i == 3)
      {
        locLength  = (uint16)((uint16)Dlt_ReceiveMessage[2] << 8);
        locLength |= (uint16)((uint16)Dlt_ReceiveMessage[3]);
      }
      bufferIndex++;
      if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
      {
        bufferIndex = 0;
      }
    }
    
    /* #-- If message has invalid length or dead lock was detected:
     *       Reject all remaining messages in receive buffer.
     */
    if ((locLength > (uint16)DLT_MAX_MESSAGE_LENGTH) ||
        (avoidDeadLock > 200))
    {
      Dlt_ReceiveBuffer.Read = Dlt_ReceiveBuffer.Write;
    }
    /* #-- Otherwise:
     *       Extract ECU ID and service ID from received message.
     *       If the message is not assigned to this ECU:
     *         Ignore message.
     *       Otherwise, if service ID is valid:
     *         Call according service function to build response.
     *       Otherwise:
     *         Respond with service not supported (DLT_NOT_SUPPORTED).
     */
    else
    {
      Dlt_ReceiveBuffer.Read = bufferIndex;
      Dlt_RxIndicationOccured--;

      Dlt_TxBuf_GetEcuIdAndService(&ecuID, &serviceID);

      if ((ecuID != (uint32)DLT_ECU_ID) &&
           (ecuID != (uint32)0))
      {
        /* Ignore incoming message if the message's ECU ID is not identically with ECUs ID */
      }
      else if (serviceID < DLT_NUMBER_OF_SERVICES)
      {
        dltServiceFctArray[serviceID]();
      }
      else
      {
        dltServiceFctArray[0]();
      }
    }
    
    avoidDeadLock++;
  }
} /* Dlt_Sm_BuildResponseMessage */

/**********************************************************************************************************************
 * Dlt_Sm_ClearBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_ClearBuffer(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Clear all buffers after transmission timeout. */
  Dlt_SendBuffer.Write = 0;
  Dlt_SendBuffer.Read = 0;
  Dlt_ControlSendBuffer.Write = 0;
  Dlt_ControlSendBuffer.Read = 0;
  Dlt_ReceiveBuffer.Write = 0;
  Dlt_ReceiveBuffer.Read = 0;

  for (i = 0; i < (uint32)DLT_MESSAGE_BUFFER_SIZE; i++)
  {
    Dlt_SendBuffer.Buffer[i] = 0;
# if (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE)
    Dlt_SendData[i] = 0;
# endif /* (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE) */
  }
  for (i = 0; i < (uint32)DLT_CONTROL_BUFFER_SIZE; i++)
  {
    Dlt_ControlSendBuffer.Buffer[i] = 0;
    Dlt_ReceiveBuffer.Buffer[i] = 0;
# if (DLT_MESSAGE_BUFFER_SIZE < DLT_CONTROL_BUFFER_SIZE)
    Dlt_SendData[i] = 0;
# endif /* (DLT_MESSAGE_BUFFER_SIZE < DLT_CONTROL_BUFFER_SIZE) */
  }
  for (i = 0; i < (uint32)DLT_MAX_MESSAGE_LENGTH; i++)
  {
    Dlt_ControlSendMessage[i] = 0;
    Dlt_ReceiveMessage[i] = 0;
  }

  Dlt_MessageBufferOverflowHappened = (uint8)FALSE;
} /* Dlt_Sm_ClearBuffer */

/**********************************************************************************************************************
 * Dlt_Sm_SendMsgBufferOverflow
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendMsgBufferOverflow(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 timestamp;
  uint32 ecuID = (uint32)DLT_ECU_ID;
  uint16 index = 0;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement timestamp is enabled, request current time.
   *     Otherwise, set timestamp to zero.
   */
#if defined (DLT_IMPLEMENT_TIMESTAMP)
  timestamp = Dlt_GetCurrentTime();
#else /* defined (DLT_IMPLEMENT_TIMESTAMP) */
  timestamp = 0;
#endif /* defined (DLT_IMPLEMENT_TIMESTAMP) */
  
  /* #-- Build response frame for service MessageBufferOverflow and write it to control buffer. */
  Dlt_Hlp_CopyLongToArrayBigEndian(0x3500001C /* Header type - Version | WTMS | NoSID | WEID | LSBF | UEH - 0b001 1 0 1 0 1, Length - 28Byte */, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(ecuID, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(timestamp, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyIntToArrayBigEndian(0x2602 /* Message info - Non-Verbose | DLT_TYPE_CONTROL | DLT_CONTROL_RESPONSE, 2 Args */, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(0x00000000 /* Application ID - all */, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(0x00000000 /* Context ID - all */, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian(0x14000000 /* Service ID - MessageBufferOverflow */, &Dlt_ControlSendMessage[index], &index);
  Dlt_Hlp_CopyIntToArrayBigEndian(0x0001 /* Status - Message Overflow happened */, &Dlt_ControlSendMessage[index], &index);
  
  Dlt_TxBuf_WriteToCtrlBuf(28);
} /* Dlt_Sm_SendMsgBufferOverflow */

/**********************************************************************************************************************
 * Dlt_Sm_GlobalStateTransition
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_GlobalStateTransition(void)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Update global state.
   *     If changed to DLT_GLOBAL_STATE_OFFLINE:
   *       clear all buffers and set Dlt_CurrentState to DLT_STATE_WAIT_FOR_RECEP.
   */
  if (Dlt_GlobalState != Dlt_GlobalStateNew)
  {
    Dlt_GlobalState = Dlt_GlobalStateNew;

    if (Dlt_GlobalState == DLT_GLOBAL_STATE_OFFLINE)
    {
      Dlt_Sm_ClearBuffer();
      Dlt_CurrentState = DLT_STATE_WAIT_FOR_RECEP;
    }
  }
} /* Dlt_Sm_GlobalStateTransition */

/* ####################################################################################################################
 *     SSSSSS    EEEEEEEEEE  RRRRRRRR    VV      VV    IIIIII      CCCCCC    EEEEEEEEEE
 *   SS      SS  EE          RR      RR  VV      VV      II      CC      CC  EE
 *   SS          EE          RR      RR  VV      VV      II      CC          EE
 *     SSSSSS    EEEEEE      RRRRRRRR    VV      VV      II      CC          EEEEEE
 *           SS  EE          RR    RR      VV  VV        II      CC          EE
 *   SS      SS  EE          RR      RR    VV  VV        II      CC      CC  EE
 *     SSSSSS    EEEEEEEEEE  RR      RR      VV        IIIIII      CCCCCC    EEEEEEEEEE
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_Srv_ParamInValidRange
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Srv_ParamInValidRange(Dlt_OptionType Opt, sint8 Value)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;
  uint8 retVal = DLT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- If log level has to be changed:
   *       Check new value to be a valid log level.
   *     Otherwise, if trace status, verbose mode or another boolean value has to be changed:
   *       Check new value to be a valid boolean value.
   *     Otherwise:
   *       The specified option is invalid.*/
  switch(Opt)
  {
  case(DLT_OPT_LOGLEVEL) :
    if ((Value < (sint8)DLT_LOG_OFF) || 
        (Value > (sint8)DLT_LOG_VERBOSE))
    {
      errorId = DLT_E_WRONG_PARAMETERS;
      retVal = DLT_ERROR;
    }
    break;

  case(DLT_OPT_TRACESTATUS) :
  case(DLT_OPT_VERBOSEMODE) :
  case(DLT_OPT_BOOLEAN) :
    if ((Value < (sint8)FALSE) || 
        (Value > (sint8)TRUE))
    {
      errorId = DLT_E_WRONG_PARAMETERS;
      retVal = DLT_ERROR;
    }
    break;
  
  default:
      errorId = DLT_E_WRONG_PARAMETERS;
      retVal = DLT_ERROR;
    break;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_MAINFUNCTION, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif
  
  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_Srv_DummyService */

/**********************************************************************************************************************
 * Dlt_Srv_FinalizeRespToCtrlBuf
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_FinalizeRespToCtrlBuf(Dlt_OptionType Option,
                                                                    boolean IsGetter, 
                                                                    uint8 NumOfArgs, 
                                                                    uint16 RespLen, 
                                                                    P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Var)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #-- Build response frame depending on received message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(RespLen);
  Dlt_ControlSendMessage[13] = NumOfArgs; /* Number of arguments - 1 */
  Dlt_ControlSendMessage[26] = DLT_OK;

  /* #-- If the received message requested data, return the requested data.
   *     Otherwise, change of configuration is requested so change it.
   */
  if (IsGetter == (boolean)TRUE)
  {
    Dlt_ControlSendMessage[27] = *Var;
  }
  else
  {
    uint16 index = 18;
    /* Get argument after service ID */
    Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);

    if (Dlt_Srv_ParamInValidRange((Dlt_OptionType)Option, (sint8)Dlt_ReceiveMessage[index] ) == DLT_OK)
    {
      *Var = Dlt_ReceiveMessage[index];
    }
    else
    {
      Dlt_ControlSendMessage[26] = DLT_ERROR;
    }
  }
  
  /* #-- Write the response frame to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(RespLen);
} /* Dlt_Srv_FinalizeRespToCtrlBuf */

/**********************************************************************************************************************
 * Dlt_Srv_ServiceNotSupported
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_ServiceNotSupported(uint16 Length, uint8 NumberOfArgs)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 i;

  /* ----- Implementation ----------------------------------------------- */
  /* #-- Build response message depending on received message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(Length);
  Dlt_ControlSendMessage[13] = NumberOfArgs; /* Number of arguments - 1 */
  Dlt_ControlSendMessage[26] = (uint8)DLT_NOT_SUPPORTED; /* Service not supported */

  /* #-- Fill rest of response message with 0 because requested service is not supported. */
  for (i = 27; (i < Length) && (i < (uint16)DLT_MAX_MESSAGE_LENGTH); i++)
  {
    Dlt_ControlSendMessage[i] = 0x00;
  }

  /* #-- Writes response message to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(Length);
} /* Dlt_Srv_ServiceNotSupported */

/**********************************************************************************************************************
 * Dlt_Srv_DummyService
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_DummyService(void) /* 0x0000 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #-- The requested service is not supported by DLT, respond with "DLT_NOT_SUPPORTED".*/
  Dlt_Srv_ServiceNotSupported(27, 1);
} /* Dlt_Srv_DummyService */

/**********************************************************************************************************************
 * Dlt_Srv_SetStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetStatus(Dlt_OptionType Option, sint8 DefaultValue)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 applicationID;
  uint32 contextID;
  uint16 index = 18;
  sint8  new_status;
  uint8  status = DLT_ERROR;
  /* uint32 com_interface; */ /* Not supported today */

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame depending on received message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(27);

  Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);
  
  /* #-- Get Context ID and Application ID of received message, depending on endianness. */
  if (Dlt_IsPayloadInBigEndian())
  { /* Message in big Endian */
    Dlt_Hlp_CopyArrayToLongBigEndian(&index, &applicationID, &Dlt_ReceiveMessage[0]);
    Dlt_Hlp_CopyArrayToLongBigEndian(&index, &contextID, &Dlt_ReceiveMessage[0]);
    /* Dlt_Hlp_CopyArrayToLongBigEndian(&index, &com_interface, &Dlt_ReceiveMessage[0]); */ /* Not supported today */
  }
  else
  { /* Message in little Endian */
    Dlt_Hlp_CopyArrayToLongLittleEndian(&index, &applicationID, &Dlt_ReceiveMessage[0]);
    Dlt_Hlp_CopyArrayToLongLittleEndian(&index, &contextID, &Dlt_ReceiveMessage[0]);
    /* Dlt_Hlp_CopyArrayToLongLittleEndian(&index, &com_interface, &Dlt_ReceiveMessage[0]); */ /* Not supported today */
  }

  /* #-- Get new state from received message. */
  new_status = (sint8) (Dlt_ReceiveMessage[index]);

  /* #-- If the requested state is -1, the option has to be set to default. */
  if (new_status == (sint8)-1)
  {
    new_status = (sint8)DefaultValue;
  }

  /* #-- If the new status is in valid range:
   *       Set the specified option of Application ID - Context ID couple to new status.
   */
  if (Dlt_Srv_ParamInValidRange(Option, (sint8)new_status) == DLT_OK)
  {
    status = Dlt_Hlp_SetOption(applicationID, contextID, Option, (uint8)new_status);
  }

  Dlt_ControlSendMessage[13] = 1; /* Number of arguments */
  Dlt_ControlSendMessage[26] = status; /* Result of option state change. */

  /* #-- Write the response message to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(27);
} /* Dlt_Srv_SetStatus() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Dlt_Srv_SetTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTraceStatus(void) /* 0x0002 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Change trace status for the couple of application ID and context ID, specified in received message. */
  Dlt_Srv_SetStatus((Dlt_OptionType)DLT_OPT_TRACESTATUS, (sint8)Dlt_DefaultTraceStatus);
} /* Dlt_Srv_SetTraceStatus() */

/**********************************************************************************************************************
 * Dlt_Srv_SetLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogLevel(void) /* 0x0001 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Change log level for the couple of application ID and context ID, specified in received message. */
  Dlt_Srv_SetStatus((Dlt_OptionType)DLT_OPT_LOGLEVEL, (sint8)Dlt_DefaultMaxLogLevel);
} /* Dlt_Srv_SetLogLevel() */

/**********************************************************************************************************************
 * Dlt_Srv_GetLogInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogInfo(void) /* 0x0003 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service GetLogInfo is not supported, build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
} /* Dlt_Srv_GetLogInfo */

/**********************************************************************************************************************
 * Dlt_Srv_SetVerboseMode
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetVerboseMode(void) /* 0x0009 */
{
  /* #-- If implement verbose mode is enabled: */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  /* ----- Local Variables ------------------------------------------------ */
  uint32 applicationID;
  uint32 contextID;
  uint16 index = 6;
  uint8  new_verbose_mode;
  uint8  status = DLT_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame depending on received message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(27);

  Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);

  /* #-- Get application ID and context ID from received message. */
  Dlt_Hlp_CopyArrayToLongBigEndian(&index, &applicationID, &Dlt_ReceiveMessage[0]);
  Dlt_Hlp_CopyArrayToLongBigEndian(&index, &contextID, &Dlt_ReceiveMessage[0]);

  index = index + 4; /* Ignore Service ID */

  /* #-- Get new verbose mode from received message. */
  new_verbose_mode = Dlt_ReceiveMessage[index];

  /* #-- If verbose mode is activated, extended header must also be active.
   *     Otherwise, extended header must also be inactive.
   */
  if (Dlt_Srv_ParamInValidRange((Dlt_OptionType)DLT_OPT_VERBOSEMODE, (sint8)new_verbose_mode ) == DLT_OK)
  {
    Dlt_HeaderUseExtendedHeader = new_verbose_mode;

    /* #-- If no Session ID is specified in received message, verbose mode of all DLT users has to be changed. > SWS_Dlt_00489 */
    if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WSID_Mask) == 0)
    {
      applicationID = 0;
      contextID = 0;
    }

    /* #-- Set new verbose mode for the specified Application ID - Context ID couple.  */
    status = Dlt_Hlp_SetOption(applicationID, contextID, (Dlt_OptionType)DLT_OPT_VERBOSEMODE, (uint8)new_verbose_mode);
  }

  Dlt_ControlSendMessage[13] = 1; /* Number of arguments - 1 */
  Dlt_ControlSendMessage[26] = status;

  /* #-- Write the response frame to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(27);
  
# else /* (DLT_IMPLEMENT_VERBOSE_MODE) */
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Otherwise, the service SetVerboseMode is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */
} /* Dlt_Srv_SetVerboseMode() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Dlt_Srv_GetVerboseModeStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetVerboseModeStatus(void) /* 0x0019 */
{
  /* #-- If implement verbose mode is enabled: */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  /* ----- Local Variables ------------------------------------------------ */
  uint32 applicationID;
  uint32 contextID;
  uint16 applIdIdx = 0;
  uint16 conIdIdx = 0;
  uint16 index = 6;
  uint8  status;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame depending on received message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(28);
  Dlt_ControlSendMessage[13] = 2; /* Number of arguments - 1 */

  Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);

  /* #-- Get application ID and context ID from received message. */
  Dlt_Hlp_CopyArrayToLongBigEndian(&index, &applicationID, &Dlt_ReceiveMessage[0]);
  Dlt_Hlp_CopyArrayToLongBigEndian(&index, &contextID, &Dlt_ReceiveMessage[0]);

  /* #-- If no Session ID is specified in received message:
     *       verbose mode of all DLT users has to be changed. > SWS_Dlt_00489 */
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WSID_Mask) == 0)
  {
    applicationID = 0;
    contextID = 0;
  }

  /* #-- If no application ID and context ID is specified:
  *        the global setting of verbose mode is requested. > [SWS_Dlt_00489] */
  if ((applicationID == 0) && (contextID == 0))
  {
    Dlt_ControlSendMessage[27] = Dlt_HeaderUseVerboseMode; /* mode_status */
    status = (uint8)DLT_OK;
  }
  /* #-- Otherwise:
   *       If the specified couple of application ID and context ID is registered:
   *         Set their verbose mode in response message.
   *       Otherwise:
   *         Set "DLT_NOT_SUPPORTED" to response frame. */
  else
  {
    status = Dlt_Hlp_GetApplicationIDIndex(applicationID, &applIdIdx);
    if (status == (uint8)DLT_OK)
    {
      status = Dlt_Hlp_GetContextIDIndex(applIdIdx, contextID, &conIdIdx);
    }
    if (status == (uint8)DLT_OK)
    {
      Dlt_ControlSendMessage[27] = Dlt_Context.app_ids[applIdIdx].context_id_info[conIdIdx].cbk_info.verbose_mode;
    }
  }
  Dlt_ControlSendMessage[26] = status;

  /* #-- Write response message to control buffer. */
  Dlt_TxBuf_WriteToCtrlBuf(28);
# else /* (DLT_IMPLEMENT_VERBOSE_MODE) */
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Otherwise, the service GetVerboseMode is not supported. Build response frame with "DLT_NOT_SUPPORTED".
     */
  Dlt_Srv_ServiceNotSupported(28, 2);
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */
} /* Dlt_Srv_GetVerboseModeStatus() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Dlt_Srv_SetDefaultLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultLogLevel(void) /* 0x0011 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Change default log level according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_LOGLEVEL, (boolean)FALSE, 1, 27, &Dlt_DefaultMaxLogLevel);
} /* Dlt_Srv_SetDefaultLogLevel */

/**********************************************************************************************************************
 * Dlt_Srv_GetDefaultLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultLogLevel(void) /* 0x0004 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame according to received message and return default log level. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_LOGLEVEL, (boolean)TRUE, 2, 28, &Dlt_DefaultMaxLogLevel);
} /* Dlt_Srv_GetDefaultLogLevel */

/**********************************************************************************************************************
 * Dlt_Srv_SetDefaultTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultTraceStatus(void) /* 0x0012 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Change default trace status according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_TRACESTATUS, (boolean)FALSE, 1, 27, &Dlt_DefaultTraceStatus);
} /* Dlt_Srv_SetDefaultTraceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_GetDefaultTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultTraceStatus(void) /* 0x0015 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame according to received message and return default trace status. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_TRACESTATUS, (boolean)TRUE, 2, 28, &Dlt_DefaultTraceStatus);
} /* Dlt_Srv_GetDefaultTraceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_SetUseECUID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseECUID(void) /* 0x000D */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Change flag Dlt_HeaderUseEcuId according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)FALSE, 1, 27, &Dlt_HeaderUseEcuId);
} /* Dlt_Srv_SetUseECUID */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseECUID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseECUID(void) /* 0x001B */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame according to received message and return flag status of Dlt_HeaderUseEcuId. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)TRUE, 2, 28, &Dlt_HeaderUseEcuId);
} /* Dlt_Srv_GetUseECUID */

/**********************************************************************************************************************
 * Dlt_Srv_SetUseSessionID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseSessionID(void) /* 0x000E */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Change flag Dlt_HeaderUseSessionId according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)FALSE, 1, 27, &Dlt_HeaderUseSessionId);
} /* Dlt_Srv_SetUseSessionID */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseSessionID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseSessionID(void) /* 0x001C */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame according to received message and return flag status of Dlt_HeaderUseSessionId. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)TRUE, 2, 28, &Dlt_HeaderUseSessionId);
} /* Dlt_Srv_GetUseSessionID */

/**********************************************************************************************************************
 * Dlt_Srv_UseTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseTimestamp(void) /* 0x000F */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement timestamp is enabled:
   *       Change flag Dlt_HeaderUseTimestamp according to received message.
   *     Otherwise:
   *       The service UseTimestamp is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)FALSE, 1, 27, &Dlt_HeaderUseTimestamp);
# else /* (DLT_IMPLEMENT_TIMESTAMP) */
  Dlt_Srv_ServiceNotSupported(27, 1);
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */
} /* Dlt_Srv_UseTimestamp */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseTimestamp(void) /* 0x001D */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement timestamp is enabled:
   *       Build response frame according to received message and return flag status of Dlt_HeaderUseTimestamp.
   *     Otherwise:
   *       The service GetUseTimestamp is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
#if defined (DLT_IMPLEMENT_TIMESTAMP)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)TRUE, 2, 28, &Dlt_HeaderUseTimestamp);
#else /* (DLT_IMPLEMENT_TIMESTAMP) */
  Dlt_Srv_ServiceNotSupported(28, 2);
#endif /* (DLT_IMPLEMENT_TIMESTAMP) */
} /* Dlt_Srv_GetUseTimestamp */

/**********************************************************************************************************************
 * Dlt_Srv_SetMessageFilterering
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetMessageFilterering(void) /* 0x000A */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement filter messages is enabled:
   *       Change flag Dlt_FilterMessages according to received message.
   *     Otherwise:
   *       The service SetMessageFilterering is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)FALSE, 1, 27, &Dlt_FilterMessages);
# else /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
  Dlt_Srv_ServiceNotSupported(27, 1);
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
} /* Dlt_Srv_SetMessageFilterering */

/**********************************************************************************************************************
 * Dlt_Srv_GetMessageFiltereringStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetMessageFiltereringStatus(void) /* 0x001A */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement filter messages is enabled:
   *       Build response frame according to received message and return flag status of Dlt_FilterMessages.
   *     Otherwise:
   *       The service GetMessageFiltereringStatus is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)TRUE, 2, 28, &Dlt_FilterMessages);
# else /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
  Dlt_Srv_ServiceNotSupported(28, 2);
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
} /* Dlt_Srv_GetMessageFiltereringStatus */

/**********************************************************************************************************************
 * Dlt_Srv_UseExtendedHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseExtendedHeader(void) /* 0x0010 */
{
  /* #-- If implement extended header is enabled: */
# if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
  /* ----- Local Variables ------------------------------------------------ */
#  if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  uint8 locStatus = (uint8)DLT_OK;
  uint16 i;
  uint16 j;
#  endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
  uint16 index = 18;
  
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Prepare response control message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(27);
  Dlt_ControlSendMessage[13] = 1; /* Number of arguments - 1 */
  Dlt_ControlSendMessage[26] = (uint8)DLT_OK;

  Dlt_Hlp_GetIndexDependingOnStandardHeader(&index);
  
  /* #-- Set global flag  */
  Dlt_HeaderUseExtendedHeader = Dlt_ReceiveMessage[index]; /* new_status */

  /* #-- If implement verbose mode is enabled:
   *       If extended header is deactivated:
   *         Verbose mode cannot be supported, so turn it off for all DLT users.
   */
#  if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  if (Dlt_HeaderUseExtendedHeader == (uint8)FALSE)
  {
    for (i = 0; (i < Dlt_Context.count_app_ids); i++)
    {
      for (j = 0; (j < Dlt_Context.app_ids[i].count_context_ids); j++)
      {
        locStatus |= Dlt_Hlp_SetOptionInOneContext(DLT_OPT_VERBOSEMODE, FALSE, i, j);
      }
    }
    
    Dlt_ControlSendMessage[26] = locStatus;
  }
#  endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */

  /* #-- Write response message to control buffer. */
  Dlt_TxBuf_WriteToCtrlBuf(27);
# else /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Otherwise, the service UseExtendedHeader is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
# endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
} /* Dlt_Srv_UseExtendedHeader */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseExtendedHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseExtendedHeader(void) /* 0x001E */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement extended header is enabled:
   *       Build response frame according to received message and return flag status of Dlt_HeaderUseExtendedHeader.
   *     Otherwise:
   *       The service GetUseExtendedHeader is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
#if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)TRUE, 2, 28, &Dlt_HeaderUseExtendedHeader);
#else /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
  Dlt_Srv_ServiceNotSupported(28, 2);
#endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
} /* Dlt_Srv_GetUseExtendedHeader */

/**********************************************************************************************************************
 * Dlt_Srv_SetTimingPackets
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTimingPackets(void) /* 0x000B - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service SetTimingPackets is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
} /* Dlt_Srv_SetTimingPackets */

/**********************************************************************************************************************
 * Dlt_Srv_GetLocalTime
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLocalTime(void) /* 0x000C */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If implement timestamp is enabled:
     *       Build response frame according to received message. No additional payload has to be set because timestamp
     *       is included in standard header.
     *     Otherwise:
     *       The service GetLocalTime is not supported. Build response frame with "DLT_NOT_SUPPORTED".
     */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
  Dlt_TxBuf_PrepareRespInCtrlBuf(27);
  Dlt_ControlSendMessage[13] = 1; /* Number of arguments - 1 */
  Dlt_ControlSendMessage[26] = DLT_OK;
  
  Dlt_TxBuf_WriteToCtrlBuf(27);
# else /* (DLT_IMPLEMENT_TIMESTAMP) */
  Dlt_Srv_ServiceNotSupported(27, 1);
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */
} /* Dlt_Srv_GetLocalTime */

/**********************************************************************************************************************
 * Dlt_Srv_SetComInterfaceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceStatus(void) /* 0x0007 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service SetComInterfaceStatus is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
} /* Dlt_Srv_SetComInterfaceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_GetComInterfaceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceStatus(void) /* 0x0016 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service GetComInterfaceStatus is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(28, 2);
} /* Dlt_Srv_GetComInterfaceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_SetComInterfaceMaxBandwidth
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceMaxBandwidth(void) /* 0x0008 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service SetComInterfaceMaxBandwidth is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
} /* Dlt_Srv_SetComInterfaceMaxBandwidth */

/**********************************************************************************************************************
 * Dlt_Srv_GetComInterfaceMaxBandwidth
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceMaxBandwidth(void) /* 0x0018 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service GetComInterfaceMaxBandwidth is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(0x1F, 2);
} /* Dlt_Srv_GetComInterfaceMaxBandwidth */

/**********************************************************************************************************************
 * Dlt_Srv_GetComInterfaceNames
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceNames(void) /* 0x0017 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service GetComInterfaceNames is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(28, 2);
} /* Dlt_Srv_GetComInterfaceNames */

/**********************************************************************************************************************
 * Dlt_Srv_StoreConfig
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_StoreConfig(void) /* 0x0005 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- The service StoreConfig is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported(27, 1);
} /* Dlt_Srv_StoreConfig */

/**********************************************************************************************************************
 * Dlt_Srv_ResetToFactoryDefault
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_ResetToFactoryDefault(void) /* 0x0006 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i;
  uint32 j;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Prepare response message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(27);
  Dlt_ControlSendMessage[13] = 1; /* Number of arguments - 1 */
  Dlt_ControlSendMessage[26] = DLT_OK;

  /* #-- Reset all global flags to their initial values. */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
  Dlt_FilterMessages          = (uint8)DLT_FILTER_MESSAGES;
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
  Dlt_HeaderUseTimestamp      = (uint8)DLT_HEADER_USE_TIMESTAMP;
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */
# if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
  Dlt_HeaderUseExtendedHeader = (uint8)DLT_HEADER_USE_EXTENDED_HEADER;
# endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  Dlt_HeaderUseVerboseMode    = (uint8)DLT_USE_VERBOSE_MODE;
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */
  Dlt_HeaderUseEcuId          = (uint8)DLT_HEADER_USE_ECU_ID;
  Dlt_HeaderUseSessionId      = (uint8)DLT_HEADER_USE_SESSION_ID;
  Dlt_DefaultMaxLogLevel      = (Dlt_MessageLogLevelType)DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_DefaultTraceStatus      = (uint8)DLT_DEFAULT_TRACE_STATUS;

  /* #-- Iterate over all registered couples of application ID and context ID:
   *       Set default values in context table for log level, trace status and verbose mode.
   *       If available, set default values in SWCs for log level, trace status and verbose mode.
   */
  for (i = 0; i < Dlt_Context.count_app_ids; i++)
  {
    for (j = 0; j < Dlt_Context.app_ids[i].count_context_ids; j++)
    {
      /* Set new trace status in DLT */
      Dlt_Context.app_ids[i].context_id_info[j].trace_status          = (sint8)Dlt_DefaultTraceStatus;
      /* Deviation from SWS_Dlt_00348: using Dlt_DefaultMaxLogLevel instead of factory default because factory default is not supported. */
      Dlt_Context.app_ids[i].context_id_info[j].log_level             = (sint8)Dlt_DefaultMaxLogLevel;
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.verbose_mode = Dlt_HeaderUseVerboseMode;
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */

      if (Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetLogLevel_ != NULL_PTR)
      {
        /* Set factory default log level in SWC */
        (void)Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetLogLevel_(Dlt_Context.app_ids[i].app_id, 
                                                                                  Dlt_Context.app_ids[i].context_id_info[j].context_id, 
                                                                                  Dlt_DefaultMaxLogLevel);
      }
      if (Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetTraceStatus_ != NULL_PTR)
      {
        /* Set factory default trace status in SWC */
        (void)Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetTraceStatus_(Dlt_Context.app_ids[i].app_id, 
                                                                                     Dlt_Context.app_ids[i].context_id_info[j].context_id, 
                                                                                     Dlt_DefaultTraceStatus);
      }
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
      if (Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetVerboseMode_ != NULL_PTR)
      {
        /* Set factory default verbose mode in SWC */
        (void)Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetVerboseMode_(Dlt_Context.app_ids[i].app_id, 
                                                                                     Dlt_Context.app_ids[i].context_id_info[j].context_id, 
                                                                                     Dlt_HeaderUseVerboseMode);
      }
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
    }
  }

  /* #-- Write response message to control buffer. */
  Dlt_TxBuf_WriteToCtrlBuf(27);
} /* Dlt_Srv_ResetToFactoryDefault */

/**********************************************************************************************************************
 * Dlt_Srv_GetSoftwareVersion
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetSoftwareVersion(void) /* 0x0013 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 index = 27;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Prepare response message. */
  Dlt_TxBuf_PrepareRespInCtrlBuf(34);
  Dlt_ControlSendMessage[13] = 3; /* Number of arguments - 3 */
  Dlt_ControlSendMessage[26] = DLT_OK;

  /* #-- Write software version of DLT in response message. Byte order depends on endianness of received message. */
  if (Dlt_IsPayloadInBigEndian())
  {
    Dlt_Hlp_CopyLongToArrayBigEndian((uint32)3, &Dlt_ControlSendMessage[index], &index);
    Dlt_ControlSendMessage[31] = (uint8)DLT_SW_MAJOR_VERSION; /* SW version */
    Dlt_ControlSendMessage[32] = (uint8)DLT_SW_MINOR_VERSION; /* SW version */
    Dlt_ControlSendMessage[33] = (uint8)DLT_SW_PATCH_VERSION; /* SW version */
  }
  else
  {
    Dlt_Hlp_CopyLongToArrayLittleEndian((uint32)3, &Dlt_ControlSendMessage[index], &index);
    Dlt_ControlSendMessage[31] = (uint8)DLT_SW_PATCH_VERSION; /* SW version */
    Dlt_ControlSendMessage[32] = (uint8)DLT_SW_MINOR_VERSION; /* SW version */
    Dlt_ControlSendMessage[33] = (uint8)DLT_SW_MAJOR_VERSION; /* SW version */
  }

  /* #-- Write response message to control buffer. */
  Dlt_TxBuf_WriteToCtrlBuf(34);
} /* Dlt_Srv_GetSoftwareVersion */

/**********************************************************************************************************************
 * Dlt_Srv_MessageBufferOverflow
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_MessageBufferOverflow(void) /* 0x0014 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Build response frame according to received message and return flag status of
   *     Dlt_MessageBufferOverflowHappened.
   */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, (boolean)TRUE, 2, 28, &Dlt_MessageBufferOverflowHappened);
} /* Dlt_Srv_MessageBufferOverflow */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 * Dlt_ComRxIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_ComRxIndication( PduIdType DltRxPduId, Std_ReturnType Result)
{
  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(DltRxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  DLT_DUMMY_STATEMENT(Result); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_ComRxIndication */

/**********************************************************************************************************************
 * Dlt_ComTxConfirmation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_ComTxConfirmation( PduIdType DltTxPduId, Std_ReturnType Result)
{
  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(DltTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #-- Transmission confirmation is only relevant in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    /* #-- If message transmission succeeded, set flag Dlt_MessageTransmitted and reset timeout. */
    if (Result == DLT_OK)
    {
      Dlt_MessageTransmitted = TRUE;
      Dlt_Timeout = 0;
    }
  }
  /*lint -e{438} */ /* PC_Dlt_438 */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Dlt_ComTxConfirmation */

/**********************************************************************************************************************
 * Dlt_ComStartOfReception
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComStartOfReception( P2CONST(PduInfoType, AUTOMATIC, DLT_CONST) info,             /* payload of the first frame and its length */
                                                           PduLengthType TpSduLength,                                   /* The length of the complete N-SDU that has to be received. */
                                                           P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr) /* Available receiving buffer of Dlt */
{
  /* ----- Local Variables ------------------------------------------------ */
  BufReq_ReturnType retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  uint32 freeBuffer;

  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(info); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #-- Reception of messages is only allowed in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

    /* #-- Calculate free space in receive buffer. */
    if (Dlt_ReceiveBuffer.Write >= Dlt_ReceiveBuffer.Read)
    {
      freeBuffer = (uint32)((DLT_CONTROL_BUFFER_SIZE - Dlt_ReceiveBuffer.Write) + Dlt_ReceiveBuffer.Read);
    }
    else /* if (Dlt_ReceiveBuffer.Write < Dlt_ReceiveBuffer.Read) */
    {
      freeBuffer = (uint32)(Dlt_ReceiveBuffer.Read - Dlt_ReceiveBuffer.Write);
    }

    /* #-- If the incoming pdu fits in receive buffer, return free buffer size.
     *     Otherwise, return overflow. */
    if (TpSduLength <= (uint16)freeBuffer)
    {
      retVal = BUFREQ_OK;
      *bufferSizePtr = (uint16)freeBuffer;
    }
    else
    {
      retVal = BUFREQ_E_OVFL;
    }

    /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  }

  /*lint -e{438} */ /* PC_Dlt_438 */
  return retVal;
} /* Dlt_ComStartOfReception */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 * Dlt_ComCopyRxData
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyRxData( P2CONST(PduInfoType, AUTOMATIC, DLT_CONST) info, 
                                                     P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  BufReq_ReturnType retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  uint32 i;
  uint32 bufferIndex = Dlt_ReceiveBuffer.Write;
  uint16 locLength;
  
  /* ----- Implementation ------------------------------------------------- */
  *bufferSizePtr = 0;

  /* #-- Reception of messages is only allowed in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

    /* #-- If no data is passed within SDU, it is the first call of Dlt_ComCopyRxData.
     *       Request the first 4 bytes of received message.
     */
    if ((info->SduLength == 0) &&
        (Dlt_CopyRxDataLength == 0))
    {
      /* Request 4Byte data (Standard Header of DLT frame) */
      *bufferSizePtr = 4;
      Dlt_CopyRxDataCounter = 1;
      retVal = (BufReq_ReturnType)BUFREQ_OK;
    }
    /* #-- Otherwise, if the first four bytes are already requested:
     *       If more than 4 bytes are passed, reject received message.
     *       Otherwise:
     *         Copy received bytes to receive buffer.
     *         If less than 4 byte are received, request the missing byte.
     *         Otherwise:
     *           Get the received message length from received message (byte 2 and 3) and request the missing bytes.
     */
    else if (Dlt_CopyRxDataCounter == 1)
    {
      if ((Dlt_CopyRxDataLength + info->SduLength) > 4)
      {
        retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
        Dlt_CopyRxDataCounter = 0;
        Dlt_CopyRxDataLength = 0;
      }
      else
      {
        for (i = 0; i < info->SduLength; i++)
        {
          Dlt_ReceiveBuffer.Buffer[bufferIndex] = info->SduDataPtr[i];
          bufferIndex++;
          if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
          {
            bufferIndex = 0;
          }
        }
        Dlt_ReceiveBuffer.Write = bufferIndex;
        Dlt_CopyRxDataLength = Dlt_CopyRxDataLength + info->SduLength;

        /* If 4Byte are received, the length of the complete DLT frame is available */
        locLength = Dlt_Hlp_GetLengthOfReceivedMsg(bufferIndex);

        *bufferSizePtr = locLength;
        retVal = (BufReq_ReturnType)BUFREQ_OK;
      }
    }
    /* #-- Otherwise, if the first 4 bytes are successfully received:
     *       If the number of bytes to be passed is greater than requested, reject the message.
     *       Otherwise:
     *         copy remaining/available bytes of received message to receive buffer.
     */
    else if (Dlt_CopyRxDataCounter == 2)
    {
      if (info->SduLength > Dlt_CopyRxDataLength)
      {
        retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
        Dlt_CopyRxDataCounter = 0;
        Dlt_CopyRxDataLength = 0;
      }
      else
      {
        /* Copy Rx Data to Dlt_ReceiveBuffer */
        for (i = 0; i < info->SduLength; i++)
        {
          Dlt_ReceiveBuffer.Buffer[bufferIndex] = info->SduDataPtr[i];
          bufferIndex++;
          if (bufferIndex >= (uint32)DLT_CONTROL_BUFFER_SIZE)
          {
            bufferIndex = 0;
          }
        }
        Dlt_ReceiveBuffer.Write = bufferIndex;
        Dlt_CopyRxDataLength = Dlt_CopyRxDataLength - info->SduLength;

        if (Dlt_CopyRxDataLength == 0)
        { /* The complete DLT message is received */
          Dlt_RxIndicationOccured++;
          Dlt_CopyRxDataCounter = 1;
          *bufferSizePtr = 4;
        }
        else
        {
          *bufferSizePtr = Dlt_CopyRxDataLength;
        }
        retVal = (BufReq_ReturnType)BUFREQ_OK;
      }
    }
    /* #-- Otherwise, reject received message. */
    else
    {
      retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
      Dlt_CopyRxDataCounter = 0;
      Dlt_CopyRxDataLength = 0;
    }

    /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  }
  
  return retVal;
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */
  /* PRQA S 6080 2 */ /* MD_MSR_STMIF */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Dlt_ComCopyRxData */ 

/**********************************************************************************************************************
 * Dlt_ComCopyTxData
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyTxData( P2CONST(PduInfoType, AUTOMATIC, DLT_CONST) info, 
                                                     P2CONST(RetryInfoType, AUTOMATIC, DLT_CONST) retry, 
                                                     P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) availableDataPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  BufReq_ReturnType retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  
  /* ----- Implementation ------------------------------------------------- */
  /* retry is not supported by Vector. */
  DLT_DUMMY_STATEMENT(retry); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #-- Transmission of messages is only allowed in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

    /* #-- If the requested number of bytes to be copied is zero:
     *       Return size of send message.
     */
    if (info->SduLength == 0)
    {
      /* Number of remaining bytes of the current tx object */
      *availableDataPtr = Dlt_SendPdu.SduLength;
      retVal = BUFREQ_OK;
    }
    /* #-- Otherwise:
     *       If size of send message is greater or equal as requested:
     *         Copy as many bytes of send message as requested.
     */
    else
    {
      if (info->SduLength <= Dlt_SendPdu.SduLength)
      {
        uint16 i;

        for (i = 0; i < info->SduLength; i++)
        {
          info->SduDataPtr[i] = Dlt_SendPdu.SduDataPtr[i]; /* Points to the event that will be send */
        }
        Dlt_SendPdu.SduLength = Dlt_SendPdu.SduLength - info->SduLength;

        if (Dlt_SendPdu.SduLength > (uint16)DLT_TCPIP_MAX_TX_BUFFER)
        {
          *availableDataPtr = (uint16)DLT_TCPIP_MAX_TX_BUFFER - info->SduLength;
        }
        else
        {
          *availableDataPtr = Dlt_SendPdu.SduLength;
        }
        Dlt_SendPdu.SduDataPtr = &Dlt_SendPdu.SduDataPtr[info->SduLength];

        retVal = BUFREQ_OK;

#if (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE) 
        if (Dlt_SendPdu.SduLength > DLT_MESSAGE_BUFFER_SIZE)
#else /* (DLT_MESSAGE_BUFFER_SIZE < DLT_CONTROL_BUFFER_SIZE) */
        if (Dlt_SendPdu.SduLength > DLT_CONTROL_BUFFER_SIZE)
#endif /* (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE) */
        {
          retVal = BUFREQ_E_NOT_OK;
        }
      }
    }

    /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  }

  /*lint -e{438} */ /* PC_Dlt_438 */
  return retVal;
} /* Dlt_ComCopyTxData */

/**********************************************************************************************************************
 * Dlt_ComSoConModeChg
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_ComSoConModeChg(Dlt_SoConModeType Mode)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
  DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

  /* #-- Stores the last socket connection mode and sets new state active. */
  Dlt_SoConLastMode = Dlt_SoConMode;
  Dlt_SoConMode = Mode;

  /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
  DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
} /* Dlt_ComSoConModeChg */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 * Dlt_InitMemory
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_InitMemory(void)
{
  /* #-- If com layer is AUTOSAR, pre-initialize global variables. */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  uint32 i;
  uint32 j;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Initialize all global variables which have to be available before initialization of DLT module. */
  Dlt_SendBuffer.Write = 0;
  Dlt_SendBuffer.Read = 0;

  Dlt_CurrentState = DLT_STATE_UNINIT;

  for (i = 0; i < DLT_MESSAGE_BUFFER_SIZE; i++)
  {
    Dlt_SendBuffer.Buffer[i] = 0;
# if (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE)
    Dlt_SendData[i] = 0;
# endif /* (DLT_MESSAGE_BUFFER_SIZE >= DLT_CONTROL_BUFFER_SIZE) */
  }
# if (DLT_MESSAGE_BUFFER_SIZE < DLT_CONTROL_BUFFER_SIZE)
  for (i = 0; i < DLT_CONTROL_BUFFER_SIZE; i++)
  {
    Dlt_SendData[i] = 0;
  }
# endif /* (DLT_MESSAGE_BUFFER_SIZE < DLT_CONTROL_BUFFER_SIZE) */

  /* Variables for Dlt runtime configuration */
# if defined (DLT_IMPLEMENT_FILTER_MESSAGES)
  Dlt_FilterMessages                     = (uint8)DLT_FILTER_MESSAGES;
# endif /* (DLT_IMPLEMENT_FILTER_MESSAGES) */
# if defined (DLT_IMPLEMENT_TIMESTAMP)
  Dlt_HeaderUseTimestamp                 = (uint8)DLT_HEADER_USE_TIMESTAMP;
# endif /* (DLT_IMPLEMENT_TIMESTAMP) */
# if defined (DLT_IMPLEMENT_EXTENDED_HEADER)
  Dlt_HeaderUseExtendedHeader            = (uint8)DLT_HEADER_USE_EXTENDED_HEADER;
# endif /* (DLT_IMPLEMENT_EXTENDED_HEADER) */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  Dlt_HeaderUseVerboseMode               = (uint8)DLT_USE_VERBOSE_MODE;
# endif /* (DLT_IMPLEMENT_VERBOSE_MODE) */

  Dlt_HeaderUseEcuId                     = (uint8) DLT_HEADER_USE_ECU_ID;
  Dlt_HeaderUseSessionId                 = (uint8) DLT_HEADER_USE_SESSION_ID;
  Dlt_DefaultMaxLogLevel                 = (uint8) DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_DefaultTraceStatus                 = (uint8) DLT_DEFAULT_TRACE_STATUS;

  Dlt_MessageBufferOverflowHappened      = (uint8) FALSE;
  Dlt_MessageCounter                     = 0;
  
  /* #-- Init Dlt context table:
   *       Initialize all contexts with default values.
   *       Add pre-defined DEM context.
   *       Add pre-defined DET context.
   */
  Dlt_Context.count_app_ids              = 0;

  for (i = 0; i < DLT_MAX_COUNT_APP_IDS; i++)
  {
    Dlt_Context.app_ids[i].app_id = 0;
    Dlt_Context.app_ids[i].app_description = NULL_PTR;
    Dlt_Context.app_ids[i].len_app_description = 0;
    Dlt_Context.app_ids[i].count_context_ids = 0;

    for (j = 0; j < DLT_MAX_COUNT_CONTEXT_IDS_PER_APP_ID; j++)
    {
      Dlt_Context.app_ids[i].context_id_info[j].context_id = (Dlt_ContextIDType)0;
      Dlt_Context.app_ids[i].context_id_info[j].context_description = (P2CONST(uint8, AUTOMATIC, DLT_CONST))NULL_PTR;
      Dlt_Context.app_ids[i].context_id_info[j].len_context_description = (uint8)0;
      Dlt_Context.app_ids[i].context_id_info[j].log_level = (sint8)Dlt_DefaultMaxLogLevel;
      Dlt_Context.app_ids[i].context_id_info[j].trace_status = (sint8)Dlt_DefaultTraceStatus;
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.session_id = (Dlt_SessionIDType)0;
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetLogLevel_ = NULL_PTR;
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetTraceStatus_ = NULL_PTR;
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.Dlt_SetVerboseMode_ = NULL_PTR;
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.verbose_mode = Dlt_HeaderUseVerboseMode;
# else
      Dlt_Context.app_ids[i].context_id_info[j].cbk_info.verbose_mode = (uint8)DLT_OFF;
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
    }
  }

  /* Add DEM context entry */
  Dlt_Context.count_app_ids++;
  Dlt_Context.app_ids[0].app_id = (Dlt_ApplicationIDType)Dlt_Dem_ApplId; /* DEM  */
  Dlt_Context.app_ids[0].app_description = (P2CONST(uint8, AUTOMATIC, DLT_CONST))Dlt_Dem_ApplId_Desc; /* DEM */
  Dlt_Context.app_ids[0].len_app_description = (uint8)DLT_DEM_APPLID_DESC_LENGTH;
  Dlt_Context.app_ids[0].count_context_ids = 1;
  Dlt_Context.app_ids[0].context_id_info[0].context_id = (Dlt_ContextIDType)Dlt_Dem_ContextId; /* STD0  */
  Dlt_Context.app_ids[0].context_id_info[0].context_description = (P2CONST(uint8, AUTOMATIC, DLT_CONST))Dlt_Dem_ContextId_Desc; /* STD0 */
  Dlt_Context.app_ids[0].context_id_info[0].len_context_description = (uint8)DLT_DEM_CONTEXTID_DESC_LENGTH;
  Dlt_Context.app_ids[0].context_id_info[0].log_level = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_Context.app_ids[0].context_id_info[0].trace_status = (sint8)DLT_DEFAULT_TRACE_STATUS;
  Dlt_Context.app_ids[0].context_id_info[0].cbk_info.session_id = (Dlt_SessionIDType)0x36; /* Dem module ID */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  Dlt_Context.app_ids[0].context_id_info[0].cbk_info.verbose_mode = Dlt_HeaderUseVerboseMode;
# else
  Dlt_Context.app_ids[0].context_id_info[0].cbk_info.verbose_mode = (uint8)0;
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
  Dlt_Context.app_ids[0].context_id_info[0].cbk_info.Dlt_SetVerboseMode_ = NULL_PTR;

  Dlt_Context.app_ids[0].context_id_info[0].cbk_info.Dlt_SetLogLevel_ = NULL_PTR;
  Dlt_Context.app_ids[0].context_id_info[0].cbk_info.Dlt_SetTraceStatus_ = NULL_PTR;

  /* Add DET context entry */
  Dlt_Context.count_app_ids++;
  Dlt_Context.app_ids[1].app_id = (Dlt_ApplicationIDType)Dlt_Det_ApplId; /* DET */
  Dlt_Context.app_ids[1].app_description = (P2CONST(uint8, AUTOMATIC, DLT_CONST))Dlt_Det_ApplId_Desc; /* DET */
  Dlt_Context.app_ids[1].len_app_description = (uint8)DLT_DET_APPLID_DESC_LENGTH;
  Dlt_Context.app_ids[1].count_context_ids = 1;
  Dlt_Context.app_ids[1].context_id_info[0].context_id = (Dlt_ContextIDType)Dlt_Det_ContextId; /* STD1 */
  Dlt_Context.app_ids[1].context_id_info[0].context_description = (P2CONST(uint8, AUTOMATIC, DLT_CONST))Dlt_Det_ContextId_Desc; /* STD1 */
  Dlt_Context.app_ids[1].context_id_info[0].len_context_description = (uint8)DLT_DET_CONTEXTID_DESC_LENGTH;
  Dlt_Context.app_ids[1].context_id_info[0].log_level = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_Context.app_ids[1].context_id_info[0].trace_status = (sint8)DLT_DEFAULT_TRACE_STATUS;
  Dlt_Context.app_ids[1].context_id_info[0].cbk_info.session_id = (Dlt_SessionIDType)0x0F; /* Det module ID */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
  Dlt_Context.app_ids[1].context_id_info[0].cbk_info.verbose_mode = Dlt_HeaderUseVerboseMode;
# else /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
  Dlt_Context.app_ids[1].context_id_info[0].cbk_info.verbose_mode = (uint8)0;
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
  Dlt_Context.app_ids[1].context_id_info[0].cbk_info.Dlt_SetVerboseMode_ = NULL_PTR;
  Dlt_Context.app_ids[1].context_id_info[0].cbk_info.Dlt_SetLogLevel_ = NULL_PTR;
  Dlt_Context.app_ids[1].context_id_info[0].cbk_info.Dlt_SetTraceStatus_ = NULL_PTR;

  Dlt_NumberOfRegisteredContextIds = 2;

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* #-- Set the global state to DLT_GLOBAL_STATE_UNINIT, now the logging of DEM and DET messages is active. */
  Dlt_GlobalState = DLT_GLOBAL_STATE_UNINIT;
  Dlt_GlobalStateNew = DLT_GLOBAL_STATE_UNINIT;
} /* Dlt_InitMemory */

/**********************************************************************************************************************
 * Dlt_Init
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_Init( P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_PBCFG) ConfigPtr)
{
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  uint16 i;
#endif

  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #-- Initialize all global variables which have to be available with DLT module initialization. */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_ControlSendBuffer.Write = 0;
  Dlt_ControlSendBuffer.Read = 0;
  Dlt_ReceiveBuffer.Write = 0;
  Dlt_ReceiveBuffer.Read = 0;
  
  for (i = 0; i < (uint16)DLT_CONTROL_BUFFER_SIZE; i++)
  {
    Dlt_ControlSendBuffer.Buffer[i] = 0;
    Dlt_ReceiveBuffer.Buffer[i] = 0;
  }

  for (i = 0; i < (uint16)DLT_MAX_MESSAGE_LENGTH; i++)
  {
    Dlt_ControlSendMessage[i] = 0;
    Dlt_ReceiveMessage[i] = 0;
    Dlt_TransmitMessage[i] = 0;
  }

  Dlt_CopyRxDataCounter = 0;
  Dlt_CopyRxDataLength = 0;

  Dlt_MessageTransmitted                 = (uint8) FALSE;
  Dlt_SendingOrderAccepted               = (uint8) DLT_NOT_OK;
  Dlt_RxIndicationOccured                = 0;
  Dlt_Timeout                            = 0;
  Dlt_SoConMode                          = (uint8)DLT_SOCON_OFFLINE;
  Dlt_SoConLastMode                      = (uint8)DLT_SOCON_RECONNECT;
  
  /* Initialize DltCom */
  DltCom_Init();

  Dlt_Sm_StateTransition(kDltEventInitDlt);
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#if (DLT_USE_COMLAYER_XCP == STD_ON)
  Dlt_DetErrorCode = 0;
  Dlt_DemEventStatus = 0;
  Dlt_NonVerboseMessageId = 0;
#endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */

  /* #-- Set the global state to DLT_GLOBAL_STATE_OFFLINE, now the logging as well as sending is disabled. */
  Dlt_GlobalState = DLT_GLOBAL_STATE_OFFLINE;
  Dlt_GlobalStateNew = DLT_GLOBAL_STATE_OFFLINE;
  /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_Init */

#if (DLT_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 * Dlt_GetVersionInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_VAR) VersionInfo)
{ /* [Dlt271] */
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  {
    VersionInfo -> vendorID         = DLT_VENDOR_ID;
    VersionInfo -> moduleID         = DLT_MODULE_ID;
    VersionInfo -> sw_major_version = DLT_SW_MAJOR_VERSION;
    VersionInfo -> sw_minor_version = DLT_SW_MINOR_VERSION;
    VersionInfo -> sw_patch_version = DLT_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_GETVERSIONINFO, errorId);
  }
# else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

  /*lint -e{550} */ /* PC_Dlt_550 */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Dlt_GetVersionInfo */
#endif /* (DLT_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 * Dlt_DetForwardErrorTrace
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_DetForwardErrorTrace( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{ 
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_FilterInfoType filterInfo;
  uint8  locPayload[21]; /* 21 == Max payload size */
  uint16 payloadLength;
  uint16 index = 0;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Forwarding of DET messages is only allowed before initialization and after DLT activation. */
  if ((Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE) ||
      (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT))
  {
    /* Dlt430, Dlt432 */
    /* #-- Prevent recursion in case DLT throws a DET error itself. */
    if (ModuleId != DLT_MODULE_ID)
    {
      /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

      /* #-- If use comlayer xcp enabled, build log message for DET error reporting. */
#if (DLT_USE_COMLAYER_XCP == STD_ON)
      if((uint8)ModuleId == Dlt_ModuleTable[(uint8)ModuleId])
      {  
        Dlt_DetErrorCode = (uint8)InstanceId; /* ESCAN00070451: necessary for components with same module ids */
        Dlt_DetErrorCode <<= 8;
        Dlt_DetErrorCode |= (uint8)ModuleId; /* ESCAN00074824 */
        Dlt_DetErrorCode <<= 8;
        Dlt_DetErrorCode |= (uint8)ApiId;
        Dlt_DetErrorCode <<= 8;
        Dlt_DetErrorCode |= (uint8)ErrorId;
      }
      else
      {
        /* [Dlt439]: detected invalid module ID are not reported to DET to prevent a loop. Instead an invalid module ID is reported to the XCP master via Dlt_DetErrorCode  */
        Dlt_DetErrorCode = (uint32)(0x01000000 | ModuleId);
      }
    
      /* #-- Send DLT-DET-message via XCP. */
      (void)Dlt_TransmitData(DLT_DET_EVENT, &Dlt_DetErrorCode, 4);
#endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

      /* #-- Otherwise, if implement verbose mode is enabled and active, build payload with type info for each argument.
	     *     Otherwise, build payload with message ID and all arguments.
	     */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
      if (Dlt_HeaderUseVerboseMode == (uint8)TRUE)
      {
        filterInfo.log_info.options = DLT_VERBOSE_MSG;
        payloadLength = 21;
        Dlt_Hlp_CopyLongToArray((uint32)0x00000042, &locPayload[index], &index); /* Type Info - ModuleId - Unsigned int, 16 bit */
        Dlt_Hlp_CopyIntToArray(ModuleId, &locPayload[index], &index);            /* ModuleId */
        Dlt_Hlp_CopyLongToArray((uint32)0x00000041, &locPayload[index], &index); /* Type Info - InstanceId - Unsigned int, 8 bit */
        locPayload[10] = (uint8)(InstanceId);                                    /* InstanceId */
        index++;
        Dlt_Hlp_CopyLongToArray((uint32)0x00000041, &locPayload[index], &index); /* Type Info - ApiId - Unsigned int, 8 bit */
        locPayload[15] = (uint8)(ApiId);                                         /* ApiId */
        index++;
        Dlt_Hlp_CopyLongToArray((uint32)0x00000041, &locPayload[index], &index); /* Type Info - ErrorId - Unsigned int, 8 bit */
        locPayload[20] = (uint8)(ErrorId);                                       /* ErrorId */
      }
      else
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
      {
        filterInfo.log_info.options = DLT_NON_VERBOSE_MSG;
        payloadLength = 9;
        Dlt_Hlp_CopyLongToArray((uint32)0x00000002, &locPayload[index], &index); /* Message Id */
        Dlt_Hlp_CopyIntToArray(ModuleId, &locPayload[index], &index);            /* ModuleId */
        locPayload[6] = (uint8)(InstanceId);                                     /* InstanceId */
        locPayload[7] = (uint8)(ApiId);                                          /* ApiId */
        locPayload[8] = (uint8)(ErrorId);                                        /* ErrorId */
      }

      /* #-- Set valid filter information to avoid message rejection. */
      filterInfo.session_id = (Dlt_SessionIDType) 0x0F;
      filterInfo.message_type = (Dlt_MessageTypeType) DLT_TYPE_LOG;
      filterInfo.log_info.app_id = (Dlt_ApplicationIDType) Dlt_Context.app_ids[1].app_id; /* DET application ID */
      filterInfo.log_info.context_id = (Dlt_ContextIDType) Dlt_Context.app_ids[1].context_id_info[0].context_id; /* DET context ID */
      filterInfo.log_info.arg_count = 4;
      filterInfo.log_info.log_level = (uint8)Dlt_Context.app_ids[1].context_id_info[0].log_level;

      /* #-- Build message, validate it and write it to send buffer. */
      (void)Dlt_TxMsg_Transmit(&filterInfo, payloadLength, locPayload);

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

      /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    }
  }
} /* Dlt_DetForwardErrorTrace */

/**********************************************************************************************************************
 * Dlt_DemTriggerOnEventStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_DemTriggerOnEventStatus(Dem_EventIdType EventId,
                                                 Dem_EventStatusExtendedType EventStatusOld,
                                                 Dem_EventStatusExtendedType EventStatusNew)
{ /* [Dlt474] */
  /* ----- Local Variables ------------------------------------------------ */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_FilterInfoType filterInfo;
  uint16 payloadLength;
  uint16 index = 0;
  uint8  locPayload[11]; /* 11 == Max payload size */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  /* #-- If required, check that event ID is in valid range. */
  /* DEM Event ID = 0 indicates no event or failure. Highest valid event ID is DLT_NUM_DEM_EVENTS. */
  if ((EventId == 0) || (EventId > DLT_NUM_DEM_EVENTS))
  {
    errorId = DLT_E_WRONG_PARAMETERS;
  }
  else
#endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  /* #-- If the initial check is passed or not required: */
  {
    /* #-- DEM events are only allowed to be send before initialization and after DLT activation. */
    if ((Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE) ||
        (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT))
    {
      /* [Dlt475]: the DEM already ensures that EventStatusOld and EventStatusNew are not equal, because Dlt_DemTriggerOnEventStatus is only called if they differ. Hence the log message can always be generated. */
      /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

      /* #-- If DEM event filtering is enabled, call callback function to add individual message filter. */
#if (DLT_DEM_EVENT_FILTERING_ENABLED == STD_ON)
      /* UREQ00041311: The implementation shall be able to filter for the following properties: DEM Event ID and DEM Event Status Bits */
      if (Appl_DltDemEventFilterCbk(EventId, EventStatusOld, EventStatusNew) == TRUE)
#else
      DLT_DUMMY_STATEMENT(EventStatusOld); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */ /* PC_Dlt_438 */
#endif
      {
        /* #-- If comlayer XCP is active, send DEM event via XCP. */
#if (DLT_USE_COMLAYER_XCP == STD_ON)
        Dlt_DemEventStatus = (uint16)EventId;
        Dlt_DemEventStatus <<= 16;
        Dlt_DemEventStatus |= (uint8)EventStatusNew;

        /* UREQ00040542: Report DEM Event change */
        (void)Dlt_TransmitData(DLT_DEM_EVENT, &Dlt_DemEventStatus, 4);
#endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
        /* #-- Otherwise:
         *       If implement verbose mode is enabled and active, build payload with according type info for each argument.
         *       Otherwise, build payload with message ID and all arguments.
         */
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
        if (Dlt_HeaderUseVerboseMode == (uint8)TRUE)
        {
          /* Payload has to be modified, add Type Info for each argument */
          filterInfo.log_info.options = DLT_VERBOSE_MSG;
          payloadLength = 11;
          Dlt_Hlp_CopyLongToArray((uint32)0x00000042, &locPayload[index], &index); /* Type Info - EventId - Unsigned int, 16 bit */
          Dlt_Hlp_CopyIntToArray((uint16)EventId, &locPayload[index], &index); /* EventId */
          Dlt_Hlp_CopyLongToArray((uint32)0x00000041, &locPayload[index], &index); /* Type Info - EventStatusNew - Unsigned int, 8 bit */
          locPayload[10] = (uint8)(EventStatusNew); /* EventStatusNew */
        }
        else
# endif /* defined (DLT_IMPLEMENT_VERBOSE_MODE) */
        {
          filterInfo.log_info.options = DLT_NON_VERBOSE_MSG;
          payloadLength = 7;
          Dlt_Hlp_CopyLongToArray((uint32)0x00000001, &locPayload[index], &index); /* Message ID */
          Dlt_Hlp_CopyIntToArray((uint16)EventId, &locPayload[index], &index); /* EventId */
          locPayload[6] = (uint8)(EventStatusNew); /* EventStatusNew */
        }

        /* #-- Set filter options accoring to DEM events. */
        filterInfo.session_id = (Dlt_SessionIDType) 0x36; /* Session ID == DEM Module ID */
        filterInfo.message_type = (Dlt_MessageTypeType)DLT_TYPE_LOG;
        filterInfo.log_info.arg_count = 2;
        filterInfo.log_info.app_id = (Dlt_ApplicationIDType)Dlt_Context.app_ids[0].app_id; /* Application ID == "DEM" */
        filterInfo.log_info.context_id = (Dlt_ContextIDType)Dlt_Context.app_ids[0].context_id_info[0].context_id; /* Context ID == "STD0" */
        filterInfo.log_info.log_level = (Rte_DT_Dlt_MessageLogInfoType_1)Dlt_Context.app_ids[0].context_id_info[0].log_level; /* Log level of DEM messages */

        /* #-- Build the DLT-DEM-Message and write it to send buffer. */
        (void)Dlt_TxMsg_Transmit(&filterInfo, payloadLength, locPayload);

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
      }

      /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_DEMTRIGGERONEVENTSTATUS, errorId);
  }
#else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */ /* PC_Dlt_438 */
#endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

  /*lint -e{438} */ /* PC_Dlt_438 */
  /*lint -e{550} */ /* PC_Dlt_550 */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Dlt_DemTriggerOnEventStatus */

/**********************************************************************************************************************
 * Dlt_SendLogMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SendLogMessage(Dlt_SessionIDType parg0, 
                                                  P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_CONST) LogInfo, 
                                                  P2CONST(uint8, AUTOMATIC, DLT_CONST) LogData, 
                                                  uint16 LogDataLength) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{ /* Dlt241 */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = DLT_E_ERROR_UNKNOWN;
  uint8 errorId = DLT_E_NO_ERROR;
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_FilterInfoType filterInfo;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* ----- Development Error Checks --------------------------------------- */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if (LogData == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
#endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */

  /* #-- If the comlayer ASR is active: */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  else if (LogInfo == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #-- If global state is DLT_GLOBAL_STATE_ONLINE, set filter options, check the message's validity and write it
     *     to the send buffer. 
     */
    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
      /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

      filterInfo.message_type = (Dlt_MessageTypeType)DLT_TYPE_LOG;
      filterInfo.session_id = parg0;
      filterInfo.log_info.app_id = LogInfo->app_id;
      filterInfo.log_info.context_id = LogInfo->context_id;
      filterInfo.log_info.arg_count = LogInfo->arg_count;
      filterInfo.log_info.options = LogInfo->options;
      filterInfo.log_info.log_level = LogInfo->log_level;

      retVal = Dlt_TxMsg_Transmit(&filterInfo, LogDataLength, LogData);

      /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    }
  }

#else /* (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
  /* #-- Otherwise: */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  else
# endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  {
    DLT_DUMMY_STATEMENT(parg0); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

    /* #-- If global state is DLT_GLOBAL_STATE_ONLINE: */
    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
      /* SWS_Dlt_00014 */
      /* #-- If message is in verbose mode AND
       *       If implement verbose mode is enabled, build and send the log message with string payload.
       *       Otherwise, invalid request. */
      if ((LogInfo->options & DLT_VERBOSE_MSG) != FALSE)
      {
# if (DLT_USE_VERBOSE_MODE == STD_ON)
        /* verbose message handling */
        if ((LogDataLength < DLT_MAX_MESSAGE_LENGTH))
        {
          /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
          DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

          VStdMemCpy(&Dlt_VerboseMessageData[0], &LogData[sizeof(Dlt_StandardHeaderType)+sizeof(Dlt_MessageIDType)], LogDataLength);

          /* SREQ00024774: If the string written to the data buffer is shorter than the defined data field it shall be terminated with 0. */
          Dlt_VerboseMessageData[LogDataLength] = '\0';

          /* UREQ00040541: generate verbose log message */
          if ((Dlt_TransmitData(DLT_VERBOSE_MSG_EVENT, (P2CONST(uint8, AUTOMATIC, DLT_CONST))&LogData, (uint16)LogDataLength)) != (XCP_EVENT_NOP))
          {
            retVal = (Dlt_ReturnType)(DLT_E_OK);
          }
          else
          {
            retVal = (Dlt_ReturnType)(DLT_E_ERROR_UNKNOWN);
          }

          /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0. */
          DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        }
        else
        {
#  if (DLT_DEV_ERROR_DETECT == STD_ON)
          errorId = DLT_E_WRONG_PARAMETERS;
#  endif
          retVal = (Dlt_ReturnType)DLT_E_MSG_TOO_LARGE;
        }
# else /* (DLT_USE_VERBOSE_MODE == STD_ON) */
        DLT_DUMMY_STATEMENT(LogDataLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */ /* PC_Dlt_438 */
        /* SWS_Dlt_00090 */
        retVal = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
# endif /* (DLT_USE_VERBOSE_MODE == STD_ON) */
      }
      /* #-- Otherwise, build the log message with message ID and send it via XCP. */
      else
      {
        /* non-verbose message handling */
        uint32 msgId = 0;
        uint16 index = 4;
        Dlt_Hlp_CopyArrayToLongLittleEndian(&index, &msgId, &LogData[0]); /* ESCAN00081868 */
        /* Dlt031: Message IDs used for Dem (0x00000001) and Det (0x00000002) are reserved and not usable for SW-Cs. */
        /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
        /* #-- If message ID in invalid range, reject send request.
         *     Otherwise, send log message via XCP. */
        if ((msgId <= 2) || (msgId >= DLT_NUM_NON_VERBOSE_MESSAGES))
        {
          errorId = DLT_E_WRONG_PARAMETERS;
          retVal = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
        }
        else
        {
          /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0 */
          DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
          
          Dlt_NonVerboseMessageId = (Dlt_MessageIDType)(msgId);

          if ((Dlt_TransmitData(DLT_NON_VERBOSE_MSG_EVENT, (P2CONST(uint8, AUTOMATIC, DLT_CONST))&LogData, (uint16)LogDataLength)) != (XCP_EVENT_NOP))
          {
            retVal = (Dlt_ReturnType)(DLT_E_OK);
          }
          else
          {
            retVal = (Dlt_ReturnType)(DLT_E_ERROR_UNKNOWN);
          }

          /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0 */
          DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        }
      }
    }
  }
#endif /* (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#if (DLT_DEV_ERROR_REPORT == STD_ON)
  /* ----- Development Error Report --------------------------------------- */
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_SENDLOGMESSAGE, errorId);
  }
#else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */ /* PC_Dlt_438 */
#endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

  /*lint -e{438} */ /* PC_Dlt_438 */
  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */

  /* PRQA S 2006 2 */ /* MD_MSR_14.7 */
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Dlt_SendLogMessage */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 * Dlt_SendTraceMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SendTraceMessage(Dlt_SessionIDType parg0, 
                                                    P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_CONST) TraceInfo, 
                                                    P2CONST(uint8, AUTOMATIC, DLT_CONST) TraceData, 
                                                    uint16 TraceDataLength) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{ /* Dlt241 */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
  Dlt_FilterInfoType filterInfo;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if (TraceData == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else if (TraceInfo == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #-- If DLT is DLT_GLOBAL_STATE_ONLINE:
     *       If requested message is of type DLT_TYPE_APP_TRACE or DLT_TYPE_NW_TRACE:
     *         Write the trace message in the send buffer. */
    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
      /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
      DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

      if ((TraceInfo->options & DLT_TYPE_APP_TRACE) == DLT_TYPE_APP_TRACE)
      {
        filterInfo.message_type = (Dlt_MessageTypeType)DLT_TYPE_APP_TRACE;
        retVal = (Dlt_ReturnType)DLT_E_OK;
      }
      else if ((TraceInfo->options & DLT_TYPE_NW_TRACE) == DLT_TYPE_NW_TRACE)
      {
        filterInfo.message_type = (Dlt_MessageTypeType)DLT_TYPE_NW_TRACE;
        retVal = (Dlt_ReturnType)DLT_E_OK;
      }
      else
      {
        /* Do nothing */
      }
  
      if (retVal == (Dlt_ReturnType)DLT_E_OK)
      {
        filterInfo.session_id = parg0;
        filterInfo.trace_info.app_id = TraceInfo->app_id;
        filterInfo.trace_info.context_id = TraceInfo->context_id;
        filterInfo.trace_info.options = TraceInfo->options;
        filterInfo.trace_info.trace_info = TraceInfo->trace_info;
        retVal = Dlt_TxMsg_Transmit(&filterInfo, TraceDataLength, TraceData);
      }

      /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0 */
      DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_SENDTRACEMESSAGE, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */

  /* PRQA S 2006 2 */ /* MD_MSR_14.7 */
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Dlt_SendTraceMessage */

/**********************************************************************************************************************
 * Dlt_RegisterContext
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_RegisterContext( Dlt_SessionIDType parg0, 
                                                    Dlt_ApplicationIDType AppId, 
                                                    Dlt_ContextIDType ContextId, 
                                                    P2CONST(uint8, AUTOMATIC, DLT_CONST) AppDescription, 
                                                    uint8 LenAppDescription, 
                                                    P2CONST(uint8, AUTOMATIC, DLT_CONST) ContextDescription, 
                                                    uint8 LenContextDescription
                                                  ) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{ /* Dlt241 */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal;
  boolean applicationAlreadyRegistered = (boolean)FALSE;
  boolean contextAlreadyRegistered;
  uint16 AppIdIndex;
  uint16 ConIdIndex;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
  DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

  /* #-- Check if the new Context already exists */
  contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(AppId, ContextId, &AppIdIndex, &ConIdIndex, &applicationAlreadyRegistered);

  /* #-- If application ID is not registered, set application ID index to next free slot in context table. */
  if (applicationAlreadyRegistered == (boolean)FALSE)
  {
    AppIdIndex = Dlt_Context.count_app_ids;
  }
  /* #-- If context is already registered, reject new registration. */
  if (contextAlreadyRegistered == (boolean)TRUE)
  {
    retVal = (Dlt_ReturnType)DLT_E_CONTEXT_ALREADY_REG;
  }
  /* #-- Otherwise, if session ID is invalid, reject registration. */
  else if (parg0 < 0x1000)
  {
    retVal = (Dlt_ReturnType)DLT_E_UNKNOWN_SESSION_ID;
  }
  /* #-- Otherwise, if there is a free slot in context table, register the new context. */
  else if ((Dlt_Context.count_app_ids < DLT_MAX_COUNT_APP_IDS) &&
           (Dlt_Context.app_ids[AppIdIndex].count_context_ids < (uint32)DLT_MAX_COUNT_CONTEXT_IDS_PER_APP_ID) &&
           (Dlt_NumberOfRegisteredContextIds < (uint32)DLT_MAX_COUNT_CONTEXT_IDS)
          )
  {
    Dlt_NumberOfRegisteredContextIds++;

    /* If Application ID is not registered yet, use new Context entry */
    ConIdIndex = Dlt_Context.app_ids[AppIdIndex].count_context_ids;

    Dlt_Context.app_ids[AppIdIndex].app_id = AppId;
    Dlt_Context.app_ids[AppIdIndex].app_description = AppDescription;
    Dlt_Context.app_ids[AppIdIndex].len_app_description = LenAppDescription;
    Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].context_id = ContextId;
    Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].context_description = ContextDescription;
    Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].len_context_description = LenContextDescription;
    Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].log_level = (sint8)Dlt_DefaultMaxLogLevel;
    Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].trace_status = (sint8)Dlt_DefaultTraceStatus;
    Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.session_id = parg0;

    /* #-- If specified session ID is in valid range, specify callback functions for log level and trace status. */
    if ((parg0 - 0x1000) < DLT_MAX_NUMBER_OF_DLT_USERS)
    {
      Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetLogLevel_ = Dlt_SetLogLevelOpClass[parg0 - 0x1000];
      Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetTraceStatus_ = Dlt_SetTraceStatusOpClass[parg0 - 0x1000];
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
      Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.verbose_mode = Dlt_HeaderUseVerboseMode;
      Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetVerboseMode_ = Dlt_SetVerboseModeOpClass[parg0 - 0x1000];
# endif
    }

    /* #-- If context is registered, increase firstly the number of context IDs, than increase number of application IDs */
    Dlt_Context.app_ids[AppIdIndex].count_context_ids++;
    
    if (applicationAlreadyRegistered == (boolean)FALSE)
    {
      Dlt_Context.count_app_ids++;
    }

    retVal = (Dlt_ReturnType)DLT_E_OK;

    /* #-- If applicable, inform SWC about default values */
    if (Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetLogLevel_ != NULL_PTR)
    {
      (void)Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetLogLevel_(AppId, ContextId, Dlt_DefaultMaxLogLevel);
      (void)Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetTraceStatus_(AppId, ContextId, Dlt_DefaultTraceStatus);
# if defined (DLT_IMPLEMENT_VERBOSE_MODE)
      (void)Dlt_Context.app_ids[AppIdIndex].context_id_info[ConIdIndex].cbk_info.Dlt_SetVerboseMode_(AppId, ContextId, Dlt_HeaderUseVerboseMode);
# endif
    }
  }
  /* #-- Otherwise, the context could not be registered because there is no space left. */
  else
  {
    /*Dlt_CallDetReportError((DLT_API_ID_REGISTERCONTEXT), (DLT_E_ERROR_TO_MANY_CONTEXT)); */
    errorId = DLT_E_ERROR_TO_MANY_CONTEXT;
    retVal = (Dlt_ReturnType)DLT_E_ERROR_UNKNOWN;
  }

  /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0 */
  DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_REGISTERCONTEXT, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */
  
  /* PRQA S 6010 4 */ /* MD_MSR_STPTH */
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */
  /* PRQA S 6060 2 */ /* MD_MSR_STPAR */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Dlt_RegisterContext */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 * Dlt_SetState
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SetState(Dlt_GlobalStateType NewState)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

    /* #-- If Dlt_GlobalState differs from new state, change the Dlt_GlobalStateNew.
     *     Otherwise, the request is invalid.
     */
    switch(Dlt_GlobalState)
    {
    case(DLT_GLOBAL_STATE_OFFLINE) :
      if (NewState == DLT_GLOBAL_STATE_ONLINE)
      {
        Dlt_GlobalStateNew = DLT_GLOBAL_STATE_ONLINE;
        retVal = E_OK;
      }
      break;

    case(DLT_GLOBAL_STATE_ONLINE) :
      if (NewState == DLT_GLOBAL_STATE_OFFLINE)
      {
        Dlt_GlobalStateNew = DLT_GLOBAL_STATE_OFFLINE;
        retVal = E_OK;
      }
      break;

    default :
      break;
    }

    /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0 */
    DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_SETSTATE, errorId);
  }
# else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_SetState */

/**********************************************************************************************************************
 * Dlt_GetState
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_GetState(P2VAR(Dlt_GlobalStateType, AUTOMATIC, DLT_APPL_VAR) CurrentStatePtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #-- The request of Dlt_GlobalState is only valid in state DLT_GLOBAL_STATE_OFFLINE and 
     *     DLT_GLOBAL_STATE_ONLINE.
     */
    switch(Dlt_GlobalState)
    {
    case(DLT_GLOBAL_STATE_OFFLINE) :
      *CurrentStatePtr = (Dlt_GlobalStateType)DLT_GLOBAL_STATE_OFFLINE;
      retVal = E_OK;
      break;

    case(DLT_GLOBAL_STATE_ONLINE) :
      *CurrentStatePtr = (Dlt_GlobalStateType)DLT_GLOBAL_STATE_ONLINE;
      retVal = E_OK;
      break;

    default :
      break;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_GETSTATE, errorId);
  }
# else /* (DLT_DEV_ERROR_REPORT == STD_ON) */
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif /* (DLT_DEV_ERROR_REPORT == STD_ON) */

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_GetState */

/**********************************************************************************************************************
 * Dlt_MainFunction
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 */
FUNC(void, DLT_CODE) Dlt_MainFunction(void)
{
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #-- Enter Dlt_DLT_EXCLUSIVE_AREA_0. */
    DLT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    
    /* #-- Change global Dlt state if requested. */
    Dlt_Sm_GlobalStateTransition();

    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
      switch(Dlt_CurrentState)
      {
      /* #-- If DLT state is DLT_STATE_WAIT_FOR_RECEP AND a message was successfully received:
       *       DLT is ready to send data, so change state to DLT_STATE_READY_TO_SEND.
       */
      case (DLT_STATE_WAIT_FOR_RECEP) :
        if (Dlt_RxIndicationOccured > 0)
        {
          Dlt_Sm_StateTransition(kDltEventMessageReceived);
        }
        break;

      /* #-- If DLT state is DLT_STATE_READY_TO_SEND AND any buffer has content:
       *       DLT has data to send, so change state to DLT_STATE_TRY_SENDING.
       */
      case (DLT_STATE_READY_TO_SEND) :
        if ((Dlt_ReceiveBuffer.Read != Dlt_ReceiveBuffer.Write)         ||
            (Dlt_ControlSendBuffer.Read != Dlt_ControlSendBuffer.Write) ||
            (Dlt_SendBuffer.Read != Dlt_SendBuffer.Write)
           )
        {
          Dlt_Sm_StateTransition(kDltEventOneBufferHasContent);
        }
        break;

      /* #-- If DLT state is DLT_STATE_TRY_SENDING:
       *       If request to send data was accepted, change state to DLT_STATE_SENDING.
       *       Otherwise, the send request was rejected, change state to DLT_STATE_READY_TO_SEND.
       */
      case (DLT_STATE_TRY_SENDING) :
        if (Dlt_SendingOrderAccepted == DLT_OK)
        {
          Dlt_SendingOrderAccepted = (uint8)DLT_NOT_OK;
          Dlt_Sm_StateTransition(kDltEventTransmitCallSuccessful);
        }
        else
        {
          Dlt_Sm_StateTransition(kDltEventTransmitCallRejected);
        }
        break;

      /* #-- If DLT state is DLT_STATE_SENDING, handle state transition. */
      case (DLT_STATE_SENDING) :
        Dlt_Sm_HandleSendingTransitions();
        break;

      /* #-- Otherwise, the current state is invalid */
      default :
# if (DLT_DEV_ERROR_DETECT == STD_ON)
        errorId = DLT_E_INVALID_STATE;
# endif
        break;
      }

      /* #-- If socket connection was closed since last call of Dlt_MainFunction, request opening of socket connection. */
      if ((Dlt_SoConMode == DLT_SOCON_OFFLINE) &&
          (Dlt_SoConLastMode != Dlt_SoConMode))
      {
        Dlt_SoConLastMode = Dlt_SoConMode;
        DltCom_OpenSoCon();
      }
    }

    /* #-- Leave Dlt_DLT_EXCLUSIVE_AREA_0 */
    DLT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_MAINFUNCTION, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

#else /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* #-- Update global state. */
  if (Dlt_GlobalState != Dlt_GlobalStateNew)
  {
    Dlt_GlobalState = Dlt_GlobalStateNew;
  }

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
  
  /*lint -e{550} */ /* PC_Dlt_550 */
  /* PRQA S 6030 1 */ /* MD_MSR_STCYC */
} /* Dlt_MainFunction */

#define DLT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:

  MD_Dlt_3218:     MISRA rule 8.7: File scope static, '%s', only accessed in one function.
     Reason:         Constants defined global but are used only once. The function uses reference, thus constants 
                     have to live after leaving the function.
     Risk:           No error.
     Prevention:     Not required.

  MD_Dlt_1514:    MISRA rule 8.7: Objects shall be defined at block scope if they are only accessed from within a 
                                  single function.
     Reason:         Constants defined global but are used only once. The function uses reference, thus constants 
                     have to live after leaving the function.
     Risk:           No error.
     Prevention:     Not required.

  MD_Dlt_0686:     MISRA rule 9.2: Braces shall be used to indicate and match the structure in the non-zero 
                                   initialisation of arrays and structures. 
     Reason:         The structure Dlt_Context cannot be initialized completely global due to variable length depending 
                     on config parameter DLT_MAX_COUNT_APP_IDS.
     Risk:           No error.
     Prevention:     Not required.

  MD_Dlt_3673:     MISRA rule 16.7: A pointer parameter in a function prototype should be declared as pointer to const
                                    if the pointer is not used to modify the addressed object.
     Reason:         The use of these parameters is configuration dependent. In some configurations the pointer is used
                     to modify the addressed object.
     Risk:           No error.
     Prevention:     Not required.
*/

/* module specific PClint deviations:
  
  PC_Dlt_438:      PClint rule 438: Last value assigned to variable 'Symbol' not used.
     Reason:         The variables which are not used in functions are covered by dummy statements, but these statements 
                     can be defined to nothing.
     Risk:           Undetected unused variables.
     Prevention:     Not required.

  PC_Dlt_550:      PClint rule 550: Symbol 'Symbol' (Location) not accessed.
     Reason:         The errorId is always created and initialized, but if development error reporting is disabled, 
                     the variable is never used.
     Risk:           Undetected unused variables.
     Prevention:     Not required.
 */

/* COV_JUSTIFICATION_BEGIN

\ID COV_DLT_FILTER_ASR_XCP
\ACCEPT XF tx xf
\ACCEPT TX tx tx 
\ACCEPT TF tx tf 
\REASON The feature Dlt_Autosar is filtered if according licence is not available

\ID COV_DLT_MSR_COMPATIBILITY
\ACCEPT TX 
\ACCEPT XF 
\REASON [COV_MSR_COMPATIBILITY]

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Dlt.c
 *********************************************************************************************************************/
 
