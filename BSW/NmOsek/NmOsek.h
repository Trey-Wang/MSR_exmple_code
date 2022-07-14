/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2011-2017 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  NmOsek.h
 *        \brief  NmOsek header file
 *
 *      \details  Vector Direct OSEK Network Management header file
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Markus Drescher               vismdr        Vector Informatik GmbH
 *  Leticia Garcia                visgle        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2011-02-04  vismdr  ESCAN00048862 Creation
 *  1.00.01   2011-03-25  vismdr  ESCAN00049575 Correct CQ Version define
 *  1.00.02   2011-03-28  vismdr  ESCAN00049564 Simplified Nm_StateChangeNotification() handling
 *                                ESCAN00049593 Corrected wrong fix of ESCAN00049575
 *                                ESCAN00049597 Adapted API ID macro definitions
 *                                ESCAN00049647 Adapted API ID macro definitions
 *                                ESCAN00049717 Renamed NmOsekHook_OnStateChange_MainFunction to
 *                                              NmOsekHook_OnStateChange_NmAction
 *                                ESCAN00049741 Adapted function parameter of NmOsek_CanSM_BusOffBegin()
 *                                ESCAN00049825 Changed logic for setting the Wake-up/Init Byte
 *                                ESCAN00049829 NMOSEK_NUMBER_OF_CONFIG_NODES >= 256u: removed source node id check
 *                                ESCAN00049836 Using NmOsek_CanSM_BusOffBegin / NmOsek_CanSM_BusOffEnd if previously
 *                                              defined
 *  1.00.03   2011-04-13  vismdr  ESCAN00049873 Optimizations for passive mode
 *                                ESCAN00049910 Reworked macro handling for NmOsek_TxMessageData
 *                                ESCAN00049988 Node Specification Number bytes are now correctly set
 *                                ESCAN00050123 Added condition for Nm_PrepareBusSleep call on TX Confirmation
 *                                ESCAN00050218 Removed Rx/Tx user data lock handling
 *  1.00.04   2011-05-19  vismdr  ESCAN00051017 Added initialization for NmOsek_SleepAckMsgIndicationFlag
 *                                ESCAN00051046 Added initialization for NmOsek_RxNmMsgSourceAddr,
 *                                              NmOsek_MsgConfirmationFlag and NmOsek_LogicalSuccessor
 *  1.01.00   2011-09-20  vismdr  ESCAN00052317 Added wake-up frame prohibition timer
 *                                ESCAN00052322 Wait for wake-up frame TX confirmation after send request from
 *                                              WaitBusSleep has been issued
 *                                ESCAN00052323 NMOSEK_ACTION_BUS_SLEEP_LIMPHOME_EXTENSION: no longer return to
 *                                              LimpHome. A Wake-up frame succeeded by an Alive frame is sent instead.
 *                                ESCAN00052383 Exclusive area in NmOsek_RxIndication() is also left if an invalid
 *                                              NM message has been received
 *                                ESCAN00052619 Renamed NMOSEK_ACTION_GO_BUSSLEEP_LIMPHOME_EXTENSION to
 *                                              NMOSEK_ACTION_BUS_SLEEP_LIMPHOME_EXTENSION
 *                                ESCAN00052620 Changed Wake-Up/Init Byte logic
 *                                ESCAN00052621 Inverted logic for TxRingDataAllowed flag
 *                                ESCAN00052622 LimpHome message request is issued after BusOff Recovery and after
 *                                              TError has elapsed
 *                                ESCAN00052665 Added support for Node Identifiers with offsets
 *                                ESCAN00052925 Added Alive message delay after ECU Power On
 *                                ESCAN00053703 Added support for Skipped Alive Message requests
 *                                ESCAN00054321 Adapted Node Specification Number in Skipped Alive Messages
 *                                ESCAN00054511 Adapted initialization of the NmOsek_CanIsBusOff flag
 *  1.01.01   2011-11-16  vismdr  ESCAN00049894 Moved some transitions to NmOsek_RxIndication / NmOsek_TxConfirmation
 *  1.01.02   2011-12-20  vismdr  ESCAN00055698 Notification from Prepare Bus Sleep to Repeat Message on Alive Message
 *  1.01.03   2012-03-05  vismdr  ESCAN00055975 Corrected nmPreviousState parameter of Nm_StateChangeNotification calls
 *                                ESCAN00057421 NMOSEK_ACTION_LH_PREPARE_SLEEP can be entered during BusOff
 *  1.02.00   2012-04-23  vismdr  ESCAN00057650 NMOSEK_ACTION_LH_PREPARE_SLEEP: Made TMax Reset on RX Ring configurable
 *                                ESCAN00057876 Ignoring All But Alive Messages in NMReset has been made configurable
 *                                ESCAN00058055 NMOSEK_ACTION_WAIT_RINGMSG: corrected condition for Ring Msg Reception
 *                                ESCAN00058056 LimpHome: Made Transition to Action Start on RX Msg configurable
 *                                ESCAN00059016 RX Transition trigger to Wait Bus Sleep has been made configurable
 *                                ESCAN00060437 Fixed wrong state transition from Wait Bus Sleep
 *  1.02.01   2012-10-16  vismdr  ESCAN00062285 Removed NMOSEK_ACTION_BUS_SLEEP_LIMPHOME_EXTENSION from NmOsek
 *                                ESCAN00062287 Keeping LimpHome bit in global status byte in NMBusSleep until wake-up
 *                                ESCAN00062509 Added NmOsek_GetLimpHomeStatus, NmOsek_ClearLimpHomeStatus functions
 *  1.02.02   2013-03-26  vismdr  ESCAN00064472 Revised internal filter mechanisms
 *                                ESCAN00066879 Added typecasts to several variable assignments
 *  1.02.03   2013-09-02  vismdr  ESCAN00069704 Fixed Skipped Alive Message Request state machine
 *  1.02.04   2013-12-09  vismdr  ESCAN00072460 Clear Msg Recv Remind flag on NmOsek_TxConfirmation
 *  1.02.05   2014-03-24  vismdr  ESCAN00074498 Reset BusError on BusOff recovery in NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX
 *  1.03.00   2014-11-19  vismdr  ESCAN00049912 Adapted usage of NmOsek_State variable
 *                                ESCAN00078515 Added Multiple Config support
 *                                ESCAN00079969 Adapted sections for NmOsek_CanSM_BusOffBegin/NmOsek_CanSM_BusOffEnd
 *  2.00.00   2014-12-09  vismdr  ESCAN00049899 First assign E_NOT_OK to return value, then eventually change to E_OK
 *                                ESCAN00049907 Perform input parameter check only if Dev Error Detect is ON
 *                                ESCAN00049954 Assign new value for previous state only on State Change Notification
 *                                ESCAN00080496 Adapted to new template / ComStackLib
 *  3.00.00   2015-04-01  vismdr  ESCAN00081714 Added function calls of Nm_SynchronizationPoint()
 *                                ESCAN00081990 Added function prototype NmOsek_SendSleepAck()
 *                                ESCAN00082043 Adapted condition to report NMOSEK_E_PDUR_TRIGGERTX_ERROR
 *                                ESCAN00082863 Added function call of Nm_NmOsek_PduRxIndication()
 *                                ESCAN00083049 Adapted usage of BusOffCounter
 *                                ESCAN00083051 Adapted usage of BusOffCounter
 *                                ESCAN00083139 Added function call of Nm_NetworkMode() on BusOff occurrence in PBS
 *  4.00.00   2015-08-06  vismdr  ESCAN00081378 No changes in this file
 *                                ESCAN00081705 Introduced Desired Nm State to delay state transitions
 *                                ESCAN00084398 Adapted the calculation for the configByte on message reception
 *                                ESCAN00084494 Improved NmOsek_RxIndication_HandleRxSleepAck(), NmOsek_SendSleepAck(),
 *                                              NmOsek_MainFunction_UpdatePeriodicMessageEnableTimer(),
 *                                              NmOsek_MainFunction_ActionBusSleep() and further optimizations
 *                                ESCAN00084669 Introduced Msg Timeout Timer
 *                                ESCAN00085297 Reset WaitBusSleep, BusSleep flags on BusOff occurrence when entering
 *                                              LimpHome
 *                                ESCAN00085610 Reset RX and TX counter if RX Sleep.ind=Sleep.ack=1 is accepted
 *                                ESCAN00085706 Provide received Node Id to Nm_NmOsek_PduRxIndication()
 *                                ESCAN00085997 Adapted preprocessor encapsulation of the byteIndex declaration
 *                                ESCAN00086002 Using proper argument for the Nm_NmOsek_PduRxIndication() call
 *                                ESCAN00086024 Added typecast for argument of NmOsek_UpdateTxMessageData() call
 *                                ESCAN00086170 SEC_VAR_ZERO_INIT sections are renamed to SEC_CLEARED
 *                                ESCAN00086246 Call Nm_SynchronizationPoint() in Wait Sendtoken when SyncPointTimer is
 *                                              restarted but is still running
 *                                ESCAN00086316 Adapted check for Prepare Bus Sleep in
 *                                              NmOsek_RxIndication_HandleRxSleepAck()
 *                                ESCAN00086570 Adapted declaration of byteIndex in NmOsek_Init()
 *                                ESCAN00086573 Adapted dummy statement in NmOsek_GetSendSleepAck()
 *                                ESCAN00086582 Added parentheses in NmOsek_GetSendSleepAck()
 *                                ESCAN00086583 Adapted function calls of NmOsek_TransmitWakeUp()
 *                                ESCAN00086592 Encapsulated the usage of
 *                                              NmOsek_GetSyncPointNetworkReleaseIntervalOfChannelConfig()
 *                                ESCAN00086725 Reset BusOff counter on transition to Wait Bus Sleep
 *                                ESCAN00086941 Using NMOSEK_NM_* #defines for Nm settings
 *  5.00.00   2016-02-08  vismdr  ESCAN00087845 Adapted PduR_NmOsekTriggerTransmit usage
 *                                ESCAN00088512 Adapted preprocessor encapsulation for PduR_NmOsekTriggerTransmit()
 *                                ESCAN00088622 Adapted usage of RxNmMsgSourceAddr
 *  6.00.00   2016-04-07  vismdr  ESCAN00089292 Added MISRA deviation justifications for SchM_Enter_* / SchM_Exit_*
 *                                ESCAN00090065 Adapted preprocessor checks in NmOsek.c
 *                                ESCAN00090119 Check for valid wake-up/awake user data PDU ID
 *                                ESCAN00090252 Adapted determination of SyncPointTimer start value
 *  6.01.00   2016-05-19  vismdr  ESCAN00086742 Release Version for certain OEM
 *                                ESCAN00087038 Released First Message shall be Nm Message feature
 *                                ESCAN00089489 Adapted State Change Notification logic for First Message Shall be
 *                                              Nm Message feature and provide new API
 *                                              NmOsek_CancelWaitForTxConfOrRxInd()
 *                                ESCAN00091622 Call PduR_NmOsekTxConfirmation() outside the Exclusive Area 0
 *  6.01.01   2016-10-18  vismdr  ESCAN00091595 Encapsulated errorId assignments in NmOsek_Init()
 *                                ESCAN00091599 NmOsek_GetUserDataFromPduR() is no longer annotated with the inline
 *                                              keyword
 *                                ESCAN00091605 NmOsek_MainFunction_Aux_RecalculateAnyChannelInLimpHome() is no longer
 *                                              annotated with the inline keyword
 *  7.00.00   2017-01-09  visgle  FEATC-870     FEAT-2223 NmDirOSek: Add API to trigger inital wakeup behavior
 *                        vismdr  FEATC-46      FEAT-1916 Release of OsekNm
 *                                ESCAN00076015 Application message transmission is re-enabled too early
 *                                ESCAN00090276 BETA version - the BSW module is in BETA state
 *                                ESCAN00092544 Remove check for the value of NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8
 *                                ESCAN00092724 MISRA deviation: MISRA-C:2004 Rule 5.1
 *********************************************************************************************************************/

