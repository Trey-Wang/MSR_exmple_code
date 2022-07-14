/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Rtm.h
 *    Component:  -
 *       Module:  RTM
 *    Generator:  -
 *
 *  Description:  RTM static header file.
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alexander Zeeb                visazb        Vector Informatik GmbH
 *  Oliver Reineke                visore        Vector Informatik GmbH
 *  David Zentner                 viszda        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-04-22  visazb  -             Initial version
 *  01.00.01  2013-08-06  visore  ESCAN00068379 Unknown runtime behavior if Rtm_InitMemory is not invoked
 *            2013-08-06  visore  ESCAN00069628 No changes here
 *            2013-07-06  visore  ESCAN00069629 Compiler warning: undeclared symbol 'Rtm_LiveMeasurement'
 *            2013-09-19  visore  ESCAN00070505 No changes here
 *            2013-09-19  visore  ESCAN00070506 Compiler error: "error C2159: more than one storage class specified" if STATIC is defined as "static"
 *            2013-09-23  visore  ESCAN00070619 Compiler warning: warning C4127: conditional expression is constant
 *            2013-09-23  visore  ESCAN00070621 DET error RTM_E_UNBALANCED is thrown if Rtm_Stop() is called before Rtm_Start()
 *  01.01.00  2013-11-26  visore  ESCAN00070889 AR4-372: Support Measurement of Net Runtimes
 *  01.01.01  2014-05-21  visore  ESCAN00075552 No changes here
 *            2014-05-21  visore  ESCAN00075805 No changes here
 *            2014-05-21  visore  ESCAN00075806 RTM_NET_RUNTIME_PAUSE_SUPPORT  is not filtered by ORGANi in in Rtm.h
 *  01.01.02  2014-07-07  visore  ESCAN00076777 No changes here
 *            2014-07-07  visore  ESCAN00076778 No changes here
 *            2014-07-07  visore  ESCAN00076779 No changes here
 *  01.02.00  2014-07-08  viszda  ESCAN00076464 AR4-817: Runtime optimization of the RTM measurement functions
 *            2014-07-09  viszda  ESCAN00076861 Duplicated external Array declaration in Rtm.h
 *            2014-07-23  viszda  ESCAN00077300 MISRA deviation: MISRA-C:2004 Rule 8.8: Duplicated declaration
 *            2014-07-29  viszda  ESCAN00077455 No changes here
 *            2014-07-29  viszda  ESCAN00077449 No changes here
 *            2014-07-29  viszda  ESCAN00076463 AR4-791: Serial and parallel measurement of CPU load
 *            2014-07-16  viszda  ESCAN00077119 No changes here
 *            2014-10-09  viszda  ESCAN00078841 No changes here
 *            2014-10-24  viszda  ESCAN00079113 No changes here
 *            2014-11-06  viszda  ESCAN00077452 Compiler warning: Duplicated declaration of Rtm_MainFunction
 *  01.02.01  2014-11-27  viszda  ESCAN00079842 No changes here
 *            2014-11-27  viszda  ESCAN00079844 No changes here
 *  01.02.02  2015-06-25  viszda  ESCAN00080430 No changes here
 *            2015-07-01  viszda  ESCAN00083746 No changes here
 *  02.00.00  2015-10-01  viszda  ESCAN00085574 FEAT-921: Multicore support for RTM Measurement Points [AR4-816]
 *            2015-10-01  viszda  ESCAN00083646 Support of 32bit timer
 *            2015-10-01  viszda  ESCAN00085572 Optimization of Net Runtime implementation
 *  2.00.01   2016-02-25  viszda  ESCAN00087879 DET occurs during start up of RTM
 *  2.00.02   2016-04-05  viszda  ESCAN00089228 No changes here
 *  2.01.00   2016-04-15  viszda  ESCAN00089508 FEAT-1846: RTM cleanup and improvements
 *            2016-05-17  viszda  ESCAN00089265 No changes here
 *            2016-04-07  viszda  ESCAN00090816 No changes here
 *  2.02.00   2016-09-14  viszda  ESCAN00091859 FEAT-2055: SafeRTM (Disable RTM safely)
 *  3.00.00   2017-04-03  viszda  STORYC-724    [AMD] Support net runtime measurement with RTM in Gen7 OS
 *            2017-04-03  viszda  ESCAN00087435 Rework RTM implementation structure and comments
 *            2017-04-03  viszda  ESCAN00080428 Correction of formal issues
 *            2017-04-05  viszda  ESCAN00093290 Wrong measurement results in case of execution time measurement
 *            2017-04-05  viszda  ESCAN00085578 Add DET checks to RTM's APIs
 *            2017-04-05  viszda  ESCAN00094525 Net runtime measurement deviates to a large extent from real runtime in special cases
 *            2017-04-07  viszda  ESCAN00094694 Add missing initialization of Rtm_DisableInterruptFlag in Rtm_InitMemory
 *  3.01.00   2017-04-19  viszda  STORYC-671    Rtm: reduce RTM measurement overhead.
 *  3.01.01   2017-06-09  viszda  ESCAN00095484 DET occurs before second initialization phase, no measurement possible
 *********************************************************************************************************************/
