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
/*! \file   CddDes.c
 *  \brief  CddDes source file
 *
 *  \details  Diagnostic event synchronizer main source file and implementation
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alexander Ditte               visade        Vector Informatik GmbH
 *  Sebastian Kobbe               vissko        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  refer to CddDes.h
 *********************************************************************************************************************/

#define CDDDES_SOURCE

/*********************************************************************************************************************
 * GLOBAL MISRA VIOLATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "CddDes.h"
#include "CddDes_Cbk.h"
#include "SchM_CddDes.h"

                            /* Development error tracer, for Det_ReportError */
/* ------------------------------------------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

                                      /* vstdlib for optimized copy routines */
/* ------------------------------------------------------------------------- */
#include "vstdlib.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of CddDes header file */
#if (  (CDDDES_SW_MAJOR_VERSION != (1u)) \
    || (CDDDES_SW_MINOR_VERSION != (0u)) \
    || (CDDDES_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of CddDes.c and CddDes.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (CDDDES_CFG_SW_MAJOR_VERSION != (1u)) \
  || (CDDDES_CFG_SW_MINOR_VERSION != (0u)) )
# error "Version numbers of Cdddes.c and BSWM-D file are inconsistent!"
#endif

#if (  (CDDDES_CFG_GEN_MAJOR_VERSION != (1u)) \
     ||(CDDDES_CFG_GEN_MINOR_VERSION != (0u)))
# error "Incompatible configuration file version used!"
#endif

/* Check for invalid configuration states */
#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON) && (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
# error "Either select Master or Dependent mode"
#endif
#if (CDDDES_CFG_SUPPORT_MASTER == STD_OFF) && (CDDDES_CFG_SUPPORT_DEPENDENT == STD_OFF)
# error "Either select Master or Dependent mode"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

# define CDDDES_CORE_MSG_HDR_LENGTH (1u) /*!< Message Header only contains the Message Type: 1 Byte */

/* Avoid warnings of unused API parameters */
#if (CDDDES_USE_DUMMY_STATEMENT == STD_ON)
# define CDDDES_IGNORE_UNREF_PARAM(param)                               ((void)(param))                                                                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define CDDDES_IGNORE_UNREF_PARAM(param)
#endif

#define CDDDES_DEM_DTC_STATUS_BIT_NONE      (0x00u)     /*!< DTC status bitmask: No relevant bit set */
#define CDDDES_DEM_DTC_STATUS_BIT_TF        (0x01u)     /*!< DTC status bitmask: testFailed */
#define CDDDES_DEM_DTC_STATUS_BIT_TFTOC     (0x02u)     /*!< DTC status bitmask: testFailedThisOperationCycle */
#define CDDDES_DEM_DTC_STATUS_BIT_TNCTOC    (0x40u)     /*!< DTC status bitmask: testNotCompletedThisOperationCycle */

#define CDDDES_DEM_DTC_STATUS_BYTE_MASK     (uint8)(CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC | CDDDES_DEM_DTC_STATUS_BIT_TNCTOC)
#define CDDDES_DEM_DTC_STATUS_BYTE_CLEARED  (0x50u)     /*!< DTC status after clear DTC */

#define CDDDES_DEM_NUM_STATUS_CHANGES       (9)

#define CDDDES_DEM_DTC_TO_CLEAR_NONE        (0xFFFFFFFFu)

#define CDDDES_HIGH_NIBBLE (0xF0u)  /*!< Masks the high nibble of an uint8 */
#define CDDDES_LOW_NIBBLE  (0x0Fu)  /*!< Masks the low nibble of an uint8 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

 /*! Type for DTC status change triggers and the derived test result */
typedef struct CddDes_Core_Buffer_s
{
  volatile CddDes_BufferSizeType ReadIndex;  /*!< Points to the Read Index. Should be updated only after whole "transaction" completed. */
  volatile CddDes_BufferSizeType WriteIndex; /*!< Points to the Write Index. Should be updated only after whole "transaction" completed. Must be updated from exclusive area as there are multiple writers. */
  uint8                 Buffer[CDDDES_CFG_COLLECTION_BUFFER_SIZE]; /*!< Actual buffer to collect status changes */
} CddDes_Core_BufferType;

typedef struct
{
  /* Flags set when message is received */
  CddDes_MsgTypeType                       LastReceivedMessage[CDDDES_CFG_NUM_CONNECTIONS]; /* Concept: Clear flag after "using" the message */
  CddDes_TxConfirmationResultType          LastTxConfirmation[CDDDES_CFG_NUM_CONNECTIONS];  /* Concept: Clear TxConf flag before entering wait state! */

  CddDes_BufferSizeType                    SyncMessageProcessingIndex; /*! Sync Message Processing Index */

                                                                       /* States are processed in MainFunction: */
  CddDes_ProtocolProcessingStateType       TopLevelState; /*! Top-Level State-Machine State */
  CddDes_ProtocolProcessingStateType       SubLevelState; /*! Sub-Level StateMachines State */

                                                          /* Timeout Counters are used for the various timeouts (reestablish connection, actual timeout, ...) */
  CddDes_TimeoutCounterType                TimeoutCounter[CDDDES_NUM_TIMEOUTS]; /* we need one per connection as we are processing things in parallel in state 'connected' */
} CddDes_Core_StateType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define CDDDES_START_SEC_VAR_INIT_8BIT
#include "MemMap.h"                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
CDDDES_LOCAL VAR(uint8, CDDDES_VAR_INIT) CddDes_Init_State = CDDDES_UNINIT;

#define CDDDES_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
# define CDDDES_START_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
CDDDES_LOCAL VAR(uint32, CDDDES_VAR) CddDes_Dem_DtcToClear;

# define CDDDES_STOP_SEC_VAR_NOINIT_32BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

#define CDDDES_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! context variable to store the connection related data */
CDDDES_LOCAL VAR(CddDes_ConnectionContextType, CDDDES_VAR) CddDes_Pdur_ConnectionContext[CDDDES_CFG_NUM_CONNECTIONS];

/*! protocol and message processing state of the Des Core */
CDDDES_LOCAL VAR(CddDes_Core_StateType, CDDDES_VAR) CddDes_Core_State;

/*! Status Change Collection Buffer */
CDDDES_LOCAL VAR(CddDes_Core_BufferType, CDDDES_VAR) CddDes_CollectionBuffer;

#define CDDDES_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

                                                            /* Helper Macros */
/* ------------------------------------------------------------------------- */
#define CddDes_SetHighNibble(Var, HighNibble) ( (Var) = (((Var) & CDDDES_LOW_NIBBLE)  | ((HighNibble) & CDDDES_HIGH_NIBBLE)) )  /*!< Clears and sets the high nibble of Var to the high nibble stored in HighNibble */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_SetLowNibble(Var, LowNibble)   ( (Var) = (((Var) & CDDDES_HIGH_NIBBLE) | ((LowNibble) & CDDDES_LOW_NIBBLE)) )    /*!< Clears and sets the low nibble of Var to the low nibble stored in LowNibble */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_GetHighNibble(Var)             ( (Var) & CDDDES_HIGH_NIBBLE ) /*!< Masks and returns the high nibble of Var */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_GetLowNibble(Var)              ( (Var) & CDDDES_LOW_NIBBLE )  /*!< Masks and returns the low nibble of Var */   /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Util_Make16Bit(hiByte,loByte)  ((uint16)((((uint16)(hiByte))<<8) | ((uint16)(loByte)))) /*!< Compose a word from single bytes */ /* PRQA S 3453 */ /* MD_MSR_19.7 */

                                                               /* Assertions */
/* ------------------------------------------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
/*! Wrapper to call Det_ReportError */
# define CddDes_Det_ReportError(ApiId, ErrorCode)   (void)Det_ReportError(CDDDES_MODULE_ID, CDDDES_INSTANCE_ID_DET, (ApiId), (ErrorCode));                  /* PRQA S 3412, 3453 */ /* MD_MSR_19.4, MD_MSR_19.7 */
#else
/*! Wrapper to call Det_ReportError */
# define CddDes_Det_ReportError(ApiId, ErrorCode)
#endif

#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
/*! Define local variable for DET error code */
# define CddDes_Det_ErrorRegisterDefine()           uint8 lErrorId;                                                                 /* PRQA S 3412, 3453 */ /* MD_MSR_19.4, MD_MSR_19.7 */
/*! Set error register with DET error code */
# define CddDes_Det_ErrorRegisterSet(value)         lErrorId = (value);                                                             /* PRQA S 3412, 3453 */ /* MD_MSR_19.4, MD_MSR_19.7 */
/*! Get error register with DET error code */
# define CddDes_Det_ErrorRegisterGet()              lErrorId                                                                        /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
/*! Define local variable for DET error code  */
# define CddDes_Det_ErrorRegisterDefine()                                                                                           /* PRQA S 3453 */ /* MD_MSR_19.7 */
/*! Set error register with DET error code */
# define CddDes_Det_ErrorRegisterSet(value)                                                                                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
/*! Get error register with DET error code */
# define CddDes_Det_ErrorRegisterGet()                                                                                              /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

                                                           /* PduR Interface */
/* ------------------------------------------------------------------------- */
#define CddDes_Pdur_SetConnectionState(Connection, Val)             CddDes_Pdur_ConnectionContext[(Connection)].State = (Val)  /*!< Set the State of the indicated connection to Val. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Pdur_GetConnectionState(Connection)                  (CddDes_Pdur_ConnectionContext[(Connection)].State)        /*!< Get the current State of the indicated connection */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Pdur_SetConnectionBufferIndex(Connection, Val)       CddDes_Pdur_ConnectionContext[(Connection)].Index = (Val)  /*!< Set the read/write position index of the currently processed message on the indicated connection to Val. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Pdur_GetConnectionBufferIndex(Connection)            (CddDes_Pdur_ConnectionContext[(Connection)].Index)        /*!< Get the read/write position index of the currently processed message on the indicated connection. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Pdur_SetConnectionMessageLength(Connection, Val)     CddDes_Pdur_ConnectionContext[(Connection)].Length = (Val) /*!< Set the currently processed message length of the indicated connection to Val. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Pdur_GetConnectionMessageLength(Connection)          (CddDes_Pdur_ConnectionContext[(Connection)].Length)       /*!< Get the length of the currently processed message. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Pdur_SetConnectionTxBufferHandle(Connection, Val)    CddDes_Pdur_ConnectionContext[(Connection)].TxBufferHandle = (Val) /*!< Sets the Buffer Handle of Connection to either the 'internal' TP buffer or the 'status collection buffer' for Transmission. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Pdur_GetConnectionTxBufferHandle(Connection)         (CddDes_Pdur_ConnectionContext[(Connection)].TxBufferHandle)       /*!< Gets the Buffer Handle of Connection used for transmission. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Pdur_GetConnectionTpBufferPtr(Connection)            (CddDes_Pdur_ConnectionContext[(Connection)].BufferPtr)     /*!< Get the pointer to the TpBuffer of Connection */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

                                                    /* Core Processing State */
/* ------------------------------------------------------------------------- */

