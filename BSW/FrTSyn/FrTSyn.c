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
/*!        \file  FrTSyn.c
 *        \brief  Vector AUTOSAR FrTSyn code file
 *
 *      \details  This is the main implementation file of the Vector AUTOSAR module FrTSyn.
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

#define FRTSYN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "FrTSyn.h"
#include "FrTSyn_Cbk.h"
#include "SchM_FrTSyn.h"
#include "StbM.h"
#include "FrIf.h"
#if (FRTSYN_CRC_USED)
# include "Crc.h"
#endif
#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of FrTSyn header file */
#if (  (FRTSYN_SW_MAJOR_VERSION != (3U)) \
    || (FRTSYN_SW_MINOR_VERSION != (3U)) \
    || (FRTSYN_SW_PATCH_VERSION != (0U)) )
# error "Vendor specific version numbers of FrTSyn.c and FrTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (FRTSYN_CFG_MAJOR_VERSION != (3U)) \
    || (FRTSYN_CFG_MINOR_VERSION != (3U)) )
# error "Version numbers of FrTSyn.c and FrTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define FRTSYN_INV_DOMAIN_IDX                              (255U)
#define FRTSYN_INV_FR_CYCLE                                (255U)
#define FRTSYN_MAX_SEQUENCE_COUNTER                        (15U)
#define FRTSYN_START_OFFSET_TIME_DOMAIN_ID                 (16U)

#define FRTSYN_TIME_SYNC_MESSAGE_DLC                       (16U)
#define FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC         (0x10U)
#define FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC             (0x20U)
#define FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC          (0x30U)
#define FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC              (0x40U)

#define FRTSYN_CRC_START_VALUE                             (0xFFU)

#define FRTSYN_TIMESTAMP_MAX_NANOSECONDS                   (999999999UL) /* 10^9 nanoseconds per second */
#define FRTSYN_TIMESTAMP_MAX_SECONDS                       (4294967295UL)
#define FRTSYN_TIMESTAMP_MAX_SECONDSHI                     (65535U)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