/* PRQA S 857 EOF */ /* MD_MSR_1.1_857 */

#if !defined (RTM_H)
# define RTM_H

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

# include "Rtm_Types.h"
# include "Rtm_Cfg.h"

# include "Xcp.h"

#if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
#include "Os.h"

# if defined (osdGetTaskIDNotUsed)
#  error "GetTaskId() API is not available but needed for the RTM net runtime measurement!"
# endif
#endif

#if (RTM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define Rtm_Start( measurement )          Rtm_Start_ ## measurement /* PRQA S 0342 */ /* MD_Rtm_0342 */
# define Rtm_Stop( measurement )           Rtm_Stop_ ## measurement /* PRQA S 0342 */ /* MD_Rtm_0342 */

/* ##V_CFG_MANAGEMENT ##CQProject : Monitoring_RuntimeMeasurement CQComponent : Implementation */
# define RTM_VERSION                       (0x0301u)
# define RTM_RELEASE_VERSION               (0x01u)

/* vendor and module identification */
# define RTM_VENDOR_ID                     (30u)
# define RTM_MODULE_ID                     (255u)

# define RTM_AR_RELEASE_MAJOR_VERSION      4u
# define RTM_AR_RELEASE_MINOR_VERSION      0u
# define RTM_AR_RELEASE_REVISION_VERSION   1u

/* Component Version Information */
# define RTM_SW_MAJOR_VERSION              (RTM_VERSION >> 8u)
# define RTM_SW_MINOR_VERSION              (RTM_VERSION & 0x00FF)
# define RTM_SW_PATCH_VERSION              (RTM_RELEASE_VERSION)

/* Development Error Detection */
# define RTM_E_NO_ERROR                    ((uint8)0x00u)
# define RTM_E_UNINIT                      ((uint8)0x01u)
# define RTM_E_WRONG_PARAMETERS            ((uint8)0x02u)
# define RTM_E_INVALID_CONFIGURATION       ((uint8)0x03u)
# define RTM_E_UNBALANCED                  ((uint8)0x04u)

/* API ID */
# define RTM_SID_CPULOADMEASUREMENT        ((uint8)0x01u)
# define RTM_SID_GETVERSIONINFO            ((uint8)0x02u)
# define RTM_SID_INIT                      ((uint8)0x03u)
# define RTM_SID_INITMEMORY                ((uint8)0x04u)
# define RTM_SID_MAINFUNCTION              ((uint8)0x05u)
# define RTM_SID_STARTMEASUREMENT          ((uint8)0x06u)
# define RTM_SID_STOPMEASUREMENT           ((uint8)0x07u)
# define RTM_SID_GETMEASUREMENTITEM        ((uint8)0x08u) /* PRQA S 0857 */ /* MD_MSR_1.1_857 */
# define RTM_SID_ENTERTASKFCT              ((uint8)0x09u)
# define RTM_SID_LEAVETASKFCT              ((uint8)0x0Au)
# define RTM_SID_ENTERISRFCT               ((uint8)0x0Bu)
# define RTM_SID_LEAVEISRFCT               ((uint8)0x0Cu)
# define RTM_SID_SCHEDULE                  ((uint8)0x0Du)
# define RTM_SID_STARTNETMEASUREMENT       ((uint8)0x0Eu)
# define RTM_SID_STOPNETMEASUREMENT        ((uint8)0x0Fu)
# define RTM_SID_SETISTIMERAVAILABLE       ((uint8)0x10u)

# define RTM_TRUE                          (0x01u)
# define RTM_FALSE                         (0x00u)

# define RTM_UNINITIALIZED                 (0x00u)
# define RTM_INITIALIZED                   (0x01u)

/* Rtm_ControlState */
#define RTM_CONTROL_STATE_DISABLED         (0x00u)
#define RTM_CONTROL_STATE_ENABLED          (0x01u)