#if !defined(NMOSEK_H)
# define NMOSEK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "NmOsek_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define NMOSEK_VENDOR_ID                        (30u)
# define NMOSEK_MODULE_ID                        (255u)

/* AUTOSAR Software Specification Version Information is not available since this is a Complex Device Driver */

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define NMOSEK_SW_MAJOR_VERSION                 (7u)
# define NMOSEK_SW_MINOR_VERSION                 (0u)
# define NMOSEK_SW_PATCH_VERSION                 (0u)

/* Instance ID is configurable in NmOsek_Cfg.h */

/* ----- API service IDs ----- */
# define NMOSEK_SID_INIT                         (0x00u) /*!< Service ID: NmOsek_Init() */
# define NMOSEK_SID_PASSIVESTARTUP               (0x01u) /*!< Service ID: NmOsek_PassiveStartup() */
# define NMOSEK_SID_NETWORKREQUEST               (0x02u) /*!< Service ID: NmOsek_NetworkRequest() */
# define NMOSEK_SID_NETWORKRELEASE               (0x03u) /*!< Service ID: NmOsek_NetworkRelease() */
# define NMOSEK_SID_SETUSERDATA                  (0x04u) /*!< Service ID: NmOsek_SetUserData() */
# define NMOSEK_SID_GETUSERDATA                  (0x05u) /*!< Service ID: NmOsek_GetUserData() */
# define NMOSEK_SID_GETNODEIDENTIFIER            (0x06u) /*!< Service ID: NmOsek_GetNodeIdentifier() */
# define NMOSEK_SID_GETLOCALNODEIDENTIFIER       (0x07u) /*!< Service ID: NmOsek_GetLocalNodeIdentifier() */
# define NMOSEK_SID_REPEATMESSAGEREQUEST         (0x08u) /*!< Service ID: NmOsek_RepeatMessageRequest() */
# define NMOSEK_SID_GETPDUDATA                   (0x0Au) /*!< Service ID: NmOsek_GetPduData() */
# define NMOSEK_SID_GETSTATE                     (0x0Bu) /*!< Service ID: NmOsek_GetState() */
# define NMOSEK_SID_DISABLECOMMUNICATION         (0x0Cu) /*!< Service ID: NmOsek_DisableCommunication() */
# define NMOSEK_SID_ENABLECOMMUNICATION          (0x0Du) /*!< Service ID: NmOsek_EnableCommunication() */
# define NMOSEK_SID_MAINFUNCTION                 (0x13u) /*!< Service ID: NmOsek_MainFunction() */
# define NMOSEK_SID_TRANSMIT                     (0x15u) /*!< Service ID: NmOsek_Transmit() */
# define NMOSEK_SID_GETSTATUS                    (0x20u) /*!< Service ID: NmOsek_GetStatus() */
# define NMOSEK_SID_GETCONFIG                    (0x21u) /*!< Service ID: NmOsek_GetConfig() */
# define NMOSEK_SID_CMPCONFIG                    (0x22u) /*!< Service ID: NmOsek_CmpConfig() */
# define NMOSEK_SID_CANSM_BUSOFFBEGIN            (0x30u) /*!< Service ID: NmOsek_CanSM_BusOffBegin() */
# define NMOSEK_SID_CANSM_BUSOFFEND              (0x31u) /*!< Service ID: NmOsek_CanSM_BusOffEnd() */
# define NMOSEK_SID_REQUESTBUSSYNC               (0xC0u) /*!< Service ID: NmOsek_RequestBusSynchronization() */
# define NMOSEK_SID_CHECKREMOTESLEEPIND          (0xD0u) /*!< Service ID: NmOsek_CheckRemoteSleepIndication() */
# define NMOSEK_SID_CANCELWAITFORTXCONFORRXIND   (0xE0u) /*!< Service ID: NmOsek_CancelWaitForTxConfOrRxInd() */
# define NMOSEK_SID_GETVERSIONINFO               (0xF1u) /*!< Service ID: NmOsek_GetVersionInfo() */

