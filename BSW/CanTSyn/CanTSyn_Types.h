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
/*!        \file  CanTSyn_Types.h
 *        \brief  Vector AUTOSAR CanTSyn Types header file
 *
 *      \details  This header file contains the type definitions of the Vector AUTOSAR module CanTSyn.

 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CANTSYN_TYPES_H)
# define CANTSYN_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"
# include "ComStack_Types.h"
# include "StbM_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define CANTSYN_NO_SLAVE_IDX                    (255U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8 CanTSyn_CanMsgType[8U];

typedef uint8 CanTSyn_DataIDListType[16U];

typedef uint8 CanTSyn_ConfigType;

/* CANTSYN State-Machine State Definitions */
typedef uint8 CanTSyn_StateMachineStateType;
# define CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC                    (0U)
# define CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP               (1U)

# define CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND                  (0U)
# define CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATION       (1U)
# define CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND             (2U)
# define CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION  (3U)

/* CANTSYN Crc handling */
typedef uint8 CanTSyn_CrcType;
# define CANTSYN_CRC_NOT_SUPPORTED                                 (0U)
# define CANTSYN_CRC_SUPPORTED                                     (1U)

# define CANTSYN_CRC_IGNORED                                       (0U)
# define CANTSYN_CRC_NOT_VALIDATED                                 (1U)
# define CANTSYN_CRC_VALIDATED                                     (2U)

/* CANTSYN Transmission State Definitions */
typedef uint8 CanTSyn_TransmissionModeType;
# define CANTSYN_TX_OFF                                            (0U)
# define CANTSYN_TX_ON                                             (1U)

/* CanTSyn Slave State Type */
typedef struct
{
  CanTSyn_StateMachineStateType State;
  uint32 RxFollowUpTimeoutCount;
  StbM_TimeStampRawType SyncTimestampRaw;
  CanTSyn_CanMsgType SyncMsg;
} CanTSyn_SlaveType;

/* CanTSyn Master State Type */
typedef struct
{
  CanTSyn_StateMachineStateType State;
  uint32 TxIntervalCount;
  uint32 MasterConfTimeoutCount;
  uint32 CyclicMsgResumeCounter;
  uint8 LastUpdateCounter;
  StbM_TimeStampType SyncTimestamp;
  StbM_TimeStampRawType SyncTimestampRaw;
  StbM_TimeStampType FupTimestamp;
  StbM_UserDataType UserData;
} CanTSyn_MasterType;

/* CanTSyn Slave Config Type */
typedef struct
{
  uint32 FollowUpTimeout;
  uint8 SequenceCounterJumpWidth;
  CanTSyn_CrcType RxCrcValidated;
  PduIdType SlaveHandleId;
} CanTSyn_SlaveConfigType;

/* CanTSyn Master Config Type */
typedef struct
{
  CanTSyn_CrcType TxCrcSecured;
  uint32 TxPeriod;
  uint32 MasterConfTimeout;
  uint32 DebounceTime;
  uint32 CyclicMsgResumeTime;
  boolean ImmediateTimeSync;
  PduIdType TxPduId;
  PduIdType MasterConfHandleId;
  uint8 DomainIdx;
} CanTSyn_MasterConfigType;

/* CanTSyn Domain Config Type */
typedef struct
{
  uint8 DomainId;
  StbM_SynchronizedTimeBaseType TimeBaseId;
  uint8 MasterCount;
  uint8 MasterStartIdx;
  uint8 SlaveIdx;
  CanTSyn_DataIDListType SyncDataIDList;
  CanTSyn_DataIDListType FupDataIDList;
} CanTSyn_DomainConfigType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif  /* CANTSYN_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: CanTSyn_Types.h
 *********************************************************************************************************************/
