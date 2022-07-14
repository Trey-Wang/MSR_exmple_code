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
 *         File:  LinIf.c
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Implementation of the AUTOSAR LIN Interface
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
 *  01.00.01  2012-09-13  visbmo  ESCAN00061396 Use defines of LIN driver
 *  01.00.02  2012-11-30  visbmo  ESCAN00063447 Compiler error: Upper layer function declaration incorrect
 *  01.01.00  2013-01-22  visbmo  ESCAN00061783 AR4-220: Remove STATIC
 *                                ESCAN00062851 Compile error: LinIf_Wakeup and Lin_Wakeup undefined
 *                                ESCAN00063142 Add precompile optimization for upper layer interfaces
 *                                ESCAN00064415 Compiler warning: Conversion from 'uint16' to 'PduLengthType'
 *  01.01.01  2013-02-15  visbmo  ESCAN00064916 LinTp: Adapt receive buffer handling
 *                                ESCAN00065148 Compiler error: LIN_CH_OPERATIONAL is now in ASR4 LIN_OPERATIONAL
 *                                ESCAN00065330 Use LIN_CH_SLEEP instead of LIN_OPERATIONAL
 *  01.02.00  2013-04-02  visbmo  ESCAN00064305 AR4-325: Implement Post-Build Loadable
 *                                ESCAN00064584 Remove unnecessary Vector parameters
 *                                ESCAN00064929 LinTp: Accept new physical request during ongoing transmission
 *                                ESCAN00065058 AR4-315: Support IF/TP CancelTransmit, TP CancelReceive, TP ChangeParameter
 *                                ESCAN00065489 LinTp: Unexpected PduR notification of NTFRSLT_E_UNEXP_PDU
 *  02.00.00  2013-09-10  visbmo  ESCAN00068503 AR4-260: Remove ProdErrorDetection configuration switch
 *                                ESCAN00068048 Support for configurations without unconditional frames
 *                                ESCAN00069725 AR4-389: Support for multiple TP connections to the same slave
 *                                ESCAN00069898 Compiler error: MemMap.h(XXXX) : fatal error #error :  "MemMap.h, wrong pragma command"
 *                                ESCAN00067709 AR4-328: Predefined Runtime Measurement Points
 *  02.00.01  2013-12-02  visbmo  ESCAN00071844 LinTp: Incorrect PduId notified to PduR if transmission of functional request failed
 *  02.01.00  2014-01-07  visbmo  ESCAN00072802 Adapt MISRA deviations; description file updated
 *  03.00.00  2014-04-24  visbmo  ESCAN00076251 Support of ComStackLib
 *                                ESCAN00071904 AR4-619: Support the StartOfReception API (with the PduInfoType), TxConfirmation and RxIndication according ASR4.1.2
 *                                ESCAN00075079 Remove obsolete support for BRS or None upper layer component
 *                                ESCAN00075131 Unexpected slave responses not forwarded with LinTpDropNotRequestedNad set to false
 *                                ESCAN00074116 AR4-642: Deleting container at post-build time
 *                                ESCAN00074304 AR4-612: [LinSM][LinIf] Retry change to FULL_COM
 *                                ESCAN00074707 TX frame in first slot after schedule table change is incorrectly handled
 *  04.00.00  2014-10-07  visbmo  ESCAN00076772 AR4-698: Post-Build Selectable (Identity Manager)
 *                                ESCAN00076219 AR4-820: Configurable call offsets for each channel for load balancing
 *                                ESCAN00076480 AR4-832: External wake up via LIN transceiver
 *                                ESCAN00077717 Wrong schedule table activated if schedule table indices contain gaps
 *                                ESCAN00077838 Support for special handling of diagnostic broadcast requests
 *                                ESCAN00078807 Close diagnostic connection on intermediate node configuration command schedule slot
 *  04.01.00  2015-01-07  visbmo  ESCAN00080299 FEAT-427: SafeBSW Step I
 *                                ESCAN00080333 FEAT-690: LIN schedule end notification [AR4-1001]
 *                                ESCAN00080336 LinTp: Reject new physical request during ongoing transmission
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

#define LINIF_SOURCE

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* Disable the following MISRA warnings because they would appear too often for individual suppression.
   See justification at the end of file */

/* PRQA S 0715 EOF */ /* MD_MSR_1.1_715 */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0779 EOF */ /* MD_MSR_5.1_779 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "LinIf.h"
#include "LinIf_Cbk.h"

/* following code coverage justification is inverted to a tool issue: LINIF_DIAGNOSTIC_FRAMES_ONLY is never defined and justication should be XF */
#if ( LINIF_DIAGNOSTIC_FRAMES_ONLY == STD_ON ) /* COV_LINIF_FBL_USECASE TX */
#else
# include "PduR_LinIf.h"
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# include "PduR_LinTp.h"
#endif

/* following code coverage justification is inverted to a tool issue: LINIF_DUMMY_STATEMENT is always defined and justication should be TX */
#if defined ( LINIF_DUMMY_STATEMENT ) /* COV_LINIF_COMPATIBILITY XF */
#else
# define LINIF_DUMMY_STATEMENT(c)
#endif

#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif

#include "SchM_LinIf.h"

#if ( LINIF_USE_INIT_POINTER == STD_ON )
# include "EcuM_Error.h"
#else
# if ( LINIF_TP_SUPPORTED == STD_ON )
#  if ( LINTP_USE_INIT_POINTER == STD_ON )
#   include "EcuM_Error.h"
#  endif
# endif
#endif

#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_ON ) || ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
/* necessary if LinSM functions are called directly from LinIf.c and not via function tables from generated data or end notification used */
# include "LinSM_Cbk.h"
#endif

#if ( LINIF_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
# include "Rtm.h"
#else
# if ( LINIF_TP_SUPPORTED == STD_ON )
#  if ( LINTP_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
#   include "Rtm.h"
#  endif
# endif
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

#if ( LINIF_SW_MAJOR_VERSION != (5u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf.h are inconsistent (LINIF_SW_MAJOR_VERSION)!"
#endif
#if ( LINIF_SW_MINOR_VERSION != (2u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf.h are inconsistent (LINIF_SW_MINOR_VERSION)!"
#endif
#if ( LINIF_SW_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf.h are inconsistent (LINIF_SW_PATCH_VERSION)!"
#endif

#if ( LINIF_TPS_MAJOR_VERSION != (5u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Types.h are inconsistent (LINIF_TPS_MAJOR_VERSION)!"
#endif
#if ( LINIF_TPS_MINOR_VERSION != (2u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Types.h are inconsistent (LINIF_TPS_MINOR_VERSION)!"
#endif
#if ( LINIF_TPS_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Types.h are inconsistent (LINIF_TPS_PATCH_VERSION)!"
#endif

#if ( LINIF_CBK_MAJOR_VERSION != (5u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Cbk.h are inconsistent (LINIF_CBK_MAJOR_VERSION)!"
#endif
#if ( LINIF_CBK_MINOR_VERSION != (2u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Cbk.h are inconsistent (LINIF_CBK_MINOR_VERSION)!"
#endif
#if ( LINIF_CBK_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Cbk.h are inconsistent (LINIF_CBK_PATCH_VERSION)!"
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# if ( LINTP_TPS_MAJOR_VERSION != (5u) ) /* COV_LINIF_CONFIG XF */
#  error "Source and Header file LinTp_Types.h are inconsistent (LINTP_TPS_MAJOR_VERSION)!"
# endif
# if ( LINTP_TPS_MINOR_VERSION != (2u) ) /* COV_LINIF_CONFIG XF */
#  error "Source and Header file LinTp_Types.h are inconsistent (LINTP_TPS_MINOR_VERSION)!"
# endif
# if ( LINTP_TPS_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
#  error "Source and Header file LinTp_Types.h are inconsistent (LINTP_TPS_PATCH_VERSION)!"
# endif
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 **********************************************************************************************************************/

#if defined ( LINIF_CONFIGURATION_VARIANT ) /* COV_LINIF_CONFIG TX */
/* following code coverage justification is inverted to a tool issue: LINIF_CONFIGURATION_VARIANT is always one of the two defines so justication should be TX tf tf */
# if ( ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_PRECOMPILE ) || \
       ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) ) /* COV_LINIF_CONFIG XF tf tf */
# else
#  error "LINIF_CONFIGURATION_VARIANT has an invalid value, check settings in generation tool!"
# endif
#else
# error "LINIF_CONFIGURATION_VARIANT not defined, check settings in generation tool!"
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# if defined ( LINTP_CONFIGURATION_VARIANT ) /* COV_LINIF_CONFIG TX */
/* following code coverage justification is inverted to a tool issue:
   LINTP_CONFIGURATION_VARIANT is always defined at this point with one of the two defines so justication should be TX tf tf */
#  if ( ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_PRECOMPILE ) || \
        ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) ) /* COV_LINIF_CONFIG XF tf tf */
#  else
#   error "LINTP_CONFIGURATION_VARIANT has an invalid value, check settings in generation tool!"
#  endif
# else
#  error "LINTP_CONFIGURATION_VARIANT not defined, check settings in generation tool!"
# endif

/* LINTP_LOCAL_BUFFER_SIZE is statically defined. The check is for SafeBsw and a hint that a change has impact on implementation */
# if defined ( LINTP_LOCAL_BUFFER_SIZE ) /* COV_LINIF_CONFIG TX */
#  if ( LINTP_LOCAL_BUFFER_SIZE != 8 ) /* COV_LINIF_CONFIG XF */
#   error "LINTP_LOCAL_BUFFER_SIZE shall be 8. If this changes, adapt the access of LinTp_Tx_Rx_Buffer."
#  endif
# else
#  error "LINTP_LOCAL_BUFFER_SIZE is not defined".
# endif
#endif

/* LINIF_LINPDU_SIZE is statically defined. The check is for SafeBsw and a hint that a change has impact on implementation */
#if defined ( LINIF_LINPDU_SIZE ) /* COV_LINIF_CONFIG TX */
# if ( LINIF_LINPDU_SIZE != 8 ) /* COV_LINIF_CONFIG XF */
#  error "LINIF_LINPDU_SIZE shall be 8. If this changes, adapt the access of Lin Pdus."
# endif
#else
# error "LINIF_LINPDU_SIZE is not defined."
#endif

#if ! defined ( LINTP_CANCEL_RECEIVE_SUPPORTED )
# define LINTP_CANCEL_RECEIVE_SUPPORTED             STD_ON
#endif

#if ! defined ( LINTP_CANCEL_TRANSMIT_SUPPORTED )
# define LINTP_CANCEL_TRANSMIT_SUPPORTED            STD_ON
#endif

#if ! defined ( LINTP_CHANGE_PARAMETER_SUPPORTED )
# define LINTP_CHANGE_PARAMETER_SUPPORTED           STD_ON
#endif

#if ! defined ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED )
# define LINTP_FUNCTIONAL_REQUEST_SUPPORTED         STD_ON
#endif

#if ! defined ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR )
# define LINTP_FORWARD_RESPONSEPENDING_TO_PDUR      STD_ON
#endif

#if ! defined ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY )
# define LINIF_WAKEUPCONF_AR403_COMPATIBILITY       STD_OFF
#endif

/* AR4-612 RFC57401 requires to check the return value of <UL>_ TriggerTransmit and to discard the slot if E_NOT_OK is
   returned. Per default, BswM is configured in such a way that the Pdu groups are disabled / enabled inside the
   schedule request confirmation callback. Because TriggerTransmit() is called before the schedule confirmation due to
   optimization reasons, it returns E_NOT_OK if the first frame inside a schedule is a Tx frame.
   Per default, the return value of TriggerTransmit is then ignored and the frame is transmitted, because MSR COM copies
   the data to the buffer even if it returns E_NOT_OK. To support RFC57401, define LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL
   to STD_OFF in a user config file. Special care has to be taken when configuring BswM in this case. */
#if ! defined ( LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL )
# define LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL        STD_ON
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
#if !defined ( LINIF_LOCAL ) /* COV_LINIF_COMPATIBILITY TX */
# define LINIF_LOCAL                                static
#endif

#if !defined ( LINIF_LOCAL_INLINE ) /* COV_LINIF_COMPATIBILITY TX */
# define LINIF_LOCAL_INLINE                         LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef uint8                                       LinIf_ScheduleTableHandlerRetType;
#define LinIf_SchdlHdlrRet_OnNullSchedule           ((LinIf_ScheduleTableHandlerRetType)0x00u)
#define LinIf_SchdlHdlrRet_NoNewMessage             ((LinIf_ScheduleTableHandlerRetType)0x01u)
#define LinIf_SchdlHdlrRet_NewMessage               ((LinIf_ScheduleTableHandlerRetType)0x02u)

/*! LinIf NULL schedule table
 * \trace CREQ-601
 */
#define LinIf_Schedule_NULL                         (NULL_SCHEDULE)
#define LinIf_Schedule_Invalid                      ((LinIf_SchHandleType)0xFFu)

#define LinIf_Channel_Invalid                       ((NetworkHandleType)0xFFu)
#define LinTp_Channel_Invalid                       ((NetworkHandleType)0xFFu)

/* List of TP frame types */
#define LinTp_FrameType_SF                          ((uint8)0x00u)
#define LinTp_FrameType_FF                          ((uint8)0x10u)
#define LinTp_FrameType_CF                          ((uint8)0x20u)

/* Node configuration command handling */
#define LinIf_AtNodeConfigurationCmd_Idle           ((uint8)0x00u)
#define LinIf_AtNodeConfigurationCmd_SendMrf        ((uint8)0x01u)
#define LinIf_AtNodeConfigurationCmd_SkipSrf        ((uint8)0x02u)
#define LinIf_AtNodeConfigurationCmd_SendSrf        ((uint8)0x03u)

/* Sleep / Wakeup handling */
#define LinIf_OnSleepModeFrameHandling_Idle         ((uint8)0x00u)
#define LinIf_OnSleepModeFrameHandling_OnReq        ((uint8)0x01u)
#define LinIf_OnSleepModeFrameHandling_OnTrans      ((uint8)0x02u)
#define LinIf_OnSleepModeFrameHandling_OnReWakeInt  ((uint8)0x03u)
#define LinIf_OnSleepModeFrameHandling_OnReqInt     ((uint8)0x04u)
#define LinIf_OnWakeupFrameHandling_OnTrans         ((uint8)0x05u)

/* Schedule table resume position */
#define LinIf_CONTINUE_AT_IT_POINT                  ((uint8)0x00u)
#define LinIf_START_FROM_BEGINNING                  ((uint8)0x01u)

/* Startup state */
#define LinIf_StartupState_Sleep                    ((uint8)0x00u)
#define LinIf_StartupState_Normal                   ((uint8)0x01u)

#define LinIf_PduRNotificationIdle                  ((uint8)0x00u)
#define LinIf_PduRNotificationTx                    ((uint8)0x01u)
#define LinIf_PduRNotificationRx                    ((uint8)0x02u)

#define LinTp_PduRNotificationIdle                  ((uint8)0x00u)
#define LinTp_PduRNotificationTx                    ((uint8)0x01u)
#define LinTp_PduRNotificationTxTm                  ((uint8)0x02u)
#define LinTp_PduRNotificationRx                    ((uint8)0x10u)

#define LinTp_PortNotificationIdle                  ((uint8)0xFFu)

/* event-triggered collision resolving table states */
#define LinIf_EvtCollResolvingState_Idle            ((uint8)0x00u)
#define LinIf_EvtCollResolvingState_Detected        ((uint8)0x01u)
#define LinIf_EvtCollResolvingState_OnResolving     ((uint8)0x02u)

#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
# define LinIf_WakeupSourceModule_None              ((uint8)0x00u)
# define LinIf_WakeupSourceModule_Driver            ((uint8)0x01u)
# define LinIf_WakeupSourceModule_Trcv              ((uint8)0x02u)
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
/* LIN TP sub channel state engine */
/*! LinTp channel state IDLE
 * \trace CREQ-603
 */
# define LINTP_CHANNEL_IDLE                       ((uint8)0x00u) /* no diagnostic communication active */
/* LIN TP sub channel state engine */
/*! LinTp channel state BUSY
 * \trace CREQ-604
 */
# define LINTP_CHANNEL_BUSY_TX_INVALID            ((uint8)0x10u) /* transmission data currently not available, pending */
# define LINTP_CHANNEL_BUSY_TX_VALID              ((uint8)0x11u) /* transmission ongoing */
# define LINTP_CHANNEL_BUSY_RX                    ((uint8)0x20u) /* reception ongoing */
# define LINTP_CHANNEL_BUSY_RX_SUSPENDED          ((uint8)0x21u) /* currently no buffer for reception available suppress SRF header transmission */
# define LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START     ((uint8)0x22u) /* a new reception is started but PduR not informed yet */
# define LINTP_CHANNEL_BUSY_RX_POLLBUFFER         ((uint8)0x23u) /* schedule next SRF header but poll for receive buffer after reception */

# define LINTP_CHANNEL_TX_MSK                     ((uint8)0x10u)
# define LINTP_CHANNEL_RX_MSK                     ((uint8)0x20u)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#if ( LINIF_USE_INIT_POINTER == STD_ON )
# define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Pointer to global LinIf Configuration */
LINIF_LOCAL P2CONST(LinIf_ConfigType, LINIF_VAR_ZERO_INIT, LINIF_INIT_DATA)     LinIf_ConfigDataPtr = NULL_PTR;

# define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# if ( LINTP_USE_INIT_POINTER == STD_ON )
#  define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Pointer to global LinTp Configuration */
LINIF_LOCAL P2CONST(LinTp_ConfigType, LINTP_VAR_ZERO_INIT, LINTP_INIT_DATA)     LinTp_ConfigDataPtr = NULL_PTR;

#  define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
# endif
#endif

#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
# define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define LINIF_UNINIT                             ((uint8)0x00u)
# define LINIF_INIT                               ((uint8)0xA5u)

/*! Initialization check variable for LinIf. Must be set after reset, either global here (if supported) or in LinIf_InitMemory */
LINIF_LOCAL VAR(uint8, LINIF_VAR_ZERO_INIT)       LinIf_Dev_InitDetect  = LINIF_UNINIT;

# if ( LINIF_TP_SUPPORTED == STD_ON )
#  define LINTP_UNINIT                            ((uint8)0x00u)
#  define LINTP_INIT                              ((uint8)0xA5u)

/*! Initialization check variable for LinTp. Must be set after reset, either global here (if supported) or in LinIf_InitMemory */
LINIF_LOCAL VAR(uint8, LINIF_VAR_ZERO_INIT)       LinTp_Dev_InitDetect  = LINTP_UNINIT;
# endif

# define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define LINIF_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL LINIF FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_ChannelInit()
 **********************************************************************************************************************/
/*! \brief       Initializes a LinIf channel.
 *  \details     Initializes the variables of a specific channel of the component.
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelInit
(
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_ChannelMainFunction()
 **********************************************************************************************************************/
/*! \brief       Performs channel specific main function handling.
 *  \details     Performs schedule and message processing for one channel.
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_FlushAllSchedules()
 **********************************************************************************************************************/
/*! \brief       Resets the referenced schedule table data.
 *  \details     Sets the referenced schedule table data to initial default values. It also sets the NULL
 *               schedule table and sets the collision resolving state to idle.
 *  \param[in]   LinIf_SchedTabDataPtr     Pointer to schedule table data structure.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL FUNC(void, LINIF_CODE) LinIf_FlushAllSchedules
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)       LinIf_SchedTabDataPtr
);

/***********************************************************************************************************************
 *  LinIf_ScheduleTableHandler()
 **********************************************************************************************************************/
/*! \brief       Handles the current schedule table.
 *  \details     Checks the current schedule state and performs the switch to the next slot or to another schedule table.
 *  \param[out]  LinIf_FrameMaxDelayPtr              Pointer to a delay value in ticks of the next schedule slot.
 *  \param[out]  LinIf_FrameHandlePtr                Pointer to a global frame index of the frame in the next schedule slot.
 *  \param[in]   LinIf_LinIfChannel                  Local LinIf channel identifier
 *  \pre         -
 *  \return      LinIf_SchdlHdlrRet_OnNullSchedule   NULL table is active
 *  \return      LinIf_SchdlHdlrRet_NoNewMessage     No new message is started
 *  \return      LinIf_SchdlHdlrRet_NewMessage       A new message is started. This also means a new schedule slot begins.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(LinIf_ScheduleTableHandlerRetType, LINIF_CODE) LinIf_ScheduleTableHandler
(
  CONSTP2VAR(LinIf_ScheduleTableTickType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_FrameMaxDelayPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, AUTOMATIC)       LinIf_FrameHandlePtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RunOnceFinalization()
 **********************************************************************************************************************/
/*! \brief       Checks if a run-once schedule table is finished and completes the handling.
 *  \details     Checks if a run-once schedule table is finished in current slot and switches back to
 *               previous schedule table and slot. Also prepares the schedule change notification to upper layer.
 *  \param[in]   LinIf_SchedTabDataPtr               Pointer to current schedule table data structure
 *  \param[in]   LinIf_LinIfChannel                  Local LinIf channel identifier
 *  \pre         -
 *  \pre         Must only be called if a new schedule slot is due in current cycle.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RunOnceFinalization
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)       LinIf_SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RequestedTableSwitch()
 **********************************************************************************************************************/
/*! \brief       Performs the switch to a new schedule table if a request is pending.
 *  \details     Performs the actual switch of a pending schedule table request if allowed. Activates the
 *               new table and slot index. It stores backup information if the new table is of type run-once or is an
 *               event-triggered frame collision table and prepares the schedule change notification to upper layer.
 *  \param[in]   LinIf_SchedTabDataPtr               Pointer to current schedule table data structure
 *  \param[in]   LinIf_LinIfChannel                  Local LinIf channel identifier
 *  \pre         -
 *  \pre         Must only be called if a new schedule slot is due in current cycle.
 *  \pre         Must be called after LinIf_ScheduleTableHandler_RunOnceFinalization.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RequestedTableSwitch
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)       LinIf_SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_CurrentMessageProcessing()
 **********************************************************************************************************************/
/*! \brief       Processes the currently transmitted or received message.
 *  \details     Polls the driver for status of the currently transmitted message and finishes the handling
 *               of the current schedule slot.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_CurrentMessageProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_MessageProcessing_Unconditional()
 **********************************************************************************************************************/
/*! \brief       Processes a transmitted or received unconditional frame in the current schedule slot.
 *  \details     Handles an unconditional frame in the current slot. If the frame is Tx, the confirmation to upper layer
 *               is prepared and sporadic frame flags are updated. If it's Rx, the received data is copied to local
 *               buffer and indication to upper layer is prepared.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \param[in]   LinIf_LinSduPtr           Pointer to current Lin SDU received from driver
 *  \param[in]   LinIf_LinStatus           Status value from driver of currently handled frame
 *  \pre         An unconditional frame was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_Unconditional
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel,
#endif
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinIf_LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                    LinIf_LinStatus
);

#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinIf_MessageProcessing_EventTriggeredFrame()
 **********************************************************************************************************************/
/*! \brief       Processes an elapsed event-triggered frame slot in the current schedule slot.
 *  \details     Depending on the reported status of the driver, the event-triggered frame is either matched with the
 *               corresponding unconditional frame and indicated to upper layer or discarded and the collision resolving
 *               schedule table is set as pending.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \param[in]   LinIf_LinSduPtr           Pointer to current Lin SDU received from driver
 *  \param[in]   LinIf_LinStatus           Status value from driver of currently handled frame
 *  \pre         An event-triggered frame was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Function only exists if support for event-triggered frames is enabled.
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_EventTriggeredFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinIf_LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                    LinIf_LinStatus
);
#endif

/***********************************************************************************************************************
 *  LinIf_MessageProcessing_SleepModeFrame()
 **********************************************************************************************************************/
/*! \brief       Processes a transmitted sleep mode frame in the current schedule slot.
 *  \details     Performs the transition to sleep state and reinits the schedule table data if the driver indicates sleep status.
 *               In case a wakeup request occured during sleep frame transmission, the wakeup procedure is restarted.
 *               Prepares a sleep confirmation to upper layer if necessary.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \param[in]   LinIf_LinStatus           Status value from driver of currently handled frame
 *  \pre         A sleep mode frame was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SleepModeFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                  LinIf_LinStatus
);

/***********************************************************************************************************************
 *  LinIf_MessageProcessing_SRF()
 **********************************************************************************************************************/
/*! \brief       Processes a received slave response frame in the current schedule slot.
 *  \details     On successful reception, either forwards the frame to LinTp, completes a node configuration command or is
 *               ignored if no request was transmitted previously. Report DET errors if necessary.
 *  \param[in]   LinIf_ChannelDataPtr     Pointer to current channel data structure
 *  \param[in]   LinIf_LinSduPtr          Pointer to current Lin SDU received from driver
 *  \param[in]   LinIf_LinIfChannel       Local LinIf channel identifier
 *  \param[in]   LinIf_LinStatus          Status value from driver of currently handled frame
 *  \pre         A slave response header shall was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SRF
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                       LinIf_LinSduPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                  LinIf_LinStatus
);

#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SporadicFrame()
 **********************************************************************************************************************/
/*! \brief       Searches for a flagged pending frame to be transmitted in current sporadic slot.
 *  \details     If frame marked as pending is found, updates the slot data with information of this frame.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinPduPtr           Pointer to Lin PDU to be passed to driver
 *  \param[out]  LinIf_NewFrameHandlePtr   Pointer to frame handle of selected sporadic frame. Only valid if function
 *                                         returns TRUE
 *  \returns     TRUE                      if frame to be transmitted in this sporadic slot was found
 *  \returns     FALSE                     if no frame is marked as pending in this sporadic slot
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SporadicFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)          LinIf_ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                    LinIf_LinPduPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_NewFrameHandlePtr
);
#endif

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SleepHandling()
 **********************************************************************************************************************/
/*! \brief       Checks if a sleep request is pending for current schedule slot and starts the sleep transition.
 *  \details     Triggers the driver to transit to sleep state, either by internal transition or with sleep frame transmission.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \returns     TRUE                      A sleep request is handled in current schedule slot
 *  \returns     FALSE                     No sleep handling is performed in current schedule slot
 *  \pre         This function shall only be called if a new schedule table slot starts.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SleepHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_UnconditionalFrame()
 **********************************************************************************************************************/
/*! \brief       Prepares an unconditional Tx frame for transmission in current slot.
 *  \details     Triggers PduR for provision of transmission data.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinPduPtr           Pointer to Lin PDU to be passed to driver
 *  \param[in]   LinIf_FrameHandle         Handle of unconditional frame to be processed.
 *  \returns     TRUE                      The header of this slot shall be transmitted, data was copied to LIN Pdu
 *  \returns     FALSE                     No header shall be transmitted, no transmission data available
 *  \pre         This function shall only be called if a new schedule table slot starts.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_UnconditionalFrame
(
  CONSTP2CONST(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)              LinIf_LinPduPtr,
  VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC)                     LinIf_FrameHandle
);

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_FrameHandling()
 **********************************************************************************************************************/
