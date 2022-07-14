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

#if !defined(CAN_LOCAL_H)
#define  CAN_LOCAL_H

/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/
#include "Can.h"         /* include also all needed types */
/* \trace SPEC-1583 */
#include "SchM_Can.h"

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_COREVERSION           != 0x0507u) /* \trace SPEC-1699, SPEC-3837 */
# error "Header file are inconsistent!"
#endif
#if (CAN_RELEASE_COREVERSION   != 0x01u)
# error "Header file are inconsistent!"
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

/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/
#define C_ENABLE_COPY_TX_DATA

#if defined(C_ENABLE_MIXED_ID) || defined(C_ENABLE_EXTENDED_ID)
# define CAN_ID_IDE_MASK            0x80000000UL
# define CAN_ID_FD_MASK             0x40000000UL /* \trace SPEC-50553 */
# define CAN_ID_MASK                0x1FFFFFFFUL
# define CAN_ID_MASK_STD            0x000007FFUL
# define CAN_ID_UNUSED_MASK         0x20000000UL
#else
# define CAN_ID_IDE_MASK            0x0000U
# define CAN_ID_FD_MASK             0x4000U /* \trace SPEC-50553 */
# define CAN_ID_MASK                0x07FFU
# define CAN_ID_MASK_STD            0x07FFU
# define CAN_ID_UNUSED_MASK         0x3800U
#endif
#define kCanAllChannels             ((CanChannelHandle)0xFFU)

#if !defined(C_DRV_INTERNAL) /* COV_CAN_INTERNAL */
# error "Can_Local.h should only be included by Can.c"
#endif
#if defined(kCanNumberOfChannels) /* COV_CAN_COMPATIBILITY */
# undef kCanNumberOfChannels /* may happen for MultiDirverInstance */ /* PRQA S 0841 */ /* MD_Can_0841 */
#endif
/* real amount of controllers in this system (LT and PB) */
#define kCanNumberOfChannels Can_GetControllerMax()
/* if kCanNumberOfChannels used to declare a table/dimension this table has to be generated to be also Link-Time-capable */
/*  -> see ESCAN00028069 controller-amount has to be Link-Time-capable */
#if defined(kCanNumberOfHwChannels) /* COV_CAN_COMPATIBILITY */
# undef kCanNumberOfHwChannels /* may happen for MultiDirverInstance */ /* PRQA S 0841 */ /* MD_Can_0841 */
#endif
/* compatibility to common-can drivers */
/* real amount of controllers in this system (LT and PB) */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COM_STACK_LIB */ /* COV_CAN_MICROSAR_VERSION */
#  define kCanNumberOfHwChannels Can_GetSizeOfLogStatus()
#else
# if ( CAN_COMMON_CAN == STD_ON )
#  define kCanNumberOfHwChannels CAN_GLOBAL_CONFIG(CanHwChannelMax)
# else
#  define kCanNumberOfHwChannels CAN_GLOBAL_CONFIG(CanControllerMax)
# endif
#endif

#if defined( C_SINGLE_RECEIVE_CHANNEL )
/* have to be done here to avoid declaration of "channel" in Can.h */
# define channel                                ((CanChannelHandle)0)
# define canHwChannel                           ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STARTINDEX(ch)       ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STOPINDEX(ch)        ((CanChannelHandle)0)
#else
# define channel                                Controller
#  define canHwChannel                          Controller
#  define CAN_HL_HW_CHANNEL_STARTINDEX(ch)      (ch)
#  define CAN_HL_HW_CHANNEL_STOPINDEX(ch)       (ch)
#endif

#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
# if !defined(CAN_EMULATE_SLEEP) /* COV_CAN_COMPATIBILITY */
#  define CAN_EMULATE_SLEEP STD_ON /* always on for AutoSar4 */
# endif
# if !defined(CAN_TRANSITION_CHECK) /* COV_CAN_COMPATIBILITY */
#  define CAN_TRANSITION_CHECK STD_ON /* default may be deactivated by user configuration */
# endif
#else
# if !defined(CAN_EMULATE_SLEEP) /* COV_CAN_COMPATIBILITY */
#  define CAN_EMULATE_SLEEP STD_OFF
# endif
# if !defined(CAN_TRANSITION_CHECK) /* COV_CAN_COMPATIBILITY */
#  define CAN_TRANSITION_CHECK STD_OFF
# endif
#endif
#if !defined(CAN_REINIT_START) /* COV_CAN_COMPATIBILITY */
# define CAN_REINIT_START STD_OFF
#endif

#if !defined(CAN_OS_TICK2MS) /* COV_CAN_COMPATIBILITY */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
#  error "CAN_OS_TICK2MS() should be defined to OS_TICKS2MS_<counterShortName>() macro from OS"
# endif
# define CAN_OS_TICK2MS(tick) (tick) /* used direct by 401 implementation */
#endif
#if !defined(CAN_OS_COUNTER_ID) /* COV_CAN_COMPATIBILITY */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
#  error "CAN_OS_COUNTER_ID should be defined to 'counterShortName' from OS"
# endif
# define CAN_OS_COUNTER_ID CanTimeoutDurationCounterId /* used direct by 401 implementation */
#endif

#if !defined(CAN_RX_BASICCAN_TYPE) /* compatibility Cfg5 to GENy */ /* COV_CAN_COMPATIBILITY */
# define CAN_RX_BASICCAN_TYPE    RxBasicCANType
# define CAN_RX_FULLCAN_TYPE     RxFullCANType
# define CAN_TX_BASICCAN_TYPE    TxBasicCANType
# define CAN_TX_FULLCAN_TYPE     TxFullCANType
# define CAN_UNUSED_CAN_TYPE     UnusedCANType
#endif

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/
/* for variable Can_ActiveSendState */
#define CAN_FREE_OBJ                (uint8)0xFF     /* mark objects as free */
#define kCanBufferFree              CAN_FREE_OBJ    /* compatibility for cancel in HW task */
#define CAN_CANCEL_OBJ              (uint8)0xFE     /* mark object as currently cancelling */
#define kCanBufferCancel            CAN_CANCEL_OBJ  /* compatibility for LL cancel interrupt / task */
#define CAN_CANCEL_OBJ_SW           (uint8)0xFD
#define CAN_SEND_OBJ                (uint8)0x01     /* occupied send object */

#if defined(C_ENABLE_CAN_RAM_CHECK)
/* for variable Can_MailboxState */
# define CAN_ENABLE_OBJ             (uint8)0x00     /* enabled mailbox */
# define CAN_DISABLED_OBJ           (uint8)0x01     /* disabled mailbox */
#endif

/* general return values */
#define kCanFailed                  CAN_NOT_OK
#define kCanOk                      CAN_OK
#define kCanNotSupported            CAN_NOT_SUPPORTED
#define kCanRequested               CAN_REQUESTED

#define kCanTrue                    0x01
#define kCanFalse                   0x00

#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COM_STACK_LIB */ /* COV_CAN_MICROSAR_VERSION */
# define Can_GetControllerMax()                         Can_GetSizeOfControllerData()

# define Can_GetCanControllerDefaultBaudrate(Index)     Can_GetCanControllerDefaultBaudrateOfControllerConfig(Index)
# define Can_GetCanControllerDefaultBaudrateIdx(Index)  Can_GetCanControllerDefaultBaudrateIdxOfControllerConfig(Index)
# if !defined(Can_GetMailboxRxBasicLengthOfControllerConfig) /* If no Object is available ComStackLib do not generate start-stop index */
#  define Can_GetMailboxRxBasicLengthOfControllerConfig(Index) 0
#  define Can_GetMailboxRxBasicStartIdxOfControllerConfig(Index) 0
#  define Can_GetMailboxRxBasicEndIdxOfControllerConfig(Index) 0
# endif
# define Can_GetRxBasicHandleMax(Index)                 Can_GetMailboxRxBasicLengthOfControllerConfig(Index)
# define Can_GetRxBasicHandleStart(Index)               (Can_HwHandleType)Can_GetMailboxRxBasicStartIdxOfControllerConfig(Index)
# define Can_GetRxBasicHandleStop(Index)                (Can_HwHandleType)Can_GetMailboxRxBasicEndIdxOfControllerConfig(Index)
# define Can_GetRxBasicHwStart(Index)                   Can_GetRxBasicHwStartOfControllerConfig(Index)
# define Can_GetRxBasicHwStop(Index)                    Can_GetRxBasicHwStopOfControllerConfig(Index)
# if !defined(Can_GetMailboxRxFullLengthOfControllerConfig)
#  define Can_GetMailboxRxFullLengthOfControllerConfig(Index) 0
#  define Can_GetMailboxRxFullStartIdxOfControllerConfig(Index) 0
#  define Can_GetMailboxRxFullEndIdxOfControllerConfig(Index) 0
# endif
# define Can_GetRxFullHandleMax(Index)                  Can_GetMailboxRxFullLengthOfControllerConfig(Index)
# define Can_GetRxFullHandleStart(Index)                (Can_HwHandleType)Can_GetMailboxRxFullStartIdxOfControllerConfig(Index)
# define Can_GetRxFullHandleStop(Index)                 (Can_HwHandleType)Can_GetMailboxRxFullEndIdxOfControllerConfig(Index)
# define Can_GetRxFullHwStart(Index)                    Can_GetRxFullHwStartOfControllerConfig(Index)
# define Can_GetRxFullHwStop(Index)                     Can_GetRxFullHwStopOfControllerConfig(Index)
# if !defined(Can_GetMailboxTxBasicLengthOfControllerConfig) /* COV_CAN_GENDATA_FAILURE */
#  define Can_GetMailboxTxBasicLengthOfControllerConfig(Index) 0
#  define Can_GetMailboxTxBasicStartIdxOfControllerConfig(Index) 0
#  define Can_GetMailboxTxBasicEndIdxOfControllerConfig(Index) 0
# endif
# define Can_GetTxBasicHandleMax(Index)                 Can_GetMailboxTxBasicLengthOfControllerConfig(Index)
# define Can_GetTxBasicHandleStart(Index)               (Can_HwHandleType)Can_GetMailboxTxBasicStartIdxOfControllerConfig(Index)
# define Can_GetTxBasicHandleStop(Index)                (Can_HwHandleType)Can_GetMailboxTxBasicEndIdxOfControllerConfig(Index)
# define Can_GetTxBasicHwStart(Index)                   Can_GetTxBasicHwStartOfControllerConfig(Index)
# define Can_GetTxBasicHwStop(Index)                    Can_GetTxBasicHwStopOfControllerConfig(Index)
# if !defined(Can_GetMailboxTxFullLengthOfControllerConfig)
#  define Can_GetMailboxTxFullLengthOfControllerConfig(Index) 0
#  define Can_GetMailboxTxFullStartIdxOfControllerConfig(Index) 0
#  define Can_GetMailboxTxFullEndIdxOfControllerConfig(Index) 0
# endif
# define Can_GetTxFullHandleMax(Index)                  Can_GetMailboxTxFullLengthOfControllerConfig(Index)
# define Can_GetTxFullHandleStart(Index)                (Can_HwHandleType)Can_GetMailboxTxFullStartIdxOfControllerConfig(Index)
# define Can_GetTxFullHandleStop(Index)                 (Can_HwHandleType)Can_GetMailboxTxFullEndIdxOfControllerConfig(Index)
# define Can_GetTxFullHwStart(Index)                    Can_GetTxFullHwStartOfControllerConfig(Index)
# define Can_GetTxFullHwStop(Index)                     Can_GetTxFullHwStopOfControllerConfig(Index)
# if !defined(Can_GetMailboxUnusedLengthOfControllerConfig)  /* COV_CAN_GENDATA_FAILURE */
#  define Can_GetMailboxUnusedLengthOfControllerConfig(Index) 0
#  define Can_GetMailboxUnusedStartIdxOfControllerConfig(Index) 0
#  define Can_GetMailboxUnusedEndIdxOfControllerConfig(Index) 0
# endif
# define Can_GetUnusedHandleMax(Index)                  Can_GetMailboxUnusedLengthOfControllerConfig(Index)
# define Can_GetUnusedHandleStart(Index)                (Can_HwHandleType)Can_GetMailboxUnusedStartIdxOfControllerConfig(Index)
# define Can_GetUnusedHandleStop(Index)                 (Can_HwHandleType)Can_GetMailboxUnusedEndIdxOfControllerConfig(Index)
# define Can_GetUnusedHwStart(Index)                    Can_GetUnusedHwStartOfControllerConfig(Index)
# define Can_GetUnusedHwStop(Index)                     Can_GetUnusedHwStopOfControllerConfig(Index)

