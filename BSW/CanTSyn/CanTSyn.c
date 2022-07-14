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
/*!        \file  CanTSyn.c
 *        \brief  Vector AUTOSAR CanTSyn code file
 *
 *      \details  This is the main implementation file of the Vector AUTOSAR module CanTSyn.
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

#define CANTSYN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "CanTSyn.h"
#include "CanTSyn_Cbk.h"
#include "SchM_CanTSyn.h"
#include "StbM.h"
#if (CANTSYN_MASTER_COUNT)
# include "CanIf.h"
#endif
#if (CANTSYN_CRC_USED)
# include "Crc.h"
#endif
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of CanTSyn header file */
#if (  (CANTSYN_SW_MAJOR_VERSION != (3U)) \
    || (CANTSYN_SW_MINOR_VERSION != (3U)) \
    || (CANTSYN_SW_PATCH_VERSION != (0U)) )
# error "Vendor specific version numbers of CanTSyn.c and CanTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (CANTSYN_CFG_MAJOR_VERSION != (3U)) \
    || (CANTSYN_CFG_MINOR_VERSION != (3U)) )
# error "Version numbers of CanTSyn.c and CanTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define CANTSYN_INV_DOMAIN_IDX                             (255U)
#define CANTSYN_MAX_SEQUENCE_COUNTER                       (15U)
#define CANTSYN_START_OFFSET_TIME_DOMAIN_ID                (16U)

#define CANTSYN_TIME_SYNC_MESSAGE_DLC                      (8U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC        (0x10U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC            (0x20U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_NOT_CRC         (0x18U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_CRC             (0x28U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC         (0x30U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC             (0x40U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_NOT_CRC        (0x38U)
#define CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_CRC            (0x48U)

#define CANTSYN_CRC_START_VALUE                            (0xFFU)

#define CANTSYN_TIMESTAMP_MAX_NANOSECONDS                  (999999999UL) /* 10^9 nanoseconds per second */
#define CANTSYN_TIMESTAMP_MAX_SECONDS                      (4294967295UL)
#define CANTSYN_TIMESTAMP_MAX_SECONDSHI                    (65535U)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

#define CanTSyn_DomainHasSlave(index)                      (CanTSyn_DomainConfig[(index)].SlaveIdx != CANTSYN_NO_SLAVE_IDX) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_DomainHasMaster(index)                     (CanTSyn_DomainConfig[(index)].MasterCount != 0) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CanTSyn_GetGlobalTimeBaseBit(status)               ((status) & StbM_Global_Time_Base_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_GetSyncToGatewayBit(status)                ((status) & StbM_Sync_To_Gateway_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CanTSyn_IsSyncMsg(type)                            (((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC) || ((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_IsFupMsg(type)                             (((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_NOT_CRC) || ((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_IsOfsMsg(type)                             (((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC) || ((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_IsOfnsMsg(type)                            (((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_NOT_CRC) || ((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CanTSyn_IsSyncCrcSecured(type)                     (((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC) || ((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_IsFupCrcSecured(type)                      (((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_CRC) || ((type) == CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_CRC)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get Message type (Multiplexer value) */
#define CanTSyn_CanSignal_GetType(msg)                     ((msg)[0U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get CRC */
#define CanTSyn_CanSignal_GetCRC(crcMsg)                   ((crcMsg)[1U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get Sequence counter */
#define CanTSyn_CanSignal_GetSC(msg)                       ((msg)[2U] & 0x0FU) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get Time Domain */
#define CanTSyn_CanSignal_GetD(msg)                        ((msg)[2U] >> 4U) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get the time value (seconds from SYNC/OFS message or nanoseconds from FUP/OFNS message) */
#define CanTSyn_CanSignal_GetTime(msg)                     (((uint32) (msg)[7U])         | \
                                                            ((uint32) (msg)[6U] <<  8U)  | \
                                                            ((uint32) (msg)[5U] <<  16U) | \
                                                            ((uint32) (msg)[4U] <<  24U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get OVS from FUP message */
#define CanTSyn_CanSignal_GetOVS(fupMsg)                   ((fupMsg)[3U] & 0x03U) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get SGW from FUP message */
#define CanTSyn_CanSignal_GetSGW(fupMsg)                   ((uint8)(((fupMsg)[3U] & 0x04U) >> 2U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get User bytes (SYNC: Byte 0 and 1; FUP: Byte 2; Byte 1 and 2 only valid for not CRC secured messages) */
#define CanTSyn_CanSignal_GetUserByte0(syncMsg)            ((syncMsg)[3U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_CanSignal_GetUserByte1(syncMsg)            ((syncMsg)[1U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_CanSignal_GetUserByte2(fupMsg)             ((fupMsg)[1U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get secondsHi from OFS/OFNS message */
#define CanTSyn_CanSignal_GetSecHi(ofsMsg)                 ((ofsMsg)[3U]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set Message type (Multiplexer value) */
#define CanTSyn_CanSignal_SetType(msg, value)              (msg)[0U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set CRC */
#define CanTSyn_CanSignal_SetCRC(crcMsg, value)            (crcMsg)[1U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set Sequence counter */
#define CanTSyn_CanSignal_SetSC(msg, value)                (msg)[2U] = (uint8) (((msg)[2U] & 0xF0U) | ((value) & 0x0FU)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set Time Domain */
#define CanTSyn_CanSignal_SetD(msg, value)                 (msg)[2U] = (uint8) (((msg)[2U] & 0x0FU) | ((value) << 4U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Get the time value (seconds from SYNC/OFS message or nanoseconds from FUP/OFNS message) */
#define CanTSyn_CanSignal_SetTime(msg, value)              {(msg)[7U] = (uint8) (value);          \
                                                            (msg)[6U] = (uint8) ((value) >>  8U); \
                                                            (msg)[5U] = (uint8) ((value) >> 16U); \
                                                            (msg)[4U] = (uint8) ((value) >> 24U);} /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* Set OVS from FUP message */
#define CanTSyn_CanSignal_SetOVS(fupMsg, value)            (fupMsg)[3U] = (uint8) (((fupMsg)[3U] & 0xFCU) | ((value) & 0x03U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set SGW from FUP message */
#define CanTSyn_CanSignal_SetSGW(fupMsg, value)            (fupMsg)[3U] = (uint8) (((fupMsg)[3U] & 0xFBU) | (((value) & 0x01U) << 2U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set reserved bits from FUP message */
#define CanTSyn_CanSignal_SetReservedFup(fupMsg, value)    (fupMsg)[3U] = (uint8) (((fupMsg)[3U] & 0x07U) | ((value) << 3U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set User bytes (SYNC: Byte 0 and 1; FUP: Byte 2; Byte 1 and 2 only valid for not CRC secured messages) */
#define CanTSyn_CanSignal_SetUserByte0(syncMsg, value)     (syncMsg)[3U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_CanSignal_SetUserByte1(syncMsg, value)     (syncMsg)[1U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CanTSyn_CanSignal_SetUserByte2(fupMsg, value)      (fupMsg)[1U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set secondsHi from OFS/OFNS message */
#define CanTSyn_CanSignal_SetSecHi(ofsMsg, value)          (ofsMsg)[3U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Set reserved byte from OFS/OFNS message */
#define CanTSyn_CanSignal_SetReservedOfs(ofsMsg, value)    (ofsMsg)[1U] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CANTSYN_LOCAL) /* COV_CANTSYN_MSR_COMPATIBILITY */
# define CANTSYN_LOCAL static
#endif

#if !defined (CANTSYN_LOCAL_INLINE) /* COV_CANTSYN_MSR_COMPATIBILITY */
# define CANTSYN_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CANTSYN_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
CANTSYN_LOCAL VAR(uint8, CANTSYN_VAR_ZERO_INIT) CanTSyn_ModuleInitialized = CANTSYN_UNINIT; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define CANTSYN_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CANTSYN_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CANTSYN_MASTER_COUNT)
/*! Transmit state of the module */
CANTSYN_LOCAL VAR(CanTSyn_StateMachineStateType, CANTSYN_VAR_NOINIT) CanTSyn_TransmitState; /* PRQA S 0850 */ /* MD_MSR_19.8 */