#define CddDes_Core_GetMessageType(Connection)                      ((CddDes_Pdur_GetConnectionTpBufferPtr(Connection)[0]) & CDDDES_MESSAGE_TYPE_MASK)    /*!< Returns the first byte of the Pdur Tp Message Buffer of Connection. The first byte of each message is used to indicate the MessageType. The message type bitmask is applied to avoid that 0xFF ("no message") is ever read as message type. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_SetMessageType(Connection, MessageType)         CddDes_Pdur_GetConnectionTpBufferPtr(Connection)[0] = (MessageType)  /*!< Sets the first byte of the Pdur Tp Message Buffer of Connection to MessageType. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_GetMessagePayloadPtr(Connection)                (&CddDes_Pdur_GetConnectionTpBufferPtr(Connection)[CDDDES_CORE_MSG_HDR_LENGTH])  /*!< Get the pointer to the TpBuffer of Connection after the header */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetMessagePayloadLength(Connection)             (CddDes_Pdur_GetConnectionMessageLength(Connection) - CDDDES_CORE_MSG_HDR_LENGTH)    /*!< Get the length of the message, after subtracting the length of the message header */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetLastReceivedMessage(Connection, MessageType) (CddDes_Core_State.LastReceivedMessage[(Connection)] = (MessageType)) /*!< Remembers the MessageType when receiving a message. Used in the main function to check if a message was received */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetLastReceivedMessage(Connection)              (CddDes_Core_State.LastReceivedMessage[(Connection)])                 /*!< Returns the MessageType of the last received message. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_ClearLastReceivedMessage(Connection)            CddDes_Core_SetLastReceivedMessage((Connection), CDDDES_MESSAGE_NONE) /*!< Clears the last received message. The last received message shall be cleared after the trigger was processed from the main function. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetLastTxConfirmation(Connection, Result)       (CddDes_Core_State.LastTxConfirmation[(Connection)] = (Result))             /*!< Remembers the result of a TxConfirmation when transmitting a message. Used in the main function to check if a message was transmitted */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetLastTxConfirmation(Connection)               (CddDes_Core_State.LastTxConfirmation[(Connection)])                        /*!< Returns TxConfirmation result of the last transmitted message. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_ClearLastTxConfirmation(Connection)             CddDes_Core_SetLastTxConfirmation((Connection), CDDDES_TXCONFIRMATION_NONE) /*!< Clears the last TxConfirmation. The last TxConfirmation shall be cleared before waiting for a new TxConfirmation. */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetTimeoutCounter(Timeout, Value)               (CddDes_Core_State.TimeoutCounter[(Timeout)] = (Value))  /*!< Sets the timeout counter Timeout to Value */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetTimeoutCounter(Timeout)                      (CddDes_Core_State.TimeoutCounter[(Timeout)])            /*!< Gets the counter value of timeout Timeout (used connection specific)*/  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetProtocolTimeoutCounter()                     (CddDes_Core_GetTimeoutCounter(CDDDES_PROTOCOL_TIMER))   /*!< Gets the counter value of protocol timeout (used for the periodic transmission) */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_DecrementTimeoutCounter(Timeout)                (CddDes_Core_State.TimeoutCounter[(Timeout)]--)          /*!< Decrements the timeout counter of timeout Timeout */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_IsTimeoutElapsed(Timeout)                       ((boolean)(CddDes_Core_GetTimeoutCounter(Timeout) == 0)) /*!< Returns true if the counter value of timeout Timeout has reached zero */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetProtocolProcessingState(ProcessingState)     (CddDes_Core_State.TopLevelState = (ProcessingState))    /*!< Sets the Processing State of the Top Level State Machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetProtocolProcessingState()                    (CddDes_Core_State.TopLevelState)                        /*!< Gets the Processing State of the Top Level State Machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetEstablishConnectionState(ProcessingState)    (CddDes_Core_State.SubLevelState = (ProcessingState))    /*!< Sets the Processing State of the Sub-Level State machine when in State 'Establish Connection' */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetEstablishConnectionState()                   (CddDes_Core_State.SubLevelState)                        /*!< Gets the Processing State of the Sub-Level State machine when in State 'Establish Connection' */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetInitialSynchronizationState(ProcessingState) (CddDes_Core_State.SubLevelState = (ProcessingState))    /*!< Sets the Processing State of the Sub-Level State machine when in State 'Initial Synchronization' */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetInitialSynchronizationState()                (CddDes_Core_State.SubLevelState)                        /*!< Gets the Processing State of the Sub-Level State machine when in State 'Initial Synchronization' */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetConnectedState(ProcessingState)              (CddDes_Core_State.SubLevelState = (ProcessingState))    /*!< Sets the Processing State of the Sub-Level State machine when in State 'Connected'. Careful: The Sub-State 'Connected' has two state machines that use the higher respectively lower nibble of the state variable */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetConnectedState()                             (CddDes_Core_State.SubLevelState)                        /*!< Gets the Processing State of the Sub-Level State machine when in State 'Connected' */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetTxNackState(ProcessingState)                 (CddDes_Core_State.SubLevelState = (ProcessingState))    /*!< Sets the Processing State of the Sub-Level State machine when in State 'TX_NACK'. Careful: The Sub-State 'Connected' has two state machines that use the higher respectively lower nibble of the state variable */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetTxNackState()                                (CddDes_Core_State.SubLevelState)                        /*!< Gets the Processing State of the Sub-Level State machine when in State 'TX_NACK' */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetMasterTxNackState(ProcessingState)           (CddDes_SetHighNibble(CddDes_Core_State.SubLevelState, (ProcessingState)))  /*!< Sets the Processing State of the Master TxNack Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetMasterTxNackState()                          (CddDes_GetHighNibble(CddDes_Core_State.SubLevelState))                     /*!< Gets the Processing State of the Master TxNack Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetDependentTxNackState(ProcessingState)        (CddDes_SetLowNibble(CddDes_Core_State.SubLevelState, (ProcessingState)))   /*!< Sets the Processing State of the Dependent TxNack Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetDependentTxNackState()                       (CddDes_GetLowNibble(CddDes_Core_State.SubLevelState))                      /*!< Gets the Processing State of the Dependent TxNack Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetSyncTransmissionState(ProcessingState)       (CddDes_SetHighNibble(CddDes_Core_State.SubLevelState, (ProcessingState)))  /*!< Sets the Processing State of the Sync Message RX Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncTransmissionState()                      (CddDes_GetHighNibble(CddDes_Core_State.SubLevelState))                     /*!< Gets the Processing State of the Sync Message RX Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetSyncReceptionState(ProcessingState)          (CddDes_SetLowNibble(CddDes_Core_State.SubLevelState, (ProcessingState)))   /*!< Sets the Processing State of the Sync Message TX Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncReceptionState()                         (CddDes_GetLowNibble(CddDes_Core_State.SubLevelState))                      /*!< Gets the Processing State of the Sync Message TX Sub-Level State machine */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_SetSyncMessageProcessingIndex(Index)            (CddDes_Core_State.SyncMessageProcessingIndex = (Index))                    /*!< Sets the Processing Index of for processing a received Sync Message */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncMessageProcessingIndex()                 (CddDes_Core_State.SyncMessageProcessingIndex)                              /*!< Gets the Processing Index of for processing a received Sync Message */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncMessageCurrentEventType()                (CddDes_Core_GetMessagePayloadPtr(CDDDES_CORE_SYNC_RECEPTION_CONNECTION)[CddDes_Core_GetSyncMessageProcessingIndex()])  /*!< Gets the byte at the current Processing Index of a received Sync Message */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncMessageCurrentEventPayloadPtr()          (&(CddDes_Core_GetMessagePayloadPtr(CDDDES_CORE_SYNC_RECEPTION_CONNECTION)[CddDes_Core_GetSyncMessageProcessingIndex()+sizeof(CddDes_SyncEventType)]))  /*!< Gets the byte at the current Processing Index of a received Sync Message */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncMessageLength()                          (CddDes_Core_GetMessagePayloadLength(CDDDES_CORE_SYNC_RECEPTION_CONNECTION)) /*!< Returns the length of the received Sync Message */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_GetSyncMessageEndOfCurrentEvent(eventSize)      ((CddDes_BufferSizeType)(CddDes_Core_GetSyncMessageProcessingIndex() + sizeof(CddDes_SyncEventType) + (eventSize)))  /*!< Returns the processing index pointing to the beginning of the next SyncEvent (current processing index + length of the given length */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* If the ring buffer size is a power of two, the % operator may also be replaced by bitmasking. However, most compilers should do this optimitation automatically. */

#define CddDes_Core_Buffer_IncrementWriteIndex(Offset)              (CddDes_CollectionBuffer.WriteIndex=((CddDes_CollectionBuffer.WriteIndex+(Offset))%(CDDDES_CFG_COLLECTION_BUFFER_SIZE)))   /*!< Increment the WriteIndex by specified amount while considering the wrap-arround. MUST NOT BE INTERRUPTED BY ITSELF (Exclusive Area!) */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
/* IncrementReadIndex is implemented as inline function. */
#define CddDes_Core_Buffer_GetWriteIndex()                          (CddDes_CollectionBuffer.WriteIndex)                                                                                      /*!< Returns the WriteIndex */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_GetReadIndex()                           (CddDes_CollectionBuffer.ReadIndex)                                                                                       /*!< Returns the ReadIndex */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_SetWriteIndex(Index)                     (CddDes_CollectionBuffer.WriteIndex=(Index))                                                                              /*!< Sets the WriteIndex without considering the wrap arround! */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_SetReadIndex(Index)                      (CddDes_CollectionBuffer.ReadIndex=(Index))                                                                               /*!< Sets the ReadIndex without considering the wrap arround! */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_ReadByteAbs(Index)                       (CddDes_CollectionBuffer.Buffer[((Index)%(CDDDES_CFG_COLLECTION_BUFFER_SIZE))])                                           /*!< Reads a byte from the ringbuffer, using the absolute index, considering the wrap-arround */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_WriteByteAbs(Index, Value)               (CddDes_CollectionBuffer.Buffer[((Index)%(CDDDES_CFG_COLLECTION_BUFFER_SIZE))] = (Value))                                 /*!< Writes a byte to the ringbuffer, using the absolute index, considering the wrap-arround */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_ReadByteRelative(Offset)                 (CddDes_Core_Buffer_ReadByteAbs(CddDes_CollectionBuffer.ReadIndex+(Offset)))                                              /*!< Reads a byte from the ringbuffer, relative from the read index, considering the wrap-arround */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Core_Buffer_WriteByteRelative(Offset, Value)         (CddDes_Core_Buffer_WriteByteAbs(CddDes_CollectionBuffer.WriteIndex+(Offset), (Value)))                                   /*!< Writes a byte to the ringbuffer, relative to the write index, considering the wrap-arround */   /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDes_Core_GetSyncEventLength(Event)                       (CddDes_SyncEventLengthTable[(Event)])                                                                                    /*!< Returns the length of the sync event or VAR_LENGTH if length field follows after type */    /* PRQA S 3453 */ /* MD_MSR_19.7 */             

                                                     /* Configuration Access */
/* ------------------------------------------------------------------------- */
#define CddDes_Cfg_GetConnectionFromRxPdu(PduId)       CddDes_GetConnectionTableIdxOfRxPduMap(PduId)                                            /*!< Retrieve the connection to a given RxPduId */    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Cfg_GetConnectionFromTxPdu(PduId)       CddDes_GetConnectionTableIdxOfTxPduMap(PduId)                                            /*!< Retrieve the connection to a given TxPduId */    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Cfg_GetConnectionTpBufferSize(Connection) CddDes_GetTpBufferSizeOfConnectionTable(Connection)                                    /*! Retrieve the Tp buffer size for a given connection */   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDes_Cfg_GetTxPduFromConnection(Connection)    CddDes_GetCddPduRUpperLayerTxPduSymNameOfConnectionTable(Connection)                   /*! Retrieve the TxPduId to a given connection */     /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define CDDDES_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/*! Table with the relevant status bit changes and their test results */
CDDDES_LOCAL CONST(CddDes_Dem_DtcStatusTriggerTableType, CDDDES_CONST) CddDes_Dem_DtcStatusTriggerTable[CDDDES_DEM_NUM_STATUS_CHANGES] =            /* PRQA S 3218, 0779 */ /* MD_CddDes_8.7, MD_MSR_5.1_779*/
{ 
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TF,                                      CDDDES_DEM_DTC_STATUS_BIT_NONE),                                  CDDDES_DEM_TRIGGER_PASSED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TNCTOC,                                  CDDDES_DEM_DTC_STATUS_BIT_NONE),                                  CDDDES_DEM_TRIGGER_PASSED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TNCTOC,   CDDDES_DEM_DTC_STATUS_BIT_NONE),                                  CDDDES_DEM_TRIGGER_PASSED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC,    CDDDES_DEM_DTC_STATUS_BIT_TFTOC),                                 CDDDES_DEM_TRIGGER_PASSED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_NONE,                                    CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC),  CDDDES_DEM_TRIGGER_FAILED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TF,                                      CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC),  CDDDES_DEM_TRIGGER_FAILED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TFTOC,                                   CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC),  CDDDES_DEM_TRIGGER_FAILED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TNCTOC,                                  CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC),  CDDDES_DEM_TRIGGER_FAILED},
  {CddDes_Util_Make16Bit(CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TNCTOC,   CDDDES_DEM_DTC_STATUS_BIT_TF | CDDDES_DEM_DTC_STATUS_BIT_TFTOC),  CDDDES_DEM_TRIGGER_FAILED}
};
#endif

CDDDES_LOCAL CONST(CddDes_BufferSizeType, CDDDES_CONST) CddDes_SyncEventLengthTable[CDDDES_NUM_SYNCEVENTS] =    /* PRQA S 3218 */ /* MD_CddDes_8.7 */
{
 /* CDDDES_SYNCEVENT_DTC_PASSED */ 3,   /* one uint24 parameter */
 /* CDDDES_SYNCEVENT_DTC_FAILED */ 3,   /* one uint24 parameter */
 /* CDDDES_SYNCEVENT_CLEAR_DTC  */ 3    /* one uint24 parameter */
}; 

#define CDDDES_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CDDDES_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CddDes_Pdur_Init()
 *********************************************************************************************************************/
/*! \brief        Initialize the PduR interface sub-module
 *  \details      Initialization of the variables related to the PduR interface sub-module
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22732
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Pdur_Init( 
  void 
  );

/**********************************************************************************************************************
 *  CddDes_Pdur_Transmit()
 *********************************************************************************************************************/
/*! \brief        Triggers the transmission of a message
 *  \details      This API triggers the transmission of a message in the PduR sub-module.
 *  \param[in]    Connection    Unique identifier for a rx/tx PDU pair
 *  \param[in]    BufferHandle  Unique identifier for the buffer where the tx data is stored
 *  \param[in]    Length        The length of the message that shall be transmitted
 *  \return       E_OK          If the transmission is accepted from the PduR
 *  \return       E_NOT_OK      If the transmission is not accepted from the PduR
 *  \pre          The connection must be locked
 *  \context      TASK
 *  \reentrant    TRUE for different connections
 *  \synchronous  FALSE
 *  \trace        DSGN-CddDes22732
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Pdur_Transmit(
  CddDes_ConnectionIdType Connection,
  CddDes_BufferHandleType BufferHandle,
  CddDes_BufferSizeType Length
  );

/**********************************************************************************************************************
 *  CddDes_Pdur_LockConnection()
*********************************************************************************************************************/
/*! \brief        Locks the connection
 *  \details      This API locks the given connection so after the call a new message can be transmitted.
 *  \param[in]    Connection    Unique identifier for a rx/tx PDU pair
 *  \return       E_OK          If the connection lock was successful
 *  \return       E_NOT_OK      If the connection lock was not successful
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE for different connections
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22732
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Pdur_LockConnection(
  CddDes_ConnectionIdType Connection
  );

/**********************************************************************************************************************
 *  CddDes_Pdur_ReleaseConnection()
*********************************************************************************************************************/
/*! \brief        Release the connection
 *  \details      This API release the given connection so after the call a new message can be received or the coonection
 *                needs to be locked for a new transmission
 *  \param[in]    Connection    Unique identifier for a rx/tx PDU pair
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE for different connections
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22732
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Pdur_ReleaseConnection(
  CddDes_ConnectionIdType Connection
  );

/**********************************************************************************************************************
 *  CddDes_Core_Init()
 *********************************************************************************************************************/
/*! \brief        Initialize the Core sub-module
 *  \details      Initialization of the variables related to the Core sub-module
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22815
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_Init(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_GetReadAvailable()
 *********************************************************************************************************************/