# define Can_GetDLC_FIFO(Index)                         Can_GetDLC_FIFOOfMailbox(Index)
# define Can_GetHwHandle(Index)                         Can_GetHwHandleOfMailbox(Index)
# define Can_GetIDValue(Index)                          Can_GetIDValueOfMailbox(Index)
# define Can_GetMailboxType(Index)                      Can_GetMailboxTypeOfMailbox(Index)
# define Can_GetMailboxDataLen(Index)                   Can_GetMaxDataLenOfMailbox(Index)
# define Can_GetMailboxFdPadding(Index)                 Can_GetFdPaddingOfMailbox(Index)
# define Can_GetMailboxController(Index)                (CanChannelHandle)Can_GetControllerConfigIdxOfMailbox(Index)

# if defined(CAN_HWOBJINDIVPOLLINGSTART)
#  define Can_GetCanHwObjIndivPolling(ch,Index)          Can_GetHwObjIndivPolling((Can_GetHwObjIndivPollingStart(ch))+(Index))
# else
#  define Can_GetCanHwObjIndivPolling(ch,Index)          Can_GetHwObjIndivPolling((Index))
# endif

# define Can_GetCanToCanIfChannelMapping(Index)          Can_GetCanIfChannelId((Index))

/* VAR */
# define Can_GetLastInitObject(Index)                   Can_GetLastInitObjectOfControllerData(Index)
# define Can_SetLastInitObject(Index,Value)             Can_SetLastInitObjectOfControllerData(Index,Value)
# define Can_GetCanInterruptOldStatus(Index)            Can_GetCanInterruptOldStatusOfControllerData(Index)
# define Can_SetCanInterruptOldStatus(Index,Value)      Can_SetCanInterruptOldStatusOfControllerData(Index,Value)
# define Can_GetCanInterruptCounter(Index)              Can_GetCanInterruptCounterOfControllerData(Index)
# define Can_SetCanInterruptCounter(Index,Value)        Can_SetCanInterruptCounterOfControllerData(Index,Value)
# define Can_IsIsWakeup(Index)                          Can_IsIsWakeupOfControllerData(Index)
# define Can_SetIsWakeup(Index,Value)                   Can_SetIsWakeupOfControllerData(Index,Value)
# define Can_IsIsBusOff(Index)                          Can_IsIsBusOffOfControllerData(Index)
# define Can_SetIsBusOff(Index,Value)                   Can_SetIsBusOffOfControllerData(Index,Value)
# define Can_GetStatusReq(Index)                        Can_GetStatusReqOfControllerData(Index)
# define Can_SetStatusReq(Index,Value)                  Can_SetStatusReqOfControllerData(Index,Value)
# define Can_GetLogStatus(Index)                        Can_GetLogStatusOfControllerData(Index)
# define Can_SetLogStatus(Index,Value)                  Can_SetLogStatusOfControllerData(Index,Value)
# define Can_GetCanLoopTimeout(ch, Index)               (Can_GetLoopTimeoutOfControllerData(ch))[(Index)]
# define Can_IsIsHardwareCanceled(Index)                Can_IsIsHardwareCanceledOfControllerData(Index)
# define Can_SetIsHardwareCanceled(Index,Value)         Can_SetIsHardwareCanceledOfControllerData(Index,Value)
# define Can_GetMirrorModeState(Index)                  Can_GetMirrorModeStateOfControllerData(Index)
# define Can_SetMirrorModeState(Index,Value)            Can_SetMirrorModeStateOfControllerData(Index,Value)
# define Can_GetSilentModeState(Index)                  Can_GetSilentModeStateOfControllerData(Index)
# define Can_SetSilentModeState(Index,Value)            Can_SetSilentModeStateOfControllerData(Index,Value)

# define Can_GetCanTxBasicDataBuffer(ch,muxTx)          (Can_GetTxBasicDataBuffer(ch))[(muxTx)]

# define Can_GetActiveSendPdu(Index)                    Can_GetPduOfActiveSendObject(Index)
# define Can_SetActiveSendPdu(Index,Value)              Can_SetPduOfActiveSendObject(Index,Value)
# define Can_GetActiveSendState(Index)                  Can_GetStateOfActiveSendObject(Index)
# define Can_SetActiveSendState(Index,Value)            Can_SetStateOfActiveSendObject(Index,Value)
# define Can_GetActiveSendPduInfo(Index)                Can_GetPduInfoOfActiveSendObject(Index)
# define Can_GetActiveSendId(Index)                     (Can_GetPduInfoOfActiveSendObject(Index)).id
# define Can_SetActiveSendId(Index,Value)               (Can_GetPduInfoOfActiveSendObject(Index)).id = (Value)
# define Can_GetActiveSendLength(Index)                 (Can_GetPduInfoOfActiveSendObject(Index)).length
# define Can_SetActiveSendLength(Index,Value)           (Can_GetPduInfoOfActiveSendObject(Index)).length = (Value)
# define Can_GetActiveSendSdu(Index)                    (Can_GetPduInfoOfActiveSendObject(Index)).sdu
# define Can_SetActiveSendSdu(Index,Value)              (Can_GetPduInfoOfActiveSendObject(Index)).sdu = (Value)
# define Can_GetActiveSendSwPduHandle(Index)            (Can_GetPduInfoOfActiveSendObject(Index)).swPduHandle
# define Can_SetActiveSendSwPduHandle(Index,Value)      (Can_GetPduInfoOfActiveSendObject(Index)).swPduHandle = (Value)

#else /* without ComStackLib */

# if defined(CAN_ENABLE_SELECTABLE_PB)
/* \trace SPEC-1727 */
/* Pointer parameter of initialization function used (selectable Post-Build) */
#  define CAN_GLOBAL_CONFIG(x)                          Can_ConfigDataPtr->x      /* PRQA S 3410 */ /* MD_Can_3410 */
#  define CAN_CONTROLLER_CONFIG_INIT                    (Can_ConfigDataPtr->CanControllerConfig_TabPtr)
#  define CAN_CONTROLLER_CONFIG(hwch)                   (Can_ConfigDataPtr->CanControllerConfig_TabPtr[(hwch)])
#  define CAN_MAILBOX                                   (Can_ConfigDataPtr->CanMailbox_TabPtr)
#  define CAN_INIT_OBJ_START_IDX                        (Can_ConfigDataPtr->CanInitObjectStartIndex_TabPtr)
#  define CAN_INIT_OBJ_BAUDRATE                         (Can_ConfigDataPtr->CanInitObjectBaudrate_TabPtr)
#  define CAN_INIT_OBJ_FD_BRS_CONFIG                    (Can_ConfigDataPtr->CanInitObjectFdConfig_TabPtr)
#  define CanHwObjIndivPolling                          (Can_ConfigDataPtr->CanHwObjIndivPolling_TabPtr) /* PRQA S 3436 */ /* MD_Can_3436 */
#  define CAN_PARTIAL_NET                               (Can_ConfigDataPtr->CanPartialNetwork_TabPtr)
/* hardware specific starts here */
#define CanInitObject            (canConfigPtr->CanInitObjectPtr)      /* PRQA S 3436 */ /* MD_Can_3436_LL */
#define CanFilter                (canConfigPtr->CanFilterPtr)          /* PRQA S 3436 */ /* MD_Can_3436_LL */
#define CanFilterStartIdx        (canConfigPtr->CanFilterStartIdxPtr)  /* PRQA S 3436 */ /* MD_Can_3436_LL */
# else
/* \trace SPEC-1728 */
/* Pointer parameter of initialization function not used (no selectable Post-Build or Link-Time configuration) */
#  if defined(C_ENABLE_MULTI_ECU_CONFIG) /* COV_CAN_MULTI_ECU_CONFIG */
#   define CAN_GLOBAL_CONFIG(x)                         Can_ConfigDataPtr->x      /* PRQA S 3410 */ /* MD_Can_3410 */
#  else
#   define CAN_GLOBAL_CONFIG(x)                         CanConfig.x               /* PRQA S 3410 */ /* MD_Can_3410 */
#  endif
#  define CAN_CONTROLLER_CONFIG_INIT                    CanControllerConfig
#  define CAN_CONTROLLER_CONFIG(hwch)                   CanControllerConfig[(hwch)]
#  define CAN_MAILBOX                                   CanMailbox
#  define CAN_INIT_OBJ_START_IDX                        CanInitObjectStartIndex
#  define CAN_INIT_OBJ_BAUDRATE                         CanInitObjectBaudrate
#  define CAN_INIT_OBJ_FD_BRS_CONFIG                    CanInitObjectFdConfig
#  define CAN_PARTIAL_NET                               CanPartialNetwork
# endif

