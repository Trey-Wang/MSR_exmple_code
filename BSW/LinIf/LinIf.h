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
 *         File:  LinIf.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Application-Interface of the AUTOSAR LIN Interface
 *                Declaration of functions, variables and constants
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
 *                                ESCAN00080336 LinTp: Reject physical physical request during ongoing transmission
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

#if !defined (LINIF_H)
# define LINIF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "LinIf_Cfg.h"

# if ( LINIF_TP_SUPPORTED == STD_ON )
#  include "LinTp_Cfg.h"
#  include "BswM_LinTp.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Component version information (decimal version of ALM implementation package) */
# define LINIF_SW_MAJOR_VERSION                     (5u)
# define LINIF_SW_MINOR_VERSION                     (2u)
# define LINIF_SW_PATCH_VERSION                     (0u)

/* Vendor and module identification */
# define LINIF_VENDOR_ID                            (30u)
# define LINIF_MODULE_ID                            (62u)

/* AUTOSAR Software specification version information */
# define LINIF_AR_RELEASE_MAJOR_VERSION             (4u)
# define LINIF_AR_RELEASE_MINOR_VERSION             (0u)
# define LINIF_AR_RELEASE_REVISION_VERSION          (3u)

/* Generator compatibility */
# define LINIF_GENERATOR_COMPATIBILITY_VERSION      ((uint16)0x0106U)

# define LINIF_INSTANCE_ID_DET                      (0x00u)

/* API service IDs */
# define LINIF_SID_INIT                             (0x01u)
# define LINIF_SID_GETVERSIONINFO                   (0x03u)
# define LINIF_SID_TRANSMIT                         (0x04u)
# define LINIF_SID_SCHEDULEREQUEST                  (0x05u)
# define LINIF_SID_GOTOSLEEP                        (0x06u)
# define LINIF_SID_WAKEUP                           (0x07u)
# define LINIF_SID_SETTRCVMODE                      (0x08u)
# define LINIF_SID_GETTRCVMODE                      (0x09u)
# define LINIF_SID_GETTRCVWAKEUPREASON              (0x0Au)
# define LINIF_SID_SETTRCVWAKEUPMODE                (0x0Bu)
# define LINIF_SID_CANCELTRANSMIT                   (0x0Cu)

# define LINTP_SID_INIT                             (0x40u)
# define LINTP_SID_TRANSMIT                         (0x41u)
# define LINTP_SID_GETVERSIONINFO                   (0x42u)
# define LINTP_SID_SHUTDOWN                         (0x43u)
# define LINTP_SID_CHANGEPARAMETER                  (0x44u)
# define LINTP_SID_CANCELTRANSMIT                   (0x46u)
# define LINTP_SID_CANCELRECEIVE                    (0x47u)

# define LINIF_SID_CHECKWAKEUP                      (0x60u)
# define LINIF_SID_WAKEUPCONFIRMATION               (0x61u)
# define LINIF_SID_MAINFUNCTION                     (0x80u)

/* Additional Service ID */
# define LINIF_SID_CHANNELINIT                      (0x02u)
# define LINIF_SID_GETSCHEDULEINFO                  (0x0Du)

/* List of frame types */
/*! LinIf frame type Unconditional
 * \trace CREQ-602
 */
# define LinIf_FrameType_UnconditionalFrame         ((uint8)0x00u)
# define LinIf_FrameType_MRF                        ((uint8)0x01u)
# define LinIf_FrameType_SRF                        ((uint8)0x02u)
/*! LinIf frame type Event-triggered
 * \trace CREQ-605
 */
# define LinIf_FrameType_EventTriggeredFrame        ((uint8)0x04u)
/*! LinIf frame type Sporadic
 * \trace CREQ-607
 */
# define LinIf_FrameType_SporadicFrame              ((uint8)0x05u)

/*! LinIf frame type Node configuration
 * \trace CREQ-600
 */
# define LinIf_FrameType_AssignFrameId              ((uint8)0x06u)
# define LinIf_FrameType_UnassignFrameId            ((uint8)0x07u)
# define LinIf_FrameType_AssignNAD                  ((uint8)0x08u)
# define LinIf_FrameType_FreeFormat                 ((uint8)0x09u)
# define LinIf_FrameType_ConditionalChangeNAD       ((uint8)0x0Au)
# define LinIf_FrameType_AssignFrameIdRange         ((uint8)0x0Bu)
# define LinIf_FrameType_SaveConfiguration          ((uint8)0x0Cu)

