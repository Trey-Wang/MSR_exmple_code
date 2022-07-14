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
 *         File:  LinIf_Types.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Declaration of types and defines of the AUTOSAR LIN Interface
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Bastian Molkenthin            visbmo        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-07-20  visbmo  ESCAN00057174 AR4 Support (derived from AR3 LinIf/LinTp)
 *  01.00.01  2012-11-30  visbmo  ESCAN00063447 Compiler error: Upper layer function declaration incorrect
 *  01.01.00  2012-12-07  visbmo  ESCAN00063142 Add precompile optimization for upper layer interfaces
 *  01.02.00  2013-04-02  visbmo  ESCAN00064305 AR4-325: Implement Post-Build Loadable
 *                                ESCAN00065539 Add precompile optimization for node configuration commands
 *  02.00.00  2013-09-10  visbmo  ESCAN00068503 AR4-260: Remove ProdErrorDetection configuration switch
 *  03.00.00  2014-04-14  visbmo  ESCAN00076251 Support of ComStackLib
 *  04.00.00  2014-08-05  visbmo  ESCAN00076772 AR4-698: Post-Build Selectable (Identity Manager)
 *                                ESCAN00076480 AR4-832: External wake up via LIN transceiver
 *  04.01.00  2015-01-07  visbmo  ESCAN00080299 FEAT-427: SafeBSW Step I
 *  04.02.00  2015-05-18  visbmo  ESCAN00081787 FEAT-1275: SafeBSW Step 2
 *  05.00.00  2015-11-05  visbmo  ESCAN00084958 Adapt to updated R14 generators (breaking change and CSL)
 *                                ESCAN00085822 Compiler error: LinTp_IsInvalidHndOfTxNSdu undefined
 *                                ESCAN00086031 Compiler error: Linker cannot found symbol LinIf_WakeupConfirmation
 *                                ESCAN00086282 Rework wakeup source handling
 *  05.01.00  2016-02-04  visbmo  ESCAN00087805 FEAT-1688: SafeBSW Step 4
 *                                ESCAN00087859 FEAT-1731: J2602 Frame Tolerance Support
 *  05.01.01  2016-07-07  visbmo  ESCAN00090787 Communication starts after external wakeup before wakeup delay has elapsed
 *  05.02.00  2017-01-19  visbmo  FEATC-565     FEAT-2161: Support for LIN drivers with different interface type
 *                                FEATC-568     FEAT-2263: LIN ScheduleTable change after frame transmission
 *                                ESCAN00093427 Support for callback to provide Lin status to application
 *                                ESCAN00093428 Support for schedule table handler information API
 *                                ESCAN00093616 Support for particular wakeup delay after external wakeup
 *********************************************************************************************************************/

#if !defined (LINIF_TYPES_H)
# define LINIF_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* include ComStack_Types.h so all needed types are known */
# include "ComStack_Types.h"
# include "EcuM.h"

/* Include types of Lin Driver for pointer to init struct */
# include "LinIf_Lin.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define LINIF_TPS_MAJOR_VERSION                    (5u)
# define LINIF_TPS_MINOR_VERSION                    (2u)
# define LINIF_TPS_PATCH_VERSION                    (0u)

# define LINIF_LINPDU_SIZE                          (8u) /* currently must be a multiple of 2 because of mask definition below */
# define LINIF_LINPDU_SIZE_MASK                     (LINIF_LINPDU_SIZE - 1)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
/* defines for LIN Driver function pointers */

typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_SendFrameType) (
  VAR(uint8, AUTOMATIC) Channel,
  P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_VAR) PduInfoPtr
);

typedef P2FUNC(Lin_StatusType, LINIF_PBCFG, LinIf_Lin_GetStatusType) (
  VAR(uint8, AUTOMATIC) Channel,
  P2VAR(Lin_u8PtrType, AUTOMATIC, LINIF_APPL_VAR) Lin_SduPtr
);

typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_CheckWakeupType) (
  VAR(uint8, AUTOMATIC) Channel
);

typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_GoToSleepType) (
  VAR(uint8, AUTOMATIC) Channel
);

typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_GoToSleepInternalType) (
  VAR(uint8, AUTOMATIC) Channel
);

typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_WakeupType) (
  VAR(uint8, AUTOMATIC) Channel
);

typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_WakeupInternalType) (
  VAR(uint8, AUTOMATIC) Channel
);

# endif

# if ( LINIF_TRCV_HANDLING == STD_ON )
/* defines for LIN Transceiver Driver function pointers */

typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_SetOpModeFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC) OpMode
);

typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_GetOpModeFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR) OpMode
);

typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_GetBusWuReasonFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR) Reason
);

typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_SetWakeupModeFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC) TrcvWakeupMode
);

typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_CheckWakeupFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex
);

# endif

typedef uint16                                LinIf_ScheduleTableTickType;
typedef uint8                                 LinIf_DetErrorIdType;

# if ( LINIF_SCHEDULE_INFO_API == STD_ON )
/* Schedule table info structure */
typedef struct LinIf_ScheduleInfoTypeTag  /*! Info structure of LinIf schedule table handler */
{
  uint8                                       CurrentTable;             /*!< Handle of schedule table, zero-based                                    */
  uint8                                       CurrentSlot;              /*!< Current slot in schedule table, Range: 0..<NumOfFramesInSchedule-1>     */
  uint8                                       RequestedTable;           /*!< Handle of requested schedule table                                      */
  LinIf_ScheduleTableTickType                 CurrentSlotLength;        /*!< Length of current schedule slot in ticks                                */
  LinIf_ScheduleTableTickType                 CurrentSlotPosition;      /*!< Position inside schedule slot in ticks, Range: 0..<CurrentSlotLength-1> */
} LinIf_ScheduleInfoType;
# endif

/**********************************************************************************************************************
 *  DATA TYPES AND STRUCTURES FOR CONFIGURABLE INTERFACES
 *********************************************************************************************************************/

/* <User>_ScheduleRequestConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_SchedReqConfFctType) (
   VAR(NetworkHandleType, AUTOMATIC)              network,
   VAR(LinIf_SchHandleType, AUTOMATIC)            schedule
);

/* ROM pointer type to schedule request notification function pointer list */
typedef P2CONST(LinIf_SchedReqConfFctType, TYPEDEF, LINIF_PBCFG)  LinIf_SchedReqConfFctListType;

/* <User>_GotoSleepConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_GotoSleepConfFctType) (
  VAR(NetworkHandleType, AUTOMATIC)               network,
  VAR(boolean, AUTOMATIC)                         success
);

/* ROM pointer type to gotosleep confirmation function pointer list */
typedef P2CONST(LinIf_GotoSleepConfFctType, TYPEDEF, LINIF_PBCFG)    LinIf_GotoSleepConfFctListType;

/* <User>_WakeupConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_WakeupConfFctType) (
  VAR(NetworkHandleType, AUTOMATIC)               network,
  VAR(boolean, AUTOMATIC)                         success
);

/* ROM pointer type to wakeup confirmation function pointer list */
typedef P2CONST(LinIf_WakeupConfFctType, TYPEDEF, LINIF_PBCFG)       LinIf_WakeupConfFctListType;

/* <User>_TriggerTransmit */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_TriggerTransmitFctType) (
  VAR(PduIdType, AUTOMATIC)                       TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_VAR)   PduInfoPtr
);

/* ROM pointer type to trigger transmit function pointer list */
typedef P2CONST(LinIf_TriggerTransmitFctType, TYPEDEF, LINIF_PBCFG)  LinIf_TriggerTransmitFctListType;

/* <User>_TxConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_TxConfFctType) (
  VAR(PduIdType, AUTOMATIC)                       TxPduId
);

/* ROM pointer type to tx confirmation function pointer list */
typedef P2CONST(LinIf_TxConfFctType, TYPEDEF, LINIF_PBCFG)           LinIf_TxConfFctListType;

/* <User>_RxIndication */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_RxIndicationFctType) (
  VAR(PduIdType, AUTOMATIC)                       RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_VAR) PduInfoPtr
);

