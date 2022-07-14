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
/*!        \file  StbM.c
 *        \brief  Vector AUTOSAR StbM code file
 *
 *      \details  This is the main implementation file of the Vector AUTOSAR module StbM.
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

#define STBM_SOURCE
/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA S 3109 EOF */ /* MD_MSR_19.4 */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "StbM.h"
#include "SchM_StbM.h"
#if (STBM_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif
#if ((STBM_GPT_LOCALTIME_USED == STD_ON) || (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON))
# include "Gpt.h"
#endif
#include "StbM_Cbk.h"
#include "Rte_StbM.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of StbM header file */
#if (  (STBM_SW_MAJOR_VERSION != (5U)) \
    || (STBM_SW_MINOR_VERSION != (2U)) \
    || (STBM_SW_PATCH_VERSION != (0U)) )
# error "Vendor specific version numbers of StbM.c and StbM.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (STBM_CFG_MAJOR_VERSION != (5U)) \
    || (STBM_CFG_MINOR_VERSION != (2U)) )
# error "Version numbers of StbM.c and StbM_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define STBM_TIMESTAMP_MAX_NANOSECONDS                  (999999999UL)   /* 10^9 nanoseconds per second */
#define STBM_TIMESTAMP_MAX_SECONDS                      (4294967295UL)
#define STBM_TIMESTAMP_MAX_SECONDSHI                    (65535U)
#define STBM_TIMESTAMPRAW_MAX_NANOSECONDS               (4294967295UL)

#define STBM_TIMEDIFF_MAX_NANOSECONDS                   (0x7FFFFFFFFFFFFFFFLL)
#define STBM_TIMEDIFF_MAX_SECONDS                       (STBM_TIMEDIFF_MAX_NANOSECONDS / STBM_NANOSECONDS_IN_ONE_SECOND)

#define STBM_NANOSECONDS_IN_ONE_MICROSECOND             (1000U)
#define STBM_NANOSECONDS_IN_ONE_SECOND                  (1000000000UL)

#define STBM_TIMELEAP_MAX_FUTURE                        (2147483647LL)
#define STBM_TIMELEAP_MAX_PAST                          (-2147483647LL)

#define STBM_GPT_TIMER_STOPPED                          (0U)
#define STBM_GPT_TIMER_START_REQUIRED                   (1U)
#define STBM_GPT_TIMER_RUNNING                          (2U)

#define STBM_TIMER_STOPPED                              (0U)
#define STBM_TIMER_RUNNING                              (1U)
#define STBM_TIMER_CBK_EXPECTED                         (2U)
#define STBM_TIMER_CALL_CBK                             (3U)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define StbM_IsOsLocalTimeUsed(index)                   (StbM_TimeBaseConfig[(index)].LocalTimeMethod == STBM_OS_LOCALTIME) /* PRQA S 3453 */   /* MD_MSR_19.7 */
#define StbM_IsGptLocalTimeUsed(index)                  (StbM_TimeBaseConfig[(index)].LocalTimeMethod == STBM_GPT_LOCALTIME) /* PRQA S 3453 */  /* MD_MSR_19.7 */
#define StbM_IncrementCustomerCounter(index)            ((StbM_CustomerState[(index)].Counter == StbM_CustomerConfig[(index)].TriggeredCustomerPeriod)? (StbM_CustomerState[(index)].Counter = 1U) : (StbM_CustomerState[(index)].Counter += 1U)) /* PRQA S 3453 */     /* MD_MSR_19.7 */
#define StbM_IncrementTimeBaseUpdateCounter(index)      ((StbM_TimeBaseState[(index)].UpdateCounter == 255U)? (StbM_TimeBaseState[(index)].UpdateCounter = 0U) : (StbM_TimeBaseState[(index)].UpdateCounter += 1U)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_GetScheduleTableTimeConvFunc(index)        (StbM_ScheduleTableTimeConvFuncs[StbM_CustomerConfig[(index)].ScheduleTableTimeConvFuncPtrIdx]) /* PRQA S 3453 */       /* MD_MSR_19.7 */
#define StbM_IsOffsetTimeBase(index)                    (StbM_TimeBaseConfig[(index)].OffsetTimeBaseId != STBM_NO_TIMEBASEID) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_GetAbsoluteValue(value)                    (((value) < 0)? ((uint64)-(value)) : ((uint64)(value))) /* PRQA S 3453 */       /* MD_MSR_19.7 */

#define StbM_SetTimeoutBit(status)                      (status) = ((status) | StbM_Timeout_BflMask) /* PRQA S 3453 */  /* MD_MSR_19.7 */
#define StbM_SetTimeLeapFutureBit(status)               (status) = ((status) | StbM_TimeLeapFuture_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_SetTimeLeapPastBit(status)                 (status) = ((status) | StbM_TimeLeapPast_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_SetSyncToGatewayBit(status)                (status) = ((status) | StbM_Sync_To_Gateway_BflMask) /* PRQA S 3453 */  /* MD_MSR_19.7 */
#define StbM_SetGlobalTimeBaseBit(status)               (status) = ((status) | StbM_Global_Time_Base_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define StbM_ClearTimeoutBit(status)                    (status) = ((status) & ~StbM_Timeout_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_ClearTimeLeapFutureBit(status)             (status) = ((status) & ~StbM_TimeLeapFuture_BflMask) /* PRQA S 3453 */        /* MD_MSR_19.7 */
#define StbM_ClearTimeLeapPastBit(status)               (status) = ((status) & ~StbM_TimeLeapPast_BflMask) /* PRQA S 3453 */        /* MD_MSR_19.7 */
#define StbM_ClearSyncToGatewayBit(status)              (status) = ((status) & ~StbM_Sync_To_Gateway_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_ClearGlobalTimeBaseBit(status)             (status) = ((status) & ~StbM_Global_Time_Base_BflMask) /* PRQA S 3453 */        /* MD_MSR_19.7 */

#define StbM_GetTimeoutBit(status)                      ((status) & StbM_Timeout_BflMask) /* PRQA S 3453 */     /* MD_MSR_19.7 */
#define StbM_GetTimeLeapFutureBit(status)               ((status) & StbM_TimeLeapFuture_BflMask) /* PRQA S 3453 */    /* MD_MSR_19.7 */
#define StbM_GetTimeLeapPastBit(status)                 ((status) & StbM_TimeLeapPast_BflMask) /* PRQA S 3453 */    /* MD_MSR_19.7 */
#define StbM_GetSyncToGatewayBit(status)                ((status) & StbM_Sync_To_Gateway_BflMask) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define StbM_GetGlobalTimeBaseBit(status)               ((status) & StbM_Global_Time_Base_BflMask) /* PRQA S 3453 */    /* MD_MSR_19.7 */

#if (STBM_DEV_ERROR_DETECT == STD_ON)
# define StbM_ReportError(API_ID, ERROR_CODE)           (void) Det_ReportError((STBM_MODULE_ID), 0U, ((uint8)(API_ID)), ((uint8)(ERROR_CODE))) /* PRQA  S 3453 */       /* MD_MSR_19.7 */
#endif

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
# define StbM_SetStatusEvGlobalTimeBase(status)           (status) = ((status) | STBM_EV_GLOBAL_TIME_BASE_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvTimeoutOccurred(status)          (status) = ((status) | STBM_EV_TIMEOUT_OCCURRED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvTimeoutRemoved(status)           (status) = ((status) | STBM_EV_TIMEOUT_REMOVED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvTimeleapFuture(status)           (status) = ((status) | STBM_EV_TIMELEAP_FUTURE_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvTimeleapFutureRemoved(status)    (status) = ((status) | STBM_EV_TIMELEAP_FUTURE_REMOVED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvTimeleapPast(status)             (status) = ((status) | STBM_EV_TIMELEAP_PAST_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvTimeleapPastRemoved(status)      (status) = ((status) | STBM_EV_TIMELEAP_PAST_REMOVED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvSyncToSubdomain(status)          (status) = ((status) | STBM_EV_SYNC_TO_SUBDOMAIN_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvSyncToGlobalMaster(status)       (status) = ((status) | STBM_EV_SYNC_TO_GLOBAL_MASTER_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvResync(status)                   (status) = ((status) | STBM_EV_RESYNC_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_SetStatusEvRatecorrection(status)           (status) = ((status) | STBM_EV_RATECORRECTION_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */

# define StbM_ClearStatusEvGlobalTimeBase(status)         (status) = ((status) & ~STBM_EV_GLOBAL_TIME_BASE_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvTimeoutOccurred(status)        (status) = ((status) & ~STBM_EV_TIMEOUT_OCCURRED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvTimeoutRemoved(status)         (status) = ((status) & ~STBM_EV_TIMEOUT_REMOVED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvTimeleapFuture(status)         (status) = ((status) & ~STBM_EV_TIMELEAP_FUTURE_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvTimeleapFutureRemoved(status)  (status) = ((status) & ~STBM_EV_TIMELEAP_FUTURE_REMOVED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvTimeleapPast(status)           (status) = ((status) & ~STBM_EV_TIMELEAP_PAST_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvTimeleapPastRemoved(status)    (status) = ((status) & ~STBM_EV_TIMELEAP_PAST_REMOVED_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvSyncToSubdomain(status)        (status) = ((status) & ~STBM_EV_SYNC_TO_SUBDOMAIN_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvSyncToGlobalMaster(status)     (status) = ((status) & ~STBM_EV_SYNC_TO_GLOBAL_MASTER_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvResync(status)                 (status) = ((status) & ~STBM_EV_RESYNC_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_ClearStatusEvRatecorrection(status)         (status) = ((status) & ~STBM_EV_RATECORRECTION_MASK) /* PRQA S 3453 */  /* MD_MSR_19.7 */

# define StbM_IsStatusEvGlobalTimeBaseSet(status)         ((status) & STBM_EV_GLOBAL_TIME_BASE_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvTimeoutOccurredSet(status)        ((status) & STBM_EV_TIMEOUT_OCCURRED_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvTimeoutRemovedSet(status)         ((status) & STBM_EV_TIMEOUT_REMOVED_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvTimeleapFutureSet(status)         ((status) & STBM_EV_TIMELEAP_FUTURE_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvTimeleapFutureRemovedSet(status)  ((status) & STBM_EV_TIMELEAP_FUTURE_REMOVED_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvTimeleapPastSet(status)           ((status) & STBM_EV_TIMELEAP_PAST_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvTimeleapPastRemovedSet(status)    ((status) & STBM_EV_TIMELEAP_PAST_REMOVED_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvSyncToSubdomainSet(status)        ((status) & STBM_EV_SYNC_TO_SUBDOMAIN_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvSyncToGlobalMasterSet(status)     ((status) & STBM_EV_SYNC_TO_GLOBAL_MASTER_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvResyncSet(status)                 ((status) & STBM_EV_RESYNC_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
# define StbM_IsStatusEvRatecorrectionSet(status)         ((status) & STBM_EV_RATECORRECTION_MASK) != 0 /* PRQA S 3453 */  /* MD_MSR_19.7 */
#endif
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (STBM_LOCAL)       /* COV_STBM_MSR_COMPATIBILITY */
# define STBM_LOCAL static
#endif

#if !defined (STBM_LOCAL_INLINE)        /* COV_STBM_MSR_COMPATIBILITY */
# define STBM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define STBM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

/*! State of time bases */
STBM_LOCAL VAR(StbM_TimeBaseType, STBM_VAR_NOINIT) StbM_TimeBaseState[STBM_TIMEBASE_COUNT];     /* PRQA S 0850 */ /* MD_MSR_19.8 */

#if (STBM_CUSTOMER_COUNT > 0U)
/*! State of customers */
STBM_LOCAL VAR(StbM_CustomerType, STBM_VAR_NOINIT) StbM_CustomerState[STBM_CUSTOMER_COUNT];     /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
# if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
/*! Synchronized Time Base Record Table */
STBM_LOCAL VAR(StbM_SyncRecordTableHeadType, STBM_VAR_NOINIT) StbM_SyncRecordTableHead[STBM_SYNC_SLAVE_TIMEBASE_COUNT];
STBM_LOCAL VAR(StbM_SyncRecordTableBlockType, STBM_VAR_NOINIT) StbM_SyncRecordTableBlock[STBM_SYNC_SLAVE_TIMEBASE_COUNT][STBM_MAX_SYNC_TIME_RECORD_TABLE_BLOCK_COUNT];
# endif

# if (STBM_OFFSET_SLAVE_TIMEBASE_COUNT > 0U)
/*! Offset Time Base Record Table */
STBM_LOCAL VAR(StbM_OffsetRecordTableHeadType, STBM_VAR_NOINIT) StbM_OffsetRecordTableHead[STBM_OFFSET_SLAVE_TIMEBASE_COUNT];
STBM_LOCAL VAR(StbM_OffsetRecordTableBlockType, STBM_VAR_NOINIT) StbM_OffsetRecordTableBlock[STBM_OFFSET_SLAVE_TIMEBASE_COUNT][STBM_MAX_OFFSET_TIME_RECORD_TABLE_BLOCK_COUNT];
# endif
#endif

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
/*! Status event variable of status notification Time bases */
STBM_LOCAL VAR(StbM_TimeBaseNotificationType, STBM_VAR_NOINIT) StbM_StatusNotificationEvent[STBM_STATUS_NOTIFICATION_CUSTOMER_COUNT];
#endif

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/*! Timer variable of time notification customers */
STBM_LOCAL VAR(StbM_TimeNotificationCustomerState, STBM_VAR_NOINIT) StbM_NotificationCustomerStates[STBM_TIME_NOTIFICATION_CUSTOMER_COUNT];

STBM_LOCAL VAR(uint8, STBM_VAR_NOINIT) StbM_GptTimerState;
#endif

#define STBM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

#define STBM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

/*! Initialization state of the module */
#if (STBM_DEV_ERROR_DETECT == STD_ON || STBM_GPT_LOCALTIME_USED == STD_ON)
STBM_LOCAL VAR(uint8, STBM_VAR_ZERO_INIT) StbM_ModuleInitialized = STBM_UNINIT; /* PRQA S 0850 */ /* MD_MSR_19.8 */
#endif

#define STBM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define STBM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  StbM_UpdateTimestamp()
 *********************************************************************************************************************/
/*! \brief      Update a timestamp
 *  \details    This function updates the timestamp of the given time-base.
 *  \param[in]  timeBaseId              The synchronized time-base, whose time is updated.
 *  \return     E_OK - Timestamp was updated
 *  \return     E_NOT_OK - Timestamp was not updated
 *  \pre        -
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateTimestamp(StbM_SynchronizedTimeBaseType timeBaseId);       /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_TimestampPlusTimestamp()
 *********************************************************************************************************************/
/*! \brief      Adds two timestamps
 *  \details    This function calculates the sum of two timestamps.
 *  \param[in]  timestamp1Ptr          Pointer to timestamp where the first summand is stored.
 *  \param[in]  timestamp2Ptr          Pointer to timestamp where the second summand is stored.
 *  \param[out] timesumPtr             Pointer to timestamp where the calculated sum is stored.
 *  \return     E_OK - Timesum is valid
 *  \return     E_NOT_OK - Timesum is invalid
 *  \pre        Exclusive Area StbM_STBM_EXCLUSIVE_AREA_0 is entered
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampPlusTimestamp(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp1Ptr, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp2Ptr, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timesumPtr);      /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_TimestampPlusTimediff()
 *********************************************************************************************************************/
/*! \brief      Adds time difference to timestamp
 *  \details    This function adds a time difference in nanoseconds to a timestamp.
 *  \param[in]  timestampPtr           Pointer to a timestamp.
 *  \param[in]  timediff               Time difference in nanoseconds that should be added to the timestamp.
 *  \param[out] timesumPtr             Pointer to timestamp where the calculated sum is stored.
 *  \return     E_OK - Timesum is valid
 *  \return     E_NOT_OK - Timesum is invalid
 *  \pre        Exclusive Area StbM_STBM_EXCLUSIVE_AREA_0 is entered
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampPlusTimediff(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestampPtr, sint64 timediff, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timesumPtr);     /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_TimestampMinusTimestamp()
 *********************************************************************************************************************/
/*! \brief      Subtracts two timestamps
 *  \details    This function calculates the time difference between timestamp1 and timestamp2
 *              (diff = timestamp1 - timestamp2).
 *  \param[in]  timestamp1Ptr     Pointer to first timestamp.
 *  \param[in]  timestamp2Ptr     Pointer to second timestamp.
 *  \param[out] timediffPtr       Pointer to timestamp with difference between timestamp1 and timestamp2.
 *  \return     E_OK - Time difference is valid
 *  \return     E_NOT_OK - Time difference is invalid
 *  \pre        Exclusive Area StbM_STBM_EXCLUSIVE_AREA_0 is entered
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampMinusTimestamp(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp1Ptr, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp2Ptr, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timediffPtr);    /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_TimestampMinusTimestampInNs
 *********************************************************************************************************************/
