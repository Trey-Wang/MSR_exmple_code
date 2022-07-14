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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Rtm.c
 *      Project:  -
 *       Module:  RTM
 *    Generator:  -
 *
 *  Description:  This file provides functions and data for runtime measurements in the scope of RTM
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
 *            2013-08-06  visore  ESCAN00069628 Missing DET check for invalid disableInterrupts parameter in Rtm_StartMeasurementFct 
 -                                              and Rtm_StopMeasurementFct
 *            2013-08-06  visore  ESCAN00069629 No changes here
 *            2013-09-19  visore  ESCAN00070505 AR4-220: redefine STATIC to static
 *            2013-09-19  visore  ESCAN00070506 No changes here
 *            2013-09-23  visore  ESCAN00070619 No changes here
 *            2013-09-23  visore  ESCAN00070621 No changes here
 *  01.01.00  2013-11-26  visore  ESCAN00070889 AR4-372: Support Measurement of Net Runtimes
 *  01.01.01  2014-05-21  visore  ESCAN00075552 Remove TODO Sections
 *            2014-05-21  visore  ESCAN00075805 Compiler warning: Rtm_GetFreeMeasurementObject(void) is called with parameter
 *            2014-05-21  visore  ESCAN00075806 No changes here
 *  01.01.02  2014-07-07  visore  ESCAN00076777 Compiler warning: ["../Source/../AMD/BSW/Rtm/Rtm.c" 758/40] invalid shift count
 *            2014-07-07  visore  ESCAN00076778 Measured minimum and maximum run times are incorrect for measurements with measurement count > 1
 *            2014-07-07  visore  ESCAN00076779 Remove the critical section from Rtm_EnterIsrFct() and Rtm_LeaveIsrFct()
 *  01.02.00  2014-07-08  viszda  ESCAN00076464 AR4-817: Runtime optimization of the RTM measurement functions
 *            2014-07-09  viszda  ESCAN00076861 No changes here
 *            2014-07-23  viszda  ESCAN00077300 No changes here
 *            2014-07-29  viszda  ESCAN00077455 Type cast from 'int' to 'Rtm_MeasurementTimestampType'
 *            2014-07-29  viszda  ESCAN00077449 Add initialization of Rtm_Ctrl in Rtm_InitMemory
 *            2014-07-29  viszda  ESCAN00076463 AR4-791: Serial and parallel measurement of CPU load
 *            2014-07-29  viszda  ESCAN00077119 The measured maximum run times are in correct in case net runtime support is enabled
 *            2014-10-20  viszda  ESCAN00078841 Calculated CPU Load is wrong
 *            2014-10-24  viszda  ESCAN00079113 Overall CPU load measurement results can be higher than 100%
 *            2014-11-06  viszda  ESCAN00077452 No changes here
 *  01.02.01  2014-11-27  viszda  ESCAN00079842 The CPU load is always 100% in CPU load control mode C_API
 *            2014-11-27  viszda  ESCAN00079844 The current result of CPU load measurement should be available for CPU load control mode C_API
 *  01.02.02  2015-06-25  viszda  ESCAN00080430 Inline function not encapsulated in RTM_START_SEC_CODE
 *            2015-07-01  viszda  ESCAN00083746 Implausible measurement results for min and max values
 *  02.00.00  2015-10-01  viszda  ESCAN00085574 FEAT-921: Multicore support for RTM Measurement Points [AR4-816]
 *            2015-10-01  viszda  ESCAN00083646 Support of 32bit timer
 *            2015-10-01  viszda  ESCAN00085572 Optimization of Net Runtime implementation
 *  2.00.01   2016-02-25  viszda  ESCAN00087879 DET occurs during start up of RTM
 *  2.00.02   2016-04-05  viszda  ESCAN00089228 No changes here
 *  2.01.00   2016-04-15  viszda  ESCAN00089508 FEAT-1846: RTM cleanup and improvements
 *            2016-05-17  viszda  ESCAN00089265 API based RTM control in CANoe
 *            2016-04-07  viszda  ESCAN00090816 Implausible measurement results due to not stopped measurement points
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

#define RTM_SOURCE
/* PRQA S 857 EOF */ /* MD_MSR_1.1_857 */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Rtm.h"

/*lint -e438 */ /* Suppress ID438 because value assigned to variable is not required */
/*lint -e550 */ /* Suppress ID550 because symbol is not required */

#if (RTM_MULTICORE_SUPPORT == STD_ON)
# include "Os.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* vendor specific version information is BCD coded */
#if (  (RTM_SW_MAJOR_VERSION != (0x03u)) \
    || (RTM_SW_MINOR_VERSION != (0x01u)) \
    || (RTM_SW_PATCH_VERSION != (0x01u)) )
# error "Vendor specific version numbers of Rtm.c and Rtm.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