# define LinIf_FrameType_SleepModeFrame             ((uint8)0x0Du)
# define LinIf_FrameType_WakeUpFrame                ((uint8)0x0Eu)

/* Values for LinIf_ChannelStateType */
# define LINIF_CHANNEL_UNINIT                       ((LinIf_ChannelStateType)0x00u)
/*! LinIf channel OPERATIONAL state
 * \trace CREQ-595
 */
# define LINIF_CHANNEL_OPERATIONAL                  ((LinIf_ChannelStateType)0x01u)
/*! LinIf channel SLEEP state
 * \trace CREQ-596
 */
# define LINIF_CHANNEL_SLEEP                        ((LinIf_ChannelStateType)0x02u)

/* Development error codes */
# define LINIF_E_NO_ERROR                           (0xBAu)
# define LINIF_E_UNINIT                             (0x00u)
# define LINIF_E_ALREADY_INITIALIZED                (0x10u)
# define LINIF_E_NONEXISTENT_CHANNEL                (0x20u)
# define LINIF_E_PARAMETER                          (0x30u)
# define LINIF_E_PARAMETER_POINTER                  (0x40u)
# define LINIF_E_SCHEDULE_REQUEST_ERROR             (0x51u)
# define LINIF_E_TRCV_INV_CHANNEL                   (0x52u)
# define LINIF_E_TRCV_INV_MODE                      (0x53u)
# define LINIF_E_TRCV_NOT_NORMAL                    (0x54u)
# define LINIF_E_PARAM_WAKEUPSOURCE                 (0x55u)
# define LINIF_E_RESPONSE                           (0x60u)
# define LINIF_E_NC_NO_RESPONSE                     (0x61u)
/* Added development error codes */
# define LINIF_E_SCHEDULE_INCONSISTENT_ERROR        (0x70u)     /*!< Error code: Frame duration longer than slot */
# define LINIF_E_CONFIG                             (0x71u)     /*!< Error code: Invalid configuration */
# define LINIF_E_TRIGGERTRANSMIT_NO_DATA            (0x72u)     /*!< Error code: No transmission data available */

/* Schedule Table Run Modes */
/*! LinIf schedule table mode RUN-CONTINUOUS
 * \trace CREQ-598
 */
# define LinIf_RunMode_RunContinuous                ((uint8)0x00u)
/*! LinIf schedule table mode RUN-CONTINUOUS
 * \trace CREQ-599
 */
# define LinIf_RunMode_RunOnce                      ((uint8)0x01u)

/* LIN Transceiver Handling */
# define LinIf_InvalidLinTrcv                       ((uint8)0xFFu)

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 **********************************************************************************************************************/
# if ! defined ( LINIF_APPL_FRAME_STATUS_INFO )
#  define LINIF_APPL_FRAME_STATUS_INFO              STD_OFF
# endif

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

# define LINIF_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  LinIf_InitMemory()
 **********************************************************************************************************************/
/*! \brief       Power-up memory initialization.
 *  \details     Initializes component variables in *_INIT_* sections at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *  \trace       CREQ-567
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_InitMemory
(
  void
);

/***********************************************************************************************************************
 *  LinIf_Init()
 **********************************************************************************************************************/
/*! \brief       Initializes component LinIf.
 *  \details     Initializes all component variables and sets the component state to initialized.
 *  \param[in]   ConfigPtr               Pointer to LinIf configuration structure
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         LinIf_InitMemory has been called unless LinIf_Dev_InitDetect is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        -
 *  \trace       CREQ-567
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_Init
(
  P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_INIT_DATA)           ConfigPtr
);

# if ( LINIF_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinIf_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo               Pointer to location to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINIF_VERSION_INFO_API
 *  \note        Configuration parameter LinIfVersionInfoApi must be enabled.
 *  \trace       CREQ-568
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
);
# endif /* LINIF_VERSION_INFO_API == STD_ON */

/***********************************************************************************************************************
 *  LinIf_Transmit()
 **********************************************************************************************************************/
/*! \brief       Marks a sporadic frame as pending for transmission.
 *  \details     Indicates a request to transmit the frame specified by LinTxPduId.
 *               Only marks the sporadic frame as pending, transmission is postponed until the corresponding schedule
 *               slot is due.
 *  \param[in]   LinTxPduId              N-PDU ID of the LIN frame to be transmitted.
 *  \param[in]   PduInfoPtr              Pointer to LIN PDU related data containing DLC and pointer to LIN SDU buffer.
 *  \return      E_OK                    Transmit request was accepted.
 *  \return      E_NOT_OK                Transmit request was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \note        -
 *  \trace       CREQ-581
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Transmit
(
  VAR(PduIdType, AUTOMATIC) LinTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) PduInfoPtr
);

/***********************************************************************************************************************
 *  LinIf_ScheduleRequest()
 **********************************************************************************************************************/