/*! Transmission state of CanControllers */
CANTSYN_LOCAL VAR(CanTSyn_TransmissionModeType, CANTSYN_VAR_NOINIT) CanTSyn_CanControllerTransmissionState[CANTSYN_CANCONTROLLER_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */

/*! Sequence counter for Tx messages */
CANTSYN_LOCAL VAR(uint8, CANTSYN_VAR_NOINIT) CanTSyn_TxSequenceCounter[CANTSYN_MASTER_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#if (CANTSYN_SLAVE_COUNT)
/*! Received sequence counter for Rx messages */
CANTSYN_LOCAL VAR(uint8, CANTSYN_VAR_NOINIT) CanTSyn_RxSequenceCounter[CANTSYN_SLAVE_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define CANTSYN_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CANTSYN_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CANTSYN_MASTER_COUNT)
/*! Debounce counter for Tx messages */
CANTSYN_LOCAL VAR(uint32, CANTSYN_VAR_NOINIT) CanTSyn_TxDebounceCounter[CANTSYN_PDUID_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define CANTSYN_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CANTSYN_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CANTSYN_MASTER_COUNT)
/*! State of masters */
CANTSYN_LOCAL VAR(CanTSyn_MasterType, CANTSYN_VAR_NOINIT) CanTSyn_MasterState[CANTSYN_MASTER_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#if (CANTSYN_SLAVE_COUNT)
/*! State of slaves */
CANTSYN_LOCAL VAR(CanTSyn_SlaveType, CANTSYN_VAR_NOINIT) CanTSyn_SlaveState[CANTSYN_SLAVE_COUNT]; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define CANTSYN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CANTSYN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CanTSyn_ProcessReceive()
 *********************************************************************************************************************/
/*! \brief      Receive processing
 *  \details    This function handles the processing of receive.
 *  \param[in]  slaveIdx          Index of the Slave that receive handling should be processed.
 *  \pre        -
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_ProcessReceive(uint8 slaveIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_HandleSync()
 *********************************************************************************************************************/
/*! \brief      Processing of received sync messages
 *  \details    This function handles the processing of received sync messages.
 *  \param[in]  domainIdx         Index of the Domain that received sync message should be handled.
 *  \param[in]  pduInfoPtr        Pointer to the PDU that contains the received sync message.
 *  \pre        Time slave is in state CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_HandleSync(
  uint8 domainIdx,
  P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) pduInfoPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_HandleFup()
 *********************************************************************************************************************/
/*! \brief      Processing of received fup messages
 *  \details    This function handles the processing of received fup messages.
 *  \param[in]  domainIdx         Index of the Domain that received fup message should be handled.
 *  \param[in]  pduInfoPtr        Pointer to the PDU that contains the received fup message.
 *  \pre        slave is in state CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_HandleFup(
  uint8 domainIdx,
  P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) pduInfoPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_CheckRxSequenceCounter()
 *********************************************************************************************************************/
/*! \brief      Check received sequence counter
 *  \details    This function checks if the received sequence counter is valid.
 *  \param[in]  domainIdx         Index of the domain of the received message.
 *  \param[in]  counter           Received counter value.
 *  \return     E_OK - Received sequence counter is valid
 *  \return     E_NOT_OK - Received sequence counter is invalid
 *  \pre        CanTSyn_RxSequenceCounter is initialized with invalid value
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckRxSequenceCounter(
  uint8 domainIdx,
  uint8 counter); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_MemCpy()
 *********************************************************************************************************************/
/*! \brief      Copy data from one buffer to another
 *  \details    This function copies data from the source data buffer to the destination data buffer.
 *  \param[in]  destination       Pointer to the destination data buffer.
 *  \param[in]  source            Pointer to the source data buffer.
 *  \param[in]  count             Number of elements to copy.
 *  \pre        destination and source are of size count
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_MemCpy(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_VAR) destination,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_DATA) source,
  uint8 count); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_ProcessSend()
 *********************************************************************************************************************/
/*! \brief      Send processing
 *  \details    This function handles the processing of send.
 *  \param[in]  masterIdx         Index of the master that send state-machine should be processed.
 *  \pre        -
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_ProcessSend(uint8 masterIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_TransmitSync()
 *********************************************************************************************************************/
/*! \brief      Transmit sync messages
 *  \details    This function handles the transmitting of sync messages.
 *  \param[in]  masterIdx         Index of the master that sync message should be transmitted.
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \pre        Master is in state CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND
 *  \pre        StbM passed a time base with GlobalTimeBaseBit set
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_TransmitSync(uint8 masterIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_TransmitFup()
 *********************************************************************************************************************/
/*! \brief      Transmit fup messages
 *  \details    This function handles the transmitting of fup messages.
 *  \param[in]  masterIdx         Index of the master that fup message should be transmitted.
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \pre        Master is in state CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_TransmitFup(uint8 masterIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_IncrementSequenceCounter()
 *********************************************************************************************************************/
/*! \brief      Increment sequence counter
 *  \details    This function increments the sequence counter.
 *  \param[in]  masterIdx         Index of the master of the transmitted message.
 *  \pre        -
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_IncrementSequenceCounter(uint8 masterIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_InitDomain()
 *********************************************************************************************************************/
/*! \brief      Initialization of CanTSyn Domain
 *  \details    This function initializes a CanTSyn Domain.
 *  \param[in]  domainIdx         Index of Domain that should be initialized.
 *  \pre        -
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_InitDomain(uint8 domainIdx); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_TimestampPlusTimestampRaw()
 *********************************************************************************************************************/
/*! \brief      Add timestamp and timestamp in raw format
 *  \details    This function calculates the sum of one timestamp and one timestamp in raw format.
 *  \param[in]  timestampPtr      Pointer to timestamp.
 *  \param[in]  timestampRawPtr   Pointer to timestamp in raw format.
 *  \param[out] timesumPtr        Pointer to timestamp with sum of timestamp and timestampRaw.
 *  \pre        -
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_TimestampPlusTimestampRaw(
  P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_APPL_DATA) timestampPtr,
  P2CONST(StbM_TimeStampRawType, AUTOMATIC, CANTSYN_APPL_DATA) timestampRawPtr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, CANTSYN_APPL_VAR) timesumPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  CanTSyn_PrepareAndSendSync()
 *********************************************************************************************************************/
/*! \brief      Prepare SYNC and OFS messages and transmit them.
 *  \details    This function retrieves timestamps from StbM and transmits them over CAN.
 *  \param[in]  domainIdx      Index of the domain that sync message should be transmitted.
 *  \param[in]  masterIdx      Index of the master that sync message should be transmitted.
 *  \return     E_OK - Message transmission was successful
 *  \return     E_NOT_OK - Message transmission was not successful
 *  \pre        -
 *********************************************************************************************************************/
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_PrepareAndSendSync(
  uint8 domainIdx,
  uint8 masterIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CanTSyn_ProcessReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * \trace DSGN-CanTSyn22814, DSGN-CanTSyn22815
 */
#if (CANTSYN_SLAVE_COUNT)
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_ProcessReceive(uint8 slaveIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
  switch (CanTSyn_SlaveState[slaveIdx].State)
  {
    /* #10 If the slave is in state CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP */
    case CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP:
      /* #20 check for FollowUp received timeout */
      SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if (CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount == 0U)
      {
        /* #30 Set slave state to CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC, if the timeout expired */
        CanTSyn_SlaveState[slaveIdx].State = CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
      }
      SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      break;
    default:
      ;
  }

} /* CanTSyn_ProcessReceive() */

/**********************************************************************************************************************
 *  CanTSyn_HandleSync()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_HandleSync(
  uint8 domainIdx,
  P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) pduInfoPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 slaveIdx;
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  slaveIdx = CanTSyn_DomainConfig[domainIdx].SlaveIdx;

  if (CanTSyn_DomainConfig[domainIdx].DomainId < CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* #10 on SYNC message Rx indication retrieve Local Time Stamp as T2raw */
    retVal = StbM_GetCurrentTimeRaw(&CanTSyn_SlaveState[slaveIdx].SyncTimestampRaw);
  }

  if (retVal == E_OK)
  {
    SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #20 get Synchronized Time Base part T0 delivered with SYNC message */
    CanTSyn_MemCpy(CanTSyn_SlaveState[slaveIdx].SyncMsg, pduInfoPtr->SduDataPtr, CANTSYN_TIME_SYNC_MESSAGE_DLC);

    /* #30 Set slave state to CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP */
    CanTSyn_SlaveState[slaveIdx].State = CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP;

    /* #40 set FollowUp receive timeout time */
    CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount = CanTSyn_SlaveConfig[slaveIdx].FollowUpTimeout; /* PRQA S 3689 */ /* MD_CanTSyn_3689 */

    SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

} /* CanTSyn_HandleSync() */

/**********************************************************************************************************************
 *  CanTSyn_HandleFup()
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
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_HandleFup(
  uint8 domainIdx,
  P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) pduInfoPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 slaveIdx, messageType;
  uint32 syncSeconds;
  StbM_TimeStampType syncTime, actualTime;
  StbM_TimeStampRawType fupTimestampRaw;
  StbM_UserDataType userData;
  StbM_MeasurementType measureData = {0};

  /* ----- Implementation ----------------------------------------------- */
  slaveIdx = CanTSyn_DomainConfig[domainIdx].SlaveIdx;

  SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  messageType = CanTSyn_CanSignal_GetType(pduInfoPtr->SduDataPtr);

  /* #10 If the domain is an offset domain */
  if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* #20 assemble second portion of the offset time base */
    actualTime.secondsHi = CanTSyn_CanSignal_GetSecHi(CanTSyn_SlaveState[slaveIdx].SyncMsg) + ((uint16)CanTSyn_CanSignal_GetSecHi(pduInfoPtr->SduDataPtr) << 8U);
    actualTime.seconds = CanTSyn_CanSignal_GetTime(CanTSyn_SlaveState[slaveIdx].SyncMsg);
    /* #30 get nanosecond portion of the offset time */
    actualTime.nanoseconds = CanTSyn_CanSignal_GetTime(pduInfoPtr->SduDataPtr);
    /* #40 Set offset time base */
    (void)StbM_SetOffset(CanTSyn_DomainConfig[domainIdx].TimeBaseId, &actualTime); /* PRQA S 3685 */ /* MD_CanTSyn_3685 */
    SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    /* #50 Handle user data considering if Crc is used */
    userData.userByte0 = CanTSyn_CanSignal_GetUserByte0(CanTSyn_SlaveState[slaveIdx].SyncMsg);

    if (CanTSyn_IsFupCrcSecured(messageType))
    {
      userData.userDataLength = 1U;
    }
    else
    {
      /* user bytes 1 and 2 are only used if the message is not crc secured */
      userData.userByte1 = CanTSyn_CanSignal_GetUserByte1(CanTSyn_SlaveState[slaveIdx].SyncMsg);
      userData.userByte2 = CanTSyn_CanSignal_GetUserByte2(pduInfoPtr->SduDataPtr);
      userData.userDataLength = 3U;
    }

    /* #60 calculate T0 + T4 */
    syncSeconds = CanTSyn_CanSignal_GetTime(CanTSyn_SlaveState[slaveIdx].SyncMsg);
    syncTime.seconds = syncSeconds + CanTSyn_CanSignal_GetOVS(pduInfoPtr->SduDataPtr);
    if (syncTime.seconds < syncSeconds)
    {
      syncTime.secondsHi = 1U;
    }
    else
    {
      syncTime.secondsHi = 0U;
    }

    /* #70 get Synchronized Time Base part T4 delivered with FUP message */
    syncTime.nanoseconds = CanTSyn_CanSignal_GetTime(pduInfoPtr->SduDataPtr);

    SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #80 If retrieving of the time difference between T3raw and T2raw succeeds */
    if (StbM_GetCurrentTimeDiff(CanTSyn_SlaveState[slaveIdx].SyncTimestampRaw, &fupTimestampRaw) == E_OK)
    {
      /* #90 calculate GlobalTimeBase as GlobalTimeBase = (T3raw - T2raw) + (T0 + T4) */
      CanTSyn_TimestampPlusTimestampRaw(&syncTime, &fupTimestampRaw, &actualTime);

      /* #100 Get timebase status from message */
      if (CanTSyn_CanSignal_GetSGW(pduInfoPtr->SduDataPtr) == 0U)
      {
        actualTime.timeBaseStatus = 0x00U;
      }
      else
      {
        actualTime.timeBaseStatus = StbM_Sync_To_Gateway_BflMask;
      }

      /* #110 Pass information to StbM */
      (void)StbM_BusSetGlobalTime(CanTSyn_DomainConfig[domainIdx].TimeBaseId, &actualTime, &userData, &measureData);
    }
  }

  /* #120 Set slave state to CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC and deactive FollowUp received timeout */
  CanTSyn_SlaveState[slaveIdx].State = CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
  CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount = 0U;

} /* CanTSyn_HandleFup() */

/**********************************************************************************************************************
 *  CanTSyn_CheckRxSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckRxSequenceCounter(
  uint8 domainIdx,
  uint8 counter) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 slaveIdx, counterJumpWidth;

  /* ----- Implementation ----------------------------------------------- */
  slaveIdx = CanTSyn_DomainConfig[domainIdx].SlaveIdx;

  /* #10 sequence counter of first received message is not checked */
  if (CanTSyn_RxSequenceCounter[slaveIdx] <= CANTSYN_MAX_SEQUENCE_COUNTER)
  {
    /* #20 calculate JumpWidth from last received SC */
    if (counter < CanTSyn_RxSequenceCounter[slaveIdx])
    {
      counterJumpWidth = (CANTSYN_MAX_SEQUENCE_COUNTER - CanTSyn_RxSequenceCounter[slaveIdx]) + counter + 1U;
    }
    else
    {
      counterJumpWidth = counter - CanTSyn_RxSequenceCounter[slaveIdx];
    }

    /* #30 Check if JumpWidth is in the permitted range */
    if ((counterJumpWidth > 0U) && (counterJumpWidth <= CanTSyn_SlaveConfig[slaveIdx].SequenceCounterJumpWidth)) /* PRQA S 3689 */ /* MD_CanTSyn_3689 */
    {
      retVal = E_OK;
    }
  }
  else
  {
    retVal = E_OK;
  }

  /* #40 update last received sequence counter */
  CanTSyn_RxSequenceCounter[slaveIdx] = counter;

  return retVal;

} /* CanTSyn_CheckRxSequenceCounter() */