/*! \brief      Calculates time difference in nanoseconds
 *  \details    This function calculates the time difference between timestamp1 and timestamp2 in nanoseconds
 *              (diff = timestamp1 - timestamp2).
 *  \param[in]  timestamp1Ptr     Pointer to first timestamp.
 *  \param[in]  timestamp2Ptr     Pointer to second timestamp.
 *  \param[out] timediffPtr       Pointer to timestamp with difference between timestamp1 and timestamp2 in nanoseconds.
 *  \return     E_OK - Time difference is valid
 *  \return     E_NOT_OK - Time difference is invalid
 *  \pre        Exclusive Area StbM_STBM_EXCLUSIVE_AREA_0 is entered
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampMinusTimestampInNs(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp1Ptr,
                                                                                   P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp2Ptr, P2VAR(sint64, AUTOMATIC,
                                                                                                                                                               STBM_APPL_VAR) timediffPtr);

/**********************************************************************************************************************
 *  StbM_CalcTimeAndGetUserData()
 *********************************************************************************************************************/
/*! \brief       Returns the current time in standard format and updates the user data
 *  \details     This function returns the current time of the submitted time-base in standard format.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is of interest.
 *  \param[out]  timeStampPtr            Current time stamp in standard format.
 *  \param[out]  userDataPtr             User data of the time base.
 *  \return      E_OK     - Timestamp is valid
 *  \return      E_NOT_OK - Timestamp is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalcTimeAndGetUserData(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr,
                                                                              P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr);

/**********************************************************************************************************************
 *  StbM_CalculateCurrentTime()
 *********************************************************************************************************************/
/*! \brief       Returns the current time in standard format and gets the corresponding user data.
 *  \details     This function returns the current time of the submitted time-base in standard format.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is of interest.
 *  \param[out]  timeStampPtr            Current time stamp in standard format.
 *  \param[out]  userDataPtr             User data of the time base.
 *  \return      E_OK     - Timestamp is valid
 *  \return      E_NOT_OK - Timestamp is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       DSGN-STBM_OFFSET_CORRECTION_TIME_BASES
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalculateCurrentTime(StbM_SynchronizedTimeBaseType timeBaseId,
                                                                            P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr,
                                                                            P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr);

/**********************************************************************************************************************
 *  StbM_CalculateCurrentOffsetTime()
 *********************************************************************************************************************/
/*! \brief       Returns the current time of a offset time-base in standard format and gets the corresponding user data.
 *  \details     Returns the current time of a offset time-base as the sum of the offset and the current time of the referenced time domain
 *  \param[in]   timeBaseId              The offset time-base, whose time is of interest.
 *  \param[out]  timeStampPtr            Current time stamp in standard format.
 *  \param[out]  userDataPtr             User data of the time base.
 *  \return      E_OK     - Timestamp is valid
 *  \return      E_NOT_OK - Timestamp is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalculateCurrentOffsetTime(StbM_SynchronizedTimeBaseType timeBaseId,
                                                                                  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr,
                                                                                  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr);

/**********************************************************************************************************************
 *  StbM_CheckTimeLeapFuture()
 *********************************************************************************************************************/
/*! \brief       Checks, whether a time leap to the future has occurred and sets the time leap future flag accordingly.
 *  \details     This function allows the recognition of leaps to the future since the last update.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is set.
 *  \param[in]   timeStampPtr            New time stamp.
 *  \return      E_NOT_OK - Any calculation error has occurred
 *               E_OK     - otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CheckTimeLeapFuture(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr);

/**********************************************************************************************************************
 *  StbM_CheckTimeLeapPast()
 *********************************************************************************************************************/
/*! \brief       Checks, whether a time leap to the past has occurred and sets the time leap past flag accordingly.
 *  \details     This function allows the recognition of leaps to the past since the last update.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is set.
 *  \param[in]   timeStampPtr            New time stamp.
 *  \return      E_NOT_OK - Any calculation error has occurred
 *               E_OK     - otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CheckTimeLeapPast(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr);

/**********************************************************************************************************************
 *  StbM_RateCorrectionMeasurement()
 *********************************************************************************************************************/
/*! \brief       Perform simultaneous rate measurement
 *  \details     This function performs simultaneous rate measurements and calculates the current rate correction value.
 *  \param[in]   timeBaseId              The synchronized time-base, for which rate correction measurement is performed
 *  \param[in]   timeStampPtr            New time stamp.
 *  \return      E_NOT_OK - Any calculation error has occurred
 *               E_OK     - otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_RateCorrectionMeasurement(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr);

/**********************************************************************************************************************
 *  StbM_CalculateRateCorrection()
 *********************************************************************************************************************/
/*! \brief       Calculates the rate correction of value of current measurement
 *  \details     -
 *  \param[in]   timeBaseId              The synchronized time-base, for which rate correction measurement is performed
 *  \param[in]   diffGlobalTime          Difference between current time and last measurement start in global time domain.
 *  \return      E_NOT_OK - Any calculation error has occurred
 *               E_OK     - otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalculateRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, sint64 diffGlobalTime);

/**********************************************************************************************************************
 *  StbM_UpdateLocalClock()
 *********************************************************************************************************************/
/*! \brief       Updates the local clock of a synchronized time base.
 *  \details     This function performs simultaneous rate measurements and calculates the current rate correction value.
 *  \param[in]   timeBaseId              The synchronized time-base, whose local time shall be updated
 *  \param[in]   retVal                  Return value of previous step
 *  \return      E_NOT_OK - Any calculation error has occurred
 *               E_OK     - otherwise
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateLocalClock(StbM_SynchronizedTimeBaseType timeBaseId, Std_ReturnType retVal);

#if (STBM_CUSTOMER_COUNT > 0U)
/**********************************************************************************************************************
 *  StbM_TriggerCustomers()
 *********************************************************************************************************************/
/*! \brief       Trigger customers
 *  \details     This function triggers customers.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_TriggerCustomers(void);
#endif

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_ResetStatusEvent()
 *********************************************************************************************************************/
/*! \brief       Initialize notification event customers
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-2927887
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_ResetStatusEvent(void);

/**********************************************************************************************************************
 *  StbM_CheckAndSetGlobalTimeBaseEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if GLOBAL_TIME_BASE in timeBaseStatus has changed from 0 to 1 and sets EV_GLOBAL_TIME_BASE notification
 *             event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \param[in]  customerIdx  - Internal index of notification event variable
 * \param[in]  customerMask - configured notification mask
 * \pre        Must be called before GLOBAL_TIME_BASE Bit in timeBaseStatus is going to be set.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetGlobalTimeBaseEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask);

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimoutNotificationEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if TIMEOUT bit in timeBaseStatus has changed from 0 to 1 and sets EV_TIMEOUT_OCCURED notification
 *             event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \param[in]  customerIdx  - Internal index of notification event variable
 * \param[in]  customerMask - configured notification mask
 * \pre        Must be called before TIMEOUT Bit in timeBaseStatus is going to be set.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimoutNotificationEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask);

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimoutRemovedNotificationEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if TIMEOUT bit in timeBaseStatus has changed from 1 to 0 and sets EV_TIMEOUT_REMOVED
 *             notification event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \param[in]  customerIdx  - Internal index of notification event variable
 * \param[in]  customerMask - configured notification mask
 * \pre        Must be called before TIMEOUT Bit in timeBaseStatus is going to be cleared.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimoutRemovedNotificationEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask);

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapFutureNotificationEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if TIMELEAP_FUTURE bit in timeBaseStatus has changed from 0 to 1 and sets EV_TIMELEAP_FUTURE notification
 *             event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \pre        Must be called before TIMELEAP_FUTURE Bit in timeBaseStatus is going to be set.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapFutureNotificationEv(uint16 timeBaseId);

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapFutureRemovedNotificationEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if TIMELEAP_FUTURE bit in timeBaseStatus has changed from 1 to 0 and sets EV_TIMELEAP_FUTURE_REMOVED
 *             notification event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \pre        Must be called before TIMELEAP_FUTURE Bit in timeBaseStatus is going to be cleared.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapFutureRemovedNotificationEv(uint16 timeBaseId);

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapPastNotificationEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if TIMELEAP_PAST bit in timeBaseStatus has changed from 0 to 1 and sets EV_TIMELEAP_PAST notification
 *             event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \pre        Must be called before TIMELEAP_PAST Bit in timeBaseStatus is going to be set.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapPastNotificationEv(uint16 timeBaseId);

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapFutureRemovedNotificationEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if TIMELEAP_PAST bit in timeBaseStatus has changed from 1 to 0 and sets EV_TIMELEAP_PAST_REMOVED
 *             notification event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \pre        Must be called before TIMELEAP_PAST Bit in timeBaseStatus is going to be cleared.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapPastRemovedNotificationEv(uint16 timeBaseId);

/**********************************************************************************************************************
 *  StbM_CheckAndSetSyncToSubdomainEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if SYNC_TO_GATEWAY bit in timeBaseStatus has changed from 0 to 1 and sets EV_SYNC_TO_SUBDOMAIN
 *             notification event accordingly.
 * \details    -
 * \param[in]  timeBaseId      - Id of Timebase whose notification event should be set
 * \param[in]  customerIdx     - Internal index of notification event variable
 * \param[in]  customerMask    - configured notification mask
 * \param[in]  isUpdateFromBus - TRUE if function is called from StbM_BusSetGlobalTime()
 *                               FALSE otherwise
 * \pre        Must be called before SYNC_TO_GATEWAY Bit in timeBaseStatus is going to be set.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetSyncToSubdomainEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask, boolean isUpdateFromBus);

/**********************************************************************************************************************
 *  StbM_CheckAndSetSyncToGlobalMasterEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if SYNC_TO_GATEWAY bit in timeBaseStatus has changed from 1 to 0 and sets EV_SYNC_TO_GLOBAL_MASTER
 *             notification event accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \param[in]  customerIdx  - Internal index of notification event variable
 * \param[in]  customerMask - configured notification mask
 * \pre        Must be called before SYNC_TO_GATEWAY Bit in timeBaseStatus is going to be cleared.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetSyncToGlobalMasterEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask);

/**********************************************************************************************************************
 *  StbM_CheckAndSetResyncEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if resynchronization has occurred and a new time value has been applied. EV_RESYNC notification
 *             event is set accordingly.
 * \details    -
 * \param[in]  customerIdx  - Internal index of notification event variable
 * \param[in]  customerMask - configured notification mask
 * \pre        -
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetResyncEv(uint16 customerIdx, uint32 customerMask);

/**********************************************************************************************************************
 *  StbM_CheckAndSetRatecorrectionEv()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if a valid rate correction value has been calculated. EV_RATECORRECTION  notification
 *             event is set accordingly.
 * \details    -
 * \param[in]  timeBaseId   - Id of Timebase whose notification event should be set
 * \param[in]  customerIdx  - Internal index of notification event variable
 * \param[in]  customerMask - configured notification mask
 * \pre        Must be called after the new rate correction value has been calculated.
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 *********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetRatecorrectionEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask);

/**********************************************************************************************************************
 *  StbM_CallStatusNotificationCallbackAndClearEvent()
 *********************************************************************************************************************/
/*!
 * \brief      Calls the configured notification callback and clears the notification event flag.
 * \details    -
 * \param[in]  customerIdx - Internal index of notification event variable
 * \pre        -
 * \context    ANY
 * \reentrant  TRUE for different customerIdx
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CallStatusNotificationCallbackAndClearEvent(uint16 customerIdx);

/**********************************************************************************************************************
 *  StbM_SetGlobalTime_SetStatusEvents()
 *********************************************************************************************************************/
/*!
 * \brief      Handle status notification events in context of StbM_SetGlobalTime
 * \details    -
 * \param[in]  timeBaseId - Id of the timeBase
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_SetGlobalTime_SetStatusEvents(StbM_SynchronizedTimeBaseType timeBaseId);
#endif

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_GetNextExpirationPoint()
 *********************************************************************************************************************/
/*!
 * \brief      Calculates the minimum of all expiration points.
 * \details    Next Expiration Point is the minimum of all running timer that might be in the next threshold interval
 * \param[in]  isCallContextTimerCbk - TRUE if function is called from StbM_TimerCallback, FALSE otherwise
 * \return     next expiration point
 * \pre        -
 * \context    ANY
 * \reentrant  TRUE
 ***************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(sint32, STBM_CODE) StbM_GetNextExpirationPoint(boolean isCallContextTimerCbk);

/**********************************************************************************************************************
 *  StbM_GetExpirationPointOfCustomer()
 *********************************************************************************************************************/
/*!
 * \brief      Calculates the expiration point of the given customer and requests the GPT Timer to be started if
 *             expiration point is within the configured threshold.
 * \details    -
 * \param[in]  isCallContextTimerCbk - TRUE if function is called from StbM_TimerCallback, FALSE otherwise
 * \param[in]  customerId - Id of customer
 * \param[in]  timeStamp -  current time of TimeBase
 * \return     next expiration point
 * \pre        -
 * \context    ANY
 * \reentrant  TRUE
 ***************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(sint32, STBM_CODE) StbM_GetExpirationPointOfCustomer(boolean isCallContextTimerCbk,  uint16 customerId,  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp);

/**********************************************************************************************************************
 *  StbM_StartGptTimer()
 *********************************************************************************************************************/
/*!
 * \brief      Starts the GPT Timer for given expirationPoint
 * \details    -
 * \param[in]  expirationPoint
 * \pre        -
 * \context    ANY
 * \reentrant  FALSE
 *********************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_StartGptTimer(sint32 expirationPoint);

/**********************************************************************************************************************
 *  StbM_UpdateExpireTime()
 *********************************************************************************************************************/
/*!
 * \brief      Updates all running time notification customer timers of given timeBaseId
 * \details    This function is called whenever a new time update is received
 * \param[in]  timeBaseId - Id of Time base
 * \param[in]  timeStampPtr - New Global Time Value
 * \pre        -
 * \context    ANY
 * \reentrant  TRUE for different TimeBase Ids
 *********************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_UpdateExpireTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr);

/**********************************************************************************************************************
 *  StbM_StopTimeNotificationCustomers()
 *********************************************************************************************************************/
/*!
 * \brief      Resets the state of all time notification customers
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_StopTimeNotificationCustomers(void);

/**********************************************************************************************************************
 *  StbM_IsExpireTimeValid()
 *********************************************************************************************************************/
/*!
* \brief       Check whether expire time is valid.
* \details     -
* \param[in]   expireTime       Time value relative to current Time Base value of the Notification Customer,
*                               when the Timer shall expire
* \return      TRUE       -     valid
*              FALSE      -     invalid
* \pre         -
* \context     TASK
* \reentrant   FALSE
* \synchronous TRUE
********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(boolean, STBM_CODE) StbM_IsExpireTimeValid(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) expireTime);
#endif

#if(STBM_OFFSET_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_CalcElapsedTimeOfOffsetTimeBase()
 *********************************************************************************************************************/
/*!
 * \brief      Calculates the current elapsed time for running timers of time notification customers that belong to
 *             an Offset Time Base, which refers the given synchronized Time Base timeBaseId
 * \details    -
 * \param[in]  timeBaseId - Id of synchronized Time base
 * \pre        -
 * \context    ANY
 * \reentrant  TRUE for different TimeBase Ids
 *****************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CalcElapsedTimeOfOffsetTimeBase(StbM_SynchronizedTimeBaseType timeBaseId);

/**********************************************************************************************************************
 *  StbM_UpdateExpireTimeOfOffsetTimeBase()
 *********************************************************************************************************************/
/*!
 * \brief      This function updates the expire Time of Time Notification Customers which belong to an offset Time Base,
 *             which refers the given synchronized Time Base timeBaseId
 * \details    -
 * \param[in]  timeBaseId - Id of synchronized Time base
 * \pre        The elapsed time has been cached prior calling this function (StbM_CalcElapsedTimeOfOffsetTimeBase
 *             is called before)
 * \context    ANY
 * \reentrant  TRUE for different timeBaseIds
 ****************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_UpdateExpireTimeOfOffsetTimeBase(StbM_SynchronizedTimeBaseType timeBaseId);
#endif

/**********************************************************************************************************************
 *  StbM_SetNewGlobalTime()
 *********************************************************************************************************************/