/*! \brief       Requests a switch to a new schedule table.
 *  \details     Stores the request to enable a given schedule table after the current slot is due.
 *  \param[in]   Channel                 Network channel
 *  \param[in]   Schedule                Handle of new schedule to be set
 *  \return      E_OK                    Schedule table request was accepted.
 *  \return      E_NOT_OK                Schedule table switch request was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \trace       CREQ-575
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_ScheduleRequest
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(LinIf_SchHandleType, AUTOMATIC) Schedule
);

/***********************************************************************************************************************
 *  LinIf_GotoSleep()
 **********************************************************************************************************************/
/*! \brief       Requests the transition into sleep state.
 *  \details     Initiates a transition into sleep mode on the selected channel. After the current slot is
 *               finished, a sleep frame transmission is triggered. Afterwards the channel state is set to SLEEP.
 *  \param[in]   Channel                  Network channel
 *  \return      E_OK                     Request to go to sleep was accepted or channel is already in sleep state.
 *  \return      E_NOT_OK                 Request to go to sleep was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \trace       CREQ-570
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep
(
  VAR(NetworkHandleType, AUTOMATIC) Channel
);

/***********************************************************************************************************************
 *  LinIf_Wakeup()
 **********************************************************************************************************************/
/*! \brief       Requests the transition into operational state.
 *  \details     Initiates the wake up process on the selected controller and transmits a wakeup frame.
 *  \param[in]   Channel                 Network channel
 *  \return      E_OK                    Request to wake up was accepted.
 *  \return      E_NOT_OK                Request to wake up was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \trace       CREQ-571
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Wakeup
(
  VAR(NetworkHandleType, AUTOMATIC) Channel
);

# if ( LINIF_TRCV_HANDLING == STD_ON )
/***********************************************************************************************************************
 *  LinIf_SetTrcvMode()
 **********************************************************************************************************************/
/*! \brief       Sets the transceiver to requested mode.
 *  \details     Requests the transceiver on the selected channel to set the given mode.
 *  \param[in]   Channel                 Network channel
 *  \param[in]   TransceiverMode         Requested transceiver mode
 *  \return      E_OK                    Request of transceiver state change was accepted.
 *  \return      E_NOT_OK                Request of transceiver state chnage was not accepted, previous state not changed.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \trace       CREQ-576
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC) TransceiverMode
);

/***********************************************************************************************************************
 *  LinIf_GetTrcvMode()
 **********************************************************************************************************************/
/*! \brief       Gets the transceiver mode.
 *  \details     Gets the current transceiver mode on the selected channel.
 *  \param[in]   Channel                 Network channel
 *  \param[out]  TransceiverModePtr      Pointer to memory location for requested transceiver mode to be stored.
 *                                       Parameter must not be NULL.
 *  \return      E_OK                    Request was accepted and transceiver mode is stored in TransceiverModePtr.
 *  \return      E_NOT_OK                Request was not accepted, TransceiverModePtr is not valid.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \trace       CREQ-574
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR) TransceiverModePtr
);

/***********************************************************************************************************************
 *  LinIf_GetTrcvWakeupReason()
 **********************************************************************************************************************/
/*! \brief       Gets the transceiver wakeup reason.
 *  \details     Gets the reason for wakeup detected by transceiver on the selected channel.
 *  \param[in]   Channel                 Network channel
 *  \param[out]  TrcvWuReasonPtr         Pointer to memory location for transceiver wake up reason to be stored.
 *                                       Parameter must not be NULL.
 *  \return      E_OK                    Request was accepted and wakeup reason is stored in TrcvWuReasonPtr.
 *  \return      E_NOT_OK                Request was not accepted, TrcvWuReasonPtr is not valid.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \trace       CREQ-578
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR) TrcvWuReasonPtr
);

/***********************************************************************************************************************
 *  LinIf_SetTrcvWakeupMode()
 **********************************************************************************************************************/
/*! \brief       Sets the transceiver wakeup mode to requested mode.
 *  \details     Sets the wakeup mode of the transceiver on the selected channel.
 *  \param[in]   Channel                 Network channel
 *  \param[in]   TrcvWakeupMode          Requested transceiver wakeup mode
 *  \return      E_OK                    Request of transceiver wakeup mode change was accepted.
 *  \return      E_NOT_OK                Request of transceiver wakeup mode change was not accepted, transceiver wakeup
 *                                       mode is not changed.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \trace       CREQ-577
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvWakeupMode
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC) LinTrcvWakeupMode
);
# endif /* LINIF_TRCV_HANDLING == STD_ON */