#if (RTM_CTR_DIRECTION == RTM_UP)
# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
# define Rtm_GetCounterDelta(START_VAL, END_VAL, RESULT_PTR, MEAS_ID) ((END_VAL) >= (START_VAL)) ?                                                                    \
                                                                      (*(RESULT_PTR) = (( (uint32)                                                                    \
                                                                                              ((Rtm_TimerOverrunCnt >= Rtm_PrevOverrunCounterValues[MEAS_ID]) ?       \
                                                                                               (Rtm_TimerOverrunCnt - Rtm_PrevOverrunCounterValues[MEAS_ID])  :       \
                                                                                               ((256 - Rtm_PrevOverrunCounterValues[MEAS_ID]) + Rtm_TimerOverrunCnt)  \
                                                                                              ) * RTM_MEASUREMENT_MAX_VAL) + (END_VAL)) - (START_VAL)) :              \
                                                                      (*(RESULT_PTR) = (( (uint32)                                                                    \
                                                                                              ((Rtm_TimerOverrunCnt > Rtm_PrevOverrunCounterValues[MEAS_ID]) ?        \
                                                                                               (Rtm_TimerOverrunCnt - Rtm_PrevOverrunCounterValues[MEAS_ID])  :       \
                                                                                               ((Rtm_TimerOverrunCnt == Rtm_PrevOverrunCounterValues[MEAS_ID]) ? 1 :  \
                                                                                               ((256 - Rtm_PrevOverrunCounterValues[MEAS_ID]) + Rtm_TimerOverrunCnt)) \
                                                                                              ) * RTM_MEASUREMENT_MAX_VAL) - (START_VAL)) + (END_VAL)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# else /* (RTM_TIMER_OVERRUN_SUPPORT == STD_ON) */
# define Rtm_GetCounterDelta(START_VAL, END_VAL, RESULT_PTR, MEAS_ID) ((END_VAL) >= (START_VAL)) ?                                \
                                                                      (*(RESULT_PTR) = (END_VAL) - (START_VAL)) :                 \
                                                                      (*(RESULT_PTR) = ((RTM_MEASUREMENT_MAX_VAL - (START_VAL)) + (END_VAL))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif /* (RTM_TIMER_OVERRUN_SUPPORT == STD_ON) */

#else /* (RTM_CTR_DIRECTION == RTM_DOWN) */
# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
# define Rtm_GetCounterDelta(START_VAL, END_VAL, RESULT_PTR, MEAS_ID) ((END_VAL) <= (START_VAL)) ?                                                                    \
                                                                      (*(RESULT_PTR) = (( (uint32)                                                                    \
                                                                                              ((Rtm_TimerOverrunCnt >= Rtm_PrevOverrunCounterValues[MEAS_ID]) ?       \
                                                                                               (Rtm_TimerOverrunCnt - Rtm_PrevOverrunCounterValues[MEAS_ID])  :       \
                                                                                               ((256 - Rtm_PrevOverrunCounterValues[MEAS_ID]) + Rtm_TimerOverrunCnt)  \
                                                                                              ) * RTM_MEASUREMENT_MAX_VAL) + (START_VAL)) - (END_VAL)) :              \
                                                                      (*(RESULT_PTR) = (( (uint32)                                                                    \
                                                                                              ((Rtm_TimerOverrunCnt > Rtm_PrevOverrunCounterValues[MEAS_ID]) ?        \
                                                                                               (Rtm_TimerOverrunCnt - Rtm_PrevOverrunCounterValues[MEAS_ID])  :       \
                                                                                               ((Rtm_TimerOverrunCnt == Rtm_PrevOverrunCounterValues[MEAS_ID]) ? 1 :  \
                                                                                               ((256 - Rtm_PrevOverrunCounterValues[MEAS_ID]) + Rtm_TimerOverrunCnt)) \
                                                                                              ) * RTM_MEASUREMENT_MAX_VAL) - (END_VAL)) + (START_VAL)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# else /* (RTM_TIMER_OVERRUN_SUPPORT == STD_ON) */
# define Rtm_GetCounterDelta(START_VAL, END_VAL, RESULT_PTR, MEAS_ID) ((END_VAL) <= (START_VAL)) ?                \
                                                                      (*(RESULT_PTR) = (START_VAL) - (END_VAL)) : \
                                                                      (*(RESULT_PTR) = ((RTM_MEASUREMENT_MAX_VAL - (END_VAL)) + (START_VAL))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif /* (RTM_TIMER_OVERRUN_SUPPORT == STD_ON) */
#endif /* RTM_CTR_DIRECTION == RTM_UP */

#if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
# if (RTM_CTR_DIRECTION == RTM_UP)
#define RTM_GET_CURRENT_TIME(time) {                                          \
                                     Rtm_GetCurrentTimeCnt++;                 \
                                     (time) = RTM_GET_TIME_MEASUREMENT_FCT(); \
                                     if (Rtm_GetCurrentTimeCnt == 1) {        \
                                       if ((time)<Rtm_PrevTimerValue) {       \
                                          Rtm_TimerOverrunCnt++;              \
                                       }                                      \
                                       Rtm_PrevTimerValue = (time);           \
                                     }                                        \
                                     Rtm_GetCurrentTimeCnt--;                 \
                                   } /* PRQA S 3453,3458 */ /* MD_MSR_19.7,MD_MSR_19.4 */

# else /* (RTM_CTR_DIRECTION == RTM_DOWN) */

/* ESCAN00079227 */
#define RTM_GET_CURRENT_TIME(time) {                                          \
                                     Rtm_GetCurrentTimeCnt++;                 \
                                     (time) = RTM_GET_TIME_MEASUREMENT_FCT(); \
                                     if (Rtm_GetCurrentTimeCnt == 1) {        \
                                       if ((time)>Rtm_PrevTimerValue) {       \
                                          Rtm_TimerOverrunCnt++;              \
                                       }                                      \
                                       Rtm_PrevTimerValue = (time);           \
                                     }                                        \
                                     Rtm_GetCurrentTimeCnt--;                 \
                                   } /* PRQA S 3453,3458 */ /* MD_MSR_19.7,MD_MSR_19.4 */
# endif /* (RTM_CTR_DIRECTION == RTM_UP) */

#else /* (RTM_TIMER_OVERRUN_SUPPORT == STD_ON) */
#define RTM_GET_CURRENT_TIME(time) (time) = RTM_GET_TIME_MEASUREMENT_FCT() /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif /* (RTM_TIMER_OVERRUN_SUPPORT == STD_ON) */

#if (RTM_USE_32BIT_TIMER == STD_ON) && (RTM_MULTICORE_SUPPORT == STD_ON)
#define Rtm_FixTypeMismatch(word) (word) = ((word) >> 1) /* PRQA S 3410,3453  */  /* MD_Rtm_3410,MD_MSR_19.7 */
#else
#define Rtm_FixTypeMismatch(word)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

# define RTM_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (RTM_CONTROL == STD_ON)
VAR(uint8, RTM_VAR_INIT)                          Rtm_ControlState = RTM_CONTROL_STATE_ENABLED; /* = 1 */
# endif

# define RTM_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# define RTM_START_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
VAR(uint32, RTM_VAR_INIT)                         Rtm_ActiveThreadId = RTM_NUMBER_OF_THREADS;
# endif

# define RTM_STOP_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# define RTM_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (RTM_CONTROL == STD_ON)
VAR(uint8, RTM_VAR_ZERO_INIT)                     Rtm_ControlDeniedCount = 0; /*lint -e552 */ /* Suppress ID552 */
# endif

#if (RTM_TIME_MEASUREMENT == STD_ON)
VAR(uint8, RTM_VAR_ZERO_INIT)                     Rtm_InitState = RTM_UNINITIALIZED; /* = 0 */ /*lint -e552 */ /* Suppress ID552 */
#endif

# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
VAR(uint8, RTM_VAR_ZERO_INIT)                     Rtm_TimerOverrunCnt = 0;
# endif

# define RTM_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# define RTM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
RTM_LOCAL VAR(Rtm_OverheadCorrectionType, RTM_VAR_INIT) Rtm_OverheadCorrection = { 0 }; /* PRQA S 3408 */  /* MD_Rtm_3408 */ /*lint -e552 */ /* Suppress ID552 */
# endif

# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
VAR(Rtm_MeasurementTimestampType, RTM_VAR_ZERO_INIT) Rtm_PrevTimerValue = 0;
# endif

# define RTM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#if (RTM_TIME_MEASUREMENT == STD_ON)
# define RTM_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

RTM_LOCAL VAR(uint32, RTM_VAR_NOINIT)             Rtm_LastCommandToEcu; /* PRQA S 3408 */  /* MD_Rtm_3408 */
RTM_LOCAL VAR(Rtm_TimestampType, RTM_VAR_NOINIT)  Rtm_MeasurementTimer; /* PRQA S 3408 */  /* MD_Rtm_3408 */
RTM_LOCAL VAR(Rtm_TimestampType, RTM_VAR_NOINIT)  Rtm_CalculatedCtrEndVal; /* PRQA S 3408 */  /* MD_Rtm_3408 */

# if (RTM_MULTICORE_SUPPORT == STD_ON)
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_XcpState; /*lint -e552 */ /* Suppress ID552 */
# else
RTM_LOCAL VAR(uint8, RTM_VAR_NOINIT)              Rtm_XcpState; /* PRQA S 3408 */  /* MD_Rtm_3408 */
# endif

VAR(uint32, RTM_VAR_NOINIT)                       Rtm_Cmd; /*lint -e552 */ /* Suppress ID552 */
VAR(uint32, RTM_VAR_NOINIT)                       Rtm_Resp; /*lint -e552 */ /* Suppress ID552 */
VAR(sint32, RTM_VAR_NOINIT)                       Rtm_MeasTimeCorrection; /*lint -e552 */ /* Suppress ID552 */

VAR(uint32, RTM_VAR_NOINIT)                       Rtm_AverageMainFunctionCycleTime;
VAR(uint32, RTM_VAR_NOINIT)                       Rtm_MainFunctionCounter;

# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_PrevOverrunCounterValues[RTM_MEAS_SECTION_COUNT];
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_GetCurrentTimeCnt;
# endif

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
RTM_LOCAL VAR(Rtm_TaskType, RTM_VAR_NOINIT)       Rtm_TaskIndex[RTM_NUMBER_OF_TASKS]; /* PRQA S 3408 */  /* MD_Rtm_3408 */
# endif

# if (RTM_MULTICORE_SUPPORT == STD_ON)
VAR(uint32, RTM_VAR_NOINIT)                       Rtm_CpuLoadTime[RTM_NUMBER_OF_CORES]; /*lint -e552 */ /* Suppress ID552 */
VAR(uint16, RTM_VAR_NOINIT)                       Rtm_MasterCore;
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_SendNextMainFunction;

# else
RTM_LOCAL VAR(uint8, RTM_VAR_NOINIT)              Rtm_LiveMeasurement; /* PRQA S 3408 */  /* MD_Rtm_3408 */
VAR(uint32, RTM_VAR_NOINIT)                       Rtm_CpuLoadTime;
# endif

RTM_LOCAL VAR(uint8, RTM_VAR_NOINIT)              Rtm_EndlessMeasurement; /* PRQA S 3408 */  /* MD_Rtm_3408 */
RTM_LOCAL VAR(uint32, RTM_VAR_NOINIT)             Rtm_LastMainFunctionTime; /* PRQA S 3408 */  /* MD_Rtm_3408 */
RTM_LOCAL VAR(uint32, RTM_VAR_NOINIT)             Rtm_AvgMainFunctionCycleTimeCalcCounter; /* PRQA S 3408 */  /* MD_Rtm_3408 */

# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
VAR(uint32, RTM_VAR_NOINIT)                       Rtm_CpuLoadLastTicks[RTM_NUMBER_OF_CORES];
RTM_LOCAL VAR(Rtm_MeasurementTimestampType, RTM_VAR_NOINIT) Rtm_CpuLoadStartTimestamp[RTM_NUMBER_OF_CORES]; /* PRQA S 3408 */  /* MD_Rtm_3408 */
RTM_LOCAL VAR(uint32, RTM_VAR_NOINIT)             Rtm_CpuLoadOverhead[RTM_NUMBER_OF_CORES]; /* PRQA S 3408 */  /* MD_Rtm_3408 */

VAR(uint32, RTM_VAR_NOINIT)                       Rtm_CpuLoadCollectedTime[RTM_NUMBER_OF_CORES];
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_CpuLoadMeasurementActive[RTM_NUMBER_OF_CORES];
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_CpuLoadWasStopped[RTM_NUMBER_OF_CORES];
#   if defined (RTM_CPU_LOAD_AUTOSTART)
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_CpuLoadSendResult[RTM_NUMBER_OF_CORES];
#   endif
#  else
RTM_LOCAL VAR(uint32, RTM_VAR_NOINIT)             Rtm_CpuLoadLastTicks; /* PRQA S 3408 */  /* MD_Rtm_3408 */ /*lint -e552 */ /* Suppress ID552 */
RTM_LOCAL VAR(Rtm_MeasurementTimestampType, RTM_VAR_NOINIT) Rtm_CpuLoadStartTimestamp; /* PRQA S 3408 */  /* MD_Rtm_3408 */
RTM_LOCAL VAR(uint32, RTM_VAR_NOINIT)             Rtm_CpuLoadOverhead; /* PRQA S 3408 */  /* MD_Rtm_3408 */

VAR(uint32, RTM_VAR_NOINIT)                       Rtm_CpuLoadCollectedTime;
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_CpuLoadMeasurementActive;
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_CpuLoadWasStopped;
#   if defined (RTM_CPU_LOAD_AUTOSTART)
VAR(uint8, RTM_VAR_NOINIT)                        Rtm_CpuLoadSendResult;
#   endif
#  endif
# endif

# define RTM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* ESCAN00080430 */
# define RTM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *   HH      HH  EEEEEEEEEE  LL          PPPPPPPP    EEEEEEEEEE  RRRRRRRR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HHHHHHHHHH  EEEEEEEE    LL          PPPPPPPP    EEEEEEEE    RRRRRRRR
 *   HH      HH  EE          LL          PP          EE          RR    RR
 *   HH      HH  EE          LL          PP          EE          RR      RR
 *   HH      HH  EEEEEEEEEE  LLLLLLLLLL  PP          EEEEEEEEEE  RR      RR
 *********************************************************************************************************************/

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead introduced by RTM's macros Rtm_Start and Rtm_Stop.
 *  \details     Calculates the runtime of all variants of macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead(void);

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_RespTime_EnableISR()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead of MPs measuring response time with interrupts enabled.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 777 2 */ /* MD_MSR_5.1_777 */
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_RespTime_EnableISR(void);

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_RespTime_DisableISR()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead of MPs measuring response time with interrupts disabled.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 777 2 */ /* MD_MSR_5.1_777 */
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_RespTime_DisableISR(void);

#  if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_EnableISR()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead of MPs measuring executing time non-nested with interrupts enabled.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and 
 *               RTM_MULTICORE_SUPPORT == STD_OFF and
 *               RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 777 2 */ /* MD_MSR_5.1_777 */
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_EnableISR(void);

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_DisableISR()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead of MPs measuring executing time non-nested with interrupts disabled.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and 
 *               RTM_MULTICORE_SUPPORT == STD_OFF and
 *               RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 777 2 */ /* MD_MSR_5.1_777 */
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_DisableISR(void);
#  endif

#  if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_Nested_EnableISR()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead of MPs measuring executing time nested with interrupts enabled.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and 
 *               RTM_MULTICORE_SUPPORT == STD_OFF and
 *               RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 777 2 */ /* MD_MSR_5.1_777 */
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_Nested_EnableISR(void);

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_Nested_DisableISR()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead of MPs measuring executing time nested with interrupts disabled.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and 
 *               RTM_MULTICORE_SUPPORT == STD_OFF and
 *               RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 777 2 */ /* MD_MSR_5.1_777 */
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_Nested_DisableISR(void);
#  endif
# endif /* (RTM_MULTICORE_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 * Rtm_Hlp_SetMPStateOfAllMPs()
 *********************************************************************************************************************/
/*! \brief       Sets the MP state of all (requested) MPs.
 *  \details     Disables all MPs or enables all MPs requested by the RTM user.
 *  \param[in]   DisableAllMPs      Decides whether the MPs are disabled or set as RTM user requested.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         -
 *  \ingroup     helper
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_SetMPStateOfAllMPs(boolean DisableAllMPs);

/**********************************************************************************************************************
 *     IIIIII    NN      NN    IIIIII    TTTTTTTTT
 *       II      NNN     NN      II         TT    
 *       II      NNNN    NN      II         TT    
 *       II      NN  NN  NN      II         TT    
 *       II      NN    NNNN      II         TT    
 *       II      NN     NNN      II         TT    
 *     IIIIII    NN      NN    IIIIII       TT    
 *********************************************************************************************************************/

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_Init_TaskContext()
 *********************************************************************************************************************/
/*! \brief       Initializes task context related data.
 *  \details     -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         -
 *  \ingroup     init
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Init_TaskContext(void);
# endif

# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
#  if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_Init_CpuLoadMeasurement()
 *********************************************************************************************************************/
/*! \brief       Calculates the overhead introduced by RTM's macros Rtm_Start and Rtm_Stop.
 *  \details     Calculates the runtime of the macros Rtm_Start and Rtm_Stop.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and
 *               RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF and
 *               RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         -
 *  \ingroup     init
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Init_CpuLoadMeasurement(void);
#  endif
# endif

/**********************************************************************************************************************
 * Rtm_Init_MeasurementData()
 *********************************************************************************************************************/
/*! \brief       Initializes measurement related data.
 *  \details     -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         -
 *  \ingroup     init
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Init_MeasurementData(void);

/**********************************************************************************************************************
 *   MM      MM  EEEEEEEEEE    AAAAAA      SSSSSS              CCCCCC      OOOOOO    NN      NN   TTTTTTTTT
 *   MMM    MMM  EE          AA      AA  SS      SS          CC      CC  OO      OO  NNN     NN      TT    
 *   MMMM  MMMM  EE          AA      AA  SS                  CC          OO      OO  NNNN    NN      TT    
 *   MM  MM  MM  EEEEEEEE    AAAAAAAAAA    SSSSSS    ======  CC          OO      OO  NN  NN  NN      TT    
 *   MM      MM  EE          AA      AA          SS          CC          OO      OO  NN    NNNN      TT    
 *   MM      MM  EE          AA      AA  SS      SS          CC      CC  OO      OO  NN     NNN      TT    
 *   MM      MM  EEEEEEEEEE  AA      AA    SSSSSS              CCCCCC      OOOOOO    NN      NN      TT    
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Rtm_MeasCtr_MeasurementControl()
 *********************************************************************************************************************/
/*! \brief       Controls the measurement.
 *  \details     Starts, stops and proceeds measurements. Clears measurement results. Triggers response.
 *  \param[in]   CurrentTime        The time when this function is called.
 *  \param[in]   TimeToMeasure      Measurement duration.
 *  \param[in]   Command            The command to be executed.
 *  \return      FALSE              RTM data should not be transmitted.
 *  \return      TRUE               RTM data should be transmitted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         -
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 3 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_MeasurementControl(Rtm_MeasurementTimestampType CurrentTime,
                                                                      Rtm_TimestampType TimeToMeasure,
                                                                      uint8 Command);

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_MeasCtr_StartLiveMeasurement()
 *********************************************************************************************************************/
/*! \brief       Starts a live measurement.
 *  \details     Starts a live measurement in time-limited or endless mode.
 *  \param[in]   CurrentTime        The time when this function is called.
 *  \param[in]   TimeToMeasure      Measurement duration.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         -
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 2 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_MeasCtr_StartLiveMeasurement(Rtm_MeasurementTimestampType CurrentTime,
                                                                        Rtm_TimestampType TimeToMeasure);

/**********************************************************************************************************************
 * Rtm_MeasCtr_ProceedLiveMeasurement()
 *********************************************************************************************************************/
/*! \brief       Proceeds with the currently running live measurement.
 *  \details     Proceeds with live measurement in time-limited and endless mode. Triggers response.
 *  \param[in]   CurrentTime        The time when this function is called.
 *  \return      FALSE              RTM data should not be transmitted.
 *  \return      TRUE               RTM data should be transmitted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         A live measurement must be running.
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_ProceedLiveMeasurement(Rtm_MeasurementTimestampType CurrentTime);
# endif

/**********************************************************************************************************************
 * Rtm_MeasCtr_StartSerialOrParallelMeasurement()
 *********************************************************************************************************************/
/*! \brief       Starts a serial or parallel measurement.
 *  \details     Starts a serial or parallel measurement in time-limited or endless mode. Triggers response.
 *  \param[in]   CurrentTime        The time when this function is called.
 *  \param[in]   TimeToMeasure      Measurement duration.
 *  \return      FALSE              RTM data should not be transmitted.
 *  \return      TRUE               RTM data should be transmitted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         -
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 2 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_StartSerialOrParallelMeasurement(Rtm_MeasurementTimestampType CurrentTime,
                                                                                    Rtm_TimestampType TimeToMeasure);

/**********************************************************************************************************************
 * Rtm_MeasCtr_ProceedSerialOrParallelMeasurement()
 *********************************************************************************************************************/
/*! \brief       Proceeds with the currently running serial or parallel measurement.
 *  \details     Proceeds with measurement in time-limited and endless mode. Triggers response.
 *  \param[in]   CurrentTime        The time when this function is called.
 *  \return      FALSE              RTM data should not be transmitted.
 *  \return      TRUE               RTM data should be transmitted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         A serial or parallel measurement must be running.
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_ProceedSerialOrParallelMeasurement(Rtm_MeasurementTimestampType CurrentTime);

/**********************************************************************************************************************
 * Rtm_MeasCtr_StopEndlessMeasurement()
 *********************************************************************************************************************/
/*! \brief       Stops an endless measurement.
 *  \details     Stops an live, serial or parallel measurement in endless mode. Triggers response.
 *  \return      FALSE              RTM data should not be transmitted.
 *  \return      TRUE               RTM data should be transmitted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         An endless measurement must be running.
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_StopEndlessMeasurement(void);

/**********************************************************************************************************************
 * Rtm_MeasCtr_ClearMeasurementResults()
 *********************************************************************************************************************/
/*! \brief       Clears measurement results.
 *  \details     Clears all the latest measurement results.
 *  \return      FALSE              RTM data should not be transmitted.
 *  \return      TRUE               RTM data should be transmitted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON.
 *  \pre         -
 *  \ingroup     measCtr
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_ClearMeasurementResults(void);

/**********************************************************************************************************************
 *     CCCCCC    PPPPPPPP    UU      UU
 *   CC      CC  PP      PP  UU      UU
 *   CC          PP      PP  UU      UU
 *   CC          PPPPPPPP    UU      UU
 *   CC          PP          UU      UU
 *   CC      CC  PP          UU      UU
 *     CCCCCC    PP            UUUUUU
 *********************************************************************************************************************/

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
#  if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
/**********************************************************************************************************************
 * Rtm_Cpu_CalculateCurrentCPULoad()
 *********************************************************************************************************************/
/*! \brief       Calculates current and cumulated measurement results of CPU load.
 *  \details     -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and 
 *               RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF and
 *               RTM_MULTICORE_SUPPORT == STD_OFF.
 *  \pre         -
 *  \ingroup     cpuLoad
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 1 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Cpu_CalculateCurrentCPULoad(void);
#  endif
# endif

/**********************************************************************************************************************
 *   NN      NN  EEEEEEEEEE  TTTTTTTTTT  
 *   NNN     NN  EE              TT      
 *   NNNN    NN  EE              TT      
 *   NN  NN  NN  EEEEEEEE        TT      
 *   NN    NNNN  EE              TT      
 *   NN     NNN  EE              TT      
 *   NN      NN  EEEEEEEEEE      TT      
 *********************************************************************************************************************/

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_Net_LeaveThread()
 *********************************************************************************************************************/
/*! \brief       Stores data of a preempted net measurement point.
 *  \details     If a net measurement section is active on preempted thread, this section is also preempted.
 *  \param[in]   LeaveThread             The thread which is preempted/terminated.
 *  \param[in]   CoreId                  The core on which the scheduling is performed.
 *  \param[in]   CurrentTime             The timestamp when the thread was preempted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must not be called if the APIs Rtm_EnterTaskFct/Rtm_LeaveTaskFct/Rtm_EnterIsrFctRtm_LeaveIsrFct
 *               are used.
 *  \note        Should be called by Rtm_Schedule or an API with equal behavior.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 3 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Net_LeaveThread( uint32 LeaveThread,
                                                           uint16 CoreId,
                                                           Rtm_MeasurementTimestampType CurrentTime );

/**********************************************************************************************************************
 * Rtm_Net_EnterThread()
 *********************************************************************************************************************/
/*! \brief       Preempts a thread and starts another thread.
 *  \details     If a net measurement section is active on preempted thread, this section is also preempted. If a net 
 *               measurement section was preempted on the entered thread, this section is also started.
 *  \param[in]   LeaveThread             The thread which is started (now active).
 *  \param[in]   CoreId                  The core on which the scheduling is performed.
 *  \param[in]   CurrentTime             The timestamp when the thread was preempted.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Available only if RTM_TIME_MEASUREMENT == STD_ON and RTM_NET_RUNTIME_SUPPORT == STD_ON.
 *  \pre         Must not be called if the APIs Rtm_EnterTaskFct/Rtm_LeaveTaskFct/Rtm_EnterIsrFctRtm_LeaveIsrFct
 *               are used.
 *  \note        Should be called by Rtm_Schedule or an API with equal behavior.
 *  \ingroup     netRuntime
 *********************************************************************************************************************/
/* PRQA S 3408, 3447 3 */  /* MD_Rtm_3408 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Net_EnterThread( uint32 EnterThread,
                                                           uint16 CoreId,
                                                           Rtm_MeasurementTimestampType CurrentTime );
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *   HH      HH  EEEEEEEEEE  LL          PPPPPPPP    EEEEEEEEEE  RRRRRRRR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HHHHHHHHHH  EEEEEEEE    LL          PPPPPPPP    EEEEEEEE    RRRRRRRR
 *   HH      HH  EE          LL          PP          EE          RR    RR
 *   HH      HH  EE          LL          PP          EE          RR      RR
 *   HH      HH  EEEEEEEEEE  LLLLLLLLLL  PP          EEEEEEEEEE  RR      RR
 *********************************************************************************************************************/

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead(void)
{
  /* #10 Start and stop the overhead MPs. */
  Rtm_Hlp_CalculateOverhead_RespTime_EnableISR();
  Rtm_Hlp_CalculateOverhead_RespTime_DisableISR();
# if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
  Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_EnableISR();
  Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_DisableISR();
# endif
# if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
  Rtm_Hlp_CalculateOverhead_ExecTime_Nested_EnableISR();
  Rtm_Hlp_CalculateOverhead_ExecTime_Nested_DisableISR();
# endif
} /* Rtm_Hlp_CalculateOverhead */

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_RespTime_EnableISR()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_RespTime_EnableISR(void)
{
  /* #10 Start and stop the overhead MPs for response time with enabled interrupts. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_RespTime_EnableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_RespTime_EnableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
} /* Rtm_Hlp_CalculateOverhead_RespTime_EnableISR */

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_RespTime_DisableISR()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_RespTime_DisableISR(void)
{
  /* #10 Start and stop the overhead MPs for response time with disabled interrupts. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_RespTime_DisableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_RespTime_DisableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
} /* Rtm_Hlp_CalculateOverhead_RespTime_DisableISR */

#  if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_EnableISR()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_EnableISR(void)
{
  /* #10 Start and stop the overhead MPs for execution time non-nested with enabled interrupts. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_NonNested_EnableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_NonNested_EnableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
} /* Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_EnableISR */

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_DisableISR()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_DisableISR(void)
{
  /* #10 Start and stop the overhead MPs for execution time non-nested with disabled interrupts. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_NonNested_DisableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_NonNested_DisableISRs); /* PRQA S 3109 */  /* MD_MSR_14.3 */
} /* Rtm_Hlp_CalculateOverhead_ExecTime_NonNested_DisableISR */
#  endif

#  if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_Nested_EnableISR()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_Nested_EnableISR(void)
{
  Rtm_OverheadCorrection.execTime_Nested_EnableISRs = 0;

  /* #10 Start and stop the overhead MPs for execution time nested with enabled interrupts. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_EnableISRs_Outer); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_EnableISRs_Inner); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_EnableISRs_Inner); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_EnableISRs_Outer); /* PRQA S 3109 */  /* MD_MSR_14.3 */

  /* #20 Calculate the correction value of nested MPs. */
  Rtm_OverheadCorrection.execTime_Nested_EnableISRs = Rtm_Results[RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_EnableISRs_Outer].time
                                                    - Rtm_Results[RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_EnableISRs_Inner].time;
  /* PRQA S 6010 1 */ /* MD_MSR_STPTH */
} /* Rtm_Hlp_CalculateOverhead_ExecTime_Nested_EnableISR */

/**********************************************************************************************************************
 * Rtm_Hlp_CalculateOverhead_ExecTime_Nested_DisableISR()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_CalculateOverhead_ExecTime_Nested_DisableISR(void)
{
  Rtm_OverheadCorrection.execTime_Nested_DisableISRs = 0;

  /* #10 Start and stop the overhead MPs for execution time nested with disabled interrupts. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_DisableISRs_Outer); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_DisableISRs_Inner); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_DisableISRs_Inner); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_DisableISRs_Outer); /* PRQA S 3109 */  /* MD_MSR_14.3 */

  /* #20 Calculate the correction value of nested MPs. */
  Rtm_OverheadCorrection.execTime_Nested_DisableISRs = Rtm_Results[RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_DisableISRs_Outer].time
                                                     - Rtm_Results[RtmConf_RtmMeasurementPoint_Rtm_Overhead_ExecTime_Nested_DisableISRs_Inner].time;
  /* PRQA S 6010 1 */ /* MD_MSR_STPTH */
} /* Rtm_Hlp_CalculateOverhead_ExecTime_Nested_DisableISR */
#  endif
# endif /* (RTM_MULTICORE_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 * Rtm_Hlp_SetMPStateOfAllMPs()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Hlp_SetMPStateOfAllMPs(boolean DisableAllMPs)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint16_least ctrlVecIndex;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If requested disable all MPs. */
  if (DisableAllMPs == RTM_TRUE)
  {
    for (ctrlVecIndex = 0; ctrlVecIndex < RTM_CTRL_VECTOR_LEN; ctrlVecIndex++)
    {
      Rtm_Ctrl[ctrlVecIndex] = (uint8) 0u;
    }
  }
  /* #20 Otherwise, set state of all MPs as requested by RTM user. */
  else
  {
    for (ctrlVecIndex = 0; ctrlVecIndex < RTM_CTRL_VECTOR_LEN; ctrlVecIndex++)
    {
      Rtm_Ctrl[ctrlVecIndex] = Rtm_MeasurementConfig[ctrlVecIndex];
    }
  }
} /* Rtm_Hlp_SetMPStateOfAllMPs */

/**********************************************************************************************************************
 *     IIIIII    NN      NN    IIIIII    TTTTTTTTT
 *       II      NNN     NN      II         TT    
 *       II      NNNN    NN      II         TT    
 *       II      NN  NN  NN      II         TT    
 *       II      NN    NNNN      II         TT    
 *       II      NN     NNN      II         TT    
 *     IIIIII    NN      NN    IIIIII       TT    
 *********************************************************************************************************************/

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_Init_TaskContext()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Init_TaskContext(void)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint16_least index;
  uint32 rtmInactive = RTM_INACTIVE;
  
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Invalidate the task indices. */
  for (index = 0; index < RTM_NUMBER_OF_TASKS; index++)
  {
    Rtm_TaskIndex[index] = (Rtm_TaskType)rtmInactive;
  }

  /* Assign each Rtm_TaskIndex index to the corresponding Rtm_TaskContext:
   *  In Rtm_TaskContext the Task IDs are specified but unsorted.
   *  Rtm_TaskIndex is used to point to Rtm_TaskContext, example:
   *    Rtm_TaskIndex[0] = 3 -> Rtm_TaskContext[3].taskId = 0
   */
  /* #20 Map real unsorted task IDs to local sorted task IDs. */
  for (index = 0; index < RTM_NUMBER_OF_TASKS; index++)
  {
    /* Each task ID must be unique, thus this condition should not fail. */
    if (Rtm_TaskIndex[Rtm_TaskContext[index].taskId] == (Rtm_TaskType)rtmInactive)
    {
      Rtm_TaskIndex[Rtm_TaskContext[index].taskId] = (Rtm_TaskType)index;
    }
  }

#  if (RTM_DEV_ERROR_DETECT == STD_ON)
  /* #30 If there is still an unmapped task index, indicate an error. */
  for (index = 0; index < RTM_NUMBER_OF_TASKS; index++)
  {
    if (Rtm_TaskIndex[index] == (Rtm_TaskType)rtmInactive)
    {
      Rtm_DetReportError(RTM_SID_INIT, RTM_E_INVALID_CONFIGURATION); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    }
  }
#  endif
} /* Rtm_Init_TaskContext */
# endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON) */

# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
#  if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_Init_CpuLoadMeasurement()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Init_CpuLoadMeasurement(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Initialize all CPU load related variables. */
  Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].count = 0x0u;
  Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].time = 0x0u;
  Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].min = 0xFFFFFFFFu;
  Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].max = 0x0u;

  Rtm_CpuLoadWasStopped = RTM_FALSE;
  Rtm_CpuLoadTime = 0;
  Rtm_CpuLoadStartTimestamp = RTM_GET_TIME_MEASUREMENT_FCT();
  Rtm_CpuLoadLastTicks = 0;

  /* #20 Calculate CPU load overhead. */
  Rtm_CpuLoadOverhead      = (uint32) 0u;
  Rtm_CpuLoadCollectedTime = (uint32) 0u;
  Rtm_CpuLoadMeasurementFunction();
  Rtm_CpuLoadCollectedTime = (uint32) 0u;
  Rtm_CpuLoadMeasurementFunction();
  Rtm_CpuLoadOverhead      = Rtm_CpuLoadCollectedTime;
  if (Rtm_CpuLoadOverhead == 0)
  {
    Rtm_CpuLoadOverhead = 1;
  }
} /* Rtm_Init_CpuLoadMeasurement */
#  endif /* (RTM_MULTICORE_SUPPORT == STD_ON) */
# endif /* (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF) */