/**********************************************************************************************************************
 *  CanTSyn_MemCpy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_MemCpy(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_VAR) destination,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_DATA) source,
  uint8 count) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 copy from source to destination */
  for (;count > 0U; count-- )
  {
    destination[count - 1U] = source[count - 1U];
  }

} /* CanTSyn_MemCpy() */
#endif

/**********************************************************************************************************************
 *  CanTSyn_ProcessSend()
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
 * \trace DSGN-CanTSyn22807, DSGN-CanTSyn22808, DSGN-CanTSyn22809, DSGN-CanTSyn22806
 */
#if (CANTSYN_MASTER_COUNT)
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_ProcessSend(uint8 masterIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 domainIdx;
  Std_ReturnType retVal;
  uint8 currentUpdateCounter;

  /* ----- Implementation ----------------------------------------------- */
  domainIdx = CanTSyn_MasterConfig[masterIdx].DomainIdx;

  switch (CanTSyn_MasterState[masterIdx].State)
  {
    /* #10 If master is in state CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
    case CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND:
      /* #20 If module is in state CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND, transmission is enabled for the associated controller and the debounce counter has expired */
      if ((CanTSyn_TransmitState == CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND)
        && (CanTSyn_CanControllerTransmissionState[CanTSyn_PduIdToCanControllerIdxMapping[CanTSyn_MasterConfig[masterIdx].MasterConfHandleId]] == CANTSYN_TX_ON) 
        && (CanTSyn_TxDebounceCounter[CanTSyn_MasterConfig[masterIdx].MasterConfHandleId] == 0U))
      {
        /* #30 If ImmediateTimeSync is enabled */
        if (CanTSyn_MasterConfig[masterIdx].ImmediateTimeSync == TRUE) 
        {
          /* #40 Get current TimeBaseUpdateCounter from StbM */
          currentUpdateCounter = StbM_GetTimeBaseUpdateCounter(CanTSyn_DomainConfig[domainIdx].TimeBaseId);
        
          /* #50 If the TimeBaseUpdateCounter has changed */
          if (currentUpdateCounter != CanTSyn_MasterState[masterIdx].LastUpdateCounter)
          {
            /* #60 Save the current TimeBaseUpdateCounter, send a Sync message and set CyclicMsgResumeCounter */
            CanTSyn_MasterState[masterIdx].LastUpdateCounter = currentUpdateCounter;
            retVal = CanTSyn_PrepareAndSendSync(domainIdx, masterIdx);
            if (retVal == E_OK)
            {
              CanTSyn_MasterState[masterIdx].CyclicMsgResumeCounter = CanTSyn_MasterConfig[masterIdx].CyclicMsgResumeTime;
            }
          }
          else
          {
            /* #70 Send a message, if master Tx timeout has expired and CyclicMsgResumeCounter is not running */
            if ((CanTSyn_MasterState[masterIdx].TxIntervalCount == 0U) && (CanTSyn_MasterState[masterIdx].CyclicMsgResumeCounter == 0U))
            {
              retVal = CanTSyn_PrepareAndSendSync(domainIdx, masterIdx);
            }
          }
        }
        else
        {
          /* #80 Send a message, if master Tx timeout has expired */
          if (CanTSyn_MasterState[masterIdx].TxIntervalCount == 0U)
          {
            retVal = CanTSyn_PrepareAndSendSync(domainIdx, masterIdx);
          }
        }
      }
      break;

    /* #90 If master is in state CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATIONS */
    case CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATION:
      /* #100 If the master confirmation timeout expires */
      if (CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount == 0U)
      {
        /* #110 Set modul and master state to CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
        SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        CanTSyn_TransmitState = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
        CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
        SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
      break;

    /* #120 If master is in state CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND */
    case CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND:
      /* #130 Delay FUP/OFNS transmission until debouncing is finished */
      if(CanTSyn_TxDebounceCounter[CanTSyn_MasterConfig[masterIdx].MasterConfHandleId] == 0U)
      {
        /* #140 If transmission on interface succeded */
        if (CanTSyn_TransmitFup(masterIdx) == E_OK)
        {
          /* #150 Set modul and master state to CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION and reset confirmation timeout */
          SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          CanTSyn_TransmitState = CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION;
          CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION;
          CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount = CanTSyn_MasterConfig[masterIdx].MasterConfTimeout;
          SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
      }
      break;

    /* #160 If master is in state CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION */
    case CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION:
      /* #170 If the master confirmation timeout expires */
      if (CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount == 0U)
      {
        /* #180 Set modul and master state to CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
        SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        CanTSyn_TransmitState = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
        CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
        SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
      break;
    default:
        ;
  }

} /* CanTSyn_ProcessSend() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CanTSyn_TransmitSync()
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
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_TransmitSync(uint8 masterIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  PduInfoType pduInfo;
  CanTSyn_CanMsgType syncMsg;
  uint8 domainIdx, counter;
# if (CANTSYN_CRC_USED)
  uint8 crcResult, dataId;
# endif

  /* ----- Implementation ----------------------------------------------- */
  domainIdx = CanTSyn_MasterConfig[masterIdx].DomainIdx;

  SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  pduInfo.SduLength  = CANTSYN_TIME_SYNC_MESSAGE_DLC;
  pduInfo.SduDataPtr = (SduDataPtrType) syncMsg;

  /* #10 increment sequence counter on every transmission request of a SYNC message */
  CanTSyn_IncrementSequenceCounter(masterIdx);
  counter = CanTSyn_TxSequenceCounter[masterIdx];

  if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* #20 Assemble unconditioned parts of OFS message */
    /* Domain numbers of offset time bases (16-31) are carried in TimeSync messages as Domain number - 16 */
    CanTSyn_CanSignal_SetD(syncMsg, CanTSyn_DomainConfig[domainIdx].DomainId - CANTSYN_START_OFFSET_TIME_DOMAIN_ID);
    /* write secondsHi portion of Offset Time Base to OfsTimeSecLsbHi */
    CanTSyn_CanSignal_SetSecHi(syncMsg, CanTSyn_MasterState[masterIdx].SyncTimestamp.secondsHi);
  }
  else
  {
    /* #30 Assemble unconditioned parts of SYNC message */
    CanTSyn_CanSignal_SetD(syncMsg, CanTSyn_DomainConfig[domainIdx].DomainId);
    CanTSyn_CanSignal_SetUserByte0(syncMsg, CanTSyn_MasterState[masterIdx].UserData.userByte0);
  }

  /* #40 Add joint message parts */
  /* for SYNC message write second portion of T0 to SyncTimeSec,
     for OFS message write second portion of Offset Time Base to OfsTimeSecLsbLo */
  CanTSyn_CanSignal_SetTime(syncMsg, CanTSyn_MasterState[masterIdx].SyncTimestamp.seconds)

  CanTSyn_CanSignal_SetSC(syncMsg, counter);

  /* #50 If no CRC is used, add the message type and user data for the type of the domain */
  if (CanTSyn_MasterConfig[masterIdx].TxCrcSecured == CANTSYN_CRC_NOT_SUPPORTED)
  {
    if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
    {
      CanTSyn_CanSignal_SetType(syncMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC);
      /* set reserved byte to default value 0 */
      CanTSyn_CanSignal_SetReservedOfs(syncMsg, 0U);
    }
    else
    {
      CanTSyn_CanSignal_SetType(syncMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC);
      CanTSyn_CanSignal_SetUserByte1(syncMsg, CanTSyn_MasterState[masterIdx].UserData.userByte1);
    }
  }
# if (CANTSYN_CRC_USED)
  else
  {
    /* #60 Add the message type for type of the domain */
    if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
    {
      CanTSyn_CanSignal_SetType(syncMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC);
    }
    else
    {
      CanTSyn_CanSignal_SetType(syncMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC);
    }

    /* #70 calculate DataID as DataIDList[SC] */
    dataId = CanTSyn_DomainConfig[domainIdx].SyncDataIDList[counter];

    /* #80 Generate CRC and add to message */
    crcResult = Crc_CalculateCRC8H2F(&(syncMsg[2U]), 6U, CANTSYN_CRC_START_VALUE, TRUE);
    crcResult = Crc_CalculateCRC8H2F(&dataId, 1U, crcResult, FALSE);
    CanTSyn_CanSignal_SetCRC(syncMsg, crcResult);
  }
# endif

  /* #90 If the current domain is a synchronized one */
  if (CanTSyn_DomainConfig[domainIdx].DomainId < CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* #100 get raw time T0raw for time measurement of transmission delay */
    SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = StbM_GetCurrentTimeRaw(&CanTSyn_MasterState[masterIdx].SyncTimestampRaw);
    SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    retVal = E_OK;
  }

  /* #110 If the previous actions succeeded, transmit message on interface */
  if (retVal == E_OK)
  {
    retVal = CanIf_Transmit(CanTSyn_MasterConfig[masterIdx].TxPduId, &pduInfo);
  }

  SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;

} /* CanTSyn_TransmitSync() */

/**********************************************************************************************************************
 *  CanTSyn_TransmitFup()
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
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_TransmitFup(uint8 masterIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  PduInfoType pduInfo;
  CanTSyn_CanMsgType fupMsg;
  uint8 domainIdx, counter;
# if (CANTSYN_CRC_USED)
  uint8 crcResult, dataId;
# endif

  /* ----- Implementation ----------------------------------------------- */
  domainIdx = CanTSyn_MasterConfig[masterIdx].DomainIdx;

  SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  pduInfo.SduLength  = CANTSYN_TIME_SYNC_MESSAGE_DLC;
  pduInfo.SduDataPtr = (SduDataPtrType) fupMsg;

  /* #10 Add unconditioned information depending on the type of domain to the message */
  if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* Domain numbers of offset time bases (16-31) are carried in TimeSync messages as Domain number - 16 */
    CanTSyn_CanSignal_SetD(fupMsg, CanTSyn_DomainConfig[domainIdx].DomainId - CANTSYN_START_OFFSET_TIME_DOMAIN_ID);
    /* write high bytes of secondsHi of Offset Time Base to OfsTimeSecMsbHi */
    CanTSyn_CanSignal_SetSecHi(fupMsg, (uint8)(CanTSyn_MasterState[masterIdx].SyncTimestamp.secondsHi >> 8U));
    /* write nanosecond portion of Offset Time Base to OfsTimeNSec */
    CanTSyn_CanSignal_SetTime(fupMsg, CanTSyn_MasterState[masterIdx].SyncTimestamp.nanoseconds)
  }
  else
  {
    CanTSyn_CanSignal_SetD(fupMsg, CanTSyn_DomainConfig[domainIdx].DomainId);
    /* set reserved bits to default value 0 */
    CanTSyn_CanSignal_SetReservedFup(fupMsg, 0U);
    CanTSyn_CanSignal_SetSGW(fupMsg, CanTSyn_GetSyncToGatewayBit(CanTSyn_MasterState[masterIdx].SyncTimestamp.timeBaseStatus));
    /* write second portion of T4 to OVS */
    CanTSyn_CanSignal_SetOVS(fupMsg, CanTSyn_MasterState[masterIdx].FupTimestamp.seconds);
    /* write nanosecond portion of T4 to SyncTimeNSec */
    CanTSyn_CanSignal_SetTime(fupMsg, CanTSyn_MasterState[masterIdx].FupTimestamp.nanoseconds)
  }

  /* #20 Add the SC to the message */
  counter = CanTSyn_TxSequenceCounter[masterIdx];
  CanTSyn_CanSignal_SetSC(fupMsg, counter);

  /* #30 If no CRC is used, add the message type and user data for the type of the domain */
  if (CanTSyn_MasterConfig[masterIdx].TxCrcSecured == CANTSYN_CRC_NOT_SUPPORTED)
  {
    if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
    {
      CanTSyn_CanSignal_SetType(fupMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_NOT_CRC);
      /* set reserved byte to default value 0 */
      CanTSyn_CanSignal_SetReservedOfs(fupMsg, 0U);
    }
    else
    {
      CanTSyn_CanSignal_SetType(fupMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_NOT_CRC);
      CanTSyn_CanSignal_SetUserByte2(fupMsg, CanTSyn_MasterState[masterIdx].UserData.userByte2);
    }
  }
# if (CANTSYN_CRC_USED)
  else
  {
    /* #40 Add the message type for type of the domain */
    if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
    {
      CanTSyn_CanSignal_SetType(fupMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_OFNS_CRC);
    }
    else
    {
      CanTSyn_CanSignal_SetType(fupMsg, CANTSYN_TIME_SYNC_MESSAGE_TYPE_FUP_CRC);
    }

    /* #50 calculate DataID as DataIDList[SC] */
    dataId = CanTSyn_DomainConfig[domainIdx].FupDataIDList[counter];

    /* #60 Generate CRC and add to message */
    crcResult = Crc_CalculateCRC8H2F(&(fupMsg[2U]), 6U, CANTSYN_CRC_START_VALUE, TRUE);
    crcResult = Crc_CalculateCRC8H2F(&dataId, 1U, crcResult, FALSE);
    CanTSyn_CanSignal_SetCRC(fupMsg, crcResult);
  }
# endif

  /* #110 Transmit message on interface */
  if (CanIf_Transmit(CanTSyn_MasterConfig[masterIdx].TxPduId, &pduInfo) == E_OK)
  {
    retVal = E_OK;
  }

  SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;

} /* CanTSyn_TransmitFup() */

/**********************************************************************************************************************
 *  CanTSyn_IncrementSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_IncrementSequenceCounter(uint8 masterIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If sequence counter overflow occurs */
  if (CanTSyn_TxSequenceCounter[masterIdx] == CANTSYN_MAX_SEQUENCE_COUNTER)
  {
    /* #20 Reset sequence counter */
    CanTSyn_TxSequenceCounter[masterIdx] = 0U;
  }
  else
  {
    /* #30 Increment sequence counter */
    CanTSyn_TxSequenceCounter[masterIdx]++;
  }

} /* CanTSyn_IncrementSequenceCounter() */
#endif