/*! \brief         Handles a new/starting schedule table slot depending on the frame type.
 *  \details       Depending on the frame type, transmission data is requested and conditions are checked if a header or
 *                 complete frame shall be transmitted in the slot or if the slot shall remain empty.
 *  \param[in]     LinIf_ChannelDataPtr   Pointer to current channel data structure
 *  \param[in]     LinIf_LinPduPtr        Pointer to Lin PDU to be passed to driver
 *  \param[in,out] LinIf_FrameHandlePtr   Pointer to frame handle to be processed in current slot. Might be overwritten
 *                                        if frame is replaced by another frame in this slot.
 *  \param[in]     LinTp_CtrlPtr          LIN TP channel dependent control structure. Only available if TP is supported.
 *  \returns       TRUE                   Slot shall be handled and header and/or response shall be transmitted
 *  \returns       FALSE                  Slot shall remain silent
 *  \pre           This function shall only be called if a new schedule table slot starts.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_FrameHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)              LinIf_LinPduPtr,
  P2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT) LinIf_FrameHandlePtr
#if ( LINIF_TP_SUPPORTED == STD_ON )
  ,CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)       LinTp_CtrlPtr
#endif
);

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing()
 **********************************************************************************************************************/
/*! \brief       Processes schedule tables and slots.
 *  \details     Performs general processing of the current schedule slot and triggers the transmission of
 *               header and response. Called every main function cycle if channel is operational.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel
);

/***********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessing()
 **********************************************************************************************************************/
#if defined ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING )
/* declared in LinIf.h */
#else
/***********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessing()
 **********************************************************************************************************************/
/*! \brief       Notifies the upper layers after each main processing function cycle of the LIN interface.
 *  \details     Performs all notification and indication to upper layers. It may be used to decouple the
 *               main processing (frame and schedule handling) from the interactions with upper layer modules.
 *               By default this function is called directly from LinIf_MainFunction().
 *               The possibility to call this function from outside is optional and should only be enabled per user
 *               config file after clarification with Vector. In this case it is mandatory to call it once after each call
 *               of LinIf_Mainfunction().
 *  \pre         This module must be initialized before this service function is called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-569
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessing
(
  void
);
#endif

/***********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessing_LinSM()
 **********************************************************************************************************************/
/*! \brief       Calls necessary indication and confirmation functions of LinSM module.
 *  \details     Indicates schedule end and schedule change confirmation to LinSM.
 *  \param[in]   LinIf_ChannelDataPtr      Pointer to current channel data structure
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \pre         Schedule and message processing was already performed for this cycle.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessing_LinSM
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel
);

#if ( LINIF_TP_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LOCAL LINTP FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_RxDataBufferHandling
 **********************************************************************************************************************/
/*! \brief       Handles the received data and passes it to PduR.
 *  \details     Passes pending local received data to PduR and performs delayed buffer retry handling.
 *  \param[in]   LinTp_CtrlPtr         Pointer to Tp channel control structure
 *  \param[in]   LinTp_LinSduPtr       Source pointer (SRF buffer), provide null at repeated call to work on local buffer
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinTp_LinSduPtr
);

/***********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_CopyRxData()
 **********************************************************************************************************************/
/*! \brief       Handles the interface with PduR for reception data.
 *  \details     Provides PduR with TP reception data and handles internal buffer states.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_CopyRxData
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       LinTp_CtrlPtr
);

/***********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_StartOfReception()
 **********************************************************************************************************************/
/*! \brief       Handles the interface with PduR for setting up a receive connection.
 *  \details     Initiates a Tp receive connection to PduR.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_StartOfReception
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       LinTp_CtrlPtr
);

/***********************************************************************************************************************
 *  LinTp_RxIndication
 **********************************************************************************************************************/
/*! \brief       Handles the reception of a SRF.
 *  \details     This function processes a received SRF containing transport protocol data.
 *  \param[in]   LinIf_LinIfChannel        Local LinIf channel identifier
 *  \param[in]   LinTp_LinSduPtr           Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-597
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication
(
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinTp_LinSduPtr
);

/***********************************************************************************************************************
 *  LinTp_RxIndication_SF()
 **********************************************************************************************************************/
/*! \brief       Handles the reception of a single frame N-PDU.
 *  \details     This function processes a received SRF containing a single frame. Terminates a possible ongoing reception
 *               on the same channel, performs response pending frame handling if necessary and initiates a new receive
 *               connection.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinTp_LinSduPtr           Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-593
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication_SF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinTp_LinSduPtr
);

/***********************************************************************************************************************
 *  LinTp_RxIndication_FF()
 **********************************************************************************************************************/
/*! \brief       Handles the reception of a first frame N-PDU.
 *  \details     This function processes a received SRF containing a first frame. Terminates a possible ongoing reception
 *               on the same channel and initiates a new receive connection.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinTp_LinSduPtr           Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication_FF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinTp_LinSduPtr
);

/***********************************************************************************************************************
 *  LinTp_RxIndication_CF()
 **********************************************************************************************************************/
/*! \brief       Handles the reception of a consecutive frame N-PDU.
 *  \details     This function processes a received SRF containing a first frame. Check for a valid sequence number and
 *               passes received data to buffer handling on active connection.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinTp_LinSduPtr           Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication_CF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinTp_LinSduPtr
);

/***********************************************************************************************************************
 *  LinTp_RxCloseConnection
 **********************************************************************************************************************/
/*! \brief       Closes a Tp Rx connection.
 *  \details     Resets a Rx connection back to idle and notifies the PduR. Does not check if an Rx
 *               connection is active.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \pre         An Rx connection shall be active.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL FUNC(void, LINIF_CODE) LinTp_RxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
);

/***********************************************************************************************************************
 *  LinTp_TxDataRequest
 **********************************************************************************************************************/
/*! \brief       Handles the transmission data retrieval from PduR.
 *  \details     Requests data from PduR and store it to local LinTp buffer.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \return      E_OK                      Data retrieved or request pending, connection to PduR is alive
 *  \return      E_NOT_OK                  Data request denied, PduR has aborted current connection
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxDataRequest
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
);

/***********************************************************************************************************************
 *  LinTp_TxCloseConnection
 **********************************************************************************************************************/
/*! \brief       Closes a Tp Tx connection.
 *  \details     Resets a Tx connection back to idle and notifies the PduR. It does not check if a Tx
 *               connection is active.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \pre         A Tx connection shall be active.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL FUNC(void, LINIF_CODE) LinTp_TxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
);

/***********************************************************************************************************************
 *  LinTp_TxMessageProcessing()
 **********************************************************************************************************************/
/*! \brief       Post-processes the transmission of a MRF containing transport protocol data.
 *  \details     Closes connection after transmission of last frame of connection, otherwise prepares next consecutive
 *               frame for transmission.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinTp_LinStatus           Status value from driver for transmitted MRF
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_TxMessageProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                    LinTp_LinStatus
);

/***********************************************************************************************************************
 *  LinTp_BufferPolling()
 **********************************************************************************************************************/
/*! \brief       Requests data from PduR for current Tp connection, both Rx and Tx.
 *  \details     Polls for suspended buffer handling. Handles Rx and Tx connections.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Function can be polled.
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_BufferPolling
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
);

/***********************************************************************************************************************
 *  LinTp_TxScheduleProcessing()
 **********************************************************************************************************************/
/*! \brief       Prepares a MRF for transmission used by transport protocol.
 *  \details     Checks whether TP transmission data is available for a connection on given channel and prepares the given
 *               LIN PDU.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinTp_IfChannelDataPtr    Pointer to current channel data
 *  \param[in]   LinTp_LinPduPtr           Pointer to Lin PDU to be passed to driver
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinTp_TxScheduleProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr,
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinTp_IfChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)              LinTp_LinPduPtr
);

/***********************************************************************************************************************
 *  LinTp_CloseActiveConnection()
 **********************************************************************************************************************/
/*! \brief       Terminates an ongoing Rx or Tx connection.
 *  \details     Aborts an active TP connection on given channel and notify upper layer. Afterwards, the TP channel state
 *               is idle.
 *  \param[in]   LinTp_CtrlPtr             Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_CloseActiveConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
);

#endif

/**********************************************************************************************************************
 *  LOCAL LINIF FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LOCAL FUNCTIONS - LinIf Schedule Table Handler
 *
 * The LinIf schedule table handler determines the current schedule position and checks if a new slot is due. It also
 * checks if a new schedule table is requested or if the current table is run through. If necessary, the actual switch
 * to a new schedule table or schedule slot is performed.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RunOnceFinalization()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RunOnceFinalization
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)         LinIf_SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                       LinIf_LinIfChannel
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If current schedule table is of type run-once and run through */
  if ( ( LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(LinIf_LinIfChannel)) +
         LinIf_SchedTabDataPtr->LinIf_CurrentTable) == LinIf_RunMode_RunOnce ) &&
       ( LinIf_SchedTabDataPtr->LinIf_CurrentSlot == (uint8)0x00u ) )
  {
    /* Current table is run once and table is all run through */

    /* #20 Change to to last continuous schedule table and saved resume slot */
    LinIf_SchedTabDataPtr->LinIf_CurrentTable  = LinIf_SchedTabDataPtr->LinIf_PreviousContinuousTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    if ( LinIf_GetResumePositionOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(LinIf_LinIfChannel)) +
          LinIf_SchedTabDataPtr->LinIf_CurrentTable ) == LinIf_CONTINUE_AT_IT_POINT )
    {
      /* Start from interrupted slot */
      LinIf_SchedTabDataPtr->LinIf_CurrentSlot = LinIf_SchedTabDataPtr->LinIf_PreviousContinuousSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }
    else
    {
      /* Start from beginning slot */
      LinIf_SchedTabDataPtr->LinIf_CurrentSlot = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }

    /* #30 If no schedule request is pending, inform about change to continuous table */
    if ( LinIf_SchedTabDataPtr->LinIf_RequestedTable == (uint8)LinIf_Schedule_Invalid )
    {
      LinIf_SchedTabDataPtr->LinIf_SchedReqConfirmTable = LinIf_SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }
  }
}

/***********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RequestedTableSwitch()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RequestedTableSwitch
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)         LinIf_SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                       LinIf_LinIfChannel
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If a new table is requested */
  if ( LinIf_SchedTabDataPtr->LinIf_RequestedTable != LinIf_Schedule_Invalid )
  {
    /* #20 If current active table is of type run-continuous */
    if ( LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(LinIf_LinIfChannel)) +
         LinIf_SchedTabDataPtr->LinIf_CurrentTable ) == LinIf_RunMode_RunContinuous )
    {
      /* #30 A normal continuous table is active, switch to requested schedule */
      if ( LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(LinIf_LinIfChannel)) +
           LinIf_SchedTabDataPtr->LinIf_RequestedTable) == LinIf_RunMode_RunOnce )
      { /* #40 If requested table is a run-once table, save position of current table */
        LinIf_SchedTabDataPtr->LinIf_PreviousContinuousTable   = LinIf_SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        LinIf_SchedTabDataPtr->LinIf_PreviousContinuousSlot    = LinIf_SchedTabDataPtr->LinIf_CurrentSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
      else if ( LinIf_SchedTabDataPtr->LinIf_EvtCollResolvingState == LinIf_EvtCollResolvingState_Detected )
      { /* #50 If new table is a collision resolving schedule, save position of current table */
        LinIf_SchedTabDataPtr->LinIf_PreviousContinuousTable   = LinIf_SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        LinIf_SchedTabDataPtr->LinIf_PreviousContinuousSlot    = LinIf_SchedTabDataPtr->LinIf_CurrentSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        LinIf_SchedTabDataPtr->LinIf_EvtCollResolvingState     = LinIf_EvtCollResolvingState_OnResolving; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
      else
      {
        /* MISRA only */
      }
#endif
      /* #60 Activate new schedule table and slot */
      LinIf_SchedTabDataPtr->LinIf_CurrentTable                = LinIf_SchedTabDataPtr->LinIf_RequestedTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      LinIf_SchedTabDataPtr->LinIf_CurrentSlot                 = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      /* schedule table change handled, remove request */
      LinIf_SchedTabDataPtr->LinIf_RequestedTable              = LinIf_Schedule_Invalid; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      /* #70 Confirm schedule request, only if new schedule is not a collision resolving table */
#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
      if ( LinIf_SchedTabDataPtr->LinIf_EvtCollResolvingState != LinIf_EvtCollResolvingState_OnResolving )
#endif
      { /* inform upper layer about schedule table change */
        LinIf_SchedTabDataPtr->LinIf_SchedReqConfirmTable      = LinIf_SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
    }
    /* else : currently a run once table is active which is not run through yet so wait until table end */
  }
}

/***********************************************************************************************************************
 *  LinIf_ScheduleTableHandler()
 **********************************************************************************************************************/
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
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(LinIf_ScheduleTableHandlerRetType, LINIF_CODE) LinIf_ScheduleTableHandler
(
  CONSTP2VAR(LinIf_ScheduleTableTickType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_FrameMaxDelayPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, AUTOMATIC)         LinIf_FrameHandlePtr,
  VAR(NetworkHandleType, AUTOMATIC)                                       LinIf_LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)              schedTabDataPtrTmp;
  LinIf_ScheduleTableListIndStartIdxOfChannelConfigType                   schedTabListIdx;
  LinIf_EntryStartIdxOfScheduleTableListType                              slotListIdx;
  LinIf_ScheduleTableHandlerRetType                                       retVal;
  LinIf_RunModeOfScheduleTableListType                                    schedRunMode;
#if ( LINIF_J2602_FRAME_TOLERANCE_SUPPORT == STD_ON )
#else
  uint8                                                                   frameLengthTmp;
#endif

  /* ----- Implementation ----------------------------------------------- */
  schedTabDataPtrTmp    = &(LinIf_GetChannelData(LinIf_LinIfChannel).LinIf_SchedTabData);

  /* #10 If Null schedule table is active and no request is pending, do nothing and quit */
  if ( (schedTabDataPtrTmp->LinIf_CurrentTable == LinIf_Schedule_NULL) &&
       (schedTabDataPtrTmp->LinIf_RequestedTable == LinIf_Schedule_Invalid) )
  {
    retVal              = LinIf_SchdlHdlrRet_OnNullSchedule;
  }
  /* #20 Otherwise: */
  else
  {
    /* #30 If current slot not yet finished, decrease delay counter */
    if ( schedTabDataPtrTmp->LinIf_CurrentSlotDelay > (LinIf_ScheduleTableTickType)0x00u )
    {
      schedTabDataPtrTmp->LinIf_CurrentSlotDelay--; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }

    /* #31 Evaluate if handling of next schedule entry can be started: */
    retVal              = LinIf_SchdlHdlrRet_NoNewMessage;
    /* #32 If slot delay has elapsed, next slot can be started in any case */
    if ( schedTabDataPtrTmp->LinIf_CurrentSlotDelay == 0 )
    {
      retVal            = LinIf_SchdlHdlrRet_NewMessage;
    }
    /* #33 If schedule change after frame delay is enabled, the next slot can also be started if a schedule table change request is pending and
     *     and the frame inside the current slot is already finished
     *     and the current schedule table is of type run-continuous or the current schedule table is of type run-once and the table is all run through */
    else
    {
       if ( ( LinIf_IsScheduleChangeBeforeSlotEndOfChannelConfig(LinIf_LinIfChannel) == TRUE ) &&
            ( schedTabDataPtrTmp->LinIf_RequestedTable != LinIf_Schedule_Invalid ) &&
            ( LinIf_GetChannelData(LinIf_LinIfChannel).LinIf_FrameMaxDelay == 0 ) ) /* COV_LINIF_SCHEDULECHANGE_POINT_1 */
       {
         schedRunMode   = LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(LinIf_LinIfChannel)) +
                        schedTabDataPtrTmp->LinIf_CurrentTable);
         if ( (( schedRunMode == LinIf_RunMode_RunOnce ) && ( schedTabDataPtrTmp->LinIf_CurrentSlot == (uint8)0x00u )) ||
               ( schedRunMode == LinIf_RunMode_RunContinuous ) ) /* COV_LINIF_SCHEDULECHANGE_POINT_2 */
         {
           retVal       = LinIf_SchdlHdlrRet_NewMessage;
         }
       }
    }

    /* #40 If handling of next schedule entry can be started */
    if ( retVal == LinIf_SchdlHdlrRet_NewMessage )
    {
      /* #50 if NULL schedule table is requested, set it and quit, no further handling required */
      if ( schedTabDataPtrTmp->LinIf_RequestedTable == LinIf_Schedule_NULL )
      {
        /* switch to NULL schedule table */
        LinIf_FlushAllSchedules(schedTabDataPtrTmp); /* SBSW_LINIF_FLUSHALLSCHED_CALL_2 */
        schedTabDataPtrTmp->LinIf_SchedReqConfirmTable  = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        retVal                                          = LinIf_SchdlHdlrRet_OnNullSchedule;
        /* Set delay to 0, no message started */
        *LinIf_FrameMaxDelayPtr                         = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
        /* schedule change handled, remove request */
        schedTabDataPtrTmp->LinIf_RequestedTable        = LinIf_Schedule_Invalid; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
      /* #60 Otherwise: Do actual schedule table handling */
      else
      {
        /* Preset return value to LinIf_SchdlHdlrRet_NewMessage, maybe changed later in function */
        retVal                                          = LinIf_SchdlHdlrRet_NewMessage;

        /* #70 At first check if a RUN_ONCE schedule is finished. In this case, switch back to previous table and slot */
        LinIf_ScheduleTableHandler_RunOnceFinalization(schedTabDataPtrTmp, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_WITH_SCHEDTABPTR */

        /* #80 If a new table is requested, perform the actual switch (if not a RUNONCE schedule is active) and notify the schedule change */
        LinIf_ScheduleTableHandler_RequestedTableSwitch(schedTabDataPtrTmp, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_WITH_SCHEDTABPTR */

        /* #90 If new table is NULL schedule table, reset frame delay and quit */
        if ( schedTabDataPtrTmp->LinIf_CurrentTable == LinIf_Schedule_NULL )
        {
          /* RUN_ONCE finished and switched back to NULL schedule table, set delay to 0, no message started */
          *LinIf_FrameMaxDelayPtr                     = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
          retVal                                      = LinIf_SchdlHdlrRet_OnNullSchedule;
        }
        /* #100 Otherwise */
        else
        {
          /* #110 Retrieve current message information */
          /* get index into global schedule table list for current table */
          schedTabListIdx                             = LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(LinIf_LinIfChannel)) +
                                                                                            schedTabDataPtrTmp->LinIf_CurrentTable;
          /* get index into global entry slot list for current slot */
          slotListIdx                                 = LinIf_GetEntryStartIdxOfScheduleTableList(schedTabListIdx) + schedTabDataPtrTmp->LinIf_CurrentSlot;
          /* store delay in ticks of current slot */
          schedTabDataPtrTmp->LinIf_CurrentSlotDelay  = LinIf_GetDelayInTimebaseTicksOfEntry(slotListIdx); /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
          /* get global index into LinIf_FrameList of referenced frame */
          *LinIf_FrameHandlePtr                       = LinIf_GetFrameListIdxOfEntry(slotListIdx); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */

#if ( LINIF_J2602_FRAME_TOLERANCE_SUPPORT == STD_ON )
          /* Get the transmission of current frame in timebase ticks for this specific frame */
          *LinIf_FrameMaxDelayPtr                     = LinIf_GetFrameLengthDelayInTicksOfFrameList(*LinIf_FrameHandlePtr); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
#else
          /* get length of frame in bytes. Because global index is directly used from entry, no need to do channel-based recalculation */
          frameLengthTmp                              = LinIf_GetLengthOfFrameList(*LinIf_FrameHandlePtr);
          /* Get the transmission time of current frame in timebase ticks depending on the length of the frame */
          *LinIf_FrameMaxDelayPtr                     = LinIf_GetFrameLengthDelayOfFrameLengthDelayList(LinIf_GetFrameLengthDelayListStartIdxOfChannelConfig(LinIf_LinIfChannel) + (frameLengthTmp - 1)); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
#endif

          /* #120 Update slot index and check if new slot is last one in table */
          schedTabDataPtrTmp->LinIf_CurrentSlot++; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
          if ( schedTabDataPtrTmp->LinIf_CurrentSlot >= LinIf_GetNumberOfScheduleEntriesOfScheduleTableList(schedTabListIdx))
          {
            schedTabDataPtrTmp->LinIf_CurrentSlot = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */

            /* #130 Inform LinSM if header of last frame of current schedule table is going to be transmitted */
#if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
            schedTabDataPtrTmp->LinIf_SchedEndNotifyTable = schedTabDataPtrTmp->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
#endif

            /* #140 Switch back to previous table if collision resolving table is run through */
#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
            if ( schedTabDataPtrTmp->LinIf_EvtCollResolvingState == LinIf_EvtCollResolvingState_OnResolving )
            { /* collision resolving table is run through, so switch back to previous table */
              schedTabDataPtrTmp->LinIf_CurrentSlot            = schedTabDataPtrTmp->LinIf_PreviousContinuousSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
              schedTabDataPtrTmp->LinIf_CurrentTable           = schedTabDataPtrTmp->LinIf_PreviousContinuousTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
              schedTabDataPtrTmp->LinIf_EvtCollResolvingState  = LinIf_EvtCollResolvingState_Idle; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
            }
#endif
          }
        }
      }
    }
  }
  return(retVal);
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC MD_MSR_STMIF */

/***********************************************************************************************************************
 *
 * END - LinIf Schedule Table Handler
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LOCAL FUNCTIONS - LinIf Schedule Processing
 *
 * Following functions handle a new schedule slot. In case a sleep request is pending, the actual slot is discarded and
 * a sleep frame is transmitted instead in this slot. Otherwise the new slot is handled by determining the actual frame
 * type of this slot, requesting transmission data if necessary, preparing the Lin PDU and sending the message by
 * triggering the LIN driver.
 *
 **********************************************************************************************************************/

#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SporadicFrame()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SporadicFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)           LinIf_ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                     LinIf_LinPduPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)   LinIf_NewFrameHandlePtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                                    byteMskTmp;
  uint8                                                                    bitMskTmp;
  uint8                                                                    sporadicFrameTmp;
  LinIf_SporUnconditionalFrameIdxOfSporUncFrameByPriorityListType          sporUncFrmIdx;
  boolean                                                                  sporFrameRequested;

  /* ----- Implementation ----------------------------------------------- */
  sporFrameRequested  = FALSE;   /* Skip frame slot by default */

  /* LinIf_FixedFrameSduReference contains the sporadic slot index into SporadicFrameInfo slot list */
  sporadicFrameTmp    = 0;

  /* #10 Iterate over all sporadic unconditional frames assigned to the sporadic slot */
  while ( sporadicFrameTmp < LinIf_GetUncFrameCountOfSporadicFrameInfo(LinIf_GetFixedFrameSduListIdxOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle)) )
  {
    /* get the index of the sporadic unconditional frame by LinIf_SporadicFrameInfo -> LinIf_SporUncFrameByPriorityList -> LinIf_SporUnconditionalFrame */
    sporUncFrmIdx = LinIf_GetSporUnconditionalFrameIdxOfSporUncFrameByPriorityList(
                          LinIf_GetSporUncFrameByPriorityListStartIdxOfSporadicFrameInfo(LinIf_GetFixedFrameSduListIdxOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle)) + sporadicFrameTmp);
    byteMskTmp    = LinIf_GetByteMskOfSporUnconditionalFrame(sporUncFrmIdx);
    bitMskTmp     = LinIf_GetBitMskOfSporUnconditionalFrame(sporUncFrmIdx);

    /* #20 If the transmission flag of the sporadic unconditional frame is set */
    if ( (LinIf_GetSporTransmitFlags(byteMskTmp) & bitMskTmp) != 0) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
    { /* Flag is set for the current unconditional frame - transmit this frame. No further search necessary as in order by priority */

      /* #30 Replace current frame information with flagged unconditional frame info to be transmitted in current cycle */
      LinIf_ChannelDataPtr->LinIf_FrameHandle              = LinIf_GetFrameListIdxOfSporUnconditionalFrame(sporUncFrmIdx); /* SBSW_LINIF_CHANNEL_DATA_PTR */
      *LinIf_NewFrameHandlePtr                             = (LinIf_FrameListIdxOfEntryType)LinIf_ChannelDataPtr->LinIf_FrameHandle; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */

      /* set sporadic frame to LinIf_FrameType_UnconditionalFrame */
      LinIf_ChannelDataPtr->LinIf_CurrentFrameType         = LinIf_GetFrameTypeOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */

      /* #40 Setup given LIN Pdu with new message information */
      LinIf_LinPduPtr->Cs                                  = (Lin_FrameCsModelType) LinIf_GetChecksumTypeOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
      LinIf_LinPduPtr->Dl                                  = (Lin_FrameDlType) LinIf_GetLengthOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
      LinIf_LinPduPtr->Drc                                 = (Lin_FrameResponseType) LinIf_GetDirectionOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
      LinIf_LinPduPtr->Pid                                 = (Lin_FramePidType) LinIf_GetPidOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */

      /* #50 Activate transmission of unconditional frame in this slot */
      sporFrameRequested                             = TRUE;

      /* quit search for flagged unconditional frame */
      break;
    }
    sporadicFrameTmp++;
  }

  return sporFrameRequested;
}
#endif

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SleepHandling()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SleepHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                                  retVal;
  boolean                                                         sleepReqHandled;

  /* ----- Implementation ----------------------------------------------- */
  sleepReqHandled = FALSE;

  /* #10 If a (normal) goto-sleep request is pending */
  if ( LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReq )
  {
    /* #20 Trigger sleep frame transmission in LIN driver (-> GoToSleep() ) */
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    retVal = LinIf_GetLin_GoToSleepFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIf_LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
    retVal = LinIf_Lin_GoToSleep_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel));
#endif

    /* #30 If sleep frame transmission request is accepted */
    if ( retVal == (Std_ReturnType)E_OK )
    {
      /* #40 Update slot info and set transmission delay */
      LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_OnTrans; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      LinIf_ChannelDataPtr->LinIf_CurrentFrameType         = LinIf_FrameType_SleepModeFrame; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#if ( LINIF_J2602_FRAME_TOLERANCE_SUPPORT == STD_ON )
      LinIf_ChannelDataPtr->LinIf_FrameMaxDelay            = LinIf_GetSleepFrameLengthDelayInTicksOfChannelConfig(LinIf_LinIfChannel); /* SBSW_LINIF_CHANNEL_DATA_PTR */
#else
      LinIf_ChannelDataPtr->LinIf_FrameMaxDelay            = LinIf_GetFrameLengthDelayOfFrameLengthDelayList(LinIf_GetFrameLengthDelayListStartIdxOfChannelConfig(LinIf_LinIfChannel) + 7); /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif
    }
    /* #50 Otherwise: request not accepted */
    else
    {
      /* #60 Directly enter sleep mode and do not check LIN Driver state later */
      LinIf_ChannelDataPtr->LinIf_ChannelState             = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */

      LinIf_FlushAllSchedules(&(LinIf_ChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */

      /* #70 Inform LinSM directly about failed sleep confirmation (sleep mode frame will not be sent) */
#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIf_LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                    LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), FALSE);
#else
      LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), FALSE);
#endif
    }