/*! \brief        Returns the number of bytes ready to be read
 *  \details      The ReadIndex is only updated after a transition completed. Therefore the returned value may only be
 *                used before starting a transaction.
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22818
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(CddDes_BufferSizeType, CDDDES_CODE) CddDes_Core_Buffer_GetReadAvailable(
  void
  );

  /**********************************************************************************************************************
 *  CddDes_Core_Buffer_InternalGetReadAvailable()
 *********************************************************************************************************************/
/*! \brief        Returns the number of bytes ready to be read 
 *  \details      Do not use this function directly, use CddDes_Core_Buffer_GetReadAvailable instead.
 *  \pre          -
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22818
 *********************************************************************************************************************/
CDDDES_LOCAL_INLINE FUNC(CddDes_BufferSizeType, CDDDES_CODE) CddDes_Core_Buffer_InternalGetReadAvailable(
  boolean considerWriteWrap
  );

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_GetWriteAvailable()
 *********************************************************************************************************************/
/*! \brief        Returns the number of bytes that may be written to the status collection buffer
 *  \details      The WriteIndex is only updated after a transition completed. Therefore the returned value may only be
 *                used before starting a transaction.
 *  \pre          Must be executed from Exclusive Area!
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22818
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(CddDes_BufferSizeType, CDDDES_CODE) CddDes_Core_Buffer_GetWriteAvailable(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_ReadBytesRelative()
 *********************************************************************************************************************/
/*! \brief        Copies Bytes from the Ring Buffer to a Target Buffer
 *  \details      The data is copied relative from the current ReadIndex and the specified Offset. The ReadIndex is 
 *                NOT updated after copying the bytes. The ReadIndex must manually be incremented after the whole read
 *                transaction completed. Parameters are not checked for validity.
 *  \param[out]   TargetPtr    Pointer to Buffer to copy the data to
 *  \param[in]    Offset       Offset that is added to the ReadIndex to allow chunkwise copying of the data
 *  \param[in]    Length       Number of bytes to copy
 *  \return       E_OK         Always returned: Data copied as requested
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22818
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Core_Buffer_ReadBytesRelative(
  P2VAR(uint8, AUTOMATIC, CDDDES_VAR) TargetPtr,
  CddDes_BufferSizeType Offset,
  CddDes_BufferSizeType Length
  );

/**********************************************************************************************************************
 *  CddDes_Core_GetSyncMessageTxPayload()
 *********************************************************************************************************************/
/*! \brief        Assembles a segment of the SyncMessage
 *  \details      If start of the message, containing the Header and optionally Collection Buffer Payload, otherwise
 *                only Collection Buffer Payload.
 *  \param[out]   TargetPtr    Pointer to Buffer to copy the data to
 *  \param[in]    Offset       Offset that is added to the ReadIndex to allow chunkwise copying of the data
 *  \param[in]    Length       Number of bytes to copy
 *  \return       E_OK         Always returned: Data copied as requested
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22815, DSGN-CddDes22818
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Core_GetSyncMessageTxPayload(
  P2VAR(uint8, AUTOMATIC, CDDDES_VAR) TargetPtr,
  CddDes_BufferSizeType Offset,
  CddDes_BufferSizeType Length
  );

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_IncrementReadIndex()
 *********************************************************************************************************************/
/*! \brief        Advances the Read index by the specified Offset
 *  \details      Takes care of the wrap arround at the end of the buffer size.
 *  \param[in]    Offset       Offset that is added to the ReadIndex
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22818
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_Buffer_IncrementReadIndex(
  CddDes_BufferSizeType Offset
  );

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_WriteEvent_Uint32Parm()
 *********************************************************************************************************************/
/*! \brief        Writes the EventType and its 32bit parameter to the ring buffer
 *  \details      If enough buffer is available, the EventId is written to the ring buffer, follwed by the Parameter
 *                (serialized LSB to MSB byteorder). The write transaction is protected by an exclusive area. The
 *                ring buffer WriteIndex is updated after all data was written to the RingBuffer.
 *  \param[in]    EventType    Event Type Id
 *  \param[in]    Param        Uint32 Parameter (for instance, DTC)
 *  \return       E_OK         Event written to RingBuffer
 *  \return       E_NOT_OK     Failed to write Event to RingBuffer (Not enough space avaialable?)
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22818, DSGN-CddDes22968, DSGN-CddDes23226
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Core_Buffer_WriteEvent_Uint32Parm(
  CddDes_SyncEventType EventType, 
  uint32 Param
  );

/**********************************************************************************************************************
 * CddDes_Util_ReadUIntFromBuffer()
 *********************************************************************************************************************/
/*! \brief           Reads an unsigned integer (up to uint32) from byte buffer
 *  \details         Value is read serialized in LSB to MSB byteorder
 *
 *  \param[in]       NoBytes      Number of bytes to read and convert
 *  \param[in]       Buffer       Pointer to Buffer to read from
 *  \return          Converted Value
 *
 *  \pre             -
 *  \trace           DSGN-CddDes23226
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(uint32, CDDDES_CODE) CddDes_Util_ReadUIntFromBuffer(
  CddDes_BufferSizeType NoBytes,
  P2CONST(uint8, AUTOMATIC, CDDDES_APPL_DATA) Buffer
  );

/**********************************************************************************************************************
 *  CddDes_Core_SetNextProtocolProcessingState()
 *********************************************************************************************************************/
/*! \brief        Set the next processing state of the protocol state machine
 *  \details      Sets the Processing State of the Top Level State Machine and also resets the Sub-Level State Machine
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_SetNextProtocolProcessingState(
  CddDes_ProtocolProcessingStateType ProcessingState
  );

/**********************************************************************************************************************
 *  CddDes_Core_MainFunction()
 *********************************************************************************************************************/
/*! \brief        Main Function of the CddDes Core.
 *  \details      Do Timeout-Processing and then execute the respective State-Processing-Function..
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22815, DSGN-CddDes23126, DSGN-CddDes22830, DSGN-CddDes22831
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_MainFunction(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_Initialized()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with State "Initialized"
 *  \details      Check if any of the transitions out of State "Initialized" are ready. Clears the respective 
 *                condition and performs the actions associated with the transition. Sets next state.
 *  \note         Different implementations for Dependent and Master realized by precompile switch!
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22791
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Initialized(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_TransmitNack()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with State "Transmit NACK"
 *  \details      Check if any of the transitions out of State "Transmit NACK" are ready. Clears the respective
 *                condition and performs the actions associated with the transition. Sets next state.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23068
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_TransmitNack(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_Master_TransmitNack()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with State "Transmit NACK"
 *  \details      Master Connection State Machine Implementation
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23068
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Master_TransmitNack(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_Dependent_TransmitNack()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with State "Transmit NACK"
 *  \details      Dependent Connection State Machine Implementation
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23068
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Dependent_TransmitNack(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_EstablishConnection()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with State "Establish Connection" and its Sub-States.
 *  \details      Processes the sub-state machine and then checks if the top-level state should be left. Performs the
 *                actions associated with the transition and sets the next state if required.
 *  \note         Different implementations for Dependent and Master realized by precompile switch!
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23143 (Master), DSGN-CddDes23131 (Dependent)
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_EstablishConnection(
  void
  );

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
 *  CddDes_Core_State_Master_EstCon()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with Sub-States "Establish Connection"
 *  \details      Processes the sub-state machine: Performs the actions associated with the transition and sets the 
                  next state if required.
 *  \pre          CDDDES_CFG_SUPPORT_MASTER == STD_ON
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23143 (Master)
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Master_EstCon(
  void
  );
#endif

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 *  CddDes_Core_State_Dependent_EstCon()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with Sub-States "Establish Connection"
 *  \details      Processes the sub-state machine: Performs the actions associated with the transition and sets the 
                  next state if required.
 *  \pre          CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23131 (Dependent)
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Dependent_EstCon(
  void
  );
#endif
/**********************************************************************************************************************
 *  CddDes_Core_State_Connected()
 *********************************************************************************************************************/
/*! \brief        Perform actions associated with State "Connected" and its Sub-States.
 *  \details      Processes the sub-state machine and then checks if the top-level state should be left. Performs the
 *                actions associated with the transition and sets the next state if required.
 *  \note         Reception/Transmission Connection are determined by precompile switches 
 *                CDDDES_CORE_SYNC_RECEPTION_CONNECTION and CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION!
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22861
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Connected(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_RxSync()
 *********************************************************************************************************************/
/*! \brief        Perform actions and state transitions for receiving a sync message in states "Connected" and
 *                "Initial Synchronization"
 *  \details      Sub-State Processing is completed when state is EXIT_OK or EXIT_NOT_OK
 *  \note         Uses the lower nibble of the Sub-State variable to store its state. The used Connection is 
 *                determined through precompile switch CDDDES_CORE_SYNC_RECEPTION_CONNECTION!
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22978
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_RxSync(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_ProcessReceivedSyncMessage()
 *********************************************************************************************************************/
/*! \brief        Processes the SyncMessage received in SubState RxSync
 *  \details      Process all events until none is left or DEM returned pending / busy. Writes the response to the  
 *                message buffer if CDDDES_OPERATION_COMPLETED is returned.
 *  \pre          Sync Message was received before, CddDes_PduR connection is still locked.
 *  \return       CDDDES_OPERATION_COMPLETED  Processing of SyncMessage Done.
 *  \return       CDDDES_OPERATION_PENDING    DEM is busy / pending message. Execute function again in the next task cycle
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \trace        DSGN-CddDes22968
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(CddDes_AsyncReturnType, CDDDES_CODE) CddDes_Core_ProcessReceivedSyncMessage(void);

/**********************************************************************************************************************
 *  CddDes_Core_ProcessSyncEvent()
 *********************************************************************************************************************/
/*! \brief        Decode the EventId and call the corresponding function
 *  \details      Depending on Master/Dependent configuration, not all functions are dispatched to allow smaller build size.
 *  \return       CDDDES_OPERATION_COMPLETED  Processing of SyncMessage Done.
 *  \return       CDDDES_OPERATION_PENDING    DEM is busy / pending message. Execute function again in the next task cycle
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \pre          -
 *  \trace        DSGN-CddDes22968
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(CddDes_AsyncReturnType, CDDDES_CODE) CddDes_Core_ProcessSyncEvent(
  CddDes_SyncEventType EventType,
  uint32 DTC
  );

/**********************************************************************************************************************
 *  CddDes_Core_State_TxSync()
 *********************************************************************************************************************/
/*! \brief        Perform actions and state transitions for transmitting a sync message in states "Connected" and
 *                "Initial Synchronization"
 *  \details      Sub-State Processing is completed when state is EXIT_OK or EXIT_NOT_OK
 *  \note         Uses the high nibble of the Sub-State variable to store its state. The used Connection is 
 *                determined through precompile switch CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION!
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes23120
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_TxSync(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Core_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Remembers the result of the TxConfirmation to allow processing from the MainFunction.
 *  \details      -
 *  \param[in]    Connection    Unique identifier for a rx/tx PDU pair
 *  \param[in]    Result        E_OK if the transmission was successful
 *                              E_NOT_OK if the transmission was not successful
 *  \context      TASK|ISR
 *  \pre          -
 *  \reentrant    TRUE for different connections
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22815
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_TxConfirmation(
  CddDes_ConnectionIdType Connection,
  Std_ReturnType Result
  );

/**********************************************************************************************************************
 *  CddDes_Core_RxIndication()
 *********************************************************************************************************************/
/*! \brief        Remembers that a message was received to allow processing from the MainFunction.
 *  \details      Uses the first byte of the received message containing the MessageType.
 *  \param[in]    Connection    Unique identifier for a rx/tx PDU pair
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different connections
 *  \synchronous  TRUE
 *  \pre          -
 *  \trace        DSGN-CddDes22815
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_RxIndication(
  CddDes_ConnectionIdType Connection
  );

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 *  CddDes_Core_AddDtcPassed()
 *********************************************************************************************************************/
/*! \brief        Enqueues a DTC passed trigger into the transmit queue
 *  \details      The DTC is enqued with the trigger for test result passed
 *  \param[in]    DTC           The Diagnostic trouble code
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \trace        DSGN-CddDes22815
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_AddDtcPassed(
  uint32 DTC
  );
#endif

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
*  CddDes_Core_AddDtcFailed()
*********************************************************************************************************************/
/*! \brief       Enqueues a DTC failed trigger into the transmit queue
*  \details      The DTC is enqued with the trigger for test result failed
*  \param[in]    DTC           The Diagnostic trouble code
*  \context      TASK
*  \reentrant    FALSE
*  \synchronous  TRUE
*  \pre          -
*  \trace        DSGN-CddDes22815
*********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_AddDtcFailed(
  uint32 DTC
  );
#endif

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
*  CddDes_Core_AddDtcCleared()
*********************************************************************************************************************/
/*! \brief        Enqueues a DTC clear trigger into the transmit queue
*   \details      The clear DTC, clear group of DTCs or clear all DTCs is enqued
*   \param[in]    DTC           The Diagnostic trouble code
*   \context      TASK
*   \reentrant    FALSE
*   \synchronous  TRUE
*   \pre          -
*   \trace        DSGN-CddDes22815
*********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_AddDtcCleared(
  uint32 DTC
  );
#endif

/**********************************************************************************************************************
 *  CddDes_Dem_Init()
 *********************************************************************************************************************/
/*! \brief        Initialize the Dem sub-module
 *  \details      Initialization of the variables related to the Dem sub-module
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \trace        DSGN-CddDes22814
 *********************************************************************************************************************/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Dem_Init(
  void
  );