/* CONST PB */
# define Can_GetBaseDll_GeneratorVersion()              CAN_GLOBAL_CONFIG(Can_BaseDll_GeneratorVersion)
# define Can_GetPlatformDll_GeneratorVersion()          CAN_GLOBAL_CONFIG(Can_PlatformDll_GeneratorVersion)
# define Can_GetPrecompileCRC()                         CAN_GLOBAL_CONFIG(Can_ePrecompileCRC)
# define Can_GetLinktimeCRC()                           CAN_GLOBAL_CONFIG(Can_eLinktimeCRC)
# define Can_GetIdentitySelected()                      CAN_GLOBAL_CONFIG(IdentitySelected)
# define Can_GetControllerMax()                         CAN_GLOBAL_CONFIG(CanControllerMax)
# define Can_GetHwChannelMax()                          CAN_GLOBAL_CONFIG(CanHwChannelMax)
# define Can_GetMaxRxDataLen()                          CAN_GLOBAL_CONFIG(CanMaxRxDataLen)
# define Can_GetMaxTxDataLen()                          CAN_GLOBAL_CONFIG(CanMaxTxDataLen)
# define Can_GetFinalMagicNumber()                      CAN_GLOBAL_CONFIG(FinalMagicNumber)

# define Can_GetCanControllerDefaultBaudrate(Index)     CAN_CONTROLLER_CONFIG(Index).CanControllerDefaultBaudrate
# define Can_GetCanControllerDefaultBaudrateIdx(Index)  CAN_CONTROLLER_CONFIG(Index).CanControllerDefaultBaudrateIdx
# define Can_GetRxBasicHandleMax(Index)                 CAN_CONTROLLER_CONFIG(Index).RxBasicHandleMax
# define Can_GetRxBasicHandleStart(Index)               CAN_CONTROLLER_CONFIG(Index).RxBasicHandleStart
# define Can_GetRxBasicHandleStop(Index)                CAN_CONTROLLER_CONFIG(Index).RxBasicHandleStop
# define Can_GetRxBasicHwStart(Index)                   CAN_CONTROLLER_CONFIG(Index).RxBasicHwStart
# define Can_GetRxBasicHwStop(Index)                    CAN_CONTROLLER_CONFIG(Index).RxBasicHwStop
# define Can_GetRxFullHandleMax(Index)                  CAN_CONTROLLER_CONFIG(Index).RxFullHandleMax
# define Can_GetRxFullHandleStart(Index)                CAN_CONTROLLER_CONFIG(Index).RxFullHandleStart
# define Can_GetRxFullHandleStop(Index)                 CAN_CONTROLLER_CONFIG(Index).RxFullHandleStop
# define Can_GetRxFullHwStart(Index)                    CAN_CONTROLLER_CONFIG(Index).RxFullHwStart
# define Can_GetRxFullHwStop(Index)                     CAN_CONTROLLER_CONFIG(Index).RxFullHwStop
# define Can_GetTxBasicHandleMax(Index)                 CAN_CONTROLLER_CONFIG(Index).TxBasicHandleMax
# define Can_GetTxBasicHandleStart(Index)               CAN_CONTROLLER_CONFIG(Index).TxBasicHandleStart
# define Can_GetTxBasicHandleStop(Index)                CAN_CONTROLLER_CONFIG(Index).TxBasicHandleStop
# define Can_GetTxBasicHwStart(Index)                   CAN_CONTROLLER_CONFIG(Index).TxBasicHwStart
# define Can_GetTxBasicHwStop(Index)                    CAN_CONTROLLER_CONFIG(Index).TxBasicHwStop
# define Can_GetTxFullHandleMax(Index)                  CAN_CONTROLLER_CONFIG(Index).TxFullHandleMax
# define Can_GetTxFullHandleStart(Index)                CAN_CONTROLLER_CONFIG(Index).TxFullHandleStart
# define Can_GetTxFullHandleStop(Index)                 CAN_CONTROLLER_CONFIG(Index).TxFullHandleStop
# define Can_GetTxFullHwStart(Index)                    CAN_CONTROLLER_CONFIG(Index).TxFullHwStart
# define Can_GetTxFullHwStop(Index)                     CAN_CONTROLLER_CONFIG(Index).TxFullHwStop
# define Can_GetUnusedHandleMax(Index)                  CAN_CONTROLLER_CONFIG(Index).UnusedHandleMax
# define Can_GetUnusedHandleStart(Index)                CAN_CONTROLLER_CONFIG(Index).UnusedHandleStart
# define Can_GetUnusedHandleStop(Index)                 CAN_CONTROLLER_CONFIG(Index).UnusedHandleStop
# define Can_GetUnusedHwStart(Index)                    CAN_CONTROLLER_CONFIG(Index).UnusedHwStart
# define Can_GetUnusedHwStop(Index)                     CAN_CONTROLLER_CONFIG(Index).UnusedHwStop

# define Can_GetDLC_FIFO(Index)                         CAN_MAILBOX[(Index)].DLC_FIFO
# define Can_GetHwHandle(Index)                         CAN_MAILBOX[(Index)].HwHandle
# define Can_GetIDValue(Index)                          CAN_MAILBOX[(Index)].IDValue
# define Can_GetMailboxType(Index)                      CAN_MAILBOX[(Index)].MailboxType
# define Can_GetMailboxDataLen(Index)                   8 /* just for DET check against maximum value - FD-mode2 not supported for GENy */
# define Can_GetMailboxController(Index)                CAN_MAILBOX[(Index)].MsgController

# define Can_GetInitObjectStartIndex(Index)             CAN_INIT_OBJ_START_IDX[(Index)]
# define Can_GetInitObjectBaudrate(Index)               CAN_INIT_OBJ_BAUDRATE[(Index)]
# define Can_GetInitObjectFdBrsConfig(Index)            CAN_INIT_OBJ_FD_BRS_CONFIG[(Index)]

# define Can_GetPartialNetwork(Index)                   CAN_PARTIAL_NET[(Index)] /* only CAN_MSR3 */

# define Can_GetCanHwObjIndivPolling(ch,Index)          CanHwObjIndivPolling[ch][(Index)]

/* CONST */
# define Can_GetWakeupSourceRef(Index)                  CanWakeupSourceRef[(Index)]
# define Can_GetControllerBaudrateConfig(Index)         Can_ControllerBaudrateConfig[(Index)] /* only CAN_MSR40 */
# define Can_GetHwChannelStartIndex(Index)              CanHwChannelStartIndex[(Index)]
# define Can_GetHwChannelToChannel(Index)               CanHwChannelToChannel[(Index)]
# define Can_GetChannelIdentityAssignment(Index)        CanChannelIdentityAssignment[(Index)]
# define Can_GetCanToCanIfChannelMapping(Index)         (Index)
# define Can_GetTxOffsetHwToLog(Index)                  canTxOffsetHwToLog[(Index)]

/* VAR */
# define Can_GetSizeOfRxQueueBuffer()                   kCanRxQueueSize

# define Can_GetActiveSendPdu(Index)                    CanActiveSendPdu[(Index)]
# define Can_SetActiveSendPdu(Index,Value)              CanActiveSendPdu[(Index)] = (Value)
# define Can_GetActiveSendState(Index)                  CanActiveSendState[(Index)]
# define Can_SetActiveSendState(Index,Value)            CanActiveSendState[(Index)] = (Value)
# define Can_GetActiveSendPduInfo(Index)                CanActiveSendId[(Index)]
# define Can_GetActiveSendId(Index)                     CanActiveSendId[(Index)].id
# define Can_SetActiveSendId(Index,Value)               CanActiveSendId[(Index)].id = (Value)
# define Can_GetActiveSendLength(Index)                 CanActiveSendId[(Index)].length
# define Can_SetActiveSendLength(Index,Value)           CanActiveSendId[(Index)].length = (Value)
# define Can_GetActiveSendSdu(Index)                    CanActiveSendId[(Index)].sdu
# define Can_SetActiveSendSdu(Index,Value)              CanActiveSendId[(Index)].sdu = (Value)
# define Can_GetActiveSendSwPduHandle(Index)            CanActiveSendId[(Index)].swPduHandle
# define Can_SetActiveSendSwPduHandle(Index,Value)      CanActiveSendId[(Index)].swPduHandle = (Value)

