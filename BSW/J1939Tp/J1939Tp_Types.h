/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Tp_Types.h
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Global types header of the SAE J1939 Transport Layer module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (J1939TP_TYPES_H)
# define J1939TP_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/*! \trace SPEC-2167284 */
# include "ComStack_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/




/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef enum
{
  /* *INDENT-OFF* */
  J1939TP_CAR_NO_ABORT = 0, /*!< No Abort Reason, do not send abort */
  /* J1939, ISOBUS (TP, ETP) */
  J1939TP_CAR_CONNECTION = 1, /*!< Abort Reason: Already in one or more connection-managed sessions and cannot support another */
  J1939TP_CAR_RESOURCES = 2, /*!< Abort Reason: System resources were needed for another task so this connection managed session was terminated */
  J1939TP_CAR_TIMEOUT = 3, /*!< Abort Reason: A timeout occurred and this is the connection abort to close the session */
  J1939TP_CAR_UNTIMELY_CTS = 4, /*!< Abort Reason: CTS messages received when data transfer is in progress */
  J1939TP_CAR_RETRANSMIT = 5, /*!< Abort Reason: Maximum retransmit request limit reached */
  /* ISOBUS (TP, ETP) */
  J1939TP_CAR_UNTIMELY_DT = 6, /*!< Abort Reason: Unexpected data transfer packet */
  J1939TP_CAR_BAD_SEQ = 7, /*!< Abort Reason: Bad sequence number (and software is not able to recover) */
  J1939TP_CAR_DUP_SEQ = 8, /*!< Abort Reason: Duplicate sequence number (and software is not able to recover) */
  /* ISOBUS (ETP) */
  J1939TP_CAR_UNTIMELY_DPO = 9, /*!< Abort Reason: Unexpected EDPO packet */
  J1939TP_CAR_DPO_PGN = 10, /*!< Abort Reason: Unexpected EDPO PGN (PGN in EDPO is bad) */
  J1939TP_CAR_DPO_PACKETS = 11, /*!< Abort Reason: EDPO number of packets is greater than CTS */
  J1939TP_CAR_DPO_OFFSET = 12, /*!< Abort Reason: Bad EDPO offset */
  J1939TP_CAR_ECTS_PGN = 14, /*!< Abort Reason: Unexpected ECTS PGN (PGN in ECTS is bad) */
  J1939TP_CAR_ECTS_PACKETS = 15, /*!< Abort Reason: ECTS requested packets exceeds message size */
  /* J1939, ISOBUS (TP, ETP) */
  J1939TP_CAR_OTHER = 255 /*!< Abort Reason: Unknown */
  /* *INDENT-ON* */
} J1939Tp_ConnectionAbortReasonType;

typedef enum
{
  J1939TP_NSDU_DIRECT = 0,
  J1939TP_NSDU_BAM = 1,
  J1939TP_NSDU_CMDT_TX = 2,
  J1939TP_NSDU_CMDT_RX = 3,
  J1939TP_NSDU_ETP_TX = 4,
  J1939TP_NSDU_ETP_RX = 5,
  J1939TP_NSDU_FPP = 6
} J1939Tp_NSduType;

