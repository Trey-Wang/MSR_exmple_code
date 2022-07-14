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
/*!        \file  FrTSyn_Types.h
 *        \brief  Vector AUTOSAR FrTSyn Types header file
 *
 *      \details  This header file contains the type definitions of the Vector AUTOSAR module FrTSyn.

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

#if !defined (FRTSYN_TYPES_H)
# define FRTSYN_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"
# include "ComStack_Types.h"
# include "StbM_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define FRTSYN_NO_SLAVE_IDX                    (255U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8 FrTSyn_FrMsgType[16U];

typedef uint8 FrTSyn_DataIDListType[16U];

typedef uint8 FrTSyn_ConfigType;

/* FRTSYN State-Machine State Definitions */
typedef uint8 FrTSyn_StateMachineStateType;
# define FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC                (0U)
# define FRTSYN_STATE_RECEIVE_SYNC_RECEIVED                   (1U)

# define FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND              (0U)
# define FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT  (1U)

/* FRTSYN Crc handling */
typedef uint8 FrTSyn_CrcType;
# define FRTSYN_CRC_NOT_SUPPORTED                             (0U)
# define FRTSYN_CRC_SUPPORTED                                 (1U)

# define FRTSYN_CRC_IGNORED                                   (0U)
# define FRTSYN_CRC_NOT_VALIDATED                             (1U)
# define FRTSYN_CRC_VALIDATED                                 (2U)

/* FRTSYN Transmission State Definitions */
typedef uint8 FrTSyn_TransmissionModeType;
# define FRTSYN_TX_OFF                                        (0U)
# define FRTSYN_TX_ON                                         (1U)

/* FrTSyn FrController State Type */
typedef struct
{
  FrTSyn_TransmissionModeType TransmissionState;
  uint16 MacroticksPerCycle;
  uint16 MacrotickDuration;
} FrTSyn_FrControllerType;

/* FrTSyn Slave State Type */
typedef struct
{
  FrTSyn_StateMachineStateType State;
  FrTSyn_FrMsgType SyncMsg;
} FrTSyn_SlaveType;

/* FrTSyn Master State Type */
typedef struct
{
  FrTSyn_StateMachineStateType State;
  uint32 TxIntervalCount;
  uint32 MasterTriggerTransmitTimeoutCount;
  uint32 CyclicMsgResumeCounter;
  uint8 LastUpdateCounter;
  FrTSyn_FrMsgType SyncMsg;
  StbM_UserDataType UserData;
} FrTSyn_MasterType;

/* FrTSyn Slave Config Type */
typedef struct
{
  uint8 SequenceCounterJumpWidth;
  FrTSyn_CrcType RxCrcValidated;
  PduIdType SlaveHandleId;
} FrTSyn_SlaveConfigType;

/* FrTSyn Master Config Type */
typedef struct
{
  FrTSyn_CrcType TxCrcSecured;
  uint32 TxPeriod;
  uint32 MasterTriggerTransmitTimeout;
  uint32 DebounceTime;
  uint32 CyclicMsgResumeTime;
  boolean ImmediateTimeSync;
  PduIdType TxPduId;
  PduIdType MasterHandleId;
  uint8 DomainIdx;
} FrTSyn_MasterConfigType;

/* FrTSyn Domain Config Type */
typedef struct
{
  uint8 DomainId;
  StbM_SynchronizedTimeBaseType TimeBaseId;
  uint8 MasterCount;
  uint8 MasterStartIdx;
  uint8 SlaveIdx;
  FrTSyn_DataIDListType SyncDataIDList;
} FrTSyn_DomainConfigType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif  /* FRTSYN_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: FrTSyn_Types.h
 *********************************************************************************************************************/