/**********************************************************************************************************************
 * Rtm_Init_MeasurementData()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Init_MeasurementData(void)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint16_least measurementIndex;
  
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Initialize MP specific data. */
  for (measurementIndex = 0; measurementIndex < RTM_MEAS_SECTION_COUNT; measurementIndex++)
  {
#  if (RTM_NESTING_COUNTER == STD_ON)
    Rtm_MeasurementNestingCtr[measurementIndex] = 0u;
#  endif
#  if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
    Rtm_PrevOverrunCounterValues[measurementIndex] = Rtm_TimerOverrunCnt;
#  endif
#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
    Rtm_MeasurementPointInfo[measurementIndex].intermediateTime = 0;
    Rtm_MeasurementPointInfo[measurementIndex].prevActive = RTM_INACTIVE;
    Rtm_MeasurementPointInfo[measurementIndex].threadContext = RTM_NUMBER_OF_THREADS;
#  endif
  }

#  if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)    
  Rtm_GetCurrentTimeCnt = 0;
#  endif

  /* #20 If net runtime measurement is enabled reset all context specific data. */
#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
  Rtm_ActiveMeasurementId = RTM_INACTIVE;
  
  for (measurementIndex = 0; measurementIndex < RTM_NUMBER_OF_CORES; measurementIndex++)
  {
    Rtm_TaskContext[measurementIndex].intermediateTime = Rtm_TaskContextInit[measurementIndex].intermediateTime;
    Rtm_TaskContext[measurementIndex].prevActive = Rtm_TaskContextInit[measurementIndex].prevActive;
    Rtm_TaskContext[measurementIndex].startTime = Rtm_TaskContextInit[measurementIndex].startTime;
  }

  Rtm_IsrContext.intermediateTime = Rtm_IsrContextInit.intermediateTime;
  Rtm_IsrContext.prevActive = Rtm_IsrContextInit.prevActive;
  Rtm_IsrContext.startTime = Rtm_IsrContextInit.startTime;
  Rtm_IsrContext.isrId = Rtm_IsrContextInit.isrId;
#  endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON) */

  /* #30 Clear active command and indicate success in response. */
  Rtm_Cmd = RTM_NULL;
  Rtm_Resp = ((uint32) RTM_RESP_OK << 28);
} /* Rtm_Init_MeasurementData */

/**********************************************************************************************************************
 *   MM      MM  EEEEEEEEEE    AAAAAA      SSSSSS              CCCCCC      OOOOOO    NN      NN  TTTTTTTTTT
 *   MMM    MMM  EE          AA      AA  SS      SS          CC      CC  OO      OO  NNN     NN      TT    
 *   MMMM  MMMM  EE          AA      AA  SS                  CC          OO      OO  NNNN    NN      TT    
 *   MM  MM  MM  EEEEEEEE    AAAAAAAAAA    SSSSSS    ======  CC          OO      OO  NN  NN  NN      TT    
 *   MM      MM  EE          AA      AA          SS          CC          OO      OO  NN    NNNN      TT    
 *   MM      MM  EE          AA      AA  SS      SS          CC      CC  OO      OO  NN     NNN      TT    
 *   MM      MM  EEEEEEEEEE  AA      AA    SSSSSS              CCCCCC      OOOOOO    NN      NN      TT    
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Rtm_MeasCtr_MeasurementControl()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_MeasurementControl(Rtm_MeasurementTimestampType CurrentTime,
                                                                      Rtm_TimestampType TimeToMeasure,
                                                                      uint8 Command)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint8 sendingRequest = RTM_FALSE;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If a valid command is passed, execute it accordingly. */
  switch(Command)
  {
  /* Start Serial Measurement: */
  case(RTM_CMD_START_SERIAL_MEASUREMENT) :
    /* Only time-limited mode supported for serial measurement. */
    if (TimeToMeasure > 0)
    {
      sendingRequest = Rtm_MeasCtr_StartSerialOrParallelMeasurement(CurrentTime, TimeToMeasure);
    }
    break;

  /* Start Parallel Measurement: */
  case(RTM_CMD_START_PARALLEL_MEASUREMENT) :
    sendingRequest = Rtm_MeasCtr_StartSerialOrParallelMeasurement(CurrentTime, TimeToMeasure);
    break;

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
  /* Start Live Measurement: */
  case(RTM_CMD_START_LIVE_MEASUREMENT) :
    Rtm_MeasCtr_StartLiveMeasurement(CurrentTime, TimeToMeasure);
    break;
# endif

  /* Stop an endless measurement: */
  case(RTM_CMD_STOP_MEASUREMENT) :
    if (Rtm_EndlessMeasurement == RTM_TRUE)
    {
# if (RTM_MULTICORE_SUPPORT == STD_OFF)
      sendingRequest = Rtm_MeasCtr_StopEndlessMeasurement();
# else
      Rtm_SendNextMainFunction = Rtm_MeasCtr_StopEndlessMeasurement();
# endif
    }
    break;

  /* Clear all measurement results: */
  case(RTM_CMD_CLEAR_RESULTS) :
    sendingRequest = Rtm_MeasCtr_ClearMeasurementResults();
    break;

  /* #20 Otherwise: */
  default :
    /* #30 If a measurement is currently running, proceed it. */
# if (RTM_MULTICORE_SUPPORT == STD_OFF)
    /* Process Live Measurement: */
    if ((Rtm_LastCommandToEcu != RTM_NULL) && (Rtm_LiveMeasurement == RTM_TRUE))
    {
      sendingRequest = Rtm_MeasCtr_ProceedLiveMeasurement(CurrentTime);
    }
    /* Process Serial/Parallel Measurement: */
    else
    if ((Rtm_LastCommandToEcu != RTM_NULL) && (Rtm_LiveMeasurement == RTM_FALSE))
    {
      sendingRequest = Rtm_MeasCtr_ProceedSerialOrParallelMeasurement(CurrentTime);
    }
    else
    {
      /* Do nothing. */
    }
    break;
# else
    /* Process Serial/Parallel Measurement: */
    if (Rtm_LastCommandToEcu != RTM_NULL)
    {
      Rtm_SendNextMainFunction = Rtm_MeasCtr_ProceedSerialOrParallelMeasurement(CurrentTime);
    }
# endif
  }

  return sendingRequest;
} /* Rtm_MeasCtr_MeasurementControl */