/**********************************************************************************************************************
*  CddDes_Dem_SetDtcStatus()
*********************************************************************************************************************/
/*! \brief    Reports the given DTC status (passed or failed) to the Dem
 *  \details  Fetches the EventId from the Dem for the given DTC and reports the qualified test reult to the Dem
 *  \param[in]    DTC           The Diagnostic trouble code
 *  \param[in]    Result        The qualified DTC test result which is either passed or failed
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 *  \trace        DSGN-CddDes22814
*********************************************************************************************************************/
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Dem_SetDtcStatus(
  uint32 DTC,
  CddDes_SyncEventType Result
  );

/**********************************************************************************************************************
*  CddDes_Dem_ClearDtc()
*********************************************************************************************************************/
/*! \brief    Clears the gived DTC in the Dem
 *  \details  Clears the given DTC, group of DTCs or all DTCs in the DEM. Must be called until function retuns COMPLETED.
 *  \param[in]    DTC           The Diagnostic trouble code or a group of DTCs or all DTCs
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \pre          -
 *  \trace        DSGN-CddDes22814
*********************************************************************************************************************/
CDDDES_LOCAL FUNC(CddDes_AsyncReturnType, CDDDES_CODE) CddDes_Dem_ClearDtc(
  uint32 DTC
  );

#define CDDDES_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define CDDDES_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * CddDes_Pdur_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Pdur_Init(
  void
  )
{ /* Initialize the connection context for both, master and dependent connection*/
  CddDes_Pdur_ConnectionContext[CDDDES_CFG_CONNECTION_MASTER].BufferPtr = &CddDes_Cfg_TpBufferMaster[0];
  CddDes_Pdur_SetConnectionTxBufferHandle(CDDDES_CFG_CONNECTION_MASTER, CDDDES_HANDLE_TP_BUFFER);
  CddDes_Pdur_SetConnectionState(CDDDES_CFG_CONNECTION_MASTER, CDDDES_PDUR_IDLE);

  CddDes_Pdur_ConnectionContext[CDDDES_CFG_CONNECTION_DEPENDENT].BufferPtr = &CddDes_Cfg_TpBufferDependent[0];
  CddDes_Pdur_SetConnectionTxBufferHandle(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_HANDLE_TP_BUFFER);
  CddDes_Pdur_SetConnectionState(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_PDUR_IDLE);
}

/**********************************************************************************************************************
 * CddDes_Pdur_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Pdur_Transmit(
  CddDes_ConnectionIdType Connection,
  CddDes_BufferHandleType BufferHandle,
  CddDes_BufferSizeType Length
  )
{
  Std_ReturnType lResult;
  PduInfoType lPduInfo;

  lPduInfo.SduDataPtr = CddDes_Pdur_GetConnectionTpBufferPtr(Connection);
  lPduInfo.SduLength = Length;

  CddDes_Pdur_SetConnectionState(Connection, CDDDES_PDUR_TX_COPY);
  CddDes_Pdur_SetConnectionBufferIndex(Connection, 0);
  CddDes_Pdur_SetConnectionMessageLength(Connection, Length);
  CddDes_Pdur_SetConnectionTxBufferHandle(Connection, BufferHandle);

  lResult = PduR_CddDesTransmit(CddDes_Cfg_GetTxPduFromConnection(Connection), &lPduInfo);

  if (lResult == E_NOT_OK)
  {
    CddDes_Pdur_SetConnectionState(Connection, CDDDES_PDUR_IDLE);
  }

  return lResult;
}

/**********************************************************************************************************************
 * CddDes_Pdur_LockConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Pdur_LockConnection(
  CddDes_ConnectionIdType Connection
  )
{
  Std_ReturnType lResult = E_NOT_OK;

  if (CddDes_Pdur_GetConnectionState(Connection) == CDDDES_PDUR_IDLE)
  {
    CddDes_Pdur_SetConnectionState(Connection, CDDDES_PDUR_TX_WAIT);

    lResult = E_OK;
  }
  return lResult;
}

/**********************************************************************************************************************
 * CddDes_Pdur_ReleaseConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Pdur_ReleaseConnection(
  CddDes_ConnectionIdType Connection
  )
{
  CddDes_Pdur_SetConnectionState(Connection, CDDDES_PDUR_IDLE);
}

/**********************************************************************************************************************
 * CddDes_Core_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_Init(
  void
  )
{
  CddDes_Core_SetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER, CDDDES_MESSAGE_NONE);
  CddDes_Core_SetLastReceivedMessage(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_MESSAGE_NONE);
  CddDes_Core_SetLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER, CDDDES_TXCONFIRMATION_NONE);
  CddDes_Core_SetLastTxConfirmation(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_TXCONFIRMATION_NONE);
  CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_MASTER, CDDDES_CFG_TIMEOUT_NONE);
  CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_CFG_TIMEOUT_NONE);

#if (CDDDES_CFG_SUPPORT_MASTER)
  CddDes_Core_SetTimeoutCounter(CDDDES_PROTOCOL_TIMER, CDDDES_CFG_TIMEOUT_NONE);
#endif
#if (CDDDES_CFG_SUPPORT_DEPENDENT)
  CddDes_Core_SetTimeoutCounter(CDDDES_PROTOCOL_TIMER, CDDDES_CFG_TIMEOUT_RECONNECT);
#endif

  CddDes_Core_SetProtocolProcessingState(CDDDES_PROTOCOL_STATE_INITIALIZED);
  /* Even though all Sub-States are mapped to the same Variable,
     use all the macros to set state to invalid... In case this
     design decision is revised... */
  CddDes_Core_SetInitialSynchronizationState(CDDDES_PROTOCOL_STATE_INVALID);
  CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_STATE_INVALID);
  CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_STATE_INVALID);
  CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_STATE_INVALID);
  CddDes_Core_SetTxNackState(CDDDES_PROTOCOL_STATE_INVALID);

  CddDes_Core_SetSyncMessageProcessingIndex(0);
  CddDes_Core_Buffer_SetWriteIndex(0);
  CddDes_Core_Buffer_SetReadIndex(0);
}

/**********************************************************************************************************************
 * CddDes_Core_SetNextProtocolProcessingState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_SetNextProtocolProcessingState(
  CddDes_ProtocolProcessingStateType ProcessingState
  )
{
  CddDes_Core_State.SubLevelState = CDDDES_PROTOCOL_SUBSTATE_ENTRY; 
  CddDes_Core_State.TopLevelState = (ProcessingState);
}

/**********************************************************************************************************************
 * CddDes_Core_MainFunction()
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
 *
 *
 *
 *
 *
 *
 *
 *
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
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_MainFunction(
  void
  )
{
  uint8_least timeoutIterator;

  /* Independently of processing state: Update all TimeoutTimers (decrement while not elapsed) */
  for (timeoutIterator = 0; timeoutIterator < CDDDES_NUM_TIMEOUTS; timeoutIterator++)
  {
    if (CddDes_Core_GetTimeoutCounter(timeoutIterator) > 0)
    {
      CddDes_Core_DecrementTimeoutCounter(timeoutIterator);
    }
  }

  /* Process Top-Level Protocol State Machine */
  switch (CddDes_Core_GetProtocolProcessingState())
  {
    case CDDDES_PROTOCOL_STATE_INITIALIZED:
    {
      CddDes_Core_State_Initialized();
      break;
    }

    case CDDDES_PROTOCOL_STATE_ESTABLISH_CONNECTION:
    {
      CddDes_Core_State_EstablishConnection();
      break;
    }

    case CDDDES_PROTOCOL_STATE_INITIAL_SYNCHRONIZATION:
    {
      /* TODO For Sprint 2 we do not perform the initial synchronization and directly jump to connected state. */
      CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_CONNECTED);
      break;
    }

    case CDDDES_PROTOCOL_STATE_CONNECTED:
    {
      CddDes_Core_State_Connected();
      break;
    }

    case CDDDES_PROTOCOL_STATE_TRANSMIT_NACK:
    {
      CddDes_Core_State_TransmitNack();
      break;
    }

    case CDDDES_PROTOCOL_STATE_INVALID:
    default:
    {
      /* CddDes has not yet been initialized. This state should not be executed ever. */
      break;
    }
  }
}

/**********************************************************************************************************************
 * CddDes_Core_State_Initialized()
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
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Initialized(
  void
  )
{
#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
  if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER) == CDDDES_MESSAGE_REQUEST_CONNECTION)
  {
    /* Transition to State "Establish Connection */
    CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_ESTABLISH_CONNECTION);
    /* "Consume" Trigger */
    CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER);
  }
#endif

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
  if (CddDes_Core_GetProtocolTimeoutCounter() == 0) /* Protocol Timeout elapsed.. */
  {
    /* Transition to State "Establish Connection */
    CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_ESTABLISH_CONNECTION);
  }
#endif

  /* Ignore received NACK messages... */
  if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER) == CDDDES_MESSAGE_NACK)
  {
    CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER);
    CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
  }

  if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_DEPENDENT) == CDDDES_MESSAGE_NACK)
  {
    CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_DEPENDENT);
    CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_DEPENDENT);
  }

  /* Send a NACK message for any other received message... */
  if ((CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER) != CDDDES_MESSAGE_NONE)
    ||(CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_DEPENDENT) != CDDDES_MESSAGE_NONE))
  {
    /* Transition to State "Transmit NACK" */
    CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_TRANSMIT_NACK);
  }
}

/**********************************************************************************************************************
 * CddDes_Core_State_Master_TransmitNack()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Master_TransmitNack(
  void
  )
{
  /* Do the master State machine */
  switch (CddDes_Core_GetMasterTxNackState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_ENTRY:
    {
      if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER) != CDDDES_MESSAGE_NONE)
      {
        /* Transition to State "Transmit NACK" */
        CddDes_Core_SetMasterTxNackState(CDDDES_PROTOCOL_SUBSTATE_MASTER_TRANSMIT_NACK);

        /* "Consume" Trigger */
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER);

        /* Set Retries */
        CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_MASTER, CDDDES_CFG_RETRIES_TRANSMISSION);

        /* Assemble Response */
        CddDes_Core_SetMessageType(CDDDES_CFG_CONNECTION_MASTER, CDDDES_MESSAGE_NACK);
      }
      else
      {
        CddDes_Core_SetMasterTxNackState(CDDDES_PROTOCOL_SUBSTATE_MASTER_EXIT);
      }
      break;
    }
    case CDDDES_PROTOCOL_SUBSTATE_MASTER_TRANSMIT_NACK:
    {
      CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER); /* Get rid of any pending but not processed TxConfirmation.. */
      if (CddDes_Pdur_Transmit(CDDDES_CFG_CONNECTION_MASTER, CDDDES_HANDLE_TP_BUFFER, CDDDES_MESSAGELENGTH_NACK) == E_OK)
      {
        CddDes_Core_SetMasterTxNackState(CDDDES_PROTOCOL_SUBSTATE_MASTER_WAIT_TX_CONFIRMATION);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_MASTER) == TRUE)
        {
          /* We failed to transmit the NACK message in time! Release Connection. */
          CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetMasterTxNackState(CDDDES_PROTOCOL_SUBSTATE_MASTER_EXIT);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_MASTER_WAIT_TX_CONFIRMATION:
    {
      if (CddDes_Core_GetLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER) != CDDDES_TXCONFIRMATION_NONE)
      {
        /* Should we report an error on negative Tx Confirmation? */
        CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER);
        CddDes_Core_SetMasterTxNackState(CDDDES_PROTOCOL_SUBSTATE_MASTER_EXIT);
      }
      break;
    }

    default:
      /* Exit or ERROR. Do nothing here.*/
      break;
  }
}

/**********************************************************************************************************************
* CddDes_Core_State_Dependent_TransmitNack()
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
 *
 *
 *
 *
 *
 *
 *
 *
*/
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Dependent_TransmitNack(
  void
  )
{
  /* Do the dependent State machine */
  switch (CddDes_Core_GetDependentTxNackState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_ENTRY:
    {

      if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_DEPENDENT) != CDDDES_MESSAGE_NONE)
      {
        /* Transition to State "Transmit NACK" */
        CddDes_Core_SetDependentTxNackState(CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_TRANSMIT_NACK);

        /* "Consume" Trigger */
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_DEPENDENT);

        /* Set Retries */
        CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_CFG_RETRIES_TRANSMISSION);

        /* Assemble Response */
        CddDes_Core_SetMessageType(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_MESSAGE_NACK);
      }
      else
      {
        CddDes_Core_SetDependentTxNackState(CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_EXIT);
      }
      break;
    }
    case CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_TRANSMIT_NACK:
    {
      CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_DEPENDENT); /* Get rid of any pending but not processed TxConfirmation.. */
      if (CddDes_Pdur_Transmit(CDDDES_CFG_CONNECTION_DEPENDENT, CDDDES_HANDLE_TP_BUFFER, CDDDES_MESSAGELENGTH_NACK) == E_OK)
      {
        CddDes_Core_SetDependentTxNackState(CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_WAIT_TX_CONFIRMATION);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_DEPENDENT) == TRUE)
        {
          /* We failed to transmit the NACK message in time! Release Connection. */
          CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_DEPENDENT);
          CddDes_Core_SetDependentTxNackState(CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_EXIT);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_WAIT_TX_CONFIRMATION:
    {
      if (CddDes_Core_GetLastTxConfirmation(CDDDES_CFG_CONNECTION_DEPENDENT) != CDDDES_TXCONFIRMATION_NONE)
      {
        /* Should we report an error on negative Tx Confirmation? */
        CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_DEPENDENT);
        CddDes_Core_SetDependentTxNackState(CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_EXIT);
      }
      break;
    }

    default:
      /* Exit or ERROR. Do nothing here.*/
      break;
  }
}