/* ----- Error codes ----- */
# define NMOSEK_E_UNINIT                         (0x01u) /*!< Error code: API service used without module initialization */
# define NMOSEK_E_INVALID_CHANNEL                (0x02u) /*!< Error code: API service used with wrong channel handle */
# define NMOSEK_E_INIT_FAILED                    (0x03u) /*!< Error code: NmOsek initialization has failed */
# define NMOSEK_E_WRONG_WAIT_BUS_SLEEP_FLAG      (0x04u) /*!< Error code: WaitBusSleep Flag is not correctly set */
# define NMOSEK_E_WRONG_BUS_SLEEP_FLAG           (0x05u) /*!< Error code: BusSleep Flag is not correctly set */
# define NMOSEK_E_PARAM_CONFIG                   (0x0Au) /*!< Error code: API service NmOsek_Init() called with wrong parameter */
# define NMOSEK_E_PARAM_POINTER                  (0x0Cu) /*!< Error code: API service used with invalid pointer parameter (NULL) */
# define NMOSEK_E_ALREADY_INITIALIZED            (0x11u) /*!< Error code: The service NmOsek_Init() is called while the module is already initialized */
# define NMOSEK_E_PDUR_TRIGGERTX_ERROR           (0x21u) /*!< Error code: Call of PduR_NmOsekTriggerTransmit() failed */
# define NMOSEK_E_NO_ERROR                       (0xFFu) /*!< used to check if no error occurred */