/**********************************************************************************************************************
 * Rtm_MeasCtr_StartSerialOrParallelMeasurement()
 *********************************************************************************************************************/
/*!
 *
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
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_StartSerialOrParallelMeasurement(Rtm_MeasurementTimestampType CurrentTime,
                                                                                    Rtm_TimestampType TimeToMeasure)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint8 sendingRequest = RTM_FALSE;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Start all requested MPs. */
  Rtm_Hlp_SetMPStateOfAllMPs(RTM_FALSE);

  /* #20 If measurement should be started in endless mode, prepare measurement. */
  if (TimeToMeasure == ((Rtm_TimestampType) 0u))
  {
    Rtm_MeasurementTimer = 0u;
    Rtm_EndlessMeasurement = RTM_TRUE;
    Rtm_LastMainFunctionTime = CurrentTime;
    Rtm_AvgMainFunctionCycleTimeCalcCounter = 0u;
  }
  /* #30 Otherwise, measurement is started in time-limited mode. Calculate predicted measurement end (Tcycle * timeToMeasure [ms]). */
  else
  {
    Rtm_MeasurementTimer = TimeToMeasure;
    Rtm_EndlessMeasurement = RTM_FALSE;

    Rtm_CalculatedCtrEndVal = (Rtm_TimestampType)(CurrentTime
                                                + ((Rtm_MeasurementTimer + 1u)
                                                *  RTM_TICKS_PER_MILISECOND
                                                * RTM_MAINFCT_CYCLE_TIME));
    Rtm_FixTypeMismatch(Rtm_CalculatedCtrEndVal); /* PRQA S 3109,3410 */  /* MD_MSR_14.3,MD_Rtm_3410 */
  }

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
  /* #40 Calculate overhead runtime of Rtm_Start and Rtm_Stop macros. */
  Rtm_Hlp_CalculateOverhead();
# endif

  /* #50 Remember the measurement mode. */
  Rtm_LastCommandToEcu = Rtm_Cmd;
  Rtm_Cmd = RTM_NULL;

  /* #60 If CPU load measurement is started from ECU start-up:
   *       Indicate to transmit results and stop CPU load measurement. */
  Rtm_CheckAutostartCpuLoad(sendingRequest);  /* PRQA S 3109 */  /* MD_MSR_14.3 */

  return sendingRequest;
} /* Rtm_MeasCtr_StartSerialOrParallelMeasurement */

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_MeasCtr_StartLiveMeasurement()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_MeasCtr_StartLiveMeasurement(Rtm_MeasurementTimestampType CurrentTime,
                                                                        Rtm_TimestampType TimeToMeasure)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  Rtm_LiveMeasurement = RTM_TRUE;
  
  /* #10 Start all requested MPs. */
  Rtm_Hlp_SetMPStateOfAllMPs(RTM_FALSE);

  /* #20 If measurement should be started in endless mode, prepare measurement. */
  if (TimeToMeasure == ((Rtm_TimestampType) 0u))
  {
    Rtm_MeasurementTimer = 0u;
    Rtm_EndlessMeasurement = RTM_TRUE;
    Rtm_MainFunctionCounter = 1;
  }
  /* #30 Otherwise, measurement is started in time-limited mode.
   *       Calculate predicted measurement end (Tcycle * timeToMeasure [ms]). */
  else
  {
    Rtm_MeasurementTimer = TimeToMeasure;
    Rtm_EndlessMeasurement = RTM_FALSE;

    Rtm_CalculatedCtrEndVal = (Rtm_TimestampType)(CurrentTime + ((Rtm_MeasurementTimer + 1u) * RTM_TICKS_PER_MILISECOND * RTM_MAINFCT_CYCLE_TIME));
  }

  /* #40 Calculate overhead runtime of Rtm_Start and Rtm_Stop macros. */
  Rtm_Hlp_CalculateOverhead();

  /* #50 Remember the measurement mode. */
  Rtm_LastCommandToEcu = Rtm_Cmd;
  Rtm_Cmd = RTM_NULL;
} /* Rtm_MeasCtr_StartLiveMeasurement */
# endif

/**********************************************************************************************************************
 * Rtm_MeasCtr_StopEndlessMeasurement()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_StopEndlessMeasurement(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Stop all MPs, reset all measurement related data and trigger transmission. */
  Rtm_EndlessMeasurement = RTM_FALSE;
  Rtm_MeasurementTimer = 0u;
  Rtm_LastMainFunctionTime = 0u;
  Rtm_AvgMainFunctionCycleTimeCalcCounter = 0u;

  Rtm_Hlp_SetMPStateOfAllMPs(TRUE);

  Rtm_Init_MeasurementData();
  
  return RTM_TRUE;
} /* Rtm_MeasCtr_StopEndlessMeasurement */

/**********************************************************************************************************************
 * Rtm_MeasCtr_ClearMeasurementResults()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_ClearMeasurementResults(void)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint16_least measurementIndex;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Clear all MP's measurement results and reset all measurement related data. */
  Rtm_AverageMainFunctionCycleTime = 0;
  Rtm_MainFunctionCounter = 0;

  for (measurementIndex = 0; measurementIndex < RTM_MEAS_SECTION_COUNT; measurementIndex++)
  {
    Rtm_Results[measurementIndex].time  = (uint32) 0u;
    Rtm_Results[measurementIndex].count = (uint32) 0u;
    Rtm_Results[measurementIndex].min   = (uint32) 0xFFFFFFFFu;
    Rtm_Results[measurementIndex].max   = (uint32) 0u;
  }

  Rtm_Init_MeasurementData();

  return RTM_TRUE;
} /* Rtm_MeasCtr_ClearMeasurementResults */

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 * Rtm_MeasCtr_ProceedLiveMeasurement()
 *********************************************************************************************************************/
/*!
 *
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
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_ProceedLiveMeasurement(Rtm_MeasurementTimestampType CurrentTime)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint8 sendingRequest = RTM_FALSE;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If XCP is disconnected, deactivate live measurement. */
  if (Rtm_XcpState != XCP_EVENT_DAQ)
  {
    /* Measurements are going to get deactivated */
    Rtm_LiveMeasurement = RTM_FALSE;
    Rtm_MeasurementTimer = 0u;
    Rtm_EndlessMeasurement = RTM_FALSE;
  }
  /* #20 Otherwise: */
  else
  {
    /* #30 If the live measurement is time-limited: */
    if (Rtm_EndlessMeasurement == RTM_FALSE)
    {
      /* #40 Calculate the correction value for live measurement. */
      Rtm_MeasTimeCorrection = (sint32)(Rtm_CalculatedCtrEndVal - CurrentTime);
      Rtm_CalculatedCtrEndVal = (Rtm_TimestampType) (CurrentTime + (1u * RTM_TICKS_PER_MILISECOND * RTM_MAINFCT_CYCLE_TIME));

      /* #50 If the measurement duration is over: Stop all measurement points.
       *     Otherwise, move main function counter. */
      if (Rtm_MeasurementTimer == 0u)
      {
        Rtm_Hlp_SetMPStateOfAllMPs(RTM_TRUE);

        Rtm_LastCommandToEcu = RTM_NULL;
        Rtm_LiveMeasurement = RTM_FALSE;
      }
      else
      {
        Rtm_MeasurementTimer--;
      }
    }
    /* #60 Otherwise, the measurement is endless. Calculate last main function duration. */
    else
    {
      Rtm_GetCounterDelta(Rtm_LastMainFunctionTime, CurrentTime, &Rtm_AverageMainFunctionCycleTime, 0);
      Rtm_LastMainFunctionTime = CurrentTime;
    }

    /* #70 Always send OK in main function during live measurement. */
    Rtm_Resp = ((uint32) RTM_RESP_OK << 28); /* ESCAN00076777 */
    sendingRequest = RTM_TRUE;
  }

  return sendingRequest;
} /* Rtm_MeasCtr_ProceedLiveMeasurement */
# endif

/**********************************************************************************************************************
 * Rtm_MeasCtr_ProceedSerialOrParallelMeasurement()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(uint8, RTM_CODE) Rtm_MeasCtr_ProceedSerialOrParallelMeasurement(Rtm_MeasurementTimestampType CurrentTime)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint8 sendingRequest = RTM_FALSE;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If endless measurement is currently active, calculate the average main function duration. */
  if (Rtm_EndlessMeasurement == RTM_TRUE)
  {
    uint32 currentMainFuncCycleTime;

    Rtm_GetCounterDelta(Rtm_LastMainFunctionTime, CurrentTime, &currentMainFuncCycleTime, 0);

    if (Rtm_AvgMainFunctionCycleTimeCalcCounter < 32)
    {
      Rtm_AvgMainFunctionCycleTimeCalcCounter++;
    }
    Rtm_AverageMainFunctionCycleTime = ((Rtm_AverageMainFunctionCycleTime
                                      * (Rtm_AvgMainFunctionCycleTimeCalcCounter - 1)) 
                                      + currentMainFuncCycleTime)
                                      / Rtm_AvgMainFunctionCycleTimeCalcCounter;

    Rtm_MainFunctionCounter++;
    Rtm_LastMainFunctionTime = CurrentTime;
  }
  /* #20 Otherwise, the measurement is time-limited: */
  else
  {
    /* #30 If the measurement is over:
     *       Stop all MPs, indicate to transmit Xcp_Event and calculate the measurement correction. */
    if (Rtm_MeasurementTimer == 0u)
    {
      Rtm_Hlp_SetMPStateOfAllMPs(RTM_TRUE);

      Rtm_LastCommandToEcu = RTM_NULL;
      Rtm_Resp = ((uint32) RTM_RESP_OK << 28); /* ESCAN00076777 */
      sendingRequest = RTM_TRUE;

      Rtm_FixTypeMismatch(CurrentTime); /* PRQA S 3109,3410 */ /* MD_MSR_14.3,MD_Rtm_3410 */
      Rtm_MeasTimeCorrection = (sint32)(Rtm_CalculatedCtrEndVal - CurrentTime);
    }
    /* #40 Otherwise, move main function counter. */
    else
    {
      Rtm_MeasurementTimer--;
    }
  }

  return sendingRequest;
} /* Rtm_MeasCtr_ProceedSerialOrParallelMeasurement */

/**********************************************************************************************************************
 *     CCCCCC    PPPPPPPP    UU      UU
 *   CC      CC  PP      PP  UU      UU
 *   CC          PP      PP  UU      UU
 *   CC          PPPPPPPP    UU      UU
 *   CC          PP          UU      UU
 *   CC      CC  PP          UU      UU
 *     CCCCCC    PP            UUUUUU
 *********************************************************************************************************************/

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
#  if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
/**********************************************************************************************************************
 * Rtm_Cpu_CalculateCurrentCPULoad()
 *********************************************************************************************************************/
/*!
 *
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
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Cpu_CalculateCurrentCPULoad(void)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
#   if (RTM_CPU_LOAD_CONTROL_MODE == RTM_CPU_LOAD_CONTROL_XCP)
  uint8 rtmCpuLoadMeasurementWasActive;
#   endif

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Execute CPU load measurement function and cumulate results. */
  Rtm_CpuLoadMeasurementFunction();
  Rtm_CpuLoadTime = Rtm_CpuLoadCollectedTime;
  Rtm_CpuLoadCollectedTime = (uint32)0u;

#   if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
  Rtm_PrevOverrunCounterValues[RTM_CPU_LOAD_MEASUREMENT_ID] = Rtm_TimerOverrunCnt;
#   endif

  /* #20 If the CPU load measurement is currently active: */
  if (Rtm_CpuLoadMeasurementActive == RTM_TRUE)
  {
    /* #30 If CPU load measurement was stopped previously, Initialize CPU load MP and make reference measurement.
     *     Otherwise, cumulate the CPU load measurement results and handle possible result overruns. */
#   if (RTM_CPU_LOAD_CONTROL_MODE == RTM_CPU_LOAD_CONTROL_C_API)
    /* If new CPU load measurement cycle is triggered, the old results are cleaned */
    if (Rtm_CpuLoadWasStopped == RTM_TRUE)
    {
      Rtm_Init_CpuLoadMeasurement();
    }
    else
    {
#   endif
      Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].count++;
      Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].time = Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].time + Rtm_CpuLoadTime;

      if (Rtm_CpuLoadLastTicks > Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].time)
      {
        Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].time = Rtm_CpuLoadTime;
        Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].count = 1;
      }
      Rtm_CpuLoadLastTicks = Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].time;

      if (Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].min > Rtm_CpuLoadTime)
      {
        Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].min = (Rtm_MeasurementTimestampType)Rtm_CpuLoadTime;
      }
      if (Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].max < Rtm_CpuLoadTime)
      {
        Rtm_Results[RTM_CPU_LOAD_MEASUREMENT_ID].max = (Rtm_MeasurementTimestampType)Rtm_CpuLoadTime;
      }
#   if (RTM_CPU_LOAD_CONTROL_MODE == RTM_CPU_LOAD_CONTROL_C_API)
    }
#   endif
  }

