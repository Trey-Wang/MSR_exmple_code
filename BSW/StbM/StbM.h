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
/*!        \file  StbM.h
 *        \brief  Vector AUTOSAR StbM header file
 *
 *      \details  This is the main header file of the Vector AUTOSAR module StbM.

 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Stephanie Schaaf              visssf        Vector Informatik GmbH
 *  Anant Gupta                   visgut        Vector Informatik GmbH
 *  Thilo Rachlitz                vistra        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-11-05  visssf  ESCAN00060987 AR4-165: Provide AR4 compliant module StbM
 *  01.01.00  2013-04-12  visssf  ESCAN00066630 Compiler error: Missing FrIf.h when the configuration contains no FlexRay
 *            2013-05-03  visssf  ESCAN00067123 Missing exclusive area protection may lead to data inconsistency
 *  02.00.00  2013-09-06  visssf  ESCAN00070221 Compiler warning: file does not end in newline
 *  03.00.00  2014-09-18  visssf  ESCAN00077484 AR4-664: Support new global time synchronization concept
 *  03.01.00  2014-12-02  visssf  ESCAN00079950 FEAT-971: Complete support of new global time synchronization concept
 *  03.01.01  2015-03-02  visssf  ESCAN00081461 Compiler error: 'Boolean' : name in formal parameter list illegal
 *            2015-03-20  visssf  ESCAN00081652 Wrong calculation of time difference for setting timeleap bit
 *  03.02.00  2015-07-14  visssf  ESCAN00083401 StbM_TimeStampRawType is not compatible to AR 4.2.1
 *  04.00.00  2015-12-22  visssf  ESCAN00085766 FEAT-1531: Support of High Resolution Time Base Reference Clock based on GPT
 *            2016-01-05  visssf  ESCAN00083451 Missing call of EthTSyn_SetGlobalTime() in StbM_SetGlobalTime() if EthTSynHardwareTimestampSupport is set to True
 *  04.01.00  2016-02-26  visssf  ESCAN00088200 FEAT-1694: Support Time Gateway
 *            2016-03-14  visssf  ESCAN00088690 TimeStamp is updated with wrong value when OsCounter is used as local time
 *            2016-03-15  visssf  ESCAN00088923 Missing interrupt locks
 *            2016-03-18  visssf  ESCAN00087326 MISRA deviation: MISRA-C:2004 Rules 12.8, 5.6, 14.3, 16.10, 13.2, 12.1, 12.5, 19.10, 19.7, 12.6 and HIS Metrics
 *            2016-03-30  visssf  ESCAN00089136 TimeBaseStatus is cleared when EthGlobalTimeDomain referenced as local time is in state ETHTSYN_UNCERTAIN
 *            2016-03-30  visssf  ESCAN00089139 SyncScheduleTable is called with <Value> out of range
 *  04.01.01  2016-04-12  visssf  ESCAN00089411 StbM_GetCurrentTimeRaw() and StbM_GetCurrentTimeDiff() return wrong time values
 *            2016-04-13  visssf  ESCAN00089428 SyncScheduleTable is called with <Value> out of range
 *  04.02.00  2017-03-07  visssf  FEATC-644     FEAT-2279: Time Synchronization acc. to AR 4.3 [Feature Pack 1]
 *            2017-03-14  visssf  ESCAN00094362 Already running GPT channel is started again
 *  05.00.00  2017-04-07  visssf  -             No changes, only generator version increased
 *  05.01.00  2017-06-19  visgut  STORYC-1016   Refactor Code taking MISRA into account
 *            2017-06-21  visgut  ESCAN00095102 Offset Time Base can not reference Synchronized Time Base with ID 0
 *            2017-06-21  visssf  STORYC-120    Global Time Precision Measurement Support
 *            2017-06-22  visssf  ESCAN00095043 Time Base gets an invalid status
 *            2017-06-28  visssf  ESCAN00095665 OS API called with disabled interrupts
 *  05.02.00  2017-07-26  visgut  STORYC-122    New Notifications for time expiration events and status changed events
 *            2017-08-02  visssf  STORYC-107    Immediate Time Synchronization
 *            2017-08-15  vistra  STORYC-2200   STORY-2225: Support handling of time leaps according AR 4.3
 *            2017-08-30  visssf  ESCAN00095280 Rate correction factor can not be calculated for time differences bigger than two seconds
 *            2017-08-31  visssf  ESCAN00096484 Overflow not correctly handled
 *            2017-09-01  vistra  ESCAN00093360 Encapsulate optional calls of OS functions
 *********************************************************************************************************************/

