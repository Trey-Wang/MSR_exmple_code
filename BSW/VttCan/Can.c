/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* STARTSINGLE_OF_MULTIPLE */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*  \file     Can.c / Can_Irq.c / Can_Local.h
 *  \brief    Implementation of the CAN driver
 *  \details  see functional description below
 *
 *********************************************************************************************************************/
/* ***************************************************************************
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Refer to the module's header file.
|************************************************************************** */

/* \trace SPEC-1570 */
/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* ECO_IGNORE_BLOCK_BEGIN */
#define C_DRV_INTERNAL /* compatibility */
#define CAN_SOURCE     /* testability */
#define __CAN_SRC__              /* compatibility RTM not aware of multi driver instance */
#if !defined(__CAN_SRC__) /* driver use ORGANI without infix */ /* COV_CAN_INTERNAL */
# define __CAN_SRC__    /* for all MSR module (used by RTM) */
#endif
/* PRQA S 3109 EOF */ /* MD_Can_3109 */
/* PRQA S 0750 EOF */ /* MD_Can_0750 */
/* PRQA S 3412 EOF */ /* MD_Can_3412 */
/* PRQA S 3458 EOF */ /* MD_Can_3458 */
/* PRQA S 3453 EOF */ /* MD_Can_3453 */
/* PRQA S 0850 EOF */ /* MD_Can_0850 */
/* PRQA S 0857 EOF */ /* MD_Can_0857 */
/* PRQA S 0828 EOF */ /* MD_Can_0828 */
/* PRQA S 0715 EOF */ /* MD_Can_0715 */

/* \trace SPEC-1408, SPEC-1590, SPEC-1588 */
#if !defined( MISRA_CHECK ) /* COV_CAN_MISRA */
# include <string.h>
#endif

#include "Can_Cfg.h"
# include "VTTCntrl_Can.h" /* PRQA S 810 */ /* include structure given by OS */
/* \trace SPEC-1392 */
#include "CanIf_Cbk.h"   /* for callback declaration */
/* \trace SPEC-1707 */
#include "CanIf.h"       /* for version check */
#include "Can_Local.h"    /* have to be last include due to channel define will lead to problems */

#if (CAN_DEV_ERROR_REPORT == STD_ON)
/* \trace SPEC-1596 */
# include "Det.h"
#endif
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if !defined(V_SUPPRESS_EXTENDED_VERSION_CHECK)
#  include "v_ver.h"
# endif
#endif

#if !defined(CAN_AMD_RUNTIME_MEASUREMENT) /* COV_CAN_COMPATIBILITY */
# define CAN_AMD_RUNTIME_MEASUREMENT STD_OFF
#endif
#if (CAN_AMD_RUNTIME_MEASUREMENT == STD_ON) /* COV_CAN_AMD_RUNTIME_MEASUREMENT */
# include "AmdRtm.h"
#endif

#if !defined(CAN_RUNTIME_MEASUREMENT_SUPPORT) /* COV_CAN_COMPATIBILITY */
# define CAN_RUNTIME_MEASUREMENT_SUPPORT STD_OFF
#endif
#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
/* AR4-328 */
# define C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT
# include "Rtm.h"
#endif

#if defined(C_ENABLE_CAN_FD_FULL)
# if defined(C_ENABLE_CANCEL_IN_HW) || defined(C_ENABLE_RX_QUEUE)
#  include "vstdlib.h"
# endif
#endif

/* ECO_IGNORE_BLOCK_END */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_COREVERSION           != 0x0507u) /* \trace SPEC-1699, SPEC-3837 */
# error "Source and Header file are inconsistent!"
#endif
#if (CAN_RELEASE_COREVERSION   != 0x01u)
# error "Source and Header file are inconsistent!"
#endif
#if defined(CAN_GEN_BASE_VERSION) /* COV_CAN_COMPATIBILITY */
# if (CAN_GEN_BASE_VERSION              != 0x0106u)
#  error "Source and Generated Header file are inconsistent!"
# endif
#elif defined(CAN_GEN_BASE_CFG5_VERSION) /* COV_CAN_COMPATIBILITY */
# define CAN_GEN_BASE_VERSION  CAN_GEN_BASE_CFG5_VERSION
# if (CAN_GEN_BASE_CFG5_VERSION         != 0x0103u)
#  error "Source and Generated Header file are inconsistent!"
# endif
#else
# error "No CAN_GEN_BASE_VERSION nor CAN_GEN_BASE_CFG5_VERSION is defined"
#endif
#if( DRVCAN_CANOEEMUCANOEASR_VERSION != 0x0507u)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN_CANOEEMUCANOEASR_RELEASE_VERSION != 0x00u)
# error "Source and Header file are inconsistent!"
#endif

#if( CAN_GEN_CanoeemuCanoeAsr_VERSION != 0x0102u)
# error "Generated Data are inconsistent!"
#endif

#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if !defined(V_SUPPRESS_EXTENDED_VERSION_CHECK)
/* The name of this constant variable is defined via v_ver.h and depends on the version of the C file. The value of the constant variable is also defined externally and is used for consistency checks, too. */
#  define CAN_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_DEF_CONST(V_NONE, uint32, CONST) DRVCAN_CANOEEMUCANOEASR_LIB_SYMBOL = DRVCAN_CANOEEMUCANOEASR_LIB_VERSION;   /* PRQA S 3408 */ /* MD_Can_Asr_3408_libCheck */
#  define CAN_STOP_SEC_CONST_UNSPECIFIED   /*--------------------------------*/
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
# endif
#endif

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/
#if defined(CAN_USE_NO_VECTOR_IF) /* for testability */
# define CAN_USE_CRITICALSECTION_OVER_CONFIRMATION
#else
# if defined(CANIF_VENDOR_ID) /* COV_CAN_COMPATIBILITY */
#  if (CANIF_VENDOR_ID == 0x001E) /* COV_CAN_COMPATIBILITY */
#   define CAN_USE_VECTOR_IF
#   if defined(IF_ASRIFCAN_VERSION) /* COV_CAN_COMPATIBILITY */
#    if (IF_ASRIFCAN_VERSION < 0x0410) /* COV_CAN_COMPATIBILITY */
#     define CAN_USE_CRITICALSECTION_OVER_CONFIRMATION
#    endif
#   endif
#  else
#   define CAN_USE_CRITICALSECTION_OVER_CONFIRMATION
#  endif
# else
#  define CAN_USE_CRITICALSECTION_OVER_CONFIRMATION
# endif
#endif
/* Compatibility to CBD */
/* instead of direct canCanInterruptCounter[] use Can_GetCanInterruptCounter(Index) abstraction for ComStackLib */

/* Hook mapping */
# if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
#define CanHookBegin_Can_Init()                              Rtm_Start(RtmConf_RtmMeasurementPoint_Can_Init)
#define CanHookEnd_Can_Init()                                Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_Init)
#define CanHookBegin_Can_InitController()                    Rtm_Start(RtmConf_RtmMeasurementPoint_Can_InitController)
#define CanHookEnd_Can_InitController()                      Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_InitController)
#define CanHookBegin_Can_SetControllerMode()                 Rtm_Start(RtmConf_RtmMeasurementPoint_Can_SetControllerMode)
#define CanHookEnd_Can_SetControllerMode()                   Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_SetControllerMode)
#define CanHookBegin_Can_Write()                             Rtm_Start(RtmConf_RtmMeasurementPoint_Can_Write)
#define CanHookEnd_Can_Write()                               Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_Write)

# else /* BRS runtime measurement */
#define CanHookBegin_Can_Init()
#define CanHookEnd_Can_Init()
#define CanHookBegin_Can_InitController() \

#define CanHookEnd_Can_InitController() \

#define CanHookBegin_Can_SetControllerMode() \

#define CanHookEnd_Can_SetControllerMode() \

#define CanHookBegin_Can_Write() \

#define CanHookEnd_Can_Write() \

# endif

#define CanHookBegin_CanHL_TxConfirmation() \

#define CanHookEnd_CanHL_TxConfirmation() \

#define CanHookBegin_CanBasicCanMsgReceived() \

#define CanHookEnd_CanBasicCanMsgReceived() \

#define CanHookBegin_CanFullCanMsgReceived() \

#define CanHookEnd_CanFullCanMsgReceived() \

#define CanHookBegin_Can_GetVersionInfo()
#define CanHookEnd_Can_GetVersionInfo()
#define CanHookBegin_Can_InitMemory()
#define CanHookEnd_Can_InitMemory()
#define CanHookBegin_Can_InitStruct()
#define CanHookEnd_Can_InitStruct()
#define CanHookBegin_Can_ChangeBaudrate()
#define CanHookEnd_Can_ChangeBaudrate()
#define CanHookBegin_Can_CheckBaudrate()
#define CanHookEnd_Can_CheckBaudrate()
#define CanHookBegin_Can_SetBaudrate()
#define CanHookEnd_Can_SetBaudrate()
#define CanHookStateChange_StartSync( oldState, newState )
#define CanHookStateChange_StopSync( oldState, newState )
#define CanHookStateChange_WakeupSync( oldState, newState )
#define CanHookStateChange_SleepSync( oldState, newState )
#define CanHookStateChange_StartAsync( oldState, newState )
#define CanHookStateChange_StopAsync( oldState, newState )
#define CanHookStateChange_WakeupAsync( oldState, newState )
#define CanHookStateChange_SleepAsync( oldState, newState )
#define CanHookBegin_Can_DisableControllerInterrupts()
#define CanHookEnd_Can_DisableControllerInterrupts()
#define CanHookBegin_Can_EnableControllerInterrupts()
#define CanHookEnd_Can_EnableControllerInterrupts()
#define CanHookBegin_Can_CheckWakeup()
#define CanHookEnd_Can_CheckWakeup()
#define CanHookBegin_Can_MainFunction_Write() \

#define CanHookEnd_Can_MainFunction_Write() \

#define CanHookBegin_Can_MainFunction_Read() \

#define CanHookEnd_Can_MainFunction_Read() \

#define CanHookBegin_Can_MainFunction_BusOff()
#define CanHookEnd_Can_MainFunction_BusOff()
#define CanHookBegin_Can_MainFunction_Wakeup()
#define CanHookEnd_Can_MainFunction_Wakeup()
#define CanHookBegin_Can_MainFunction_Mode()
#define CanHookEnd_Can_MainFunction_Mode()
#define CanHookBegin_Can_CancelTx()
#define CanHookEnd_Can_CancelTx()
#define CanHookBegin_Can_ResetBusOffStart()
#define CanHookEnd_Can_ResetBusOffStart()
#define CanHookBegin_Can_ResetBusOffEnd()
#define CanHookEnd_Can_ResetBusOffEnd()
#define CanHookBegin_Can_GetStatus()
#define CanHookEnd_Can_GetStatus()

/* Define chiphardware                     */
/* Constants concerning can chip registers */
/* control registers for msg objects       */
# if (CANOEAPI_VERSION < 204) /* support CAN FD */
#  error "This CANOEAPI_VERSION is not supported"
# endif
# define CANCELL_ERRORACTIVE   VTTCAN_ERR_ACTIVE    /* notify Active  is reached */
# define CANCELL_ERRORPASSIVE  VTTCAN_ERR_PASSIVE   /* notify Passive is reached */
# define CANCELL_WARNINGLIMIT  VTTCAN_ERR_WARNINGLIMIT    /* notify Warning is reached */
# define CANCELL_BUSOFF        VTTCAN_ERR_BUSOFF          /* notify BUSOFF  is reached */

# define CanCell_InitChannel(busName, ch, mode)               VttCan_SetCanConfiguration((vuint8)(ch), \
                                                                                         (double)Can_GetarbBaudrateOfInitObject(Can_GetLastInitObject(ch)), Can_GetarbTseg1OfInitObject(Can_GetLastInitObject(ch)),  /* PRQA S 5013 */ /* MD_Can_5013_LL */ \
                                                                                         Can_GetarbTseg2OfInitObject(Can_GetLastInitObject(ch)), Can_GetarbSjwOfInitObject(Can_GetLastInitObject(ch)), \
                                                                                         Can_GetarbSamOfInitObject(Can_GetLastInitObject(ch)), (mode), \
                                                                                         (double)Can_GetdbrBaudrateOfInitObject(Can_GetLastInitObject(ch)), Can_GetdbrTseg1OfInitObject(Can_GetLastInitObject(ch)),  /* PRQA S 5013 */ /* MD_Can_5013_LL */ \
                                                                                         Can_GetdbrTseg2OfInitObject(Can_GetLastInitObject(ch)), Can_GetdbrSjwOfInitObject(Can_GetLastInitObject(ch)), \
                                                                                         Can_GetdbrSamOfInitObject(Can_GetLastInitObject(ch)), (mode)); \
                                                              VttCan_SetMode((ch), VTTCAN_MODE_INIT)
# define CanCell_StopChannel(busName, ch)                     VttCan_SetMode((ch), VTTCAN_MODE_STOP)
# define CanCell_StartChannel(busName, ch)                    VttCan_SetMode((ch), VTTCAN_MODE_START)
# define CanCell_SleepChannel(busName, ch)                    VttCan_SetMode((ch), VTTCAN_MODE_SLEEP)
# define CanCell_WakeupChannel(busName, ch)                   VttCan_SetMode((ch), VTTCAN_MODE_WAKEUP)
# define CanCell_SendMessage(ch, id, dataLength, data, flags) VttCan_SendCanMessage((ch), (id), (flags), 0, CAN_DECODE_DLI(dataLength), (data))
# define CanCell_EnableInterrupts(ch, tx, rx, err, wakeup)    Can_GetgCanController(ch).IntEnable = (vuint8)((vuint8)((tx)*CAN_INT_TX) + (vuint8)((rx)*CAN_INT_RX) + (vuint8)((err)*CAN_INT_ERR) + (vuint8)((wakeup)*CAN_INT_WAKEUP)); \
                                                                VttCan_EnableISR((vuint8)(ch), (vuint8)(tx), (vuint8)(rx), (vuint8)(err), (vuint8)(wakeup))
# define CanCell_CallInterrupts(ch)                           /* done by Interrupt controller */
# define CanCell_AtomicBegin()                                VttCan_AtomicBegin()
# define CanCell_AtomicEnd()                                  VttCan_AtomicEnd()

#if defined(C_SINGLE_RECEIVE_CHANNEL)
# define CanLL_ApplCanTimerStart(loop)  ApplCanTimerStart(loop)
# define CanLL_ApplCanTimerEnd(loop)    ApplCanTimerEnd(loop)
# define CanLL_ApplCanTimerLoop(loop)   ApplCanTimerLoop(loop)
#else
# define CanLL_ApplCanTimerStart(loop)  ApplCanTimerStart(channel, loop)
# define CanLL_ApplCanTimerEnd(loop)    ApplCanTimerEnd(channel, loop)
# define CanLL_ApplCanTimerLoop(loop)   ApplCanTimerLoop(channel, loop)
#endif

#define assertHardwareReturn(condition, ch, id) assertHardware((condition), (ch), (id)); if(!(condition)) { \
                                                                                           return; \
                                                                                         }

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/
#if defined(CAN_TXINDIRECTIONHWTOLOG) /* COV_CAN_TXINDIRECTIONHWTOLOG */
/* CAN_HL_TX_OFFSET_HW_TO_LOG(ch) not supported for LL use CanHL_GetTxHwToLogHandle or CanHL_GetTxLogToHwHandle instead */
#else
# define CAN_HL_TX_OFFSET_HW_TO_LOG(ch) (Can_GetTxOffsetHwToLog(ch)) /* used in LL */
#endif

/* for LL compatibility */
#if defined(C_ENABLE_GEN_HW_START_STOP_IDX) /* COV_CAN_GEN_HW_START_STOP_IDX */
# define CAN_HL_HW_TX_NORMAL_INDEX(hwch)     (Can_GetTxBasicHwStart(hwch))
#  define CAN_HL_HW_TX_STARTINDEX(hwch)      (Can_GetTxFullHwStart(hwch))
# define CAN_HL_HW_TX_FULL_STARTINDEX(hwch)  (Can_GetTxFullHwStart(hwch))
# define CAN_HL_HW_UNUSED_STARTINDEX(hwch)   (Can_GetUnusedHwStart(hwch))
# define CAN_HL_HW_RX_FULL_STARTINDEX(hwch)  (Can_GetRxFullHwStart(hwch))
# define CAN_HL_HW_RX_BASIC_STARTINDEX(hwch) (Can_GetRxBasicHwStart(hwch))
#  define CAN_HL_HW_TX_STOPINDEX(hwch)       (Can_GetTxBasicHwStop(hwch))
# define CAN_HL_HW_TX_FULL_STOPINDEX(hwch)   (Can_GetTxFullHwStop(hwch))
# define CAN_HL_HW_UNUSED_STOPINDEX(hwch)    (Can_GetUnusedHwStop(hwch))
# define CAN_HL_HW_RX_FULL_STOPINDEX(hwch)   (Can_GetRxFullHwStop(hwch))
# define CAN_HL_HW_RX_BASIC_STOPINDEX(hwch)  (Can_GetRxBasicHwStop(hwch))
#else
# define CAN_ASR_HW_TX_NORMAL_INDEX(hwch)     (Can_GetHwHandle(Can_GetTxBasicHandleStart(hwch)))
#  define CAN_ASR_HW_TX_STARTINDEX(hwch)      (CAN_ASR_HW_TX_FULL_STARTINDEX(hwch))
# if !defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
/* >= R19 --> size for each Object may differ for MULTIPLEXED_TRANSMISSION (please use ENABLE_GEN_HW_START_STOP_IDX instead) */
#  define CAN_ASR_HW_TX_STOPINDEX(hwch)       (CAN_ASR_HW_TX_STARTINDEX(hwch) + (Can_GetTxBasicHandleMax(hwch) * CAN_MULTIPLEXED_TX_MAX) + Can_GetTxFullHandleMax(hwch))
# endif
#  define CAN_ASR_HW_TX_FULL_STARTINDEX(hwch)  (Can_GetHwHandle(Can_GetTxFullHandleStart(hwch)))
#  define CAN_ASR_HW_TX_FULL_STOPINDEX(hwch)   (CAN_ASR_HW_TX_FULL_STARTINDEX(hwch) + Can_GetTxFullHandleMax(hwch))

# define CAN_ASR_HW_UNUSED_STARTINDEX(hwch)   (Can_GetHwHandle(Can_GetUnusedHandleStart(hwch)))
# define CAN_ASR_HW_UNUSED_STOPINDEX(hwch)   ((Can_GetUnusedHandleStart(hwch) != Can_GetUnusedHandleStop(hwch)) ? \
  (Can_GetHwHandle((Can_GetUnusedHandleStop(hwch)-1)) + Can_GetDLC_FIFO((Can_GetUnusedHandleStop(hwch)-1))) : (Can_GetHwHandle(Can_GetUnusedHandleStart(hwch))))

#  define CAN_ASR_HW_RX_FULL_STARTINDEX(hwch)  (Can_GetHwHandle(Can_GetRxFullHandleStart(hwch)))
#  define CAN_ASR_HW_RX_FULL_STOPINDEX(hwch)   ((Can_GetRxFullHandleStart(hwch) != Can_GetRxFullHandleStop(hwch)) ? \
  (Can_GetHwHandle((Can_GetRxFullHandleStop(hwch)-1))+1) : (Can_GetHwHandle(Can_GetRxFullHandleStart(hwch))))

#  define CAN_ASR_HW_RX_BASIC_STARTINDEX(hwch) (Can_GetHwHandle(Can_GetRxBasicHandleStart(hwch)))
#  define CAN_ASR_HW_RX_BASIC_STOPINDEX(hwch)  ((Can_GetRxBasicHandleStart(hwch) != Can_GetRxBasicHandleStop(hwch)) ? \
  (Can_GetHwHandle((Can_GetRxBasicHandleStop(hwch)-1)) + Can_GetDLC_FIFO((Can_GetRxBasicHandleStop(hwch)-1))) : \
  (Can_GetHwHandle(Can_GetRxBasicHandleStart(hwch))))
# if !defined(CAN_HL_HW_TX_NORMAL_INDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_TX_NORMAL_INDEX(hwch)     CAN_ASR_HW_TX_NORMAL_INDEX(hwch)
# endif
# if !defined(CAN_HL_HW_TX_FULL_STARTINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_TX_FULL_STARTINDEX(hwch)  CAN_ASR_HW_TX_FULL_STARTINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_TX_FULL_STOPINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_TX_FULL_STOPINDEX(hwch)   CAN_ASR_HW_TX_FULL_STOPINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_TX_STARTINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_TX_STARTINDEX(hwch)       CAN_ASR_HW_TX_STARTINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_TX_STOPINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_TX_STOPINDEX(hwch)        CAN_ASR_HW_TX_STOPINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_UNUSED_STARTINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_UNUSED_STARTINDEX(hwch)   CAN_ASR_HW_UNUSED_STARTINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_UNUSED_STOPINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_UNUSED_STOPINDEX(hwch)    CAN_ASR_HW_UNUSED_STOPINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_RX_FULL_STARTINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_RX_FULL_STARTINDEX(hwch)  CAN_ASR_HW_RX_FULL_STARTINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_RX_FULL_STOPINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_RX_FULL_STOPINDEX(hwch)   CAN_ASR_HW_RX_FULL_STOPINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_RX_BASIC_STARTINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_RX_BASIC_STARTINDEX(hwch) CAN_ASR_HW_RX_BASIC_STARTINDEX(hwch)
# endif
# if !defined(CAN_HL_HW_RX_BASIC_STOPINDEX) /* COV_CAN_COMPATIBILITY  */
#  define CAN_HL_HW_RX_BASIC_STOPINDEX(hwch)  CAN_ASR_HW_RX_BASIC_STOPINDEX(hwch)
# endif
#endif
/* may be used by LL to free TX mailbox between CanLL_TxBegin and CanLL_TxEnd (only necessary when not using CanLL_TxStart return value) */
#define CAN_FREE_TX_OBJECT(obj) Can_SetActiveSendState(obj, CAN_FREE_OBJ)

#if defined(C_ENABLE_INDIVIDUAL_POLLING) || defined(CAN_HWOBJINDIVPOLLING) /* COV_CAN_HWOBJINDIVPOLLING */
/* define first index to array CanHwObjIndivPolling[][] */
#  define CAN_HWOBJINDIVPOLLING_INDEX0                     (canHwChannel)
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
# if !defined(CAN_RAM_CHECK_MAILBOX_RESULT) /* May be defined by test suite to stimulate RAM_CHECK failure */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#   define CAN_RAM_CHECK_MAILBOX_RESULT(ch, hwObjHandle) CanLL_IsMailboxCorrupt((hwObjHandle))
#  else
#   define CAN_RAM_CHECK_MAILBOX_RESULT(ch, hwObjHandle) CanLL_IsMailboxCorrupt((ch), (hwObjHandle))
#  endif
# endif
# if !defined(CAN_RAM_CHECK_FINISHED_CTP) /* May be defined by test suite to check RAM_CHECK finished / executed */
#  define CAN_RAM_CHECK_FINISHED_CTP(ch)
# endif
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
# if !defined(CAN_RAM_CHECK_BEGIN_REG_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#   define CAN_RAM_CHECK_BEGIN_REG_RESULT(ch) CanLL_InitBeginIsRegisterCorrupt()
#  else
#   define CAN_RAM_CHECK_BEGIN_REG_RESULT(ch) CanLL_InitBeginIsRegisterCorrupt((ch))
#  endif
# endif
# if !defined(CAN_RAM_CHECK_END_REG_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#   define CAN_RAM_CHECK_END_REG_RESULT(ch) CanLL_InitEndIsRegisterCorrupt()
#  else
#   define CAN_RAM_CHECK_END_REG_RESULT(ch) CanLL_InitEndIsRegisterCorrupt((ch))
#  endif
# endif
# if !defined(CAN_RAM_CHECK_READ_BACK_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  define CAN_RAM_CHECK_READ_BACK_RESULT(ch)
# endif
#endif

#define CanHL_IsStart(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_START) == CAN_STATUS_START)
#define CanHL_IsSleep(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_SLEEP) == CAN_STATUS_SLEEP)
#define CanHL_IsStop(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_STOP) == CAN_STATUS_STOP)
#define CanHL_IsBusOff(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_BUSOFF) == CAN_STATUS_BUSOFF)
#define CanHL_IsControllerInit(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_INIT) == CAN_STATUS_INIT)

#if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
# define cantVIdentityMsk uint16
# define CanSetActiveIdentity(identityLog) \
{ \
  canActiveIdentityLog = (identityLog); \
  canActiveIdentityMsk = (cantVIdentityMsk) (((cantVIdentityMsk) 1) << (identityLog)); \
}
# if defined V_ACTIVE_IDENTITY_MSK
#  undef V_ACTIVE_IDENTITY_MSK  /* this define from v_cfg.h is replaced and no more used (but header may included by other module) */ /* PRQA S 0841 */ /* MD_Can_0841 */
# endif
# define V_ACTIVE_IDENTITY_MSK                canActiveIdentityMsk
# if defined V_ACTIVE_IDENTITY_LOG
#  undef V_ACTIVE_IDENTITY_LOG  /* this define from v_cfg.h is replaced and no more used (but header may included by other module) */ /* PRQA S 0841 */ /* MD_Can_0841 */
# endif
# define V_ACTIVE_IDENTITY_LOG                canActiveIdentityLog
#endif

#if (CAN_GENERIC_PRETRANSMIT == STD_ON)
# define PDUINFO_PTR PduInfoPtr_var
#else
# define PDUINFO_PTR PduInfo
#endif
#if defined(C_ENABLE_CAN_FD_USED)
# define PDUINFO_ID  pduInfo_var_id
# define PDUINFO_DLC pduInfo_var_dlc
#else
# define PDUINFO_ID  PDUINFO_PTR->id
# define PDUINFO_DLC PDUINFO_PTR->length
#endif

#if !defined(CAN_APPLICATION_DEM_REPORTERROR) /* COV_CAN_COMPATIBILITY */
# define CAN_APPLICATION_DEM_REPORTERROR STD_OFF
#endif
#if (CAN_APPLICATION_DEM_REPORTERROR == STD_OFF) /* COV_CAN_USER_CONFIG */
# define Can_Dem_ReportError(EventId) ( (void) Dem_ReportErrorStatus((EventId), DEM_EVENT_STATUS_FAILED) )
#endif

#if defined(C_ENABLE_CAN_FD_USED) /* also for BRS because DLC may be bigger than 8 and will be checked by CanIf */
# define CAN_DECODE_DLI(dlc) (Can_DliToBytesMap[((uint8)(dlc)) & ((uint8)0xFu)])
# define CAN_ENCODE_DLI(length) (((uint8)Can_BytesToDliMap[(length)]) & ((uint8)0x0Fu))
#else
# define CAN_DECODE_DLI(dlc) (dlc)
# define CAN_ENCODE_DLI(length) (length)
#endif
#if defined(C_ENABLE_CAN_FD_FULL)
# define CAN_MAX_RX_DATALEN         Can_GetMaxRxDataLen()
# define CAN_MAX_TX_DATALEN         Can_GetMaxTxDataLen()
# define CAN_MAX_DATALEN_OBJ(Index) Can_GetMailboxDataLen(Index)
#else
# define CAN_MAX_RX_DATALEN         8u
# define CAN_MAX_TX_DATALEN         8u
# define CAN_MAX_DATALEN_OBJ(Index) 8u
#endif
#define CanHL_IsFdMessage(id) (((id) & (Can_IdType)CAN_ID_FD_MASK) == (Can_IdType)CAN_ID_FD_MASK) /* used for LL */ /* \trace SPEC-60432, SPEC-50586 */
#define CanHL_IsFdTxBrs(ch)   (Can_GetInitObjectFdBrsConfig(Can_GetLastInitObject(ch)) == CAN_FD_RXTX) /* used for LL */ /* \trace SPEC-60432, SPEC-50586 */

#if (CAN_MICROSAR_VERSION == CAN_MSR403) /* COV_CAN_MICROSAR_VERSION */
# define CAN_MULTIPLEXED_TX_MAX_OBJ(hth) ((Can_GetMailboxType(hth) == CAN_TX_BASICCAN_MUX_TYPE) ? CAN_MULTIPLEXED_TX_MAX : 1)
#else
# define CAN_MULTIPLEXED_TX_MAX_OBJ(hth) CAN_MULTIPLEXED_TX_MAX
#endif

/*
|<DataModelStart>| CanLL_TxIsHWObjFree
Relation_Context:
Relation:
ChannelAmount, TxBasicAmount
ChannelAmount, TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) \
                (Can_GetgCanController(channel).MsgObj[txObjHandle].mIsFree == (vuint8)kCanTrue)

/*
|<DataModelStart>| CanLL_HwIsSleep
Relation_Context:
Relation:
Wakeup, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#if defined( C_ENABLE_SLEEP_WAKEUP )
# define CanLL_HwIsSleep( CAN_CHANNEL_CANPARA_ONLY) (Can_GetgCanController(channel).Mode == CAN_MODE_SLEEP)
#else
# define CanLL_HwIsSleep( CAN_CHANNEL_CANPARA_ONLY) (kCanFalse != kCanFalse)  /* avoid misra warning */
#endif

/*
|<DataModelStart>| CanLL_HwIsStop
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_HwIsStop( CAN_HW_CHANNEL_CANPARA_ONLY ) (Can_GetgCanController(channel).Mode == CAN_MODE_STOP)

/*
|<DataModelStart>| CanLL_HwIsBusOff
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_HwIsBusOff( CAN_HW_CHANNEL_CANPARA_ONLY ) \
                         (Can_GetgCanController(channel).ErrCode == (vuint32)CANCELL_BUSOFF)

#if defined( C_ENABLE_EXTENDED_STATUS )
/*
|<DataModelStart>| CanLL_HwIsPassive
Relation_Context:
Relation:
CanGetStatus, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# define CanLL_HwIsPassive( CAN_HW_CHANNEL_CANPARA_ONLY ) \
                           (Can_GetgCanController(channel).ErrCode == (vuint32)CANCELL_ERRORPASSIVE)

/*
|<DataModelStart>| CanLL_HwIsWarning
Relation_Context:
Relation:
CanGetStatus, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# define CanLL_HwIsWarning( CAN_HW_CHANNEL_CANPARA_ONLY )  \
                           (Can_GetgCanController(channel).ErrCode == (vuint32)CANCELL_WARNINGLIMIT)
#endif /* C_ENABLE_EXTENDED_STATUS */

#if !defined(CAN_HWOBJINDIVPOLLING_INDEX0) /* CBD only */
# define CAN_HWOBJINDIVPOLLING_INDEX0                     (channel)
#endif

#if !defined(CAN_HL_INFO_INIT) /* COV_CAN_COMPATIBILITY */
#define CAN_HL_INFO_INIT(member)     member
#define CAN_HL_INFO_INIT_PTR(member) member
#endif
#if !defined(CAN_HL_INFO_RXBASIC) /* COV_CAN_COMPATIBILITY */
#define CAN_HL_INFO_RXBASIC(member)     member
#define CAN_HL_INFO_RXBASIC_PTR(member) member
#endif

/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/
#define CAN_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* BR:012 */
V_DEF_CONST(V_NONE, uint8, CONST) Can_MainVersion         = (uint8)CAN_SW_MAJOR_VERSION; /* PRQA S 3408 */ /* MD_Can_Asr_3408_libCheck */
V_DEF_CONST(V_NONE, uint8, CONST) Can_SubVersion          = (uint8)CAN_SW_MINOR_VERSION; /* PRQA S 3408 */ /* MD_Can_Asr_3408_libCheck */
V_DEF_CONST(V_NONE, uint8, CONST) Can_ReleaseVersion      = (uint8)CAN_SW_PATCH_VERSION; /* PRQA S 3408 */ /* MD_Can_Asr_3408_libCheck */

#if defined(C_ENABLE_CAN_FD_USED)
V_DEF_CONST(CAN_STATIC, uint8, CONST) Can_DliToBytesMap[16] = /* PRQA S 3218 */ /* MD_Can_3218 */
{
  0,  1,  2,  3,
  4,  5,  6,  7,
  8, 12, 16, 20,
 24, 32, 48, 64
};
V_DEF_CONST(CAN_STATIC, uint8, CONST) Can_BytesToDliMap[65] = /* PRQA S 3218 */ /* MD_Can_3218 */
{
/* 00..07 */  0,  1,  2,  3,  4,  5,  6,  7,
/* 08..15 */  8,  9,  9,  9,  9, 10, 10, 10,
/* 16..23 */ 10, 11, 11, 11, 11, 12, 12, 12,
/* 24..31 */ 12, 13, 13, 13, 13, 13, 13, 13,
/* 32..39 */ 13, 14, 14, 14, 14, 14, 14, 14,
/* 40..47 */ 14, 14, 14, 14, 14, 14, 14, 14,
/* 48..55 */ 14, 15, 15, 15, 15, 15, 15, 15,
/* 56..63 */ 15, 15, 15, 15, 15, 15, 15, 15,
/* 64     */ 15
};
#endif

/* Global constants with CAN driver main and subversion */

#define CAN_STOP_SEC_CONST_UNSPECIFIED  /*---------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***************************************************************************/
/* CAN-Hardware Data Definitions                                           */
/***************************************************************************/

/***************************************************************************/
/* global data definitions                                                 */
/***************************************************************************/
#define CAN_START_SEC_VAR_NOINIT_UNSPECIFIED  /*---------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
/*! bitmask of the active channels in the current identity */
V_DEF_VAR(V_NONE, uint16, VAR_NOINIT) canActiveIdentityMsk; /* PRQA S 3408 */ /* MD_Can_Asr_3408 */
/*! the activated identity handle */
V_DEF_VAR(V_NONE, uint8, VAR_NOINIT)  canActiveIdentityLog; /* PRQA S 3408 */ /* MD_Can_Asr_3408 */
#endif

/* The emulated CAN-Controller */

/***************************************************************************************/
/* local data definitions   (need also INFIX because STATIC may be defined to nothing) */
/***************************************************************************************/
/* \trace SPEC-1585 */
/*! Semaphore to block reentrancy for Can_MainFunction_Read() */
V_DEF_VAR(CAN_STATIC, Can_ReturnType, VAR_NOINIT) canRxTaskActive;
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
# endif
#endif
#if defined(C_ENABLE_RX_QUEUE)
V_DEF_VAR(CAN_STATIC, uint16, VAR_NOINIT) canRxQueueMaxDataSize; /* PRQA S 3218 */ /* MD_Can_3218 */
#endif

#define CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /*----------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define CAN_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* ! Mark module to be initialized (used to check double/none initialization) */
V_DEF_VAR(CAN_STATIC, uint8, VAR_INIT) canConfigInitFlag = CAN_STATUS_UNINIT;
# define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* not static because of debug/test usage */
#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER)
# define CAN_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*! Pointer to the current configuration */
V_DEF_P2CONST (V_NONE, Can_ConfigType, VAR_INIT, CONST_PBCFG) Can_ConfigDataPtr = NULL_PTR; /* UREQ00035484 */ /* PRQA S 3408,1504 */ /* MD_Can_3408_extLinkage */
# define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#else
# define CAN_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
/*! Pointer to the current configuration */
V_DEF_CONSTP2CONST(V_NONE, Can_ConfigType, CONST, CONST_PBCFG) Can_ConfigDataPtr = NULL_PTR; /* PRQA S 3408,3218,1514 */ /* MD_Can_3408_extLinkage,MD_Can_3218 */
# else
/*! Pointer to the current configuration */
V_DEF_CONSTP2CONST(V_NONE, Can_ConfigType, CONST, CONST_PBCFG) Can_ConfigDataPtr = &CanConfig; /* PRQA S 3408,3218,1514 */ /* MD_Can_3408_extLinkage,MD_Can_3218 */
# endif
# define CAN_STOP_SEC_CONST_UNSPECIFIED  /*---------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/***************************************************************************/
/*  Safe Check                                                             */
/***************************************************************************/
#if !defined(CAN_SAFE_BSW) /* COV_CAN_COMPATIBILITY */
# define CAN_SAFE_BSW   STD_OFF
#else
# if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
#  if defined(C_ENABLE_MULTI_ECU_CONFIG) || \
     !defined(CAN_DEV_ERROR_DETECT) || \
     defined(CAN_ENABLE_TRACING) || \
     defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) || \
     !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) || \
     defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) || \
     defined(C_ENABLE_PARTIAL_NETWORK) || \
     defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) || \
     defined(CAN_ENABLE_BUSOFF_SUPPORT_API) || \
     defined(CAN_ENABLE_EXTENDED_STATE_MACHINE) || \
     (CAN_EMULATE_SLEEP != STD_ON) || \
     !defined(CAN_ENABLE_DYNAMIC_FULLCAN_ID) || \
     (CAN_AMD_RUNTIME_MEASUREMENT == STD_ON) || \
     !defined(C_ENABLE_GEN_HW_START_STOP_IDX) /* COV_CAN_SAFE_BSW_EXCLUSION */
#   error "Unsupported Feature activated for SafeBSW"
#  endif
# endif
#endif