# define Can_GetLastInitObject(Index)                   lastInitObject[(Index)]
# define Can_SetLastInitObject(Index,Value)             lastInitObject[(Index)] = (Value)
# define Can_GetCanInterruptOldStatus(Index)            canCanInterruptOldStatus[(Index)]
# define Can_SetCanInterruptOldStatus(Index,Value)      canCanInterruptOldStatus[(Index)] = (Value)
# define Can_GetCanInterruptCounter(Index)              canInterruptCounter[(Index)]
# define Can_SetCanInterruptCounter(Index,Value)        canInterruptCounter[(Index)] = (Value)
# define Can_IsIsWakeup(Index)                          (canIsWakeup[(Index)] != FALSE)
# define Can_SetIsWakeup(Index,Value)                   canIsWakeup[(Index)] = (Value)
# define Can_IsIsBusOff(Index)                          (canIsBusOff[(Index)] != FALSE)
# define Can_SetIsBusOff(Index,Value)                   canIsBusOff[(Index)] = (Value)
# define Can_GetStatusReq(Index)                        canStatusReq[(Index)] /* only CAN_MSR4 or RAM_CHECK */
# define Can_SetStatusReq(Index,Value)                  canStatusReq[(Index)] = (Value) /* only CAN_MSR4 or RAM_CHECK */
# define Can_GetLogStatus(Index)                        canStatus[(Index)]
# define Can_SetLogStatus(Index,Value)                  canStatus[(Index)] = (Value)
# define Can_GetCanLoopTimeout(ch, Index)               canLoopTimeout[(ch)][(Index)]
# define Can_IsIsHardwareCanceled(Index)                (canIsHardwareCanceled[(Index)] != FALSE)
# define Can_SetIsHardwareCanceled(Index,Value)         canIsHardwareCanceled[(Index)] = (Value)
# define Can_GetMirrorModeState(Index)                  canMirrorMode[(Index)]             /* GENy will be generated by platforms that support this feature */
# define Can_SetMirrorModeState(Index,Value)            canMirrorMode[(Index)] = (Value)   /* GENy will be generated by platforms that support this feature */
# define Can_GetSilentModeState(Index)                  canSilentMode[(Index)]             /* GENy will be generated by platforms that support this feature */
# define Can_SetSilentModeState(Index,Value)            canSilentMode[(Index)] = (Value)   /* GENy will be generated by platforms that support this feature */
# define Can_GetMailboxState(Index)                     canMailboxState[(Index)] /* only RAM_CHECK */
# define Can_SetMailboxState(Index,Value)               canMailboxState[(Index)] = (Value) /* only RAM_CHECK */

# define Can_GetCanTxBasicDataBuffer(ch,muxTx)          canTxBasicDataBuffer[(ch)][(muxTx)]
# define Can_GetRxQueueInfo()                           (canRxQueueInfo)
# define Can_GetRxQueueBuffer(Index)                    (canRxQueueBuffer[(Index)])
# define Can_IsTimeoutCall(Index)                       (canTimeoutCall[(Index)] != FALSE) /* only CAN_MSR3 */
# define Can_SetTimeoutCall(Index,Value)                canTimeoutCall[(Index)] = (Value)

#endif

# if !defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COM_STACK_LIB */ /* COV_CAN_MICROSAR_VERSION */
/* HighLevel */
#  if !defined(Can_GetLastInitObject)
#   define Can_GetLastInitObject(Index)          lastInitObject[(Index)]
#  endif

/* CONST PB */
#  define Can_GetBCanHwIdxOfFilter(Index)        CanFilter[(Index)].BCanHwIdx
#  define Can_GetCodeOfFilter(Index)             CanFilter[(Index)].Code
#  define Can_GetMaskOfFilter(Index)             CanFilter[(Index)].Mask

#  define Can_GetFilterStartIdx(Index)           CanFilterStartIdx[(Index)]

#  define Can_GetCBTR0OfInitObject(Index)        CanInitObject[(Index)].CBTR0
#  define Can_GetCBTR1OfInitObject(Index)        CanInitObject[(Index)].CBTR1

/* CONST LT */
#  define Can_GetBusName(Index)                  CanBusName[(Index)]

/* VAR LT */
#  define  Can_GetRxMsg(Index)                   (canRxMsg[(Index)])
#  define  Can_GetgCanController(Index)          (gCanController[(Index)])

# else
#  define  Can_GetRxMsg(Index)                   Can_GetRxMsgOfControllerData(Index)
#  define  Can_GetgCanController(Index)          Can_GetgCanControllerOfControllerData(Index)
#  define  Can_GetBusName(Index)                 Can_GetBusNameOfControllerConfig(Index)
# endif

/***************************************************************************/
/* Default switches                                                        */
/* Automatic define settings, depending on user configuration in can_cfg.h */
/***************************************************************************/
#define C_HL_DISABLE_OVERRUN_IN_STATUS

#define C_HL_DISABLE_HW_RANGES_FILTER

#define C_HL_ENABLE_IDTYPE_IN_ID

/* Specify the ID tables which contain the ID type */
# define C_LL_RX_IDTYPE_TABLE      CanRxId0
# define C_LL_TX_IDTYPE_TABLE      CanTxId0

#define C_HL_DISABLE_DUMMY_FCT_CALL

#define C_HL_DISABLE_TX_MSG_DESTROYED

#define C_HL_DISABLE_CANCEL_IN_HW_TASK

#define C_HL_ENABLE_HW_EXIT_TRANSMIT /* do not enter TX outside START mode - only emulated STOP! */

#define C_HL_ENABLE_LAST_INIT_OBJ

#define C_HL_ENABLE_CAN_IRQ_DISABLE

#define C_HL_DISABLE_ADJUST_RXHANDLE

#define C_HL_ENABLE_REJECT_UNWANTED_IDTYPE

#define C_HL_DISABLE_REJECT_REMOTE_FRAME

#define C_HL_DISABLE_REJECT_REMOTE_FRAME_FULLCAN

#define C_HL_DISABLE_COPROCESSOR_SUPPORT

#define C_HL_DISABLE_RETRANSMIT_FCT

#define C_HL_ENABLE_RETRANSMIT_CONF_ISR

#define C_HL_DISABLE_AVOID_REENTRANT_APPLCANCANCELNOTIFICATION

/* Possibility to disable the prototypes of interrupt service routines in the driver header file */

#define C_DISABLE_TX_ACTUAL_MACROS

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

/* macros to set DLC and ID in RX-,TX-structures -------------------------- */
/* MK... macros are used by the generation tool to fill the RX and TX structures */
/* In case the IDE bit is located in the ID registers, this bit has to be handled as must match.
   In general, this is done by setting the IDE bit to 0 in the .._MASK_.. macros.
*/

#define MK_RX_RANGE_MASK_IDSTD0(id)   ( MK_STDID0(id) & ~kCanIdTypeExt )
/*
#define MK_RX_RANGE_MASK_IDSTD1(id)   ( MK_STDID1(id) )
#define MK_RX_RANGE_MASK_IDSTD2(id)   ( MK_STDID2(id) )
#define MK_RX_RANGE_MASK_IDSTD3(id)   ( MK_STDID3(id) )
#define MK_RX_RANGE_MASK_IDSTD4(id)   ( MK_STDID4(id) )
*/

#define MK_RX_RANGE_CODE_IDSTD0(id)   ( MK_STDID0(id) )
/*
#define MK_RX_RANGE_CODE_IDSTD1(id)   ( MK_STDID1(id) )
#define MK_RX_RANGE_CODE_IDSTD2(id)   ( MK_STDID2(id) )
#define MK_RX_RANGE_CODE_IDSTD3(id)   ( MK_STDID3(id) )
#define MK_RX_RANGE_CODE_IDSTD4(id)   ( MK_STDID4(id) )
*/

#define MK_RX_RANGE_MASK_IDEXT0(id)   ( MK_EXTID0(id)  & ~kCanIdTypeExt)
/*
#define MK_RX_RANGE_MASK_IDEXT1(id)   ( MK_EXTID1(id) )
#define MK_RX_RANGE_MASK_IDEXT2(id)   ( MK_EXTID2(id) )
#define MK_RX_RANGE_MASK_IDEXT3(id)   ( MK_EXTID3(id) )
#define MK_RX_RANGE_MASK_IDEXT4(id)   ( MK_EXTID4(id) )
*/

#define MK_RX_RANGE_CODE_IDEXT0(id)   ( MK_EXTID0(id))
/*
#define MK_RX_RANGE_CODE_IDEXT1(id)   ( MK_EXTID1(id) )
#define MK_RX_RANGE_CODE_IDEXT2(id)   ( MK_EXTID2(id) )
#define MK_RX_RANGE_CODE_IDEXT3(id)   ( MK_EXTID3(id) )
#define MK_RX_RANGE_CODE_IDEXT4(id)   ( MK_EXTID4(id) )
*/

#define MK_STDID0(id)            ( (vuint32)((id) & 0x07FFU) )
/*
#define MK_STDID1(id)            (0)
#define MK_STDID2(id)            (0)
#define MK_STDID3(id)            (0)
#define MK_STDID4(id)            (0)
*/

#define MK_EXTID0(id)            (  kCanIdTypeExt | (vuint32)((id) & 0x1FFFFFFFUL) )
/*
#define MK_EXTID1(id)            (0)
#define MK_EXTID2(id)            (0)
#define MK_EXTID3(id)            (0)
#define MK_EXTID4(id)            (0)
*/

/* Message Configuration Register:
 * Example C16x:
 * DLC3 DLC2 DLC1 DLC0 Dir Xtd 0 0
 *                      |   |+------------- 0: standard ID; 1: extended ID
 *                      +------------------ 0: receive    ; 1: transmit
*/
#define C_RX_DIR             0x00
#define C_TX_DIR             0x08

#define MK_TX_DLC(dlc)       (dlc)
#define MK_TX_DLC_EXT(dlc)   (dlc)

/* extract  DLC from register format to logical format */
#define XT_TX_DLC(dlc)       ((vuint8)(dlc))        /* return code has to be vuint8 */

#if defined( C_SINGLE_RECEIVE_CHANNEL )
/* Bus-Off functions */
# define CanResetBusOffStart(a)  (CanInit(a))
# define CanResetBusOffEnd(a)