/**********************************************************************************************************************
 * CddDes_Core_State_TransmitNack()
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
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_TransmitNack(
  void
  )
{

  /* Perform both Sub-State-Machines (Master & Dependent Connection) */

  CddDes_Core_State_Master_TransmitNack();
  CddDes_Core_State_Dependent_TransmitNack();

  /* Check Exit Condition */

  if ( (CddDes_Core_GetMasterTxNackState() == CDDDES_PROTOCOL_SUBSTATE_MASTER_EXIT) &&
       (CddDes_Core_GetDependentTxNackState() == CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_EXIT) )
  {
    CddDes_Core_SetProtocolProcessingState(CDDDES_PROTOCOL_STATE_INITIALIZED);
    /* We do not reset the protocol timeout (=Reconnect is not delayed by transmitting NACK messages) */
  }
}

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
 * CddDes_Core_State_Master_EstCon()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
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
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Master_EstCon(
  void
  )
{
  switch (CddDes_Core_GetEstablishConnectionState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_ENTRY:
    {
      /* Reset the Timer on entry.. */
      CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_MASTER, CDDDES_CFG_RETRIES_TRANSMISSION);

      /* Do protocol validation: */

      if ((CddDes_Pdur_GetConnectionMessageLength(CDDDES_CFG_CONNECTION_MASTER) == CDDDES_MESSAGELENGTH_REQUEST_CONNECTION)
          && (CddDes_Core_GetMessagePayloadPtr(CDDDES_CFG_CONNECTION_MASTER)[0] == CDDDES_PROTOCOL_VERSION))
      {
        /* Transmit ACK Response Message */
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_ACK);
        CddDes_Core_SetMessageType(CDDDES_CFG_CONNECTION_MASTER, CDDDES_MESSAGE_REQUEST_CONNECTION_ACK);
      }
      else
      {
        /* Transmit NACK Response Message */
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_NACK);
        CddDes_Core_SetMessageType(CDDDES_CFG_CONNECTION_MASTER, CDDDES_MESSAGE_NACK);
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_ACK:
    {
      /* Possible Transitions out of this sub-state are:

         Timeout==0       => go to NOT_OK exit State
         Transmit OK.     => go to WAIT_TX_CONFIRMATION
         Transmit NOT_OK  => stay in this state

      */
      CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER); /* Get rid of any pending but not processed TxConfirmation.. */
      if (CddDes_Pdur_Transmit(CDDDES_CFG_CONNECTION_MASTER, CDDDES_HANDLE_TP_BUFFER, CDDDES_MESSAGELENGTH_ACK) == E_OK)
      {
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_TX_CONFIRMATION);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_MASTER) == TRUE)
        {
          /* We failed to transmit the ACK message in time! Release Connection. */
          CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_TX_CONFIRMATION:
    {
      /* Possible Transitions out of this sub-state are:

         TxConfirmation OK      => go to Assemble Initial Sync State
                     or NOT_OK  => go to NOT_OK exit State

      */
      switch (CddDes_Core_GetLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER))
      {
        case CDDDES_TXCONFIRMATION_E_OK:
        {
          CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_ASSEMBLE_INIT_SYNC);
          break;
        }
        case CDDDES_TXCONFIRMATION_E_NOT_OK:
        {
          CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
          break;
        }
        case CDDDES_TXCONFIRMATION_NONE:
        default:
          /* Stay in this state ... */
          break;
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_ASSEMBLE_INIT_SYNC:
    {
      /* TODO: Add collection of initial state from Dem. */
      CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_OK);
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_NACK:
    {
      /* Possible Transitions out of this sub-state are:

         Timeout==0       => go to NOT_OK exit State
         Transmit OK.     => go to WAIT_NACK_TX_CONF
         Transmit NOT_OK  => stay in this state

      */
      CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER); /* Get rid of any pending but not processed TxConfirmation.. */
      if (CddDes_Pdur_Transmit(CDDDES_CFG_CONNECTION_MASTER, CDDDES_HANDLE_TP_BUFFER, CDDDES_MESSAGELENGTH_NACK) == E_OK)
      {
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_NACK_TX_CONF);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_MASTER) == TRUE)
        {
          /* We failed to transmit the ACK message in time! Release Connection. */
          CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_NACK_TX_CONF:
    {
      /* Possible Transitions out of this sub-state are:
         Any TxConfirmation      => go to NOT_OK exit State
      */
      if (CddDes_Core_GetLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER) != CDDDES_TXCONFIRMATION_NONE)
      {
        CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER);
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
      }
      break;
    }

    default:
      break;
  }
}   /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 * CddDes_Core_State_Dependent_EstCon()
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
 *
 *
 *
 *
 *
 *
 *
 *
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
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Dependent_EstCon(
  void
  )
{
  switch (CddDes_Core_GetEstablishConnectionState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_ENTRY:
    {
      /* Reset the Timer on entry.. */
      CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_MASTER, (CDDDES_CFG_RETRIES_TRANSMISSION-1)); /* As this state is "fall-through", the first trial happens in this task cycle */
      CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_REQUEST_CONNECTION);
      /* Intentionally skipping break; */
    }
    /* fall through */
    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_REQUEST_CONNECTION:  /* PRQA S 2003 */ /* MD_CddDes_2003 */
    {
      /* Possible Transitions out of this sub-state are:

         Timeout==0       => go to NOT_OK exit State
         Lock Buffer OK.  => go to TRANSMIT_REQUEST
      */

      if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_MASTER) == TRUE)
      {
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
      }
      else 
      {
        if (CddDes_Pdur_LockConnection(CDDDES_CFG_CONNECTION_MASTER) == E_OK)
        {
          /* Assemble Message.. */
          CddDes_Core_SetMessageType(CDDDES_CFG_CONNECTION_MASTER, CDDDES_MESSAGE_REQUEST_CONNECTION);
          CddDes_Core_GetMessagePayloadPtr(CDDDES_CFG_CONNECTION_MASTER)[0] = CDDDES_PROTOCOL_VERSION;
          /* Reset Timeout */
          CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_MASTER, CDDDES_CFG_RETRIES_TRANSMISSION);
          /* Prepare next State..*/
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_REQUEST);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_REQUEST:
    {
      /* Possible Transitions out of this sub-state are:

         Timeout==0       => go to NOT_OK exit State, release connection
         Transmit OK.     => go to WAIT_TX_CONFIRMATION
         Transmit NOT_OK  => stay in this state
      */

      CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER); /* Get rid of any pending but not processed TxConfirmation.. */
      if (CddDes_Pdur_Transmit(CDDDES_CFG_CONNECTION_MASTER, CDDDES_HANDLE_TP_BUFFER, CDDDES_MESSAGELENGTH_REQUEST_CONNECTION) == E_OK)
      {
        /* Go to next State..*/
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_TX_CONFIRMATION);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_MASTER) == TRUE)
        {
          /* We failed to transmit the ACK message in time! Release Connection. */
          CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_TX_CONFIRMATION:
    {
      /* Possible Transitions out of this sub-state are:

         TxConfirmation OK      => go to WAIT_RESPONSE State, reset Timeout
                     or NOT_OK  => go to NOT_OK exit State
      */
      switch (CddDes_Core_GetLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER))
      {
        case CDDDES_TXCONFIRMATION_E_OK:
        {
          CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetTimeoutCounter(CDDDES_CFG_CONNECTION_MASTER, CDDDES_CFG_TIMEOUT_RESPONSE);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_RESPONSE);
          break;
        }
        case CDDDES_TXCONFIRMATION_E_NOT_OK:
        {
          CddDes_Core_ClearLastTxConfirmation(CDDDES_CFG_CONNECTION_MASTER);
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
          break;
        }
        case CDDDES_TXCONFIRMATION_NONE:
        default:
          /* Stay in this state ... */
          break;
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_RESPONSE:
    {
      /* Possible Transitions out of this sub-state are:

         Timeout==0         => go to NOT_OK exit State
         ACK Response Received  => go to EXIT_OK, release Connection
         Any other Response Received  => go to EXIT_NOT_OK, release Connection
      */
      if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER) == CDDDES_MESSAGE_REQUEST_CONNECTION_ACK)
      {
        /* "Consume" Trigger */
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER);
        /* Release Connection */
        CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
        /* Transition to State "OK" */
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_OK);
      }
      else if (CddDes_Core_GetLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER) != CDDDES_MESSAGE_NONE)
      {
        /* "Consume" Trigger */
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CFG_CONNECTION_MASTER);
        /* Release Connection */
        CddDes_Pdur_ReleaseConnection(CDDDES_CFG_CONNECTION_MASTER);
        /* Transition to State "NOT_OK" */
        CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
      }
      else /* No message received.. Do timeout check. */
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CFG_CONNECTION_MASTER) == TRUE)
        {
          /* We failed to receive the response message in time! */
          CddDes_Core_SetEstablishConnectionState(CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK);
        }
      }
      break;
    }

    default:
      break;
  }
}   /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

/**********************************************************************************************************************
 * CddDes_Core_State_EstablishConnection()
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
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_EstablishConnection(
  void
  )
{
#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
  CddDes_Core_State_Master_EstCon();
#endif
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
  CddDes_Core_State_Dependent_EstCon();
#endif

  /* Check Transitions out of this State: */
  switch (CddDes_Core_GetEstablishConnectionState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_EXIT_OK:
    {
      CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_INITIAL_SYNCHRONIZATION);
      break;
    }
    case CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK:
    {
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
      CddDes_Core_SetTimeoutCounter(CDDDES_PROTOCOL_TIMER, CDDDES_CFG_TIMEOUT_RECONNECT);
#endif
      CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_INITIALIZED);
      break;
    }
    default:
      /* Continue sub-state processing...*/
      break;
  }
}

/**********************************************************************************************************************
 * CddDes_Core_State_RxSync()
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
 *
 *
 *
 *
 *
 *
 *
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
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_RxSync(
  void
  )
{
  switch (CddDes_Core_GetSyncReceptionState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_RX_ENTRY:
    {
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
      if (CddDes_Core_GetProtocolProcessingState() == CDDDES_PROTOCOL_STATE_INITIAL_SYNCHRONIZATION)
#endif
      {
        CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_CFG_TIMEOUT_CONNECTION_ALIVE);
      }
      CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_SYNC_MSG);
      /* break intentionally omitted as we directly jump into the Wait State! */
    }
    /* fall through */
    case CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_SYNC_MSG:       /* PRQA S 2003 */ /* MD_CddDes_2003 */
    {
      /*
        possible transitions out of this state are:
          - Sync Message Received
          - Any other message received
          - Timeout
      */
      if (CddDes_Core_GetLastReceivedMessage(CDDDES_CORE_SYNC_RECEPTION_CONNECTION) == CDDDES_MESSAGE_SYNC)
      {
        /* Expected message received */
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CORE_SYNC_RECEPTION_CONNECTION);
        CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_PROCESS_SYNC_MSG);
        CddDes_Core_SetSyncMessageProcessingIndex(0); /* Reset Processing Index */
        CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_CFG_TIMEOUT_NONE); /* No timeout while processing the received message.. */
      }
      else if (CddDes_Core_GetLastReceivedMessage(CDDDES_CORE_SYNC_RECEPTION_CONNECTION) != CDDDES_MESSAGE_NONE)
      {
        /* Unexpected message received */
        CddDes_Pdur_ReleaseConnection(CDDDES_CORE_SYNC_RECEPTION_CONNECTION);
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CORE_SYNC_RECEPTION_CONNECTION);
        CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK);
      }
      else
      {
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
        if (CddDes_Core_GetProtocolProcessingState() == CDDDES_PROTOCOL_STATE_INITIAL_SYNCHRONIZATION)
#endif
        {
          if (CddDes_Core_IsTimeoutElapsed(CDDDES_CORE_SYNC_RECEPTION_CONNECTION) == TRUE)
          {
            CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK);
          }
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_RX_PROCESS_SYNC_MSG:
    {
      if(CddDes_Core_ProcessReceivedSyncMessage() == CDDDES_OPERATION_COMPLETED)
      {
        CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_TRANSMIT_RESPONSE);
        CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_CFG_RETRIES_TRANSMISSION);
      }
      /* else: Continiue next task cycle */
      break;
    }
    case CDDDES_PROTOCOL_SUBSTATE_RX_TRANSMIT_RESPONSE:   /* PRQA S 2003 */ /* MD_CddDes_2003 */
    {
      CddDes_Core_ClearLastTxConfirmation(CDDDES_CORE_SYNC_RECEPTION_CONNECTION);
      if (CddDes_Pdur_Transmit(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_HANDLE_TP_BUFFER, CDDDES_MESSAGELENGTH_ACK) == E_OK)  /* Sends NACK or ACK message. Both have the same length, so we do not have to remember the response length. */
      {
        CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_TX_CONFIRMATION);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CORE_SYNC_RECEPTION_CONNECTION) == TRUE)
        {
          CddDes_Pdur_ReleaseConnection(CDDDES_CORE_SYNC_RECEPTION_CONNECTION);
          CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_TX_CONFIRMATION:
    {
      if (CddDes_Core_GetLastTxConfirmation(CDDDES_CORE_SYNC_RECEPTION_CONNECTION) == CDDDES_TXCONFIRMATION_E_OK)
      {
        CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_OK);
      }
      else if (CddDes_Core_GetLastTxConfirmation(CDDDES_CORE_SYNC_RECEPTION_CONNECTION) == CDDDES_TXCONFIRMATION_E_NOT_OK)
      {
        CddDes_Core_SetSyncReceptionState(CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK);
      }
      else
      {
         /* Stay in this State ... */
      }
      break;
    }

    default:
      /* Invalid State.. Do nothing. */
      break;
  }
} /* PRQA S 6080, 6030 */ /* MD_MSR_STMIF, MD_MSR_STCYC */ /* It does not make sense to further split up this state machine processing ... */