/* ESCAN00076777 */
/* Rtm_Cmd */
#define RTM_NULL                           (0x00u)
#define RTM_CMD_START_SERIAL_MEASUREMENT   (0x01u)
#define RTM_CMD_START_PARALLEL_MEASUREMENT (0x02u)
#define RTM_CMD_START_LIVE_MEASUREMENT     (0x03u)
#define RTM_CMD_STOP_MEASUREMENT           (0x04u)
#define RTM_CMD_CLEAR_RESULTS              (0x05u)
#define RTM_RESP_OK                        (0x06u)

#if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
#define RTM_INACTIVE                       (0xFFFFFFFFUL)
#define RTM_PAUSED_BY_TASK                 (0xFFFFFFFEUL)
#define RTM_PAUSED_BY_ISR                  (0xFFFFFFFDUL)
#define RTM_PAUSED_BY_MEASUREMENT          ((uint8)0x02u)
#define RTM_PAUSED_BY_USER                 ((uint8)0x08u)
#endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON) */

#define RTM_MP_TYPE_RESPONSETIME            (0x00u)
#define RTM_MP_TYPE_EXECUTIONTIME_NONNESTED (0x01u)
#define RTM_MP_TYPE_EXECUTIONTIME_NESTED    (0x02u)

/* Keyword macros */
#if !defined (STATIC)                 /* COV_RTM_MSR_COMPATIBILITY */
# define STATIC static
#endif

#if !defined (LOCAL_INLINE)           /* COV_RTM_MSR_COMPATIBILITY */
# define LOCAL_INLINE INLINE STATIC
#endif

#if !defined (RTM_LOCAL)              /* COV_RTM_MSR_COMPATIBILITY */
# define RTM_LOCAL STATIC
#endif

#if !defined (RTM_LOCAL_INLINE)       /* COV_RTM_MSR_COMPATIBILITY */
# define RTM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# if !defined (RTM_DUMMY_STATEMENT)     /* COV_RTM_MSR_COMPATIBILITY */
#  if defined (RTM_USE_DUMMY_STATEMENT)  
# define RTM_DUMMY_STATEMENT(statement) (void)(statement) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  else
# define RTM_DUMMY_STATEMENT(statement)
#  endif
# endif

# if !defined (RTM_DUMMY_STATEMENT_CONST)     /* COV_RTM_MSR_COMPATIBILITY */
#  if defined (RTM_USE_DUMMY_STATEMENT_CONST)  
# define RTM_DUMMY_STATEMENT_CONST(statement) (void)(statement) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  else
# define RTM_DUMMY_STATEMENT_CONST(statement)
#  endif
# endif

#if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
# ifndef OS_VTH_SCHEDULE
#define OS_VTH_SCHEDULE(FromThreadId, FromThreadReason, ToThreadId, ToThreadReason, CallerCoreId) { \
   Rtm_Schedule( (uint32)(FromThreadId), (uint32)(ToThreadId), (uint8)(CallerCoreId) );             \
 } /* PRQA S 3458 */ /* MD_MSR_19.4 */
# endif

# if defined OS_TRACE_INVALID_THREAD
#  if (OS_TRACE_INVALID_THREAD != RTM_NUMBER_OF_THREADS)
#   error "The number of OS threads does not match the number specified by RTM!"
#  endif
# endif
#endif

#if (RTM_DEV_ERROR_REPORT == STD_ON)
# if !defined Rtm_DetReportError
# define Rtm_DetReportError(api, err)                              (void)Det_ReportError( RTM_MODULE_ID, 0, (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif
#else
# if !defined Rtm_DetReportError
# define Rtm_DetReportError(api, err)
# endif
#endif /* (RTM_DEV_ERROR_DETECT == STD_ON) */

# define RTM_UP                                                       (0x1u)
# define RTM_DOWN                                                     (0x2u)

# define Rtm_ConvertTicksToUs( ticks )                                ((ticks)/(RTM_TICKS_PER_MILISECOND/1000u)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