/* Bus-Sleep functions */
# define CanResetBusSleep(a)     (CanInit(a))

#else
/* Bus-Off functions */
# define CanResetBusOffStart(ch, a)  (CanInit((ch), (a)))
# define CanResetBusOffEnd(ch, a)
  
/* Bus-Sleep functions */
# define CanResetBusSleep(ch, a)     (CanInit((ch), (a)))
#endif

/* The macros CanRxActualIdRawx() have to provide the register context of the ID registers.
   It is not necessary to mask any bits. These macros are for internal use only and masking
   will be done in can_hls.c
*/
#if defined( C_ENABLE_EXTENDED_ID )
# define CanRxActualIdRaw0(rxStruct)     ((rxStruct)->pChipMsgObj->IdRaw)
/* #define CanRxActualIdRaw1(rxStruct)   0
   #define CanRxActualIdRaw2(rxStruct)   0
   #define CanRxActualIdRaw3(rxStruct)   0
   #define CanRxActualIdRaw4(rxStruct)   0 */
#else
# define CanRxActualIdRaw0(rxStruct)     (vuint16)((rxStruct)->pChipMsgObj->IdRaw)
/* #define CanRxActualIdRaw1(rxStruct)   0
   #define CanRxActualIdRaw2(rxStruct)   0
   #define CanRxActualIdRaw3(rxStruct)   0
   #define CanRxActualIdRaw4(rxStruct)   0 */
#endif

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */

#define CanRxActualExtId(rxStruct)        ((vuint32)(((rxStruct)->pChipMsgObj->IdRaw) & 0x1FFFFFFFUL))
#define CanRxActualStdId(rxStruct)        ((vuint16)(((rxStruct)->pChipMsgObj->IdRaw) & 0x7FFU))

#define CanRxActualData(rxStruct,i)       (((rxStruct)->pChipMsgObj->DataFld[i]))
#define CanRxActualDLC(rxStruct)          ((vuint8)(((rxStruct)->pChipMsgObj->DlcRaw) & 0x0FU ))
#define CanRxActualIdType(rxStruct)       ((vuint32)(((rxStruct)->pChipMsgObj->IdRaw) & kCanIdTypeExt ))

#define CanRxActualFdType(rxStruct)       ((vuint32)(((rxStruct)->pChipMsgObj->IdRaw) & kCanFdTypeFd ))

#if defined( C_SINGLE_RECEIVE_CHANNEL )
 #define CanRxActualErrorCounter()        ( Can_GetgCanController(0).ErrCntRx )
 #define CanTxActualErrorCounter()        ( Can_GetgCanController(0).ErrCntTx )
#else
 #define CanRxActualErrorCounter(channel) ( Can_GetgCanController(channel).ErrCntRx )
 #define CanTxActualErrorCounter(channel) ( Can_GetgCanController(channel).ErrCntTx )
#endif

/* Macros for CAN message access within GenericConfirmation() function */
#define CanTxActualStdId(txStruct)        ((vuint16)((txStruct)->pChipMsgObj->IdRaw) & 0x7FFU)
#define CanTxActualExtId(txStruct)        ((vuint32)((txStruct)->pChipMsgObj->IdRaw) & 0x1FFFFFFFUL)
#define CanTxActualDLC(txStruct)          ((txStruct)->pChipMsgObj->DlcRaw)
#define CanTxActualIdType(txStruct)       ((vuint32)((txStruct)->pChipMsgObj->IdRaw) & kCanIdTypeExt )

/* Macros for CAN message access within PreTransmit() function */
#if defined( C_ENABLE_EXTENDED_ID )
# define CanTxWriteActExtId(txStruct,id)  { (txStruct).pChipMsgObj->IdRaw = MK_EXTID0(id);}
#endif
#define CanTxWriteActId(txStruct,id)  { (txStruct).pChipMsgObj->IdRaw = MK_STDID0(id);}

#define CanTxWriteActDLC(txStruct,dlc)  {*(txStruct).pChipMsgObj->DlcRaw = (vuint8)(dlc);}

/* Macros to fill variables of the type tCanMsgTransmitStruct */
#define CanMsgTransmitSetStdId(canMsgTransmitStruct, id)  { (canMsgTransmitStruct)->IdRaw  = MK_STDID0(id);}
#define CanMsgTransmitSetExtId(canMsgTransmitStruct, id)  { (canMsgTransmitStruct)->IdRaw  = MK_EXTID0(id);}
#define CanMsgTransmitSetDlc(canMsgTransmitStruct, dlc)   { (canMsgTransmitStruct)->DlcRaw = MK_TX_DLC(dlc); }
#define CanMsgTransmitSetData(canMsgTransmitStruct, dataByte, msgData) { (canMsgTransmitStruct)->DataFld[dataByte] = (msgData); }

/* macros for buffer access */
#define StartRxReadSync()          (VStdSuspendAllInterrupts())
#define EndRxReadSync()            (VStdResumeAllInterrupts())

#define StartRxWriteSync()         (VStdSuspendAllInterrupts())
#define EndRxWriteSync()           (VStdResumeAllInterrupts())

#define StartTxReadSync()
#define EndTxReadSync()

#define StartTxWriteSync()         (VStdSuspendAllInterrupts())
#define EndTxWriteSync()           (VStdResumeAllInterrupts())

#define StartRxWriteCANSync() 
#define EndRxWriteCANSync()   

#define StartRxReadCANSync()  
#define EndRxReadCANSync()    

#define StartRxFullCANReadSync(Obj)   
#define EndRxFullCANReadSync(Obj)     

#define StartRxFullCANWriteSync(obj)
#define EndRxFullCANWriteSync(obj)

/* macros for flag access */
#define CanStartFlagWriteSync()    (VStdSuspendAllInterrupts())
#define CanEndFlagWriteSync()      (VStdResumeAllInterrupts())

#define CAN_HL_MIN(x,y) (((x)>(y)) ? (y) : (x)) /* COV_CAN_GENDATA_FAILURE */
#define CAN_HL_MAX(x,y) (((x)<(y)) ? (y) : (x)) /* COV_CAN_GENDATA_FAILURE */

/***************************************************************************************************/
/* Macros for dummy statements */
#if !defined(CAN_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
#  define CAN_DUMMY_STATEMENT(x) (x) = (x)
# else
#  define CAN_DUMMY_STATEMENT(x)
# endif
#endif
#if !defined(CAN_DUMMY_STATEMENT_CONST) /* COV_CAN_COMPATIBILITY */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
#  define CAN_DUMMY_STATEMENT_CONST(x) (void)(x)
# else
#  define CAN_DUMMY_STATEMENT_CONST(x)
# endif
#endif

/***************************************************************************************************/
/* Macros for DET and assertions */
#if (CAN_DEV_ERROR_REPORT == STD_ON) /* \trace SPEC-1404, SPEC-1696, SPEC-3803, SPEC-3849 */
/* \trace SPEC-1725, SPEC-1403, SPEC-1568, SPEC-3781, SPEC-3819 */
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
#  define Can_CallDetReportError(api, err) ((void) Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, (api), (err)))
# else
#  define Can_CallDetReportError(api, err) (Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, (api), (err)))
# endif
# define assertHardware(a, b, c)  if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define assertInternal(a, b, c)  if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define assertUser(a, b, c)      if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define assertGen(a, b, c)       if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define C_ENABLE_HARDWARE_CHECK
# define C_ENABLE_INTERNAL_CHECK
# define C_ENABLE_USER_CHECK
# define C_ENABLE_GEN_CHECK
#else
# define Can_CallDetReportError(api, err)
# define assertHardware(a, b, c)
# define assertInternal(a, b, c)
# define assertUser(a, b, c)
# define assertGen(a, b, c)
#endif
#if (CAN_DEV_ERROR_DETECT == STD_ON) 
# define CAN_IS_NO_ERROR(errorId) if ((errorId) == CAN_E_NO_ERROR) /* COV_CAN_HARDWARE_FAILURE */
#else
# define CAN_IS_NO_ERROR(errorId)
#endif

/***************************************************************************************************/
/* Macros for Critical Area abstraction */
#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */
# define Can_EnterCritical(area)     SchM_Enter_Can_##area() /* PRQA S 0342 */ /* MD_Can_0342 */
# define Can_LeaveCritical(area)     SchM_Exit_Can_##area() /* PRQA S 0342*/ /* MD_Can_0342 */
#else
# define Can_EnterCritical(area)     SchM_Enter_Can(area) /* do not use double brackets here ESCAN00060494 */
# define Can_LeaveCritical(area)     SchM_Exit_Can(area)
#endif

#define CanDeclareGlobalInterruptOldStatus
/* CAN_EXCLUSIVE_AREA_0 called only by LL code */
#define CanSingleGlobalInterruptDisable()     Can_EnterCritical(CAN_EXCLUSIVE_AREA_0)
#define CanSingleGlobalInterruptRestore()     Can_LeaveCritical(CAN_EXCLUSIVE_AREA_0)
#define CanGlobalInterruptDisable()           Can_EnterCritical(CAN_EXCLUSIVE_AREA_0)
#define CanGlobalInterruptRestore()           Can_LeaveCritical(CAN_EXCLUSIVE_AREA_0)
#define CanNestedGlobalInterruptDisable()     Can_EnterCritical(CAN_EXCLUSIVE_AREA_0)
#define CanNestedGlobalInterruptRestore()     Can_LeaveCritical(CAN_EXCLUSIVE_AREA_0)

/* For LL: parameter has to be 'channel' not CAN_CHANNEL_PARA_ONLY */
# define CAN_CAN_INTERRUPT_RESTORE(ch)  Can_EnableControllerInterrupts((uint8)(ch))
# define CAN_CAN_INTERRUPT_DISABLE(ch)  Can_DisableControllerInterrupts((uint8)(ch))