#define FrTSyn_DomainHasSlave(index)                       (FrTSyn_DomainConfig[(index)].SlaveIdx != FRTSYN_NO_SLAVE_IDX) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_DomainHasMaster(index)                      (FrTSyn_DomainConfig[(index)].MasterCount != 0) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define FrTSyn_GetGlobalTimeBaseBit(status)                ((status) & StbM_Global_Time_Base_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_GetSyncToGatewayBit(status)                 ((status) & StbM_Sync_To_Gateway_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define FrTSyn_IsSyncMsg(type)                             (((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC) || ((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_IsOfsMsg(type)                              (((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC) || ((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_IsCrcSecured(type)                          (((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC) || ((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get Message type (Multiplexer value) */
#define FrTSyn_FrSignal_GetType(msg)                       ((msg)[0U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get CRC */
#define FrTSyn_FrSignal_GetCRC(crcMsg)                     ((crcMsg)[1U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get Sequence counter */
#define FrTSyn_FrSignal_GetSC(msg)                         ((msg)[2U] & 0x0FU) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get Time Domain */
#define FrTSyn_FrSignal_GetD(msg)                          ((msg)[2U] >> 4U) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get SGW */
#define FrTSyn_FrSignal_GetSGW(syncMsg)                    ((uint8)(((syncMsg)[3U] & 0x02U) >> 1U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get FCNT */
#define FrTSyn_FrSignal_GetFCNT(syncMsg)                   ((syncMsg)[3U] >> 2U) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get the time value */
#define FrTSyn_FrSignal_GetSecHi(msg)                      (((uint16) (msg)[7U])        | \
                                                            ((uint16) (msg)[6U] << 8U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define FrTSyn_FrSignal_GetSec(msg)                        (((uint32) (msg)[11U])        | \
                                                            ((uint32) (msg)[10U] <<  8U) | \
                                                            ((uint32) (msg)[9U]  << 16U) | \
                                                            ((uint32) (msg)[8U]  << 24U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define FrTSyn_FrSignal_GetNSec(msg)                       (((uint32) (msg)[15U])        | \
                                                            ((uint32) (msg)[14U] <<  8U) | \
                                                            ((uint32) (msg)[13U] << 16U) | \
                                                            ((uint32) (msg)[12U] << 24U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get User bytes from SYNC message (Byte 2 only valid for not CRC secured messages) */
#define FrTSyn_FrSignal_GetUserByte0(syncMsg)               ((syncMsg)[4U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_FrSignal_GetUserByte1(syncMsg)               ((syncMsg)[5U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_FrSignal_GetUserByte2(syncMsg)               ((syncMsg)[1U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set Message type (Multiplexer value) */
#define FrTSyn_FrSignal_SetType(msg, value)                (msg)[0U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set CRC */
#define FrTSyn_FrSignal_SetCRC(crcMsg, value)              (crcMsg)[1U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set Sequence counter */
#define FrTSyn_FrSignal_SetSC(msg, value)                  (msg)[2U] = (uint8) (((msg)[2U] & 0xF0U) | ((value) & 0x0FU)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set Time Domain */
#define FrTSyn_FrSignal_SetD(msg, value)                   (msg)[2U] = (uint8) (((msg)[2U] & 0x0FU) | ((value) << 4U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set reserved Bit */
#define FrTSyn_FrSignal_SetReserved(msg, value)            (msg)[3U] = (uint8) (((msg)[3U] & 0xFEU) | ((value) & 0x01U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set SGW */
#define FrTSyn_FrSignal_SetSGW(msg, value)                 (msg)[3U] = (uint8) (((msg)[3U] & 0xFDU) | (((value) & 0x01U) << 1U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set FCNT */
#define FrTSyn_FrSignal_SetFCNT(msg, value)                (msg)[3U] = (uint8) (((msg)[3U] & 0x02U) | ((value) << 2U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set the time value */
#define FrTSyn_FrSignal_SetSecHi(msg, value)               {(msg)[7U] = (uint8) (value);          \
                                                            (msg)[6U] = (uint8) ((value) >>  8U);} /* PRQA S 3458 */ /* MD_MSR_19.4 */

#define FrTSyn_FrSignal_SetSec(msg, value)                 {(msg)[11U] = (uint8) (value);          \
                                                            (msg)[10U] = (uint8) ((value) >>  8U); \
                                                            (msg)[9U]  = (uint8) ((value) >> 16U); \
                                                            (msg)[8U]  = (uint8) ((value) >> 24U);} /* PRQA S 3458 */ /* MD_MSR_19.4 */

#define FrTSyn_FrSignal_SetNSec(msg, value)                {(msg)[15U] = (uint8) (value);          \
                                                            (msg)[14U] = (uint8) ((value) >>  8U); \
                                                            (msg)[13U] = (uint8) ((value) >> 16U); \
                                                            (msg)[12U] = (uint8) ((value) >> 24U);} /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* Set User bytes from SYNC message (Byte 2 only valid for not CRC secured messages) */
#define FrTSyn_FrSignal_SetUserByte0(syncMsg, value)       (syncMsg)[4U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_FrSignal_SetUserByte1(syncMsg, value)       (syncMsg)[5U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FrTSyn_FrSignal_SetUserByte2(syncMsg, value)       (syncMsg)[1U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (FRTSYN_LOCAL) /* COV_FRTSYN_MSR_COMPATIBILITY */
# define FRTSYN_LOCAL static
#endif

#if !defined (FRTSYN_LOCAL_INLINE) /* COV_FRTSYN_MSR_COMPATIBILITY */
# define FRTSYN_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define FRTSYN_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
FRTSYN_LOCAL VAR(uint8, FRTSYN_VAR_ZERO_INIT) FrTSyn_ModuleInitialized = FRTSYN_UNINIT; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define FRTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FRTSYN_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (FRTSYN_MASTER_COUNT)
/*! Transmit state of the module */
FRTSYN_LOCAL VAR(FrTSyn_StateMachineStateType, FRTSYN_VAR_NOINIT) FrTSyn_TransmitState; /* PRQA S 0850 */ /* MD_MSR_19.8 */

/*! FrCycle of last transmission */
FRTSYN_LOCAL VAR(uint8, FRTSYN_VAR_NOINIT) FrTSyn_LastTransmitFrCycle; /* PRQA S 0850, 3218 */ /* MD_MSR_19.8, MD_FrTSyn_3218 */

/*! Sequence counter for Tx messages */
FRTSYN_LOCAL VAR(uint8, FRTSYN_VAR_NOINIT) FrTSyn_TxSequenceCounter[FRTSYN_MASTER_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#if (FRTSYN_SLAVE_COUNT)
/*! Received sequence counter for Rx messages */
FRTSYN_LOCAL VAR(uint8, FRTSYN_VAR_NOINIT) FrTSyn_RxSequenceCounter[FRTSYN_SLAVE_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define FRTSYN_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FRTSYN_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (FRTSYN_MASTER_COUNT)
/*! Debounce counter for Tx messages */
FRTSYN_LOCAL VAR(uint32, FRTSYN_VAR_NOINIT) FrTSyn_TxDebounceCounter[FRTSYN_TXPDUID_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define FRTSYN_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FRTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (FRTSYN_MASTER_COUNT)
/*! State of masters */
FRTSYN_LOCAL VAR(FrTSyn_MasterType, FRTSYN_VAR_NOINIT) FrTSyn_MasterState[FRTSYN_MASTER_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#if (FRTSYN_SLAVE_COUNT)
/*! State of slaves */
FRTSYN_LOCAL VAR(FrTSyn_SlaveType, FRTSYN_VAR_NOINIT) FrTSyn_SlaveState[FRTSYN_SLAVE_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

/*! State of FrControllers */
FRTSYN_LOCAL VAR(FrTSyn_FrControllerType, FRTSYN_VAR_NOINIT) FrTSyn_FrControllerState[FRTSYN_FRCONTROLLER_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define FRTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define FRTSYN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FrTSyn_HandleSync()
 *********************************************************************************************************************/
/*! \brief      Processing of received sync messages
 *  \details    This function handles the processing of received sync messages.
 *  \param[in]  domainIdx         Index of the Domain that received sync message should be handled.
 *  \pre        Time slave is in state FRTSYN_STATE_RECEIVE_SYNC_RECEIVED
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_HandleSync(uint8 domainIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_CheckRxSequenceCounter()
 *********************************************************************************************************************/
/*! \brief      Check received sequence counter
 *  \details    This function checks if the received sequence counter is valid.
 *  \param[in]  domainIdx         Index of the domain of the received message.
 *  \param[in]  counter           Received counter value.
 *  \return     E_OK - Received sequence counter is valid
 *  \return     E_NOT_OK - Received sequence counter is invalid
 *  \pre        FrTSyn_RxSequenceCounter is initialized with invalid value
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_CheckRxSequenceCounter(
  uint8 domainIdx,
  uint8 counter); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_MemCpy()
 *********************************************************************************************************************/
/*! \brief      Copy data from one buffer to another
 *  \details    This function copies data from the source data buffer to the destination data buffer.
 *  \param[in]  destination       Pointer to the destination data buffer.
 *  \param[in]  source            Pointer to the source data buffer.
 *  \param[in]  count             Number of elements to copy.
 *  \pre        destination and source are of size count
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_MemCpy(
  P2VAR(uint8, AUTOMATIC, FRTSYN_APPL_VAR) destination,
  P2CONST(uint8, AUTOMATIC, FRTSYN_APPL_DATA) source,
  uint8 count); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_ProcessSend()
 *********************************************************************************************************************/
/*! \brief      Send processing
 *  \details    This function handles the processing of send.
 *  \param[in]  masterIdx         Index of the master that send state-machine should be processed.
 *  \pre        -
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_ProcessSend(uint8 masterIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_TransmitSync()
 *********************************************************************************************************************/
/*! \brief      Transmit sync messages
 *  \details    This function handles the transmitting of sync messages.
 *  \param[in]  masterIdx         Index of the master that sync message should be transmitted.
 *  \param[in]  frIfCycle         Current cycle of the FlexRay.
 *  \param[in]  frIfMacroticks    Current macroticks of the FlexRay.
 *  \param[in]  syncTimestamp     Time value that should be transmitted in the SYNC message.
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \pre        Master is in state FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND
 *  \pre        StbM passed a time base with GlobalTimeBaseBit set
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TransmitSync(
  uint8 masterIdx,
  uint8 frIfCycle,
  uint16 frIfMacroticks,
  StbM_TimeStampType syncTimestamp); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_InitDomain()
 *********************************************************************************************************************/
/*! \brief      Initialization of FrTSyn Domain
 *  \details    This function initializes a FrTSyn Domain.
 *  \param[in]  domainIdx         Index of Domain that should be initialized.
 *  \pre        -
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_InitDomain(uint8 domainIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_IncrementSequenceCounter()
 *********************************************************************************************************************/
/*! \brief      Increment sequence counter
 *  \details    This function increments the sequence counter.
 *  \param[in]  masterIdx         Index of the master of the transmitted message.
 *  \pre        -
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_IncrementSequenceCounter(uint8 masterIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_TimestampPlusTimestamp()
 *********************************************************************************************************************/
/*! \brief      Adds two timestamps
 *  \details    This function calculates the sum of two timestamps.
 *  \param[in]   timestamp1Ptr          Pointer to timestamp where the first summand is stored.
 *  \param[in]   timestamp2Ptr          Pointer to timestamp where the second summand is stored.
 *  \param[out]  timesumPtr             Pointer to timestamp where the calculated sum is stored.
 *  \return     E_OK - Timesum is valid
 *  \return     E_NOT_OK - Timesum is invalid
 *  \pre        -
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TimestampPlusTimestamp(
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timesumPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_TimestampMinusTimestamp()
 *********************************************************************************************************************/
/*! \brief      Subtracts two timestamps
 *  \details    This function calculates the time difference between timestamp1 and timestamp2
 *              (diff = timestamp1 - timestamp2).
 *  \param[in]  timestamp1Ptr     Pointer to first timestamp.
 *  \param[in]  timestamp2Ptr     Pointer to second timestamp.
 *  \param[out] timediffPtr       Pointer to timestamp with difference between timestamp1 and timestamp2.
 *  \return     E_OK - Time difference is valid
 *  \return     E_NOT_OK - Time difference is invalid
 *  \pre        timestamp1 is greater or equal to timestamp2
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TimestampMinusTimestamp(
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timediffPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_ConvertFrTicksToTimestamp()
 *********************************************************************************************************************/
/*! \brief      Converts number of FlexRay ticks to timestamp
 *  \details    This function converts the given number of FlexRay macroticks with the given macrotick duration to a timestamp.
 *  \param[in]   macroticks             Number of FlexRay macroticks, that should be converted to a timestamp.
 *  \param[in]   macrotickDuration      Duration of one macrotick in ns.
 *  \param[out]  timestampPtr           Pointer to timestamp calculated for ticks value.
 *  \pre         Max value of macroticks is 1024000, max value of macrotickDuration is 6000ns
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_ConvertFrTicksToTimestamp(
  uint32 macroticks,
  uint16 macrotickDuration,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timestampPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_PrepareAndSendSync()
 *********************************************************************************************************************/
/*! \brief      Prepare SYNC and OFS messages and transmit them.
 *  \details    This function retrieves timestamps from StbM and transmits them over FlexRay.
 *  \param[in]  domainIdx               Index of the domain for which sync message should be transmitted.
 *  \param[in]  masterIdx               Index of the master for which sync message should be transmitted.
 *  \param[in]  frControllerIdx         Index of the FlexRay controller that sync message should be transmitted.
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \pre        -
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_PrepareAndSendSync(
  uint8 domainIdx,
  uint8 masterIdx,
  uint8 frControllerIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_HandleSync()
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
 * \trace DSGN-FrTSyn22814, DSGN-FrTSyn22815
 */
#if (FRTSYN_SLAVE_COUNT)
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_HandleSync(uint8 domainIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 slaveIdx;
  StbM_TimeStampType syncTime, elapsedTime, currentTime, correctionTime, actualTime;
  uint8 frControllerIdx, frControllerId, frIfCycle, messageType;
  FrIf_StateType frIfState;
  Std_ReturnType retVal = E_OK;
  uint16 frIfMacroticks;
  uint32 macroticks;
  StbM_UserDataType userData;
  StbM_MeasurementType measureData = {0};

  /* ----- Implementation ----------------------------------------------- */
  slaveIdx = FrTSyn_DomainConfig[domainIdx].SlaveIdx;

  SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 get received time stamp */
  syncTime.secondsHi = FrTSyn_FrSignal_GetSecHi(FrTSyn_SlaveState[slaveIdx].SyncMsg);
  syncTime.seconds = FrTSyn_FrSignal_GetSec(FrTSyn_SlaveState[slaveIdx].SyncMsg);
  syncTime.nanoseconds = FrTSyn_FrSignal_GetNSec(FrTSyn_SlaveState[slaveIdx].SyncMsg);
  
  /* #20 Set offset time base and set slave state to FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC, if time domain is an offset domain */
  if (FrTSyn_DomainConfig[domainIdx].DomainId >= FRTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    (void)StbM_SetOffset(FrTSyn_DomainConfig[domainIdx].TimeBaseId, &syncTime);
    FrTSyn_SlaveState[slaveIdx].State = FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
  }
  else
  {
    frControllerIdx = FrTSyn_RxPduIdToFrControllerIdxMapping[FrTSyn_SlaveConfig[slaveIdx].SlaveHandleId];
    /* #30 If FlexRay interface is in state FRIF_STATE_ONLINE */
    if ((FrIf_GetState(FrTSyn_FrControllerIdxToFrClusterIdMapping[frControllerIdx], &frIfState) == E_OK) && (frIfState == FRIF_STATE_ONLINE))
    {

      /* #40 get currentCycle and currentMacroticks from FlexRay driver */
      frControllerId = FrTSyn_FrControllerIdList[frControllerIdx];
      if (FrIf_GetGlobalTime(frControllerId, &frIfCycle, &frIfMacroticks) == E_OK)
      {
        /* #50 calculate current time of Local Time Base */
        macroticks = ((uint32)FrTSyn_FrControllerState[frControllerIdx].MacroticksPerCycle * frIfCycle) + frIfMacroticks;
        FrTSyn_ConvertFrTicksToTimestamp(macroticks, FrTSyn_FrControllerState[frControllerIdx].MacrotickDuration, &elapsedTime);
        if (FrTSyn_TimestampPlusTimestamp(&syncTime, &elapsedTime, &currentTime) == E_OK)
        {
          actualTime = currentTime;
          if (frIfCycle >= FrTSyn_FrSignal_GetFCNT(FrTSyn_SlaveState[slaveIdx].SyncMsg))
          {
            macroticks = (uint32)FrTSyn_FrControllerState[frControllerIdx].MacroticksPerCycle << 6U;
            FrTSyn_ConvertFrTicksToTimestamp(macroticks, FrTSyn_FrControllerState[frControllerIdx].MacrotickDuration, &correctionTime);
            retVal = FrTSyn_TimestampMinusTimestamp(&currentTime, &correctionTime, &actualTime);
          }

          /* #60 If previous actions succeded */
          if (retVal == E_OK)
          {
            messageType = FrTSyn_FrSignal_GetType(FrTSyn_SlaveState[slaveIdx].SyncMsg);

            /* #70 Get user data from message */
            userData.userByte0 = FrTSyn_FrSignal_GetUserByte0(FrTSyn_SlaveState[slaveIdx].SyncMsg);
            userData.userByte1 = FrTSyn_FrSignal_GetUserByte1(FrTSyn_SlaveState[slaveIdx].SyncMsg);

            if (FrTSyn_IsCrcSecured(messageType))
            {
              userData.userDataLength = 2U;
            }
            else
            {
              /* #80 user byte 2 is only used if the message is not crc secured */
              userData.userByte2 = FrTSyn_FrSignal_GetUserByte2(FrTSyn_SlaveState[slaveIdx].SyncMsg);
              userData.userDataLength = 3U;
            }

            /* #90 Get timebase status from message */
            if (FrTSyn_FrSignal_GetSGW(FrTSyn_SlaveState[slaveIdx].SyncMsg) == 0U)
            {
              actualTime.timeBaseStatus = 0x00U;
            }
            else
            {
              actualTime.timeBaseStatus = StbM_Sync_To_Gateway_BflMask;
            }

            /* #100 Pass information to StbM and set Slave state to FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC */
            SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            (void)StbM_BusSetGlobalTime(FrTSyn_DomainConfig[domainIdx].TimeBaseId, &actualTime, &userData, &measureData);
            SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            FrTSyn_SlaveState[slaveIdx].State = FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
          }
        }
      }
    }
  }

  SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* FrTSyn_HandleSync() */ /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  FrTSyn_CheckRxSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_CheckRxSequenceCounter(
  uint8 domainIdx,
  uint8 counter) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 slaveIdx, counterJumpWidth;

  /* ----- Implementation ----------------------------------------------- */
  slaveIdx = FrTSyn_DomainConfig[domainIdx].SlaveIdx;

  /* #10 sequence counter of first received message is not checked */
  if (FrTSyn_RxSequenceCounter[slaveIdx] <= FRTSYN_MAX_SEQUENCE_COUNTER)
  {
    /* #20 calculate JumpWidth from last received SC */
    if (counter < FrTSyn_RxSequenceCounter[slaveIdx])
    {
      counterJumpWidth = (FRTSYN_MAX_SEQUENCE_COUNTER - FrTSyn_RxSequenceCounter[slaveIdx]) + counter + 1U;
    }
    else
    {
      counterJumpWidth = counter - FrTSyn_RxSequenceCounter[slaveIdx];
    }

    /* #30 Check if JumpWidth is in the permitted range */
    if ((counterJumpWidth > 0U) && (counterJumpWidth <= FrTSyn_SlaveConfig[slaveIdx].SequenceCounterJumpWidth)) /* PRQA S 3689 */ /* MD_FrTSyn_3689 */
    {
      retVal = E_OK;
    }
  }
  else
  {
    retVal = E_OK;
  }

  /* #40 update last received sequence counter */
  FrTSyn_RxSequenceCounter[slaveIdx] = counter;

  return retVal;

} /* FrTSyn_CheckRxSequenceCounter() */
#endif

/**********************************************************************************************************************
 *  FrTSyn_MemCpy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_MemCpy(
  P2VAR(uint8, AUTOMATIC, FRTSYN_APPL_VAR) destination,
  P2CONST(uint8, AUTOMATIC, FRTSYN_APPL_DATA) source,
  uint8 count) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 copy from source to destination */
  for ( ;count > 0U; count-- )
  {
    destination[count - 1U] = source[count - 1U];
  }

} /* FrTSyn_MemCpy() */

/**********************************************************************************************************************
 *  FrTSyn_ProcessSend()
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
 * \trace DSGN-FrTSyn22808, DSGN-FrTSyn22809
 */
#if (FRTSYN_MASTER_COUNT)
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_ProcessSend(uint8 masterIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 domainIdx, frControllerIdx;
  Std_ReturnType retVal;
  uint8 currentUpdateCounter;

  /* ----- Implementation ----------------------------------------------- */
  switch (FrTSyn_MasterState[masterIdx].State)
  {
    /* #10 If master is in state FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
    case FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND:
      frControllerIdx = FrTSyn_TxPduIdToFrControllerIdxMapping[FrTSyn_MasterConfig[masterIdx].MasterHandleId];

      /* #20 If module is in state FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND, transmission is enabled for the associated controller and the debounce counter has expired */
      if ((FrTSyn_TransmitState == FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND)
        && (FrTSyn_FrControllerState[frControllerIdx].TransmissionState == FRTSYN_TX_ON)
        && (FrTSyn_TxDebounceCounter[FrTSyn_MasterConfig[masterIdx].MasterHandleId] == 0U))
      {
        domainIdx = FrTSyn_MasterConfig[masterIdx].DomainIdx;
        /* #30 If ImmediateTimeSync is enabled */
        if (FrTSyn_MasterConfig[masterIdx].ImmediateTimeSync == TRUE) 
        {
          /* #40 Get current TimeBaseUpdateCounter from StbM */
          currentUpdateCounter = StbM_GetTimeBaseUpdateCounter(FrTSyn_DomainConfig[domainIdx].TimeBaseId);
        
          /* #50 If the TimeBaseUpdateCounter has changed */
          if (currentUpdateCounter != FrTSyn_MasterState[masterIdx].LastUpdateCounter)
          {
            /* #60 Save the current TimeBaseUpdateCounter, send a Sync message and set CyclicMsgResumeCounter */
            FrTSyn_MasterState[masterIdx].LastUpdateCounter = currentUpdateCounter;
            retVal = FrTSyn_PrepareAndSendSync(domainIdx, masterIdx, frControllerIdx);
            if (retVal == E_OK)
            {
              FrTSyn_MasterState[masterIdx].CyclicMsgResumeCounter = FrTSyn_MasterConfig[masterIdx].CyclicMsgResumeTime;
            }
          }
          else
          {
            /* #70 Send a message, if master Tx timeout has expired and CyclicMsgResumeCounter is not running */
            if ((FrTSyn_MasterState[masterIdx].TxIntervalCount == 0U) && (FrTSyn_MasterState[masterIdx].CyclicMsgResumeCounter == 0U))
            {
              retVal = FrTSyn_PrepareAndSendSync(domainIdx, masterIdx, frControllerIdx);
            }
          }
        }
        else
        {
          /* #80 Send a message, if master Tx timeout has expired */
          if (FrTSyn_MasterState[masterIdx].TxIntervalCount == 0U)
          {
            retVal = FrTSyn_PrepareAndSendSync(domainIdx, masterIdx, frControllerIdx);
          }
        }
      }
      break;

    /* #90 If master is in state FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
    case FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT:
      /* #100 If MasterTriggerTransmitTimeout expired */
      if (FrTSyn_MasterState[masterIdx].MasterTriggerTransmitTimeoutCount == 0U)
      {
        /* #110 Set modul and master state to FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
        SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        FrTSyn_TransmitState = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
        FrTSyn_MasterState[masterIdx].State = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
        SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
      break;
    default:
      break;
  }

} /* FrTSyn_ProcessSend() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  FrTSyn_TransmitSync()
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
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TransmitSync(
  uint8 masterIdx,
  uint8 frIfCycle,
  uint16 frIfMacroticks,
  StbM_TimeStampType syncTimestamp) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  PduInfoType pduInfo;
  FrTSyn_FrMsgType syncMsg;
  uint8 domainIdx, counter, frControllerIdx;
  uint32 macroticks;
  StbM_TimeStampType residualTime, syncTime;
# if (FRTSYN_CRC_USED)
  uint8 crcResult, dataId;
# endif

  /* ----- Implementation ----------------------------------------------- */
  domainIdx = FrTSyn_MasterConfig[masterIdx].DomainIdx;

  pduInfo.SduLength  = FRTSYN_TIME_SYNC_MESSAGE_DLC;
  pduInfo.SduDataPtr = (SduDataPtrType) syncMsg;

  /* #10 increment sequence counter on every transmission request of a SYNC message */
  FrTSyn_IncrementSequenceCounter(masterIdx);
  counter = FrTSyn_TxSequenceCounter[masterIdx];

  if (FrTSyn_DomainConfig[domainIdx].DomainId >= FRTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* #20 Assemble unconditioned parts of OFS message */
    /* Domain numbers of offset time bases (16-31) are carried in TimeSync messages as Domain number - 16 */
    FrTSyn_FrSignal_SetD(syncMsg, FrTSyn_DomainConfig[domainIdx].DomainId - FRTSYN_START_OFFSET_TIME_DOMAIN_ID);
    FrTSyn_FrSignal_SetSecHi(syncMsg, syncTimestamp.secondsHi); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    FrTSyn_FrSignal_SetSec(syncMsg, syncTimestamp.seconds); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    FrTSyn_FrSignal_SetNSec(syncMsg, syncTimestamp.nanoseconds); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
  }
  else
  {
    /* #30 Assemble unconditioned parts of SYNC message */
    FrTSyn_FrSignal_SetD(syncMsg, FrTSyn_DomainConfig[domainIdx].DomainId);

    FrTSyn_FrSignal_SetUserByte0(syncMsg, FrTSyn_MasterState[masterIdx].UserData.userByte0);
    FrTSyn_FrSignal_SetUserByte1(syncMsg, FrTSyn_MasterState[masterIdx].UserData.userByte1);

    FrTSyn_FrSignal_SetFCNT(syncMsg, frIfCycle);
    FrTSyn_FrSignal_SetSGW(syncMsg, FrTSyn_GetSyncToGatewayBit(syncTimestamp.timeBaseStatus));
    FrTSyn_FrSignal_SetReserved(syncMsg, 0U);

    /* #40 calculate timestamp Tsync and it add to message*/
    frControllerIdx = FrTSyn_TxPduIdToFrControllerIdxMapping[FrTSyn_MasterConfig[masterIdx].MasterHandleId];
    macroticks = ((uint32)FrTSyn_FrControllerState[frControllerIdx].MacroticksPerCycle * (64U - frIfCycle)) - frIfMacroticks;
    FrTSyn_ConvertFrTicksToTimestamp(macroticks, FrTSyn_FrControllerState[frControllerIdx].MacrotickDuration, &residualTime);
    retVal = FrTSyn_TimestampPlusTimestamp(&syncTimestamp, &residualTime, &syncTime);
    if (retVal == E_OK)
    {
      FrTSyn_FrSignal_SetSecHi(syncMsg, syncTime.secondsHi); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      FrTSyn_FrSignal_SetSec(syncMsg, syncTime.seconds); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      FrTSyn_FrSignal_SetNSec(syncMsg, syncTime.nanoseconds); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* #50 If previous actions succeded */
  if (retVal == E_OK)
  {
    /* #60 Add SC to message */
    FrTSyn_FrSignal_SetSC(syncMsg, counter);

    /* #70 If no CRC is used, add the message type and user data for the type of the domain */
    if (FrTSyn_MasterConfig[masterIdx].TxCrcSecured == FRTSYN_CRC_NOT_SUPPORTED)
    {
      if (FrTSyn_DomainConfig[domainIdx].DomainId >= FRTSYN_START_OFFSET_TIME_DOMAIN_ID)
      {
        FrTSyn_FrSignal_SetType(syncMsg, FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC);
      }
      else
      {
        FrTSyn_FrSignal_SetType(syncMsg, FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC);
        FrTSyn_FrSignal_SetUserByte2(syncMsg, FrTSyn_MasterState[masterIdx].UserData.userByte2);
      }
    }
# if (FRTSYN_CRC_USED)
    else
    {
      /* #80 Add the message type for type of the domain */
      if (FrTSyn_DomainConfig[domainIdx].DomainId >= FRTSYN_START_OFFSET_TIME_DOMAIN_ID)
      {
        FrTSyn_FrSignal_SetType(syncMsg, FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC);
      }
      else
      {
        FrTSyn_FrSignal_SetType(syncMsg, FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC);
      }

      /* #90 calculate DataID as DataIDList[SC] */
      dataId = FrTSyn_DomainConfig[domainIdx].SyncDataIDList[counter];

      /* #100 Generate CRC and add to message */
      crcResult = Crc_CalculateCRC8H2F(&(syncMsg[2U]), 14U, FRTSYN_CRC_START_VALUE, TRUE);
      crcResult = Crc_CalculateCRC8H2F(&dataId, 1U, crcResult, FALSE);
      FrTSyn_FrSignal_SetCRC(syncMsg, crcResult);
    }
# endif

    /* #110 Save message content in master state and request transmission on FlexRay interface */
    FrTSyn_MemCpy(FrTSyn_MasterState[masterIdx].SyncMsg, syncMsg, FRTSYN_TIME_SYNC_MESSAGE_DLC);
    retVal = FrIf_Transmit(FrTSyn_MasterConfig[masterIdx].TxPduId, &pduInfo);
  }

  return retVal;

} /* FrTSyn_TransmitSync() */

/**********************************************************************************************************************
 *  FrTSyn_IncrementSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_IncrementSequenceCounter(uint8 masterIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If sequence counter overflow occurs */
  if (FrTSyn_TxSequenceCounter[masterIdx] == FRTSYN_MAX_SEQUENCE_COUNTER)
  {
    /* #20 Reset sequence counter */
    FrTSyn_TxSequenceCounter[masterIdx] = 0U;
  }
  else
  {
    /* #30 Increment sequence counter */
    FrTSyn_TxSequenceCounter[masterIdx]++;
  }

} /* FrTSyn_IncrementSequenceCounter() */
#endif

/**********************************************************************************************************************
 *  FrTSyn_InitDomain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_InitDomain(uint8 domainIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
#if (FRTSYN_SLAVE_COUNT)
  uint8 slaveIdx;
#endif
#if (FRTSYN_MASTER_COUNT)
  uint8 masterCount, masterIdx;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (FRTSYN_SLAVE_COUNT)
  /* #10 init slave state */
  if (FrTSyn_DomainHasSlave(domainIdx))
  {
    slaveIdx = FrTSyn_DomainConfig[domainIdx].SlaveIdx;
    FrTSyn_SlaveState[slaveIdx].State = FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
    /* #20 initialize Rx sequence counter with invalid value to enable detection of no received message */
    FrTSyn_RxSequenceCounter[slaveIdx] = FRTSYN_MAX_SEQUENCE_COUNTER + 1U;
  }
#endif

#if (FRTSYN_MASTER_COUNT)
  /* #30 init master state */
  if (FrTSyn_DomainHasMaster(domainIdx))
  {
    for (masterCount = 0U; masterCount < FrTSyn_DomainConfig[domainIdx].MasterCount; masterCount++)
    {
      masterIdx = FrTSyn_DomainConfig[domainIdx].MasterStartIdx + masterCount;
      FrTSyn_MasterState[masterIdx].State = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
      FrTSyn_MasterState[masterIdx].TxIntervalCount = FrTSyn_MasterConfig[masterIdx].TxPeriod;
      FrTSyn_MasterState[masterIdx].MasterTriggerTransmitTimeoutCount = 0U;
      /* #40 initialize Immediate TimeSync counters */
      if (FrTSyn_MasterConfig[masterIdx].ImmediateTimeSync == TRUE)
      {
        FrTSyn_MasterState[masterIdx].CyclicMsgResumeCounter = 0U;
        FrTSyn_MasterState[masterIdx].LastUpdateCounter = 0U;
      }
      /* #40 initialize Tx sequence counter */
      FrTSyn_TxSequenceCounter[masterIdx] = FRTSYN_MAX_SEQUENCE_COUNTER;
    }
  }
#endif

} /* FrTSyn_InitDomain() */

/**********************************************************************************************************************
 *  StbM_TimestampPlusTimestamp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TimestampPlusTimestamp(
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timesumPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 add the two timestamps */
  timesumPtr->nanoseconds = timestamp1Ptr->nanoseconds + timestamp2Ptr->nanoseconds;
  timesumPtr->seconds = timestamp1Ptr->seconds + timestamp2Ptr->seconds;
  timesumPtr->secondsHi = timestamp1Ptr->secondsHi + timestamp2Ptr->secondsHi;

  /* #20 check for overflow of nanoseconds */
  /* no real overflow of nanoseconds possible, since max value is 10^9 - 1 */
  if (timesumPtr->nanoseconds > FRTSYN_TIMESTAMP_MAX_NANOSECONDS)
  {
    timesumPtr->seconds++;
    timesumPtr->nanoseconds -= FRTSYN_TIMESTAMP_MAX_NANOSECONDS + 1U;
  }

  /* #30 check for overflow of seconds */
  if (timesumPtr->seconds < (timestamp1Ptr->seconds | timestamp2Ptr->seconds))
  {
    timesumPtr->secondsHi++;
  }

  /* #40 check for overflow of seconds high */
  /* calculated sum is invalid, if overflow of secondsHi occurred */
  if ((timesumPtr->secondsHi >= timestamp1Ptr->secondsHi) && (timesumPtr->secondsHi >= timestamp2Ptr->secondsHi))
  {
    retVal = E_OK;
  }

  return retVal;
} /* FrTSyn_TimestampPlusTimestamp() */

/**********************************************************************************************************************
 *  StbM_TimestampMinusTimestamp()
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
#if (FRTSYN_SLAVE_COUNT)
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TimestampMinusTimestamp(
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timediffPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If seconds high 1 is equal or larger than seconds high 2 */
  if (timestamp1Ptr->secondsHi >= timestamp2Ptr->secondsHi)
  {
    /* #20 Calculate time difference */
    timediffPtr->secondsHi = timestamp1Ptr->secondsHi - timestamp2Ptr->secondsHi;
    timediffPtr->seconds = timestamp1Ptr->seconds - timestamp2Ptr->seconds;

    /* #30 Check for overflow of seconds and correct secondsHi */
    if (timestamp1Ptr->seconds < timestamp2Ptr->seconds)
    {
      timediffPtr->secondsHi--;
    }

    /* #40 Check for overflow of nanoseconds and correct seconds */
    if (timestamp1Ptr->nanoseconds < timestamp2Ptr->nanoseconds)
    {
      timediffPtr->seconds--;

      /* #50 Check for overflow of seconds and correct secondsHi */
      if (timediffPtr->seconds == FRTSYN_TIMESTAMP_MAX_SECONDS)
      {
        timediffPtr->secondsHi--;
      }

      /* #60 Calculate nanoseconds after overflow*/
      timediffPtr->nanoseconds = (FRTSYN_TIMESTAMP_MAX_NANOSECONDS - timestamp2Ptr->nanoseconds) + (1U + timestamp1Ptr->nanoseconds);
    }
    else
    {
      /* #70 Calculate nanoseconds */
      timediffPtr->nanoseconds = timestamp1Ptr->nanoseconds - timestamp2Ptr->nanoseconds;
    }
    retVal = E_OK;
  }
  return retVal;
} /* FrTSyn_TimestampMinusTimestamp() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  FrTSyn_ConvertFrTicksToTimestamp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_ConvertFrTicksToTimestamp(
  uint32 macroticks,
  uint16 macrotickDuration,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timestampPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /*  (max value of MacroticksPerCycle is 16000, max value of Cycles is 64 --> max value of macroticks is 16000*64=1024000;
       max value of MacrotickDuration is 6000ns --> max resulting time is 1024000*6000ns=6144000000ns=6s,144000000ns) */
  /* #10 If the resulting time exceeds the range of nanoseconds */
  if ((macroticks >= 1000000UL) && (macrotickDuration >= 1000U))
  {
    /* #20 Calculate seconds and nanoseconds */
    timestampPtr->seconds = (uint32)macrotickDuration / 1000U;
    timestampPtr->nanoseconds = (macroticks - 1000000U) * macrotickDuration;
  }
  else
  {
    /* #30 Calculate nanoseconds */
    timestampPtr->seconds = 0U;
    timestampPtr->nanoseconds = macroticks * macrotickDuration;
  }
  timestampPtr->secondsHi = 0U;
} /* FrTSyn_ConvertFrTicksToTimestamp() */

/**********************************************************************************************************************
 *  FrTSyn_PrepareAndSendSync()
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
#if (FRTSYN_MASTER_COUNT)
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_PrepareAndSendSync(uint8 domainIdx, uint8 masterIdx, uint8 frControllerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 frControllerId, frIfCycle;
  FrIf_StateType frIfState;
  uint16 frIfMacroticks;
  Std_ReturnType retVal = E_NOT_OK;
  StbM_TimeStampType syncTimestamp;

  /* ----- Implementation ----------------------------------------------- */
  frControllerId = FrTSyn_FrControllerIdList[frControllerIdx];
  /* #10 If FlexRay interface is in state FRIF_STATE_ONLINE */
  if ((FrIf_GetState(FrTSyn_FrControllerIdxToFrClusterIdMapping[frControllerIdx], &frIfState) == E_OK) && (frIfState == FRIF_STATE_ONLINE))
  {
    /* #20 If FlexRay interface returns valid cycle and macroticks and current cycle is different from last transmit cycle */
    if ((FrIf_GetGlobalTime(frControllerId, &frIfCycle, &frIfMacroticks) == E_OK) && (frIfCycle != FrTSyn_LastTransmitFrCycle))
    {
      FrTSyn_LastTransmitFrCycle = frIfCycle;

      /* #30 If domain is an offset domain */
      if (FrTSyn_DomainConfig[domainIdx].DomainId >= FRTSYN_START_OFFSET_TIME_DOMAIN_ID)
      {
        /* #40 get Offset Time Base */
        retVal = StbM_GetOffset(FrTSyn_DomainConfig[domainIdx].TimeBaseId, &syncTimestamp);
      }
      else
      {
        /* #50 get Synchronized Time Base Tsync */
        retVal = StbM_GetCurrentTime(FrTSyn_DomainConfig[domainIdx].TimeBaseId, &syncTimestamp, &FrTSyn_MasterState[masterIdx].UserData);
      }

      /* #60 If previous operations succeded and GlobalTimeBaseBit is set */
      if ((retVal == E_OK) && (FrTSyn_GetGlobalTimeBaseBit(syncTimestamp.timeBaseStatus) != 0U))
      {
        retVal = FrTSyn_TransmitSync(masterIdx, frIfCycle, frIfMacroticks, syncTimestamp);
        /* #70 If transmit request to FlexRay interface succeded */
        if (retVal == E_OK)
        {
          /* #80 Set modul and master state to FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT and reset master timeouts */
          SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          FrTSyn_TransmitState = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT;
          FrTSyn_MasterState[masterIdx].State = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT;
          FrTSyn_MasterState[masterIdx].TxIntervalCount = FrTSyn_MasterConfig[masterIdx].TxPeriod;
          FrTSyn_MasterState[masterIdx].MasterTriggerTransmitTimeoutCount = FrTSyn_MasterConfig[masterIdx].MasterTriggerTransmitTimeout;
          SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
      }
    }
  }

  return retVal;
} /* FrTSyn_PrepareAndSendSync() */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (FRTSYN_SLAVE_COUNT)
/**********************************************************************************************************************
 *  FrTSyn_RxIndication()
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
FUNC(void, FRTSYN_CODE) FrTSyn_RxIndication(
  PduIdType RxPduId, 
  P2CONST(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = FRTSYN_E_NO_ERROR;
# endif
  uint8 domainId, domainIdx, slaveIdx, messageType, counter;
  boolean validMessage = FALSE;
# if (FRTSYN_CRC_USED)
  uint8 crcResult, dataId;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (FrTSyn_ModuleInitialized == (uint8)FRTSYN_UNINIT)
  {
    errorId = FRTSYN_E_NOT_INITIALIZED;
  }
  /* #20 Check parameter 'PduInfoPtr' for NULL pointer */
  else if (PduInfoPtr == NULL_PTR)
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  /* #30 Check parameter 'PduInfoPtr->SduDataPtr' */
  else if (PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 check DLC of received message */
    if (PduInfoPtr->SduLength == FRTSYN_TIME_SYNC_MESSAGE_DLC)
    {
      domainId = FrTSyn_FrSignal_GetD(PduInfoPtr->SduDataPtr);
      messageType = FrTSyn_FrSignal_GetType(PduInfoPtr->SduDataPtr);

      if (FrTSyn_IsOfsMsg(messageType))
      { /* TimeSync messages of Offset Time Bases carry the Domain number - 16 */
        domainId += FRTSYN_START_OFFSET_TIME_DOMAIN_ID;
      }

      /* #50 check if the domain matches to one of the configured Time Domains */
      for (domainIdx = 0U; domainIdx < FRTSYN_DOMAIN_COUNT; domainIdx++)
      {
        if (FrTSyn_DomainConfig[domainIdx].DomainId == domainId)
        {
          if (FrTSyn_DomainHasSlave(domainIdx))
          {
            slaveIdx = FrTSyn_DomainConfig[domainIdx].SlaveIdx;

            /* #60 check if the received PduId matches to the configured PduId */
            if (FrTSyn_SlaveConfig[slaveIdx].SlaveHandleId == RxPduId)
            {
              /* #70 check if SYNC or OFS message was received */
              if (FrTSyn_IsSyncMsg(messageType) || FrTSyn_IsOfsMsg(messageType))
              { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                counter = FrTSyn_FrSignal_GetSC(PduInfoPtr->SduDataPtr);

                /* #80 check if SC matches to the expected value */
                if ((FrTSyn_CheckRxSequenceCounter(domainIdx, counter) == E_OK) && (FrTSyn_FrSignal_GetNSec(PduInfoPtr->SduDataPtr) <= FRTSYN_TIMESTAMP_MAX_NANOSECONDS))
                {
                  switch (FrTSyn_SlaveConfig[slaveIdx].RxCrcValidated) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  {
                    case FRTSYN_CRC_IGNORED:
                      validMessage = TRUE;
                      break;

                    case FRTSYN_CRC_NOT_VALIDATED:
                      /* #90 check if Type matches depending on the FrTSynRxCrcValidated parameter */
                      if (!FrTSyn_IsCrcSecured(messageType))
                      {
                        validMessage = TRUE;
                      }
                      break;

# if (FRTSYN_CRC_USED)
                    case FRTSYN_CRC_VALIDATED:
                      /* #90 check if Type matches depending on the FrTSynRxCrcValidated parameter */
                      if (FrTSyn_IsCrcSecured(messageType))
                      {
                        /* #100 calculate CRC over received PDU */
                        crcResult = Crc_CalculateCRC8H2F(&(PduInfoPtr->SduDataPtr[2U]), 14U, FRTSYN_CRC_START_VALUE, TRUE);
                        dataId = FrTSyn_DomainConfig[domainIdx].SyncDataIDList[counter];
                        crcResult = Crc_CalculateCRC8H2F(&dataId, 1U, crcResult, FALSE);

                        /* #110 check CRC */
                        if (crcResult == FrTSyn_FrSignal_GetCRC(PduInfoPtr->SduDataPtr))
                        {
                          validMessage = TRUE;
                        }
                      }
                      break;
# endif

                    default:
                      break;
                  }

                  if (validMessage == TRUE) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  {
                    FrTSyn_SlaveState[slaveIdx].State = FRTSYN_STATE_RECEIVE_SYNC_RECEIVED;
                    FrTSyn_MemCpy(FrTSyn_SlaveState[slaveIdx].SyncMsg, PduInfoPtr->SduDataPtr, FRTSYN_TIME_SYNC_MESSAGE_DLC);
                  }
                }
              }
            }
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
            else
            {
              /* #120 Domain has configured different RxPduId */
              errorId = FRTSYN_E_INVALID_PDUID;
            }
# endif
          }
          break;
        }
      }
    }
  }

# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_RX_INDICATION, errorId);
  }
# endif

} /* FrTSyn_RxIndication() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (FRTSYN_MASTER_COUNT)
/**********************************************************************************************************************
 *  FrTSyn_TriggerTransmit()
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
FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TriggerTransmit(
  PduIdType TxPduId, 
  P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) PduInfoPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = FRTSYN_E_NO_ERROR;
  uint8 domainIdx = FRTSYN_INV_DOMAIN_IDX;
# endif
  uint8 domainCount, masterCount, masterIdx;
  boolean foundMaster = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (FrTSyn_ModuleInitialized == (uint8)FRTSYN_UNINIT)
  {
    errorId = FRTSYN_E_NOT_INITIALIZED;
  }
  /* #20 Check parameter 'PduInfoPtr' for NULL pointer */
  else if (PduInfoPtr == NULL_PTR)
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  /* #30 Check parameter 'PduInfoPtr->SduDataPtr' */
  else if (PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 search for domain index */
    for (domainCount = 0U; domainCount < FRTSYN_DOMAIN_COUNT; domainCount++)
    {
      if (FrTSyn_DomainHasMaster(domainCount))
      {
        /* #50 Iterate over all masters of the current domain */
        for (masterCount = 0U; masterCount < FrTSyn_DomainConfig[domainCount].MasterCount; masterCount++)
        {
          masterIdx = FrTSyn_DomainConfig[domainCount].MasterStartIdx + masterCount;
          /* #60 If the current master uses TxPduId and is in state FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT */
          if ((FrTSyn_MasterConfig[masterIdx].MasterHandleId == TxPduId) && (FrTSyn_MasterState[masterIdx].State == FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT))
          {
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
            domainIdx = domainCount;
# endif
            /* #70 Transmit message */
            if (PduInfoPtr->SduLength >= FRTSYN_TIME_SYNC_MESSAGE_DLC)
            {
              PduInfoPtr->SduLength = FRTSYN_TIME_SYNC_MESSAGE_DLC;
              FrTSyn_MemCpy(PduInfoPtr->SduDataPtr, FrTSyn_MasterState[masterIdx].SyncMsg, FRTSYN_TIME_SYNC_MESSAGE_DLC);
              retVal = E_OK;
            }

            /* #80 Set state to waiting for sending of next message and reset TriggerTransmit timeout */
            FrTSyn_TransmitState = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
            FrTSyn_MasterState[masterIdx].State = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
            FrTSyn_MasterState[masterIdx].MasterTriggerTransmitTimeoutCount = 0U;
            /* #90 Start debounce timer for this TxPduId */
            FrTSyn_TxDebounceCounter[FrTSyn_MasterConfig[masterIdx].MasterHandleId] = FrTSyn_MasterConfig[masterIdx].DebounceTime;
            foundMaster = TRUE;
            break;
          }
        }
        if (foundMaster == TRUE)
        {
          break;
        }
      }
    }

# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
    if (domainIdx == FRTSYN_INV_DOMAIN_IDX)
    {
      /* found no corresponding domain index -> no message transmitted */
      errorId = FRTSYN_E_INVALID_PDUID;
    }
# endif
  }

# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_TRIGGER_TRANSMIT, errorId);
  }
# endif

return retVal;

} /* FrTSyn_TriggerTransmit() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (FRTSYN_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FRTSYN_APPL_VAR) versioninfo) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = FRTSYN_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter versioninfo for NULL pointer */
  if (versioninfo == NULL_PTR)
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo with corresponding macros from component header */
    versioninfo->vendorID = (FRTSYN_VENDOR_ID);
    versioninfo->moduleID = (FRTSYN_MODULE_ID);
    versioninfo->sw_major_version = (FRTSYN_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (FRTSYN_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (FRTSYN_SW_PATCH_VERSION);
  }

# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_GET_VERSION_INFO, errorId);
  }
# endif

} /* FrTSyn_GetVersionInfo() */
#endif /* (FRTSYN_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  FrTSyn_MainFunction()
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
FUNC(void, FRTSYN_CODE) FrTSyn_MainFunction(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = FRTSYN_E_NO_ERROR;
#endif
  uint8 domainIdx;
#if (FRTSYN_SLAVE_COUNT)
  uint8 slaveIdx;
#endif
#if (FRTSYN_MASTER_COUNT)
  uint8 masterCount, masterIdx, txPduCount;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (FrTSyn_ModuleInitialized == (uint8)FRTSYN_UNINIT)
  {
    errorId = FRTSYN_E_NOT_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Iterate over all domains */
    for (domainIdx = 0U; domainIdx < FRTSYN_DOMAIN_COUNT; domainIdx++)
    {
#if (FRTSYN_SLAVE_COUNT)
      /* #30 If the current domain has a slave role AND the slave is in state FRTSYN_STATE_RECEIVE_SYNC_RECEIVED, process the received message */
      if (FrTSyn_DomainHasSlave(domainIdx))
      {
        slaveIdx = FrTSyn_DomainConfig[domainIdx].SlaveIdx;

        if (FrTSyn_SlaveState[slaveIdx].State == FRTSYN_STATE_RECEIVE_SYNC_RECEIVED)
        {
         FrTSyn_HandleSync(domainIdx);
        }
      }
#endif

#if (FRTSYN_MASTER_COUNT)
      /* #40 If the domain has at least one master role */
      if (FrTSyn_DomainHasMaster(domainIdx))
      {
        /* #50 Iterate over all masters of the domain */
        for (masterCount = 0U; masterCount < FrTSyn_DomainConfig[domainIdx].MasterCount; masterCount++)
        {
          masterIdx = FrTSyn_DomainConfig[domainIdx].MasterStartIdx + masterCount;

          /* #60 TxPeriod Handling */
          if (FrTSyn_MasterState[masterIdx].TxIntervalCount > 0U)
          {
            FrTSyn_MasterState[masterIdx].TxIntervalCount--;
          }

          /* #70 MasterTriggerTransmitTimeout Handling */
          if (FrTSyn_MasterState[masterIdx].MasterTriggerTransmitTimeoutCount > 0U)
          {
            FrTSyn_MasterState[masterIdx].MasterTriggerTransmitTimeoutCount--;
          }

          /* #80 CyclicMsgResumeCounter Handling */
          if ((FrTSyn_MasterConfig[masterIdx].ImmediateTimeSync == TRUE) && (FrTSyn_MasterState[masterIdx].CyclicMsgResumeCounter > 0U)) 
          {
            FrTSyn_MasterState[masterIdx].CyclicMsgResumeCounter--;
          }

          /* #90 Message transmission handling */
          FrTSyn_ProcessSend(masterIdx);
        }
      }
#endif
    }

#if (FRTSYN_MASTER_COUNT)
    /* #100 DebounceCounter Handling */
    for (txPduCount = 0U; txPduCount < FRTSYN_TXPDUID_COUNT; txPduCount++)
    {
      if (FrTSyn_TxDebounceCounter[txPduCount] > 0U)
      {
        FrTSyn_TxDebounceCounter[txPduCount]--;
      }
    }
#endif
  }

#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_MAIN_FUNCTION, errorId);
  }
#endif

} /* FrTSyn_MainFunction() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  FrTSyn_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, FRTSYN_CODE)  FrTSyn_InitMemory(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  FrTSyn_ModuleInitialized = FRTSYN_UNINIT;
#endif

} /* FrTSyn_InitMemory() */