/*!
 * \brief        Sets new global time
 * \details      This function sets the global time to the value given in timeStamp.
 * \param[in]    timeBaseId              The time base, whose global time is updated.
 * \param[in]    timeStamp               New time stamp.
 * \param[in]    userData                New user data.
 * \return       E_OK - New global time was set
 * \return       E_NOT_OK - New global time was not set
 * \pre          -
 ****************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_SetNewGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  StbM_UpdateTimestamp()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateTimestamp(StbM_SynchronizedTimeBaseType timeBaseId)        /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint32 elapsedCounterValue = 0U;
  uint32 elapsedTime, nanosecondsSum;
  uint8 elapsedSeconds = 0U;
#if (STBM_OS_LOCALTIME_USED == STD_ON)
  TickType osTicks, elapsedOsCounterValue;
#endif
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
  uint32 OldGptChannelValue;
#endif
#if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
  Eth_TimeStampQualType ethIfTimeQual;
  Eth_TimeStampType ethIfTimeStamp;
  StbM_TimeStampType newLocalTime;
  StbM_TimeStampType oldLocalTime;
  StbM_TimeStampType diffLocalTime;
  StbM_TimeStampType localTime;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if ((STBM_GPT_LOCALTIME_USED == STD_ON) || (STBM_OS_LOCALTIME_USED == STD_ON))
  /* #10 If OS or GPT local time is used */
  if(StbM_IsOsLocalTimeUsed(timeBaseId) || StbM_IsGptLocalTimeUsed(timeBaseId))
  {
#if (STBM_OS_LOCALTIME_USED == STD_ON)
    /* #20 If OS local time is used, set elapsed counter value to time passed in OS timer ticks */
    if(StbM_IsOsLocalTimeUsed(timeBaseId))
    {
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      osTicks = StbM_TimeBaseState[timeBaseId].LocalTimeTickValue;
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
      (void) GetElapsedValue((CounterType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID, &osTicks, &elapsedOsCounterValue);
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) osTicks;
      elapsedCounterValue = (uint32) elapsedOsCounterValue;
    }
#endif
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
    /* #30 Else (if GPT local time is used), set elapsed counter value to time passed in GPT ticks */
    if(StbM_IsGptLocalTimeUsed(timeBaseId))
    {
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      OldGptChannelValue = StbM_TimeBaseState[timeBaseId].LocalTimeTickValue;
      StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) Gpt_GetTimeElapsed((Gpt_ChannelType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID);
      /* check for overflow of tick value */
      if(OldGptChannelValue < StbM_TimeBaseState[timeBaseId].LocalTimeTickValue)
      {
        elapsedCounterValue = StbM_TimeBaseState[timeBaseId].LocalTimeTickValue - OldGptChannelValue;
      }
      else
      {
        elapsedCounterValue = (StbM_TimeBaseConfig[timeBaseId].LocalTimeMaxValue - OldGptChannelValue) + (1U + StbM_TimeBaseState[timeBaseId].LocalTimeTickValue);
      }
    }
#endif
    /* #40 Calculate and add elapsed time to timestamp value */
    elapsedTime = elapsedCounterValue * StbM_TimeBaseConfig[timeBaseId].LocalTimeNsPerTick;
    nanosecondsSum = StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds + elapsedTime;
    /* #50 Check for overflow of nanoseconds and add up the full seconds */
    if(nanosecondsSum < elapsedTime)
    {
      /* split seconds from elapsedTime */
      elapsedSeconds = (uint8) (elapsedTime / STBM_NANOSECONDS_IN_ONE_SECOND);
      nanosecondsSum = (elapsedTime % STBM_NANOSECONDS_IN_ONE_SECOND) + StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds;
    }
    /* check if nanoseconds are still bigger than one second */
    if(nanosecondsSum > STBM_TIMESTAMP_MAX_NANOSECONDS)
    {
      elapsedSeconds += (uint8) (nanosecondsSum / STBM_NANOSECONDS_IN_ONE_SECOND);
      nanosecondsSum = nanosecondsSum % STBM_NANOSECONDS_IN_ONE_SECOND;
    }
    StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds = nanosecondsSum;
    /* #60 If one or more full seconds have elapsed, add them to the timestamp */
    if(elapsedSeconds > 0U)
    {
      if(StbM_TimeBaseState[timeBaseId].TimeStamp.seconds > (STBM_TIMESTAMP_MAX_SECONDS - elapsedSeconds))
      {
        if(StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi < STBM_TIMESTAMP_MAX_SECONDSHI)
        {
          StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi++;
          retVal = E_OK;
        }
      }
      else
      {
        retVal = E_OK;
      }
      StbM_TimeBaseState[timeBaseId].TimeStamp.seconds += elapsedSeconds;
    }
    else
    {
      retVal = E_OK;
    }
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
#endif
#if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
  /* #70 If Eth TSyn local time is used */
  if(StbM_TimeBaseConfig[timeBaseId].UsesEthTSynLocalTime == TRUE)
  {
    /* #80 Get current time from EthIf */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = EthIf_GetCurrentTime(StbM_TimeBaseConfig[timeBaseId].EthIfCtrlIdx, &ethIfTimeQual, &ethIfTimeStamp);
    /* #90 If getting the current time was successful */
    if((retVal == E_OK) && (ethIfTimeQual == ETH_VALID))
    {
      newLocalTime.nanoseconds = ethIfTimeStamp.nanoseconds;
      newLocalTime.seconds = ethIfTimeStamp.seconds;
      newLocalTime.secondsHi = ethIfTimeStamp.secondsHi;

      oldLocalTime.nanoseconds = StbM_TimeBaseState[timeBaseId].LocalTime.nanoseconds;
      oldLocalTime.seconds = StbM_TimeBaseState[timeBaseId].LocalTime.seconds;
      oldLocalTime.secondsHi = StbM_TimeBaseState[timeBaseId].LocalTime.secondsHi;

      /* #100 Calculate time passed in Eth TSyn clock and add to timestamp */
      retVal = StbM_TimestampMinusTimestamp(&newLocalTime, &oldLocalTime, &diffLocalTime);
      if(retVal == E_OK)
      {
        StbM_TimeBaseState[timeBaseId].LocalTime = ethIfTimeStamp;
        retVal = StbM_TimestampPlusTimestamp(&StbM_TimeBaseState[timeBaseId].TimeStamp, &diffLocalTime, &localTime);
        if(retVal == E_OK)
        {
          StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds = localTime.nanoseconds;
          StbM_TimeBaseState[timeBaseId].TimeStamp.seconds = localTime.seconds;
          StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi = localTime.secondsHi;
        }
      }
    }
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
#endif

  return retVal;
}       /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampPlusTimestamp(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp1Ptr, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp2Ptr, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timesumPtr)       /* PRQA S 0850 */ /* MD_MSR_19.8 */
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
  if(timesumPtr->nanoseconds > STBM_TIMESTAMP_MAX_NANOSECONDS)
  {
    timesumPtr->seconds++;
    timesumPtr->nanoseconds -= STBM_NANOSECONDS_IN_ONE_SECOND;
  }

  /* #30 check for overflow of seconds */
  if(timesumPtr->seconds < (timestamp1Ptr->seconds | timestamp2Ptr->seconds))
  {
    timesumPtr->secondsHi++;
  }

  /* #40 check for overflow of seconds high */
  /* calculated sum is invalid, if overflow of secondsHi occurred */
  if((timesumPtr->secondsHi >= timestamp1Ptr->secondsHi) && (timesumPtr->secondsHi >= timestamp2Ptr->secondsHi))
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  StbM_TimestampPlusTimediff()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampPlusTimediff(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestampPtr, sint64 timediff, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timesumPtr)      /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint64 timediffSeconds;
  StbM_TimeStampType timestampDiff = { 0, 0, 0, 0 };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If time difference is larger than zero */
  if(timediff > 0)
  {
    /* #20 Calculate and add timestamp for the time difference */
    timediffSeconds = ((uint64)timediff / (uint64)STBM_NANOSECONDS_IN_ONE_SECOND);
    timestampDiff.secondsHi = (uint16) (timediffSeconds >> 32U);
    timestampDiff.seconds = (uint32) timediffSeconds;
    timestampDiff.nanoseconds = ((uint32) timediff % STBM_NANOSECONDS_IN_ONE_SECOND);
    retVal = StbM_TimestampPlusTimestamp(timestampPtr, &timestampDiff, timesumPtr);
  }
  /* #30 If time difference is equal or lower than zero */
  else
  {
    /* #40 Calculate and subtract timestamp for the time difference */
    timediffSeconds = ((uint64)-timediff / (uint64)STBM_NANOSECONDS_IN_ONE_SECOND);
    timestampDiff.secondsHi = (uint16) (timediffSeconds >> 32U);
    timestampDiff.seconds = (uint32) timediffSeconds;
    timestampDiff.nanoseconds = ((uint32) - timediff % STBM_NANOSECONDS_IN_ONE_SECOND);
    retVal = StbM_TimestampMinusTimestamp(timestampPtr, &timestampDiff, timesumPtr);
  }
  return retVal;
}

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
 */
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampMinusTimestamp(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp1Ptr, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp2Ptr, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timediffPtr)     /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If seconds high 1 is equal or larger than seconds high 2 */
  if(timestamp1Ptr->secondsHi >= timestamp2Ptr->secondsHi)
  {
    /* #20 Calculate time difference */
    timediffPtr->secondsHi = timestamp1Ptr->secondsHi - timestamp2Ptr->secondsHi;
    timediffPtr->seconds = timestamp1Ptr->seconds - timestamp2Ptr->seconds;

    /* #30 Check for overflow of seconds */
    if(timestamp1Ptr->seconds < timestamp2Ptr->seconds)
    {
      timediffPtr->secondsHi--;
    }

    /* #40 Check for overflow of nanoseconds */
    if(timestamp1Ptr->nanoseconds < timestamp2Ptr->nanoseconds)
    {
      timediffPtr->seconds--;

      /* #50 Check for overflow of seconds */
      if (timediffPtr->seconds == STBM_TIMESTAMP_MAX_SECONDS)
      {
        timediffPtr->secondsHi--;
      }

      timediffPtr->nanoseconds = (STBM_TIMESTAMP_MAX_NANOSECONDS - timestamp2Ptr->nanoseconds) + (1U + timestamp1Ptr->nanoseconds);
    }
    else
    {
      timediffPtr->nanoseconds = timestamp1Ptr->nanoseconds - timestamp2Ptr->nanoseconds;
    }
    retVal = E_OK;
  }
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_TimestampMinusTimestampInNs()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_TimestampMinusTimestampInNs(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp1Ptr, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timestamp2Ptr, P2VAR(sint64, AUTOMATIC, STBM_APPL_VAR) timediffPtr)     /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If secondsHi 1 is equal or larger than secondsHi 2, subtract secondsHi 2 from secondsHi 1 */
  if(timestamp1Ptr->secondsHi >= timestamp2Ptr->secondsHi)
  {
    *timediffPtr = (sint64) (((uint64)timestamp1Ptr->secondsHi<<32U) - ((uint64)timestamp2Ptr->secondsHi<<32U));
  }
  /* #20 Else, subtract secondsHi 1 from secondsHi 2 */
  else
  {
    *timediffPtr = -(sint64) (((uint64)timestamp2Ptr->secondsHi<<32U) - ((uint64)timestamp1Ptr->secondsHi<<32U));
  }

  /* #30 If seconds 1 is equal or larger than seconds 2, subtract seconds 2 from seconds 1 */
  if(timestamp1Ptr->seconds >= timestamp2Ptr->seconds)
  {
    *timediffPtr += ((sint64)timestamp1Ptr->seconds - (sint64)timestamp2Ptr->seconds);
  }
  /* #40 Else, subtract seconds 1 from seconds 2 */
  else
  {
    *timediffPtr += -(sint64) (timestamp2Ptr->seconds - timestamp1Ptr->seconds);
  }

  /* #50 If time difference in seconds is suitable for nanoseconds difference value */
  if((*timediffPtr > -STBM_TIMEDIFF_MAX_SECONDS) && (*timediffPtr < STBM_TIMEDIFF_MAX_SECONDS))
  {
    /* #60 Convert time difference to nanoseconds */
    *timediffPtr *= (sint64) STBM_NANOSECONDS_IN_ONE_SECOND;
    /* #70 If nanoseconds 1 is greater or equal nanoseconds 2, add nanoseconds difference to time difference */
    if(timestamp1Ptr->nanoseconds >= timestamp2Ptr->nanoseconds)
    {
      *timediffPtr += ((sint64)timestamp1Ptr->nanoseconds - (sint64)timestamp2Ptr->nanoseconds);
    }
    /* #80 Else, subtract nanoseconds difference from time difference */
    else
    {
      *timediffPtr += -(sint64) (timestamp2Ptr->nanoseconds - timestamp1Ptr->nanoseconds);
    }
    retVal = E_OK;
  }

  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_CalcTimeAndGetUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalcTimeAndGetUserData(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr)       /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  Std_ReturnType retVal;

  if(StbM_IsOffsetTimeBase(timeBaseId))
  {
    retVal = StbM_CalculateCurrentOffsetTime(timeBaseId, timeStampPtr, userDataPtr);
  }
  else
  {
    retVal = StbM_CalculateCurrentTime(timeBaseId, timeStampPtr, userDataPtr);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  StbM_CalculateCurrentTime()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalculateCurrentTime(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  sint64 diffLocalTime;
  sint64 rateCorrectedLocalTime;
  sint64 offsetCorrectedLocalTime;
  sint64 diffOffset;
  uint64 diffOffsetAbs;
  uint64 diffLocalTimeAbs;
  sint64 offsetCorrectionRate;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update local Time-Base */
  Std_ReturnType retVal = StbM_UpdateTimestamp(timeBaseId);

  SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

  /* #20 If Time-Base was correctly updated */
  if(retVal == E_OK)
  {
    /*- #30 If rate correction is configured */
    if(StbM_TimeBaseState[timeBaseId].RateCorrection != 0)
    {
      /* correct local time with last calculated rate */
      retVal =
        StbM_TimestampMinusTimestampInNs(&StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime,
                                         &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime, &diffOffset);
      if(retVal == E_OK)
      {
        retVal =
          StbM_TimestampMinusTimestampInNs(&StbM_TimeBaseState[timeBaseId].TimeStamp,
                                           &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime, &diffLocalTime);
        if(retVal == E_OK)
        {
          diffOffsetAbs = StbM_GetAbsoluteValue(diffOffset);
          diffLocalTimeAbs = StbM_GetAbsoluteValue(diffLocalTime);

          rateCorrectedLocalTime = (sint64)(diffLocalTimeAbs * StbM_GetAbsoluteValue(StbM_TimeBaseState[timeBaseId].RateCorrection));
          if (((diffLocalTime < 0) && (StbM_TimeBaseState[timeBaseId].RateCorrection > 0)) || ((diffLocalTime > 0) && (StbM_TimeBaseState[timeBaseId].RateCorrection < 0)))
          {
            rateCorrectedLocalTime = -rateCorrectedLocalTime;
          }

          /* #40 If absolute difference between start global time and start local time exceeds jump threshold or adaptive rate correction interval is exceeded */
          if((diffOffsetAbs >= StbM_TimeBaseConfig[timeBaseId].OffsetCorrectionJumpThreshold) || (diffLocalTimeAbs > StbM_TimeBaseConfig[timeBaseId].OffsetCorrectionAdaptionInterval))
          {
            /*- #50 Perform rate correction */
            retVal =
              StbM_TimestampPlusTimediff(&StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime, rateCorrectedLocalTime, timeStampPtr);
          }
          /* #55 Else */
          else
          {
            /* - #60 Perform rate and adaptive offset correction */
            if(diffOffset < 0)
            {
              offsetCorrectionRate = -(sint64) ((uint64) - diffOffset / StbM_TimeBaseConfig[timeBaseId].OffsetCorrectionAdaptionInterval);
            }
            else
            {
              offsetCorrectionRate = (sint64) ((uint64) diffOffset / StbM_TimeBaseConfig[timeBaseId].OffsetCorrectionAdaptionInterval);
            }
            offsetCorrectionRate += 1;
            offsetCorrectedLocalTime = (sint64)(StbM_GetAbsoluteValue(rateCorrectedLocalTime) * StbM_GetAbsoluteValue(offsetCorrectionRate));
            if (((rateCorrectedLocalTime < 0) && (offsetCorrectionRate > 0)) || ((rateCorrectedLocalTime > 0) && (offsetCorrectionRate < 0)))
            {
              offsetCorrectedLocalTime = -offsetCorrectedLocalTime;
            }
            retVal =
              StbM_TimestampPlusTimediff(&StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime, offsetCorrectedLocalTime, timeStampPtr);
          }
        }
      }
    }
    /* - #70 Else set current time stamp pointer to updated local time stamp */
    else
    {
      *timeStampPtr = StbM_TimeBaseState[timeBaseId].TimeStamp;
    }
    /* - #80 Get user data */
    *userDataPtr = StbM_TimeBaseState[timeBaseId].UserData;
  }
  SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_CheckTimeLeapFuture()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CheckTimeLeapFuture(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  StbM_TimeStampType diffTimeStamp;
  sint64 diffTime;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If time leap future threshold is configured */
  if((StbM_TimeBaseConfig[timeBaseId].TimeLeapFutureThresholdSec > 0U) || (StbM_TimeBaseConfig[timeBaseId].TimeLeapFutureThresholdNsec > 0U))
  {
    /* #20 Calculate absolute time difference */
    if((timeStampPtr->secondsHi > StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi)
      || ((timeStampPtr->secondsHi == StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi) && (timeStampPtr->seconds > StbM_TimeBaseState[timeBaseId].TimeStamp.seconds))
      || ((timeStampPtr->secondsHi == StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi) && (timeStampPtr->seconds == StbM_TimeBaseState[timeBaseId].TimeStamp.seconds) && (timeStampPtr->nanoseconds > StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds)))
    {
      retVal = StbM_TimestampMinusTimestamp(timeStampPtr, &StbM_TimeBaseState[timeBaseId].TimeStamp, &diffTimeStamp);

      /* #30 If no error occurred */
      if(retVal == E_OK)
      {
        /* #40 If absolute time difference has exceeded threshold */
        if((diffTimeStamp.secondsHi > 0) || (diffTimeStamp.seconds > StbM_TimeBaseConfig[timeBaseId].TimeLeapFutureThresholdSec) || ((diffTimeStamp.seconds == StbM_TimeBaseConfig[timeBaseId].TimeLeapFutureThresholdSec) && (diffTimeStamp.nanoseconds > StbM_TimeBaseConfig[timeBaseId].TimeLeapFutureThresholdNsec)))
        {
          /* #50 Set time leap future bit and counter */
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
          StbM_CheckAndSetTimeleapFutureNotificationEv(timeBaseId);
#endif
          StbM_SetTimeLeapFutureBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
          StbM_TimeBaseState[timeBaseId].TimeLeapFutureThresholdCount = StbM_TimeBaseConfig[timeBaseId].ClearTimeleapCount;
          if(StbM_TimestampMinusTimestampInNs(timeStampPtr, &StbM_TimeBaseState[timeBaseId].TimeStamp, &diffTime)  == E_OK)
          {
            StbM_TimeBaseState[timeBaseId].LastTimeLeapValue = diffTime;
          }
        }
        else
        {
          /* #60 Handle time leap future counter */
          if(StbM_TimeBaseState[timeBaseId].TimeLeapFutureThresholdCount > 0)
          {
            StbM_TimeBaseState[timeBaseId].TimeLeapFutureThresholdCount--;
            /* #70 Clear time leap future bit, if time leap future counter has expired */
            if(StbM_TimeBaseState[timeBaseId].TimeLeapFutureThresholdCount == 0)
            {
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
              StbM_CheckAndSetTimeleapFutureRemovedNotificationEv(timeBaseId);
#endif
              StbM_ClearTimeLeapFutureBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
              StbM_TimeBaseState[timeBaseId].LastTimeLeapValue = 0;
            }
          }
        }
      }
    }
  }

  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_CheckTimeLeapPast()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CheckTimeLeapPast(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  StbM_TimeStampType diffTimeStamp;
  sint64 diffTime;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 If time leap past threshold is configured */
  if((StbM_TimeBaseConfig[timeBaseId].TimeLeapPastThresholdSec > 0U) || (StbM_TimeBaseConfig[timeBaseId].TimeLeapPastThresholdNsec > 0U))
  {
    /* #20 Calculate absolute time difference */
    if((timeStampPtr->secondsHi < StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi)
      || ((timeStampPtr->secondsHi == StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi) && (timeStampPtr->seconds < StbM_TimeBaseState[timeBaseId].TimeStamp.seconds))
      || ((timeStampPtr->secondsHi == StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi) && (timeStampPtr->seconds == StbM_TimeBaseState[timeBaseId].TimeStamp.seconds) && (timeStampPtr->nanoseconds < StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds)))
    {
      retVal = StbM_TimestampMinusTimestamp(&StbM_TimeBaseState[timeBaseId].TimeStamp, timeStampPtr, &diffTimeStamp);

      /* #30 If no error occurred */
      if(retVal == E_OK)
      {
        /* #40 If absolute time difference has exceeded threshold */
        if((diffTimeStamp.secondsHi > 0) || (diffTimeStamp.seconds > StbM_TimeBaseConfig[timeBaseId].TimeLeapPastThresholdSec) || ((diffTimeStamp.seconds == StbM_TimeBaseConfig[timeBaseId].TimeLeapPastThresholdSec) && (diffTimeStamp.nanoseconds > StbM_TimeBaseConfig[timeBaseId].TimeLeapPastThresholdNsec)))
        {
          /* #50 Set time leap past bit and counter */
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
          StbM_CheckAndSetTimeleapPastNotificationEv(timeBaseId);
#endif
          StbM_SetTimeLeapPastBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
          StbM_TimeBaseState[timeBaseId].TimeLeapPastThresholdCount = StbM_TimeBaseConfig[timeBaseId].ClearTimeleapCount;
          if(StbM_TimestampMinusTimestampInNs(timeStampPtr, &StbM_TimeBaseState[timeBaseId].TimeStamp, &diffTime) == E_OK)
          {
            StbM_TimeBaseState[timeBaseId].LastTimeLeapValue = diffTime;
          }
        }
        else
        {
          /* #60 Handle time leap past counter */
          if(StbM_TimeBaseState[timeBaseId].TimeLeapPastThresholdCount > 0)
          {
            StbM_TimeBaseState[timeBaseId].TimeLeapPastThresholdCount--;
            /* #70 Clear time leap past bit, if time leap future counter has expired */
            if(StbM_TimeBaseState[timeBaseId].TimeLeapPastThresholdCount == 0)
            {
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
              StbM_CheckAndSetTimeleapPastRemovedNotificationEv(timeBaseId);
#endif
              StbM_ClearTimeLeapPastBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
              StbM_TimeBaseState[timeBaseId].LastTimeLeapValue = 0;
            }
          }
        }
      }
    }
  }

  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_RateCorrectionMeasurement()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_RateCorrectionMeasurement(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint16 lastRateMeasurement;
  sint64 diffGlobalTime;
  uint64 diffGlobalTimeAbs;
  StbM_UserDataType synchronizedUserData;
  StbM_TimeStampType synchronizedTimeStamp;
  boolean startNewRateMeasurement = FALSE;
  uint32 rateMeasurementOffset;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If Rate Correction Measurement is configured */
  if(StbM_TimeBaseConfig[timeBaseId].RateCorrectionMeasurementDuration > 0U)
  {
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

    /* #20 Progress rate measurement index */
    lastRateMeasurement = StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement;
    if(StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement == (StbM_TimeBaseConfig[timeBaseId].RateCorrectionsPerMeasurementDuration - 1U))
    {
      StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement = 0U;
    }
    else
    {
      StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement++;
    }

    /* #30 If the current measurement was started before */
    if((StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime.nanoseconds > 0U) ||
       (StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime.seconds > 0U) ||
       (StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime.secondsHi > 0U))
    {
      retVal = StbM_TimestampMinusTimestampInNs(timeStampPtr, &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime, &diffGlobalTime);
      diffGlobalTimeAbs = StbM_GetAbsoluteValue(diffGlobalTime);

      /* #40 Check if measurement duration has passed */
      if((retVal == E_OK) && (diffGlobalTimeAbs >= StbM_TimeBaseConfig[timeBaseId].RateCorrectionMeasurementDuration))
      {
        /* #50 If state of the time base has not changed during measurement */
        if(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus ==
           StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime.timeBaseStatus)
        {
          SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
          /* #60 Calculate rate correction value */
          retVal = StbM_CalculateRateCorrection(timeBaseId, diffGlobalTime);
          SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
        }

        /* #70 Request start of new rate measurement */
        startNewRateMeasurement = TRUE;
      }
    }
    /* #80 else if previous rate measurement was started */
    else if(StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement > 0U)
    {
      /* #90 Request start of new rate measurement if offset has passed */
      rateMeasurementOffset = StbM_TimeBaseConfig[timeBaseId].RateCorrectionMeasurementDuration / StbM_TimeBaseConfig[timeBaseId].RateCorrectionsPerMeasurementDuration;
      retVal =
        StbM_TimestampMinusTimestampInNs(timeStampPtr, &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement - 1U].StartGlobalTime, &diffGlobalTime);
      diffGlobalTimeAbs = StbM_GetAbsoluteValue(diffGlobalTime);
      if((retVal == E_OK) && (diffGlobalTimeAbs >= rateMeasurementOffset))
      {
        startNewRateMeasurement = TRUE;
      }
    }
    /* #100 Otherwise request new rate measurement */
    else
    {
      /* start first measurement */
      startNewRateMeasurement = TRUE;
    }

    /* #110 If new rate measurement was requested and no time leap was recognized */
    if(startNewRateMeasurement && (StbM_GetTimeLeapFutureBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U) && (StbM_GetTimeLeapPastBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U))
    {
      /* #120 Take sample of global and local time for new rate measurement */
      StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime = *timeStampPtr;
      StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime.timeBaseStatus = StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus;
      if(StbM_IsOffsetTimeBase(timeBaseId))
      {
        SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
        retVal = StbM_CalculateCurrentTime(StbM_TimeBaseConfig[timeBaseId].OffsetTimeBaseId, &synchronizedTimeStamp, &synchronizedUserData);
        SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

        if(retVal == E_OK)
        {
          StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime = synchronizedTimeStamp;
        }
      }
      else
      {
        StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime = StbM_TimeBaseState[timeBaseId].TimeStamp;
      }
    }
    /* #130 Otherwise reset rate measurement index */
    else
    {
      StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement = lastRateMeasurement;
    }

    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }

  return retVal;
}       /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  StbM_CalculateRateCorrection()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalculateRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, sint64 diffGlobalTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  sint64 diffLocalTime;
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
  uint16 customerIdx;
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If Time Base is a Offset Time */
  if(StbM_IsOffsetTimeBase(timeBaseId))
  {
    StbM_TimeStampType synchronizedTimeStamp;
    StbM_UserDataType synchronizedUserData;

    /* #20 Calculate current local time of referred synchronized time base */
    retVal = StbM_CalculateCurrentTime(StbM_TimeBaseConfig[timeBaseId].OffsetTimeBaseId, &synchronizedTimeStamp, &synchronizedUserData);

    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

    /* #30 If no error has occurred */
    if(retVal == E_OK)
    {

      /* #40 Set rate correction to 1 + (diffGlobalTime)/(current local time - local measurement start) */
      retVal =
        StbM_TimestampMinusTimestampInNs(&synchronizedTimeStamp, &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime, &diffLocalTime);
      if(retVal == E_OK)
      {
        /* Check EV_RATECORRECTION.*/
        StbM_TimeBaseState[timeBaseId].RateCorrection = (sint32)((diffLocalTime + diffGlobalTime) / diffLocalTime);

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
        customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

        /* #41 If notification customer is configured */
        if(customerIdx != STBM_UNUSED_INDEX)
        {
          /* Check EV_RATECORRECTION */
          /* #42 Check and set Ratecorrection notification event */
          StbM_CheckAndSetRatecorrectionEv(timeBaseId, customerIdx, StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask);
        }
#endif
      }
    }

    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
  /* #50 Otherwise set rate correction to (diffGlobalTime / elapsed time of local time base since last measurement update ) */
  else
  {
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

    retVal =
      StbM_TimestampMinusTimestampInNs(&StbM_TimeBaseState[timeBaseId].TimeStamp,
                                       &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime, &diffLocalTime);
    if(retVal == E_OK)
    {
      StbM_TimeBaseState[timeBaseId].RateCorrection = (sint32)(diffGlobalTime / diffLocalTime);

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
      customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

      /* #51 If notification customer is configured */
      if(customerIdx != STBM_UNUSED_INDEX)
      {
        /* Check EV_RATECORRECTION */
        /* #52 Check and set Ratecorrection notification event */
        StbM_CheckAndSetRatecorrectionEv(timeBaseId, customerIdx, StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask);
      }
#endif
    }

    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }

  return retVal;
}

/**********************************************************************************************************************
 *  StbM_UpdateLocalClock()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateLocalClock(StbM_SynchronizedTimeBaseType timeBaseId, Std_ReturnType retVal)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnValue = retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* get counter value of last update of local time */

#if (STBM_OS_LOCALTIME_USED == STD_ON)
  /* #10 If Os Local Time is used */
  if(StbM_IsOsLocalTimeUsed(timeBaseId))
  {
    TickType osTicks;

    (void) GetCounterValue((CounterType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID, &osTicks);
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    /* #20 Set Local Clock to counter value of OS. */
    StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) osTicks;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
#endif
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
  /* #30 else if Gpt Time is used */
  if(StbM_IsGptLocalTimeUsed(timeBaseId))
  {
    /* #40 Set Local Clock to elapsed time */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) Gpt_GetTimeElapsed((Gpt_ChannelType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID);
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
#endif
#if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
  /* #50 else if Ethernet Local Time is used */
  if(StbM_TimeBaseConfig[timeBaseId].UsesEthTSynLocalTime == TRUE)
  {
    Eth_TimeStampQualType ethIfTimeQual;
    Eth_TimeStampType ethIfTimeStamp;
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

    /* #60 Set Local Clock to Current Time of EthIf if no error has occurred */
    returnValue = EthIf_GetCurrentTime(StbM_TimeBaseConfig[timeBaseId].EthIfCtrlIdx, &ethIfTimeQual, &ethIfTimeStamp);
    if((returnValue == E_OK) && (ethIfTimeQual == ETH_VALID))
    {
      StbM_TimeBaseState[timeBaseId].LocalTime = ethIfTimeStamp;
    }
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
#endif

  return returnValue;
}
/**********************************************************************************************************************
 *  StbM_CalculateCurrentOffsetTime()
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
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_CalculateCurrentOffsetTime(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr)   /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  StbM_TimeStampType synchronizedTimeStamp;
  StbM_UserDataType synchronizedUserData;
  sint64 diffLocalTime;
  sint64 correctedLocalTime;
  StbM_TimeStampType startTimeStamp;

  /* ----- Implementation ----------------------------------------------- */
  /* for offset time bases, calculate the time stamp by adding the offset to the current time of the referenced time domain */

  /* #10 Get current Timestamp of reference Time-Base */
  retVal = StbM_CalculateCurrentTime(StbM_TimeBaseConfig[timeBaseId].OffsetTimeBaseId, &synchronizedTimeStamp, &synchronizedUserData);

  SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
  /* #20 If timestamp is valid */
  if(retVal == E_OK)
  {
    /* #30 If offset has been set */
    if(StbM_GetGlobalTimeBaseBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) != 0U)
    {
      /* #40 If rate correction is configured and rate correction value has been measured */
      if(StbM_TimeBaseState[timeBaseId].RateCorrection != 0)
      {
        /* #50 Correct local time with last calculated rate */
        retVal =
          StbM_TimestampMinusTimestampInNs(&synchronizedTimeStamp, &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime,
                                           &diffLocalTime);
        if(retVal == E_OK)
        {
          retVal =
            StbM_TimestampPlusTimestamp(&StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartGlobalTime,
                                        &StbM_TimeBaseState[timeBaseId].RateMeasurement[StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement].StartLocalTime, &startTimeStamp);
          if(retVal == E_OK)
          {
            correctedLocalTime = diffLocalTime * StbM_TimeBaseState[timeBaseId].RateCorrection;
            retVal = StbM_TimestampPlusTimediff(&startTimeStamp, correctedLocalTime, timeStampPtr);
          }
        }
      }
      /* #60 Otherwise calculate current offset time as sum synchronized timestamp + offset */
      else
      {
        retVal = StbM_TimestampPlusTimestamp(&synchronizedTimeStamp, &StbM_TimeBaseState[timeBaseId].TimeStamp, timeStampPtr);
      }

      /* #70 If no calculation error has occurred, set status of offset time base status equal to status of synchronized time base */
      if(retVal == E_OK)
      {
        timeStampPtr->timeBaseStatus = synchronizedTimeStamp.timeBaseStatus;
         /* It should be not required to check notification customers as the internal timeBaseStatus is not changed. Only the getCurrentTime Status is affected. */
      }
    }
    /* #80 Otherwise set current time of offset time base equal to time of referred synchronized time base and clear all status bits */
    else
    {
      /* no offset value available */
      *timeStampPtr = synchronizedTimeStamp;
      timeStampPtr->timeBaseStatus = 0x00U;
    }
  }

  /* #90 Get corresponding user data */
  *userDataPtr = StbM_TimeBaseState[timeBaseId].UserData;

  SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (STBM_CUSTOMER_COUNT > 0U)
/**********************************************************************************************************************
 *  StbM_TriggerCustomers()
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
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_TriggerCustomers(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 timeBaseId;
  uint16 customerId;
  ScheduleTableStatusType scheduleStatus;
  uint32 elapsedNanoseconds;
  TickType elapsedOsTicks, osTicks;

  /* ----- Implementation ----------------------------------------------- */
  /* trigger customers */
  for(customerId = 0U; customerId < STBM_CUSTOMER_COUNT; customerId++)
  {
    timeBaseId = StbM_CustomerConfig[customerId].TimeBaseID;

    StbM_IncrementCustomerCounter(customerId);

    /* #10 If Triggered Customer Period counter is zero and Time Base has been synchronized to global time and timeout has not occured and no timeleap has been recognized */
    if((StbM_CustomerState[customerId].Counter == StbM_CustomerConfig[customerId].TriggeredCustomerPeriod)
       && (StbM_GetGlobalTimeBaseBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) != 0U) && (StbM_GetTimeoutBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U)
       && (StbM_GetTimeLeapFutureBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U) && (StbM_GetTimeLeapPastBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U))
    {
      /* #20 If Table status is RUNNING || WAITING || RUNNING_AND_SYNCHRONOUS */
      if((GetScheduleTableStatus(StbM_CustomerConfig[customerId].ScheduleTableID, &scheduleStatus) == E_OK)
         && ((scheduleStatus == SCHEDULETABLE_WAITING) || (scheduleStatus == SCHEDULETABLE_RUNNING) || (scheduleStatus == SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS)))
      {
        /* #30 If tables have not been synchronized before, synchronize Tables with zero Ticks */
        if((StbM_CustomerState[customerId].LastSyncTimeStamp.nanoseconds == 0U) && (StbM_CustomerState[customerId].LastSyncTimeStamp.seconds == 0U)
           && (StbM_CustomerState[customerId].LastSyncTimeStamp.secondsHi == 0U))
        {
          (void) SyncScheduleTable(StbM_CustomerConfig[customerId].ScheduleTableID, 0U);
        }
        /* #40 Otherwise synchronize Tables with current local time (last synchronized counter value + elapsed time) */
        else
        {
          if((StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi > StbM_CustomerState[customerId].LastSyncTimeStamp.secondsHi)
             || (StbM_TimeBaseState[timeBaseId].TimeStamp.seconds > StbM_CustomerState[customerId].LastSyncTimeStamp.seconds))
          {
            elapsedNanoseconds = (STBM_TIMESTAMP_MAX_NANOSECONDS - StbM_CustomerState[customerId].LastSyncTimeStamp.nanoseconds) + (1U + StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds);
          }
          else
          {
            elapsedNanoseconds = StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds - StbM_CustomerState[customerId].LastSyncTimeStamp.nanoseconds;
          }
          elapsedOsTicks = StbM_GetScheduleTableTimeConvFunc(customerId) (elapsedNanoseconds / STBM_NANOSECONDS_IN_ONE_MICROSECOND);
          elapsedOsTicks %= StbM_CustomerConfig[customerId].ScheduleTableDuration;
          osTicks = StbM_CustomerState[customerId].LastSyncValue + elapsedOsTicks;

          /* overflow check */
          if((osTicks < elapsedOsTicks) || (osTicks >= StbM_CustomerConfig[customerId].ScheduleTableDuration))
          {
            osTicks = elapsedOsTicks - (StbM_CustomerConfig[customerId].ScheduleTableDuration - StbM_CustomerState[customerId].LastSyncValue);
          }

          (void) SyncScheduleTable(StbM_CustomerConfig[customerId].ScheduleTableID, osTicks);

          StbM_CustomerState[customerId].LastSyncValue = osTicks;
        }

        /* #50 Sample current time of Time base */
        StbM_CustomerState[customerId].LastSyncTimeStamp = StbM_TimeBaseState[timeBaseId].TimeStamp;
      }
    }
  }
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_ResetStatusEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_ResetStatusEvent(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 notificationCustomerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize status event variables with zero */
  for(notificationCustomerIdx = 0U; notificationCustomerIdx < STBM_STATUS_NOTIFICATION_CUSTOMER_COUNT; notificationCustomerIdx++)
  {
    /*! Status event variable of status notification Time bases */
    StbM_StatusNotificationEvent[notificationCustomerIdx] = 0U;
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetGlobalTimeBaseEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetGlobalTimeBaseEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask)
{
  if(StbM_IsStatusEvGlobalTimeBaseSet(customerMask))
  {
    if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_Global_Time_Base_BflMask) == 0)
    {
      StbM_SetStatusEvGlobalTimeBase(StbM_StatusNotificationEvent[customerIdx]);
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimoutNotificationEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimoutNotificationEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask)
{
  /* #10 If Timeout Ev is subscribed by notification customer */
  if(StbM_IsStatusEvTimeoutOccurredSet(customerMask))
  {
    /* #20 Set timeout Notification Ev if timeout was not detected before */
    if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_Timeout_BflMask) == 0)
    {
      StbM_SetStatusEvTimeoutOccurred(StbM_StatusNotificationEvent[customerIdx]);
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimoutRemovedNotificationEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimoutRemovedNotificationEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask)
{
  /* #10 If Timeout Removed Ev is subscribed by notification customer */
  if(StbM_IsStatusEvTimeoutRemovedSet(customerMask))
  {
    /* #20 Set timeout removed Notification Ev if timeout was detected before */
    if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_Timeout_BflMask) != 0)
    {
      StbM_SetStatusEvTimeoutRemoved(StbM_StatusNotificationEvent[customerIdx]);
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapFutureNotificationEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapFutureNotificationEv(uint16 timeBaseId)
{
  uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

  /* #10 If notification customer is configured */
  if(customerIdx != STBM_UNUSED_INDEX)
  {
    uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;
    /* #20 If Timeout Ev is subscribed by notification customer */
    if(StbM_IsStatusEvTimeleapFutureSet(customerMask))
    {
      /* #30 Set Timeleap_Future Notification Ev if time leap was not detected before */
      if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_TimeLeapFuture_BflMask) == 0)
      {
        StbM_SetStatusEvTimeleapFuture(StbM_StatusNotificationEvent[customerIdx]);
      }
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapFutureRemovedNotificationEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapFutureRemovedNotificationEv(uint16 timeBaseId)
{
  uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

  /* #10 If notification customer is configured */
  if(customerIdx != STBM_UNUSED_INDEX)
  {
    uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;
    /* #20 Timeleap Future Removed Ev is subscribed by notification customer */
    if(StbM_IsStatusEvTimeleapFutureRemovedSet(customerMask))
    {
      /* #30 Set Timeleap Future Removed Notification Ev if time leap was detected before */
      if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_TimeLeapFuture_BflMask) != 0)
      {
        StbM_SetStatusEvTimeleapFutureRemoved(StbM_StatusNotificationEvent[customerIdx]);
      }
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapPastNotificationEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapPastNotificationEv(uint16 timeBaseId)
{
  uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

  /* #10 If notification customer is configured */
  if(customerIdx != STBM_UNUSED_INDEX)
  {
    uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;
    /* #20 If Timeout Ev is subscribed by notification customer */
    if(StbM_IsStatusEvTimeleapPastSet(customerMask))
    {
      /* #30 Set Timeleap_Past Notification Ev if time leap was not detected before */
      if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_TimeLeapPast_BflMask) == 0)
      {
        StbM_SetStatusEvTimeleapPast(StbM_StatusNotificationEvent[customerIdx]);
      }
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetTimeleapPastRemovedNotificationEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetTimeleapPastRemovedNotificationEv(uint16 timeBaseId)
{
  uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

  /* #10 If notification customer is configured */
  if(customerIdx != STBM_UNUSED_INDEX)
  {
    uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;
    /* #20 Timeleap Past Removed Ev is subscribed by notification customer */
    if(StbM_IsStatusEvTimeleapPastRemovedSet(customerMask))
    {
      /* #30 Set Timeleap Past Removed Notification Ev if time leap was detected before */
      if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_TimeLeapPast_BflMask) != 0)
      {
        StbM_SetStatusEvTimeleapPastRemoved(StbM_StatusNotificationEvent[customerIdx]);
      }
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetSyncToSubdomainEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetSyncToSubdomainEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask, boolean isUpdateFromBus)
{
  if(StbM_IsStatusEvSyncToSubdomainSet(customerMask))
  {
    if((StbM_TimeBaseConfig[timeBaseId].IsTimeGateway == TRUE) || isUpdateFromBus)
    {
      if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_Sync_To_Gateway_BflMask) == 0)
      {
        StbM_SetStatusEvSyncToSubdomain(StbM_StatusNotificationEvent[customerIdx]);
      }
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetSyncToGlobalMasterEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetSyncToGlobalMasterEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask)
{
  if(StbM_IsStatusEvSyncToGlobalMasterSet(customerMask))
  {
    if((StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus & StbM_Sync_To_Gateway_BflMask) != 0)
    {
      StbM_SetStatusEvSyncToGlobalMaster(StbM_StatusNotificationEvent[customerIdx]);
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetResyncEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetResyncEv(uint16 customerIdx, uint32 customerMask)
{
  if(StbM_IsStatusEvResyncSet(customerMask))
  {
    StbM_SetStatusEvResync(StbM_StatusNotificationEvent[customerIdx]);
  }
}

/**********************************************************************************************************************
 *  StbM_CheckAndSetRatecorrectionEv()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CheckAndSetRatecorrectionEv(uint16 timeBaseId, uint16 customerIdx, uint32 customerMask)
{
  if(StbM_IsStatusEvRatecorrectionSet(customerMask))
  {
    if(StbM_TimeBaseState[timeBaseId].RateCorrection != 0)
    {
      StbM_SetStatusEvRatecorrection(StbM_StatusNotificationEvent[customerIdx]);
    }
  }
}

/**********************************************************************************************************************
 *  StbM_CallStatusNotificationCallbackAndClearEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CallStatusNotificationCallbackAndClearEvent(uint16 customerIdx)
{
  StbM_TimeBaseNotificationType event;
  /* #10 If notification event was registered */
  if(StbM_StatusNotificationEvent[customerIdx] != 0)
  {
    /* #20 Clear notification event and call status notification callback */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    event =  StbM_StatusNotificationEvent[customerIdx];
    StbM_StatusNotificationEvent[customerIdx] = 0;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

    (void) StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationCallback(event);
  }
}

/**********************************************************************************************************************
 *  StbM_SetGlobalTime_SetStatusEvents()
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
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_SetGlobalTime_SetStatusEvents(StbM_SynchronizedTimeBaseType timeBaseId)
{
  uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

  /* #51 If notification customer is configured */
  if(customerIdx != STBM_UNUSED_INDEX)
  {
    /* Check EV_GLOBAL_TIME_BASE, EV_TIMEOUT_REMOVED, EV_SYNC_TO_GLOBAL_MASTER, EV_RESYNC */
    uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;

    /* #52 Check and set sync to Global TimeBase notification event */
    StbM_CheckAndSetGlobalTimeBaseEv(timeBaseId, customerIdx, customerMask);

    /* #53 Check and set Timeout Removed event */
    StbM_CheckAndSetTimoutRemovedNotificationEv(timeBaseId, customerIdx, customerMask);

    /* #54 Check and set Sync To Global Master event */
    StbM_CheckAndSetSyncToGlobalMasterEv(timeBaseId, customerIdx, customerMask);

    /* #55 Check and set ReSync event */
    StbM_CheckAndSetResyncEv(customerIdx, customerMask);
  }
}
#endif

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_GetNextExpirationPoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 ******************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(sint32, STBM_CODE) StbM_GetNextExpirationPoint(boolean isCallContextTimerCbk)
{
  StbM_SynchronizedTimeBaseType timeBaseId = 0;
  sint32 nextExpirationPoint = STBM_INVALID_EXPIRATION_POINT;

  for(; timeBaseId < STBM_TIMEBASE_COUNT; timeBaseId++)
  {
    StbM_TimeStampType timeStamp;
    StbM_UserDataType userData;

    if(StbM_TimeBaseConfig[timeBaseId].TimeNotificationCustomerStartIndex != STBM_UNUSED_INDEX)
    {
      uint16 customerId = StbM_TimeBaseConfig[timeBaseId].TimeNotificationCustomerStartIndex;

      /* #10 Get current time of timebase */
      Std_ReturnType timeStampValid = StbM_CalcTimeAndGetUserData(timeBaseId, &timeStamp, &userData);
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      for(;customerId < StbM_TimeBaseConfig[timeBaseId].TimeNotificationCustomerEndIndex; customerId++)
      {
        /* #20 If time is valid */
        if(timeStampValid == E_OK)
        {
          /* #30 Get remaining time of time notification customer */
          sint32 expirationPoint = StbM_GetExpirationPointOfCustomer(isCallContextTimerCbk, customerId, &timeStamp);

          /* #50 If current remaining time is the minimum of all remaining times, set as next expiration point */
          if(expirationPoint < nextExpirationPoint)
          {
            nextExpirationPoint = expirationPoint;
          }
        }
        /* #60 Otherwise stop timer of belonging customer */
        else
        {
          StbM_NotificationCustomerStates[customerId].timerState = STBM_TIMER_STOPPED;
        }
      }
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    }
  }

  return nextExpirationPoint;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_GetExpirationPointOfCustomer()
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
STBM_LOCAL_INLINE FUNC(sint32, STBM_CODE) StbM_GetExpirationPointOfCustomer(boolean isCallContextTimerCbk,  uint16 customerId,  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp)
{
  StbM_TimeDiffType expirationPoint = STBM_INVALID_EXPIRATION_POINT;
  /* #10 If timer of customer is running */
  if(StbM_NotificationCustomerStates[customerId].timerState == STBM_TIMER_RUNNING)
  {
    Std_ReturnType retVal;
    sint64 expirationPoint_temp;

    retVal = StbM_TimestampMinusTimestampInNs(&(StbM_NotificationCustomerStates[customerId].expireTime), timeStamp, &expirationPoint_temp);

    if((retVal == E_OK) && (expirationPoint_temp > (-2 * (sint64)STBM_NANOSECONDS_IN_ONE_SECOND)) && (expirationPoint_temp < (2 * (sint64)STBM_NANOSECONDS_IN_ONE_SECOND)))
    /* If timeUntilTimeout > 2s --> Can't be in GPT Threshold --> skip */
    {
      expirationPoint = (StbM_TimeDiffType)expirationPoint_temp;
      StbM_NotificationCustomerStates[customerId].gptExpirationPoint = expirationPoint;

      /* #20 If time has not already exceeded expire time */
      if(expirationPoint > 0)
      {
        /* #30 Check if remaining time till timeout is within configured threshold */
        if(expirationPoint <= StbM_TimeNotificationCustomerConfig[customerId].TimerStartThreshold)
        {
          if((StbM_GptTimerState == STBM_GPT_TIMER_STOPPED) || isCallContextTimerCbk)
          {
            /* #40 Set GPT timer state to REQUIRED */
            StbM_GptTimerState = STBM_GPT_TIMER_START_REQUIRED;
          }
        }
      }else{
        /* #60 Set State of notification customer to CALL_CBK */
        StbM_NotificationCustomerStates[customerId].timerState = STBM_TIMER_CALL_CBK;
        expirationPoint = STBM_INVALID_EXPIRATION_POINT;
      }
    }
  }
  return expirationPoint;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
/**********************************************************************************************************************
 *  StbM_StartGptTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_StartGptTimer(sint32 expirationPoint)
{
  /* #10 If GPT Timer is required */
  if(StbM_GptTimerState == STBM_GPT_TIMER_START_REQUIRED)
  {
    uint16 timeCustomerIdx = 0;

    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    /* #20 Set status of time notification customer to "Callback Expected" if timer will elapse with next expiration point */
    for(; timeCustomerIdx < STBM_TIME_NOTIFICATION_CUSTOMER_COUNT; timeCustomerIdx++)
    {
      if(StbM_NotificationCustomerStates[timeCustomerIdx].timerState == STBM_TIMER_RUNNING)
      {
        if(StbM_NotificationCustomerStates[timeCustomerIdx].gptExpirationPoint == expirationPoint){
          StbM_NotificationCustomerStates[timeCustomerIdx].timerState = STBM_TIMER_CBK_EXPECTED;
        }
      }
    }

    /* #30 Set GPT Timer State to "RUNNING" */
    StbM_GptTimerState = STBM_GPT_TIMER_RUNNING;

    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

    /* #40 Start GPT Timer */
    Gpt_StartTimer(STBM_TIME_NOTIFICATION_GPT_CHANNEL, ((uint32) expirationPoint) / STBM_NS_TICKS_GPT_NOTIFICATION_TIMER);
  }
}

/**********************************************************************************************************************
 *  StbM_UpdateExpireTime()
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
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_UpdateExpireTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr)
{
  uint16 timeCustomerId = StbM_TimeBaseConfig[timeBaseId].TimeNotificationCustomerStartIndex;
  Std_ReturnType currentTimeIsValid;
  /* #10 If Time Notification Customers are configured */
  if(timeCustomerId != STBM_UNUSED_INDEX)
  {
    StbM_TimeStampType currentTime;
    StbM_UserDataType userDataTemp;

    /* #20 Calculate current time of time base */
    currentTimeIsValid = StbM_CalcTimeAndGetUserData(timeBaseId, &currentTime, &userDataTemp);

    /* #25 If TimeValue is valid */
    if(currentTimeIsValid == E_OK)
    {
      for(; timeCustomerId < StbM_TimeBaseConfig[timeBaseId].TimeNotificationCustomerEndIndex; timeCustomerId++)
      {
        /* #30 If timer is not stopped, update expire time to expireTime = (expireTime - currentTime) + new TimeValue */
        if(StbM_NotificationCustomerStates[timeCustomerId].timerState != STBM_TIMER_STOPPED)
        {
          StbM_TimeStampType remainingTime;
          Std_ReturnType retValPerCustomer;
          StbM_TimeStampType newTimeStamp;

          SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
          retValPerCustomer = StbM_TimestampMinusTimestamp(&(StbM_NotificationCustomerStates[timeCustomerId].expireTime), &currentTime, &remainingTime);

          if(StbM_IsOffsetTimeBase(timeBaseId))
          {
            StbM_TimeStampType timeWithoutOffset;
            /* remove old offset */
            retValPerCustomer |= StbM_TimestampMinusTimestamp(&currentTime, &(StbM_TimeBaseState[timeBaseId].TimeStamp), &timeWithoutOffset);

            /* add new offset */
            retValPerCustomer |= StbM_TimestampPlusTimestamp(&timeWithoutOffset, timeStampPtr , &newTimeStamp);

          }
          else
          {
            newTimeStamp = *timeStampPtr;
          }

          retValPerCustomer |= StbM_TimestampPlusTimestamp(&remainingTime, &newTimeStamp, &(StbM_NotificationCustomerStates[timeCustomerId].expireTime));

          if(retValPerCustomer == E_NOT_OK)
          {
            /* #40 If any error occurred, stop specific timer */
            StbM_NotificationCustomerStates[timeCustomerId].timerState = STBM_TIMER_STOPPED;
          }
          SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
        }
      }
    }
    /* #50 Else stop all corresponding timer */
    else
    {
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      for(; timeCustomerId < StbM_TimeBaseConfig[timeBaseId].TimeNotificationCustomerEndIndex; timeCustomerId++)
      {
        StbM_NotificationCustomerStates[timeCustomerId].timerState = STBM_TIMER_STOPPED;
      }
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    }
  }
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_StopTimeNotificationCustomers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_StopTimeNotificationCustomers(void)
{
  uint16 timeNotificationCustomer = 0;

  /* #10 Set GPT Timer State to STOPPED */
  StbM_GptTimerState = STBM_GPT_TIMER_STOPPED;

  /* #20 Stop all time notification customers */
  for(; timeNotificationCustomer < STBM_TIME_NOTIFICATION_CUSTOMER_COUNT; timeNotificationCustomer++)
  {
    StbM_NotificationCustomerStates[timeNotificationCustomer].timerState = STBM_TIMER_STOPPED;
  }
}

/**********************************************************************************************************************
 *  StbM_IsExpireTimeValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *******************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(boolean, STBM_CODE) StbM_IsExpireTimeValid(P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) expireTime)
{
  boolean retVal = FALSE;

  /* #10 Check if expireTime is larger than one main function cycle and larger than the greatest GPT threshold if GPT Timer is not in idle state */
  if(
      (expireTime->secondsHi > STBM_MAINFUNCTION_CYCLETIME_S_HI) ||
      ((expireTime->secondsHi == STBM_MAINFUNCTION_CYCLETIME_S_HI) && (expireTime->seconds > STBM_MAINFUNCTION_CYCLETIME_S)) ||
      ((expireTime->secondsHi == STBM_MAINFUNCTION_CYCLETIME_S_HI) && (expireTime->seconds == STBM_MAINFUNCTION_CYCLETIME_S) && (expireTime->nanoseconds > STBM_MAINFUNCTION_CYCLETIME_NS))
    )
  {
    if((StbM_GptTimerState == STBM_GPT_TIMER_STOPPED)
        ||  (
            (expireTime->secondsHi > STBM_MAX_GPT_THRESHOLD_HIS) ||
            ((expireTime->secondsHi == STBM_MAX_GPT_THRESHOLD_HIS) && (expireTime->seconds > STBM_MAX_GPT_THRESHOLD_S)) ||
            ((expireTime->secondsHi == STBM_MAX_GPT_THRESHOLD_HIS) && (expireTime->seconds == STBM_MAX_GPT_THRESHOLD_S) && (expireTime->nanoseconds > STBM_MAX_GPT_THRESHOLD_NS))))
    {
      retVal = TRUE;
    }
  }

  return retVal;
}
#endif

#if(STBM_OFFSET_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_CalcElapsedTimeOfOffsetTimeBase()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_CalcElapsedTimeOfOffsetTimeBase(StbM_SynchronizedTimeBaseType timeBaseId)
{
  uint16 offsetTimeCustomerStartInd = StbM_OffsetTimeNotificationCustomerIndirection[timeBaseId].startIdxOfInd;
  /* #10 If timeBase is referred by an offset time base having time notification customers */
  if(offsetTimeCustomerStartInd != STBM_UNUSED_INDEX)
  {
    for(; offsetTimeCustomerStartInd < StbM_OffsetTimeNotificationCustomerIndirection[timeBaseId].endIdxOfInd; offsetTimeCustomerStartInd++)
    {
      uint16 customerId = StbM_InternalIdOfOffsetNotificationCustomer[offsetTimeCustomerStartInd];
      /* #20 Cache current elapsed time since start of running timers */
      if(StbM_NotificationCustomerStates[customerId].timerState != STBM_TIMER_STOPPED)
      {
        uint16 offsetTimeBaseId = StbM_TimeNotificationCustomerConfig[customerId].ParentTimeBaseId;
        StbM_TimeStampType currentTime;
        StbM_UserDataType userDataTemp;

        Std_ReturnType currentTimeIsValid =  StbM_CalculateCurrentOffsetTime(offsetTimeBaseId, &currentTime, &userDataTemp);

        SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
        if(currentTimeIsValid == E_OK)
        {
          currentTimeIsValid = StbM_TimestampMinusTimestamp(&(StbM_NotificationCustomerStates[customerId].expireTime), &currentTime, &(StbM_NotificationCustomerStates[customerId].elapsedOffsetTime));
        }

        /* 30 If any error occurred, stop related timer */
        if(currentTimeIsValid == E_NOT_OK)
        {
          StbM_NotificationCustomerStates[customerId].timerState = STBM_TIMER_STOPPED;
        }
        SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
      }
    }
  }
}

/**********************************************************************************************************************
 *  StbM_UpdateExpireTimeOfOffsetTimeBase()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 ******************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(void, STBM_CODE) StbM_UpdateExpireTimeOfOffsetTimeBase(StbM_SynchronizedTimeBaseType timeBaseId)
{
  uint16 offsetTimeCustomerStartInd = StbM_OffsetTimeNotificationCustomerIndirection[timeBaseId].startIdxOfInd;
  /* #10 If timeBase is referred by an offset time base having time notification customers */
  if(offsetTimeCustomerStartInd != STBM_UNUSED_INDEX)
  {
    for(; offsetTimeCustomerStartInd < StbM_OffsetTimeNotificationCustomerIndirection[timeBaseId].endIdxOfInd; offsetTimeCustomerStartInd++)
    {
      uint16 customerId = StbM_InternalIdOfOffsetNotificationCustomer[offsetTimeCustomerStartInd];
      /* #20 Re-adjust expire time of offset timeBases customer */
      if(StbM_NotificationCustomerStates[customerId].timerState != STBM_TIMER_STOPPED)
      {
        uint16 offsetTimeBaseId = StbM_TimeNotificationCustomerConfig[customerId].ParentTimeBaseId;
        StbM_TimeStampType currentTime;
        StbM_UserDataType userDataTemp;

        Std_ReturnType currentTimeIsValid =  StbM_CalculateCurrentOffsetTime(offsetTimeBaseId, &currentTime, &userDataTemp);

        SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
        if(currentTimeIsValid == E_OK)
        {
          currentTimeIsValid = StbM_TimestampPlusTimestamp(&(StbM_NotificationCustomerStates[customerId].elapsedOffsetTime), &currentTime, &(StbM_NotificationCustomerStates[customerId].expireTime));
        }

        /* #30 If any error occurred, stop related timer */
        if(currentTimeIsValid == E_NOT_OK)
        {
          StbM_NotificationCustomerStates[customerId].timerState = STBM_TIMER_STOPPED;
        }
        SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
      }
    }
  }
}
#endif

/**********************************************************************************************************************
 *  StbM_SetNewGlobalTime()
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
 ********************************************************************************************************************/
STBM_LOCAL_INLINE FUNC(Std_ReturnType, STBM_CODE) StbM_SetNewGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_OS_LOCALTIME_USED == STD_ON)
  TickType osTicks;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  /* #10 Update Expire Time of Time Notification Customers */
  StbM_UpdateExpireTime(timeBaseId, timeStamp);
#endif
#if(STBM_OFFSET_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  /* #15 Cache elapsed time of Offset Time Notification customers referring the given synchronized Time base */
  StbM_CalcElapsedTimeOfOffsetTimeBase(timeBaseId);
#endif

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
  /* #20 Set status notification events */
  StbM_SetGlobalTime_SetStatusEvents(timeBaseId);
#endif

  SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
  /* #30 Get current time from timeStamp */
  StbM_TimeBaseState[timeBaseId].TimeStamp = *timeStamp;
  /* #35 Get current user data from userData */
  StbM_TimeBaseState[timeBaseId].UserData = *userData;

  /* #40 Set the globalTimeBase bit within TimeBaseStatus, clear all other bits */
  StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus = StbM_Global_Time_Base_BflMask;

  SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

  /* #45 Get counter value of last update of local time */
#if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
  if(StbM_TimeBaseConfig[timeBaseId].UsesEthTSynLocalTime == TRUE)
  {
    Eth_TimeStampQualType ethIfTimeQual;
    Eth_TimeStampType ethIfTimeStamp;
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = EthIf_GetCurrentTime(StbM_TimeBaseConfig[timeBaseId].EthIfCtrlIdx, &ethIfTimeQual, &ethIfTimeStamp);
    if((retVal == E_OK) && (ethIfTimeQual == ETH_VALID))
    {
      StbM_TimeBaseState[timeBaseId].LocalTime = ethIfTimeStamp;
    }
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }
  else
#endif
  {
#if (STBM_OS_LOCALTIME_USED == STD_ON)
    if(StbM_IsOsLocalTimeUsed(timeBaseId))
    {
      (void) GetCounterValue((CounterType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID, &osTicks);
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) osTicks;
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    }
#endif
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
    if(StbM_IsGptLocalTimeUsed(timeBaseId))
    {
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) Gpt_GetTimeElapsed((Gpt_ChannelType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID);
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    }
#endif
    retVal = E_OK;
  }

#if(STBM_OFFSET_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  /* #50 Update expire time Offset Time Notification customers referring the given synchronized Time base */
  StbM_UpdateExpireTimeOfOffsetTimeBase(timeBaseId);
#endif

  return retVal;
} /* PRQA S 6050*/ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  StbM_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, STBM_CODE) StbM_InitMemory(void)     /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Implementation ----------------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON || STBM_GPT_LOCALTIME_USED == STD_ON)
  StbM_ModuleInitialized = STBM_UNINIT;
#endif
}

/***********************************************************************************************************************
 *  StbM_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, STBM_CODE) StbM_Init(void)   /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
  uint16 gptChannelId;
  boolean gptChannelActive[STBM_GPT_CHANNEL_COUNT];
#endif
  uint16 timeBaseId;
#if (STBM_OS_LOCALTIME_USED == STD_ON)
  TickType osTicks;
#endif
#if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
  Eth_TimeStampQualType ethIfTimeQual;
  Eth_TimeStampType ethIfTimeStamp;
#endif
  uint16 rateMeasurementIdx;
#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
  uint16 timeBaseIdx;
#endif
#if (STBM_CUSTOMER_COUNT > 0U)
  uint16 customerId;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
  for(gptChannelId = 0U; gptChannelId < STBM_GPT_CHANNEL_COUNT; gptChannelId++)
  {
    gptChannelActive[gptChannelId] = FALSE;
  }
#endif
  for(timeBaseId = 0U; timeBaseId < STBM_TIMEBASE_COUNT; timeBaseId++)
  {
    StbM_TimeBaseState[timeBaseId].SyncLossTimeoutCount = 0U;
    StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus = 0x00U;

    /* TODO: only initialize timebase with zero, if value is not stored non-volatile or restore from NvM was not successful */
    StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds = 0U;
    StbM_TimeBaseState[timeBaseId].TimeStamp.seconds = 0U;
    StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi = 0U;

#if (STBM_OS_LOCALTIME_USED == STD_ON)
    /* get counter value of last update of local time */
    if(StbM_IsOsLocalTimeUsed(timeBaseId))
    {
      (void) GetCounterValue((CounterType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID, &osTicks);
      StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = (uint32) osTicks;
    }
#endif
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
    if(StbM_IsGptLocalTimeUsed(timeBaseId) && (StbM_ModuleInitialized == (uint8) STBM_UNINIT))
    {
      if(gptChannelActive[StbM_TimeBaseConfig[timeBaseId].GptChannelIdx] == FALSE)
      {
        Gpt_StartTimer((Gpt_ChannelType) StbM_TimeBaseConfig[timeBaseId].LocalTimeID, (Gpt_ValueType) StbM_TimeBaseConfig[timeBaseId].LocalTimeMaxValue);
        gptChannelActive[StbM_TimeBaseConfig[timeBaseId].GptChannelIdx] = TRUE;
      }
      StbM_TimeBaseState[timeBaseId].LocalTimeTickValue = 0U;
    }
#endif
#if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
    if(StbM_TimeBaseConfig[timeBaseId].UsesEthTSynLocalTime == TRUE)
    {
      if((EthIf_GetCurrentTime(StbM_TimeBaseConfig[timeBaseId].EthIfCtrlIdx, &ethIfTimeQual, &ethIfTimeStamp) == E_OK) && (ethIfTimeQual == ETH_VALID))
      {
        StbM_TimeBaseState[timeBaseId].LocalTime = ethIfTimeStamp;
      }
    }
#endif

    StbM_TimeBaseState[timeBaseId].UserData.userDataLength = 0U;
    StbM_TimeBaseState[timeBaseId].UserData.userByte0 = 0U;
    StbM_TimeBaseState[timeBaseId].UserData.userByte1 = 0U;
    StbM_TimeBaseState[timeBaseId].UserData.userByte2 = 0U;
    StbM_TimeBaseState[timeBaseId].CurrentRateMeasurement = StbM_TimeBaseConfig[timeBaseId].RateCorrectionsPerMeasurementDuration - 1U;
    StbM_TimeBaseState[timeBaseId].RateCorrection = 0U;
    for(rateMeasurementIdx = 0; rateMeasurementIdx < StbM_TimeBaseConfig[timeBaseId].RateCorrectionsPerMeasurementDuration; rateMeasurementIdx++)
    {
      StbM_TimeBaseState[timeBaseId].RateMeasurement[rateMeasurementIdx].StartGlobalTime.nanoseconds = 0U;
      StbM_TimeBaseState[timeBaseId].RateMeasurement[rateMeasurementIdx].StartGlobalTime.seconds = 0U;
      StbM_TimeBaseState[timeBaseId].RateMeasurement[rateMeasurementIdx].StartGlobalTime.secondsHi = 0U;
    }

    StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock = 0U;
    StbM_TimeBaseState[timeBaseId].UpdateCounter = 0U;
    StbM_TimeBaseState[timeBaseId].TimeLeapFutureThresholdCount = 0U;
    StbM_TimeBaseState[timeBaseId].TimeLeapPastThresholdCount = 0U;

#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
    timeBaseIdx = StbM_TimeBaseConfig[timeBaseId].SlaveTimeBaseIdx;

    if (timeBaseIdx != STBM_NO_TIMEBASEID)
    {
# if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
      if(!StbM_IsOffsetTimeBase(timeBaseId))
      {
        uint16 syncRecordTableBlockIdx;

        StbM_SyncRecordTableHead[timeBaseIdx].SynchronizedTimeDomain = StbM_TimeBaseConfig[timeBaseId].TimeDomainID;
        StbM_SyncRecordTableHead[timeBaseIdx].HWfrequency = StbM_TimeBaseConfig[timeBaseId].HWfrequency;
        StbM_SyncRecordTableHead[timeBaseIdx].HWprescaler = StbM_TimeBaseConfig[timeBaseId].HWprescaler;

        for(syncRecordTableBlockIdx = 0; syncRecordTableBlockIdx < StbM_TimeBaseConfig[timeBaseId].RecordTableBlockCount; syncRecordTableBlockIdx++)
        {
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].GlbSeconds = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].GlbNanoSeconds = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].TimeBaseStatus = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].HWcounter = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].RateDeviation = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].LocSeconds = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].LocNanoSeconds = 0U;
          StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx].PathDelay = 0U;
        }
      }