/**********************************************************************************************************************
 * CddDes_Core_State_TxSync()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
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
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_TxSync(
  void
  )
{
  switch (CddDes_Core_GetSyncTransmissionState())
  {
    case CDDDES_PROTOCOL_SUBSTATE_TX_ENTRY:
    {
      CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_STATUS_CHANGED);
      /* break intentionally omitted as we directly jump into the Wait State! */
    }
    /* fall through */
    case CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_STATUS_CHANGED:     /* PRQA S 2003 */ /* MD_CddDes_2003 */
    {
      boolean transmitCondition = FALSE;
      /*
        possible transitions out of this state are:
          - TransmitCondition reached.
          Master:    >= 1 Event Queued
          Dependent: ProtocolTimeout or EventQueue filled > x%
      */
#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
      if(CddDes_Core_Buffer_GetReadAvailable() > 0)
      {
        transmitCondition = TRUE;
      }
#endif
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
      if ( (CddDes_Core_Buffer_GetReadAvailable() >= CDDDES_CFG_COLLECTION_BUFFER_TRANSMIT_THRESHOLD) 
        || (CddDes_Core_IsTimeoutElapsed(CDDDES_PROTOCOL_TIMER) == TRUE) )
      {
        transmitCondition = TRUE;
      }
#endif
      if (transmitCondition == TRUE)
      {
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_PREPARE_TRANSMIT);
        CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION, CDDDES_CFG_RETRIES_TRANSMISSION);
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
        CddDes_Core_SetTimeoutCounter(CDDDES_PROTOCOL_TIMER, CDDDES_CFG_TIMEOUT_CYCLIC_TRANSMISSION);
#endif
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_TX_PREPARE_TRANSMIT:
    {
      if (CddDes_Core_IsTimeoutElapsed(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == TRUE)
      {
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK);
      }
      else
      {
        if (CddDes_Pdur_LockConnection(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == E_OK)
        {
          CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION, CDDDES_CFG_RETRIES_TRANSMISSION);
          CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_TRANSMIT_REQUEST);
          CddDes_Core_SetMessageType(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION, CDDDES_MESSAGE_SYNC);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_TX_TRANSMIT_REQUEST:
    {
      CddDes_BufferSizeType messageLength = CddDes_Core_Buffer_GetReadAvailable() + CDDDES_MESSAGELENGTH_SYNCHDR;
      CddDes_Core_ClearLastTxConfirmation(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION);
      if (CddDes_Pdur_Transmit(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION, CDDDES_HANDLE_COLLECTION_BUFFER, messageLength) == E_OK)
      {
        CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION, CDDDES_CFG_TIMEOUT_NONE);
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_TX_CONFIRMATION);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == TRUE)
        {
          CddDes_Pdur_ReleaseConnection(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION);
          CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK);
        }
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_TX_CONFIRMATION:
    {
      if (CddDes_Core_GetLastTxConfirmation(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == CDDDES_TXCONFIRMATION_E_OK)
      {
        if (CddDes_Pdur_GetConnectionMessageLength(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) > CDDDES_MESSAGELENGTH_SYNCHDR)
        {
          CddDes_Core_Buffer_IncrementReadIndex(CddDes_Pdur_GetConnectionMessageLength(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) - CDDDES_MESSAGELENGTH_SYNCHDR);
        }
        CddDes_Core_SetTimeoutCounter(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION, CDDDES_CFG_TIMEOUT_RESPONSE);
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_RESPONSE);
      }
      else if (CddDes_Core_GetLastTxConfirmation(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == CDDDES_TXCONFIRMATION_E_NOT_OK)
      {
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK);
      }
      else
      {
        /* still waiting for TxConfirmation... */
      }
      break;
    }

    case CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_RESPONSE:
    {
      /*
        possible transitions out of this state are:
          - Sync Message Received
          - Any other message received
          - Timeout
      */
      if (CddDes_Core_GetLastReceivedMessage(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == CDDDES_MESSAGE_SYNC_ACK)
      {
        /* Expected message received */
        CddDes_Pdur_ReleaseConnection(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION);
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION);
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_OK);
      }
      else if (CddDes_Core_GetLastReceivedMessage(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) != CDDDES_MESSAGE_NONE)
      {
        /* Unexpected message received */
        CddDes_Pdur_ReleaseConnection(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION);
        CddDes_Core_ClearLastReceivedMessage(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION);
        CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK);
      }
      else
      {
        if (CddDes_Core_IsTimeoutElapsed(CDDDES_CORE_SYNC_TRANSMISSION_CONNECTION) == TRUE)
        {
          CddDes_Core_SetSyncTransmissionState(CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK);
        }
      }
      break;
    }

    default:
    {
      break;
    }
  }
}  /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* It does not make sense to further split up this state machine processing ... */

/**********************************************************************************************************************
 * CddDes_Core_State_Connected()
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
 *
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_State_Connected(
  void
  )
{
  boolean leaveState = FALSE;

  /* Check if we are in entry-state? */
  if (CddDes_Core_GetConnectedState() == CDDDES_PROTOCOL_SUBSTATE_ENTRY)
  {
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
    CddDes_Core_SetTimeoutCounter(CDDDES_PROTOCOL_TIMER, CDDDES_CFG_TIMEOUT_CYCLIC_TRANSMISSION);
#endif
    /* Let the individual State-Machines for Rx/Tx do their Job. */
    CddDes_Core_SetSyncReceptionState    (CDDDES_PROTOCOL_SUBSTATE_RX_ENTRY);
    CddDes_Core_SetSyncTransmissionState (CDDDES_PROTOCOL_SUBSTATE_TX_ENTRY);
  }

  /* Do the Sub-State processing */
  CddDes_Core_State_TxSync();  /* for "WaitAndTransmitSyncMessage" */
  CddDes_Core_State_RxSync();  /* for "WaitAndProcessSyncMessage"  */

  /* Do the Repeat until Failure Loop for "WaitAndTransmitSyncMessage" .*/
  if ((CddDes_Core_GetSyncReceptionState()    == CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_OK) &&
      (CddDes_Core_GetSyncTransmissionState() != CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK))
  {
    CddDes_Core_SetSyncReceptionState   (CDDDES_PROTOCOL_SUBSTATE_RX_ENTRY);
  }

  /* Do the Repeat until Failure Loop for "WaitAndProcessSyncMessage" .*/
  if ((CddDes_Core_GetSyncTransmissionState() == CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_OK) &&
      (CddDes_Core_GetSyncReceptionState()    != CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK))
  {
    CddDes_Core_SetSyncTransmissionState   (CDDDES_PROTOCOL_SUBSTATE_TX_ENTRY);
  }

  /* Check Transitions out of this State: */
  if (     (CddDes_Core_GetSyncReceptionState()    == CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK)  /* Rx has come to an unsuccessful stop..   AND ... */
     && (  (CddDes_Core_GetSyncTransmissionState() == CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK)             /* - Tx also failed, or */
        || (CddDes_Core_GetSyncTransmissionState() == CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_OK)                 /* - Tx completed successfully, or */
        || (CddDes_Core_GetSyncTransmissionState() == CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_STATUS_CHANGED))    /* - Tx is only waiting (not processing) */
     )
  {
    leaveState = TRUE;
  }
  /* or... */
  if (     (CddDes_Core_GetSyncTransmissionState() == CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK) /* Tx has come to an unsuccessful stop..   AND ... */
     && (  (CddDes_Core_GetSyncReceptionState()    == CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK)             /* - Rx also failed, or */
        || (CddDes_Core_GetSyncReceptionState()    == CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_OK)                 /* - Rx completed successfully, or */
        || (CddDes_Core_GetSyncReceptionState()    == CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_SYNC_MSG))          /* - Rx is only waiting (not processing) */
     )
  {
    leaveState = TRUE;
  }

  if (leaveState == TRUE)
  {
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
      CddDes_Core_SetTimeoutCounter(CDDDES_PROTOCOL_TIMER, CDDDES_CFG_TIMEOUT_RECONNECT);
#endif
      CddDes_Core_SetNextProtocolProcessingState(CDDDES_PROTOCOL_STATE_INITIALIZED);
  }
}

/**********************************************************************************************************************
 * CddDes_Core_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_TxConfirmation(
  CddDes_ConnectionIdType Connection,
  Std_ReturnType Result
  )
{
  CddDes_Core_SetLastTxConfirmation(Connection, Result);
}

/**********************************************************************************************************************
 * CddDes_Core_RxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_RxIndication(
  CddDes_ConnectionIdType Connection
  )
{
  /* We only get a positive RxIndication. A timeout mechanism must be in place for each received message */
  /* Extract messageType from received message (first byte) */
  CddDes_Core_SetLastReceivedMessage(Connection, CddDes_Core_GetMessageType(Connection));
}

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 *  CddDes_Core_AddDtcPassed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_AddDtcPassed(
  uint32 DTC
  )
{
  (void)CddDes_Core_Buffer_WriteEvent_Uint32Parm(CDDDES_SYNCEVENT_DTC_PASSED, DTC);
}
#endif

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 *  CddDes_Core_AddDtcFailed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_AddDtcFailed(
  uint32 DTC
  )
{
  (void)CddDes_Core_Buffer_WriteEvent_Uint32Parm(CDDDES_SYNCEVENT_DTC_FAILED, DTC);
}
#endif

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
 *  CddDes_Core_AddDtcCleared()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_AddDtcCleared(
  uint32 DTC
  )
{
  (void)CddDes_Core_Buffer_WriteEvent_Uint32Parm(CDDDES_SYNCEVENT_CLEAR_DTC, DTC);
}
#endif

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_InternalGetReadAvailable()
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
CDDDES_LOCAL_INLINE FUNC(CddDes_BufferSizeType, CDDDES_CODE) CddDes_Core_Buffer_InternalGetReadAvailable(
  boolean considerWriteWrap
  )
{
  CddDes_BufferSizeType bytesAvailable;

  if (considerWriteWrap == TRUE)
  {
    /* We have to consider the wrap-arround here.. */
    bytesAvailable = (CDDDES_CFG_COLLECTION_BUFFER_SIZE + CddDes_Core_Buffer_GetWriteIndex()) - CddDes_Core_Buffer_GetReadIndex();
  }
  else
  {
    bytesAvailable = CddDes_Core_Buffer_GetWriteIndex() - CddDes_Core_Buffer_GetReadIndex();
  }

  return bytesAvailable;
}

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_GetReadAvailable()
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
 *
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(CddDes_BufferSizeType, CDDDES_CODE) CddDes_Core_Buffer_GetReadAvailable(
  void
  )
{
  CddDes_BufferSizeType bytesAvailable;
  boolean wrapHappenedBefore = FALSE;
  boolean wrapHappenedAfter = FALSE;
  
  if(CddDes_Core_Buffer_GetReadIndex() > CddDes_Core_Buffer_GetWriteIndex())
  {
    wrapHappenedBefore = TRUE;
  }
  
  bytesAvailable = CddDes_Core_Buffer_InternalGetReadAvailable(wrapHappenedBefore);

  /* Check again if a wrap happened while we were calculating the available bytes.. */
  if (CddDes_Core_Buffer_GetReadIndex() > CddDes_Core_Buffer_GetWriteIndex())
  {
    wrapHappenedAfter = TRUE;
  }

  if(wrapHappenedBefore != wrapHappenedAfter)
  {
    /* This time we want to be sure to get a sane value... */
    CddDes_EnterExclusiveArea_CollectionBuffer();
    
    if (CddDes_Core_Buffer_GetReadIndex() > CddDes_Core_Buffer_GetWriteIndex())
    {
      wrapHappenedAfter = TRUE;
    }
    else
    {
      wrapHappenedAfter = FALSE;
    }

    bytesAvailable = CddDes_Core_Buffer_InternalGetReadAvailable(wrapHappenedAfter);

    CddDes_LeaveExclusiveArea_CollectionBuffer();
  }

  return bytesAvailable;
}

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_GetWriteAvailable()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(CddDes_BufferSizeType, CDDDES_CODE) CddDes_Core_Buffer_GetWriteAvailable(
  void
  )
{
  CddDes_BufferSizeType bytesAvailable;

  if (CddDes_Core_Buffer_GetWriteIndex() < CddDes_Core_Buffer_GetReadIndex())
  {
    bytesAvailable = CddDes_Core_Buffer_GetReadIndex() - CddDes_Core_Buffer_GetWriteIndex();
  }
  else
  {
    /* We have to consider the wrap-arround here.. */
    bytesAvailable = (CDDDES_CFG_COLLECTION_BUFFER_SIZE + CddDes_Core_Buffer_GetReadIndex()) - CddDes_Core_Buffer_GetWriteIndex();
  }

  return bytesAvailable;
}

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_ReadBytesRelative()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Core_Buffer_ReadBytesRelative(
   P2VAR(uint8, AUTOMATIC, CDDDES_VAR) TargetPtr, 
   CddDes_BufferSizeType Offset, 
   CddDes_BufferSizeType Length
   )
{
  /* This function does not perform any parameter checks as it is only called after those parameters have been checked. */

  CddDes_BufferSizeType copyPos;
  for(copyPos=0; copyPos < Length; copyPos++)
  {
    TargetPtr[copyPos] = CddDes_Core_Buffer_ReadByteRelative(Offset + copyPos);
  }

  return E_OK;
}