#if !defined (STBM_H)
# define STBM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "StbM_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Vendor and module identification */
# define STBM_VENDOR_ID                                (30U)
# define STBM_MODULE_ID                                (0xA0U)

/* AUTOSAR Software specification version information */
# define STBM_AR_RELEASE_MAJOR_VERSION                 (0x04U)
# define STBM_AR_RELEASE_MINOR_VERSION                 (0x03U)
# define STBM_AR_RELEASE_REVISION_VERSION              (0x00U)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define STBM_SW_MAJOR_VERSION                         (5U)
# define STBM_SW_MINOR_VERSION                         (2U)
# define STBM_SW_PATCH_VERSION                         (0U)

/* ----- API service IDs ----- */
# define STBM_SID_INIT                                 (0x00U)  /* !< Service ID: StbM_Init() */
# define STBM_SID_MAIN_FUNCTION                        (0x04U)  /* !< Service ID: StbM_MainFunction() */
# define STBM_SID_GET_VERSION_INFO                     (0x05U)  /* !< Service ID: StbM_GetVersionInfo() */
# define STBM_SID_GET_CURRENT_TIME                     (0x07U)  /* !< Service ID: StbM_GetCurrentTime() */
# define STBM_SID_GET_CURRENT_TIME_EXTENDED            (0x08U)  /* !< Service ID: StbM_GetCurrentTimeExtended() */
# define STBM_SID_GET_CURRENT_TIME_RAW                 (0x09U)  /* !< Service ID: StbM_GetCurrentTimeRaw() */
# define STBM_SID_GET_CURRENT_TIME_DIFF                (0x0AU)  /* !< Service ID: StbM_GetCurrentTimeDiff() */
# define STBM_SID_SET_GLOBAL_TIME                      (0x0BU)  /* !< Service ID: StbM_SetGlobalTime() */
# define STBM_SID_SET_USER_DATA                        (0x0CU)  /* !< Service ID: StbM_SetUserData() */
# define STBM_SID_SET_OFFSET                           (0x0DU)  /* !< Service ID: StbM_SetOffset() */
# define STBM_SID_GET_OFFSET                           (0x0EU)  /* !< Service ID: StbM_GetOffset() */
# define STBM_SID_BUS_SET_GLOBAL_TIME                  (0x0FU)  /* !< Service ID: StbM_BusSetGlobalTime() */
# define STBM_SID_UPDATE_GLOBAL_TIME                   (0x10U)  /* !< Service ID: StbM_UpdateGlobalTime() */
# define STBM_SID_GET_RATE_DEVIATION                   (0x11U)  /* !< Service ID: StbM_GetRateDeviation() */
# define STBM_SID_SET_RATE_CORRECTION                  (0x12U)  /* !< Service ID: StbM_SetRateCorrection() */
# define STBM_SID_GET_TIMELEAP                         (0x13U)  /* !< Service ID: StbM_GetTimeLeap() */
# define STBM_SID_START_TIMER                          (0x15U)  /* !< Service ID: StbM_StartTimer() */
# define STBM_SID_GET_SYNC_TIME_RECORD_HEAD            (0x16U)  /* !< Service ID: StbM_GetSyncTimeRecordHead() */
# define STBM_SID_GET_OFFSET_TIME_RECORD_HEAD          (0x17U)  /* !< Service ID: StbM_GetOffsetTimeRecordHead() */
# define STBM_SID_GET_TIME_BASE_UPDATE_COUNTER         (0x1BU)  /* !< Service ID: StbM_GetTimeBaseUpdateCounter() */
# define STBM_SID_TRIGGER_TIME_TRANSMISSION            (0x1CU)  /* !< Service ID: StbM_TriggerTimeTransmission() */

/* ----- Error codes ----- */
# define STBM_E_NO_ERROR                               (0x00U)  /* !< used to check if no error occurred */
# define STBM_E_PARAM                                  (0x0AU)  /* !< Error code: API service called with wrong parameter */
# define STBM_E_NOT_INITIALIZED                        (0x0BU)  /* !< Error code: API service used without module initialization */
# define STBM_E_PARAM_POINTER                          (0x10U)  /* !< Error code: API service called with NULL_PTR as parameter */
# define STBM_E_SERVICE_DISABLED                       (0x12U)  /* !< Error code: API service called that is disabled by configuration */