/***************************************************************************/
/*  Error Check                                                            */
/***************************************************************************/
#if (CAN_TRANSMIT_BUFFER == STD_OFF) && defined(C_ENABLE_CANCEL_IN_HW)
# error "C_ENABLE_CANCEL_IN_HW need CAN_TRANSMIT_BUFFER == STD_ON (no FIFO buffering but PRIO_BY_CANID in CanIf) -> check Generated data"
#endif
#if !defined(C_ENABLE_SLEEP_WAKEUP)
# if (CAN_SLEEP_SUPPORT == STD_ON) && (CAN_WAKEUP_SUPPORT == STD_ON)
#  error "C_ENABLE_SLEEP_WAKEUP organified but generated like supported"
# endif
#endif
#if (CAN_WAKEUP_SUPPORT == STD_ON) && (CAN_SLEEP_SUPPORT == STD_OFF)
# error "activated CAN_WAKEUP_SUPPORT need activated CAN_SLEEP_SUPPORT"
#endif
# if (CAN_HARDWARE_CANCELLATION == STD_ON)
#  error "C_ENABLE_HW_LOOP_TIMER organified but generated like supported"
# endif
#if !defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG TX */
# if (CAN_MULTI_ECU_CONFIG == STD_ON)
#  error "C_ENABLE_MULTI_ECU_CONFIG organified but generated like supported"
# endif
#endif
#if defined(C_ENABLE_CANCEL_IN_HW)
#else
# if (CAN_HW_TRANSMIT_CANCELLATION == STD_ON)
#  error "C_ENABLE_CANCEL_IN_HW organified but generated like supported"
# endif
#endif
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
#  if (CAN_DEV_ERROR_DETECT == STD_OFF)
#   error "CAN_DEV_TIMEOUT_DETECT is switched on but CAN_DEV_ERROR_DETECT is not"
#  endif
# endif
#endif
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
# if defined(C_ENABLE_PARTIAL_NETWORK) /* COV_CAN_PARTIAL_NETWORK */
#  error "No need to switch on partial Network for MicroSar in version 4.x"
# endif
# if defined(CAN_ENABLE_BUSOFF_SUPPORT_API) /* COV_CAN_BUSOFF_SUPPORT_API */
#  error "BUSOFF_SUPPORT_API not supported for MicroSar in version 4.x"
# endif
# if defined(CAN_ENABLE_EXTENDED_STATE_MACHINE) /* COV_CAN_EXTENDED_STATE_MACHINE */
#  error "CAN_EXTENDED_STATE_MACHINE not supported for MicroSar in version 4.x"
# endif
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
#  error "CAN_DEV_TIMEOUT_DETECT not supported for MicroSar in version 4.x"
# endif
#endif
#  if defined(C_ENABLE_PARTIAL_NETWORK) /* COV_CAN_PARTIAL_NETWORK */
#   error "Feature 'partial network' (CanPnEnable) need 'Hardware Loop Check'"
#  endif
#if (CAN_MIRROR_MODE == STD_ON) && (!defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2) || (CAN_GENERIC_PRECOPY == STD_OFF))
# error "CAN_MIRROR_MODE need CAN_GENERIC_CONFIRMATION with CAN_API2 and CAN_GENERIC_PRECOPY as STD_ON"
#endif
#if !defined(CAN_MULTIPLEXED_TX_MAX)
# error "CAN_MULTIPLEXED_TX_MAX is not generated"
#endif
#if (CAN_MULTIPLEXED_TRANSMISSION == STD_ON)
# if (CAN_MULTIPLEXED_TX_MAX != 3) /* CM_CAN_HL19 */
#  error "CAN_MULTIPLEXED_TX_MAX generated with wrong value"
# endif
#else
# if (CAN_MULTIPLEXED_TX_MAX != 1) /* CM_CAN_HL19 */
#  error "CAN_MULTIPLEXED_TX_MAX generated with wrong value"
# endif
#endif
#if (CAN_RAM_CHECK != CAN_NONE)
# if defined(CAN_ENABLE_EXTENDED_STATE_MACHINE) /* COV_CAN_EXTENDED_STATE_MACHINE */
# error "Feature RAM_CHECK is not available in combination with EXTENDED_STATE_MACHINE"
/* no STOP mode called after WAKEUP but need to run RAM check */
# endif
#endif
#if (CAN_MULTIPLE_BASICCAN_TX == STD_ON)
# if (CAN_HW_TRANSMIT_CANCELLATION == STD_ON)
/* cancellation does not work in CanHL_WritePrepareCancelTx for multiple BasicCAN messages, and is not useful for FIFO queue in CanIf (normally combined with multiple BCAN)*/
# error "CAN_MULTIPLE_BASICCAN_TX and CAN_HW_TRANSMIT_CANCELLATION are STD_ON but this combination is not supported"
# endif
#endif

#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
#  error "Extended RamCheck is only available for MICROSAR403"
# endif
#endif

/***************************************************************************/
/*  Functions                                                              */
/***************************************************************************/
#define CAN_START_SEC_STATIC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* no HW loop is used at all */
# define CanHL_ApplCanTimerStart( timerIdx )
# define CanHL_ApplCanTimerLoop( timerIdx )
# define CanHL_ApplCanTimerEnd( timerIdx )

/****************************************************************************
| NAME:             CanInit
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanInit
Relation_Context:
Relation:
OneChOpt
ChannelAmount
BusOffRecovery
HardwareLoop
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanInit( CAN_CHANNEL_CANTYPE_FIRST uint8 initObject ) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Development Error Checks ------------------------------------- */
  /* parameter initObject is already checked by caller */
  /* parameter channel is already checked by caller */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Reinitialize the controller (Call fitting controller initialization function Can_InitController()) */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
    Can_InitController((uint8)channel, &Can_GetControllerBaudrateConfig(((uint8)(initObject - (uint8)(Can_GetInitObjectStartIndex(channel))) & 0x03u))); /* SBSW_CAN_HL03 */
#else
    Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
    CAN_DUMMY_STATEMENT(initObject); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if defined(CAN_ENABLE_BUSOFF_SUPPORT_API) /* COV_CAN_BUSOFF_SUPPORT_API */
    /* #110 Switch to START mode if BUSOFF handling is done by application */
    /* normally CanResetBusOffStart/STOP (and CanResetBusSleep) use this call so no other action is needed.
       But when application do this call the state has to be set to START here because it will not be done in SetControllerMode(). */
    { 
      (void)Can_SetControllerMode((uint8)channel, CAN_T_START);
    }
#endif
  }
}

/****************************************************************************
| NAME:             Can_CleanUpSendState
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| Can_CleanUpSendState
Relation_Context:
Relation:
OneChOpt
MultiplexedTx
CommonCan
IfTxBuffer
TxFullCANSupport
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) Can_CleanUpSendState( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle logTxObjHandle;
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
  CanObjectHandle logTxObjHandleStop;
# endif
  Can_HwHandleType Htrh;
  /* ----- Implementation ----------------------------------------------- */
  /* parameter channel is already checked by caller */
  /* #10 set all BasicCAN TX Objects to state FREE to allow a new transmission */
  Htrh = Can_GetTxBasicHandleStart(canHwChannel);
# if defined(C_ENABLE_MULTIPLE_BASICCAN_TX)
  for (; Htrh < Can_GetTxBasicHandleStop(canHwChannel); Htrh++)
# endif
  {
    logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST Can_GetHwHandle(Htrh));
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
    logTxObjHandleStop = logTxObjHandle + (Can_HwHandleType)CAN_MULTIPLEXED_TX_MAX_OBJ(Htrh); /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    for (; logTxObjHandle < logTxObjHandleStop; logTxObjHandle++)
# endif
    {
      Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
    }
  }
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
  /* #20 set all FullCAN TX Objects to state FREE to allow a new transmission */
  for (Htrh = Can_GetTxFullHandleStart(canHwChannel); Htrh < Can_GetTxFullHandleStop(canHwChannel); Htrh++)
  {
    logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST Can_GetHwHandle(Htrh));
    Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */ 
  }
# endif
}

/****************************************************************************
| NAME:             CanHL_StartTransition
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_StartTransition
Relation_Context:
Relation:
OneChOpt
CommonCan
HardwareLoop
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_StartTransition( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType transitionRequest;
  Can_ReturnType retval = CAN_NOT_OK; /* PRQA S 3197 */ /* MD_MSR_14.2 */
  /* ----- Implementation ----------------------------------------------- */
  { /* #20 HW request transition (LL) \trace SPEC-1628 */
    /*
    |<DataModelStart>| CanLL_StartRequest
    Relation_Context:
    Relation:
    SilentMode
    ChannelAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    #if defined(C_ENABLE_SILENT_MODE)
    /* Just to switch SilentMode */
    if (Can_GetSilentModeState(channel) == CAN_SILENT_ACTIVE)
    {
      CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, CANOEAPI_CAN_SILENTMODE);  /* PRQA S 5013,0310,0311,1334,432 */ /* MD_Can_5013_LL, MD_Can_0310, MD_Can_0311, MD_Can_1334, MD_Can_1334 */ /* COV_CAN_LL_EMULATION */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */
    }
    else
    #endif
    {
      CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, CANOEAPI_CAN_NORMALMODE);  /* PRQA S 5013,0310,0311,1334,432 */ /* MD_Can_5013_LL, MD_Can_0310, MD_Can_0311, MD_Can_1334, MD_Can_1334 */ /* COV_CAN_LL_EMULATION */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */
    }
    CanCell_StartChannel((const char*)Can_GetBusName(channel), channel); /* PRQA S 5013 */ /* MD_Can_5013_LL */
    transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_START) ? kCanOk : kCanRequested; /* COV_CAN_LL_MODE_CHANGE */
    if ( transitionRequest == CAN_REQUESTED ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_START */
    { /* PRQA S 3201 */ /* MD_Can_3201 */
      CanHL_ApplCanTimerStart(kCanLoopStart); /* SBSW_CAN_HL38 */
      /* #40 start Loop that wait (ASR4: short time) for transition \trace SPEC-1642, SPEC-1635, SPEC-1630 */
      do
      { /* #45 HW check mode reached (LL) */
        /*
        |<DataModelStart>| CanLL_StartProcessed
        Relation_Context:
        Relation:
        ChannelAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        Can_GetgCanController(channel).Mode = CAN_MODE_START; /* SBSW_CAN_LL08 */
        transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_START) ? kCanOk : kCanFailed; /* COV_CAN_LL_MODE_CHANGE */
        CanHL_ApplCanTimerLoop(kCanLoopStart); /* COV_CAN_TRANSITION_REQUEST_START */
      } while ( transitionRequest != CAN_OK ); /* COV_CAN_TRANSITION_REQUEST_START */
      CanHL_ApplCanTimerEnd (kCanLoopStart); /* SBSW_CAN_HL38 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */ 
      if ( transitionRequest != CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_START */
      { /* PRQA S 3201 */ /* MD_Can_3201 */
#  if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
        /* #50 ASR4: transition failed so return OK but repeat it asynchronous */
        Can_SetStatusReq(channel, CAN_REQ_START); /* Asynchronous polling on */ /* SBSW_CAN_HL01 */
        retval = CAN_OK; /* PRQA S 3197,3198 */ /* MD_MSR_14.2 */
#  endif
      }
# endif
    }
  } /* loop canHwChannel */
  if ( transitionRequest == CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_START */
  { /* PRQA S 3201 */ /* MD_Can_3201 */
    /* #70 transition finished synchronous return OK and change internal state */
    CanHookStateChange_StartSync(Can_GetLogStatus(channel), (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_START) );
    Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_START)); /* SBSW_CAN_HL02 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
    /* #80 allow reinitialization in notification */
    Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
    /* #90 ASR4: call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
    CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), CANIF_CS_STARTED );
#endif
    retval = CAN_OK;
  }
  return retval;
} /* PRQA S 6010,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL */

/****************************************************************************
| NAME:             CanHL_StopTransition
****************************************************************************/
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
 */
/*
|<DataModelStart>| CanHL_StopTransition
Relation_Context:
Relation:
OneChOpt
CommonCan
HardwareLoop
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_StopTransition( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK; /* PRQA S 3197 */ /* MD_MSR_14.2 */
  /* ----- Implementation ----------------------------------------------- */
  /* #20 ESCAN00073272 need Can_InitController() (in case of HW failure) */
  if( (Can_GetLogStatus(channel) & (CAN_STATUS_STOP | CAN_STATUS_START)) == 0 ) /* COV_CAN_HARDWARE_FAILURE */
  {
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
    Can_InitController((uint8)channel, &Can_GetControllerBaudrateConfig(((uint8)(Can_GetLastInitObject(channel) - (uint8)Can_GetInitObjectStartIndex(channel)) & 0x03u))); /* SBSW_CAN_HL03 */
#else
    Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
#endif
  }
  {
    /* #25 set STOP request if not done in Can_InitController() before */
    Can_ReturnType transitionRequest;
    { /* #40 HW request transition (LL) \trace SPEC-1632 */
      /*
      |<DataModelStart>| CanLL_StopRequest
      Relation_Context:
      Relation:
      ChannelAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, CANOEAPI_CAN_SILENTMODE);  /* PRQA S 5013,0310,0311,1334,432 */ /* MD_Can_5013_LL, MD_Can_0310, MD_Can_0311, MD_Can_1334, MD_Can_1334 */ /* COV_CAN_LL_EMULATION */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */
      CanCell_StopChannel((const char*)Can_GetBusName(channel), channel); /* PRQA S 5013 */ /* MD_Can_5013_LL */
      transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_STOP) ? kCanOk : kCanRequested; /* COV_CAN_LL_MODE_CHANGE */
      if ( transitionRequest == CAN_REQUESTED ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_STOP */
      { /* PRQA S 3201 */ /* MD_Can_3201 */
        CanHL_ApplCanTimerStart(kCanLoopStop); /* SBSW_CAN_HL38 */
        /* #60 start Loop that wait (ASR4: short time) for transition \trace SPEC-1636, SPEC-1635, SPEC-1630 */
        do
        { /* #65 HW check mode reached (LL) */
          /*
          |<DataModelStart>| CanLL_StopProcessed
          Relation_Context:
          Relation:
          ChannelAmount
          Parameter_PreCompile:
          Parameter_Data:
          Constrain:
          |<DataModelEnd>|
          */
          Can_GetgCanController(channel).Mode = CAN_MODE_STOP; /* SBSW_CAN_LL08 */
          transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_STOP) ? kCanOk : kCanFailed; /* COV_CAN_LL_MODE_CHANGE */
          CanHL_ApplCanTimerLoop(kCanLoopStop); /* COV_CAN_TRANSITION_REQUEST_STOP */
        } while ( transitionRequest != CAN_OK ); /* COV_CAN_TRANSITION_REQUEST_STOP */
        CanHL_ApplCanTimerEnd(kCanLoopStop); /* SBSW_CAN_HL38 */
#  if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */ 
        if ( transitionRequest != CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_STOP */
        { /* PRQA S 3201 */ /* MD_Can_3201 */
#   if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
          /* #70 ASR4: transition failed so return OK but repeat it asynchronous */
          Can_SetStatusReq(channel, CAN_REQ_STOP); /* Asynchronous polling on */ /* SBSW_CAN_HL01 */
          retval = CAN_OK;
#   endif
        }
#  endif
      }
    } /* loop canHwChannel */
    if ( transitionRequest == CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_STOP */
    { /* PRQA S 3201 */ /* MD_Can_3201 */
      /* #110 transition finished synchronous return OK and change internal state */
      CanHookStateChange_StopSync(Can_GetLogStatus(channel), (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP) );
      retval = CAN_OK;
      { /* #120 Use short initialization instead of Can_InitController() (LL) \trace SPEC-1627, SPEC-1646, SPEC-3965 */
        CanHL_StopReinit(CAN_HW_CHANNEL_CANPARA_ONLY);
      }
      Can_CleanUpSendState(CAN_CHANNEL_CANPARA_ONLY);
      Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP)); /* SBSW_CAN_HL02 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
      /* #130 allow reinitialization in notification */
      Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
      /* #140 ASR4: call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
      CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), CANIF_CS_STOPPED );
#endif
    }
  }
  return retval;
} /* PRQA S 6010,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL */

/****************************************************************************
| NAME:             CanHL_WakeupTransition
****************************************************************************/
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
 */
/*
|<DataModelStart>| CanHL_WakeupTransition
Relation_Context:
Relation:
OneChOpt
Wakeup
CommonCan
HardwareLoop
RamCheck, CommonCan
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_WakeupTransition( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  /* ----- Implementation ----------------------------------------------- */
#if defined(C_ENABLE_SLEEP_WAKEUP)
  if (Can_GetWakeupSourceRef(channel) != 0) /* COV_CAN_WAKEUP_CHANNEL_NOT_IN_ALL_CONFIG */
  { /* #5 hardware is only in SLEEP if WAKEUP over bus is active and supported -> otherwise emulated SLEEP */
    /* #7 WAKEUP: set request and wait limited time and notify upper Layer (do RAM check) */
    Can_ReturnType transitionRequest;
    retval = CAN_NOT_OK; /* PRQA S 3197,3198 */ /* MD_MSR_14.2 */
    { /* #20 HW request transition (LL) */
      /*
      |<DataModelStart>| CanLL_WakeUpRequest
      Relation_Context:
      Wakeup
      Relation:
      ChannelAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CanCell_WakeupChannel((const char*)Can_GetBusName(channel), channel); /* PRQA S 5013 */ /* MD_Can_5013_LL */
      transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_STOP) ? kCanOk : kCanRequested; /* COV_CAN_LL_MODE_CHANGE */
      if ( transitionRequest == CAN_REQUESTED ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_WAKEUP */
      { /* PRQA S 3201 */ /* MD_Can_3201 */
        CanHL_ApplCanTimerStart(kCanLoopWakeup); /* SBSW_CAN_HL38 */
        /* #40 start Loop that wait (ASR4: short time) for transition \trace SPEC-1643, SPEC-1635, SPEC-1630 */
        do
        { /* #45 HW check mode reached (LL) */
          /*
          |<DataModelStart>| CanLL_WakeUpProcessed
          Relation_Context:
          Wakeup
          Relation:
          ChannelAmount
          Parameter_PreCompile:
          Parameter_Data:
          Constrain:
          |<DataModelEnd>|
          */
          Can_GetgCanController(channel).Mode = CAN_MODE_STOP; /* SBSW_CAN_LL08 */
          transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_STOP) ? kCanOk : kCanFailed; /* COV_CAN_LL_MODE_CHANGE */
          CanHL_ApplCanTimerLoop(kCanLoopWakeup); /* COV_CAN_TRANSITION_REQUEST_WAKEUP */
        } while ( transitionRequest != CAN_OK ); /* COV_CAN_TRANSITION_REQUEST_WAKEUP */
        CanHL_ApplCanTimerEnd(kCanLoopWakeup); /* SBSW_CAN_HL38 */
#  if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */ 
        if ( transitionRequest != CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_WAKEUP */
        { /* PRQA S 3201 */ /* MD_Can_3201 */
#   if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
          /* #50 ASR4: transition failed so return OK but repeat it asynchronous */
          Can_SetStatusReq(channel, CAN_REQ_WAKEUP); /* Asynchronous polling on */ /* SBSW_CAN_HL01 */
          retval = CAN_OK;
#   endif
        }
#  endif
      }
    } /* loop canHwChannel */
    { /* PRQA S 3201 */ /* MD_Can_3201 */
      /* #70 transition finished synchronous return OK and change internal state */
      CanHookStateChange_WakeupSync(Can_GetLogStatus(channel), (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP) );
# if defined(C_ENABLE_CAN_RAM_CHECK) && !defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
      if (!CanHL_IsStop(channel)) { /* do not call RAM_CHECK again in case CanIf call wakeup a second time (buswakeup + stop transition afterwards) */
        /* #80 do standard RAM check CanHL_DoRamCheck() */
        Can_SetLogStatus(channel, CAN_STATUS_UNINIT); /* Like PowerOn */ /* SBSW_CAN_HL02 */
        CanHL_DoRamCheck(CAN_CHANNEL_CANPARA_ONLY);
      }
# else
      { /* #90 Use short initialization instead of Can_InitController() (LL) */
        CanHL_StopReinit(CAN_HW_CHANNEL_CANPARA_ONLY);
      }
# endif
      /* #100 remove SLEEP and set to STOP */
      Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP)); /* SBSW_CAN_HL02 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
      /* #110 allow reinitialization in notification */
      Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
      /* #120 ASR4: call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60426, SPEC-60446 */
      CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), CANIF_CS_STOPPED );
# endif
      retval = CAN_OK;
    }

  }
  else /* no WAKEUP-source-ref: do emulated SLEEP mode */
#endif
  { /* #130 emulated SLEEP (CAN_NOT_SUPPORTED is CAN_OK) (stay in STOP mode instead) \trace SPEC-1638 */
    /* #140 transition finished synchronous return OK and change internal state */
    retval = CAN_OK;
#if defined(C_ENABLE_CAN_RAM_CHECK) && !defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
      /* #150 do standard RAM check CanHL_DoRamCheck() */
    Can_SetLogStatus(channel, CAN_STATUS_UNINIT); /* Like PowerOn */ /* SBSW_CAN_HL02 */
    CanHL_DoRamCheck(CAN_CHANNEL_CANPARA_ONLY);
#endif
    /* #160 delete SLEEP for CAN_EMULATE_SLEEP and set to STOP \trace SPEC-1653 */
    Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP)); /* SBSW_CAN_HL02 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
    /* #170 allow reinitialization in notification */
    Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
    /* #180 ASR4: call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
    CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), CANIF_CS_STOPPED );
#endif
  }
  return retval;
} /* PRQA S 6010,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL */

#if defined(C_ENABLE_CAN_RAM_CHECK)
/****************************************************************************
| NAME:             CanHL_DoRamCheck
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_DoRamCheck
Relation_Context:
Wakeup
RamCheck
Relation:
RamCheck
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_DoRamCheck( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* SLEEP is already set as logical status for first WAKEUP after SLEEP (changed to STOP after this) */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
  /* #20 remove mark ongoing state change (force STOP transition in Can_InitController()) */
  Can_SetStatusReq(channel, Can_GetStatusReq(channel) & (uint8)(~(CAN_REQ_INIT_NO_STOP))); /* PRQA S 0277 */ /* MD_Can_0277_negation */ /* SBSW_CAN_HL01 */
# endif
  Can_SetStatusReq(channel, Can_GetStatusReq(channel) | CAN_REQ_RAM_CHECK); /* SBSW_CAN_HL01 */
  /* #30 call Can_InitController() to perform the RAM check */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
  Can_InitController((uint8)channel, &Can_GetControllerBaudrateConfig(((uint8)(Can_GetLastInitObject(channel) - (uint8)Can_GetInitObjectStartIndex(channel)) & 0x03u))); /* SBSW_CAN_HL03 */
# else
  Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
# endif
}
#endif /* C_ENABLE_CAN_RAM_CHECK */

/****************************************************************************
| NAME:             CanHL_SleepTransition
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_SleepTransition
Relation_Context:
Relation:
Wakeup, CommonCan
Wakeup, ExtendedStateMachine
Wakeup, PnEnable
Wakeup, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_SleepTransition( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK; /* PRQA S 3197 */ /* MD_MSR_14.2 */
  /* ----- Implementation ----------------------------------------------- */
#if defined(C_ENABLE_SLEEP_WAKEUP)
  /* #10 Transition only when WAKEUP by bus is configured, otherwise use emulated SLEEP \trace SPEC-1633 */
  if (Can_GetWakeupSourceRef(channel) != 0) /* COV_CAN_WAKEUP_CHANNEL_NOT_IN_ALL_CONFIG */
  {
    Can_ReturnType transitionRequest;
# if defined(CAN_ENABLE_EXTENDED_STATE_MACHINE) /* COV_CAN_EXTENDED_STATE_MACHINE */
    /* #20 call CanResetBusSleep() for ENABLE_EXTENDED_STATE_MACHINE */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
    CanResetBusSleep(channel, Can_GetLastInitObject(channel));
#  else
    CanResetBusSleep(Can_GetLastInitObject(channel));
#  endif
# endif

# if defined(C_ENABLE_PARTIAL_NETWORK) /* COV_CAN_PARTIAL_NETWORK */
    if (Can_GetPartialNetwork(channel) == TRUE)
    { /* #30 for partial network channels deactivate DET notification in HW-loop */
      Can_SetTimeoutCall(channel,FALSE); /* SBSW_CAN_HL13 */
    }
# endif
    { /* #50 HW request transition (LL) */ /* \trace SPEC-1639 */
      /*
      |<DataModelStart>| CanLL_SleepRequest
      Relation_Context:
      Wakeup
      Relation:
      ChannelAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, CANOEAPI_CAN_NORMALMODE);  /* PRQA S 5013,0310,0311,1334,432 */ /* MD_Can_5013_LL, MD_Can_0310, MD_Can_0311, MD_Can_1334, MD_Can_1334 */ /* COV_CAN_LL_EMULATION */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */
      CanCell_SleepChannel((const char*)Can_GetBusName(channel), channel); /* PRQA S 5013 */ /* MD_Can_5013_LL */
      transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_SLEEP) ? kCanOk : kCanRequested; /* COV_CAN_LL_MODE_CHANGE */
      if ( transitionRequest == CAN_REQUESTED ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_SLEEP */
      { /* PRQA S 3201 */ /* MD_Can_3201 */
        CanHL_ApplCanTimerStart(kCanLoopSleep); /* SBSW_CAN_HL38 */
        /* #70 start Loop that wait (ASR4: short time) for transition \trace SPEC-1651, SPEC-1635, SPEC-1630 */
        do
        { /* #80 HW check mode reached (LL) */
          /*
          |<DataModelStart>| CanLL_SleepProcessed
          Relation_Context:
          Wakeup
          Relation:
          ChannelAmount
          Parameter_PreCompile:
          Parameter_Data:
          Constrain:
          |<DataModelEnd>|
          */
          Can_GetgCanController(channel).Mode = CAN_MODE_SLEEP; /* SBSW_CAN_LL08 */
          transitionRequest = (Can_GetgCanController(channel).Mode == CAN_MODE_SLEEP) ? kCanOk : kCanFailed; /* COV_CAN_LL_MODE_CHANGE */
          CanHL_ApplCanTimerLoop(kCanLoopSleep); /* COV_CAN_TRANSITION_REQUEST_SLEEP */
        } while ( transitionRequest != CAN_OK ); /* COV_CAN_TRANSITION_REQUEST_SLEEP */
        CanHL_ApplCanTimerEnd(kCanLoopSleep); /* SBSW_CAN_HL38 */
#  if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */ 
        if ( transitionRequest != CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_SLEEP */
        { /* PRQA S 3201 */ /* MD_Can_3201 */
#   if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
          /* #90 ASR4: transition failed so return OK but repeat it asynchronous */
          Can_SetStatusReq(channel, CAN_REQ_SLEEP); /* Asynchronous polling on */ /* SBSW_CAN_HL01 */
          retval = CAN_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
#   endif
        }
#  endif
      }
    } /* loop canHwChannel */
# if defined(C_ENABLE_PARTIAL_NETWORK) /* COV_CAN_PARTIAL_NETWORK */
    /* #110 partial network: activate DET notification again */
    Can_SetTimeoutCall(channel,TRUE); /* SBSW_CAN_HL13 */
# endif
    if ( transitionRequest == CAN_OK ) /* PRQA S 3355,3356,3358,3359 */ /* MD_Can_13.7,MD_Can_13.7,MD_Can_13.7,MD_Can_13.7 */ /* COV_CAN_TRANSITION_REQUEST_SLEEP */
    /* #120 transition finished synchronous return OK and change internal state */
    { /* PRQA S 3201 */ /* MD_Can_3201 */
      CanHookStateChange_SleepSync(Can_GetLogStatus(channel), (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_SLEEP) );
      Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_SLEEP)); /* SBSW_CAN_HL02 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
      /* #130 allow reinitialization in notification */
      Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
      /* #140 ASR4: call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
      CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), CANIF_CS_SLEEP );
# endif
      retval = CAN_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
    }
  }
  else 
#endif
  /* #150 no WAKEUP-source-ref: do emulated SLEEP mode \trace SPEC-1629, SPEC-1641, SPEC-1645, SPEC-3914, SPEC-3927 */
  {
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
    /* #160 controller stay in STOP mode / CAN_NOT_SUPPORTED is CAN_OK */
    retval = CAN_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
#endif
#if (CAN_EMULATE_SLEEP == STD_ON) /* COV_CAN_EMULATE_SLEEP */
    retval = CAN_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
    Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_SLEEP)); /* SBSW_CAN_HL02 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
    /* #170 allow reinitialization in notification */
    Can_SetStatusReq(channel, Can_GetStatusReq(channel)& (uint8)(~(CAN_REQ_INIT_NO_STOP))); /* PRQA S 0277 */ /* MD_Can_0277_negation */ /* SBSW_CAN_HL01 */
    /* #180 ASR4: call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
    CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), CANIF_CS_SLEEP );
# endif
#endif
  }
#if !defined(C_ENABLE_SLEEP_WAKEUP)
# if (CAN_EMULATE_SLEEP == STD_OFF) /* COV_CAN_EMULATE_SLEEP */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
# endif
#endif
  return retval;
} /* PRQA S 6010,6030,6050,6080*/ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#if defined(C_ENABLE_CANCEL_IN_HW)
/****************************************************************************
| NAME:             CanHL_IsHigherCanIdPrio
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_IsHigherCanIdPrio
Relation_Context:
HwCancelTx
HwCancelTx, MultiplexedTx
HwCancelTx, IdenticalIdCancel
Relation:
IDType
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_IsHigherCanIdPrio(Can_IdType CanId1, Can_IdType CanId2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
# if defined(C_ENABLE_MIXED_ID)
  /* #10 take care about mixed Extended and Standard ID format (shift Standard ID to match Extended format) */
  if (((CanId1) & CAN_ID_IDE_MASK) == CAN_ID_IDE_MASK)
  {
    CanId1 = CanId1 & (Can_IdType)(CAN_ID_MASK);
  } else {
    CanId1 = (Can_IdType)(((CanId1) & CAN_ID_MASK_STD) << 18);
  }
  if (((CanId2) & CAN_ID_IDE_MASK) == CAN_ID_IDE_MASK)
  {
    CanId2 = CanId2 & (Can_IdType)(CAN_ID_MASK);
  } else {
    CanId2 = (Can_IdType)(((CanId2) & CAN_ID_MASK_STD) << 18);
  }
# else
  /* #20 mask out unused bits */
  CanId1 = CanId1 & (Can_IdType)CAN_ID_MASK;
  CanId2 = CanId2 & (Can_IdType)CAN_ID_MASK;
# endif
  /* #30 compare CAN ID priority */
  if(CanId1 < CanId2)
  {
    retval = CAN_OK;
  }
  return retval;
}
#endif

#if defined(C_ENABLE_CANCEL_IN_HW) && defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
/****************************************************************************
| NAME:             CanHL_SortlogTxObjHandlePrioList
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_SortlogTxObjHandlePrioList
Relation_Context:
HwCancelTx, MultiplexedTx, IdenticalIdCancel #CanHL_WritePrepareCancelTx#
Relation:
TxBasicAmount #ActiveSendId#
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_SortlogTxObjHandlePrioList( CanHlObjectHandlePtrType logTxObjHandlePrioList )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle tmp_logTxObjHandle;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 sort element 0 and 1 */
  if ( CanHL_IsHigherCanIdPrio(Can_GetActiveSendId(logTxObjHandlePrioList[0]), Can_GetActiveSendId(logTxObjHandlePrioList[1])) == CAN_OK)
  {
    tmp_logTxObjHandle = logTxObjHandlePrioList[0];
    logTxObjHandlePrioList[0] = logTxObjHandlePrioList[1]; /* SBSW_CAN_HL21 */
    logTxObjHandlePrioList[1] = tmp_logTxObjHandle; /* SBSW_CAN_HL21 */
  }
  /* #20 sort element 1 and 2 */
  if ( CanHL_IsHigherCanIdPrio(Can_GetActiveSendId(logTxObjHandlePrioList[1]), Can_GetActiveSendId(logTxObjHandlePrioList[2])) == CAN_OK)
  {
    tmp_logTxObjHandle = logTxObjHandlePrioList[1];
    logTxObjHandlePrioList[1] = logTxObjHandlePrioList[2]; /* SBSW_CAN_HL21 */
    logTxObjHandlePrioList[2] = tmp_logTxObjHandle; /* SBSW_CAN_HL21 */
  }
  /* #30 sort element 0 and 1 again */
  if ( CanHL_IsHigherCanIdPrio(Can_GetActiveSendId(logTxObjHandlePrioList[0]), Can_GetActiveSendId(logTxObjHandlePrioList[1])) == CAN_OK)
  {
    tmp_logTxObjHandle = logTxObjHandlePrioList[0];
    logTxObjHandlePrioList[0] = logTxObjHandlePrioList[1]; /* SBSW_CAN_HL21 */
    logTxObjHandlePrioList[1] = tmp_logTxObjHandle; /* SBSW_CAN_HL21 */
  }
}
#endif

/****************************************************************************
| NAME:             CanHL_WritePrepare
****************************************************************************/
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
 */
/*
|<DataModelStart>| CanHL_WritePrepare
Relation_Context:
Relation:
TxFullCANSupport
HwCancelTx
HwCancelTx, MultiplexedTx
MultiplexedTx, HwCancelTx, IdenticalIdCancel
ChannelAmount
TxBasicAmount
TxFullAmount
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, CanObjectHandle, STATIC_CODE) CanHL_WritePrepare( CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType Hth, Can_PduInfoPtrType PduInfo )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle  logTxObjHandle;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 calculate logical TX mailbox handle for FullCAN or for BasicCAN (Multiplexed TX) as start value */
  logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST Can_GetHwHandle(Hth));
#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
  if ( Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#endif
  { /* #20 BasicCAN preparation - search for hardware and logical transmit handle and do cancellation */
#if defined(C_ENABLE_CANCEL_IN_HW)
    CanObjectHandle logTxObjHandlePrioList[CAN_MULTIPLEXED_TX_MAX]; /* CM_CAN_HL19 */
#endif
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_CANCEL_IN_HW)
    uint8_least muxTx;
    uint8_least muxTx_max;
    uint8_least muxTx_use;
    muxTx_max = CAN_MULTIPLEXED_TX_MAX_OBJ(Hth); /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    muxTx_use = muxTx_max;
#endif
    /* \trace SPEC-1677, SPEC-3800, SPEC-1672, SPEC-1679, SPEC-1673, SPEC-1671 */
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
    for (muxTx = 0; muxTx < muxTx_max; muxTx++)
    { /* #30 over all multiplexed TX objects: find free mailbox or mailbox with identical ID*/
# if defined(C_ENABLE_CANCEL_IN_HW)
      logTxObjHandlePrioList[muxTx] = logTxObjHandle; /* SBSW_CAN_HL21 */
# endif
# if (CAN_IDENTICAL_ID_CANCELLATION == STD_ON) && defined(C_ENABLE_CANCEL_IN_HW)
      if ( ((PduInfo->id) == Can_GetActiveSendId(logTxObjHandle) ) && (Can_GetActiveSendState(logTxObjHandle) != CAN_FREE_OBJ) )
      { /* #40 Identical ID: priority 1 - cancel identical (also when free is available) */
        muxTx_use = muxTx;
        break;
      }
      if (Can_GetActiveSendState(logTxObjHandle) == CAN_FREE_OBJ)
      { /* #50 Identical ID: priority 2 - find free mailbox (no cancellation) */
        muxTx_use = muxTx; /* do not break ... next may be identical ID */
      }
# else
      if (Can_GetActiveSendState(logTxObjHandle) == CAN_FREE_OBJ)
      { /* #60 else priority 1 - find free mailbox (no cancellation) */
        break;
      }
# endif
      logTxObjHandle++;
    }
# if (CAN_IDENTICAL_ID_CANCELLATION == STD_ON) && defined(C_ENABLE_CANCEL_IN_HW)
    if (muxTx_use == muxTx_max)
    { /* #70 found no free mailbox or identical ID - so set to last valid handle */
      logTxObjHandle--;
    }
    else
    { /* #80 found free or identical mailbox - so set handle to it */
      muxTx = muxTx_use;
      logTxObjHandle = logTxObjHandlePrioList[muxTx]; /* PRQA S 3353 */ /* MD_Can_Asr_3353_logTxobjHandle */
    }
# else
    if (muxTx == muxTx_max)
    { /* #90 found no free mailbox - so set to last valid handle */
      muxTx = 0; /* PRQA S 3199 */ /* MD_Can_3199_dummy */
      logTxObjHandle--;
    } /* #100 otherwise the muxTx and logTxObjHandle are the free or to be cancelled one */
# endif
#else /* C_ENABLE_MULTIPLEXED_TRANSMISSION */
# if defined(C_ENABLE_CANCEL_IN_HW)
    muxTx = 0;
    logTxObjHandlePrioList[muxTx] = logTxObjHandle; /* SBSW_CAN_HL21 */
# endif
#endif
#if defined(C_ENABLE_CANCEL_IN_HW) /* \trace SPEC-3982, SPEC-1674 */
    CanHL_WritePrepareCancelTx(CAN_CHANNEL_CANPARA_FIRST logTxObjHandle, muxTx, muxTx_max, logTxObjHandlePrioList, PduInfo); /* SBSW_CAN_HL28 */
#endif /* (C_ENABLE_CANCEL_IN_HW) */
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_CANCEL_IN_HW)
    CAN_DUMMY_STATEMENT(muxTx_use); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  } /* end BasicCAN handling */
  /* #200 no special FullCAN handling needed */
#if !defined(C_ENABLE_CANCEL_IN_HW)
  CAN_DUMMY_STATEMENT(PduInfo); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return logTxObjHandle;
}

#if defined(C_ENABLE_CANCEL_IN_HW)
/****************************************************************************
| NAME:             CanHL_WritePrepareCancelTx
****************************************************************************/
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
 */