/**********************************************************************************************************************
 *  CanTSyn_InitDomain()
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
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_InitDomain(uint8 domainIdx) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
#if (CANTSYN_SLAVE_COUNT)
  uint8 slaveIdx;
#endif
#if (CANTSYN_MASTER_COUNT)
  uint8 masterCount, masterIdx;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (CANTSYN_SLAVE_COUNT)
  /* #10 init slave state */
  if (CanTSyn_DomainHasSlave(domainIdx))
  {
    slaveIdx = CanTSyn_DomainConfig[domainIdx].SlaveIdx;
    CanTSyn_SlaveState[slaveIdx].State = CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
    CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount = 0U;
    /* #20 initialize Rx sequence counter with invalid value to enable detection of no received message */
    CanTSyn_RxSequenceCounter[slaveIdx] = CANTSYN_MAX_SEQUENCE_COUNTER + 1U;
  }
#endif

#if (CANTSYN_MASTER_COUNT)
  /* #30 init master state */
  if (CanTSyn_DomainHasMaster(domainIdx))
  {
    for (masterCount = 0U; masterCount < CanTSyn_DomainConfig[domainIdx].MasterCount; masterCount++)
    {
      masterIdx = CanTSyn_DomainConfig[domainIdx].MasterStartIdx + masterCount;
      CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
      CanTSyn_MasterState[masterIdx].TxIntervalCount = CanTSyn_MasterConfig[masterIdx].TxPeriod;
      CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount = 0U;
      /* #40 initialize Immediate TimeSync counters */
      if (CanTSyn_MasterConfig[masterIdx].ImmediateTimeSync == TRUE)
      {
        CanTSyn_MasterState[masterIdx].CyclicMsgResumeCounter = 0U;
        CanTSyn_MasterState[masterIdx].LastUpdateCounter = 0U;
      }
      /* #50 initialize Tx sequence counter */
      CanTSyn_TxSequenceCounter[masterIdx] = CANTSYN_MAX_SEQUENCE_COUNTER;
    }
  }
