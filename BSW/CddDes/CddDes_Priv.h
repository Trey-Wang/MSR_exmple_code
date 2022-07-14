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
/*! \file  CddDes_Priv.h
 *   \brief  CddDes private type definitions
 *
 *   \details  CddDes definitions of structs and enumerations and simple types.
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

#ifndef CDDDES_PRIV_H
# define CDDDES_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# if !defined (CDDDES_LOCAL)
#  define CDDDES_LOCAL static
# endif

# if !defined (CDDDES_LOCAL_INLINE)
#  define CDDDES_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Connection ID types */
typedef uint8    CddDes_ConnectionIdType;
# define CDDDES_INVALID_CONNECTION_ID                  (0x02u)  /*!< Invalid CddDes connection */

typedef uint16 CddDes_BufferSizeType;                           /*!< Buffer size type */

/*! Buffer handle types */
typedef uint8 CddDes_BufferHandleType;
# define CDDDES_HANDLE_TP_BUFFER                       (0)      /*!< Buffer handle for TP buffer */
# define CDDDES_HANDLE_COLLECTION_BUFFER               (1)      /*!< Buffer handle for collection buffer */
# define CDDDES_HANDLE_INVALID                         (2)      /*!< Invalid buffer handle */

/*! Connection management data */
typedef struct CddDes_ConnectionContextType_s
{
  P2VAR(uint8, AUTOMATIC, CDDDES_VAR)  BufferPtr;   /*!< Pointer to the TP buffer */
  CddDes_BufferSizeType Length;                     /*!< Total rx/tx message size */
  CddDes_BufferSizeType Index;                      /*!< Current index in rx/tx buffer */
  CddDes_BufferHandleType TxBufferHandle;           /*!< Handle for the buffer that is used for data transmission */
  uint8 State;                                      /*!< Current connection state */
} CddDes_ConnectionContextType;


/*! Message Type: The first byte of a transmitted / received message */
typedef uint8 CddDes_MsgTypeType;
# define CDDDES_MESSAGE_ACK_FLAG                                     (0x40u)    /*!< Any positive response to a request is the request type | 0x40, similar to UDS */
# define CDDDES_MESSAGE_NONE                                         (0xFFu)    /*!< Used to indicate that no message was received. Last received message is set to this type after message reception was handled by the state machine. */
# define CDDDES_MESSAGE_REQUEST_CONNECTION                           (0x01u)    /*!< Request Connection. Payload: 1 Byte: Protocol version */
# define CDDDES_MESSAGE_REQUEST_CONNECTION_ACK                       (CDDDES_MESSAGE_REQUEST_CONNECTION | CDDDES_MESSAGE_ACK_FLAG)  /*!< Request Connection Acknowledgment message */
# define CDDDES_MESSAGE_SYNC                                         (0x02u)    /*!< Status has changed. Payload: n Byte, depending on actual changes. */
# define CDDDES_MESSAGE_SYNC_ACK                                     (CDDDES_MESSAGE_SYNC | CDDDES_MESSAGE_ACK_FLAG) /*!< Status Changed Acknowledgment message */
# define CDDDES_MESSAGE_NACK                                         (0x7Fu)    /*!< Negative Acknowledgment: The received message was not expected or could not be processed. TBD: Connection is reset. */
# define CDDDES_MESSAGE_TYPE_MASK                                    (0x7Fu)    /*!< Mask the message Type to avoid that a 0xFF message type is ever set as pending message (indicating MESSAGE_NONE). */

# define CDDDES_MESSAGELENGTH_ACK                                    (1u)    /*!< Length of an acknowledgment message: only contains the MessageTypeId|0x40 */
# define CDDDES_MESSAGELENGTH_NACK                                   (1u)    /*!< Length of a negative acknowledgment message: only contains 0x7F */
# define CDDDES_MESSAGELENGTH_REQUEST_CONNECTION                     (2u)    /*!< Length of an request connection message: contains one byte: Protocol Version . */
# define CDDDES_MESSAGELENGTH_SYNCHDR                                (1u)    /*!< Length of the header of a SYNC message */