typedef enum
{
  J1939TP_STATE_IDLE = 0x0000,
  J1939TP_STATE_FINISH = 0x0001,

  J1939TP_STATEMASK_BUSY = 0x0040,
  J1939TP_STATEMASK_CONF = 0x0080,

  J1939TP_STATEMASK_DIRECT = 0x0100,
  J1939TP_STATE_DIRECT = 0x0101,
  J1939TP_STATE_DIRECT_BUSY = 0x0141,
  J1939TP_STATE_DIRECT_CONF = 0x0181,

  J1939TP_STATEMASK_BAM = 0x0200,
  J1939TP_STATE_BAM_BAM = 0x0201,
  J1939TP_STATE_BAM_DATA = 0x0205,
  J1939TP_STATE_BAM_BAM_BUSY = 0x0241,
  J1939TP_STATE_BAM_DATA_BUSY = 0x0245,
  J1939TP_STATE_BAM_BAM_CONF = 0x0281,
  J1939TP_STATE_BAM_DATA_CONF = 0x0285,

  J1939TP_STATEMASK_CMDT = 0x0400,
  J1939TP_STATE_CMDT_RTS = 0x0401,
  J1939TP_STATE_CMDT_CTSWAIT = 0x0402,
  J1939TP_STATE_CMDT_CTS = 0x0403,
  J1939TP_STATE_CMDT_DATA_FIRST = 0x0404,
  J1939TP_STATE_CMDT_DATA = 0x0405,
  J1939TP_STATE_CMDT_EOMACK = 0x0406,
  J1939TP_STATE_CMDT_ABORT = 0x0407,
  J1939TP_STATE_CMDT_RTS_BUSY = 0x0441,
  J1939TP_STATE_CMDT_CTSWAIT_BUSY = 0x0442,
  J1939TP_STATE_CMDT_CTS_BUSY = 0x0443,
  J1939TP_STATE_CMDT_DATA_BUSY = 0x0445,
  J1939TP_STATE_CMDT_EOMACK_BUSY = 0x0446,
  J1939TP_STATE_CMDT_ABORT_BUSY = 0x0447,
  J1939TP_STATE_CMDT_RTS_CONF = 0x0481,
  J1939TP_STATE_CMDT_CTSWAIT_CONF = 0x0482,
  J1939TP_STATE_CMDT_CTS_CONF = 0x0483,
  J1939TP_STATE_CMDT_DATA_CONF = 0x0485,
  J1939TP_STATE_CMDT_EOMACK_CONF = 0x0486,
  J1939TP_STATE_CMDT_ABORT_CONF = 0x0487,

  J1939TP_STATEMASK_ETP = 0x0800,
  J1939TP_STATE_ETP_RTS = 0x0801,
  J1939TP_STATE_ETP_CTSWAIT = 0x0802,
  J1939TP_STATE_ETP_CTS = 0x0803,
  J1939TP_STATE_ETP_DPO = 0x0804,
  J1939TP_STATE_ETP_DATA = 0x0805,
  J1939TP_STATE_ETP_EOMACK = 0x0806,
  J1939TP_STATE_ETP_ABORT = 0x0807,
  J1939TP_STATE_ETP_RTS_BUSY = 0x0841,
  J1939TP_STATE_ETP_CTSWAIT_BUSY = 0x0842,
  J1939TP_STATE_ETP_CTS_BUSY = 0x0843,
  J1939TP_STATE_ETP_DPO_BUSY = 0x0844,
  J1939TP_STATE_ETP_DATA_BUSY = 0x0845,
  J1939TP_STATE_ETP_EOMACK_BUSY = 0x0846,
  J1939TP_STATE_ETP_ABORT_BUSY = 0x0847,
  J1939TP_STATE_ETP_RTS_CONF = 0x0881,
  J1939TP_STATE_ETP_CTSWAIT_CONF = 0x0882,
  J1939TP_STATE_ETP_CTS_CONF = 0x0883,
  J1939TP_STATE_ETP_DPO_CONF = 0x0884,
  J1939TP_STATE_ETP_DATA_CONF = 0x0885,
  J1939TP_STATE_ETP_EOMACK_CONF = 0x0886,
  J1939TP_STATE_ETP_ABORT_CONF = 0x0887,

  J1939TP_STATEMASK_FPP = 0x1000,
  J1939TP_STATE_FPP_FF = 0x1001,
  J1939TP_STATE_FPP_AF = 0x1002,
  J1939TP_STATE_FPP_FF_BUSY = 0x1041,
  J1939TP_STATE_FPP_AF_BUSY = 0x1042,
  J1939TP_STATE_FPP_FF_CONF = 0x1081,
  J1939TP_STATE_FPP_AF_CONF = 0x1082,

  J1939TP_STATEMASK_PROTOCOL = 0x1F00
} J1939Tp_StateType;

typedef struct
{
  uint32 Size;
  uint32 Packets; /*!< Number of (E)TP.DT frames for BAM/CMDT/ETP, number of additional frames for FPP */
  uint32 PacketOffset;
  PduLengthType DataSize;
  uint16 TimeOut;
  volatile J1939Tp_StateType State;
  J1939Tp_ConnectionAbortReasonType AbortReason;
  uint8 Aborted;
  uint8 Sequence; /*!< Sequence number for BAM/CMDT/ETP, frame counter for FPP */
  uint8 NextBlock; /*!< Packet number that starts the next block for BAM/CMDT/ETP, sequence counter for FPP */
  uint8 MaxBlock;
  uint8 Sender;
  uint8 Receiver;
  uint8 Priority;
} J1939Tp_TxNSduStateType;

typedef struct
{
  uint32 Size;
  uint32 Packets; /*!< Number of (E)TP.DT frames for BAM/CMDT/ETP, number of additional frames for FPP */
  uint32 PacketOffset;
  PduLengthType BufSize;
  uint16 TimeOut;
  volatile J1939Tp_StateType State;
  J1939Tp_ConnectionAbortReasonType AbortReason;
  uint8 Aborted;
  uint8 InSession;
  uint8 Sequence; /*!< Sequence number for BAM/CMDT/ETP, frame counter for FPP */
  uint8 NextBlock; /*!< Packet number that starts the next block for BAM/CMDT/ETP, sequence counter for FPP */
  uint8 MaxBlock;
  uint8 Sender;
  uint8 Receiver;
  uint8 Priority;
} J1939Tp_RxNSduStateType;

typedef struct
{
  uint16 ConfTimeout;
  J1939Tp_NSduType CurrentNSduType;
  PduIdType CurrentNSduId;
  volatile boolean Locked;
} J1939Tp_TxNPduStateType;

#endif /* !defined (J1939TP_TYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: J1939Tp_Types.h
 *********************************************************************************************************************/