# endif

# if (STBM_OFFSET_SLAVE_TIMEBASE_COUNT > 0U)
      if(StbM_IsOffsetTimeBase(timeBaseId))
      {
        uint16 offsetRecordTableBlockIdx;

        StbM_OffsetRecordTableHead[timeBaseIdx].OffsetTimeDomain = StbM_TimeBaseConfig[timeBaseId].TimeDomainID;

        for(offsetRecordTableBlockIdx = 0; offsetRecordTableBlockIdx < StbM_TimeBaseConfig[timeBaseId].RecordTableBlockCount; offsetRecordTableBlockIdx++)
        {
          StbM_OffsetRecordTableBlock[timeBaseIdx][offsetRecordTableBlockIdx].GlbSeconds = 0U;
          StbM_OffsetRecordTableBlock[timeBaseIdx][offsetRecordTableBlockIdx].GlbNanoSeconds = 0U;
          StbM_OffsetRecordTableBlock[timeBaseIdx][offsetRecordTableBlockIdx].TimeBaseStatus = 0U;
        }
      }
# endif
    }
#endif
  }
#if (STBM_CUSTOMER_COUNT > 0U)
  for(customerId = 0U; customerId < STBM_CUSTOMER_COUNT; customerId++)
  {
    StbM_CustomerState[customerId].Counter = 0U;
    StbM_CustomerState[customerId].LastSyncTimeStamp.nanoseconds = 0U;
    StbM_CustomerState[customerId].LastSyncTimeStamp.seconds = 0U;
    StbM_CustomerState[customerId].LastSyncTimeStamp.secondsHi = 0U;
    StbM_CustomerState[customerId].LastSyncValue = 0U;
  }