# if (NMOSEK_API_OPTIMIZATION == STD_ON)
/* PRQA S 3460 1 */ /* MD_MSR_19.4 */
#  define NMOSEK_CHANNEL_APPLTYPE_ONLY            void
#  define NMOSEK_CHANNEL_APPLTYPE_FIRST
# else
#  define NMOSEK_CHANNEL_APPLTYPE_ONLY            CONST(NetworkHandleType, AUTOMATIC) nmNetworkHandle
#  define NMOSEK_CHANNEL_APPLTYPE_FIRST           CONST(NetworkHandleType, AUTOMATIC) nmNetworkHandle,
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON) || (NMOSEK_USE_INIT_POINTER == STD_ON)
#else
# define NmOsek_InitMemory()
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define NMOSEK_START_SEC_CONST_8
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Requirement Specification Template version */
extern CONST(uint8, NMOSEK_CONST) NmOsek_MainVersion;
extern CONST(uint8, NMOSEK_CONST) NmOsek_SubVersion;
extern CONST(uint8, NMOSEK_CONST) NmOsek_ReleaseVersion;

# define NMOSEK_STOP_SEC_CONST_8
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NMOSEK_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * NmOsek_Init()
 *********************************************************************************************************************/
/*! \brief        Initialization function
 *  \details      This function initializes the module NmOsek. It initializes all variables and sets the module state to
 *                initialized.
 *  \param[in]    nmOsekConfigPtr         Configuration structure for initializing the module
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          Interrupts are disabled.
 *  \pre          Module is uninitialized.
 *  \pre          NmOsek_InitMemory has been called unless NmOsek_ModuleInitialized is initialized by start-up code.
 *  \trace        CREQ-119829
 *********************************************************************************************************************/