#   if (RTM_CPU_LOAD_CONTROL_MODE == RTM_CPU_LOAD_CONTROL_XCP)
  /* #40 Remember whether CPU load measurement is currently active. */
  rtmCpuLoadMeasurementWasActive = Rtm_CpuLoadMeasurementActive;

  /* #50 Start or stop CPU load measurement. */
  Rtm_Start(RtmConf_RtmMeasurementPoint_Rtm_CpuLoadMeasurement); /* PRQA S 3109 */  /* MD_MSR_14.3 */

  /* #60 If CPU load measurement is started:
   *       Initialize CPU load MP and make reference measurement. */
  if ( (Rtm_CpuLoadMeasurementActive == RTM_TRUE)
    && ((rtmCpuLoadMeasurementWasActive == RTM_FALSE) 
    ||  (Rtm_CpuLoadWasStopped == RTM_TRUE))
     )
  {
    Rtm_Init_CpuLoadMeasurement();
  }
#   endif
} /* Rtm_Cpu_CalculateCurrentCPULoad */
#  endif /* (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF) */
# endif /* (RTM_MULTICORE_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *   NN      NN  EEEEEEEEEE  TTTTTTTTTT  
 *   NNN     NN  EE              TT      
 *   NNNN    NN  EE              TT      
 *   NN  NN  NN  EEEEEEEE        TT      
 *   NN    NNNN  EE              TT      
 *   NN     NNN  EE              TT      
 *   NN      NN  EEEEEEEEEE      TT      
 *********************************************************************************************************************/

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_Net_LeaveThread()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Net_LeaveThread( uint32 LeaveThread,
                                                           uint16 CoreId,
                                                           Rtm_MeasurementTimestampType CurrentTime )
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If the Thread is in valid range:
   *       Remember which MP is preempted and when this happened. */
  if (LeaveThread < RTM_NUMBER_OF_THREADS)
  {
    Rtm_ThreadContext[LeaveThread].prevActiveMP = Rtm_ActiveMeasurementId;
    Rtm_ThreadContext[LeaveThread].preemptionTime = CurrentTime;
  }

  RTM_DUMMY_STATEMENT(CoreId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
} /* Rtm_Net_LeaveThread */

/**********************************************************************************************************************
 * Rtm_Net_EnterThread()
 *********************************************************************************************************************/
/*!
 *
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
RTM_LOCAL_INLINE FUNC(void, RTM_CODE) Rtm_Net_EnterThread( uint32 EnterThread,
                                                           uint16 CoreId,
                                                           Rtm_MeasurementTimestampType CurrentTime )
{
  /* ----- Local Variables --------------------------------------------------------------------- */
#   if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
  uint32_least i;
  uint32 locPrevActive;
#   endif
  
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #20 If a Thread is started,
   *       and the Thread has interrupted a MP: */
  if (EnterThread < RTM_NUMBER_OF_THREADS)
  {
    Rtm_ActiveThreadId = EnterThread;
    Rtm_ActiveMeasurementId = Rtm_ThreadContext[EnterThread].prevActiveMP;

    if (Rtm_ActiveMeasurementId < RTM_MEAS_SECTION_COUNT)
    {
      /* #30 Calculate the preemption time. */
      Rtm_GetCounterDelta(Rtm_ThreadContext[EnterThread].preemptionTime, CurrentTime, &CurrentTime, Rtm_ActiveMeasurementId);

      /* #40 If there are MPs of NONNESTED and NESTED type, add preemption time to last active MP and all interrupted NONNESTED MPs. */
#   if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) && (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
      Rtm_MeasurementPointInfo[Rtm_ActiveMeasurementId].intermediateTime += CurrentTime;
      locPrevActive = Rtm_MeasurementPointInfo[Rtm_ActiveMeasurementId].prevActive;

      for ( i = 1; 
            (i < RTM_MEAS_SECTION_COUNT) && 
            (locPrevActive < RTM_MEAS_SECTION_COUNT) &&
            (Rtm_MeasurementPointInfo[locPrevActive].threadContext == (uint32)EnterThread);
            i++ )
      {
        if (Rtm_MeasurementPointInfo[locPrevActive].mpType == RTM_MP_TYPE_EXECUTIONTIME_NONNESTED)
        {
          Rtm_MeasurementPointInfo[locPrevActive].intermediateTime += CurrentTime;
        }

        locPrevActive = Rtm_MeasurementPointInfo[locPrevActive].prevActive;
      }
#   else

      /* #50 If all MPs are of NONNESTED type, add preemption time to all interrupted MPs. */
#    if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
      locPrevActive = Rtm_ActiveMeasurementId;

      for ( i = 0; 
            (i < RTM_MEAS_SECTION_COUNT) && 
            (locPrevActive < RTM_MEAS_SECTION_COUNT) &&
            (Rtm_MeasurementPointInfo[locPrevActive].threadContext == (uint32)EnterThread);
            i++ )
      {
        Rtm_MeasurementPointInfo[locPrevActive].intermediateTime += CurrentTime;

        locPrevActive = Rtm_MeasurementPointInfo[locPrevActive].prevActive;
      }
#    endif

      /* #60 If all MPs are of NESTED type, add preemption time to the last active MP. */
#    if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
      Rtm_MeasurementPointInfo[Rtm_ActiveMeasurementId].intermediateTime += CurrentTime;
#    endif

#   endif
    }
  }
  /* #-- Otherwise, no new context started, therefore no MP is currently active. */
  else
  {
    Rtm_ActiveMeasurementId = RTM_INACTIVE;
    Rtm_ActiveThreadId = RTM_NUMBER_OF_THREADS;
  }

  RTM_DUMMY_STATEMENT(CoreId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
} /* Rtm_Net_EnterThread */
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Rtm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, RTM_APPL_VAR) Versioninfo)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  uint8 errorCode = RTM_E_NO_ERROR;

  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    /* ----- Development Error Checks ------------------------------------------------------------ */
    /* #20 If parameter is invalid, indicate error.
     *     Otherwise, return version info. */
# if (RTM_DEV_ERROR_DETECT == STD_ON)
    if (Versioninfo == NULL_PTR)
    {
      errorCode = RTM_E_WRONG_PARAMETERS;
    }
    else
# endif
    {

      Versioninfo->vendorID = RTM_VENDOR_ID;
      Versioninfo->moduleID = RTM_MODULE_ID;
      Versioninfo->sw_major_version = RTM_SW_MAJOR_VERSION;
      Versioninfo->sw_minor_version = RTM_SW_MINOR_VERSION;
      Versioninfo->sw_patch_version = RTM_SW_PATCH_VERSION;
    }

      /* ----- Development Error Report ------------------------------------------------------------ */
# if (RTM_DEV_ERROR_REPORT == STD_ON)
    if (errorCode != RTM_E_NO_ERROR)
    {
      Rtm_DetReportError(RTM_SID_GETVERSIONINFO, errorCode);
    }
# else
    RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Rtm_GetVersionInfo */

/**********************************************************************************************************************
 * Rtm_StartMeasurementFct()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_StartMeasurementFct(uint32 measurementId) /* ESCAN00076464 */
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  Rtm_MeasurementTimestampType currentTime;
  uint8 errorCode = RTM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------------------------------ */
  /* #10 If the parameter is invalid, indicate an error. */
# if (RTM_DEV_ERROR_DETECT == STD_ON)
  if (measurementId >= RTM_MEAS_SECTION_COUNT)
  {
    errorCode = RTM_E_WRONG_PARAMETERS;
  }
  else
  /* If the MP is assigned to one specific core, the MP must be executed on this core. */
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
  if ( (Rtm_MeasurementPointInfo[measurementId].coreId < RTM_NUMBER_OF_CORES)
    && ((uint16)Rtm_GetCoreId() != Rtm_MeasurementPointInfo[measurementId].coreId)
     )
  {
    errorCode = RTM_E_INVALID_CONFIGURATION;
  }
  else
#  endif
# endif
  /* #20 Otherwise, if the MP is started the first time:
   *       Store current time. */
  {
    /* ----- Implementation -------------------------------------------------------------------- */
# if (RTM_NESTING_COUNTER == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]++;
    if (Rtm_MeasurementNestingCtr[measurementId] == 1)
# else
#  if (RTM_DEV_ERROR_DETECT == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]++;
#  endif
# endif
    {
      RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */

      Rtm_StartTimeStamps[measurementId] = currentTime;
# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
      Rtm_PrevOverrunCounterValues[measurementId] = Rtm_TimerOverrunCnt;    
# endif
    }

    /* #30 If the MP was started too often, indicate error. */
# if (RTM_NESTING_COUNTER == STD_ON) || (RTM_DEV_ERROR_DETECT == STD_ON)
    if (Rtm_MeasurementNestingCtr[measurementId] >= (sint8)100)
    {
     errorCode = RTM_E_UNBALANCED;
    }
# endif
  }

  /* ----- Development Error Report ------------------------------------------------------------ */
# if (RTM_DEV_ERROR_REPORT == STD_ON)
  if (errorCode != RTM_E_NO_ERROR)
  {
    Rtm_DetReportError(RTM_SID_STARTMEASUREMENT, errorCode);
  }
# else
  RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */
  /* PRQA S 6010 2 */ /* MD_MSR_STPTH */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Rtm_StartMeasurementFct */

/**********************************************************************************************************************
 * Rtm_StopMeasurementFct()
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, RTM_CODE) Rtm_StopMeasurementFct(uint32 measurementId) /* ESCAN00076464 */
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  Rtm_MeasurementTimestampType currentTime;
  uint8 errorCode = RTM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------------------------------ */
  /* #10 If the parameter is invalid, indicate an error. */
# if (RTM_DEV_ERROR_DETECT == STD_ON)
  if (measurementId >= RTM_MEAS_SECTION_COUNT)
  {
    errorCode = RTM_E_WRONG_PARAMETERS;
  }
  else
  /* If the MP is assigned to one specific core, the MP must be executed on this core. */
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
  if ( (Rtm_MeasurementPointInfo[measurementId].coreId < RTM_NUMBER_OF_CORES)
    && ((uint16)Rtm_GetCoreId() != Rtm_MeasurementPointInfo[measurementId].coreId)
     )
  {
    errorCode = RTM_E_INVALID_CONFIGURATION;
  }
  else
#  endif
# endif
  /* #20 Otherwise: */
  {
    /* ----- Implementation -------------------------------------------------------------------- */
    /* #30 If the MP was started prior:
     *       Calculate time difference since measurement start.
     *       Check for new minimum and maximum value.
     *       Cumulate the overall runtime.
     *       Reset results if overflow occurred. 
     *       Call callback if threshold exceeded. */
# if (RTM_NESTING_COUNTER == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]--;
    if (Rtm_MeasurementNestingCtr[measurementId] == 0) /* ESCAN00076464 */
# else
#  if (RTM_DEV_ERROR_DETECT == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]--;
#  endif
# endif
    {
      RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
      Rtm_GetCounterDelta(Rtm_StartTimeStamps[measurementId], currentTime, &currentTime, measurementId);

      if (currentTime > Rtm_Results[measurementId].max)
      {
        Rtm_Results[measurementId].max = currentTime;
      }
      if (currentTime < Rtm_Results[measurementId].min)
      {
        Rtm_Results[measurementId].min = currentTime;
      }

      Rtm_Results[measurementId].time = Rtm_Results[measurementId].time + currentTime;
      Rtm_Results[measurementId].count = Rtm_Results[measurementId].count + 1;

      if (Rtm_Results[measurementId].time < currentTime)
      {
        Rtm_Results[measurementId].time = (uint32) 0u;
        Rtm_Results[measurementId].count = (uint32) 0u;
      }

      if ((Rtm_ThresholdTimes[measurementId] < currentTime) && (0 < Rtm_ThresholdTimes[measurementId]))
      {
        (Rtm_ThresholdCbkFctArr[measurementId]) (currentTime);
      }
    }

    /* #40 If the MP was stopped twice, indicate an error. */
# if (RTM_NESTING_COUNTER == STD_ON) || (RTM_DEV_ERROR_DETECT == STD_ON)
    if (Rtm_MeasurementNestingCtr[measurementId] < (sint8)0)
    {
      errorCode = RTM_E_UNBALANCED;
    }
# endif
  }

  /* ----- Development Error Report ------------------------------------------------------------ */
# if (RTM_DEV_ERROR_REPORT == STD_ON)
  if (errorCode != RTM_E_NO_ERROR)
  {
    Rtm_DetReportError(RTM_SID_STOPMEASUREMENT, errorCode);
  }
# else
  RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  
  /* PRQA S 6080 4 */ /* MD_MSR_STMIF */  
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */    
  /* PRQA S 6010 2 */ /* MD_MSR_STPTH */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Rtm_StopMeasurementFct */

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
#  if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_StartNetMeasurementFct()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_StartNetMeasurementFct(uint32 measurementId) /* ESCAN00076464 */
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  Rtm_MeasurementTimestampType currentTime;
  uint8 errorCode = RTM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------------------------------ */
  /* #10 If the parameter is invalid, indicate an error. */
# if (RTM_DEV_ERROR_DETECT == STD_ON)
  if (measurementId >= RTM_MEAS_SECTION_COUNT)
  {
    errorCode = RTM_E_WRONG_PARAMETERS;
  }
  else
# endif
  /* #20 Otherwise: */
  {
    /* ----- Implementation -------------------------------------------------------------------- */
    /* #30 If the MP is started the first time and running on a valid thread:
     *       Store current time.
     *     Otherwise, indicate an error. */
#   if (RTM_NESTING_COUNTER == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]++;
    if (Rtm_MeasurementNestingCtr[measurementId] == 1)
#   else
#    if (RTM_DEV_ERROR_DETECT == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]++;
#    endif
#   endif
    {
      /* #-- A MP of type execution time has to be running in context of a thread. */
      if (Rtm_ActiveThreadId < RTM_NUMBER_OF_THREADS)
      {
        Rtm_MeasurementPointInfo[measurementId].threadContext = Rtm_ActiveThreadId;
        Rtm_MeasurementPointInfo[measurementId].prevActive = Rtm_ActiveMeasurementId;
    
        Rtm_ActiveMeasurementId = measurementId;
        RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        Rtm_StartTimeStamps[measurementId] = currentTime;

#   if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
        Rtm_PrevOverrunCounterValues[measurementId] = Rtm_TimerOverrunCnt;    
#   endif
      }
      /* #-- Otherwise, report an DET error. */
      else
      {
        errorCode = RTM_E_INVALID_CONFIGURATION;
      }
    }

# if (RTM_NESTING_COUNTER == STD_ON) || (RTM_DEV_ERROR_DETECT == STD_ON)
    if (Rtm_MeasurementNestingCtr[measurementId] >= (sint8)100)
    {
      errorCode = RTM_E_UNBALANCED;
    }
# endif
  }

  /* ----- Development Error Report ------------------------------------------------------------ */