#endif

} /* CanTSyn_InitDomain() */

/**********************************************************************************************************************
 *  CanTSyn_TimestampPlusTimestampRaw()
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
CANTSYN_LOCAL_INLINE FUNC(void, CANTSYN_CODE) CanTSyn_TimestampPlusTimestampRaw(
  P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_APPL_DATA) timestampPtr,
  P2CONST(StbM_TimeStampRawType, AUTOMATIC, CANTSYN_APPL_DATA) timestampRawPtr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, CANTSYN_APPL_VAR) timesumPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 overflowSeconds = 0U;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #10 Calculate the sum of nanoseconds */
  timesumPtr->nanoseconds = timestampPtr->nanoseconds + *timestampRawPtr;

  /* #20 check for overflow of nanoseconds */
  if (timesumPtr->nanoseconds < *timestampRawPtr)
  {
    /* #30 split seconds from timestampRawPtr */
    overflowSeconds = (uint8)(*timestampRawPtr / (CANTSYN_TIMESTAMP_MAX_NANOSECONDS + 1U));
    timesumPtr->nanoseconds = (*timestampRawPtr % (CANTSYN_TIMESTAMP_MAX_NANOSECONDS + 1U)) + timestampPtr->nanoseconds;
  }
  /* #40 check if nanoseconds are still bigger than one second */
  if (timesumPtr->nanoseconds > CANTSYN_TIMESTAMP_MAX_NANOSECONDS)
  {
    /* #50 Split seconds from timesumPtr */
    overflowSeconds += (uint8)(timesumPtr->nanoseconds / (CANTSYN_TIMESTAMP_MAX_NANOSECONDS + 1U));
    timesumPtr->nanoseconds = timesumPtr->nanoseconds % (CANTSYN_TIMESTAMP_MAX_NANOSECONDS + 1U);
  }

  /* #60 Set resulting seconds and secondsHi */
  timesumPtr->seconds = timestampPtr->seconds;
  timesumPtr->secondsHi = timestampPtr->secondsHi;

  /* #70 If a split of seconds occured */
  if (overflowSeconds > 0U)
  {
    /* #80 Increment secondsHi, if an overflow of seconds occured */
    if (timestampPtr->seconds > (CANTSYN_TIMESTAMP_MAX_SECONDS - overflowSeconds))
    {
        timesumPtr->secondsHi++;
    }
    /* #90 Adapt the seconds */
    timesumPtr->seconds += overflowSeconds;
  }

  SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* CanTSyn_TimestampPlusTimestampRaw() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CanTSyn_PrepareAndSendSync()
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
#if (CANTSYN_MASTER_COUNT)
CANTSYN_LOCAL_INLINE FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_PrepareAndSendSync(uint8 domainIdx, uint8 masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If domain is an offset domain */
  if (CanTSyn_DomainConfig[domainIdx].DomainId >= CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
  {
    /* #20 get Offset Time Base */
    retVal = StbM_GetOffset(CanTSyn_DomainConfig[domainIdx].TimeBaseId, &CanTSyn_MasterState[masterIdx].SyncTimestamp); /* PRQA S 3685 */ /* MD_CanTSyn_3685 */
  }
  else
  {
    /* #30 get Synchronized Time Base T0 */
    retVal = StbM_GetCurrentTime(CanTSyn_DomainConfig[domainIdx].TimeBaseId, &CanTSyn_MasterState[masterIdx].SyncTimestamp, &CanTSyn_MasterState[masterIdx].UserData);
  }

  /* #40 If previous operations succeded and GlobalTimeBaseBit is set */
  if ((retVal == E_OK) && (CanTSyn_GetGlobalTimeBaseBit(CanTSyn_MasterState[masterIdx].SyncTimestamp.timeBaseStatus) != 0U))
  {
    /* #50 If transmission on interface succeded */
    retVal = CanTSyn_TransmitSync(masterIdx);
    if (retVal == E_OK)
    {
      /* #60 Set modul and master state to CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATION and reset master timeouts */
      SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      CanTSyn_TransmitState = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATION;
      CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATION;
      CanTSyn_MasterState[masterIdx].TxIntervalCount = CanTSyn_MasterConfig[masterIdx].TxPeriod;
      CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount = CanTSyn_MasterConfig[masterIdx].MasterConfTimeout;
      SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  return retVal;
} /* CanTSyn_PrepareAndSendSync() */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CANTSYN_SLAVE_COUNT)
/**********************************************************************************************************************
 *  CanTSyn_RxIndication()
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
 */
FUNC(void, CANTSYN_CODE) CanTSyn_RxIndication(
  PduIdType RxPduId, 
  P2CONST(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = CANTSYN_E_NO_ERROR;
# endif
  uint8 domainId, domainIdx, slaveIdx, messageType, counter;
# if (CANTSYN_CRC_USED)
  uint8 crcResult, dataId;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (CanTSyn_ModuleInitialized == (uint8)CANTSYN_UNINIT)
  {
    errorId = CANTSYN_E_NOT_INITIALIZED;
  }
  /* #20 Check parameter 'PduInfoPtr' for NULL pointer */
  else if (PduInfoPtr == NULL_PTR)
  {
    errorId = CANTSYN_E_NULL_POINTER;
  }
  /* #30 Check parameter 'PduInfoPtr->SduDataPtr' */
  else if (PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = CANTSYN_E_NULL_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 check DLC of received message */
    if (PduInfoPtr->SduLength == CANTSYN_TIME_SYNC_MESSAGE_DLC)
    {
      domainId = CanTSyn_CanSignal_GetD(PduInfoPtr->SduDataPtr);
      messageType = CanTSyn_CanSignal_GetType(PduInfoPtr->SduDataPtr);

      if (CanTSyn_IsOfsMsg(messageType) || CanTSyn_IsOfnsMsg(messageType))
      { /* TimeSync messages of Offset Time Bases carry the Domain number - 16 */
        domainId += CANTSYN_START_OFFSET_TIME_DOMAIN_ID;
      }

      /* #50 check if the domain matches to one of the configured Time Domains */
      for (domainIdx = 0U; domainIdx < CANTSYN_DOMAIN_COUNT; domainIdx++)
      {
        if (CanTSyn_DomainConfig[domainIdx].DomainId == domainId)
        {
          if (CanTSyn_DomainHasSlave(domainIdx))
          {
            slaveIdx = CanTSyn_DomainConfig[domainIdx].SlaveIdx;

            /* #60 check if the received PduId matches to the configured PduId */
            if (CanTSyn_SlaveConfig[slaveIdx].SlaveHandleId == RxPduId)
            {
              /* #70 check if SYNC message was received and expected */
              if ((CanTSyn_SlaveState[slaveIdx].State == CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC) && (CanTSyn_IsSyncMsg(messageType) || CanTSyn_IsOfsMsg(messageType)))
              { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                counter = CanTSyn_CanSignal_GetSC(PduInfoPtr->SduDataPtr);

                /* #80 check if SC matches to the expected value */
                if (CanTSyn_CheckRxSequenceCounter(domainIdx, counter) == E_OK)
                {
                  switch (CanTSyn_SlaveConfig[slaveIdx].RxCrcValidated) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  {
                    case CANTSYN_CRC_IGNORED:
                      CanTSyn_HandleSync(domainIdx, PduInfoPtr);
                      break;

                    case CANTSYN_CRC_NOT_VALIDATED:
                      /* #90 check if Type matches depending on the CanTSynRxCrcValidated parameter */
                      if (!CanTSyn_IsSyncCrcSecured(messageType))
                      {
                        CanTSyn_HandleSync(domainIdx, PduInfoPtr);
                      }
                      break;

# if (CANTSYN_CRC_USED)
                    case CANTSYN_CRC_VALIDATED:
                      /* #90 check if Type matches depending on the CanTSynRxCrcValidated parameter */
                      if (CanTSyn_IsSyncCrcSecured(messageType))
                      {
                        /* #100 calculate CRC over received PDU */
                        crcResult = Crc_CalculateCRC8H2F(&(PduInfoPtr->SduDataPtr[2U]), 6U, CANTSYN_CRC_START_VALUE, TRUE);
                        dataId = CanTSyn_DomainConfig[domainIdx].SyncDataIDList[counter];
                        crcResult = Crc_CalculateCRC8H2F(&dataId, 1U, crcResult, FALSE);

                        /* #110 check CRC */
                        if (crcResult == CanTSyn_CanSignal_GetCRC(PduInfoPtr->SduDataPtr))
                        {
                          CanTSyn_HandleSync(domainIdx, PduInfoPtr);
                        }
                      }
                      break;
# endif

                    default:
                      ;
                  }
                }
              }
              /* #120 check if FUP message was received and expected */
              else if ((CanTSyn_SlaveState[slaveIdx].State == CANTSYN_STATE_RECEIVE_WAITING_FOR_FOLLOW_UP) && (CanTSyn_IsFupMsg(messageType) || CanTSyn_IsOfnsMsg(messageType))) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
              {
                counter = CanTSyn_CanSignal_GetSC(PduInfoPtr->SduDataPtr);

                /* #130 check if SC matches to the expected value and SyncTimeNSec matches the defined range of StbM_TimeStampType.nanoseconds */
                if ((counter != CanTSyn_CanSignal_GetSC(CanTSyn_SlaveState[slaveIdx].SyncMsg)) || (CanTSyn_CanSignal_GetTime(PduInfoPtr->SduDataPtr) > CANTSYN_TIMESTAMP_MAX_NANOSECONDS))
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  CanTSyn_SlaveState[slaveIdx].State = CANTSYN_STATE_RECEIVE_WAITING_FOR_SYNC;
                  CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount = 0U;
                }
                else
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  switch (CanTSyn_SlaveConfig[slaveIdx].RxCrcValidated)
                  {
                    case CANTSYN_CRC_IGNORED:
                      CanTSyn_HandleFup(domainIdx, PduInfoPtr);
                      break;

                    case CANTSYN_CRC_NOT_VALIDATED:
                      /* #140 check if Type matches depending on the CanTSynRxCrcValidated parameter */
                      if (!CanTSyn_IsFupCrcSecured(messageType))
                      {
                        CanTSyn_HandleFup(domainIdx, PduInfoPtr);
                      }
                      break;

# if (CANTSYN_CRC_USED)
                    case CANTSYN_CRC_VALIDATED:
                      /* #150 check if Type matches depending on the CanTSynRxCrcValidated parameter */
                      if (CanTSyn_IsFupCrcSecured(messageType))
                      {
                        /* #160 calculate CRC over received PDU */
                        crcResult = Crc_CalculateCRC8H2F(&(PduInfoPtr->SduDataPtr[2U]), 6U, CANTSYN_CRC_START_VALUE, TRUE);
                        dataId = CanTSyn_DomainConfig[domainIdx].FupDataIDList[counter];
                        crcResult = Crc_CalculateCRC8H2F(&dataId, 1U, crcResult, FALSE);

                        /* #170 check CRC */
                        if (crcResult == CanTSyn_CanSignal_GetCRC(PduInfoPtr->SduDataPtr))
                        {
                          CanTSyn_HandleFup(domainIdx, PduInfoPtr);
                        }
                      }
                      break;
# endif

                    default:
                      ;
                  }
                }
              }
              else
              {
                /* not expected message */
              }
            }
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
            else
            {
              /* #180 Domain has configured different RxPduId */
              errorId = CANTSYN_E_INVALID_PDUID;
            }
# endif
          }
          break;
        }
      }
    }
  }

# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != CANTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID_DET, CANTSYN_SID_RX_INDICATION, errorId);
  }
# endif

} /* CanTSyn_RxIndication() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CANTSYN_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CanTSyn_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CANTSYN_CODE) CanTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTSYN_APPL_VAR) versioninfo) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = CANTSYN_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter versioninfo for NULL pointer */
  if (versioninfo == NULL_PTR)
  {
    errorId = CANTSYN_E_NULL_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo with corresponding macros from component header */
    versioninfo->vendorID = (CANTSYN_VENDOR_ID);
    versioninfo->moduleID = (CANTSYN_MODULE_ID);
    versioninfo->sw_major_version = (CANTSYN_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (CANTSYN_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (CANTSYN_SW_PATCH_VERSION);
  }

# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != CANTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID_DET, CANTSYN_SID_GET_VERSION_INFO, errorId);
  }
# endif

} /* CanTSyn_GetVersionInfo() */
#endif /* (CANTSYN_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  CanTSyn_MainFunction()
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
FUNC(void, CANTSYN_CODE) CanTSyn_MainFunction(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = CANTSYN_E_NO_ERROR;
#endif
  uint8 domainIdx;
#if (CANTSYN_SLAVE_COUNT)
  uint8 slaveIdx;
#endif
#if (CANTSYN_MASTER_COUNT)
  uint8 masterCount, masterIdx, txPduCount;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (CanTSyn_ModuleInitialized == (uint8)CANTSYN_UNINIT)
  {
    errorId = CANTSYN_E_NOT_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Iterate over all domains */
    for (domainIdx = 0U; domainIdx < CANTSYN_DOMAIN_COUNT; domainIdx++)
    {
#if (CANTSYN_SLAVE_COUNT)
      /* #30 If the domain has a slave */
      if (CanTSyn_DomainHasSlave(domainIdx))
      {
        slaveIdx = CanTSyn_DomainConfig[domainIdx].SlaveIdx;

        /* #40 Rx Timeout Handling */
        SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        if (CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount > 0U)
        {
          CanTSyn_SlaveState[slaveIdx].RxFollowUpTimeoutCount--;
        }
        SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /* #50 Process the received message */
        CanTSyn_ProcessReceive(slaveIdx);
      }
#endif

#if (CANTSYN_MASTER_COUNT)
      /* #60 If the domain has at least one master role */
      if (CanTSyn_DomainHasMaster(domainIdx))
      {
        /* #70 Iterate over all masters of the domain */
        for (masterCount = 0U; masterCount < CanTSyn_DomainConfig[domainIdx].MasterCount; masterCount++)
        {
          masterIdx = CanTSyn_DomainConfig[domainIdx].MasterStartIdx + masterCount;

          /* #80 TxPeriod Handling */
          if (CanTSyn_MasterState[masterIdx].TxIntervalCount > 0U)
          {
            CanTSyn_MasterState[masterIdx].TxIntervalCount--;
          }

          /* #90 MasterConfTimeout Handling */
          if (CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount > 0U)
          {
            CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount--;
          }

          /* #100 CyclicMsgResumeCounter Handling */
          if ((CanTSyn_MasterConfig[masterIdx].ImmediateTimeSync == TRUE) && (CanTSyn_MasterState[masterIdx].CyclicMsgResumeCounter > 0U)) 
          {
            CanTSyn_MasterState[masterIdx].CyclicMsgResumeCounter--;
          }

          /* #110 Message transmission handling */
          CanTSyn_ProcessSend(masterIdx);
        }
      }
#endif
    }

#if (CANTSYN_MASTER_COUNT)
    /* #120 DebounceCounter Handling */
    for (txPduCount = 0U; txPduCount < CANTSYN_PDUID_COUNT; txPduCount++)
    {
      if (CanTSyn_TxDebounceCounter[txPduCount] > 0U)
      {
        CanTSyn_TxDebounceCounter[txPduCount]--;
      }
    }
#endif
  }

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != CANTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID_DET, CANTSYN_SID_MAIN_FUNCTION, errorId);
  }