extern FUNC(void, NMOSEK_CODE) NmOsek_Init(P2CONST(NmOsek_ConfigType, AUTOMATIC, NMOSEK_INIT_DATA) nmOsekConfigPtr);

/**********************************************************************************************************************
 * NmOsek_PassiveStartUp()
 *********************************************************************************************************************/
/*! \brief        Passive startup of the OSEK NM.
 *  \details      It triggers the transition from Bus Sleep Mode (NMBusSleep) to the Network Mode (NMAwake) into Repeat
 *                Message State (NMReset). This service has no effect if the current state is not equal to Bus Sleep Mode.
 *                In that case E_NOT_OK is returned.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Passive startup of network management has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_PassiveStartUp(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);

# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  NmOsek_NetworkRequest()
 *********************************************************************************************************************/
/*! \brief        Request the network since ECU needs to communicate on the bus.
 *  \details      Triggers a transition
 *                - from Bus Sleep Mode (NMBusSleep) or Prepare Bus Sleep Mode (NMTwbsNormal) to Network Mode (NMAwake)
 *                  into Repeat Message State (NMReset).
 *                - from Prepare Bus Sleep (NMTwbsLimpHome) to Network Mode (NMLimpHome)
 *                - from Ready Sleep (NMNormalPrepSleep) / NMLimpHomePrepSleep) to Repeat Message / Normal Operation
 *                  (NMNormal)
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Requesting network has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \config       (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
 *  \pre          -
 *  \note         Called by NM Interface
 *  \trace        CREQ-119858
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_NetworkRequest(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);

/**********************************************************************************************************************
 *  NmOsek_NetworkRelease()
 *********************************************************************************************************************/
/*! \brief        Release the network since ECU doesn't have to communicate on the bus.
 *  \details      Trigger a transition to Bus Sleep mode in dependency of the other nodes on the bus
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Releasing network has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \config       (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
 *  \pre          -
 *  \note         Called by NM Interface
 *  \trace        CREQ-119845
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_NetworkRelease(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);
# endif

# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_DisableCommunication()
 *********************************************************************************************************************/
/*! \brief        Disable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service
 *  \details      NM PDU will not be sent until NmOsek_EnableCommunication() is called. Node will not be able to
 *                participate in the logical ring anymore. If a logical ring has been established, it needs to be
 *                re-established. The node cannot keep the network awake while the NM PDU transmission is disabled.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Disabling the NM PDU transmission ability has failed.
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \config       (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_DisableCommunication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);

/**********************************************************************************************************************
 *  NmOsek_EnableCommunication()
 *********************************************************************************************************************/
/*! \brief        Enable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service
 *  \details      NM PDU will be sent after this function call. The node can then re-enter a logical ring.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Enabling the NM PDU transmission ability has failed.
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *  \config       (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_EnableCommunication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);
# endif

# if (NMOSEK_NM_USER_DATA_ENABLED == STD_ON)
/* User Data service functions */
#  if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
#   if (NMOSEK_NM_COM_USER_DATA_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  NmOsek_SetUserData()
 *********************************************************************************************************************/