/*
|<DataModelStart>| CanHL_WritePrepareCancelTx
Relation_Context:
HwCancelTx
Relation:
DevErrorDetect
CanFdSupport
MultiplexedTx, IdenticalIdCancel
OneChOpt
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WritePrepareCancelTx( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle logTxObjHandle, uint8_least muxTx, uint8_least muxTx_max, CanHlObjectHandlePtrType logTxObjHandlePrioList, Can_PduInfoPtrType PduInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 backup ID, DLC, data, PDU for possible cancellation - as parameter to IF */
  if (Can_GetActiveSendState(logTxObjHandle) == CAN_FREE_OBJ)
  {
    /* #20 SDU pointer may be a null pointer in case the DLC is 0 - so do not copy data in this case */
    if(PduInfo->sdu != NULL_PTR)
    {
# if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
      if(sizeof(Can_GetCanTxBasicDataBuffer(channel,muxTx)) >= PduInfo->length) /* CM_CAN_HL20 */
# endif
      {
# if defined(C_ENABLE_CAN_FD_FULL)
        /* #25 copy data with VStdLib optimized copy routine for CAN-FD messages (use alignment info) */
        VStdMemCpy(Can_GetCanTxBasicDataBuffer(channel,muxTx),PduInfo->sdu,PduInfo->length); /* SBSW_CAN_HL22 */
# else
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[0] = PduInfo->sdu[0]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[1] = PduInfo->sdu[1]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[2] = PduInfo->sdu[2]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[3] = PduInfo->sdu[3]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[4] = PduInfo->sdu[4]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[5] = PduInfo->sdu[5]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[6] = PduInfo->sdu[6]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
        Can_GetCanTxBasicDataBuffer(channel,muxTx)[7] = PduInfo->sdu[7]; /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL22 */
# endif
      }
    }
    Can_SetActiveSendId(logTxObjHandle, PduInfo->id); /* SBSW_CAN_HL23 */
    Can_SetActiveSendLength(logTxObjHandle, PduInfo->length); /* SBSW_CAN_HL23 */
    Can_SetActiveSendSdu(logTxObjHandle, &Can_GetCanTxBasicDataBuffer(channel,muxTx)[0]); /* PRQA S 3689 */ /* MD_Can_Asr_3689_MuxTx */ /* SBSW_CAN_HL23 */
    Can_SetActiveSendSwPduHandle(logTxObjHandle, PduInfo->swPduHandle); /* SBSW_CAN_HL23 */
  }
  else
  /* #30 no free send mailbox available */
  {
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
#  if (CAN_IDENTICAL_ID_CANCELLATION == STD_ON)
    /* #40 no identical ID found before */
    if (muxTx == muxTx_max) 
#  endif
    { /* #50 sort multiplexed TX handles to get the low priority first */
      CanHL_SortlogTxObjHandlePrioList(logTxObjHandlePrioList); /* SBSW_CAN_HL17 */
      muxTx = 0;
    }
    for (; muxTx < muxTx_max; muxTx++)
# else
    muxTx=0;
# endif
    { /* #60 no-MUX:       use 1st mailbox */
      /* #70 MUX:          iterate over all normal TX mailboxes */
      /* #80 identical ID: use only identical ID in mailbox */
      /* \trace SPEC-1753 */
# if (CAN_IDENTICAL_ID_CANCELLATION == STD_ON)
#  if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
      /* #90 identical include FD and ExtID flags */
      /* #100 finished when identical ID is already cancelled */
      if ( (Can_GetActiveSendState(logTxObjHandlePrioList[muxTx]) == CAN_CANCEL_OBJ) && (PduInfo->id == Can_GetActiveSendId(logTxObjHandlePrioList[muxTx])))
      {
        break; 
      }
      else
#  endif
      if ( ((CanHL_IsHigherCanIdPrio(PduInfo->id, Can_GetActiveSendId(logTxObjHandlePrioList[muxTx])) == CAN_OK) || (PduInfo->id == Can_GetActiveSendId(logTxObjHandlePrioList[muxTx]))) && \
           (Can_GetActiveSendState(logTxObjHandlePrioList[muxTx]) == CAN_SEND_OBJ) )
# else
      if ( ( CanHL_IsHigherCanIdPrio(PduInfo->id, Can_GetActiveSendId(logTxObjHandlePrioList[muxTx])) == CAN_OK) && \
           (Can_GetActiveSendState(logTxObjHandlePrioList[muxTx]) == CAN_SEND_OBJ) )
# endif
      /* #110 found lower priority PDU or Identical ID - ready to cancel (others may in cancel process) \trace SPEC-1747, SPEC-15133, SPEC-1685, SPEC-1690, SPEC-1692, SPEC-1676, SPEC-3856, SPEC-1683, SPEC-15126 */
      {
        Can_HwHandleType txObjHandle;
        /* #120 mark as to be cancelled by hardware */
        Can_SetActiveSendState(logTxObjHandlePrioList[muxTx], CAN_CANCEL_OBJ); /* SBSW_CAN_HL41 */
        txObjHandle    = CanHL_GetTxLogToHwHandle(CAN_CHANNEL_CANPARA_FIRST logTxObjHandlePrioList[muxTx]);
        /* #125 HW cancel mailbox (LL) */
        /*
        |<DataModelStart>| CanLL_CancelInHw
        Relation_Context:
        HwCancelTx
        Relation:
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        /*
        Do not activate the following line! CANoe is only a emulation and does not really cancel the message out of hardware
        Can_GetgCanController(channel).MsgObj[txObjHandle].mCanCancel = (vuint8)kCanTrue;  SBSW_CAN_LL01
        */
        CAN_DUMMY_STATEMENT(txObjHandle); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
        /* #130 do not notify IF about successful cancellation because of recursion will not work! Hardware support not possible in this case */
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
        /* #140 finished - only cancel one mailbox */ 
        break; /* PRQA S 0771,3333 */ /* MD_Can_14.6 */
# endif
      }
      else
      { /* avoid MISRA warning 2004 (if else if without ending else) */
        /* \trace SPEC-1754 */
      }
    }
  }
  CAN_DUMMY_STATEMENT(muxTx_max); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* C_ENABLE_CANCEL_IN_HW */

/****************************************************************************
| NAME:             CanHL_WriteStart
****************************************************************************/
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
/*
|<DataModelStart>| Can_WriteStart
Relation_Context:
Relation:
GenericPreTransmit
CanFdSupport
GenericPreTransmit, DynamicFullCanTxID, TxFullCANSupport
IDType, MixedId, DynamicFullCanTxID, TxFullCANSupport
OneChOpt
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_WriteStart( CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType Hth, CanObjectHandle logTxObjHandle, Can_PduInfoPtrType PduInfo )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 free TX mailbox found so start transmission \trace SPEC-1744 */
  if (Can_GetActiveSendState(logTxObjHandle) == CAN_FREE_OBJ)
  {
    Can_HwHandleType txObjHandle;
    Can_SduPtrType   CanMemCopySrcPtr;
#if (CAN_GENERIC_PRETRANSMIT == STD_ON)
    /* #20 temporary buffer to avoid data modification on constant data */
    Can_PduType PduInfo_var;
#endif
#if defined(C_ENABLE_CAN_FD_USED)
    Can_IdType pduInfo_var_id;
    uint8 pduInfo_var_dlc;
# if defined(C_ENABLE_CAN_FD_FULL)
    uint8 pduInfo_var_dli;
# endif
#endif
    /*
    |<DataModelStart>| CanLL_TxTransmissionLocalParameter
    Relation_Context:
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
#if (CAN_GENERIC_PRETRANSMIT == STD_ON)
    Can_PduInfoPtrType_var PduInfoPtr_var;
    /* #25 PduInfo copy from ROM pointer context to RAM pointer context */
    PduInfoPtr_var = &PduInfo_var;
    PduInfo_var.id = PduInfo->id; 
    PduInfo_var.length = PduInfo->length;
    PduInfo_var.sdu = PduInfo->sdu;
    PduInfo_var.swPduHandle = PduInfo->swPduHandle;
    Appl_GenericPreTransmit( (uint8)channel, PduInfoPtr_var );  /* PRQA S 0312 */ /* MD_Can_0312 */ /* SBSW_CAN_HL24 */
#endif
#if defined(C_ENABLE_CAN_FD_USED)
    pduInfo_var_id = PDUINFO_PTR->id;
    if (PduInfo->length <= 8) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      if (Can_GetInitObjectFdBrsConfig(Can_GetLastInitObject(channel)) == CAN_NONE)
      { /* #30 remove FD flag when no FD support is activated in this initialization structure and DLC not greater than 8 */
        /* CAN FD also support an extended payload which allows the transmission of up to 64 bytes.
        This feature also depends on the CAN FD configuration (see CanControllerFdBaudrateConfig).
        Therefore, if the CAN FD feature is enabled and the CAN FD flag is set in CanId passed to
         Can_Write(), CanDrv supports the transmission of PDUs with a length up to 64 bytes.
        If there is a request to transmit a CAN FD frame without the CAN FD feature enabled the
        frame is sent as conventional CAN frame as long as the PDU length <= 8 bytes. */
        pduInfo_var_id = pduInfo_var_id & ((Can_IdType)(CAN_ID_IDE_MASK | CAN_ID_MASK)); /* CAN_FD_   mask out the FD bit */
      }
    }
#endif
    /* #40 remember PDU and send state to use it in Confirmation function \trace SPEC-1675, SPEC-3921 */
    Can_SetActiveSendPdu(logTxObjHandle, PDUINFO_PTR->swPduHandle); /* SBSW_CAN_HL05 */
    Can_SetActiveSendState(logTxObjHandle, CAN_SEND_OBJ); /* SBSW_CAN_HL04 */
    txObjHandle =  CanHL_GetTxLogToHwHandle(CAN_CHANNEL_CANPARA_FIRST logTxObjHandle);
#if defined(C_ENABLE_CAN_FD_USED)
    /* #50 CAN FD convert data length (0-64) to DLC (0-15) and to DLI (0-8,12,16,20,24,32,48,64) */
    pduInfo_var_dlc = CAN_ENCODE_DLI(PDUINFO_PTR->length);
# if defined(C_ENABLE_CAN_FD_FULL)
    pduInfo_var_dli = CAN_DECODE_DLI(pduInfo_var_dlc);
# endif
#endif
    /* #55 HW prepare transmission (LL) */
    /*
    |<DataModelStart>| CanLL_TxBegin
    Relation_Context:
    Relation:
    ChannelAmount
    TxBasicAmount
    TxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    Can_GetgCanController(channel).MsgObj[txObjHandle].mIsFree = (vuint8)kCanFalse; /* SBSW_CAN_LL01 */
    Can_GetgCanController(channel).MsgObj[txObjHandle].mCanCancel = (vuint8)kCanFalse; /* SBSW_CAN_LL01 */
    /* #60 Set ID, ID type and DLC in HW register depend on ID type */
#if defined(C_ENABLE_EXTENDED_ID)
# if defined(C_ENABLE_MIXED_ID)
    if ( ((PDUINFO_ID) & CAN_ID_IDE_MASK) != CAN_ID_IDE_MASK)
    {
#  if !defined(CAN_ENABLE_DYNAMIC_FULLCAN_ID) /* COV_DYNAMIC_FULLCAN_ID */
#   if defined(C_ENABLE_TX_FULLCAN_OBJECTS) 
      if (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE)
#   endif
#  endif
      {
        /*
        |<DataModelStart>| CanLL_TxSetId
        Relation_Context:
        IDType, MixedId
        Relation:
        ChannelAmount
        TxBasicAmount
        TxFullAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.IdRaw = MK_STDID0(PDUINFO_ID); /* SBSW_CAN_LL01 */
      }
      /*
      |<DataModelStart>| CanLL_TxSetDlc
      Relation_Context:
      IDType, MixedId
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DlcRaw = MK_TX_DLC(PDUINFO_DLC); /* SBSW_CAN_LL01 */
    }
    else
# endif
    {
# if !defined(CAN_ENABLE_DYNAMIC_FULLCAN_ID) /* COV_DYNAMIC_FULLCAN_ID */
#  if defined(C_ENABLE_TX_FULLCAN_OBJECTS) 
      if (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE)
#  endif
# endif
      {
        /*
        |<DataModelStart>| CanLL_TxSetId
        Relation_Context:
        IDType, MixedId
        Relation:
        ChannelAmount
        TxBasicAmount
        TxFullAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.IdRaw = MK_EXTID0(PDUINFO_ID); /* SBSW_CAN_LL01 */
      }
      /*
      |<DataModelStart>| CanLL_TxSetDlc
      Relation_Context:
      IDType, MixedId
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DlcRaw = MK_TX_DLC_EXT(PDUINFO_DLC); /* SBSW_CAN_LL01 */
    }
#else
# if !defined(CAN_ENABLE_DYNAMIC_FULLCAN_ID) /* COV_DYNAMIC_FULLCAN_ID */
#  if defined(C_ENABLE_TX_FULLCAN_OBJECTS) 
    if (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE)
#  endif
# endif
    {
      /*
      |<DataModelStart>| CanLL_TxSetId
      Relation_Context:
      IDType, MixedId
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.IdRaw = MK_STDID0(PDUINFO_ID); /* SBSW_CAN_LL01 */
    }
    /*
    |<DataModelStart>| CanLL_TxSetDlc
    Relation_Context:
    IDType, MixedId
    Relation:
    ChannelAmount
    TxBasicAmount
    TxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DlcRaw = MK_TX_DLC(PDUINFO_DLC); /* SBSW_CAN_LL01 */
#endif
    if(PDUINFO_PTR->sdu != NULL_PTR)
    { /* #70 Set CAN data in HW register (LL) - only when data pointer is not null \trace SPEC-1680, SPEC-1572, SPEC-1678, SPEC-60421 */
      CanMemCopySrcPtr = PDUINFO_PTR->sdu;
#if defined(C_ENABLE_CAN_FD_FULL)
      /*
      |<DataModelStart>| CanLL_TxCopyToCanFd
      Relation_Context:
      CanFdSupport
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      (void)memcpy( (void*)Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld, CanMemCopySrcPtr, PDUINFO_PTR->length); /* PRQA S 3335,5115,1594,1510 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
      {
        vuint8 idx;
        for (idx=PDUINFO_PTR->length; idx<pduInfo_var_dli; idx++)
        {
          Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld[idx] = Can_GetMailboxFdPadding(Hth); /* SBSW_CAN_LL02 */
        }
      }
#else
      /*
      |<DataModelStart>| CanLL_TxCopyToCan
      Relation_Context:
      CanFdSupport
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      (void)memcpy( (void*)Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld, CanMemCopySrcPtr, 8); /* PRQA S 3335,5115,1594,1510 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
#endif
    }
    /* #80 HW start transmission (LL) */
    retval = CAN_OK; /* default is OK - LL set it to kCanFailed/CAN_NOT_OK in case of issue */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
    /*
    |<DataModelStart>| CanLL_TxStart
    Relation_Context:
    Relation:
    CanFdSupport
    ChannelAmount
    TxBasicAmount
    TxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    {
      /* Message Flags 
         1 RTR-Flag 
         2 Wakeup (high voltage message on single wire CAN)
         4 transceiver error (NERR) on two wire low speed CAN bus */
      vuint32 flags; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
      
      if ( Can_GetgCanController(channel).HighVoltageMode == 1 ) /* COV_CAN_LL_HIGHVOLTAGE */
      {
        flags = 2;
      }
      else
      {
        flags = 0;
      }
    #if (CAN_FD_SUPPORT != CAN_NONE) 
      if ( CanHL_IsFdMessage(PDUINFO_ID) )
      {
        flags |= CANOEAPI_CAN_EDL;
        if ( CanHL_IsFdTxBrs(channel) )
        {
          flags |= CANOEAPI_CAN_BRS;
        }
      }
    #endif
      CanCell_SendMessage((vuint8)channel,                                                           /* channel */  /* PRQA S 3757,3760 */ /* conversion is ok here */ /* SBSW_CAN_LL09 */
                           Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.IdRaw,            /* can identifier */
                           (int)Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DlcRaw,      /* data length code */ /* PRQA S 5013 */ /* MD_Can_5013_LL */
                           (vuint8*)Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld, /* data field */
                           flags);                                                                   /* message flags */ 
    
    }
    /* HW finish transmission (LL) */
    /*
    |<DataModelStart>| CanLL_TxEnd
    Relation_Context:
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
  }
  else
  { /* #200 no free TX mailbox found so return busy - also in case cancellation (BasicCAN) was possible \trace SPEC-1754, SPEC-1764, SPEC-1747, SPEC-3878, SPEC-3856 */
    retval = CAN_BUSY;
  }
  CAN_DUMMY_STATEMENT(Hth); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  return retval;
}  /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             CanHL_GetTxHwToLogHandle
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_GetTxHwToLogHandle
Relation_Context:
TxFullCANSupport
HwCancelTx
Relation:
DevErrorDetect
OneChOpt
ChannelAmount
TxFullAmount
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, CanObjectHandle, STATIC_CODE) CanHL_GetTxHwToLogHandle( CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType hwTxObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle logTxObjHandle;
  /* ----- Implementation ----------------------------------------------- */
#if defined(CAN_TXINDIRECTIONHWTOLOG) /* COV_CAN_TXINDIRECTIONHWTOLOG */
  /* #10 get logical handle from generated indirection table */
  logTxObjHandle = (CanObjectHandle) Can_GetTxIndirectionHwToLog((hwTxObjHandle) + Can_GetTxIndirectionHwToLogStart(channel));
#else
  /* #20 get logical handle by add generated offset to hardware handle */
  logTxObjHandle = (CanObjectHandle)(hwTxObjHandle + Can_GetTxOffsetHwToLog(channel));
#endif
#if (CAN_SAFE_BSW == STD_ON)
  if ((logTxObjHandle + CAN_MULTIPLEXED_TX_MAX) >= (Can_HwHandleType)Can_GetSizeOfActiveSendObject()) /* CM_CAN_HL03 */ /* COV_CAN_GENDATA_FAILURE */
  { /* just to secure SilentBSW */
    logTxObjHandle = (Can_HwHandleType)(Can_GetSizeOfActiveSendObject() - CAN_MULTIPLEXED_TX_MAX); /* -1 is used later on so don't use 0 here */
# if (CAN_DEV_ERROR_REPORT == STD_ON)  
    Can_CallDetReportError(CAN_WRITE_ID, CAN_E_PARAM_HANDLE);
# endif
  }
#endif
  return logTxObjHandle;
}

/****************************************************************************
| NAME:             CanHL_GetTxLogToHwHandle
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_GetTxLogToHwHandle
Relation_Context:
HwCancelTx
Relation:
OneChOpt
ChannelAmount
TxFullAmount
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_HwHandleType, STATIC_CODE) CanHL_GetTxLogToHwHandle( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle logTxObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_HwHandleType hwTxObjHandle;
  /* ----- Implementation ----------------------------------------------- */
#if defined(CAN_TXINDIRECTIONHWTOLOG) /* COV_CAN_TXINDIRECTIONHWTOLOG */
  /* #10 get hardware handle from generated indirection table */
  hwTxObjHandle = (Can_HwHandleType) Can_GetTxIndirectionLogToHw(logTxObjHandle);
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#else
  /* #20 get hardware handle by subtract generated offset from logical handle */
  hwTxObjHandle = (Can_HwHandleType)(logTxObjHandle - Can_GetTxOffsetHwToLog(channel));
#endif
  return hwTxObjHandle;
}

#if defined(C_ENABLE_CAN_RAM_CHECK)
/****************************************************************************
| NAME:             CanHL_RamCheckMailboxNotification
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_RamCheckMailboxNotification
Relation_Context:
TxFullCANSupport
RxFullCANSupport
RxBasicCANSupport
Relation:
RamCheck
OneChOpt
ChannelAmount
TxFullAmount
TxBasicAmount
RxFullAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RamCheckMailboxNotification( CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType hwObjHandle, Can_HwHandleType htrh ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 notify application about corrupt mailbox */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  CanIf_RamCheckCorruptMailbox((uint8)Can_GetCanToCanIfChannelMapping(channel), htrh);
# else
#  if defined(C_ENABLE_NOTIFY_CORRUPT_MAILBOX) /* with Mailbox Notification */
  Appl_CanCorruptMailbox((uint8)channel, (hwObjHandle));
#  endif
# endif
  /* #20 deactivate mailbox */
  Can_SetMailboxState(htrh, CAN_DISABLED_OBJ); /* SBSW_CAN_HL34 */
  /* #30 deactivate controller */
  Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL02 */

  CAN_DUMMY_STATEMENT(hwObjHandle); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CAN_DUMMY_STATEMENT(htrh); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
}
#endif

/****************************************************************************
| NAME:             CanHL_SetValue8
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_SetValue8
Relation_Context:
Relation:
RamCheck
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_SetValue8( CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr8 regPtr, uint8 value, uint8 readMask ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3219 */ /* MD_Can_Asr_3219 */
{
  uint8 result = kCanFalse;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_HL35 */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
  {
    /* #20 read back value for RamCheck */
    volatile uint8 readBack;
    readBack = *regPtr;
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCanTrue; /* PRQA S 3198 */ /* MD_MSR_14.2 */
    }
    CAN_RAM_CHECK_READ_BACK_RESULT(channel)
  }
# else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  CAN_DUMMY_STATEMENT(readMask); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return result;
}

/****************************************************************************
| NAME:             CanHL_SetValue16
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_SetValue16
Relation_Context:
Relation:
RamCheck
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_SetValue16( CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr16 regPtr, uint16 value, uint16 readMask ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3219 */ /* MD_Can_Asr_3219 */
{
  uint8 result = kCanFalse;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_HL35 */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
  {
    /* #20 read back value for RamCheck */
    volatile uint16 readBack;
    readBack = *regPtr;
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCanTrue; /* PRQA S 3198 */ /* MD_MSR_14.2 */
    }
    CAN_RAM_CHECK_READ_BACK_RESULT(channel)
  }
# else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  CAN_DUMMY_STATEMENT(readMask); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return result;
}

/****************************************************************************
| NAME:             CanHL_SetValue32
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_SetValue32
Relation_Context:
Relation:
RamCheck
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_SetValue32( CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr32 regPtr, uint32 value, uint32 readMask ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3219 */ /* MD_Can_Asr_3219 */
{
  uint8 result = kCanFalse;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_HL35 */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
  {
    /* #20 read back value for RamCheck */
    volatile uint32 readBack;
    readBack = *regPtr;
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCanTrue; /* PRQA S 3198 */ /* MD_MSR_14.2 */
    }
    CAN_RAM_CHECK_READ_BACK_RESULT(channel)
  }
# else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  CAN_DUMMY_STATEMENT(readMask); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return result;
}

/****************************************************************************
| NAME:             CanHL_InitBegin
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitBegin
Relation_Context:
Relation:
CommonCan
OneChOpt
RamCheck
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitBegin( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanInitHandle  initObject; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL
  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->channel;
#endif
  initObject = initInfo->initObject;

  /* #10 Set controller to initialization mode (LL) */
  /*
  |<DataModelStart>| CanLL_InitBeginSetToInitMode
  Relation_Context:
  Relation:
  ChannelAmount
  HardwareLoop, ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  {
    CAN_HL_INFO_INIT_PTR(canLLController) = &Can_GetgCanController(channel); /* SBSW_CAN_LL10 */
  
    CAN_HL_INFO_INIT_PTR(canLLController)->Mode            = CAN_MODE_INIT; /* SBSW_CAN_LL10 */
  }

#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  /* #20 Execute pattern RamCheck */
  if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
  {
    initInfo->isChCorrupt |= CAN_RAM_CHECK_BEGIN_REG_RESULT(channel); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL36 */
  } 
#endif

  /* #30 Initialize controller registers (Begin section) (LL) */
  /*
  |<DataModelStart>| CanLL_InitBeginRegister
  Relation_Context:
  Relation:
  ChannelAmount
  RxBasicAmount
  TxFullAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  {
    Can_HwHandleType canLL_hwHandle;
    CanCell_AtomicBegin();
  
    CAN_HL_INFO_INIT_PTR(canLLController) = &Can_GetgCanController(channel); /* SBSW_CAN_LL10 */
  
    /* CAN_HL_INFO_INIT_PTR(canLLController)->IntEnable       = CAN_INT_NONE;  will be set with CanCell_EnableInterrupts() */
    CAN_HL_INFO_INIT_PTR(canLLController)->HighVoltageMode = 0; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->ErrCode         = (vuint32)CANCELL_ERRORACTIVE; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->ErrCntTx        = 0; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->ErrCntRx        = 0; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->WakeupPending   = (vuint8)kCanFalse; /* reset pending flags */ /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->BusOffNotify    = (vuint8)kCanFalse; /* unused (just use ErrCode) */ /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->ErrorPending    = (vuint8)kCanFalse; /* unused (just use ErrCode) */ /* SBSW_CAN_LL10 */
    for (canLL_hwHandle = 0; canLL_hwHandle < CAN_MAX_BCAN_MAILBOX; canLL_hwHandle++)
    {
      CAN_HL_INFO_INIT_PTR(canLLController)->BCanWrittenPointer[canLL_hwHandle] = 0; /* SBSW_CAN_LL10 */
    }
    /* initialize bit timing registers */
    /* initialize CANoe RX and TX Callbacks for Emulation */
    CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, CANOEAPI_CAN_SILENTMODE);  /* PRQA S 5013,0310,0311,1334,432 */ /* MD_Can_5013_LL, MD_Can_0310, MD_Can_0311, MD_Can_1334, MD_Can_1334 */ /* COV_CAN_LL_EMULATION */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */ /* SBSW_CAN_LL13 */
    /* initialize all TX objects not only the logical TX start objects (emulate multiple mailboxes for logical send object - FIFO size != 1 for FullCAN TX or BasicCAN TX) */
    for (canLL_hwHandle = CAN_HL_HW_TX_STARTINDEX(channel);canLL_hwHandle <CAN_HL_HW_TX_STOPINDEX(channel); canLL_hwHandle++) /* COV_CAN_LL_EMULATION */
    {
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[canLL_hwHandle].mObjType    = CAN_MSGOBJ_TX; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[canLL_hwHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[canLL_hwHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[canLL_hwHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[canLL_hwHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    }
    {
      /* just for simulation */
      vuint8 simReg8;
      vuint16 simReg16;
      vuint32 simReg32;
      initInfo->isChCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isChCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isChCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
    }
  }

#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CAN_DUMMY_STATEMENT(initObject); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  return errorId;
} /* PRQA S 6010,6050 */ /* MD_MSR_STPTH,MD_MSR_STCAL */

#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_InitTxFullCAN
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_InitTxFullCAN
Relation_Context:
TxFullCANSupport
Relation:
CommonCan
OneChOpt
RamCheck
IDType
DynamicFullCanTxID
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxFullCAN( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_HwHandleType hwObjHandle;
  Can_HwHandleType Htrh;
  CanObjectHandle logTxObjHandle;
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->Controller;
# endif
  errorId = CAN_E_NO_ERROR;
  /* #10 iterate over all FullCAN TX */
  for (Htrh = Can_GetTxFullHandleStart(canHwChannel); Htrh < Can_GetTxFullHandleStop(canHwChannel); Htrh++)
  { /* #20 initialize FullCAN TX mailboxes */
    hwObjHandle = Can_GetHwHandle(Htrh);
    logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
    Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #30 RAM check for FullCAN TX mailboxes */
    initInfo->isMbCorrupt = kCanFalse; /* SBSW_CAN_HL36 */
    if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
    {
      initInfo->isMbCorrupt = CAN_RAM_CHECK_MAILBOX_RESULT(channel, (hwObjHandle)); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL36 */
    } 
# endif
    /* HW specific TX initialization (LL) */
    /*
    |<DataModelStart>| CanLL_InitMailboxTx
    Relation_Context:
    TxFullCANSupport
    Relation:
    ChannelAmount
    TxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_TX; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    {
      /* just for simulation */
      vuint8 simReg8;
      vuint16 simReg16;
      vuint32 simReg32;
      initInfo->isMbCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isMbCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isMbCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
    }
# if !defined(CAN_ENABLE_DYNAMIC_FULLCAN_ID) /* if needed the initialization of ID, DLC and type is done in InitTxObj to avoid problems with uninitialized registers */ /* COV_DYNAMIC_FULLCAN_ID */
    /* #40 set ID and DLC for FullCAN TX mailboxes */
#  if defined(C_ENABLE_EXTENDED_ID)
#   if defined(C_ENABLE_MIXED_ID)
    if ( (Can_GetIDValue(Htrh) & CAN_ID_IDE_MASK) != CAN_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      /*
      |<DataModelStart>| CanLL_InitTxSetId
      Relation_Context:
      TxFullCANSupport, DynamicFullCanTxID, IDType
      Relation:
      ChannelAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.IdRaw = MK_STDID0(Can_GetIDValue(Htrh)); /* SBSW_CAN_LL10 */
      /*
      |<DataModelStart>| CanLL_InitTxSetDlc
      Relation_Context:
      TxFullCANSupport, DynamicFullCanTxID, IDType
      Relation:
      ChannelAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.DlcRaw = MK_TX_DLC(Can_GetDLC_FIFO(Htrh)); /* SBSW_CAN_LL10 */
    }
    else
#   endif
    {
      /*
      |<DataModelStart>| CanLL_InitTxSetId
      Relation_Context:
      TxFullCANSupport, DynamicFullCanTxID, IDType
      Relation:
      ChannelAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.IdRaw = MK_EXTID0(Can_GetIDValue(Htrh)); /* SBSW_CAN_LL10 */
      /*
      |<DataModelStart>| CanLL_InitTxSetDlc
      Relation_Context:
      TxFullCANSupport, DynamicFullCanTxID, IDType
      Relation:
      ChannelAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.DlcRaw = MK_TX_DLC_EXT(Can_GetDLC_FIFO(Htrh)); /* SBSW_CAN_LL10 */
    }
#  else
    /*
    |<DataModelStart>| CanLL_InitTxSetId
    Relation_Context:
    TxFullCANSupport, DynamicFullCanTxID, IDType
    Relation:
    ChannelAmount
    TxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.IdRaw = MK_STDID0(Can_GetIDValue(Htrh)); /* SBSW_CAN_LL10 */
    /*
    |<DataModelStart>| CanLL_InitTxSetDlc
    Relation_Context:
    TxFullCANSupport, DynamicFullCanTxID, IDType
    Relation:
    ChannelAmount
    TxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.DlcRaw = MK_TX_DLC(Can_GetDLC_FIFO(Htrh)); /* SBSW_CAN_LL10 */
#  endif
# endif
# if defined(C_ENABLE_CAN_RAM_CHECK)
    if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
    { /* PowerOn */
      Can_SetMailboxState(Htrh, CAN_ENABLE_OBJ); /* SBSW_CAN_HL34 */
    }
    if( (initInfo->isMbCorrupt == kCanTrue) || (Can_GetMailboxState(Htrh) == CAN_DISABLED_OBJ) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      /*
      |<DataModelStart>| CanLL_InitMailboxTxDeactivate
      Relation_Context:
      TxFullCANSupport
      Relation:
      ChannelAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      if (initInfo->isMbCorrupt == kCanTrue)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST hwObjHandle, Htrh);
      }
    }
# endif
  }
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
  CAN_DUMMY_STATEMENT(initInfo); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return errorId;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
#endif

/****************************************************************************
| NAME:             CanHL_InitTxBasicCAN
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitTxBasicCAN
Relation_Context:
Relation:
MultiplexedTx
CommonCan
OneChOpt
RamCheck
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxBasicCAN( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_HwHandleType hwObjHandle;
  Can_HwHandleType Htrh;
  CanObjectHandle logTxObjHandle;
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
  CanObjectHandle logTxObjHandleStop;
  uint8_least muxTx_max;
#endif
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->Controller;
#endif

  Htrh = Can_GetTxBasicHandleStart(canHwChannel);
#if defined(C_ENABLE_MULTIPLE_BASICCAN_TX)
  for (; Htrh < Can_GetTxBasicHandleStop(canHwChannel); Htrh++)
#endif
  { /* #10 iterate over all logical BasicCAN TX */
    hwObjHandle = Can_GetHwHandle(Htrh); /* do not use: hwObjHandle = CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) - Htrh may used by LL */
    logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
    muxTx_max = CAN_MULTIPLEXED_TX_MAX_OBJ(Htrh); /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    logTxObjHandleStop = logTxObjHandle + (Can_HwHandleType) muxTx_max;
    for (; logTxObjHandle < logTxObjHandleStop; logTxObjHandle++)
#endif
    { /* #20 initialize BasicCAN TX mailboxes */
      Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
#if defined(C_ENABLE_CAN_RAM_CHECK)
      /* #30 RAM check for BasicCAN TX mailboxes */
      if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
      {
        initInfo->isMbCorrupt = CAN_RAM_CHECK_MAILBOX_RESULT(channel, (hwObjHandle)); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL36 */
      } 
      else
      {
        initInfo->isMbCorrupt = kCanFalse; /* SBSW_CAN_HL36 */
      }
#endif
      /* HW specific TX initialization (LL) */
      /*
      |<DataModelStart>| CanLL_InitMailboxTx
      Relation_Context:
      TxFullCANSupport
      Relation:
      ChannelAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_TX; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      {
        /* just for simulation */
        vuint8 simReg8;
        vuint16 simReg16;
        vuint32 simReg32;
        initInfo->isMbCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      }
#if defined(C_ENABLE_CAN_RAM_CHECK)
      if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
      { /* PowerOn */
        Can_SetMailboxState(Htrh, CAN_ENABLE_OBJ); /* SBSW_CAN_HL34 */
      }
      if( (initInfo->isMbCorrupt == kCanTrue) || (Can_GetMailboxState(Htrh) == CAN_DISABLED_OBJ) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
      { /* #45 deactivate corrupt mailbox */
        /*
        |<DataModelStart>| CanLL_InitMailboxTxDeactivate
        Relation_Context:
        TxFullCANSupport
        Relation:
        ChannelAmount
        TxFullAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
        if (initInfo->isMbCorrupt == kCanTrue)
        {  /* #50 notify application about corrupt mailbox and deactivate controller */
          CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST hwObjHandle, Htrh);
        }
      }
#endif
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
      hwObjHandle += (Can_HwHandleType)(Can_GetDLC_FIFO(Htrh) / muxTx_max); /* PRQA S 3199,3198 */ /* MD_Can_3199_dummy */
#endif
    }
  }
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#else
  CAN_DUMMY_STATEMENT(initInfo); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return errorId;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             CanHL_InitUnusedObj
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitUnusedObj
Relation_Context:
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitUnusedObj( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_HwHandleType hwObjHandle;
  Can_HwHandleType Htrh;
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->Controller;
# endif
  for (Htrh = Can_GetUnusedHandleStart(canHwChannel); Htrh < Can_GetUnusedHandleStop(canHwChannel); Htrh++)
  { /* #10 iterate over all Unused mailboxes */
    for (hwObjHandle = Can_GetHwHandle(Htrh); hwObjHandle < (Can_HwHandleType)(Can_GetHwHandle(Htrh) + Can_GetDLC_FIFO(Htrh)); hwObjHandle++)
    { /* #20 HW initialize unused mailboxes (LL) */
      /*
      |<DataModelStart>| CanLL_InitUnusedObj
      Relation_Context:
      Relation:
      ChannelAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_UNUSED; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    }
  }
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
  CAN_DUMMY_STATEMENT(initInfo); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return errorId;
}

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_InitRxFullCAN
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitRxFullCAN
Relation_Context:
RxFullCANSupport
Relation:
CommonCan
OneChOpt
RamCheck
IDType
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxFullCAN( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_HwHandleType hwObjHandle;
  Can_HwHandleType Htrh;
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->Controller;
# endif
  for (Htrh = Can_GetRxFullHandleStart(canHwChannel); Htrh < Can_GetRxFullHandleStop(canHwChannel); Htrh++)
  { /* #10 iterate over all FullCAN RX mailboxes */
    hwObjHandle=Can_GetHwHandle(Htrh);
# if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #20 RAM check for FullCAN RX mailboxes */
    initInfo->isMbCorrupt = kCanFalse; /* SBSW_CAN_HL36 */
    if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
    {
      initInfo->isMbCorrupt = CAN_RAM_CHECK_MAILBOX_RESULT(channel, (hwObjHandle)); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL36 */
    } 
# endif
    /* #30 set ID for FullCAN RX mailboxes (LL) */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
    if ( (Can_GetIDValue(Htrh) & CAN_ID_IDE_MASK) != CAN_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      /*
      |<DataModelStart>| CanLL_InitMailboxFullCAN
      Relation_Context:
      RxFullCANSupport
      Relation:
      ChannelAmount
      RxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_RXFULL; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.IdRaw  = MK_STDID0(Can_GetIDValue(Htrh)); /* SBSW_CAN_LL10 */
      {
        /* just for simulation */
        vuint8 simReg8;
        vuint16 simReg16;
        vuint32 simReg32;
        initInfo->isMbCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      }
    }
    else
#  endif
    {
      /*
      |<DataModelStart>| CanLL_InitMailboxFullCAN
      Relation_Context:
      RxFullCANSupport
      Relation:
      ChannelAmount
      RxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_RXFULL; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.IdRaw  = MK_EXTID0(Can_GetIDValue(Htrh)); /* SBSW_CAN_LL10 */
      {
        /* just for simulation */
        vuint8 simReg8;
        vuint16 simReg16;
        vuint32 simReg32;
        initInfo->isMbCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      }
    }
# else
    /*
    |<DataModelStart>| CanLL_InitMailboxFullCAN
    Relation_Context:
    RxFullCANSupport
    Relation:
    ChannelAmount
    RxFullAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_RXFULL; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
    CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mMsg.IdRaw  = MK_STDID0(Can_GetIDValue(Htrh)); /* SBSW_CAN_LL10 */
    {
      /* just for simulation */
      vuint8 simReg8;
      vuint16 simReg16;
      vuint32 simReg32;
      initInfo->isMbCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isMbCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isMbCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
    }
# endif
# if defined(C_ENABLE_CAN_RAM_CHECK)
    if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
    { /* PowerOn */
      Can_SetMailboxState(Htrh, CAN_ENABLE_OBJ); /* SBSW_CAN_HL34 */
    }
    if( (initInfo->isMbCorrupt == kCanTrue) || (Can_GetMailboxState(Htrh) == CAN_DISABLED_OBJ) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      /*
      |<DataModelStart>| CanLL_InitMailboxFullCANDeactivate
      Relation_Context:
      RxFullCANSupport
      Relation:
      ChannelAmount
      RxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      if (initInfo->isMbCorrupt == kCanTrue)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST hwObjHandle, Htrh);
      }
    }
# endif
  }

# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
  CAN_DUMMY_STATEMENT(initInfo); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return errorId;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
#endif

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_InitRxBasicCAN
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitRxBasicCAN
Relation_Context:
RxBasicCANSupport
Relation:
CommonCan
OneChOpt
RamCheck
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxBasicCAN( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_HwHandleType hwObjHandle;
  Can_HwHandleType Htrh;
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->Controller;
# endif
  for (Htrh = Can_GetRxBasicHandleStart(canHwChannel); Htrh < Can_GetRxBasicHandleStop(canHwChannel); Htrh++)
  { /* #10 iterate over all BasicCAN RX mailboxes */
    for (hwObjHandle = Can_GetHwHandle(Htrh); hwObjHandle < (Can_HwHandleType)(Can_GetHwHandle(Htrh) + Can_GetDLC_FIFO(Htrh)); hwObjHandle++)
    {
# if defined(C_ENABLE_CAN_RAM_CHECK)
      /* #20 RAM check for BasicCAN RX mailboxes */
      initInfo->isMbCorrupt = kCanFalse; /* SBSW_CAN_HL36 */
      if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
      {
        initInfo->isMbCorrupt = CAN_RAM_CHECK_MAILBOX_RESULT(channel, (hwObjHandle)); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL36 */
      } 
# endif
      /* #30 initialize BasicCAN RX mailboxes (LL) */
      /*
      |<DataModelStart>| CanLL_InitMailboxBasicCAN
      Relation_Context:
      RxBasicCANSupport
      Relation:
      ChannelAmount
      RxBasicAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mObjType    = CAN_MSGOBJ_RXBASIC; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
      {
        /* just for simulation */
        vuint8 simReg8;
        vuint16 simReg16;
        vuint32 simReg32;
        initInfo->isMbCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
        initInfo->isMbCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      }
# if defined(C_ENABLE_CAN_RAM_CHECK)
      if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
      { /* PowerOn */
        Can_SetMailboxState(Htrh, CAN_ENABLE_OBJ); /* SBSW_CAN_HL34 */
      }
      if( (initInfo->isMbCorrupt == kCanTrue) || (Can_GetMailboxState(Htrh) == CAN_DISABLED_OBJ) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
      { /* #45 deactivate corrupt mailbox */
        /*
        |<DataModelStart>| CanLL_InitMailboxBasicCANDeactivate
        Relation_Context:
        RxBasicCANSupport
        Relation:
        ChannelAmount
        RxBasicAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        CAN_HL_INFO_INIT_PTR(canLLController)->MsgObj[hwObjHandle].mIsFree     = (vuint8)kCanFalse; /* SBSW_CAN_LL10 */
        if (initInfo->isMbCorrupt == kCanTrue)
        {  /* #50 notify application about corrupt mailbox and deactivate controller */
          CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST hwObjHandle, Htrh);
        }
      }
# endif
    }
  }
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
  CAN_DUMMY_STATEMENT(initInfo); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return errorId;
}  /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
#endif

/****************************************************************************
| NAME:             CanHL_InitEnd_InitMode
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitEnd_InitMode
Relation_Context:
Relation:
CommonCan
OneChOpt
RamCheck
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitEnd_InitMode( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanInitHandle  initObject; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->channel;
#endif
  initObject = initInfo->initObject;

#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  /* #10 Execute pattern RamCheck */
  if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
  {
    initInfo->isChCorrupt |= CAN_RAM_CHECK_END_REG_RESULT(channel); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL36 */
  } 
#endif

  /* #20 HW specific initialization end (LL) */
  /*
  |<DataModelStart>| CanLL_InitEndRegister
  Relation_Context:
  Relation:
  ChannelAmount
  TxPolling, IndividualPolling
  RxPolling, RxBasicCANSupport, RxFullCANSupport, IndividualPolling
  StatusPolling
  WakeupPolling
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  if ( (Can_GetLogStatus(channel) & kCanHwIsInit) == 0 ) /* only in power on initialisation */
  {
    vuint8 txInt, rxInt, errInt, wakInt;
  #if !defined( C_ENABLE_TX_POLLING ) || defined(C_ENABLE_INDIVIDUAL_POLLING)
    txInt = 1;
  #else
    txInt = 0;
  #endif
  #if (!defined( C_ENABLE_RX_BASICCAN_POLLING ) && defined( C_ENABLE_RX_BASICCAN_OBJECTS )) || \
      (!defined( C_ENABLE_RX_FULLCAN_POLLING )  && defined( C_ENABLE_RX_FULLCAN_OBJECTS )) || \
      defined(C_ENABLE_INDIVIDUAL_POLLING)
    rxInt = 1;
  #else
    rxInt = 0;
  #endif
  #if !defined( C_ENABLE_ERROR_POLLING )
    errInt = 1;
  #else
    errInt = 0;
  #endif
    wakInt = 1;
    CanCell_EnableInterrupts(channel, txInt, rxInt, errInt, wakInt); /* SBSW_CAN_LL08 */
    {
      /* just for simulation */
      vuint8 simReg8;
      vuint16 simReg16;
      vuint32 simReg32;
      initInfo->isChCorrupt |= CanHL_SetValue8(CAN_CHANNEL_CANPARA_FIRST &simReg8, 0x55u, 0xFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isChCorrupt |= CanHL_SetValue16(CAN_CHANNEL_CANPARA_FIRST &simReg16, 0xAAAAu, 0xFFFFu); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
      initInfo->isChCorrupt |= CanHL_SetValue32(CAN_CHANNEL_CANPARA_FIRST &simReg32, 0x55555555UL, 0xFFFFFFFFUL); /* SBSW_CAN_HL36 */ /* SBSW_CAN_LL14 */
    }
  }
  CanCell_AtomicEnd();

#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CAN_DUMMY_STATEMENT(initObject); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  return errorId;
}

/****************************************************************************
| NAME:             CanHL_InitEnd_StopMode
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitEnd_StopMode
Relation_Context:
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitEnd_StopMode( CanInitInfoPtrType initInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanInitHandle  initObject; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
  errorId = CAN_E_NO_ERROR;
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = initInfo->channel;
#endif
  initObject = initInfo->initObject;

  /* #10 HW specific initialization transition to STOP (LL) */
  /*
  |<DataModelStart>| CanLL_InitEnd_StopMode
  Relation_Context:
  Relation:
  ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  CanCell_StopChannel((const char*)Can_GetBusName(channel), channel); /* PRQA S 5013 */ /* MD_Can_5013_LL */
  CAN_HL_INFO_INIT_PTR(canLLController)->Mode = CAN_MODE_STOP; /* SBSW_CAN_LL10 */

#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CAN_DUMMY_STATEMENT(initObject); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  return errorId;
}

/****************************************************************************
| NAME:             CanHL_StopReinit
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_StopReinit
Relation_Context:
Wakeup, RamCheck
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_StopReinit( CAN_HW_CHANNEL_CANTYPE_ONLY ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 do a short initialization instead of full Can_InitController() in case of STOP transition (CanLL_StopReinit) */
  /* #20 Delete all Pending Interrupt and Pending flags for TX,RX,BUSOFF and WAKEUP */
  /*
  |<DataModelStart>| CanLL_StopReinit
  Relation_Context:
  Relation:
  ChannelAmount
  RxBasicAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  vuint16 canHwObjIdx;
  CanCell_AtomicBegin();
  for(canHwObjIdx=0; canHwObjIdx < CAN_HL_HW_RX_BASIC_STOPINDEX(channel); canHwObjIdx++)
  {
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL04 */
  #if !defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL04 */
  #endif
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL04 */
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL04 */
  }
  CanCell_AtomicEnd();
}