#endif
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
  StbM_ResetStatusEvent();
#endif
#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  StbM_StopTimeNotificationCustomers();
#endif
#if (STBM_DEV_ERROR_DETECT == STD_ON || STBM_GPT_LOCALTIME_USED == STD_ON)
  StbM_ModuleInitialized = (uint8) STBM_INIT;
#endif
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_MainFunction()
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
FUNC(void, STBM_CODE) StbM_MainFunction(void)   /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 timeBaseId;

  /* ----- Implementation ----------------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check If module is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    return;
  }
#endif
  for(timeBaseId = 0U; timeBaseId < STBM_TIMEBASE_COUNT; timeBaseId++)
  {
    /* #20 Update synchronized local time bases */
    if(!StbM_IsOffsetTimeBase(timeBaseId))
    {
      (void) StbM_UpdateTimestamp(timeBaseId);
    }
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    /* #30 If Timeout has occurred */
    if(StbM_TimeBaseState[timeBaseId].SyncLossTimeoutCount > 0U)
    {
      StbM_TimeBaseState[timeBaseId].SyncLossTimeoutCount--;
      if(StbM_TimeBaseState[timeBaseId].SyncLossTimeoutCount == 0U)
      {
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
        uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

        if(customerIdx != STBM_UNUSED_INDEX)
        {
          uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;

          /* #31 Check and set Timeout notification event */
          StbM_CheckAndSetTimoutNotificationEv(timeBaseId, customerIdx, customerMask);

          /* #32 Check and set sync to subdomain notification event */
          StbM_CheckAndSetSyncToSubdomainEv(timeBaseId, customerIdx, customerMask, FALSE);
        }
#endif
        /* #40 Set Timeout Bit */
        StbM_SetTimeoutBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);

        /* #50 If TimeGateway is present, set Gateway Bit */
        if(StbM_TimeBaseConfig[timeBaseId].IsTimeGateway == TRUE)
        {
          StbM_SetSyncToGatewayBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
        }
      }
    }