/*! \brief        Set user data for NM messages transmitted next on the bus
 *  \details      Update the TX message buffer by copying the provided bytes into the user data bytes of the NM PDU.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[in]    nmUserDataPtr           Pointer where the user data for the next transmitted NM message shall be copied
 *                                        from
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Setting user data has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_USER_DATA_ENABLED == STD_ON) && (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) &&
 *                (NMOSEK_NM_COM_USER_DATA_SUPPORT == STD_OFF)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_SetUserData(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                            CONSTP2CONST(uint8, AUTOMATIC, NMOSEK_APPL_VAR)
                                                            nmUserDataPtr);
#   endif
#  endif
/**********************************************************************************************************************
 *  NmOsek_GetUserData()
 *********************************************************************************************************************/
/*! \brief        Get user data out of the most recently received NM message
 *  \details      Copy the user data bytes out of the RX message buffer to the provided address.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[in]    nmUserDataPtr           Pointer where user data out of the most recently received NM message shall be
 *                                        copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Getting user data has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_USER_DATA_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetUserData(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                            CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR)
                                                            nmUserDataPtr);
# endif

# if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_Transmit()
 *********************************************************************************************************************/
/*! \brief        Empty function provided for PduR to prevent linker errors.
 *  \details      This empty function always returns E_OK.
 *  \param[in]    NmOsekTxPduId           Handle of the NmOsek user data PDU
 *  \param[in]    PduInfoPtr              Pointer to a structure containing the pointer to the NM user data buffer and
 *                                        the related DLC
 *  \return       E_OK - Transmit request has been accepted
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
 *  \pre          -
 *  \note         Called by PDU Router
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_Transmit(VAR(PduIdType, AUTOMATIC) NmOsekTxPduId,
                                                         P2CONST(PduInfoType, AUTOMATIC, NMOSEK_APPL_VAR) PduInfoPtr);
# endif

# if (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
/* Node Identifier */
/**********************************************************************************************************************
 *  NmOsek_GetNodeIdentifier()
 *********************************************************************************************************************/
/*! \brief        Get node identifier of the most recently received NM PDU
 *  \details      Copy the most recently received node identifier to the provided address
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmNodeIdPtr             Pointer where node identifier out of the most recently received NM PDU shall be
 *                                        copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Getting the node identifier out of the most recently received NM PDU has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                  CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR)
                                                                  nmNodeIdPtr);
/**********************************************************************************************************************
 *  NmOsek_GetLocalNodeIdentifier()
 *********************************************************************************************************************/
/*! \brief        Get node identifier configured for the local node
 *  \details      Copy the local node identifier to the provided address
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmNodeIdPtr             Pointer where node identifier of the local node shall be copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Getting the node identifier of the local node has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetLocalNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                       CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR)
                                                                       nmNodeIdPtr);
# endif

# if (NMOSEK_NM_NODE_DETECTION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_RepeatMessageRequest()
 *********************************************************************************************************************/
/*! \brief        Request state change to Repeat Message (NMNormal) State
 *  \details      Functionality is not supported and therefore this function always returns E_NOT_OK.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_NOT_OK - Transition to NMNormal Request (Repeat Message) has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_NODE_DETECTION_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_RepeatMessageRequest(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);
# endif

# if (NMOSEK_NM_NODE_ID_ENABLED == STD_ON) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_GetPduData()
 *********************************************************************************************************************/
/*! \brief        Get the whole PDU data out of the most recently received NM message
 *  \details      Copy the most recently received PDU data from the RX message buffer to the provided address.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmPduDataPtr            Pointer where NM PDU Data shall be copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Getting the NM PDU data has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_NODE_ID_ENABLED == STD_ON) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetPduData(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                           CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmPduDataPtr);
# endif

/**********************************************************************************************************************
 *  NmOsek_GetState()
 *********************************************************************************************************************/
/*! \brief        Return the state and the mode of the network management.
 *  \details      Return the state and the mode of the provided NM-channel to the provided addresses.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmStatePtr              Pointer where the state of the network management shall be copied to
 *  \param[out]   nmModePtr               Pointer where the mode of the network management shall be copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Getting the NM state has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetState(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                         CONSTP2VAR(Nm_StateType, AUTOMATIC, NMOSEK_APPL_VAR)
                                                         nmStatePtr, CONSTP2VAR(Nm_ModeType, AUTOMATIC, NMOSEK_APPL_VAR)
                                                         nmModePtr);

# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_CancelWaitForTxConfOrRxInd()
 *********************************************************************************************************************/