# if (RTM_DEV_ERROR_REPORT == STD_ON)
  if (errorCode != RTM_E_NO_ERROR)
  {
    Rtm_DetReportError(RTM_SID_STARTNETMEASUREMENT, errorCode);
  }
# else
  RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* PRQA S 6080 4 */ /* MD_MSR_STMIF */
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */
  /* PRQA S 6010 2 */ /* MD_MSR_STPTH */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Rtm_StartNetMeasurementFct */

/**********************************************************************************************************************
 * Rtm_StopNetMeasurementFct()
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, RTM_CODE) Rtm_StopNetMeasurementFct(uint32 measurementId) /* ESCAN00076464 */
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  Rtm_MeasurementTimestampType currentTime;
  uint32 interTime;
  uint32 interruptedMeasId;
  uint8  errorCode = RTM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------------------------------ */
  /* #10 If the parameter is invalid, indicate an error. */
# if (RTM_DEV_ERROR_DETECT == STD_ON)
  if (measurementId >= RTM_MEAS_SECTION_COUNT)
  {
    errorCode = RTM_E_WRONG_PARAMETERS;
  }
  else
# endif
  /* #20 Otherwise: */
  {
    /* ----- Implementation -------------------------------------------------------------------- */
    /* #30 If the MP was started prior: */
#   if (RTM_NESTING_COUNTER == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]--;
    if (Rtm_MeasurementNestingCtr[measurementId] == 0) /* ESCAN00076464 */
#   else
#    if (RTM_DEV_ERROR_DETECT == STD_ON)
    Rtm_MeasurementNestingCtr[measurementId]--;
#    endif
#   endif
    {
      /* #40 If the currently active MP is stopped: */
      if (Rtm_ActiveMeasurementId == measurementId)
      {
        /* #50 Calculate time difference since measurement start. */
        RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        interTime = Rtm_MeasurementPointInfo[measurementId].intermediateTime;
        Rtm_GetCounterDelta(Rtm_StartTimeStamps[measurementId], currentTime, &currentTime, measurementId);

        interruptedMeasId = Rtm_MeasurementPointInfo[measurementId].prevActive;

        /* #60 If the interrupted measurement point is of type execution time nested:
         *       Add gross runtime of now stopped MP and overhead correction value to the intermediateTime of the interrupted MP. */
#   if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
        if ((interruptedMeasId < RTM_MEAS_SECTION_COUNT)
#    if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
          && (Rtm_MeasurementPointInfo[interruptedMeasId].mpType == RTM_MP_TYPE_EXECUTIONTIME_NESTED)
#    endif
           )
        {
          Rtm_MeasurementPointInfo[interruptedMeasId].intermediateTime = Rtm_MeasurementPointInfo[interruptedMeasId].intermediateTime + currentTime; /* Add brutto runtime of current measurement point */
          if (Rtm_MeasurementPointInfo[interruptedMeasId].interruptsDisabled == TRUE)
          {
            Rtm_MeasurementPointInfo[interruptedMeasId].intermediateTime = Rtm_MeasurementPointInfo[interruptedMeasId].intermediateTime + Rtm_OverheadCorrection.execTime_Nested_DisableISRs;
          }
          else
          {
            Rtm_MeasurementPointInfo[interruptedMeasId].intermediateTime = Rtm_MeasurementPointInfo[interruptedMeasId].intermediateTime + Rtm_OverheadCorrection.execTime_Nested_EnableISRs;
          }
        }
#   endif

        /* #70 Activate the interrupted MP. */
        Rtm_MeasurementPointInfo[measurementId].threadContext = RTM_NUMBER_OF_THREADS;
        Rtm_ActiveMeasurementId = interruptedMeasId;
        Rtm_MeasurementPointInfo[measurementId].intermediateTime = 0;

        /* #80 Subtract intermediateTime from current result. */
        if (interTime < currentTime)
        {
          currentTime = currentTime - interTime;
        }
        else
        {
          currentTime = 0;
        }

        /* #90 Check for new minimum and maximum value. */
        if (currentTime > Rtm_Results[measurementId].max)
        {
          Rtm_Results[measurementId].max = currentTime;
        }
        if (currentTime < Rtm_Results[measurementId].min)
        {
          Rtm_Results[measurementId].min = currentTime;
        }

        /* #100 Cumulate the overall runtime, reset results if overflow detected. */
        Rtm_Results[measurementId].time = Rtm_Results[measurementId].time + currentTime;
        Rtm_Results[measurementId].count = Rtm_Results[measurementId].count + 1;

        if (Rtm_Results[measurementId].time < currentTime)
        {
          /* Overflow occurred, reset results to defined state */
          Rtm_Results[measurementId].time = (uint32) 0u;
          Rtm_Results[measurementId].count = (uint32) 0u;
        }

        /* #110 Call callback if threshold exceeded. */
        if ((Rtm_ThresholdTimes[measurementId] < currentTime) && (0 < Rtm_ThresholdTimes[measurementId]))
        {
          (Rtm_ThresholdCbkFctArr[measurementId]) (currentTime);
        }
      }
      /* #120 Otherwise, inconsistent Start/Stop sequence detected; Only the active MP is allowed to be stopped. */
      else
      {
        errorCode = RTM_E_UNBALANCED;
      }
    }

# if (RTM_NESTING_COUNTER == STD_ON) || (RTM_DEV_ERROR_DETECT == STD_ON)
    if (Rtm_MeasurementNestingCtr[measurementId] < (sint8)0)
    {
      errorCode = RTM_E_UNBALANCED;
    }
# endif
  }
  
  /* ----- Development Error Report ------------------------------------------------------------ */
# if (RTM_DEV_ERROR_REPORT == STD_ON)
  if (errorCode != RTM_E_NO_ERROR)
  {
    Rtm_DetReportError(RTM_SID_STOPNETMEASUREMENT, errorCode);
  }
# else
  RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* PRQA S 6080 4 */ /* MD_MSR_STMIF */
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */
  /* PRQA S 6010 2 */ /* MD_MSR_STPTH */
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Rtm_StopNetMeasurementFct */

/**********************************************************************************************************************
 * Rtm_Schedule()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_Schedule( uint32 FromThreadId,
                                   uint32 ToThreadId,
                                   uint16 CoreId )
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
#   if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
#   endif
  {
    Rtm_MeasurementTimestampType currentTime = 0;

    /* #15 If the active MP is interrupted or if a interrupted MP is resumed, get the current time. */
    if ((Rtm_ActiveMeasurementId != RTM_INACTIVE) 
     || ((ToThreadId < RTM_NUMBER_OF_THREADS)
     &&  (Rtm_ThreadContext[ToThreadId].prevActiveMP < RTM_MEAS_SECTION_COUNT)))
    {
      RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    }

    /* #20 If a Thread is suspended/preempted:
     *       Store the preempted MP and the current time. */
    Rtm_Net_LeaveThread( FromThreadId, CoreId, currentTime );

    /* #30 If a Thread is started and the Thread interrupted a net MP:
     *       Start the preempted net MP and add the preemption runtime to these MP and all interrupted nonnested MPs. */
    Rtm_Net_EnterThread( ToThreadId, CoreId, currentTime );
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif

  RTM_DUMMY_STATEMENT(CoreId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
} /* Rtm_Schedule */

FUNC(void, RTM_CODE) Rtm_EnterTaskFct(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    uint8 errorCode = RTM_E_NO_ERROR;

    /* Only active measurements can be interrupted */
    Rtm_TaskType locTaskId;

    GetTaskID((TaskRefType)&locTaskId); /* PRQA S 3200 */ /* MD_Rtm_3200 */
  
    if (locTaskId < RTM_NUMBER_OF_TASKS)
    {
      Rtm_ThreadContext[locTaskId].prevActiveMP = Rtm_ActiveThreadId;
      Rtm_ActiveThreadId = locTaskId;

      if (Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].prevActive < RTM_MEAS_SECTION_COUNT)
      {
#   if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
        uint16_least i;
#   endif
        uint32 locPrevActive = Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].prevActive;
        Rtm_MeasurementTimestampType currentTime;

        RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        /* Get the intermediate time */
        Rtm_GetCounterDelta(Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].startTime, currentTime, &currentTime, locPrevActive);

        /* There are measurement points set to execution time with nested and with non nested. */
#   if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) && (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)

        /* The first and all interrupted non nested MPs get the interrupting task runtime. */
        for (i = 0; (i < RTM_MEAS_SECTION_COUNT) && (locPrevActive < RTM_MEAS_SECTION_COUNT); i++)
        {
          if ((i == 0) || (Rtm_MeasurementPointInfo[locPrevActive].mpType == RTM_MP_TYPE_EXECUTIONTIME_NONNESTED))
          {
            Rtm_MeasurementPointInfo[locPrevActive].intermediateTime = Rtm_MeasurementPointInfo[locPrevActive].intermediateTime + currentTime;
          }

          locPrevActive = Rtm_MeasurementPointInfo[locPrevActive].prevActive;
        }
#   else

        /* All measurement points set to execution time are not nestable. */
#    if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
        /* Add task runtime to each measurement point's intermediate time which is set to execution time. */
        for (i = 0; (i < RTM_MEAS_SECTION_COUNT) && (locPrevActive < RTM_MEAS_SECTION_COUNT); i++)
        {
          Rtm_MeasurementPointInfo[locPrevActive].intermediateTime = Rtm_MeasurementPointInfo[locPrevActive].intermediateTime + currentTime;

          locPrevActive = Rtm_MeasurementPointInfo[locPrevActive].prevActive;
        }
#    endif /* (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) */

        /* All measurement points set to execution time are also nestable. */
#    if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
        /* Add task runtime to the single interrupted measurement point (only one can simultaneously be active). */
        /* The following line cannot be executed with previous for loops, thus a memory corruption is not possible here. */
        Rtm_MeasurementPointInfo[locPrevActive].intermediateTime = Rtm_MeasurementPointInfo[locPrevActive].intermediateTime + currentTime;
#    endif /* (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON) */

#   endif /* (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) && (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON) */
        Rtm_ActiveMeasurementId = Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].prevActive;
        Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].prevActive = RTM_MEAS_SECTION_COUNT;
      }
    }
    else
    {
      errorCode = RTM_E_INVALID_CONFIGURATION;
    }

    /* ----- Development Error Report ------------------------------------------------------------ */
#  if (RTM_DEV_ERROR_REPORT == STD_ON)
    if (errorCode != RTM_E_NO_ERROR)
    {
      Rtm_DetReportError(RTM_SID_ENTERTASKFCT, errorCode);
    }
#  else
    RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Rtm_EnterTaskFct */

FUNC(void, RTM_CODE) Rtm_LeaveTaskFct(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    uint8 errorCode = RTM_E_NO_ERROR;

    Rtm_TaskType locTaskId;
    GetTaskID((TaskRefType)&locTaskId); /* PRQA S 3200 */ /* MD_Rtm_3200 */

    if (locTaskId < RTM_NUMBER_OF_TASKS)
    {
      Rtm_ActiveThreadId = Rtm_ThreadContext[locTaskId].prevActiveMP;
    
      /* Only active measurements can be interrupted */
      if (Rtm_ActiveMeasurementId < RTM_MEAS_SECTION_COUNT)
      {
        Rtm_TimestampType currentTime;
        Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].prevActive = Rtm_ActiveMeasurementId;
        Rtm_ActiveMeasurementId = RTM_INACTIVE;
        RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */

        Rtm_TaskContext[Rtm_TaskIndex[locTaskId]].startTime = currentTime;
      }
    }
    else
    {
      errorCode = RTM_E_INVALID_CONFIGURATION;
    }

    /* ----- Development Error Report ------------------------------------------------------------ */
#  if (RTM_DEV_ERROR_REPORT == STD_ON)
    if (errorCode != RTM_E_NO_ERROR)
    {
      Rtm_DetReportError(RTM_SID_LEAVETASKFCT, errorCode);
    }
#  else
    RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Rtm_LeaveTaskFct */