/**********************************************************************************************************************
 *  CddDes_Core_GetSyncMessageTxPayload()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Core_GetSyncMessageTxPayload(
  P2VAR(uint8, AUTOMATIC, CDDDES_VAR) TargetPtr, 
  CddDes_BufferSizeType Offset, 
  CddDes_BufferSizeType Length
  )
{
  CddDes_BufferSizeType segmentOffset = 0;
  Std_ReturnType retVal = E_NOT_OK;

  /* This function does not perform any parameter checks as it is only called after those parameters have been checked. */
  if ( (Offset < CDDDES_MESSAGELENGTH_SYNCHDR) && (Length >= CDDDES_MESSAGELENGTH_SYNCHDR) )
  {
    TargetPtr[0] = CDDDES_MESSAGE_SYNC;
    segmentOffset = CDDDES_MESSAGELENGTH_SYNCHDR;
    retVal = E_OK;
  }

  if ( ((Offset + segmentOffset) >= CDDDES_MESSAGELENGTH_SYNCHDR) && (Length>segmentOffset) )
  {
    retVal = CddDes_Core_Buffer_ReadBytesRelative(
                   &TargetPtr[segmentOffset],
                   (CddDes_BufferSizeType)((Offset + segmentOffset) - CDDDES_MESSAGELENGTH_SYNCHDR),        /* PRQA S 3382 */ /* MD_CddDes_21.1 */
                   (CddDes_BufferSizeType)(Length - segmentOffset)                                          /* PRQA S 0291 */ /* MD_CddDes_21.1 */
             );
  }

  return retVal;
}

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_WriteEvent_Uint32Parm()
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
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Core_Buffer_WriteEvent_Uint32Parm(
  CddDes_SyncEventType EventType, 
  uint32 Param
  )
{
  const CddDes_BufferSizeType messageLength = sizeof(CddDes_SyncEventType) + CddDes_Core_GetSyncEventLength(EventType); /* Type + Parm */
  Std_ReturnType retVal = E_NOT_OK;

  CddDes_EnterExclusiveArea_CollectionBuffer();

  if (CddDes_Core_Buffer_GetWriteAvailable() > messageLength) /* >: make sure that the WriteIndex does not become equal to the ReadIndex */
  {
    uint8_least bytePos;
    CddDes_Core_Buffer_WriteByteRelative(0, EventType);

    /* Serialize uint32 (LSB to MSB) Byteorder must be considered when restoring the uint32 value! */
    for (bytePos = 0; bytePos < CddDes_Core_GetSyncEventLength(EventType); bytePos++)
    {
      CddDes_Core_Buffer_WriteByteRelative((sizeof(CddDes_SyncEventType) + bytePos), (uint8)Param);
      Param=Param>>8; /* Shift to next byte.*/
    }

    CddDes_Core_Buffer_IncrementWriteIndex(messageLength);

    retVal = E_OK;
  }

  CddDes_LeaveExclusiveArea_CollectionBuffer();

#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (retVal == E_NOT_OK)   /* This may only happen if there was a buffer overflow! */
  {
    CddDes_Det_ReportError(CDDDES_SID_INTERNAL, CDDDES_E_COLLECTION_BUFFER_OVERFLOW)
  }
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  CddDes_Core_Buffer_IncrementReadIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Core_Buffer_IncrementReadIndex(
  CddDes_BufferSizeType Offset
  )
{
  CddDes_EnterExclusiveArea_CollectionBuffer();
  CddDes_CollectionBuffer.ReadIndex = ((CddDes_CollectionBuffer.ReadIndex + (Offset)) % (CDDDES_CFG_COLLECTION_BUFFER_SIZE));  /*!< Increment the ReadIndex by specified amount while considering the wrap-arround. */
  CddDes_LeaveExclusiveArea_CollectionBuffer();
}

/**********************************************************************************************************************
 *  CddDes_Core_ProcessSyncEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDES_LOCAL FUNC(CddDes_AsyncReturnType, CDDDES_CODE) CddDes_Core_ProcessSyncEvent(
  CddDes_SyncEventType EventType,
  uint32 DTC
  )
{
  CddDes_AsyncReturnType retVal = CDDDES_OPERATION_COMPLETED;
  switch (EventType)
  {
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
    case CDDDES_SYNCEVENT_CLEAR_DTC:
    {
      retVal=CddDes_Dem_ClearDtc(DTC);
      break;
    }
#endif
#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
    case CDDDES_SYNCEVENT_DTC_FAILED:
    case CDDDES_SYNCEVENT_DTC_PASSED:
    {
      (void)CddDes_Dem_SetDtcStatus(DTC, EventType);
      break;
    }
#endif
    default:
    {
      /* This should never happen. */
      break;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  CddDes_Core_ProcessReceivedSyncMessage()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
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
CDDDES_LOCAL FUNC(CddDes_AsyncReturnType, CDDDES_CODE) CddDes_Core_ProcessReceivedSyncMessage(void)
{
  CddDes_AsyncReturnType retVal = CDDDES_OPERATION_PENDING_INTERNALLY;

  while ((retVal == CDDDES_OPERATION_PENDING_INTERNALLY) &&
         (CddDes_Core_GetSyncMessageProcessingIndex() < CddDes_Core_GetSyncMessageLength()))
  {
    CddDes_SyncEventType eventType = CddDes_Core_GetSyncMessageCurrentEventType();
    CddDes_BufferSizeType eventSize;

    /*
      Validity Check for Master / Dependent is different.
      Master supports: DTC_FAILED, DTC_PASSED
      Dependent support: CLEAR_DTC
    */
#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
    if ((eventType == CDDDES_SYNCEVENT_DTC_FAILED) ||
        (eventType == CDDDES_SYNCEVENT_DTC_PASSED))
#endif
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
    if (eventType == CDDDES_SYNCEVENT_CLEAR_DTC)
#endif
    {
      eventSize = CddDes_Core_GetSyncEventLength(eventType);
      
      if (CddDes_Core_GetSyncMessageEndOfCurrentEvent(eventSize) <= CddDes_Core_GetSyncMessageLength())
      {
        uint32 DTC;

        DTC = CddDes_Util_ReadUIntFromBuffer(eventSize, CddDes_Core_GetSyncMessageCurrentEventPayloadPtr());

        if (CddDes_Core_ProcessSyncEvent(eventType, DTC) == CDDDES_OPERATION_COMPLETED)
        {
          CddDes_Core_SetSyncMessageProcessingIndex(CddDes_Core_GetSyncMessageEndOfCurrentEvent(eventSize));
        }
        else
        {
          retVal = CDDDES_OPERATION_PENDING; /* Continiue in the next task cycle */
        }
      }
      else /* Message too short*/
      {
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
        /* Report error to DET */
        CddDes_Det_ReportError(CDDDES_SID_INTERNAL, CDDDES_E_RECEIVED_MESSAGE_INVALID)
#endif
        retVal = CDDDES_OPERATION_COMPLETED;
        CddDes_Core_SetMessageType(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_MESSAGE_NACK);
      }
    }
    else /* Invalid Event Type */
    {
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
      /* Report error to DET */
      CddDes_Det_ReportError(CDDDES_SID_INTERNAL, CDDDES_E_RECEIVED_MESSAGE_INVALID)
#endif
      retVal = CDDDES_OPERATION_COMPLETED;
      CddDes_Core_SetMessageType(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_MESSAGE_NACK);
    }
  }

  if (retVal == CDDDES_OPERATION_PENDING_INTERNALLY) /* We exited the loop because we reached the end of the message */
  {
    retVal = CDDDES_OPERATION_COMPLETED;
    CddDes_Core_SetMessageType(CDDDES_CORE_SYNC_RECEPTION_CONNECTION, CDDDES_MESSAGE_SYNC_ACK);
  }

  return retVal;
}

/**********************************************************************************************************************
 * CddDes_Dem_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDES_LOCAL FUNC(void, CDDDES_CODE) CddDes_Dem_Init(
  void
  )
{
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
  CddDes_Dem_DtcToClear = CDDDES_DEM_DTC_TO_CLEAR_NONE;
#endif
} 

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
* CddDes_Dem_SetDtcStatus()
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
CDDDES_LOCAL FUNC(Std_ReturnType, CDDDES_CODE) CddDes_Dem_SetDtcStatus(
  uint32 DTC,
  CddDes_SyncEventType Result
)
{
  Std_ReturnType lResult;
  
  lResult = Dem_SelectDTC(CDDDES_CFG_DEM_CLIENT_ID, DTC, DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY);

  if (lResult == E_OK)
  {
    Dem_EventIdType     lEventId;
    Dem_EventStatusType lEventStatus;

    lResult = Dem_GetEventIdOfDTC(CDDDES_CFG_DEM_CLIENT_ID, &lEventId);

    if (lResult == E_OK)
    {
      if (Result == CDDDES_SYNCEVENT_DTC_FAILED)
      {
        lEventStatus = DEM_EVENT_STATUS_FAILED;
      }
      else
      {
        lEventStatus = DEM_EVENT_STATUS_PASSED;
      }

      lResult = Dem_SetEventStatus(lEventId, lEventStatus);
    }
  }
  return lResult;
}
#endif

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
* CddDes_Dem_ClearDtc()
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
CDDDES_LOCAL FUNC(CddDes_AsyncReturnType, CDDDES_CODE) CddDes_Dem_ClearDtc(
  uint32 DTC
)
{
  CddDes_AsyncReturnType lResult = CDDDES_OPERATION_COMPLETED;

  if (CddDes_Dem_DtcToClear == CDDDES_DEM_DTC_TO_CLEAR_NONE)
  {
    Std_ReturnType lSelectDtcResult;

    lSelectDtcResult = Dem_SelectDTC(CDDDES_CFG_DEM_CLIENT_ID, DTC, DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY);

    if (lSelectDtcResult == E_OK)
    {
      CddDes_Dem_DtcToClear = DTC;
    }
  }
  
  if (CddDes_Dem_DtcToClear == DTC)
  {
    Std_ReturnType lClearDtcResult;

    lClearDtcResult = Dem_ClearDTC(CDDDES_CFG_DEM_CLIENT_ID);

    if ( (lClearDtcResult == DEM_CLEAR_BUSY)
      || (lClearDtcResult == DEM_PENDING) )
    {
      lResult = CDDDES_OPERATION_PENDING;
    }
    else
    {
      CddDes_Dem_DtcToClear = CDDDES_DEM_DTC_TO_CLEAR_NONE;
      /* CDDDES_OPERATION_COMPLETED initially set */
    }
  }
  
  return lResult;
}
#endif

#define CDDDES_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CDDDES_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CddDes_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CDDDES_CODE) CddDes_InitMemory(
  void
  )
{
  /* ----- Implementation ----------------------------------------------- */
  CddDes_Init_State = CDDDES_UNINIT;
}

/**********************************************************************************************************************
 *  CddDes_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CDDDES_CODE) CddDes_Init(
  P2CONST(CddDes_ConfigType, AUTOMATIC, CDDDES_PBCFG) ConfigPtr
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()
  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
/* Check initialization state of the component */
  if (CddDes_Init_State == CDDDES_INIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_ALREADY_INITIALIZED)
  }
  else if (ConfigPtr != NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_VALUE)
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDes_Pdur_Init();
    CddDes_Core_Init();
    CddDes_Dem_Init();

    CddDes_Init_State = CDDDES_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_INIT, CddDes_Det_ErrorRegisterGet())
  }
#endif

  CDDDES_IGNORE_UNREF_PARAM(ConfigPtr);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}

/**********************************************************************************************************************
 *  CddDes_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDES_CODE) CddDes_MainFunction(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
/* Check initialization state of the component */
  if (CddDes_Init_State == (uint8)CDDDES_UNINIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (CddDes_Init_State == (uint8)CDDDES_INIT)
    {
      CddDes_Core_MainFunction();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_MAINFUNCTION, CddDes_Det_ErrorRegisterGet())
  }
#endif
}

# if (CDDDES_SUPPORT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CddDes_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDES_CODE) CddDes_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CDDDES_APPL_VAR) VersionInfo
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID = (CDDDES_VENDOR_ID);
    VersionInfo->moduleID = (CDDDES_MODULE_ID);
    VersionInfo->sw_major_version = (CDDDES_SW_MAJOR_VERSION);
    VersionInfo->sw_minor_version = (CDDDES_SW_MINOR_VERSION);
    VersionInfo->sw_patch_version = (CDDDES_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_GET_VERSION_INFO, CddDes_Det_ErrorRegisterGet())
  }
# endif
}
# endif

/**********************************************************************************************************************
 *  CddDes_TpTxConfirmation()
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
FUNC(void, CDDDES_CODE) CddDes_TpTxConfirmation(
  PduIdType CddDesTxPduId,
  Std_ReturnType Result
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State == (uint8)CDDDES_UNINIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else if (CddDesTxPduId >= CDDDES_CFG_NUM_TX_PDUS)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PDU_ID_TX_OUT_OF_RANGE)
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDes_ConnectionIdType lConnection;

    lConnection = CddDes_Cfg_GetConnectionFromTxPdu(CddDesTxPduId);

#if (CDDDES_DEV_ERROR_DETECT == STD_ON)                                                                                                                
    if (CddDes_Pdur_GetConnectionState(lConnection) != CDDDES_PDUR_TX_COPY)
    {
      CddDes_Det_ErrorRegisterSet(CDDDES_E_ILLEGAL_STATE)
    }
    else
#endif
    {
      CddDes_Core_TxConfirmation(lConnection, Result);
      CddDes_Pdur_SetConnectionState(lConnection, CDDDES_PDUR_IDLE);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_TX_CONFIRMATION, CddDes_Det_ErrorRegisterGet())
  }
#endif  
}

/**********************************************************************************************************************
 *  CddDes_StartOfReception()
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
 *
 */