/* ----- Modes ----- */
# define STBM_UNINIT                                   (0x00U)
# define STBM_INIT                                     (0x01U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define STBM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  StbM_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function can be used to initialize the
 *               variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
**********************************************************************************************************************/
FUNC(void, STBM_CODE) StbM_InitMemory(void);    /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module StbM. It initializes all variables and sets the module state to
 *               initialized.
 *  \pre         Interrupts are disabled.
 *  \pre         StbM_InitMemory has been called unless StbM_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, STBM_CODE) StbM_Init(void);  /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main function
 *  \details     This function gets time information from providers and triggers customers.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  StbM_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     StbM_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, STBM_CODE) StbM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, STBM_APPL_VAR) versioninfo);    /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_GetCurrentTime()
 *********************************************************************************************************************/
/*! \brief       Returns the current time in standard format
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
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTime(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr);        /* PRQA S 0850 */ /* MD_MSR_19.8 */

# if (STBM_GET_CURRENT_TIME_EXTENDED_API == STD_ON)
/**********************************************************************************************************************
 *  StbM_GetCurrentTimeExtended()
 *********************************************************************************************************************/
/*! \brief       Returns the current time in extended format
 *  \details     This function returns the current time of the submitted time-base in extended format.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is of interest.
 *  \param[out]  timeStampPtr            Current time stamp in extended format.
 *  \param[out]  userDataPtr             User data of the time base.
 *  \return      E_OK     - Timestamp is valid
 *  \return      E_NOT_OK - Timestamp is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeExtended(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampExtendedType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr, P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_VAR) userDataPtr);        /* PRQA S 0850 */ /* MD_MSR_19.8 */
# endif

/**********************************************************************************************************************
 *  StbM_GetCurrentTimeRaw()
 *********************************************************************************************************************/
/*! \brief       Returns time value in raw format from most accurate time source
 *  \details     This function returns a time value in raw format from the most accurate time source.
 *  \param[out]  timeStampPtr            Current time stamp in raw format.
 *  \return      E_OK     - Timestamp is valid
 *  \return      E_NOT_OK - Timestamp is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeRaw(P2VAR(StbM_TimeStampRawType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr);    /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_GetCurrentTimeDiff()
 *********************************************************************************************************************/
/*! \brief       Returns difference of current time raw minus given time raw
 *  \details     This function returns the time difference of current time raw minus given time raw by using the most accurate time source.
 *  \param[in]   givenTimeStamp          Given time stamp as as difference calculation basis.
 *  \param[out]  timeStampDiffPtr        Time difference of current time stamp minus given time stamp.
 *  \return      E_OK     - Time difference is valid
 *  \return      E_NOT_OK - Time difference is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeDiff(StbM_TimeStampRawType givenTimeStamp, P2VAR(StbM_TimeStampRawType, AUTOMATIC, STBM_APPL_VAR) timeStampDiffPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_SetGlobalTime()
 *********************************************************************************************************************/
/*! \brief       Sets new global time
 *  \details     This function allows the customers to set the new global time.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is set.
 *  \param[in]   timeStampPtr            New time stamp.
 *  \param[in]   userDataPtr             New user data.
 *  \return      E_OK     - New global time was set
 *  \return      E_NOT_OK - New global time was not set
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_SetGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr);   /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_SetUserData()
 *********************************************************************************************************************/
/*! \brief       Sets new user data
 *  \details     This function allows the customers to set the new user data.
 *  \param[in]   timeBaseId              The synchronized time-base, whose user data is set.
 *  \param[in]   userDataPtr             New user data.
 *  \return      E_OK     - New user data was set
 *  \return      E_NOT_OK - New user data was not set
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_SetUserData(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr);  /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_SetOffset()
 *********************************************************************************************************************/
/*! \brief       Sets the offset time
 *  \details     This function allows the customers and the time base provider modules to set the offset time.
 *  \param[in]   timeBaseId              The synchronized time-base, whose offset time is set.
 *  \param[in]   timeStampPtr            New offset time stamp.
 *  \return      E_OK     - New offset time was set
 *  \return      E_NOT_OK - New offset time was not set
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_SetOffset(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr);  /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_GetOffset()
 *********************************************************************************************************************/