/*! TxConfirmationResult Type: Remember the last received TxConfirmation or 'none' */
typedef uint8 CddDes_TxConfirmationResultType;
# define CDDDES_TXCONFIRMATION_NONE                                  (0xAB)     /*!< No TxConfirmation received yet */
# define CDDDES_TXCONFIRMATION_E_OK                                  (E_OK)     /*!< The last received TxConfirmation had result E_OK */
# define CDDDES_TXCONFIRMATION_E_NOT_OK                              (E_NOT_OK) /*!< The last received TxConfirmation had result E_NOT_OK */

/*! The CddDes StateMachine is structured hierarchically. Each level has its own variable to store the currently processed (sub)state */
typedef uint8 CddDes_ProtocolProcessingStateType;
# define CDDDES_PROTOCOL_STATE_INVALID                               (0xFFu)  /*!< The State-Machine is in an invalid state. */

/* ===== TOP LEVEL PROTOCOL STATE MACHINE STATES ====== */
/* Main States are generic for Master and Dependent: */
# define CDDDES_PROTOCOL_STATE_INITIALIZED                           (0x01u)  /*!< State INITIALIZED. Wait on request connection message (Master) or timeout (Dependent). Transitions to: ESTABLISH_CONNECTION and TRANSMIT_NACK */
# define CDDDES_PROTOCOL_STATE_ESTABLISH_CONNECTION                  (0x02u)  /*!< State ESTABLISH_CONNECTION. See Substates. Transitions to: INITIAL_SYNCHRONIZATION and INITIALIZED */
# define CDDDES_PROTOCOL_STATE_INITIAL_SYNCHRONIZATION               (0x03u)  /*!< State INITIAL_SYNCHRONIZATION. See Substates. Transitions to: CONNECTED and INITIALIZED */
# define CDDDES_PROTOCOL_STATE_CONNECTED                             (0x04u)  /*!< State CONNECTED. See Substates. Transitions to: INITIALIZED */
# define CDDDES_PROTOCOL_STATE_TRANSMIT_NACK                         (0x08u)  /*!< State TRANSMIT_NACK. The CddDes tries to transmit an NACK message. Transitions to: INITIALIZED */

/* ===== SUB LEVEL PROTOCOL STATE MACHINE STATES ====== */