# if ( LINIF_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinIf_CancelTransmit()
 **********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   LinTxPduId              N-PDU ID of the LIN frame for which a cancellation is requested.
 *  \return      E_NOT_OK                Cancellation request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_CANCEL_TRANSMIT_SUPPORTED
 *  \trace       CREQ-582
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC) LinTxPduId
);
# endif

# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  LinIf_CheckWakeup()
 **********************************************************************************************************************/
/*! \brief       Checks a possible wakeup event on a specific channel.
 *  \details     Checks the given wakeup source and calls either LIN transceiver or LIN driver on a specific channel
 *               to perform further wakeup validation.
 *  \param[in]   WakeupSource             Source which initiated the wakeup event: LIN controller or LIN transceiver.
 *  \return      E_OK                     Request was accepted, source of wakeup belongs to a LIN channel.
 *  \return      E_NOT_OK                 Request was not accepted, wakeup source invalid.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different wakeup sources
 *  \synchronous TRUE
 *  \config      LINIF_LIN_CHANNEL_WAKEUP_SUPPORT or LINIF_LINTRCV_WAKEUP_SUPPORT
 *  \trace       CREQ-573
 *  \note        Wakeup support in LinTrcv or Lin has to be enabled on at least one LIN channel.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC) WakeupSource
);
# endif

/***********************************************************************************************************************
 *  LinIf_MainFunction()
 **********************************************************************************************************************/
/*! \brief       Performs the main processing of the LIN interface.
 *  \details     Processes the schedule and frame handling and performs the appropriate state transitions.
 *               It is called periodically.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-569, CREQ-594
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_MainFunction
(
  void
);

# if defined ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING )
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
 *  \config      LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING
 *  \trace       CREQ-569, CREQ-590
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessing
(
  void
);
# endif

# if ( LINIF_SCHEDULE_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinIf_GetScheduleInfo()
 **********************************************************************************************************************/
/*! \brief       Returns the schedule table handler information.
 *  \details     Returns the current state of the LinIf schedule table handler. The returned information contains the
 *               current table identifier, current slot index, requested (pending) table identifier, current slot length
 *               and position inside slot.
 *  \param[in]   Channel                  Network channel
 *  \param[out]  versioninfo              Pointer to location to store the schedule information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINIF_SCHEDULE_INFO_API
 *  \note        Configuration parameter LinIfScheduleInfoApi must be enabled.
 *  \trace       CREQ-120270
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_GetScheduleInfo
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  P2VAR(LinIf_ScheduleInfoType, AUTOMATIC, LINIF_APPL_VAR) ScheduleInfo
);
# endif /* LINIF_SCHEDULE_INFO_API == STD_ON */

# if ( LINIF_APPL_FRAME_STATUS_INFO == STD_ON )
/***********************************************************************************************************************
 *  Appl_LinIfGetLinStatus()
 **********************************************************************************************************************/
/*! \brief       Informs the application about the driver status for the current frame.
 *  \details     This callback function is called after each frame to inform the upper layer directly about the status
 *               returned by LIN driver of the last processed frame.
 *               This callout is not called for sleep and wakeup frames.
 *  \param[in]   Channel                  Network channel
 *  \param[in]   Pid                      Protected identifier of frame
 *  \param[in]   Status                   LIN Status value retrieved from LIN driver
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_APPL_FRAME_STATUS_INFO
 *  \note        This callback function has to be enabled with a user config file where LINIF_APPL_FRAME_STATUS_INFO is
 *               defined to STD_ON and has to be provided by application.
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) Appl_LinIfGetLinStatus
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(Lin_FramePidType, AUTOMATIC) Pid,
  VAR(Lin_StatusType, AUTOMATIC) Status
);
# endif

# if ( LINIF_TP_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_Init()
 **********************************************************************************************************************/
/*! \brief       Initializes component LinTp.
 *  \details     Initializes all component variables and sets the component state to initialized.
 *  \param[in]   ConfigPtr               Pointer to LinTp configuration structure
 *  \pre         Module is uninitialized.
 *  \pre         Interrupts are disabled.
 *  \pre         LinIf_InitMemory has been called unless LinTp_Dev_InitDetect is initialized by start-up code.
 *  \pre         LinIf must be initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *  \trace       CREQ-587
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Init
(
  P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_INIT_DATA) ConfigPtr
);

/***********************************************************************************************************************
 *  LinTp_Transmit()
 **********************************************************************************************************************/