# if (RTM_TIME_MEASUREMENT == STD_OFF) /* ESCAN00076464 */
# define Rtm_CpuLoadMeasurementFunction()
# define Rtm_GetVersionInfo( versionInfoPtr )
# define Rtm_Init()
# define Rtm_StartMeasurementFct(id)
# define Rtm_StopMeasurementFct(id)
#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
# define Rtm_StartNetMeasurementFct(id)
# define Rtm_StopNetMeasurementFct(id)
#  endif
# endif /* RTM_TIME_MEASUREMENT == STD_OFF */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define RTM_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (RTM_CONTROL == STD_ON)
extern VAR(uint8, RTM_VAR_ZERO_INIT)                Rtm_ControlDeniedCount;
# endif
# if (RTM_TIME_MEASUREMENT == STD_ON)
extern VAR(uint8, RTM_VAR_ZERO_INIT)                Rtm_InitState; /* ESCAN00068379 */
# endif
#  if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
extern VAR(uint8,  RTM_VAR_ZERO_INIT)               Rtm_TimerOverrunCnt;
#  endif

# define RTM_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# define RTM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
extern VAR(Rtm_MeasurementTimestampType, RTM_VAR_ZERO_INIT) Rtm_PrevTimerValue;
#  endif

# define RTM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (RTM_CONTROL == STD_ON)
# define RTM_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

extern VAR(uint8, RTM_VAR_INIT)                     Rtm_ControlState;

# define RTM_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */
# endif

# if (RTM_TIME_MEASUREMENT == STD_ON)
# define RTM_START_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  if (RTM_NESTING_COUNTER == STD_ON) || (RTM_DEV_ERROR_DETECT == STD_ON)
extern VAR(sint8, RTM_VAR_INIT)                     Rtm_MeasurementNestingCtr[RTM_MEAS_SECTION_COUNT];
#  endif

extern VAR(uint8, RTM_VAR_INIT)                     Rtm_Ctrl[RTM_CTRL_VECTOR_LEN];
extern VAR(uint8, RTM_VAR_INIT)                     Rtm_MeasurementConfig[RTM_CTRL_VECTOR_LEN];
extern VAR(Rtm_MeasurementTimestampType, RTM_VAR_INIT) Rtm_StartTimeStamps[RTM_MEAS_SECTION_COUNT];
extern VAR(Rtm_DataSet, RTM_VAR_INIT)               Rtm_Results[RTM_MEAS_SECTION_COUNT];

#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
extern VAR(Rtm_TaskContextInfoType, RTM_VAR_INIT)   Rtm_TaskContext[RTM_NUMBER_OF_TASKS];
extern VAR(Rtm_IsrContextInfoType, RTM_VAR_INIT)    Rtm_IsrContext;
extern VAR(Rtm_ThreadContextInfoType, RTM_VAR_INIT) Rtm_ThreadContext[RTM_NUMBER_OF_THREADS];

extern VAR(uint32, RTM_VAR_INIT)                    Rtm_ActiveMeasurementId;
extern VAR(uint32, RTM_VAR_INIT)                    Rtm_ActiveThreadId;
#  endif

# define RTM_STOP_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# define RTM_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
extern VAR(uint8, RTM_VAR_NOINIT)                   Rtm_PrevOverrunCounterValues[RTM_MEAS_SECTION_COUNT];
extern VAR(uint8,  RTM_VAR_NOINIT)                  Rtm_GetCurrentTimeCnt;
#  endif

extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_Cmd;
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_Resp;
extern VAR(sint32, RTM_VAR_NOINIT)                  Rtm_MeasTimeCorrection;

extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_AverageMainFunctionCycleTime;
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_MainFunctionCounter;

#  if (RTM_MULTICORE_SUPPORT == STD_ON)
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_CpuLoadTime[RTM_NUMBER_OF_CORES];
extern VAR(uint8, RTM_VAR_NOINIT)                   Rtm_XcpState;
extern VAR(uint8, RTM_VAR_NOINIT)                   Rtm_SendNextMainFunction;

#  else
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_CpuLoadTime;
#  endif

#  if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
#   if (RTM_MULTICORE_SUPPORT == STD_ON)
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_CpuLoadLastTicks[RTM_NUMBER_OF_CORES];
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_CpuLoadCollectedTime[RTM_NUMBER_OF_CORES];
extern VAR(uint8, RTM_VAR_NOINIT)                   Rtm_CpuLoadWasStopped[RTM_NUMBER_OF_CORES];

#   else
extern VAR(uint32, RTM_VAR_NOINIT)                  Rtm_CpuLoadCollectedTime;
extern VAR(uint8, RTM_VAR_NOINIT)                   Rtm_CpuLoadWasStopped;

#   endif
#  endif /* (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF) */

#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON) || (RTM_MULTICORE_SUPPORT == STD_ON)
extern VAR(Rtm_MeasurementPointInfoType, RTM_VAR_INIT) Rtm_MeasurementPointInfo[RTM_MEAS_SECTION_COUNT];
#  endif