#endif

} /* CanTSyn_MainFunction() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CanTSyn_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CANTSYN_CODE)  CanTSyn_InitMemory(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  CanTSyn_ModuleInitialized = CANTSYN_UNINIT;
#endif

} /* CanTSyn_InitMemory() */

/**********************************************************************************************************************
 *  CanTSyn_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CANTSYN_CODE) CanTSyn_Init(P2CONST(CanTSyn_ConfigType, AUTOMATIC, CANTSYN_APPL_DATA) configPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 domainIdx;
#if (CANTSYN_MASTER_COUNT)
  uint8 canControllerIdx, txPduCount;
#endif

  /* ----- Implementation ----------------------------------------------- */
  configPtr = configPtr; /* PRQA S 3199 */ /* MD_CanTSyn_3199 */

  /* init all domains */
  for (domainIdx = 0u; domainIdx < CANTSYN_DOMAIN_COUNT; domainIdx++)
  {
    CanTSyn_InitDomain(domainIdx);
  }

#if (CANTSYN_MASTER_COUNT)
  CanTSyn_TransmitState = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
  for (canControllerIdx = 0U; canControllerIdx < CANTSYN_CANCONTROLLER_COUNT; canControllerIdx++)
  {
    CanTSyn_CanControllerTransmissionState[canControllerIdx] = CANTSYN_TX_ON;
  }

  for (txPduCount = 0U; txPduCount < CANTSYN_PDUID_COUNT; txPduCount++)
  {
    CanTSyn_TxDebounceCounter[txPduCount] = 0U;
  }