/*! \brief        Cancel the process of NmOsek to wait for a message confirmation or message reception
 *  \details      Call this function to let NmOsek no longer wait for one of these events to happen:
 *                - NM PDU reception
 *                - NM PDU confirmation
 *                If this function has been called, neither a NM PDU reception nor a NM PDU confirmation needs to
 *                happen to prevent state changes to NM_STATE_REPEAT_MESSAGE, NM_STATE_NORMAL_OPERATION or
 *                NM_STATE_READY_SLEEP.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - The flag to indicate that NmOsek shall no longer wait for a TxConfirmation or RxIndication
 *                           cannot be set in the current state or incorrect nmChannelHandle
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 *  \note         Called by Application
 *  \trace        CREQ-119851
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_CancelWaitForTxConfOrRxInd(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);
# endif

# if (NMOSEK_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief        This service returns the version information of this module.
 *  \details      Copy version information to the memory location the pointer versioninfo points to.
 *  \param[out]   versioninfo             Pointer to a variable where the version information of this module shall be
 *                                        stored
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_VERSION_INFO_API == STD_ON)
 *  \pre          -
 *  \note         Called by application
 *  \trace        CREQ-119830
 *********************************************************************************************************************/
extern FUNC(void, NMOSEK_CODE) NmOsek_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, NMOSEK_APPL_VAR)
                                                     versioninfo);
# endif

# if (NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_RequestBusSynchronization()
 *********************************************************************************************************************/
/*! \brief        Request bus synchronization.
 *  \details      Empty function implementation always returning E_NOT_OK.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \return       E_NOT_OK - Requesting bus synchronization has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_RequestBusSynchronization(CONST(NetworkHandleType, AUTOMATIC)
                                                                          nmChannelHandle);
# endif

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_CheckRemoteSleepIndication()
 *********************************************************************************************************************/
/*! \brief        Check whether remote sleep state is reached or not
 *  \details      Return the information whether all other nodes have indicated sleep indication bit set to 1 for at least
 *                two ring cycles
 *                (TRUE is written to nmRemoteSleepIndPtr) or not (FALSE is written to nmRemoteSleepIndPtr)
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmRemoteSleepIndPtr     Pointer where check result of remote sleep indication shall be copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Checking remote sleep indication has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
 *  \pre          -
 *  \note         Called by NM Interface
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_CheckRemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                           CONSTP2VAR(boolean, AUTOMATIC, NMOSEK_APPL_VAR)
                                                                           nmRemoteSleepIndPtr);
# endif

#if (NMOSEK_DEV_ERROR_DETECT == STD_ON) || (NMOSEK_USE_INIT_POINTER == STD_ON)
/***********************************************************************************************************************
 *  NmOsek_InitMemory()
 **********************************************************************************************************************/
/*! \brief        Function for *_INIT_*-variable initialization
 *  \details      Service to initialize module global variables at power up. This function initializes the
 *                variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_DEV_ERROR_DETECT == STD_ON) || (NMOSEK_USE_INIT_POINTER == STD_ON)
 *  \pre          Module is uninitialized.
 **********************************************************************************************************************/
extern FUNC(void, NMOSEK_CODE) NmOsek_InitMemory(void);
#endif

/**********************************************************************************************************************
 *  NmOsek_GetStatus()
 *********************************************************************************************************************/
/*! \brief        Return the global status byte of the network management.
 *  \details      Copy the global status byte to the provided address.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmStatusPtr             Pointer where the status byte of the network management shall be copied to
 *  \return       E_OK - No error
 *  \return       E_NOT_OK - Getting the NM state has failed
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 *  \note         Called by Application
 *  \trace        CREQ-119847
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetStatus(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                          CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmStatusPtr);

# if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_GetConfig()
 *********************************************************************************************************************/
/*! \brief        This API copies the current node configuration into an application specific buffer.
 *  \details      Copy all bytes of the node configuration to the provided address.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[out]   nmDataPtr               Pointer where the node configuration array is copied to
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_USE_NM_NODE_LIST == STD_ON)
 *  \pre          -
 *  \note         Called by Application
 *********************************************************************************************************************/
extern FUNC(void, NMOSEK_CODE) NmOsek_GetConfig(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                CONSTP2VAR(NmOsek_NodeConfigType, AUTOMATIC, NMOSEK_APPL_VAR)
                                                nmDataPtr);
/**********************************************************************************************************************
 *  NmOsek_CmpConfig()
 *********************************************************************************************************************/