FUNC(BufReq_ReturnType, CDDDES_CODE) CddDes_StartOfReception(
  PduIdType CddDesRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CDDDES_APPL_DATA) PduInfoPtr,           /* PRQA S 3673 */ /* MD_CddDes_3673 */
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, CDDDES_APPL_DATA) BufferSizePtr
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State != CDDDES_INIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else if (CddDesRxPduId >= CDDDES_CFG_NUM_RX_PDUS)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PDU_ID_RX_OUT_OF_RANGE)
  }
  else if (PduInfoPtr != NULL_PTR)
  { /* infoPool and meta-data not supported yet -> must always be NULL_PTR for CddDes PDUs */
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else if (BufferSizePtr == NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else
#endif
  { 
    /* ----- Implementation ----------------------------------------------- */
    CddDes_ConnectionIdType lConnection;

    lConnection = CddDes_Cfg_GetConnectionFromRxPdu(CddDesRxPduId);

    /* Proceed if no other reception is pending and message fits into the buffer */
    if (CddDes_Pdur_GetConnectionState(lConnection) == CDDDES_PDUR_IDLE)
    {
      if (CddDes_Cfg_GetConnectionTpBufferSize(lConnection) < TpSduLength)
      { /* Too few buffer */
        lResult = BUFREQ_E_OVFL;
        CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_CONFIG)
      }
      else
      { /* Set state to not idle anymore, this locks the connection so no other reception/transmission on this connection is possible.
           Additionally store the message length and reset the index into the buffer. */
        CddDes_Pdur_SetConnectionState(lConnection, CDDDES_PDUR_RX_COPY);
        CddDes_Pdur_SetConnectionMessageLength(lConnection, (CddDes_BufferSizeType)TpSduLength);
        CddDes_Pdur_SetConnectionBufferIndex(lConnection, 0);

        *BufferSizePtr = CddDes_Cfg_GetConnectionTpBufferSize(lConnection);

        lResult = BUFREQ_OK;
      }
    }
  }
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_START_OF_RECEPTION, CddDes_Det_ErrorRegisterGet())
  }
#endif
  return lResult;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CddDes_CopyRxData()
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
 *
 */
FUNC(BufReq_ReturnType, CDDDES_CODE) CddDes_CopyRxData(
  PduIdType CddDesRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CDDDES_APPL_DATA) PduInfoPtr,            /* PRQA S 3673 */ /* MD_CddDes_3673 */
  P2VAR(PduLengthType, AUTOMATIC, CDDDES_APPL_DATA) BufferSizePtr
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State != CDDDES_INIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else if (CddDesRxPduId >= CDDDES_CFG_NUM_RX_PDUS)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PDU_ID_RX_OUT_OF_RANGE)
  }
  else if (PduInfoPtr == NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else if ((PduInfoPtr->SduLength != 0)
            && (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else if (BufferSizePtr == NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDes_ConnectionIdType lConnection;

    lConnection = CddDes_Cfg_GetConnectionFromRxPdu(CddDesRxPduId);

#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
    if (CddDes_Pdur_GetConnectionState(lConnection) != CDDDES_PDUR_RX_COPY)
    {
      CddDes_Det_ErrorRegisterSet(CDDDES_E_ILLEGAL_STATE)
    }
    else if ((CddDes_Pdur_GetConnectionBufferIndex(lConnection) + PduInfoPtr->SduLength) >  CddDes_Pdur_GetConnectionMessageLength(lConnection))
    {
      CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_VALUE)
    }
    else
#endif
    {
      CddDes_BufferSizeType lBufferIndex = CddDes_Pdur_GetConnectionBufferIndex(lConnection);

      if (PduInfoPtr->SduLength != 0u) /* need to copy any payload now? */
      { /* Copy current data portion */
        VStdMemCpy(&CddDes_Pdur_ConnectionContext[lConnection].BufferPtr[lBufferIndex],
                   &PduInfoPtr->SduDataPtr[0],
                    PduInfoPtr->SduLength);

        lBufferIndex += (CddDes_BufferSizeType)PduInfoPtr->SduLength;  /* Update next position */
        CddDes_Pdur_SetConnectionBufferIndex(lConnection, lBufferIndex);
      }

      *BufferSizePtr = (PduLengthType)(CddDes_Pdur_GetConnectionMessageLength(lConnection) - lBufferIndex); /* Notify lower layer about available buffer space */
      lResult = BUFREQ_OK;
    }
  }

#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_COPY_RX_DATA, CddDes_Det_ErrorRegisterGet())
  }
#endif
  return lResult;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CddDes_CopyTxData()
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
FUNC(BufReq_ReturnType, CDDDES_CODE) CddDes_CopyTxData(
  PduIdType CddDesTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CDDDES_APPL_DATA) PduInfoPtr,           /* PRQA S 3673 */ /* MD_CddDes_3673 */
  P2VAR(RetryInfoType, AUTOMATIC, CDDDES_APPL_DATA) Retry,              /* PRQA S 3673 */ /* MD_CddDes_3673 */
  P2VAR(PduLengthType, AUTOMATIC, CDDDES_APPL_DATA) AvailableDataPtr
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State != CDDDES_INIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else if (CddDesTxPduId >= CDDDES_CFG_NUM_TX_PDUS)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PDU_ID_TX_OUT_OF_RANGE)
  }
  else if (PduInfoPtr == NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else if ( (PduInfoPtr->SduLength != 0)
         && (PduInfoPtr->SduDataPtr == NULL_PTR) )
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else if (AvailableDataPtr == NULL_PTR)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_POINTER)
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDes_ConnectionIdType lConnection;

    lConnection = CddDes_Cfg_GetConnectionFromTxPdu(CddDesTxPduId);

#if (CDDDES_DEV_ERROR_DETECT == STD_ON)                                                                                                                
    if (CddDes_Pdur_GetConnectionState(lConnection) != CDDDES_PDUR_TX_COPY)
    {
      CddDes_Det_ErrorRegisterSet(CDDDES_E_ILLEGAL_STATE)
    }
    else if ((PduInfoPtr->SduLength != 0u)
      && (CddDes_Pdur_GetConnectionBufferIndex(lConnection) >= CddDes_Pdur_GetConnectionMessageLength(lConnection)))
    { /* no more data to copy */
      CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_VALUE)
    }
    else if ((PduLengthType)(CddDes_Pdur_GetConnectionMessageLength(lConnection) - CddDes_Pdur_GetConnectionBufferIndex(lConnection)) < PduInfoPtr->SduLength)
    {
      CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_VALUE)
    }
    else
#endif
    {
      CddDes_BufferSizeType lBufferIndex = CddDes_Pdur_GetConnectionBufferIndex(lConnection);
      
      if (CddDes_Pdur_GetConnectionTxBufferHandle(lConnection) == CDDDES_HANDLE_TP_BUFFER)
      {
        /* Copy current data portion */
        VStdMemCpy(PduInfoPtr->SduDataPtr, 
                   &CddDes_Pdur_GetConnectionTpBufferPtr(lConnection)[lBufferIndex],
                   PduInfoPtr->SduLength);
      }
      else
      {
        (void)CddDes_Core_GetSyncMessageTxPayload(PduInfoPtr->SduDataPtr, lBufferIndex, PduInfoPtr->SduLength);
      }

      lBufferIndex += (CddDes_BufferSizeType)PduInfoPtr->SduLength;  /* Update next position */
      CddDes_Pdur_SetConnectionBufferIndex(lConnection, lBufferIndex);

      *AvailableDataPtr = (PduLengthType)(CddDes_Pdur_GetConnectionMessageLength(lConnection) - lBufferIndex); /* Notify lower layer about remaining data size */
      lResult = BUFREQ_OK;
    }
  }

  /* ----- Development Error Report -------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_COPY_TX_DATA, CddDes_Det_ErrorRegisterGet())
  }
#endif

  CDDDES_IGNORE_UNREF_PARAM(Retry);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return lResult;
} /* PRQA S 6080, 6030 */ /* MD_MSR_STMIF, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  CddDes_TpRxIndication()
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
 */
FUNC(void, CDDDES_CODE) CddDes_TpRxIndication(
  PduIdType CddDesRxPduId,
  Std_ReturnType Result
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State == (uint8)CDDDES_UNINIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else if (CddDesRxPduId >= CDDDES_CFG_NUM_RX_PDUS)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PDU_ID_RX_OUT_OF_RANGE)
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDes_ConnectionIdType lConnection;

    lConnection = CddDes_Cfg_GetConnectionFromRxPdu(CddDesRxPduId);

#if (CDDDES_DEV_ERROR_DETECT == STD_ON)
    if (CddDes_Pdur_GetConnectionState(lConnection) != CDDDES_PDUR_RX_COPY)
    {
      CddDes_Det_ErrorRegisterSet(CDDDES_E_ILLEGAL_STATE)
    }
    else
#endif
    {
      /* Change state to wait for a transmission and notify Core sub-module if recption was successful otherwise release connection */
      if (Result == E_OK)
      {
        CddDes_Pdur_SetConnectionState(lConnection, CDDDES_PDUR_TX_WAIT);
        CddDes_Core_RxIndication(lConnection);
      }
      else
      {
        CddDes_Pdur_SetConnectionState(lConnection, CDDDES_PDUR_IDLE);
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_RX_INDICATION, CddDes_Det_ErrorRegisterGet())
  }
#endif
}

/* == DEM Interface == */
#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 *  CddDes_DtcStatusChanged()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDES_CODE) CddDes_DtcStatusChanged(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

    /* ----- Development Error Checks ------------------------------------- */
# if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State == (uint8)CDDDES_UNINIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least lStatusTrigger;
    uint16 lDtcStatusOldNew;

    lDtcStatusOldNew = CddDes_Util_Make16Bit((uint8)(DTCStatusOld & CDDDES_DEM_DTC_STATUS_BYTE_MASK), (uint8)(DTCStatusNew & CDDDES_DEM_DTC_STATUS_BYTE_MASK));
      
    /* check if the DTC status change is a relevant trigger */
    for (lStatusTrigger = 0; lStatusTrigger < CDDDES_DEM_NUM_STATUS_CHANGES; lStatusTrigger++)
    {
      if (CddDes_Dem_DtcStatusTriggerTable[lStatusTrigger].DtcStatusOldNew == lDtcStatusOldNew)
      {
        if (CddDes_Dem_DtcStatusTriggerTable[lStatusTrigger].Trigger == CDDDES_DEM_TRIGGER_PASSED)
        {
          CddDes_Core_AddDtcPassed(DTC);
        }
        else /* Trigger Failed */
        {
          CddDes_Core_AddDtcFailed(DTC);
        }
        break;  /* leave the loop since only one trigger is possible */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_DTC_STATUS_CHANGED, CddDes_Det_ErrorRegisterGet())
  }
# endif

  return E_OK;
}
#endif

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
 *  CddDes_ClearDtcNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDES_CODE) CddDes_ClearDtcNotification(
  uint32 DTC,
  Dem_DTCFormatType DTCFormat,
  Dem_DTCOriginType DTCOrigin
  )
{
   /* ----- Local Variables ---------------------------------------------- */
  CddDes_Det_ErrorRegisterDefine()

  CddDes_Det_ErrorRegisterSet(CDDDES_E_NO_ERROR)

    /* ----- Development Error Checks ------------------------------------- */
# if (CDDDES_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDes_Init_State == (uint8)CDDDES_UNINIT)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_UNINIT)
  }
  else if (DTCFormat != DEM_DTC_FORMAT_UDS)
  {
    CddDes_Det_ErrorRegisterSet(CDDDES_E_PARAM_VALUE)
  }
  else
# endif
  {
    if (DTCOrigin == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
    {
      CddDes_Core_AddDtcCleared(DTC);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CDDDES_DEV_ERROR_REPORT == STD_ON)
  if (CddDes_Det_ErrorRegisterGet() != CDDDES_E_NO_ERROR)
  {
    CddDes_Det_ReportError(CDDDES_SID_DTC_CLEAR_NOTIFICATION, CddDes_Det_ErrorRegisterGet())
  }
# endif

  CDDDES_IGNORE_UNREF_PARAM(DTCFormat);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return E_OK;
}
#endif

/**********************************************************************************************************************
* CddDes_Util_ReadUIntFromBuffer()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
CDDDES_LOCAL FUNC(uint32, CDDDES_CODE) CddDes_Util_ReadUIntFromBuffer(
  CddDes_BufferSizeType NoBytes,
  P2CONST(uint8, AUTOMATIC, CDDDES_APPL_DATA) Buffer
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least sourceBytePos;
  uint32 retVal = 0;

  /* ----- Implementation ----------------------------------------------- */

  /* Assemble uint value from buffer, in LSB to MSB format */
  for (sourceBytePos = 0; sourceBytePos < NoBytes; sourceBytePos++) /* the LSB is the first byte we read */
  {
    retVal += ((uint32)Buffer[sourceBytePos]) << (8 * sourceBytePos);
  }

  return retVal;
}

#define CDDDES_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_CddDes_3673: 
       Description: Rule 16.7
                    A pointer parameter in a function prototype should be declared as pointer to 
                    const if the pointer is not used to modify the addressed object.
      Reason:       The function signature is defined by AUTOSAR standard and therefore may not be changed.
      Risk:         Inefficient code generation.
      Prevention:   None.

 MD_CddDes_2003: 
      Description: Rule 15.2
                   The preceding non-empty 'case' or 'default' clause does not end with an explicit 
                   'break' or 'return' statement.
      Reason:      Often states are placed as "logical step" in the processing that do have a very short
                   execution time and have exactly one following state. The state machine implementation immediately
                   transitions into the next state of the state machine to reduce processing delays.
      Risk:        Control flow might not be as intended.
      Prevention:  Code inspection.

 MD_CddDes_8.7:
      Description: Rule 8.7
                   Objects shall be defined at block scope if they are only accessed from within a single function.
      Reason:      Vector style guide prevents usage of static variables in function scope
      Risk:        None.
      Prevention:  None.

 MD_CddDes_21.1:
      Description: Rule 21.1
                   Minimisation of run-time failures shall be ensured by the use of at least one of (a) static analysis
                   tools/techniques; (b) dynamic analysis tools/techniques; (c) explicit coding of checks to handle
                   run-time faults.
      Reason:      Unsigned arithmetic apparently results in wraparound past zero, 
                   an integer expression with a value that is apparently negative is being converted to an unsigned type.
      Risk:        None.
      Prevention:  Code inspection verified that the apparent unintended behaviors cannot occur. Additional runtime checks
                   are implemented.

*/

/**********************************************************************************************************************
 *  END OF FILE: CddDes.c
 *********************************************************************************************************************/