# define RTM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# define RTM_START_SEC_CONST_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

extern CONST(uint16, RTM_CONST)                     Rtm_ThresholdTimes[RTM_MEAS_SECTION_COUNT];
extern CONST(Rtm_ThresholdCbkFctType, RTM_CONST)    Rtm_ThresholdCbkFctArr[RTM_MEAS_SECTION_COUNT];
extern CONST(uint8, RTM_CONST)                      Rtm_CtrlConfig[RTM_CTRL_VECTOR_LEN];

#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
extern CONST(Rtm_TaskContextInfoType, RTM_CONST)    Rtm_TaskContextInit[RTM_NUMBER_OF_TASKS];
extern CONST(Rtm_IsrContextInfoType, RTM_CONST)     Rtm_IsrContextInit;
extern CONST(Rtm_ThreadContextInfoType, RTM_CONST)  Rtm_ThreadContextInit[RTM_NUMBER_OF_THREADS];
#  endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON)*/

# define RTM_STOP_SEC_CONST_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define RTM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Rtm_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_InitMemory(void);

/**********************************************************************************************************************
 * Rtm_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the Rtm module.
 *  \details     This function initializes the module Rtm. It initializes all variables and sets the module state to
 *               initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         Rtm_InitMemory has been called unless Xcp_ModuleInitialized is initialized by start-up code.
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_Init(void);

/**********************************************************************************************************************
 *  Rtm_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  Versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, RTM_APPL_VAR) Versioninfo);

/* ESCAN00078841 */
/**********************************************************************************************************************
 * Rtm_CheckTimerOverrun()
 *********************************************************************************************************************/
/*! \brief       This function handles timer overruns.
 *  \details     It must be called cyclically with a higher frequency than the used timer.
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         Should be called if RTM_TIMER_OVERRUN_SUPPORT is STD_ON.
 *  \ingroup     timer
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_CheckTimerOverrun(void);

/* ESCAN00069629 */
#  if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
/**********************************************************************************************************************
 * Rtm_CpuLoadMeasurementFunction()
 *********************************************************************************************************************/
/*! \brief       This function calculates the MCUs overall CPU-Load.
 *  \details     In multi core systems the overall CPU load of all cores can be calculated.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF.
 *  \pre         -
 *  \ingroup     cpuLoad
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_CpuLoadMeasurementFunction(void);

#   if (RTM_MULTICORE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_InitCpuLoadMeasurement()
 *********************************************************************************************************************/
/*! \brief       This function initializes RTMs overall CPU load measurement.
 *  \details     -
 *  \param[in]   coreId                  The core on which the CPU load data should be initialized.
 *  \param[in]   cpuMeasId               The ID of the core specific CPU load MP.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF and 
 *               RTM_MULTICORE_SUPPORT == STD_ON.
 *  \pre         -
 *  \ingroup     cpuLoad
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_InitCpuLoadMeasurement(uint16 coreId, uint32 cpuMeasId);
#   endif /* (RTM_MULTICORE_SUPPORT == STD_ON) */
#  endif /* RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF */

/**********************************************************************************************************************
 * Rtm_GetMeasurementItem()
 *********************************************************************************************************************/
/*! \brief       Returns the CPU load measurement results.
 *  \details     Returns average CPU load, current CPU load, min or max CPU load.
 *  \param[in]   FromThreadId            The thread which is preempted/terminated.
 *  \param[in]   ToThreadId              The thread which is entered (now running).
 *  \param[in]   CoreId                  The core on which the scheduling is performed.
 *  \return      FALSE                   The request was failed.
 *  \return      TRUE                    The request was succeeded.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         Only the CPU load measurement section is allowed to be requested. Only returns valid result if 
 *               RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF.
 *  \ingroup     cpuLoad
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, RTM_CODE) Rtm_GetMeasurementItem( CONST(uint32, AUTOMATIC) measurementId, 
                                                              CONST(Rtm_ItemType, AUTOMATIC) itemType, 
                                                              P2VAR(uint32, AUTOMATIC, RTM_APPL_VAR) itemValuePtr);

/**********************************************************************************************************************
 * Rtm_StartMeasurementFct()
 *********************************************************************************************************************/
/*! \brief       Starts runtime measurement for the given measurement section.
 *  \details     -
 *  \param[in]   measurementId           ID of the measurement section.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         Runtime measurement was started globally and this measurement section is active.
 *  \ingroup     runtime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_StartMeasurementFct(uint32 measurementId); /* ESCAN00076464 */