#endif

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  CanTSyn_ModuleInitialized = (uint8)CANTSYN_INIT;
#endif

} /* CanTSyn_Init() */

#if (CANTSYN_MASTER_COUNT)
/**********************************************************************************************************************
 *  CanTSyn_TxConfirmation()
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
FUNC(void, CANTSYN_CODE) CanTSyn_TxConfirmation(PduIdType TxPduId) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = CANTSYN_E_NO_ERROR;
# endif
  uint8 domainIdx;
  uint8 domainCount, masterCount, masterIdx;
  StbM_TimeStampType syncTime;
  StbM_TimeStampRawType syncTimestampRaw, fupTimestampRaw;
  boolean foundMaster = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  domainIdx = CANTSYN_INV_DOMAIN_IDX;
  /* #10 Check initialization state of the component */
  if (CanTSyn_ModuleInitialized == (uint8)CANTSYN_UNINIT)
  {
    errorId = CANTSYN_E_NOT_INITIALIZED;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* search for domain index */
    for (domainCount = 0U; domainCount < CANTSYN_DOMAIN_COUNT; domainCount++)
    {
      if (CanTSyn_DomainHasMaster(domainCount))
      {
        for (masterCount = 0U; masterCount < CanTSyn_DomainConfig[domainCount].MasterCount; masterCount++)
        {
          masterIdx = CanTSyn_DomainConfig[domainCount].MasterStartIdx + masterCount;
          /* #20 If the confirmation belongs to a SYNC or OFS message */
          if (CanTSyn_MasterConfig[masterIdx].MasterConfHandleId == TxPduId)
          {
            domainIdx = domainCount;
            if (CanTSyn_MasterState[masterIdx].State == CANTSYN_STATE_SEND_WAITING_FOR_SYNC_TX_CONFIRMATION)
            {
              /* #30 Prepare transmission of FUP message */
              if (CanTSyn_DomainConfig[domainIdx].DomainId < CANTSYN_START_OFFSET_TIME_DOMAIN_ID)
              {
                /* retrieve time difference T0diff of the transmission delay */
                SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                syncTimestampRaw = CanTSyn_MasterState[masterIdx].SyncTimestampRaw;

                SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                (void)StbM_GetCurrentTimeDiff(syncTimestampRaw, &fupTimestampRaw);
                SchM_Enter_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

                /* calculate T4 for FUP message as T4 = (T0ns + T0diff) with T0ns as nanosecond portion of T0 */
                syncTime.secondsHi = 0U;
                syncTime.seconds = 0U;
                syncTime.nanoseconds = CanTSyn_MasterState[masterIdx].SyncTimestamp.nanoseconds;

                CanTSyn_TimestampPlusTimestampRaw(&syncTime, &fupTimestampRaw, &CanTSyn_MasterState[masterIdx].FupTimestamp);
                /* #40 Set state to CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND */
                CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND;

                SchM_Exit_CanTSyn_CANTSYN_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
              }
              else
              {
                /* #40 Set state to CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND */
                CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_SEND;
              }

              foundMaster = TRUE;
            }
            /* #50 If the confirmation belongs to a FUP message */
            else if (CanTSyn_MasterState[masterIdx].State == CANTSYN_STATE_SEND_WAITING_FOR_FOLLOW_UP_TX_CONFIRMATION)
            {
              /* #60 Set modul and master state to CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND */
              CanTSyn_TransmitState = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
              CanTSyn_MasterState[masterIdx].State = CANTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND;
              foundMaster = TRUE;
            }
            else
            {
              /* for MISRA */
            }
            if (foundMaster == TRUE)
            {
              /* #70 Disable master confirmation timeout */
              CanTSyn_MasterState[masterIdx].MasterConfTimeoutCount = 0U;
              /* #80 Start debounce timer for this TxPduId */
              CanTSyn_TxDebounceCounter[CanTSyn_MasterConfig[masterIdx].MasterConfHandleId] = CanTSyn_MasterConfig[masterIdx].DebounceTime;
              break;
            }
          }
        }
        if (foundMaster == TRUE)
        {
          break;
        }
      }
    }

# if (CANTSYN_DEV_ERROR_DETECT)
    if (domainIdx == CANTSYN_INV_DOMAIN_IDX)
    {
      /* found no corresponding domain index -> drop message */
      errorId = CANTSYN_E_INVALID_PDUID;
    }
# endif
  }

# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != CANTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID_DET, CANTSYN_SID_TX_CONFIRMATION, errorId);
  }
# endif

} /* CanTSyn_TxConfirmation() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CanTSyn_SetTransmissionMode()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SetTransmissionMode(
  uint8 CtrlIdx,
  CanTSyn_TransmissionModeType Mode) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = CANTSYN_E_NO_ERROR;
# endif
  uint8 canControllerIdx;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (CanTSyn_ModuleInitialized == (uint8)CANTSYN_UNINIT)
  {
    errorId = CANTSYN_E_NOT_INITIALIZED;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Search for CAN controllers with given CtrlIdx */
    for (canControllerIdx = 0U; canControllerIdx < CANTSYN_CANCONTROLLER_COUNT; canControllerIdx++)
    {
      if (CanTSyn_CanControllerIdList[canControllerIdx] == CtrlIdx)
      {
        /* #30 Set transmission mode to given mode */
        CanTSyn_CanControllerTransmissionState[canControllerIdx] = Mode;
        break;
      }
    }

    retVal = E_OK;
  }

# if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
  if (errorId != CANTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID_DET, CANTSYN_SID_SET_TRANSMISSION_MODE, errorId);
  }
# endif

  return retVal;

} /* CanTSyn_SetTransmissionMode() */
#endif

#define CANTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_CanTSyn_3199:
      Reason:     Dummy statement required to avoid compiler warnings.
      Risk:       No functional risk.
      Prevention: Not required.
 MD_CanTSyn_3685: rule 21.1
      Reason:     A function parameter is used to access an array element. The parameter is always passed by an internal function that ensures that the parameter is in a valid range.
      Risk:       Runtime failure might be introduced by later code changes.
      Prevention: Covered by code review.
 MD_CanTSyn_3689: rule 21.1
      Reason:     A function parameter is used to access an array element. The parameter is always passed by an internal function that ensures that the parameter is in a valid range.
      Risk:       Runtime failure might be introduced by later code changes.
      Prevention: Covered by code review.
*/

/* COV_JUSTIFICATION_BEGIN

--- Preprocessor Coverage Justifications ------------------------------------------------------------------------------

  \ID COV_CANTSYN_MSR_COMPATIBILITY
  \ACCEPT TX 
  \REASON [COV_MSR_COMPATIBILITY]
  
COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: CanTSyn.c
 *********************************************************************************************************************/