#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
    /* #60 If each block of the record table has been updated */
    if(StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock == StbM_TimeBaseConfig[timeBaseId].RecordTableBlockCount)
    {
      /* #70 Notify the application by calling the callback block by block */
      uint16 timeBaseIdx = StbM_TimeBaseConfig[timeBaseId].SlaveTimeBaseIdx;

# if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
      if (!StbM_IsOffsetTimeBase(timeBaseId))
      {
        uint16 syncRecordTableBlockIdx;

        for(syncRecordTableBlockIdx = 0; syncRecordTableBlockIdx < StbM_TimeBaseConfig[timeBaseId].RecordTableBlockCount; syncRecordTableBlockIdx++)
        {
          StbM_SyncTimeRecordBlockCallbacks[timeBaseIdx](&StbM_SyncRecordTableBlock[timeBaseIdx][syncRecordTableBlockIdx]);
        }
      }
# endif

# if (STBM_OFFSET_SLAVE_TIMEBASE_COUNT > 0U)
      if (StbM_IsOffsetTimeBase(timeBaseId))
      {
        uint16 offsetRecordTableBlockIdx;

        for(offsetRecordTableBlockIdx = 0; offsetRecordTableBlockIdx < StbM_TimeBaseConfig[timeBaseId].RecordTableBlockCount; offsetRecordTableBlockIdx++)
        {
          StbM_OffsetTimeRecordBlockCallbacks[timeBaseIdx](&StbM_OffsetRecordTableBlock[timeBaseIdx][offsetRecordTableBlockIdx]);
        }
      }
# endif

      StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock = 0U;
    }