#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
    /* #80 Reset wakeup flag in any case */
    LinIf_ChannelDataPtr->LinIf_WakeupFlag                 = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif

    sleepReqHandled = TRUE;
  }
  /* #90 Otherwise: check if an internal goto-sleep request is pending */
  else if ( LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReqInt )
  {
    /* #100 Trigger internal sleep mode transition in LIN driver (-> Lin_GoToSleepInternal() ) */
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    (void) LinIf_GetLin_GoToSleepInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIf_LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
    (void) LinIf_Lin_GoToSleepInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel));
#endif

    /* #110 Enter sleep mode, reset wakeup flag, inform LinSM directly about sleep confirmation */
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
    LinIf_ChannelDataPtr->LinIf_WakeupFlag                 = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif
    LinIf_ChannelDataPtr->LinIf_ChannelState               = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_CHANNEL_DATA_PTR */
    LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling   = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */

    LinIf_FlushAllSchedules(&(LinIf_ChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */

    /* Sleep mode frame will not be sent, but also inform LinSM */
#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIf_LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                    LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), TRUE);
#else
    LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), TRUE);
#endif

    sleepReqHandled = TRUE;
  }
  else
  {
    /* nothing to do */
  }

  return sleepReqHandled;
}

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_UnconditionalFrame()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_UnconditionalFrame
(
  CONSTP2CONST(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)              LinIf_LinPduPtr,
  VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC)                     LinIf_FrameHandle
)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType                                                       pduInfo;
  boolean                                                           sendFrameTmp;

  /* ----- Implementation ----------------------------------------------- */
  sendFrameTmp = TRUE;

  /* #10 If unconditional frame has direction Tx */
  if ( LinIf_GetDirectionOfFrameList(LinIf_FrameHandle) == LIN_MASTER_RESPONSE )
  {
    /* set pointer to transmit data */
    LinIf_LinPduPtr->SduPtr   = (uint8*)(&(LinIf_ChannelDataPtr->LinIf_TxData[0])); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */

    /* #20 Request data from PduR for this frame */
    pduInfo.SduLength      = LinIf_GetLengthOfFrameList(LinIf_FrameHandle);
    pduInfo.SduDataPtr     = LinIf_LinPduPtr->SduPtr;
#if ( LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL == STD_OFF )
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
    if (LinIf_GetTxTriggerFctList(LinIf_GetTxTriggerFctListTxTriggerFctIdxOfFrameList(LinIf_FrameHandle)) /* SBSW_LINIF_FCT_POINTER_CALL_PDUINFO */
              (LinIf_GetPduIdOfFrameList(LinIf_FrameHandle), &pduInfo) == E_NOT_OK)
# else
    if (PduR_LinIfTriggerTransmit(LinIf_GetPduIdOfFrameList(LinIf_FrameHandle), &pduInfo) == E_NOT_OK) /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR_PDUINFO */
# endif
    { /* #30 Leave slot empty if transmission data cannot be provided */
      sendFrameTmp      = FALSE;
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, LINIF_E_TRIGGERTRANSMIT_NO_DATA);
# endif
    }
#else
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
    (void)LinIf_GetTxTriggerFctList(LinIf_GetTxTriggerFctListTxTriggerFctIdxOfFrameList(LinIf_FrameHandle))  /* SBSW_LINIF_FCT_POINTER_CALL_PDUINFO */
              (LinIf_GetPduIdOfFrameList(LinIf_FrameHandle), &pduInfo);
# else
    (void)PduR_LinIfTriggerTransmit(LinIf_GetPduIdOfFrameList(LinIf_FrameHandle), &pduInfo); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR_PDUINFO */
# endif
#endif
  }

  return sendFrameTmp;
}

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing_FrameHandling()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_FrameHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)            LinIf_ChannelDataPtr, /* PRQA S 3673 */ /* MD_LIN_IF_16.7 */
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                      LinIf_LinPduPtr,
  P2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)         LinIf_FrameHandlePtr /* PRQA S 3673 */ /* MD_LIN_IF_16.7 */
#if ( LINIF_TP_SUPPORTED == STD_ON )
  ,CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)               LinTp_CtrlPtr
#endif
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean                                                                   sendFrameTmp;
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  uint8_least                                                               bufferCounterTmp;
#endif

  /* ----- Implementation ----------------------------------------------- */
  sendFrameTmp = TRUE;

  /* #10 Handle frame depending on frame type */
  switch ( LinIf_ChannelDataPtr->LinIf_CurrentFrameType )
  {

#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
    /***********************************************************************************************************
      *  LinIf_FrameType_SporadicFrame
      **********************************************************************************************************/
    /* #20 Sporadic frame: */
    case LinIf_FrameType_SporadicFrame:

      /* #30 If an associated unconditional frames is flagged for transmission,
       *     enter unconditional case to request transmit data from PduR (-> LinIf_ScheduleProcessing_SporadicFrame() ) */
      sendFrameTmp = LinIf_ScheduleProcessing_SporadicFrame(LinIf_ChannelDataPtr, LinIf_LinPduPtr, LinIf_FrameHandlePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */

      if (sendFrameTmp == FALSE) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
      {
        /* no sporadic frames marked for transmission or exists on channel: explicit break to avoid entering unconditional frames branch
         * due to the explicit fallthrough */
        break; /* PRQA S 3333 */ /* MD_LIN_IF_14.6 */
      }
      /* NOTE:
         explicit fallthrough: if sporadic frame was activated for transmission in sporadic slot, its type is set to unconditional frame
         and transmit data has to be requested from PduR
        */
#endif

    /***********************************************************************************************************
      *  LinIf_FrameType_UnconditionalFrame
      **********************************************************************************************************/
    /* #40 Unconditional frame: Default RX and TX frame handling (-> LinIf_ScheduleProcessing_UnconditionalFrame() ) */
    case LinIf_FrameType_UnconditionalFrame: /* PRQA S 2003 */ /* MD_LIN_IF_15.2 */
      sendFrameTmp = LinIf_ScheduleProcessing_UnconditionalFrame(LinIf_ChannelDataPtr, LinIf_LinPduPtr, *LinIf_FrameHandlePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
      break;

    /***********************************************************************************************************
      *  LinIf_FrameType_MRF
      **********************************************************************************************************/
      /* #50 Master request frame: Forward to LinTp if supported, otherwise skip MRF transmission (-> LinTp_TxScheduleProcessing() ) */
    case LinIf_FrameType_MRF:
#if ( LINIF_TP_SUPPORTED == STD_ON )
      sendFrameTmp      = LinTp_TxScheduleProcessing(LinTp_CtrlPtr, LinIf_ChannelDataPtr, LinIf_LinPduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
#else
      sendFrameTmp      = FALSE;
#endif
      break;

    /***********************************************************************************************************
      *  LinIf_FrameType_SRF
      **********************************************************************************************************/
    /* #60 Slave response frame: */
    case LinIf_FrameType_SRF:
      /* #70 Only send SRF header if node configuration response expected and LinTp is not in pending state (if LinTp is supported) */
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
      if ( LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_Idle )
#endif
      {
#if ( LINIF_TP_SUPPORTED == STD_ON )
        if ( (LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED ) || ( LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START ) ) /* COV_LINTP_SHORT_SRF_SLOT TF tf xf */
        {
          /* skip current SRF frame */
          sendFrameTmp       = FALSE;
        }
#else
        /* Header of SlaveResp is always send. Nothing to do. */
#endif
      }
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
      else if ( LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_SkipSrf )
      {
        /* Skip transmission of SRF after not successful Node Configuration Command */
        sendFrameTmp     = FALSE;
      }
      else
      {
        /* No handling required, MISRA only */
      }
#endif
      break;

    /***********************************************************************************************************
      *  LinIf_FrameType_... Node Configuration Command
      **********************************************************************************************************/
    /* #80 Node configuration frame: */
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    case LinIf_FrameType_AssignFrameId:
    case LinIf_FrameType_UnassignFrameId:
    case LinIf_FrameType_AssignNAD:
    case LinIf_FrameType_FreeFormat:
    case LinIf_FrameType_ConditionalChangeNAD:
    case LinIf_FrameType_AssignFrameIdRange:
    case LinIf_FrameType_SaveConfiguration:

      /* #90 Abort any active TP connection if a node configuration command is going to be transmitted */
# if ( LINIF_TP_SUPPORTED == STD_ON )
      LinTp_CloseActiveConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
# endif
      /* #100 Prepare fixed transmission data */
      /* set pointer to transmit buffer */
      LinIf_LinPduPtr->SduPtr = &(LinIf_ChannelDataPtr->LinIf_TxData[0]); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
      /* copy fixed data bytes to buffer */
      for (bufferCounterTmp = (uint8_least)0x00u; bufferCounterTmp < (uint8_least)LINIF_LINPDU_SIZE; bufferCounterTmp++)
      {
        LinIf_LinPduPtr->SduPtr[bufferCounterTmp] = LinIf_GetSduByteOfFixedFrameSduList(LinIf_GetFixedFrameSduListIdxOfFrameList(*LinIf_FrameHandlePtr) + bufferCounterTmp); /* SBSW_LINIF_LINPDU_SDUPTR */
      }
      LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_SendMrf; /* SBSW_LINIF_CHANNEL_DATA_PTR */

      break;
#endif

    /***********************************************************************************************************
      *  LinIf_FrameType_EventTriggeredFrame
      **********************************************************************************************************/
    /* case LinIf_FrameType_EventTriggeredFrame: */
      /* nothing to do, header is always sent */

    /* #110 Any other frame type: do nothing */
    default: /* COV_LINIF_MISRA_DEFAULTBRANCH X */
      break;
  }

  return sendFrameTmp;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  LinIf_ScheduleProcessing()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel
)
{
#if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)           tpCtrlTmp;
#endif
  LinIf_ScheduleTableHandlerRetType                               scheduleTableHandlerRetTmp;
  LinIf_ScheduleTableTickType                                     frameMaxDelayTmp;     /* actual frame transmission time in multiple time base ticks */
  LinIf_FrameListIdxOfEntryType                                   frameHandleTmp;
  Lin_PduType                                                     linPduTmp;
  Std_ReturnType                                                  retVal;
  boolean                                                         sendFrameTmp;
  boolean                                                         boolRetTmp;
  uint8                                                           errorId;

  errorId = LINIF_E_NO_ERROR;

  /* #10 LinTp: Request periodically PduR for transmission data or reception buffer (if necessary) */
#if ( LINIF_TP_SUPPORTED == STD_ON )
  tpCtrlTmp = LinTp_GetAddrCtrl(LinIf_LinIfChannel);

  LinTp_BufferPolling(tpCtrlTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#endif

  /* #20 Investigate the current time base tick for any necessary action (start new frame, on frame transmission, no scheduling) (-> LinIf_ScheduleTableHandler() ) */
  frameMaxDelayTmp            = 0;   /* avoid possible compiler warning, however will be overwritten if actually used */
  scheduleTableHandlerRetTmp  = LinIf_ScheduleTableHandler(&frameMaxDelayTmp, &frameHandleTmp, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

  /* #30 If new slot is started or NULL schedule table active (= new slot can be started) */
  if ( ( scheduleTableHandlerRetTmp != LinIf_SchdlHdlrRet_NoNewMessage ) &&
       ( LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling != LinIf_OnSleepModeFrameHandling_OnTrans ) )
  {
    /* #40 If previous frame is still ongoing (frame duration vs. slot length mismatch), report error if applicable */
    /* If NULL schedule table is active, a sleep frame transmission could be ongoing. Because LinIf_ScheduleTableHandler() does not consider sleep frame slots,
       this case has to be additionally checked to avoid DET error below */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( (LinIf_ChannelDataPtr->LinIf_FrameMaxDelay != (LinIf_ScheduleTableTickType)0x00u) ) /* frame transmission time not elapsed yet but new schedule slot should start */
    {
      /* Calculated maximum frame transmission time shorter than appropriate schedule slot */
      errorId = LINIF_E_SCHEDULE_INCONSISTENT_ERROR;
    }
#endif

    /* Next message will be started or NULL schedule table is active */

    /* #50 If a sleep request is pending, handle it if necessary (-> LinIf_ScheduleProcessing_SleepHandling() ) */
    boolRetTmp = LinIf_ScheduleProcessing_SleepHandling(LinIf_ChannelDataPtr, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */

    /* #60 Otherwise: */
    if ( (boolRetTmp == FALSE) && (scheduleTableHandlerRetTmp == LinIf_SchdlHdlrRet_NewMessage) )
    { /* new message shall be started, no sleep request was handled in this slot, null schedule table is not set */

      /* #70 Process new slot: update slot information and setup LIN PDU for driver */
      LinIf_ChannelDataPtr->LinIf_FrameHandle         = frameHandleTmp; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      LinIf_ChannelDataPtr->LinIf_CurrentFrameType    = LinIf_GetFrameTypeOfFrameList(frameHandleTmp); /* SBSW_LINIF_CHANNEL_DATA_PTR */

      /* set LinPdu for driver */
      linPduTmp.Cs                              = (Lin_FrameCsModelType) LinIf_GetChecksumTypeOfFrameList(frameHandleTmp);
      linPduTmp.Dl                              = (Lin_FrameDlType) LinIf_GetLengthOfFrameList(frameHandleTmp);
      linPduTmp.Drc                             = (Lin_FrameResponseType) LinIf_GetDirectionOfFrameList(frameHandleTmp);
      linPduTmp.Pid                             = (Lin_FramePidType) LinIf_GetPidOfFrameList(frameHandleTmp);
      linPduTmp.SduPtr                          = NULL_PTR; /* set SDU pointer to a defined value, will be overwritten in case of Tx */

      /* #80 Check possible skipping of slot and request transmission data for Tx frames simultaneously (-> LinIf_ScheduleProcessing_FrameHandling() ) */
#if ( LINIF_TP_SUPPORTED == STD_ON )
      sendFrameTmp                              = LinIf_ScheduleProcessing_FrameHandling(LinIf_ChannelDataPtr, &linPduTmp, &frameHandleTmp, tpCtrlTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#else
      sendFrameTmp                              = LinIf_ScheduleProcessing_FrameHandling(LinIf_ChannelDataPtr, &linPduTmp, &frameHandleTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#endif

      if ( sendFrameTmp == TRUE )
      {
        /* # 90 if frame is handled in this slot, send header / message and setup transmission time */
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
        retVal = LinIf_GetLin_SendFrameFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIf_LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel), &linPduTmp); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
        retVal = LinIf_Lin_SendFrame_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel), &linPduTmp); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
#endif

        if ( ( retVal == (Std_ReturnType)E_OK ) &&
             ( LinIf_GetDirectionOfFrameList(frameHandleTmp) != LIN_SLAVE_TO_SLAVE ) )
        {
          /* transmission successful, S2S frames are the only type which do not require further handling */
          /* Store maximum frame transmission time within current slot for rx and tx frame */
          LinIf_ChannelDataPtr->LinIf_FrameMaxDelay   = frameMaxDelayTmp; /* SBSW_LINIF_CHANNEL_DATA_PTR */
        }
        /* else: Transmission of frame not successful, skip further processing, no error must reported according to spec */
      }
    } /* new message handled */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
}

/***********************************************************************************************************************
 *
 * END - LinIf Schedule Processing
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LOCAL FUNCTIONS - LinIf Message Processing
 *
 * Following functions handle transmitted / received messages of the current slot. After the maximum transmission time,
 * the LIN driver is requested for the status of the last message. In case of successful reception, the received data
 * is indicated to the appropriate module depending on the frame type. A successful transmission is confirmed. If an
 * error is reported by the driver, appropriate error handling is performed.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * Functions for handling transmitted / received messages of current slot.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_MessageProcessing_Unconditional()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_Unconditional
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel,
#endif
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinIf_LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                    LinIf_LinStatus
)
{
  uint16_least                                                      indexTmp;
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  uint8                                                             byteMskTmp;
  uint8                                                             bitMskTmp;
  LinIf_SporUnconditionalFrameIdxOfSporUncFrameByHandleListType     idxToSporUncFrame;
#endif
  uint8                                                             errorId;

  errorId = LINIF_E_NO_ERROR;

  /* #10 If unconditional frame is Tx */
  if ( LinIf_GetDirectionOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle) == LIN_MASTER_RESPONSE )
  {
    /* #20 If transmission was successful */
    if ( LinIf_LinStatus == (Lin_StatusType)LIN_TX_OK )
    {
      /* #30 Inform upper layer about transmitted frame */
      LinIf_ChannelDataPtr->LinIf_PduRNotification         = LinIf_PduRNotificationTx; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      LinIf_ChannelDataPtr->LinIf_PduRNotificationPduId    = LinIf_GetPduIdOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */
#if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_ChannelDataPtr->LinIf_PduRNotificationFctIndex = (uint8)LinIf_GetIndConfFctIdxOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif

      /* #40 Search over all unconditional frames which are used in sporadic slots on current channel */
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
      /* clear flag of unconditional frame if sporadic slot was occupied or unconditional frame was transmitted regularly in schedule table */
      indexTmp = 0;
      /* search all unconditional frames assigned to sporadic slots on this channel in list sorted by frame handle which was saved before transmission */
      while (indexTmp < LinIf_GetNumberOfSporadicUncFramesOfChannelConfig(LinIf_LinIfChannel)) /* COV_LINIF_SPOR_FRAME_HANDLE_CONFIG */
      {
        /* get the index of the sporadic unconditional frame by LinIf_SporUncFrameByHandleList -> LinIf_SporUnconditionalFrame */
        idxToSporUncFrame = LinIf_GetSporUnconditionalFrameIdxOfSporUncFrameByHandleList(LinIf_GetSporUncFrameByHandleListStartIdxOfChannelConfig(LinIf_LinIfChannel) + indexTmp);
        if ( LinIf_ChannelDataPtr->LinIf_FrameHandle <= LinIf_GetFrameListIdxOfSporUnconditionalFrame(idxToSporUncFrame))
        {
          /* #50 If current transmitted unconditional frame matches one in a sporadic slot, clear transmission flags of this frame */
          if ( LinIf_ChannelDataPtr->LinIf_FrameHandle == LinIf_GetFrameListIdxOfSporUnconditionalFrame(idxToSporUncFrame))
          {
            byteMskTmp = LinIf_GetByteMskOfSporUnconditionalFrame(idxToSporUncFrame);
            bitMskTmp  = LinIf_GetBitMskOfSporUnconditionalFrame(idxToSporUncFrame);
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
            if ( byteMskTmp >= LinIf_GetSizeOfSporTransmitFlags() ) /* defensive programming, condition will never be true in a valid configuration */ /* COV_LINIF_INV_STATE_ALWAYSFALSE */
            {
              errorId = LINIF_E_CONFIG;
            }
            else
# endif
            {
              LinIf_SetSporTransmitFlags(byteMskTmp, LinIf_GetSporTransmitFlags(byteMskTmp) &= (uint8)(~bitMskTmp)); /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS */ /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS */
            }
          }
          /* frame was transmitted in sporadic slot (flag cleared afterwards) or frame handle not transmitted at all in this slot */
          /* in each case abort the loop as the search is finished */
          break;
        }
        indexTmp++;
      }
#endif
    }
    /* #60 Report Det error if frame was not successfully transmitted */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      if ( ( LinIf_LinStatus == (Lin_StatusType)LIN_TX_BUSY ) || ( LinIf_LinStatus == (Lin_StatusType)LIN_TX_ERROR ) )
      { /* Inform about error in transmitted frame */
        errorId = LINIF_E_RESPONSE;
      }
    }
#endif
  }
  /* #70 otherwise if unconditional frame is Rx */
  else if ( LinIf_GetDirectionOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle) == LIN_SLAVE_RESPONSE ) /* COV_LINIF_MISRA_ELSEBRANCH */
  {
    /* #80 If reception was successful */
    if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_OK )
    {
      /* #90 Inform PduR about received frame and data */
      LinIf_ChannelDataPtr->LinIf_PduRNotification               = LinIf_PduRNotificationRx; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      LinIf_ChannelDataPtr->LinIf_PduRNotificationPduId          = LinIf_GetPduIdOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */
#if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_ChannelDataPtr->LinIf_PduRNotificationFctIndex       = (uint8)LinIf_GetIndConfFctIdxOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif
      /* save length because FrameInfoPtr may be overwritten in LinIf_ScheduleProcessing() before PduR confirmation */
      LinIf_ChannelDataPtr->LinIf_PduRNotificationPduLength      = LinIf_GetLengthOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */

      for (indexTmp = 0; indexTmp < LinIf_GetLengthOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); indexTmp++)
      { /* copy Lin receive data to LinIf buffer for data consistency */
        LinIf_ChannelDataPtr->LinIf_RxData[indexTmp & LINIF_LINPDU_SIZE_MASK] = LinIf_LinSduPtr[indexTmp & LINIF_LINPDU_SIZE_MASK]; /* SBSW_LINIF_RX_BUFFER_ARRAY */
      }
    }
    /* #100 Report Det error if frame was not successfully received */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      if ( ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_BUSY ) ||
           ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_ERROR ) ||
           ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE ) )
      {
        /* Inform about error in received frame */
        errorId = LINIF_E_RESPONSE;
      }
    }
#endif
  }
  /* #110 otherwise on S2S frame: do nothing */
  else
  {
    /* On S2S Frame - never reach this point as the response part of a S2S frame is not handled at all */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinIf_MessageProcessing_EventTriggeredFrame()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_EventTriggeredFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)    LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                 LinIf_LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinIf_LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                    LinIf_LinStatus
)
{
  uint8                                                             pidTmp;
  uint16_least                                                      indexTmp;

  /* #10 If EVT frame is successfully received without collision, handle frame as any unconditional frame */
  if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_OK )
  {
    /* evaluate first byte of frame for received PID to assign appropriate unconditional frame */
    pidTmp = LinIf_LinSduPtr[0];

    /* get the start index of the assigned unconditional frame of current EVT slot */
    indexTmp = LinIf_GetEvtUnconditionalFrameInfoStartIdxOfEvtFrameInfo( LinIf_GetFixedFrameSduListIdxOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle) );

    /* #20 Iterate over each unconditional frame assigned to the EVT slot and search for a match with the received frame indicated by the PID */
    while ( indexTmp < LinIf_GetEvtUnconditionalFrameInfoEndIdxOfEvtFrameInfo(LinIf_LinIfChannel))
    {
      if ( pidTmp == LinIf_GetPidOfEvtUnconditionalFrameInfo(indexTmp) )
      { /* PID matches - convert current EVT frame information to unconditional frame info */
        /* replace current frame information with flagged unconditional frame */
        LinIf_ChannelDataPtr->LinIf_FrameHandle                  = LinIf_GetFrameListIdxOfEvtUnconditionalFrameInfo(indexTmp); /* SBSW_LINIF_CHANNEL_DATA_PTR */
        /* quit search for matching unconditional frame */
        break;
      }
      indexTmp++;
    }

    /* #30 If corresponding unconditional frame is found, inform PduR about received frame and data */
    if ( indexTmp < LinIf_GetEvtUnconditionalFrameInfoEndIdxOfEvtFrameInfo(LinIf_LinIfChannel) )
    {
      LinIf_ChannelDataPtr->LinIf_PduRNotification               = LinIf_PduRNotificationRx; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      LinIf_ChannelDataPtr->LinIf_PduRNotificationPduId          = LinIf_GetPduIdOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_ChannelDataPtr->LinIf_PduRNotificationFctIndex       = (uint8)LinIf_GetIndConfFctIdxOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */
# endif
      /* save length because FrameInfoPtr may be overwritten in LinIf_ScheduleProcessing() before PduR confirmation */
      LinIf_ChannelDataPtr->LinIf_PduRNotificationPduLength      = LinIf_GetLengthOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_CHANNEL_DATA_PTR */

      for (indexTmp = 0; indexTmp < LinIf_GetLengthOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle); indexTmp++)
      { /* copy Lin receive data to LinIf buffer for data consistency */
        LinIf_ChannelDataPtr->LinIf_RxData[indexTmp]       = LinIf_LinSduPtr[indexTmp]; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      }
    }
  }
  /* #40 Otherwise: */
  else
  {
    /* #50 If error occurred, treat as collision and set collision resolving schedule table as pending */
    if ( ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_BUSY ) || ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_ERROR ) )
    {
      /* collision on EVT frame detected */
      if ( LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_RequestedTable == LinIf_Schedule_Invalid )
      {
        /* collision resolving is only started, if no schedule table change has been requested */
        /* if a sleep mode transition was requested, collision resolving is discarded - this case will be handled in ScheduleProcessing() */
        LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_RequestedTable = LinIf_GetCollResolvSchedHandleOfEvtFrameInfo( LinIf_GetFixedFrameSduListIdxOfFrameList( LinIf_ChannelDataPtr->LinIf_FrameHandle) ); /* SBSW_LINIF_CHANNEL_DATA_PTR */
        LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_EvtCollResolvingState = LinIf_EvtCollResolvingState_Detected; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      }
      /* else: schedule table change was requested before, so collision resolving is lost */
    }
    /* #60 If no response received, no slave has responded, nothing to do */
    else
    {
      if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE )
      {
        /* silent EVT frame - no further action required */
      }
      /* else: ignore all other reportings */
    }
  }
}
#endif

/***********************************************************************************************************************
 *  LinIf_MessageProcessing_SleepModeFrame()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SleepModeFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                  LinIf_LinStatus
)
{
  /* #10 If sleep mode frame state is still on transmission */
  if ( LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTrans )
  {
    LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */

    /* #20 If driver reported sleep status */
    if ( LinIf_LinStatus == (Lin_StatusType)LIN_CH_SLEEP)
    {
      /* #30 Enter sleep state and inform LinSM */
      /* Sleep mode frame either successfully transmitted or transmitted with error */
      /* According to specification any error occurred is ignored and sleep mode is also entered. */
      LinIf_FlushAllSchedules(&(LinIf_ChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */

      LinIf_ChannelDataPtr->LinIf_ChannelState             = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      /* Sleep mode frame has been send, inform the higher layer */
#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIf_LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                          LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), TRUE);
#else
      LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), TRUE);
#endif
    }
    /* #40 Otherwise: */
    else
    {
      /* #50 Driver not in sleep state, so wakeup frame received after sleep frame transmission -> restart wakeup process, inform LinSM */
      LinIf_ChannelDataPtr->LinIf_WakeupDelayTimer         = LinIf_GetWakeupDelayExternalOfChannelConfig(LinIf_LinIfChannel); /* SBSW_LINIF_CHANNEL_DATA_PTR */

#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIf_LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                      LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), FALSE);
#else
      LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), FALSE);