/****************************************************************************
| NAME:             CanHL_InitPowerOn
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitPowerOn
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_InitPowerOn(void)
{
  /* #10 HW general power on initialization (LL) */
  /*
  |<DataModelStart>| CanLL_InitPowerOn
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  /* For osCAN-Library CANoe here is nothing to do.*/
}

/****************************************************************************
| NAME:             CanHL_InitPowerOnChannelSpecific
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanHL_InitPowerOnChannelSpecific
Relation_Context:
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_InitPowerOnChannelSpecific( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* #10 HW controller specific power on initialization (LL) */
  /*
  |<DataModelStart>| CanLL_InitPowerOnChannelSpecific
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */

#if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_RxBasicMsgReceivedBegin
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxBasicMsgReceivedBegin
Relation_Context:
RxBasicCANSupport
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxBasicMsgReceivedBegin( CanRxBasicInfoPtrType rxBasicInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = rxBasicInfo->Controller; /* PRQA S 3199 */ /* MD_MSR_14.2 */
# endif
  rxObjHandle = rxBasicInfo->rxObjHandle; /* PRQA S 3199 */ /* MD_MSR_14.2 */

  /* #10 HW reception handling begin (LL) */
  /*
  |<DataModelStart>| CanLL_RxBasicMsgReceivedBegin
  Relation_Context:
  RxBasicCANSupport
  Relation:
  HardwareLoop
  CanFdSupport
  ChannelAmount
  RxBasicAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  CanCell_AtomicBegin();
  do
  { /* repeat until consistent data , this loop is only for emulation of a real hardware (with atomic data access no inconsistent data will appear) */
    Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending = (vuint8)kCanFalse; /* release pending object*/ /* SBSW_CAN_LL04 */
    Can_GetRxMsg(channel).IdRaw = Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.IdRaw; /* SBSW_CAN_LL03 */
    Can_GetRxMsg(channel).DlcRaw = Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DlcRaw; /* SBSW_CAN_LL03 */
  #if (CAN_FD_SUPPORT == CAN_FULL) 
    (void)memcpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DataFld, CAN_DECODE_DLI(Can_GetRxMsg(channel).DlcRaw)); /* PRQA S 3335,5115,1594,1510 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL05 */
  #else
    (void)memcpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DataFld, CAN_HL_MIN(8, CAN_DECODE_DLI(Can_GetRxMsg(channel).DlcRaw))); /* PRQA S 3335,5115,1594,1510 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL05 */
  #endif
  } while( Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue ); /* new message appear meanwhile -> repeat copy */ /* COV_CAN_LL_RECEIVE_LOOP */
  if ( Can_GetgCanController(channel).MsgObj[rxObjHandle].mCanOverrun == (vuint8)kCanTrue )
  {  /* read out gCanController here (inside CanCell_AtomicBegin) */
     /* reset overrun */
     Can_GetgCanController(channel).MsgObj[rxObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL04 */
     CAN_HL_INFO_RXBASIC_PTR(overrunOccur) = (vuint8)kCanTrue; /* SBSW_CAN_LL06 */
  } 
  else 
  {
     CAN_HL_INFO_RXBASIC_PTR(overrunOccur) = (vuint8)kCanFalse; /* SBSW_CAN_LL06 */
  }
  CanCell_AtomicEnd();

  /* #20 HW get mailbox pointer (LL) */
  /*
  |<DataModelStart>| CanLL_RxBasicGetCANObjPtr
  Relation_Context:
  RxBasicCANSupport
  Relation:
  ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  (rxBasicInfo->rxInfo.pChipMsgObj) = (CanChipMsgPtr) (&Can_GetRxMsg(channel)); /* SBSW_CAN_HL29 */

  /* #30 HW get data pointer (LL) */
  /*
  |<DataModelStart>| CanLL_RxBasicGetCANDataPtr
  Relation_Context:
  RxBasicCANSupport
  Relation:
  ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  (rxBasicInfo->rxInfo.pChipData) = (CanChipDataPtr) (&Can_GetRxMsg(channel).DataFld); /* PRQA S 0310 */ /* MD_Can_0310 */ /* SBSW_CAN_HL29 */

  retval = CAN_OK;
  return retval;
} /* PRQA S 6030 */ /* MD_MSR_STCYC*/

/****************************************************************************
| NAME:             CanHL_RxBasicMsgReceivedSearch
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxBasicMsgReceivedSearch
Relation_Context:
RxBasicCANSupport
Relation:
CommonCan
OneChOpt
ChannelAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxBasicMsgReceivedSearch( CanRxBasicInfoPtrType rxBasicInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL
  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = rxBasicInfo->Controller;
# endif
  rxObjHandle = rxBasicInfo->rxObjHandle;
  retval = CAN_NOT_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  /* #10 HRH Search linear (multiple BasicCAN) */
  for (rxBasicInfo->rxInfo.localHrh = Can_GetRxBasicHandleStart(canHwChannel); rxBasicInfo->rxInfo.localHrh < Can_GetRxBasicHandleStop(canHwChannel); rxBasicInfo->rxInfo.localHrh++) /* SBSW_CAN_HL29 */ /* SBSW_CAN_HL29 */ /* COV_CAN_BASICCAN_SEARCH_LINEAR */
  {
    if ( (rxObjHandle >= Can_GetHwHandle(rxBasicInfo->rxInfo.localHrh)) && (rxObjHandle < (CanObjectHandle)(Can_GetHwHandle(rxBasicInfo->rxInfo.localHrh)+Can_GetDLC_FIFO(rxBasicInfo->rxInfo.localHrh))) ) /* COV_CAN_BASICCAN_SEARCH_LINEAR */
    {
      retval = CAN_OK;
      break;
    }
  }
  return retval;
}

/****************************************************************************
| NAME:             CanHL_RxBasicMsgReceivedEnd
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxBasicMsgReceivedEnd
Relation_Context:
RxBasicCANSupport
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxBasicMsgReceivedEnd( CanRxBasicInfoPtrType rxBasicInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = rxBasicInfo->Controller; /* PRQA S 3199 */ /* MD_MSR_14.2 */
# endif
  rxObjHandle = rxBasicInfo->rxObjHandle; /* PRQA S 3199 */ /* MD_MSR_14.2 */

  /* #10 HW release mailbox (LL) */
  /*
  |<DataModelStart>| CanLL_RxBasicReleaseObj
  Relation_Context:
  RxBasicCANSupport
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */

  /*
  |<DataModelStart>| CanLL_RxBasicMsgReceivedEnd
  Relation_Context:
  RxBasicCANSupport
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  /* for osCAN-Library CANoe, here is nothing to do */

  CAN_DUMMY_STATEMENT(rxObjHandle);  /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller);   /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}

# if defined(C_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_RxBasicMsgReceivePolling
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_RxBasicMsgReceivePolling
Relation_Context:
RxBasicCANSupport, RxPolling
Relation:
CommonCan
IndividualPolling
OneChOpt
ChannelAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxBasicMsgReceivePolling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  /*
  |<DataModelStart>| CanLL_RxBasicCANTaskLocalParameter
  Relation_Context:
  RxBasicCANSupport, RxPolling
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 check global pending flag (over all mailboxes - if available) */
  /*
  |<DataModelStart>| CanLL_if_RxBasicIsGlobalIndPending
  Relation_Context:
  RxBasicCANSupport, RxPolling
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  {
    /* #20 loop over all BasicCAN mailboxes */
    { /* HW specific loop over all BasicCAN hardware objects assigned to the BasicCAN which starts with rxObjHandle */
      /*
      |<DataModelStart>| CanLL_Loop_RxBasicCanTaskLoopBegin
      Relation_Context:
      RxBasicCANSupport, RxPolling
      Relation:
      IndividualPolling
      ChannelAmount
      RxBasicAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      Can_HwHandleType Htrh;
      vuint8 bCanIdx; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
      CanObjectHandle bCanSize;
      CanObjectHandle bCanBaseHwIdx;
      CanObjectHandle bCanWrittenPointer;
      boolean isPending;
      bCanIdx=0;
      for (Htrh = Can_GetRxBasicHandleStart(channel); Htrh < Can_GetRxBasicHandleStop(channel); )
      { /* loop over all BasicCAN mailboxes */
        bCanBaseHwIdx = Can_GetHwHandle(Htrh);
      #  if defined( C_ENABLE_INDIVIDUAL_POLLING )
        if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,bCanBaseHwIdx) == (uint8)0 )
        { /* nothing to do for this object in polling */
          Htrh++; /* PRQA S 2469 */ /* Used to acelerate search - no risk for endless loop */
          bCanIdx++;
          continue; /* PRQA S 0770 */ /* Used to acelerate search */
        }
      #  endif
        bCanSize = Can_GetDLC_FIFO(Htrh);
        bCanWrittenPointer = Can_GetgCanController(channel).BCanWrittenPointer[bCanIdx] + bCanBaseHwIdx;
        isPending = FALSE;
        /* search for next mailbox BasicCAN-FIFO-element to be executed  */
        rxObjHandle = bCanWrittenPointer;
        do
        {
          rxObjHandle++;
          if (rxObjHandle >= (bCanBaseHwIdx+bCanSize))
          {
            rxObjHandle = bCanBaseHwIdx;
          }
          if (Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue)
          {
            isPending = TRUE;
            break;
          }
        } while (rxObjHandle != bCanWrittenPointer);
        if (isPending == FALSE)
        { /* no more pending element in BasicCAN-FIFO so continue with next BasicCAN */
          Htrh++; /* PRQA S 2469 */ /* Used to acelerate search - no risk for endless loop */
          bCanIdx++;
          continue; /* PRQA S 0770 */ /* Used to acelerate search */
        }
#  if defined(C_ENABLE_INDIVIDUAL_POLLING) || defined(CAN_HWOBJINDIVPOLLING) /* COV_CAN_HWOBJINDIVPOLLING */
        /* #30 check individual polling flag */
        if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,rxObjHandle) != (uint8)0 ) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
        { 
          /* #40 check mailbox pending flag */
          /*
          |<DataModelStart>| CanLL_if_RxBasicIsObjIndPending
          Relation_Context:
          RxBasicCANSupport, RxPolling
          Relation:
          Parameter_PreCompile:
          Parameter_Data:
          Constrain:
          |<DataModelEnd>|
          */
          /* done by CanLL_Loop_RxBasicCanTaskLoopBegin 
          if (Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue)
          */
          {
            /* #50 disable CAN interrupts \trace SPEC-1569, SPEC-3785 */
            Can_DisableControllerInterrupts((uint8)channel);
            /* #60 mailbox notification CanBasicCanMsgReceived() */
            CanBasicCanMsgReceived(CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
            /* #70 enable CAN interrupts */
            Can_EnableControllerInterrupts((uint8)channel);
          }
        }
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* C_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_RxFullMsgReceivedBegin
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxFullMsgReceivedBegin
Relation_Context:
RxFullCANSupport
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxFullMsgReceivedBegin( CanRxFullInfoPtrType rxFullInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = rxFullInfo->Controller;
# endif
  rxObjHandle = rxFullInfo->rxObjHandle;

  /* #10 HW reception handling begin (LL) */
  /*
  |<DataModelStart>| CanLL_RxFullMsgReceivedBegin
  Relation_Context:
  RxFullCANSupport
  Relation:
  HardwareLoop
  CanFdSupport
  ChannelAmount
  RxFullAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  CanCell_AtomicBegin();
  do
  { /* repeat until consistent data , this loop is only for emulation of a real hardware (with atomic data access no inconsistent data will appear) */
    Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending = (vuint8)kCanFalse; /* release pending object*/ /* SBSW_CAN_LL04 */
    Can_GetRxMsg(channel).IdRaw = Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.IdRaw; /* SBSW_CAN_LL03 */
    Can_GetRxMsg(channel).DlcRaw = Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DlcRaw; /* SBSW_CAN_LL03 */
  #if (CAN_FD_SUPPORT == CAN_FULL) 
    (void)memcpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DataFld, CAN_DECODE_DLI(Can_GetRxMsg(channel).DlcRaw)); /* PRQA S 3335,5115,1594,1510 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL05 */
  #else
    (void)memcpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DataFld, CAN_HL_MIN(8, CAN_DECODE_DLI(Can_GetRxMsg(channel).DlcRaw))); /* PRQA S 3335,5115,1594,1510 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL05 */
  #endif
  } while( Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue ); /* new message appear meanwhile -> repeat copy */ /* COV_CAN_LL_RECEIVE_LOOP */
  if ( Can_GetgCanController(channel).MsgObj[rxObjHandle].mCanOverrun == (vuint8)kCanTrue )
  {  /* read out gCanController here (inside CanCell_AtomicBegin) */
     /* reset overrun */
     Can_GetgCanController(channel).MsgObj[rxObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL04 */
     CAN_HL_INFO_RXFULL_PTR(overrunOccur) = (vuint8)kCanTrue; /* SBSW_CAN_LL07 */
  } 
  else
  {
     CAN_HL_INFO_RXFULL_PTR(overrunOccur) = (vuint8)kCanFalse; /* SBSW_CAN_LL07 */
  }
  CanCell_AtomicEnd();
  
  /* #20 HW get mailbox pointer (LL) */
  /*
  |<DataModelStart>| CanLL_RxFullGetCANObjPtr
  Relation_Context:
  RxFullCANSupport
  Relation:
  ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  (rxFullInfo->rxInfo.pChipMsgObj) = (CanChipMsgPtr) (&Can_GetRxMsg(channel)); /* SBSW_CAN_HL30 */

  /* #30 HW get data pointer (LL) */
  /*
  |<DataModelStart>| CanLL_RxFullGetCANDataPtr
  Relation_Context:
  RxFullCANSupport
  Relation:
  ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  (rxFullInfo->rxInfo.pChipData) = (CanChipDataPtr) (&Can_GetRxMsg(channel).DataFld); /* PRQA S 0310 */ /* MD_Can_0310 */ /* SBSW_CAN_HL30 */

  retval = CAN_OK;
  return retval;
}

/****************************************************************************
| NAME:             CanHL_RxFullMsgReceivedSearch
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxFullMsgReceivedSearch
Relation_Context:
RxFullCANSupport
Relation:
CommonCan
OneChOpt
ChannelAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxFullMsgReceivedSearch( CanRxFullInfoPtrType rxFullInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = rxFullInfo->channel;
# endif
  rxObjHandle = rxFullInfo->rxObjHandle;

  /* #20 HRH calculate */
  rxFullInfo->rxInfo.localHrh = (Can_HwHandleType)((Can_HwHandleType)((Can_HwHandleType)rxObjHandle - CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)) + Can_GetRxFullHandleStart(canHwChannel)); /* SBSW_CAN_HL30 */

  retval = CAN_OK;
  return retval;
}

/****************************************************************************
| NAME:             CanHL_RxFullMsgReceivedEnd
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxFullMsgReceivedEnd
Relation_Context:
RxFullCANSupport
Relation:
CommonCan
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxFullMsgReceivedEnd( CanRxFullInfoPtrType rxFullInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  CAN_CHANNEL_CANTYPE_LOCAL

  /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  Controller = rxFullInfo->channel;
# endif
  rxObjHandle = rxFullInfo->rxObjHandle;

  /* #10 HW release mailbox (LL) */
  /*
  |<DataModelStart>| CanLL_RxFullReleaseObj
  Relation_Context:
  RxFullCANSupport
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */

  /*
  |<DataModelStart>| CanLL_RxFullMsgReceivedEnd
  Relation_Context:
  RxFullCANSupport
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */

  CAN_DUMMY_STATEMENT(rxObjHandle);  /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller);   /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}

# if defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_RxFullMsgReceivePolling
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_RxFullMsgReceivedEnd
Relation_Context:
RxFullCANSupport, RxPolling
Relation:
CommonCan
IndividualPolling
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxFullMsgReceivePolling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_HwHandleType Htrh;
  CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  /*
  |<DataModelStart>| CanLL_RxFullCANTaskLocalParameter
  Relation_Context:
  RxFullCANSupport, RxPolling
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 check global pending flag (over all mailboxes - if available) */
  /*
  |<DataModelStart>| CanLL_if_RxFullIsGlobalIndPending
  Relation_Context:
  RxFullCANSupport, RxPolling
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  {
    /* #20 loop over all FullCAN mailboxes */
    for (Htrh = Can_GetRxFullHandleStart(canHwChannel); Htrh < Can_GetRxFullHandleStop(canHwChannel); Htrh++ )
    { 
      rxObjHandle = Can_GetHwHandle(Htrh);
#  if defined(C_ENABLE_INDIVIDUAL_POLLING) || defined(CAN_HWOBJINDIVPOLLING) /* COV_CAN_HWOBJINDIVPOLLING */
      /* #30 check individual polling flag */
      if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,rxObjHandle) != (uint8)0 ) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
      { /* #40 check mailbox pending flag */
        /*
        |<DataModelStart>| CanLL_if_RxFullIsObjIndPending
        Relation_Context:
        RxFullCANSupport, RxPolling
        Relation:
        ChannelAmount
        RxFullAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        if (Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue)
        {
          /* #50 disable CAN Interrupt \trace SPEC-1569, SPEC-3785 */
          Can_DisableControllerInterrupts((uint8)channel);
          /* #60 mailbox notification CanFullCanMsgReceived() */
          CanFullCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
          /* #70 enable CAN Interrupt */
          Can_EnableControllerInterrupts((uint8)channel);
        }
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* C_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS) || defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_RxMsgReceivedNotification
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_RxMsgReceivedNotification
Relation_Context:
RxFullCANSupport, RxBasicCANSupport
Relation:
RxQueue, DevErrorDetect
RxQueue, CanFdSupport
RxQueue, GenericPreCopy, MirrorMode
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_RxMsgReceivedNotification( CAN_CHANNEL_CANTYPE_FIRST CanRxInfoPtrType rxInfo ) /* PRQA S 3673 */ /* MD_Can_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
# if !defined(C_ENABLE_RX_QUEUE) && (CAN_GENERIC_PRECOPY == STD_ON)
  Can_ReturnType generic_retval;
# endif
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 RX queue */
# if defined(C_ENABLE_RX_QUEUE)
  /**************************** RxQueue handling **********************************/
    /* #20 EXCLUSIVE_AREA_4 secure RxQueue data handling */
  Can_EnterCritical(CAN_EXCLUSIVE_AREA_4);
  if (Can_GetRxQueueInfo().Count < Can_GetSizeOfRxQueueBuffer()) /* COV_CAN_GENDATA_FAILURE */
  { /* #30 Queue not full: */
    /* #45 copy HRH, ID, DLC and data to RX queue */
    Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).Hrh = rxInfo->localHrh; /* SBSW_CAN_HL18 */
    Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).id  = rxInfo->localId; /* SBSW_CAN_HL18 */
    Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).dlc = rxInfo->localDlc; /* SBSW_CAN_HL18 */

#  if (CAN_DEV_ERROR_DETECT == STD_ON)
    if (canRxQueueMaxDataSize < rxInfo->localDlc) /* CM_CAN_HL17 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_E_PARAM_DLC;
    }
    else
#  endif
    {

#  if defined(C_ENABLE_CAN_FD_FULL)
      /* #46 copy data with VStdLib optimized copy routine for CAN-FD messages (use alignment info) */
      VStdMemCpy(Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).data, rxInfo->pChipData, rxInfo->localDlc); /* SBSW_CAN_HL19 */ /* PRQA S 0312 */ /* MD_Can_0312 */
#  else
      {
        uint8_least i;
        for (i = 0; i < rxInfo->localDlc; i++)
        {
          Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).data[i] = CanRxActualData(rxInfo,i); /* SBSW_CAN_HL19 */
        }
      }
#  endif
    }
      /* #47 increase pointer to next queue element */
    Can_GetRxQueueInfo().Count++;
    if (Can_GetRxQueueInfo().WriteIndex < (uint16)(Can_GetSizeOfRxQueueBuffer() - 1) ) /* CM_CAN_HL24 */
    {
      Can_GetRxQueueInfo().WriteIndex++;
    }
    else
    {
      Can_GetRxQueueInfo().WriteIndex = 0; /* CM_CAN_HL24 */
    }
  }
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  else 
  { /* #50 Queue full: last message will be lost -> overrun in queue do not change ReadIndex to override latest message because inconsistency problem (currently reading queue) */
    errorId = CAN_E_RXQUEUE;
  }
#  endif
  Can_LeaveCritical(CAN_EXCLUSIVE_AREA_4);
# else /* C_ENABLE_RX_QUEUE */
  /* #100 no RX queue */
  /* -------------------------- Interface + APPL Notifications --- */
    /* #110 call Appl_GenericPrecopy() depend on configuration and if mirror mode is active */
#  if (CAN_GENERIC_PRECOPY == STD_ON)
#   if defined(C_ENABLE_MIRROR_MODE)
  if (Can_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
  { /* do not call Appl_GenericPrecopy() in case mirror mode was disabled */
    generic_retval = CAN_OK;
  }
  else
#   endif
  { /* \trace SPEC-3960 */
    generic_retval = Appl_GenericPrecopy((uint8)channel, (rxInfo->localId & (CAN_ID_IDE_MASK | CAN_ID_MASK)), rxInfo->localDlc, (Can_DataPtrType)rxInfo->pChipData);  /* PRQA S 0312 */ /* MD_Can_0312 */ /* SBSW_CAN_HL06 */
  }
  if (generic_retval == CAN_OK) /* \trace SPEC-15119 */
#  endif
  { /* #120 call CanIf_RxIndication() if mirror mode is no active or Appl_GenericPrecopy() return OK \trace SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687 */
    CanIf_RxIndication(rxInfo->localHrh, rxInfo->localId, rxInfo->localDlc, (Can_DataPtrType)rxInfo->pChipData); /* PRQA S 0312 */ /* MD_Can_0312 */ /* SBSW_CAN_HL07 */
  }
# endif
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  return errorId;
}
#endif

#if defined(C_ENABLE_RX_QUEUE)
/****************************************************************************
| NAME:             CanHL_RxQueueExecution
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_RxQueueExecution
Relation_Context:
RxQueue
Relation:
GenericPreCopy, OneChOpt, CommonCan
GenericPreCopy, OneChOpt, DevErrorDetect
GenericPreCopy, MirrorMode
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_RxQueueExecution( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute RxQueue (over all channels) */
  while ( Can_GetRxQueueInfo().Count != (uint16)0 )
  { /* #20 over all filled RxQueue elements */
# if (CAN_GENERIC_PRECOPY == STD_ON)
    Can_ReturnType generic_retval;
    CAN_CHANNEL_CANTYPE_LOCAL
    generic_retval = CAN_NOT_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_GetMailboxController(Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).Hrh);
#   if (CAN_DEV_ERROR_DETECT == STD_ON)
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    { /* #30 Check controller is valid */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#   endif
#  endif
#  if defined(C_ENABLE_MIRROR_MODE)
    if (Can_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
    { /* #40 MirrorMode: only when mirror is activated for this controller */
      generic_retval = CAN_OK;
    }
    else
#  endif
    { /* #50 call Appl_GenericPrecopy() */
      generic_retval = Appl_GenericPrecopy( (uint8)channel, /* PRQA S 2004 */ /* MD_Can_Asr_2004 */
                (Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).id & (CAN_ID_IDE_MASK | CAN_ID_MASK)),
                Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).dlc,
                (Can_DataPtrType)Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).data );  /* PRQA S 0312 */ /* MD_Can_0312 */ /* SBSW_CAN_HL06 */
    }
    if (generic_retval == CAN_OK)
# endif
    { /* #60 call CanIf_RxIndication() if Appl_GenericPrecopy() return OK or no generic precopy is used \trace SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687 */
      CanIf_RxIndication( Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).Hrh,
              Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).id,
              Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).dlc,
              (Can_DataPtrType)Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).data ); /* PRQA S 0312 */ /* MD_Can_0312 */ /* SBSW_CAN_HL07 */
    }
    /* #70 EXCLUSIVE_AREA_4 secure RxQueue handling */
    Can_EnterCritical(CAN_EXCLUSIVE_AREA_4);
    /* #80 get next RxQueue element pointer */
    if (Can_GetRxQueueInfo().ReadIndex >= (uint16)(Can_GetSizeOfRxQueueBuffer() - 1) ) /* CM_CAN_HL25 */
    {
      Can_GetRxQueueInfo().ReadIndex = 0; /* CM_CAN_HL25 */
    }
    else
    {
      Can_GetRxQueueInfo().ReadIndex++;
    }
    Can_GetRxQueueInfo().Count--;
    Can_LeaveCritical(CAN_EXCLUSIVE_AREA_4);
  }
  return errorId;
}
#endif

#if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */
/****************************************************************************
| NAME:             CanHL_TxTaskConfPendingHandling
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_TxTaskConfPendingHandling
Relation_Context:
TxPolling
Relation:
CommonCan
IndividualPolling
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_TxTaskConfPendingHandling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  CanObjectHandle txObjHandle;
  /*
  |<DataModelStart>| CanLL_TxTaskLocalParameter
  Relation_Context:
  TxPolling, HwCancelTx
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  /*
  |<DataModelStart>| CanLL_if_TxIsGlobalConfPending
  Relation_Context:
  TxPolling
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  {
    for ( txObjHandle = CAN_HL_HW_TX_STARTINDEX(canHwChannel); txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel); txObjHandle++ )
    { /* #10 over all TX mailboxes */
# if defined(C_ENABLE_INDIVIDUAL_POLLING) || defined(CAN_HWOBJINDIVPOLLING) /* COV_CAN_HWOBJINDIVPOLLING */
      if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,txObjHandle) != (uint8)0 ) /* COV_CAN_HWOBJINDIVPOLLING */
# endif
      { /* #20 polling object? do individual polling */
        /*
        |<DataModelStart>| CanLL_if_TxIsObjConfPending
        Relation_Context:
        TxPolling
        Relation:
        ChannelAmount
        TxBasicAmount
        TxFullAmount
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        if (Can_GetgCanController(channel).MsgObj[txObjHandle].mIsPending == (vuint8)kCanTrue)
        {
          /* #25 Lock CAN controller interrupts. (avoid nested call of confirmation (individual polling) and CancelTx interrupt confirmation out of Can interrupt like TP) */
          Can_DisableControllerInterrupts((uint8)channel);
          /* #30 confirmation pending - do handling \trace SPEC-1574, SPEC-3779 */
          CanHL_TxConfirmation( CAN_HW_CHANNEL_CANPARA_FIRST txObjHandle );
          Can_EnableControllerInterrupts((uint8)channel);
        }
      }
    }
  }
}
#endif

#if defined(C_ENABLE_CANCEL_TX_IN_HW) 
#endif

/* CODE CATEGORY 2 START */
/****************************************************************************
| NAME:             CanLL_CanInterruptDisable
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanLL_CanInterruptDisable
Relation_Context:
IntLock
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptDisable( CanChannelHandle canHwCh, CanLLCanIntOldPtrType localInterruptOldFlagPtr )
{
  /* #10 save old CAN interrupt status in given parameter */
  *(localInterruptOldFlagPtr) = Can_GetgCanController(canHwCh).IntEnable; /* SBSW_CAN_LL12 */
  /* #20 deactivate the CAN interrupt */
  CanCell_EnableInterrupts(canHwCh, 0, 0, 0, 0); /* SBSW_CAN_LL08 */
}
/* CODE CATEGORY 2 END */

/* CODE CATEGORY 2 START */
/****************************************************************************
| NAME:             CanLL_CanInterruptRestore
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanLL_CanInterruptRestore
Relation_Context:
IntLock
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptRestore( CanChannelHandle canHwCh, tCanLLCanIntOld localInterruptOldFlag )
{
  /* #10 restore old CAN interrupt status given as parameter */
  CanCell_EnableInterrupts((canHwCh), (((localInterruptOldFlag) & CAN_INT_TX) / CAN_INT_TX), (((localInterruptOldFlag) & CAN_INT_RX) / CAN_INT_RX),   /* SBSW_CAN_LL08 */
                                      (((localInterruptOldFlag) & CAN_INT_ERR) / CAN_INT_ERR), (((localInterruptOldFlag) & CAN_INT_WAKEUP) / CAN_INT_WAKEUP));
  CanCell_CallInterrupts(canHwCh);
}
/* CODE CATEGORY 2 END */

#if defined( C_ENABLE_SLEEP_WAKEUP )
/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_WakeUpHandling
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanLL_WakeUpHandling
Relation_Context:
Wakeup
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_WakeUpHandling( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* notify the application here and call WAKEUP handling */
  /* #20 call wakeup transition handling */
  /* stop transition will be triggered after notification by CanIf */
  /* In case of CommonCAN, it might be necessary to call CanWakeUp() / CAN_WAKEUP() even if the hardware
     wakes up automatically to make sure all associated HW channels are awaken. */
  CAN_WAKEUP( channel ); /* SBSW_CAN_HL01 */
  /* #30 call wakeup indication */
  APPL_CAN_WAKEUP( channel );
}
/* CODE CATEGORY 4 END */
#endif /*C_ENABLE_SLEEP_WAKEUP*/