/*! \brief       Requests the transmission of diagnostic data using transport protocol.
 *  \details     Request the transfer of segmented data over the LIN bus using master request frames.
 *  \param[in]   LinTpTxSduId            Unique N-SDU identifier of LIN N-SDU to be transmitted.
 *  \param[in]   LinTpTxInfoPtr          Pointer to LIN N-SDU related data containing DLC and pointer to LIN n-SDU buffer.
 *  \return      E_OK                    Request was accepted and can be started successfully.
 *  \return      E_NOT_OK                Request was not accepted and cannot be started.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \config      LINIF_TP_SUPPORTED
 *  \trace       CREQ-584
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_Transmit
(
  VAR(PduIdType, AUTOMATIC)                         LinTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)  LinTpTxInfoPtr
);

#  if ( LINTP_VERSION_INFO_API == STD_ON )
/***********************************************************************************************************************
 *  LinTp_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo               Pointer to location to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-583
 *  \config      LINIF_TP_SUPPORTED and LINTP_VERSION_INFO_API
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
);
#  endif /* LINIF_VERSION_INFO_API == STD_ON */

/***********************************************************************************************************************
 *  LinTp_Shutdown()
 **********************************************************************************************************************/
/*! \brief       Shutdowns the component.
 *  \details     Closes all pending transport protocol connection, frees all resources and sets the module into
 *               uninitialized state.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *  \trace       CREQ-585
 **********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Shutdown
(
  void
);

#  if ( LINTP_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_CancelTransmit()
 **********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   LinTpTxSduId            Unique N-SDU identifier of LIN Tx N-SDU which transfer shall be cancelled.
 *  \return      E_NOT_OK                Cancellation request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED and LINTP_CANCEL_TRANSMIT_SUPPORTED
 *  \trace       CREQ-586
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC) LinTpTxSduId
);
#  endif

#  if ( LINTP_CHANGE_PARAMETER_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_ChangeParameter()
 **********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   id                      ID of the connection for whose channel the change shall be done.
 *  \param[in]   parameter               Selects the parameter that the request shall change (STmin).
 *  \param[in]   value                   New value of the parameter.
 *  \return      E_NOT_OK                Request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED and LINTP_CHANGE_PARAMETER_SUPPORTED
 *  \trace       CREQ-586
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_ChangeParameter
(
  VAR(PduIdType, AUTOMATIC)                 id,
  VAR(TPParameterType, AUTOMATIC)           parameter,
  VAR(uint16, AUTOMATIC)                    value
);
#  endif

#  if ( LINTP_CANCEL_RECEIVE_SUPPORTED == STD_ON )
/***********************************************************************************************************************
 *  LinTp_CancelReceive()
 **********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   LinTpRxSduId            Unique N-SDU identifier of LIN Rx N-SDU which transfer has to be cancelled.
 *  \return      E_NOT_OK                Cancellation request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED and LINTP_CANCEL_RECEIVE_SUPPORTED
 *  \trace       CREQ-586
 **********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelReceive
(
  VAR(PduIdType, AUTOMATIC) LinTpRxSduId
);
#  endif /* LINTP_CANCEL_RECEIVE_SUPPORTED */

# endif /* LINIF_TP_SUPPORTED == STD_ON */

# define LINIF_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \exclusivearea LINIF_EXCLUSIVE_AREA_0
 * Ensures consistency while modifying global variables and states in LinIf main function. Ensures consistent reception data from Lin driver.
 * \protects LinIf_ChannelData and LinTp_Ctrl
 * \usedin LinIf_MainFunction
 * \exclude All functions provided by LinIf except LinIf_MainFunction.
 * \length LONG One channel main function cycle.
 * \endexclusivearea
 */

/*! \exclusivearea LINIF_EXCLUSIVE_AREA_1
 * Ensures consistency in LinIf service API functions if main function can interrupt it's own service functions.
 * \protects LinIf_ChannelData and LinTp_Ctrl
 * \usedin LinIf_ScheduleRequest, LinIf_GotoSleep, LinIf_Wakeup, LinIf_CheckWakeup, LinIf_WakeupConfirmation,
 *         LinTp_Shutdown, LinTp_Transmit
 * \exclude LinIf_MainFunction.
 * \length LONG Services of other modules like Lin driver are called.
 * \endexclusivearea
 */

#endif /* LINIF_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf.h
 *********************************************************************************************************************/