/*! \brief        This API compares the current NM node configuration with a reference configuration stored in memory.
 *  \details      Compare the current NM node configuration with an existing one using ConfigMaskTable and
 *                TargetConfigTable.
 *  \param[in]    nmChannelHandle         Identification of the NM-channel
 *  \param[in]    idx                     Index of ConfigMaskTable and TargetConfigTable
 *  \return       TRUE  Configuration matches the reference
 *  \return       FALSE Configuration does not match the reference
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       (NMOSEK_USE_NM_NODE_LIST == STD_ON)
 *  \pre          -
 *  \note         Called by Application
 *********************************************************************************************************************/
extern FUNC(boolean, NMOSEK_CODE) NmOsek_CmpConfig(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                   CONST(uint8, AUTOMATIC) idx);
# endif

# define NMOSEK_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*-- Application Callback Function Prototypes -----------------------------------------------------------------------*/
# define NMOSEK_START_SEC_APPL_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define NMOSEK_STOP_SEC_APPL_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 **********************************************************************************************************************/
/*!
 * \exclusivearea NMOSEK_EXCLUSIVE_AREA_0
 * Ensures data consistency for status variables.
 * \protects NmOsek_SAMsgReqState, NmOsek_InternalState, NmOsek_PrepSendSleepAck, NmOsek_TxRequest, NmOsek_RxTxCnt,
 *           NmOsek_TxMessageData, NmOsek_DesiredNmState, NmOsek_NmState, NmOsek_RingCycleTimer, NmOsek_SyncPointTimer,
 *           NmOsek_RemoteSleepState, NmOsek_SAMsgReqTimer, NmOsek_BusOffCounter, NmOsek_FirstTxConfOrRxInd,
 *           NmOsek_MsgTimeoutTimer, NmOsek_Action, NmOsek_TimeoutTimer, NmOsek_PeriodicMsgTimer,
 *           NmOsek_PreviousNmState, NmOsek_RxNmMsgSourceAddr, NmOsek_NodeConfiguration, NmOsek_LogicalSuccessor,
 *           NmOsek_RxMessageData, NmOsek_AnyChannelInLimpHome, NmOsek_SendWakeUpFrame
 * \usedin NmOsek_MainFunction_IssueTxRequestIfDemandedAndPossible(), NmOsek_NetworkRequest(),
 *         NmOsek_SkippedAliveMessageRequest(), NmOsek_TxConfirmation(), NmOsek_RxIndication(), NmOsek_MainFunction(),
 *         NmOsek_GetConfig(), NmOsek_CmpConfig(), NmOsek_SendSleepAck(), NmOsek_ClearLimpHomeStatus()
 * \exclude -
 * \length LONG There are function calls to other modules within the exclusive area.
 * \endexclusivearea
 *
 * \exclusivearea NMOSEK_EXCLUSIVE_AREA_1
 * Ensures data consistency for the internal state variable.
 * \protects NmOsek_InternalState
 * \usedin NmOsek_NetworkRelease(), NmOsek_DisableCommunication(), NmOsek_EnableCommunication()
 * \exclude -
 * \length SHORT There is only a conditional variable assignment within the Exclusive Area.
 * \endexclusivearea
 *
 * \exclusivearea NMOSEK_EXCLUSIVE_AREA_2
 * Ensures data consistency for a Passive Start-up.
 * \protects NmOsek_InternalState, PrepSendSleepAck, TxMessageData
 * \usedin NmOsek_PassiveStartUp()
 * \exclude -
 * \length SHORT There are only a few conditional variable assignments within the Exclusive Area.
 * \endexclusivearea
 *
 * \exclusivearea NMOSEK_EXCLUSIVE_AREA_3
 * Ensures data consistency for Setting User Data Bytes.
 * \protects TxMessageData
 * \usedin NmOsek_SetUserData()
 * \exclude -
 * \length SHORT There are only a few bytes copied within the Exclusive Area.
 * \endexclusivearea
 *
 * \exclusivearea NMOSEK_EXCLUSIVE_AREA_4
 * Ensures a consistent copy of User Data Bytes.
 * \protects -
 * \usedin NmOsek_GetUserData()
 * \exclude -
 * \length SHORT There are only a few bytes copied within the Exclusive Area.
 * \endexclusivearea
 *
 * \exclusivearea NMOSEK_EXCLUSIVE_AREA_5
 * Ensures a consistent copy of PDU Data Bytes.
 * \protects -
 * \usedin NmOsek_GetPduData()
 * \exclude -
 * \length SHORT There are only a few bytes copied within the Exclusive Area.
 * \endexclusivearea
 */

#endif /* NMOSEK_H */

/**********************************************************************************************************************
 *  END OF FILE: NmOsek.h
 *********************************************************************************************************************/