/**********************************************************************************************************************
 * Rtm_StopMeasurementFct()
 *********************************************************************************************************************/
/*! \brief       Stops runtime measurement for the given measurement section.
 *  \details     This function calculates the runtime for the given measurement section.
 *  \param[in]   measurementId           ID of the measurement section.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         Runtime measurement was started globally and this measurement section is active.
 *  \ingroup     runtime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_StopMeasurementFct(uint32 measurementId); /* ESCAN00076464 */

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_StartNetMeasurementFct()
 *********************************************************************************************************************/
/*! \brief       Starts net runtime measurement for the given measurement section.
 *  \details     -
 *  \param[in]   measurementId           ID of the measurement section.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Runtime measurement was started globally and this measurement section is active.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_StartNetMeasurementFct(uint32 measurementId); /* ESCAN00076464 */

/**********************************************************************************************************************
 * Rtm_StopNetMeasurementFct()
 *********************************************************************************************************************/
/*! \brief       Stops net runtime measurement for the given measurement section.
 *  \details     This function calculates the net runtime for the given measurement section.
 *  \param[in]   measurementId           ID of the measurement section.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Runtime measurement was started globally and this measurement section is active.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_StopNetMeasurementFct(uint32 measurementId); /* ESCAN00076464 */

/**********************************************************************************************************************
 * Rtm_EnterTaskFct()
 *********************************************************************************************************************/
/*! \brief       Starts a preempted net measurement section.
 *  \details     Was a net measurement section active on the entered task this measurement section is reactivated.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must be called by PreTaskHook of OS.
 *  \note        Must not be called if Rtm_Schedule is used.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_EnterTaskFct(void);

/**********************************************************************************************************************
 * Rtm_LeaveTaskFct()
 *********************************************************************************************************************/
/*! \brief       Preempts an active net measurement section.
 *  \details     Is a net measurement section active on the left task this measurement section is preempted.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must be called by PostTaskHook of OS.
 *  \note        Must not be called if Rtm_Schedule is used.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_LeaveTaskFct(void);

/**********************************************************************************************************************
 * Rtm_EnterIsrFct()
 *********************************************************************************************************************/
/*! \brief       Starts a preempted net measurement section.
 *  \details     Was a net measurement section active on the entered ISR this measurement section is reactivated.
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must be called by PreISRHook of OS.
 *  \note        Must not be called if Rtm_Schedule is used.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_EnterIsrFct(void);

/**********************************************************************************************************************
 * Rtm_LeaveIsrFct()
 *********************************************************************************************************************/
/*! \brief       Preempts an active net measurement section.
 *  \details     Is a net measurement section active on the left ISR this measurement section is preempted.
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must be called by PostISRHook of OS.
 *  \note        Must not be called if Rtm_Schedule is used.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_LeaveIsrFct(void);

/**********************************************************************************************************************
 * Rtm_Schedule()
 *********************************************************************************************************************/
/*! \brief       Handles a scheduling event of the OS.
 *  \details     -
 *  \param[in]   FromThreadId            The thread which is preempted/terminated.
 *  \param[in]   ToThreadId              The thread which is entered (now running).
 *  \param[in]   CoreId                  The core on which the scheduling is performed.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must not be called if the APIs Rtm_EnterTaskFct/Rtm_LeaveTaskFct/Rtm_EnterIsrFctRtm_LeaveIsrFct
 *               are used.
 *  \note        Should be called by the TimingHook OS_VTH_SCHEDULE.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
extern FUNC(void, RTM_CODE) Rtm_Schedule( uint32 FromThreadId,
                                          uint32 ToThreadId,
                                          uint16 CoreId );
#  endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  Rtm_MainFunction()
 *********************************************************************************************************************/
/*! \fn          void Rtm_MainFunction(void)
 *  \brief       Cyclically executed API service of the Rtm.
 *  \details     Interprets commands from Rtm user, transmits runtime measurement results and control variables via 
 *               XCP, calculates CPU load measurement results.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
/* extern FUNC(void, RTM_CODE) Rtm_MainFunction(void); */

# define RTM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# else /* RTM_TIME_MEASUREMENT == STD_ON */
# define RTM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

extern FUNC(void, RTM_CODE) Rtm_InitMemory(void);

# define RTM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# endif /* RTM_TIME_MEASUREMENT == STD_ON */

#endif /* RTM_H */

/**********************************************************************************************************************
 *  END OF FILE: Rtm.h
 *********************************************************************************************************************/