#endif

    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  /* #75 Start GPT Timer for Time Notification Customers if required */
  StbM_StartGptTimer(StbM_GetNextExpirationPoint(FALSE));
#endif

#if (STBM_CUSTOMER_COUNT > 0U)
  /* #80 Trigger customers */
  StbM_TriggerCustomers();
#endif

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
  {
    uint16 customerIdx = 0;
    /* #90 Call status notification and clear event */
    for(; customerIdx < STBM_STATUS_NOTIFICATION_CUSTOMER_COUNT; customerIdx++)
    {
      StbM_CallStatusNotificationCallbackAndClearEvent(customerIdx);
    }
  }
#endif
}       /* PRQA S 2006, 6010, 6030, 6050, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_NotificationFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 ********************************************************************************************************************/
FUNC(void, STBM_CODE) StbM_NotificationFunction(void)   /* PRQA S 0850 */ /* MD_MSR_19.8 */
{

  uint16 customerId = 0;

  sint64 deviationTime;
  StbM_TimeStampType timeStamp;
  StbM_UserDataType userData;
  StbM_SynchronizedTimeBaseType timeBaseId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call callbacks of elapsed timer */
  for(;customerId < STBM_TIME_NOTIFICATION_CUSTOMER_COUNT; customerId++)
  {
    if(StbM_NotificationCustomerStates[customerId].timerState == STBM_TIMER_CALL_CBK)
    {
      timeBaseId = StbM_TimeNotificationCustomerConfig[customerId].ParentTimeBaseId;
      (void) StbM_CalcTimeAndGetUserData(timeBaseId, &timeStamp, &userData);

      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

      StbM_NotificationCustomerStates[customerId].timerState = STBM_TIMER_STOPPED;
      StbM_NotificationCustomerStates[customerId].gptExpirationPoint = STBM_INVALID_EXPIRATION_POINT;
      (void) StbM_TimestampMinusTimestampInNs(&timeStamp, &(StbM_NotificationCustomerStates[customerId].expireTime), &deviationTime);

      (void) StbM_TimeNotificationCustomerConfig[customerId].TimeNotificationCallback((StbM_TimeDiffType)deviationTime);
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    }
  }
}
#endif

#if (STBM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  StbM_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, STBM_CODE) StbM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, STBM_APPL_VAR) versioninfo)     /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* Check parameter 'versioninfo' */
  if(versioninfo == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (STBM_VENDOR_ID);
    versioninfo->moduleID = (STBM_MODULE_ID);
    versioninfo->sw_major_version = (STBM_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (STBM_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (STBM_SW_PATCH_VERSION);
  }
# if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_VERSION_INFO, errorId);
  }
# endif
}
#endif

/**********************************************************************************************************************
 *  StbM_GetCurrentTime()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTime(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr)  /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check timeStampPtr parameter for NULL pointer */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  /* #40 Check userDataPtr parameter for NULL pointer */
  else if(userDataPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Calculate current time and update user data of the submitted time-base */
    retVal = StbM_CalcTimeAndGetUserData(timeBaseId, timeStampPtr, userDataPtr);
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* #60 Report error if applicable */
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_CURRENT_TIME, errorId);
  }
#endif
  return retVal;
}       /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#if (STBM_GET_CURRENT_TIME_EXTENDED_API == STD_ON)
/**********************************************************************************************************************
 *  StbM_GetCurrentTimeExtended()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeExtended(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampExtendedType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr)  /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
# if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
# endif
  StbM_TimeStampType currentTime;

  /* ----- Development Error Checks ------------------------------------- */
# if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check timeStampPtr parameter for NULL pointer */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  /* #40 Check userDataPtr parameter for NULL pointer */
  else if(userDataPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Calculate current time of the submitted time-base in standard format and update user data */
    if(StbM_CalcTimeAndGetUserData(timeBaseId, &currentTime, userDataPtr) == E_OK)
    {
      /* #60 Convert current time from standard format to extended format */
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      timeStampPtr->seconds = (uint64) currentTime.secondsHi << 16U;
      timeStampPtr->seconds <<= 16U;
      timeStampPtr->seconds += currentTime.seconds;
      timeStampPtr->nanoseconds = currentTime.nanoseconds;
      timeStampPtr->timeBaseStatus = currentTime.timeBaseStatus;
      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
      retVal = E_OK;
    }
  }

# if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_CURRENT_TIME_EXTENDED, errorId);
  }
# endif
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  StbM_GetCurrentTimeRaw()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeRaw(P2VAR(StbM_TimeStampRawType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr)      /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif
  uint32 ticks;
#if (STBM_GPT_LOCALTIME_USED == STD_OFF)
  TickType osTicks;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Check initialization state of the component */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* Check parameter 'timeStampPtr' */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
    ticks = (uint32) Gpt_GetTimeElapsed((Gpt_ChannelType) STBM_MOST_ACCURATE_LOCALTIME_ID);
#else
    (void) GetCounterValue(STBM_MOST_ACCURATE_LOCALTIME_ID, &osTicks);
    ticks = (uint32) osTicks;
#endif
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    *timeStampPtr = ticks * STBM_MOST_ACCURATE_LOCALTIME_NSPERTICK;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = E_OK;
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_CURRENT_TIME_RAW, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_GetCurrentTimeDiff()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeDiff(StbM_TimeStampRawType givenTimeStamp, P2VAR(StbM_TimeStampRawType, AUTOMATIC, STBM_APPL_VAR) timeStampDiffPtr)  /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif
  uint32 ticks;
  uint32 currentNanoseconds;
#if (STBM_GPT_LOCALTIME_USED == STD_OFF)
  TickType osTicks;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Check initialization state of the component */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* Check parameter 'timeStampDiffPtr' */
  else if(timeStampDiffPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (STBM_GPT_LOCALTIME_USED == STD_ON)
    ticks = (uint32) Gpt_GetTimeElapsed((Gpt_ChannelType) STBM_MOST_ACCURATE_LOCALTIME_ID);
#else
    (void) GetCounterValue(STBM_MOST_ACCURATE_LOCALTIME_ID, &osTicks);
    ticks = (uint32) osTicks;
#endif
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    currentNanoseconds = ticks * STBM_MOST_ACCURATE_LOCALTIME_NSPERTICK;
    if(currentNanoseconds < givenTimeStamp)
    {
      *timeStampDiffPtr = (STBM_TIMESTAMPRAW_MAX_NANOSECONDS - givenTimeStamp) + (1U + currentNanoseconds);
    }
    else
    {
      *timeStampDiffPtr = currentNanoseconds - givenTimeStamp;
    }
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = E_OK;
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_CURRENT_TIME_DIFF, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_SetGlobalTime()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_SetGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr)     /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check timeStampPtr parameter for NULL pointer */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  /* #40 Check parameter 'userDataPtr' */
  else if(userDataPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Increment update counter of time base */
    StbM_IncrementTimeBaseUpdateCounter(timeBaseId);

    /* #60 Set new global time */
    retVal = StbM_SetNewGlobalTime(timeBaseId, timeStampPtr, userDataPtr);
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_SET_GLOBAL_TIME, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_SetUserData()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_SetUserData(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr)    /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Check initialization state of the component */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* Check parameter 'timeBaseId' */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* Check parameter 'userDataPtr' */
  else if(userDataPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    StbM_TimeBaseState[timeBaseId].UserData = *userDataPtr;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = E_OK;
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_SET_USER_DATA, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_SetOffset()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_SetOffset(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr)    /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
    uint16 customerIdx;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check parameter 'timeBaseId' */
  else if((timeBaseId >= STBM_TIMEBASE_COUNT) || (!StbM_IsOffsetTimeBase(timeBaseId)))
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check parameter 'timeStampPtr' */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
    StbM_UpdateExpireTime(timeBaseId, timeStampPtr);
#endif
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
    customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

    /* #40 If notification customer is configured */
    if(customerIdx != STBM_UNUSED_INDEX)
    {
      /* Check EV_GLOBAL_TIME_BASE, EV_RESYNC */
      uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;

      /* #50 Check and set Global TimeBase notification event */
      StbM_CheckAndSetGlobalTimeBaseEv(timeBaseId, customerIdx, customerMask);

      /* #60 Check and set ReSync event */
      StbM_CheckAndSetResyncEv(customerIdx, customerMask);
    }
#endif

    /* #70 set globalTimeBase bit */
    StbM_SetGlobalTimeBaseBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
    StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds = timeStampPtr->nanoseconds;
    StbM_TimeBaseState[timeBaseId].TimeStamp.seconds = timeStampPtr->seconds;
    StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi = timeStampPtr->secondsHi;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = E_OK;

  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_SET_OFFSET, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_GetOffset()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffset(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr)      /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Check initialization state of the component */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* Check parameter 'timeBaseId' */
  else if((timeBaseId >= STBM_TIMEBASE_COUNT) || (!StbM_IsOffsetTimeBase(timeBaseId)))
  {
    errorId = STBM_E_PARAM;
  }
  /* Check parameter 'timeStampPtr' */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    *timeStampPtr = StbM_TimeBaseState[timeBaseId].TimeStamp;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
    retVal = E_OK;
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_OFFSET, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_BusSetGlobalTime()
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
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_BusSetGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr, P2CONST(StbM_MeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif
#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
  uint16 timeBaseIdx;
  uint16 currentRecordTableBlock;
#endif
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
      uint16 customerIdx;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check timeStampPtr parameter for NULL pointer */
  else if(timeStampPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  /* #40 Check measureDataPtr parameter for NULL pointer */
  else if(measureDataPtr == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = E_OK;

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
    StbM_UpdateExpireTime(timeBaseId, timeStampPtr);
#endif
#if(STBM_OFFSET_TIME_NOTIFICATION_CUSTOMER == STD_ON)
    StbM_CalcElapsedTimeOfOffsetTimeBase(timeBaseId);
#endif

    /* #50 Update local time of synchronized Time-Base */
    if(!StbM_IsOffsetTimeBase(timeBaseId))
    {
      retVal = StbM_UpdateTimestamp(timeBaseId);
    }
    /* #60 If Update was successful */
    if(retVal == E_OK)
    {
      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      /* #70 Perform Time Leap Check */
      if(StbM_GetGlobalTimeBaseBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) != 0U)
      {
        retVal = StbM_CheckTimeLeapFuture(timeBaseId, timeStampPtr);
        if(retVal == E_OK)
        {
          retVal = StbM_CheckTimeLeapPast(timeBaseId, timeStampPtr);
        }
      }

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
      customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;

      /* #71 If notification customer is configured */
      if(customerIdx != STBM_UNUSED_INDEX)
      {
        /* Check EV_GLOBAL_TIME_BASE, EV_TIMEOUT_REMOVED, EV_SYNC_TO_SUBDOMAIN, EV_SYNC_TO_GLOBAL_MASTER, EV_RESYNC */
        uint32 customerMask = StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask;

        /* #72 Check and set sync to Global TimeBase notification event */
        StbM_CheckAndSetGlobalTimeBaseEv(timeBaseId, customerIdx, customerMask);

        /* #73 Check and set TimeoutRemoved notification event */
        StbM_CheckAndSetTimoutRemovedNotificationEv(timeBaseId, customerIdx, customerMask);

        /* #74 Check and set SyncToGlobalMaster event, if SYNC_TO_GATEWAY Bit in status of provided timestamp ptr is not set */
        if(StbM_GetSyncToGatewayBit(timeStampPtr->timeBaseStatus) == 0U)
        {
          StbM_CheckAndSetSyncToGlobalMasterEv(timeBaseId, customerIdx, customerMask);
        }
        /* #75 Otherwise, check and set SyncToSubdomain notification event */
        else
        {
          StbM_CheckAndSetSyncToSubdomainEv(timeBaseId, customerIdx, customerMask, TRUE);
        }

        /* #76 Check and set ReSync event */
        StbM_CheckAndSetResyncEv(customerIdx, customerMask);

        /* Hint: Rate correction ev is set within the context of rate correction value calculation */
      }
#endif

      /* #80 Set syncToGateway bit according syncToGateway bit within timeBaseStatus of the timeStampPtr argument */
      if(StbM_GetSyncToGatewayBit(timeStampPtr->timeBaseStatus) == 0U)
      {
        StbM_ClearSyncToGatewayBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
      }
      else
      {
        StbM_SetSyncToGatewayBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);
      }

      /* #90 Set globalTimeBase bit */
      StbM_SetGlobalTimeBaseBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);

      /* #100 If no error occurred */
      if(retVal == E_OK)
      {
        SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
        /* #110 Perform rate correction measurement */
        retVal = StbM_RateCorrectionMeasurement(timeBaseId, timeStampPtr);
        SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      }

      /* #120 Clear timeout bit */
      StbM_ClearTimeoutBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus);

      /* #130 Start new measuring of timeout */
      StbM_TimeBaseState[timeBaseId].SyncLossTimeoutCount = StbM_TimeBaseConfig[timeBaseId].SyncLossTimeout;

/* #140 If time recording support is enabled */
#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
      if(StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock == StbM_TimeBaseConfig[timeBaseId].RecordTableBlockCount)
      {
        StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock = 0U;
      }

      timeBaseIdx = StbM_TimeBaseConfig[timeBaseId].SlaveTimeBaseIdx;
      currentRecordTableBlock = StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock;

# if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
      if (timeBaseIdx != STBM_NO_TIMEBASEID)
      {
        /* #150 If time base is a synchronized time base */
        if(!StbM_IsOffsetTimeBase(timeBaseId))
        {
          /* #160 Write current local time according to block elements of record table */
          StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].LocSeconds = StbM_TimeBaseState[timeBaseId].TimeStamp.seconds;
          StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].LocNanoSeconds = StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds;
        }
      }
# endif
#endif

      /* #170 Set local time to global time */
      StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds = timeStampPtr->nanoseconds;
      StbM_TimeBaseState[timeBaseId].TimeStamp.seconds = timeStampPtr->seconds;
      StbM_TimeBaseState[timeBaseId].TimeStamp.secondsHi = timeStampPtr->secondsHi;

      /* #180 Increment update counter of time base */
      StbM_IncrementTimeBaseUpdateCounter(timeBaseId);

      /* #190 Update User Data if provided data is not NULL */
      if(userDataPtr != NULL_PTR)
      {
        StbM_TimeBaseState[timeBaseId].UserData = *userDataPtr;
      }

      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

      /* #200 Update local clock of synchronized time base */
      if(!StbM_IsOffsetTimeBase(timeBaseId))
      {
        retVal = StbM_UpdateLocalClock(timeBaseId, retVal);
      }

/* #210 If time recording support is enabled */
#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
      if (timeBaseIdx != STBM_NO_TIMEBASEID)
      {
        /* #220 Write residual block elements of record table */
# if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
        if(!StbM_IsOffsetTimeBase(timeBaseId))
        {
          StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].GlbSeconds = StbM_TimeBaseState[timeBaseId].TimeStamp.seconds;
          StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].GlbNanoSeconds = StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds;
          StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].TimeBaseStatus = StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus;
          if(StbM_IsOsLocalTimeUsed(timeBaseId) || StbM_IsGptLocalTimeUsed(timeBaseId))
          {
            StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].HWcounter = StbM_TimeBaseState[timeBaseId].LocalTimeTickValue;
          }