FUNC(void, RTM_CODE) Rtm_EnterIsrFct(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    uint32 locIsrId = GetISRID();

    if (locIsrId < RTM_NUMBER_OF_ISRS)
    {
      Rtm_ThreadContext[locIsrId + RTM_NUMBER_OF_TASKS].prevActiveMP = Rtm_ActiveThreadId;
      Rtm_ActiveThreadId = locIsrId + RTM_NUMBER_OF_TASKS;
    }

    /* Only active measurements can be interrupted */
    if (Rtm_ActiveMeasurementId < RTM_MEAS_SECTION_COUNT)
    {
      /* Only the outer ISR may change the runtime of measurement points */
      if (Rtm_IsrContext.isrId == RTM_NUMBER_OF_ISRS)
      {
        Rtm_TimestampType currentTime;
        Rtm_IsrContext.isrId = locIsrId;
        /* Safe measurement context */
        Rtm_IsrContext.prevActive = Rtm_ActiveMeasurementId;
        Rtm_ActiveMeasurementId = (uint32)RTM_PAUSED_BY_ISR;
        RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        Rtm_IsrContext.startTime = currentTime;
      }
    }
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Rtm_EnterIsrFct */

FUNC(void, RTM_CODE) Rtm_LeaveIsrFct(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    uint32 locIsrId = GetISRID();

    if (locIsrId < RTM_NUMBER_OF_ISRS)
    {
      Rtm_ActiveThreadId = Rtm_ThreadContext[locIsrId + RTM_NUMBER_OF_TASKS].prevActiveMP;
    }

    /* Only active measurements can be interrupted */
    if (Rtm_ActiveMeasurementId == RTM_PAUSED_BY_ISR)
    {
      if (Rtm_IsrContext.isrId == locIsrId)
      {
#   if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
        uint16_least i;
#   endif
        uint32 currentTime;
        uint32 locPrevActive = Rtm_IsrContext.prevActive;
        /* Restore measurement context */
        RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        Rtm_GetCounterDelta(Rtm_IsrContext.startTime, currentTime, &currentTime, locPrevActive);

        /* There are measurement points set to execution time with nested and with non nested. */
#   if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) && (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
        /* The last active measurement point and all non nested measurement points get the interrupting task runtime. */
        for (i = 0; (i < RTM_MEAS_SECTION_COUNT) && (locPrevActive < RTM_MEAS_SECTION_COUNT); i++)
        {
          if ((i == 0) || (Rtm_MeasurementPointInfo[locPrevActive].mpType == RTM_MP_TYPE_EXECUTIONTIME_NONNESTED))
          {
            Rtm_MeasurementPointInfo[locPrevActive].intermediateTime = Rtm_MeasurementPointInfo[locPrevActive].intermediateTime + currentTime;
          }

          locPrevActive = Rtm_MeasurementPointInfo[locPrevActive].prevActive;
        }
#   else

        /* All measurement points set to execution time are not nestable. */
#    if (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON)
        /* Add task runtime to each measurement point's intermediate time which is set to execution time. */
        for (i = 0; (i < RTM_MEAS_SECTION_COUNT) && (locPrevActive < RTM_MEAS_SECTION_COUNT); i++)
        {
          Rtm_MeasurementPointInfo[locPrevActive].intermediateTime = Rtm_MeasurementPointInfo[locPrevActive].intermediateTime + currentTime;

          locPrevActive = Rtm_MeasurementPointInfo[locPrevActive].prevActive;
        }
#    endif /* (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) */

        /* All measurement points set to execution time are also nestable. */
#    if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
        /* Add task runtime to the single interrupted measurement point (only one can simultaneously be active). */
        Rtm_MeasurementPointInfo[locPrevActive].intermediateTime = Rtm_MeasurementPointInfo[locPrevActive].intermediateTime + currentTime;
#    endif /* (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON) */

#   endif /* (RTM_EXECUTION_TIME_NONNESTED_AVAILABLE == STD_ON) && (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON) */

        Rtm_ActiveMeasurementId = Rtm_IsrContext.prevActive;
        Rtm_IsrContext.prevActive = RTM_MEAS_SECTION_COUNT;
        Rtm_IsrContext.isrId = RTM_NUMBER_OF_ISRS;
      }
    }
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Rtm_LeaveIsrFct */
#  endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON) */
# endif /* (RTM_MULTICORE_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 * Rtm_CheckTimerOverrun()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* ESCAN00078841 */
FUNC(void, RTM_CODE) Rtm_CheckTimerOverrun(void) 
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled:
   *       Enter exclusive area 0.
   *       Detect and handle counter overflow.
   *       Leave exclusive area 0. */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    Rtm_MeasurementTimestampType currentTime;
    SchM_Enter_Rtm_RTM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */ 
    RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    SchM_Exit_Rtm_RTM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
    RTM_DUMMY_STATEMENT(currentTime); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
} /* Rtm_CheckTimerOverrun */

# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
/**********************************************************************************************************************
 * Rtm_CpuLoadMeasurementFunction()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_CpuLoadMeasurementFunction(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
#  if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
#  endif
  {
    Rtm_MeasurementTimestampType cpuLoadSinceLastCall; 
    Rtm_MeasurementTimestampType currentTime;
#   if (RTM_MULTICORE_SUPPORT == STD_ON)
    uint16 locCoreId;
#   endif

    /* #20 Enter exclusive area 2. */
    SchM_Enter_Rtm_RTM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

    /* #30 If the CPU load measurement is active: */
#   if (RTM_MULTICORE_SUPPORT == STD_ON)
    locCoreId = (uint16)Rtm_GetCoreId();
    if (Rtm_CpuLoadMeasurementActive[locCoreId] == RTM_TRUE)
#   else
    if (Rtm_CpuLoadMeasurementActive == RTM_TRUE)
#   endif
    {
      /* #40 Calculate the time difference since last call.
       *     If the difference is greater than the calculated overhead, cumulate the time. */
      RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */

#   if (RTM_MULTICORE_SUPPORT == STD_ON)
      /* ESCAN00078841 */
      Rtm_GetCounterDelta(Rtm_CpuLoadStartTimestamp[locCoreId], currentTime, &cpuLoadSinceLastCall, RTM_CPU_LOAD_MEASUREMENT_ID);

      if (cpuLoadSinceLastCall > (Rtm_CpuLoadOverhead[locCoreId] + (Rtm_CpuLoadOverhead[locCoreId] >> 1))) /* Add 50% safety margin */
      {
        /* Only count runtimes which are not caused by this function itself */
        Rtm_CpuLoadCollectedTime[locCoreId] = (Rtm_CpuLoadCollectedTime[locCoreId] + cpuLoadSinceLastCall) - Rtm_CpuLoadOverhead[locCoreId];
      }
      Rtm_CpuLoadStartTimestamp[locCoreId] = currentTime;
#   else
      /* ESCAN00078841 */
      Rtm_GetCounterDelta(Rtm_CpuLoadStartTimestamp, currentTime, &cpuLoadSinceLastCall, RTM_CPU_LOAD_MEASUREMENT_ID);

      if (cpuLoadSinceLastCall > (Rtm_CpuLoadOverhead + (Rtm_CpuLoadOverhead >> 1))) /* Add 50% safety margin */
      {
        /* Only count runtimes which are not caused by this function itself */
        Rtm_CpuLoadCollectedTime = (Rtm_CpuLoadCollectedTime + cpuLoadSinceLastCall) - Rtm_CpuLoadOverhead;
      }
      Rtm_CpuLoadStartTimestamp = currentTime;
#   endif
  
#   if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)  
      Rtm_PrevTimerValue = Rtm_CpuLoadStartTimestamp;
      Rtm_PrevOverrunCounterValues[RTM_CPU_LOAD_MEASUREMENT_ID] = Rtm_TimerOverrunCnt;
#   endif
    }

    /* #50 Leave exclusive area 2. */
    SchM_Exit_Rtm_RTM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
  }
#  if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
#  endif
} /* Rtm_CpuLoadMeasurementFunction */
# endif /* RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF */

/**********************************************************************************************************************
 * Rtm_GetMeasurementItem()
 *********************************************************************************************************************/
/*!
 *
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
 */
/* PRQA S 3673 3 */ /* MD_Rtm_3673 */
FUNC(Std_ReturnType, RTM_CODE) Rtm_GetMeasurementItem( CONST(uint32, AUTOMATIC) measurementId, 
                                                       CONST(Rtm_ItemType, AUTOMATIC) itemType, 
                                                       P2VAR(uint32, AUTOMATIC, RTM_APPL_VAR) itemValuePtr)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorCode = RTM_E_NO_ERROR;

  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
    /* ----- Development Error Checks ------------------------------------------------------------ */
    /* #20 If the parameters are invalid, indicate an error. */
# if (RTM_DEV_ERROR_DETECT == STD_ON)
    if (itemValuePtr == NULL_PTR)
    {
      errorCode = RTM_E_WRONG_PARAMETERS;
    }
    else
# endif
    /* #30 Otherwise: */
    {
      /* ----- Implementation -------------------------------------------------------------------- */
# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
      uint32 locMeasId = measurementId;
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
      uint16 locCoreId = RTM_NUMBER_OF_CORES;
#  endif

      /* PRQA S 3109,3201,3355,3358 15 */  /* MD_MSR_14.3,MD_Rtm_3201,MD_Rtm_3355,MD_Rtm_3358 */
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
      Rtm_CheckValidMeasurementId(locMeasId, locCoreId);
#  else
      Rtm_CheckValidMeasurementId(locMeasId);
#  endif

      /* #40 If the request is valid: */
      /*lint -e415 */ /* Suppress ID415 because array access is blocked for invalid index. */
      /* PRQA S 3201,3355,3356,3358,3359 12 */ /* MD_Rtm_3201,MD_Rtm_3355,MD_Rtm_3356,MD_Rtm_3358,MD_Rtm_3359 */
      if ((locMeasId < RTM_MEAS_SECTION_COUNT)
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
         && (locCoreId < RTM_NUMBER_OF_CORES)
#  endif
#  if (RTM_DEV_ERROR_DETECT == STD_OFF)
         && (itemValuePtr != NULL_PTR)
#  endif
         )
      {
        switch(itemType)
        {
        /* #50 If the average CPU load is requested:
         *       Enter exclusive area 0.
         *       Calculate the average CPU load.
         *       Leave exclusive area 0. */
        case (RTM_ITEM_CPU_LOAD_AVERAGE):
          SchM_Enter_Rtm_RTM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
          if (Rtm_Results[locMeasId].count > 0)
          {
            *itemValuePtr = (uint32)((100 * (((Rtm_Results[locMeasId].time) / (Rtm_Results[locMeasId].count)))) / (RTM_MAINFCT_CYCLE_TIME * RTM_TICKS_PER_MILISECOND));
            if (*itemValuePtr > 100)
            {
              *itemValuePtr = 100;
            }
          }
          else
          {
            *itemValuePtr = 0;
          }
          retVal = E_OK;
          SchM_Exit_Rtm_RTM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        break;

        /* #60 Otherwise, if the current CPU load is requested:
         *       Enter exclusive area 0.
         *       Calculate the current CPU load.
         *       Leave exclusive area 0. */
        case (RTM_ITEM_CPU_LOAD_CURRENT): /* ESCAN00079844 */
          SchM_Enter_Rtm_RTM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
          *itemValuePtr = (uint32)((100 * Rtm_CpuLoadTime[locCoreId]) / (RTM_MAINFCT_CYCLE_TIME * RTM_TICKS_PER_MILISECOND));
#  else
          *itemValuePtr = (uint32)((100 * Rtm_CpuLoadTime) / (RTM_MAINFCT_CYCLE_TIME * RTM_TICKS_PER_MILISECOND));
#  endif
          retVal = E_OK;
          SchM_Exit_Rtm_RTM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
        break;

        /* #70 Otherwise, if the minimum CPU load is requested, calculate it. */
        case (RTM_ITEM_MIN):
          if (Rtm_Results[locMeasId].min < 0xFFFFFFFFu)
          {
            *itemValuePtr = (uint32)((100 * (Rtm_Results[locMeasId].min)) / (RTM_MAINFCT_CYCLE_TIME * RTM_TICKS_PER_MILISECOND));
          }
          else
          {
            *itemValuePtr = 0;
          }
          retVal = E_OK;
        break;

        /* #80 Otherwise, if the maximum CPU load is requested, calculate it. */
        case (RTM_ITEM_MAX):
          *itemValuePtr = (uint32)((100 * (Rtm_Results[locMeasId].max)) / (RTM_MAINFCT_CYCLE_TIME * RTM_TICKS_PER_MILISECOND));
          retVal = E_OK;
        break;

        /* #90 Otherwise, request is invalid. */
        default: /* PRQA S 2018 */ /* MD_Rtm_2018 */
          *itemValuePtr = 0;
        break;
        }
      }
      else /* PRQA S 3201 1 */ /* MD_Rtm_3201 */
      {
        /* Required for MISRA */
      }
# else /* (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF) */
      RTM_DUMMY_STATEMENT_CONST(measurementId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
      RTM_DUMMY_STATEMENT_CONST(itemType); /* PRQA S 3112 */ /* MD_MSR_14.2 */
      RTM_DUMMY_STATEMENT(itemValuePtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF) */
    }
    
    /* ----- Development Error Report ------------------------------------------------------------ */
#  if (RTM_DEV_ERROR_REPORT == STD_ON)
    if (errorCode != RTM_E_NO_ERROR)
    {
      Rtm_DetReportError(RTM_SID_GETMEASUREMENTITEM, errorCode);
    }
#  else
    RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
  return retVal;

  /* PRQA S 2006 3 */ /* MD_MSR_14.7 */
  /* PRQA S 6080 2 */ /* MD_MSR_STMIF */
  /* PRQA S 6030 1 */ /* MD_MSR_STCYC */
} /* Rtm_GetMeasurementItem */

# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Rtm_InitCpuLoadMeasurement()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_InitCpuLoadMeasurement(uint16 coreId, uint32 cpuMeasId)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Initialize all CPU load related variables. */
  Rtm_Results[cpuMeasId].count = 0x0u;
  Rtm_Results[cpuMeasId].time = 0x0u;
  Rtm_Results[cpuMeasId].min = 0xFFFFFFFFu;
  Rtm_Results[cpuMeasId].max = 0x0u;

  Rtm_CpuLoadWasStopped[coreId] = RTM_FALSE;
  Rtm_CpuLoadTime[coreId] = 0;
  Rtm_CpuLoadStartTimestamp[coreId] = RTM_GET_TIME_MEASUREMENT_FCT();
  Rtm_CpuLoadLastTicks[coreId] = 0;

  /* #20 Calculate CPU load overhead. */
  Rtm_CpuLoadOverhead[coreId]      = (uint32) 0u;
  Rtm_CpuLoadCollectedTime[coreId] = (uint32) 0u;
  Rtm_CpuLoadMeasurementFunction();
  Rtm_CpuLoadCollectedTime[coreId] = (uint32) 0u;
  Rtm_CpuLoadMeasurementFunction();
  Rtm_CpuLoadOverhead[coreId]      = Rtm_CpuLoadCollectedTime[coreId];
  if (Rtm_CpuLoadOverhead[coreId] == 0)
  {
    Rtm_CpuLoadOverhead[coreId] = 1;
  }
}
#  endif /* (RTM_MULTICORE_SUPPORT == STD_ON) */
# endif /* (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF) */
#endif /* RTM_TIME_MEASUREMENT == STD_ON */

/**********************************************************************************************************************
 * Rtm_InitMemory()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_InitMemory(void)
{
  /* ----- Local Variables --------------------------------------------------------------------- */
#if (RTM_TIME_MEASUREMENT == STD_ON)
  uint16_least i;
  
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 Initialize MP specific data. */
  for (i = 0; i < RTM_MEAS_SECTION_COUNT; i++)
  {
# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
    Rtm_MeasurementPointInfo[i].intermediateTime = 0;
    Rtm_MeasurementPointInfo[i].prevActive = RTM_INACTIVE;
    Rtm_MeasurementPointInfo[i].threadContext = RTM_NUMBER_OF_THREADS;
# endif
# if ((RTM_NESTING_COUNTER == STD_ON) || (RTM_DEV_ERROR_DETECT == STD_ON))
    Rtm_MeasurementNestingCtr[i] = 0u;
# endif
    Rtm_StartTimeStamps[i] = 0u;
    Rtm_Results[i].time = 0u;
    Rtm_Results[i].count = 0u;
    Rtm_Results[i].min = (uint32) 0xFFFFFFFFu;
    Rtm_Results[i].max = 0u;
# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
    Rtm_PrevOverrunCounterValues[i] = 0u;
# endif
  }

  for (i = 0; i < RTM_CTRL_VECTOR_LEN; i++)
  {
    Rtm_MeasurementConfig[i] = 0u;
    Rtm_Ctrl[i] = Rtm_CtrlConfig[i]; /* ESCAN00077449 */
  }

# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
  /* #20 Initialize thread context specific data. */
  for (i = 0; i < RTM_NUMBER_OF_TASKS; i++)
  {
    Rtm_TaskContext[i].intermediateTime = Rtm_TaskContextInit[i].intermediateTime;
    Rtm_TaskContext[i].prevActive = Rtm_TaskContextInit[i].prevActive;
    Rtm_TaskContext[i].startTime = Rtm_TaskContextInit[i].startTime;
    Rtm_TaskContext[i].taskId = Rtm_TaskContextInit[i].taskId; /* ESCAN00087879 */
  }

  for (i = 0; i < RTM_NUMBER_OF_THREADS; i++)
  {
    Rtm_ThreadContext[i].preemptionTime = Rtm_ThreadContextInit[i].preemptionTime;
    Rtm_ThreadContext[i].prevActiveMP = Rtm_ThreadContextInit[i].prevActiveMP;
  }

  /* Initialize ISR context */
  Rtm_ActiveMeasurementId = RTM_INACTIVE;
  Rtm_ActiveThreadId = RTM_NUMBER_OF_THREADS;

  Rtm_IsrContext.intermediateTime = Rtm_IsrContextInit.intermediateTime;
  Rtm_IsrContext.prevActive = Rtm_IsrContextInit.prevActive;
  Rtm_IsrContext.startTime = Rtm_IsrContextInit.startTime;
  Rtm_IsrContext.isrId = Rtm_IsrContextInit.isrId;
# endif

# if (RTM_EXECUTION_TIME_NESTED_AVAILABLE == STD_ON)
  Rtm_OverheadCorrection.execTime_Nested_EnableISRs = 0;
  Rtm_OverheadCorrection.execTime_Nested_DisableISRs = 0;
# endif

  /* #30 Initialize timer overrun specific data. */
# if (RTM_TIMER_OVERRUN_SUPPORT == STD_ON)
  Rtm_TimerOverrunCnt = 0;
  Rtm_PrevTimerValue = 0;
# endif

  /* #40 Initialize multicore specific data. */
# if (RTM_MULTICORE_SUPPORT == STD_ON)
  for (i = 0; i < RTM_NUMBER_OF_DISABLED_INT; i++)
  {
    Rtm_DisableInterruptFlag[i] = 0;
  }
# endif

  /* Set RTM state to uninitialized */
  Rtm_InitState = RTM_UNINITIALIZED;
#endif /* RTM_TIME_MEASUREMENT == STD_ON */

  /* #50 Initialize SafeBSW specific data. */
#if (RTM_CONTROL == STD_ON)
  Rtm_ControlState       = RTM_CONTROL_STATE_ENABLED;
  Rtm_ControlDeniedCount = 0;
#endif
} /* Rtm_InitMemory */

#if (RTM_TIME_MEASUREMENT == STD_ON)
/**********************************************************************************************************************
 * Rtm_Init()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, RTM_CODE) Rtm_Init(void)
{
  /* ----- Implementation -------------------------------------------------------------------- */
  /* #10 If Rtm's functionality is enabled: */
# if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
# endif
  {
# if (RTM_MULTICORE_SUPPORT == STD_ON)
    uint16_least i;
# endif /* (RTM_MULTICORE_SUPPORT == STD_ON) */

    /* #20 Set task mapping. */
# if (RTM_NET_RUNTIME_SUPPORT == STD_ON)
    Rtm_Init_TaskContext();
# endif /* (RTM_NET_RUNTIME_SUPPORT == STD_ON) */

    /* #30 Initialize CPU load specific data. */
# if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
#  if (RTM_MULTICORE_SUPPORT == STD_ON)
    /* PRQA S 3109, 3201, 3355, 3358 1 */  /* MD_MSR_14.3, MD_Rtm_3201, MD_Rtm_3355, MD_Rtm_3358 */
    Rtm_ActivateCpuLoadMeasurementPoints();

    for (i = 0; i < RTM_NUMBER_OF_CORES; i++)
    {
      Rtm_CpuLoadOverhead[i] = 0x0u;
      Rtm_CpuLoadCollectedTime[i] = 0x0u;
      Rtm_CpuLoadStartTimestamp[i] = 0x0u;
      Rtm_CpuLoadWasStopped[i] = RTM_TRUE; /* ESCAN00079842 */

      Rtm_CpuLoadLastTicks[i] = 0;
    }

#  else
    Rtm_CpuLoadOverhead = 0x0u;
    Rtm_CpuLoadCollectedTime = 0x0u;
    Rtm_CpuLoadStartTimestamp = 0x0u;
#   if defined (RTM_CPU_LOAD_AUTOSTART)
    Rtm_CpuLoadMeasurementActive = RTM_TRUE;
    Rtm_CpuLoadSendResult = RTM_TRUE;
#   else
    Rtm_CpuLoadMeasurementActive = RTM_FALSE;
#   endif
    Rtm_CpuLoadWasStopped = RTM_TRUE; /* ESCAN00079842 */

    Rtm_CpuLoadLastTicks = 0;
#  endif
# endif

# if (RTM_MULTICORE_SUPPORT == STD_ON)
    Rtm_SendNextMainFunction = RTM_FALSE;
    Rtm_MasterCore = (uint16)Rtm_GetCoreId();
    for (i = 0; i < RTM_NUMBER_OF_CORES; i++)
    {
      Rtm_CpuLoadTime[i] = 0x0u;
    }
# else
    Rtm_CpuLoadTime = 0x0u;
    Rtm_LiveMeasurement = RTM_FALSE;
# endif

    /* #40 Initialize common Rtm control data. */
    Rtm_CalculatedCtrEndVal = 0x0u;
    Rtm_MeasTimeCorrection = 0x0;
    Rtm_MeasurementTimer = 0x0u;

    Rtm_LastCommandToEcu = RTM_NULL;
    Rtm_Cmd = RTM_NULL;
    Rtm_Resp = RTM_NULL;
    Rtm_XcpState = XCP_EVENT_NOP;

    Rtm_AverageMainFunctionCycleTime = 0x0;
    Rtm_MainFunctionCounter = 0x0;
    Rtm_EndlessMeasurement = RTM_FALSE;
    Rtm_AvgMainFunctionCycleTimeCalcCounter = 0x0u;
    Rtm_LastMainFunctionTime = 0x0u;

    Rtm_InitState = RTM_INITIALIZED;
  }
# if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
# endif
} /* Rtm_Init */

#endif /* RTM_TIME_MEASUREMENT == STD_ON */

/**********************************************************************************************************************
 * Rtm_MainFunction()
 *********************************************************************************************************************/
/*!
 *
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
 */
#if (RTM_MULTICORE_SUPPORT == STD_ON)
FUNC(uint8, RTM_CODE) Rtm_MainFunction(uint16 coreId)
#else
FUNC(void, RTM_CODE) Rtm_MainFunction(void)
#endif
{
  /* ----- Local Variables --------------------------------------------------------------------- */
#if (RTM_TIME_MEASUREMENT == STD_ON)
  uint8 sendEvent0 = RTM_FALSE;
# if (RTM_MULTICORE_SUPPORT == STD_ON)
  uint8 errorCode = RTM_E_NO_ERROR;
# endif
#endif

  /* #10 If the Rtm is currently enabled: */
#if (RTM_CONTROL == STD_ON)
  if (Rtm_ControlState == RTM_CONTROL_STATE_ENABLED)
#endif
  {
    /* #20 If the core ID is invalid, report a DET error. */
    /* #30 If the current core is the BSW core: */
#if (RTM_TIME_MEASUREMENT == STD_ON)
# if (RTM_MULTICORE_SUPPORT == STD_ON)
    /* ----- Development Error Checks ------------------------------------------------------------ */
#  if (RTM_DEV_ERROR_DETECT == STD_ON)
    if (coreId >= RTM_NUMBER_OF_CORES)
    {
      errorCode = RTM_E_WRONG_PARAMETERS;
    }
    else
#  endif
    {
      if (Rtm_MasterCore == coreId)
# endif
      {
        /* ----- Implementation -------------------------------------------------------------------- */
        /* #40 If Rtm is already initialized: */
# if (RTM_MULTICORE_SUPPORT == STD_OFF)
        if (Rtm_InitState == RTM_INITIALIZED)
# endif
        {
          Rtm_MeasurementTimestampType currentTime;
          Rtm_TimestampType timeToMeasure;
          uint8 cmd;

# if (RTM_MULTICORE_SUPPORT == STD_OFF)
          /* #50 Enter exclusive area 2. */
          SchM_Enter_Rtm_RTM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */  /* MD_MSR_14.3 */
# endif

          RTM_GET_CURRENT_TIME(currentTime); /* PRQA S 3109 */  /* MD_MSR_14.3 */

          /* #60 Get requested command and requested measurement duration. */
          cmd = (uint8)(Rtm_Cmd >> 28);
          timeToMeasure = (uint16)Rtm_Cmd;

          /* #70 If delayed sending was requested, send in this Rtm_MainFunction-Cycle.
           *     Otherwise, execute the requested command. */
# if (RTM_MULTICORE_SUPPORT == STD_ON)
          if (Rtm_SendNextMainFunction == RTM_TRUE)
          {
            Rtm_SendNextMainFunction = RTM_FALSE;
            sendEvent0 = RTM_TRUE;
          }
          else
# endif
          {
            sendEvent0 = Rtm_MeasCtr_MeasurementControl(currentTime, timeToMeasure, cmd);
          }

          /* #80 Calculate new CPU load measurement result. */
# if (RTM_MULTICORE_SUPPORT == STD_OFF)
#  if (RTM_CPU_LOAD_CONTROL_MODE != RTM_CPU_LOAD_CONTROL_OFF)
          Rtm_Cpu_CalculateCurrentCPULoad();
#  endif

          /* #90 Leave exclusive area 2. */
          SchM_Exit_Rtm_RTM_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */  /* MD_MSR_14.3 */

          /* #100 Transmit current data if requested. */
          if ((sendEvent0 == RTM_TRUE) || (Rtm_LiveMeasurement == RTM_TRUE))
          {
            Rtm_XcpState = Xcp_Event(EV_RTM);
          }
# endif
        }
      }
# if (RTM_MULTICORE_SUPPORT == STD_ON)
    }

    /* ----- Development Error Report ------------------------------------------------------------ */
#  if (RTM_DEV_ERROR_REPORT == STD_ON)
    if (errorCode != RTM_E_NO_ERROR)
    {
      Rtm_DetReportError(RTM_SID_MAINFUNCTION, errorCode);
    }
#  else
    RTM_DUMMY_STATEMENT(errorCode);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
# endif

#else /* (RTM_TIME_MEASUREMENT == STD_ON) */
# if (RTM_MULTICORE_SUPPORT == STD_ON)
    RTM_DUMMY_STATEMENT(coreId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
#endif /* (RTM_TIME_MEASUREMENT == STD_ON) */
  }
#if (RTM_CONTROL == STD_ON)
  else
  {
    Rtm_ControlDeniedCount++;
  }
#endif

#if (RTM_MULTICORE_SUPPORT == STD_ON)
  return sendEvent0;
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* Rtm_MainFunction */

#define RTM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 ********************************************************************************************************************/
/* module specific MISRA deviations:

   MD_Rtm_3201:    MISRA rule: 14.1: 
     Reason:        If the function is called with invalid parameters the statement is not reachable.
     Risk:          Compiler warning.
     Prevention:    Covered by code review.

   MD_Rtm_3410:     MISRA rule: 19.10:
     Reason:        Assignment cannot be enclosed in brackets.
     Risk:          Compile error.
     Prevention:    Not required.

   MD_Rtm_0342:     MISRA rule: 19.13: K&R compilers do not support the ISO glue operator ##. AUTOSAR requires C90 and 
                                       C90 supports ## operators.
     Reason:        Identifier has to be a combination of both IDs
     Risk:          Compile error.
     Prevention:    Not required.

   MD_Rtm_3355:     MISRA Rule: 13.7:
     Reason:        Result or value is always true or false depend on configuration aspect and used platform specific implementation.
     Risk:          Incorrect platform specific return will be ignored.
     Prevention:    Code inspection and test of the different variants in the component test.

   MD_Rtm_3356:     MISRA Rule: 13.7:
     Reason:        Result or value is always true or false depend on configuration aspect and used platform specific implementation
     Risk:          Incorrect platform specific return will be ignored.
     Prevention:    Code inspection and test of the different variants in the component test.

   MD_Rtm_3358:     MISRA Rule: 13.7:
     Reason:        Result or value is always true or false depend on configuration aspect and used platform specific implementation
     Risk:          Incorrect platform specific return will be ignored.
     Prevention:    Code inspection and test of the different variants in the component test.

   MD_Rtm_3359:     MISRA Rule: 13.7:
     Reason:        Result or value is always true or false depend on configuration aspect and used platform specific implementation
     Risk:          Incorrect platform specific return will be ignored.
     Prevention:    Code inspection and test of the different variants in the component test.

   MD_Rtm_5115:     MISRA rule: 20.2: The names of standard library macros, objects and functions shall not be reused.
     Reason:        Name of Rtm_DataSet struct member is called 'time'. Automotive software does not used standard libraries.
     Risk:          No risk, because RTM 'time' is encapsulated in struct.
     Prevention:    Do not use standard libraries.

   MD_Rtm_3200:     MISRA rule: 16.10: '%s' returns a value which is not being used.
     Reason:        The API is defined by OS. To be robust against defining GetTaskID as code 
                    ("#define GetTaskID { ... }") the void cast is omitted.
     Risk:          The return value is not handled by RTM, thus the compiler has to care about it.
     Prevention:    Not required.

   MD_Rtm_3673:      MISRA rule: 16.7: The object addressed by the pointer parameter '%s' is not modified and so the
                                      pointer could be of type 'pointer to const'.
     Reason:        This violation only occurs if RTM_CPU_LOAD_CONTROL_MODE is set to RTM_CPU_LOAD_CONTROL_OFF 
                    because the function Rtm_GetMeasurementItem does not return a value in this case.
     Risk:          The return value is not handled by RTM, thus the compiler has to care about it.
     Prevention:    Not required.

   MD_Rtm_3408:     MISRA rule: 8.8: '-identifier-' has external linkage but is being defined without any previous
                                     declaration.
     Reason:        This violation only occurs if RTM_LOCAL is defined to nothing.
     Risk:          The global variables could be re-defined in another file.
     Prevention:    Define RTM_LOCAL as static.

   MD_Rtm_883:      MISRA rule: 19.15: Precautions shall be taken in order to prevent the contents of a header file
                                       being included twice.
     Reason:        QAC does not find the "#if !defined / #define / #endif" in Rtm_Cfg.h although it exists.
     Risk:          Could be removed without being detected by QAC (=> Compile Error).
     Prevention:    Code inspection and test of the different variants in the component test.

   MD_Rtm_2018:     MISRA rule: 14.1: There shall be no unreachable code.
     Reason:        The default path cannot be reached if DET is enabled, but a default path in a switch or
                    if-/if-else-statement is always required.
     Risk:          No risk.
     Prevention:    Not required.
*/

/* COV_JUSTIFICATION_BEGIN

\ID COV_RTM_FILTER_PAUSE_SUPPORT
\ACCEPT XF xf xx
\ACCEPT XF xf ??
\REASON The feature pause support is always filtered

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Rtm.c
 *********************************************************************************************************************/