#endif
    }
  }
  /* #60 Otherwise if a wakeup request occured during sleep mode frame transmission */
  else if ( LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReWakeInt ) /* COV_LINIF_MISRA_ELSEBRANCH */
  {
    /* #70 Remain in operational state, do not inform the higher layer (sleep state not reached according to spec) */
    LinIf_ChannelDataPtr->LinIf_ChannelState               = LINIF_CHANNEL_OPERATIONAL; /* SBSW_LINIF_CHANNEL_DATA_PTR */

    /* #80 Reset frame delay to check wakeup frame in next cycle and reload wakeup delay timer */
    LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling   = LinIf_OnWakeupFrameHandling_OnTrans; /* SBSW_LINIF_CHANNEL_DATA_PTR */
    LinIf_ChannelDataPtr->LinIf_CurrentFrameType           = LinIf_FrameType_WakeUpFrame; /* SBSW_LINIF_CHANNEL_DATA_PTR */

    LinIf_ChannelDataPtr->LinIf_FrameMaxDelay              = (LinIf_ScheduleTableTickType)0x01u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
    LinIf_ChannelDataPtr->LinIf_WakeupDelayTimer           = LinIf_GetWakeupDelayInternalOfChannelConfig(LinIf_LinIfChannel); /* SBSW_LINIF_CHANNEL_DATA_PTR */

    /* #90 Trigger driver to send wakeup frame (Lin_Wakeup) */
    /* return value is ignored, because it can not be forwarded anyway */
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    (void) LinIf_GetLin_WakeupFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIf_LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
    (void) LinIf_Lin_Wakeup_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel));
#endif
  }
  else
  {
    /* MISRA only, cannot happen */
  }
}

/***********************************************************************************************************************
 *  LinIf_MessageProcessing_SRF()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SRF
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr, /* PRQA S 3673 */ /* MD_LIN_IF_16.7 */
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                       LinIf_LinSduPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                  LinIf_LinStatus
)
{
  uint8                                                           errorId;

  errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT_CONST(LinIf_ChannelDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(LinIf_LinSduPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(LinIf_LinIfChannel); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(LinIf_LinStatus); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

/* #10 If no node configuration response is expected */
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  if ( LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_Idle )
#endif
  {
    /* #20 Forward received SRF to LinTp (->LinTp_RxIndication()) in case no bus error occurred, otherwise report error */
#if ( LINIF_TP_SUPPORTED == STD_ON )
    if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_OK )
    {
      /* LinTp SRF indicated check current state */
      LinTp_RxIndication(LinIf_LinIfChannel, LinIf_LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
    }
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    else if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE )
    {
      /* No response must be handled different for SlaveResp message to error in response. */
    }
    else if ( ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_BUSY ) || ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_ERROR ) )
    {
      /* Error in node configuration slave response */
      errorId = LINIF_E_RESPONSE;
    }
    else
    {
      /* No handling required, MISRA only */
    }
# endif
#endif
  }
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  else if ( LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_SendSrf ) /* COV_LINIF_NODECONFIG_ELSEBRANCH */
  {
    /* #30 Otherwise (node configuration response expected): Ignore SRF if no bus error occurred, otherwise report error */
    /* This is a node configuration command answer, which must be processed within the LinIf */

    LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE )
    {
      errorId = LINIF_E_NC_NO_RESPONSE;
    }
    else if ( ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_BUSY ) || ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_ERROR ) )
    {
      /* Error in node configuration slave response */
      errorId = LINIF_E_RESPONSE;
    }
    /* else if ( LinIf_LinStatus == (Lin_StatusType)LIN_RX_OK ) : nothing to do */
    else
    {
      /* No handling required, MISRA only */
    }
# endif
  }
  else
  {
    /* Never happens, MISRA only */
  }
#endif

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
}

/***********************************************************************************************************************
 *  LinIf_CurrentMessageProcessing()
 **********************************************************************************************************************/
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
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_CurrentMessageProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel
)
{
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR)                         linSduPtrTmp;
  Lin_StatusType                                                  linStatusTmp;
#if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)           tpCtrlTmp;
#endif
  uint8                                                           errorId;

  errorId = LINIF_E_NO_ERROR;

  /* #10 If maximum frame duration has expired */
  if ( LinIf_ChannelDataPtr->LinIf_FrameMaxDelay > (LinIf_ScheduleTableTickType)0x00u )
  {
    LinIf_ChannelDataPtr->LinIf_FrameMaxDelay--; /* SBSW_LINIF_CHANNEL_DATA_PTR */
    if ( LinIf_ChannelDataPtr->LinIf_FrameMaxDelay == (LinIf_ScheduleTableTickType)0x00u )
    {
      /* #20 Frame delay expired, check LIN driver for status of last frame (-> Lin_GetStatus()) */
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
      linStatusTmp = LinIf_GetLin_GetStatusFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIf_LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel), &linSduPtrTmp); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
      linStatusTmp = LinIf_Lin_GetStatus_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel), &linSduPtrTmp); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
#endif

      /* #21 Inform application about status of last frame if enabled, excluding sleep and wakeup frames */
#if ( LINIF_APPL_FRAME_STATUS_INFO == STD_ON )
      if ( !((LinIf_ChannelDataPtr->LinIf_CurrentFrameType == LinIf_FrameType_SleepModeFrame) || (LinIf_ChannelDataPtr->LinIf_CurrentFrameType == LinIf_FrameType_WakeUpFrame)) )
      {
        Appl_LinIfGetLinStatus(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), LinIf_GetPidOfFrameList(LinIf_ChannelDataPtr->LinIf_FrameHandle), linStatusTmp);
      }
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
      tpCtrlTmp = LinTp_GetAddrCtrl(LinIf_LinIfChannel);
#endif

      /* #30 Further handling depends on frame type: */
      switch ( LinIf_ChannelDataPtr->LinIf_CurrentFrameType )
      {
        /************************************************************************************************************
          *  LinIf_FrameType_MRF
          ***********************************************************************************************************/
        /* #40 On MRF: LinTp is the only user, so a MRF is never scheduled if LinTp is not activated */
#if ( LINIF_TP_SUPPORTED == STD_ON )
        case LinIf_FrameType_MRF:
          /* #50 Forward transmitted MRF to LinTp if supported (-> LinTp_TxMessageProcessing() ) */
          LinTp_TxMessageProcessing(tpCtrlTmp, linStatusTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

          /* #60 Report DET error on transmission failure */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
          if ( ( linStatusTmp == (Lin_StatusType)LIN_TX_BUSY ) || ( linStatusTmp == (Lin_StatusType)LIN_TX_ERROR ) )
          {
            /* Inform DET about error in transmitted frame */
            errorId = LINIF_E_RESPONSE;
          }
# endif
          break;
#endif

        /************************************************************************************************************
          *  LinIf_FrameType_UnconditionalFrame
          ***********************************************************************************************************/
        /* #70 On unconditional frame: perform handling (-> LinIf_MessageProcessing_Unconditional() ) */
        case LinIf_FrameType_UnconditionalFrame:
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
          LinIf_MessageProcessing_Unconditional(LinIf_ChannelDataPtr, LinIf_LinIfChannel, linSduPtrTmp, linStatusTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#else
          LinIf_MessageProcessing_Unconditional(LinIf_ChannelDataPtr, linSduPtrTmp, linStatusTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#endif
          break;

        /************************************************************************************************************
          *  LinIf_FrameType_SRF
          ***********************************************************************************************************/
        /* #80 On SRF: perform handling (-> LinIf_MessageProcessing_SRF() ) */
        case LinIf_FrameType_SRF:
          LinIf_MessageProcessing_SRF(LinIf_ChannelDataPtr, linSduPtrTmp, LinIf_LinIfChannel, linStatusTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
          break;

#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
        /************************************************************************************************************
          *  LinIf_FrameType_EventTriggeredFrame
          ***********************************************************************************************************/
        /* #90 On EVT frame: perform performed (-> LinIf_MessageProcessing_EventTriggeredFrame() ) */
        case LinIf_FrameType_EventTriggeredFrame:
          LinIf_MessageProcessing_EventTriggeredFrame(LinIf_ChannelDataPtr, LinIf_LinIfChannel, linSduPtrTmp, linStatusTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
          break;
#endif

#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
        /************************************************************************************************************
          *  LinIf_FrameType_... Node Configuration Command
          ***********************************************************************************************************/
        /* #100 On node configuration command: on successful transmission, send succeeding SRF header, otherwise skip it */
        case LinIf_FrameType_AssignFrameId:
        case LinIf_FrameType_UnassignFrameId:
        case LinIf_FrameType_AssignNAD:
        case LinIf_FrameType_FreeFormat:
        case LinIf_FrameType_ConditionalChangeNAD:
        case LinIf_FrameType_AssignFrameIdRange:
        case LinIf_FrameType_SaveConfiguration:
          /* These are schedule table commands and always tx messages, which must be processed in the LinIf */
          if ( linStatusTmp == (Lin_StatusType)LIN_TX_OK )
          {
            /* Store that the frame has been send and the following SRF has to be handled without the LinTp */
            LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_SendSrf; /* SBSW_LINIF_CHANNEL_DATA_PTR */
          }
          else
          {
            LinIf_ChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_SkipSrf; /* SBSW_LINIF_CHANNEL_DATA_PTR */
          }
          break;
#endif

        /************************************************************************************************************
          *  LinIf_FrameType_SleepModeFrame
          ***********************************************************************************************************/
        /* #110 On sleep mode frame: perform handling (-> LinIf_MessageProcessing_SleepModeFrame() ) */
        case LinIf_FrameType_SleepModeFrame:
          /* Sleep Mode Frame has been send */
          LinIf_MessageProcessing_SleepModeFrame(LinIf_ChannelDataPtr, LinIf_LinIfChannel, linStatusTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
          break;

        /************************************************************************************************************
          *  LinIf_FrameType_WakeUpFrame
          ***********************************************************************************************************/
        /* #120 On wakeup frame: inform upper layer about wakeup, only if no sleep request occurred during wakeup frame transmission */
        case LinIf_FrameType_WakeUpFrame:
          /* Wake Up Frame has been send */
          if ( LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnWakeupFrameHandling_OnTrans )
          { /* Only Process the wakeup frame transmission if no other request (i.e. Sleep Mode Frame) is pending */
            LinIf_ChannelDataPtr->LinIf_OnSleepModeFrameHandling  = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
            LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIf_LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                    LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), TRUE);
#else
            LinSM_WakeupConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), TRUE);
#endif
          }
          break;

        /* #130 On any other frame type, nothing to do, especially not for sporadic frame as they are handled as unconditional frames */
        /************************************************************************************************************
          *  case LinIf_FrameType_SporadicFrame:
          *
          *  Case not required here as a sporadic slot is empty or replaced by an unconditional frame
          *
          *  break;
          *
          *
          ***********************************************************************************************************/
        /************************************************************************************************************
          *  default
          ***********************************************************************************************************/
        default:
          break;

      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_TP_SUPPORTED == STD_ON ) /* currently only Tp could report an error in this function */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
} /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/***********************************************************************************************************************
 *
 * END - LinIf Message Processing
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinIf Initialization, Task and General Function Interface - Local Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_ChannelInit()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelInit
(
  VAR(NetworkHandleType, AUTOMATIC)                         LinIf_LinIfChannel
)
{
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;

  /* #10 Initialize all channel specific variables to default */
  channelDataPtrTmp                                               = LinIf_GetAddrChannelData(LinIf_LinIfChannel);

  /* Set default values of internal variables */
  channelDataPtrTmp->LinIf_FrameMaxDelay                          = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
  channelDataPtrTmp->LinIf_FrameHandle                            = (uint16)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
  channelDataPtrTmp->LinIf_OnSleepModeFrameHandling               = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  channelDataPtrTmp->LinIf_AtNodeConfigurationCmd                 = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  channelDataPtrTmp->LinIf_WakeupFlag                             = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif
  channelDataPtrTmp->LinIf_WakeupDelayTimer                       = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR */
  channelDataPtrTmp->LinIf_PduRNotification                       = LinIf_PduRNotificationIdle; /* SBSW_LINIF_CHANNEL_DATA_PTR */

  /* #20 Initialize call cycle offset for load balancing */
  channelDataPtrTmp->LinIf_ChannelFuncCallCycleCounter            = (uint8)(LinIf_GetLoadBalancingOffsetOfChannelConfig(LinIf_LinIfChannel)); /* SBSW_LINIF_CHANNEL_DATA_PTR */

  /* #30 Clear schedule table data and set NULL schedule table */
  LinIf_FlushAllSchedules(&(channelDataPtrTmp->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */

#if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
  channelDataPtrTmp->LinIf_SchedTabData.LinIf_SchedEndNotifyTable  = LinIf_Schedule_Invalid; /* SBSW_LINIF_CHANNEL_DATA_PTR */
#endif
  channelDataPtrTmp->LinIf_SchedTabData.LinIf_SchedReqConfirmTable = LinIf_Schedule_Invalid; /* SBSW_LINIF_CHANNEL_DATA_PTR */

  /* #40 Set channel state depending on configured startup state */
  if (LinIf_StartupState_Sleep == LinIf_GetStartupStateOfChannelConfig(LinIf_LinIfChannel))
  {
    /* Set channel to sleep mode */
    channelDataPtrTmp->LinIf_ChannelState                         = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_CHANNEL_DATA_PTR */
    /* Set Lin driver channel to sleep mode */
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    (void) LinIf_GetLin_GoToSleepInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIf_LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
    (void) LinIf_Lin_GoToSleepInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIf_LinIfChannel));
#endif
  }
  else
  {
    /* Set channel to operational mode */
    channelDataPtrTmp->LinIf_ChannelState                         = LINIF_CHANNEL_OPERATIONAL; /* SBSW_LINIF_CHANNEL_DATA_PTR */
  }
}

/***********************************************************************************************************************
 *  LinIf_ChannelMainFunction()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                         LinIf_LinIfChannel
)
{
  /* This function handles the main function for one channel */

  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;
#if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType,     AUTOMATIC, LINIF_VAR_NOINIT) tpCtrlTmp;
#endif

  channelDataPtrTmp = LinIf_GetAddrChannelData(LinIf_LinIfChannel);

#if ( LINIF_TP_SUPPORTED == STD_ON )
  tpCtrlTmp = LinTp_GetAddrCtrl(LinIf_LinIfChannel);
#endif

  /* #10 If channel state is operational */
  if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
  {
    /* #20 Call main message processing function (-> LinIf_CurrentMessageProcessing() ) */
    /**********************************************************
     * Processing of last message
     **********************************************************/
    LinIf_CurrentMessageProcessing(channelDataPtrTmp, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

    /* #30 Update Tp timer and abort connection on timeout */
    /**********************************************************
     * Tp timeout observation
     **********************************************************/
#if ( LINIF_TP_SUPPORTED == STD_ON )
    /* Lin Tp timeout handling */
    if (tpCtrlTmp->LinTp_Timer > 0)
    {
      tpCtrlTmp->LinTp_Timer--; /* SBSW_LINIF_TP_CTRL_PTR */
      if (tpCtrlTmp->LinTp_Timer == 0)
      { /* timeout on running transmission / reception occurred check current state */
        LinTp_CloseActiveConnection(tpCtrlTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
    }
#endif

    /* #40 If channel is still in operation state */
    if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
    {
      /**********************************************************
       * Processing of next time slot
       **********************************************************/
      /* #50 If wakeup delay timer is active, update it, otherwise call main schedule table processing function (-> LinIf_ScheduleProcessing() ) */
      if ( channelDataPtrTmp->LinIf_WakeupDelayTimer == (uint8)0x00u )
      {
        /* Normal Message processing */
        LinIf_ScheduleProcessing(channelDataPtrTmp, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
      else
      {
        /* Wait for end of wakeup delay */
        channelDataPtrTmp->LinIf_WakeupDelayTimer--; /* SBSW_LINIF_CHANNEL_DATA_PTR */
      }
    }
    /* else: Skip further processing if sleep mode has been entered */
  }
  /* #60 Otherwise if channel state is sleep and an internal goto sleep request is pending */
  else if ( ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_SLEEP ) &&
            ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReqInt ) )
  {
    /* #70 Process one tick of main schedule table processing function for internal sleep transition */
    LinIf_ScheduleProcessing(channelDataPtrTmp, LinIf_LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  } /* sleep state */
  else
  {
    /* MISRA only */
  }
}

/***********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessing_LinSM()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessing_LinSM
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_ChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                               LinIf_LinIfChannel
)
{
  /**********************************************************
  * LinSM notification that current schedule is run through
  **********************************************************/
  /* #10 Report schedule table end confirmation if notification is pending */
#if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
  if (LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable != LinIf_Schedule_Invalid)
  {
    LinSM_ScheduleEndNotification(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel), LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable );
    LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable = LinIf_Schedule_Invalid; /* SBSW_LINIF_CHANNEL_DATA_PTR */
  }
#endif

  /**********************************************************
  * LinIf post notification to LinSm of schedule change
  **********************************************************/
  /* #20 Report schedule table change confirmation if notification is pending */
  /* delayed schedule request confirmation to LinSM */
  if (LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable != LinIf_Schedule_Invalid)
  {
#if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    LinIf_GetScheduleReqConfFctList(LinIf_GetScheduleReqConfFctListIdxOfChannelConfig(LinIf_LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                        LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel),
                                        LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable);
#else
    LinSM_ScheduleRequestConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIf_LinIfChannel),
                                      LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable);
#endif
    /* invalidate schedule request confirmation notification flag */
    LinIf_ChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable = LinIf_Schedule_Invalid; /* SBSW_LINIF_CHANNEL_DATA_PTR */
  }
}

/***********************************************************************************************************************
 *
 * END - LinIf Initialization, Task and General Function Interface - Local Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf Schedule Handling - Local Functions
 *
 * The schedule handler manages the different LIN schedule tables.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_FlushAllSchedules()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
LINIF_LOCAL FUNC(void, LINIF_CODE) LinIf_FlushAllSchedules
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinIf_SchedTabDataPtr
)
{
  /* #10 Reset all schedule table related variables, set NULL schedule table, abort collision resolving table */
  LinIf_SchedTabDataPtr->LinIf_CurrentTable              = (uint8)0x00u; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
  LinIf_SchedTabDataPtr->LinIf_CurrentSlot               = (uint8)0x00u; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
  LinIf_SchedTabDataPtr->LinIf_PreviousContinuousTable   = (uint8)0x00u; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
  LinIf_SchedTabDataPtr->LinIf_PreviousContinuousSlot    = (uint8)0x00u; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
  LinIf_SchedTabDataPtr->LinIf_CurrentSlotDelay          = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
  LinIf_SchedTabDataPtr->LinIf_RequestedTable            = LinIf_Schedule_Invalid; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
  LinIf_SchedTabDataPtr->LinIf_SchedReqConfirmTable      = LinIf_Schedule_Invalid; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
  LinIf_SchedTabDataPtr->LinIf_EvtCollResolvingState     = LinIf_EvtCollResolvingState_Idle; /* SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR */
#endif
}

/***********************************************************************************************************************
 *
 * END - LinIf Schedule Handling - Local Functions
 *
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL LINIF FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinIf Initialization, Task and General Function Interface - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_InitMemory()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, LINIF_CODE) LinIf_InitMemory
(
  void
)
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 Set initialization detection flags for LinIf and LinTp to UNINIT */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  LinIf_Dev_InitDetect  = LINIF_UNINIT;

# if ( LINIF_TP_SUPPORTED == STD_ON )
  /* LIN TP set to uninit state after reset */
  LinTp_Dev_InitDetect  = LINTP_UNINIT;
# endif
#endif
}

/***********************************************************************************************************************
 *  LinIf_Init()
 **********************************************************************************************************************/
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
FUNC(void, LINIF_CODE) LinIf_Init
(
  P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_INIT_DATA)   ConfigPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least                                         linIfChannelTmp;
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  uint8_least                                         indexTmp;
#endif
  uint8                                               errorId;

#if ( LINIF_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Start(RtmConf_RtmMeasurementPoint_LinIf_Init);  /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif
  LINIF_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is already initialized */
  if ( LinIf_Dev_InitDetect == LINIF_INIT )
  {
    errorId = LINIF_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
#if ( LINIF_USE_INIT_POINTER == STD_ON )
    LinIf_ConfigDataPtr         = ConfigPtr;
    /* #20 Check validity of config pointer (only post-build) */
    if (LinIf_ConfigDataPtr == (P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_INIT_DATA))NULL_PTR)
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_PARAMETER_POINTER;
# endif
# if ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_NULLPTR );
# endif
    }
# if ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    /* #30 Check generator version (only post-build loadable) */
    else if (LinIf_GetGeneratorCompatibilityVersion() != (uint16) LINIF_GENERATOR_COMPATIBILITY_VERSION)
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION );
    }
    /* #40 Check magic number of config structure (only post-build loadable) */
    else if (LinIf_GetFinalMagicNumber() != LINIF_FINAL_MAGIC_NUMBER)
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER );
    }
# endif
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #50 Set LinIf initialization flag to INIT */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      LinIf_Dev_InitDetect  = LINIF_INIT;
#endif

      /* #60 Initialize all channel dependent variables for all LinIf channels (-> LinIf_ChannelInit() ) */
      for (linIfChannelTmp = 0x00u; linIfChannelTmp < LinIf_GetSizeOfChannelData(); linIfChannelTmp++)
      {
        LinIf_GetChannelData(linIfChannelTmp).LinIf_ChannelState   = LINIF_CHANNEL_UNINIT; /* SBSW_LINIF_CHANNEL_DATA_ACCESS */
        LinIf_ChannelInit((NetworkHandleType)linIfChannelTmp);
      }

      /* #70 Clear sporadic transmission flags of all sporadic slots (if sporadic frames supported) */
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
      if (0 != LinIf_GetSporUncFrmTotalNumber()) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
      { /* sporadic frames exist in current configuration - clear flags */
        for (indexTmp = 0; indexTmp < LinIf_GetSizeOfSporTransmitFlags(); indexTmp++)
        {
          LinIf_SetSporTransmitFlags(indexTmp, 0); /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS_CSL */
        }
      }
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_INIT, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

#if ( LINIF_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Stop(RtmConf_RtmMeasurementPoint_LinIf_Init);  /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( LINIF_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinIf_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                 errorId;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINIF_E_NO_ERROR;
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter versioninfo. Note: no uninit check is performed */
  if ( versioninfo == (P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo parameter with component information */
    versioninfo->vendorID           = (uint16)LINIF_VENDOR_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->moduleID           = (uint16)LINIF_MODULE_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_major_version   = (uint8)LINIF_SW_MAJOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_minor_version   = (uint8)LINIF_SW_MINOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_patch_version   = (uint8)LINIF_SW_PATCH_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETVERSIONINFO, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
}
#endif /* LINIF_VERSION_INFO_API == STD_ON */

/***********************************************************************************************************************
 *  LinIf_MainFunction()
 **********************************************************************************************************************/
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
FUNC(void, LINIF_CODE) LinIf_MainFunction
(
  void
)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT) tpCtrlTmp;
#endif
  uint8_least                                           linIfChannelTmp;
  uint8                                                 errorId;

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
# if ( LINIF_TP_SUPPORTED == STD_ON )
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
# else
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
# endif
  {
    errorId = LINIF_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Iterate over all channels: */
    for (linIfChannelTmp = 0x00u; linIfChannelTmp < LinIf_GetSizeOfChannelData(); linIfChannelTmp++)
    {
      /* #30 Update channel cycle counter */
      LinIf_GetChannelData(linIfChannelTmp).LinIf_ChannelFuncCallCycleCounter--; /* SBSW_LINIF_CHANNEL_DATA_ACCESS */

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 Call channel main function if channel cycle is ready (->LinIf_ChannelMainFunction() ) */
      if ( LinIf_GetChannelData(linIfChannelTmp).LinIf_ChannelFuncCallCycleCounter == 0 ) /* COV_LINIF_FUNC_CALL_CYCLE_CONFIG */
      {
        LinIf_GetChannelData(linIfChannelTmp).LinIf_ChannelFuncCallCycleCounter = (uint8)(LinIf_GetChannelFuncCallCycleOfChannelConfig(linIfChannelTmp)); /* SBSW_LINIF_CHANNEL_DATA_ACCESS */
        LinIf_ChannelMainFunction((NetworkHandleType)linIfChannelTmp);
      }

      /* #50 Update LinTp P2 timeout (only if LinTp is supported) */
#if ( LINIF_TP_SUPPORTED == STD_ON )
      tpCtrlTmp = LinTp_GetAddrCtrl(linIfChannelTmp);
      if (tpCtrlTmp->LinTp_P2Timer > 0)
      {
        tpCtrlTmp->LinTp_P2Timer--; /* SBSW_LINIF_TP_CTRL_PTR */
        if (tpCtrlTmp->LinTp_P2Timer == 0)
        { /* timeout occurred - indicate release */
          tpCtrlTmp->LinTp_PortNotification        = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          tpCtrlTmp->LinTp_ComModeRestore          = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
          /* clear request NAD to prevent further reception */
          tpCtrlTmp->LinTp_NAD_Requested           = 0; /* SBSW_LINIF_TP_CTRL_PTR */
        }
      }
#endif

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    } /* channel loop */

    /* #60 Call post processing function (if necessary) for upper layer interactions (->LinIf_MainFunctionPostProcessing() ) */
    /**********************************************************
     * Postprocessing of main task
     **********************************************************/
#if defined ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING )
    /* application is responsible to call LinIf_MainFunctionPostProcessing() */
#else
    /* inform upper layers after processing of all channels to reduce jitter */
    LinIf_MainFunctionPostProcessing();
#endif

  } /* LinIf_Dev_InitDetect */

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif
}

/***********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessing()
 **********************************************************************************************************************/
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
#if defined ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING )
FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessing
#else
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessing
#endif
(
  void
)
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType                                         linIfChannelTmp;
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;
#if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)     tpCtrlTmp;
  LinTp_ChannelType                                         linTpChannelTmp;