#  if (STBM_ETHTSYN_LOCALTIME_USED == STD_ON)
          else
          {
            StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].HWcounter = StbM_TimeBaseState[timeBaseId].LocalTime.nanoseconds;
          }
#  endif
          if(StbM_TimeBaseState[timeBaseId].RateCorrection != 0)
          {
            StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].RateDeviation = (sint16)StbM_TimeBaseState[timeBaseId].RateCorrection - 1;
          }
          StbM_SyncRecordTableBlock[timeBaseIdx][currentRecordTableBlock].PathDelay = measureDataPtr->pathDelay;
        }
# endif

# if (STBM_OFFSET_SLAVE_TIMEBASE_COUNT > 0U)
        if(StbM_IsOffsetTimeBase(timeBaseId))
        {
          StbM_OffsetRecordTableBlock[timeBaseIdx][currentRecordTableBlock].GlbSeconds = StbM_TimeBaseState[timeBaseId].TimeStamp.seconds;
          StbM_OffsetRecordTableBlock[timeBaseIdx][currentRecordTableBlock].GlbNanoSeconds = StbM_TimeBaseState[timeBaseId].TimeStamp.nanoseconds;
          StbM_OffsetRecordTableBlock[timeBaseIdx][currentRecordTableBlock].TimeBaseStatus = StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus;
        }
# endif

        StbM_TimeBaseState[timeBaseId].CurrentRecordTableBlock++;
      }
#endif

#if(STBM_OFFSET_TIME_NOTIFICATION_CUSTOMER == STD_ON)
      StbM_UpdateExpireTimeOfOffsetTimeBase(timeBaseId);
#endif

    }
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_BUS_SET_GLOBAL_TIME, errorId);
  }
#elif ((STBM_TIME_RECORDING_SUPPORT_USED == STD_OFF) || (STBM_SYNC_SLAVE_TIMEBASE_COUNT == 0U))
  STBM_DUMMY_STATEMENT(measureDataPtr);        /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
#endif
  return retVal;
}       /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_GetRateDeviation()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetRateDeviation(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_RateDeviationType, AUTOMATIC, STBM_APPL_VAR) rateDeviation)   /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if((timeBaseId >= STBM_TIMEBASE_COUNT) || StbM_IsOffsetTimeBase(timeBaseId))
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check rateDeviation parameter for NULL pointer */
  else if(rateDeviation == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    /* #40 If rate correction value is set */
    if(StbM_TimeBaseState[timeBaseId].RateCorrection != 0)
    {
      /* #50 Derive rateDeviation as rateCorrection - 1 */
      *rateDeviation = StbM_TimeBaseState[timeBaseId].RateCorrection - 1;
      retVal = E_OK;
    }
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_RATE_DEVIATION, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_SetRateCorrection()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_SetRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType rateDeviation)   /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check parameter 'timeBaseId' */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If service is enabled */
    if(StbM_TimeBaseConfig[timeBaseId].IsMasterRateCorrectionAllowed == TRUE)
    {
#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
      uint16 customerIdx = StbM_TimeBaseConfig[timeBaseId].StatusNotificationCustomerIndex;
#endif

      SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
      /* #40 If rateDeviation exceeds MasterRateDeviationMax, set value to MasterRateDeviationMax */
      if(rateDeviation > StbM_TimeBaseConfig[timeBaseId].MasterRateDeviationMax)
      {
        StbM_TimeBaseState[timeBaseId].RateCorrection = StbM_TimeBaseConfig[timeBaseId].MasterRateDeviationMax;
      }
      else if(rateDeviation < -StbM_TimeBaseConfig[timeBaseId].MasterRateDeviationMax)
      {
        StbM_TimeBaseState[timeBaseId].RateCorrection = -StbM_TimeBaseConfig[timeBaseId].MasterRateDeviationMax;
      }
      /* #50 otherwise set RateCorrection value to rateDeviation + 1 */
      else
      {
        StbM_TimeBaseState[timeBaseId].RateCorrection = rateDeviation + 1;
      }

#if (STBM_STATUS_NOTIFICATION_CUSTOMER == STD_ON)
      /* #60 If notification customer is configured */
      if(customerIdx != STBM_UNUSED_INDEX)
      {
        /* Check EV_RATECORRECTION */
        /* #70 Check and set Ratecorrection notification event */
        StbM_CheckAndSetRatecorrectionEv(timeBaseId, customerIdx, StbM_StatusNotificationCustomerConfig[customerIdx].StatusNotificationMask);
      }
#endif

      SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();

      retVal = E_OK;
    }
#if (STBM_DEV_ERROR_DETECT == STD_ON)
    else
    {
      errorId = STBM_E_SERVICE_DISABLED;
    }
#endif
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_SET_RATE_CORRECTION, errorId);
  }
#endif
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
# if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
/**********************************************************************************************************************
 *  StbM_GetSyncTimeRecordHead()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetSyncTimeRecordHead(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_VAR) syncRecordTableHead)  /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#  if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#  endif

  /* ----- Development Error Checks ------------------------------------- */
#  if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (StbM_ModuleInitialized == (uint8)STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* Check parameter 'timeBaseId' */
  else if (timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* Check parameter 'syncRecordTableHead' */
  else if (syncRecordTableHead == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    *syncRecordTableHead = StbM_SyncRecordTableHead[StbM_TimeBaseConfig[timeBaseId].SlaveTimeBaseIdx];
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
  }

#  if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_SYNC_TIME_RECORD_HEAD, errorId);
  }
#  endif
  return retVal;
}
# endif

# if (STBM_OFFSET_SLAVE_TIMEBASE_COUNT > 0U)
/**********************************************************************************************************************
 *  StbM_GetOffsetTimeRecordHead()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffsetTimeRecordHead(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_VAR) offsetRecordTableHead)  /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#  if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#  endif

  /* ----- Development Error Checks ------------------------------------- */
#  if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (StbM_ModuleInitialized == (uint8)STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* Check parameter 'timeBaseId' */
  else if (timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* Check parameter 'offsetRecordTableHead' */
  else if (offsetRecordTableHead == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    *offsetRecordTableHead = StbM_OffsetRecordTableHead[StbM_TimeBaseConfig[timeBaseId].SlaveTimeBaseIdx];
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    retVal = E_OK;
  }

#  if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_OFFSET_TIME_RECORD_HEAD, errorId);
  }
#  endif
  return retVal;
}
# endif
#endif

/**********************************************************************************************************************
 *  StbM_TimerCallback()
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
FUNC(void, STBM_CODE) StbM_TimerCallback(void)
{
#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  uint16 customerId = 0;
  sint32 nextExpirationPoint;

  /* #5 Change state of customers, whose timer has elapsed */
  SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
  for(; customerId < STBM_TIME_NOTIFICATION_CUSTOMER_COUNT; customerId++)
  {
    if(StbM_NotificationCustomerStates[customerId].timerState == STBM_TIMER_CBK_EXPECTED)
    {
      StbM_NotificationCustomerStates[customerId].timerState = STBM_TIMER_CALL_CBK;
    }
  }
  SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  /* #10 Calculate next expiration point */
  nextExpirationPoint = StbM_GetNextExpirationPoint(TRUE);

  /* #20 Start GPT Timer if requested, otherwise set status of GPT Timer to Stopped */
  if(StbM_GptTimerState == STBM_GPT_TIMER_START_REQUIRED)
  {
    StbM_StartGptTimer(nextExpirationPoint);
  }
  else
  {
    SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
    StbM_GptTimerState = STBM_GPT_TIMER_STOPPED;
    SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
  }

  /* #30 Set Event for StbM_NotificationFunction if event is configured */
# if (STBM_USE_OS_SET_EVENT_API == STD_ON)
  (void) SetEvent(STBM_TIME_CUSTOMER_TASK_NAME, STBM_TIME_CUSTOMER_EVENT_NAME);
# else
  /* #40 Otherwise Activate Task for StbM_NotificationFunction */
  (void) ActivateTask(STBM_TIME_CUSTOMER_TASK_NAME);
# endif
#endif
}

/**********************************************************************************************************************
 *  StbM_StartTimer()
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
 ******************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_StartTimer(StbM_SynchronizedTimeBaseType timeBaseId, StbM_CustomerIdType customerId,  /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
                                                P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) expireTime)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (StbM_ModuleInitialized == (uint8)STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
# if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
  /* #30 Check timeStampPtr parameter for NULL pointer */
  else if(customerId >= STBM_TIME_NOTIFICATION_CUSTOMER_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #40 Check if customerId belongs to given TimeBase Id */
  else if(StbM_TimeNotificationCustomerConfig[StbM_InternalIdOfNotificationCustomerConfig[customerId]].ParentTimeBaseId != timeBaseId)
  {
    errorId = STBM_E_PARAM;
  }
# endif
  /* #50 Check expireTime against NULL_PTR */
  else if(expireTime == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
    uint16 customerIdx = StbM_InternalIdOfNotificationCustomerConfig[customerId];

    /* #60 If timer is not already running */
    if(StbM_NotificationCustomerStates[customerIdx].timerState == STBM_TIMER_STOPPED)
    {
      StbM_TimeStampType currentTime;
      StbM_UserDataType userData;

      if(StbM_CalcTimeAndGetUserData(timeBaseId, &currentTime, &userData) == E_OK)
      {
        SchM_Enter_StbM_STBM_EXCLUSIVE_AREA_0();
        /* #70 If given expireTime is valid */
        if(StbM_IsExpireTimeValid(expireTime) == TRUE)
        {
          /* #80 Calculate expire point as current time + given expire time */
          retVal = StbM_TimestampPlusTimestamp(&currentTime, expireTime, &(StbM_NotificationCustomerStates[customerIdx].expireTime));
          if(retVal == E_OK)
          {
            /* #90 If no error occurred, update state of customer as started */
            StbM_NotificationCustomerStates[customerIdx].timerState = STBM_TIMER_RUNNING;
            StbM_NotificationCustomerStates[customerIdx].gptExpirationPoint = STBM_INVALID_EXPIRATION_POINT;
          }
        }
        SchM_Exit_StbM_STBM_EXCLUSIVE_AREA_0();
      }
    }

#endif
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_START_TIMER, errorId);
  }
#elif (STBM_TIME_NOTIFICATION_CUSTOMER == STD_OFF)
  STBM_DUMMY_STATEMENT(expireTime); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  STBM_DUMMY_STATEMENT(customerId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  STBM_DUMMY_STATEMENT(timeBaseId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  StbM_UpdateGlobalTime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData)     /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check timeStamp parameter for NULL pointer */
  else if(timeStamp == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Set new global time */
    retVal = StbM_SetNewGlobalTime(timeBaseId, timeStamp, userData);
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_UPDATE_GLOBAL_TIME, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_TriggerTimeTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_TriggerTimeTransmission(StbM_SynchronizedTimeBaseType timeBaseId)   /* PRQA S 0850, 1330 */ /* MD_MSR_19.8, MD_StbM_1330 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Increment update counter of time base */
    StbM_IncrementTimeBaseUpdateCounter(timeBaseId);
    retVal = E_OK;
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_TRIGGER_TIME_TRANSMISSION, errorId);
  }
#endif
  return retVal;
}

/**********************************************************************************************************************
 *  StbM_GetTimeBaseUpdateCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(uint8, STBM_CODE) StbM_GetTimeBaseUpdateCounter(StbM_SynchronizedTimeBaseType timeBaseId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = 0U;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = StbM_TimeBaseState[timeBaseId].UpdateCounter;
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_TIME_BASE_UPDATE_COUNTER, errorId);
  }
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  StbM_GetTimeLeap()
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeLeap(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeDiffType, AUTOMATIC, STBM_APPL_VAR) timeJump)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = STBM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (STBM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if component is initialized */
  if(StbM_ModuleInitialized == (uint8) STBM_UNINIT)
  {
    errorId = STBM_E_NOT_INITIALIZED;
  }
  /* #20 Check timeBaseId parameter validity against configuration */
  else if(timeBaseId >= STBM_TIMEBASE_COUNT)
  {
    errorId = STBM_E_PARAM;
  }
  /* #30 Check timeJump parameter for NULL pointer */
  else if(timeJump == NULL_PTR)
  {
    errorId = STBM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If the last time leap fits into the range, set timeJump to the corresponding value */
    if(((StbM_GetTimeLeapFutureBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U) && (StbM_GetTimeLeapPastBit(StbM_TimeBaseState[timeBaseId].TimeStamp.timeBaseStatus) == 0U))
      || (StbM_TimeBaseState[timeBaseId].LastTimeLeapValue > STBM_TIMELEAP_MAX_FUTURE) || (StbM_TimeBaseState[timeBaseId].LastTimeLeapValue < STBM_TIMELEAP_MAX_PAST))
    {
      retVal = E_NOT_OK;
    }
    else
    {
      *timeJump = (sint32) StbM_TimeBaseState[timeBaseId].LastTimeLeapValue;
      retVal = E_OK;
    }
  }

#if (STBM_DEV_ERROR_DETECT == STD_ON)
  if(errorId != STBM_E_NO_ERROR)
  {
    StbM_ReportError(STBM_SID_GET_TIMELEAP, errorId);
  }
#endif
  return retVal;
}

#define STBM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_StbM_1330:  MISRA rule: 16.4
     Reason:     The RTE Generator uses default names for parameter identifiers of port defined arguments of service modules.
                 Therefore the parameter identifiers in the function declaration differs from those of the implementation of the BSW module.
     Risk:       No functional risk.
     Prevention: Not required.
*/

/* COV_JUSTIFICATION_BEGIN

--- Runtime Coverage Justifications ------------------------------------------------------------------------------
  \ID COV_STBM_MSR_COMPATIBILITY
    \ACCEPT TX
    \REASON [COV_MSR_COMPATIBILITY]

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: StbM.c
 *********************************************************************************************************************/