/*! \brief       Returns the current offset time
 *  \details     This function allows the time base provider modules to get the current offset time.
 *  \param[in]   timeBaseId              The synchronized time-base, whose offset time is of interest.
 *  \param[out]  timeStampPtr            Current offset time stamp.
 *  \return      E_OK     - Timestamp is valid
 *  \return      E_NOT_OK - Timestamp is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffset(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_VAR) timeStampPtr);     /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_BusSetGlobalTime()
 *********************************************************************************************************************/
/*! \brief       Sets new global time
 *  \details     This function allows the time base provider modules to set the new global time.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is set.
 *  \param[in]   timeStampPtr            New time stamp.
 *  \param[in]   userDataPtr             New user data.
 *  \param[in]   measureDataPtr          New measurement data.
 *  \return      E_OK     - New global time was set
 *  \return      E_NOT_OK - New global time was not set
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_BusSetGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr, P2CONST(StbM_MeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr);        /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_GetRateDeviation()
 *********************************************************************************************************************/
/*! \brief       Returns the current rate deviation
 *  \details     This function allows the customers to get the current rate deviation.
 *  \param[in]   timeBaseId              The synchronized time-base, whose rate deviation is of interest.
 *  \param[out]  rateDeviation           Current rate deviation.
 *  \return      E_OK     - Rate deviation is valid
 *  \return      E_NOT_OK - Rate deviation is invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetRateDeviation(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_RateDeviationType, AUTOMATIC, STBM_APPL_VAR) rateDeviation); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_SetRateCorrection()
 *********************************************************************************************************************/
/*! \brief       Sets rate of a time base
 *  \details     This function allows the customers to set the rate of a synchronized time base.
 *  \param[in]   timeBaseId              The synchronized time-base, whose rate is set.
 *  \param[in]   rateDeviation           New rate.
 *  \return      E_OK     - New rate was set
 *  \return      E_NOT_OK - New rate was not set
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_SetRateCorrection(StbM_SynchronizedTimeBaseType timeBaseId, StbM_RateDeviationType rateDeviation); /* PRQA S 0850 */ /* MD_MSR_19.8 */

# if (STBM_TIME_RECORDING_SUPPORT_USED == STD_ON)
#  if (STBM_SYNC_SLAVE_TIMEBASE_COUNT > 0U)
/**********************************************************************************************************************
 *  StbM_GetSyncTimeRecordHead()
 *********************************************************************************************************************/
/*! \brief       Accesses the header of a synchronized time base
 *  \details     This function allows the customers to access the recorded snapshot data header of the table belonging to the Synchronized Time Base.
 *  \param[in]   timeBaseId              The synchronized time-base, whose header is of interest.
 *  \param[out]  syncRecordTableHead     Header of the recorded snapshot data.
 *  \return      E_OK     - Table access done
 *  \return      E_NOT_OK - Table contains no data or access invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetSyncTimeRecordHead(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_VAR) syncRecordTableHead); /* PRQA S 0850 */ /* MD_MSR_19.8 */
#  endif

#  if (STBM_OFFSET_SLAVE_TIMEBASE_COUNT > 0U)
/**********************************************************************************************************************
 *  StbM_GetOffsetTimeRecordHead()
 *********************************************************************************************************************/
/*! \brief       Accesses the header of an offset time base
 *  \details     This function allows the customers to access the recorded snapshot data header of the table belonging to the Offset Time Base.
 *  \param[in]   timeBaseId              The offset time-base, whose header is of interest.
 *  \param[out]  offsetRecordTableHead   Header of the recorded snapshot data.
 *  \return      E_OK     - Table access done
 *  \return      E_NOT_OK - Table contains no data or access invalid
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffsetTimeRecordHead(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_VAR) offsetRecordTableHead); /* PRQA S 0850 */ /* MD_MSR_19.8 */
#  endif
# endif

/**********************************************************************************************************************
 *  StbM_StartTimer()
 *********************************************************************************************************************/