#endif
  PduInfoType                                               pduInfo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all LinIf channels: */
  for (linIfChannelTmp = 0x00u; linIfChannelTmp < LinIf_GetSizeOfChannelData(); linIfChannelTmp++)
  {
    channelDataPtrTmp = LinIf_GetAddrChannelData(linIfChannelTmp);

    /* #20 Inform BswM module about diagnostic schedule change notification (if supported) */
    /**********************************************************
    * LinTp diagnostic schedule change notification
    **********************************************************/
#if ( LINIF_TP_SUPPORTED == STD_ON )
    tpCtrlTmp = LinTp_GetAddrCtrl(linIfChannelTmp);
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
      if (tpCtrlTmp->LinTp_PortNotification != LinTp_PortNotificationIdle)
      {
        linTpChannelTmp = LinTp_GetChannelOfLinIfToLinTpChannel(linIfChannelTmp);
        /* no check for linTpChannelTmp required because port notification flag cannot be set on an inactive tp channel */
        if (LinTp_IsLinTp_SchedChangeNotifyOfChannelConfig(linTpChannelTmp)) /* COV_LINTP_SCHEDCHANGENOTIFY */
        { /* schedule change notification enabled */
          BswM_LinTp_RequestMode(LinIf_GetSystemChannelIndexOfChannelIndTable(linIfChannelTmp), (LinTp_Mode)(tpCtrlTmp->LinTp_PortNotification));
        }
        /* clear notification */
        tpCtrlTmp->LinTp_PortNotification = LinTp_PortNotificationIdle; /* SBSW_LINIF_TP_CTRL_PTR */
      }
#endif /* LINIF_TP_SUPPORTED */

    /**********************************************************
    / LinIf post notification to the PduR of last transmitted/ received frame
    **********************************************************/
    /* #30 Inform PduR about transmitted non-diagnostic frame (with upper layer PduId) */
    if ( channelDataPtrTmp->LinIf_PduRNotification == LinIf_PduRNotificationTx )
    { /* report transmitted frame to PduR with enabled interrupts */
#if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetTxConfFctList(channelDataPtrTmp->LinIf_PduRNotificationFctIndex)(channelDataPtrTmp->LinIf_PduRNotificationPduId); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
      PduR_LinIfTxConfirmation(channelDataPtrTmp->LinIf_PduRNotificationPduId);
#endif
    }
    /* #40 Inform PduR about received frame with reception data */
    else if ( channelDataPtrTmp->LinIf_PduRNotification == LinIf_PduRNotificationRx )
    { /* report received frame to PduR with enabled interrupts */
      pduInfo.SduLength = channelDataPtrTmp->LinIf_PduRNotificationPduLength;
      pduInfo.SduDataPtr = &(channelDataPtrTmp->LinIf_RxData[0]);
#if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetRxIndFctList(channelDataPtrTmp->LinIf_PduRNotificationFctIndex)(channelDataPtrTmp->LinIf_PduRNotificationPduId, &pduInfo); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
      PduR_LinIfRxIndication(channelDataPtrTmp->LinIf_PduRNotificationPduId, &pduInfo); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
#endif
    }
    else
    {
      /* nothing to be done */
    }
    /* clear pending notification to PduR if set */
    channelDataPtrTmp->LinIf_PduRNotification = LinIf_PduRNotificationIdle; /* SBSW_LINIF_CHANNEL_DATA_PTR */

    /**********************************************************
    * LinSM notification and confirmations
    **********************************************************/
    /* #50 Inform LinSM or CDD about schedule table handling (-> LinIf_MainFunctionPostProcessing_LinSM()) */
    LinIf_MainFunctionPostProcessing_LinSM(channelDataPtrTmp, linIfChannelTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

#if ( LINIF_TP_SUPPORTED == STD_ON )
    /**********************************************************
    / LinTp post notification to the PduR of last transmitted/ received TP message
    **********************************************************/
    /* #60 Inform PduR about transmitted diagnostic frame */
    if ( (tpCtrlTmp->LinTp_PduRNotification & LinTp_PduRNotificationTx) != LinTp_PduRNotificationIdle )
    { /* report transmitted frame to PduR with enabled interrupts */
      PduR_LinTpTxConfirmation(tpCtrlTmp->LinTp_PduRNSduIdTx, tpCtrlTmp->LinTp_PduRNotifyResultTx);
    }

    if ( (tpCtrlTmp->LinTp_PduRNotification & LinTp_PduRNotificationRx) != LinTp_PduRNotificationIdle )
    { /* report received frame to PduR with enabled interrupts */
      PduR_LinTpRxIndication(tpCtrlTmp->LinTp_PduRNSduIdRx, tpCtrlTmp->LinTp_PduRNotifyResultRx);
    }

# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    /**********************************************************
    / LinTp post notification to the PduR of last requested TP message of LinTp_Transmit
    **********************************************************/
    /* #70 Inform PduR about received diagnostic frame */
    if ( (tpCtrlTmp->LinTp_PduRNotification & LinTp_PduRNotificationTxTm) != LinTp_PduRNotificationIdle )
    { /* report transmitted frame to PduR with enabled interrupts */
      PduR_LinTpTxConfirmation(tpCtrlTmp->LinTp_PduRNSduIdTm, E_NOT_OK);
    }
# endif

    /* clear pending notification to PduR if set */
    tpCtrlTmp->LinTp_PduRNotification       = LinTp_PduRNotificationIdle; /* SBSW_LINIF_TP_CTRL_PTR */
#endif
  } /* channel loop */
}

/***********************************************************************************************************************
 *
 * END - LinIf Initialization, Task and General Function Interface - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf Transmission Handler
 *
 * The LinIf Transmission handler interface processes sporadic frame transmission requests.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_Transmit()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Transmit
(
  VAR(PduIdType, AUTOMATIC)                         LinTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)  PduInfoPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                    retVal;
  uint8                                             errorId;
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  uint8_least                                       indexTmp;
  uint8                                             byteMskTmp;
  uint8                                             bitMskTmp;
#endif
  /* Function must be existing for signals with cycle time in COM */

  LINIF_DUMMY_STATEMENT(LinTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  retVal  = (Std_ReturnType)E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINIF_E_NO_ERROR;
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter PduInfoPtr */
  else if ( PduInfoPtr == (P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  /* #30 Check validity of parameter LinTxPduId */
  else if ( LinTxPduId >= LinIf_GetUncTxFrmTotalNumber() )
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Search all sporadic frames for the one with given LinTxPduId and set its transmission bit flag */
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
    if ( 0 != LinIf_GetSporUncFrmTotalNumber() ) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
    { /* sporadic frames exist in current configuration */
      /* search all entries of SporadicFrameLookupTable for the entry with the given PduId */
      for (indexTmp = 0; indexTmp < LinIf_GetSporUncFrmTotalNumber(); indexTmp++)
      {
        if ( LinIf_GetTxPduIdOfSporadicFrameLookupTable(indexTmp) == LinTxPduId )
        { /* LinTxPduId matches - set transmit request flag */
          byteMskTmp = LinIf_GetByteMskOfSporadicFrameLookupTable(indexTmp);
          bitMskTmp  = LinIf_GetBitMskOfSporadicFrameLookupTable(indexTmp);
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
          if ( byteMskTmp >= LinIf_GetSizeOfSporTransmitFlags() ) /* defensive programming, condition will never be true in a valid configuration */ /* COV_LINIF_INV_STATE_ALWAYSFALSE */
          {
            errorId = LINIF_E_CONFIG;
          }
          else
# endif
          {
            LinIf_SetSporTransmitFlags( byteMskTmp, (LinIf_SporTransmitFlagsType)(LinIf_GetSporTransmitFlags(byteMskTmp) | bitMskTmp)); /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS */
          }
          break; /* quit search */
        }
      }
    }
#endif

    /* #50 Set the return value to E_OK if LinTxPduId is valid */
    /* E_NOT_OK shall only be returned if LinIf is not initialized or referenced PDU does not exist (identification is out of range) */
    /* That means E_OK should also be returned for non-sporadic frames */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    retVal = (Std_ReturnType)E_OK;  /* TxPduId exists, already checked above */
#else
    if ( LinTxPduId < LinIf_GetUncTxFrmTotalNumber() )
    { /* Only return E_OK for existing PduIds, this is independent of DET check usage */
      retVal = (Std_ReturnType)E_OK;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_TRANSMIT, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( LINIF_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinIf_CancelTransmit()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC)                                 LinTxPduId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                            retVal;
  uint8                                                     errorId;

  LINIF_DUMMY_STATEMENT(LinTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* request is always rejected by returning E_NOT_OK */
  retVal  = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTxPduId */
  else if ( LinTxPduId >= LinIf_GetUncTxFrmTotalNumber() )
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* nothing to do */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_CANCELTRANSMIT, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
}
#endif

/***********************************************************************************************************************
 *
 * END - LinIf Transmission Handler
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf Schedule Handling - Service Functions
 *
 * The schedule handler manages the different LIN schedule tables.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_ScheduleRequest()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_ScheduleRequest
(
  VAR(NetworkHandleType, AUTOMATIC)                         Channel,
  VAR(LinIf_SchHandleType, AUTOMATIC)                       Schedule
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;
  NetworkHandleType                                         linIfChannelTmp;
  Std_ReturnType                                            retVal;
  uint8                                                     errorId;

  retVal = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
#endif
  {
    linIfChannelTmp           = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() )
    {
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
#endif
    }
    else
    {
      channelDataPtrTmp       = LinIf_GetAddrChannelData(linIfChannelTmp);

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #30 Check if channel state is operational */
      if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_SLEEP )
      {
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_SCHEDULE_REQUEST_ERROR;
#endif
      }
      /* #40 Check validity of parameter Schedule */
      else if ( ( Schedule > ( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndEndIdxOfChannelConfig(linIfChannelTmp)-1) - LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(linIfChannelTmp))) )
#if (LINIF_INVALIDHNDOFSCHEDULETABLELIST == STD_ON)
             || ( LinIf_IsInvalidHndOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(linIfChannelTmp)) + Schedule) == TRUE )
#endif
              ) /* COV_LINIF_HANDLE_GAPS_CONFIG TF tf xf */
      {
        /* schedule index larger than biggest available schedule handle id on channel or schedule index is a gap in the schedule handle id range */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_SCHEDULE_REQUEST_ERROR;
#endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Store the requested schedule and mark it pending (to be set after the next slot) */
        /* continuous, run once or NULL table requested */
        channelDataPtrTmp->LinIf_SchedTabData.LinIf_RequestedTable        = Schedule; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */

        /* #60 Abort a possibly ongoing event-triggered frame collision resolving handling */
#if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
        /* collision resolving table is discontinued on a new schedule request */
        channelDataPtrTmp->LinIf_SchedTabData.LinIf_EvtCollResolvingState = LinIf_EvtCollResolvingState_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
#endif

        retVal = (Std_ReturnType)E_OK;
      }
      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_SCHEDULEREQUEST, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( LINIF_SCHEDULE_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinIf_GetScheduleInfo()
 **********************************************************************************************************************/
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
FUNC(void, LINIF_CODE) LinIf_GetScheduleInfo
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  P2VAR(LinIf_ScheduleInfoType, AUTOMATIC, LINIF_APPL_VAR) ScheduleInfo
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;
  NetworkHandleType                                         linIfChannelTmp;
  LinIf_ScheduleTableListIndStartIdxOfChannelConfigType     schedTabListIdx;
  LinIf_EntryStartIdxOfScheduleTableListType                slotListIdx;
  uint8                                                     errorId;

  /* ----- Development Error Checks ------------------------------------- */
  errorId = LINIF_E_NO_ERROR;
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  /* #30 Check validity of parameter ScheduleInfo */
  else if ( ScheduleInfo == (P2VAR(LinIf_ScheduleInfoType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    linIfChannelTmp           = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);
    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() )
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
# endif
    }
    else
    {
      channelDataPtrTmp                   = LinIf_GetAddrChannelData(linIfChannelTmp);

      /* get index into global schedule table list for current table */
      schedTabListIdx                     = LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfChannelConfig(linIfChannelTmp)) +
                                                                          channelDataPtrTmp->LinIf_SchedTabData.LinIf_CurrentTable;

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #40 Set current and requested schedule table values of ScheduleInfo parameter */
      ScheduleInfo->CurrentTable          = channelDataPtrTmp->LinIf_SchedTabData.LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULEINFO_PTR */
      ScheduleInfo->RequestedTable        = channelDataPtrTmp->LinIf_SchedTabData.LinIf_RequestedTable; /* SBSW_LINIF_SCHEDULEINFO_PTR */

      /* #50 If current table is the NULL schedule table, set slot index, length and position to zero */
      if ( ScheduleInfo->CurrentTable == LinIf_Schedule_NULL )
      {
        ScheduleInfo->CurrentSlot         = 0; /* SBSW_LINIF_SCHEDULEINFO_PTR */
        ScheduleInfo->CurrentSlotLength   = 0; /* SBSW_LINIF_SCHEDULEINFO_PTR */
        ScheduleInfo->CurrentSlotPosition = 0; /* SBSW_LINIF_SCHEDULEINFO_PTR */
      }
      /* #60 Else set slot information based on current schedule table handler state */
      else
      {
        /* the stored slot index addresses already the subsequent slot, so the current slot has to be set to the previous slot */
        if ( channelDataPtrTmp->LinIf_SchedTabData.LinIf_CurrentSlot == 0)
        {
          ScheduleInfo->CurrentSlot       = (uint8)(LinIf_GetNumberOfScheduleEntriesOfScheduleTableList(schedTabListIdx) - 1); /* SBSW_LINIF_SCHEDULEINFO_PTR */
        }
        else
        {
          ScheduleInfo->CurrentSlot       = (uint8)(channelDataPtrTmp->LinIf_SchedTabData.LinIf_CurrentSlot - 1); /* SBSW_LINIF_SCHEDULEINFO_PTR */
        }

        /* get index into global entry slot list for current slot */
        slotListIdx                       = LinIf_GetEntryStartIdxOfScheduleTableList(schedTabListIdx) + ScheduleInfo->CurrentSlot;
        /* set total delay of current slot */
        ScheduleInfo->CurrentSlotLength   = LinIf_GetDelayInTimebaseTicksOfEntry(slotListIdx); /* SBSW_LINIF_SCHEDULEINFO_PTR */
        /* set remaining delay of current slot */
        ScheduleInfo->CurrentSlotPosition = ScheduleInfo->CurrentSlotLength - channelDataPtrTmp->LinIf_SchedTabData.LinIf_CurrentSlotDelay; /* SBSW_LINIF_SCHEDULEINFO_PTR */
      }

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETSCHEDULEINFO, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/***********************************************************************************************************************
 *
 * END - LinIf Schedule Handling - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf Node Management - Service Functions
 *
 * The LinIf node management handles the transition of a LIN channel between operational and sleep state.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf_GotoSleep()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep
(
  VAR(NetworkHandleType, AUTOMATIC)                         Channel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;
  NetworkHandleType                                         linIfChannelTmp;
  Std_ReturnType                                            retVal;
  uint8                                                     errorId;

  retVal  = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
#endif
  {
    linIfChannelTmp = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      channelDataPtrTmp         = LinIf_GetAddrChannelData(linIfChannelTmp);

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #30 If channel state is operational */
      if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
      {
        /* #40 If no sleep transition is already ongoing, store gotosleep request as pending */
        if ( ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_Idle ) ||
             ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnWakeupFrameHandling_OnTrans ) )
        {
          /* Only set new request if not already on processing a request */
          channelDataPtrTmp->LinIf_OnSleepModeFrameHandling   = LinIf_OnSleepModeFrameHandling_OnReq; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
        }
      }
      /* #50 Otherwise if channel state is sleep */
      else if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_SLEEP )
      {
        /* #60 If no sleep transition is already ongoing, store gotosleep request as pending and mark it as internal request */
        /* Actually we could call Lin_GotoSleepInternal here directly because in sleep state no communication is active.
           However LINIF597 explicitly states that Lin_GotoSleepInternal is called by LinIf_MainFunction. */
        if ( ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_Idle ) ||
             ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnWakeupFrameHandling_OnTrans ) )
        {
          channelDataPtrTmp->LinIf_OnSleepModeFrameHandling   = LinIf_OnSleepModeFrameHandling_OnReqInt; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
        }
      }
      else
      {
        /* MISRA only */
      }

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      retVal = (Std_ReturnType)E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GOTOSLEEP, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinIf_Wakeup()
 **********************************************************************************************************************/
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
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Wakeup
(
  VAR(NetworkHandleType, AUTOMATIC)                         Channel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtrTmp;
  NetworkHandleType                                         linIfChannelTmp;
  Std_ReturnType                                            retVal;
  uint8                                                     errorId;

  retVal = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
#endif
  {
    linIfChannelTmp                                   = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);
    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() )
    {
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
#endif
    }
    else
    {
      /* ----- Implementation ----------------------------------------------- */
      channelDataPtrTmp                               = LinIf_GetAddrChannelData(linIfChannelTmp);

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #30 If channel state is operational */
      if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
      {
        /* #40 If a gotosleep transition has been requested but not been started */
        if ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReq )
        {
          /* #50 Reset pending gotosleep request */
          /* Clear current pending condition, sleep mode frame not yet started */
          channelDataPtrTmp->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */

          /* #60 Confirm the wakeup */
          /* Call function directly called, so no wakeup is send, remain in operational state, inform LinSM */
#if ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY == STD_ON )
          /* FALSE returned because no wakeup frame is sent */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
          LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(linIfChannelTmp))(Channel, FALSE); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
          LinSM_WakeupConfirmation(Channel, FALSE);
# endif
#else
          /* TRUE returned because wakeup is accepted according to AR4-612 (AR4.1.x) */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
          LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(linIfChannelTmp))(Channel, TRUE); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
          LinSM_WakeupConfirmation(Channel, TRUE);
# endif
#endif
        }
        /* #70 If a gotosleep frame is currently transmitted */
       else if ( channelDataPtrTmp->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTrans )
        {
          /* #80 Set flag to send wakeup frame afterwards */
          /* Sleep mode frame has been started, but has not yet been checked */
          channelDataPtrTmp->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_OnReWakeInt; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
        }
        /* #90 Otherwise just confirm the wakeup to upper layer because the state is already operational */
        else
        {
          /* Ignore request */
          /* Call function directly called, so no wakeup is send, to inform upper layer */
#if ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY == STD_ON )
          /* FALSE returned because no wakeup frame is sent */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
          LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(linIfChannelTmp))(Channel, FALSE); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
          LinSM_WakeupConfirmation(Channel, FALSE);
# endif
#else
          /* TRUE returned because wakeup is accepted according to AR4-612 (AR4.1.x) */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
          LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(linIfChannelTmp))(Channel, TRUE); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
          LinSM_WakeupConfirmation(Channel, TRUE);
# endif
#endif
        }
      }
      /* #100 Otherwise if channel state is sleep */
      else if ( channelDataPtrTmp->LinIf_ChannelState == LINIF_CHANNEL_SLEEP ) /* COV_LINIF_MISRA_ELSEBRANCH */
      {
        /* #110 Set channel state to operational */
        /* Only accept WakeUp in sleep state */
        channelDataPtrTmp->LinIf_ChannelState               = LINIF_CHANNEL_OPERATIONAL; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */

        /* #120 If the external wakeup flag is not set, start transmission of a wakeup frame (-> Lin_Wakeup()) and start internal wakeup delay timer */
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
        if (channelDataPtrTmp->LinIf_WakeupFlag == (uint8)0x00u)
#endif
        { /* no external wakeup was indicated previously */

          /* Wakeup frame has been started, but has not yet been checked */
          channelDataPtrTmp->LinIf_OnSleepModeFrameHandling = LinIf_OnWakeupFrameHandling_OnTrans; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
          channelDataPtrTmp->LinIf_CurrentFrameType         = LinIf_FrameType_WakeUpFrame; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */

          /* Check status of LIN Driver in next main function call */
          channelDataPtrTmp->LinIf_FrameMaxDelay            = (LinIf_ScheduleTableTickType)0x01u; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */

#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
          (void) LinIf_GetLin_WakeupFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(linIfChannelTmp)) (LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelTmp)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
          (void) LinIf_Lin_Wakeup_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelTmp));
#endif

          /* start internal wakeup delay */
          channelDataPtrTmp->LinIf_WakeupDelayTimer         = LinIf_GetWakeupDelayInternalOfChannelConfig(linIfChannelTmp); /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
        }
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
        /* #130 If the external wakeup flag is set, set the LIN driver to operational state (-> Lin_WakeupInternal() ) and start external wakeup delay timer */
        else
        {
          /* external wakeup already reported by driver or transceiver, suppress active wakeup frame transmission */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
          (void) LinIf_GetLin_WakeupInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(linIfChannelTmp)) (LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelTmp)); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
          (void) LinIf_Lin_WakeupInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelTmp));
# endif

# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
          LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(linIfChannelTmp))(Channel, TRUE); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
          LinSM_WakeupConfirmation(Channel, TRUE);
# endif

          /* start external wakeup delay */
          channelDataPtrTmp->LinIf_WakeupDelayTimer         = LinIf_GetWakeupDelayExternalOfChannelConfig(linIfChannelTmp); /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
        }
#endif
      }
      else
      {
        /* MISRA only */
      }

      /* #150 Clear wakeup flag in any case */
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
      channelDataPtrTmp->LinIf_WakeupFlag                   = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR_TMP */
#endif

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      retVal                                                = (Std_ReturnType)E_OK;
    }
  }

#if ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY == STD_ON )
  /* no wakeup confirmation called if E_NOT_OK is returned */
#else
  if ( retVal == (Std_ReturnType)E_NOT_OK)
  { /* according to AR4-612, inform LinSM with parameter FALSE if wakeup is not accepted */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    /* possible invalid channel parameter, not possible to select valid configured upper layer */
# else
    /* FALSE returned because wakeup is accepted according to AR4-612 (AR4.1.x) */
    LinSM_WakeupConfirmation(Channel, FALSE);
# endif
  }
#endif

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_WAKEUP, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  LinIf_CheckWakeup()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC)                 WakeupSource
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                        retVal;
  uint8_least                                           linIfChannelLoopIndex;
# if ( LINIF_TRCV_HANDLING == STD_ON )
#  if ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  uint8                                                 transceiverChannelTmp;
  uint8                                                 transceiverNrTmp;
#  endif
# endif
  uint8                                                 errorId;

  LINIF_DUMMY_STATEMENT(WakeupSource); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  retVal                                                = (Std_ReturnType)E_NOT_OK;
  errorId                                               = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for each LinIf channel if given wakeup source is configured for this channel */
    for (linIfChannelLoopIndex = 0; linIfChannelLoopIndex < LinIf_GetSizeOfChannelData(); linIfChannelLoopIndex++)
    {
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON )
      /* #30 If wakeup source belongs to LIN driver module, call Lin_CheckWakeup for the identified channel */
      if ( (LinIf_GetLinWakeupSourceOfChannelToDrvWakeupInfo(linIfChannelLoopIndex) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#  if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
        (void) LinIf_GetLin_CheckWakeupFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(linIfChannelLoopIndex)) (LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelLoopIndex)); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
        (void) LinIf_Lin_CheckWakeup_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelLoopIndex));
#  endif
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        retVal = (Std_ReturnType)E_OK;
      }
# endif

# if ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
      /* #40 If wakeup source belongs to a LIN transceiver, call LinTrcv_CheckWakeup for the identified transceiver and channel */
      if ( (LinIf_GetLinTrcvWakeupSourceOfChannelToTrcvWakeupInfo(linIfChannelLoopIndex) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        /* LIN Transceiver Driver reports a woken up channel with EcuM_SetWakeupEvent() */
        transceiverChannelTmp   = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelLoopIndex);
        /* at this point the transceiver channel index cannot be invalid because a wakeup source is assigned to it, no need for check of LinIf_InvalidLinTrcv */
        transceiverNrTmp      = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelLoopIndex);
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        (void) LinIf_GetLinTrcv_CheckWakeupFct(transceiverNrTmp)(transceiverChannelTmp); /* SBSW_LINIF_FCT_POINTER_CALL */
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        retVal = (Std_ReturnType)E_OK;
      }
# endif
    }

    /* #50 If wakeup source does not belong to any Lin channel: report error */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if (retVal != E_OK)
    {
      errorId = LINIF_E_PARAM_WAKEUPSOURCE;
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_CHECKWAKEUP, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
}
#endif