/* Those states are generic for all the sub-state machines: */
# define CDDDES_PROTOCOL_SUBSTATE_INVALID                            (CDDDES_PROTOCOL_STATE_INVALID)  /*!< The State-Machine is in an invalid state. */
# define CDDDES_PROTOCOL_SUBSTATE_ENTRY                              (0x00u)  /*!< The top-level state that includes this sub-level state-machine was just entered. When entering a Top-Level state the sub-level state shall be set to this 'entry' state. Automatically done when using the SetNextProtocolProcessingState macro. */
# define CDDDES_PROTOCOL_SUBSTATE_EXIT_OK                            (0xAAu)  /*!< The sub-state machine completed processing (successfully). Evaluated in the respective the top-level state. */
# define CDDDES_PROTOCOL_SUBSTATE_EXIT_NOT_OK                        (0xBBu)  /*!< The sub-state machine completed processing (without success). Evaluated in the respective the top-level state. */

  /* Sub State ESTABLISH_CONNECTION (Master) */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_ACK              (0x21u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes tries to transmit an ACK message. Transitions to: WAIT_TX_CONFIRMATION */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_TX_CONFIRMATION      (0x22u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes waits until its ACK message was transmitted. Transitions to: (Master) ASSEMBLE_INIT_SYNC, (Dependent) WAIT_RESPONSE and (Both) EXIT_NOT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_ASSEMBLE_INIT_SYNC        (0x23u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes requests the initial state from the Dem and writes it to the status sync buffer. Transitions to: EXIT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_NACK             (0x24u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes tries to transmit an NACK message. Transitions to: WAIT_NACK_TX_CONF */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_NACK_TX_CONF         (0x25u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes waits until its NACK message was transmitted. Transitions to: EXIT_NOT_OK */

  /* Sub State ESTABLISH_CONNECTION (Dependent) */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_REQUEST_CONNECTION        (0x24u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes tries to lock the PduR Buffer and, if successful assembles the request connection message. Transitions to: TRANSMIT_REQUEST */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_TRANSMIT_REQUEST          (0x25u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes tries to transmit an ACK message. Transitions to: WAIT_TX_CONFIRMATION */
/* CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_TX_CONFIRMATION is shared with Master */
# define CDDDES_PROTOCOL_SUBSTATE_EST_CON_WAIT_RESPONSE             (0x26u)  /*!< Sub-State (used in ESTABLISH_CONNECTION). The CddDes waits until a response is received from the Master. Transitions to: EXIT_OK and EXIT_NOT_OK (Timeout, Nack) */

/* Sub State STATE_CONNECTED and INITIAL_SYNCHRONIZATION share the same sub-states... */
/* Here we have to be careful with the nibbles as two separate state-machines are stored in the same byte..
   Transmission is HighNibbel, Reception is LowNibble */

# define CDDDES_PROTOCOL_SUBSTATE_RX_ENTRY                           (0x03u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: WAIT_SYNC_MSG */ /* We cannot use 0x00 here as this is used for entering the actual sub-state */
# define CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_SYNC_MSG                   (0x04u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: PROCESS_SYNC_MSG and EXIT_NOT_OK (Timeout) */
# define CDDDES_PROTOCOL_SUBSTATE_RX_PROCESS_SYNC_MSG                (0x05u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). See Sub-Sub-State Message Processing. Transitions to: TRANSMIT_RESPONSE */
# define CDDDES_PROTOCOL_SUBSTATE_RX_TRANSMIT_RESPONSE               (0x06u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: WAIT_TX_CONFIRMATION and EXIT_NOT_OK (Retries exceeded) */
# define CDDDES_PROTOCOL_SUBSTATE_RX_WAIT_TX_CONFIRMATION            (0x07u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: EXIT_OK and EXIT_NOT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_OK                         (0x0Au)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). One Sync message received and processed successfully */
# define CDDDES_PROTOCOL_SUBSTATE_RX_EXIT_NOT_OK                     (0x0Bu)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Timeout or any other error */

# define CDDDES_PROTOCOL_SUBSTATE_TX_ENTRY                           (0x30u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: WAIT_STATUS_CHANGED */
# define CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_STATUS_CHANGED             (0x40u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: PREPARE_TRANSMIT*/
# define CDDDES_PROTOCOL_SUBSTATE_TX_PREPARE_TRANSMIT                (0x50u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: TRANSMIT_REQUEST, EXIT_NOT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_TX_TRANSMIT_REQUEST                (0x60u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: WAIT_TX_CONFIRMATION, EXIT_NOT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_TX_CONFIRMATION            (0x70u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: WAIT_RESPONSE , EXIT_NOT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_TX_WAIT_RESPONSE                   (0x80u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Transitions to: EXIT_OK and EXIT_NOT_OK */
# define CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_OK                         (0xA0u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). One Sync message received and processed successfully */
# define CDDDES_PROTOCOL_SUBSTATE_TX_EXIT_NOT_OK                     (0xB0u)  /*!< Sub-State (used in CONNECTED, INITAL SYNC). Timeout or any other error */

/* Sub State TRANSMIT_NACK may transmit a NACK on the dependent connection, master connection, or both. */
/* Therefore we use two separate state-machines are stored in the same byte..
   Dependent is HighNibbel, Dependent is LowNibble */

# define CDDDES_PROTOCOL_SUBSTATE_MASTER_TRANSMIT_NACK               (0x30u)  /*!< Sub-State (used in TRANSMIT_NACK). Transitions to: MASTER_WAIT_TXCONF */
# define CDDDES_PROTOCOL_SUBSTATE_MASTER_WAIT_TX_CONFIRMATION        (0x40u)  /*!< Sub-State (used in TRANSMIT_NACK). Transitions to: MASTER_EXIT */
# define CDDDES_PROTOCOL_SUBSTATE_MASTER_EXIT                        (0xA0u)  /*!< Sub-State (used in TRANSMIT_NACK). Final State */

# define CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_TRANSMIT_NACK            (0x03u)  /*!< Sub-State (used in TRANSMIT_NACK). Transitions to: DEPENDENT_WAIT_TXCONF */
# define CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_WAIT_TX_CONFIRMATION     (0x04u)  /*!< Sub-State (used in TRANSMIT_NACK). Transitions to: DEPENDENT_EXIT */
# define CDDDES_PROTOCOL_SUBSTATE_DEPENDENT_EXIT                     (0x0Au)  /*!< Sub-State (used in TRANSMIT_NACK). Final State */

/* ===== SUB SUB LEVEL PROTOCOL STATE MACHINE STATES ====== */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_INVALID                   (CDDDES_PROTOCOL_STATE_INVALID)   /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_ENTRY_POINT               (CDDDES_PROTOCOL_SUBSTATE_ENTRY)  /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_READY                     (0x51u)                           /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_READ_NEXT                 (0x52u)                           /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_DISPATCH_DEM              (0x53u)                           /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_WAIT_TASK_CYCLE           (0x54u)                           /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */
# define CDDDES_PROCESS_SYNC_MESSAGE_STATE_EXIT_POINT                (CDDDES_PROTOCOL_SUBSTATE_EXIT)   /*!< Sub-Sub-State (used in CONNECTED, INITAL SYNC RX). TBD TODO TBD */

/*! Counter type used for timeouts and retries */
typedef uint16 CddDes_TimeoutCounterType;

/*! Type for qualified DTC test result */
typedef uint8 CddDes_Dem_DtcStatusTriggerType;
# define CDDDES_DEM_TRIGGER_PASSED                                    (0x00u)   /*!< DTC tested as passed */
# define CDDDES_DEM_TRIGGER_FAILED                                    (0x01u)   /*!< DTC tested as failed */

/*! Type for DTC status change triggers and the derived test result */
typedef struct CddDes_Dem_DtcStatusTriggerTable_s
{
  uint16 DtcStatusOldNew;                           /*!< High byte: relevant status bit values of the OLD DTC status, Low Byte: relevant status bit values of the NEW DTC status */
  CddDes_Dem_DtcStatusTriggerType  Trigger;         /*!< The resulting PASSED/FAILED trigger */
} CddDes_Dem_DtcStatusTriggerTableType;

/*! Type for DTC status change triggers and the derived test result */
typedef uint8 CddDes_SyncEventType;
#define CDDDES_SYNCEVENT_DTC_PASSED                                  (0x00u)   /*!< DTC tested as passed */
#define CDDDES_SYNCEVENT_DTC_FAILED                                  (0x01u)   /*!< DTC tested as failed */
#define CDDDES_SYNCEVENT_CLEAR_DTC                                   (0x02u)   /*!< Clear DTC Event      */
#define CDDDES_NUM_SYNCEVENTS                                        (3u)

/*! Type for asynchroneous processing */
typedef uint8 CddDes_AsyncReturnType;
#define CDDDES_OPERATION_COMPLETED                                   (0x00u)  /*!< operation/processing finished */
#define CDDDES_OPERATION_PENDING                                     (0x01u)  /*!< operation/processing is pending */
#define CDDDES_OPERATION_PENDING_INTERNALLY                          (0xffu)  /*!< internal operation/processing is pending */


/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

#endif /* CDDDES_PRIV_H */

/**********************************************************************************************************************
*  END OF FILE: CddDes_Priv.h
*********************************************************************************************************************/