/* ROM pointer type to rx indication function pointer list */
typedef P2CONST(LinIf_RxIndicationFctType, TYPEDEF, LINIF_PBCFG)     LinIf_RxIndicationFctListType;

/**********************************************************************************************************************
 *  RAM STRUCTURES
 *********************************************************************************************************************/

typedef uint8                                   LinIf_ChannelStateType;

/* Schedule table RAM structure */
typedef struct LinIf_SchedTabDataTypeTag /*! Control structure of LinIf schedule table handler */
{
  LinIf_ScheduleTableTickType                   LinIf_CurrentSlotDelay;             /*!< Remaining slot time in ticks                                         */
  uint8                                         LinIf_CurrentSlot;                  /*!< Current slot in schedule table, Range: 0..<NumOfFramesInSchedule-1>  */
  uint8                                         LinIf_CurrentTable;                 /*!< Handle of schedule table, zero-based per channel                     */
  uint8                                         LinIf_PreviousContinuousSlot;       /*!< Backup of schedule slot of RUN-CONTINUOUS table if RUN-ONCE active   */
  uint8                                         LinIf_PreviousContinuousTable;      /*!< Backup of schedule gandle of RUN-CONTINUOUS table if RUN-ONCE active */
  uint8                                         LinIf_RequestedTable;               /*!< Handle of current requested schedule table until it's activated      */
  uint8                                         LinIf_SchedReqConfirmTable;         /*!< Handle of schedule table to confirm to upper layer                   */
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
  uint8                                         LinIf_EvtCollResolvingState;        /*!< Event-triggered frame resolving state machine                        */
# endif
# if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
  uint8                                         LinIf_SchedEndNotifyTable;          /*!< Handle of schedule table to pass to schedule end notification        */
# endif
} LinIf_SchedTabDataType;

/* Channel RAM structure */
typedef struct LinIf_ChannelDataTypeTag /*! Control structure of LinIf for one channel */
{
  LinIf_SchedTabDataType                        LinIf_SchedTabData;                 /*!< Pointer to schedule table handling variables                         */
  LinIf_ScheduleTableTickType                   LinIf_FrameMaxDelay;                /*!< Remaining transmission duration of current frame in ticks            */
  LinIf_ChannelStateType                        LinIf_ChannelState;                 /*!< Channel state engine                                                 */
  uint16                                        LinIf_FrameHandle;                  /*!< Handle of current processed/active frame                             */
  uint8                                         LinIf_OnSleepModeFrameHandling;     /*!< Internal node management state machine                               */
  uint8                                         LinIf_CurrentFrameType;             /*!< Type of current processed/active frame                               */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  uint8                                         LinIf_AtNodeConfigurationCmd;       /*!< Node configuration command state machine                             */
# endif
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  uint8                                         LinIf_WakeupFlag;                   /*!< External wakeup detection flag. Range: 0 or 1                        */
# endif
  uint8                                         LinIf_WakeupDelayTimer;             /*!< Current countdown in ticks until start of scheduling after wakeup    */
  uint8                                         LinIf_ChannelFuncCallCycleCounter;  /*!< Countdown in ticks until invocation of channel main function         */
  uint8                                         LinIf_PduRNotification;             /*!< PduR notification pending flag; either Idle, Rx or Tx notification   */
  PduIdType                                     LinIf_PduRNotificationPduId;        /*!< PduId to pass to PduR if notification is pending                     */
  uint8                                         LinIf_PduRNotificationPduLength;    /*!< Pdu length to pass to PduR if notification is pending                */
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
  uint8                                         LinIf_PduRNotificationFctIndex;     /*!< Index of PduR notification callback function pointer list            */
# endif
  uint8                                         LinIf_RxData[LINIF_LINPDU_SIZE];    /*!< Local buffer to receive frames                                       */
  /* additional TxData buffer is required: If in the same cycle an Rx frame (of previous slot) is processed/received
     while a slot with a Tx frame is started, only one internal buffer is not sufficient */
  uint8                                         LinIf_TxData[LINIF_LINPDU_SIZE];    /*!< Local buffer to transmission frames                                  */
} LinIf_ChannelDataType;

#endif /* LINIF_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Types.h
 *********************************************************************************************************************/