/***************************************************************************************************/
/* Macros for LL-HLL compatibility */
#if defined(C_ENABLE_SLEEP_WAKEUP)
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define APPL_CAN_WAKEUP(ch)        (CanHL_WakeUpHandling())
# else
#  define APPL_CAN_WAKEUP(ch)        (CanHL_WakeUpHandling(ch))
# endif
# if defined(CAN_ENABLE_MICROSAR_VERSION_MSR40X) /* COV_CAN_MICROSAR_VERSION */

#  if defined(MISRA_CHECK) /* COV_CAN_MISRA */
#   pragma PRQA_MACRO_MESSAGES_OFF "CAN_WAKEUP",0277 /* MD_Can_0277_negation */
#  endif
/* remove StatusReq - sleep because wakeup may occur before sleep is reached and will cause DET otherwise */
#  define CAN_WAKEUP(ch)              Can_SetStatusReq(ch, Can_GetStatusReq(ch) & (uint8)(~CAN_REQ_SLEEP)); \
                                      ((void) Can_SetControllerMode((uint8)(ch), CAN_T_WAKEUP))
# else
#  define CAN_WAKEUP(ch)              ((void) Can_SetControllerMode((uint8)(ch), CAN_T_WAKEUP))
# endif
#endif

/***************************************************************************/
/* error codes                                                             */
/***************************************************************************/
/* error numbers for User Assertions 0x20-0x3f - hardware depended */

/* error numbers for Generation tool Assertions 0x60-0x7f - hardware depended */

/* error numbers for Hardware Assertions 0xa0-0xbf - hardware depended */
#define kErrorWrongChannelForMsgEvent   ((vuint8)0xa0)
#define kErrorWrongDlcReceived          ((vuint8)0xa1)  /* DLC > 8 */

/* error numbers for Internal Assertions 0xe0-0xff - hardware depended */

/***************************************************************************/
/* Special switches need by CAN driver                                     */
/***************************************************************************/

/***************************************************************************/
/* Defines / data types / structures / unions                              */
/***************************************************************************/
/* Define CAN Chip hardware; segment must be located in locator file    */
/* register layout of the can chip                                      */
/* Structure describing CAN receive buffer. */

typedef struct sCanRxInfoStruct
{
  CanChipMsgPtr  pChipMsgObj;
  CanChipDataPtr pChipData;
  Can_IdType localId;
  Can_HwHandleType localHrh;
  uint8 localDlc;
  /* LL specific part */
} tCanRxInfoStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanRxInfoStruct, VAR_NOINIT) CanRxInfoPtrType;

#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
typedef struct sCanTxInfoStruct
{
  CanChipDataPtr pChipData;
  /* LL specific part */
    CanChipMsgPtr     pChipMsgObj;   /* only for macros used in pretransmit */
} tCanTxInfoStruct;
#endif

typedef struct sCanInitInfoStruct
{
  CAN_CHANNEL_CANTYPE_LOCAL
  CanInitHandle  initObject;
  uint8 isMbCorrupt;
  uint8 isChCorrupt;
  /* LL specific part */
  /*
  |<DataModelStart>| CanLL_InitLocalParameter
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  Can_ControllerType *canLLController;
} tCanInitInfoStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanInitInfoStruct, VAR_NOINIT) CanInitInfoPtrType;
#define CAN_HL_INFO_INIT_PTR(member)(initInfo->member) /* PRQA S 3410 */ /* MD_Can_3410 */
#define CAN_HL_INFO_INIT(member)    (initInfo.member)  /* PRQA S 3410 */ /* MD_Can_3410 */

typedef struct sCanRxBasicInfoStruct
{
  tCanRxInfoStruct rxInfo;
  CanObjectHandle rxObjHandle;
  CAN_CHANNEL_CANTYPE_LOCAL
  /* LL specific part */
  /*
  |<DataModelStart>| CanLL_RxBasicCanMsgReceivedLocalParameter
  Relation_Context:
  RxBasicCANSupport
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  vuint8 overrunOccur;
} tCanRxBasicInfoStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanRxBasicInfoStruct, VAR_NOINIT) CanRxBasicInfoPtrType;
#define CAN_HL_INFO_RXBASIC_PTR(member) ((*rxBasicInfo).member) /* PRQA S 3410 */ /* MD_Can_3410 */
#define CAN_HL_INFO_RXBASIC(member)     (rxBasicInfo.member)  /* PRQA S 3410 */ /* MD_Can_3410 */

typedef struct sCanRxFullInfoStruct
{
  tCanRxInfoStruct rxInfo;
  CanObjectHandle rxObjHandle;
  CAN_CHANNEL_CANTYPE_LOCAL
  /* LL specific part */
  /*
  |<DataModelStart>| CanLL_RxFullCanMsgReceivedLocalParameter
  Relation_Context:
  RxFullCANSupport
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
  |<DataModelEnd>|
  */
  vuint8 overrunOccur;
} tCanRxFullInfoStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanRxFullInfoStruct, VAR_NOINIT) CanRxFullInfoPtrType;
#define CAN_HL_INFO_RXFULL_PTR(member) ((*rxFullInfo).member) /* PRQA S 3410 */ /* MD_Can_3410 */
#define CAN_HL_INFO_RXFULL(member)     (rxFullInfo.member)  /* PRQA S 3410 */ /* MD_Can_3410 */

V_DEF_P2VAR_TYPE(V_NONE, CanObjectHandle, VAR_NOINIT) CanHlObjectHandlePtrType;
    
/***************************************************************************/
/* module global variable declaration                                      */
/***************************************************************************/

/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/
#define CAN_START_SEC_VAR_NOINIT_UNSPECIFIED  /*---------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /*----------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***************************************************************************/
/* local function prototypes (only internal usage)                         */
/***************************************************************************/
#define CAN_START_SEC_STATIC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/************************************************************************************************************
 *  CanLL_CanInterruptDisable()
 ***********************************************************************************************************/
/*! \brief         disable CAN interrupt
 *  \details       saves the current status (enable or disable) of all 
 *                 CAN interrupts to localInterruptOldFlagPtr
 *                 and disables all CAN interrupts  (RX, TX, error, wakeup).
 *  \param[in]     canHwChannel              CAN HW controller
 *  \param[out]    localInterruptOldFlagPtr  get old interrupt state to be restored
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptDisable(CanChannelHandle canHwCh, CanLLCanIntOldPtrType localInterruptOldFlagPtr);
/************************************************************************************************************
 *  CanLL_CanInterruptRestore()
 **********************************************************************************************************/
/*! \brief         enable CAN interrupt
 *  \details       restores all CAN interrupts (RX, TX, error, wakeup).
 *  \param[in]     canHwChannel              CAN HW controller
 *  \param[in]     localInterruptOldFlag     old interrupt state to be restored
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptRestore(CanChannelHandle canHwCh, tCanLLCanIntOld localInterruptOldFlag);

#if defined( C_ENABLE_SLEEP_WAKEUP )
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_WakeUpHandling()
 ***********************************************************************************************************/
/*! \brief         WAKEUP handling
 *  \details       perform wakeup handling.
 *  \param[in]     Controller          CAN controller (Controller must be in valid range)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */
#endif
#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS ) || defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_RxIndicationMsgCopy()
 ***********************************************************************************************************/
/*! \brief         Copy Message data
 *  \details       finish Can_RxIndication by copy data to emulated CAN cell.
 *  \param[in]     canChannel            CAN controller
 *  \param[in]     flags                 additional message information
 *  \param[in]     dataLength            data length of the received message
 *  \param[in]     data                  pointer to the received message data
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE)  CanLL_RxIndicationMsgCopy(vuint8 canChannel, CanObjectHandle rxObjHandle, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data);
/* CODE CATEGORY 1 END */
#endif
#if defined( C_ENABLE_CAN_RAM_CHECK )
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_IsMailboxCorrupt()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       check the current mailbox at index hwObjHandle by write and read back patterns.
 *  \param[in]     Controller          CAN controller
 *  \param[in]     hwObjHandle         Handle to hardware object
 *  \return        kCanTrue:    Mailbox is corrupt
 *  \return        kCanFalse:   Mailbox is not corrupt  
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle);
/* CODE CATEGORY 4 END */