#if defined( C_ENABLE_CAN_RAM_CHECK )
/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_IsMailboxCorrupt
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanLL_IsMailboxCorrupt
Relation_Context:
RamCheck, TxFullCANSupport
RamCheck, RxBasicCANSupport
RamCheck, RxFullCANSupport
Relation:
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_IsMailboxCorrupt( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* do nothing here CANoe is just a emulation without HW bug detection */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CAN_DUMMY_STATEMENT(hwObjHandle); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  /* #10 just simulate a RAM check without issue */
  return(kCanFalse);
}
/* CODE CATEGORY 4 END */

# if defined( C_ENABLE_CAN_RAM_CHECK_EXTENDED )
/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_InitBeginIsRegisterCorrupt
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanLL_InitBeginIsRegisterCorrupt
Relation_Context:
RamCheck
Relation:
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBeginIsRegisterCorrupt( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* do nothing here CANoe is just a emulation without HW bug detection */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  /* #10 just simulate a RAM check without issue */
  return(kCanFalse);
}
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_InitEndIsRegisterCorrupt
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanLL_InitEndIsRegisterCorrupt
Relation_Context:
RamCheck
Relation:
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEndIsRegisterCorrupt( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* do nothing here CANoe is just a emulation without HW bug detection */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  /* #10 just simulate a RAM check without issue */
  return(kCanFalse);
}
/* CODE CATEGORY 4 END */
# endif
#endif

#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS ) || defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanLL_RxIndicationMsgCopy
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| CanLL_RxIndicationMsgCopy
Relation_Context:
RamCheck, RxFullCANSupport
RamCheck, RxBasicCANSupport
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE)  CanLL_RxIndicationMsgCopy( vuint8 canChannel, CanObjectHandle rxObjHandle, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 maxDlc;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 copy message data to RAM */
  if ((flags & CANOEAPI_CAN_EDL) == CANOEAPI_CAN_EDL)
  {
    Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mMsg.IdRaw = id | CAN_ID_FD_MASK; /* SBSW_CAN_LL04 */
  }
  else
  {
    Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mMsg.IdRaw = id; /* SBSW_CAN_LL04 */
  }

  maxDlc = sizeof(Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mMsg.DataFld);
  dataLength = (dataLength > maxDlc) ? maxDlc : dataLength; /* COV_CAN_LL_EMULATION */
  Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mMsg.DlcRaw = CAN_ENCODE_DLI(dataLength); /* SBSW_CAN_LL04 */
  (void)memcpy( (void*)Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mMsg.DataFld, (void*)data, CAN_HL_MIN(CAN_MAX_RX_DATALEN,dataLength)); /* PRQA S 311,3335,5115,1594,1510 */ /* MD_Can_0311, compiler intrinsic function not declared */ /* SBSW_CAN_LL04 */

  /* #20 set control bits */
# if defined( C_ENABLE_CAN_RAM_CHECK )
  if (Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mIsFree == (vuint8)kCanTrue) /* Mailbox is deactivated by RAM_CHECK */ /* COV_CAN_LL_EMULATION */
# endif
  {
    if (Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue)
    {
      Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mCanOverrun = (vuint8)kCanTrue; /* SBSW_CAN_LL04 */
    }
    else
    {
      Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mIsPending = (vuint8)kCanTrue; /* SBSW_CAN_LL04 */
    }
  }
} /* PRQA S 6060 */ /* MD_MSR_STPAR */
/* CODE CATEGORY 1 END */
#endif

# define CAN_STOP_SEC_STATIC_CODE  /*---------------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/********* STATIC CODE STOP HERE - GLOBAL START ************************************* */

# define CAN_START_SEC_CODE  /*---------------------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanSetHighVoltageMode
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanSetHighVoltageMode
Relation_Context:
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) CanSetHighVoltageMode( CAN_CHANNEL_CANTYPE_FIRST vuint8 mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 set high voltage mode */
  Can_GetgCanController(channel).HighVoltageMode = mode; /* SBSW_CAN_LL08 */
}
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             Can_RxIndication
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| Can_RxIndication
Relation_Context:
Relation:
RxFullCANSupport
RxBasicCANSupport
IDType
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_RxIndication( vuint8 canChannel, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data ) /* PRQA S 3408 */ /* used by vttctrl.h */
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle rxObjHandle=0; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 RX indication handling */
  if ((canChannel < kCanNumberOfChannels)
      && (Can_GetgCanController(canChannel).Mode != CAN_MODE_STOP) /* emulated so ignore message while STOP */
      && ((flags & CANOEAPI_CAN_RemoteFrame) != CANOEAPI_CAN_RemoteFrame) /* reject RemoteFrames */
# if !defined( C_ENABLE_MIXED_ID )
#  if defined( C_ENABLE_EXTENDED_ID )
      && (((id) & kCanIdTypeExt) == kCanIdTypeExt) /* wrong id type passed HW-filter (may not closed so just reject) */
#  else
      && (((id) & kCanIdTypeExt) != kCanIdTypeExt) /* wrong id type passed HW-filter (may not closed so just reject) */
#  endif
# endif
     ) /* COV_CAN_LL_EMULATION */
  {
 
    if ( Can_GetgCanController(canChannel).Mode == CAN_MODE_SLEEP )
    {
      Can_GetgCanController(canChannel).WakeupPending = (vuint8)kCanTrue; /* SBSW_CAN_LL08 */
      /* remember wakeup message to do handle it afterwards */
      /* continue with reception after wakeup notification */
    }

#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
    for (rxObjHandle=CAN_HL_HW_RX_FULL_STARTINDEX(canChannel); rxObjHandle < CAN_HL_HW_RX_FULL_STOPINDEX(canChannel); rxObjHandle++) /* COV_CAN_HARDWARE_FAILURE */
    {
      if ( (Can_GetgCanController(canChannel).MsgObj[rxObjHandle].mMsg.IdRaw | CAN_ID_FD_MASK) == (id | CAN_ID_FD_MASK) )
      {
        CanLL_RxIndicationMsgCopy(canChannel, rxObjHandle, id, flags, dataLength, data); /* SBSW_CAN_LL11 */
        return;
      }
    }
#endif
#if defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
    {
      vuint8 filterIdx;
      for (filterIdx = Can_GetFilterStartIdx(canChannel); filterIdx < Can_GetFilterStartIdx(canChannel+1); filterIdx++) /* COV_CAN_HARDWARE_FAILURE */
      {
        if ( (Can_GetMaskOfFilter(filterIdx) & id ) == (Can_GetMaskOfFilter(filterIdx) & Can_GetCodeOfFilter(filterIdx)) ) /* COV_CAN_HARDWARE_FAILURE */
        {
          vuint8 bCanLastWrittenPointer = Can_GetgCanController(canChannel).BCanWrittenPointer[Can_GetBCanLogIdxOfFilter(filterIdx)];
          if ( (bCanLastWrittenPointer + 1) >= Can_GetBCanHwSizeOfFilter(filterIdx) )
          {
            bCanLastWrittenPointer = 0;
          } else {
            bCanLastWrittenPointer++;
          }
          rxObjHandle = Can_GetBCanHwIdxOfFilter(filterIdx) + bCanLastWrittenPointer;
          CanLL_RxIndicationMsgCopy(canChannel, rxObjHandle, id, flags, dataLength, data); /* SBSW_CAN_LL11 */
          Can_GetgCanController(canChannel).BCanWrittenPointer[Can_GetBCanLogIdxOfFilter(filterIdx)] = bCanLastWrittenPointer; /* SBSW_CAN_LL08 */
          return;
        }
      }
    }
#endif
  }
  else 
  {
    assertHardware((canChannel < kCanNumberOfChannels), (canChannel), (kErrorWrongChannelForMsgEvent)); /* COV_CAN_HARDWARE_FAILURE */
  }
#if !defined(C_ENABLE_RX_FULLCAN_OBJECTS ) && !defined(C_ENABLE_RX_BASICCAN_OBJECTS)
  CAN_DUMMY_STATEMENT(data); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CAN_DUMMY_STATEMENT(dataLength); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CAN_DUMMY_STATEMENT(rxObjHandle); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 2006 */ /* MD_Can_2006 */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             Can_TxConfirmation
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| Can_TxConfirmation
Relation_Context:
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_TxConfirmation( vuint8 canChannel, vuint32 id, uint32 flags, vuint8 dataLength, const vuint8* data ) /* PRQA S 3408 */ /* used by vttctrl.h */
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle txObjHandle;
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((canChannel < kCanNumberOfChannels),canChannel,kErrorWrongChannelForMsgEvent); /* lint !e506 */ /* COV_CAN_HARDWARE_FAILURE */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 TX confirmation handling */
  if ( Can_GetgCanController(canChannel).Mode != CAN_MODE_START ) /* COV_CAN_LL_EMULATION */
  {
     return; /* emulated so ignore message while !START */
  }
  /* search the TX object, which sends this message */
  for (txObjHandle=CAN_HL_HW_TX_STARTINDEX(canChannel); txObjHandle<CAN_HL_HW_TX_STOPINDEX(canChannel); txObjHandle++) /* COV_CAN_HARDWARE_FAILURE */
  {
    if ( ((Can_GetgCanController(canChannel).MsgObj[txObjHandle].mMsg.IdRaw | CAN_ID_FD_MASK) == (id | CAN_ID_FD_MASK)) && (Can_GetgCanController(canChannel).MsgObj[txObjHandle].mIsFree != (vuint8)kCanTrue)) /* COV_CAN_LL_EMULATION */
    {
      Can_GetgCanController(canChannel).MsgObj[txObjHandle].mIsPending = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
      break;
    }
  }
  CAN_DUMMY_STATEMENT(flags); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CAN_DUMMY_STATEMENT(dataLength); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CAN_DUMMY_STATEMENT(data); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
} /* PRQA S 2006 */ /* MD_Can_2006 */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             Can_ErrIndication
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| Can_ErrIndication
Relation_Context:
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_ErrIndication( vuint8 canChannel, vuint32 errCode, vuint8 errCntTx, vuint8 errCntRx ) /* PRQA S 3408 */ /* used by vttctrl.h */
{
  /* ----- Local Variables ---------------------------------------------- */
  assertHardwareReturn((canChannel < kCanNumberOfChannels),canChannel,kErrorWrongChannelForMsgEvent); /* lint !e506 */ /* COV_CAN_HARDWARE_FAILURE */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Error handling */
  Can_GetgCanController(canChannel).ErrCode  = errCode; /* SBSW_CAN_LL08 */
  Can_GetgCanController(canChannel).ErrCntTx = errCntTx; /* SBSW_CAN_LL08 */
  Can_GetgCanController(canChannel).ErrCntRx = errCntRx; /* SBSW_CAN_LL08 */
} /* PRQA S 2006 */ /* MD_Can_2006 */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptTx
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanInterruptTx
Relation_Context:
Relation:
OneChOpt
RuntimeMeasurement
TxPolling, IndividualPolling
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptTx( vuint8 canChannel ) /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#else
  CanChannelHandle channel; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  channel = canChannel; /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((channel < kCanNumberOfChannels),channel,kErrorWrongChannelForMsgEvent); /* PRQA S 3201,3325 */ /* MD_Can_3201,MD_Can_3325 */ /* COV_CAN_LL_EMULATION */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 TX interrupt */
#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptTx);
#endif
#if !defined( C_ENABLE_TX_POLLING ) || defined( C_ENABLE_INDIVIDUAL_POLLING )
  {
    CanObjectHandle txObjHandle;
  /* search the TX objects, which generate interrupt for this message */
    for (txObjHandle=CAN_HL_HW_TX_STARTINDEX(channel); txObjHandle<CAN_HL_HW_TX_STOPINDEX(channel); txObjHandle++) /* COV_CAN_HARDWARE_FAILURE */
    {
      if ( Can_GetgCanController(channel).MsgObj[txObjHandle].mIsPending == (vuint8)kCanTrue )
      {
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
        if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,txObjHandle) == (uint8)0 )
# endif
        {
          CanHL_TxConfirmation(CAN_CHANNEL_CANPARA_FIRST txObjHandle);
        }
      }
    }
  }
#endif
#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptTx);
#endif
} /* CanInterruptTx */ /* PRQA S 2006 */ /* MD_Can_2006 */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptWakeup
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanInterruptWakeup
Relation_Context:
Wakeup
Relation:
OneChOpt
RuntimeMeasurement
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptWakeup( vuint8 canChannel ) /* PRQA S 3673 */ /* CANoe API provide an none const pointer but this is not used here */ /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_ENABLE_SLEEP_WAKEUP )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
  CanChannelHandle channel; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  channel = canChannel; /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Wakeup interrupt */
# if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptWakeup);
# endif
  if (CanLL_HwIsSleep(CAN_CHANNEL_CANPARA_ONLY) && (Can_GetgCanController(channel).WakeupPending == (vuint8)kCanTrue)) /* COV_CAN_LL_EMULATION */
  {
    CanLL_WakeUpHandling(CAN_CHANNEL_CANPARA_ONLY);
    /* this message only WAKEUP the cell, and is received */
    /* visbir: VTT only: WAKEUP message will be lost because WAKEUP clean message buffer (possible workaround, remember WAKEUP message) */
  }
  Can_GetgCanController(channel).WakeupPending = (vuint8)kCanFalse; /* reset old pending flags */ /* SBSW_CAN_LL08 */
# if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptWakeup);
# endif
#else
  CAN_DUMMY_STATEMENT(canChannel); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* CanInterruptWakeup */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptRx
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanInterruptRx
Relation_Context:
Relation:
OneChOpt
RuntimeMeasurement
RxFullCANSupport, RxPolling, IndividualPolling
RxBasicCANSupport, RxPolling, IndividualPolling
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptRx( vuint8 canChannel ) /* PRQA S 3673 */ /* CANoe API provide an none constant pointer but this is not used here */ /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#else
  CanChannelHandle channel; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  channel = canChannel; /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((channel < kCanNumberOfChannels),channel,kErrorWrongChannelForMsgEvent);  /* PRQA S 3201,3325 */ /* MD_Can_3201,MD_Can_3325 */ /* COV_CAN_LL_EMULATION */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 RX interrupt */
#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptRx);
#endif
#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
# if !defined( C_ENABLE_RX_FULLCAN_POLLING ) || defined( C_ENABLE_INDIVIDUAL_POLLING )
  {
    CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
    for (rxObjHandle=CAN_HL_HW_RX_FULL_STARTINDEX(channel); rxObjHandle < CAN_HL_HW_RX_FULL_STOPINDEX(channel); rxObjHandle++)
    {
      if ( Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue )
      { /* FullCan search */
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
        if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,rxObjHandle) == (uint8)0 )
#  endif
        {
          CanFullCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST rxObjHandle);
        }
      }
    }
  }
# endif
#endif
#if defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
# if !defined( C_ENABLE_RX_BASICCAN_POLLING ) || defined( C_ENABLE_INDIVIDUAL_POLLING )
  {
    Can_HwHandleType Htrh;
    CanObjectHandle rxObjHandle; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
    vuint8 bCanIdx; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
    CanObjectHandle bCanSize;
    CanObjectHandle bCanBaseHwIdx;
    CanObjectHandle bCanWrittenPointer;
    boolean isPending;
    bCanIdx=0;
    for (Htrh = Can_GetRxBasicHandleStart(channel); Htrh < Can_GetRxBasicHandleStop(channel); )
    { /* loop over all BasicCAN mailboxes */
      bCanBaseHwIdx = Can_GetHwHandle(Htrh);
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
      if ( Can_GetCanHwObjIndivPolling(CAN_HWOBJINDIVPOLLING_INDEX0,bCanBaseHwIdx) == (uint8)1 )
      {
        Htrh++; /* PRQA S 2469 */ /* Used to acelerate search - no risk for endless loop */
        bCanIdx++;
        continue; /* PRQA S 0770 */ /* Used to acelerate search */
      }
#  endif
      bCanSize = Can_GetDLC_FIFO(Htrh);
      bCanWrittenPointer = Can_GetgCanController(channel).BCanWrittenPointer[bCanIdx] + bCanBaseHwIdx;
      isPending = FALSE;
      /* search for next mailbox BasicCAN-FIFO-element to be executed  */
      rxObjHandle = bCanWrittenPointer;
      do
      {
        rxObjHandle++;
        if (rxObjHandle >= (bCanBaseHwIdx+bCanSize))
        {
          rxObjHandle = bCanBaseHwIdx;
        }
        if (Can_GetgCanController(channel).MsgObj[rxObjHandle].mIsPending == (vuint8)kCanTrue)
        {
          isPending = TRUE;
          break;
        }
      } while (rxObjHandle != bCanWrittenPointer);
      if (isPending == TRUE)
      {
        CanBasicCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST rxObjHandle);
      } else { /* no more pending element in BasicCAN-FIFO so continue with next BasicCAN */ 
        Htrh++; /* PRQA S 2469 */ /* Used to acelerate search - no risk for endless loop */
        bCanIdx++;
      }
    }
  }
# endif
#endif
#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptRx);
#endif
} /* CanInterruptRx */ /* PRQA S 6010,6030,2006 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_Can_2006 */
/* CODE CATEGORY 1 END */

/* CANoe Error Callback for Emulation  (new with CANOEAPI 1.06)*/
/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptErr
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/*
|<DataModelStart>| CanInterruptErr
Relation_Context:
Relation:
OneChOpt
StatusPolling, RuntimeMeasurement
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptErr( vuint8 canChannel ) /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#else
  CanChannelHandle channel; /* PRQA S 781 */ /* MD_Can_Asr_0781 */
  channel = canChannel; /* PRQA S 3199 */ /* MD_Can_3199_dummy */
  CAN_DUMMY_STATEMENT(channel); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((channel < kCanNumberOfChannels),channel,kErrorWrongChannelForMsgEvent);  /* PRQA S 3201,3325 */ /* MD_Can_3201,MD_Can_3325 */ /* COV_CAN_LL_EMULATION */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Error interrupt */
#if !defined( C_ENABLE_ERROR_POLLING )
# if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptErr);
# endif
  CanHL_ErrorHandling(CAN_CHANNEL_CANPARA_ONLY);
# if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptErr);
# endif
#endif
} /* PRQA S 2006 */ /* MD_Can_2006 */
/* CODE CATEGORY 1 END */

# define CAN_STOP_SEC_CODE  /*----------------------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/********* GLOBAL STOP HERE - STATIC RESTORED *************************************** */

# define CAN_START_SEC_STATIC_CODE  /*--------------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/****************************************************************************
| NAME:             CanHL_TxConfirmation
****************************************************************************/
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
 */
/*
|<DataModelStart>| CanHL_TxConfirmation
Relation_Context:
Relation:
CommonCan
DevErrorDetect, TxPolling, OneChOpt
TransmitCancellationAPI
UseVectorCanIf, IfTxBuffer, GenericConfirmation
HwCancelTx
GenericConfirmation, MirrorMode
GenericConfirmation, IDType
HwCancelTx, TransmitCancellationAPI, DevErrorDetect
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_TxConfirmation( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_CanHL_TxConfirmation();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
# if !defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING */
  /* #10 Check if parameter controller and txObjHandle is valid (only for Interrupt system, polling do this by caller) */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (((txObjHandle < CAN_HL_HW_TX_STARTINDEX(canHwChannel)) || (txObjHandle >= CAN_HL_HW_TX_STOPINDEX(canHwChannel)))) /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CanObjectHandle logTxObjHandle;
    PduIdType tmp_pdu;
#if (CAN_CANCEL_SUPPORT_API == STD_ON)
    uint8_least tmp_state;
#endif
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
    Can_ReturnType generic_retval;
#endif
    /*
    |<DataModelStart>| CanLL_TxMsgConfLocalParameter
    Relation_Context:
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
#if (defined(CAN_USE_CRITICALSECTION_OVER_CONFIRMATION) && (CAN_TRANSMIT_BUFFER == STD_ON)) || defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
    /* #100 EXCLUSIVE_AREA_3 needed to avoid another can_write while interface send out of queue (first send out of queue avoid inversion) or call GenericConfirmation2 (data consistency) */
    Can_EnterCritical(CAN_EXCLUSIVE_AREA_3);
#endif
    logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST (Can_HwHandleType)txObjHandle);
    {
      /* #120 Pre Release HW transmit object (LL) */
      /*
      |<DataModelStart>| CanLL_TxMsgConfBegin
      Relation_Context:
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      /* release transmit object */
      Can_GetgCanController(channel).MsgObj[txObjHandle].mIsFree = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
      Can_GetgCanController(channel).MsgObj[txObjHandle].mIsPending = (vuint8)kCanFalse; /* SBSW_CAN_LL01 */
      /* #130 Remember PDU for confirmation parameter */
      tmp_pdu   = Can_GetActiveSendPdu(logTxObjHandle);
#if (CAN_CANCEL_SUPPORT_API == STD_ON)
      /* #140 Remember SEND STATE which may be modified due to new transmit call in indication */
      tmp_state = (uint8_least)Can_GetActiveSendState(logTxObjHandle);
#endif
#if defined(C_ENABLE_CANCEL_IN_HW) 
      /* #150 If the message was successfully transmitted (without cancellation) (LL): */
      /*
      |<DataModelStart>| CanLL_TxIsMsgTransmitted
      Relation_Context:
      HwCancelTx
      Relation:
      ChannelAmount
      TxBasicAmount
      TxFullAmount
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      if (Can_GetgCanController(channel).MsgObj[txObjHandle].mCanCancel == (vuint8)kCanFalse) /* COV_CAN_LL_EMULATION */
#endif
      { /* #200 Set send state to free to allow next TX out of confirmation or direct (reload queue) */
        /* #210 Notify the application by call Appl_GenericConfirmation() and CanIf_TxConfirmation() */
        /*      - Appl_GenericConfirmation() if configured and mirror mode is active
                and afterwards depend of return value
                - CanIf_TxConfirmation() if generic confirmation return OK, or no generic is used, and TX is not cancelled */
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API1)
        Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
        if (Appl_GenericConfirmation( tmp_pdu ) == CAN_OK)
#elif defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
# if defined(C_ENABLE_MIRROR_MODE)
        if (Can_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
        { /* MirrorMode is inactive so call the following CanIf_TxConfirmation() */
          Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
          generic_retval = CAN_OK;
        }
        else
# endif
        { /* #230 For generic confirmation with API2 copy PDU data as parameter for Appl_GenericConfirmation() */
          Can_PduType canPdu;
          tCanTxInfoStruct canTxInfoStruct;
          
          /*
          |<DataModelStart>| CanLL_GetTxCANDataPtr
          Relation_Context:
          GenericConfirmation
          Relation:
          ChannelAmount
          TxBasicAmount
          TxFullAmount
          Parameter_PreCompile:
          Parameter_Data:
          Constrain:
          |<DataModelEnd>|
          */
          (canTxInfoStruct.pChipData) = Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld;
          (canTxInfoStruct.pChipMsgObj) = (CanChipMsgPtr) (&(Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg)); /* only for macros used in pretransmit */
          canPdu.sdu = (Can_SduPtrType)canTxInfoStruct.pChipData; /* PRQA S 0312 */ /* MD_Can_0312 */
          canPdu.length = CanTxActualDLC((&canTxInfoStruct));
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
          if ( CanTxActualIdType((&canTxInfoStruct)) == kCanIdTypeStd)
          { /* Mixed ID */
            canPdu.id = (Can_IdType)CanTxActualStdId((&canTxInfoStruct));
          }
          else
#  endif  /* pure Extended ID */
          { /* Extended ID */
            canPdu.id = (Can_IdType)(CanTxActualExtId((&canTxInfoStruct)) | CAN_ID_IDE_MASK);
          }
# else    /* Standard ID */
          canPdu.id = CanTxActualStdId((&canTxInfoStruct));
# endif
          canPdu.swPduHandle = tmp_pdu;
          Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */ /* release after copy data to avoid data inconsistency */
          /* call Appl_GenericConfirmation() and in case of CAN_OK also the following CanIf_TxConfirmation() */
          generic_retval = Appl_GenericConfirmation( (uint8)channel, &canPdu ); /* PRQA S 0312 */ /* MD_Can_0312 */ /* SBSW_CAN_HL32 */
        }
        /* #300 Appl_GenericConfirmation() decide to call Indication */
        if (generic_retval == CAN_OK)
#else /* no generic confirmation */
        Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
#endif
        {
#if (CAN_CANCEL_SUPPORT_API == STD_ON)
          /* #310 If TX is SW cancelled call CanIf_CancelTxNotification() */
          if (tmp_state == CAN_CANCEL_OBJ_SW) /* COV_CAN_CANCEL_SW_STIMULATION */
          {
            CanIf_CancelTxNotification( tmp_pdu, FALSE );
          }
          else
#endif  /* #320 otherwise call CanIf_TxConfirmation() */
          {
            CanIf_TxConfirmation( tmp_pdu ); /* \trace SPEC-1726, SPEC-1571 */
          }
        }
#if (defined(CAN_ENABLE_GENERIC_CONFIRMATION_API1) || defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)) && (CAN_TRANSMIT_BUFFER == STD_ON)
        /* #350 otherwise no CanIf_TxConfirmation() will be called so call CanIf_CancelTxNotification() just to notify IF to restart sending out of Queue */
        else
        {
# if (CAN_CANCEL_SUPPORT_API == STD_OFF)
#  error "When using Generic Confirmation and Transmit buffer (If) the Cancel-support-api (if) has to be activated"
# else
          CanIf_CancelTxNotification( tmp_pdu, FALSE );
# endif
        }
#endif
      }
#if defined(C_ENABLE_CANCEL_TX_IN_HW)
      /* #400 otherwise mailbox is cancelled successful */
      else
      {
#  if (CAN_CANCEL_SUPPORT_API == STD_ON)
        if (tmp_state == CAN_CANCEL_OBJ_SW) /* COV_CAN_CANCEL_SW_STIMULATION */
        { /* #410 free active PDU to allow next TX out of confirmation or direct (reload queue) */
          Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ); /* SBSW_CAN_HL04 */
          /* #420 SW cancellation: just call CanIf_CancelTxNotification() (no interrupt lock used here! - worst case can_write came in between and a unnecessary cancellation appear) */
          CanIf_CancelTxNotification( tmp_pdu, TRUE );
        }
        else
#  endif
        {
#  if defined(C_ENABLE_CANCEL_IN_HW) /* COV_CAN_CANCEL_IN_HW TX */
          /* always CancelSW when only FullCAN TX is cancelled so never called */
#   if (CAN_DEV_ERROR_DETECT == STD_ON)
          /* #440 Check send state is not free now, to secure that ActiveSendPduInfo is already set before used */
          if (Can_GetActiveSendState(logTxObjHandle) == CAN_FREE_OBJ) /* COV_CAN_HARDWARE_FAILURE */
          {
            errorId = CAN_E_PARAM_HANDLE;
          }
          else
#   endif
          { /* #450 EXCLUSIVE_AREA_3 -> no call to upper layer here */
            Can_EnterCritical(CAN_EXCLUSIVE_AREA_3);
            /* #460 free active PDU to allow next TX out of confirmation or direct (reload queue) */
            Can_SetActiveSendState(logTxObjHandle, CAN_FREE_OBJ);  /* SBSW_CAN_HL04 */          
            /* #470 call CanIf_CancelTxConfirmation() to re trigger queue \trace SPEC-1726, SPEC-1725, SPEC-1684, SPEC-1689, SPEC-3932, SPEC-3908 */
#   if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
            CanIf_CancelTxConfirmation( Can_GetActiveSendSwPduHandle(logTxObjHandle), &Can_GetActiveSendPduInfo(logTxObjHandle) ); /* SBSW_CAN_HL25 */
#   else
            CanIf_CancelTxConfirmation( &Can_GetActiveSendPduInfo(logTxObjHandle) ); /* SBSW_CAN_HL25 */
#   endif
            Can_LeaveCritical(CAN_EXCLUSIVE_AREA_3);
          }
#  endif
        }
      }
#endif
      /* #500 Post release HW transmit object (LL) */
      /*
      |<DataModelStart>| CanLL_TxMsgConfEnd
      Relation_Context:
      Relation:
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      /* for osCAN-Library CANoe, here is nothing to do */
    }

#if (defined(CAN_USE_CRITICALSECTION_OVER_CONFIRMATION) && (CAN_TRANSMIT_BUFFER == STD_ON)) || defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
    Can_LeaveCritical(CAN_EXCLUSIVE_AREA_3);
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
# if !defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING */
  if (errorId != CAN_E_NO_ERROR) /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_TXCNF_ID, errorId);
  }