/***********************************************************************************************************************
 *  LinIf_WakeupConfirmation()
 **********************************************************************************************************************/
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
FUNC(void, LINIF_CODE) LinIf_WakeupConfirmation
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC)                 WakeupSource
)
{
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                 errorId;
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  uint8                                                 wakeupsourceFound;
# endif
  uint8                                                 indexTmp;

  errorId           = LINIF_E_NO_ERROR;
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  wakeupsourceFound = 0;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for each LinIf channel if given wakeup source is configured for this channel */
    for (indexTmp = 0; indexTmp < LinIf_GetSizeOfChannelData(); indexTmp++)
    {
      /* #30 If wakeup source belongs to a Lin (driver) channel, set the external wakeup flag for the identified channel */
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON )
      if ( (LinIf_GetLinWakeupSourceOfChannelToDrvWakeupInfo(indexTmp) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        LinIf_GetChannelData(indexTmp).LinIf_WakeupFlag = (uint8)0x01u; /* SBSW_LINIF_WAKEUPCONF_CHANNEL_DATA */
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
        wakeupsourceFound = 1;
#  endif
      }
# endif

# if ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
      /* #40 If wakeup source belongs to a Lin transceiver channel, set the external wakeup flag for the identified channel */
      if ( (LinIf_GetLinTrcvWakeupSourceOfChannelToTrcvWakeupInfo(indexTmp) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        LinIf_GetChannelData(indexTmp).LinIf_WakeupFlag = (uint8)0x01u; /* SBSW_LINIF_WAKEUPCONF_CHANNEL_DATA */
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
        wakeupsourceFound = 1;
#  endif
      }
# endif
    }

    /* #50 If wakeup source does not belong to any Lin channel: report error */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
    if (wakeupsourceFound == 0)
    {
      errorId = LINIF_E_PARAM_WAKEUPSOURCE;
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_WAKEUPCONFIRMATION, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
#endif
}

/***********************************************************************************************************************
 *
 * END - LinIf Node Management
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinIf Transceiver Handler
 *
 * Handles the interface to one or more LIN transceivers.
 **********************************************************************************************************************/
#if ( LINIF_TRCV_HANDLING == STD_ON )

/***********************************************************************************************************************
 *  LinIf_SetTrcvMode()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC)         Channel,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC)      TransceiverMode
)
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType                         linIfChannelTmp;
  Std_ReturnType                            retVal;
  uint8                                     transceiverChannelTmp;
  uint8                                     transceiverNrTmp;
  uint8                                     errorId;

  retVal  = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter TransceiverMode */
  /* PRQA S 3355,3356,3359 1 */ /* MD_LIN_IF_13.7_Trcv */
  else if ( (TransceiverMode != LINTRCV_TRCV_MODE_NORMAL) && (TransceiverMode != LINTRCV_TRCV_MODE_SLEEP) && (TransceiverMode != LINTRCV_TRCV_MODE_STANDBY) )
  { /* PRQA S 3201 */ /* MD_LIN_IF_14.1 */
    errorId = LINIF_E_TRCV_INV_MODE;
  }
  else
# endif
  {
    linIfChannelTmp = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV XF */
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
    }
    else
    {
      /* #40 Check if transceiver instance exists on given channel */
      transceiverChannelTmp = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelTmp);

      if ( transceiverChannelTmp == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV XF */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Get transceiver instance and set given mode (-> LinTrcv_<Trcv>_SetOpMode()) */
        transceiverNrTmp    = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelTmp);
        retVal              = LinIf_GetLinTrcv_SetOpModeFct(transceiverNrTmp)(transceiverChannelTmp, TransceiverMode); /* SBSW_LINIF_FCT_POINTER_CALL */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_SETTRCVMODE, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinIf_GetTrcvMode()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC)                           Channel,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR)      TransceiverModePtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType                                           linIfChannelTmp;
  Std_ReturnType                                              retVal;
  uint8                                                       transceiverChannelTmp;
  uint8                                                       transceiverNrTmp;
  uint8                                                       errorId;

  retVal  = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter TransceiverModePtr */
  else if ( TransceiverModePtr == (P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
    linIfChannelTmp = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV XF */
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
    }
    else
    {
      /* #40 Check if transceiver instance exists on given channel */
      transceiverChannelTmp = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelTmp);

      if ( transceiverChannelTmp == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV XF */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Get transceiver instance and retrieve transceiver mode (-> LinTrcv_<Trcv>_GetOpMode() ) */
        transceiverNrTmp    = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelTmp);
        retVal              = LinIf_GetLinTrcv_GetOpModeFct(transceiverNrTmp)(transceiverChannelTmp, TransceiverModePtr); /* SBSW_LINIF_FCT_POINTER_CALL */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETTRCVMODE, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinIf_GetTrcvWakeupReason()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason
(
  VAR(NetworkHandleType, AUTOMATIC)                               Channel,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR)  TrcvWuReasonPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType                                               linIfChannelTmp;
  Std_ReturnType                                                  retVal;
  uint8                                                           transceiverChannelTmp;
  uint8                                                           transceiverNrTmp;
  uint8                                                           errorId;
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  LinTrcv_TrcvModeType                                            transceiverModePtr;
  Std_ReturnType                                                  retTrcvMode;
# endif

  retVal  = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter TrcvWuReasonPtr */
  else if ( TrcvWuReasonPtr == (P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    /* #40 Check if transceiver is in normal mode */
    retTrcvMode = LinIf_GetTrcvMode(Channel, &transceiverModePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    if ( (retTrcvMode == E_OK) && (transceiverModePtr != LINTRCV_TRCV_MODE_NORMAL) )
    {
      errorId = LINIF_E_TRCV_NOT_NORMAL;
    }
    else
# endif
    {
      linIfChannelTmp         = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

      if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV XF */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* #50 Check if transceiver instance exists on given channel */
        transceiverChannelTmp = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelTmp);

        if ( transceiverChannelTmp == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV XF */
        {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
          errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
        }
        else
        {
          /* ----- Implementation ----------------------------------------------- */
          /* #60 Get transceiver instance and retrieve wakeup reason (-> LinTrcv_<Trcv>_GetBusWuReason()) */
          transceiverNrTmp    = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelTmp);
          retVal              = LinIf_GetLinTrcv_GetBusWuReasonFct(transceiverNrTmp)(transceiverChannelTmp, TrcvWuReasonPtr); /* SBSW_LINIF_FCT_POINTER_CALL */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETTRCVWAKEUPREASON, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinIf_SetTrcvWakeupMode()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvWakeupMode
(
  VAR(NetworkHandleType, AUTOMATIC)           Channel,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC)  LinTrcvWakeupMode
)
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType                           linIfChannelTmp;
  Std_ReturnType                              retVal;
  uint8                                       transceiverChannelTmp;
  uint8                                       transceiverNrTmp;
  uint8                                       errorId;

  retVal  = (Std_ReturnType)E_NOT_OK;
  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetNumberOfSystemChannels() )
  {
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter LinTrcvWakeupMode */
  /* PRQA S 3355,3356,3359 1 */ /* MD_LIN_IF_13.7_Trcv */
  else if ( !((LinTrcvWakeupMode == LINTRCV_WUMODE_ENABLE) || (LinTrcvWakeupMode == LINTRCV_WUMODE_DISABLE) || (LinTrcvWakeupMode == LINTRCV_WUMODE_CLEAR)) )
  { /* PRQA S 3201 */ /* MD_LIN_IF_14.1 */
    errorId = LINIF_E_PARAMETER;
  }
  else
# endif
  {
    linIfChannelTmp         = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

    if ( linIfChannelTmp >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV XF */
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
    }
    else
    {
      /* #40 Check if transceiver instance exists on given channel */
      transceiverChannelTmp = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelTmp);

      if ( transceiverChannelTmp == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV XF */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Get transceiver instance and set wakeup mode (-> LinTrcv_<Trcv>_SetWakeupModeFct()) */
        transceiverNrTmp    = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelTmp);
        retVal              = LinIf_GetLinTrcv_SetWakeupModeFct(transceiverNrTmp)(transceiverChannelTmp, LinTrcvWakeupMode); /* SBSW_LINIF_FCT_POINTER_CALL */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_SETTRCVWAKEUPMODE, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#endif /* LINIF_TRCV_HANDLING == STD_ON */

/***********************************************************************************************************************
 *
 * END - LinIf Transceiver Handler
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LIN TRANSPORT PROTOCOL
 *
 * Description:  Implementation of the AUTOSAR LIN Transport Protocol (part of LIN Interface)
 *
 **********************************************************************************************************************/

#if ( LINIF_TP_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *  LOCAL LINTP FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinTp Buffer Handling - Local Functions
 *
 * Handling of transmission / reception data and buffer.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_BufferPolling()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_BufferPolling
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)  LinTp_CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                              retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If TX connection active but not sufficient data for transmission available */
  if ( LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_TX_INVALID)
  {
    /* #20 Request transmission data from PduR (-> LinTp_TxDataRequest() ) */
    retVal = LinTp_TxDataRequest(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */

    /* #30 If PduR request is denied, abort TX connection */
    if (E_NOT_OK == retVal)
    { /* unable to obtain data from PduR - permanent failure */
      LinTp_TxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
    }
  }
  /* #40 Otherwise if RX connection active but not sufficient reception buffer previously indicated by PduR */
  else if ( (LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED ) || (LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START) )
  {
    /* #50 Poll PduR again if receive buffer is available (-> LinTp_RxDataBufferHandling() ) */
    /* provide NULL pointer as second argument as the buffer handling is performed on existing local buffer */
    LinTp_RxDataBufferHandling(LinTp_CtrlPtr, NULL_PTR); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
  }
  else
  {
    /* No TP connection active or data available, nothing to do */
  }
}

/**********************************************************************************************************************
 *
 *  LinTp Buffer Handling - Rx
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_StartOfReception()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_StartOfReception
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) LinTp_CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                           bufferReqRetVal;
  PduLengthType                                               remainingPduRBufferSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Start actual reception handling with PduR (->PduR_LinTpStartOfReception()) */
  bufferReqRetVal = PduR_LinTpStartOfReception(LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id, NULL_PTR, LinTp_CtrlPtr->LinTp_RemainingLength, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_START_RX */

  /* #20 If request is accepted and available buffer size sufficient, proceed */
  if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) && (remainingPduRBufferSize >= LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length ) )
  {
    /* sufficient buffer can be provided for first frame, just proceed */
    LinTp_CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_BUSY_RX; /* SBSW_LINIF_TP_CTRL_PTR */
  }
  /* #30 Otherwise: */
  else
  {
    /* #40 Reset TP connection */
    LinTp_CtrlPtr->LinTp_Timer                  = 0; /* SBSW_LINIF_TP_CTRL_PTR */
    LinTp_CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
    LinTp_CtrlPtr->LinTp_NAD_Requested          = 0; /* SBSW_LINIF_TP_CTRL_PTR */
    if ( LinTp_CtrlPtr->LinTp_PortNotification == LinTp_PortNotificationIdle ) /* COV_LINTP_FUNCREQUEST_SRF TX */
    { /* No new functional request pending */
      LinTp_CtrlPtr->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
    }
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    LinTp_CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
    /* clear P2 timer */
    LinTp_CtrlPtr->LinTp_P2Timer                = 0; /* SBSW_LINIF_TP_CTRL_PTR */

    /* #50 Report termination of reception if request was not rejected but provided buffer is too small */
    if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) || (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_E_BUSY) )
    {
      /* report abort of reception to PduR, e.g. if the provided buffer is too small.
          Actually this should never happen - would be a configuration fault in PduR */
      LinTp_CtrlPtr->LinTp_PduRNotification    |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotifyResultRx   = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNSduIdRx         = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
    }
    /* else: nothing to report to PduR, no connection was established */

  }
}

/***********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_CopyRxData()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_CopyRxData
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) LinTp_CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                           bufferReqRetVal;
  PduLengthType                                               remainingPduRBufferSize;
  PduLengthType                                               succFramePayloadSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request PduR to copy provided reception data (->PduR_LinTpCopyRxData() ) */
  bufferReqRetVal = PduR_LinTpCopyRxData( LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id, &LinTp_CtrlPtr->LinTp_PduInfoPtr, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_RX */

  /* #20 If request is accepted */
  if ( bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK )
  {
    /* Rx data copied by PduR */
    /* #30 If all expected data of whole message is received
     *     Close TP connection and inform PduR about completed successful reception */
    LinTp_CtrlPtr->LinTp_RemainingLength          = LinTp_CtrlPtr->LinTp_RemainingLength - LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength; /* SBSW_LINIF_TP_CTRL_PTR */
    if (LinTp_CtrlPtr->LinTp_RemainingLength == 0)
    {
      LinTp_CtrlPtr->LinTp_Timer                  = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotifyResultRx     = E_OK; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNSduIdRx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */

      /* reset requested NAD and change schedule only if previous received frame was not a response pending */
# if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
      if (LinTp_CtrlPtr->LinTp_RespPendingCounter == 0)
# endif
      { /* no response pending frame */
        LinTp_CtrlPtr->LinTp_NAD_Requested = 0; /* SBSW_LINIF_TP_CTRL_PTR */

        if ( LinTp_CtrlPtr->LinTp_PortNotification == LinTp_PortNotificationIdle ) /* COV_LINTP_FUNCREQUEST_SRF TX */
        { /* No new functional request pending */
          LinTp_CtrlPtr->LinTp_PortNotification   = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
        }
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
        LinTp_CtrlPtr->LinTp_ComModeRestore       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
        /* clear P2 timer */
        LinTp_CtrlPtr->LinTp_P2Timer              = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      }
    }
    /* #50 Otherwise: more frames expected */
    else
    {
      /* #60 Calculate payload size of next frame */
      if ( LinTp_CtrlPtr->LinTp_RemainingLength < 6 )
      {
        succFramePayloadSize                = LinTp_CtrlPtr->LinTp_RemainingLength;
      }
      else
      {
        succFramePayloadSize                = 6;
      }

      /* #70 If next frame fits in remaining PduR buffer just proceed, otherwise enter pending state (poll buffer, no SRF header will be transmitted) */
      if ( remainingPduRBufferSize >= succFramePayloadSize)
      {
        /* next frame can be stored, just proceed */
        LinTp_CtrlPtr->LinTp_Ch_State             = LINTP_CHANNEL_BUSY_RX; /* SBSW_LINIF_TP_CTRL_PTR */
      }
      else
      {
        /* next frame cannot be stored, however local buffer is empty in any case, so schedule next header */
        /* This case is only entered when a FF or CF was handled, received data was successfully given to PduR
            but the remaining buffer size is too small to store the following CF. For efficiency, the next CF
            header is scheduled anyway but remember to check PduR for reception buffer:
            - either there will be enough free buffer available so the handling is as usual (but no header
              transmission delay was introduced!)
            - or still not sufficient buffer available, then transit to LINTP_CHANNEL_BUSY_RX_SUSPENDED state */
        LinTp_CtrlPtr->LinTp_Ch_State           = LINTP_CHANNEL_BUSY_RX_POLLBUFFER; /* SBSW_LINIF_TP_CTRL_PTR */
      }
    }
  }
  /* #80 Otherwise: */
  else
  {
    /* #90 Quit reception and report abortion to PduR */
    LinTp_RxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  }
}

/***********************************************************************************************************************
 *  LinTp_RxDataBufferHandling()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                   LinTp_LinSduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                           bufferReqRetVal;
  uint8_least                                                 byteIndex;
  PduLengthType                                               remainingPduRBufferSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a PduR notification is already pending in this cycle */
  if ( (LinTp_CtrlPtr->LinTp_PduRNotification & LinTp_PduRNotificationRx) == LinTp_PduRNotificationIdle )
  { /* no PduR event to notify in this cycle, proceed with normal handling */

    /* #20 If reception handling to PduR not yet started, initiate connection (->LinTp_RxDataBufferHandling_StartOfReception()) */
    if (LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START)
    { /* actual start of PduR reception handling */
      LinTp_RxDataBufferHandling_StartOfReception(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }

    /* #30 If start of reception handling succeeded or connection already established, update local buffer */
    if ( ( LinTp_LinSduPtr == NULL_PTR ) || ( LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_IDLE ) )
    {
      /* repeated call or StartOfReception call failed, thus SduDataPtr already point to local buffer or receive data not needed */
    }
    else
    {
      LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength = LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length; /* SBSW_LINIF_TP_CTRL_PTR */
      /* copy data to local buffer in any case for data consistency, important for delayed handling */
      for (byteIndex = 0; byteIndex < LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length; byteIndex++)
      {
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[byteIndex] = LinTp_LinSduPtr[(LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + byteIndex) & LINTP_LOCAL_BUFFER_SIZE_MASK]; /* SBSW_LINIF_TP_CTRL_BUFFER_ARRAY */
      }
      /* set pdu pointer given to PduR to local received data - note that directly point to LinTp_LinSduPtr is not consistent as it may get overwritten(!) */
      LinTp_CtrlPtr->LinTp_PduInfoPtr.SduDataPtr = &LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[0]; /* SBSW_LINIF_TP_CTRL_PTR */
    }

    /* delayed reception buffer handling */
    /* #40 If temporarily no sufficient receive buffer is available */
    if ( (LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED) || (LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_POLLBUFFER) )
    {
      /* #50 Poll PduR for current buffer size (by passing 0 as length) */
      /* continue reception if data can be copied in the next call */
      LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength         = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      bufferReqRetVal = PduR_LinTpCopyRxData( LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id, &LinTp_CtrlPtr->LinTp_PduInfoPtr, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_RX */
      LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength         = LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length; /* SBSW_LINIF_TP_CTRL_PTR */

      /* #60 If PduR indicates sufficient buffer, continue normal reception handling, otherwise transit to suspended state */
      if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) && (remainingPduRBufferSize >= LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length) )
      {
        /* buffer available, provide below the received data to PduR */
        LinTp_CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_RX; /* SBSW_LINIF_TP_CTRL_PTR */
      }
      else
      {
        /* If we reach this location in state LINTP_CHANNEL_BUSY_RX_POLLBUFFER, LinTp_LinSduPtr is never NULL.
           The SRF response has just been received, however local TP buffer occupied and no Pdur buffer available, suppress further SRF header */
        LinTp_CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_RX_SUSPENDED; /* SBSW_LINIF_TP_CTRL_PTR */
      }
    }

    /* #70 If receive buffer available, provide received data to PduR (-> LinTp_RxDataBufferHandling_CopyRxData() ) */
    if ( LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX )
    { /* valid state, provide receive data to upper layer */
      LinTp_RxDataBufferHandling_CopyRxData(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
  /* #80 Otherwise: PduR notification pending, postpone buffer handling till next cycle */
  else
  {
    /* postpone buffer handling till next cycle in order to not overwrite post notification */
    if (LinTp_LinSduPtr != NULL_PTR)
    {
      /* prepare pdu info structure */
      LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength = LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduInfoPtr.SduDataPtr = &LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[0]; /* SBSW_LINIF_TP_CTRL_PTR */
      for (byteIndex = 0; byteIndex < LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length; byteIndex++)
      {
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[byteIndex] = LinTp_LinSduPtr[(LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + byteIndex) & LINTP_LOCAL_BUFFER_SIZE_MASK]; /* SBSW_LINIF_TP_CTRL_BUFFER_ARRAY */
      }
    }
  }
}

/**********************************************************************************************************************
 *
 *  LinTp Buffer Handling - TX
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_TxDataRequest()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
LINIF_LOCAL FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxDataRequest
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)  LinTp_CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                              retVal;
  BufReq_ReturnType                                           bufferReqRetVal;
  uint8                                                       byteIndex;
  PduLengthType                                               remainingPduRBufferSize;  /* dummy variable for call, not used till now */

  /* ----- Implementation ----------------------------------------------- */
  retVal = (Std_ReturnType)E_OK;
  LinTp_CtrlPtr->LinTp_Ch_State                           = LINTP_CHANNEL_BUSY_TX_INVALID; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */

  /* #10 Request PduR to copy transmission data into provided local Tp buffer */
  bufferReqRetVal = PduR_LinTpCopyTxData(LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id, &(LinTp_CtrlPtr->LinTp_PduInfoPtr), NULL_PTR, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_TX */

  /* #20 If transmission data is successfully provided */
  if ( bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK )
  { /* all requested data was copied into local buffer */

    /* #30 In case it's the last frame in current connection, stuff frame will fill bytes */
    LinTp_CtrlPtr->LinTp_RemainingLength                  = LinTp_CtrlPtr->LinTp_RemainingLength - LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
    if (LinTp_CtrlPtr->LinTp_RemainingLength == 0)
    {
      /* SF or last CF - check if stuffing is necessary */
      if ((LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength) < 8)
      {
        /* stuff with fill bytes */
        for (byteIndex = (uint8)(LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength); byteIndex < 8; byteIndex++)
        {
          LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[byteIndex]  = 0xFF; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
        }
      }
      LinTp_CtrlPtr->LinTp_RemainingLength                = 0; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
    }
    /* else: more frames to come */

    LinTp_CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_TX_VALID; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
  }
  /* #40 Otherwise if PduR does not accept request, abort or delay transmission */
  else
  {
    if ( bufferReqRetVal == (BufReq_ReturnType)BUFREQ_E_BUSY )
    {
      /* retry in next main cycle, return E_OK to not abort request */
    }
    else /* (BufReq_ReturnType)BUFREQ_E_NOT_OK ) */
    {
      /* abort transmission */
      retVal = (Std_ReturnType)E_NOT_OK;
    }
  }

  return(retVal);
}

/***********************************************************************************************************************
 *
 * END - LinTp Buffer Handling
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinTp Reception Handling - Local Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_RxIndication_SF()
 **********************************************************************************************************************/
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
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication_SF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                   LinTp_LinSduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean                                                     proceedHandlingFlag;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if NAD of response match the NAD of request */
  if ( LinTp_GetNADOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id) == LinTp_LinSduPtr[0] ) /* COV_LINTP_STRICTNADCHECK_UNREACHEDCODE TX */
  {
    /* #20 Check if length information in SF is valid */
    if ( (LinTp_LinSduPtr[1] > 0x00u) && (LinTp_LinSduPtr[1] <= 0x06u) )
    {
      proceedHandlingFlag = TRUE;

      /* #30 If RX connection already active on channel, abort it and inform PduR */
      if( ( LinTp_CtrlPtr->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK) != 0 )
      { /* Indicate error and restart reception with new SF */
        LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNSduIdRx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* Upper layer ID of currently active connection */ /* SBSW_LINIF_TP_CTRL_PTR */
      }
      /* #40 Otherwise if received frame is a response pending frame (in this case channel state always idle) */
      else if ( (LinTp_LinSduPtr[1] == 0x03u) &&
                (LinTp_LinSduPtr[2] == 0x7Fu) &&
                (LinTp_LinSduPtr[4] == 0x78u) )
      { /* response pending frame */

        /* #50 Update RP counter and check if maximum allowed number of response pending frames received */
        LinTp_CtrlPtr->LinTp_RespPendingCounter++; /* SBSW_LINIF_TP_CTRL_PTR */
        if (LinTp_CtrlPtr->LinTp_RespPendingCounter > LinTp_GetMaxRespPendingFrames())
        {
          /*#60 Abort reception, indication to PduR depends if forward-RP-frames-to-PduR feature activated, stop further handling */
# if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
          /* do only report the stop of reception if a connection was established, thus if preceding RP frames were reported to PduR */
          LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_CTRL_PTR */
          LinTp_CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
          LinTp_CtrlPtr->LinTp_PduRNSduIdRx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
          /* reset requested NAD - only one response accepted for each request */
          LinTp_CtrlPtr->LinTp_NAD_Requested          = 0; /* SBSW_LINIF_TP_CTRL_PTR */
          LinTp_CtrlPtr->LinTp_P2Timer                = 0; /* important to abort timeout handling */ /* SBSW_LINIF_TP_CTRL_PTR */

          LinTp_CtrlPtr->LinTp_PortNotification       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          LinTp_CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
          /* no further handling required */
          proceedHandlingFlag = FALSE;
        }
        else
        {
          /* #70 Reinitialize P2 timeout with P2Max */
          LinTp_CtrlPtr->LinTp_P2Timer = LinTp_GetP2MaxTime(); /* SBSW_LINIF_TP_CTRL_PTR */
        }

        /* #80 If forward-RP-frames-to-PduR feature deactivated, stop further handling */
# if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
# else
        proceedHandlingFlag = FALSE;
# endif
      /* #90 Otherwise: Normal SF received */
      }
      else
      { /* reset RP counter to trigger schedule change request in LinTp_Rx_Fill_Buffer */
        LinTp_CtrlPtr->LinTp_RespPendingCounter = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      }

      if ( proceedHandlingFlag == TRUE )
      {
        /* #100 Initialize new RX connection */
        LinTp_CtrlPtr->LinTp_Timer                       = LinTp_GetNcrOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_TimerReload                 = LinTp_GetNcrOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_RemainingLength             = (PduLengthType)(LinTp_LinSduPtr[1]); /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length         = (uint8)(LinTp_CtrlPtr->LinTp_RemainingLength); /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx            = 2; /* index of the buffer copied from - SF:2 */ /* SBSW_LINIF_TP_CTRL_PTR */

        LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id             = LinTp_GetUpperLayerPduIdOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR */

        /* #110 Reset P2 timer */
# if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
# else
        /* stop P2 timer after receiving start of response */
        LinTp_CtrlPtr->LinTp_P2Timer                     = 0; /* SBSW_LINIF_TP_CTRL_PTR */
# endif

        LinTp_CtrlPtr->LinTp_Ch_State                    = LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START; /* SBSW_LINIF_TP_CTRL_PTR */

        /* #120 Start receive buffer handling (-> LinTp_RxDataBufferHandling() ) */
        /* do buffer handling in any case, on suspended state data is copied to local buffer */
        LinTp_RxDataBufferHandling(LinTp_CtrlPtr, LinTp_LinSduPtr); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
      }
    }
    /* else: ignore SF with invalid length - conform to LIN standard but not fully LINIF652, see bugzilla 52237 */
  }
  /* else: entered only for unknown (not configured) NAD and Strict NAD check disabled - ignore frame */

} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinTp_RxIndication_FF()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication_FF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                     LinTp_LinSduPtr
)
{
  /* #10 Check if NAD of response match the NAD of request */
  if ( LinTp_GetNADOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id) == LinTp_LinSduPtr[0] ) /* COV_LINTP_STRICTNADCHECK_UNREACHEDCODE TX */
  {
    /* #20 Check if length information in FF is valid */
    if ( ( ((uint16)(LinTp_LinSduPtr[1] & 0x0F) << 8) | (LinTp_LinSduPtr[2]) ) > 6)
    { /* valid FF length */

      /* #30 If RX connection already active on channel, abort it and inform PduR */
      if( ( LinTp_CtrlPtr->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK) != 0 )
      { /* Indicate error and postpone actual reception handling of FF to next cycle */
        LinTp_CtrlPtr->LinTp_Timer                  = 0; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNSduIdRx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
      }

      /* #40 Initialize new RX connection */
      LinTp_CtrlPtr->LinTp_Timer                       = LinTp_GetNcrOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_TimerReload                 = LinTp_GetNcrOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_RemainingLength             = (PduLengthType)( ((uint16)(LinTp_LinSduPtr[1] & 0x0F) << 8) | (LinTp_LinSduPtr[2]) ); /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length         = 5; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx            = 3; /* index of the buffer copied from - FF:3 */ /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_SN                          = 1; /* SBSW_LINIF_TP_CTRL_PTR */

      LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id             = LinTp_GetUpperLayerPduIdOfRxNSdu(LinTp_CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR */

      /* #50 Reset P2 timer */
      /* stop P2 timer after receiving start of response */
      LinTp_CtrlPtr->LinTp_P2Timer                     = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      /* reset RP counter after receiving start of response to trigger schedule change request */
      LinTp_CtrlPtr->LinTp_RespPendingCounter          = 0; /* SBSW_LINIF_TP_CTRL_PTR */

      LinTp_CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START; /* SBSW_LINIF_TP_CTRL_PTR */

      /* #60 Start receive buffer handling (-> LinTp_RxDataBufferHandling() ) */
      LinTp_RxDataBufferHandling(LinTp_CtrlPtr, LinTp_LinSduPtr); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
    }
    /* else: ignore FF with invalid length - conform to LIN standard but not fully to LINIF652, see bugzilla 52237 */
  }
}

/***********************************************************************************************************************
 *  LinTp_RxIndication_CF()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxIndication_CF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   LinTp_CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                     LinTp_LinSduPtr
)
{
  /* #10 Check if consecutive frame is expected */
  if (LinTp_CtrlPtr->LinTp_RemainingLength > 0)
  { /* CF awaited, note that NAD is already checked */

    /* #20 Check if sequence number is correct */
    if ( LinTp_CtrlPtr->LinTp_SN == (uint8)(LinTp_LinSduPtr[1] & 0x0F) )
    {
      /* #30 Update sequence number and Tp timeout counter */
      LinTp_CtrlPtr->LinTp_SN                       = (uint8)((LinTp_CtrlPtr->LinTp_SN + 1) & 0x0F); /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_Timer                    = LinTp_CtrlPtr->LinTp_TimerReload; /* SBSW_LINIF_TP_CTRL_PTR */

      /* #40 Continue with receive buffer handling (-> LinTp_RxDataBufferHandling() ) */
      /* check for end condition */
      if (LinTp_CtrlPtr->LinTp_RemainingLength < 6)
      {
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length    = (uint8)(LinTp_CtrlPtr->LinTp_RemainingLength); /* SBSW_LINIF_TP_CTRL_PTR */
      }
      else
      {
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Length    = 6; /* SBSW_LINIF_TP_CTRL_PTR */
      }

      LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx         = 2; /* index of the buffer copied from - CF:2 */ /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_RxDataBufferHandling(LinTp_CtrlPtr, LinTp_LinSduPtr); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
    }
    /* #50 Otherwise: Invalid sequence number in CF */
    else
    {
      /* #60 Abort connection and inform PduR */
      LinTp_RxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
    }
  }
  /* else: ignore unexpected consecutive frames */
}