# if defined( C_ENABLE_CAN_RAM_CHECK_EXTENDED )
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_InitBeginIsRegisterCorrupt()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       check the channel register at the beginning of the initialization by write and read back patterns.
 *  \param[in]     Controller          CAN controller
 *  \return        kCanTrue:    Channel is corrupt
 *  \return        kCanFalse:   Channel is not corrupt  
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBeginIsRegisterCorrupt(CAN_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_InitEndIsRegisterCorrupt()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       check the channel register at the ending of the initialization by write and read back patterns.
 *  \param[in]     Controller          CAN controller
 *  \return        kCanTrue:    Channel is corrupt
 *  \return        kCanFalse:   Channel is not corrupt  
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEndIsRegisterCorrupt(CAN_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */
# endif
#endif

/************************************************************************************************************
 *  CanInit()
 ***********************************************************************************************************/
/*! \brief         Wrapper for Can_InitController()
 *  \details       Initialization of controller specific CAN hardware. \n
 *                 The CAN driver registers and variables are initialized. \n
 *                 The CAN controller is fully initialized and left back within the state "STOP mode", ready to change to "Running Mode".
 *                 Called from LowLevel driver code while bus off handling or in case of mode transition that requires a reinitialization.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \param[in]     initObject    Index of the initialization structure to be initialized (specifies the baud rate to be used.)
 *  \pre           initObject has to be valid
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanInit(CAN_CHANNEL_CANTYPE_FIRST uint8 initObject);
/************************************************************************************************************
 *  Can_CleanUpSendState()
 ***********************************************************************************************************/
/*! \brief         Clean up send state
 *  \details       Reset active send state (semaphore blocking TX while send).
 *                 Called by Can_SetControllerMode(), Can_Mainfunction_Mode() (Can_InitController() do same but without this call).
 *  \param[in]     Controller    CAN controller \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) Can_CleanUpSendState(CAN_CHANNEL_CANTYPE_ONLY);

/************************************************************************************************************
 *  CanHL_StartTransition()
 ***********************************************************************************************************/
/*! \brief         Request START
 *  \details       Handle START transition and wait ASR3:synchronous, ASR4:asynchronous-short time and notify to upper layer.
 *                 Called by Can_SetControllerMode().
 *  \param[in]     Controller    CAN controller (Controller must be in valid range). \n
 *                               (only if not using "Optimize for one controller")
 *  \return        CAN_NOT_OK    transition rejected
 *  \return        CAN_OK        transition finished successful or is ongoing asynchronous
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_StartTransition(CAN_CHANNEL_CANTYPE_ONLY);

/************************************************************************************************************
 *  CanHL_StopTransition()
 ***********************************************************************************************************/
/*! \brief         Request STOP
 *  \details       Handle STOP transition and wait ASR3:synchronous, ASR4:asynchronous-short time and notify to upper layer.
 *                 Called by Can_SetControllerMode().
 *  \param[in]     Controller    CAN controller (Controller must be in valid range). \n
 *                               (only if not using "Optimize for one controller")
 *  \return        CAN_NOT_OK    transition rejected
 *  \return        CAN_OK        transition finished successful or is ongoing asynchronous
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_StopTransition(CAN_CHANNEL_CANTYPE_ONLY);

/************************************************************************************************************
 *  CanHL_WakeupTransition()
 ***********************************************************************************************************/
/*! \brief         Request WAKEUP
 *  \details       Handle WAKEUP transition and wait ASR3:synchronous, ASR4:asynchronous-short time and notify to upper layer.
 *                 Called by Can_SetControllerMode().
 *  \param[in]     Controller    CAN controller (Controller must be in valid range). \n
 *                               (only if not using "Optimize for one controller")
 *  \return        CAN_NOT_OK    transition rejected
 *  \return        CAN_OK        transition finished successful or is ongoing asynchronous
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_WakeupTransition(CAN_CHANNEL_CANTYPE_ONLY);

#if defined(C_ENABLE_CAN_RAM_CHECK)
/************************************************************************************************************
 *  CanHL_DoRamCheck()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       Start RAM check by set request and start Can_InitController doing the check.
 *                 Called by CanHL_WakeupTransition(), Wakeup Processing, Can_RamCheckExecution().
 *  \param[in]     Controller    CAN controller (Controller must be in valid range). \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_DoRamCheck(CAN_CHANNEL_CANTYPE_ONLY);
#endif

/************************************************************************************************************
 *  CanHL_SleepTransition()
 ***********************************************************************************************************/
/*! \brief         Request SLEEP
 *  \details       Handle SLEEP transition and wait ASR3:synchronous, ASR4:asynchronous-short time and notify to upper layer.
 *                 Called by Can_SetControllerMode().
 *  \param[in]     Controller    CAN controller (Controller must be in valid range). \n
 *                               (only if not using "Optimize for one controller")
 *  \return        CAN_NOT_OK    transition rejected
 *  \return        CAN_OK        transition finished successful or is ongoing asynchronous
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_SleepTransition(CAN_CHANNEL_CANTYPE_ONLY);

#if defined(C_ENABLE_CANCEL_IN_HW)
/************************************************************************************************************
 *  CanHL_IsHigherCanIdPrio()
 ***********************************************************************************************************/
/*! \brief         check CAN ID priority
 *  \details       compare two CAN IDs and check if first one is of higher priority.
 *  \param[in]     CanId1        first CAN ID for compare
 *  \param[in]     CanId2        second CAN ID for compare
 *  \return        CAN_OK        first CAN ID has higher priority
 *  \return        CAN_NOT_OK    first CAN ID has no higher priority
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_IsHigherCanIdPrio(Can_IdType CanId1, Can_IdType CanId2);
#endif

#if defined(C_ENABLE_CANCEL_IN_HW) && defined(C_ENABLE_MULTIPLEXED_TRANSMISSION)
/************************************************************************************************************
 *  CanHL_SortlogTxObjHandlePrioList()
 ***********************************************************************************************************/
/*! \brief         Sort TX handle
 *  \details       Sort transmit hardware priority list to check witch hardware object should be cancelled first 
 *                 (not all objects ready to cancel e.g. already cancelled) so low priority ID is first in list.
 *                 Called by Can_Write().
 *  \param[out]    logTxObjHandlePrioList      Array[3] in case of MultiplexedTx which contain the logical handle index
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_SortlogTxObjHandlePrioList(CanHlObjectHandlePtrType logTxObjHandlePrioList);
#endif

/************************************************************************************************************
 *  CanHL_WritePrepare()
 ***********************************************************************************************************/
/*! \brief         TX preparation
 *  \details       Search for BasicCAN object handle to transmit (Multiplexed TX) and backup data for cancel TX object if necessary
 *                 Called by Can_Write().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     Hth              hardware transmit handle from (CanMailboxId)
 *  \param[in]     PduInfo          PDU info include ID, DLC, data, PDU
 *  \return        logTxObjHandle   logical transmit handle to be send
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, CanObjectHandle, STATIC_CODE) CanHL_WritePrepare(CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType Hth, Can_PduInfoPtrType PduInfo);

#if defined(C_ENABLE_CANCEL_IN_HW)
/************************************************************************************************************
 *  CanHL_WritePrepareCancelTx()
 ***********************************************************************************************************/
/*! \brief         TX cancellation
 *  \details       Cancel the BasicCAN TX object
 *                 Called by CanHL_WritePrepare().
 *  \param[in]     Controller              CAN controller. \n
 *                                         (only if not using "Optimize for one controller")
 *  \param[in]     logTxObjHandle          logical transmit handle to be send
 *  \param[in]     muxTx                   multiplexed TX index (or 0 for none)
 *  \param[in]     muxTx_max               multiplexed TX max size (1 or 3)
 *  \param[in]     logTxObjHandlePrioList  logTxObjHandle list
 *  \param[in]     PduInfo                 PDU info include ID, DLC, data, PDU
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WritePrepareCancelTx(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle logTxObjHandle, uint8_least muxTx, uint8_least muxTx_max, CanHlObjectHandlePtrType logTxObjHandlePrioList, Can_PduInfoPtrType PduInfo);
#endif

/************************************************************************************************************
 *  CanHL_WriteStart()
 ***********************************************************************************************************/
/*! \brief         TX start
 *  \details       Write ID, DLC and Data to FullCAN or BasicCAN object and set transmit request
 *                 Called by Can_Write().
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     Hth             hardware transmit handle from (CanMailboxId)
 *  \param[in]     logTxObjHandle  logical TX object handle
 *  \param[in]     PduInfo         PDU info include ID, DLC, data, PDU
 *  \return        CAN_NOT_OK      transmission failed.
 *  \return        CAN_OK          transmission successful.
 *  \return        CAN_BUSY        transition object busy - repeat it.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_WriteStart(CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType Hth, CanObjectHandle logTxObjHandle, Can_PduInfoPtrType PduInfo);

/************************************************************************************************************
 *  CanHL_GetTxHwToLogHandle()
 ***********************************************************************************************************/
/*! \brief         Convert Handle
 *  \details       Hardware to logical Handle conversion.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     hwTxObjHandle   Handle to hardware mailbox
 *  \return        logical handle for the given hardware mailbox
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, CanObjectHandle, STATIC_CODE) CanHL_GetTxHwToLogHandle(CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType hwTxObjHandle);

/************************************************************************************************************
 *  CanHL_GetTxLogToHwHandle()
 ***********************************************************************************************************/
/*! \brief         Convert Handle
 *  \details       Logical to hardware Handle conversion.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     logTxObjHandle  Logical handle of TX mailbox
 *  \return        hardware mailbox for the given logical handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_HwHandleType, STATIC_CODE) CanHL_GetTxLogToHwHandle(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle logTxObjHandle);

#if defined(C_ENABLE_CAN_RAM_CHECK)
/************************************************************************************************************
 *  CanHL_RamCheckMailboxNotification()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       Notify upper layer in case of an HW issue and deactivate controller.
 *                 Called by Can_InitController().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     hwObjHandle      Handle to hardware object
 *  \param[in]     htrh             Hrh or Hth of the hardware object
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANTYPE_FIRST Can_HwHandleType hwObjHandle, Can_HwHandleType htrh);
#endif

/************************************************************************************************************
 *  CanHL_SetValue8()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck. 
 *                 Notify upper layer in case of an HW issue and deactivate controller.
 *                 Called by Can_InitController().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_SetValue8(CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr8 regPtr, uint8 value, uint8 readMask);

/************************************************************************************************************
 *  CanHL_SetValue16()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck. 
 *                 Notify upper layer in case of an HW issue and deactivate controller.
 *                 Called by Can_InitController().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_SetValue16(CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr16 regPtr, uint16 value, uint16 readMask);

/************************************************************************************************************
 *  CanHL_SetValue32()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck. 
 *                 Notify upper layer in case of an HW issue and deactivate controller.
 *                 Called by Can_InitController().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_SetValue32(CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr32 regPtr, uint32 value, uint32 readMask);

/************************************************************************************************************
 *  CanHL_InitBegin()
 ***********************************************************************************************************/
/*! \brief         Start Initialization
 *  \details       LL implementation see token CanLL_InitBegin description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitBegin(CanInitInfoPtrType initInfo);

#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_InitTxFullCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize TX FullCAN
 *  \details       LL implementation see token CanLL_InitTxObj description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxFullCAN(CanInitInfoPtrType initInfo);
#endif

/************************************************************************************************************
 *  CanHL_InitTxBasicCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize TX BasicCAN
 *  \details       LL implementation see token CanLL_InitTxObj description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxBasicCAN(CanInitInfoPtrType initInfo);

/************************************************************************************************************
 *  CanHL_InitUnusedObj()
 ***********************************************************************************************************/
/*! \brief         Initialize unused mailbox objects
 *  \details       LL implementation see token CanLL_InitUnusedObj description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitUnusedObj(CanInitInfoPtrType initInfo);

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_InitRxFullCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize RX FullCAN
 *  \details       LL implementation see token CanLL_InitFullCANObj description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxFullCAN(CanInitInfoPtrType initInfo);
#endif

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_InitRxBasicCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize RX BasicCAN
 *  \details       LL implementation see token CanLL_InitBasicCANObj description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxBasicCAN(CanInitInfoPtrType initInfo);
#endif

/************************************************************************************************************
 *  CanHL_InitEnd_InitMode()
 ***********************************************************************************************************/
/*! \brief         End Initialization
 *  \details       LL implementation see token CanLL_InitEnd_InitMode description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitEnd_InitMode(CanInitInfoPtrType initInfo);

/************************************************************************************************************
 *  CanHL_InitEnd_StopMode()
 ***********************************************************************************************************/
/*! \brief         Transition to STOP
 *  \details       LL implementation see token CanLL_InitEnd_StopMode description.
 *                 Called by Can_InitController().
 *  \param[in]     initInfo          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitEnd_StopMode(CanInitInfoPtrType initInfo);

/************************************************************************************************************
 *  CanHL_StopReinit()
 ***********************************************************************************************************/
/*! \brief         Reinitialization after STOP
 *  \details       LL implementation see token CanLL_StopReinit description.
 *                 Called by Can_InitController().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_StopReinit(CAN_HW_CHANNEL_CANTYPE_ONLY);

/************************************************************************************************************
 *  CanHL_InitPowerOn()
 ***********************************************************************************************************/
/*! \brief         Power on initialization
 *  \details       LL implementation see token CanLL_InitPowerOn description.
 *                 Called by Can_InitController().
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_InitPowerOn(void);

/************************************************************************************************************
 *  CanHL_InitPowerOnChannelSpecific()
 ***********************************************************************************************************/
/*! \brief         Power on channel initialization
 *  \details       LL implementation see token CanLL_InitPowerOnChannelSpecific description.
 *                 Called by Can_Init().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_InitPowerOnChannelSpecific(CAN_CHANNEL_CANTYPE_ONLY);

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_RxBasicMsgReceivedBegin()
 ***********************************************************************************************************/
/*! \brief         RX BasicCAN start
 *  \details       RX BasicCAN handling start.
 *                 Called by CanBasicCanMsgReceived().
 *  \param[in]     rxBasicInfo          include controller and all LL specific settings.
 *  \return        CAN_NOT_OK      reception failed.
 *  \return        CAN_OK          reception successful.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxBasicMsgReceivedBegin(CanRxBasicInfoPtrType rxBasicInfo);

/************************************************************************************************************
 *  CanHL_RxBasicMsgReceivedSearch()
 ***********************************************************************************************************/
/*! \brief         RX BasicCAN search
 *  \details       RX BasicCAN search logical HRH from hardware hwRxHandle.
 *                 Called by CanBasicCanMsgReceived().
 *  \param[out]    rxBasicInfo          include Hrh, controller and all LL specific settings.
 *  \return        CAN_NOT_OK      reception failed.
 *  \return        CAN_OK          reception successful.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxBasicMsgReceivedSearch(CanRxBasicInfoPtrType rxBasicInfo);

/************************************************************************************************************
 *  CanHL_RxBasicMsgReceivedEnd()
 ***********************************************************************************************************/
/*! \brief         RX BasicCAN end
 *  \details       RX BasicCAN handling end.
 *                 Called by CanBasicCanMsgReceived().
 *  \param[in]     rxBasicInfo          include controller and all LL specific settings.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxBasicMsgReceivedEnd(CanRxBasicInfoPtrType rxBasicInfo);

# if defined(C_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/************************************************************************************************************
 *  CanHL_RxBasicMsgReceivePolling()
 ***********************************************************************************************************/
/*! \brief         RX BasicCAN polling
 *  \details       RX BasicCAN polling handling.
 *                 Called by Can_MainFunctionRead().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxBasicMsgReceivePolling(CAN_HW_CHANNEL_CANTYPE_ONLY);
# endif
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_RxFullMsgReceivedBegin()
 ***********************************************************************************************************/
/*! \brief         RX FullCAN start
 *  \details       RX FullCAN handling start.
 *                 Called by CanFullCanMsgReceived().
 *  \param[in]     rxFullInfo          include controller and all LL specific settings.
 *  \return        CAN_NOT_OK      reception failed.
 *  \return        CAN_OK          reception successful.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxFullMsgReceivedBegin(CanRxFullInfoPtrType rxFullInfo);

/************************************************************************************************************
 *  CanHL_RxFullMsgReceivedSearch()
 ***********************************************************************************************************/
/*! \brief         RX FullCAN search
 *  \details       RX FullCAN search logical HRH from hardware hwTxHandle.
 *                 Called by CanFullCanMsgReceived().
 *  \param[out]    rxFullInfo          include HRH, controller and all LL specific settings.
 *  \return        CAN_NOT_OK      reception failed.
 *  \return        CAN_OK          reception successful.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_RxFullMsgReceivedSearch(CanRxFullInfoPtrType rxFullInfo);

/************************************************************************************************************
 *  CanHL_RxFullMsgReceivedEnd()
 ***********************************************************************************************************/
/*! \brief         RX FullCAN end
 *  \details       RX FullCAN handling end.
 *                 Called by CanFullCanMsgReceived().
 *  \param[in]     rxFullInfo          include controller and all LL specific settings.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxFullMsgReceivedEnd(CanRxFullInfoPtrType rxFullInfo);

# if defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/************************************************************************************************************
 *  CanHL_RxFullMsgReceivePolling()
 ***********************************************************************************************************/
/*! \brief         RX FullCAN polling
 *  \details       RX FullCAN polling handling.
 *                 Called by Can_MainFunctionRead().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxFullMsgReceivePolling(CAN_HW_CHANNEL_CANTYPE_ONLY);
# endif
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS) || defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_RxMsgReceivedNotification()
 ***********************************************************************************************************/
/*! \brief         RX notification
 *  \details       RX queue and notification handling for BasicCAN and FullCAN reception.
 *                 Called by CanBasicCanMsgReceived(),CanFullCanMsgReceived().
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxInfo          include controller and all LL specific settings.
 *  \return        errorId         error identifier reporting to DET
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_RxMsgReceivedNotification(CAN_CHANNEL_CANTYPE_FIRST CanRxInfoPtrType rxInfo);
#endif

#if defined(C_ENABLE_RX_QUEUE)
/************************************************************************************************************
 *  CanHL_RxQueueExecution()
 ***********************************************************************************************************/
/*! \brief         RX queue
 *  \details       RX queue execution and notification.
 *                 Called by Can_MainFunction_Read().
 *  \return        errorId         error identifier reporting to DET
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_RxQueueExecution(void);
#endif

/************************************************************************************************************
 *  CanHL_TxConfirmation()
 ***********************************************************************************************************/
/*! \brief         TX Confirmation handling
 *  \details       Called for a transmit or cancel event. Finish transmission by free send mailbox and call confirmation. \n
 *                 Called for FullCAN and BasicCAN handling.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \param[in]     txObjHandle   transmit hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_TxConfirmation(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS)
/************************************************************************************************************
 *  CanBasicCanMsgReceived()
 ***********************************************************************************************************/
/*! \brief         Handle reception
 *  \details       Receive BasicCAN data and call indication function.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \param[in]     rxObjHandle   received hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanBasicCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
#endif

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanFullCanMsgReceived()
 ***********************************************************************************************************/
/*! \brief         Handle reception
 *  \details       Receive FullCAN data and call indication function.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \param[in]     rxObjHandle   received hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanFullCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
#endif

/************************************************************************************************************
 *  CanHL_ErrorHandling()
 ***********************************************************************************************************/
/*! \brief         BUSOFF handling
 *  \details       Handler for bus off situation.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_ErrorHandling(CAN_HW_CHANNEL_CANTYPE_ONLY);

# if defined(C_ENABLE_SLEEP_WAKEUP)
/************************************************************************************************************
 *  CanHL_WakeUpHandling()
 ***********************************************************************************************************/
/*! \brief         WAKUP handling
 *  \details       Handler for WAKEUP event. Call EcuM for notification. Will be called in case of WAKEUP occurred.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY);
# endif

#if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */
/************************************************************************************************************
 *  CanHL_TxTaskConfPendingHandling()
 ***********************************************************************************************************/
/*! \brief         Tx task confirmation
 *  \details       Tx task handle the pending mailboxes.
 *                 Called by Can_MainFunction_Write().
 *  \param[in]     Controller    CAN controller. \n
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_TxTaskConfPendingHandling(CAN_HW_CHANNEL_CANTYPE_ONLY);
#endif

#if defined(C_ENABLE_CANCEL_TX_IN_HW) 
#endif

#define CAN_STOP_SEC_STATIC_CODE  /*----------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CAN_START_SEC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined(CAN_ENABLE_MICROSAR_VERSION_MSR403) /* COV_CAN_MICROSAR_VERSION */
/************************************************************************************************************
 *  Can_InitController()
 ***********************************************************************************************************/
/*! \brief         Initialization of controller specific CAN hardware.
 *  \details       The CAN driver registers and variables are initialized. \n
 *                 The CAN controller is fully initialized and left back within the state "STOP Mode", ready to change to "Running Mode".
 *  \param[in]     Controller            CAN controller
 *  \param[in]     ControllerConfigPtr   Pointer to the configuration data structure.
 *  \pre           Disabled Interrupts.
 *  \pre           Can_Init() has to be called.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Has to be called during the start up sequence before CAN communication takes place. \n
 *                 Must not be called while in "SLEEP Mode".
***********************************************************************************************************/
V_DEF_FUNC(V_NONE, void, CODE) Can_InitController( uint8 Controller, Can_ControllerConfigPtrType ControllerConfigPtr );
#endif

#define CAN_STOP_SEC_CODE  /*----------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CAN_LOCAL_H */

/* End of channel */

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