/*! \brief       Sets a time value, which the Time Base value is compared against
 *  \details     Starts a timer for a Time Base
 *  \param[in]   timeBaseId       Time Base reference
 *  \param[in]   customerId       Id of customer
 *  \param[in]   expireTime       Time value relative to current Time Base value of the Notification Customer,
 *                                when the Timer shall expire
 *  \return      E_OK       -     successful
 *               E_NOT_OK   -     failed
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_StartTimer(StbM_SynchronizedTimeBaseType timeBaseId, StbM_CustomerIdType customerId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) expireTime); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_UpdateGlobalTime()
 *********************************************************************************************************************/
/*! \brief       Sets new global time
 *  \details     This function allows the customers to set the new global time without leading to an immediate transmission of the global time.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time is set.
 *  \param[in]   timeStamp               New time stamp.
 *  \param[in]   userData                New user data.
 *  \return      E_OK     - New global time was set
 *  \return      E_NOT_OK - New global time was not set
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateGlobalTime(StbM_SynchronizedTimeBaseType timeBaseId, P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp, P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_TriggerTimeTransmission()
 *********************************************************************************************************************/
/*! \brief       Increments the update counter of a time base
 *  \details     This function allows the customers to force the Timesync module to transmit the time base again due to an incremented timeBaseUpdateCounter.
 *  \param[in]   timeBaseId               The time-base, whose update counter shall be incremented.
 *  \return      E_OK     - Update counter was incremented
 *               E_NOT_OK - Update counter was not incremented
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_TriggerTimeTransmission(StbM_SynchronizedTimeBaseType timeBaseId); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  StbM_GetTimeBaseUpdateCounter()
 *********************************************************************************************************************/
/*! \brief       Indicates if a time base has been updated
 *  \details     This function allows the Timesync modules to detect, whether a time base should be transmitted immediately in the subsequent <Bus>TSyn_MainFunction() cycle.
 *  \param[in]   timeBaseId               The time-base, whose update counter is of interest.
 *  \return      counter value - Counter value belonging to the time base, that indicates a time base update to the Timesync modules.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(uint8, STBM_CODE) StbM_GetTimeBaseUpdateCounter(StbM_SynchronizedTimeBaseType timeBaseId); /* PRQA S 0850 */ /* MD_MSR_19.8 */

#if (STBM_TIME_NOTIFICATION_CUSTOMER == STD_ON)
/**********************************************************************************************************************
 *  StbM_NotificationFunction()
 *********************************************************************************************************************/
/*! \brief       This function calls the Callbacks Function of Time Notification Customers, whose timer has expired.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *******************************************************************************************************************/
#endif

/**********************************************************************************************************************
 *  StbM_GetTimeLeap()
 *********************************************************************************************************************/
/*! \brief       Returns the the value of the last time leap
 *  \details     This function allows the customers to get the value of Time Leap, if StbMTimeLeapFuture/PastThreshold is exceeded.
 *  \param[in]   timeBaseId              The synchronized time-base, whose time leap is of interest.
 *  \param[out]  timeJump                Time leap value.
 *  \return      E_OK     - Time leap is valid.
 *  \return      E_NOT_OK - No Time leap occured or time leap is out of range.
 *  \pre         Application has to ensure that the TIMELEAP_FUTURE or TIMELEAP_PAST bit within timebase status has been set.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeLeap(StbM_SynchronizedTimeBaseType timeBaseId, P2VAR(StbM_TimeDiffType, AUTOMATIC, STBM_APPL_VAR) timeJump);

# define STBM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/*!
* \exclusivearea STBM_EXCLUSIVE_AREA_0
* Ensures consistency of global RAM variables.
* \protects All affected global RAM variables.
* \usedin StbM_MainFunction, StbM_GetCurrentTime, StbM_GetCurrentTimeExtended, StbM_GetCurrentTimeRaw, StbM_GetCurrentTimeDiff, StbM_SetGlobalTime, StbM_SetUserData, StbM_SetOffset, StbM_GetOffset, StbM_BusSetGlobalTime, StbM_GetRateDeviation, StbM_SetRateCorrection, StbM_UpdateTimestamp, StbM_GetSyncTimeRecordHead, StbM_GetOffsetTimeRecordHead
* \exclude All functions provided by StbM.
* \length SHORT depending on the API and the call graph.
* \endexclusivearea
*/

#endif /* STBM_H */

/**********************************************************************************************************************
 *  END OF FILE: StbM.h
 *********************************************************************************************************************/