/***********************************************************************************************************************
 *  LinTp_RxIndication()
 **********************************************************************************************************************/
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
LINIF_LOCAL FUNC(void, LINIF_CODE) LinTp_RxIndication
(
  VAR(NetworkHandleType, AUTOMATIC)                         LinIf_LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                 LinTp_LinSduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)    tpCtrlTmp;
  LinTp_ChannelType                                         linTpChannelTmp;

  /* ----- Implementation ----------------------------------------------- */
  tpCtrlTmp = LinTp_GetAddrCtrl(LinIf_LinIfChannel);
  linTpChannelTmp = LinTp_GetChannelOfLinIfToLinTpChannel(LinIf_LinIfChannel);

  /* #10 Preprocess NAD: inspect if Strict-NAD check is disabled and received frame is valid SF or FF */
  if ( ( LinTp_IsLinTp_StrictNADCheckOfChannelConfig(linTpChannelTmp) == FALSE ) &&
       ( ( (LinTp_LinSduPtr[1] & 0xF0u) == LinTp_FrameType_SF) || ( (LinTp_LinSduPtr[1] & 0xF0u) == LinTp_FrameType_FF) ) ) /* COV_LINTP_STRICTNADCHECK_CONDITION */
  { /* NAD check disabled; an unexpected NAD is only expected for new reception (SF or FF); during a multi frame reception a new NAD is discarded */

    /* To avoid further 'Is NAD check option enabled' case distinctions, fake further comparisons by setting requested NAD to received one */
    tpCtrlTmp->LinTp_NAD_Requested = LinTp_LinSduPtr[0]; /* SBSW_LINIF_TP_CTRL_PTR */

    /* #20 If received NAD belongs to a configured RxNSdu */
    /* If actual received slave response does not match with Rx NSdu reference stored during transmission request of TxNSdu
       or no prior transmission request handled at all, search for the correct RxNSdu. */
    if ( LinTp_GetNADOfRxNSdu(tpCtrlTmp->LinTp_Curr_Rx_Id) != LinTp_LinSduPtr[0] ) /* COV_LINTP_STRICTNADCHECK_UNREACHEDCODE XX */
    {
      uint8_least sduIdx;
      for (sduIdx = 0; sduIdx < LinTp_GetNumberOfLinTpRxNSdu(); sduIdx++) /* COV_LINTP_STRICTNADCHECK_UNREACHEDCODE XX */
      {
        if ( (LinTp_GetNADOfRxNSdu(sduIdx) == LinTp_LinSduPtr[0]) &&
# if ( LINTP_INVALIDHNDOFRXNSDU == STD_ON )
             (LinTp_IsInvalidHndOfRxNSdu(sduIdx) == FALSE) &&
# endif
             (LinTp_GetCtrlIdxOfRxNSdu(sduIdx) == LinIf_LinIfChannel) ) /* COV_LINTP_STRICTNADCHECK_NSDU_CONDITION */
        {
          /* #30 If RxNSdu found, reception of frame is accepted */
          tpCtrlTmp->LinTp_Curr_Rx_Id = (PduIdType)sduIdx; /* SBSW_LINIF_TP_CTRL_PTR */
          break;
        }
      }
      /* #40 If no RxNSdu found, skip processing of frame */
      /* processing is skipped below */
    }
  }

  /* #50 If no transmission is active and received NAD is accepted (only if Strict-NAD check enabled) */
  /* a possible active Rx connection is handled below */
  if ( ((tpCtrlTmp->LinTp_Ch_State & LINTP_CHANNEL_TX_MSK ) == 0) && (tpCtrlTmp->LinTp_NAD_Requested == LinTp_LinSduPtr[0]) )
  {
    /* #60 Test frame type for valid PCI */
    switch (LinTp_LinSduPtr[1] & 0xF0u)
    {

    /***********************************************************************************************************
     *  LinTp_FrameType_SF
     ***********************************************************************************************************/
    case LinTp_FrameType_SF:
      /* #70 Single frame received (-> LinTp_RxIndication_SF() ) */
      LinTp_RxIndication_SF(tpCtrlTmp, LinTp_LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      break;

    /***********************************************************************************************************
     *  LinTp_FrameType_FF
     ***********************************************************************************************************/
    case LinTp_FrameType_FF:
      /* #80 First frame received (-> LinTp_RxIndication_FF() ) */
      LinTp_RxIndication_FF(tpCtrlTmp, LinTp_LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      break;

    /***********************************************************************************************************
     *  LinTp_FrameType_CF
     ***********************************************************************************************************/
    case LinTp_FrameType_CF:
      /* #90 Consecutive frame received (-> LinTp_RxIndication_CF() ) */
      LinTp_RxIndication_CF(tpCtrlTmp, LinTp_LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      break;

    default:
      /* #100 PDUs with unexpected PCI types shall be ignored (according to LIN standard) */
      break;
    }

  }
  /* #110 Otherwise if invalid NAD received and reception active, close Rx connection */
  else
  {
    /* stop of reception and notification to PduR is only performed if channel state is not idle, thus frames with
       unknown NADs are ignored if they do not interfere an ongoing reception. Furthermore, if no connection is
       established there is no upper layer PduId to report anyway */
    if ( ( tpCtrlTmp->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK ) != 0 )
    { /* Unexpected NAD received - stop receiving Tp message */
      LinTp_RxCloseConnection(tpCtrlTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
}

/***********************************************************************************************************************
 *  LinTp_RxCloseConnection()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
LINIF_LOCAL FUNC(void, LINIF_CODE) LinTp_RxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable TP and P2 Timer, reset NAD, set channel state to idle */
  LinTp_CtrlPtr->LinTp_Timer                  = 0; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_NAD_Requested          = 0; /* reset requested NAD - only one response accepted for each request */ /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
  /* clear P2 timer */
  LinTp_CtrlPtr->LinTp_P2Timer                = 0; /* SBSW_LINIF_TP_CTRL_PTR */
  /* #20 Inform PduR about connection abortion, request applicative schedule from BswM */
  LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_PduRNSduIdRx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
  if ( LinTp_CtrlPtr->LinTp_PortNotification == LinTp_PortNotificationIdle ) /* COV_LINTP_FUNCREQUEST_SRF TX */
  { /* No new functional request pending */
    LinTp_CtrlPtr->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
  }
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  LinTp_CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
}

/***********************************************************************************************************************
 *
 * END - LinTp Reception Handling
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinTp Transmission Handling - Local Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_TxMessageProcessing()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_TxMessageProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)  LinTp_CtrlPtr,
  VAR(Lin_StatusType, AUTOMATIC)                              LinTp_LinStatus
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                              retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If MRF is successfully transmitted */
  if ( LinTp_LinStatus == (Lin_StatusType)LIN_TX_OK )
  {
    /* #20 If MRF is a functional request, confirm it by notifying PduR and reset schedule request to previous schedule */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    if (LinTp_CtrlPtr->LinTp_FuncRequest == LINTP_FUNC_REQUEST_TRANSMITTED)
    { /* functional request transmitted - reset schedule request to previous schedule */
      LinTp_CtrlPtr->LinTp_FuncRequest            = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PortNotification       = LinTp_CtrlPtr->LinTp_ComModeRestore; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotifyResultTx     = E_OK; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNSduIdTx           = LinTp_CtrlPtr->LinTp_Curr_Func_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
    }
    /* #30 Otherwise a physical Tx connection is active */
    /* Note that an ongoing transmission cannot be cancelled, also not by another transmit request */
    else
# endif
    {
      /* #40 If complete Tp message is finished */
      if (LinTp_CtrlPtr->LinTp_RemainingLength == 0)
      {
        /* #50 Tp request transmitted successfully, close connection and inform PduR */
        LinTp_CtrlPtr->LinTp_Timer                  = 0; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNotifyResultTx     = E_OK; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_PduRNSduIdTx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */

        /* #60 If request is a broadcast request */
# if ( LINTP_BROADCAST_REQUEST_HANDLING == STD_ON )
        if ( LinTp_CtrlPtr->LinTp_NAD_Requested == 0x7Fu ) /* COV_LINTP_BROADCASTHANDLING XF */
        {
          /* #70 Broadcast request transmitted, switch directly back to applicative schedule */
          LinTp_CtrlPtr->LinTp_NAD_Requested        = 0; /* SBSW_LINIF_TP_CTRL_PTR */
          LinTp_CtrlPtr->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          LinTp_CtrlPtr->LinTp_ComModeRestore       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
#  endif
        }
        else
# endif
        {
          /* #80 Normal physical request transmitted, switch to response schedule and activate P2 timeout observation */
          LinTp_CtrlPtr->LinTp_PortNotification     = LINTP_DIAG_RESPONSE; /* SBSW_LINIF_TP_CTRL_PTR */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          LinTp_CtrlPtr->LinTp_ComModeRestore       = LINTP_DIAG_RESPONSE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
          /* set up P2 time */
          LinTp_CtrlPtr->LinTp_P2Timer              = LinTp_GetP2Time(); /* SBSW_LINIF_TP_CTRL_PTR */
        }
      }
      /* #90 Otherwise: more frames to be transmitted for current request */
      else
      {
        /* #100 Prepare buffer for next CF */
        LinTp_CtrlPtr->LinTp_SN                     = (uint8)((LinTp_CtrlPtr->LinTp_SN + 1) & 0x0F); /* SBSW_LINIF_TP_CTRL_PTR */
        /* set PCI */
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[1]        = (uint8)(LinTp_FrameType_CF | LinTp_CtrlPtr->LinTp_SN); /* SBSW_LINIF_TP_CTRL_PTR */
        LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer_Idx       = 2; /* SBSW_LINIF_TP_CTRL_PTR */
        /* prepare pdu structure for buffer request */
        LinTp_CtrlPtr->LinTp_PduInfoPtr.SduDataPtr  = &LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[2]; /* SBSW_LINIF_TP_CTRL_PTR */
        if ( LinTp_CtrlPtr->LinTp_RemainingLength <= 6)
        {
          LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength = LinTp_CtrlPtr->LinTp_RemainingLength; /* SBSW_LINIF_TP_CTRL_PTR */
        }
        else
        {
          LinTp_CtrlPtr->LinTp_PduInfoPtr.SduLength = 6; /* SBSW_LINIF_TP_CTRL_PTR */
        }

        /* #110 Request new transmit data (-> LinTp_TxDataRequest() ) */
        retVal = LinTp_TxDataRequest(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        if (E_NOT_OK == retVal)
        {
          /* #120 If request is denied, close connection and inform PduR - permanent failure */
          LinTp_TxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
        }
        else
        {
          /* #130 Request accepted, reload timer with CF timeout value */
          LinTp_CtrlPtr->LinTp_Timer                  = LinTp_CtrlPtr->LinTp_TimerReload; /* SBSW_LINIF_TP_CTRL_PTR */
        }
      }
    }
  }
  /* #150 Otherwise: MRF transmission failed */
  else
  {
    /* #160 If MRF is a functional request, inform PduR and request change to previous schedule */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    if (LinTp_CtrlPtr->LinTp_FuncRequest == LINTP_FUNC_REQUEST_TRANSMITTED)
    {
      LinTp_CtrlPtr->LinTp_FuncRequest            = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PortNotification       = LinTp_CtrlPtr->LinTp_ComModeRestore; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNSduIdTx           = LinTp_CtrlPtr->LinTp_Curr_Func_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_CTRL_PTR */
      LinTp_CtrlPtr->LinTp_PduRNotifyResultTx     = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
    }
    /* 170 Otherwise (physcial request): Close connection and request change to application schedule */
    else
# endif
    {
      LinTp_TxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  LinTp_TxScheduleProcessing()
 **********************************************************************************************************************/
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
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinTp_TxScheduleProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)      LinTp_CtrlPtr, /* PRQA S 3673 */ /* MD_LIN_IF_16.7 */
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  LinTp_IfChannelDataPtr, /* PRQA S 3673 */ /* MD_LIN_IF_16.7 */
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)            LinTp_LinPduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean                                                         sendHeaderFlag;
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  BufReq_ReturnType                                               bufferReqRetVal;
  PduLengthType                                                   remainingPduRBufferSize;
# endif

  /* ----- Implementation ----------------------------------------------- */
  LINIF_DUMMY_STATEMENT_CONST(LinTp_IfChannelDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  sendHeaderFlag = TRUE;  /* be default, MRF header shall be transmitted */

  /* #10 If a functional request is pending for transmission */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  if ( LinTp_CtrlPtr->LinTp_FuncRequest == LINTP_FUNC_REQUEST_PENDING )
  {
    /* #20 Initialize local buffer for functional request (always SF) */
#  if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    LinTp_IfChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
#  endif

    /* abuse the LinIf buffer for a functional MRF because this is handled in one cycle  */
    LinTp_LinPduPtr->SduPtr = &(LinTp_IfChannelDataPtr->LinIf_TxData[0]); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */

    /* prepare buffer */
    LinTp_LinPduPtr->SduPtr[0] = 0x7Eu; /* SBSW_LINIF_TP_LINPDU_SDUPTR */
    LinTp_LinPduPtr->SduPtr[1] = (uint8)LinTp_CtrlPtr->LinTp_FuncPduInfoPtr.SduLength; /* SBSW_LINIF_TP_LINPDU_SDUPTR */
    LinTp_CtrlPtr->LinTp_FuncPduInfoPtr.SduDataPtr = &(LinTp_LinPduPtr->SduPtr[2]); /* SBSW_LINIF_TP_CTRL_PTR */

    /* #30 Request PduR to copy transmission data into provided local Tp buffer */
    bufferReqRetVal = PduR_LinTpCopyTxData(LinTp_CtrlPtr->LinTp_Curr_Func_NSdu_Id, &(LinTp_CtrlPtr->LinTp_FuncPduInfoPtr), /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_TX */
                                                  NULL_PTR, &remainingPduRBufferSize);
    /* #40 If transmission data is successfully provided */
    if (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK)
    {
      /* #50 Stuff unused payload bytes with fill value if necessary */
      uint8 i;
      for (i = (uint8)(LinTp_LinPduPtr->SduPtr[1] + 2); i < LINIF_LINPDU_SIZE; i++)
      {
        LinTp_LinPduPtr->SduPtr[i] = 0xFF; /* SBSW_LINIF_TP_LINPDU_SDUPTR */
      }
      LinTp_CtrlPtr->LinTp_FuncRequest = LINTP_FUNC_REQUEST_TRANSMITTED; /* SBSW_LINIF_TP_CTRL_PTR */
    }
    /* #60 Otherwise: request for transmission data not successful */
    else
    {
      /* #70 Abort functional request if data cannot be provided at this point */
      sendHeaderFlag                                = FALSE;
      LinTp_CtrlPtr->LinTp_FuncRequest              = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
      /* reject attempt and free buffer */
      LinTp_CtrlPtr->LinTp_PduRNotification        |= LinTp_PduRNotificationTxTm; /* SBSW_LINIF_TP_CTRL_PTR */
      /* result is always E_NOT_OK for TxTm notification */
      LinTp_CtrlPtr->LinTp_PduRNSduIdTm             = LinTp_CtrlPtr->LinTp_Curr_Func_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
    }
  }
  /* #80 Otherwise if transmission data for physical request already available in local buffer, transmit MRF */
  else if ( LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_TX_VALID )
# else
  if ( LinTp_CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_TX_VALID )
# endif
  {
    /* #90 Reset node configuration command state because the following SRF must be handled by TP */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    LinTp_IfChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
# endif
    /* #100 Update provided Lin Pdu with transmission data */
    LinTp_LinPduPtr->SduPtr = (uint8*)(&(LinTp_CtrlPtr->LinTp_Tx_Rx_Buffer[0])); /* SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC */
  }
  /* #110 Otherwise: Skip transmission of MRF frame */
  else
  {
    sendHeaderFlag = FALSE;
  }

  return sendHeaderFlag;
}

/***********************************************************************************************************************
 *  LinTp_TxCloseConnection()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
LINIF_LOCAL FUNC(void, LINIF_CODE) LinTp_TxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        LinTp_CtrlPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable TP timer and set channel state to idle */
  LinTp_CtrlPtr->LinTp_Timer                  = 0; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
  /* #20 Inform PduR about connection abortion and request applicative schedule from BswM */
  LinTp_CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_PduRNotifyResultTx     = E_NOT_OK; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_PduRNSduIdTx           = LinTp_CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_CTRL_PTR */
  LinTp_CtrlPtr->LinTp_PortNotification       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  LinTp_CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
# endif
}

/***********************************************************************************************************************
 *
 * END - LinTp Transmission Handling
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_CloseActiveConnection()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_CloseActiveConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)      LinTp_CtrlPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( LinTp_CtrlPtr->LinTp_Ch_State != LINTP_CHANNEL_IDLE ) /* COV_LINTP_NODECONFIGURATION TX */
  {
    /* #10 If Tx connection is active, abort Tp connection and inform PduR */
    if ( (LinTp_CtrlPtr->LinTp_Ch_State & LINTP_CHANNEL_TX_MSK ) == LINTP_CHANNEL_TX_MSK )
    {
      LinTp_TxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
    }
    /* #20 If Rx connection is active, abort Tp connection and inform PduR */
    else
    { /* not idle and no Tx connection implies Rx connection */
      LinTp_RxCloseConnection(LinTp_CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER */
    }
  }
}

/**********************************************************************************************************************
 *  GLOBAL LINTP FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinTp Initialization and General Function Interface - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_Init()
 **********************************************************************************************************************/
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
FUNC(void, LINIF_CODE) LinTp_Init
(
  P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_INIT_DATA) ConfigPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                               linIfChannelTmp;
  uint8                                               errorId;

# if ( LINTP_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Start(RtmConf_RtmMeasurementPoint_LinTp_Init);  /* PRQA S 3109 */ /* MD_MSR_14.3 */
# endif

  LINIF_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if LinIf component is not yet initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check if LinTp component is already initialized */
  else if ( LinTp_Dev_InitDetect == LINTP_INIT )
  {
    errorId = LINIF_E_ALREADY_INITIALIZED;
  }
  else
# endif
  {
# if ( LINTP_USE_INIT_POINTER == STD_ON )
    LinTp_ConfigDataPtr     = ConfigPtr;

    /* #30 Check validity of parameter ConfigPtr (only post-build) */
    if (LinTp_ConfigDataPtr == (P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_INIT_DATA))NULL_PTR)
    {
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_PARAMETER_POINTER;
#  endif
#  if ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_NULLPTR );
#  endif
    }
#  if ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    /* #40 Check generator version (only post-build loadable) */
    else if ( LinTp_GetGeneratorCompatibilityVersion() != (uint16) LINTP_GENERATOR_COMPATIBILITY_VERSION )
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION );
    }
    /* #50 Check magic number of config structure (only post-build loadable) */
    else if ( LinTp_GetFinalMagicNumber() != LINTP_FINAL_MAGIC_NUMBER)
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER );
    }
#  endif
    else
# endif
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      /* #60 Check LinIf and LinTp channel data structures for consistent size */
      /* PRQA S 3201, 3325 2 */ /* MD_LIN_IF_14.1 */
      if ( LinIf_GetSizeOfChannelData() < LinTp_GetSizeOfCtrl() ) /* COV_LINIF_INV_STATE_ALWAYSFALSE */
      {
        /* defensive programming, will never occur in a valid configuration. This check ensures that state remains UNINIT in an invalid configuration.
           Check it once here in initialization to avoid check in main function that linIfChannelTmp has valid size */
        errorId = LINIF_E_CONFIG;
      }
      else
# endif
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #70 Set LinTp initialization flag to INIT */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        LinTp_Dev_InitDetect  = LINTP_INIT;
# endif

        /* #80 Initialize all channel dependent transport protocol variables for all channels */
        for (linIfChannelTmp = 0x00u; linIfChannelTmp < LinTp_GetSizeOfCtrl(); linIfChannelTmp++ )
        {
          LinTp_GetCtrl(linIfChannelTmp).LinTp_Ch_State            = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_ARRAY */
          /* clear NAD of request */
          LinTp_GetCtrl(linIfChannelTmp).LinTp_NAD_Requested       = 0; /* SBSW_LINIF_TP_CTRL_ARRAY */

          LinTp_GetCtrl(linIfChannelTmp).LinTp_P2Timer             = 0; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelTmp).LinTp_RespPendingCounter  = 0; /* SBSW_LINIF_TP_CTRL_ARRAY */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          LinTp_GetCtrl(linIfChannelTmp).LinTp_ComModeRestore      = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelTmp).LinTp_FuncRequest         = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_ARRAY */
# endif
          LinTp_GetCtrl(linIfChannelTmp).LinTp_PortNotification    = LinTp_PortNotificationIdle; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelTmp).LinTp_Timer               = 0; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelTmp).LinTp_SN                  = 0; /* SBSW_LINIF_TP_CTRL_ARRAY */

          LinTp_GetCtrl(linIfChannelTmp).LinTp_PduRNotification    = LinTp_PduRNotificationIdle; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelTmp).LinTp_Curr_Rx_Id          = 0; /* SBSW_LINIF_TP_CTRL_ARRAY */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_INIT, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

# if ( LINTP_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Stop(RtmConf_RtmMeasurementPoint_LinTp_Init);  /* PRQA S 3109 */ /* MD_MSR_14.3 */
# endif
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

# if ( LINTP_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinTp_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter versioninfo. Note: no uninit check is performed */
  if ( versioninfo == (P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo parameter with component information */
    versioninfo->vendorID           = (uint16)LINIF_VENDOR_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->moduleID           = (uint16)LINIF_MODULE_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_major_version   = (uint8)LINIF_SW_MAJOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_minor_version   = (uint8)LINIF_SW_MINOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_patch_version   = (uint8)LINIF_SW_PATCH_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_GETVERSIONINFO, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#  endif
}
# endif /* LINIF_VERSION_INFO_API == STD_ON */

/***********************************************************************************************************************
 *  LinTp_Shutdown()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinTp_Shutdown
(
  void
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)     tpCtrlTmp;
  uint8_least                                               linIfChannelTmp;
  uint8                                                     errorId;

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
    errorId = LINIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* clear all LIN TP resources */
    /* #20 Reset all transport protocol variables and connections for all LinIf channels */
    for (linIfChannelTmp = 0x00u; linIfChannelTmp < LinTp_GetSizeOfCtrl(); linIfChannelTmp++ )
    {
      /* according to LINIF357 no reporting done in this routine */
      tpCtrlTmp = LinTp_GetAddrCtrl(linIfChannelTmp);

      tpCtrlTmp->LinTp_Timer            = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      tpCtrlTmp->LinTp_Ch_State         = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
      tpCtrlTmp->LinTp_NAD_Requested    = 0; /* SBSW_LINIF_TP_CTRL_PTR */
      /* stop session timer */
      tpCtrlTmp->LinTp_P2Timer          = 0; /* SBSW_LINIF_TP_CTRL_PTR */

# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
      if( (tpCtrlTmp->LinTp_FuncRequest == LINTP_FUNC_REQUEST_PENDING)  ||
          (tpCtrlTmp->LinTp_FuncRequest == LINTP_FUNC_REQUEST_TRANSMITTED) )
      {
        tpCtrlTmp->LinTp_FuncRequest = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_PTR */
      }
      if (tpCtrlTmp->LinTp_ComModeRestore != LINTP_APPLICATIVE_SCHEDULE)
      { /* release schedule Request */
        tpCtrlTmp->LinTp_PortNotification  = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR */
      }
# endif
    }

    SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 Set LinTp initialization flag to UNINIT */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    LinTp_Dev_InitDetect = LINTP_UNINIT;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_SHUTDOWN, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif
}