# endif
#endif
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CanHookEnd_CanHL_TxConfirmation();
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/****************************************************************************
| NAME:             CanBasicCanMsgReceived
****************************************************************************/
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
/*
|<DataModelStart>| CanBasicCanMsgReceived
Relation_Context:
RxBasicCANSupport
Relation:
CommonCan
OneChOpt
DevErrorDetect, RxPolling, OneChOpt
RamCheck
Overrun
IDType
CanFdSupport
DevErrorDetect, TxPolling, IndividualPolling
ChannelAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCanRxBasicInfoStruct rxBasicInfo;
  CanHookBegin_CanBasicCanMsgReceived();
  errorId = CAN_E_NO_ERROR;
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  rxBasicInfo.Controller = Controller;
# endif
  rxBasicInfo.rxObjHandle = rxObjHandle;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON) && !defined(C_ENABLE_RX_BASICCAN_POLLING)
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit (already checked in polling task - do it only for interrupt) */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif 
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #25 receive handling begin CanHL_RxBasicMsgReceivedBegin() \trace SPEC-1682, SPEC-1694 */
    if (CanHL_RxBasicMsgReceivedBegin(&rxBasicInfo) == CAN_OK) /* SBSW_CAN_HL26 */ /* COV_CAN_RxBasicMsgReceivedBegin */
    {
# if defined(C_ENABLE_CAN_RAM_CHECK)
      /* -------------------------- RAM check failed --- */
      if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* COV_CAN_HARDWARE_FAILURE */
# endif
      { /* #30 RAM CHECK pass so continue reception */
# if defined(C_ENABLE_OVERRUN)
        /* -------------------------- Overrun --- */
        /*
        |<DataModelStart>| CanLL_if_RxBasicCanOverrun
        Relation_Context:
        RxBasicCANSupport, Overrun
        Relation:
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        if ( CAN_HL_INFO_RXBASIC(overrunOccur) == (vuint8)kCanTrue )
        {
          /* #40 Overrun occur so notify upper layer \trace SPEC-1686 */
#   if (CAN_OVERRUN_NOTIFICATION == CAN_DET)
          Can_CallDetReportError(CAN_RXINDI_ID, CAN_E_DATALOST); /* Call DET direct because Overrun will be notified additional to following DETs */
#   else /* CAN_OVERRUN_NOTIFICATION == CAN_APPL */
          Appl_CanOverrun( (uint8)channel );
#   endif
        }
# endif
        /* \trace SPEC-1346 */
        {
          /* -------------------------- Get DLC and Data Pointer --- */
          /* #60 get ID, DLC and data from HW \trace SPEC-1691, SPEC-1693 */
          rxBasicInfo.rxInfo.localDlc = CAN_DECODE_DLI(CanRxActualDLC((&rxBasicInfo.rxInfo)));
          /* -------------------------- Get ID  &  reject unwanted ID type --- */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
          if ( CanRxActualIdType((&rxBasicInfo.rxInfo)) == kCanIdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
          { /* mixed - standard ID */
            rxBasicInfo.rxInfo.localId = (Can_IdType)CanRxActualStdId((&rxBasicInfo.rxInfo));
          }
          else
#  endif 
          { /* mixed or pure - extended ID */
            rxBasicInfo.rxInfo.localId = (Can_IdType)(CanRxActualExtId((&rxBasicInfo.rxInfo)) | CAN_ID_IDE_MASK);
          }
          {
# else 
          { /* pure - standard ID */
            rxBasicInfo.rxInfo.localId = CanRxActualStdId((&rxBasicInfo.rxInfo));
# endif
# if defined(C_ENABLE_CAN_FD_USED)
            if ( CanRxActualFdType((&rxBasicInfo.rxInfo)) == kCanFdTypeFd )
            { /* \trace SPEC-60428 */
              rxBasicInfo.rxInfo.localId |= (Can_IdType)CAN_ID_FD_MASK;
            }
            /* DLC is not bigger than expected length (8) for CLASSIC CAN msg (additional to HashTag 145) */
            if ((CanRxActualFdType((&rxBasicInfo.rxInfo)) != kCanFdTypeFd) && (rxBasicInfo.rxInfo.localDlc > 8)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_E_PARAM_DLC;
            }
            else
# endif
            /* -------------------------- Search HRH  --- */
            /* #130 search HRH for the given rxObjHandle CanHL_RxBasicMsgReceivedSearch() */
            if (CanHL_RxBasicMsgReceivedSearch(&rxBasicInfo) == CAN_OK) /* PRQA S 2004 */ /* MD_Can_Asr_2004 */ /* SBSW_CAN_HL26 */ /* COV_CAN_GENDATA_FAILURE */
            {
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if (!defined(C_ENABLE_RX_BASICCAN_POLLING) || defined(C_ENABLE_INDIVIDUAL_POLLING))
              /* #140 Check HRH is BasicCAN (already checked in polling task - do it only for interrupt) */
              if ( (rxBasicInfo.rxInfo.localHrh < Can_GetRxBasicHandleStart(canHwChannel)) ||
                   (rxBasicInfo.rxInfo.localHrh >= Can_GetRxBasicHandleStop(canHwChannel)) ) /* CM_CAN_HL18 */ /* COV_CAN_GENDATA_FAILURE */
              {
                errorId = CAN_E_PARAM_HANDLE;
              } 
              else
#  endif
# endif
              /* #145 DLC is not bigger than expected length from mailbox (MAX over all BasicCAN msg) */
              if (rxBasicInfo.rxInfo.localDlc > CAN_MAX_DATALEN_OBJ(rxBasicInfo.rxInfo.localHrh)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
              {
                errorId = CAN_E_PARAM_DLC;
              }
              else
              {
                /* -------------------------- Notification --- */
                /* #150 RX queue and notification handling CanHL_RxMsgReceivedNotification() */
                errorId = CanHL_RxMsgReceivedNotification(CAN_CHANNEL_CANPARA_FIRST &rxBasicInfo.rxInfo); /* SBSW_CAN_HL26 */
              }
            }
          }
        }
      }
    }
    /* #160 receive handling end CanHL_RxBasicMsgReceivedEnd() */
    CanHL_RxBasicMsgReceivedEnd(&rxBasicInfo); /* SBSW_CAN_HL26 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 3356,3359 */ /* MD_Can_13.7 */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */ /* PRQA S 3201 */ /* MD_Can_3201 */
    Can_CallDetReportError(CAN_RXINDI_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CanHookEnd_CanBasicCanMsgReceived();
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanFullCanMsgReceived
****************************************************************************/
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
/*
|<DataModelStart>| CanFullCanMsgReceived
Relation_Context:
RxFullCANSupport
Relation:
CommonCan
OneChOpt
DevErrorDetect, RxPolling, OneChOpt
RamCheck
Overrun
IDType
CanFdSupport
DevErrorDetect, TxPolling, IndividualPolling
ChannelAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanFullCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCanRxFullInfoStruct rxFullInfo;
  CanHookBegin_CanFullCanMsgReceived();
  errorId = CAN_E_NO_ERROR;
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  rxFullInfo.Controller = channel;
# endif
  rxFullInfo.rxObjHandle = rxObjHandle;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if !defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
#   if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit (already checked in polling task - do it only for interrupt) */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#   endif
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #25 receive handling begin CanHL_RxFullMsgReceivedBegin() \trace SPEC-1682, SPEC-1694 */
    if (CanHL_RxFullMsgReceivedBegin(&rxFullInfo) == CAN_OK) /* SBSW_CAN_HL27 */ /* COV_CAN_RxFullMsgReceivedBegin */
    {
  /* -------------------------- RAM check failed --- */
# if defined(C_ENABLE_CAN_RAM_CHECK)
      if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* COV_CAN_HARDWARE_FAILURE */
# endif
      { /* #30 RAM CHECK pass so continue reception */
        /* -------------------------- Overrun --- */
# if defined(C_ENABLE_FULLCAN_OVERRUN)
        /*
        |<DataModelStart>| CanLL_if_RxFullCanOverrun
        Relation_Context:
        RxFullCANSupport, Overrun
        Relation:
        Parameter_PreCompile:
        Parameter_Data:
        Constrain:
        |<DataModelEnd>|
        */
        if ( CAN_HL_INFO_RXFULL(overrunOccur) == (vuint8)kCanTrue )
        {
          /* #40 Overrun occur so notify upper layer */
#   if (CAN_OVERRUN_NOTIFICATION == CAN_DET)
          Can_CallDetReportError(CAN_RXINDI_ID, CAN_E_DATALOST); /* \trace SPEC-1686 */ /* Call DET direct because Overrun will be notified additional to following DETs */
#   else /* CAN_OVERRUN_NOTIFICATION == CAN_APPL */
          Appl_CanFullCanOverrun( (uint8)channel );
#   endif
        }
# endif
        {
          /* -------------------------- Get DLC and Data Pointer --- */
          /* #60 get ID, DLC and data from HW \trace SPEC-1691, SPEC-1693 */
          rxFullInfo.rxInfo.localDlc = CAN_DECODE_DLI(CanRxActualDLC((&rxFullInfo.rxInfo)));
          /* -------------------------- Get ID  &  reject unwanted ID type --- */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
          if ( CanRxActualIdType((&rxFullInfo.rxInfo)) == kCanIdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
          { /* mixed - standard ID*/
            rxFullInfo.rxInfo.localId = (Can_IdType)CanRxActualStdId((&rxFullInfo.rxInfo));
          }
          else
#  endif 
          { /* mixed or pure - extended ID */
            rxFullInfo.rxInfo.localId = (Can_IdType)(CanRxActualExtId((&rxFullInfo.rxInfo)) | CAN_ID_IDE_MASK);
          }
          {
# else 
          { /* pure - standard ID */
            rxFullInfo.rxInfo.localId = CanRxActualStdId((&rxFullInfo.rxInfo));
# endif
# if defined(C_ENABLE_CAN_FD_USED)
            if ( CanRxActualFdType((&rxFullInfo.rxInfo)) == kCanFdTypeFd )
            { /* \trace SPEC-60428 */
              rxFullInfo.rxInfo.localId |= (Can_IdType)CAN_ID_FD_MASK;
            }
# endif
            /* -------------------------- Search HRH --- */
            /* #130 search HRH for the given rxObjHandle */
            if (CanHL_RxFullMsgReceivedSearch(&rxFullInfo) == CAN_OK) /* SBSW_CAN_HL27 */ /* COV_CAN_GENDATA_FAILURE */
            {
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if (!defined(C_ENABLE_RX_FULLCAN_POLLING) || defined(C_ENABLE_INDIVIDUAL_POLLING)) /* COV_CAN_RX_FULLCAN_POLLING */
              /* #140 Check HRH is FullCAN (already checked in polling task - do it only for interrupt) */
              if ( (rxFullInfo.rxInfo.localHrh < Can_GetRxFullHandleStart(canHwChannel)) ||
                   (rxFullInfo.rxInfo.localHrh >= Can_GetRxFullHandleStop(canHwChannel)) ) /* CM_CAN_HL18 */ /* COV_CAN_GENDATA_FAILURE */
              {
                errorId = CAN_E_PARAM_HANDLE;
              }
              else
#  endif
# endif
              /* #145 DLC is not bigger than expected length from mailbox */
              if (rxFullInfo.rxInfo.localDlc > CAN_MAX_DATALEN_OBJ(rxFullInfo.rxInfo.localHrh)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
              {
                errorId = CAN_E_PARAM_DLC;
              }
              else
              {
                /* -------------------------- Notification --- */
                /* #150 RX queue and notification handling CanHL_RxMsgReceivedNotification() */
                errorId = CanHL_RxMsgReceivedNotification(CAN_CHANNEL_CANPARA_FIRST &rxFullInfo.rxInfo); /* SBSW_CAN_HL27 */
              }
            }
          }
        }
      }
    }
    /* #160 receive handling end CanHL_RxFullMsgReceivedEnd() */
    CanHL_RxFullMsgReceivedEnd(&rxFullInfo); /* SBSW_CAN_HL27 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 3356,3359 */ /* MD_Can_13.7 */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */ /* PRQA S 3201 */ /* MD_Can_3201 */
    Can_CallDetReportError(CAN_RXINDI_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CanHookEnd_CanFullCanMsgReceived();
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

/****************************************************************************
| NAME:             CanHL_ErrorHandling
****************************************************************************/
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
/*
|<DataModelStart>| CanHL_ErrorHandling
Relation_Context:
Relation:
CommonCan
DevErrorDetect, StatusPolling, OneChOpt
BusOffRecovery, OneChOpt
Overrun
DevErrorDetect
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
# if !defined(C_ENABLE_ERROR_POLLING) /* COV_CAN_ERROR_POLLING */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /*
    |<DataModelStart>| CanLL_ErrorHandlingLocalParameter
    Relation_Context:
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    /* #20 Pre Error handling (LL) */
    /*
    |<DataModelStart>| CanLL_ErrorHandlingBegin
    Relation_Context:
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    CanCell_AtomicBegin();
    /*
    |<DataModelStart>| CanLL_if_BusOffOccured
    Relation_Context:
    Relation:
    ChannelAmount
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    if ( Can_GetgCanController(channel).ErrCode == (vuint32)CANCELL_BUSOFF )
    {
      /* #90 EXCLUSIVE_AREA_6 secure mode changes */
      Can_EnterCritical(CAN_EXCLUSIVE_AREA_6);
      /* #100 BUSOFF occur -> ECU Manager restart Controller (no ResetBusOffStart/End needed) */
      Can_SetIsBusOff(channel,TRUE); /* SBSW_CAN_HL08 */ /* attention: platform use this for BUSOFF 128*11 treatment!!! */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_BUSOFF); /* SBSW_CAN_HL02 */
#if !defined(CAN_ENABLE_BUSOFF_SUPPORT_API) /* COV_CAN_BUSOFF_SUPPORT_API */
      /* #110 CanResetBusOffStart need when BUSOFF handled by Application */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
      CanResetBusOffStart(channel, Can_GetLastInitObject(channel));
# else
      CanResetBusOffStart(Can_GetLastInitObject(channel));
# endif
#endif
      /* #115 Controller is in STOP mode after CanResetBusOffStart \trace SPEC-1578, SPEC-1664, SPEC-1663, SPEC-3828 */
      /* #120 Status changed to STOP - no more call of STOP from CAN Interface */
      Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP)); /* SBSW_CAN_HL02 */
      Can_LeaveCritical(CAN_EXCLUSIVE_AREA_6);
      /* #130 notify higher layer by call CanIf_ControllerBusOff() \trace SPEC-1726, SPEC-1578 */
      CanIf_ControllerBusOff((uint8)Can_GetCanToCanIfChannelMapping(channel));
    }
    /* #200 Post Error handling (LL) */
    /*
    |<DataModelStart>| CanLL_ErrorHandlingEnd
    Relation_Context:
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    CanCell_AtomicEnd();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* COV_CAN_GENDATA_FAILURE */ /* PRQA S 3356,3359 */ /* MD_Can_13.7,MD_Can_13.7 */
  { /* Throw DET if an error detected before */ /* PRQA S 3201 */ /* MD_Can_3201 */
    Can_CallDetReportError(CAN_CTRBUSOFF_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 6010,6030,6050 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL */

# if defined(C_ENABLE_SLEEP_WAKEUP)

/****************************************************************************
| NAME:             CanHL_WakeUpHandling
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_WakeUpHandling
Relation_Context:
Wakeup
Relation:
DevErrorDetect, WakeupPolling, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WakeUpHandling( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
#    if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_HARDWARE_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#    endif
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 WAKEUP already done in "CanLL_WakeUpHandling" via macro "CanWakeUp" -> (void)Can_SetControllerMode(controller, CAN_T_WAKEUP); */
    /* #30 ESCAN00032401: don't change to STOP mode here this will be done later from inside CANIf after Transceiver is switched to normal mode to avoid blocking while Transceiver produce dominant disturbance */
    Can_SetIsWakeup(channel,TRUE); /* SBSW_CAN_HL09 */
    /* #40 call EcuM_CheckWakeup()  - old API: CanIf_SetWakeupEvent();  \trace SPEC-1725, SPEC-1659, SPEC-1697, SPEC-3966, SPEC-3971 */
    EcuM_CheckWakeup(Can_GetWakeupSourceRef(channel));
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* COV_CAN_HARDWARE_FAILURE */ /* PRQA S 3356,3359 */ /* MD_Can_13.7,MD_Can_13.7 */
  { /* Throw DET if an error detected before */ /* PRQA S 3201 */ /* MD_Can_3201 */
    Can_CallDetReportError(CAN_CTRWAKEUP_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
}
# endif

#define CAN_STOP_SEC_STATIC_CODE  /*---------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*-------------------------------------------------------------------------*/

#define CAN_START_SEC_CODE  /*---------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CAN_VERSION_INFO_API == STD_ON) /* \trace SPEC-1716, SPEC-3964 */
/****************************************************************************
| NAME:             Can_GetVersionInfo
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| Can_GetVersionInfo
Relation_Context:
VersionInfoApi
Relation:
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_GetVersionInfo( Can_VersionInfoPtrType VersionInfo )
{ /* \trace SPEC-1723, SPEC-3881 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_GetVersionInfo();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter versionInfo is an illegal null pointer \trace SPEC-1721, SPEC-3847 */
  if (VersionInfo == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
  { /* #100 copy version info to given pointer parameter \trace SPEC-1717, SPEC-3845 */
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID   = CAN_VENDOR_ID; /* SBSW_CAN_HL10 */
    VersionInfo->moduleID   = CAN_MODULE_ID; /* SBSW_CAN_HL10 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
    VersionInfo->instanceID = CAN_INSTANCE_ID; /* SBSW_CAN_HL10 */
# endif
    VersionInfo->sw_major_version = (uint8)CAN_SW_MAJOR_VERSION; /* SBSW_CAN_HL10 */
    VersionInfo->sw_minor_version = (uint8)CAN_SW_MINOR_VERSION; /* SBSW_CAN_HL10 */
    VersionInfo->sw_patch_version = (uint8)CAN_SW_PATCH_VERSION; /* SBSW_CAN_HL10 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_VERSION_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CanHookEnd_Can_GetVersionInfo();
}
#endif

/****************************************************************************
| NAME:             Can_InitMemory
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_InitMemory
Relation_Context:
Relation:
Variant
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_InitMemory( void )
{ /* BR:011 */
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  CanHookBegin_Can_InitMemory();
  /* #10 mark driver as uninitialized \trace SPEC-1650, SPEC-3787 */
  canConfigInitFlag = CAN_STATUS_UNINIT;
#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER)
  /* #20 reset global configuration pointer */
  Can_ConfigDataPtr = NULL_PTR;
#endif /* otherwise constant pointer is used */
  /* #30 HW reset memory */
  /*
  |<DataModelStart>| CanLL_InitMemoryPowerOn
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
#if(CAN_DEV_ERROR_DETECT == STD_OFF)
  CAN_DUMMY_STATEMENT(canConfigInitFlag); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_InitMemory();
}

#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
/****************************************************************************
| NAME:             Can_InitStruct
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/*
|<DataModelStart>| Can_InitStruct
Relation_Context:
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_InitStruct( uint8 Controller, uint8 Index )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_InitStruct();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON) && !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller and index are valid */
  if ( (channel >= kCanNumberOfChannels) || (Index >= (uint8)(Can_GetInitObjectStartIndex(channel+1) - Can_GetInitObjectStartIndex(channel))) ) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
  { /* #20 set initialization structure that should be used for next controller initialization. */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetLastInitObject(channel, (uint8)(Can_GetInitObjectStartIndex(channel) + Index)); /* SBSW_CAN_HL11 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON) && !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_INIT_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CanHookEnd_Can_InitStruct();
}
#endif

#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if ((CAN_CHANGE_BAUDRATE_API == STD_ON) || (CAN_SET_BAUDRATE_API == STD_OFF))
/****************************************************************************
| NAME:             Can_ChangeBaudrate
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_ChangeBaudrate
Relation_Context:
ChangeBaudrate
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_ChangeBaudrate( uint8 Controller, V_DEF_CONST(V_NONE, uint16, APPL_CONST) Baudrate )
{ /* \trace SPEC-20314 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId;
  CanHookBegin_Can_ChangeBaudrate();
  retVal = E_NOT_OK;
  errorId = CAN_E_PARAM_BAUDRATE; /* explicit default as Issue - remove when baud rate fit */ /* \trace SPEC-20321 */
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT and STOP \trace SPEC-20338 */
  if ( canConfigInitFlag == CAN_STATUS_UNINIT )
  {
    errorId = CAN_E_UNINIT;
  }
  else
#   if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCanNumberOfChannels ) /* CM_CAN_HL01 */
  { /* \trace SPEC-20331 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#   endif
  if ( (Can_GetLogStatus(channel) & CAN_STATUS_STOP) != CAN_STATUS_STOP )
  { /* \trace SPEC-1655, SPEC-20312 */
    errorId = CAN_E_TRANSITION;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex;
    for (baudrateIndex = Can_GetInitObjectStartIndex(channel); baudrateIndex < Can_GetInitObjectStartIndex(channel+1); baudrateIndex++)
    { /* #30 search for baud rate configuration */
      if (Can_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #40 set baud rate and reinitialize controller to activate baud rate \trace SPEC-1669 */
        Can_SetLastInitObject(channel, (uint8)baudrateIndex); /* SBSW_CAN_HL11 */
        Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
        CanHookEnd_Can_ChangeBaudrate();
        retVal = E_OK;
        errorId = CAN_E_NO_ERROR;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CHANGE_BR_ID, errorId);
  }
#  else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  CanHookEnd_Can_ChangeBaudrate();
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (CAN_CHANGE_BAUDRATE_API == STD_ON)
/****************************************************************************
| NAME:             Can_CheckBaudrate
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_CheckBaudrate
Relation_Context:
ChangeBaudrate
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_CheckBaudrate( uint8 Controller, V_DEF_CONST(V_NONE, uint16, APPL_CONST) Baudrate )
{ /* \trace SPEC-20311 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId;
  CanHookBegin_Can_CheckBaudrate();
  retVal = E_NOT_OK;
  errorId = CAN_E_PARAM_BAUDRATE; /* \trace SPEC-20317 */
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT \trace SPEC-20328, SPEC-20318 */
  if ( canConfigInitFlag == CAN_STATUS_UNINIT )
  {
    errorId = CAN_E_UNINIT;
  }
  else
#   if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCanNumberOfChannels )
  { /* \trace SPEC-20335 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#   endif
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex;
    for (baudrateIndex = Can_GetInitObjectStartIndex(channel); baudrateIndex < Can_GetInitObjectStartIndex(channel+1); baudrateIndex++)
    { /* #30 search for baud rate configuration */
      if (Can_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #40 requested baud rate is set - return OK */
        CanHookEnd_Can_CheckBaudrate();
        retVal = E_OK;
        errorId = CAN_E_NO_ERROR;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CHECK_BR_ID, errorId);
  }
#  else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  CanHookEnd_Can_CheckBaudrate();
  return retVal;
}
# endif /* CAN_CHANGE_BAUDRATE_API == STD_ON */

# if (CAN_SET_BAUDRATE_API == STD_ON)
/****************************************************************************
| NAME:             Can_SetBaudrate
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_SetBaudrate
Relation_Context:
ChangeBaudrate
Relation:
DevErrorDetect, OneChOpt
HardwareLoop
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID )
{ /* \trace SPEC-50605 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  CanHookBegin_Can_SetBaudrate();
  /* #5 return E_NOT_OK in case no fitting baudrate is found \trace-2622524 */
  retval = E_NOT_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT and STOP \trace SPEC-50595 */
  if ( canConfigInitFlag == CAN_STATUS_UNINIT )
  {
    errorId = CAN_E_UNINIT;
  }
  else
#   if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCanNumberOfChannels ) /* CM_CAN_HL01 */
  { /* \trace SPEC-50587 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#   endif
  if ( (Can_GetLogStatus(channel) & CAN_STATUS_STOP) != CAN_STATUS_STOP )
  { /* \trace SPEC-50584 */
    errorId = CAN_E_TRANSITION;
  }
  else
  if ( BaudRateConfigID >= (uint16)(Can_GetInitObjectStartIndex(channel+1) - Can_GetInitObjectStartIndex(channel)) )
  { /* \trace SPEC-50625, SPEC-50563 */
    errorId = CAN_E_PARAM_BAUDRATE;
  }
  else
#  endif
  { /* #50 set baud rate and reinitialize controller to activate baud rate */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetLastInitObject(channel, (uint8)(Can_GetInitObjectStartIndex(channel) + BaudRateConfigID)); /* SBSW_CAN_HL11 */
    Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */ /* \trace SPEC-50576, SPEC-50563, SPEC-50617 */
    {
      retval = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_SET_BR_ID, errorId);
  }
#  else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
  CanHookEnd_Can_SetBaudrate();
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* CAN_SET_BAUDRATE_API == STD_ON */
#endif /* only MSR403 */

/****************************************************************************
| NAME:             Can_Init
****************************************************************************/
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
/*
|<DataModelStart>| Can_Init
Relation_Context:
Relation:
Variant, DevErrorDetect
PrecompileAndLinktimeCRCCheck
DevErrorDetect, Variant, CommonCan
CanDevTimeoutDetect, HardwareLoop, HardwareCancelByAppl, DevErrorDetect
RxQueue
Variant
OneChOpt
RamCheck
Wakeup
PnEnable
MirrorMode
SilentMode
ChangeBaudrate
UseVectorCanIf
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_Init( Can_ConfigPtrType ConfigPtr )
{ /* \trace SPEC-1587, SPEC-3850, SPEC-3794, SPEC-1708, SPEC-3826, SPEC-3861 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CAN_CHANNEL_CANTYPE_LOCAL
  CanHookBegin_Can_Init();
  errorId = CAN_E_NO_ERROR;
#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER) /* PostBuild (load+sel) */
  Can_ConfigDataPtr = ConfigPtr; /* \trace SPEC-1394, SPEC-1575 */
# if defined(CAN_ENABLE_USE_ECUM_BSW_ERROR_HOOK) /* PostBuildLoadable */
  if (ConfigPtr == NULL_PTR)
  { /* #10 EcuM null pointer configuration check and notification */
    EcuM_BswErrorHook((uint16) CAN_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
  }
  else
#  if (CAN_FINALMAGICNUMBER == STD_ON) /* COV_CAN_FINALMAGICNUMBER */
  if (Can_GetFinalMagicNumber() != 20510u) /* COV_CAN_GENDATA_FAILURE */
  { /* #20 EcuM wrong magic number check and notification */
    EcuM_BswErrorHook((uint16) CAN_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  else
#  endif
# else
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #30 Check if all parameters are valid \trace SPEC-1724, SPEC-3789 */
  if (ConfigPtr == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
  /* #40 Check wrong magic number */
#   if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
  /* not CAN_ENABLE_USE_ECUM_BSW_ERROR_HOOK --> not PostBuildLoadable so CAN_FINALMAGICNUMBER is always false */
#   else
  if (Can_GetFinalMagicNumber() != 0xaffeu) /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
#   endif
#  endif
# endif
#else
  CAN_DUMMY_STATEMENT(ConfigPtr); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199, 3198 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
  ConfigPtr = Can_ConfigDataPtr; /* dummy assignment avoid compiler warning */ /* PRQA S 3199 */ /* MD_Can_3199_dummy */
# endif
#endif /* CAN_ENABLE_USE_INIT_ROOT_POINTER */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag != CAN_STATUS_UNINIT)
  {
    /* #60 Check already initialized -> 2nd call is not allowed \trace SPEC-1712, SPEC-3934, SPEC-3767, SPEC-1722 */
    errorId = CAN_E_TRANSITION;
  }
  else
#endif
  if (  ( Can_GetBaseDll_GeneratorVersion()     != (uint16)CAN_GEN_BASE_VERSION ) ||  /* PRQA S 3325 */ /* MD_Can_3325 */
        ( Can_GetPlatformDll_GeneratorVersion() != (uint16)CAN_GEN_CanoeemuCanoeAsr_VERSION ) ) /* COV_CAN_GENDATA_FAILURE */
  { /* PRQA S 3201 */ /* MD_Can_3201 */
    /* #70 EcuM generator and compatibility version check and notification */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_USE_ECUM_BSW_ERROR_HOOK) /* not for PostBuildSelectable */
    EcuM_BswErrorHook((uint16) CAN_MODULE_ID, (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION);
# else
    errorId = CAN_E_UNINIT;
# endif
#else
    EcuM_GeneratorCompatibilityError((uint16) CAN_MODULE_ID, (uint8) 0);
#endif
  }
  else
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if (CAN_CRC_CHECK == STD_ON)
  if (( Can_GetPrecompileCRC() != CAN_PRECOMPILE_CRC ) || ( Can_GetLinktimeCRC() != Can_LinktimeCRC )) /* COV_CAN_GENDATA_FAILURE */
  { /* #80 EcuM generator pre compile and link time CRC check and notification */
    EcuM_GeneratorCompatibilityError((uint16) CAN_MODULE_ID, (uint8) 0u);
  }
  else
# endif
#endif
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if CAN_DEV_ERROR_DETECT == STD_ON
#  if defined(CAN_ENABLE_SELECTABLE_PB) 
  if (
       (kCanNumberOfChannels > kCanNumberOfMaxUsedControllers) ||
       (kCanNumberOfUsedHwChannels > kCanNumberOfMaxUsedControllers) )
  { /* #90 check consistency of generated Data (real channel amount against maximum value) */
    errorId = CAN_E_PARAM_POINTER;
  }
  else
#  endif
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    {
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
      Can_SignedHwHandleType tmp_objcnt;
      tmp_objcnt=0;
#endif
      canRxTaskActive = CAN_FREE;
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
# endif
#endif
#if defined(C_ENABLE_RX_QUEUE)
      /* #110 delete RxQueue (all channels) */
      Can_GetRxQueueInfo().WriteIndex = 0;  /* CM_CAN_HL24 */
      Can_GetRxQueueInfo().ReadIndex = 0;
      Can_GetRxQueueInfo().Count = 0;
      canRxQueueMaxDataSize = (uint16) (sizeof(Can_GetRxQueueBuffer(0).data) / sizeof(Can_GetRxQueueBuffer(0).data[0]));
#endif
      /* #120 Set active Identity */
#if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
      CanSetActiveIdentity(Can_GetIdentitySelected());
#endif
      /* #130 HW specific power on (LL) */
      CanHL_InitPowerOn();

      canConfigInitFlag = CAN_STATUS_INIT; /* \trace SPEC-1648, SPEC-3962, SPEC-3943 */ /* before Controller initialization begin */
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
      for (channel = 0; channel < kCanNumberOfChannels; channel++) /* CM_CAN_HL01 */
#endif
      { /* #132 for each controller */
#if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
        if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
        { /* #134 not for inactive controller */
          continue; /* PRQA S 0770 */ /* MD_Can_0770 */
        }
#endif
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
# if defined(C_ENABLE_GEN_HW_START_STOP_IDX) /* generated as tool-table */
        /* #136 calculate logical TX handle (HW to log - done here because of multiple ECU) */
        CAN_HL_TX_OFFSET_HW_TO_LOG(channel) = (Can_SignedHwHandleType)(tmp_objcnt - (Can_SignedHwHandleType)Can_GetTxFullHwStart(CAN_HL_HW_CHANNEL_STARTINDEX(channel)));
# else
        CAN_HL_TX_OFFSET_HW_TO_LOG(channel) = (Can_SignedHwHandleType)(tmp_objcnt - (Can_SignedHwHandleType)Can_GetHwHandle(Can_GetTxFullHandleStart(CAN_HL_HW_CHANNEL_STARTINDEX(channel))));
# endif
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
        tmp_objcnt = (Can_SignedHwHandleType)(tmp_objcnt + (Can_SignedHwHandleType)Can_GetTxFullHandleMax(CAN_HL_HW_CHANNEL_STARTINDEX(channel)) + (Can_SignedHwHandleType)(Can_GetTxBasicHandleMax(CAN_HL_HW_CHANNEL_STARTINDEX(channel)) * CAN_MULTIPLEXED_TX_MAX));
# endif
#endif
        /* #140 initialize start up values for each controllers */
        Can_SetLogStatus(channel, CAN_STATUS_UNINIT); /* SBSW_CAN_HL02 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
        Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK)
        Can_SetStatusReq(channel, Can_GetStatusReq(channel) | CAN_REQ_RAM_CHECK); /* execute power on RamCheck*/ /* SBSW_CAN_HL01 */
#endif
        Can_SetIsBusOff(channel,FALSE);  /* start up: no BUSOFF */ /* SBSW_CAN_HL08 */
        Can_SetLastInitObject(channel, (uint8)(Can_GetInitObjectStartIndex(channel))); /* SBSW_CAN_HL11 */
#if defined(C_ENABLE_SLEEP_WAKEUP)
        Can_SetIsWakeup(channel,FALSE); /* SBSW_CAN_HL09 */
#endif
        Can_SetCanInterruptCounter(channel, 0); /* SBSW_CAN_HL12 */
#if defined(C_ENABLE_PARTIAL_NETWORK) /* COV_CAN_PARTIAL_NETWORK */
        Can_SetTimeoutCall(channel,TRUE); /* SBSW_CAN_HL13 */
#endif
#if defined(C_ENABLE_MIRROR_MODE)
        Can_SetMirrorModeState(channel, CDDMIRROR_INACTIVE); /* SBSW_CAN_HL20 */
#endif
#if defined(C_ENABLE_SILENT_MODE)
        Can_SetSilentModeState(channel, CAN_SILENT_INACTIVE); /* SBSW_CAN_HL20 */
#endif
        /* #155 HW channel specific power on */
        CanHL_InitPowerOnChannelSpecific(CAN_CHANNEL_CANPARA_ONLY);

        /* #160 set baud rate and initialize all CAN controllers \trace SPEC-1587, SPEC-1656, SPEC-3981 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
        Can_InitController((uint8)channel, &Can_GetControllerBaudrateConfig(0));      /* use always first initStruct */ /* SBSW_CAN_HL03 */
#elif defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
        Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP); /* set to STOP mode before set baud rate to avoid DET */ /* SBSW_CAN_HL02 */
# if (CAN_SET_BAUDRATE_API == STD_ON)
        (void)Can_SetBaudrate((uint8)channel, Can_GetCanControllerDefaultBaudrateIdx(CAN_HL_HW_CHANNEL_STARTINDEX(channel)));
# else
        (void)Can_ChangeBaudrate((uint8)channel, Can_GetCanControllerDefaultBaudrate(CAN_HL_HW_CHANNEL_STARTINDEX(channel))); /* \trace SPEC-15127 */
# endif
#else
# if !defined(CAN_USE_VECTOR_IF) 
        /* #170 call Can_InitController() - For Vector ASR3 this will be done within Can Interface */
        Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
# endif
#endif
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_INIT_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_Init();
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_InitController
****************************************************************************/
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
 */
/*
|<DataModelStart>| Can_InitController
Relation_Context:
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
RamCheck
CanDevTimeoutDetect, HardwareLoop, HardwareCancelByAppl, DevErrorDetect
CommonCan
TxFullCANSupport
RxFullCANSupport
RxBasicCANSupport
RamCheck
Wakeup
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
V_DEF_FUNC(V_NONE, void, CODE) Can_InitController( uint8 Controller, Can_ControllerConfigPtrType ControllerConfigPtr )
#elif defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
V_DEF_FUNC(V_NONE, void, CODE) Can_InitController( uint8 Controller, Can_ControllerBaudrateConfigPtrType Config )
#elif  defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
V_DEF_FUNC(V_NONE, void, CODE) Can_InitController( uint8 Controller, Can_ControllerConfigPtrType ControllerConfigPtr )
#endif
{ /* \trace SPEC-1719, SPEC-1589, SPEC-1346, SPEC-1565, SPEC-3883 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_InitController();
  errorId = CAN_E_NO_ERROR; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and Config pointer is valid and state is INIT \trace SPEC-3962 */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1718, SPEC-3797 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1713, SPEC-3821 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
  if (Config == NULL_PTR)
  { /* \trace SPEC-1711, SPEC-3774 */
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* \trace SPEC-1713 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
#endif
#if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
  /* #50 Check controller is in active ECU - otherwise nothing to do here */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) != (cantVIdentityMsk)0 )  /* PRQA S 2004 */ /* MD_Can_Asr_2004 */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    tCanInitInfoStruct initInfo;
    /* #100 initialization controller states and initialization objects --- */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
    Can_SetLastInitObject(channel, (uint8)((uint8)Can_GetInitObjectStartIndex(channel) + (uint8)*Config)); /* SBSW_CAN_HL11 */
#endif
    initInfo.initObject = Can_GetLastInitObject(channel);
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
    initInfo.Controller = channel;
#endif
    initInfo.isChCorrupt = kCanFalse;
    /* #110 clean up status (do not activate communication and let SLEEP and initialization to detect ram-check request) */
#if defined(C_ENABLE_CAN_RAM_CHECK) && !defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
    { /* #112 standard RAM check will be performed so delete old status CAN_DEACTIVATE_CONTROLLER */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & CAN_STATUS_INIT);  /* SBSW_CAN_HL02 */
    }
    else
#endif
    { /* #114 extended RAM check may be performed so delete old status except CAN_DEACTIVATE_CONTROLLER */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)(CAN_DEACTIVATE_CONTROLLER | CAN_STATUS_INIT));  /* SBSW_CAN_HL02 */
    }
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
# endif
#endif
    { /* #125 begin initialization CanHL_InitBegin() */
      {
        errorId = CanHL_InitBegin(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
      }
      {
#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
        /* #140 do TX FullCAN initialization CanHL_InitTxFullCAN()*/
        CAN_IS_NO_ERROR(errorId)
        { /* do it only when no error occur before */
          errorId = CanHL_InitTxFullCAN(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
        }
#endif
        /* #150 do TX BasicCAN initialization CanHL_InitTxBasicCAN() */
        CAN_IS_NO_ERROR(errorId)
        { /* do it only when no error occur before */
          errorId = CanHL_InitTxBasicCAN(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
        }
      } /* Common CAN:  TX only on first hwChannel */
      /* #160 do unused object initialization CanHL_InitUnusedObj()*/
      CAN_IS_NO_ERROR(errorId)
      { /* do it only when no error occur before */
        errorId = CanHL_InitUnusedObj(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
      }
#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
      /* #170 do RX FullCAN initialization CanHL_InitRxFullCAN() */
      CAN_IS_NO_ERROR(errorId)
      { /* do it only when no error occur before */
        errorId = CanHL_InitRxFullCAN(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
      }
#endif
#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
      /* #180 do RX BasicCAN initialization CanHL_InitRxBasicCAN() */
      CAN_IS_NO_ERROR(errorId)
      { /* do it only when no error occur before */
        errorId = CanHL_InitRxBasicCAN(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
      }
#endif
      /* #190 end initialization CanHL_InitEnd_InitMode() */
      CAN_IS_NO_ERROR(errorId)
      { /* do it only when no error occur before */
        errorId = CanHL_InitEnd_InitMode(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
      }
#if defined(C_ENABLE_CAN_RAM_CHECK)
      if ((Can_GetStatusReq(channel) & CAN_REQ_RAM_CHECK) == CAN_REQ_RAM_CHECK)
      { 
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
        /* #200 extended RamChecks new issue occur - notify corrupt register / deactivate controller */
        if (initInfo.isChCorrupt == kCanTrue) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        {
          CanIf_RamCheckCorruptController((uint8)Can_GetCanToCanIfChannelMapping(channel));
          Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL02 */
        }
# else
        if( (Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) == CAN_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        { /* #210 standard RAM check issue occur - call Appl_CanRamCheckFailed() */
          if (Appl_CanRamCheckFailed((uint8)channel) != CAN_DEACTIVATE_CONTROLLER)
          { /* #220 let the application decide if communication stay disabled as set by RAM check or not */
            Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)(~(CAN_DEACTIVATE_CONTROLLER))); /* Application decide to activate communication in case RAM_CHECK failed */  /* PRQA S 0277 */ /* MD_Can_0277_negation */ /* SBSW_CAN_HL02 */
          }
        }
# endif
        CAN_RAM_CHECK_FINISHED_CTP(channel)
      }
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
      /* #230 no STOP transition for defect Controllers */
      if( (Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
#endif
      {
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
        /* #250 BUSOFF, WAKEUP and Re-initialization lead direct to STOP mode, otherwise it is only a re-initialization with transition afterwards. */
        if ( (Can_GetStatusReq(channel) & CAN_REQ_INIT_NO_STOP) != CAN_REQ_INIT_NO_STOP ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
        { /* #260 no more polling or notification, BUSOFF, WAKEUP notified before otherwise Re-initialization only allowed in STOP mode */
          Can_SetStatusReq(channel, CAN_REQ_NONE); /* SBSW_CAN_HL01 */
#endif
          /* #270 Switch to STOP mode CanHL_InitEnd_StopMode() */
          CAN_IS_NO_ERROR(errorId)
          { /* do it only when no error occur before */
            errorId = CanHL_InitEnd_StopMode(&initInfo); /* SBSW_CAN_HL31 */ /* PRQA S 3198 */ /* MD_MSR_14.2 */
          }
          Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP);  /* \trace SPEC-1657, SPEC-1647 set STOP */ /* SBSW_CAN_HL02 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
        }
#endif
      }
    } /* end of loop over all HW channels */
#if (CAN_DEV_ERROR_DETECT == STD_ON) 
    if (errorId != CAN_E_NO_ERROR) /* COV_CAN_HARDWARE_FAILURE */
    { /* #275 remove controller init flag when issue occure before */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)(~(CAN_STATUS_INIT))); /* PRQA S 0277 */ /* MD_Can_0277_negation */ /* SBSW_CAN_HL02 */
    }
    else
#endif
    { /* #280 set controller init flag when no issue occure before */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) | (CAN_STATUS_INIT)); /* SBSW_CAN_HL02 */
    }
#if defined(C_ENABLE_SLEEP_WAKEUP)
    if (Can_IsIsWakeup(channel)) /* \trace SPEC-15124 */ /* COV_CAN_HARDWARE_FAILURE */
    { /* #290 notify EcuM in case controller WAKEUP while initialization (LL: check before) \trace SPEC-15124 */
      Can_SetIsWakeup(channel,FALSE); /* SBSW_CAN_HL09 */
      EcuM_SetWakeupEvent(Can_GetWakeupSourceRef(channel));
    }
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK)
    Can_SetStatusReq(channel, Can_GetStatusReq(channel) & (uint8)(~(CAN_REQ_RAM_CHECK))); /* deactivate RamCheck - finished */ /* SBSW_CAN_HL01 */ /* PRQA S 0277 */ /* MD_Can_0277_negation */ 
#endif
  } /* controller active in multiple ECU configuration */

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_INITCTR_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
  CAN_DUMMY_STATEMENT(ControllerConfigPtr); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_InitController();
} /* PRQA S 6010,6050,6030,6080 */ /* MD_Can_Asr_STPTH,MD_MSR_STCAL,MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_SetControllerMode
****************************************************************************/
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
 *
 *
 *
 */
/*
|<DataModelStart>| Can_SetControllerMode
Relation_Context:
Relation:
CommonCan
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
RamCheck
CanDevTimeoutDetect, HardwareLoop, HardwareCancelByAppl, DevErrorDetect
BusOffRecovery
ReInitStart
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Can_ReturnType, CODE) Can_SetControllerMode( uint8 Controller, Can_StateTransitionType Transition )
{ /* \trace SPEC-1715, SPEC-3894 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_ReturnType retval;
  CanHookBegin_Can_SetControllerMode();
  retval = CAN_NOT_OK;  /* \trace SPEC-1407 */ /* For successful transition it is set explicit to CAN_OK */
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and Transition is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1731, SPEC-3962, SPEC-3872 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1732 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { 
    errorId = CAN_E_PARAM_CONTROLLER; /* \trace SPEC3874 */
  }
  else
# endif
#endif
#if defined(C_ENABLE_MULTI_ECU_CONFIG) /* move below channel DET check */ /* COV_CAN_MULTI_ECU_CONFIG */
  /* #40 Check controller is in active ECU */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) != (cantVIdentityMsk)0 ) /* PRQA S 2004 */ /* MD_Can_Asr_2004 */ /* only for active Controller */
#endif
  { /* #50 second level check (modes and transitions) only valid when controller is in active ECU and valid */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
    if (!CanHL_IsControllerInit(channel))
    { /* #60 Check controller is initialized \trace SPEC-3962 */
      errorId = CAN_E_UNINIT;
    }
    else
    if ((!CanHL_IsStop(channel)) && (Transition == CAN_T_START))
    { /* #70 Check No-STOP -> START-Req is invalid \trace SPEC-1637, SPEC-3862 */ /* ESCAN00073272 */
      errorId = CAN_E_TRANSITION;
    }
    else
# if (CAN_TRANSITION_CHECK == STD_ON)  /* ASR4 extension SREQ00000291 */ /* COV_CAN_USER_CONFIG */
    if ( ((!CanHL_IsStop(channel)) && (!CanHL_IsSleep(channel)) && ((Transition == CAN_T_WAKEUP) || (Transition == CAN_T_SLEEP))) ||
         ((CanHL_IsSleep(channel)) && (Transition == CAN_T_STOP)) )
    { /* #80 Check  No STOP Nor SLEEP -> SLEEP-Req   or   No SLEEP Nor STOP --> WAKEUP-Req   is invalid \trace SPEC-1654, SPEC-1649 */
      /* #90 Check  No START Nor STOP -> STOP-Req is invalid \trace SPEC-1652 */
      errorId = CAN_E_TRANSITION; /* trace SPEC-3862 */
    }
    else
# endif
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    /* #95 controller is active (no RAM check issue): */
    if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* PRQA S 2004 */ /* MD_Can_Asr_2004 */ /* COV_CAN_RAM_CHECK_NO_STIMULATION */
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 EXCLUSIVE_AREA_6 avoid nesting mode changes (e.g. same API, initialization or BUSOFF) */
      Can_EnterCritical(CAN_EXCLUSIVE_AREA_6);
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
# endif
#endif
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
      /* #120 mark ongoing state change (suppress STOP transition in Can_InitController()) */ 
      Can_SetStatusReq(channel, Can_GetStatusReq(channel) | CAN_REQ_INIT_NO_STOP); /* SBSW_CAN_HL01 */
#endif
      /* #125 HW begin mode transition */
      /*
      |<DataModelStart>| CanLL_SetControllerModeBegin
      Relation_Context:
      Relation:
      Parameter_PreCompile:
      Parameter_Data:
      Constrain:
      |<DataModelEnd>|
      */
      switch(Transition)
      {
      case CAN_T_START: /* \trace SPEC-3963 */
        /* #200 Handle --- START --- transition */
#if !defined(CAN_ENABLE_BUSOFF_SUPPORT_API) /* COV_CAN_BUSOFF_SUPPORT_API */
        if (Can_IsIsBusOff(channel)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
        { /* #210 BUSOFF occur so finish BUSOFF handling with CanResetBusOffEnd() */
          /* #215 reset BUSOFF flag - do this before CanResetBusOffEnd() so LL may overwrite it */
          Can_SetIsBusOff(channel,FALSE); /* SBSW_CAN_HL08 */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
          CanResetBusOffEnd(channel, Can_GetLastInitObject(channel));
# else
          CanResetBusOffEnd(Can_GetLastInitObject(channel));
# endif
          /* #220 do not re-initialization after BUSOFF, this will be done while CanResetBusOffStart/End() */
        }
        else
#endif
        {
#if (CAN_REINIT_START == STD_ON) 
          /* #230 additional to STOP-transition-re initialization a Can_InitController() may be activated for START transition \trace SPEC-1730 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
          Can_InitController((uint8)channel, &Can_GetControllerBaudrateConfig(((uint8)(Can_GetLastInitObject(channel) - (uint8)Can_GetInitObjectStartIndex(channel)) & 0x03u))); /* SBSW_CAN_HL03 */
# else
          Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
# endif
#endif
        }
        /* #250 START: set request and wait limited time and notify upper Layer - CanHL_StartTransition() */
        retval = CanHL_StartTransition(CAN_CHANNEL_CANPARA_ONLY);
        break;

      case CAN_T_STOP: /* \trace SPEC-3976 */
        /* #300 Handle --- STOP --- transition */
        /* #310 ESCAN00032291: no call of STOP inside BUSOFF (avoid blocking) */
        if (!Can_IsIsBusOff(channel)) /* COV_CAN_HARDWARE_FAILURE */
        {
          /* #320 STOP: set request and wait limited time and notify upper Layer - CanHL_StopTransition() */
          retval = CanHL_StopTransition(CAN_CHANNEL_CANPARA_ONLY);
        } /* IsBusOff */
        break;

      case CAN_T_WAKEUP:
        /* #400 Handle --- WAKEUP --- transition */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* does not appear in MSR30 (synchronous) */ /* COV_CAN_MICROSAR_VERSION */
        if ( (Can_GetStatusReq(channel) & CAN_REQ_SLEEP) == CAN_REQ_SLEEP )
        { /* #410 ASR4: Check WAKEUP not allowed while transition to SLEEP \trace SPEC-1573, SPEC-1402, SPEC-1403, SPEC-3848, SPEC-1660 */
          errorId = CAN_E_TRANSITION;
        }
        else
#endif
        {
          /* #420 WAKEUP: set request and wait limited time and notify upper Layer (RAM-check, Emulated SLEEP handling) - CanHL_WakeupTransition() */
          retval = CanHL_WakeupTransition(CAN_CHANNEL_CANPARA_ONLY);
        }
        break;

      case CAN_T_SLEEP: /* \trace SPEC-3926, SPEC-1639 */
        /* #500 Handle --- SLEEP --- transition */
        /* #510 SLEEP: set request and wait limited time and notify upper Layer (Emulated SLEEP handling) - CanHL_SleepTransition() */
        retval = CanHL_SleepTransition(CAN_CHANNEL_CANPARA_ONLY);
        break; /* CAN_T_SLEEP */

      default: /* PRQA S 2018 */ /* MD_Can_Asr_2018 */
        /* #600 check unknown transition, return NOT_OK \trace SPEC-1573, SPEC-1402, SPEC-1403 */
        errorId = CAN_E_TRANSITION; /* \trace SPEC-1733, SPEC-1407 */
        break;
      } /* switch */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
      /* #700 remove mark ongoing state change (suppress STOP transition in Can_InitController()) */ 
      Can_SetStatusReq(channel, Can_GetStatusReq(channel) & (uint8)(~(CAN_REQ_INIT_NO_STOP))); /* PRQA S 0277 */ /* MD_Can_0277_negation */ /* SBSW_CAN_HL01 */
#endif
      Can_LeaveCritical(CAN_EXCLUSIVE_AREA_6);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_SETCTR_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_SetControllerMode();
  return retval;
} /* PRQA S 6030,6050,6080 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_DisableControllerInterrupts
****************************************************************************/
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
/*
|<DataModelStart>| Can_DisableControllerInterrupts
Relation_Context:
Relation:
CommonCan
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
CanDevTimeoutDetect, HardwareLoop, HardwareCancelByAppl, DevErrorDetect
IntLock, CommonCan
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_DisableControllerInterrupts( uint8 Controller )
{ /* \trace SPEC-1746, SPEC-3886 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_DisableControllerInterrupts();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1750, SPEC-3880, SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1742, SPEC-3860 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* move below channel DET check */ /* COV_CAN_MULTI_ECU_CONFIG */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
  { /* #30 Check controller is in active ECU */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
  if (!CanHL_IsControllerInit(channel))
  { /* #40 Check controller is initialized \trace SPEC-1750, SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #100 EXCLUSIVE_AREA_1 secure interrupt lock handling */
    Can_EnterCritical(CAN_EXCLUSIVE_AREA_1);
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
# endif
#endif
    if (Can_GetCanInterruptCounter(channel) == 0)
    { /* #110 Disable only when not disabled before (count nesting) \trace SPEC-1735, SPEC-1745, SPEC-3783 */
#if (CAN_INTLOCK == CAN_DRIVER) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
      { /* #120 Disable CAN interrupt for each HW channel */
        CanLL_CanInterruptDisable(canHwChannel, &Can_GetCanInterruptOldStatus(canHwChannel)); /* SBSW_CAN_HL16 */
      }
#endif
#if (CAN_INTLOCK == CAN_APPL) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
      /* #130 Let application Disable CAN interrupt ApplCanInterruptDisable() */
      ApplCanInterruptDisable((uint8)channel);
#endif
    }
    Can_SetCanInterruptCounter(channel, Can_GetCanInterruptCounter(channel)+1); /* \trace SPEC-1748, SPEC-3875, SPEC-3876 */ /* SBSW_CAN_HL12 */
    Can_LeaveCritical(CAN_EXCLUSIVE_AREA_1);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_DIINT_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_DisableControllerInterrupts();
} /* PRQA S 6050,6080 */ /* MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_EnableControllerInterrupts
****************************************************************************/
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
/*
|<DataModelStart>| Can_EnableControllerInterrupts
Relation_Context:
Relation:
CommonCan
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
CanDevTimeoutDetect, HardwareLoop, HardwareCancelByAppl, DevErrorDetect
IntLock, CommonCan
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_EnableControllerInterrupts( uint8 Controller )
{ /* \trace SPEC-1741, SPEC-3891 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_EnableControllerInterrupts();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-3866, SPEC-1739, SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1752, SPEC-3858 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* move below channel DET check */ /* COV_CAN_MULTI_ECU_CONFIG */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
  { /* #30 Check controller is in active ECU */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
  if (!CanHL_IsControllerInit(channel))
  { /* #40 Check controller is initialized \trace SPEC-1739, SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #100 EXCLUSIVE_AREA_1 secure interrupt lock handling */
    Can_EnterCritical(CAN_EXCLUSIVE_AREA_1);
    if (Can_GetCanInterruptCounter(channel) != 0)
    { /* #110 Enable only when disabled before (count nesting) \trace SPEC-1749, SPEC-1748, SPEC-3875 */
      Can_SetCanInterruptCounter(channel, Can_GetCanInterruptCounter(channel)-1); /* SBSW_CAN_HL12 */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR30) /* COV_CAN_MICROSAR_VERSION */
# if defined(CAN_ENABLE_DEV_TIMEOUT_DETECT) /* COV_CAN_DEV_TIMEOUT_DETECT */
# endif
#endif
      if (Can_GetCanInterruptCounter(channel) == 0) /* \trace SPEC-1736, SPEC-1756, SPEC-3877 */
      {
#if (CAN_INTLOCK == CAN_DRIVER) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
        { /* #120 Disable CAN interrupt for each HW channel */
          CanLL_CanInterruptRestore(canHwChannel, Can_GetCanInterruptOldStatus(canHwChannel));
        }
#endif
#if (CAN_INTLOCK == CAN_APPL) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
        /* #130 Let application Enable CAN interrupt ApplCanInterruptRestore() */
        ApplCanInterruptRestore((uint8)channel);
#endif
      }
    }
    Can_LeaveCritical(CAN_EXCLUSIVE_AREA_1);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_ENINT_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_EnableControllerInterrupts();
} /* PRQA S 6050,6080 */ /* MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_Write
****************************************************************************/
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
/*
|<DataModelStart>| Can_Write
Relation_Context:
Relation:
CommonCan
DevErrorDetect, MicrosarVersion
OneChOpt, CommonCan
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
DevErrorDetect, TxFullCANSupport
DevErrorDetect, CanFdSupport
DevErrorDetect, MixedId
RamCheck
UseVectorCanIf
DevErrorDetect
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Can_ReturnType, CODE) Can_Write( Can_HwHandleType Hth, Can_PduInfoPtrType PduInfo )
{ /* \trace SPEC-1751, SPEC-3887,   SPEC-1760, SPEC-1406 */
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  CanHookBegin_Can_Write();
  errorId = CAN_E_NO_ERROR;
  retval = CAN_NOT_OK; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1759, SPEC-3864 */
    errorId = CAN_E_UNINIT;
  }
  else
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
  if (Hth >= Can_GetSizeOfMailbox()) {
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
#endif
  {
#if !defined( C_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_GetMailboxController(Hth);
#endif
#if (CAN_DEV_ERROR_DETECT == STD_ON)
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
# endif
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* move below channel DET check */ /* COV_CAN_MULTI_ECU_CONFIG */
    if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
    { /* #30 Check controller is in active ECU */
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
# elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
# endif
    if (!CanHL_IsControllerInit(channel))
    { /* #40 Check controller is initialized \trace SPEC-1407, SPEC-3962 */
      errorId = CAN_E_UNINIT;
    }
    else
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
    if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE))
# else
    if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE))
# endif
    { /* \trace SPEC-1763, SPEC-3870 */
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
    if ((PduInfo == NULL_PTR) || ((PduInfo->sdu == NULL_PTR) && (PduInfo->length != 0)))
    { /* \trace SPEC-1761, SPEC-3868 */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
# if !defined(C_ENABLE_CAN_FD_USED)
    if ( CanHL_IsFdMessage(PduInfo->id) )
    { /* no CAN_FD: Check parameter PduInfo->id should not have FD bit in ID */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
# endif
# if defined(C_ENABLE_CAN_FD_FULL)
    if ( ((PduInfo->length > 8) && (Can_GetInitObjectFdBrsConfig(Can_GetLastInitObject(channel)) == CAN_NONE)) ||
         (PduInfo->length > CAN_MAX_DATALEN_OBJ(Hth)) ) /* COV_CAN_GENDATA_FAILURE */
         { /* \trace SPEC-1758, SPEC-3865 */
      /* \trace SPEC-60433 */ /* CAN_FD: Check parameter PduInfo->length is smaller than 9 for none FD configuration
         If development error detection for the CAN module is enabled and the CAN Controller is not in CAN FD mode (no CanControllerFdBaudrateConfig):
         The function Can_Write shall raise the error CAN_E_PARAM_DLC and shall return CAN_NOT_OK if the length is more than 8 byte.() */
      /* CAN_FD: Check parameter PduInfo->length against PDU maximum size */
      errorId = CAN_E_PARAM_DLC;
    }
    else
# else
    if ( PduInfo->length > CAN_MAX_DATALEN_OBJ(Hth) ) /* CM_CAN_HL20 */
    { /* \trace SPEC-1758 */ /* Check parameter PduInfo->length against maximum buffer size */
      errorId = CAN_E_PARAM_DLC;
    }
    else
# endif
# if defined(C_ENABLE_MIXED_ID)
    if ( ((PduInfo->id & (Can_IdType)CAN_ID_IDE_MASK) != (Can_IdType)CAN_ID_IDE_MASK ) && ( (PduInfo->id & (Can_IdType)CAN_ID_MASK) > (Can_IdType)CAN_ID_MASK_STD ) )
    { /* Check parameter PduInfo->id for STD ID is in STD range (EXT is always in range) */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
# endif
    if ( (PduInfo->id & (Can_IdType)CAN_ID_UNUSED_MASK) != 0 )
    { /* Check parameter PduInfo->id contains illegal bits (e.g. bigger ID value than MAX for given type) */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
#endif
    /* ----- Implementation ----------------------------------------------- */
#if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #150 reject deactivated mailbox (by RamCheck) */
    if (Can_GetMailboxState(Hth) == CAN_DISABLED_OBJ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
      retval = CAN_NOT_OK;
    }
    else
#endif
    /* #180 check Hardware is in BUSOFF recovery state and is not able to handle a request: return BUSY \trace SPEC-1764 */
    if (Can_IsIsBusOff(channel)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
    {
      retval = CAN_NOT_OK; /* ESCAN00096369 */
    }
    else
#if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #200 controller is active (no RAM check issue): */
    if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* PRQA S 2004 */ /* MD_Can_Asr_2004 */ /* COV_CAN_HARDWARE_FAILURE */
#endif
    {
      CanObjectHandle  logTxObjHandle;
      /* #210 EXCLUSIVE_AREA_2: Interrupts may not be locked and re-entrant call may occur. */
      /*                        Time in between ID search and lock of mailbox is critical 
                                (only when no Vector Interface used otherwise CanIf EXCLUSIVE_AREA is used instead) 
                                Can_Write() could be called re-entrant, there for also for polling systems the interrupts have to be locked  
                                this is essential while check for mailbox is free (in between if (free) and set to not free) and save PDU handle */
#if !defined(CAN_USE_VECTOR_IF)
      Can_EnterCritical(CAN_EXCLUSIVE_AREA_2);
#endif
      /* #220 search for BasicCAN object handle to transmit (Multiplexed TX) and backup data for cancel TX object if necessary */
      logTxObjHandle = CanHL_WritePrepare(CAN_CHANNEL_CANPARA_FIRST Hth, PduInfo); /* SBSW_CAN_HL28 */
      /* #230 start transmission for given handle (if free) and return state */
      retval = CanHL_WriteStart(CAN_CHANNEL_CANPARA_FIRST Hth, logTxObjHandle, PduInfo); /* SBSW_CAN_HL28 */

#if !defined(CAN_USE_VECTOR_IF) /* No Vector Interface used, Interrupts may not be locked */
      /* avoid change of PDU information due to TX interrupt while changing */
      Can_LeaveCritical(CAN_EXCLUSIVE_AREA_2);
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_WRITE_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_Write();
  return retval;
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             ASR4.x - Can_CheckWakeup / ASR3.x - Can_Cbk_CheckWakeup
****************************************************************************/
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
/*
|<DataModelStart>| Can_CheckWakeup
Relation_Context:
Relation:
Wakeup, DevErrorDetect, OneChOpt
Wakeup, DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_CheckWakeup( uint8 Controller )
{ /* \trace SPEC-1740, SPEC-3922 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  CanHookBegin_Can_CheckWakeup();
  retval = E_NOT_OK; /* \trace SPEC-1407 */
#if defined(C_ENABLE_SLEEP_WAKEUP)
  {
    uint8 errorId;
    errorId = CAN_E_NO_ERROR;
  
    /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
    /* #10 Check if parameter controller is valid and state is INIT */
    if (canConfigInitFlag == CAN_STATUS_UNINIT)
    { /* #20 Check Driver is initialized \trace SPEC-3962, SPEC-3903 */
      errorId = CAN_E_UNINIT;
    }
    else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
    { /* \trace SPEC-1738, SPEC-3945 */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
    if (!CanHL_IsControllerInit(channel))
    { /* #40 Check controller is initialized \trace SPEC-1743 */ /* ESCAN00045884 */
      errorId = CAN_E_UNINIT;
    }
    else
# endif
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
    if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) != (cantVIdentityMsk)0 ) /* PRQA S 2004 */ /* MD_Can_Asr_2004 */
      /* #50 Check controller is in active ECU */
# endif
    {
     /* ----- Implementation ----------------------------------------------- */
     /* #100 if WAKEUP occur: */
     if (Can_IsIsWakeup(channel))
      {
        Can_SetIsWakeup(channel,FALSE); /* SBSW_CAN_HL09 */
        /* #110 Driver call EcuM_SetWakeupEvent() to prevent missing WAKEUP notification (multiple call is OK) */
        /*      since there is no more call from CanIf to EcuM_SetWakeupEvent() in AutoSar 3.2.1/4.0.3 */
        EcuM_SetWakeupEvent(Can_GetWakeupSourceRef(channel)); /* \trace SPEC-50550 */
        retval = E_OK; /* \trace SPEC-3954, SPEC-1755 */
      }
    }

    /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
    if (errorId != CAN_E_NO_ERROR)
    { /* Throw DET if an error detected before */
      Can_CallDetReportError(CAN_CKWAKEUP_ID, errorId);
    }
#  else
    CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
    CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  }
#else
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_CheckWakeup();
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_MainFunction_Write
****************************************************************************/
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
/*
|<DataModelStart>| Can_MainFunction_Write
Relation_Context:
Relation:
TxPolling, HwCancelTx, DevErrorDetect
TxPolling, HwCancelTx, OneChOpt
TxPolling, HwCancelTx, Variant
TxPolling, HwCancelTx, Wakeup
TxPolling, HwCancelTx, CommonCan
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Write( void )
{ /* \trace SPEC-3873, SPEC-1766 */
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_ENABLE_TX_POLLING) 
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  CanHookBegin_Can_MainFunction_Write();
  /* canSendSemaphor no more needed because of ControllerInterrupts disabled while Can_Write() */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3784, SPEC-1767,  SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  {  
    /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0; channel < kCanNumberOfChannels; channel++)
# endif
    { /* #20 over all active controller */
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* one controller optimization is always off */ /* COV_CAN_MULTI_ECU_CONFIG */
      if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
      { /* not for inactive controller */
        continue; /* PRQA S 0770 */ /* MD_Can_0770 */
      }
# elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue; /* PRQA S 0770 */ /* MD_Can_0770 */
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON) /* \trace SPEC-1573, SPEC-1402, SPEC-1403 */
      if (!CanHL_IsControllerInit(channel))
      { /* #50 Check controller is initialized */ /* \trace SPEC-3962 */
        errorId = CAN_E_UNINIT;
      }
      else
# endif
      {
# if defined(C_ENABLE_SLEEP_WAKEUP) || (CAN_EMULATE_SLEEP == STD_ON) /* COV_CAN_EMULATE_SLEEP */
        if ( !CanHL_IsSleep(channel) )
# endif
        { /* #60 do not access CAN hardware in SLEEP */
          {
# if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */ /* always true when no hw-cancel is supported */
            /* #100 do confirmation handling for pending mailboxes */
            CanHL_TxTaskConfPendingHandling(CAN_HW_CHANNEL_CANPARA_ONLY);
# endif
# if defined(C_ENABLE_CANCEL_TX_IN_HW) 
# endif /* defined(C_ENABLE_CANCEL_TX_IN_HW) */
          } /* CanHL_IsSleep */
        } /* end of loop over all HW channels */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_WRITE_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

#else
  CanHookBegin_Can_MainFunction_Write();
#endif /* C_ENABLE_TX_POLLING */
  CanHookEnd_Can_MainFunction_Write();
}

/****************************************************************************
| NAME:             Can_MainFunction_Read
****************************************************************************/
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
/*
|<DataModelStart>| Can_MainFunction_Read
Relation_Context:
Relation:
DevErrorDetect
RxPolling, OneChOpt
RxPolling, Variant
RxPolling, DevErrorDetect
RxPolling, Wakeup
RxPolling, CommonCan
RxPolling, RxFullCANSupport
RxPolling, RxBasicCANSupport
RxQueue
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Read( void )
{ /* \trace SPEC-3871, SPEC-1776 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_MainFunction_Read();
  errorId = CAN_E_NO_ERROR; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check Driver is initialized \trace SPEC-3841, SPEC-1784, SPEC-3852,  SPEC-3962 */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  {
    errorId = CAN_E_UNINIT; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  }
  else
#endif
  /* ----- Implementation ----------------------------------------------- */
  /* #20 semaphore to secure parallel access to RX buffers interrupt or polling \trace SPEC-1569, SPEC-3785 */
  if ( canRxTaskActive == CAN_FREE)  /* PRQA S 2004 */ /* MD_Can_Asr_2004 */
  {
#if defined(C_ENABLE_RX_POLLING) /* COV_CAN_RX_POLLING */
    CAN_CHANNEL_CANTYPE_LOCAL
#endif
    canRxTaskActive = CAN_BUSY; /* PRQA S 3198 */ /* MD_MSR_14.2 */
#if defined(C_ENABLE_RX_POLLING) /* \trace SPEC-1782, SPEC-3771 */ /* COV_CAN_RX_POLLING */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0; channel < kCanNumberOfChannels; channel++)
# endif
    { /* #30 iterate over all active controller */
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* one controller optimization is always off */ /* COV_CAN_MULTI_ECU_CONFIG */
      if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
      { /* not for inactive controller */
        continue; /* PRQA S 0770 */ /* MD_Can_0770 */
      }
# elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue; /* PRQA S 0770 */ /* MD_Can_0770 */
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_IsControllerInit(channel))
      { /* #60 Check controller is initialized \trace SPEC-3962 */
        errorId = CAN_E_UNINIT; /* PRQA S 3198 */ /* MD_MSR_14.2 */
      }
      else
# endif
      {
# if defined(C_ENABLE_SLEEP_WAKEUP) || (CAN_EMULATE_SLEEP == STD_ON) /* COV_CAN_EMULATE_SLEEP */
        if ( !CanHL_IsSleep(channel) )
# endif
        { /* #70 do not access CAN hardware in SLEEP */
          { /* #80 over all HW channel \trace SPEC-3814, SPEC-1774 */
# if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
            /* #90 search for all pending FullCAN mailbox objects and call notification */
            CanHL_RxFullMsgReceivePolling(CAN_HW_CHANNEL_CANPARA_ONLY);
# endif
# if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
            /* #100 search for all pending BasicCAN mailbox objects and call notification */
            CanHL_RxBasicMsgReceivePolling(CAN_HW_CHANNEL_CANPARA_ONLY);
# endif
          } /* CanHL_IsSleep */
        } /* end of loop over all HW channels */
      }
    }
#endif /* C_ENABLE_RX_POLLING */
#if defined(C_ENABLE_RX_QUEUE)
    /* #200 execute RxQueue CanHL_RxQueueExecution() */
    errorId |= CanHL_RxQueueExecution();
#endif
    canRxTaskActive = CAN_FREE;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 3356,3359 */ /* MD_Can_13.7,MD_Can_13.7 */
  { /* Throw DET if an error detected before */ /* PRQA S 3201 */ /* MD_Can_3201 */
    Can_CallDetReportError(CAN_MAINFCT_READ_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  CanHookEnd_Can_MainFunction_Read();
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_MainFunction_BusOff
****************************************************************************/
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
/*
|<DataModelStart>| Can_MainFunction_BusOff
Relation_Context:
Relation:
StatusPolling, DevErrorDetect
StatusPolling, OneChOpt
StatusPolling, Variant
StatusPolling, Wakeup
StatusPolling, CommonCan
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_BusOff( void )
{ /* \trace SPEC-1783, SPEC-3867 */
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_ENABLE_ERROR_POLLING) /* \trace SPEC-1778, SPEC-3818 */ /* COV_CAN_ERROR_POLLING */
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  CanHookBegin_Can_MainFunction_BusOff();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3835,  SPEC-1780, SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  {  
    /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0; channel < kCanNumberOfChannels; channel++)
# endif
    { /* #20 over all active controller */
# if defined(C_ENABLE_MULTI_ECU_CONFIG) /* one controller optimization is always off */ /* COV_CAN_MULTI_ECU_CONFIG */
      if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
      { /* not for inactive controller */
        continue; /* PRQA S 0770 */ /* MD_Can_0770 */
      }
# elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue; /* PRQA S 0770 */ /* MD_Can_0770 */
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_IsControllerInit(channel))
      { /* #50 Check controller is initialized */ /* \trace SPEC-3962 */
        errorId = CAN_E_UNINIT;
      }
      else
# endif
      {
# if defined(C_ENABLE_SLEEP_WAKEUP) || (CAN_EMULATE_SLEEP == STD_ON) /* COV_CAN_EMULATE_SLEEP */
        if ( !CanHL_IsSleep(channel) )
# endif
        { /* #60 do not access CAN hardware in SLEEP */
          { /* #70 loop over all HW controllers and call CanHL_ErrorHandling() \trace SPEC-1772, SPEC-3790 */
            /* #80 disable CAN interrupts */
            Can_DisableControllerInterrupts((uint8)channel);
            CanHL_ErrorHandling( CAN_HW_CHANNEL_CANPARA_ONLY );
            Can_EnableControllerInterrupts((uint8)channel);
          }
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_BO_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#else /* C_ENABLE_ERROR_POLLING */
  CanHookBegin_Can_MainFunction_BusOff();
#endif /* C_ENABLE_ERROR_POLLING */
  CanHookEnd_Can_MainFunction_BusOff();
}

/****************************************************************************
| NAME:             Can_MainFunction_Wakeup
****************************************************************************/
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
/*
|<DataModelStart>| Can_MainFunction_Wakeup
Relation_Context:
Relation:
WakeupPolling, Wakeup, DevErrorDetect
WakeupPolling, Wakeup, OneChOpt
WakeupPolling, Wakeup, Variant
WakeupPolling, Wakeup, CommonCan
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Wakeup( void )
{ /* \trace SPEC-1770, SPEC-3885 */
  /* ----- Local Variables ---------------------------------------------- */
  CanHookBegin_Can_MainFunction_Wakeup();
  CanHookEnd_Can_MainFunction_Wakeup();
}

#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
/****************************************************************************
| NAME:             Can_MainFunction_Mode
****************************************************************************/
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
/*
|<DataModelStart>| Can_MainFunction_Mode
Relation_Context:
Relation:
DevErrorDetect
OneChOpt
Variant
Wakeup
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Mode( void )
{ /* \trace SPEC-1775 */
  /* ----- Local Variables ---------------------------------------------- */
  CanHookBegin_Can_MainFunction_Mode();
  CanHookEnd_Can_MainFunction_Mode();
} /* PRQA S 6030,6050 */ /* MD_MSR_STCYC,MD_MSR_STCAL */

#endif

/****************************************************************************
| NAME:             Can_CancelTx
****************************************************************************/
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
 */
/*
|<DataModelStart>| Can_CancelTx
Relation_Context:
Relation:
TransmitCancellationAPI, CommonCan
TransmitCancellationAPI, DevErrorDetect
TransmitCancellationAPI, TxFullCANSupport
TransmitCancellationAPI, OneChOpt, CommonCan
TransmitCancellationAPI, DevErrorDetect, OneChOpt
TransmitCancellationAPI, DevErrorDetect, Variant
TransmitCancellationAPI, MultiplexedTx, TxFullCANSupport
TransmitCancellationAPI, MultiplexedTx, HwCancelTx
ChannelAmount
TxFullAmount
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_CancelTx( Can_HwHandleType Hth, PduIdType PduId )
{
  /* ----- Local Variables ---------------------------------------------- */
#if (CAN_CANCEL_SUPPORT_API == STD_ON)
  CAN_CHANNEL_CANTYPE_LOCAL
  Can_HwHandleType txObjHandle;
  CanObjectHandle logTxObjHandle;
  uint8 errorId;
  CanHookBegin_Can_CancelTx();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
  if (Hth >= Can_GetSizeOfMailbox())
  { /* #20 Check Hth is in range */
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
#  endif
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
  if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE))
# else
  if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE))
# endif
  { /* #30 Check Hth is FullCAN or BasicCAN */
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
  {
# if !defined( C_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_GetMailboxController(Hth);
# endif
    txObjHandle = Can_GetHwHandle(Hth);
    logTxObjHandle = CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANPARA_FIRST txObjHandle);  
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    { /* #40 Check parameter controller is in range */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
#  if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
    if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 ) /* COV_CAN_GENDATA_FAILURE */
    { /* #50 Check controller is in active ECU */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_FAILURE */
    { /* #60 Check parameter controller is valid */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
    if (!CanHL_IsControllerInit(channel))
    { /* #70 Check controller is initialized \trace SPEC-3962 */
      errorId = CAN_E_UNINIT;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) /* \trace SPEC-1677, SPEC-3957 */
      uint8_least muxTx;
#  if defined (C_ENABLE_TX_FULLCAN_OBJECTS)
      if ( Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE )
#  endif
      { /* #80 is BasicCAN and Multiplexed TX - search for handle */
        uint8_least muxTx_max;
        muxTx_max = CAN_MULTIPLEXED_TX_MAX_OBJ(Hth); /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
        for (muxTx = 0; muxTx < muxTx_max; muxTx++)
        {
          if (Can_GetActiveSendPdu(logTxObjHandle) == PduId)
          {
            if ((Can_GetActiveSendState(logTxObjHandle) == CAN_SEND_OBJ)
#  if defined(C_ENABLE_CANCEL_TX_IN_HW)
                || (Can_GetActiveSendState(logTxObjHandle) == CAN_CANCEL_OBJ)
#  endif
                )  /* COV_CAN_CANCEL_SW_STIMULATION */
            { /* #90 PDU found in active send list - handle found */
              break;
            }
          }
          logTxObjHandle++;
          txObjHandle++;
        }
      }
# endif
      if (Can_GetActiveSendPdu(logTxObjHandle) == PduId)
      {
        if ((Can_GetActiveSendState(logTxObjHandle) == CAN_SEND_OBJ)
# if defined(C_ENABLE_CANCEL_TX_IN_HW)
            || (Can_GetActiveSendState(logTxObjHandle) == CAN_CANCEL_OBJ)
# endif
           )  /* COV_CAN_CANCEL_SW_STIMULATION */
        { /* #100 mailbox to cancel is valid (PDU, handle, send state OK) */
          Can_SetActiveSendState(logTxObjHandle, CAN_CANCEL_OBJ_SW); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
          /* #110 CANCEL_IN_HW nor CANCEL_FULLCAN_IN_HW is active - just suppress TX confirmation */
# if defined(C_ENABLE_CANCEL_IN_HW) 
          /* #120 only CANCEL_FULLCAN_IN_HW so no BasicCAN cancellation - need both arguments because of ORGANI */
          { /* #130 LL TX cancel in HW */ /* \trace SPEC-1690, SPEC-3947 */
            /*
            |<DataModelStart>| CanLL_CancelInHw
            Relation_Context:
            HwCancelTx
            Relation:
            Parameter_PreCompile:
            Parameter_Data:
            Constrain:
            |<DataModelEnd>|
            */
            /*
            Do not activate the following line! CANoe is only a emulation and does not really cancel the message out of hardware
            Can_GetgCanController(channel).MsgObj[txObjHandle].mCanCancel = (vuint8)kCanTrue;  SBSW_CAN_LL01
            */
            CAN_DUMMY_STATEMENT(txObjHandle); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
          }
# endif
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_WRITE_ID, errorId);
  }
#  else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
#else /* CAN_CANCEL_SUPPORT_API == STD_ON */
  CanHookBegin_Can_CancelTx();
  CAN_DUMMY_STATEMENT(Hth); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CAN_DUMMY_STATEMENT(PduId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* CAN_CANCEL_SUPPORT_API == STD_ON */
  CanHookEnd_Can_CancelTx();
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_ResetBusOffStart
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_ResetBusOffStart
Relation_Context:
Relation:
BusOffRecovery, DevErrorDetect, OneChOpt
BusOffRecovery, DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_ResetBusOffStart( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined(CAN_ENABLE_BUSOFF_SUPPORT_API) /* COV_CAN_BUSOFF_SUPPORT_API */
  uint8 errorId;
  CanHookBegin_Can_ResetBusOffStart();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #20 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
  { /* #40 Check controller is in active ECU */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* #50 Check controller is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #60 do BUSOFF handling begin by call CanResetBusOffStart() - controller should be stopped afterwards but does not wait for bus transition here */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    CanResetBusOffStart(channel, Can_GetLastInitObject(channel));
# else
    CanResetBusOffStart(Can_GetLastInitObject(channel));
    CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CTRBUSOFF_ID, errorId);
  }
#  else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
#else
  CanHookBegin_Can_ResetBusOffStart();
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ( BUSOFF_SUPPORT_API ) */
  CanHookEnd_Can_ResetBusOffStart();
}

/****************************************************************************
| NAME:             Can_ResetBusOffEnd
****************************************************************************/
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
/*
|<DataModelStart>| Can_ResetBusOffEnd
Relation_Context:
Relation:
BusOffRecovery, DevErrorDetect, OneChOpt
BusOffRecovery, DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_ResetBusOffEnd( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined(CAN_ENABLE_BUSOFF_SUPPORT_API) /* COV_CAN_BUSOFF_SUPPORT_API */
  uint8 errorId;
  CanHookBegin_Can_ResetBusOffEnd();
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #20 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
  { /* #40 Check controller is in active ECU */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* #50 Check controller is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  { /* #60 reset BUSOFF flag do this before CanResetBusOffEnd() so LL may overwrite it */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetIsBusOff(channel,FALSE); /* SBSW_CAN_HL08 */ 
    /* #70 do BUSOFF finish handling by call CanResetBusOffEnd() - controller should be re initialized afterwards */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    CanResetBusOffEnd(channel, Can_GetLastInitObject(channel));
    CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# else
    CanResetBusOffEnd(Can_GetLastInitObject(channel));
    CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CTRBUSOFF_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#else
  CanHookBegin_Can_ResetBusOffEnd();
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ( BUSOFF_SUPPORT_API ) */
  CanHookEnd_Can_ResetBusOffEnd();
}

#if (CAN_GET_STATUS == STD_ON)
/****************************************************************************
| NAME:             Can_GetStatus
****************************************************************************/
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
/*
|<DataModelStart>| Can_GetStatus
Relation_Context:
Relation:
CanGetStatus, CommonCan
CanGetStatus, DevErrorDetect, OneChOpt
CanGetStatus, DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, uint8, CODE) Can_GetStatus( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 canReturnCode;
  uint8 errorId;
  /*
  |<DataModelStart>| CanLL_GetStatusLocalParameter
  Relation_Context:
  CanGetStatus
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  CanHookBegin_Can_GetStatus();
  errorId = CAN_E_NO_ERROR;
  canReturnCode = CAN_STATUS_INCONSISTENT; /* PRQA S 3198 */ /* MD_MSR_14.2 */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #20 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
  if ( (cantVIdentityMsk)(Can_GetChannelIdentityAssignment(channel) & V_ACTIVE_IDENTITY_MSK) == (cantVIdentityMsk)0 )
  { /* #40 Check controller is in active ECU */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  elif (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* #50 Check controller is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /*
    |<DataModelStart>| CanLL_GetStatusBegin
    Relation_Context:
    CanGetStatus
    Relation:
    Parameter_PreCompile:
    Parameter_Data:
    Constrain:
    |<DataModelEnd>|
    */
    /* #100 set logical state as return value */
    canReturnCode = (uint8)Can_GetLogStatus(channel); /* PRQA S 3198 */ /* MD_MSR_14.2 */
    if ( CanLL_HwIsBusOff(CAN_HW_CHANNEL_CANPARA_ONLY) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #150 set BUSOFF - BUSOFF can only happen on first HW channel (with TX objects) */
      canReturnCode |= CAN_STATUS_BUSOFF;
    }
# if defined(C_ENABLE_EXTENDED_STATUS) /* COV_CAN_EXTENDED_STATUS */
    {    
      if ( CanLL_HwIsPassive(CAN_HW_CHANNEL_CANPARA_ONLY) ) /* COV_CAN_HARDWARE_FAILURE */
      { /* #160 set Error Passive */
        canReturnCode |= CAN_STATUS_PASSIVE;
      }    
      if ( CanLL_HwIsWarning(CAN_HW_CHANNEL_CANPARA_ONLY) ) /* COV_CAN_HARDWARE_FAILURE */
      { /* #170 set Error Warning */
        canReturnCode |= CAN_STATUS_WARNING;
      }
    }
# endif /* C_ENABLE_EXTENDED_STATUS */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  CanHookEnd_Can_GetStatus();
  return (uint8)canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if defined(C_ENABLE_MIRROR_MODE)
/****************************************************************************
| NAME:             Can_SetMirrorMode
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_SetMirrorMode
Relation_Context:
MirrorMode
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_SetMirrorMode( uint8 Controller , CddMirror_MirrorModeType mirrorMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 set mirror mode for given controller */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetMirrorModeState(channel, mirrorMode); /* SBSW_CAN_HL20 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}
#endif

#if defined(C_ENABLE_SILENT_MODE)
/****************************************************************************
| NAME:             Can_SetSilentMode
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_SetSilentMode
Relation_Context:
SilentMode
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_SetSilentMode( uint8 Controller, Can_SilentModeType silentMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Std_ReturnType retVal;
  errorId = CAN_E_NO_ERROR;
  retVal = E_NOT_OK;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 set mirror mode for given controller */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetSilentModeState(channel, silentMode); /* SBSW_CAN_HL20 */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return retVal;
}
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
/****************************************************************************
| NAME:             Can_RamCheckExecute
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_RamCheckExecute
Relation_Context:
RamCheck
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_RamCheckExecute( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 Do RamCheck by execute Can_InitController() with activated check */
    /* ----- Implementation ----------------------------------------------- */
    CanHL_DoRamCheck(CAN_CHANNEL_CANPARA_ONLY);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}

/****************************************************************************
| NAME:             Can_RamCheckEnableMailbox
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_RamCheckEnableMailbox
Relation_Context:
RamCheck
Relation:
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_RamCheckEnableMailbox( Can_HwHandleType htrh )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
  if (htrh >= Can_GetSizeOfMailbox()) { /* CM_CAN_HL28 */
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #15 disable controller to force reinitialization afterwards in Can_RamCheckEnableController() */
    Can_SetLogStatus(Can_GetMailboxController(htrh), CAN_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL40 */
    /* #20 reactivate mailbox */
    Can_SetMailboxState(htrh, CAN_ENABLE_OBJ); /* SBSW_CAN_HL34 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}

/****************************************************************************
| NAME:             Can_RamCheckEnableController
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| Can_RamCheckEnableController
Relation_Context:
RamCheck
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(V_NONE, void, CODE) Can_RamCheckEnableController( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-3962 */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  {

    /* ----- Implementation ----------------------------------------------- */
    /* #30 activate controller only when currently disabled */
    if ( (Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) == CAN_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
      /* #40 call Can_InitController() to activate mailboxes and controller after RAM check issue */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)(~(CAN_DEACTIVATE_CONTROLLER)));  /* PRQA S 0277 */ /* MD_Can_0277_negation */ /* SBSW_CAN_HL02 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40) /* COV_CAN_MICROSAR_VERSION */
      Can_InitController((uint8)channel, &Can_GetControllerBaudrateConfig(((uint8)(Can_GetLastInitObject(channel) - (uint8)Can_GetInitObjectStartIndex(channel)) & 0x03u))); /* SBSW_CAN_HL03 */
# else
      Can_InitController((uint8)channel, NULL_PTR); /* SBSW_CAN_HL03 */
# endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* dummy assignment avoid compiler warning */ /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}
#endif

#define CAN_STOP_SEC_CODE  /*----------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/****************************************************************************/

/****************************************************************************/
/*  excluded Requirements                                                   */
/****************************************************************************/
/* part of other component: SPEC-1597, SPEC-3898, SPEC-3972 */
/* Currently no use-case / not supported: SPEC-1601, SPEC-3889, SPEC-1592, SPEC-1582 */

/****************************************************************************/
/*  MISRA deviations                                                        */
/****************************************************************************/
/* Justification for module-specific MISRA deviations:

MD_Can_Asr_Unused
Misra Rule Major
  Reason: Symbol not needed in each configuration or for each platform/derivative
  Risk: None
  Prevention: None

MD_Can_Asr_0781
Misra Rule 5.6
  Reason: Same name is used for same kind of data
  Risk: None
  Prevention: None

MD_Can_Asr_3408_libCheck
Misra Rule 8.8
  Reason: external linkage used for library check
  Risk: library check failed.
  Prevention: Integration test version check

MD_Can_Asr_3447
Misra Rule 8.8
  Reason: external linkage but not in header file because this is only used for Driver internal
  Risk: Symbolic conflict for other module
  Prevention: module prefix are part of symbol and possible conflicts checked by linker

MD_Can_Asr_3408
Misra Rule 8.8
  Reason: external linkage but not in header file because this is only used for Driver internal
  Risk: Symbolic conflict for other module
  Prevention: module prefix are part of symbol and possible conflicts checked by linker

MD_Can_Asr_3451
Misra Rule 8.8
  Reason: external linked symbol used in more than one file but should not be declared in header to prevent a external visibility
  Risk: different declaration of same object
  Prevention: possible conflicts checked by linker

MD_Can_Asr_3353_logTxobjHandle
Misra Rule 9.1
  Reason: Because CAN_MULTIPLEXED_TX_MAX is at least 1 the variable logTxobjHandle_use is always set before
  Risk: None
  Prevention: None

MD_Can_Asr_3353_CommonCanTransition
Misra Rule 9.1
  Reason: TransitionDone will be set depend on CommonCAN loop usage. Because CommonCAN loop will always be entered the value will be set.
  Risk: None
  Prevention: None

MD_Can_Asr_3418
Misra Rule 12.10
  Reason: comma operator in for-loop header used to get a compact code
  Risk: None
  Prevention: None

MD_Can_Asr_2018
Misra Rule 14.1
  Reason: default case is not necessary for all configurations
  Risk: None
  Prevention: None

MD_Can_Asr_3219
Misra Rule 14.1
  Reason: static function is not used depend on configuration aspect. No preprocessor used here to avoid unreadable code.
  Risk: None
  Prevention: None

MD_Can_Asr_2004
Misra Rule 14.10
  Reason: no default handling needed for if-else-if here
  Risk: missing default handling
  Prevention: Review

MD_Can_Asr_0881_abstract
Misra Rule 19.12
  Reason: Compiler abstraction need replacement of classes to avoid name conflict. Order of ## operator is irrelevant here.
  Risk: None
  Prevention: None

MD_Can_Asr_3689_MuxTx
Misra Rule 21.1
  Reason: muxTx index seems to be out of bounds for the array canTxBasicDataBuffer but it is limited to maximum size of array.
  Risk: None
  Prevention: Review

MD_Can_Asr_STPTH
Misra Metric STPTH
  Reason: control structure to evaluate ErrorId used sequential instead of nested to increase readability. PTH is 128 times lower than the worst case value from QAC.
  Risk: None
  Prevention: Review

MD_Can_3436:
Major
  Reason: Compatibility define used here.
  Risk: name conflict.
  Prevention: Code review and compiler check for name conflicts.

MD_Can_3199_dummy:
Redundancy
  Reason: Dummy assignment used to satisfy API parameters that are never used.
  Risk: None.
  Prevention: None.

MD_Can_0715:
Misra Rule 1.1
  Reason: Complex code structure used and not split in modules or functions to make the code compact and efficient.
  Risk: Hard to understand.
  Prevention: Code inspection and multiple configuration aspects used in component test.

MD_Can_0828:
Misra Rule 1.1
  Reason: Nesting of #if statements bigger than 8 depends on configuration aspects.
  Risk: Compiler does not support that.
  Prevention: Used Compilers checked and limitations form ANSI is not relevant.

MD_Can_0857:
Misra Rule 1.1
  Reason: Macros used to make the code compact and efficient. Some of the macros are generated with long names to unify them.
  Risk: Hard to understand.
  Prevention: Code inspection and multiple configuration aspects used in component test.

MD_Can_0505:
Misra Rule 1.2
  Reason: Assertion prevents dereferencing a NULL pointer
  Risk: Undefined behaviour.
  Prevention: Code inspection.

MD_Can_0277_negation:
Misra Rule 3.1
  Reason: negation of an unsigned value used to mask bits.
  Risk: None.
  Prevention: None.

MD_Can_0277_invalid:
Misra Rule 3.1
  Reason: define a invalid value by use highest value of the given data type.
  Risk: None.
  Prevention: None.

MD_Can_3218:
Misra Rule 8.7
  Reason: The usage of variables depend on configuration aspects and may be used only once, or defined global to improve overview.
  Risk: name conflict.
  Prevention: Compiler optimize and check for name conflicts.

MD_Can_3207:
Misra Rule 8.7
  Reason: The usage of variables depend on configuration aspects and may be used or not once in this particular configuration.
  Risk: unnecessary RAM occupied.
  Prevention: Compiler optimize and check for name conflicts.
  
MD_Can_3408_extLinkage:
Misra Rule 8.8
  Reason: Declaration of generated data is only internal to restrict visibility
  Risk: None.
  Prevention: None.

MD_Can_3447:
Misra Rule 8.8
  Reason: This appears only in internal test configurations.
  Risk: Symbolic conflict for other module.
  Prevention: -
  
MD_Can_3684:
Misra Rule 8.12
  Reason: Arrays declared without size, because size depends on configuration and is unknown here, especially for link time tables.
  Risk: Data access outside table.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_3760_LogToHw:
Misra Rule 10.01
  Reason: Calculating hardware index out of logical need a negative value in calculation, but result is always positive.
  Risk: Data access out of bounce.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_1281:
Misra Rule 10.6
  Reason: type of casting variable can be configured
  Risk: wrong size of data type could be used
  Prevention: Code inspection.

MD_Can_0303_HWaccess:
Misra Rule 11.3
  Reason: Hardware access need pointer to CAN cell as volatile.
  Risk: Access of unknown memory.
  Prevention: Runtime tests.

MD_Can_0310:
Misra Rule 11.4
  Reason: cast increase performance here.
  Risk: pointer alignment does not fit and wrong memory is accessed.
  Prevention: Code inspection and component test.

MD_Can_0311:
Misra Rule 11.5
  Reason: Constant pointer used only for read access. Same macro is used also for none constant pointer.
  Risk: None.
  Prevention: None.

MD_Can_0312:
Misra Rule 11.5
  Reason: cast to none volatile pointer is ok here because usage of data is read only here.
  Risk: Upper Layer has to make sure that there is no write access to this data.
  Prevention: Documentation of API.

MD_Can_13.7:
Misra Rule 13.7
  Reason: Result or value is always true or false depend on configuration aspect and used platform specific implementation
  Risk: Incorrect platform specific return will be ignored.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_3201:
Misra Rule 14.1
  Reason: Configuration or platform specific dependent unreachable code.
  Risk: Incorrect behaviour depend on settings.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_3325:
Misra Rule 14.1
  Reason: This control expression is always true or false depend on configuration aspect and used platform specific implementation.
  Risk: Incorrect behaviour depend on settings.
  Prevention: Code inspection and test of the different variants in the component test.
  
MD_Can_3109:
Misra Rule 14.3
  Reason: Empty statements occur multiple times because macros used like functions, and configuration aspects may deactivate complete functionality.
  Risk: Functionality unexpected missing.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_2001:
Misra Rule 14.4
  Reason: 'goto' statement used to optimize code (readability and runtime).
  Risk: Incorrect jump.
  Prevention: Code inspection.

MD_Can_0770:
Misra Rule 14.5
  Reason: 'continue' statement used to optimize code (readability and runtime).
  Risk: Relevant fragments of code not executed.
  Prevention: Code inspection.

MD_Can_14.6:
Misra Rule 14.6
  Reason: multiple break used for optimize code runtime
  Risk: Relevant fragments of code not executed.
  Prevention: Code inspection.

MD_Can_2006:
Misra Rule 14.7
  Reason: Multiple return path used to optimize code (readability and runtime).
  Risk: Return a function too soon.
  Prevention: Code inspection.

MD_Can_1330:
Misra Rule 16.4
  Reason: no impact on functionality, due to different configurations the function paramters can be different
  Risk: traceability could be difficult 
  Prevention: Code inspection.
  
MD_Can_1334:
Misra Rule 16.4
  Reason: no impact on functionality, due to different configurations the function paramters can be different
  Risk: traceability could be difficult 
  Prevention: Code inspection.
  
MD_Can_3673:
Misra Rule 16.7
  Reason: no change due to compatibility of the API
  Risk: data could be modified without intention
  Prevention: Code inspection.
  
MD_Can_17.4:
Misra Rule 17.4
  Reason: Pointer arithmetic used multiple times to optimize runtime.
  Risk: Access wrong memory.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_0750:
Misra Rule 18.4
  Reason: Using union type to handle configuration specific data access.
  Risk: Misinterpreted data.
  Prevention: Code inspection.
  
MD_Can_0759:
Misra Rule 18.4
  Reason: Using union type to handle configuration specific data access.
  Risk: Misinterpreted data.
  Prevention: Code inspection.

MD_Can_3412:
Misra Rule 19.4
  Reason: Complex Macros with unrecognised code-fragment used to optimize code runtime.
  Risk: Readability reduced.
  Prevention: Code inspection.

MD_Can_3458:
Misra Rule 19.4
  Reason:Complex Macros without braced code statement blocks used to optimize code runtime.
  Risk: Readability reduced.
  Prevention: Code inspection.

MD_Can_3460:
Misra Rule 19.4
  Reason: void used as macro to get configuration specific API.
  Risk: Readability reduced.
  Prevention: Compile test.
  
MD_Can_0841:
Misra Rule 19.6
  Reason:undefine macro for compatibility reasons and re-usage of platform specific code.
  Risk: Meaning of macro is uncertain and may lead to wrong implementation.
  Prevention: Code inspection, compile tests and runtime tests.

MD_Can_3453:
Misra Rule 19.7
  Reason: Complex Macros used to optimize code runtime.
  Risk: Readability reduced.
  Prevention: Code inspection.

MD_Can_0850:
Misra Rule 19.8
  Reason: Macro argument is empty depend on usage of compiler abstraction.
  Risk: None.
  Prevention: Code inspection.

  MD_Can_3410:
Misra Rule 19.10
  Reason: Parameter used in structures that does not allow brackets
  Risk: None atomic parameter lead to compile errors
  Prevention: Used parameter is always atomic depend on usage

MD_Can_0342:
Misra Rule 19.13
  Reason: Glue operator used to abstract AuotSar compiler abstraction
  Risk: Only K&R compiler support glue operator
  Prevention: Compile test show whether compiler accept glue operator

MD_Can_0291:
Misra Rule 21.1
  Reason: "if" query prevents negative arithmetic result
  Risk: index of array could be negative and therefore undefined
  Prevention: Code inspection. 
  
MD_Can_3382:
Misra Rule 21.1
  Reason: "if" query prevents overflow arithmetic result
  Risk: index of array could exceed dimension (due to overflow) and therefore undefined
  Prevention: Code inspection.  
  
MD_Can_3689:
Misra Rule 21.1
  Reason: assertion prevents out of bound indexing
  Risk: undefined behaviour
  Prevention: Code inspection. Index cannot exceed the maximum dimension of the array.
  
MD_Can_3305_LL:
Major
  Reason: Pointer cast to stricter alignment is no problem here because it points to a aligned structure
  Risk: None.
  Prevention: None.

MD_Can_3436_LL
Major
  Reason: Indirection between original table name and defined pointer to same table used for post-build abstraction
  Risk: Naming conflict
  Prevention: Review

MD_Can_5013_LL:
MISRA Rule 6.3
  Reason: API use explicit generic type and cast is needed here.
  Risk: None
  Prevention: None

*/

/****************************************************************************/
/*  Silent deviations                                                       */
/****************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CAN_HL01
 \DESCRIPTION Write access to CanStatusReq with controller as index. Controller index is checked against sizeof CanStatusReq in Can_SetControllerMode(), Can_RamCheckExecute(), Can_MainFunction_Mode(), Can_Init() and Can_InitController().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL02
 \DESCRIPTION Write access to CanLogStatus with controller as index. Controller index is checked against sizeof CanLogStatus in Can_SetControllerMode(), CanHL_ErrorHandling(), Can_RamCheckEnableController(), Can_MainFunction_Mode(), Can_Init() and Can_InitController().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL03
 \DESCRIPTION Call Can_InitController() with parameter configuration pointer, which is never been used within MSR403. Error directive ensures that preprocessor switch MSR403 is set in SafeBSW context.
 \COUNTERMEASURE \N The configuration pointer is never used in called function (only used for MSR401 to get data).

\ID SBSW_CAN_HL04
 \DESCRIPTION Write access to CanActiveSendState with logTxObjHandle as index. LogTxObjHandle is checked against size of ActiveSendObject in Can_CleanUpSendState(), CanHL_WritePrepare(), CanHL_InitTxFullCAN(), CanHL_InitTxBasicCAN(), CanHL_TxTaskCancelationHandling(), CanHL_TxConfirmation() and Can_CancelTx().
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL05
 \DESCRIPTION Write access to CanActiveSendPdu with logTxObjHandle as index. LogTxObjHandle is checked against size of ActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL06
 \DESCRIPTION Call Can_GenericPrecopy() with parameter data pointer. The data pointer is only used for read access and it is always a valid pointer (module local defined).
 \COUNTERMEASURE \R [CM_CAN_HL25]

\ID SBSW_CAN_HL07
 \DESCRIPTION Call CanIf_RxIndication() with parameter data pointer. The data pointer is only used for read access. Parameter is of type P2CONST in API.
 \COUNTERMEASURE \R [CM_CAN_HL25]

\ID SBSW_CAN_HL08
 \DESCRIPTION Write access to CanIsBusOff with controller as index. Controller index is checked against sizeof CanIsBusOff in Can_SetControllerMode(), CanHL_ErrorHandling() and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL09
 \DESCRIPTION Write access to CanIsWakeup with controller as index. Controller index is checked against sizeof CanIsWakeup in CanHL_WakeUpHandling(), Can_CheckWakeup(), Can_InitController() and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL10
 \DESCRIPTION Write access to VersionInfo pointer within function Can_GetVersionInfo()
 \COUNTERMEASURE \N caller of Can_GetVersionInfo() has to ensure that VersionInfo pointer is valid. (global description available)

\ID SBSW_CAN_HL11
 \DESCRIPTION Write access to CanLastInitObject with controller as index. Controller index is checked against sizeof CanLastInitObject in Can_InitStruct(), Can_ChangeBaudrate(), Can_SetBaudrate() and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL12
 \DESCRIPTION Write access to CanInterruptCounter with controller as index. Controller index is checked against sizeof CanInterruptCounter in Can_Init(), Can_EnableControllerInterrupts() and Can_DisableControllerInterrupts()
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL13
 \DESCRIPTION Write access to CanTimeoutCall with controller as index. Controller index is checked against sizeof CanTimeoutCall in Can_Init(), Can_SetControllerMode() and Can_Mainfunction_Mode().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL14
 \DESCRIPTION Write access to CanIsHardwareCanceled with controller as index. Controller index is checked against sizeof CanIsHardwareCanceled in Can_Init(), Can_SetControllerMode(), Can_SetBaudrate() and ApplCanTimerLoop().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL15
 \DESCRIPTION Write access to CanLoopTimeout with controller and source as index. Controller index is checked against sizeof CanLoopTimeout.
 \COUNTERMEASURE \R [CM_CAN_HL01]
                 \R [CM_CAN_HL22]

\ID SBSW_CAN_HL16
 \DESCRIPTION Call CanLL_CanInterruptDisable() with parameter CanInterruptOldStatus as pointer (module local defined).
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL17
 \DESCRIPTION Call CanHL_SortlogTxObjHandlePrioList() with parameter logTxObjHandlePrioList as pointer (module local defined).
 \COUNTERMEASURE \N [CM_CAN_HL19]

\ID SBSW_CAN_HL18
 \DESCRIPTION Write access to CanRxQueueBuffer with queue write index. Write index is checked against size of CanRxQueueBuffer.
 \COUNTERMEASURE \R [CM_CAN_HL24]

\ID SBSW_CAN_HL19
 \DESCRIPTION Write access to CanRxQueueBuffer.data with queue write index and data index. Write index is checked against size of CanRxQueueBuffer. (see also generator CM_CAN_HL04 qualified use case CSL02)
 \COUNTERMEASURE \R [CM_CAN_HL24]
                 \R [CM_CAN_HL17]

\ID SBSW_CAN_HL20
 \DESCRIPTION Write access to CanMirrorModeState with controller as index. Controller is checked against size of CanMirrorModeState.
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL21
 \DESCRIPTION Write access to logTxObjHandlePrioList with muxTx as index. Index is checked against size of logTxObjHandlePrioList.
 \COUNTERMEASURE \R [CM_CAN_HL19]

\ID SBSW_CAN_HL22
 \DESCRIPTION Write access to CanTxBasicDataBuffer with controller, muxTx and dataByte as index. Controller Index is checked against size of CanTxBasicDataBuffer (first dimension). Second dimension is statically defined CAN_MULTIPLEXED_TX_MAX and given as user type in ComStackLib. Third dimension is checked against size of table.
 \COUNTERMEASURE \R [CM_CAN_HL01]
                 \R [CM_CAN_HL20]

\ID SBSW_CAN_HL23
 \DESCRIPTION Write access to CanActiveSendPduInfo with logTxObjHandle as index. LogTxObjHandle is checked against size of CanActiveSendPduInfo.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL24
 \DESCRIPTION Call Appl_GenericPreTransmit() with parameter PduInfoPtr_var pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL25
 \DESCRIPTION Call CanIf_CancelTxConfirmation() with parameter CanActiveSendPduInfo[logTxObjHandle] as pointer (module local defined).
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL26
 \DESCRIPTION Call CanHL_RxBasicMsgReceivedBegin(), CanHL_RxBasicMsgReceivedSearch(), CanHL_RxMsgReceivedNotification() and CanHL_RxBasicMsgReceivedEnd() with parameter rxBasicInfo pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL27
 \DESCRIPTION Call CanHL_RxFullMsgReceivedBegin(), CanHL_RxFullMsgReceivedSearch(), CanHL_RxFullReceivedNotification() and CanHL_RxFullMsgReceivedEnd() with parameter rxFullInfo pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL28
 \DESCRIPTION Call CanHL_WritePrepare(), CanHL_WriteStart() and CanHL_WritePrepareCancelTx() with parameter PduInfo pointer secured by caller part of Can_Write() description.
 \COUNTERMEASURE \N Caller ensures validity of pointer parameter.

\ID SBSW_CAN_HL29
 \DESCRIPTION Write access to rxBasicInfo (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL30
 \DESCRIPTION Write access to rxFullInfo (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL31
 \DESCRIPTION Call Initialization subroutines with initInfo pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable)

\ID SBSW_CAN_HL32
 \DESCRIPTION Call Appl_GenericConfirmation with canPdu pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL34
 \DESCRIPTION Write access to CanMailboxState with mailbox handle as index. Index secured by ComStackLib [CM_CAN_HL29] and [CM_CAN_HL30] and checked against size of CanMailboxState.
 \COUNTERMEASURE \R [CM_CAN_HL28]

\ID SBSW_CAN_HL35
 \DESCRIPTION Write access to CAN cell register.
 \COUNTERMEASURE \N Caller ensures validity of pointer parameter.

\ID SBSW_CAN_HL36
 \DESCRIPTION Write access to initInfo structure as pointer (local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL37
 \DESCRIPTION Call GetElapsedValue with timer value pointers as parameter (module local defined).
 \COUNTERMEASURE \N The timer values are static and always valid.

\ID SBSW_CAN_HL38
 \DESCRIPTION Call GetCounterValue with timer value pointers as parameter. definition of kCanLoopXXX as index in LoopTimeout table is always smaller than the dimension of the table given as static user type in ComStackLib.
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL39
 \DESCRIPTION Undefine preprocessor switch to stimulate user defined feature (only test purpose).
 \COUNTERMEASURE \N Redefinement activates a none documented feature.

\ID SBSW_CAN_HL40
 \DESCRIPTION Write access to CanLogStatus with controller as index. Controller is secured by qualified use-case CSL03 of ComStackLib.
 \COUNTERMEASURE \N [CM_CAN_HL02]

\ID SBSW_CAN_HL41
 \DESCRIPTION Write access to CanActiveSendState with logTxObjHandle as index. logTxObjHandle is temporary stored in a list of [1,3] elements where each element is initialized to a valid value and always written with size check before.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_LL01
 \DESCRIPTION Write access to Can_GetgCanController(channel).MsgObj[txObjHandle].
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL02
 \DESCRIPTION Write access to Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL03
 \DESCRIPTION Write access to Can_GetRxMsg(channel).
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL04
 \DESCRIPTION Write access to Can_GetgCanController(channel).MsgObj[rxObjHandle].
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL05
 \DESCRIPTION Write access to Can_GetgCanController(channel).MsgObj[rxObjHandle].mMsg.DataFld.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL06
 \DESCRIPTION Write access to CAN_HL_INFO_RXBASIC_PTR(overrunOccur).
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL07
 \DESCRIPTION Write access to CAN_HL_INFO_RXFULL_PTR(overrunOccur).
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL08
 \DESCRIPTION Write access to Can_GetgCanController(channel).
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL09
 \DESCRIPTION Call CanCell_SendMessage() with parameter rxFullInfo Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld pointer.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL10
 \DESCRIPTION Write access to CAN_HL_INFO_INIT_PTR.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL11
 \DESCRIPTION Call CanLL_RxIndicationMsgCopy() with parameter data pointer.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL12
 \DESCRIPTION write access to localInterruptOldFlagPtr data pointer.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL13
 \DESCRIPTION Call CanCell_InitChannel() that call some CANoeAPIs with pointer parameter.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL14
 \DESCRIPTION Call CanHL_SetValue() that modify the CAN cell registers.
 \COUNTERMEASURE \N [CM_CAN_LL01]

SBSW_JUSTIFICATION_END */

/* Counter measurement description

\CM_CAN_HL01 A runtime check ensures controller index is always smaller than number of elements in arrays with controller amount as dimension (Qualified use-case CSL01 of ComStackLib size of ControllerData).
\CM_CAN_HL02 Qualified use-case CSL03 of ComStackLib (CanMailbox.ControllerConfigIdx is always in range of controller ControllerData)
\CM_CAN_HL03 A runtime check ensures logObjHandle is in valid range for Can_ActiveSendObject table (Qualified use-case CSL01 of ComStackLib - Can_ActiveSendObject size is extended by CAN_MULTIPLEXED_TX_MAX within generator to allow an access to logObjHandle+CAN_MULTIPLEXED_TX_MAX).
\CM_CAN_HL04 Qualified use-case CSL02 of ComStackLib (size of CanTxBasicDataBuffer == size of ControllerData).
\CM_CAN_HL17 A runtime check ensures DLC index is always smaller than the dimension of CanRxQueueBuffer.data.
\CM_CAN_HL18 A runtime check ensures HRH values is always in range of start stop index of the given mailbox type. (Qualified use-case CSL03 of ComStackLib - start-stop index fit to mailbox entrys)
\CM_CAN_HL19 Static defined size of logTxObjHandlePrioList (muxTx limited to 1 or 3 by preprocessor define), or pointer to this static variable used.
\CM_CAN_HL20 A runtime check ensures size of CanTxBasicDataBuffer (dataByte). For CAN-FD it is checked against the size of the table. For none CAN-FD size is checked against 8 as defined.
\CM_CAN_HL22 A runtime check ensures source index for LoopTimeout array is always smaller than the dimension of LoopTimeout.
\CM_CAN_HL24 A runtime check ensures RxQueue write index is always smaller than the dimension of RxQueue (Qualified use-case CSL01 of ComStackLib).
\CM_CAN_HL25 A runtime check ensures RxQueue read index is always smaller than the dimension of RxQueue (Qualified use-case CSL01 of ComStackLib).
\CM_CAN_HL28 A runtime check ensures htrh index is always smaller than the dimension of CanMailboxState (Qualified use-case CSL01 of ComStackLib).
\CM_CAN_HL29 Qualified use-case CSL02 of ComStackLib (size of CanMailboxState == size of CanMailbox) 
\CM_CAN_HL30 Qualified use-case CSL03 of ComStackLib (CanMailbox - CanController Start/Stop HtrhIndex).

\CM_CAN_LL01 CANoeEmu is not Silent tested.

*/

/****************************************************************************/
/*  Coverage Justification                                                  */
/****************************************************************************/
/* START_COVERAGE_JUSTIFICATION

\ID COV_CAN_MISRA
  \ACCEPT XF
  \ACCEPT TX
  \REASON only used for MISRA analysis not active in production code.

\ID COV_CAN_COMPATIBILITY
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TX xf xf
  \ACCEPT XF tf tf
  \REASON The condition checks whether a switch is defined and conditionally assigns a default value. This is used to ensure compatibility to older AUTOSAR/Component versions. 

\ID COV_CAN_INTERNAL
  \ACCEPT XF
  \ACCEPT TX
  \REASON This switch is always active within CAN driver context. Secure visibility of internal interface and cannot be stimulated to off while test. The code is verified by code inspection.

\ID COV_CAN_USER_CONFIG
  \ACCEPT TX
  \REASON This switch is used to ensure compatibility to none Vector stacks. It will be deactivated on demand in a special integration SIP. The code is verified by code inspection.

\ID COV_CAN_CANCEL_SW_STIMULATION
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TF xf tf
  \REASON It is not possible to stimulate a sw cancellation while the message is already start to send when hw cancellation is active. The code is verified by code inspection.

\ID COV_CAN_MICROSAR_VERSION
  \ACCEPT TX
  \ACCEPT TX tx tx
  \ACCEPT TX tx xf
  \ACCEPT TX xf tx
  \ACCEPT TX xf xf
  \ACCEPT TX tx tf
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT TF tx tf
  \ACCEPT TF xf tf
  \ACCEPT TF tf tx
  \REASON The MicroSar version is not changeable in a project. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_MULTI_ECU_CONFIG
  \ACCEPT XF
  \REASON The feature Multiple ECU Configurations is not supported for MSR403 but will be used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_AMD_RUNTIME_MEASUREMENT
  \ACCEPT XF
  \REASON The feature AMD Runtime Measurement is not supported for SafeBSW but will be used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_DEV_TIMEOUT_DETECT
  \ACCEPT XF
  \REASON The feature Develop Timeout Detect is not supported for SafeBSW but will be used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_BUSOFF_SUPPORT_API
  \ACCEPT TX
  \ACCEPT XF
  \REASON The feature Busoff Support API is not supported for SafeBSW but will be used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_EXTENDED_STATE_MACHINE
  \ACCEPT XF
  \REASON The feature Extended State Machine is not supported for SafeBSW but will be used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_DYNAMIC_FULLCAN_ID
  \ACCEPT TX
  \ACCEPT XF
  \REASON The feature Dynamic FullCAN ID is always supported for SafeBSW but may be not used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_FINALMAGICNUMBER
  \ACCEPT TX
  \REASON Switch is always true is OK here no side effect will occur when switch is false verified by review.

\ID COV_CAN_SAFE_BSW_EXCLUSION
  \ACCEPT XF xf tx xf xf tx xf xf xf xf xf xf tx xf tx
  \REASON SafeBsw restricted feature will not be enforced to be deactivated or activated.

\ID COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG
  \ACCEPT X
  \ACCEPT XF
  \REASON BUSOFF not stimulated in each configruation. The behavior does not depend on each configuration variant but TCASE-274 takes long runtime and will not be executed for each configuration.   

\ID COV_CAN_WAKEUP_CHANNEL_NOT_IN_ALL_CONFIG
  \ACCEPT XF
  \ACCEPT TX
  \REASON Wakeup may be always activated or deactivated depend on configuration. Not all configurations contain activated or deactivated wakeup for at least one controller, but is sufficiantly tested in some configuations with both settings.

---------------- LL ------------------

\ID COV_CAN_LL_MODE_CHANGE
  \ACCEPT TX
  \ACCEPT XF
  \REASON CANoeEmu only emulated hardware loops for test purpose. CANoeEmu change states ASAP.

\ID COV_CAN_LL_HIGHVOLTAGE
  \ACCEPT XF
  \REASON Currently explicit not tested in CANoeEmu. Special hardware not available.

\ID COV_CAN_LL_RECEIVE_LOOP
  \ACCEPT XF
  \REASON CANoeEmu only emulated receive loops. No real inconsistency will occur.

\ID COV_CAN_LL_EMULATION
  \ACCEPT TX
  \ACCEPT TX tx tx
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT TF tf xf
  \ACCEPT TF tx tf
  \ACCEPT TF tf tx
  \ACCEPT TF tf tx tf
  \ACCEPT TF tf tx tf tf
  \REASON CANoeEmu only. no Safe requirements.

\ID COV_CAN_LL_ISR_USAGE
  \ACCEPT X
  \ACCEPT TF
  \ACCEPT TX
  \ACCEPT XF
  \REASON not all ISRs used.

\ID COV_CAN_LL_OS_USAGE
  \ACCEPT TF
  \ACCEPT XF
  \REASON not all ISRs used.

\ID COV_CAN_GEN_HW_START_STOP_IDX
  \ACCEPT TX
  \REASON The feature GEN_HW_START_STOP is always supported for SafeBSW but may be not used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_PARTIAL_NETWORK
  \ACCEPT XF
  \REASON The feature Partial Network is not supported for SafeBSW but be used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_MULTIPLE_BASICCAN_TX
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_SLEEP_SUPPORT
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_WAKEUP_SUPPORT
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_EMULATE_SLEEP
  \ACCEPT TX
  \ACCEPT TX tf tx
  \ACCEPT TX tx tx
  \ACCEPT TX xf tx
  \ACCEPT XF
  \REASON The feature Emulated Sleep is always supported for SafeBSW but used in MSR3. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_TX_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_BASICCAN_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_FULLCAN_POLLING
  \ACCEPT TF
  \REASON The feature configuration specific - fully supported.

\ID COV_CAN_ERROR_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_WAKEUP_POLLING
  \ACCEPT XF
  \REASON The feature is platform specific - always off.

\ID COV_CAN_INDIVIDUAL_POLLING
  \ACCEPT TF
  \REASON The feature is platform and Project specific.

\ID COV_CAN_TX_POLLING_OR_CANCEL_IN_HW_TASK
  \ACCEPT TF tf tf tx
  \ACCEPT TF tf tf xf
  \REASON The feature is platform specific - C_HL_ENABLE_CANCEL_IN_HW_TASK is always true or false.

\ID COV_CAN_GENERIC_CONFIRMATION_API2
  \ACCEPT TF
  \REASON The feature is platform and Project specific - fully supported.

\ID COV_CAN_FD_SUPPORT
  \ACCEPT XF
  \REASON The feature is platform and Project specific - always off.

\ID COV_CAN_MULTIPLE_BASICCAN
  \ACCEPT TF
  \REASON The feature is platform and Project specific - fully supported.

\ID COV_CAN_TX_FULLCAN_OBJECTS
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_FULLCAN_OBJECTS
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HW_TRANSMIT_CANCELLATION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_CANCEL_IN_HW
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HARDWARE_CANCELLATION
  \ACCEPT XF
  \ACCEPT TF
  \REASON The feature is platform specific - always off.

\ID COV_CAN_EXTENDED_ID
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_MIXED_ID
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_SECURE_TEMP_BUFFER
  \ACCEPT XF
  \REASON The feature is platform specific - always off.

\ID COV_CAN_COMMON_CAN
  \ACCEPT XF
  \REASON The feature is platform specific - always off.

\ID COV_CAN_OVERRUN_NOTIFICATION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_MULTIPLEXED_TRANSMISSION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_QUEUE
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_TXINDIRECTIONHWTOLOG
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_HW_LAYOUT_TXBASIC_FIRST
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_TX_FULLCAN_HWHANDLE_REVERSE
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_RX_FULLCAN_HWHANDLE_REVERSE
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_BASICCAN_HWHANDLE_REVERSE
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_EXTENDED_STATUS
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_OVERRUN_IN_STATUS
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_RxBasicMsgReceivedBegin
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_RxFullMsgReceivedBegin
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_HW_EXIT_TRANSMIT
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_HARDWARE_FAILURE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT XF tf xf
  \ACCEPT TF tf xf
  \ACCEPT TF tf tx
  \REASON The condition checks for hardware failures that cannot be stimulated. The code is verified by code inspection.

\ID COV_CAN_GENDATA_FAILURE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT TF tf xf
  \ACCEPT TF tf xf tf
  \REASON The condition checks for generated data failures that cannot be simulated. The code is verified by code inspection.

\ID COV_CAN_GENDATA_NOT_IN_ALL_CONFIG
  \ACCEPT TX
  \ACCEPT XF
  \REASON Generated data does not contain all data sets but checked in code. Not encapsulated with preprocessor switch to simplify code.  

\ID COV_CAN_OS_USAGE
  \ACCEPT TX
  \ACCEPT TX xf tx
  \ACCEPT TX tx xf
  \ACCEPT XF
  \ACCEPT XF xf tx
  \REASON Os may be used in component test or not. The code is verified by code inspection.

\ID COV_CAN_LOCK_ISR_BY_APPL
  \ACCEPT TX tf tf
  \ACCEPT TF xf tf 
  \REASON CAN interrupts never locked by application only within tests. It is not part of the component test to check application code. The code is verified by code inspection.

\ID COV_CAN_RAM_CHECK
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RAM_CHECK_EXTENDED
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_TIMEOUT_DURATION
  \ACCEPT TF
  \REASON Hardware loop time out is platform specific never reached.

\ID COV_CAN_TRANSITION_REQUEST_START
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TF tf tf
  \REASON Mode transition is platform specific always true or false.

\ID COV_CAN_TRANSITION_REQUEST_STOP
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TF tf tf
  \REASON Mode transition is platform specific always true or false.

\ID COV_CAN_TRANSITION_REQUEST_WAKEUP
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TF tf tf
  \REASON Mode transition is platform specific always true or false.

\ID COV_CAN_TRANSITION_REQUEST_SLEEP
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TF tf tf
  \REASON Mode transition is platform specific always true or false.

\ID COV_CAN_BASICCAN_SEARCH_LINEAR
  \ACCEPT TX
  \ACCEPT TF tx tf
  \ACCEPT TX tx tx
  \REASON Received BasicCAN mailbox is always a valid object and found while search.

\ID COV_CAN_FULLCAN_SEARCH_LINEAR
  \ACCEPT TX
  \ACCEPT TF tx tf
  \REASON Received FullCAN mailbox is always a valid object and found while search.

\ID COV_CAN_RAM_CHECK_NO_STIMULATION
  \ACCEPT X
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT XF xf xf
  \ACCEPT TF tf xf
  \REASON some configurations do not stimulate RamCheck by test suite, to check the standard CAN driver behaviour.

\ID COV_CAN_SAFE_BSW
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HWOBJINDIVPOLLING
  \ACCEPT TX tf tx
  \ACCEPT TX
  \REASON usage of individual hardware object polling is platform specific always true or false.

END_COVERAGE_JUSTIFICATION */ 

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 677 */