/**********************************************************************************************************************
 *  FrTSyn_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_Init(P2CONST(FrTSyn_ConfigType, AUTOMATIC, FRTSYN_APPL_DATA) configPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 domainIdx;
  uint8 frControllerIdx, frControllerId;
#if (FRTSYN_MASTER_COUNT)
  uint8 txPduCount;
#endif

  /* ----- Implementation ----------------------------------------------- */
  configPtr = configPtr; /* PRQA S 3199 */ /* MD_FrTSyn_3199 */

  /* init all domains */
  for (domainIdx = 0U; domainIdx < FRTSYN_DOMAIN_COUNT; domainIdx++)
  {
    FrTSyn_InitDomain(domainIdx);
  }

#if (FRTSYN_MASTER_COUNT)
  FrTSyn_TransmitState = FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
  FrTSyn_LastTransmitFrCycle = FRTSYN_INV_FR_CYCLE;
#endif

  for (frControllerIdx = 0U; frControllerIdx < FRTSYN_FRCONTROLLER_COUNT; frControllerIdx++)
  {
#if (FRTSYN_MASTER_COUNT)
    FrTSyn_FrControllerState[frControllerIdx].TransmissionState = FRTSYN_TX_ON;
#endif

    frControllerId = FrTSyn_FrControllerIdList[frControllerIdx];
    FrTSyn_FrControllerState[frControllerIdx].MacroticksPerCycle = FrIf_GetMacroticksPerCycle(frControllerId);
    FrTSyn_FrControllerState[frControllerIdx].MacrotickDuration = FrIf_GetMacrotickDuration(frControllerId);
  }