/***********************************************************************************************************************
 *
 * END - LinTp Initialization and General Function Interface - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinTp Transmission Handling - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LinTp_Transmit()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, LINIF_CODE) LinTp_Transmit
(
  VAR(PduIdType, AUTOMATIC)                                 LinTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)          LinTpTxInfoPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)     tpCtrlTmp;
  Std_ReturnType                                            retVal;
  uint8                                                     errorId;

  LINIF_DUMMY_STATEMENT(LinTpTxSduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(LinTpTxInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  retVal          = (Std_ReturnType)E_NOT_OK;
  errorId         = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTpTxInfoPtr */
  else if ( LinTpTxInfoPtr == (P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  /* #30 Check validity of parameter LinTpTxSduId */
#  if ( LINTP_INVALIDHNDOFTXNSDU == STD_ON)
  else if ( (LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu()) || (LinTp_IsInvalidHndOfTxNSdu(LinTpTxSduId) == TRUE) )
#  else
  else if ( LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu() )
#  endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  /* at this point LinTpTxSduId is valid, so the LinIfChannel belonging to this Pdu is guaranteed to be valid and used by LinTp */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    tpCtrlTmp = LinTp_GetAddrCtrl(LinTp_GetCtrlIdxOfTxNSdu(LinTpTxSduId));

# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    /* #40 If requested Nsdu is a valid functional request */
    if ( (LinTp_GetNADOfTxNSdu(LinTpTxSduId) == 0x7Eu) && (LinTpTxInfoPtr->SduLength <= 6) )
    {
      /* #50 If length is valid (SF), set functional request flag and trigger diagnostic schedule table */
      /* postpone buffer request of functional request until MRF is actually triggered to be transmitted, just store length of request here */
      tpCtrlTmp->LinTp_FuncPduInfoPtr.SduLength = LinTpTxInfoPtr->SduLength; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      /* request appropriate schedule */
      tpCtrlTmp->LinTp_PortNotification         = LINTP_DIAG_REQUEST; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      /* store NSdu of functional request */
      tpCtrlTmp->LinTp_Curr_Func_NSdu_Id        = LinTp_GetUpperLayerPduIdOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      tpCtrlTmp->LinTp_FuncRequest              = LINTP_FUNC_REQUEST_PENDING; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      retVal                                    = (Std_ReturnType)E_OK;
      /* further handling of functional frame is done on task level */
    }
    /* #60 Otherwise: (physical request) */
    else
# endif
    {
      /* #70 Abort possibly ongoing Tp Rx connection */
      if ( ( tpCtrlTmp->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK ) == LINTP_CHANNEL_RX_MSK )
      { /* Rx connection active */
        PduR_LinTpRxIndication(tpCtrlTmp->LinTp_Curr_UL_NSdu_Id, E_NOT_OK); /* direct notification to PduR */
        tpCtrlTmp->LinTp_Timer                = 0; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        tpCtrlTmp->LinTp_P2Timer              = 0; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        tpCtrlTmp->LinTp_Ch_State             = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        /* request applicative schedule here, overwritten below if request is accepted */
        tpCtrlTmp->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
        tpCtrlTmp->LinTp_ComModeRestore       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
# endif
      }

      /* #80 An ongoing Tp Tx connection should not be interfered, the new request is rejected in this case */
      if ( tpCtrlTmp->LinTp_Ch_State == LINTP_CHANNEL_IDLE )
      {
        /* #90 Channel idle, prepare frame: setup connection, init local buffer */
        /* Save current N-SDU */
        tpCtrlTmp->LinTp_Curr_UL_NSdu_Id          = LinTp_GetUpperLayerPduIdOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        /* Save internal ID of corresponding RxNSdu */
        tpCtrlTmp->LinTp_Curr_Rx_Id               = LinTp_GetAssociatedRxNSduIdOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */

        /* save buffer to local TP buffer */
        tpCtrlTmp->LinTp_Tx_Rx_Buffer[0]          = LinTp_GetNADOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        tpCtrlTmp->LinTp_RemainingLength          = LinTpTxInfoPtr->SduLength; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */

        /* #100 Prepare frame: Set PCI information depending in frame type */
        if (LinTpTxInfoPtr->SduLength <= 6)
        {
          /* SF request */
          tpCtrlTmp->LinTp_Tx_Rx_Buffer[1]        = (uint8)(LinTpTxInfoPtr->SduLength & 0x07u); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          tpCtrlTmp->LinTp_Tx_Rx_Buffer_Idx       = 2; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* prepare pdu structure for buffer request */
          tpCtrlTmp->LinTp_PduInfoPtr.SduDataPtr  = &tpCtrlTmp->LinTp_Tx_Rx_Buffer[2]; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          tpCtrlTmp->LinTp_PduInfoPtr.SduLength   = LinTpTxInfoPtr->SduLength; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        }
        else
        {
          /* multi frame request */
          /* set FF token and upper nibble of data length */
          tpCtrlTmp->LinTp_Tx_Rx_Buffer[1]        = (uint8)(((LinTpTxInfoPtr->SduLength & 0x0F00) >> 8) | LinTp_FrameType_FF); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* set lower data length byte */
          tpCtrlTmp->LinTp_Tx_Rx_Buffer[2]        = (uint8)  (LinTpTxInfoPtr->SduLength & 0x00FF); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          tpCtrlTmp->LinTp_Tx_Rx_Buffer_Idx       = 3; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* init sequence number, this number will be incremented after FF transmission for correct CF start value 1 */
          tpCtrlTmp->LinTp_SN                     = 0; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* prepare pdu structure for buffer request */
          tpCtrlTmp->LinTp_PduInfoPtr.SduDataPtr  = &tpCtrlTmp->LinTp_Tx_Rx_Buffer[3]; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          tpCtrlTmp->LinTp_PduInfoPtr.SduLength   = 5; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        }

        /* #110 Request PduR to provide transmission data */
        retVal = LinTp_TxDataRequest(tpCtrlTmp); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        /* #120 If request is rejected, close Tp connection */
        if (E_NOT_OK == retVal)
        { /* unable to obtain data from PduR - permanent failure, inform PduR directly */
          PduR_LinTpTxConfirmation(tpCtrlTmp->LinTp_Curr_UL_NSdu_Id, E_NOT_OK);
          tpCtrlTmp->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        }
        /* #130 Otherwise: save NAD, activate Tp timer, trigger diagnostic request schedule */
        else
        {
          tpCtrlTmp->LinTp_PortNotification       = LINTP_DIAG_REQUEST; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
# if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          tpCtrlTmp->LinTp_ComModeRestore         = LINTP_DIAG_REQUEST; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
# endif
          /* load SF/FF confirmation timeout */
          tpCtrlTmp->LinTp_Timer                  = LinTp_GetNasOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* next timeout reload value refers to CFs */
          tpCtrlTmp->LinTp_TimerReload            = LinTp_GetNcsOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* save current NAD - only matching responses are accepted */
          tpCtrlTmp->LinTp_NAD_Requested          = LinTp_GetNADOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* clear response pending counter */
          tpCtrlTmp->LinTp_RespPendingCounter     = 0; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* clear P2 timer */
          tpCtrlTmp->LinTp_P2Timer                = 0; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
        }
      }
    }

    SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_TRANSMIT, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
# endif

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

# if ( LINTP_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_CancelTransmit()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC)               LinTpTxSduId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC)                   errorId;

  LINIF_DUMMY_STATEMENT(LinTpTxSduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTpTxSduId */
#   if ( LINTP_INVALIDHNDOFTXNSDU == STD_ON)
  else if ( (LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu()) || (LinTp_IsInvalidHndOfTxNSdu(LinTpTxSduId) == TRUE) )
#   else
  else if ( LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu() )
#   endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  else if ( LinTp_GetChannelOfLinIfToLinTpChannel(LinTp_GetCtrlIdxOfTxNSdu(LinTpTxSduId)) == LinTp_Channel_Invalid )
  { /* LinTp is not configured on requested LinIf channel */
    errorId = LINIF_E_PARAMETER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Function does nothing, just return E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_CANCELTRANSMIT, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#  endif

  /* cancellation request is always rejected */
  return E_NOT_OK;
}
# endif /* LINTP_CANCEL_TRANSMIT_SUPPORTED */

/***********************************************************************************************************************
 *
 * END - LinTp Transmission Handling - Service Functions
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *
 * LinTp Reception Handling - Service Functions
 *
 **********************************************************************************************************************/

# if ( LINTP_CANCEL_RECEIVE_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_CancelReceive()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelReceive
(
  VAR(PduIdType, AUTOMATIC) LinTpRxSduId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC)                   errorId;

  LINIF_DUMMY_STATEMENT(LinTpRxSduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTpRxSduId */
#   if ( LINTP_INVALIDHNDOFRXNSDU == STD_ON )
  else if ( (LinTpRxSduId >= LinTp_GetNumberOfLinTpRxNSdu()) || (LinTp_IsInvalidHndOfRxNSdu(LinTpRxSduId) == TRUE) )
#   else
  else if ( LinTpRxSduId >= LinTp_GetNumberOfLinTpRxNSdu() )
#   endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  else if ( LinTp_GetChannelOfLinIfToLinTpChannel(LinTp_GetCtrlIdxOfRxNSdu(LinTpRxSduId)) == LinTp_Channel_Invalid )
  { /* LinTp is not configured on requested LinIf channel */
    errorId = LINIF_E_PARAMETER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Function does nothing, just return E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_CANCELRECEIVE, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#  endif

  /* cancellation request is always rejected */
  return E_NOT_OK;
}
# endif /* LINTP_CANCEL_RECEIVE_SUPPORTED */

# if ( LINTP_CHANGE_PARAMETER_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_ChangeParameter()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_ChangeParameter
(
  VAR(PduIdType, AUTOMATIC)                 id, /* Identifier of the received N-SDU on which the reception parameter has to be changed.*/
  VAR(TPParameterType, AUTOMATIC)           parameter,
  VAR(uint16, AUTOMATIC)                    value
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                     errorId;

  LINIF_DUMMY_STATEMENT(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(parameter); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  LINIF_DUMMY_STATEMENT(value); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter id */
#   if ( LINTP_INVALIDHNDOFRXNSDU == STD_ON )
  else if ( (id >= LinTp_GetNumberOfLinTpRxNSdu()) || (LinTp_IsInvalidHndOfRxNSdu(id) == TRUE) )
#   else
  else if ( id >= LinTp_GetNumberOfLinTpRxNSdu() )
#   endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  else if ( LinTp_GetChannelOfLinIfToLinTpChannel(LinTp_GetCtrlIdxOfRxNSdu(id)) == LinTp_Channel_Invalid )
  { /* LinTp is not configured on requested LinIf channel */
    errorId = LINIF_E_PARAMETER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Function does nothing, just return E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_CHANGEPARAMETER, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#  endif

  return E_NOT_OK;
}
# endif /* LINTP_CHANGE_PARAMETER_SUPPORTED */

/***********************************************************************************************************************
 *
 * END - LinTp Reception Handling - Service Functions
 *
 **********************************************************************************************************************/

#endif /* LINIF_TP_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
   MD_LIN_IF_13.7_Trcv:
     Reason: Invalid values might occur due to corrupted variable content. This check is required by AUTOSAR.
     Risk: None.
     Prevention: None.

   MD_LIN_IF_14.1:
     Reason: Statement is reachable in case of corrupted variable content. See also justification of preceding condition.
     Risk: None.
     Prevention: None.

   MD_LIN_IF_14.6:
     Reason: Since efficiency (runtime as well as code size) is a primary implementation target this design is chosen to gain good performance.
     Risk: Resulting code is difficult to understand or may not work as expected.
     Prevention: Code inspection and test of the different variants in the component test.

   MD_LIN_IF_15.2:
      Reason: Explicit fallthrough at end of case-branch used for code size and runtime optimization.
      Risk: Code is difficult to understand.
      Prevention: Code inspection and test of the different variants in the component test.

   MD_LIN_IF_16.7:
      Reason: The usage of this pointer depends on the actual configuration and cannot be declared as 'pointer to const' in every case.
      Risk: Unintentional modification of the object addressed by the pointer.
      Prevention: Code inspection and test of the different variants in the component test.
*/

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR_PDUINFO
    \DESCRIPTION Call of external function with pointer parameter.
                 The passed pduinfo pointer points always to a local variable on the stack, therefore it's always valid.
                 The SduDataPtr points always to LinIf_TxData which is 8 bytes long. The length value is always in range [1,8], see countermeasure.
    \COUNTERMEASURE \S It must be verified that all frame length values are in range [1,8]. SMI-148.

\ID SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR
    \DESCRIPTION Call of external function with pointer parameter.
    \COUNTERMEASURE \N The pointer points always to a local variable on the stack, therefore it's always valid.

\ID SBSW_LINIF_VERSIONINFO_PTR
    \DESCRIPTION The function <Msn>_GetVersionInfo writes to the object referenced by parameter versioninfo.
    \COUNTERMEASURE \S The caller must ensure that the pointers passed to the parameters versioninfo references an object
                       of type Std_VersionInfoType. SMI-16.
                    \R A check of versioninfo against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINIF_SCHEDULEINFO_PTR
    \DESCRIPTION The function Linf_GetScheduleInfo writes to the object referenced by parameter ScheduleInfo.
    \COUNTERMEASURE \S The caller must ensure that the pointers passed to the parameters ScheduleInfo references an object
                       of type LinIf_ScheduleInfoType. SMI-16.
                    \R A check of ScheduleInfo against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINIF_LINPDU_SDUPTR
    \DESCRIPTION Array write access to LIN Pdu data buffer. The pointer SduPtr of structure LinIf_LinPduPtr is set to
                 LinIf Tx data array before array access.
    \COUNTERMEASURE \N The size of this array is always equal to the loop variable used for element access by using the
                       same define LINIF_LINPDU_SIZE. No out of bounds array access can occur.

\ID SBSW_LINIF_CHANNEL_DATA_ACCESS
    \DESCRIPTION Array write access of LinIf_ChannelData using ComStackLib. LinIf_LinIfChannelTmp is a loop variables
                 and ranges from 0 to LinIf_GetSizeOfChannelData() - 1.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_CHANNEL_DATA_PTR
    \DESCRIPTION Access of LinIf_ChannelData using ComStackLib.
                 The pointer is retrieved once at the beginning of one channel main function cycle in function
                 LinIf_MainFunction by accessing LinIf_ChannelData using the Get macro of ComStackLib and
                 a loop variable ranging from 0 to LinIf_GetSizeOfChannelData() - 1.
                 The pointer just is passed-through to all local functions in one main function cycle without modification.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_CHANNEL_DATA_PTR_TMP
    \DESCRIPTION Pointer write access to channel dependent data in externally called service function.
                 The pointer is retrieved by accessing the array LinIf_ChannelData using the index LinIf_LinIfChannelTmp.
                 LinIf_LinIfChannelTmp is checked before using a runtime check to be less than LinIf_GetSizeOfChannelData
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER
    \DESCRIPTION Access of LinIf_ChannelData.LinIf_SchedTabData using ComStackLib.
                 The pointer is retrieved once at the beginning of function LinIf_ScheduleTableHandler by accessing
                 the array LinIf_ChannelData using the index LinIf_LinIfChannel.
                 The variable LinIf_LinIfChannel is the loop variable in LinIf_MainFunction and ranges from 0
                 to LinIf_GetSizeOfChannelData() - 1.
                 The pointer just is passed-through to all local functions from LinIf_ScheduleTableHandler onwards.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_FLUSHALLSCHED_CALL_1
    \DESCRIPTION Function call LinIf_FlushAllSchedules with pointer parameter.
                 The pointer LinIf_ChannelDataPtr itself is valid due to [SBSW_LINIF_CHANNEL_DATA_PTR]. This implies that
                 LinIf_ChannelDataPtr->LinIf_SchedTabData is also valid because LinIf_ChannelData is statically allocated.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_FLUSHALLSCHED_CALL_2
    \DESCRIPTION Function call LinIf_FlushAllSchedules with pointer parameter.
                 The provided pointer LinIf_SchedTabDataPtrTmp is valid due to [SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER].
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_FLUSHALLSCHED_SCHED_DATA_PTR
    \DESCRIPTION Pointer write access to provided argument LinIf_SchedTabDataPtr.
    \COUNTERMEASURE \N The caller of local function LinIf_FlushAllSchedules ensures validity of provided pointer.

\ID SBSW_LINIF_FCT_CALL_WITH_SCHEDTABPTR
    \DESCRIPTION Function call with pointer parameter to schedule table data.
    \COUNTERMEASURE \N The provided pointer LinIf_SchedTabDataPtrTmp is valid due to [SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER].

\ID SBSW_LINIF_WAKEUPCONF_CHANNEL_DATA
    \DESCRIPTION Array write access to LinIf_ChannelData using index variable LinIf_LinIfChannelTmp using ComStackLib access.
                 The variable LinIf_LinIfChannelTmp is checked before to be less than LinIf_GetSizeOfChannelData().
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_SPOR_TXFLAGS_ACCESS_CSL
    \DESCRIPTION Array write access of LinIf_SporTransmitFlags using ComStackLib with index variable LinIf_IndexTmp.
                 LinIf_IndexTmp is a loop variables and ranges from 0 to LinIf_GetSizeOfSporTransmitFlags() - 1.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_SPOR_TXFLAGS_ACCESS
    \DESCRIPTION Array write access of LinIf_SporTransmitFlags.
    \COUNTERMEASURE \R It is ensured by a runtime check before that the index LinIf_ByteMskTmp is less than LinIf_GetSizeOfSporTransmitFlags.

\ID SBSW_LINIF_FCT_POINTER_CALL
    \DESCRIPTION Function pointer call via indirection over function table.
                 The index LinIf_LinIfChannel/LinIf_LinIfChannelTmp is valid due to [SBSW_LINIF_CHANNEL_DATA_ACCESS].
                 The indirection to get actual index into table is not critical due to qualified use-case CSL03 of ComStackLib.
    \COUNTERMEASURE \S It must be verified that only valid function names with matching parameter lists are contained in the
                       function table. Especially no NULL_PTR or explicit address values are allowed. SMI-388.

\ID SBSW_LINIF_FCT_POINTER_CALL_PDUINFO
    \DESCRIPTION Function pointer call via indirection over function table.
                 The index LinIf_LinIfChannel/LinIf_LinIfChannelTmp is valid due to [SBSW_LINIF_CHANNEL_DATA_ACCESS].
                 The indirection to get actual index into table is not critical due to qualified use-case CSL03 of ComStackLib.
                 The passed pduinfo pointer points always to a local variable on the stack, therefore it's always valid.
                 The SduDataPtr points always to LinIf_TxData which is 8 bytes long. The length value is always in range [1,8], see countermeasure.
    \COUNTERMEASURE \S It must be verified that only valid function names with matching parameter lists are contained in the
                       function table. Especially no NULL_PTR or explicit address values are allowed. SMI-388.
                       It must be verified that all frame length values are in range [1,8]. SMI-148.

\ID SBSW_LINIF_PASSED_PTR_ACCESS_IN_STATIC_FUNC
    \DESCRIPTION Write access of pointer passed to static function.
    \COUNTERMEASURE \N Local function only called by LinIf internally. Caller assures that passed pointer is always valid.

\ID SBSW_LINIF_FCT_CALL_PTR_HANDOVER
    \DESCRIPTION Call of function with pointer parameter.
                 The pointer parameter was provided by the caller of this function and the pointer is just passed
                 to the called function without accessing it.
    \COUNTERMEASURE \N The caller ensures validity of pointer parameter.

\ID SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL
    \DESCRIPTION Call of function with pointer parameter. There are three possible pointer origins:
                 a) The pointer was provided by the caller of this function and the pointer is just passed to the called function without accessing it.
                 b) The pointer points to a local stack variable.
                 c) The pointer is retrieved by an address macro of ComStackLib. The passed channel index comes from channel iteration
                    in LinIf_MainFunction where it is already checked against LinIf_GetSizeOfChannelData() and/or LinTp_GetSizeOfCtrl.
    \COUNTERMEASURE \N The pointer is valid in all three cases.

\ID SBSW_LINIF_TP_CTRL_ARRAY
    \DESCRIPTION Array write access to LinTp_Ctrl using ComStackLib with index variable LinTp_LinIfChannelTmp.
                 LinTp_LinIfChannelTmp is a loop variables and ranges from 0 to LinTp_GetSizeOfCtrl() - 1.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_TP_TRANSMIT_CTRL_PTR
    \DESCRIPTION Pointer write access of to LinTp_Ctrl using ComStackLib. LinTp_Ctrl is accessed via an indirection using
                 index variable LinTpTxSduId. LinTpTxSduId is checked before to be less than LinTp_GetNumberOfLinTpTxNSdu.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID SBSW_LINIF_TP_CTRL_PTR
    \DESCRIPTION Pointer write access of LinTp_Ctrl using ComStackLib.
                 The pointer is retrieved once at the beginning of one channel main function cycle in function
                 LinIf_MainFunction by accessing LinTp_Ctrl using the Get macro of ComStackLib and
                 a loop variable ranging from 0 to LinTp_GetSizeOfCtrl() - 1.
                 The pointer just is passed-through to all local functions in one main function cycle without modification.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_TP_RXDATABUFHANDLING_CALL
    \DESCRIPTION Function call LinTp_RxDataBufferHandling with pointer parameters.
    \COUNTERMEASURE \N The first passed pointer is valid due to [SBSW_LINIF_TP_CTRL_ARRAY], [SBSW_LINIF_TP_CTRL_PTR]
                       or [SBSW_LINIF_TP_TRANSMIT_CTRL_PTR].
                       The second parameter was either provided by the caller of this function or is the NULL_PTR.
                       Note that LinTp_RxDataBufferHandling explicitly allows to pass a NULL_PTR as argument to
                       indicate special handling.

\ID SBSW_LINIF_TP_LINPDU_SDUPTR
    \DESCRIPTION Array write access to LIN Pdu data buffer using LinTp_LinPduPtr->SduPtr.
                 The pointer SduPtr of structure LinPdu is set to LinIf_TxData array directly before array access and
                 LinIf_TxData has size LINIF_LINPDU_SIZE.
    \COUNTERMEASURE \R A preprocessor check ensures that LINIF_LINPDU_SIZE is at least two.
                       The loop variable used for element access uses LINIF_LINPDU_SIZE as border so it never exceeds
                       the size of the array LinTp_LinPduPtr->SduPtr.

\ID SBSW_LINIF_TP_PDUR_FCT_CALL_START_RX
    \DESCRIPTION Call of function PduR_LinTpStartOfReception with pointer parameters.
    \COUNTERMEASURE \N The first passed pointer is fixed to NULL_PTR to indicate no meta data. Providing a NULL pointer is
                       explicitly allowed by this function.
                       The second passed pointer points to a local stack variable and is always valid.

\ID SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_RX
    \DESCRIPTION Call of function PduR_LinTpCopyRxData with pointer parameters.
    \COUNTERMEASURE \N The pointer parameter was either provided by the caller of this function and the pointer is just
                       passed to the called function. Or the pointer is a or points to a local stack variable and is
                       always valid.

\ID SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_TX
    \DESCRIPTION Call of function PduR_LinTpCopyTxData with pointer parameters.
    \COUNTERMEASURE \N The passed pointer for parameter retry is fixed to NULL_PTR to indicate no retry.
                       Providing a NULL pointer for retry is explicitly allowed by this function.
                       The other pointer parameters were either provided by the caller of this function and the pointers
                       are just passed to the called function. Or the pointers are or point to a local stack variables
                       and are always valid.

\ID SBSW_LINIF_TP_CTRL_BUFFER_ARRAY
    \DESCRIPTION Array access to local Tp buffer LinTp_Tx_Rx_Buffer with size LINTP_LOCAL_BUFFER_SIZE via index variable LinTp_Index.
    \COUNTERMEASURE \R The index cannot exceed the size of the array because the index is limited to range from 0
                       to LINTP_LOCAL_BUFFER_SIZE - 1. Length of LinTp_Tx_Rx_Buffer is LINTP_LOCAL_BUFFER_SIZE.
                       It is additionally verified with a preprocessor check that LINTP_LOCAL_BUFFER_SIZE has valid size.

\ID SBSW_LINIF_RX_BUFFER_ARRAY
    \DESCRIPTION Array access to local Rx buffer LinIf_RxData with size LINIF_LINPDU_SIZE via index variable indexTmp.
    \COUNTERMEASURE \R The index cannot exceed the size of the array because the index is limited to range from 0
                       to LINIF_LINPDU_SIZE - 1. Length of LinIf_RxData is LINIF_LINPDU_SIZE.
                       It is additionally verified with a preprocessor check that LINIF_LINPDU_SIZE has valid size.

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

\ID COV_LINIF_MISRA_ELSEBRANCH
  \ACCEPT TX
  \REASON [COV_MSR_MISRA]

\ID COV_LINIF_MISRA_DEFAULTBRANCH
  \REASON [COV_MSR_MISRA]

\ID COV_LINIF_INV_STATE_ALWAYSFALSE
  \ACCEPT XF
  \REASON [COV_MSR_INV_STATE]

\ID COV_LINIF_INV_STATE_ALWAYSTRUE_MAINFUNC
  \ACCEPT TF tf tx
  \REASON [COV_MSR_INV_STATE]

\ID COV_LINIF_COMPATIBILITY
  \REASON [COV_MSR_COMPATIBILITY]

\ID COV_LINIF_CONFIG
  \REASON [COV_MSR_CONFIG]

\ID COV_LINIF_SPOR_FRAME_HANDLE_CONFIG
  \ACCEPT TX
  \ACCEPT XF
  \REASON Depending on the configuration (e.g. frame names, order of frames), inside the FrameList data structure no unconditional
          Tx frames follow behind the sporadic unconditional frames. This means the frame handle of all other unconditional frames
          is smaller than that of any sporadic unconditional frames - in this case the loop is entered only once and left due to the
          break statement in the first loop cycle. In case a sproadic frame is actually transmitted, then it is found in this search
          and the while loop reaches never the limit, but is also quit by the break statement.
          In postbuild configurations, sporadic frames support is enabled but zero sporadic frames exist. In this case the condition
          is always false.

\ID COV_LINIF_HANDLE_GAPS_CONFIG
  \REASON In case there are no gaps inside the handle list, then no invalid rows in the data structure exist, therefore the check for
          handlegaps can never be evaluated true.

\ID COV_LINIF_FUNC_CALL_CYCLE_CONFIG
  \ACCEPT TX
  \REASON In a single channel configuration or in a multi channel configuration where each channel uses the same time base, the channel
          function call cycle is 1 (the channel main funcion is called in every LinIf main function cycle). Therefore this statement is
          always true.

\ID COV_LINIF_NODECONFIG_ELSEBRANCH
  \ACCEPT TX
  \ACCEPT XX
  \REASON For TX, see [MSR_COV_MISRA]
          In postbuild configurations, node configuration frames support is enabled but no node configuration frames could exist. In this case
          this state is never reached.

\ID COV_LINIF_FBL_USECASE
  \ACCEPT TX
  \REASON In special FBL configuration, only diagnostic frames is used but no other frame types. This means that LinIf does not handle any
          frames and header file PduR_LinIf.h does not exist in the configuration. Because this switch has not any other impact except this
          inclusion, it is not tested.

\ID COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL
  \ACCEPT XF
  \ACCEPT XX
  \ACCEPT TX
  \REASON The support for sporadic frames globally enabled, but there is no sporadic frame configured on the channel under test and it never
          explicitely triggered. In postbuild configurations, this location may be never covered if no sporadic frames exist at all (XX case).

\ID COV_LINIF_TRCV
  \REASON In a single channel configuration with transceiver handling enabled, there is no Lin channel without transceiver, so this code cannot be
          covered in such a configuration.

\ID COV_LINIF_SCHEDULECHANGE_POINT_1
  \ACCEPT TF tx tf tf
  \ACCEPT XF xf xx xx
  \REASON This boolean value is either always FALSE or always TRUE for one channel. This configuration parameter supports postbuild.

\ID COV_LINIF_SCHEDULECHANGE_POINT_2
  \ACCEPT XX xx xx xx
  \REASON Previous decision depends on boolean (postbuild) configuration parameter that could be either FALSE or TRUE. If it's FALSE then this decision
          is never reached.

\ID COV_LINTP_SHORT_SRF_SLOT
  \REASON The state can only be LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START at this point if a previous SRF slot is finished in the same cycle as a
          new SRF slot begins and the received SRF aborts the previous diagnostic response connection. The abortion notification to PduR is pending,
          so the buffer request for the new connection is postponed to the next main function cycle.
          This can only happen in configuration where the SRF slot is as long as the transmission duration of the frame in ticks, and this constraint
          is only provided in special configurations.

\ID COV_LINTP_SCHEDCHANGENOTIFY
  \ACCEPT TX
  \ACCEPT XF
  \REASON In a single channel configuration, this boolean value is either always FALSE or always TRUE. This configuration parameter supports postbuild.

\ID COV_LINTP_STRICTNADCHECK_CONDITION
  \ACCEPT XF xf xx xx
  \ACCEPT TF tx tf tf
  \REASON In a single channel configuration, this boolean value is either always FALSE or always TRUE. This configuration parameter supports postbuild.

\ID COV_LINTP_STRICTNADCHECK_UNREACHEDCODE
  \REASON If strict NAD check parameter (postbuild parameter) is TRUE, the NAD handling is performed before in LinTp_RxIndication and this code location
          is never reached in case of an unexpected NAD (NAD discarded).

\ID COV_LINTP_STRICTNADCHECK_NSDU_CONDITION
  \ACCEPT XX xx xx xx
  \ACCEPT TF tf tx tx
  \ACCEPT TF tf tx
  \ACCEPT TF tf tf tx
  \ACCEPT XF tf xf xx
  \REASON Case 1: If strict NAD check parameter (postbuild parameter) is TRUE, this location is never reached, see check above.
          Case 2,3,4: In a single channel configuration, last condition is always TRUE.

\ID COV_LINTP_BROADCASTHANDLING
  \REASON In postbuild configurations, the support for broadcast requests may be enabled but no broadcase request exists in configuration.

\ID COV_LINTP_NODECONFIGURATION
  \REASON The channel state can only be IDLE in this state if a node configuration request is scheduled and an active LinTp connection is therefore closed.
          In configurations without node configuration frames, this state can never happen.

\ID COV_LINTP_FUNCREQUEST_SRF
  \REASON This case that a port notification for a functional request is still pending at this location can only be stimulated in configurations which
          supports and contains a functional request and in which schedule change notification is enabled and where the SRF slot is exactly one main
          function cycle longer than the SRF frame duration (special timing case).

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: LinIf.c
 *********************************************************************************************************************/