#if (FRTSYN_MASTER_COUNT)
  for (txPduCount = 0U; txPduCount < FRTSYN_TXPDUID_COUNT; txPduCount++)
  {
    FrTSyn_TxDebounceCounter[txPduCount] = 0U;
  }
#endif

#if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  FrTSyn_ModuleInitialized = (uint8)FRTSYN_INIT;
#endif

} /* FrTSyn_Init() */

#if (FRTSYN_MASTER_COUNT)
/**********************************************************************************************************************
 *  FrTSyn_SetTransmissionMode()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_SetTransmissionMode(
  uint8 CtrlIdx,
  FrTSyn_TransmissionModeType Mode) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = FRTSYN_E_NO_ERROR;
# endif
  uint8 frControllerIdx;

  /* ----- Development Error Checks ------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (FrTSyn_ModuleInitialized == (uint8)FRTSYN_UNINIT)
  {
    errorId = FRTSYN_E_NOT_INITIALIZED;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Search for FlexRay controllers with given CtrlIdx */
    for (frControllerIdx = 0U; frControllerIdx < FRTSYN_FRCONTROLLER_COUNT; frControllerIdx++)
    {
      if (FrTSyn_FrControllerIdList[frControllerIdx] == CtrlIdx)
      {
        /* #30 Set transmission mode to given mode */
        FrTSyn_FrControllerState[frControllerIdx].TransmissionState = Mode;
        break;
      }
    }
  }

# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_SET_TRANSMISSION_MODE, errorId);
  }
# endif

} /* FrTSyn_SetTransmissionMode() */
#endif

#define FRTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_FrTSyn_3199:
      Reason:     Dummy statement required to avoid compiler warnings.
      Risk:       No functional risk.
      Prevention: Not required.
 MD_FrTSyn_3218: rule 8.7
      Reason:     Vector style guide prevents usage of static variables/constant objects in function scope.
      Risk:       None.
      Prevention: None.
 MD_FrTSyn_3689: rule 21.1
      Reason:     A function parameter is used to access an array element. The parameter is always passed by an internal function that ensures that the parameter is in a valid range.
      Risk:       Runtime failure might be introduced by later code changes.
      Prevention: Covered by code review.
*/

/* COV_JUSTIFICATION_BEGIN

--- Preprocessor Coverage Justifications ------------------------------------------------------------------------------

  \ID COV_FRTSYN_MSR_COMPATIBILITY
  \ACCEPT TX 
  \REASON [COV_MSR_COMPATIBILITY]
  
COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: FrTSyn.c
 *********************************************************************************************************************/
